#include "app_env_data_manage.h"

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#include "json_generator.h"
#include "json_parser.h"

#include "app_mqtt.h"

#define DEVICE_LABEL    "iot_garden"

static TaskHandle_t mqtt_upload_task;
static TaskHandle_t set_data_task;
static QueueHandle_t setting_data_queue;
static SemaphoreHandle_t setting_data_mutex;

typedef struct
{
    APP_ENV_SETTING_TYPE_t type;
    float value;
} __attribute__((packed)) SETTING_DATA_t;

static ENVIRONMENT_DATA_t environment_data = 
{
    .illuminance =
    {
        .enable = 1,
        .value = 0,
    },
    .air_temperature =
    {
        .enable = 1,
        .value = 0,
    },
    .air_humidity =
    {
        .enable = 1,
        .value = 0,
    },
    .soil_moisture =
    {
        .enable = 1,
        .value = 0,
    },
};

static void set_data_task_handler()
{
    SETTING_DATA_t setting_data = {};
    while(1)
    {
        if(xQueueReceive(setting_data_queue, &setting_data, portMAX_DELAY))
        {
            xSemaphoreTake(setting_data_mutex, portMAX_DELAY);
            SENSOR_DATA_t *sensor_data = (SENSOR_DATA_t *)&environment_data;
            (sensor_data + setting_data.type)->value = setting_data.value;
            xSemaphoreGive(setting_data_mutex);
        }

    }
}

static void mqtt_upload_task_handler()
{
    vTaskDelay(5000/portTICK_PERIOD_MS);
    
    while(1)
    {
        xSemaphoreTake(setting_data_mutex, portMAX_DELAY);
        char buf[256] = {};
        json_gen_str_t jstr;

        json_gen_str_start(&jstr, buf, sizeof(buf), NULL, NULL);
        json_gen_start_object(&jstr);

        if(environment_data.illuminance.enable)
            json_gen_obj_set_float(&jstr, "illuminance", environment_data.illuminance.value);
        else
            json_gen_obj_set_float(&jstr, "illuminance", 0);
        
        if(environment_data.air_temperature.enable)
            json_gen_obj_set_float(&jstr, "air_temperature", environment_data.air_temperature.value);
        else
            json_gen_obj_set_float(&jstr, "air_temperature", 0);

        if(environment_data.air_humidity.enable)
            json_gen_obj_set_float(&jstr, "air_humidity", environment_data.air_humidity.value);
        else
            json_gen_obj_set_float(&jstr, "air_humidity", 0);

        if(environment_data.soil_moisture.enable)
            json_gen_obj_set_float(&jstr, "soil_moisture", environment_data.soil_moisture.value);
        else
            json_gen_obj_set_float(&jstr, "soil_moisture", 0);

        json_gen_end_object(&jstr);
        json_gen_str_end(&jstr);

        printf("JSON string: %s\n", buf);
        APP_MQTT_SendJson("/v1.6/devices/" DEVICE_LABEL, buf);

        xSemaphoreGive(setting_data_mutex);

        vTaskDelay(5000/portTICK_PERIOD_MS);
    }
}

void APP_ENV_DATA_MANAGE_Init()
{
    setting_data_mutex = xSemaphoreCreateMutex();
    setting_data_queue = xQueueCreate(8, sizeof(SETTING_DATA_t));
    xTaskCreate(mqtt_upload_task_handler, "mqtt_upload_task", 3072, NULL, 2, &mqtt_upload_task);
    xTaskCreate(set_data_task_handler, "set_data_task_handler", 2048, NULL, 10, &set_data_task);
}

void APP_ENV_DATA_MANAGE_SetValue(APP_ENV_SETTING_TYPE_t type, float value)
{
    SETTING_DATA_t setting_data = 
    {
        .type = type,
        .value = value,
    };
    xQueueSend(setting_data_queue, &setting_data, 0);
}

ENVIRONMENT_DATA_t APP_ENV_DATA_MANAGE_GetValue()
{
    xSemaphoreTake(setting_data_mutex, portMAX_DELAY);
    ENVIRONMENT_DATA_t ret = environment_data;
    xSemaphoreGive(setting_data_mutex);
    return ret;
}

void APP_ENV_DATA_MANAGE_StopSendData()
{
    vTaskSuspend(mqtt_upload_task);
}

void APP_ENV_DATA_MANAGE_ResumeSendData()
{
    vTaskResume(mqtt_upload_task);
}

void APP_ENV_DATA_MANAGE_SetEnable(APP_ENV_SETTING_TYPE_t type)
{
    // Coming soon
}

void APP_ENV_DATA_MANAGE_SetDisable(APP_ENV_SETTING_TYPE_t type)
{
    // Coming soon
}
