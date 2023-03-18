#include <stdio.h>
#include <stdint.h>

#include "lwip/sockets.h"

#include "esp_log.h"

#include "json_generator.h"
#include "json_parser.h"

#include "app_nvs.h"
#include "app_mqtt.h"
#include "app_wifi_config.h"

#define KEY   "restart_cnt"

static const char *TAG = "APP_MAIN";

typedef struct
{
  char *buf[256];
  size_t offset;
} JSON_DATA_t;

void json_gen();

void app_main(void)
{
  ESP_LOGI(TAG, "[APP] Startup..");
  ESP_LOGI(TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size());
  ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

  APP_NVS_Init();

  // uint8_t restart_cnt = 0;
  // APP_NVS_GetValueU8(KEY, &restart_cnt);
  // restart_cnt++;
  // APP_NVS_SetValueU8(KEY, restart_cnt);

  // char str[20];
  // APP_NVS_GetValueStr("String", str);
  // APP_NVS_SetValueStr("String", "Hello");

  // json_gen();


  APP_WIFI_CONFIG_Init();
  APP_WIFI_CONFIG_Handle(PROVISION_BY_SMARTCONFIG);
  APP_MQTT_Start();

}

// void json_gen()
// {
//   JSON_DATA_t result = {};
//   char buf[32];
//   json_gen_str_t jstr;

//   json_gen_str_start(&jstr, buf, sizeof(buf), NULL, NULL);
//   json_gen_start_object(&jstr);
//   json_gen_obj_set_string(&jstr, "Name", "Minh");
//   json_gen_obj_set_string(&jstr, "Course", "K20");
//   json_gen_obj_set_bool(&jstr, "Is_student", true);
//   json_gen_obj_set_float(&jstr, "Score", 7.6);
//   json_gen_end_object(&jstr);
//   json_gen_str_end(&jstr);

//   printf("JSON string: %s\noffset: %d", result.buf, result.offset);

// }
