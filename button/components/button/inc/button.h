#ifndef BUTTON_H

#include <stdint.h>

typedef void (*BUTTON_CALLBACK_FUNCTION_t) (int pin);

typedef struct
{
    static BUTTON_CALLBACK_FUNCTION_t BUTTON_PressingCallback = NULL;
    static BUTTON_CALLBACK_FUNCTION_t BUTTON_ReleasingCallback = NULL;
    static BUTTON_CALLBACK_FUNCTION_t BUTTON_ShortPressingCallback = NULL;
    static BUTTON_CALLBACK_FUNCTION_t BUTTON_LongPressingCallback = NULL;
    static uint32_t gpio_pin = 0;
    static uint64_t deboucing_timer = 0;
    static uint64_t pressing_timer = 0;
    static uint8_t is_pressing = 0;
} ButtonHandle_t;

void BUTTON_Init(int pin);
void BUTTON_Set_Callback_Function(void *PressingCallback, void *ReleasingCallback);


#endif