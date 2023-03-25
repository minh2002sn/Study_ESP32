#ifndef APP_ENV_DATA_MANAGE_H
#define APP_ENV_DATA_MANAGE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint8_t enable;
    float value;
} __attribute__((packed)) SENSOR_DATA_t;

typedef struct
{
    SENSOR_DATA_t illuminance;
    SENSOR_DATA_t air_temperature;
    SENSOR_DATA_t air_humidity;
    SENSOR_DATA_t soil_moisture;
} __attribute__((packed)) ENVIRONMENT_DATA_t;

void APP_ENV_DATA_MANAGE_Init();
void APP_ENV_DATA_MANAGE_SetEnvData(ENVIRONMENT_DATA_t *env_data, bool is_change_on_or_off, bool is_change_value);
void APP_ENV_DATA_MANAGE_SetIllData(SENSOR_DATA_t *sensor_data, bool is_change_on_or_off, bool is_change_value);
void APP_ENV_DATA_MANAGE_SetAirTempData(SENSOR_DATA_t *sensor_data, bool is_change_on_or_off, bool is_change_value);
void APP_ENV_DATA_MANAGE_SetAirHumiData(SENSOR_DATA_t *sensor_data, bool is_change_on_or_off, bool is_change_value);
void APP_ENV_DATA_MANAGE_SetSoilMoistData(SENSOR_DATA_t *sensor_data, bool is_change_on_or_off, bool is_change_value);
ENVIRONMENT_DATA_t APP_ENV_DATA_MANAGE_GetEnvData();
SENSOR_DATA_t APP_ENV_DATA_MANAGE_GetIllData();
SENSOR_DATA_t APP_ENV_DATA_MANAGE_GetAirTempData();
SENSOR_DATA_t APP_ENV_DATA_MANAGE_GetAirHumiData();
SENSOR_DATA_t APP_ENV_DATA_MANAGE_GetSoilMoisData();

#endif
