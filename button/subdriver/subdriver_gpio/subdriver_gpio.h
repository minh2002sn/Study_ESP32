#ifndef SUBDRIVER_GPIO_H
#define SUBDRIVER_GPIO_H

#include <stdint.h>

typedef enum
{
    GPIO_PULL_UP,
    GPIO_PULL_DOWN,
} GPIO_RES_PULL_t;

void SUBDRIVER_GPIO_InputInit(uint32_t pin, GPIO_RES_PULL_t gpio_res_pull);
void SUBDRIVER_GPIO_OutputInit(uint32_t pin);
void SUBDRIVER_GPIO_SetState(uint32_t pin, uint8_t state);
uint8_t SUBDRIVER_GPIO_GetState(uint32_t pin);

#endif
