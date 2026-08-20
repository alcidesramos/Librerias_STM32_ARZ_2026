/*
 * ADXL345.c
 *
 *  Created on: Oct 21, 2025
 *      Author: ALCIDES_RAMOS
 */


#include "ADXL345.h"
#define EPSILON          1e-6f // Límite para evitar divisiones por cero
#define RAD_TO_DEG       57.295779513f
#define DEG_TO_RAD 0.01745329251f

uint8_t ADXL345_BUF[10];
float ADXL345_ACELX, ADXL345_ACELY, ADXL345_ACELZ;
float  acel_pitch, acel_roll;

void ADXL345_Write_Reg (uint8_t Reg, uint8_t Byte)
{
	HAL_I2C_Mem_Write(&ADXL345I2C ,ADXL_I2CAddress, Reg, 1, &Byte, 1, 100);
}

void ADXL345_Read (uint8_t Reg, uint8_t *Buffer, size_t len)
{
	HAL_I2C_Mem_Read (&ADXL345I2C ,ADXL_I2CAddress, Reg, 1, Buffer, len, 2000);
}

void ADXL345_Init ()
{
	uint8_t chipID=0;
	ADXL345_Read(0x00, &chipID, 1);
	if (chipID == 0xE5)
	{
		ADXL345_Write_Reg (0x2d, 0x00);  // rese
		ADXL345_Write_Reg (0x2d, 0x08);  // 8hz
	}
}
void ADXL345_READ_ACEL()
{

	  ADXL345_Read(0x32, ADXL345_BUF, 6);
	  int16_t RAWX = ((ADXL345_BUF[1]<<8)|ADXL345_BUF[0]);
	  int16_t RAWY = ((ADXL345_BUF[3]<<8)|ADXL345_BUF[2]);
	  int16_t RAWZ = ((ADXL345_BUF[5]<<8)|ADXL345_BUF[4]);

	  ADXL345_ACELX = RAWX * .0078;
	  ADXL345_ACELY = RAWY * .0078;
	  ADXL345_ACELZ = RAWZ * .0078;

}


void ADXL345_Angulos()
{
float denom = sqrtf(ADXL345_ACELY * ADXL345_ACELY + ADXL345_ACELZ * ADXL345_ACELZ);
if (denom < EPSILON) denom = EPSILON;
acel_pitch = atan2f(ADXL345_ACELX, denom) * RAD_TO_DEG;
denom = sqrtf(ADXL345_ACELX * ADXL345_ACELX + ADXL345_ACELZ * ADXL345_ACELZ);
if (denom < EPSILON) denom = EPSILON;
acel_roll = atan2f(ADXL345_ACELY, denom) * RAD_TO_DEG;
}
