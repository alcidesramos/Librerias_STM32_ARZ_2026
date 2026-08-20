/*
 * CRUCE_CERO.c
 *
 *  Created on: Jan 28, 2025
 *      Author: ALCIDES_RAMOS
 */
#include "CRUCE_CERO.h"

//configura una intrpcion en un pin por flanco de bajado
//configurar como salida el pin de disparo
//configurar un timer en miceoseguntos preescalador=cristal-1

/*EN EL ARCHIVO DE INTERRPCION
 #include "CRUCE_CERO.h"
 Y LLAMAR LOS PROCEDIMEINTOS DE INT DE FLANCO Y DE TIEMR DESADO

 */

/*
 ANTES DEL MAIN LLAMAR
 cruce_init();

 EN EL WHILE CARGAR EL VALOR DE ENERGIA
 LA VARIABLE TIEMPO ES OPCIONAL E SLA VARIBLE QUE TENGA EL VALOR DESEADO
 Cruce_set(&tiempo);

 */

uint16_t cero_valor=LIM_SUP;

//incia los retardos de microsegundos
void cruce_init()
{
Delay_init();//inica retardos
}

//interrpcion por flanco
void flanco_cero_int()
{
	if (__HAL_GPIO_EXTI_GET_IT(C_CERO_Pin) != RESET)
	{


		 //si es menor que el limite inferior  enciende
		if (cero_valor<=LIM_INF+LIM_BASE) PIN_ON(DISP);

		    //si es superior al superior apaga
	   else if (cero_valor>=LIM_SUP-LIM_BASE) PIN_OFF(DISP);

	//sino activa el timer
	   else
	   {
		   //carga valor de disparo en el timer
		   	TIM_CERO_V->ARR=cero_valor;
		   __HAL_TIM_CLEAR_FLAG(&TIM_CERO, TIM_FLAG_UPDATE);
        //inica conreo del timer
		HAL_TIM_Base_Start_IT(&TIM_CERO);//empieza el timer
	   }

	}
}

//interrpcion desbiorde del timer
void timer_cero_int()
{
	    HAL_TIM_Base_Stop_IT(&TIM_CERO);//para el timer
  //  Procede a disparar
	 PIN_ON(DISP);// enciende el triac
	   delay_us(10);  //retardo de disparo
	  PIN_OFF(DISP); //apaga el triac

}

void Cruce_set(float *valor_tiempo)
{
	//almacena el valor quiere posteriormnete cargara en el timer

	cero_valor=(uint16_t)*valor_tiempo;
	}
