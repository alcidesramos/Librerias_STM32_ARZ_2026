/*
 * HX710.h
 *
 *  Created on: Apr 21, 2025
 *      Author: Alcides Ramos
 */

#ifndef LIBRERIAS_HX710_H_
#define LIBRERIAS_HX710_H_

#include "main.h"
#include "puertos.h"
#include "delays.h"

#define delay_promedio 150  //retardo en ms para promedio
#define delay_pulso  1// retardo pulsos, en  microsegundi


#define SW1_TARAR HAL_GPIO_ReadPin(SW1_TARAR_GPIO_Port, SW1_TARAR_Pin)
#define SW1_CAL   HAL_GPIO_ReadPin(SW1_CAL_GPIO_Port, SW1_CAL_Pin)

extern const float offset_manualhx;  //offset, valor leido a 0
extern const float calibrahx;  // valor a la calobtracion menos el offset

//IMPORTANTE ESTE VALOR poner el peso conocido
extern const float prueba_hx;  //  presion de prueba para calibrahxción    gramos


extern float factor_hx; //factor de multiplicativo a lectura base    debe colocarse aqui

extern int32_t offset_hx;// carga el offset manual al offset a usar
extern int32_t celda_hx;
extern float presion_hx,cal_hx;




/*
#define HX710_DAT_Pin GPIO_PIN_10
#define HX710_DAT_GPIO_Port GPIOA
#define HX710_CK_Pin GPIO_PIN_11
#define HX710_CK_GPIO_Port GPIOA
*/

void HX710_init();
int32_t HX710_lectura(uint8_t frecuencia);
int32_t HX710_promedio(uint8_t veces,uint8_t frecuencia);  //numero de datos, a que frecuencia
void HX710_tarar(uint8_t veces,uint8_t frecuencia) ;
#endif /* LIBRERIAS_HX710_H_ */
