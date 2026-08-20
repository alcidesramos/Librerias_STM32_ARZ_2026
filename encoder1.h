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
 //incluir encoder.c los timer usados
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;

//configura encoder a usar
ENCODERS ENCODER1 = {&htim1,TIM1};
ENCODERS ENCODER2 = {&htim3,TIM3};

 //en el procedimeinto Encoder_velo
  incluir los encoder a analizar velocidad
  		//repetir por cada encoder que se quiera saber velocidad
		ENCODER2.velo=ENCODER2.velo-ENCODER2.velo_ant;
		ENCODER2.velo_ant=ENCODER2.velo;
		ENCODER2.velocidad=ENCODER2.velo/0.5; //2200ms 0.2seg

	    //colocar otro encoder si se desea
			ENCODER1.velo=ENCODER1.velo-ENCODER1.velo_ant;
			ENCODER1.velo_ant=ENCODER1.velo;
			ENCODER1.velocidad=ENCODER1.velo/0.5; //500ms  0.5seg


			//va al final
			encoder_velo=0;//limpia el encoder

 */
/*
 en el archivo "stm32f4xx_it.c"
 en el procedimeinto SysTick_Handler
 llamar a
 Encoder_velo();
 e incluir #include "encoder.h"

 */
/*
 en el main .c
 //inicar los encoder  normalmente 4x pero si desea 2x  buen puede
  Encoder_init(&ENCODER1, 4);
  Encoder_init(&ENCODER2, 4);

  // ya puede usar las variales dle encodre ejemplo
   sprintf(texto,"PUL1=%d  PUL2=%d  vel1=%.1f\r\n\r\n",ENCODER2.posicion,ENCODER1.posicion,ENCODER2.velocidad);
  uartx_write_text(&huart1, texto);
  HAL_Delay(100);
 */
typedef struct {
    TIM_HandleTypeDef *htim;
    uint32_t TIMX;
    uint32_t pulsos;
    int16_t posicion;
    uint16_t resolucion;
    int16_t velo;
    int16_t velo_ant;
    float velocidad;
} ENCODERS;

extern ENCODERS ENCODER1;
extern ENCODERS ENCODER2;
extern ENCODERS ENCODER3;
extern ENCODERS ENCODER4;

void Encoder_init(ENCODERS *encoder,uint16_t resol);
void Encoder_velo();


#endif /* LIBRERIAS_ENCODER_H_ */
