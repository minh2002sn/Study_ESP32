#include "app_wifi_config.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "app_http_server.h"

#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "esp_smartconfig.h"

static const char *TAG = "APP WIFI CONFIG";

PROVISION_TYPE_t provision_type = PROVISION_BY_ACCESSPOINT;

char ssid[64];
char pass[64];

#define ESPTOUCH_DONE_BIT       (1 << 0)
#define HTTP_CONFIG_DONE_BIT    (1 << 1)
#define WIFI_CONNECTED_BIT      (1 << 2)
static EventGroupHandle_t s_wifi_event_group;

static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        // ESP_LOGI(TAG, "station " (MACSTR) " join, AID=%d", MAC2STR(event->mac), event->aid);
        ESP_LOGI(TAG, "station join, AID=%d", event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        // ESP_LOGI(TAG, "station " (MACSTR) " leave, AID=%d", MAC2STR(event->mac), event->aid);
        ESP_LOGI(TAG, "station leave, AID=%d", event->aid);
    }

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        // xTaskCreate(smartconfig_example_task, "smartconfig_example_task", 4096, NULL, 3, NULL);
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
        xEventGroupClearBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    } else if (event_base == SC_EVENT && event_id == SC_EVENT_SCAN_DONE) {
        ESP_LOGI(TAG, "Scan done");
    } else if (event_base == SC_EVENT && event_id == SC_EVENT_FOUND_CHANNEL) {
        ESP_LOGI(TAG, "Found channel");
    } else if (event_base == SC_EVENT && event_id == SC_EVENT_GOT_SSID_PSWD) {
        ESP_LOGI(TAG, "Got SSID and password");

        smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;
        wifi_config_t wifi_config;
        uint8_t rvd_data[33] = { 0 };

        bzero(&wifi_config, sizeof(wifi_config_t));
        memcpy(wifi_config.sta.ssid, evt->ssid, sizeof(wifi_config.sta.ssid));
        memcpy(wifi_config.sta.password, evt->password, sizeof(wifi_config.sta.password));
        wifi_config.sta.bssid_set = evt->bssid_set;
        if (wifi_config.sta.bssid_set == true) {
            memcpy(wifi_config.sta.bssid, evt->bssid, sizeof(wifi_config.sta.bssid));
        }

        memcpy(ssid, evt->ssid, sizeof(evt->ssid));
        memcpy(pass, evt->password, sizeof(evt->password));
        ESP_LOGI(TAG, "SSID:%s", ssid);
        ESP_LOGI(TAG, "PASSWORD:%s", pass);
        if (evt->type == SC_TYPE_ESPTOUCH_V2) {
            ESP_ERROR_CHECK( esp_smartconfig_get_rvd_data(rvd_data, sizeof(rvd_data)) );
            ESP_LOGI(TAG, "RVD_DATA:");
            for (int i=0; i<33; i++) {
                printf("%02x ", rvd_data[i]);
            }
            printf("\n");
        }

        ESP_ERROR_CHECK( esp_wifi_disconnect() );
        ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
        esp_wifi_connect();
    } else if (event_base == SC_EVENT && event_id == SC_EVENT_SEND_ACK_DONE) {
        xEventGroupSetBits(s_wifi_event_group, ESPTOUCH_DONE_BIT);
    }
}

static bool is_provisioned()
{
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    
    wifi_config_t wifi_config;
    esp_wifi_get_config(WIFI_IF_STA, &wifi_config);

    if(wifi_config.sta.ssid[0] == 0)
    {
        ESP_LOGI(TAG, "No wifi info");
        return false;
    }
    ESP_LOGI(TAG, "Wifi info already");
    return true;

}

static void http_post_wifi_info_callback(char *buf, uint8_t length)
{
    
    if(buf != NULL)
    {
        strcpy(ssid, strtok(buf, "\n"));
        strcpy(pass, strtok(NULL, "\n"));
    }
    else
    {
        return;
    }

    xEventGroupSetBits(s_wifi_event_group, HTTP_CONFIG_DONE_BIT);
}

static void start_access_point(char *ssid, char *pass)
{
    esp_netif_create_default_wifi_ap();

    wifi_config_t wifi_config = {
        .ap = {
            .ssid_len = strlen(ssid),
            .channel = 1,
            .max_connection = 4,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK,
            .pmf_cfg = {
                    .required = false,
            },
        },
    };

    strcpy((char *)wifi_config.ap.ssid, ssid);
    strcpy((char *)wifi_config.ap.password, pass);

    if(pass[0] == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }
    else
    {
        wifi_config.ap.authmode = WIFI_AUTH_WPA2_PSK;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

}

void APP_WIFI_CONFIG_Handle(PROVISION_TYPE_t type)
{
    provision_type = type;
    s_wifi_event_group = xEventGroupCreate();
    // ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    ESP_ERROR_CHECK( esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL) );
    ESP_ERROR_CHECK( esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL) );
    ESP_ERROR_CHECK( esp_event_handler_register(SC_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL) );

    if(is_provisioned())
    {
        ESP_ERROR_CHECK(esp_wifi_start());
        // xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
    else
    {
        switch(provision_type)
        {
            case PROVISION_BY_ACCESSPOINT:
                start_access_point("ESP32", "123456789");
                APP_HTTP_SERVER_PostSetCallback(http_post_wifi_info_callback);
                APP_HTTP_SERVER_Start();
                xEventGroupWaitBits(s_wifi_event_group, HTTP_CONFIG_DONE_BIT, true, false, portMAX_DELAY);
                APP_HTTP_SERVER_Stop();
                esp_wifi_disconnect();
                esp_wifi_stop();
                wifi_config_t wifi_config = {
                    .sta = {
                        .threshold.authmode = WIFI_AUTH_WPA2_PSK,
                        .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
                        .pmf_cfg = 
                        {
                            .capable = true,
                            .required = false
                        }
                    },
                };
                strcpy((char *)wifi_config.sta.ssid, ssid);
                strcpy((char *)wifi_config.sta.password, pass);
                ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
                ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
                ESP_ERROR_CHECK(esp_wifi_start());
                break;

            case PROVISION_BY_SMARTCONFIG:
                ESP_ERROR_CHECK(esp_wifi_start());
                ESP_ERROR_CHECK(esp_smartconfig_set_type(SC_TYPE_ESPTOUCH));
                smartconfig_start_config_t cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
                ESP_ERROR_CHECK(esp_smartconfig_start(&cfg));
                xEventGroupWaitBits(s_wifi_event_group, ESPTOUCH_DONE_BIT, true, false, portMAX_DELAY);
                esp_smartconfig_stop();
                break;

            default:
                break;
        }
    }

    xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT, false, false, portMAX_DELAY);
    ESP_LOGI(TAG, "wifi connected");

}

void APP_WIFI_CONFIG_Init()
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
}
