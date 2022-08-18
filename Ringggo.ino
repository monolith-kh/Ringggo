#define FIRM_VER    "0.1.15"

#define BAUDRATE    115200

#define CAR_ID      10

#include "Packet.h"

#include <WiFi.h>
#include <SPI.h>

TaskHandle_t* wifiTaskHandler;

//#include <ArduinoOTA.h>

#include <Adafruit_PN532.h>
#define PN532_SS    (5)
#define PN532_MOSI  (23)
#define PN532_MISO  (19)
#define PN532_SCK   (18)

Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

xQueueHandle xQueueBumper, xQueueLed, xQueueMp3, xQueueRtls, xQueueBattery, xQueueNfc, xQueueSubBoard;

char hostName[32] = { 0, };


void setup() {
  // put your setup code here, to run once:
  Serial.begin((unsigned long)BAUDRATE);
  vTaskDelay(10);
  log_i("Hello Ringggo");

  sprintf(hostName, "CAR_%d(%s)", CAR_ID, FIRM_VER);
  log_i("hostname: %s", hostName);

  xQueueBumper = xQueueCreate(10, sizeof(uint8_t));
  xQueueBattery = xQueueCreate(5, sizeof(uint8_t));
  xQueueNfc = xQueueCreate(5, sizeof(uint8_t));
  xQueueLed = xQueueCreate(5, sizeof(uint8_t[7]));
  xQueueMp3 = xQueueCreate(5, sizeof(uint8_t[2]));
  xQueueRtls = xQueueCreate(10, sizeof(uint8_t[2]));
  xQueueSubBoard = xQueueCreate(10, sizeof(uint8_t));

  WifiInit();

  vTaskDelay(1000);

  nfc.begin();

  //  NtpInit();

  // log_i("Start RtlsServer Task");
  // xTaskCreate(
  //   RtlsServerTask,         // Task 함수 이름
  //   "RtlsServerTask",       // Task 이름
  //   10000,                  // Task 스택 크기
  //   NULL,                   // Task 파라미터
  //   14,                      // Task 우선순위
  //   NULL);                  // Task handle

  // log_i("Start Bumper Task");
  // xTaskCreate(
  //   BumperTask,            // Task 함수 이름
  //   "BumperTask",              // Task 이름
  //   10000,                  // Task 스택 크기
  //   NULL,                   // Task 파라미터
  //   12,                      // Task 우선순위
  //   NULL);                  // Task handle

  log_i("Start GameServer Task");
  xTaskCreate(
    GameServerTask,         // Task 함수 이름
    "GameServerTask",       // Task 이름
    10000,                  // Task 스택 크기
    NULL,                   // Task 파라미터
    9,                      // Task 우선순위
    NULL);                  // Task handle

  log_i("Start GameServer Send Task");
  xTaskCreate(
    GameServerSendTask,         // Task 함수 이름
    "GameServerSendTask",       // Task 이름
    10000,                  // Task 스택 크기
    NULL,                   // Task 파라미터
    9,                      // Task 우선순위
    NULL);                  // Task handle

  // log_i("Start Wifi Task");
  // xTaskCreate(
  //   WifiTask,                // Task 함수 이름
  //   "WifiTask",              // Task 이름
  //   10000,                  // Task 스택 크기
  //   NULL,                   // Task 파라미터
  //   7,                      // Task 우선순위
  //   wifiTaskHandler);        // Task handle

  log_i("Start Led Task");
  xTaskCreate(
    LedTask,                // Task 함수 이름
    "LedTask",              // Task 이름
    10000,                  // Task 스택 크기
    NULL,                   // Task 파라미터
    6,                      // Task 우선순위
    NULL);                  // Task handle

  log_i("Start Battery Task");
  xTaskCreate(
    BatteryTask,            // Task 함수 이름
    "BatteryTask",              // Task 이름
    1000,                  // Task 스택 크기
    NULL,                   // Task 파라미터
    5,                      // Task 우선순위
    NULL);                  // Task handle

  // Serial.println("Start Ntp Task");
  // xTaskCreate(
  //   NtpTask,                // Task 함수 이름
  //   "NtpTask",              // Task 이름
  //   1000,                  // Task 스택 크기
  //   NULL,                   // Task 파라미터
  //   4,                      // Task 우선순위
  //   NULL);                  // Task handle

  // Serial.println("Start Monitor Task");
  // xTaskCreate(
  //   MonitorTask,            // Task 함수 이름
  //   "MonitorTask",          // Task 이름
  //   10000,                  // Task 스택 크기
  //   NULL,                   // Task 파라미터
  //   3,                     // Task 우선순위
  //   NULL);                  // Task handle

  log_i("Start Mp3 Task");
  xTaskCreate(
    Mp3Task,                // Task 함수 이름
    "Mp3Task",              // Task 이름
    1000,                  // Task 스택 크기
    NULL,                   // Task 파라미터
    2,                      // Task 우선순위
    NULL);                  // Task handle

  log_i("Start NFC Task");
  xTaskCreate(
    NfcTask,                // Task 함수 이름
    "NfcTask",              // Task 이름
    10000,                  // Task 스택 크기
    NULL,                   // Task 파라미터
    1,                     // Task 우선순위
    NULL);                  // Task handle

  log_i("Start SubBoard Task");
  xTaskCreate(
    SubBoardTask,                // Task 함수 이름
    "SubBoardTask",              // Task 이름
    10000,                  // Task 스택 크기
    NULL,                   // Task 파라미터
    1,                      // Task 우선순위
    NULL);                  // Task handle

  // Port defaults to 3232
  //  ArduinoOTA.setPort(3232);
  //  ArduinoOTA.setHostname(hostName);
  //  ArduinoOTA.setPassword(FIRM_VER);
  //
  //  ArduinoOTA
  //  .onStart([]() {
  //    String type;
  //    if (ArduinoOTA.getCommand() == U_FLASH)
  //      type = "sketch";
  //    else // U_SPIFFS
  //      type = "filesystem";
  //
  //    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
  //    Serial.println("Start updating " + type);
  //  })
  //  .onEnd([]() {
  //    Serial.println("\nEnd");
  //  })
  //  .onProgress([](unsigned int progress, unsigned int total) {
  //    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  //  })
  //  .onError([](ota_error_t error) {
  //    Serial.printf("Error[%u]: ", error);
  //    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
  //    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
  //    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
  //    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
  //    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  //  });
  //
  //  ArduinoOTA.begin();

  Serial.println(heap_caps_check_integrity_all(true));
}


void loop() {
  // put your main code here, to run repeatedly:
  //  ArduinoOTA.handle();
  vTaskDelay(1);
}
