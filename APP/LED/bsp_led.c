#include "bsp_led.h"

void green_led(uint8_t flag)
{
	if(flag == 2)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
	}
	else
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, flag);
}

void purple_led(uint8_t flag)
{
	if(flag == 2)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
	}
	else
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, flag);
}
