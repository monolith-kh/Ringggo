#define GAME_HOST   "manunited.ringggo.981park.net"
// #define GAME_HOST   "192.168.41.254"
#define GAME_PORT   9998

#define STOP      0
#define START     1
#define SUSPEND   2
#define RESUME    3


WiFiClient gameClient;


void GameServerInit()
{
  log_i("Init GameServer");
//  gameClient.setNoDelay(true);
  if (!gameClient.connect(GAME_HOST, GAME_PORT))
  {
    log_i("connection failed");
    return;
  }
  log_i("connected GameServer");
  gameClient.setNoDelay(true);
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
      log_i("%x %x %x %x %x %x %x %x ", packetHeader[0], packetHeader[1], packetHeader[2], packetHeader[3], packetHeader[4], packetHeader[5], packetHeader[6], packetHeader[7]);
      
      if (packetHeader[0] == PK_WHO_ARE_YOU_ANS) {
        log_i("whoyouare received");
        Protocol_t protocol = {PK_IAM_ANS, CAR, 8, CAR_ID, 0, };
        gameClient.write((const uint8_t *)&protocol, sizeof(protocol));
        log_i("send answer");
      } else if (packetHeader[0] == PK_CHECK_CONNECTION_REQ) {
        log_i("check received");
        Protocol_t protocol = {PK_CHECK_CONNECTION_ANS, CAR, 8, CAR_ID, 0, };
        gameClient.write((const uint8_t *)&protocol, sizeof(protocol));
        log_i("send answer");
      } else if (packetHeader[0] == PK_CARLED_NOTI) {
        gameClient.readBytes(packetBody, 7);
        log_i("car led received: %d, %d, %d, %d, %d, %d, %d", packetBody[0], packetBody[1], packetBody[2], packetBody[3], packetBody[4], packetBody[5], packetBody[6]);
        xStatus = xQueueSendToFront(xQueueLed, packetBody, 100);
        if(xStatus == pdPASS)
        {
            log_i("led queue send");
        }
      } else if (packetHeader[0] == PK_CARSOUND_NOTI) {
        gameClient.readBytes(packetBody, 2);
        log_i("car sound received: %d, %d", packetBody[0], packetBody[1]);
        xStatus = xQueueSendToFront(xQueueMp3, packetBody, 100);
        if(xStatus == pdPASS)
        {
            log_i("mp3 queue send");
        }
      } else if (packetHeader[0] == PK_CARACTIVEMODE_NOTI) {
        gameClient.readBytes(packetBody, 1);
        log_i("car start/stop received: %d\n", packetBody[0]);
        xStatus = xQueueSendToFront(xQueueSubBoard, packetBody, 100);
        if(xStatus == pdPASS)
        {
            log_i("subboard queue send");
        }
      } else {
        log_i("invalid req code");
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
    log_i("send battery");
}

void SendNfc(char wrbId[], uint8_t uid[])
{
    Protocol_nfct protocol = {PK_NFC_NOTI, CAR, 8, CAR_ID, 0,};
    memcpy(protocol.wrbId, wrbId, 13);
    protocol.sep = ',';
    memcpy(protocol.uid, uid, 7);
    gameClient.write((const uint8_t *)&protocol, sizeof(protocol));
    log_i("send nfc");
}

void SendBumper(int bumper)
{
    Protocol_bumpert protocol = {PK_BUMP_NOTI, CAR, 8, CAR_ID, 0, (uint8_t)bumper};
    gameClient.write((const uint8_t *)&protocol, sizeof(protocol));
    log_i("send bumper");
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
      log_i("receive queue: 0x%x", data);
    }
    xStatus = xQueueReceive(xQueueBattery, &data, 100);
    if(xStatus == pdPASS)
    {
      SendBattery(data);
      log_i("receive queue: %d%", data);
    }

    vTaskDelay(100);
  }
  vTaskDelete(NULL);
}
