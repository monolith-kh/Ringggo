//#include <SPI.h>

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <time.h>

#define VSPI_MOSI   25
#define VSPI_MISO   26
#define VSPI_SCLK   27
#define VSPI_SS     14

#define VSPI_CLK     1000000


#define SPI_CMD_EVENT           0x11
#define SPI_CMD_STATUS          0x21
#define SPI_CMD_REMOTE_SET      0x31
#define SPI_CMD_LOCATION_GET    0x41
#define SPI_CMD_CARNUM_GET      0x51

#define EVENT_REMOTE_ON         0x00
#define EVENT_REMOTE_OFF        0x01
#define EVENT_MANUAL_ON         0x02
#define EVENT_MANUAL_OFF        0x03
#define EVENT_NPC_ON            0x04
#define EVENT_NPC_OFF           0x05
#define EVENT_RESUME            0x06
#define EVENT_SUSPEND           0x07

#define STATUS_READY            0x00
#define STATUS_MANUAL           0x01
#define STATUS_REMOTE           0x02
#define STATUS_PAUSE            0x03
#define STATUS_NPC              0x04

#define RTLS_HOST   "oldtrafford.ringggo.981park.net"
// #define RTLS_HOST   "192.168.41.254"
#define RTLS_PORT   9999

#define POSITION_X_MAX  1500
#define POSITION_Y_MAX  2000
#define HEADANGLE_MAX  360


SPIClass* vspi = NULL;


time_t t;

WiFiUDP rtlsClient, ntpUDP;

// By default 'pool.ntp.org' is used with 60 seconds update interval and
// no offset
NTPClient timeClient(ntpUDP);


void SubBoardInit()
{
  Serial.println("Init SubBoard [start]");
  vspi = new SPIClass(VSPI);
  Serial.println("Init SubBoard [start] 1");
  vspi->begin(VSPI_SCLK, VSPI_MISO, VSPI_MOSI, VSPI_SS);
  Serial.println("Init SubBoard [start] 2");
  pinMode(VSPI_SS, OUTPUT);
  Serial.println("Init SubBoard [end]");
}

void SubBoardTask(void* parameter)
{
//  SubBoardInit();

  log_i("SubBoard Task");

  vspi = new SPIClass(VSPI);
  
  //clock miso mosi ss

  vspi->begin(VSPI_SCLK, VSPI_MISO, VSPI_MOSI, VSPI_SS);
  pinMode(VSPI_SS, OUTPUT);

  
  BaseType_t xStatus;
  uint8_t data = 0;
  
//  uint8_t spiCmd = 0x00;
//  uint8_t spiLength = 0;
  uint8_t spiResult[7] = {0, };
//  uint8_t spiResultLength = 0;

  char sBuffer[32] = {0, };

//  spiCmd = SPI_CMD_LOCATION_GET;
//  spiLength = 0;
//  spiResultLength = 7;

//  timeClient.begin();
//  timeClient.update();


  for (;;)
  {
    // Serial.println("SubBoard Task");

     xStatus = xQueueReceive(xQueueSubBoard, &data, 100);
     if(xStatus == pdPASS)
     {
       Serial.printf("receive queue: 0x%x\n", data);

       vspi->beginTransaction(SPISettings(VSPI_CLK, MSBFIRST, SPI_MODE0));
       digitalWrite(VSPI_SS, LOW); //pull SS slow to prep other end for transfer
       vspi->transfer(SPI_CMD_EVENT);
       vspi->transfer(1);
       if(data == STOP)
       {
         vspi->transfer(EVENT_MANUAL_OFF);
       }
       else if(data == START)
       {
         vspi->transfer(EVENT_MANUAL_ON);
       }else
       {
         Serial.println("wrong data");
       }
       digitalWrite(VSPI_SS, HIGH); //pull ss high to signify end of data transfer
       vTaskDelay(1000);

       spiResult[0] = 0;
       spiResult[1] = 0;
       spiResult[2] = 0;
      
       digitalWrite(VSPI_SS, LOW); //pull SS slow to prep other end for transfer
       vspi->transferBytes(spiResult, spiResult, 3);
       digitalWrite(VSPI_SS, HIGH); //pull ss high to signify end of data transfer
       vspi->endTransaction();

       Serial.printf("Send: 0x%x\n", SPI_CMD_EVENT);
       sprintf(sBuffer, "Receive: 0x%02x 0x%02x 0x%02x", spiResult[0], spiResult[1], spiResult[2]);
       Serial.println(sBuffer);
       vTaskDelay(1000);


       vspi->beginTransaction(SPISettings(VSPI_CLK, MSBFIRST, SPI_MODE0));
       digitalWrite(VSPI_SS, LOW); //pull SS slow to prep other end for transfer
       vspi->transfer(SPI_CMD_STATUS);
       vspi->transfer(0);
       digitalWrite(VSPI_SS, HIGH); //pull ss high to signify end of data transfer
       vTaskDelay(1000);

       spiResult[0] = 0;
       spiResult[1] = 0;
       spiResult[2] = 0;
      
       digitalWrite(VSPI_SS, LOW); //pull SS slow to prep other end for transfer
       vspi->transferBytes(spiResult, spiResult, 3);
       digitalWrite(VSPI_SS, HIGH); //pull ss high to signify end of data transfer
       vspi->endTransaction();

       Serial.printf("Send: 0x%x\n", SPI_CMD_STATUS);
       sprintf(sBuffer, "Receive: 0x%02x 0x%02x 0x%02x", spiResult[0], spiResult[1], spiResult[2]);
       Serial.println(sBuffer);
       vTaskDelay(1000);

       vspi->beginTransaction(SPISettings(VSPI_CLK, MSBFIRST, SPI_MODE0));
       digitalWrite(VSPI_SS, LOW); //pull SS slow to prep other end for transfer
       vspi->transfer(SPI_CMD_CARNUM_GET);
       vspi->transfer(0);
       digitalWrite(VSPI_SS, HIGH); //pull ss high to signify end of data transfer
       vTaskDelay(1000);

       spiResult[0] = 0;
       spiResult[1] = 0;
      
       digitalWrite(VSPI_SS, LOW); //pull SS slow to prep other end for transfer
       vspi->transferBytes(spiResult, spiResult, 2);
       digitalWrite(VSPI_SS, HIGH); //pull ss high to signify end of data transfer
       vspi->endTransaction();

       Serial.printf("Send: 0x%x\n", SPI_CMD_CARNUM_GET);
       sprintf(sBuffer, "Receive: 0x%02x 0x%02x", spiResult[0], spiResult[1]);
       Serial.println(sBuffer);
       vTaskDelay(1000);

     }

   vspi->beginTransaction(SPISettings(VSPI_CLK, MSBFIRST, SPI_MODE0));
   digitalWrite(VSPI_SS, LOW); //pull SS slow to prep other end for transfer
   vspi->transfer(SPI_CMD_LOCATION_GET);
   vspi->transfer(0);
   digitalWrite(VSPI_SS, HIGH); //pull ss high to signify end of data transfer
   vTaskDelay(50);

   spiResult[0] = 0;
   spiResult[1] = 0;
   spiResult[2] = 0;
   spiResult[3] = 0;
   spiResult[4] = 0;
   spiResult[5] = 0;
   spiResult[6] = 0;
   
   digitalWrite(VSPI_SS, LOW); //pull SS slow to prep other end for transfer
   vspi->transferBytes(spiResult, spiResult, 7);
   digitalWrite(VSPI_SS, HIGH); //pull ss high to signify end of data transfer
   vspi->endTransaction();

   log_d("Wifi status: %d", WiFi.status());
   log_d("Send: 0x%x", SPI_CMD_LOCATION_GET);
   log_d("Receive: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x", spiResult[0], spiResult[1], spiResult[2], spiResult[3], spiResult[4], spiResult[5], spiResult[6]);

   rtlsClient.beginPacket(RTLS_HOST, RTLS_PORT);
   Protocol_position_t protocol = { PK_POSITION_NOTI, CAR, 22, CAR_ID, 0, };
   t = time(NULL);
   protocol.timestamp = (int64_t)t;
//    protocol.timestamp = (int64_t)0x80808080FFFFFFFF;
   protocol.positionX = (int16_t)(spiResult[2] << 8 | spiResult[1]);
   protocol.positionY = (int16_t)(spiResult[4] << 8 | spiResult[3]);
   protocol.headAngle = (int16_t)(spiResult[6] << 8 | spiResult[5]);
   rtlsClient.write((uint8_t *)&protocol, sizeof(protocol));
   rtlsClient.endPacket();

   vTaskDelay(50);
  }
  vTaskDelete(NULL);
}
