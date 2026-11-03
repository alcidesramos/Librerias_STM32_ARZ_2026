/*
 * encoder.h
 *
 *  Created on: Mar 18, 2025
 *      Author: ALCIDES_RAMOS
 */

#ifndef LIBRERIAS_ENCODER_H_
#define LIBRERIAS_ENCODER_H_

#include "main.h"
extern uint16_t encoder_velo;

//como usar
/*
 //incluir en encoder.c los timer usados
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;

//configura encoder a usar
ENCODERS ENCODER1 = {&htim1,TIM1};
ENCODERS ENCODER2 = {&htim3,TIM3};

 //en encoder.c definir ENCODERx_DIR (signo posicion) y ENCODERx_VEL (signo velocidad)
 //y en el procedimiento Encoder_velo agregar una linea por cada encoder:
    Encoder_actualiza(&ENCODER1, ENCODER1_DIR, ENCODER1_VEL, Ts);

 */
/*
 en el archivo "stm32h5xx_it.c"
 en el procedimeinto SysTick_Handler
 llamar a
 Encoder_velo();
 e incluir #include "encoder.h"

 */
/*
 en el main .c
 //inicar los encoder  normalmente 4x pero si desea 2x  tambien puede y por ultimpo #bits el timer 16 o 32 bits
  Encoder_init(&ENCODER1, 4,16);
  Encoder_init(&ENCODER2, 4,32);

  // ya puede usar las variales dle encodre ejemplo
  sprintf(texto,"PUL1=%d  PUL2=%d  vel1=%.4f   vel2=%.4f\r\n\r\n",ENCODER1.posicion,ENCODER2.posicion,ENCODER1.velocidad,ENCODER2.velocidad);
  uartx_write_text(&huart1, texto);
  HAL_Delay(100);
 */
typedef struct {
    TIM_HandleTypeDef *htim;
    TIM_TypeDef *TIMX;
    uint32_t pulsos;      //valor crudo del contador del timer
    int32_t posicion;     //cuenta / resolucion (signo segun ENCODERx_DIR)
    uint16_t resolucion;  //4 = modo x4, 2 = modo x2
    int32_t velo;         //pulsos en el ultimo periodo de muestreo
    int32_t velo_ant;     //contador en la muestra anterior
    float velocidad;      //pulsos/resolucion por segundo (signo segun ENCODERx_VEL)
    uint8_t numbits;      //16 o 32 bits del timer
    int32_t cuenta;       //pulsos acumulados (signo segun ENCODERx_DIR)
} ENCODERS;

extern ENCODERS ENCODER1;
extern ENCODERS ENCODER2;
extern ENCODERS ENCODER3;
extern ENCODERS ENCODER4;

void Encoder_init(ENCODERS *encoder,uint16_t resol,uint8_t numebits);
void Encoder_velo();


#endif /* LIBRERIAS_ENCODER_H_ */
