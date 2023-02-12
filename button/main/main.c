/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "button.h"
#include "output_driver.h"
#include <driver/gpio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define GPIO_BUTTON_PIN     9
#define GPIO_RGB_LED_PIN_1  3
#define GPIO_RGB_LED_PIN_2  4
#define GPIO_RGB_LED_PIN_3  5
#define GPIO_OUTPUT_PIN     18

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
        OUTPUT_DRIVER_Set_State(GPIO_RGB_LED_PIN_1, counter & 1);
        OUTPUT_DRIVER_Set_State(GPIO_RGB_LED_PIN_2, counter & 2);
        OUTPUT_DRIVER_Set_State(GPIO_RGB_LED_PIN_3, counter & 4);
    }
}

void app_main(void)
{
    OUTPUT_DRIVER_Init(GPIO_RGB_LED_PIN_1);
    OUTPUT_DRIVER_Init(GPIO_RGB_LED_PIN_2);
    OUTPUT_DRIVER_Init(GPIO_RGB_LED_PIN_3);

    BUTTON_Set_Callback_Function(BUTTON_Pressing_Handle, BUTTON_Releasing_Handle);
    BUTTON_Init(GPIO_BUTTON_PIN);

}
