#define WIFI_SSID      "ringo-dev"
#define WIFI_PASSWORD  "mono1010"


// #include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>


void WifiInit()
{
  log_i("Init Wifi");

  WiFi.disconnect(true);
  WiFi.setHostname(hostName);
  WiFi.mode(WIFI_STA);

  // WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  log_i("");
  log_i("WiFi connected");
  log_i("IP address:");
  Serial.println(WiFi.localIP());

}

void WifiTask(void* parameter)
{
   
    for (;;)
    {
      log_i("Wifi Task");
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
