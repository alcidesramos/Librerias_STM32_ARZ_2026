/*
 * ws2812b.c
 *
 *  Created on: Oct 7, 2024
 *      Author: ALCIDES_RAMOS
 */

#include "ws2812b.h"


//matrix para almacenar datos del # led y RGB_Color
uint8_t LED_RGB_Color[led_num][4];
//matriz si se quiere guardar el brillo d elso led
uint8_t LED_BRILLO[led_num][4];
//tamaño buffer para almacener los PWM que saldran por DMA
#define trama_largo (24*led_num)+1+41 //si se coloca 0 al inicio y  41 al final(50us)
uint16_t PWM_Trama[trama_largo];//  para ubicar un cero al final;


extern float brilloled;


//carga un RGB_Color deseado al led desado
void WS2812_LED_N_Color (uint8_t LED_N, uint8_t rojo, uint8_t verde, uint8_t azul)
{
	LED_RGB_Color[LED_N][0] = LED_N;
	LED_RGB_Color[LED_N][1] = verde;
	LED_RGB_Color[LED_N][2] = rojo;
	LED_RGB_Color[LED_N][3] = azul;

}
//  funcion para aplicar el brillo con la funcion gamma

#define GAMMA_CORRECTION 2.2f  // Valor de corrección gamma adpatada al ojo humano
#define MAX_BRIGHTNESS 255   //valor maximo del brillo todo encendido

#include <math.h>  // Para usar la función pow()

#define GAMMA 2.2f  // Valor de corrección gamma
#define MAX_BRIGHTNESS 255  // Valor máximo de brillo

uint8_t Gamma_correccion(uint8_t color, float brillo_)
{
    // Limitar el valor de brillo a un rango de 0 a 100
    if (brillo_ > 100.0f) brillo_ = 100.0f;
    if (brillo_ < 0.0f) brillo_ = 0.0f;

    // Normalizo el brillo al rango de 0.0 a 1.0
    float brillo_Factor = brillo_ / 100.0f;

    // aplica el facot
    float adjustedColor = color * brillo_Factor;

    // formula de escalamiento gamma
    float gammaCorrectedColor = pow((adjustedColor / 255.0f), GAMMA) * 255.0f;

    //analiza que este dentro los limites
    if (gammaCorrectedColor > 255.0f) gammaCorrectedColor = 255.0f;
    if (gammaCorrectedColor < 0.0f) gammaCorrectedColor = 0.0f;

    // lo regresa como 8 bit escalado
    return (uint8_t)gammaCorrectedColor;
}

void WS2812_Arma_trama ()
{
	uint32_t cont_led=1;
	uint32_t RGB_Color=0;


	for (uint16_t i= 0; i<led_num; i++)
	{
#ifdef brillo
		LED_BRILLO[i][0] = LED_RGB_Color[i][0];
		for (uint8_t j=1; j<4; j++)
		{
			// Aplicar corrección gamma y brillo
	  	int8_t  brill=Gamma_correccion(LED_RGB_Color[i][j], brilloled);//micros mas memoria
		//int8_t  brill=LED_RGB_Color[i][j]*brilloled/100.0;//,icros poca memoria
			LED_BRILLO[i][j] = brill;
		}

		RGB_Color = (((uint32_t)LED_BRILLO[i][1]<<16) | ((uint32_t)LED_BRILLO[i][2]<<8) | ((uint32_t)LED_BRILLO[i][3]));

#else
		RGB_Color = (((uint32_t)LED_RGB_Color[i][1]<<16) | ((uint32_t)LED_RGB_Color[i][2]<<8) | ((uint32_t)LED_RGB_Color[i][3]));
#endif

		for (int16_t i1=23; i1>=0; i1--)
		{
			if (RGB_Color&((uint32_t)1<<i1))
			{
				PWM_Trama[cont_led] = PWM_1;  //valor del 1
			}

			else PWM_Trama[cont_led] = PWM_0; // valor del cero

			cont_led++;
        }//fin for i=23

	}//for i<led_num

	for(uint8_t i=1;i<=41;i++)
		{
		PWM_Trama[cont_led] = 0;//coloca 50 cero al final
		cont_led++;
		}

	PWM_Trama[0] = 0;//coloca un cero al inicio
}

void WS2812_Manda_Trama ()
{
	WS2812_Arma_trama();
	HAL_TIM_PWM_Start_DMA(& ws2812_timer, ws2812_canal, (uint32_t *) PWM_Trama, trama_largo);
   // HAL_Delay(1);
}

void WS2812_RESET ()
{

	for (uint8_t i=0; i<led_num; i++)
	{
		LED_RGB_Color[i][0] = i;
		LED_RGB_Color[i][1] = 0;
		LED_RGB_Color[i][2] = 0;
		LED_RGB_Color[i][3] = 0;
	}
}
void WS2812_Color(uint8_t rojo, uint8_t verde, uint8_t azul)
{

	for (uint8_t i=0; i<led_num; i++)
	{
		LED_RGB_Color[i][0] = i;
		LED_RGB_Color[i][1] = verde;
		LED_RGB_Color[i][2] = rojo;
		LED_RGB_Color[i][3] = azul;
	}
}

uint16_t paso=0;
uint8_t ws2812_rainbown_izq() {
    // Strip ID: 0 - Effect: Rainbow - LEDS: 12
    // Steps: 12 - Delay: 21
    // Colors: 3 (255.0.0, 0.255.0, 0.0.255)
    // Options: rainbowlen=12, toLeft=true,
  HAL_Delay(40);
  float factor1, factor2;
  uint16_t ind;

  for(uint16_t j=0;j<12;j++) {
    ind = paso + j * 1;
    switch((int)((ind % 12) / 4)) {
      case 0: factor1 = 1.0 - ((float)(ind % 12 - 0 * 4) / 4);
              factor2 = (float)((int)(ind - 0) % 12) / 4;
              WS2812_LED_N_Color(j, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2);
              WS2812_Manda_Trama();
              break;
      case 1: factor1 = 1.0 - ((float)(ind % 12 - 1 * 4) / 4);
              factor2 = (float)((int)(ind - 4) % 12) / 4;
              WS2812_LED_N_Color(j, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2);
              WS2812_Manda_Trama();
              break;
      case 2: factor1 = 1.0 - ((float)(ind % 12 - 2 * 4) / 4);
              factor2 = (float)((int)(ind - 8) % 12) / 4;
              WS2812_LED_N_Color(j, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2);
              WS2812_Manda_Trama();
              break;
    }
  }
  if(paso >= 12) {paso=0; return 0x03; }
  else paso++;
  return 0x01;
}


uint8_t  ws2812_rainbown_der() {
    // Strip ID: 0 - Effect: Rainbow - LEDS: 12
    // Steps: 12 - Delay: 21
    // Colors: 3 (255.0.0, 0.255.0, 0.0.255)
    // Options: rainbowlen=12, toLeft=false,
 // if(millis() - strip_0.effStart < 21 * (paso)) return 0x00;
  HAL_Delay(40);
	float factor1, factor2;
  uint16_t ind;
  for(uint16_t j=0;j<12;j++) {
    ind = 12 - (uint16_t)(paso - j * 1) % 12;
    switch((int)((ind % 12) / 4)) {
      case 0: factor1 = 1.0 - ((float)(ind % 12 - 0 * 4) / 4);
              factor2 = (float)((int)(ind - 0) % 12) / 4;
              WS2812_LED_N_Color (j, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2);
              WS2812_Manda_Trama();
              break;
      case 1: factor1 = 1.0 - ((float)(ind % 12 - 1 * 4) / 4);
              factor2 = (float)((int)(ind - 4) % 12) / 4;
              WS2812_LED_N_Color (j, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2);
              WS2812_Manda_Trama();
              break;
      case 2: factor1 = 1.0 - ((float)(ind % 12 - 2 * 4) / 4);
              factor2 = (float)((int)(ind - 8) % 12) / 4;
              WS2812_LED_N_Color (j, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2);
              WS2812_Manda_Trama();
              break;
    }
  }
  if(paso >= 12) {paso=0; return 0x03; }
  else paso++;
  return 0x01;
}


