#include "spi_adc.h"

#include "driver/spi_master.h"

#define PIN_NUM_MISO 2
#define PIN_NUM_MOSI 7
#define PIN_NUM_CLK  6
#define PIN_NUM_CS   10

static spi_device_handle_t dev_handler;
uint8_t is_init_spi = 0;

void SPI_ADC_Init(SPI_ADC_DATA_t *spi_adc, uint8_t adc_channel)
{
    spi_adc->adc_channel = adc_channel;
    if(is_init_spi == 0)
    {
        spi_bus_config_t bus_cfg = 
        {
            .miso_io_num = PIN_NUM_MISO,
            .mosi_io_num = PIN_NUM_MOSI,
            .sclk_io_num = PIN_NUM_CLK,
            .quadwp_io_num = -1,  // no use
            .quadhd_io_num = -1,  // no use
            // .max_transfer_sz = PARALLEL_LINES*320*2+8
        };
        spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO);

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
        is_init_spi = 1;
    }
}

esp_err_t SPI_ADC_ReadValue(SPI_ADC_DATA_t *spi_adc)
{
    spi_transaction_t trans_data =
    {
        .cmd = 0b001100000 | (spi_adc->adc_channel << 2),
        .rx_buffer = NULL,
        .length = 12,
        .rxlength = 12,
        .flags = SPI_TRANS_USE_RXDATA,

    };
    esp_err_t ret = spi_device_polling_transmit(dev_handler, &trans_data);
    if(ret == ESP_OK)
    {
        spi_adc->raw_value = ((uint16_t)trans_data.rx_data[0] << 4) | ((uint16_t)trans_data.rx_data[1] >> 4);
        printf("Status[%d]: %04X - %d\n", ret, spi_adc->raw_value, spi_adc->raw_value);
    }
    return ret;

}