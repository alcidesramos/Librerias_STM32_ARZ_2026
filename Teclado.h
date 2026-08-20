/*
 * Teclado.h
 *
 *  Created on: 26/06/2023
 *      Author: alcid
 */

#ifndef LIBRERIAS_TECLADO_H_
#define LIBRERIAS_TECLADO_H_

#include "main.h"
#include "delays.h"

//DEFINE SI LAS RESISTENCIAS DE LAS COLUMNAS SON PULL UP O PULL DOWM
#define res 0;  //0=TIERRA    1=VDD

//PARA LA LECTURA D ELAS COLUMNAS
#define te_c1 HAL_GPIO_ReadPin(TC1_GPIO_Port, TC1_Pin)
#define te_c2 HAL_GPIO_ReadPin(TC2_GPIO_Port, TC2_Pin)
#define te_c3 HAL_GPIO_ReadPin(TC3_GPIO_Port, TC3_Pin)
#define te_c4 HAL_GPIO_ReadPin(TC4_GPIO_Port, TC4_Pin)

uint8_t tecla_presionada();//retorna el valor de la tecla leida
uint8_t tecla_deco();//decodifoca la tecla leida

void teclado_estado();
void teclado_init();
void teclado_retardo(int16_t valor);





#endif /* LIBRERIAS_TECLADO_H_ */
