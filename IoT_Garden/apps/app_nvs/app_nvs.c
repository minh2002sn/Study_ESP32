#include "app_nvs.h"

#include <stdio.h>
#include <inttypes.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"

#define TAG                     "APP_NVS"

#define USER_NVS_NAMESPACE      "app_nvs_data"

static nvs_handle_t my_handle;

void APP_NVS_Init()
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );
}

void APP_NVS_SetValueU8(char *key, uint8_t value)
{
    esp_err_t err;
    err = nvs_open(USER_NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS open fail");
    } else {
        err = nvs_set_u8(my_handle, key, value);
        if(err == ESP_OK)
            ESP_LOGI(TAG, "Write success");
        else
            ESP_LOGE(TAG, "Write fail");

        err = nvs_commit(my_handle);
        if(err == ESP_OK)
            ESP_LOGI(TAG, "Commit success");
        else
            ESP_LOGE(TAG, "Commit fail");

        nvs_close(my_handle);
    }
}

void APP_NVS_GetValueU8(char *key, uint8_t *value)
{
    esp_err_t err;
    err = nvs_open(USER_NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS open fail");
    } else {
        err = nvs_get_u8(my_handle, key, value);
        switch (err) {
            case ESP_OK:
                ESP_LOGI(TAG, "Done\n%s = %u", key, *value);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                ESP_LOGE(TAG, "The value is not initialized yet!");
                break;
            default :
                ESP_LOGE(TAG, "Error (%s) reading!", esp_err_to_name(err));
        }

        nvs_close(my_handle);
    }
}

void APP_NVS_SetValueU16(char *key, uint16_t value)
{
    esp_err_t err;
    err = nvs_open(USER_NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS open fail");
    } else {
        err = nvs_set_u16(my_handle, key, value);
        if(err == ESP_OK)
            ESP_LOGI(TAG, "Write success");
        else
            ESP_LOGE(TAG, "Write fail");

        err = nvs_commit(my_handle);
        if(err == ESP_OK)
            ESP_LOGI(TAG, "Commit success");
        else
            ESP_LOGE(TAG, "Commit fail");

        nvs_close(my_handle);
    }
}

void APP_NVS_GetValueU16(char *key, uint16_t *value)
{
    esp_err_t err;
    err = nvs_open(USER_NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS open fail");
    } else {
        err = nvs_get_u16(my_handle, key, value);
        switch (err) {
            case ESP_OK:
                ESP_LOGI(TAG, "Done\n%s = %u", key, *value);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                ESP_LOGE(TAG, "The value is not initialized yet!");
                break;
            default :
                ESP_LOGE(TAG, "Error (%s) reading!", esp_err_to_name(err));
        }

        nvs_close(my_handle);
    }
}

void APP_NVS_SetValueU32(char *key, uint32_t value)
{
    esp_err_t err;
    err = nvs_open(USER_NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS open fail");
    } else {
        err = nvs_set_u32(my_handle, key, value);
        if(err == ESP_OK)
            ESP_LOGI(TAG, "Write success");
        else
            ESP_LOGE(TAG, "Write fail");

        err = nvs_commit(my_handle);
        if(err == ESP_OK)
            ESP_LOGI(TAG, "Commit success");
        else
            ESP_LOGE(TAG, "Commit fail");

        nvs_close(my_handle);
    }
}

void APP_NVS_GetValueU32(char *key, uint32_t *value)
{
    esp_err_t err;
    err = nvs_open(USER_NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS open fail");
    } else {
        err = nvs_get_u32(my_handle, key, value);
        switch (err) {
            case ESP_OK:
                ESP_LOGI(TAG, "Done\n%s = %lu", key, *value);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                ESP_LOGE(TAG, "The value is not initialized yet!");
                break;
            default :
                ESP_LOGE(TAG, "Error (%s) reading!", esp_err_to_name(err));
        }

        nvs_close(my_handle);
    }
}

void APP_NVS_SetValueU64(char *key, uint64_t value)
{
    esp_err_t err;
    err = nvs_open(USER_NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS open fail");
    } else {
        err = nvs_set_u64(my_handle, key, value);
        if(err == ESP_OK)
            ESP_LOGI(TAG, "Write success");
        else
            ESP_LOGE(TAG, "Write fail");

        err = nvs_commit(my_handle);
        if(err == ESP_OK)
            ESP_LOGI(TAG, "Commit success");
        else
            ESP_LOGE(TAG, "Commit fail");

        nvs_close(my_handle);
    }
}

void APP_NVS_GetValueU64(char *key, uint64_t *value)
{
    esp_err_t err;
    err = nvs_open(USER_NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS open fail");
    } else {
        err = nvs_get_u64(my_handle, key, value);
        switch (err) {
            case ESP_OK:
                ESP_LOGI(TAG, "Done\n%s = %llu", key, *value);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                ESP_LOGE(TAG, "The value is not initialized yet!");
                break;
            default :
                ESP_LOGE(TAG, "Error (%s) reading!", esp_err_to_name(err));
        }

        nvs_close(my_handle);
    }
}

void APP_NVS_SetValueStr(char *key, char *value)
{
    esp_err_t err;
    err = nvs_open(USER_NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS open fail");
    } else {
        err = nvs_set_str(my_handle, key, value);
        if(err == ESP_OK)
            ESP_LOGI(TAG, "Write success");
        else
            ESP_LOGE(TAG, "Write fail");

        err = nvs_commit(my_handle);
        if(err == ESP_OK)
            ESP_LOGI(TAG, "Commit success");
        else
            ESP_LOGE(TAG, "Commit fail");

        nvs_close(my_handle);
    }
}

void APP_NVS_GetValueStr(char *key, char *value)
{
    esp_err_t err;
    err = nvs_open(USER_NVS_NAMESPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "NVS open fail");
    } else {
        size_t required_size = 0;
        // Read the size of memory space required for string
        err = nvs_get_str(my_handle, key, NULL, &required_size);
        // Read string
        err = nvs_get_str(my_handle, key, value, &required_size);
        switch (err) {
            case ESP_OK:
                ESP_LOGI(TAG, "Done\n%s = %s", key, value);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                ESP_LOGE(TAG, "The value is not initialized yet!");
                break;
            default :
                ESP_LOGE(TAG, "Error (%s) reading!", esp_err_to_name(err));
        }

        nvs_close(my_handle);
    }
}
