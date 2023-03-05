/* LEDC (LED Controller) basic example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "driver/ledc.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "subdriver_pwm.h"

#define GPIO_BUTTON_PIN         9
#define GPIO_RGB_RED_PIN        3
#define GPIO_RGB_GREEN_PIN      4
#define GPIO_RGB_BLUE_PIN       5
#define GPIO_ORANGE_LED         18
#define GPIO_WHITE_LED          19



void app_main(void)
{
    SUBDRIVER_PWM_Init(GPIO_RGB_RED_PIN);
    SUBDRIVER_PWM_Init(GPIO_RGB_BLUE_PIN);
    SUBDRIVER_PWM_Init(GPIO_RGB_GREEN_PIN);

    while(1)
    {
        for(int i = 0; i <= 100; i++)
        {
            SUBDRIVER_PWM_SetValue(3, i);
            vTaskDelay(50 / portTICK_PERIOD_MS);
        }

        for(int i = 0; i <= 100; i++)
        {
            SUBDRIVER_PWM_SetValue(3, 100 - i);
            vTaskDelay(50 / portTICK_PERIOD_MS);
        }
    }
}
