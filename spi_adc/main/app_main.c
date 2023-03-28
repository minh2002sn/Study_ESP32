#include <stdio.h>
#include <stdint.h>

#include "driver/spi_master.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define PIN_NUM_MISO 2
#define PIN_NUM_MOSI 7
#define PIN_NUM_CLK  6
#define PIN_NUM_CS   10

void app_main(void)
{
    uint8_t tx_data[4] = {0x06, 0x40, 0x00, 0x00};
    
    spi_bus_config_t bus_cfg = 
    {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,  // no use
        .quadhd_io_num = -1,  // no use
        // .max_transfer_sz = PARALLEL_LINES*320*2+8
    };
    spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_DISABLED);

    spi_device_handle_t dev_handler;
    spi_device_interface_config_t dev_cfg =
    {
        .clock_speed_hz = 1000000,           //Clock out at 1 MHz
        .mode = 0,                                //SPI mode 0
        .spics_io_num = PIN_NUM_CS,                       //CS pin
        .queue_size = 4,                          //We want to be able to queue 7 transactions at a time
        .command_bits = 7,
        .flags = SPI_TRANS_MODE_OCT,
    };
    spi_bus_add_device(SPI2_HOST, &dev_cfg, &dev_handler);



    while(1)
    {
        uint8_t rx_data[3] = {};
        spi_transaction_t trans_data =
        {
            .cmd = 0b1100100,
            .rx_buffer = NULL,
            // .tx_buffer = tx_data,
            .length = 12,
            .rxlength = 12,
            .flags = SPI_TRANS_USE_RXDATA,

        };

        esp_err_t ret = spi_device_polling_transmit(dev_handler, &trans_data);
        // printf("%d: 0x%02X 0x%02X\n", ret, rx_data[1], rx_data[0]);
        printf("Status[%d]: %08lX\n", ret, *((uint32_t *)trans_data.rx_data));
        printf("Status[%d]: %08lX\n\n", ret, *((uint32_t *)rx_data));
        // printf("%d: 0x%02X 0x%02X\n", ret, rx_data[1], trx_data[0]);
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }

}
