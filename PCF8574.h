/*
 * I2C_MUX.h
 *
 *  Created on: Jun 8, 2024
 *      Author: ALCIDES RAMOS
 */

#ifndef LIBRERIA_PCF8574_H_
#define LIBRERIA_PCF8574_H_
#include "main.h"


//#define PCF8574_ADDR 0x4E        //direccion del i2c 0b0100     A2A1A0     1
#define PCF8574_ADDR 0x40      //direccion del i2c     0b0100     A2A1A0     0

//EL I2C A USAR
extern I2C_HandleTypeDef hi2c2;
#define PCF8574_I2C hi2c2

void    PCF8574_Write(uint8_t value);
uint8_t PCF8574_Read();
void  PCF8574_WritePin(uint8_t pin,uint8_t value);
uint8_t  PCF8574_ReadPin(uint8_t pin);
void  PCF8574_TogglePin(uint8_t pin);




#endif /* LIBRERIA_PCF8574_H_ */
