#include "subdriver_pwm.h"

#define MAX_CHANNEL_NUM     8

typedef struct
{
    int pin;
    ledc_channel_t channel;
} PWM_CHANNEL_t;
static PWM_CHANNEL_t pwm_channel[8];
static uint8_t num_of_channel = 0;
static ledc_timer_t selected_timer = LEDC_TIMER_0;

void SUBDRIVER_PWM_Init(int pin)
{
    if(num_of_channel == 8) return;
    for(int i = 0; i < MAX_CHANNEL_NUM; i++)
    {
        if(pwm_channel[i].pin == pin)
        {
            return;
        }
    }

    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = 
    {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .timer_num        = selected_timer,
        .duty_resolution  = LEDC_TIMER_13_BIT,
        .freq_hz          = 5000,  // Set output frequency at 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0 + num_of_channel,
        .timer_sel      = selected_timer,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = pin,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    pwm_channel[num_of_channel].pin = pin;
    pwm_channel[num_of_channel].channel = LEDC_CHANNEL_0 + num_of_channel;
    num_of_channel++;
}

void SUBDRIVER_PWM_SetValue(int pin, uint32_t duty)
{
    if(duty > 100)
        duty = 100;
    for(int i = 0; i < MAX_CHANNEL_NUM; i++)
    {
        if(pwm_channel[i].pin == pin)
        {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, pwm_channel[i].channel, 8191*duty/100.0); // (2^13 - 1)*duty/100
            ledc_update_duty(LEDC_LOW_SPEED_MODE, pwm_channel[i].channel);
            break;
        }
    }
}

#ifdef NOT_USING_TIMER_0
void SUBDRIVER_PWM_SetTimer(ledc_timer_t timer)
{
    selected_timer = timer;
}
#endif
