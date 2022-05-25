#define WIFI_SSID      "ringo-dev"
#define WIFI_PASSWORD  "mono1010"


#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>


void WifiInit()
{
  Serial.println("Init Wifi");
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, 13);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void WifiTask(void* parameter)
{
   
    for (;;)
    {
      // TODO
      Serial.println("Wifi Task");
      if(WiFi.status() == WL_CONNECTED)
      {
        Serial.println("Wifi online");
        Serial.println(portTICK_PERIOD_MS);
        Serial.println(WiFi.RSSI());
        vTaskDelay(10000 / portTICK_PERIOD_MS);
        continue;
      }
//      vTaskDelay(1000);
    }
    vTaskDelete(NULL);
}
