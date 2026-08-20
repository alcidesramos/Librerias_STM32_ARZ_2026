/*
 * DRV8833.c
 *
 *  Created on: Nov 2, 2024
 *      Author: ALCIDES_RAMOS
 */

#include "DRV8833.h"


//inicia los dvr correspondientes, en el caso 2
void DVR8833_Init()
{
	  PWM_init(&PWM1);
	  PWM_init(&PWM2);
	  PWM_init(&PWM3);
	  PWM_init(&PWM4);
	  MD_Stop();
	  MI_Stop();

}

void MD_Adelante(uint32_t vm1)
{

	  PWM_RAW(&PWM1, vm1);
	  PWM_RAW(&PWM2,0);

}
void MD_Atras(uint32_t vm1)
{

	  PWM_RAW(&PWM1, 0);
	  PWM_RAW(&PWM2,vm1);

}
void MI_Adelante(uint32_t vm2)
{

	  PWM_RAW(&PWM3, 0);
	  PWM_RAW(&PWM4,vm2);

}
void MI_Atras(uint32_t vm2)
{

	PWM_RAW(&PWM3, vm2);
    PWM_RAW(&PWM4,0);

}
void MD_Stop()
{
	  PWM_RAW(&PWM1, 0);
	  PWM_RAW(&PWM2,0);

}
void MI_Stop()
{
	  PWM_RAW(&PWM3, 0);
	  PWM_RAW(&PWM4,0);
}



