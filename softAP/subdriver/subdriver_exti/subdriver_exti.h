#ifndef SUBDRIVER_EXTI_H
#define SUBDRIVER_EXTI_H

#include <stdint.h>

typedef void (*EXTI_CALLBACK_FUNCTION_t) (uint32_t pin);

typedef enum
{
    EXTI_EDGE_RISING     = 1,
    EXTI_EDGE_FALLING    = 2,
    EXTI_EDGE_ANY        = 3,
} EXTI_EDGE_t;

void SUBDRIVER_EXTI_Init(uint32_t pin, EXTI_EDGE_t edge);
void SUBDRIVER_EXTI_SetCallbackFunction(EXTI_CALLBACK_FUNCTION_t CallbackFunction);

#endif
