#include "app_soil_moisture.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_log.h"

#include "spi_adc.h"
#include "app_env_data_manage.h"

#define TAG                     "APP_ILLUMINANCE"

#define TASK_DELAY_TIME         5000
#define WAITING_SET_DATA_TIME   1000

#define MAX_ADC_RAW_VALUE       3400
#define MIN_ADC_RAW_VALUE       2300

static TaskHandle_t get_soil_moisture_task;

static void get_soil_moisture_task_handler()
{
    SPI_ADC_DATA_t sm_sensor_adc;
    SPI_ADC_Init(&sm_sensor_adc, SPI_ADC_CH1);
    while(1)
    {
        esp_err_t ret = SPI_ADC_ReadValue(&sm_sensor_adc);
        if(ret == ESP_OK)
        {
            if(sm_sensor_adc.raw_value > MAX_ADC_RAW_VALUE)
                sm_sensor_adc.raw_value = MAX_ADC_RAW_VALUE;
            if(sm_sensor_adc.raw_value < MIN_ADC_RAW_VALUE)
                sm_sensor_adc.raw_value = MIN_ADC_RAW_VALUE;

            APP_ENV_DATA_MANAGE_SetValue(APP_ENV_SET_SOIL_MOISTURE, (MAX_ADC_RAW_VALUE - sm_sensor_adc.raw_value)*100.0/(MAX_ADC_RAW_VALUE - MIN_ADC_RAW_VALUE));
        }
        vTaskDelay(TASK_DELAY_TIME/portTICK_PERIOD_MS);
    }
}

void APP_SOIL_MOISTURE_Init()
{
    xTaskCreate(get_soil_moisture_task_handler, "get_soil_moisture_task_handler", 2048, NULL, tskIDLE_PRIORITY, &get_soil_moisture_task);
}

