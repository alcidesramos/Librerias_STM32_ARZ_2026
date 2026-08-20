/*
 * HX711.h
 *
 *  Created on: Apr 9, 2023
 *      Author: Alcides Ramos
 */

#ifndef LIBRERIAS_HX711_H_
#define LIBRERIAS_HX711_H_

#include "main.h"
#include "puertos.h"
#include "delays.h"

#define delay_promedio 150  //retardo en ms para promedio
#define delay_pulso  1// retardo pulsos, en  microsegundi

/*
#define HX711_DAT_Pin GPIO_PIN_10
#define HX711_DAT_GPIO_Port GPIOA
#define HX711_CK_Pin GPIO_PIN_11
#define HX711_CK_GPIO_Port GPIOA
*/

#define sw_tarar HAL_GPIO_ReadPin(sw_tarar_GPIO_Port, sw_tarar_Pin)
#define sw_cal   HAL_GPIO_ReadPin(sw_cal_GPIO_Port, sw_cal_Pin)

void HX711_init();
int32_t HX711_lectura(uint8_t ganancia);
int32_t HX711_promedio(uint8_t veces,uint8_t ganancia);  //numero de datos, a que ganancia
void HX_tarar(uint8_t veces,uint8_t ganancia);  //numero de datos, a que ganancia

#endif /* LIBRERIAS_HX711_H_ */
