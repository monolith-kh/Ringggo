#define RTLS_HOST   "192.168.41.254"
#define RTLS_PORT   9999

#include <WiFiUdp.h>

WiFiUDP rtlsClient;


void RtlsServerInit()
{
  Serial.println("Init RtlsServer");
  Serial.print("connecting to ");
  Serial.println(RTLS_HOST);

  // Use WiFiUDP class to create UDP connections
  rtlsClient.beginPacket(RTLS_HOST, RTLS_PORT);
  Protocol_t protocol = {PK_IAM_ANS, CAR, 8, 10, 0, };
  // byte pb[40];
  // memcpy(pb, &protocol, 40);
  rtlsClient.write((const uint8_t *)&protocol, sizeof(protocol));
  rtlsClient.endPacket();
  // Serial.printf("0x%x\n", [0]);
  // Serial.printf("%d\n", pb[1]);
  // Serial.printf("%d\t%d\n", pb[2], pb[3]);
  // Serial.printf("%d\t%d\n", pb[4], pb[5]);
  // Serial.printf("%d\t%d\n", pb[6], pb[7]);
  // Serial.printf("%d\t%d\n", pb[8], pb[9]);
}

void RtlsServerTask(void* parameter)
{
  for (;;)
  {
    // TODO
    Serial.println("RtlsServer Task");
    rtlsClient.beginPacket(RTLS_HOST, RTLS_PORT);
    Protocol_t protocol = { PK_IAM_ANS, CAR, 8, 10, 0, };
    rtlsClient.write((uint8_t *)&protocol, sizeof(protocol));
    rtlsClient.endPacket();

    vTaskDelay(100);
  }
  vTaskDelete(NULL);
}
