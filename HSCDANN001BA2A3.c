/*
 * HSCDANN001BA2A3.c
 *
 *  Created on: Jul 3, 2024
 *      Author: AveMaria
 */

#include"HSCDANN001BA2A3.h"

void I2C_Write_n(uint8_t *bufft,uint8_t ndat)
{
     HAL_I2C_Master_Transmit(&HSCDANN001BA2A3_I2C,HSCDANN001BA2A3_DIR ,&bufft, ndat, 300);
}


void I2C_Read_n(uint8_t *buffr,uint8_t ndat)
{

 HAL_I2C_Master_Receive(&HSCDANN001BA2A3_I2C,HSCDANN001BA2A3_DIR ,buffr, ndat, 1000);

}
 // la presion esta en Kpa y la temperatura en grads C

void HSCDANN001BA2A3_read(float *presion, float *temperatura)
{
   uint8_t buffer[4];
   I2C_Read_n(buffer,4);
   buffer[0]=buffer[0] & 0b00111111;


   uint16_t Salida2=(uint16_t)buffer[0] << 8 | buffer[1];
   uint16_t temperatura_raw=(uint16_t)buffer[2] << 8 | buffer[3];
   temperatura_raw=temperatura_raw >> 5;

   *presion = (((Salida2 - SalidaMin) * (PresionMax - PresionMin)) / (SalidaMax - SalidaMin)) + PresionMin;
   *temperatura=(temperatura_raw*200.0/2047.0)-50.0;

}

uint8_t HSCDANN001BA2A3_ready()
{

   if ((HAL_I2C_IsDeviceReady(&HSCDANN001BA2A3_I2C, HSCDANN001BA2A3_DIR , 10, 1000)== HAL_OK))

		   return 1;
   else
	   return 0;

}
