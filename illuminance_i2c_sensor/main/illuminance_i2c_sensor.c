#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "bh1750fvi.h" 

#define I2C_MASTER_SDA_IO           4      /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_SCL_IO           5      /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_NUM              0                          /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          100000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

#define SLAVE_ADDRESS               0b1011100
#define ONE_SHOT_H_RES_MODE         0b00100000

void app_main(void)
{
    BH1750_DATA_t bh1750; 
    BH1750_Init(&bh1750, 4, 5, 0, 0);

    while(1)
    {
        BH1750_OneShotRead(&bh1750);
        printf("%d\n", bh1750.illu_value);

        vTaskDelay(1000/portTICK_PERIOD_MS);
    }

}
