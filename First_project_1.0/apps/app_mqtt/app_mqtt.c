/* MQTT Mutual Authentication Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <inttypes.h>

#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

#include "json_parser.h"
#include "app_output.h"

static const char *TAG = "APP_MQTT";

#define TOKEN                   "BBFF-lTB4HGgqcojJXY7yhSbrgVsTl3RUbG"
#define PRE_TOPIC               "/v1.6/devices/iot_garden/"

#define SUB_TOPIC_REF_ILLU      "ref-illuminance"
#define SUB_TOPIC_REF_AIR_TEMP  "ref-air-temperature"
#define SUB_TOPIC_REF_AIR_HUMI  "ref-air-humidity"
#define SUB_TOPIC_REF_SOIL_MOIS "ref-soil-moisture"
#define SUB_TOPIC_ENB_ON_OFF    "enable-on-off-output"
#define SUB_TOPIC_ENB_PWM       "enable-pwm-output"

static char *sub_topic[] = 
{
    SUB_TOPIC_REF_ILLU,
    SUB_TOPIC_REF_AIR_TEMP,
    SUB_TOPIC_REF_AIR_HUMI,
    SUB_TOPIC_REF_SOIL_MOIS,
    SUB_TOPIC_ENB_ON_OFF,
    SUB_TOPIC_ENB_PWM
};

extern const uint8_t client_cert_pem_start[] asm("_binary_client_crt_start");
extern const uint8_t client_cert_pem_end[] asm("_binary_client_crt_end");

static esp_mqtt_client_handle_t client;

static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

static void data_handle(char *topic, uint32_t topic_len, char *data, uint32_t data_len)
{
    for(int i = 0; i < 6; i++)
    {
        if(strstr(topic, sub_topic[i]) != NULL)
        {
            jparse_ctx_t jctx;
            json_parse_start(&jctx, data, data_len);
            float value = 0.0;
            json_obj_get_float(&jctx, "value", &value);
            APP_OUTPUT_SetRefValue(i, value);
            json_parse_end(&jctx);
            return;
        }
    }
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%ld", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        // msg_id = esp_mqtt_client_subscribe(client, "/v1.6/devices/test_mqtt/ref-air-temperature", 1);
        // msg_id = esp_mqtt_client_subscribe(client, "/v1.6/devices/test_mqtt/ref-air-humidity", 1);
        // msg_id = esp_mqtt_client_subscribe(client, "/v1.6/devices/test_mqtt/ref-soil-moisture", 1);
        // msg_id = esp_mqtt_client_subscribe(client, "/v1.6/devices/test_mqtt/ref-illuminance", 1);
        // msg_id = esp_mqtt_client_subscribe(client, "/v1.6/devices/test_mqtt/enable-on-off-output", 1);
        // msg_id = esp_mqtt_client_subscribe(client, "/v1.6/devices/test_mqtt/enable-pwm-output", 1);
        
        msg_id = esp_mqtt_client_subscribe(client, PRE_TOPIC SUB_TOPIC_REF_ILLU, 1);
        msg_id = esp_mqtt_client_subscribe(client, PRE_TOPIC SUB_TOPIC_REF_AIR_TEMP, 1);
        msg_id = esp_mqtt_client_subscribe(client, PRE_TOPIC SUB_TOPIC_REF_AIR_HUMI, 1);
        msg_id = esp_mqtt_client_subscribe(client, PRE_TOPIC SUB_TOPIC_REF_SOIL_MOIS, 1);
        msg_id = esp_mqtt_client_subscribe(client, PRE_TOPIC SUB_TOPIC_ENB_ON_OFF, 1);
        msg_id = esp_mqtt_client_subscribe(client, PRE_TOPIC SUB_TOPIC_ENB_PWM, 1);
        
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        // msg_id = esp_mqtt_client_publish(client, "/deltacross/qos0", "data", 0, 0, 0);
        // ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        data_handle(event->topic, event->topic_len, event->data, event->data_len);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

void APP_MQTT_Start(void)
{
    const esp_mqtt_client_config_t mqtt_cfg = 
    {
        .broker.address.uri = "mqtts://industrial.api.ubidots.com:8883",
        .broker.verification.certificate = (const char *)client_cert_pem_start,
        .credentials.username = TOKEN,
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

void APP_MQTT_SendJson(char *topic, char *json_data)
{
    int msg_id = esp_mqtt_client_publish(client, topic, json_data, 0, 1, 0);
    ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
}
