/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"
#include "output_driver.h"

static const char *TAG = "example";

/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define CONFIG_BLINK_LED_GPIO   1
#define BLINK_GPIO              3

static uint8_t s_led_state = 0;

void app_main(void)
{

    /* Configure the peripheral according to the LED type */
    // configure_led();
    OUTPUT_DRIVER_Init(BLINK_GPIO);

    while (1) {
        OUTPUT_DRIVER_Toggle(BLINK_GPIO);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
