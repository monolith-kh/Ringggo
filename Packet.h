/* reqCode on Protocol_t */
#define PK_WHO_ARE_YOU_ANS			(0x90)
#define PK_IAM_ANS					(0x92)
#define PK_CHECK_CONNECTION_REQ		(0xB1)
#define PK_CHECK_CONNECTION_ANS		(0xB2)
#define PK_NFC_NOTI					(0xC1)
#define PK_BATTERY_NOTI				(0xC2)
#define PK_CAR_SPEED_NOTI			(0xC3)
#define PK_CAR_LED_NOTI				(0xC4)
#define PK_CAR_SOUND_NOTI			(0xC5)
#define PK_CAR_START_STOP_NOTI		(0xC6)
#define PK_BUMP_NOTI                (0xC7)
#define PK_POSITION_NOTI            (0xD1)

// #pragma pack(1)
typedef struct {
    uint8_t reqCode;
    uint8_t sender;
    uint16_t length;
    uint16_t carNo;
    uint16_t reserved;
    uint8_t body[32];
} Protocol_t;

typedef enum {
    GAME_SERVER = 3,
    CAR = 6,
    RTLS_SERVER = 7
} Sender;
