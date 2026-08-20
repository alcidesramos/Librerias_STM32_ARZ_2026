

#include "puertos.h"

extern uint32_t pasos;

//forma más rapida



void Output_Pin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
		{

			uint8_t i;


			for (i = 0x00; i < 0x10; i++)
				{
					if (GPIO_Pin & (1 << i))

			     {

			#ifdef STM32F1  //so es la blue pill
		     uint8_t i1;

						                                         //  borra los 4 bits      //coloca en 0001
						if (i<8)
						{

    						  GPIOx->CRL &= ~(0b1111 << (4 * i)); // Borra los 4 bit de MODE y del CNF (SALIDA PUSH PULL)
	    		     		   GPIOx->CRL |=  GPIO_SPEED_FREQ_MEDIUM  << (4 *i); // Selecciona la velocidad de salida, puede cambiarse si se desea

						}
							else
								{
									i1=i-8; //despalza 8 unidades
												//  borra los 4 bits      //coloca en 0001
									GPIOx->CRH &= ~(0b1111 << (4 * i1)); // Borra los 4 bit de MODE y del CNF (SALIDA PUSH PULL)
									GPIOx->CRH |=  GPIO_SPEED_FREQ_MEDIUM  << (4 * i1); // Selecciona la velocidad de salida, puede cambiarse si se desea

								}//else

						break;

		    #else

                             						//borra los 2 bits y pone como salida
 						GPIOx->MODER  = (GPIOx->MODER & ~(0b11 << (2 * i))) | (MODE_OUTPUT << (2 * i));

						//si se desea  configurar el tipo de salida, si es open drive o push-pull


 						//borra el bit     //0=push pull(default)(OUTPUT_PP)  1=open driver(OUTPUT_OD)
						GPIOx->OTYPER=(GPIOx->OTYPER & ~(1<<i))  | (OUTPUT_PP  << i);


 						//si se desea configurar la velocidad el la salida
						    /*00=low
						      01=mediud
							  10=fast
							  11=super fast*/
						                  //borra los bits        pone la velocidad
					  GPIOx->OSPEEDR=(GPIOx->OSPEEDR & ~(0b11 << (2 * i))) | (GPIO_SPEED_FREQ_MEDIUM << (2 *i));

						break;

			#endif
					}//if 1<<i

		    }//for
		}

void Input_Pin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
		{
			uint8_t i;

				for (i = 0x00; i < 0x10; i++)
				{
					//si es 1
					if (GPIO_Pin & (1 << i))
					{

			#ifdef STM32F1  //la blue
						uint8_t i1;
																						//  borra los 4 bits      //coloca en 0100
		  if (i<8) GPIOx->CRL = (GPIOx->CRL & ~(0b1111 << (4*i))) | ( 0b0100 << (4*i)) ;   //STM32F10X
			    else
						{
		          i1=i-8; //despalza 8 unidades
																							//  borra los 4 bits      //coloca en 0100
				GPIOx->CRH = (GPIOx->CRH & ~(0b1111 << (4*i1))) | ( 0b0100 << (4*i1)); //STM32F10X
						}//else
		               break;
			  #else
			     //borra los bit //no es necesario colocar 1

		                  GPIOx->MODER &= ~(0b11UL << (2 * i));
						//si se desea se configiura la parte de pull up o pull down
						/*
						 0=GPIO_NOPULL
						 1=GPIO_PULLUP
						 2=GPIO_PULLDOWN
						*/

					                              //borra los bits     tipo de entrada
				GPIOx->PUPDR=(GPIOx->PUPDR & ~(0b11UL << (2 * i))) | (GPIO_NOPULL << (2 * i));
				break;
				#endif


					}//if 1<<i
				}//for
			}



//Forma más lenta

void Output_Pin1 (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	}

void Input_Pin1 (GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);

}

#ifdef TIMX


uint8_t waitforhigh(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,uint32_t tiem)
{
	TIMX->CNT=0;// Resetea el timer

	//espera respuesta del sensor por 100us
			while (!HAL_GPIO_ReadPin (GPIOx, GPIO_Pin))
			{
				if (TIMX->CNT>=tiem) return(0);

			}
	    return (1);


}


uint8_t waitforlow(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,uint32_t tiem)
{
	TIMX->CNT=0;// Resetea el timer

	//espera respuesta del sensor por 100us
			while (HAL_GPIO_ReadPin (GPIOx, GPIO_Pin))
			{
				if (TIMX->CNT>=tiem) return(0);

			}
	    return (1);


}

#endif


#if __CORTEX_M !=0
uint8_t waitforhigh(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,uint32_t tiem)
{
	DWT->CYCCNT=0;// Resetea el timer

	//espera respuesta del sensor por 100us
			while (!HAL_GPIO_ReadPin (GPIOx, GPIO_Pin))
			{
			if (DWT->CYCCNT>=pasos*tiem) return(0);

			}
	    return (1);


}


uint8_t waitforlow(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,uint32_t tiem)
{
	DWT->CYCCNT=0;// Resetea el timer

	//espera respuesta del sensor por 100us
			while (HAL_GPIO_ReadPin (GPIOx, GPIO_Pin))
			{
				if (DWT->CYCCNT>=pasos*tiem) return(0);

			}
	    return (1);


}


#endif




