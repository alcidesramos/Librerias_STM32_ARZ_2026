/*
 * gps.c
 *
 *  Created on: 23/11/2021
 *      Author: Alcides Ramos
 */


#include "gps.h"
#include "stdlib.h"
#include "string.h"
#include "main.h"


extern UART_HandleTypeDef huart5;
unsigned char rx2_dato;
float latitud,longitud;
unsigned char trama_rx2[200];
unsigned char flag_valido=0;
unsigned char flag_rx2=0;


// GPS//   lo que se va a capturar se toma desde PGGA  HASTA *
//estados de la recepcion de variables
enum estados_  {cap_P,cap_R,cap_M,cap_C,esp_final} ;
float grados,minutos;
//estado inicial
unsigned  char estado_rx2=cap_P;
char procesa[100];
char c_grados[10];   // grados en cadena
unsigned char puntero_rx2=0;  ///   recorrer el vector tramarx


void gps_interrupcion()
{
   if (flag_rx2==0)// en caso que no se haya armado la trama
  {

	 HAL_UART_Receive_IT(&huart5, &rx2_dato, 1);// inicia otra lectura por interrpción

	          // ANSLIZA LOS ESTADOS
	           switch (estado_rx2)
	           {

	           case cap_P:

	        	          trama_rx2[puntero_rx2]=rx2_dato;
	                       if (rx2_dato!='P') puntero_rx2=0;    //no fue el dato que me interesa
	                       else
	                       {
	                          puntero_rx2++;//incrementa contador para proximo dato
	                          estado_rx2=cap_R; //pasa al estado de capturar la g
	                       }
	                     break;



	           case cap_R:

	                       trama_rx2[puntero_rx2]=rx2_dato;
	                       if (rx2_dato!='R')
	                       {
	                       puntero_rx2=0;
	                       estado_rx2=cap_P; //regresa al estado inical
	                       }
	                       else
	                       {
	                          puntero_rx2++;//incrementa contador para proximo dato
	                         estado_rx2=cap_M; //pasa al estado de capturar la g
	                       }
	                     break;

	           case cap_M:
	                       trama_rx2[puntero_rx2]=rx2_dato;
	                       if (rx2_dato!='M')
	                       {
	                       puntero_rx2=0;
	                       estado_rx2=cap_P; //regresa al estado inical
	                       }
	                       else
	                       {
	                          puntero_rx2++;//incrementa contador para proximo dato
	                          estado_rx2=cap_C; //pasa al estado de capturar la a
	                       }
	                     break;


	           case cap_C:
	                       trama_rx2[puntero_rx2]=rx2_dato;
	                       if (rx2_dato!='C')
	                       {
	                       puntero_rx2=0;
	                       estado_rx2=cap_P; //regresa al estado inical

	                       }
	                       else
	                       {
	                          puntero_rx2++;//incrementa contador para proximo dato
	                          estado_rx2=esp_final; //pasa al estado de capturar la u
	                       }

	                     break;

	           case esp_final:
	                       trama_rx2[puntero_rx2]=rx2_dato;
	                       puntero_rx2++;
	                       if (rx2_dato=='*')
	                       {
	                      trama_rx2[puntero_rx2]='\r';
	                      puntero_rx2++;
	                      trama_rx2[puntero_rx2]='\n';
	                     puntero_rx2++;
	                      trama_rx2[puntero_rx2]=0;

	                      HAL_NVIC_DisableIRQ(UART5_IRQn);//deshabilita interrpción serial
	                        estado_rx2=cap_P; //regresa al estado inical
	                        puntero_rx2=0;
	                        flag_rx2=1;

	                       }
	                       break;

	               }//fin del switch
       }// fin de si flagrx2==0


  }

unsigned char procesa_GPS_pos()
{
 //captura la latitud
   strcpy(procesa,strtok(trama_rx2, ","));  //inicia captura de tokens desde el =

   strcpy(procesa, strtok(0, ","));  //captura hasta segunda coma
    strcpy(procesa, strtok(0, ","));  //captura hasta lo que esta tercera coma es decir, latitud
       memset(c_grados,0,10);//limpía los grados
       strncpy(c_grados,procesa,2);//captura los grados
        grados=atof(c_grados);      // pasa de alfanumerico o cadena a flotante

          // apunta a los minutos
         minutos=atof(&procesa[2])/60.0;  //lo pasa a grados
           latitud=grados+minutos;
       strcpy(procesa, strtok(0, ","));  //captura siguiente coma  //orientacion o signo de la lat
       if (procesa[0]=='S') latitud=-latitud;
   //ACONDICIONA PARA ENVIAR
   //         sprintf(texto,"lat=%f\r\n",latitud);
   //   UART2_Write_Text(texto);


  //  captura la longitud
       strcpy(procesa, strtok(0, ","));  //captura siguiente coma la longitud
     memset(c_grados,0,10);//limpía los grados
       strncpy(c_grados,procesa,3);//captura los grados  3 posiciones
          grados=atof(c_grados);
    // apunta a los minutos
         minutos=atof(&procesa[3])/60.0;  //lo pasa a grados
           longitud=grados+minutos;
       strcpy(procesa, strtok(0, ","));  //captura siguiente coma  //orientacion o signo de la long
       if (procesa[0]=='W') longitud=-longitud;
          //  sprintf(texto,"lon=%f\r\n\r\n",longitud);
          //UART2_Write_Text(texto);

       strcpy(procesa, strtok(0, ","));  //captura siguiente coma  VALIDEZ
        flag_valido=0;
       if (procesa[0]=='1')   flag_valido=1;
        else if (procesa[0]=='2')   flag_valido=1;
      //  else if (procesa[0]=='4')   flag_valido=1;

        return flag_valido;

}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
 /* Prevent unused argument(s) compilation warning */
 UNUSED(huart);
 //HAL_GPIO_TogglePin(led_rojo_GPIO_Port, led_rojo_Pin);
 gps_interrupcion();
 //uartx_write(&huart5, rx_dato);// si se desea retransmitir



 /* NOTE : This function should not be modified, when the callback is needed,
           the HAL_UART_RxCpltCallback can be implemented in the user file.
  */
}
