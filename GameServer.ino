#define GAME_HOST   "manunited.ringggo.981park.net"
// #define GAME_HOST   "192.168.41.254"
#define GAME_PORT   9998

#define START     0
#define PAUSE     1
#define UNPAUSE   2
#define STOP      3
#define RETURN    4

#include <WiFi.h>

WiFiClient gameClient;


void GameServerInit()
{
  Serial.println("Init GameServer");
  if (!gameClient.connect(GAME_HOST, GAME_PORT))
  {
    Serial.println("connection failed");
    return;
  }
  Serial.println("connected GameServer");

}

void GameServerTask(void* parameter)
{
  BaseType_t xStatus;

  uint8_t packetHeader[8] = { 0, };
  uint8_t packetBody[8] = { 0, };

  GameServerInit();
  
  for (;;)
  {
    // Serial.println("GameServer Task");

    while (gameClient.available())
    {
      gameClient.readBytes(packetHeader, 8);
      Serial.printf("%x %x %x %x %x %x %x %x\n ", packetHeader[0], packetHeader[1], packetHeader[2], packetHeader[3], packetHeader[4], packetHeader[5], packetHeader[6], packetHeader[7]);
      
      if (packetHeader[0] == PK_WHO_ARE_YOU_ANS) {
        Serial.println("whoyouare received");
        Protocol_t protocol = {PK_IAM_ANS, CAR, 8, CAR_ID, 0, };
        gameClient.write((const uint8_t *)&protocol, sizeof(protocol));
        Serial.println("send answer");
        Mp3Play(6);
        sensor.clearFields();
        sensor.addField("connection", 1);
        if(!idbClient.writePoint(sensor)) {
          Serial.print("InfluxDB write failed: ");
          Serial.println(idbClient.getLastErrorMessage());
        }
      } else if (packetHeader[0] == PK_CHECK_CONNECTION_REQ) {
        Serial.println("check received");
        Protocol_t protocol = {PK_CHECK_CONNECTION_ANS, CAR, 8, CAR_ID, 0, };
        gameClient.write((const uint8_t *)&protocol, sizeof(protocol));
        Serial.println("send answer");
        sensor.clearFields();
        sensor.addField("healthcheck", 1);
        sensor.addField("rssi", WiFi.RSSI());
        if(!idbClient.writePoint(sensor)) {
          Serial.print("InfluxDB write failed: ");
          Serial.println(idbClient.getLastErrorMessage());
        }

      } else if (packetHeader[0] == PK_CARLED_NOTI) {
        gameClient.readBytes(packetBody, 4);
        Serial.printf("car led received: %d, %d, %d, %d\n", packetBody[0], packetBody[1], packetBody[2], packetBody[3]);
        xStatus = xQueueSendToFront(xQueueLed, packetBody, 100);
        if(xStatus == pdPASS)
        {
            Serial.printf("led queue send\n");
        }
      } else if (packetHeader[0] == PK_CARSOUND_NOTI) {
        gameClient.readBytes(packetBody, 2);
        Serial.printf("car sound received: %d, %d\n", packetBody[0], packetBody[1]);
        xStatus = xQueueSendToFront(xQueueMp3, packetBody, 100);
        if(xStatus == pdPASS)
        {
            Serial.printf("mp3 queue send\n");
        }
      } else if (packetHeader[0] == PK_CARSTARTSTOP_NOTI) {
        gameClient.readBytes(packetBody, 1);
        Serial.printf("car start/stop received: %d\n", packetBody[0]);
        // TODO xQueueSubBoard
      } else {
        Serial.println("invalid req code");
      }
    }
    vTaskDelay(1000);

  }
  vTaskDelete(NULL);
}

void SendBattery(uint8_t percentage)
{
    Protocol_bt protocol = {PK_BATTERY_NOTI, CAR, 8, CAR_ID, 0, percentage};
    gameClient.write((const uint8_t *)&protocol, sizeof(protocol));
    Serial.println("send battery");
}

void SendNfc(char wrbId[], uint8_t uid[])
{
    Protocol_nfct protocol = {PK_NFC_NOTI, CAR, 8, CAR_ID, 0,};
    memcpy(protocol.wrbId, wrbId, 13);
    protocol.sep = ',';
    memcpy(protocol.uid, uid, 7);
    gameClient.write((const uint8_t *)&protocol, sizeof(protocol));
    Serial.println("send nfc");
}

void SendBumper(int bumper)
{
    Protocol_bumpert protocol = {PK_BUMP_NOTI, CAR, 8, CAR_ID, 0, (uint8_t)bumper};
    gameClient.write((const uint8_t *)&protocol, sizeof(protocol));
    Serial.println("send bumper");
}

void GameServerSendTask(void* parameter)
{
  BaseType_t xStatus;
  uint8_t data = 0;
  
  for (;;)
  {
    // Serial.println("GameServer Send Task");
    xStatus = xQueueReceive(xQueueBumper, &data, 100);
    if(xStatus == pdPASS)
    {
      SendBumper(data);
      Serial.printf("receive queue: 0x%x\n", data);
      sensor.clearFields();
      sensor.addField("bumper", data);
      if(!idbClient.writePoint(sensor)) {
        Serial.print("InfluxDB write failed: ");
        Serial.println(idbClient.getLastErrorMessage());
      }
    }
    xStatus = xQueueReceive(xQueueBattery, &data, 100);
    if(xStatus == pdPASS)
    {
      SendBattery(data);
      Serial.printf("receive queue: %d%\n", data);
      sensor.clearFields();
      sensor.addField("battery", data);
      if(!idbClient.writePoint(sensor)) {
        Serial.print("InfluxDB write failed: ");
        Serial.println(idbClient.getLastErrorMessage());
      }
    }

    vTaskDelay(100);
  }
  vTaskDelete(NULL);
}