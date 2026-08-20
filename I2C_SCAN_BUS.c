/*
 * I2C_SACAN_BUS.c
 *
 *  Created on: 31/03/2026
 *      Author: Alcides Ramos
 */
#include "I2C_SCAN_BUS.h"


//DEBE PONERSE <<1  A LA DIRECCION ENCONTRADA MICRO
void I2C_SCAN(I2C_HandleTypeDef I2CMIO)
{
char buffer[32];
	for (uint8_t addr = 0; addr < 127; addr++)
	{
		if (HAL_I2C_IsDeviceReady(&I2CMIO, addr << 1, 2, 300) == HAL_OK)
		{
			sprintf(buffer, "DIR: 0x%02X\r\n", addr);
			uartx_write_text(&uart_i2c , buffer);
		}
	}
}


void  I2C_RESPONDE(I2C_HandleTypeDef I2CMIO,uint8_t dir)
{

	//inetnto 3 veces hasta por 300 ms  si no repsonde
	uint8_t si_estoy = HAL_I2C_IsDeviceReady(&I2CMIO, dir , 3, 300);

	if(si_estoy == HAL_OK)
	{
		uartx_write_text(&uart_i2c, "SI ESTOY\r\n");

	}
	else
		uartx_write_text(&uart_i2c, "NO EXISTO\r\n");


}


