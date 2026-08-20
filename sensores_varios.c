/*
 * PMS7003.c
 *
 *  Created on: Dec 20, 2023
 *      Author: Alcides Ramos
 */

#include "sensores_varios.h"
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart4;

#define UARTM2_2 huart4
#define UARTM2_3 huart4
#define UARTM2_4 huart4



#include "uart.h"

//VARIABLES
/*
 //ZHO6
uint16_t ZHO6_1_0,ZHO6_2_5,ZHO6_10,ZHO6_checsum;
//CO2
uint16_t CO2,CO2_checsum;
//ALCOHOL
uint16_t ALCOHOL,ALCOHOL_checsum;
uint8_t checsum=0;

 */

//MAIN
/*
ZHO6_PASIVO();//PASA EL ZH06 A MODO PASIVO
//CO2
CO2_PASIVO();//PASA EL CO2 A MODO PASIVO
//ALCOHOL
ALCOHOL_PASIVO();//PASA EL PM57003 A MODO PASIVO
*/
//SE USAN CON IDEL DETECT INT
/*\
 //Sensor ZHO6
	        uartdat_it_idle_dma_init();
	        ZHO6_READ();  //LEE EL SENSOR
	       	HAL_Delay(100);// ESPERA LA RESPUeSTA
	        if(flag_rx==1)  // SI HAY RESPeUSTA DELS ENRO
	             	{
	       	        checsum=0;
	           	  //calcula el checksum
	           	  for (uint8_t cont=1;cont<8;cont++) checsum=checsum+trama_rx1[cont];
                  checsum= ~checsum+1;//INVIERTE Y SUMA UNO
	           //	  for (uint8_t cont=0;cont<9;cont++) uartx_write(&UART_LORA, trama_rx1[cont]);
                   //ARMA LOS DATOS
	           	   ZHO6_2_5=(uint16_t)trama_rx1[2]<<16 | trama_rx1[2];
	               ZHO6_10=(uint16_t)trama_rx1[4]<<16 | trama_rx1[5];
	               ZHO6_1_0=(uint16_t)trama_rx1[6]<<16 | trama_rx1[7];
	               ZHO6_checsum= trama_rx1[8];

	       //valida informacion del sensor
	           if(checsum==ZHO6_checsum)
	       {
	            sprintf(texto,"PM2.5=%u\r\n",ZHO6_2_5);
	             uartx_write_text(&UART_LORA, texto);
	             sprintf(texto,"PM1.0=%u\r\n",ZHO6_1_0);
	              uartx_write_text(&UART_LORA, texto);
	              sprintf(texto,"PM10=%u\r\n",ZHO6_10);
	             uartx_write_text(&UART_LORA, texto);
	              uartx_write_text(&UART_LORA, "\r\n");

	       }
	          		flag_rx=0;
	          }
   //SENSOR DE C02
	       	        uartdat_it_idle_dma_init();
	       	        CO2_READ();  //LEE EL SENSOR
	       	       	HAL_Delay(100);// ESPERA LA RESPUeSTA
	       	        if(flag_rx==1)  // SI HAY RESPeUSTA DELS ENRO
	       	             	{
	       	       	        checsum=0;
	       	           	  //calcula el checksum
	       	           	  for (uint8_t cont=1;cont<8;cont++) checsum=checsum+trama_rx1[cont];
	                         checsum= ~checsum+1;//INVIERTE Y SUMA UNO
	       	           //	  for (uint8_t cont=0;cont<9;cont++) uartx_write(&UART_LORA, trama_rx1[cont]);
	                          //ARMA LOS DATOS
	       	           	   CO2=(uint16_t)trama_rx1[2]<<16 | trama_rx1[2];
	       	               CO2_checsum= trama_rx1[8];

	       	       //valida informacion del sensor
	       	           if(checsum==CO2_checsum)
	       	       {
	       	            sprintf(texto,"CO2=%u\r\n",CO2);
	       	             uartx_write_text(&UART_LORA, texto);
	       	              uartx_write_text(&UART_LORA, "\r\n");
	       	       }
	       	          		flag_rx=0;
	       	          }

	       //SENSOR DE ALCOHOL
	       	   	 uartdat_it_idle_dma_init();
	       	   	 ALCOHOL_READ();  //LEE EL SENSOR
	       	   	 HAL_Delay(100);// ESPERA LA RESPUeSTA
	       	   	 if(flag_rx==1)  // SI HAY RESPeUSTA DELS ENRO
	       	   	  {
	       	   	   checsum=0;
	       	   	  //calcula el checksum
                   for (uint8_t cont=1;cont<8;cont++) checsum=checsum+trama_rx1[cont];
	       	   	    checsum= ~checsum+1;//INVIERTE Y SUMA UNO
	       	   	    //	  for (uint8_t cont=0;cont<9;cont++) uartx_write(&UART_LORA, trama_rx1[cont]);
	       	   	     //ARMA LOS DATOS
	       	   	       ALCOHOL=(uint16_t)trama_rx1[6]<<16 | trama_rx1[7];
	       	   	       ALCOHOL_checsum= trama_rx1[8];
	       	   	      //valida informacion del sensor
	       	   	      if(checsum==ALCOHOL_checsum)
	       	   	       {
	       	   	       	sprintf(texto,"ALCOHOL=%u\r\n",CO2);
	       	   	       	uartx_write_text(&UART_LORA, texto);
	       	   	        uartx_write_text(&UART_LORA, "\r\n");
	       	   	       }
	       	   	       	flag_rx=0;
	       	   	       }

 */

const  uint8_t PMS7003_CMD_PASIVO []={0x42,0x4D,0xE1,0x00,0x00,0x01,0x70};
const  uint8_t PMS7003_CMD_ACTIVO []={0x42,0x4D,0xE1,0x00,0x01,0x01,0x71};
const  uint8_t PMS7003_CMD_READ   []={0x42,0x4D,0xE2,0x00,0x00,0x01,0x71};

//ZHO6 Material particulado
const  uint8_t  ZHO6_CMD_ACTIVO []={0xFF,0x01,0x78,0x40,0x00,0x00,0x00,0x00,0X47};
const  uint8_t  ZHO6_CMD_PASIVO []={0xFF,0x01,0x78,0x41,0x00,0x00,0x00,0x00,0X46};
const  uint8_t  ZHO6_CMD_READ []={0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0X79};
//CO2
const  uint8_t  CO2_CMD_ACTIVO []= {0xFF,0x01,0x78,0x40,0x00,0x00,0x00,0x00,0X47};
const  uint8_t  CO2_CMD_PASIVO []= {0xFF,0x01,0x78,0x41,0x00,0x00,0x00,0x00,0X46};
const  uint8_t  CO2_CMD_READ []=   {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0X79};

//ALCOHOL
const  uint8_t  ALCOHOL_CMD_PASIVO []={0xFF,0x01,0x78,0x41,0x00,0x00,0x00,0x00,0X46};
const  uint8_t  ALCOHOL_CMD_READ []={0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0X79};
const  uint8_t  ALCOHOL_CMD_ACTIVO []={0xFF,0x01,0x78,0x40,0x00,0x00,0x00,0x00,0X47};



void ZHO6_PASIVO()
{
for (uint8_t cont=0;cont<9;cont++) uartx_write(&UARTM2_2, ZHO6_CMD_PASIVO[cont]);

}

void ZHO6_READ()
{
for (uint8_t cont=0;cont<9;cont++) uartx_write(&UARTM2_2, ZHO6_CMD_READ[cont]);
}

void ZHO6_ACTIVO()
{
for (uint8_t cont=0;cont<9;cont++) uartx_write(&UARTM2_2, ZHO6_CMD_ACTIVO[cont]);
}

//C02
void CO2_READ()
{
for (uint8_t cont=0;cont<9;cont++) uartx_write(&UARTM2_3, CO2_CMD_READ[cont]);
}

void CO2_PASIVO()
{
for (uint8_t cont=0;cont<9;cont++) uartx_write(&UARTM2_3, CO2_CMD_PASIVO[cont]);
}

void CO2_ACTIVO()
{
for (uint8_t cont=0;cont<9;cont++) uartx_write(&UARTM2_3, CO2_CMD_ACTIVO[cont]);
}

//Alcohol

void ALCOHOL_PASIVO()
{
for (uint8_t cont=0;cont<9;cont++) uartx_write(&UARTM2_4, ALCOHOL_CMD_PASIVO[cont]);

}

void ALCOHOL_READ()
{
for (uint8_t cont=0;cont<9;cont++) uartx_write(&UARTM2_4, ALCOHOL_CMD_READ[cont]);
}

void ALCOHOL_ACTIVO()
{
for (uint8_t cont=0;cont<9;cont++) uartx_write(&UARTM2_4, ALCOHOL_CMD_ACTIVO[cont]);
}



void PMS7003_PASIVO()
{
for (uint8_t cont=0;cont<7;cont++) uartx_write(&UARTM2_3, PMS7003_CMD_PASIVO[cont]);
}

void PMS7003_ACTIVO()
{
for (uint8_t cont=0;cont<7;cont++) uartx_write(&UARTM2_3, PMS7003_CMD_ACTIVO[cont]);
}

void PMS7003_READ()
{
for (uint8_t cont=0;cont<7;cont++) uartx_write(&UARTM2_3, PMS7003_CMD_READ[cont]);
}





