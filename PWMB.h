/*
 * PWM.H
 *
  *  Created on: Junio 9, 2024
 *      Author: Alcides Ramos
*/

//en el main
/*
  PWM_init(&PWM1);
  PWM_init(&PWM2);

// en el while

  PWM_valor(&PWM1, 10);//canal y valor
  PWM_valor(&PWM2, 90);//canal y valor

*/


#include "main.h"


#ifndef LIBRERIAS_PWM_H_
#define LIBRERIAS_PWM_H_

//estructura de pwm
typedef struct {
    TIM_HandleTypeDef *htim;
    volatile uint32_t *ccr;
    uint32_t channel;
} PWMS;

//canales pwm a usar
extern PWMS PWM1;
extern PWMS PWM2;
extern PWMS PWM3;
extern PWMS PWM4;
extern PWMS PWM5;
extern PWMS PWM6;
extern PWMS PWM7;
extern PWMS PWM8;
extern PWMS PWM9;
extern PWMS PWM10;
extern PWMS PWM11;
extern PWMS PWM12;
extern PWMS PWM13;
extern PWMS PWM14;
extern PWMS PWM15;
extern PWMS PWM16;
extern PWMS PWM17;
extern PWMS PWM18;
extern PWMS PWM19;
extern PWMS PWM20;

extern float escala;

void PWM_init(PWMS *pwm);
void PWM_stop(PWMS *pwm);
void PWM_valor(PWMS *pwm, float valor);
void PWM_valor_escala(PWMS *pwm, float valor,float escalar);
void PWM_RAW(PWMS *pwm, uint32_t valor);


#endif /* LIBRERIAS_PWM_H_ */
