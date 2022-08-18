#include <FastLED.h>

#define NUM_LEDS 2

#define DATA_PIN 4
#define CLOCK_PIN 15

#define LED_0               0
#define LED_1               1
#define LED_ALL             255

#define LED_OFF             0
#define LED_ON              1
#define LED_BLINK           2
#define LED_BLINK_RANDOM    255


#define COLOR_RINGGGO   0x00AC30
#define COLOR_WHITE     0x808080
#define COLOR_YELLOW    0xFFFF00
#define COLOR_RED       0xFF0000
#define COLOR_DARKRED   0x8B0000
#define COLOR_LIME      0x00FF00
#define COLOR_GREEN     0x008000
#define COLOR_BLUE      0x0000FF
#define COLOR_AQUA      0x00FFFF


uint32_t colorList[9] = {
  COLOR_RINGGGO, COLOR_WHITE, COLOR_YELLOW, COLOR_RED,
  COLOR_DARKRED, COLOR_LIME, COLOR_GREEN, COLOR_BLUE,
  COLOR_AQUA
};

#define RGB(r,g,b)      (r<<16|g<<8|b)

#define RED(color)      (color>>16)
#define GREEN(color)    (color>>8 & 0x000000ff)
#define BLUE(color)     (color & 0x000000ff)


CRGB leds[NUM_LEDS];


void LedInit()
{
  log_i("Init Led");

  FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
}

void LedOn(uint8_t index, uint32_t color)
{
  if(index == LED_ALL)
  {
    leds[0] = color;
    leds[1] = color;
  }
  else
  {
    leds[index] = color;
  }
  FastLED.show();

}

void LedOn(uint8_t index, uint32_t color, uint32_t msec)
{
  for(int i=0; i<256; i++)
  {
    leds[0].r = (i/256.0) * RED(color);
    leds[0].g = (i/256.0) * GREEN(color);
    leds[0].b = (i/256.0) * BLUE(color);
    leds[1].r = (i/256.0) * RED(color);
    leds[1].g = (i/256.0) * GREEN(color);
    leds[1].b = (i/256.0) * BLUE(color);
    FastLED.show();
    vTaskDelay(5);
  }

}

void LedOff(uint8_t index)
{
  if(index == LED_ALL)
  {
    leds[0] = CRGB::Black;
    leds[1] = CRGB::Black;
  }
  else
  {
    leds[index] = CRGB::Black;
  }
  FastLED.show();

}

void Blink(uint8_t index, uint32_t color, uint16_t msec)
{
  LedOn(index, color);
  vTaskDelay(msec);
  LedOff(index);
  vTaskDelay(msec);
}

void LedTask(void* parameter)
{
    BaseType_t xStatus;

    uint8_t packetBody[7] = { 0, };

    uint8_t index = LED_ALL;
    uint8_t type = LED_BLINK;
    uint16_t ledTime = 500;
    uint32_t color = COLOR_RINGGGO;

    LedInit();
  
    for (;;)
    {
      // Serial.println("Led Task");
      xStatus = xQueueReceive(xQueueLed, packetBody, 100);
      if(xStatus == pdPASS)
      {
        Serial.printf("led queue received: %d, %d, %d, %d, %d, %d, %d\n", packetBody[0], packetBody[1], packetBody[2], packetBody[3], packetBody[4], packetBody[5], packetBody[6]);
        index = packetBody[0];
        type = packetBody[1];
        ledTime = (packetBody[2] | packetBody[3] << 8);
        color = RGB(packetBody[4], packetBody[5], packetBody[6]);
      }
      
      if(type == LED_OFF) {
        LedOff(index);
      } else if(type == LED_ON) {
        if(ledTime >= 500)
        {
          LedOn(index, color, ledTime);
        }
        else
        {
          LedOn(index, color);
        }
      } else if (type == LED_BLINK) {
        Blink(index, color, ledTime);
      } else if (type == LED_BLINK_RANDOM) {
        Blink(index, colorList[rand()%sizeof(colorList)], ledTime);
      } else {
        Serial.println("invalid led type");
      }

    }
    vTaskDelete(NULL);
}
