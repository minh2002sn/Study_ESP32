#include "output_driver.h"
#include "driver/gpio.h"

void OUTPUT_DRIVER_Init(int pin)
{
    gpio_config_t io_conf = 
    {
        .pin_bit_mask   =   (1ULL << pin),
        .mode           =   GPIO_MODE_OUTPUT,
        .pull_up_en     =   GPIO_PULLUP_DISABLE,
        .pull_down_en   =   GPIO_PULLDOWN_DISABLE,
        .intr_type      =   GPIO_INTR_DISABLE,
    };

    gpio_config(&io_conf);    
}

void OUTPUT_DRIVER_Set_State(int pin, int state)
{
    gpio_set_level(pin, state);
}

void OUTPUT_DRIVER_Toggle(int pin)
{
    gpio_set_level(pin, !gpio_get_level(pin));
}
