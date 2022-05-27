#define CAR_ID      13


#define BAUDRATE    115200

#include "Packet.h"


TaskHandle_t* wifiTaskHandler;

#include <Adafruit_PN532.h>
#define PN532_SS    (5)
#define PN532_MOSI  (23)
#define PN532_MISO  (19)
#define PN532_SCK   (18)

Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

#define INFLUXDB_URL       "http://iwinv.jaehoonj.981park.net:8086"
#define INFLUXDB_DB_NAME   "esp32"

#include <InfluxDbClient.h>
InfluxDBClient idbClient(INFLUXDB_URL, INFLUXDB_DB_NAME);
Point sensor("ringggo");

xQueueHandle xQueueBumper, xQueueLed, xQueueMp3, xQueueRtls, xQueueBattery, xQueueNfc;


void setup() {
  // put your setup code here, to run once:
  Serial.begin((unsigned long)BAUDRATE);
  delay(100);
  Serial.println("Hello Ringggo");

  xQueueBumper = xQueueCreate(10, sizeof(uint8_t));
  xQueueBattery = xQueueCreate(5, sizeof(uint8_t));
  xQueueNfc = xQueueCreate(5, sizeof(uint8_t));
  xQueueLed = xQueueCreate(5, sizeof(uint8_t[4]));
  xQueueMp3 = xQueueCreate(5, sizeof(uint8_t[2]));
  xQueueRtls = xQueueCreate(10, sizeof(uint8_t[2]));

  WifiInit();

  delay(1000);

  nfc.begin();

  sensor.addTag("car_id", (String)CAR_ID);
  sensor.addTag("ssid", WiFi.SSID());
  
  sensor.clearFields();
  sensor.addField("start", 1);
  sensor.addField("rssi", WiFi.RSSI());
  if(!idbClient.writePoint(sensor)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(idbClient.getLastErrorMessage());
  }


  // Serial.println("Start RtlsServer Task");
  // xTaskCreate(
  //   RtlsServerTask,         // Task 함수 이름
  //   "RtlsServerTask",       // Task 이름
  //   10000,                  // Task 스택 크기
  //   NULL,                   // Task 파라미터
  //   14,                      // Task 우선순위
  //   NULL);                  // Task handle

  Serial.println("Start Bumper Task");
  xTaskCreate(
    BumperTask,            // Task 함수 이름
    "BumperTask",              // Task 이름
    10000,                  // Task 스택 크기
    NULL,                   // Task 파라미터
    12,                      // Task 우선순위
    NULL);                  // Task handle

  Serial.println("Start GameServer Task");
  xTaskCreate(
    GameServerTask,         // Task 함수 이름
    "GameServerTask",       // Task 이름
    10000,                  // Task 스택 크기
    NULL,                   // Task 파라미터
    9,                      // Task 우선순위
    NULL);                  // Task handle

  Serial.println("Start GameServer Send Task");
  xTaskCreate(
    GameServerSendTask,         // Task 함수 이름
    "GameServerSendTask",       // Task 이름
    10000,                  // Task 스택 크기
    NULL,                   // Task 파라미터
    9,                      // Task 우선순위
    NULL);                  // Task handle

  // Serial.println("Start Wifi Task");
  // xTaskCreate(
  //   WifiTask,                // Task 함수 이름
  //   "WifiTask",              // Task 이름
  //   10000,                  // Task 스택 크기
  //   NULL,                   // Task 파라미터
  //   7,                      // Task 우선순위
  //   wifiTaskHandler);        // Task handle

  Serial.println("Start Led Task");
  xTaskCreate(
    LedTask,                // Task 함수 이름
    "LedTask",              // Task 이름
    10000,                  // Task 스택 크기
    NULL,                   // Task 파라미터
    6,                      // Task 우선순위
    NULL);                  // Task handle

  Serial.println("Start Battery Task");
  xTaskCreate(
    BatteryTask,            // Task 함수 이름
    "BatteryTask",              // Task 이름
    1000,                  // Task 스택 크기
    NULL,                   // Task 파라미터
    5,                      // Task 우선순위
    NULL);                  // Task handle

  Serial.println("Start Mp3 Task");
  xTaskCreate(
    Mp3Task,                // Task 함수 이름
    "Mp3Task",              // Task 이름
    1000,                  // Task 스택 크기
    NULL,                   // Task 파라미터
    2,                      // Task 우선순위
    NULL);                  // Task handle

  Serial.println("Start NFC Task");
  xTaskCreate(
    NfcTask,                // Task 함수 이름
    "NfcTask",              // Task 이름
    10000,                  // Task 스택 크기
    NULL,                   // Task 파라미터
    1,                     // Task 우선순위
    NULL);                  // Task handle

  Serial.println("Start SubBoard Task");
  xTaskCreate(
    SubBoardTask,                // Task 함수 이름
    "SubBoardTask",              // Task 이름
    10000,                  // Task 스택 크기
    NULL,                   // Task 파라미터
    1,                      // Task 우선순위
    NULL);                  // Task handle

  Serial.println(xPortGetFreeHeapSize());
  Serial.println(heap_caps_check_integrity_all(true));
}


void loop() {
  // put your main code here, to run repeatedly:
}
