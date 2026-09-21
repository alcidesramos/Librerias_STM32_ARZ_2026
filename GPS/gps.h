/*
 * GPS.h
 *
 *  Created on: Agosto 4  2026
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

#define  hor_utc -5  // define la hora de colombia utc -5

#define latitud1 10.42258
#define longitud1 -75.53930

#define presicion 200 //radio de operacion en metros


/*
 debe usarse en conjunto con UART_INT_DMA_IDLE.c UARTRX.c UARTRX.h
 //define en UARTRX.c  el puerto a usar y nombre d ela variable
  extern UART_HandleTypeDef huart2;

 //defina la variable asociada al gps
  UARTRXS GPS_UARTRX = {&huart2,USART2,1000}; //en que puerto y tamaño
  //VERIFICAR QUE esté, de esta manera en UARTRX.h
   extern UARTRXS   GPS_UARTRX;

  //coloque tambien el tamaño del buffer en GPS.c  y el UTC deseado

#define  hor_utc -5  // define la hora de colombia utc -5

//en el archivo UART_INT_DMA_IDLE.c incluya
  #include "gps.h"
//y llame en la interrupccion HAL_UARTEx_RxEventCallback  a
GPS_Interrupt(huart, Size);

y en HAL_UART_ErrorCallback  a
GPS_Error(huart);


  //en el main antes del while llame a
   uartRX_it_idle_dma_init(&GPS_UARTRX);

   y dentro del while puede leer los parmetor de gps ejemplo:
if(GPS_UARTRX.flag_rx==1) //si hay trama de gps
   {
 GPS_RMC();
     GPS_GGA();
     
		  uartRX_DMA_Re_init(&GPS_UARTRX);//recepcion serial para otra trama
		 
		 sprintf(texto,"LAT=%.5f   LON=%.5f\r\n",latitud,longitud);
		 uartx_write_text(&huart1,texto);

		 sprintf(texto,"A=%u  M=%u  D=%u\r\n",an_gps,mes_gps,dia_gps);
 	    uartx_write_text(&huart1,texto);

		 sprintf(texto,"hor=%u  min=%u  seg=%u\r\n",hor_gps,min_gps,seg_gps);
		 uartx_write_text(&huart1,texto);

		 sprintf(texto,"vel=%.2f\r\n",gps_vel_kph);
 		 uartx_write_text(&huart1,texto);


 		 sprintf(texto,"Rumbo=%.2f\r\n",gps_rumbo);
		 uartx_write_text(&huart1,texto);

		 sprintf(texto,"devi=%.2f\r\n\r\n",gps_desv_mag);
		 uartx_write_text(&huart1,texto);



		

			 sprintf(texto,"hor=%u  min=%u  seg=%u\r\n",hor_gps,min_gps,seg_gps);
			 uartx_write_text(&huart1,texto);

			 sprintf(texto,"LAT=%.5f   LON=%.5f\r\n",latitud,longitud);
			 uartx_write_text(&huart1,texto);
			 sprintf(texto,"MOD=%u  SAT=%u\r\n",gps_modo,gps_satelites);
		  uartx_write_text(&huart1,texto);


			 sprintf(texto,"hor_di=%.2f   ALTU=%.2f\r\n",gps_hor_dilu,gps_altura);
			 uartx_write_text(&huart1,texto);

       
       if(estaDentroDelRadio(latitud1, longitud1, latitud, longitud)) 
       {
           sprintf(texto,"Dentro del radio de operación\r\n");
           uartx_write_text(&huart1,texto);
           HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET); // Enciende el LED
       }
       else
       {
           sprintf(texto,"Fuera del radio de operación\r\n");
           uartx_write_text(&huart1,texto);
           HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET); // Apaga el LED
       }

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
void GPS_Error(UART_HandleTypeDef *huart);
double calcularDistanciaGPS(double lat1, double lon1, double lat2, double lon2);
uint8_t estaDentroDelRadio(double lat1, double lon1,double lat_gps, double lon_gps);


#endif /* LIBRERIAS_GPS_H_ */
