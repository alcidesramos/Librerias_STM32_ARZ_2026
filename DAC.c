/*
 * DAC.c
 *
 *  Created on: 3 may 2026
 *      Author: Alcides Ramos
 */

#include "DAC.h"
//COLOCA DAC a usar del miro
extern DAC_HandleTypeDef hdac1;

//Define buffer de datos para DMA  de LOS DAC DEL USUARIO NO CAMBIAR
uint32_t DACU1_Buffer[sizebuff_DACU1];
uint32_t DACU2_Buffer[sizebuff_DACU2];

//declare estrutura pAra manejo de los dac del usuario  sin DMA
//DACS DACU1 = {&hdac1,DAC_CHANNEL_1};
//DACS DACU2 = {&hdac1,DAC_CHANNEL_2};

//SI USA DMA
//defina timer usado
extern TIM_HandleTypeDef htim6;

DACS DACU1 = {&hdac1,DAC_CHANNEL_1,&htim6, DACU1_Buffer,sizebuff_DACU1};
DACS DACU2 = {&hdac1,DAC_CHANNEL_2,&htim6, DACU2_Buffer,sizebuff_DACU2};

 //FIN SI USA DMA






void  DAC_INIT(DACS *dac)
{
HAL_DAC_Start(dac->hdac, dac->channel);

}

void DAC_OUT_CODIGO(DACS *dac,uint16_t codigo)
{
	HAL_DAC_SetValue(dac->hdac,dac->channel,DAC_ALIGN_12B_R,codigo);

}

void DAC_OUT_VOLTAJE(DACS *dac,float valor)
{
	uint16_t  code;
	code=valor*4095/(float) DAC_VREF ;
	HAL_DAC_SetValue(dac->hdac,dac->channel,DAC_ALIGN_12B_R,code);

}


void  DAC_INIT_DMA(DACS *dac)
{

	 HAL_DAC_Start_DMA(dac->hdac,dac->channel,(uint32_t*)dac->buffer,dac->size,DAC_ALIGN_12B_R);
	 HAL_TIM_Base_Start(dac->htim);
}


//timer calculo PRESCALADOR Y ARR ADECUADO autmatiamente
uint8_t DAC_TimerConfig(DACS *dac, float frecuencia)
{
    if (frecuencia <= 0.0f || dac->size == 0)
        return 0;

    uint32_t clk = HAL_RCC_GetSysClockFreq();

    uint8_t is32 =
        (dac->htim->Instance == TIM2 ||
         dac->htim->Instance == TIM5);

    uint32_t max_arr = is32 ? 0xFFFFFFFF : 0xFFFF;

    float best_error = 1e9;
    uint32_t best_psc = 0;
    uint32_t best_arr = 0;

    for (uint32_t psc = 0; psc < 65536; psc++)
    {
        float denom = frecuencia * dac->size * (psc + 1);

        if (denom <= 0.0f) continue;

        float arr_f = ((float)clk / denom) - 1.0f;

        if (arr_f < 0.0f) continue;

        uint32_t arr = (uint32_t)(arr_f + 0.5f); // redondeo

        if (arr > max_arr) continue;

        float f_real = (float)clk /
            ((psc + 1.0f) * (arr + 1.0f) * dac->size);

        float error = fabsf(f_real - frecuencia);

        // criterio: menor error
        if (error < best_error)
        {
            best_error = error;
            best_psc = psc;
            best_arr = arr;

            if (error < 0.0001f)
                break;
        }
    }

    if (best_error == 1e9)
        return 0;

    dac->htim->Instance->PSC = best_psc;
    dac->htim->Instance->ARR = best_arr;

    return 1;
}


//funciones de generacion de señales
//SENO
void SENO(DACS *dac,float frecuencia,float amplitud,float offset)
{

    if (amplitud > DAC_VREF) amplitud = DAC_VREF;
    if (offset < 0.0f) offset = 0.0f;
    if (offset > amplitud) offset = amplitud;

    float scale = (DAC_RESOLUCION - 1.0f) / DAC_VREF;

    for (uint32_t i = 0; i < dac->size; i++)
    {
        float angulo = (2.0f * M_PI * i) / dac->size;

        float voltaje_dac =
            offset + (amplitud - offset) *
            ( (sinf(angulo) + 1.0f) * 0.5f );

        int32_t code = (int32_t)(voltaje_dac * scale);

        if (code < 0) code = 0;
        if (code > (int32_t)(DAC_RESOLUCION - 1))
            code = (int32_t)(DAC_RESOLUCION - 1);

       dac->buffer[i] = (uint32_t)code;
    }


  DAC_TimerConfig(dac, frecuencia);

   // calcula = (uint32_t)(HAL_RCC_GetSysClockFreq() /
         //     (frecuencia * dac->size * (dac->htim->Instance->PSC + 1))) - 1;

    //dac->htim->Instance->ARR = calcula;

   // return calcula;
}
//seno con fase
void SENO_FASE(DACS *dac, float frecuencia, float amplitud, float offset, float fase_grados)
{

	if (amplitud > DAC_VREF) amplitud = DAC_VREF;
    if (offset < 0.0f) offset = 0.0f;
    if (offset > amplitud) offset = amplitud;

    float scale = (DAC_RESOLUCION - 1.0f) / DAC_VREF;

    // paso a radianes
    float fase_rad = fase_grados * (M_PI / 180.0f);

    for (uint32_t i = 0; i < dac->size; i++)
    {
        float angulo = (2.0f * M_PI * i) / dac->size;

    //hago desfase
        float voltaje_dac =
            offset + (amplitud - offset) *
            ((sinf(angulo + fase_rad) + 1.0f) * 0.5f);

        int32_t code = (int32_t)(voltaje_dac * scale);

        if (code < 0) code = 0;
        if (code > (int32_t)(DAC_RESOLUCION - 1))
            code = (int32_t)(DAC_RESOLUCION - 1);

        dac->buffer[i] = (uint32_t)code;
    }

    DAC_TimerConfig(dac, frecuencia);
}

void TRIANGULAR(DACS *dac,float frecuencia, float amplitud,float offset)
{

    if (amplitud > DAC_VREF) amplitud = DAC_VREF;
    if (offset < 0.0f) offset = 0.0f;
    if (offset > amplitud) offset = amplitud;

    float scale = (DAC_RESOLUCION - 1.0f) / DAC_VREF;

    for (uint32_t i = 0; i < dac->size; i++)
    {
        float t = (float)i / dac->size;

        float tri;
        if (t < 0.5f)
            tri = 2.0f * t;              // 0 → 1
        else
            tri = 2.0f * (1.0f - t);     // 1 → 0

        float voltaje_dac = offset + (amplitud - offset) * tri;

        int32_t code = (int32_t)(voltaje_dac * scale);

        if (code < 0) code = 0;
        if (code > (int32_t)(DAC_RESOLUCION - 1))
            code = (int32_t)(DAC_RESOLUCION - 1);

       dac->buffer[i] = (uint32_t)code;
    }


    DAC_TimerConfig(dac, frecuencia);
    /*
    calcula = (uint32_t)(HAL_RCC_GetSysClockFreq() /
              (frecuencia * dac->size * (dac->htim->Instance->PSC + 1))) - 1;

    dac->htim->Instance->ARR = calcula;
*/
  //  return calcula;
}

void DIENTE_SIERRA(DACS *dac,float frecuencia, float amplitud,float offset)
{

    if (amplitud > DAC_VREF) amplitud = DAC_VREF;
    if (offset < 0.0f) offset = 0.0f;
    if (offset > amplitud) offset = amplitud;

    float scale = (DAC_RESOLUCION - 1.0f) / DAC_VREF;

    for (uint32_t i = 0; i < dac->size; i++)
    {
        float t = (float)i / dac->size;   // 0 → 1

        float voltaje_dac = offset + (amplitud - offset) * t;

        int32_t code = (int32_t)(voltaje_dac * scale);

        if (code < 0) code = 0;
        if (code > (int32_t)(DAC_RESOLUCION - 1))
            code = (int32_t)(DAC_RESOLUCION - 1);

       dac->buffer[i] = (uint32_t)code;
    }

    DAC_TimerConfig(dac, frecuencia);
    /*
    calcula = (uint32_t)(HAL_RCC_GetSysClockFreq() /
              (frecuencia * dac->size * (dac->htim->Instance->PSC + 1))) - 1;

    dac->htim->Instance->ARR = calcula;
*/
    //return calcula;
}

void CUADRADA(DACS *dac,float frecuencia,float amplitud,float duty,float offset)
{

    if (duty < 0.0f) duty = 0.0f;
    if (duty > 100.0f) duty = 100.0f;

    float duty_norm = duty / 100.0f;

    if (amplitud > DAC_VREF) amplitud = DAC_VREF;
    if (offset < 0.0f) offset = 0.0f;
    if (offset > amplitud) offset = amplitud;

    float scale = (DAC_RESOLUCION - 1.0f) / DAC_VREF;

    for (uint32_t i = 0; i < dac->size; i++)
    {
        float t = (float)i / dac->size;

        float cuadrada = (t < duty_norm) ? 1.0f : 0.0f;

        float voltaje_dac =
            offset + (amplitud - offset) * cuadrada;

        int32_t code = (int32_t)(voltaje_dac * scale);

        if (code < 0) code = 0;
        if (code > (int32_t)(DAC_RESOLUCION - 1))
            code = (int32_t)(DAC_RESOLUCION - 1);

       dac->buffer[i] = (uint32_t)code;
    }
    DAC_TimerConfig(dac, frecuencia);
    /*
    calcula = (uint32_t)(HAL_RCC_GetSysClockFreq() /
              (frecuencia * dac->size * (dac->htim->Instance->PSC + 1))) - 1;

    dac->htim->Instance->ARR = calcula;

    return calcula;*/

}

//roto a la DERECHA un buffer n posiciones
void ROTA_DER(uint32_t *buffer, uint32_t size, uint32_t n)
{
    if (size == 0) return;

    n = n % size;
    if (n == 0) return;

    for (uint32_t i = 0; i < n; i++)
    {
        uint32_t temp = buffer[size - 1];

        for (int32_t j = size - 1; j > 0; j--)
        {
            buffer[j] = buffer[j - 1];
        }

        buffer[0] = temp;
    }
}
//roto a la IZQUEIRDA un buffer n posiciones
void ROTA_IZQ(uint32_t *buffer, uint32_t size, uint32_t n)
{
    if (size == 0) return;

    n = n % size;
    if (n == 0) return;

    for (uint32_t i = 0; i < n; i++)
    {
        uint32_t temp = buffer[0];

        for (uint32_t j = 0; j < size - 1; j++)
        {
            buffer[j] = buffer[j + 1];
        }

        buffer[size - 1] = temp;
    }
}

