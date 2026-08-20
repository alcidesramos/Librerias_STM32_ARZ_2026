/*
 * ADXL345.h
 *
 *  Created on: Oct 21, 2025
 *      Author: ALCIDES_RAMOS
 */

#ifndef LIBRERIAS_ADXL345_H_
#define LIBRERIAS_ADXL345_H_
#include "main.h"
#include "math.h"

#define ADXL_I2CAddress 0xA6

#define  ADXL345I2C hi2c1  //i2c a usar

/*
en el main incluir
#include "ADXL345.h"

en el  main llamar
ADXL345_Init();


y en el while llamar lo deseado
 ADXL345_READ_ACEL();//LEE DATOS ACELERACIONES
	  ADXL345_Angulos();//LEE ANGULOS
sprintf(texto ,"AX=%.2f  AY=%.2f   AZ=%.2f\r\n",ADXL345_ACELX,ADXL345_ACELY,ADXL345_ACELZ);
uartx_write_text(&huart1, texto);

sprintf(texto ,"pitch=%.2f  roll=%.2f\r\n\r\n",acel_pitch,acel_roll);
uartx_write_text(&huart1, texto);
 */
extern float ADXL345_ACELX, ADXL345_ACELY, ADXL345_ACELZ;
extern float  acel_pitch, acel_roll;
extern I2C_HandleTypeDef ADXL345I2C;

void ADXL345_Init ();
void ADXL345_READ_ACEL();
void ADXL345_Angulos();

#endif /* LIBRERIAS_ADXL345_H_ */
