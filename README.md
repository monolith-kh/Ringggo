# Ringggo

## Board 환경 설치
- File → Preferences → Additional Boards Manager URLs
- https://dl.espressif.com/dl/package_esp32_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json
- Tools → Board → Boards Manager → esp32 검색 및 설치

## Tools에 세팅
- Board: ESP32 Wrover Module
- Upload Speed: 921600 (default)
- Flash Frequency: 80MHz (default)
- Flash Mode: DIO
- Partition Scheme: 8M Flash (3MB APP/1.5MB FAT)
- Core Debug Level: None

## 필요한 라이브러리 설치
- Adafruit BusIO 1.2.2 (버전 준수)
- Adafruit PN532
- FastLED
- DFRobotDFPlayerMini
- ESP8266 Influxdb 3.12.0

## 드라이버 설치
- ESP32 USBtoSerial Driver 받는 곳 (https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)

## 참고
- Installing the ESP32 Board in Arduino IDE (https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/)

