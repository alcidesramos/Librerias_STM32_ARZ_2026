/*
 * UBIDOTS.h
 *
 *  Created on: Jan 16, 2025
 *      Author: ALCIDES_RAMOS
 */

#ifndef SRC_LIBRERIAS_UBIDOTS_H_
#define SRC_LIBRERIAS_UBIDOTS_H_
/*
 Trabaja en conjunco con uart_int_idle_dma.h
 colocar tamaño de la trama de recepcion en 1000

 //en el while se calcula la rama  de variables y largo asi
    sprintf(ubidot_var,"{\"led\": %u,\"temperatura\": %.1f}",led,temperatura);
     ubidot_trama_size=strlen(ubidot_var);
   //luego se envia asi
    *

 */

#include "main.h"
#include "stdio.h"
#include  "stdint.h"
#include "stdlib.h"
#include "string.h"


#define Token "BBUS-yoe1NpdvyOzO4yDGvXnQxsObkau49K"
#define Device "SUSCRIBETE"
#define TIME_POST 4 //tiempo de espra respuest GET servidor en segundos
#define TIME_GET 3 //tiempo de espra respuest GET servidor   en segundos

//TRASNMISION
# define ubidot_tramavar_largo 100  //largo max trama armado de variables
# define ubidot_tramatx_size 500  //largo max trama a enviar
#define uartubidotreg USART1   //define puerto serial a analizar
#define uartubidot huart1


# define trama_rx_ubidots_size 1000  //coloque el tamaño del buffer recepcion





//extern char ubidots_tx[];
extern char ubidot_var[];
extern uint16_t ubidot_tramavar_size;

//extern char flag_rx_ubidots;
//extern uint8_t trama_rx_ubidots[];
//extern uint8_t procesa_ubidots[];

extern float ubidot_float;
extern UART_HandleTypeDef uartubidot;


void ubidots_dma_init();
void  Ubidots_Interrutp(UART_HandleTypeDef *huart);
void Ubidots_POST();
void Ubidots_GET(char *lee_var);
float Ubidot_GET_Valor();
#endif /* SRC_LIBRERIAS_UBIDOTS_H_ */
