/*
 * tono.c
 *
 *  Created on: Jul 10, 2023
 *      Author: alcid
 */

#include "tono.h"

void Sound_play(uint32_t frec,uint32_t dura)
{
	uint32_t dela=500000/frec;//  la mitad de un mega
	uint32_t  repe=frec*dura/1000;// calcula cuantas veces se repite

		while(repe--)
	  {
	  parlante_GPIO_Port->BSRR=parlante_Pin;
	  delay_us(dela);
	  parlante_GPIO_Port->BSRR=parlante_Pin<<16;
	  delay_us(dela);
      }

}
