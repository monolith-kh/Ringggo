#define GET_NFC_DATA    1
#define MAX_PACKET      16

#define PN532_SS  (5)


#include <Adafruit_PN532.h>

Adafruit_PN532 nfc(PN532_SS);


void NfcInit()
{
  Serial.println("Init Nfc");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
      Serial.println("Didn't find PN53x board");
  }
  else
  {
      Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
      Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
      Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);
  }
  // configure board to read RFID tags
  nfc.SAMConfig();
}

void NfcTask(void* parameter)
{
    uint8_t success;
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    uint8_t data[128] = { 0, };
    char l_arrRtnPacket[MAX_PACKET] = { 0, };
    char l_arrID[32] = { 0, };
    uint8_t keya[6] = { 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 };
    uint8_t keyb[6] = { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7 };
    
    for (;;)
    {
        Serial.println("Nfc Task");
        
        success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

        if (success)
        {
            Serial.println("Found a card!");
            Serial.print("UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
            Serial.print("UID Value: ");
            for (uint8_t i = 0; i < uidLength; i++)
            {
                Serial.print(" 0x"); Serial.print(uid[i], HEX);
            }
            Serial.println("");

            success = nfc.mifareclassic_ReadDataBlock(4, data);
            success = nfc.mifareclassic_ReadDataBlock(8, &data[16]);

            if (success) 
            {
                // Data seems to have been read ... spit it out
                // 읽었으니 데이터 전송.
                memcpy(l_arrID, &data[14], 13); l_arrID[13] = '\0';
                sprintf(l_arrRtnPacket, "$%d,%d,%s;", CAR_ID, GET_NFC_DATA, l_arrID);
                Serial.println(l_arrRtnPacket);

                Serial.println("Reading Block 4:, 8");
                nfc.PrintHexChar(data, 48);
                Serial.println("");

                // 그외 NDEF NFC는 고객테그로 간주하고 서버로 데이터 송신 
//                g_udNFCFlag = 1;
                Serial.println(l_arrID);
//                SendNFCData((char*)uid, l_arrID);


                // Wait a bit before reading the card again
                vTaskDelay(1000);
            }
            else
            {
                // 리딩실패
                Serial.println("Ooops ... unable to read the requested block.  Try another key?");
            }
            // 요기서 읽음.

            
        }
        else
        {
            Serial.println("Timed out waiting for a card");
        }
        vTaskDelay(10);
    }

    vTaskDelete(NULL);
}
