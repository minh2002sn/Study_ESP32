#include <stdio.h>
#include <stdint.h>

#include <inttypes.h>

#include "lwip/sockets.h"

#include "esp_log.h"

#include "app_nvs.h"
#include "app_output.h"
#include "app_mqtt.h"
#include "app_wifi_config.h"
#include "app_env_data_manage.h"
#include "app_dht.h"
#include "app_illuminance.h"
#include "app_soil_moisture.h"

static const char *TAG = "APP_MAIN";

void app_main(void)
{
  ESP_LOGI(TAG, "[APP] Startup..");
  ESP_LOGI(TAG, "[APP] Free memory: %lu bytes", esp_get_free_heap_size());
  ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

  APP_NVS_Init();

  APP_WIFI_CONFIG_Init();
  APP_WIFI_CONFIG_Handle(PROVISION_BY_SMARTCONFIG);

  APP_ENV_DATA_MANAGE_Init();
  APP_DHT_Init();
  APP_ILLUMINANCE_Init();
  APP_SOIL_MOISTURE_Init();

  APP_OUTPUT_Init();
  APP_MQTT_Start();
}
