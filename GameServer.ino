#define GAME_HOST   "192.168.41.254"
#define GAME_PORT   8888


void GameServerInit()
{
  Serial.println("Init GameServer");
}

void GameServerTask(void* parameter)
{
   
    for (;;)
    {
      // TODO
      Serial.println("GameServer Task");
      vTaskDelay(1000);
  
    }
    vTaskDelete(NULL);
}
