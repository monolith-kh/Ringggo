#define CAR_ID          8
#define BAUDRATE    115200

#include "Packet.h"

char inputKey;


void setup() {
  // put your setup code here, to run once:
  Serial.begin((unsigned long)BAUDRATE);
  delay(10);
  Serial.println("Hello Ringggo");

  WifiInit();
  GameServerInit();
  RtlsServerInit();
  NfcInit();
  BatteryInit();
  SubBoardInit();
  LedInit();
  BatteryInit();
  // Mp3Init();

  // Mp3Play(1);
  
  Serial.println("Start NFC Task");
  xTaskCreate(  
    NfcTask,                // Task 함수 이름
    "NfcTask",              // Task 이름
    100000,                  // Task 스택 크기
    NULL,                   // Task 파라미터
    10,                     // Task 우선순위
    NULL);                  // Task handle

  Serial.println("Start GameServer Task");
  xTaskCreate(
      GameServerTask,         // Task 함수 이름
      "GameServerTask",       // Task 이름
      10000,                  // Task 스택 크기
      NULL,                   // Task 파라미터
      9,                      // Task 우선순위
      NULL);                  // Task handle

  Serial.println("Start RtlsServer Task");
  xTaskCreate(
      RtlsServerTask,         // Task 함수 이름
      "RtlsServerTask",       // Task 이름
      10000,                  // Task 스택 크기
      NULL,                   // Task 파라미터
      8,                      // Task 우선순위
      NULL);                  // Task handle

  Serial.println("Start Wifi Task");
  xTaskCreate(
      WifiTask,                // Task 함수 이름
      "WifiTask",              // Task 이름
      10000,                  // Task 스택 크기
      NULL,                   // Task 파라미터
      7,                      // Task 우선순위
      NULL);                  // Task handle

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
      10000,                  // Task 스택 크기
      NULL,                   // Task 파라미터
      5,                      // Task 우선순위
      NULL);                  // Task handle

}

void loop() {
  // put your main code here, to run repeatedly:
//  Serial.println("Ringggo car monitor program");
//  Serial.println("press h to help");
//  Serial.println("");
//  while(1) {
//      if(Serial.available()) {
//          inputKey = (char)Serial.read();
//          if(inputKey == 'h') {
//          }else if(inputKey == 's') {
//            Mp3Play(1);
//          }else if(inputKey == 'm') {
//            Mp3Stop();
//          }else if(inputKey == 'l') {
//            while(1)
//            {
//              Blink();
//              vTaskDelay(1) ;
//            }
//          }else {
//          }
//          Serial.print(">>> ");
//          vTaskDelay(1) ;
//      }
//   }
}
