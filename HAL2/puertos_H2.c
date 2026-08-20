#include "puertos.h"

extern uint32_t pasos;


void Output_Pin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	LL_GPIO_SetPinMode(GPIOx, GPIO_Pin, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinOutputType(GPIOx, GPIO_Pin, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinSpeed(GPIOx, GPIO_Pin, LL_GPIO_SPEED_FREQ_MEDIUM);
}

void Input_Pin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	LL_GPIO_SetPinMode(GPIOx, GPIO_Pin, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinPull(GPIOx, GPIO_Pin, LL_GPIO_PULL_NO);
}


/* ==========================================================================
   FORMA "LENTA": via HAL_GPIO_Init (stm32c5xx_hal_gpio.h)
   GPIOx es hal_gpio_t (enum: HAL_GPIOA, HAL_GPIOB, ...), NO un puntero.
   El pin ya no va dentro del struct, se pasa como argumento aparte.
   ========================================================================== */

void Output_Pin1 (hal_gpio_t GPIOx, uint32_t GPIO_Pin)
{
	hal_gpio_config_t GPIO_InitStruct = {0};
	GPIO_InitStruct.mode        = HAL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.pull        = HAL_GPIO_PULL_UP;
	GPIO_InitStruct.speed       = HAL_GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOx, GPIO_Pin, &GPIO_InitStruct);
}

void Input_Pin1 (hal_gpio_t GPIOx, uint32_t GPIO_Pin)
{
	hal_gpio_config_t GPIO_InitStruct = {0};
	GPIO_InitStruct.mode = HAL_GPIO_MODE_INPUT;
	GPIO_InitStruct.pull = HAL_GPIO_PULL_NO;
	HAL_GPIO_Init(GPIOx, GPIO_Pin, &GPIO_InitStruct);
}



#ifdef TIMX

uint8_t waitforhigh(hal_gpio_t GPIOx, uint32_t GPIO_Pin, uint32_t tiem)
{
	TIMX->CNT = 0; // Resetea el timer

	while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == HAL_GPIO_PIN_RESET)
	{
		if (TIMX->CNT >= tiem) return (0);
	}
	return (1);
}

uint8_t waitforlow(hal_gpio_t GPIOx, uint32_t GPIO_Pin, uint32_t tiem)
{
	TIMX->CNT = 0; // Resetea el timer

	while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == HAL_GPIO_PIN_SET)
	{
		if (TIMX->CNT >= tiem) return (0);
	}
	return (1);
}

#endif


#if __CORTEX_M != 0

uint8_t waitforhigh(hal_gpio_t GPIOx, uint32_t GPIO_Pin, uint32_t tiem)
{
	DWT->CYCCNT = 0; // Resetea el timer

	while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == HAL_GPIO_PIN_RESET)
	{
		if (DWT->CYCCNT >= pasos * tiem) return (0);
	}
	return (1);
}

uint8_t waitforlow(hal_gpio_t GPIOx, uint32_t GPIO_Pin, uint32_t tiem)
{
	DWT->CYCCNT = 0; // Resetea el timer

	while (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == HAL_GPIO_PIN_SET)
	{
		if (DWT->CYCCNT >= pasos * tiem) return (0);
	}
	return (1);
}

#endif