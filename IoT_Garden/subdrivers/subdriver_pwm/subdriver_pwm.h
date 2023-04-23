#ifndef SUBDRIVER_PWM_H
#define SUBDRIVER_PWM_H

#include <stdint.h>
#include "driver/ledc.h"

// #define NOT_USING_TIMER_0
#ifdef NOT_USING_TIMER_0
void SUBDRIVER_PWM_SetTimer(ledc_timer_t timer);
#endif

void SUBDRIVER_PWM_Init(int pin);
void SUBDRIVER_PWM_SetValue(int pin, uint32_t duty);

#endif
