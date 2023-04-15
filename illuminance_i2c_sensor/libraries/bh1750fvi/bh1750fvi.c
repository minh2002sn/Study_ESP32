#include "bh1750fvi.h"
#include "driver/i2c.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define I2C_MASTER_FREQ_HZ          100000
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

#define SLAVE_ADDRESS_ADDR_LOW      0b0100011
#define SLAVE_ADDRESS_ADDR_HIGH     0b1011100
#define ONE_SHOT_H_RES_MODE         0b00100000

void BH1750_Init(BH1750_DATA_t *bh1750, uint32_t i2c_sda_pin, uint32_t i2c_scl_pin, uint8_t i2c_num, uint8_t addr_pin_state)
{
    bh1750->i2c_sda_pin = i2c_sda_pin;
    bh1750->i2c_scl_pin = i2c_scl_pin;
    bh1750->i2c_num = i2c_num;
    if(addr_pin_state)
        bh1750->address = SLAVE_ADDRESS_ADDR_HIGH;
    else
        bh1750->address = SLAVE_ADDRESS_ADDR_LOW;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = bh1750->i2c_sda_pin,
        .scl_io_num = bh1750->i2c_scl_pin,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(bh1750->i2c_num, &conf);

    i2c_driver_install(bh1750->i2c_num, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

void BH1750_OneShotRead(BH1750_DATA_t *bh1750)
{
    uint8_t tx_data[] = {ONE_SHOT_H_RES_MODE};
    i2c_master_write_to_device(bh1750->i2c_num, bh1750->address, tx_data, 1, I2C_MASTER_TIMEOUT_MS/portTICK_PERIOD_MS);
    vTaskDelay(120/portTICK_PERIOD_MS);
    uint8_t rx_data[2] = {};
    i2c_master_read_from_device(bh1750->i2c_num, bh1750->address, rx_data, 2, I2C_MASTER_TIMEOUT_MS/portTICK_PERIOD_MS);
    bh1750->illu_valuefvi = ((uint16_t)rx_data[0] << 8) | rx_data[1];
    // printf("%02X %02X %d\n", rx_data[0], rx_data[1], illu);
}
