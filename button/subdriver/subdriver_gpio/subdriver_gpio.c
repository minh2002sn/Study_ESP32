#include "subdriver_gpio.h"
#include "driver/gpio.h"

void SUBDRIVER_GPIO_InputInit(uint32_t pin, GPIO_RES_PULL_t gpio_res_pull)
{
    gpio_config_t io_conf = 
    {
        .pin_bit_mask   =   (1ULL << pin),
        .mode           =   GPIO_MODE_INPUT,
        .pull_up_en     =   !gpio_res_pull,
        .pull_down_en   =   gpio_res_pull,
        .intr_type      =   GPIO_INTR_DISABLE,
    };

    gpio_config(&io_conf);
}

void SUBDRIVER_GPIO_OutputInit(uint32_t pin)
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

void SUBDRIVER_GPIO_SetState(uint32_t pin, uint8_t state)
{
    gpio_set_level(pin, state);
}

uint8_t SUBDRIVER_GPIO_GetState(uint32_t pin)
{
    return gpio_get_level(pin);
}
