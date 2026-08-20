/*
 * PWM_IN.C
 *
 *  Created on: Jul 20, 2023
 *      Author: alcides ramos
 */
/*
  colocar variables en main
//float frecuencia;

//iniciar
TCS3200_Init();


 //   y en el while
   PIN_OFF(S2);
    PIN_OFF(S3);
    HAL_Delay(2); //tiempo necesario para captura de datos de frecuencia
    if(frecuencia>rojo_min)  RGB(1,0,0);
    sprintf(texto,"FrecRojo=%.1f\r\n",frecuencia);
    uartx_write_text(&huart1, texto);
    frecuencia=0;


 	//Aplica filtro  verde
     PIN_ON(S2);
     PIN_ON(S3);
     HAL_Delay(2); //tiempo necesario para captura de datos de frecuencia
     if(frecuencia>verde_min)  RGB(0,1,0);
     sprintf(texto,"FrecVerde=%.1f\r\n",frecuencia);
     uartx_write_text(&huart1, texto);
     frecuencia=0;

  	//Aplica filtro  azul
      PIN_OFF(S2);
      PIN_ON(S3);
      HAL_Delay(2); //tiempo necesario para captura de datos de frecuencia
      if(frecuencia>azul_min)  RGB(0,0,1);
      sprintf(texto,"FrecAzul=%.1f\r\n",frecuencia);
      uartx_write_text(&huart1, texto);
      frecuencia=0;



//y en el archivo stm32f4xx_it.c debe quedar asi este procedimiento

void TIM1_CC_IRQHandler(void)
{


#include <TCS3200_COLOR.h>  //debe incluirlo
 TSC3200_Interrupt();



HAL_TIM_IRQHandler(&htim1);

  }
  */

#include <TCS3200_COLOR.h>

//Timer a usar
extern TIM_HandleTypeDef htim1;
//configurar timer input capture
COLORS TCS3200_SEN = {&htim1, &(TIM1->CCR1),
		              TIM_CHANNEL_1,TIM_FLAG_CC1};


uint16_t cap1=0,cap2=0;
uint16_t frec1,frec2;
uint16_t frec1_1,frec1_2;
extern float frecuencia;


void  TCS3200_Init()
{
	//INICIA LE SENSOR DE COLOR
HAL_TIM_IC_Start_IT(TCS3200_SEN.htim,TCS3200_SEN.channel);

}

void TCS3200_Interrupt()
{
	//mira si la interrpcion fue por el sensor de color
	if (__HAL_TIM_GET_FLAG(TCS3200_SEN.htim, TCS3200_SEN.flag_co) ==1)
	{
//primera captura
		if (cap1 == 0)
		{
			//guarda valor del contador incial
			cap1=1;
			frec1_1 = *TCS3200_SEN.ccr;

		}

		//segunda captura
		else if (cap1 == 1)
		{
					//guarda valor del contador
					cap1= 0;
					frec1_2 = *TCS3200_SEN.ccr;
                    //calcula tiempo entre captura
					if (frec1_2 > frec1_1)
					frec1 = (frec1_2 - frec1_1);
					else
     			    frec1 =(TCS3200_SEN.htim->Instance->ARR - frec1_1) + frec1_2;
					//calculo la frecuencia
					frecuencia=HAL_RCC_GetHCLKFreq()/((TCS3200_SEN.htim->Instance->PSC+1)*frec1);

		}

        //limpia bandera d einterrpcion
		__HAL_TIM_CLEAR_FLAG(TCS3200_SEN.htim, TCS3200_SEN.flag_co);//limpia la bandera
	}
}


//COLOCAR  LOS MISMOS PROCEDIMEINTOS PERO ASOCIADOS A OTROS ITMERS SI ES NECESRIO
