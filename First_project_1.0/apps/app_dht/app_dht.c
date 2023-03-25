#include "app_dht.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_log.h"

#include "dht.h"
#include "app_env_data_manage.h"

#define TAG                 "APP_DHT"

#define TASK_DELAY_TIME     5000

#define DHT_PIN             6

static TaskHandle_t get_dht_task;

static DHT_DATA_t dht_data;

static void get_dht_task_handler()
{
    DHT_Init(&dht_data, DHT22, DHT_PIN);
    while(1)
    {
        // DHT_Read_Value(&dht_data);
        ESP_LOGI(TAG, "Is success: %d - Temp: %2.2f - Humi: %2.2f\n", DHT_Read_Value(&dht_data), dht_data.temperature, dht_data.humidity);

        // SENSOR_DATA_t sensor_data = 
        // {
        //     .value = dht_data.temperature
        // };
        // APP_ENV_DATA_MANAGE_SetAirTempData(&sensor_data, false, true);

        // sensor_data.value = dht_data.huminity;
        // APP_ENV_DATA_MANAGE_SetAirHumiData(&sensor_data, false, true);

        vTaskDelay(TASK_DELAY_TIME/portTICK_PERIOD_MS);
    }

}

void APP_DHT_Init()
{
    xTaskCreate(get_dht_task_handler, "get_dht_task_handler", 2048, NULL, 10, &get_dht_task);
}
