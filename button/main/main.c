/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <driver/gpio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"

#include "button.h"
#include "subdriver_gpio.h" 
#include "subdriver_exti.h"

#define GPIO_BUTTON_PIN     9
#define GPIO_RGB_LED_PIN_1  3
#define GPIO_RGB_LED_PIN_2  4
#define GPIO_RGB_LED_PIN_3  5
#define GPIO_OUTPUT_PIN     18

BUTTON_HANDLE_t button;

extern QueueHandle_t        button_event_queue;

static void BUTTON_Pressing_Handle(int pin)
{
    if(pin == GPIO_BUTTON_PIN)
    {
        printf("Pressing\n");
    }
}

static void BUTTON_Releasing_Handle(int pin)
{
    static uint8_t counter = 0;
    if(pin == GPIO_BUTTON_PIN)
    {
        printf("Releasing\n");
        counter++;
        if(counter == 8) counter = 0;
        SUBDRIVER_GPIO_SetState(GPIO_RGB_LED_PIN_1, counter & 1);
        SUBDRIVER_GPIO_SetState(GPIO_RGB_LED_PIN_2, counter & 2);
        SUBDRIVER_GPIO_SetState(GPIO_RGB_LED_PIN_3, counter & 4);
    }
}

static void BUTTON_Short_Press_Handle(int pin)
{
    if(pin == GPIO_BUTTON_PIN)
    {
        printf("Short press\n");
    }
}

static void BUTTON_Long_Press_Handle(int pin)
{
    if(pin == GPIO_BUTTON_PIN)
    {
        printf("Long press\n");
    }
}

static void EXTI_CallbackFunction(uint32_t pin)
{

    BUTTON_EXTI_Handle(&button, pin);
} 

void app_main(void)
{
    SUBDRIVER_GPIO_OutputInit(GPIO_RGB_LED_PIN_1);
    SUBDRIVER_GPIO_OutputInit(GPIO_RGB_LED_PIN_2);
    SUBDRIVER_GPIO_OutputInit(GPIO_RGB_LED_PIN_3);

    // SUBDRIVER_EXTI_Init(GPIO_BUTTON_PIN, EXTI_EDGE_ANY);
    SUBDRIVER_EXTI_SetCallbackFunction(EXTI_CallbackFunction);

    BUTTON_SetCallbackFunction( BUTTON_Pressing_Handle, BUTTON_Releasing_Handle, 
                                BUTTON_Short_Press_Handle, BUTTON_Long_Press_Handle);
    BUTTON_Init(&button, GPIO_BUTTON_PIN);

}
 