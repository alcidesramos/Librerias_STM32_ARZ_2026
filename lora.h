/*
 * lora.h
 *
 *  Created on: Jul 2, 2024
 *      Author: alcid
 */

#ifndef LIBRERIAS_LORA_H_
#define LIBRERIAS_LORA_H_
#include "main.h"
#include "uart.h"
#include <uart_int_idle_dma.h>


void lora_conex();
void Buffer_hexstring(uint8_t *buffdato,uint16_t largo, char *salida);

#endif /* LIBRERIAS_LORA_H_ */
