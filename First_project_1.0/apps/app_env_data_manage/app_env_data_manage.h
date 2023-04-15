#ifndef APP_ENV_DATA_MANAGE_H
#define APP_ENV_DATA_MANAGE_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    APP_ENV_SET_ILLUMINANCE,
    APP_ENV_SET_AIR_TEMPERATURE,
    APP_ENV_SET_AIR_HUMIDITY,
    APP_ENV_SET_SOIL_MOISTURE,
} APP_ENV_SETTING_TYPE_t;

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

void APP_ENV_DATA_MANAGE_SetValue(APP_ENV_SETTING_TYPE_t type, float value);
void APP_ENV_DATA_MANAGE_SetEnable(APP_ENV_SETTING_TYPE_t type);
void APP_ENV_DATA_MANAGE_SetDisable(APP_ENV_SETTING_TYPE_t type);

// void APP_ENV_DATA_MANAGE_SetEnvData(ENVIRONMENT_DATA_t *env_data, bool is_change_on_or_off, bool is_change_value, uint64_t delay_tick);
// void APP_ENV_DATA_MANAGE_SetIllData(SENSOR_DATA_t *sensor_data, bool is_change_on_or_off, bool is_change_value, uint64_t delay_tick);
// void APP_ENV_DATA_MANAGE_SetAirTempData(SENSOR_DATA_t *sensor_data, bool is_change_on_or_off, bool is_change_value, uint64_t delay_tick);
// void APP_ENV_DATA_MANAGE_SetAirHumiData(SENSOR_DATA_t *sensor_data, bool is_change_on_or_off, bool is_change_value, uint64_t delay_tick);
// void APP_ENV_DATA_MANAGE_SetSoilMoistData(SENSOR_DATA_t *sensor_data, bool is_change_on_or_off, bool is_change_value, uint64_t delay_tick);
// ENVIRONMENT_DATA_t APP_ENV_DATA_MANAGE_GetEnvData(uint64_t delay_tick);
// SENSOR_DATA_t APP_ENV_DATA_MANAGE_GetIllData(uint64_t delay_tick);
// SENSOR_DATA_t APP_ENV_DATA_MANAGE_GetAirTempData(uint64_t delay_tick);
// SENSOR_DATA_t APP_ENV_DATA_MANAGE_GetAirHumiData(uint64_t delay_tick);
// SENSOR_DATA_t APP_ENV_DATA_MANAGE_GetSoilMoisData(uint64_t delay_tick);

#endif
