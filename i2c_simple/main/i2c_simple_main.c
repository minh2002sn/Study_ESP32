/* i2c - Simple example

   Simple I2C example that shows how to initialize I2C
   as well as reading and writing from and to registers for a sensor connected over I2C.

   The sensor used in this example is a MPU9250 inertial measurement unit.

   For other examples please check:
   https://github.com/espressif/esp-idf/tree/master/examples

   See README.md file to get detailed usage of this example.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "driver/i2c.h"

static const char *TAG = "i2c-simple-example";

// #define I2C_MASTER_SCL_IO           CONFIG_I2C_MASTER_SCL      /*!< GPIO number used for I2C master clock */
// #define I2C_MASTER_SDA_IO           CONFIG_I2C_MASTER_SDA      /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_SCL_IO           7      /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           8      /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              0                          /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          400000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

#define DS3231_ADDRESS			    (0x68)
#define SECOND_VALUE_ADDRESS	    (0x00)

#include "subdriver_gpio.h"
#define GPIO_BUTTON_PIN         9
#define GPIO_RGB_RED_PIN        3
#define GPIO_RGB_GREEN_PIN      4
#define GPIO_RGB_BLUE_PIN       5
#define GPIO_ORANGE_LED         18
#define GPIO_WHITE_LED          19

typedef struct{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
} __attribute__((packed)) DS3231_DATA_t;
DS3231_DATA_t ds3231_data;

static uint8_t BCD_To_DEC(uint8_t p_BCD_value){
	return ((p_BCD_value >> 4) * 10) + (p_BCD_value & 0x0F);
}

static uint8_t DEC_To_BCD(uint8_t p_DEC_value){
	return ((p_DEC_value / 10) << 4) | (p_DEC_value % 10);
}

/**
 * @brief Read a sequence of bytes from a DS3231 sensor registers
 */
static esp_err_t ds3231_register_read(DS3231_DATA_t *data)
{
    uint8_t reg_addr = SECOND_VALUE_ADDRESS;
    int ret = i2c_master_write_read_device(I2C_MASTER_NUM, DS3231_ADDRESS, &reg_addr, 1, (uint8_t *)data, 7, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    for(int i = 0; i < sizeof(DS3231_DATA_t); i++)
    {
        *((uint8_t *)data + i) = BCD_To_DEC(*((uint8_t *)data + i));
        // printf("%d\n", *((uint8_t *)data + i));
    }
    return ret;
}

/**
 * @brief Write a byte to a MPU9250 sensor register
 */
static esp_err_t ds3231_register_write_byte(DS3231_DATA_t *realtime)
{
    int ret;
    uint8_t tx_buf[8] = {};
    for(int i = 0; i < sizeof(DS3231_DATA_t); i++)
    {   
        tx_buf[i + 1] = DEC_To_BCD(*((uint8_t *)realtime + i));
    }

    ret = i2c_master_write_to_device(I2C_MASTER_NUM, DS3231_ADDRESS, tx_buf, sizeof(tx_buf), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

    return ret;
}

/**
 * @brief i2c master initialization
 */
static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

static EventGroupHandle_t alarm_event_group;
#define NORMAL_STATE                (1 << 0)
#define ALARMING_STATE              (1 << 1)

void alarm_handle_task()
{
    SUBDRIVER_GPIO_OutputInit(GPIO_RGB_GREEN_PIN);
    SUBDRIVER_GPIO_SetState(GPIO_RGB_GREEN_PIN, 0);
    alarm_event_group = xEventGroupCreate();
    while(1)
    {
        EventBits_t uxBits = xEventGroupWaitBits(alarm_event_group, 0x00FFFFFF, pdTRUE, pdFALSE, portMAX_DELAY);
        if(uxBits & NORMAL_STATE)
        {
            SUBDRIVER_GPIO_SetState(GPIO_RGB_GREEN_PIN, 0);
        }
        else if(uxBits & ALARMING_STATE)
        {
            SUBDRIVER_GPIO_SetState(GPIO_RGB_GREEN_PIN, 1);
            vTaskDelay(pdMS_TO_TICKS(200));
            SUBDRIVER_GPIO_SetState(GPIO_RGB_GREEN_PIN, 0);
            vTaskDelay(pdMS_TO_TICKS(200));
        }
    }
}

void app_main(void)
{
    xTaskCreate(alarm_handle_task, "alarm_handle_task", 2048, NULL, 13, NULL);

    DS3231_DATA_t alarm_data = 
    {
        .hour = 23,
        .minute = 1,
    };

    // DS3231_DATA_t realtime = 
    // {
    //     .hour = 22,
    //     .minute = 42,
    //     .day = 1,
    //     .date = 5,
    //     .month = 3,
    //     .year = 23
    // };

    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");

    // ds3231_register_write_byte(&realtime);

    while(1)
    {
        ds3231_register_read((&ds3231_data));

        printf("[%02d/%02d/20%02d] %02d:%02d\n", ds3231_data.date, ds3231_data.month, 
                            ds3231_data.year, ds3231_data.hour, ds3231_data.minute);

        if(ds3231_data.hour*60 + ds3231_data.minute == alarm_data.hour*60 + alarm_data.minute)
        {
            xEventGroupClearBits(alarm_event_group, NORMAL_STATE);
            xEventGroupSetBits(alarm_event_group, ALARMING_STATE);
        }
        else
        {
            xEventGroupSetBits(alarm_event_group, NORMAL_STATE);
            xEventGroupClearBits(alarm_event_group, ALARMING_STATE);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
