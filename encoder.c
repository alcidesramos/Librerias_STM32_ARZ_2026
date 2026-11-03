/*
 * encoder.c
 *
 *  Created on: Mar 18, 2025
 *      Author: ALCIDES_RAMOS
 */

#include  "encoder.h"

//incluir los timer usados
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

//configura encoder a usar
ENCODERS ENCODER1 = {&htim1,TIM1};
ENCODERS ENCODER2 = {&htim2,TIM2};

//define direccion de giro encoders (signo de la posicion)
#define ENCODER1_DIR   1
#define ENCODER2_DIR  1

//define signo de la velocidad
#define ENCODER1_VEL   1
#define ENCODER2_VEL   1

//PONGO el meustrteo para velocidad d elos encoder
#define ENCODER_TS_MS  20




uint16_t encoder_velo=0;   //contador de milisegundos para el muestreo

void Encoder_init(ENCODERS *encoder,uint16_t resol,uint8_t numebits)
{
  encoder->resolucion=0;   //mientras sea 0, Encoder_velo no toca este encoder

  HAL_TIM_Encoder_Start(encoder->htim,TIM_CHANNEL_ALL);   //sin interrupcion: se lee el contador cada ENCODER_TS_MS

  encoder->numbits=numebits;
  encoder->pulsos=__HAL_TIM_GET_COUNTER(encoder->htim);
  encoder->velo_ant=(int32_t)encoder->pulsos;   //evita un salto en la primera medicion
  encoder->velo=0;
  encoder->cuenta=0;
  encoder->posicion=0;
  encoder->velocidad=0;

  encoder->resolucion=resol;   //va al final: habilita el encoder en Encoder_velo
}

//lee el timer y actualiza posicion y velocidad de UN encoder
static void Encoder_actualiza(ENCODERS *encoder, int8_t dir, int8_t vel, float Ts)
{
  if (encoder->resolucion == 0) return;   //aun no inicializado

  uint32_t cnt = __HAL_TIM_GET_COUNTER(encoder->htim);
  int32_t diff;

  //diferencia de pulsos; el cast resuelve el desborde del contador
  if (encoder->numbits == 16)
    diff = (int16_t)((uint16_t)cnt - (uint16_t)encoder->velo_ant);
  else
    diff = (int32_t)(cnt - (uint32_t)encoder->velo_ant);

  encoder->velo_ant = (int32_t)cnt;
  encoder->pulsos   = cnt;     //valor crudo del timer
  encoder->velo     = diff;    //pulsos en el ultimo periodo

  //posicion acumulada (con signo ENCODERx_DIR)
  encoder->cuenta  += dir * diff;
  encoder->posicion = encoder->cuenta / encoder->resolucion;

  //velocidad (con signo ENCODERx_VEL)
  encoder->velocidad = (float)(vel * diff) / encoder->resolucion / Ts;
}

//Lo llamo cada 1 ms en (SysTick_Handler)
void Encoder_velo(void)
{
  const float Ts = ENCODER_TS_MS / 1000.0f;

  encoder_velo++;

  if (encoder_velo >= ENCODER_TS_MS)
  {
    encoder_velo = 0;

    //agregar una linea por cada encoder usado
    Encoder_actualiza(&ENCODER1, ENCODER1_DIR, ENCODER1_VEL, Ts);
    Encoder_actualiza(&ENCODER2, ENCODER2_DIR, ENCODER2_VEL, Ts);
  }
}
