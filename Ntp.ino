//#include <NTPClient.h>
//#include <WiFiUdp.h>
//
//WiFiUDP ntpUDP;

// By default 'pool.ntp.org' is used with 60 seconds update interval and
// no offset
//NTPClient timeClient(ntpUDP);
// NTPClient timeClient(ntpUDP, "time.bora.net", 0, 60000);

// You can specify the time server pool and the offset, (in seconds)
// additionally you can specify the update interval (in milliseconds).
// NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);


//void NtpInit()
//{
//    Serial.println("Init Ntp");
//
//    timeClient.begin();
//    timeClient.update();
//}

// void NtpTask(void* parameter)
// {
//    NtpInit();
//  
//   for (;;)
//   {
//       Serial.println("Ntp Task");

//       timeClient.update();
//       Serial.println(timeClient.getFormattedTime());
//       Serial.println(timeClient.getEpochTime());

//       vTaskDelay(1000);
//   }

//   vTaskDelete(NULL);
// }
