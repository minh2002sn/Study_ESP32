/* WiFi station Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "wifi_station_app.h"
#include "http_server_app.h"
#include "subdriver_gpio.h"
#include "subdriver_pwm.h"

#define GPIO_BUTTON_PIN         9
#define GPIO_RGB_RED_PIN        3
#define GPIO_RGB_GREEN_PIN      4
#define GPIO_RGB_BLUE_PIN       5
#define GPIO_ORANGE_LED         18
#define GPIO_WHITE_LED          19

void web_switch_callback(int state)
{
    SUBDRIVER_GPIO_SetState(GPIO_ORANGE_LED, state);
}

void web_slider_callback(int duty)
{
    SUBDRIVER_PWM_SetValue(GPIO_RGB_RED_PIN, duty);
}

void app_main(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    SUBDRIVER_GPIO_OutputInit(GPIO_ORANGE_LED);
    SUBDRIVER_GPIO_OutputInit(GPIO_WHITE_LED);
    switch_set_callback(web_switch_callback);

    SUBDRIVER_PWM_Init(GPIO_RGB_RED_PIN);
    slider_set_callback(web_slider_callback);

    wifi_send_set_callback(WIFI_STA_APP_Connect);

    WIFI_STA_APP_Connect("Cafe-18", "37cafe18");

}
