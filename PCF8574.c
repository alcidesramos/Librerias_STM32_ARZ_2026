/*
 * I2C_MUX.C
 *
 *  Created on: Jun 8, 2024
 *      Author: ALCIDES RAMOS
 */

#ifndef LIBRERIA_I2C_MUX_C_
#define LIBRERIA_I2C_MUX_C_
#include <PCF8574.h>
#include "stdint.h"
 uint8_t dataIn     = 0; //mantiede estado d elas entradas
 uint8_t dataOut    = 0xFF;//manriene estado de las salidas

//funcion que escribe en el PCF8574 8 BITS
void  PCF8574_Write(uint8_t value)
{
	dataOut=value;
	HAL_I2C_Master_Transmit(&PCF8574_I2C, PCF8574_ADDR ,&value, 1, 1000);
}

//funcion que lee en el PCF8574 8 BITS
uint8_t  PCF8574_Read()
{
HAL_I2C_Master_Receive(&PCF8574_I2C, PCF8574_ADDR ,&dataIn, 1, 1000);
return (dataIn);
}

void  PCF8574_WritePin(uint8_t pin,uint8_t value)
{
	  if (value == 0)
	  {
	    dataOut &= ~(1 << pin);
	  }
	  else
	  {
	    dataOut |= 1<<pin;
	  }
	  PCF8574_Write(dataOut);
}
uint8_t  PCF8574_ReadPin(uint8_t pin)
{
uint8_t retu;
retu=PCF8574_Read();
if ((retu & (1 << pin))>0) return (1);
else return (0);
}


void  PCF8574_TogglePin(uint8_t pin)
{
	 dataOut ^= 1<<pin;

	  PCF8574_Write(dataOut);
	}






#endif /* LIBRERIA_I2C_MUX_C_ */
