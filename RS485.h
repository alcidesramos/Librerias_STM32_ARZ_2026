/*
 * RS485.h
 *
 *  Created on: Jun 25, 2024
 *      Author: alcid
 */

#ifndef LIBRERIAS_RS485_H_
#define LIBRERIAS_RS485_H_
#include "main.h"
#include "pinout.h"

//Tramas a enviar por  RS485
extern const  uint8_t VIENTO_VEL [];
extern const  uint8_t VIENTO_DIR [];

void RS485_WRITE( uint8_t *info, uint8_t Num_Datos);
void RS485_WRITE_TEXT( uint8_t *info);
uint8_t RS485_READ(uint8_t *info, uint8_t Num_Datos,uint16_t wait);

#endif /* LIBRERIAS_RS485_H_ */
