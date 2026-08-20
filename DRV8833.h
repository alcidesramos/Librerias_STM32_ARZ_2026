/*
 * DRV8833.h
 *
 *  Created on: Nov 2, 2024
 *      Author: ALCIDES_RAMOS
 */

#ifndef SRC_LIBRERIAS_DRV8833_H_
#define SRC_LIBRERIAS_DRV8833_H_
#include "main.h"
#include "PWMB.h"

void DVR8833_Init();
void MD_Adelante(uint32_t vm1);
void MD_Atras(uint32_t vm1);
void MI_Adelante(uint32_t vm2);
void MI_Atras(uint32_t vm2);
void MD_Stop();
void MI_Stop();



#endif /* SRC_LIBRERIAS_DRV8833_H_ */
