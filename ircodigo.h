/*
 * ircodigo.h
 *
 *  Created on: Sep 21, 2024
 *      Author: ALCIDES_RAMOS
 */

// en el inicio poner estas variables

/*
//variables ir
#define IRSTART  0XB2411237
#define IRSTOP   0X5CAC5F85
*/

// en el main  si se quiere calibrar poner
//ir_calibra();

// en el while

/*
    if (flag_codigo==1)
	  {
	//	  printraw();
//		  ir_normaliza();
sprintf(texto,"cuenta=%d\r\n",cuenta);
//uartx_write_text(&huart1, texto);
	// printraw();
 analiza_ir();
  sprintf(texto,"codigo=%0X\r\n",codigo_ir);
  uartx_write_text(&huart1, texto);
  if (codigo_ir==IRSTART) uartx_write_text(&huart1, "START\r\n");
  if (codigo_ir==IRSTOP) uartx_write_text(&huart1, "STOP\r\n");
  ir_init();

	  }
 */
 // en el archivo stm32f....it incluir
// #include "ircodigo.h"

// en el void int ext
//ir_interrupcion();

//en el void del timer usado
//ir_timer_interrupcion();
//en el .ioc  el timer counter 8000 us

#include "main.h"
#include "uart.h"



// colocar timer usado
extern TIM_HandleTypeDef htim4;
#define TIMIR TIM4   //USA EL TIMER 4
#define htimIR  htim4 //USA EL TIMER 4
//define pin de interrpccion mirar en el .ioc
#define int_pinir EXTI9_5_IRQn
#define puerto serial a usar

//colocar puerto serial a usar
extern UART_HandleTypeDef huart1;
#define irport huart1


extern uint32_t codigo_ir; // variable salida has que almcena codigo unico
extern uint8_t  flag_codigo;
extern uint16_t   cuenta;
extern uint16_t tiempo[];
extern uint16_t min,max ;

#ifndef LIBRERIAS_IRCODIGO_H_
#define LIBRERIAS_IRCODIGO_H_


void ir_init();
void ir_timer_interrupcion();
void ir_interrupcion();

void printraw();
uint32_t  Hash_algoritmo ();
void analiza_ir();
void ir_calibra();
uint8_t ir_ordena();
void ir_normaliza();



#endif /* LIBRERIAS_IRCODIGO_H_ */
