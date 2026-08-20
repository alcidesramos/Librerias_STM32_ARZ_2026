/*
 * Nextion.c
 *
 *  Created on: Apr 8, 2025
 *      Author: ALCIDES_RAMOS
 */

#include "Nextion.h"

//COLOCAR UART A USAR
extern UART_HandleTypeDef huart1;
//defina uart y tamaño para la nexton
UARTRXS UARTNEXTION= {&huart1,USART1,100};


//si se quieren leer mas o menos  texto poner tamaño
char nextion_text[50];


//define a que estructura de UART de recepcion asocia la int serial de la nextion
//verificar que  este en  UARTRX.h
//extern UARTRXS   UARTNEXTION; sino coloquelo

/*
 el el void de interrup de UARTRX COLOCAR

       #include "Nextion.h"
    	Nextion_interrup(huart,sizex);


 //en el main  antes del while

  nextionRX_init();
 //  en el while

 //para recibir
  if (UARTNEXTION.flag_rx==1)
	  {
	      procesa_nextion();  //declarar en el main y procesar
	      nextionRX_REinit();
	  }

 //para enviar
 //envia voltaje
	Nextion_Send_Valor("pa0.vol.val=",voltajex);

	Nextion_Send_Valor("pa0.vf.val=",voltaje1*10);



	//envia la temperatura
    Nextion_Send_Valor("pa0.te.val=",temperatura1);

//   envia la temperatura float
    Nextion_Send_Valor("pa0.tef.val=", temperatura*10);

//    	 SUICHES O SEÑALES DIGITALES
	 if (PIN_READ(sw)==1)
		 Nextion_Send_Valor("pa0.led1.pic=", 1);
	else
		Nextion_Send_Text("pa0.led1.pic=0");


	 if (graf==1)
	 {
		 nextion_Send_Graf(2,0,map(voltaje1,0,3.3,0,200));

	 }



  //ejemplo de procesa_nextion
  void procesa_nextion()
{

  if (Nextion_Read_Text("sw1=on")==1) PIN_OFF(led);
  else if (Nextion_Read_Text("sw1=off")==1) PIN_ON(led);

  else if (Nextion_Read_Text("graon")==1) graf=1;
  else if (Nextion_Read_Text("graoff")==1) graf=2;



  //calcula una variable
  else if (Nextion_Read_Text("pwm1=")==1)
  {
  uint8_t ancho=Nextion_Read_1Byte();
  PWM_valor(&PWM1, ancho);
  }

  else if (Nextion_Read_Text("setint=")==1)
    {
    uint16_t numero=Nextion_Read_2Byte();
    sprintf(texto,"\r\nnum1=%d\r\n",numero);
	uartx_write_text(&huart2, texto);
	HAL_Delay(2000);
    }
  else if (Nextion_Read_Text("set4=")==1)
     {
     uint32_t numero=Nextion_Read_4Byte();
     sprintf(texto,"\r\nnum1=%lu\r\n",numero);
 	uartx_write_text(&huart2, texto);
 	HAL_Delay(2000);
     }
  //lee una avrible flotante tipo texto
  else if (Nextion_Read_Text("setext=")==1)
      {
        Nextion_Read_TextVar();//lee  la variable entre = y /
 //relaiza algo
        uint16_t ancho=atoi(nextion_text);
		PWM_valor(&PWM1, ancho);

      }
  //lee una info tipo texto no variab;e
  else if (Nextion_Read_Text("nom=")==1)
        {
          Nextion_Read_TextVar();//lee  la variable entre = y /
   //relaiza algo
          uartx_write_text(&huart2, nextion_text);
           	HAL_Delay(2000);
        }



}


 */






void nextionRX_init()
{
uartRX_it_idle_dma_init( &UARTNEXTION);
}

void nextionRX_REinit()
{
uartRX_DMA_Re_init(&UARTNEXTION);
}


void Nextion_Send_Valor(const uint8_t *info,uint32_t valor)
{
	char nexti[100];
  sprintf(nexti,"%s%lu\xFF\xFF\xFF",info,valor);
  uartx_write_text(UARTNEXTION.huart, nexti);
}

void Nextion_Send_Text(const uint8_t *info)
{
	char nexti[100];
	  sprintf(nexti,"%s\xFF\xFF\xFF",info);
	uartx_write_text(UARTNEXTION.huart, nexti);
}

uint8_t Nextion_Read_Text(const uint8_t *info)
{
	if (strstr(UARTNEXTION.trama_rx,info))  return(1);
	else return(0);
}

//lee un byte de una variable
uint8_t Nextion_Read_1Byte()
{
	char *punte = strchr(UARTNEXTION.trama_rx, '=');
	uint8_t num1=*(punte+1);
	return(num1) ;
}

//lee   una variable de 2 bytes
uint16_t Nextion_Read_2Byte()
{
	char *punte = strchr(UARTNEXTION.trama_rx, '=');
	uint8_t num1=*(punte+1);
    uint8_t num2=*(punte+2);
	int16_t numero=(uint16_t)num2<<8 | num1;
	return(numero) ;
}

//lee   una variable de 4 bytes
uint32_t Nextion_Read_4Byte()
{
	char *punte = strchr(UARTNEXTION.trama_rx, '=');
	uint8_t num1=*(punte+1);
    uint8_t num2=*(punte+2);
    uint8_t num3=*(punte+3);
    uint8_t num4=*(punte+4);

	int32_t numero=(uint32_t)num4<<24 | (uint32_t)num3<<16| (uint16_t)num2<<8 | num1;
	return(numero) ;
}

//lee   una variable en forma cadena
//debe ir entre =  y  /
 void Nextion_Read_TextVar()
{
		strcpy(nextion_text, strtok(UARTNEXTION.trama_rx, "="));  //inicia captura de tokens desde el =
	    strcpy(nextion_text, strtok(0, "/"));  //captura hasta el /
}

 void nextion_Send_Graf(uint8_t id,uint8_t canal,uint16_t valor)
 {
	 char nexti[100];
	sprintf(nexti,"add %u,%u,%u\xFF\xFF\xFF",id,canal,valor);
	uartx_write_text(UARTNEXTION.huart, nexti);

 }

void Nextion_interrup(UART_HandleTypeDef *huart,uint16_t sizex)
{

	//colocar una por cada usart usado
	if ((UARTNEXTION.flag_rx==0)&& (huart->Instance == UARTNEXTION.usart_instance))//si es el uart de datos
		{
		HAL_UART_DMAStop(UARTNEXTION.huart);  //para la recepcion temporarmente
		UARTNEXTION.num_datos=sizex;
		UARTNEXTION.flag_rx=1;
		 }

}


