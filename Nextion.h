/*
 * Nextion.h
 *
 *  Created on: Apr 8, 2025
 *      Author: ALCIDES_RAMOS
 */

// SE DEBE PEGAR EN LIBERRIAS  UARTRX.h Y UART.C Y UART_INT_DMA_IDLE.c

#ifndef LIBRERIAS_NEXTION_H_
#define LIBRERIAS_NEXTION_H_

#include "main.h"
#include "uart.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "UARTRX.h"
extern char nextion_text[];

void nextionRX_init();
void nextionRX_REinit();
void Nextion_Send_Valor(const uint8_t *info,uint32_t valor);
void Nextion_Send_Text(const uint8_t *info);
uint8_t Nextion_Read_Text(const uint8_t *info);
uint8_t Nextion_Read_1Byte();
uint16_t Nextion_Read_2Byte();
uint32_t Nextion_Read_4Byte();
void Nextion_Read_TextVar();
void nextion_Send_Graf(uint8_t id,uint8_t canal,uint16_t valor);
void Nextion_interrup(UART_HandleTypeDef *huart,uint16_t sizex);

#endif /* LIBRERIAS_NEXTION_H_ */
