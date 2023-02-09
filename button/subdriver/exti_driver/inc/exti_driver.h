#ifndef EXTI_DRIVER_H
#define EXTI_DRIVER_H

3include <stdint.h>

typedef enum
{
    RISING_EDGE     = 1,
    FALLING_EDGE    = 2,
    ANY_EDGE        = 3,
} ExtiEdge_t

void exti_driver_init(uint32_t pin, ExtiEdge_t edge);

#endif
