/*
 * tono.h
 *
 *  Created on: Jul 10, 2023
 *      Author: alcid
 */

#ifndef LIBRERIAS_TONO_H_
#define LIBRERIAS_TONO_H_
//definir si se desea el pin del parlante
//#define parlante_Pin GPIO_PIN_0
//#define parlante_GPIO_Port GPIOA

#include "main.h"
#include "delays.h"
void Sound_play(uint32_t frec,uint32_t dura);


#endif /* LIBRERIAS_TONO_H_ */
