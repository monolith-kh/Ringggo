// #define RTLS_HOST   "oldtrafford.ringggo.981park.net"
// // #define RTLS_HOST   "192.168.41.254"
// #define RTLS_PORT   9999

// #define POSITION_X_MAX  1500
// #define POSITION_Y_MAX  2000
// #define HEADANGLE_MAX  360

// #include <WiFiUdp.h>

// WiFiUDP rtlsClient;


// void RtlsServerInit()
// {
//   Serial.println("Init RtlsServer");
//   Serial.print("connecting to ");
//   Serial.println(RTLS_HOST);

// }

// void RtlsServerTask(void* parameter)
// {
//   RtlsServerInit();
  
//   for (;;)
//   {
//     // TODO
//     Serial.println("RtlsServer Task");
//     rtlsClient.beginPacket(RTLS_HOST, RTLS_PORT);
//     Protocol_position_t protocol = { PK_POSITION_NOTI, CAR, 22, CAR_ID, 0, };
//     protocol.timestamp = (int64_t)0x80808080FFFFFFFF;
//     protocol.positionX = (int16_t)(rand() % POSITION_X_MAX);
//     protocol.positionY = (int16_t)(rand() % POSITION_Y_MAX);
//     protocol.headAngle = (int16_t)(rand() % HEADANGLE_MAX);
//     rtlsClient.write((uint8_t *)&protocol, sizeof(protocol));
//     rtlsClient.endPacket();

//     vTaskDelay(100);
//   }
//   vTaskDelete(NULL);
// }
