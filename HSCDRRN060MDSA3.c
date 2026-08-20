/*
 * HSCDRRN060MDSA3.c
 *
 *  Created on: Aug 10, 2024
 *      Author: AveMaria
 */

#include "HSCDRRN060MDSA3.h"

extern SPI_HandleTypeDef hspi1;

void SPI_Read_n(uint8_t *bufft,uint8_t ndat)
{

	HAL_GPIO_WritePin(SPI1_SS_GPIO_Port, SPI1_SS_Pin, 0);
	HAL_Delay(1);
	HAL_SPI_Receive(&hspi1, bufft, ndat, 10000);
	//HAL_SPI_TransmitReceive(&hspi1, 127, bufft, ndat, 1000);
	HAL_GPIO_WritePin(SPI1_SS_GPIO_Port, SPI1_SS_Pin, 1);
}


void HSCDRRN060MDSA3_read(float *presion, float *temperatura)
{
	 uint8_t buffer[4];
		 	    HAL_GPIO_WritePin(SPI1_SS_GPIO_Port, SPI1_SS_Pin, 0);
		 		HAL_Delay(1);
		 		HAL_SPI_Receive(&hspi1, &buffer, 4, 10000);
		 		//HAL_SPI_TransmitReceive(&hspi1, 0, buffer, 4, 1000);
		 		HAL_GPIO_WritePin(SPI1_SS_GPIO_Port, SPI1_SS_Pin, 1);
		         HAL_Delay(1);
		         buffer[0]=buffer[0] & 0b00111111;
		         uint16_t Salida2=(uint16_t)buffer[0] << 8 | buffer[1];

		         uint16_t temperatura_raw=(uint16_t)buffer[2] << 8 | buffer[3];



		            temperatura_raw=temperatura_raw >> 5;


		            *temperatura=(temperatura_raw*200.0/2047.0)-50.0;

		   *presion = (((Salida2 - SalidaMin) * (PresionMax - PresionMin)) / (SalidaMax - SalidaMin)) + PresionMin;


}
