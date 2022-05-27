#define BMP_SEN_FRONT   36
#define BMP_SEN_LEFT    34
#define BMP_SEN_RIGHT   39
#define BMP_SEN_REAR    35

#define FRONT   0x01
#define LEFT    0x02
#define RIGHT   0x04
#define REAR    0x08

uint8_t bumper;
bool pressed;

void BumperInit()
{
    Serial.println("Init Bumper");

    bumper = 0;
    pressed = false;

    pinMode(BMP_SEN_FRONT, INPUT);
    pinMode(BMP_SEN_LEFT, INPUT);
    pinMode(BMP_SEN_RIGHT, INPUT);
    pinMode(BMP_SEN_REAR, INPUT);
}

void BumperTask(void* parameter)
{
    BaseType_t xStatus;

    BumperInit();

    for (;;)
    {
        // Serial.println("Bumper Task");

        if(digitalRead(BMP_SEN_FRONT) == LOW)
        {
            bumper |= FRONT;
            pressed = true;
        }
        if(digitalRead(BMP_SEN_RIGHT) == LOW)
        {
            bumper |= RIGHT;
            pressed = true;
        }
        if(digitalRead(BMP_SEN_LEFT) == LOW)
        {
            bumper |= LEFT;
            pressed = true;
        }
        if(digitalRead(BMP_SEN_REAR) == LOW)
        {
            bumper |= REAR;
            pressed = true;
        }

        if(bumper && pressed)
        {
            // SendBumper(bumper);
            xStatus = xQueueSendToFront(xQueueBumper, &bumper, 100);
            if(xStatus == pdPASS)
            {
                Serial.printf("send game server queue: %x\n", bumper);
            }
            bumper = 0;
            pressed = false;
        }
        
        vTaskDelay(100);
    }

    vTaskDelete(NULL);
}
