/*
 * GPS_PROCESA.h
 *
 *  Created on: Jun 11, 2024
 *      Author: Alcides Ramos
 */

#ifndef LIBRERIAS_GPS_PROCESA_H_
#define LIBRERIAS_GPS_PROCESA_H_
#include "main.h"
#include "string.h"
#include "stdlib.h"
#include "stdint.h"
#include "math.h"
#include "stdio.h"



#define   size_gps  200
extern uint8_t GPS_buffer[size_gps];
extern uint8_t GPS_buffer1[sizeof(GPS_buffer)];

//definir variabel en el main
/*
 //GPS
uint8_t GPS_buffer[size_gps];
uint8_t GPS_buffer1[sizeof(GPS_buffer)];
extern float latitud, longitud,velocidad;
extern uint8_t hor_gps,min_gps,seg_gps,dia_gps,mes_gps,an_gps;

 */
// en el while
/*
          GPS_RMC(GPS_buffer1);
	      sprintf(texto,"GPS_TIME=%0u:%0u:%0u\r\n",hor_gps,min_gps,seg_gps);
	      uartx_write_text(&UART_LORA, texto);
	      sprintf(texto,"GPS_FECHA=%0u/%0u/%0u\r\n",dia_gps,mes_gps,an_gps);
	      uartx_write_text(&UART_LORA, texto);
	      sprintf(texto,"LAT=%.7f  LON=%.7f\r\n",latitud,longitud);
	      uartx_write_text(&UART_LORA, texto);
  */


uint8_t GPS_RMC(char *trama_gps);

#endif /* LIBRERIAS_GPS_PROCESA_H_ */
