/*
 * I2C_MUX.h
 *
 *  Created on: 31/03/2026
 *      Author: Alcides Ramos
 */

#ifndef LIBRERIAS_I2C_MUX_TCA9548_H_
#define LIBRERIAS_I2C_MUX_TCA9548_H_
#include "main.h"
#include "uart.h"
#include "lidarvl53.h"

//puerto I2C a usar
#define  I2C_PORT  hi2c1
//PUERTO UART A USAR
#define UART_PORT huart1
//direccion del sensor Tca9548
#define  LIDAR_ADDR  0x70<<1

extern float distancia[];
extern I2C_HandleTypeDef  I2C_PORT;
extern UART_HandleTypeDef  UART_PORT;
void I2C_MUX(uint8_t channel);
void INIT_LIDARS();
void INIT_LIDARS_FAST();
void Manda_Lidars();
void Manda_Lidars_Teleplot();
void LEE_LIDARS();
void LEE_LIDARS_FAST();

#endif /* LIBRERIAS_I2C_MUX_TCA9548_H_ */
