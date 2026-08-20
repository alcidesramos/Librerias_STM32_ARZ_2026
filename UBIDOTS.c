/*
 * UBIDOTS.c
 *
 *  Created on: Jan 16, 2025
 *      Author: ALCIDES_RAMOS
 */


#include "UBIDOTS.h"

/*

 en este archivo uart.int_idle.c el puerto de datos debe ser diferente al de ubidots
 en el archivo uart.int_idle.c  poner en el void HAL_UARTEx_RxEventCallback(

 	#include "UBIDOTS.h"
	Ubidots_Interrutp(huart);

 en el ubidot.h  colocar  los token y tiempos en segundos de espera respeusta servidor

#define Token "BBUS-yoe1NpdvyOzO4yDGvXnQxsObkau49K"
#define Device "SUSCRIBETE"
#define TIME_POST 4 //tiempo de espra respuest GET servidor en segundos
#define TIME_GET 3 //tiempo de espra respuest GET servidor   en segundos


  // en el main
   incluir #include "UBIDOTS.h"

   //en el while llamar al post y el get
    //ARMA PRIMERO TRAMA DE VARIABLES DEL PSOT
     *   //ARMA TRAMA DE VARIABLES
     sprintf(ubidot_var,"{\"led\": %u,\"temperatura\": %.1f}",led,temperatura);
     ubidot_tramavar_size=strlen(ubidot_var);

          Ubidots_POST();

       Ubidots_GET("pwm");
      ubidot_float= Ubidot_GET_Valor();
      PWM_valor(&PWM1, (uint32_t)ubidot_float);

     sprintf(texto,"pwm=%.1f\r\n\r\n",ubidot_float);
     uartx_write_text(&huart2, texto);



 */
//Transmision
char ubidot_var[ubidot_tramavar_largo];
uint16_t ubidot_tramavar_size;
char ubidots_tx[ubidot_tramatx_size];


//recepcion

uint8_t trama_rx_ubidots[trama_rx_ubidots_size];
char flag_rx_ubidots=0;
uint8_t procesa_ubidots[20];

float ubidot_float;
void  ubidots_dma_init()
{
    memset(trama_rx_ubidots,0,trama_rx_ubidots_size);//limpia el  buffer

// en STM32G y stmH
   // uint8_t dummy = uartdatosreg->RDR;
    //uartdatosreg->ICR = USART_ICR_ORECF;  // Escribir 1 en ORECF para limpiar ORE

   //  en STM32F
    uartubidotreg->SR &= ~( UART_FLAG_RXNE | UART_FLAG_ORE);
  	uint8_t  dummy = uartubidotreg->DR;

    HAL_UARTEx_ReceiveToIdle_DMA(&uartubidot, (uint8_t*)trama_rx_ubidots, trama_rx_ubidots_size);//inica la recepcion por idle
}



void  Ubidots_Interrutp(UART_HandleTypeDef *huart)
{
	//si la interrpcion es por ubidtos
		if ((flag_rx_ubidots==0)&& (huart->Instance == uartubidotreg))//si es el uart de datos
				{
				HAL_UART_DMAStop(&uartubidot);  //para la recepcion temporarmente
				flag_rx_ubidots=1;
			    	}

}

void Ubidot_delay(uint16_t tiem)
{
  for(uint16_t cuen=0;cuen<tiem*1000;cuen++)
  {
   HAL_Delay(1);
   if (flag_rx_ubidots==1) break;
  }

}


void Ubidots_POST()
{
	 ubidots_dma_init();
	memset(ubidots_tx,0, ubidot_tramatx_size);
	sprintf(ubidots_tx,"POST /api/v1.6/devices/%s/?token=%s HTTP/1.1\r\n"
   	                   "Host: things.ubidots.com\r\n"
	                   "Content-Type: application/json\r\n"
	                   "Content-Length: %d\r\n"
	                   "\r\n"
			            "%s"
			            ,Device,Token,ubidot_tramavar_size,ubidot_var);
	flag_rx_ubidots=0;
	uartx_write_text(&uartubidot, ubidots_tx);//enva por el uart deseado la trama ubidots
    	Ubidot_delay(TIME_POST);

}

void Ubidots_GET(char *lee_var)
{
	  ubidots_dma_init();
	  memset(ubidots_tx,0, ubidot_tramatx_size);
	  sprintf(ubidots_tx,"GET /api/v1.6/devices/%s/%s/values?page_size=1&token=%s HTTP/1.1\n"
	     	                   "Host: things.ubidots.com\n\n"
	  	                      ,Device,lee_var,Token);
	  flag_rx_ubidots=0;
	  uartx_write_text(&uartubidot, ubidots_tx);//enva por el uart deseado la trama ubidots
	   Ubidot_delay(TIME_GET);

}

float Ubidot_GET_Valor()
{
//	extern UART_HandleTypeDef huart2;
	     if(flag_rx_ubidots==1)
	     {
	    	//uartx_write_text(&huart2,trama_rx_ubidots);
	    	// uartx_write_text(&huart2, "\r\n\r\n");
	    	 flag_rx_ubidots=0;
             char *punte;
             punte=strstr(trama_rx_ubidots,"lue\"");//apunta al valor
	    	 if (punte)//si existe la pal;abra lue
	    	 	    {
	    		       strcpy(procesa_ubidots, strtok(punte, ":"));  //inicia captura de tokens desde el =
	    		       strcpy(procesa_ubidots, strtok(0, ","));  //captura hasta el /
	    		       //uartx_write_text(&huart2,procesa_ubidots);
	    		       //uartx_write_text(&huart2, "\r\n");
	    		       float valorok=atof(procesa_ubidots);
	    		 	   return(valorok);
	    	 	    }

	     return(-2.0);
	     }

	     return(-1.0);
}
