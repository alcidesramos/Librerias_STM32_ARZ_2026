/*
 * ircodigo.c
 *
 *  Created on: Sep 21, 2024
 *      Author: ALCIDES_RAMOS
 */

#include "ircodigo.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"


//Necesarias para el algoritmo has
#define FNV_PRIME 16777619
#define FNV_BASIS  2166136261

uint32_t codigo_ir;
uint8_t  flag_codigo=0;
uint16_t   cuenta=0;
uint16_t timer_aux;
uint16_t tiempo[100];
uint16_t tiempo1[100];

//rango de diferncia entre leidos
#define ir_rango 200

//colocar  valores obtenidos de calibracion

uint8_t grupos=2;  //coloca cuando grupos son

//uint16_t promedios[5]={0,0,0,0,0,0}; //colocar valores representativos
uint16_t promedios[5]={885,1799,0,0,0,0}; //colocar valores representativos

//uint16_t promedios[5]={223,775,1819,0,0}; //colocar valores representativos


extern  uint32_t codigo_ir; // variable salida has que almcena codigo unico



 uint32_t  Hash_algoritmo ()
 {
	 uint32_t  hash_acum = FNV_BASIS;

       for (uint8_t cont=0;cont<cuenta-1;cont++)
           {
          	// coloca el valor dentro del hash
 		hash_acum = (hash_acum * FNV_PRIME) ^ tiempo[cont];
 	}

 	return (hash_acum);
 }



void ir_init()
{
	  cuenta=0;
  	  flag_codigo=0;
  	   TIMIR->CNT=0;// Resetea el timer
	__HAL_TIM_CLEAR_IT(&htimIR, TIM_IT_UPDATE);//limpia la bandera
	 __HAL_GPIO_EXTI_CLEAR_IT(IR_in_Pin);//limpia la bandera de inte
	  HAL_NVIC_EnableIRQ(int_pinir);   //Habilita int pin

	   //EXTI->IMR |= pinir;//Habilita int pin

}

void ir_timer_interrupcion()
{
    // Verificar si la interrupción fue generada por TIMIR
	    if (__HAL_TIM_GET_FLAG(&htimIR, TIM_FLAG_UPDATE))
	    {
	   	__HAL_GPIO_EXTI_CLEAR_IT(IR_in_Pin);//limpia la bandera de inte
	   	HAL_NVIC_DisableIRQ(int_pinir); //  Deshabilita int pin

	   	//      EXTI->IMR &= ~(pinir);//deshabilita la int pinir
  	    	cuenta--;
    	   	  flag_codigo=1;

    	   	  HAL_TIM_Base_Stop_IT(&htimIR);//deshabilita itTimer
	        __HAL_TIM_CLEAR_IT(&htimIR, TIM_IT_UPDATE);//limpia la bandera
	//        HAL_NVIC_DisableIRQ(TIM4_IRQn);//deshabilita int del timerIR
	    }

}
//interrpcion pro flancos
void ir_interrupcion()

{
	if (__HAL_GPIO_EXTI_GET_IT(IR_in_Pin) != RESET)
	{
     //   si la interrupccion fue por el sensor ir
		   if (flag_codigo==0)
		    {
					 if (cuenta!=0)   // omite el primer dato de interrpcion
					 {
					   timer_aux=TIMIR->CNT; //captura el valor del timer
					   TIMIR->CNT=0; // resetea el timer para otra lectura
					   tiempo[cuenta-1]=timer_aux;
					   cuenta++;
					   }//fin if cuenta

				   else    // SOLO PARA PRIMERA INTERRUPCION
				   {
					cuenta++;
						  TIMIR->CNT=0;// Resetea el timer
		//				  HAL_NVIC_EnableIRQ(TIM4_IRQn);
						__HAL_TIM_CLEAR_IT(&htimIR, TIM_IT_UPDATE);//limpia la bandera
						HAL_TIM_Base_Start_IT(&htimIR);//hab int timerir

		          }
	        }
	  }
	 __HAL_GPIO_EXTI_CLEAR_IT(IR_in_Pin);//limpia la bandera de inte

}

//imprime los datos crudos  usado para calcular los codigos basicos y despues normalizar
void printraw()
{
	char te[20];
		//uartx_write_text(&irport,"codigo recibido\r\n");
    	for (uint16_t cont=0;cont<cuenta-1;cont++)
          {
         sprintf(te,"%d ",tiempo[cont]);
      uartx_write_text(&irport, te);
		  }
    	uartx_write_text(&irport, "\r\n");

	 }


void ir_normaliza()
{

	for (uint16_t cont=0;cont<cuenta-1;cont++)
	{
	   for(uint8_t i=0;i<5;i++)
	   {
		   //resta  el dato con cada promedio mira si esta dentro de ir range
		   int16_t res= abs(tiempo[cont] - promedios[i]);
		   if (res<=ir_rango)  tiempo[cont]=promedios[i];
	   }

	}

}

// Función para ordenar los elementos
uint8_t ir_ordena()
{
    uint32_t suma = 0;  // Acumulador para sumar valores dentro de un grupo
    uint8_t elementosGrupo = 0;  // Contador de elementos en un grupo
   uint8_t grupocont =1;
    // Copiar los datos originales
    for (uint8_t i = 0; i < cuenta; i++) {
        tiempo1[i] = tiempo[i];
    }

    // Ordenar los datos de mayor a menor
    for (uint8_t i = 0; i < cuenta - 1; i++)
    {
        for (uint8_t j = 0; j < cuenta - i - 1; j++)
        {
            if (tiempo1[j] > tiempo1[j + 1]) {
                uint16_t temp = tiempo1[j];
                tiempo1[j] = tiempo1[j + 1];
                tiempo1[j + 1] = temp;
            }
        }
    }

    suma = tiempo1[0];  // carga en suma el primer valor
    elementosGrupo = 1;

    // Iterar para encontrar los grupos y calcular el promedio
    for (uint8_t i = 1; i < cuenta; i++)
    {
        if (tiempo1[i] - tiempo1[i - 1] > ir_rango)
        {
            // Si hay una diferencia mayor que ir_rango, se finaliza el grupo anterior
            promedios[grupocont - 1] = suma / elementosGrupo;  // Guardar el promedio del grupo anterior

            grupocont++;  // Nuevo grupo
            suma = tiempo1[i];  // Reiniciar la suma con el nuevo valor
            elementosGrupo = 1;  // Reiniciar el contador de elementos en el nuevo grupo
        } else {
            // Si el valor sigue dentro del grupo, sumarlo
            suma += tiempo1[i];
            elementosGrupo++;
        }
    }

    // Guardar el promedio del último grupo
    promedios[grupocont - 1] = suma / elementosGrupo;
    ir_normaliza();
    grupos=grupocont;
    return grupocont;
}
void ir_calibra()
{
char te[200];
memset(promedios,0,sizeof(promedios));//limPia promedio anteriores
while (flag_codigo==0);
printraw();
uint8_t son=ir_ordena();
printraw();
sprintf(te,"cambios=%d\r\n",son);
uartx_write_text(&irport, te);


//se hace hasta cinco no va habr mas de 5 czambios en los codigos

     for (uint8_t cont=0;cont<5;cont++)
		{
	   sprintf(te,"PRO%d=%d  ",cont,promedios[cont]);
	      uartx_write_text(&irport, te);
			  }
	    	uartx_write_text(&irport, "\r\n");
	    	codigo_ir=Hash_algoritmo();//aplica algoritmo hash
	    	sprintf(te,"codigo=%0X\r\n",codigo_ir);
	    		uartx_write_text(&huart1, te);

ir_init();
uartx_write_text(&huart1, "\r\n");
}

void analiza_ir()
{
    ir_normaliza();
	codigo_ir=Hash_algoritmo();//aplica algoritmo hash
  }

