#define INFLUXDB_URL       "http://iwinv.jaehoonj.981park.net:8086"
#define INFLUXDB_DB_NAME   "esp32"

#include <InfluxDbClient.h>


void MonitorInit()
{
  Serial.println("Init Monitor");
  Serial.print("connecting to ");
  Serial.println(INFLUXDB_URL);

}

void MonitorTask(void* parameter)
{
  MonitorInit();

  InfluxDBClient client(INFLUXDB_URL, INFLUXDB_DB_NAME);

  vTaskDelay(3000);

  if(client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  Point sensor("wifi_status");
  sensor.addTag("car_id", (String)CAR_ID);
  sensor.addTag("ssid", WiFi.SSID());
  
  for (;;)
  {
    // TODO
    Serial.println("Monitor Task");
    sensor.clearFields();
    sensor.addField("rssi", WiFi.RSSI());
    sensor.addField("uptime", millis());
    sensor.addField("battery", g_dBatteryValue);
    // sensor.addField("healthcheck", 1);
    // sensor.addField("rssi", "test");

    if(!client.writePoint(sensor)) {
      Serial.print("InfluxDB write failed: ");
      Serial.println(client.getLastErrorMessage());
    }
    vTaskDelay(60000);
  }
  vTaskDelete(NULL);
}
