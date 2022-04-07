#include <DFRobotDFPlayerMini.h>

DFRobotDFPlayerMini mp3Player;

int volume = 5;

void Mp3Init()
{
  Serial.println("Init Mp3");

  Serial2.begin(9600);
  mp3Player.begin(Serial2);

  mp3Player.setTimeOut(500); //Set serial communictaion time out 500ms
  mp3Player.volume(volume);  //Set volume value. From 0 to 30
  mp3Player.enableDAC();          //Enable On-chip DAC
  mp3Player.outputSetting(true, 0);
  mp3Player.EQ(DFPLAYER_EQ_NORMAL);
  mp3Player.outputDevice(DFPLAYER_DEVICE_SD);

}

void Mp3Play(int number)
{
  Serial.println("Mp3 play");
  mp3Player.play(number);
}

void Mp3Stop()
{
  Serial.println("Mp3 stop");
  mp3Player.stop();
}

void Mp3Task(void* parameter)
{
   
    for (;;)
    {
      // TODO
      Serial.println("Mp3 Task");
      vTaskDelay(100);
  
    }
    vTaskDelete(NULL);
}
