/*
 * encoder.c
 *
 *  Created on: Mar 18, 2025
 *      Author: ALCIDES_RAMOS
 */

#include  "encoder.h"

//incluir los timer usados
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim5;

//configura encoder a usar
ENCODERS ENCODER1 = {&htim2,TIM2};
ENCODERS ENCODER2 = {&htim5,TIM5};

//define direccion de giro encoders
#define ENCODER1_DIR   -1
#define ENCODER2_DIR  1

#define TIMER_BITS 32   //  #bits timer  usado

uint16_t encoder_velo=0;
void Encoder_init(ENCODERS *encoder,uint16_t resol)
{
  encoder->posicion=0;
  encoder->pulsos=0;
  encoder->resolucion=resol;
  encoder->velo=0;
  encoder->velo_ant=0;
  HAL_TIM_Encoder_Start_IT(encoder->htim,TIM_CHANNEL_ALL);
  }



//interrpcion de encoder usados
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */

	//agragar por cada encoder usado
	if (htim->Instance == ENCODER1.TIMX)
	{
		ENCODER1.pulsos=__HAL_TIM_GET_COUNTER(htim);
		ENCODER1.posicion=ENCODER1_DIR *(int32_t)ENCODER1.pulsos/ENCODER1.resolucion;
		ENCODER1.velo=ENCODER1.posicion;

	}

//AGRAGAR POR CADA ENCODER USADO
	//AGRGAGR Y CAMBIAR AL ENCODER CORRESPONDIENTE
	if (htim->Instance == ENCODER2.TIMX)
	{
		ENCODER2.pulsos=__HAL_TIM_GET_COUNTER(htim);
		ENCODER2.posicion=(int32_t)ENCODER2.pulsos/ENCODER2.resolucion;
		ENCODER2.posicion=ENCODER2_DIR *ENCODER2.posicion;
		ENCODER2.velo=ENCODER2.posicion;

 }

	UNUSED(htim);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_IC_CaptureCallback could be implemented in the user file
   */
}
void Encoder_velo(void)
{
    static uint16_t encoder_velo = 0;
    const float Ts = 0.02f; // periodo de muestreo (20 ms)

    encoder_velo++;

    if (encoder_velo > 20)
    {
        //ENCODER 1
        int32_t cnt = (int32_t)__HAL_TIM_GET_COUNTER(ENCODER1.htim);
        int32_t diff = cnt - ENCODER1.velo_ant;
        ENCODER1.velo_ant = cnt;

        // Desborde del encoder
    #if (TIMER_BITS == 16)
        if (diff > 32767) diff -= 65536;
        else if (diff < -32768) diff += 65536;
    #elif (TIMER_BITS == 32)
        if (diff > (int32_t)(0x7FFFFFFF / 2)) diff -= (int32_t)0xFFFFFFFF;
        else if (diff < -(int32_t)(0x7FFFFFFF / 2)) diff += (int32_t)0xFFFFFFFF;
    #endif

        // Actualiza posición acumulada
        ENCODER1.posicion += diff;

        // Calcula velocidad (vueltas por segundo)
        ENCODER1.velocidad = (float)(ENCODER1_DIR * diff) / ENCODER1.resolucion / Ts;


        //===================== ENCODER 2 =====================
        cnt = (int32_t)__HAL_TIM_GET_COUNTER(ENCODER2.htim);
        diff = cnt - ENCODER2.velo_ant;
        ENCODER2.velo_ant = cnt;

    #if (TIMER_BITS == 16)
        if (diff > 32767) diff -= 65536;
        else if (diff < -32768) diff += 65536;
    #elif (TIMER_BITS == 32)
        if (diff > (int32_t)(0x7FFFFFFF / 2)) diff -= (int32_t)0xFFFFFFFF;
        else if (diff < -(int32_t)(0x7FFFFFFF / 2)) diff += (int32_t)0xFFFFFFFF;
    #endif

        ENCODER2.posicion += diff;

        ENCODER2.velocidad = (float)(ENCODER2_DIR * diff) / ENCODER2.resolucion / Ts;

        // Reinicia el contador de ciclos de muestreo
        encoder_velo = 0;
    }
}
