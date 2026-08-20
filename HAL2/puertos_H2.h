#ifndef PUERTOS_H
#define PUERTOS_H

#include "main.h"
#include "delays_H2.h"

#define MODE_OUTPUT   0b01
#define OUTPUT_PP     0b0


void Output_Pin (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void Input_Pin (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);


void Output_Pin1 (hal_gpio_t GPIOx, uint32_t GPIO_Pin);
void Input_Pin1 (hal_gpio_t GPIOx, uint32_t GPIO_Pin);

uint8_t waitforhigh(hal_gpio_t GPIOx, uint32_t GPIO_Pin, uint32_t tiem);
uint8_t waitforlow(hal_gpio_t GPIOx, uint32_t GPIO_Pin, uint32_t tiem);

#endif /* PUERTOS_H */