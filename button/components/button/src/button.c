#include <stdint.h>
#include <driver/gpio.h>
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_attr.h"
#include "button.h"


static BUTTON_CALLBACK_FUNCTION_t BUTTON_PressingCallback = NULL;
static BUTTON_CALLBACK_FUNCTION_t BUTTON_ReleasingCallback = NULL;
static int gpio_pin = 0;
static uint64_t button_timer = 0;

static void IRAM_ATTR GPIO_ISR_Handler(void* arg)
{
    if((uint32_t)arg == gpio_pin)
    {
        if(esp_timer_get_time() - button_timer < 20000)
        {
            return;
        }
        button_timer = esp_timer_get_time();

        if(gpio_get_level(gpio_pin) == 0)
        {
            if(BUTTON_PressingCallback != NULL)
            {
                BUTTON_PressingCallback(gpio_pin);
            }
        }
        else
        {
            if(BUTTON_ReleasingCallback != NULL)
            {
                BUTTON_ReleasingCallback(gpio_pin);
            }
        }
    }
}

void BUTTON_Init(int pin)
{
    gpio_pin = pin;

    gpio_config_t io_conf;
    //interrupt of rising edge
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = (1ULL<<pin);
    //set as input mode    
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    io_conf.pull_down_en = 0;

    gpio_config(&io_conf);    

    //install gpio isr service
    gpio_install_isr_service(0);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(pin, GPIO_ISR_Handler, (void*) pin);    
}

void BUTTON_Set_Callback_Function(void *PressingCallback, void *ReleasingCallback)
{
    BUTTON_PressingCallback = PressingCallback;
    BUTTON_ReleasingCallback = ReleasingCallback;
}
