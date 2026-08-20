/*
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


#define negro 1
#define blanco 0

//en el main declarar
//uint16_t lecturas[8];


//en el main
//QTR_Init();
 //QTR_calibra(80);
 //  QTR_Manda_Limites();//opcional


//en el while

      //QTR_lectura(lecturas);
     // QTR_Normaliza(lecturas);
/*
	  sprintf(texto,"SEN1=%d\r\n",lecturas[0]);
	uartx_write_text(&huart1, texto);
	  sprintf(texto,"SEN2=%d\r\n",lecturas[1]);
		uartx_write_text(&huart1, texto);
*/


///ojo aqui empezar a modificar

//si se desea se cambia manualmente el timer usado

//#define TIMIR TIM9
//#define hTIMIR htim9



//se puede poner manual los pines de los sensores usados

/*
#define SIR1_Pin GPIO_PIN_0
#define SIR1_GPIO_Port GPIOB

#define SIR2_Pin GPIO_PIN_1
#define SIR2_GPIO_Port GPIOB

#define SIR3_Pin GPIO_PIN_2
#define SIR3_GPIO_Port GPIOB

#define SIR4_Pin GPIO_PIN_3
#define SIR4_GPIO_Port GPIOB

#define SIR5_Pin GPIO_PIN_4
#define SIR5_GPIO_Port GPIOB

#define SIR6_Pin GPIO_PIN_5
#define SIR6_GPIO_Port GPIOB

#define SIR7_Pin GPIO_PIN_6
#define SIR7_GPIO_Port GPIOB

#define SIR8_Pin GPIO_PIN_7
#define SIR8_GPIO_Port GPIOB
*/
  //puerto uart a usar
extern UART_HandleTypeDef huart1;

extern TIM_HandleTypeDef hTIMIR;

void QTR_Init();
void QTR_lectura(uint16_t *lectu_QTR);
void QTR_calibra(uint16_t veces);
void QTR_Manda_Limites();
void QTR_Normaliza(uint16_t *lectu_QTR);

