/*
 * ds18b20.c
 *
 *  Created on: 17/10/2021
 *      Author: Alcides Ramos
 */



#include "ds18b20.h"



float DS18b20_temp()
{
uint8_t t1_,t2_;
int16_t  temp_;


    	onewire_reset();// reset
	   onewire_Write (0xCC);
		 onewire_Write (0x44);  // inicia conversión
		  HAL_Delay (800);//c0nversion time

		  onewire_reset ();// reset onewire
	      onewire_Write (0xCC);
	       onewire_Write (0xBE);

	      t1_ =onewire_Read();
	      t2_ = onewire_Read();

		  temp_=( (unsigned int)t2_<<8) | t1_;

		  return ((float)temp_/16.0);


}
