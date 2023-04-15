#include "app_env_data_manage.h"

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#include "json_generator.h"
#include "json_parser.h"

#include "app_mqtt.h"

#define DEVICE_LABEL    "test_mqtt"

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
    xTaskCreate(mqtt_upload_task_handler, "mqtt_upload_task", 3072, NULL, 10, &mqtt_upload_task);
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

void APP_ENV_DATA_MANAGE_SetEnable(APP_ENV_SETTING_TYPE_t type)
{
    // Coming soon
}

void APP_ENV_DATA_MANAGE_SetDisable(APP_ENV_SETTING_TYPE_t type)
{
    // Coming soon
}

#if 0
void APP_ENV_DATA_MANAGE_SetEnvData(ENVIRONMENT_DATA_t *env_data, bool is_change_on_or_off, bool is_change_value, uint64_t delay_tick)
{
    xSemaphoreTake(env_data_mutex, delay_tick);
    if(is_change_on_or_off)
    {
        environment_data.illuminance.enable = env_data->illuminance.enable;
        environment_data.air_temperature.enable = env_data->air_temperature.enable;
        environment_data.air_humidity.enable = env_data->air_humidity.enable;
        environment_data.soil_moisture.enable = env_data->soil_moisture.enable;
    }

    if(is_change_value)
    {
        environment_data.illuminance.value = env_data->illuminance.value;
        environment_data.air_temperature.value = env_data->air_temperature.value;
        environment_data.air_humidity.value = env_data->air_humidity.value;
        environment_data.soil_moisture.value = env_data->soil_moisture.value;
    }
    xSemaphoreGive(env_data_mutex);
}

void APP_ENV_DATA_MANAGE_SetIllData(SENSOR_DATA_t *sensor_data, bool is_change_on_or_off, bool is_change_value, uint64_t delay_tick)
{
    xSemaphoreTake(env_data_mutex, delay_tick);
    if(is_change_on_or_off)
    {
        environment_data.illuminance.enable = sensor_data->enable;
    }

    if(is_change_value)
    {
        environment_data.illuminance.value = sensor_data->value;
    }
    xSemaphoreGive(env_data_mutex);
}

void APP_ENV_DATA_MANAGE_SetAirTempData(SENSOR_DATA_t *sensor_data, bool is_change_on_or_off, bool is_change_value, uint64_t delay_tick)
{
    xSemaphoreTake(env_data_mutex, delay_tick);
    if(is_change_on_or_off)
    {
        environment_data.air_temperature.enable = sensor_data->enable;
    }

    if(is_change_value)
    {
        environment_data.air_temperature.value = sensor_data->value;
    }
    xSemaphoreGive(env_data_mutex);
}

void APP_ENV_DATA_MANAGE_SetAirHumiData(SENSOR_DATA_t *sensor_data, bool is_change_on_or_off, bool is_change_value, uint64_t delay_tick)
{
    xSemaphoreTake(env_data_mutex, delay_tick);
    if(is_change_on_or_off)
    {
        environment_data.air_humidity.enable = sensor_data->enable;
    }

    if(is_change_value)
    {
        environment_data.air_humidity.value = sensor_data->value;
    }
    xSemaphoreGive(env_data_mutex);
}

void APP_ENV_DATA_MANAGE_SetSoilMoistData(SENSOR_DATA_t *sensor_data, bool is_change_on_or_off, bool is_change_value, uint64_t delay_tick)
{
    xSemaphoreTake(env_data_mutex, delay_tick);
    if(is_change_on_or_off)
    {
        environment_data.soil_moisture.enable = sensor_data->enable;
    }

    if(is_change_value)
    {
        environment_data.soil_moisture.value = sensor_data->value;
    }
    xSemaphoreGive(env_data_mutex);
}

ENVIRONMENT_DATA_t APP_ENV_DATA_MANAGE_GetEnvData(uint64_t delay_tick)
{
    xSemaphoreTake(env_data_mutex, delay_tick);
    xSemaphoreGive(env_data_mutex);
    return environment_data;
}

SENSOR_DATA_t APP_ENV_DATA_MANAGE_GetIllData(uint64_t delay_tick)
{
    xSemaphoreTake(env_data_mutex, delay_tick);
    xSemaphoreGive(env_data_mutex);
    return environment_data.illuminance;
}

SENSOR_DATA_t APP_ENV_DATA_MANAGE_GetAirTempData(uint64_t delay_tick)
{
    xSemaphoreTake(env_data_mutex, delay_tick);
    xSemaphoreGive(env_data_mutex);
    return environment_data.air_temperature;
}


SENSOR_DATA_t APP_ENV_DATA_MANAGE_GetAirHumiData(uint64_t delay_tick)
{
    xSemaphoreTake(env_data_mutex, delay_tick);
    xSemaphoreGive(env_data_mutex);
    return environment_data.air_humidity;
}

SENSOR_DATA_t APP_ENV_DATA_MANAGE_GetSoilMoisData(uint64_t delay_tick)
{
    xSemaphoreTake(env_data_mutex, delay_tick);
    xSemaphoreGive(env_data_mutex);
    return environment_data.soil_moisture;
}

#endif

