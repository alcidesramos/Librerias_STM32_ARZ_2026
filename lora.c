/*
 * lora.c
 *
 *  Created on: Jul 2, 2024
 *      Author: Alcides Ramos
 */

#include "lora.h"
extern UART_HandleTypeDef huart3;
#define UART_LORA huart3



void Buffer_hexstring(uint8_t *buffdato,uint16_t largo, char *salida)
{
    for (uint16_t i = 0; i < largo; i++)
    {
      sprintf(salida + (i * 2), "%02X", buffdato[i]);
    }
}

void lora_conex()
{
uartdat_it_idle_dma_lora();
uartx_write_text(&UART_LORA, "AT+JOIN\r\n");
}
