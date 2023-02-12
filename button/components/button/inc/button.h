#ifndef BUTTON_H

#include <stdint.h>

typedef void (*BUTTON_CALLBACK_FUNCTION_t) (int pin);

typedef struct
{
    uint32_t gpio_pin;
    uint64_t debouncing_timer;
    uint64_t pressing_timer;
    uint8_t is_pressing;
} ButtonHandle_t;

void BUTTON_Init(ButtonHandle_t *button, uint32_t pin);
void BUTTON_Set_Callback_Function(void *PressingCallback, void *ReleasingCallback);
void BUTTON_EXTI_Handle(ButtonHandle_t *button, uint32_t pin);

#endif