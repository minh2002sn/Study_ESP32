#include "app_illuminance.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_log.h"

#include "bh1750fvi.h"
#include "app_env_data_manage.h"

#define TAG                     "APP_ILLUMINANCE"

#define TASK_DELAY_TIME         5000
#define WAITING_SET_DATA_TIME   1000

#define A                       1.11921246
#define B                       0.18181753

static TaskHandle_t get_illuminance_task;


static void get_illuminance_task_handler()
{
    BH1750_DATA_t bh1750;
    BH1750_Init(&bh1750, 4, 5, 0, 0);
    while(1)
    {
        esp_err_t ret = BH1750_OneShotRead(&bh1750);
        if(ret == ESP_OK)
        {
            APP_ENV_DATA_MANAGE_SetValue(APP_ENV_SET_ILLUMINANCE, bh1750.illu_value*1.0);
        }
        vTaskDelay(TASK_DELAY_TIME/portTICK_PERIOD_MS);
    }
}

void APP_ILLUMINANCE_Init()
{
    xTaskCreate(get_illuminance_task_handler, "get_illuminance_task_handler", 2048, NULL, 11, &get_illuminance_task);
}
