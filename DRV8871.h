/*
 * DRV8871.h
 *
 *  Created on: Nov 2, 2024
 *      Author: ALCIDES_RAMOS
 */

#ifndef LIBRERIAS_DRV8871_H_
#define LIBRERIAS_DRV8871_H_
#include "main.h"
#include "PWMB.h"

void DRV8871_Init();
void MD_Adelante(uint32_t vm1);
void MD_Atras(uint32_t vm1);
void MI_Adelante(uint32_t vm2);
void MI_Atras(uint32_t vm2);
void MD_Stop();
void MI_Stop();



#endif //LIBRERIAS_ DVR8871_H_ */
