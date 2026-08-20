/*
 * GPS_PA1010.c
 *
 *  Created on: Jun 10, 2024
 *      Author: Alcides Ramos
 */

#include "main.h"
#include "GPS_PA1010.h"


/*
 void  GPS_analiza()
{
	   // Lee los datos del GPS a través de I2C
	   memset(GPS_buffer,0,sizeof(GPS_buffer));
	   memset (GPS_buffer1,0,sizeof(GPS_buffer1));
	   uint8_t GPS_DAT = HAL_I2C_Master_Receive(&GPS_I2C, GPS_I2C_ADDRESS,GPS_buffer,  sizeof(GPS_buffer)-1 , 2000);
    if (GPS_DAT == HAL_OK)

     {
    	 usb_uart_write("GPS I2C OK \r\n");
    	 usb_uart_write(GPS_buffer);
    	 usb_uart_write("\r\n");

    	 STR_QUITA(0x0A,&GPS_buffer , &GPS_buffer1);
 	//   GPS_CMD( PMTK_SET_NMEA_OUTPUT_OFF);
 	 usb_uart_write("GPS PROCESANDO \r\n");
	 usb_uart_write(GPS_buffer1);
	 usb_uart_write("\r\n");


 	 if (strlen(GPS_buffer1)>195)
		  {

 		 usb_uart_write("TRAMA GPS OBTENIDA\r\n");

 		usb_uart_write(GPS_buffer1);
 		usb_uart_write( "\r\n\r\n");
					 //$GNRMC,023555.000,A,1023.1935,N,07529.5229,W,0.98,358.84,120624,,,A*6D
			  GPS_RMC(GPS_buffer1);
			  sprintf(texto,"GPS_TIME=%0u:%0u:%0u\r\n",hor_gps,min_gps,seg_gps);
			 // uartx_write_text(&UART_LORA, texto);
			  usb_uart_write(texto);
			  sprintf(texto,"GPS_FECHA=%0u/%0u/%0u\r\n",dia_gps,mes_gps,an_gps);
			 // uartx_write_text(&UART_LORA, texto);
			  usb_uart_write(texto);
     	  sprintf(texto,"LAT=%.7f  LON=%.7f\r\n",latitud,longitud);
			 // uartx_write_text(&UART_LORA, texto);
			  usb_uart_write(texto);

		  }
     }


}

 */

//miro si el GPS esta conectado
uint8_t GPS_I2C_CONNECT()
{
uint8_t GPS_LISTO = HAL_I2C_IsDeviceReady(&GPS_I2C, GPS_I2C_ADDRESS , 5, 5);
if (GPS_LISTO==HAL_OK)  return (1);
else return(0);

}

//envio comandos al GPS
void GPS_CMD(const char *comando)
{
 HAL_I2C_Master_Transmit(&GPS_I2C, GPS_I2C_ADDRESS, (uint8_t *)comando, strlen(comando), 500);

 }

void STR_QUITA( const char caract, char *src, char *dst)
{

    while (*src)

    {
        if (*src != caract)  *dst++ = *src;
        src++;
    }
    *dst = '\0';
}
