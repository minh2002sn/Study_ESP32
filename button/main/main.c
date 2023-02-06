/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "button.h"
#include <driver/gpio.h>

#define GPIO_BUTTON_PIN     5
#define GPIO_LED_PIN        2

static void BUTTON_Pressing_Handle(int pin)
{
    if(pin == GPIO_BUTTON_PIN)
    {
        gpio_set_level(GPIO_LED_PIN, 1);
    }
}

static void BUTTON_Releasing_Handle(int pin)
{
    if(pin == GPIO_BUTTON_PIN)
    {
        gpio_set_level(GPIO_LED_PIN, 0);
    }
}

void app_main(void)
{
    gpio_config_t io_config;
    io_config.mode = GPIO_MODE_OUTPUT;
    io_config.intr_type = GPIO_INTR_DISABLE;
    io_config.pin_bit_mask = 1U << GPIO_LED_PIN;
    io_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_config.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_config);

    BUTTON_Init(GPIO_BUTTON_PIN);
    BUTTON_Set_Callback_Function(BUTTON_Pressing_Handle, BUTTON_Releasing_Handle);
}
