/*
 *
 *
 * QTR.H
 *
 *  Created on: Jul 25, 2023
 *      Author: alcid
 */

#include "main.h"
#include "delays.h"
#include "puertos.h"
#include "bits.h"
#include "uart.h"


#define ir_negro 1
#define ir_blanco 0
#define rango_normaliza 1000//  se coloca rango a normalizar recomendable 1000
#define lin_blancolow 50
#define  lin_blacohigh 400
#define  lin_negro   500
///ojo aqui empezar a modificar

//si se desea se cambia manualmente el timer usado
extern TIM_HandleTypeDef htim8;
#define TIMQTR TIM8
#define hTIMQTR htim8

#define n_sensores 10   //numero d e sensores
#define muestro_ir 16000 //milisegubdos de muestreo ir
  //puerto uart a usar
extern UART_HandleTypeDef huart1;
#define QTR_UART huart1

extern uint16_t lecturasir[];

//en el main
//QTR_Init();
 //QTR_calibra(80);
 //  QTR_Manda_Limites();//opcional


//en el while

      //QTR_lectura();
     // QTR_Normaliza();
/*
	  sprintf(texto,"SEN1=%d\r\n",lecturas[0]);
	uartx_write_text(&huart1, texto);
	  sprintf(texto,"SEN2=%d\r\n",lecturas[1]);
		uartx_write_text(&huart1, texto);
*/

//se puede poner manual los pines de los sensores usados

#define LEDIR_Pin GPIO_PIN_13
#define LEDIR_GPIO_Port GPIOC

#define SIR1_Pin GPIO_PIN_0
#define SIR1_GPIO_Port GPIOA

#define SIR2_Pin GPIO_PIN_2
#define SIR2_GPIO_Port GPIOA

#define SIR3_Pin GPIO_PIN_3
#define SIR3_GPIO_Port GPIOA

#define SIR4_Pin GPIO_PIN_4
#define SIR4_GPIO_Port GPIOA

#define SIR5_Pin GPIO_PIN_4
#define SIR5_GPIO_Port GPIOC

#define SIR6_Pin GPIO_PIN_11
#define SIR6_GPIO_Port GPIOB

#define SIR7_Pin GPIO_PIN_12
#define SIR7_GPIO_Port GPIOB

#define SIR8_Pin GPIO_PIN_14
#define SIR8_GPIO_Port GPIOB

#define SIR9_Pin GPIO_PIN_9
#define SIR9_GPIO_Port GPIOA

#define SIR10_Pin GPIO_PIN_15
#define SIR10_GPIO_Port GPIOC






void QTR_Init();
void QTR_lectura();
void QTR_calibra(uint16_t veces);
void QTR_Manda_Lecturas();
void QTR_Manda_Limites();
void QTR_Normaliza();
void QTR_Normaliza_A();
float QTR_Calcula_Pesos();
float QTR_Calcula_Pesos_N();
float QTR_Calcula_Pesos_ARZ();

float analisis_pesos();
float calcularError();
