#include "exti_driver.h"
#include "driver/gpio.h"

static void IRAM_ATTR GPIO_ISR_Handler(void* arg)
{
    exti_driver_callback((uint32_t)arg);
}

void exti_driver_init(uint32_t pin, ExtiEdge_t edge)
{
    gpio_config_t io_conf = 
    {
        .pin_bit_mask   = (1ULL << pin),
        .mode           = GPIO_MODE_INPUT,
        .pull_up_en     = GPIO_PULLUP_ENABLE,
        .pull_down_en   = GPIO_PULLDOWN_DISABLE,
        .intr_type      = edge,
    };
    gpio_config(&io_conf);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(pin, GPIO_ISR_Handler, (void*) pin);
}
