/*
 * GPS.h
 *
 *  Created on: Apr 19, 2025
 *      Author: ALCIDES_RAMOS
 */

#ifndef LIBRERIAS_GPS_H_
#define LIBRERIAS_GPS_H_
#include "main.h"
#include "uart.h"
#include "UARTRX.h"
#include "string.h"
#include "stdlib.h"
#include "stdint.h"
#include "math.h"
#include "stdio.h"




/*
 debe usarse en conjunto con UART_INT_DMA_IDLE.c UARTRX.c UARTRX.h
 //define en UARTRX.c  el puerto a usar y nombre d ela variable
  extern UART_HandleTypeDef huart1;

 //defina la variable asociada al gps
  UARTRXS GPS_UARTRX = {&huart1,USART1,600}; //en que puerto y tamaño
  //VERIFICAR QUE esté, de esta manera en UARTRX.h
   extern UARTRXS   GPS_UARTRX;

  //coloque tambien el tamaño del buffer en GPS.c  y el UTC deseado

char GPS_buffer[600];  //tamaño buffer para  caprota d edatos gps
#define  hor_utc -5  // define la hora de colombia utc -5

//en el archivo UART_INT_DMA_IDLE.c incluya
  #include "GPS.h"
//y llame en la interrupccion HAL_UARTEx_RxEventCallback  a
 GPS_Interrupt(huart, Size);


  //en el main antes del while llame a
   uartRX_it_idle_dma_init(&GPS_UARTRX);

   y dentro del while puede leer los parmetor de gps ejemplo:


	  if (GPS_UARTRX.flag_rx==1)
	  	  {


		 // uartx_write_text(&huart2, UARTRX1.trama_rx);
		 GPS_RMC();
		  // procesa_rx(); //procedimeinto donde se procesan los datos

		 sprintf(texto,"LAT=%.5f   LON=%.5f\r\n",latitud,longitud);
		 uartx_write_text(&huart2,texto);

		 sprintf(texto,"A=%u  M=%u  D=%u\r\n",an_gps,mes_gps,dia_gps);
 	   uartx_write_text(&huart2,texto);

		 sprintf(texto,"hor=%u  min=%u  seg=%u\r\n",hor_gps,min_gps,seg_gps);
		 uartx_write_text(&huart2,texto);

		 sprintf(texto,"vel=%.2f\r\n",gps_vel_kph);
 		 uartx_write_text(&huart2,texto);


 		 sprintf(texto,"Rumbo=%.2f\r\n",gps_rumbo);
		 uartx_write_text(&huart2,texto);

		 sprintf(texto,"devi=%.2f\r\n\r\n",gps_desv_mag);
		 uartx_write_text(&huart2,texto);



		  GPS_GGA();

			 sprintf(texto,"hor=%u  min=%u  seg=%u\r\n",hor_gps,min_gps,seg_gps);
			 uartx_write_text(&huart2,texto);

			 sprintf(texto,"LAT=%.5f   LON=%.5f\r\n",latitud,longitud);
			 uartx_write_text(&huart2,texto);
			 sprintf(texto,"MOD=%u  SAT=%u\r\n",gps_modo,gps_satelites);
		  uartx_write_text(&huart2,texto);


			 sprintf(texto,"hor_di=%.2f   ALTU=%.2f\r\n",gps_hor_dilu,gps_altura);
			 uartx_write_text(&huart2,texto);


		  uartRX_DMA_Re_init(&GPS_UARTRX);
	  	  }



 */



extern double latitud, longitud,velocidad;
extern uint8_t min_gps,seg_gps,dia_gps,mes_gps,an_gps;
extern int8_t hor_gps;//
extern float gps_vel_nudos,gps_vel_kph,gps_rumbo,gps_desv_mag;
extern int8_t gps_modo,gps_satelites;
extern float gps_hor_dilu,gps_altura;




uint8_t GPS_RMC();
uint8_t GPS_GGA();
void GPS_Interrupt( UART_HandleTypeDef *huart,uint16_t sizex);

#endif /* LIBRERIAS_GPS_H_ */
