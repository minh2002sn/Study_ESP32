#include <stdio.h>
#include "app_button.h"
#include "button.h"
#include "subdriver_exti.h"
#include "app_wifi_config.h"

#define TAG                     "APP_BUTTON"

#define BUTTON_PIN              1

BUTTON_HANDLE_t button;

static void BUTTON_Long_Press_Handle(int pin)
{
    if(pin == BUTTON_PIN)
    {
        printf("Long press\n");
        APP_WIFI_CONFIG_Connect(PROVISION_BY_SMARTCONFIG);
    }
}

static void EXTI_CallbackFunction(uint32_t pin)
{

    BUTTON_EXTI_Handle(&button, pin);
} 

void APP_BUTTON_Init()
{
    SUBDRIVER_EXTI_SetCallbackFunction(EXTI_CallbackFunction);

    BUTTON_SetCallbackFunction(NULL, NULL, NULL, BUTTON_Long_Press_Handle);
    BUTTON_Init(&button, BUTTON_PIN);

}
