#ifndef APP_LED
#define APP_LED

typedef enum
{
    LED_OFF_STATE,
    LED_ON_STATE,
    LED_BLINK_LOW,
    LED_BLINK_FAST,
} LED_STATE_t;

void APP_LED_Init();
void APP_LED_SetState(LED_STATE_t state);

#endif
