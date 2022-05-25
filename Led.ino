#include <FastLED.h>

#define NUM_LEDS 1

#define DATA_PIN 4
#define CLOCK_PIN 15

#define LED_OFF             0
#define LED_ON              1
#define LED_BLINK_200M_SEC  2
#define LED_BLINK_500M_SEC  3
#define LED_BLINK_RANDOM    4


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


CRGB leds[NUM_LEDS];


void LedInit()
{
  Serial.println("Init Led");

  FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
}

void LedOn(uint32_t color)
{
  Serial.println("Led On");

  // Turn the LED on, then pause
  leds[0] = color;
  FastLED.show();
  
}

void LedOff()
{
  Serial.println("Led Off");
  // Now turn the LED off, then pause
  leds[0] = CRGB::Black;
  FastLED.show();

}

void Blink(uint32_t color, uint32_t msec)
{
  LedOn(color);
  vTaskDelay(msec);
  LedOff();
  vTaskDelay(msec);
}

void LedTask(void* parameter)
{
    BaseType_t xStatus;

    uint8_t packetBody[4] = { 0, };

    uint8_t type = LED_BLINK_200M_SEC;
    uint32_t color = COLOR_RINGGGO;

    LedInit();
  
    for (;;)
    {
      Serial.println("Led Task");
      xStatus = xQueueReceive(xQueueLed, packetBody, 100);
      if(xStatus == pdPASS)
      {
        Serial.printf("led queue received: %d, %d, %d, %d\n", packetBody[0], packetBody[1], packetBody[2], packetBody[3]);
        type = packetBody[0];
        color = RGB(packetBody[1], packetBody[2], packetBody[3]);
      }
      
      if(type == LED_OFF) {
        LedOff();
      } else if(type == LED_ON) {
        LedOn(color);
      } else if (type == LED_BLINK_200M_SEC) {
        Blink(color, 200);
      } else if (type == LED_BLINK_500M_SEC) {
        Blink(color, 500);
      } else if (type == LED_BLINK_RANDOM) {
        Blink(colorList[rand()%sizeof(colorList)], 500);
      } else {
        Serial.println("invalid led type");
      }

      // Blink(COLOR_RINGGGO, 500);
      // Blink(COLOR_RED, 500);
      // Blink(COLOR_WHITE, 500);
      // Blink(COLOR_YELLOW, 500);
      // Blink(COLOR_DARKRED, 500);
      // Blink(COLOR_LIME, 500);
      // Blink(COLOR_BLUE, 500);
      // Blink(COLOR_GREEN, 500);
      // Blink(COLOR_AQUA, 500);
    }
    vTaskDelete(NULL);
}
