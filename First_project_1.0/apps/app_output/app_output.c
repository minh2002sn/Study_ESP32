#include "app_output.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#include "esp_err.h"
#include "esp_log.h"

#include "app_env_data_manage.h"
#include "subdriver_gpio.h"
#include "subdriver_pwm.h"

#define TAG                     "APP_OUTPUT"

#define GET_TICK                (xTaskGetTickCount() * portTICK_PERIOD_MS)
#define CHANGE_TIME             5000

#define ON_OFF_OUTPUT_PIN       3
#define PWM_0_OUTPUT_PIN        19
#define PWM_1_OUTPUT_PIN        18

typedef struct
{
    APP_OUTPUT_SETTING_REF_TYPE_t type;
    float value;
} __attribute__((packed)) SETTING_DATA_t;

typedef struct
{
    uint16_t air_temp;
    uint16_t air_humi;
    uint16_t soil_mois;
    uint16_t illu;
    uint16_t enable_on_of;
    uint16_t eneble_pwm;
} REF_DATA_t;
static REF_DATA_t ref_data;

static TaskHandle_t output_handle_task;
static TaskHandle_t setting_data_task;
static QueueHandle_t setting_data_queue;
static SemaphoreHandle_t setting_data_mutex;

static uint64_t change_state_time = 0;
static uint8_t is_changing_state = 0;
static uint8_t output_state = 0;

static void output_handle_task_handler()
{
    SUBDRIVER_GPIO_OutputInit(ON_OFF_OUTPUT_PIN);
    SUBDRIVER_GPIO_SetState(ON_OFF_OUTPUT_PIN, 0);

    SUBDRIVER_PWM_Init(PWM_0_OUTPUT_PIN);
    SUBDRIVER_PWM_Init(PWM_1_OUTPUT_PIN);
    SUBDRIVER_PWM_SetValue(PWM_0_OUTPUT_PIN, 0);
    SUBDRIVER_PWM_SetValue(PWM_1_OUTPUT_PIN, 0);

    while(1)
    {
        ENVIRONMENT_DATA_t env_data = APP_ENV_DATA_MANAGE_GetValue();
        if(is_changing_state == 0)
        {
            if(env_data.air_temperature.value > ref_data.air_temp ||
                env_data.air_humidity.value > ref_data.air_humi ||
                env_data.soil_moisture.value > ref_data.soil_mois ||
                env_data.illuminance.value > ref_data.illu)
            {
                output_state = 1;
                // is_changing_state = 1;
                // change_state_time = GET_TICK;
            }
            else
            {
                output_state = 0;
                // is_changing_state = 1;
                // change_state_time = GET_TICK;
            }
        }

        // if(is_changing_state == 1)
        // {
        //     if(GET_TICK - change_state_time > CHANGE_TIME)
        //         output_state = !output_state;
        // }

        if(output_state == 1)
        {
            // printf("state = 1\n");
            if(ref_data.enable_on_of == 1)
            {
                SUBDRIVER_GPIO_SetState(ON_OFF_OUTPUT_PIN, 1);
            }
            else
            {
                SUBDRIVER_GPIO_SetState(ON_OFF_OUTPUT_PIN, 0);
            }

            if(ref_data.eneble_pwm == 1)
            {
                SUBDRIVER_PWM_SetValue(PWM_0_OUTPUT_PIN, 50);
                SUBDRIVER_PWM_SetValue(PWM_1_OUTPUT_PIN, 0);
            }
            else
            {
                SUBDRIVER_PWM_SetValue(PWM_0_OUTPUT_PIN, 0);
                SUBDRIVER_PWM_SetValue(PWM_1_OUTPUT_PIN, 0);
            }
        }
        else
        {
            // printf("state = 0\n");
            SUBDRIVER_GPIO_SetState(ON_OFF_OUTPUT_PIN, 0);

            if(ref_data.eneble_pwm == 1)
            {
                SUBDRIVER_PWM_SetValue(PWM_0_OUTPUT_PIN, 0);
                SUBDRIVER_PWM_SetValue(PWM_1_OUTPUT_PIN, 50);
            }
            else
            {
                SUBDRIVER_PWM_SetValue(PWM_0_OUTPUT_PIN, 0);
                SUBDRIVER_PWM_SetValue(PWM_1_OUTPUT_PIN, 0);
            }
        }
    }
}

static void setting_data_task_handler()
{
    SETTING_DATA_t setting_data = {};
    while(1)
    {
        if(xQueueReceive(setting_data_queue, &setting_data, portMAX_DELAY))
        {
            xSemaphoreTake(setting_data_mutex, portMAX_DELAY);
            *((uint16_t *)&ref_data + setting_data.type) = setting_data.value;
            printf("[%d] - %f\n", setting_data.type, setting_data.value);
            xSemaphoreGive(setting_data_mutex);
        }
    }
}

void APP_OUTPUT_Init()
{
    setting_data_mutex = xSemaphoreCreateMutex();
    setting_data_queue = xQueueCreate(8, sizeof(SETTING_DATA_t));
    xTaskCreate(output_handle_task_handler, "output_handle_task", 2048, NULL, tskIDLE_PRIORITY, &output_handle_task);
    xTaskCreate(setting_data_task_handler, "setting_data_task", 2048, NULL, tskIDLE_PRIORITY, &setting_data_task);
}

void APP_OUTPUT_SetRefValue(APP_OUTPUT_SETTING_REF_TYPE_t type, float value)
{
    SETTING_DATA_t setting_data = 
    {
        .type = type,
        .value = value,
    };
    xQueueSend(setting_data_queue, &setting_data, 0);
}
