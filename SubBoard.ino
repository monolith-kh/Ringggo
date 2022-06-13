#include <SPI.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#define VSPI_MOSI   25
#define VSPI_MISO   26
#define VSPI_SCLK   27
#define VSPI_SS     14

#define VSPI_CLK     1000000


#define SPI_CMD_EVENT           0x10
#define SPI_CMD_STATUS          0x20
#define SPI_CMD_REMOTE_SET      0x30
#define SPI_CMD_LOCATION_GET    0x40

#define SPI_CMD_WRITE           0x00
#define SPI_CMD_READ            0x01

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

WiFiUDP rtlsClient, ntpUDP;
NTPClient timeClient(ntpUDP);

void SubBoardInit()
{
  Serial.println("Init SubBoard");
  vspi = new SPIClass(VSPI);
  vspi->begin(VSPI_SCLK, VSPI_MISO, VSPI_MOSI, VSPI_SS);
  pinMode(VSPI_SS, OUTPUT);
}

void SubBoardTask(void* parameter)
{
  SubBoardInit();
  
  uint8_t spiCmd = 0x00;
  uint8_t spiLength = 0;
  uint8_t spiResult[7] = {0, };
  uint8_t spiResultLength = 0;

  char sBuffer[32] = {0, };

  spiCmd = SPI_CMD_LOCATION_GET | SPI_CMD_READ;
  spiLength = 0;
  spiResultLength = 7;

  for (;;)
  {
    // Serial.println("SubBoard Task");

    vspi->beginTransaction(SPISettings(VSPI_CLK, MSBFIRST, SPI_MODE0));
    digitalWrite(VSPI_SS, LOW); //pull SS slow to prep other end for transfer
    vspi->transfer(spiCmd);
    vspi->transfer(spiLength);
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
    vspi->transferBytes(spiResult, spiResult, spiResultLength);
    digitalWrite(VSPI_SS, HIGH); //pull ss high to signify end of data transfer
    vspi->endTransaction();
    
    Serial.printf("Send: 0x%x\n", spiCmd);
    sprintf(sBuffer, "Receive: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x", spiResult[0], spiResult[1], spiResult[2], spiResult[3], spiResult[4], spiResult[5], spiResult[6]);
    Serial.println(sBuffer);

    rtlsClient.beginPacket(RTLS_HOST, RTLS_PORT);
    Protocol_position_t protocol = { PK_POSITION_NOTI, CAR, 22, CAR_ID, 0, };
    protocol.timestamp = (int64_t)0x80808080FFFFFFFF;
    protocol.positionX = (int16_t)(spiResult[2] << 8 | spiResult[1]);
    protocol.positionY = (int16_t)(spiResult[4] << 8 | spiResult[3]);
    protocol.headAngle = (int16_t)(spiResult[6] << 8 | spiResult[5]);
    rtlsClient.write((uint8_t *)&protocol, sizeof(protocol));
    rtlsClient.endPacket();

    vTaskDelay(50);
  }
  vTaskDelete(NULL);
}
