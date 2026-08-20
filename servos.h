/*
 * servos.h
 *
 *  Created on: Apr 8, 2023
 *      Author: Alcides Ramos
 */


#ifndef LIBRERIAS_SERVOS_H_
#define LIBRERIAS_SERVOS_H_

#include "main.h"

//limites en  de los servos
#define ser_lim_inf_ms 	1.0
#define  ser_lim_sup_ms	2.0

//movimiento deseados del servo
#define  ser_inf		-90.0
#define  ser_sup 		90.0


//en el main
/*
  SERVO_init(&SERVO1);
  SERVO_init(&SERVO2);

// en el while


SERVO_ANG(&SERVO1,30);   //saca en angulo
SERVO_ANG(&SERVO2,-30);   //saca en angulo

SERVO_MILI(&SERVO1,1.3);//  saca en milis
//si se quiere mover gradual
           servo    ini   fin  paso  del(velocidad)
 ERVO_MUEVE(&SERVO1, s1_, s1, 10, 50);
 
*/



//estructura de pwm
typedef struct {
    TIM_HandleTypeDef *htim;
    volatile uint32_t *ccr;
    uint32_t channel;
} SERVOS;

//SERVOS  a usar
extern SERVOS SERVO1;
extern SERVOS SERVO2;
extern SERVOS SERVO3;
extern SERVOS SERVO4;
extern SERVOS SERVO5;
extern SERVOS SERVO6;
extern SERVOS SERVO7;
extern SERVOS SERVO8;
extern SERVOS SERVO9;
extern SERVOS SERVO10;
extern SERVOS SERVO11;
extern SERVOS SERVO12;
extern SERVOS SERVO13;
extern SERVOS SERVO14;
extern SERVOS SERVO15;
extern SERVOS SERVO16;
extern SERVOS SERVO17;
extern SERVOS SERVO18;
extern SERVOS SERVO19;
extern SERVOS SERVO20;
extern float s1,s2,s3,s4;
extern float s1_,s2_,s3_,s4_;



void  SERVO_init(SERVOS *servo);
void SERVO_ANG(SERVOS *servo,float posi);
void SERVO_MICRO(SERVOS *servo,float micros);
void SERVO_MILI(SERVOS *servo,float milis);
void SERVO_MUEVE(SERVOS *servo,float ini, float final,float paso,float ret);


/*
s1=DATOS;
SERVO_MUEVE(&SERVO1, s1_, s1, 10, 50);
 s1_=s1;
*/




#endif /* LIBRERIAS_SERVOS_H_ */
