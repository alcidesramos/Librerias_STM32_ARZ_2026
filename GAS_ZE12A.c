/*
 * ZE12A.c
 *
 *  Created on: Jun 17, 2024
 *      Author: Alcides Ramos
 */

#include <GAS_ZE12A.h>

extern UART_HandleTypeDef huart2;

#define UARTM1_1 huart2
#define UARTM1_2 huart2
#define UARTM1_3 huart2
#define UARTM1_4 huart2

//Cualquier sensor
const  uint8_t ZE12A_CMD_ACTIVO []={0xFF,0x01,0x78,0x40,0x00,0x00,0x00,0x00,0X47};
const  uint8_t ZE12A_CMD_PASIVO []={0xFF,0x01,0x78,0x41,0x00,0x00,0x00,0x00,0X46};
const  uint8_t ZE12A_CMD_READ   []={0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0X79};


void ZE12A_PASIVO()
{
for (uint8_t cont=0;cont<9;cont++) uartx_write(&UARTM1_1, ZE12A_CMD_PASIVO[cont]);

}

void ZE12A_READ()
{
for (uint8_t cont=0;cont<9;cont++) uartx_write(&UARTM1_1, ZE12A_CMD_READ[cont]);
}

void ZE12A_ACTIVO()
{
for (uint8_t cont=0;cont<9;cont++) uartx_write(&UARTM1_1, ZE12A_CMD_ACTIVO[cont]);
}

