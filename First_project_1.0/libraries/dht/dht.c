#include "dht.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "rom/ets_sys.h"

#include "subdriver_gpio.h"

#define GET_TICK            (xTaskGetTickCount() * portTICK_PERIOD_MS)

#define DHT11_STARTTIME 18	// in ms
#define DHT22_STARTTIME 12	// in ms

#define TIME_OUT		10

static void DHT_Set_Pin_Output(DHT_DATA_t* DHT){
    SUBDRIVER_GPIO_OutputInit(DHT->Pin);
}

static void DHT_Set_Pin_Input(DHT_DATA_t* DHT){
    SUBDRIVER_GPIO_InputInit(DHT->Pin, GPIO_NOPULL);
}

static void DHT_Start(DHT_DATA_t* DHT){
	DHT_Set_Pin_Output(DHT);
	SUBDRIVER_GPIO_SetState(DHT->Pin, 0);
    ets_delay_us(((DHT->sensor_type == DHT11) ? DHT11_STARTTIME : DHT22_STARTTIME)*1000);
	// vTaskDelay(((DHT->sensor_type == DHT11) ? DHT11_STARTTIME : DHT22_STARTTIME)/portTICK_PERIOD_MS);
    SUBDRIVER_GPIO_SetState(DHT->Pin, 1);
}

static uint8_t DHT_Check_Response(DHT_DATA_t* DHT){
	DHT_Set_Pin_Input(DHT);
	ets_delay_us(40);
	if(!SUBDRIVER_GPIO_GetState(DHT->Pin))
    {
		ets_delay_us(80);
		if(!SUBDRIVER_GPIO_GetState(DHT->Pin))
			return DHT_FAIL;
	}
    else
		return DHT_FAIL;

	uint32_t timer_out = GET_TICK;
	while(SUBDRIVER_GPIO_GetState(DHT->Pin))
    {
		if(GET_TICK - timer_out > TIME_OUT)
			return DHT_FAIL;
	}

	return DHT_OK;
}

static uint8_t DHT_Read(DHT_DATA_t* DHT, uint8_t p_data[5]){
	DHT_Set_Pin_Input(DHT);
	for(int i = 0; i < 5; i++){
		for(int j = 0; j < 8; j++)
		{
			uint32_t timer_out = GET_TICK;
			while(!SUBDRIVER_GPIO_GetState(DHT->Pin)){
				if(GET_TICK - timer_out > TIME_OUT){
					return DHT_FAIL;
				}
			}
			ets_delay_us(40);
			if(!SUBDRIVER_GPIO_GetState(DHT->Pin))
			{
				*(p_data + i) &= ~(1 << (7-j));
			} else {
				*(p_data + i) |= (1 << (7-j));
			}
			timer_out = GET_TICK;
			while(SUBDRIVER_GPIO_GetState(DHT->Pin)){
				if(GET_TICK - timer_out > TIME_OUT){
					return DHT_FAIL;
				}
			}
		}
	}
	return DHT_OK;
}

void DHT_Init(DHT_DATA_t* DHT, uint8_t sensor_type, uint32_t Pin){
	DHT->sensor_type = sensor_type;
	DHT->Pin = Pin;
	DHT->temperature = 0.0;
	DHT->humidity = 0.0;

	// gpio_config_t io_conf = 
    // {
    //     .pin_bit_mask   =   (1ULL << Pin),
    //     .mode           =   GPIO_MODE_INPUT_OUTPUT,
    //     .pull_up_en     =   GPIO_PULLUP_DISABLE,
    //     .pull_down_en   =   GPIO_PULLDOWN_DISABLE,
    //     .intr_type      =   GPIO_INTR_DISABLE,
    // };

    // gpio_config(&io_conf);

}

uint8_t DHT_Read_Value(DHT_DATA_t* DHT){
	uint8_t t_data[5] = {};
	DHT_Start(DHT);
	printf("1\n");
	if(DHT_Check_Response(DHT) == DHT_FAIL)
		return DHT_FAIL;
	printf("2\n");
	if(DHT_Read(DHT, t_data) == DHT_FAIL)
		return DHT_FAIL;
	printf("3\n");
	if(((t_data[0] + t_data[1] + t_data[2] + t_data[3]) & 0xFF) != t_data[4])
		return DHT_FAIL;

	if(t_data[0] < 100 && t_data[0] > 0)
		DHT->humidity = t_data[0] / 1.0;
	else
		return DHT_FAIL;

	if(t_data[2] < 100 && t_data[2] > 0)
		DHT->temperature = t_data[2] / 1.0;
	else
		return DHT_FAIL;

	return DHT_OK;
}