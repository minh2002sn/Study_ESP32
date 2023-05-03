#ifndef APP_ENV_DATA_MANAGE_H
#define APP_ENV_DATA_MANAGE_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    APP_ENV_SET_AIR_TEMPERATURE,
    APP_ENV_SET_AIR_HUMIDITY,
    APP_ENV_SET_SOIL_MOISTURE,
    APP_ENV_SET_ILLUMINANCE,
} APP_ENV_SETTING_TYPE_t;

typedef struct
{
    uint8_t enable;
    float value;
} __attribute__((packed)) SENSOR_DATA_t;

typedef struct
{
    SENSOR_DATA_t air_temperature;
    SENSOR_DATA_t air_humidity;
    SENSOR_DATA_t soil_moisture;
    SENSOR_DATA_t illuminance;
} __attribute__((packed)) ENVIRONMENT_DATA_t;

void APP_ENV_DATA_MANAGE_Init();

void APP_ENV_DATA_MANAGE_SetValue(APP_ENV_SETTING_TYPE_t type, float value);
ENVIRONMENT_DATA_t APP_ENV_DATA_MANAGE_GetValue();
void APP_ENV_DATA_MANAGE_SetEnable(APP_ENV_SETTING_TYPE_t type);
void APP_ENV_DATA_MANAGE_SetDisable(APP_ENV_SETTING_TYPE_t type);
void APP_ENV_DATA_MANAGE_StopSendData();
void APP_ENV_DATA_MANAGE_ResumeSendData();

#endif
