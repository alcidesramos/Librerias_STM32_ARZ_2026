
#include "puertos_H2.h"
#include "delays_H2.h"
#include "main.h"

#define onewire_GPIO_Port  GPIOC
#define onewire_Pin   HAL_GPIO_PIN_7

uint8_t onewire_reset ();
void onewire_Write (uint8_t dato);
uint8_t onewire_Read();
