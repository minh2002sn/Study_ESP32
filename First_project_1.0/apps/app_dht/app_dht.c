#include "app_dht.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_log.h"

#include "dht.h"
#include "app_env_data_manage.h"

#define TAG                     "APP_DHT"

#define TASK_DELAY_TIME         5000
#define WAITING_SET_DATA_TIME   1000

#define DHT_PIN                 8

static TaskHandle_t get_dht_task;

static DHT_DATA_t dht_data;

static void get_dht_task_handler()
{
    DHT_Init(&dht_data, DHT_PIN, DHT22);
    while(1)
    {
        int status = DHT_Read(&dht_data);
        // ESP_LOGI(TAG, "Is success: %d - Temp: %2.2f - Humi: %2.2f\n", status, dht_data.temperature, dht_data.humidity);
        if(status == DHT_OK)
        {
            APP_ENV_DATA_MANAGE_SetValue(APP_ENV_SET_AIR_TEMPERATURE, dht_data.temperature);
            APP_ENV_DATA_MANAGE_SetValue(APP_ENV_SET_AIR_HUMIDITY, dht_data.humidity);
        }
        vTaskDelay(TASK_DELAY_TIME/portTICK_PERIOD_MS);
    }

}

void APP_DHT_Init()
{
    xTaskCreate(get_dht_task_handler, "get_dht_task_handler", 2048, NULL, tskIDLE_PRIORITY, &get_dht_task);
}
