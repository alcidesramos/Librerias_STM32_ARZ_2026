


#include <onewire.h>



uint8_t onewire_reset ()
{
	uint8_t sen_ok = 0;

	HAL_GPIO_WritePin (onewire_GPIO_Port, onewire_Pin, 0);  // saca un cero
	Output_Pin(onewire_GPIO_Port, onewire_Pin);   // configura el pin como salida
	delay_us (480);//tiempo minimo por datasheet

	Input_Pin(onewire_GPIO_Port, onewire_Pin);    // Configura el pin como entrada
    delay_us (60);//espera 60us para ver si hay sensor

    if (!(HAL_GPIO_ReadPin (onewire_GPIO_Port, onewire_Pin))) sen_ok =1;  //1 si el sensor está conectado
	else sen_ok =0;

	delay_us (480);//tiempo minimo por datasheet
		return (sen_ok);

}

// procedimeinto para enviar 1 byte al sensor
void onewire_Write (uint8_t dato)
{


	for (uint8_t s_cont=0; s_cont<8; s_cont++)
		{
				if ((dato & (1<<s_cont))!=0)//escribir un 1
					{
					HAL_GPIO_WritePin (onewire_GPIO_Port, onewire_Pin, 0);

					Output_Pin(onewire_GPIO_Port, onewire_Pin);   // configura el pin como salida
					delay_us (8);//10us
					Input_Pin(onewire_GPIO_Port, onewire_Pin);   // configura el pin como salida
					delay_us(60);//60us
					}
				else   //escribir un cero
					{
					HAL_GPIO_WritePin (onewire_GPIO_Port, onewire_Pin, 0);
					Output_Pin(onewire_GPIO_Port, onewire_Pin);   // configura el pin como salida
					delay_us (60);//65us
					Input_Pin(onewire_GPIO_Port, onewire_Pin);   // configura el pin como salida
					delay_us (8);//5us
					}
		}


	          }


uint8_t onewire_readbit()
{
    	uint8_t bit_ = 0;
	     HAL_GPIO_WritePin (onewire_GPIO_Port, onewire_Pin, 0);  // saca un 0
			Output_Pin(onewire_GPIO_Port, onewire_Pin);   // configura salida
			delay_us(1) ;// por 3us

			Input_Pin(onewire_GPIO_Port, onewire_Pin);  // Configura como estrada
			delay_us(3) ;// espera 10 us para muestrear no puede pasar de 15
			if (HAL_GPIO_ReadPin (onewire_GPIO_Port, onewire_Pin)==1)  bit_=1;
			delay_us(60);  // retardo de 60us
			return bit_;

}


uint8_t onewire_Read ()
{
	uint8_t dato_leido=0;

	for (unsigned char s_cont=0;s_cont<8;s_cont++)
	{
	     HAL_GPIO_WritePin (onewire_GPIO_Port, onewire_Pin, 0);  // saca un 0
			Output_Pin(onewire_GPIO_Port, onewire_Pin);   // configura salida
			delay_us(1) ;// por 2us

			Input_Pin(onewire_GPIO_Port, onewire_Pin);  // Configura como estrada
			delay_us(3) ;// espera 8us para muestreas no puede poasar de 15
			if (HAL_GPIO_ReadPin (onewire_GPIO_Port, onewire_Pin)==1)  dato_leido |= 1<<s_cont; //coloca 1 en el dato
			delay_us(60);  // retardo de 60us

	}
		return ( dato_leido);//retorna el byte leido

}


