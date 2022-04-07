void SubBoardInit()
{
  Serial.println("Init SubBoard");
}

void SubBoardTask(void* parameter)
{
   
    for (;;)
    {
      // TODO
      Serial.println("SubBoard Task");
      vTaskDelay(100);
  
    }
    vTaskDelete(NULL);
}
