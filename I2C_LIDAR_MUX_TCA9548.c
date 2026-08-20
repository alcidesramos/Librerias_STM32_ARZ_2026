/*
 * I2C_MUX.c
 *
 *  Created on: 31/03/2026
 *      Author: Alcides Ramos
 */

#ifndef SRC_I2C_MUX_C_
#define SRC_I2C_MUX_C_
#include "I2C_LIDAR_MUX_TCA9548.h"


float distancia[8];
char textomux[30];


void I2C_MUX(uint8_t channel)
{
    uint8_t mask = (1 << channel);
    HAL_I2C_Master_Transmit(&I2C_PORT, LIDAR_ADDR, &mask, 1, 500);
}

//colocar  las funciones de lso sensofres a usar
void INIT_LIDARS()
{
	for (uint8_t canal = 0; canal < 8; canal++)
	{
	  I2C_MUX(canal);
	  if (HAL_I2C_IsDeviceReady(&I2C_PORT, dir_s1 , 2, 1000) == HAL_OK)
	     {
		  sprintf(textomux,"LIDAR%u OK\r\n",canal+1);
	       uartx_write_text(&UART_PORT,textomux);
	     	lidar_init(dir_s1);
	     }

	}
	 }

void INIT_LIDARS_FAST()
{
	for (uint8_t canal = 0; canal < 8; canal++)
		{
		  I2C_MUX(canal);
		  if (HAL_I2C_IsDeviceReady(&I2C_PORT, dir_s1 , 2, 200) == HAL_OK)
		     {
			  sprintf(textomux,"LIDAR%u OK\r\n",canal+1);
			uartx_write_text(&UART_PORT,textomux);
			lidar_init_fast(dir_s1);
		   }

		}
}
void Manda_Lidars()
{
	   sprintf(textomux,"Dis1=%.1f\r\n",distancia[0]);
	   	uartx_write_text(&UART_PORT, textomux);
		sprintf(textomux,"Dis2=%.1f\r\n",distancia[1]);
		uartx_write_text(&UART_PORT, textomux);
	    sprintf(textomux,"Dis3=%.1f\r\n",distancia[2]);
	    uartx_write_text(&UART_PORT, textomux);
		sprintf(textomux,"Dis4=%.1f\r\n",distancia[3]);
		uartx_write_text(&UART_PORT, textomux);
		sprintf(textomux,"Dis5=%.1f\r\n",distancia[4]);
		uartx_write_text(&UART_PORT, textomux);
		sprintf(textomux,"Dis6=%.1f\r\n",distancia[5]);
		uartx_write_text(&UART_PORT, textomux);
		sprintf(textomux,"Dis7=%.1f\r\n",distancia[6]);
		uartx_write_text(&UART_PORT, textomux);
		sprintf(textomux,"Dis8=%.1f\r\n",distancia[7]);
		uartx_write_text(&UART_PORT, textomux);
		uartx_write_text(&UART_PORT,"\r\n");
}
void Manda_Lidars_Teleplot()
{
	sprintf(textomux,">L1:%.1f\n",distancia[0]);
    uartx_write_text(&UART_PORT, textomux);
	sprintf(textomux,">L2:%.1f\n",distancia[1]);
    uartx_write_text(&UART_PORT, textomux);
	sprintf(textomux,">L3:%.1f\n",distancia[2]);
    uartx_write_text(&UART_PORT, textomux);
	sprintf(textomux,">L4:%.1f\n",distancia[3]);
    uartx_write_text(&UART_PORT, textomux);
	sprintf(textomux,">L5:%.1f\n",distancia[4]);
    uartx_write_text(&UART_PORT, textomux);
	sprintf(textomux,">L6:%.1f\n",distancia[5]);
    uartx_write_text(&UART_PORT, textomux);
	sprintf(textomux,">L7:%.1f\n",distancia[6]);
    uartx_write_text(&UART_PORT, textomux);
	sprintf(textomux,">L8:%.1f\n",distancia[7]);
    uartx_write_text(&UART_PORT, textomux);

}


void LEE_LIDARS()
{

	for (uint8_t canal = 0; canal < 8; canal++)
	{
	    I2C_MUX(canal);  // activa el canal correspondiente (0–7)
	    if (HAL_I2C_IsDeviceReady(&I2C_PORT, dir_s1 , 2, 200) == HAL_OK)
	   {distancia[canal]=lidar_lee_cm(dir_s1);}

	   }
}

void LEE_LIDARS_FAST()
{

	for (uint8_t canal = 0; canal < 8; canal++)
		{
		    I2C_MUX(canal);  // activa el canal correspondiente (0–7)
		    if (HAL_I2C_IsDeviceReady(&I2C_PORT, dir_s1 , 1, 100) == HAL_OK)
		    {distancia[canal]=lidar_lee_cm_fast(dir_s1);}

		 }
}


#endif /* SRC_I2C_MUX_C_ */
