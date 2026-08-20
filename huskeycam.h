/*
 * huskeycam.h
 *
 *  Created on: Dec 9, 2023
 *      Author: alcid
 */

#ifndef LIBRERIAS_HUSKEYCAM_H_
#define LIBRERIAS_HUSKEYCAM_H_
#include "main.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"

//colocar el puerto serial a usar

#define uart_huskey huart1
#define huskeycam USART1

void Huskey_init();
void Huskeylen_Block();



#endif /* LIBRERIAS_HUSKEYCAM_H_ */
