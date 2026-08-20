/*
 * ws2812b.h
 *
 *  Created on: Oct 7, 2024
 *      Author: ALCIDES_RAMOS
 */

#ifndef LIBRERIAS_WS2812B_H_
#define LIBRERIAS_WS2812B_H_

#include "main.h"
#include "math.h"


//Numero de leds
#define led_num 6

//valores de ancho de pulso del uno y el cero
#define PWM_1  136
#define PWM_0  68
//colocar el timer y canal usado
extern TIM_HandleTypeDef htim1;
//coloque el timer usado
#define ws2812_timer htim1
#define ws2812_canal TIM_CHANNEL_3

//#define brillo  // si se quiere trabajar con brillo



void WS2812_LED_N_Color (uint8_t LED_N, uint8_t rojo, uint8_t verde, uint8_t azul);
void WS2812_BRILLO (float brillo_);
void WS2812_Arma_trama ();
void WS2812_Manda_Trama ();
void WS2812_RESET ();
void WS2812_Color(uint8_t rojo, uint8_t verde, uint8_t azul);

uint8_t Gamma_correccion(uint8_t color, float brillo_);

uint8_t ws2812_rainbown_izq();
uint8_t ws2812_rainbown_der();

#endif /* LIBRERIAS_WS2812B_H_ */
