#include <FastLED.h>

#define NUM_LEDS 1

#define DATA_PIN 4
#define CLOCK_PIN 15

CRGB leds[NUM_LEDS];


void LedInit()
{
  Serial.println("Init Led");

  FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
}

void LedOn()
{
  Serial.println("Led On");

  // Turn the LED on, then pause
//  leds[0] = CRGB::Khaki;
  leds[0] = 0x00AC30;
//  leds[0] = CRGB(255,0,0);
  FastLED.show();
  
}

void LedOff()
{
  Serial.println("Led Off");
  // Now turn the LED off, then pause
  leds[0] = CRGB::Black;
//  leds[0] = CRGB(0, 0, 0);
  FastLED.show();

}

void Blink()
{
  LedOn();
  delay(500);
  LedOff();
  delay(500);
}

void LedTask(void* parameter)
{
    for (;;)
    {
      // TODO
      Serial.println("Led Task");
      LedOn();
      vTaskDelay(500);
      LedOff();
      vTaskDelay(500);
    }
    vTaskDelete(NULL);
}
