/* reqCode on Protocol_t */
#define PK_WHO_ARE_YOU_ANS			0x90
#define PK_IAM_ANS					0x92
#define PK_CHECK_CONNECTION_REQ		0xB1
#define PK_CHECK_CONNECTION_ANS		0xB2
#define PK_NFC_NOTI				    0xC1
#define PK_BATTERY_NOTI				0xC2
#define PK_CARSPEED_NOTI			0xC3
#define PK_CARLED_NOTI				0xC4
#define PK_CARSOUND_NOTI			0xC5
#define PK_CARSTARTSTOP_NOTI		0xC6
#define PK_BUMP_NOTI                0xC7
#define PK_POSITION_NOTI            0xD1

#pragma pack(1)
typedef struct {
    uint8_t reqCode;
    uint8_t sender;
    uint16_t length;
    uint16_t carNo;
    uint16_t reserved;
    uint8_t body[];
} Protocol_t;

#pragma pack(1)
typedef struct {
    uint8_t reqCode;
    uint8_t sender;
    uint16_t length;
    uint16_t carNo;
    uint16_t reserved;
    uint8_t body;
} Protocol_bt;

#pragma pack(1)
typedef struct {
    uint8_t reqCode;
    uint8_t sender;
    uint16_t length;
    uint16_t carNo;
    uint16_t reserved;
    char wrbId[13];
    uint8_t sep;
    uint8_t uid[7];
} Protocol_nfct;

#pragma pack(1)
typedef struct {
    uint8_t reqCode;
    uint8_t sender;
    uint16_t length;
    uint16_t carNo;
    uint16_t reserved;
    uint8_t bumper;
} Protocol_bumpert;

#pragma pack(1)
typedef struct {
    uint8_t reqCode;
    uint8_t sender;
    uint16_t length;
    uint16_t carNo;
    uint16_t reserved;
    int64_t timestamp;
    int16_t positionX;
    int16_t positionY;
    int16_t headAngle;
} Protocol_position_t;

typedef enum {
    GAME_SERVER = 3,
    CAR = 6,
    RTLS_SERVER = 7
} Sender;
