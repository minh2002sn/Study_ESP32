#include "app_dht.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_log.h"

#include "dht.h"
#include "app_env_data_manage.h"

#define TAG                 "APP_DHT"

#define TASK_DELAY_TIME     5000

#define DHT_PIN             1

static TaskHandle_t get_dht_task;

static DHT_DATA_t dht_data;

static void get_dht_task_handler()
{
    DHT_Init(&dht_data, DHT_PIN, DHT22);
    int status = 0;
    while(1)
    {
        status = DHT_Read(&dht_data);
        ESP_LOGI(TAG, "Is success: %d - Temp: %2.2f - Humi: %2.2f\n", status, dht_data.temperature, dht_data.humidity);

        SENSOR_DATA_t sensor_data = 
        {
            .value = dht_data.temperature
        };
        // APP_ENV_DATA_MANAGE_SetAirTempData(&sensor_data, false, true);

        sensor_data.value = dht_data.humidity;
        // APP_ENV_DATA_MANAGE_SetAirHumiData(&sensor_data, false, true);

        vTaskDelay(TASK_DELAY_TIME/portTICK_PERIOD_MS);
    }

}

void APP_DHT_Init()
{
    xTaskCreate(get_dht_task_handler, "get_dht_task_handler", 2048, NULL, 11, &get_dht_task);
}
