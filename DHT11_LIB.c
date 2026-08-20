
#include "DHT11_LIB.h"


//DHT11
extern unsigned int DHT11_TEMP;// Declarar en al main
extern unsigned int DHT11_HUM;  //declarar en el main

unsigned  char DHT11_CHKSM;



// Envio una señal al sensor.
void DHT11_start (void)
{

	Output_Pin(DHT11_GPIO_Port, DHT11_Pin);   // configura salida
	HAL_GPIO_WritePin (DHT11_GPIO_Port, DHT11_Pin, 1);   // SACA 1
	  HAL_Delay(50);

    HAL_GPIO_WritePin (DHT11_GPIO_Port, DHT11_Pin, 0);   // saca un 0
    HAL_Delay(18); //saca cero por 18ms
    	Input_Pin(DHT11_GPIO_Port, DHT11_Pin);  // Configura como entrada
    	delay_us(40);

}

uint8_t DHT11_ok()
{

	 DHT11_start();
	 waitforhigh(DHT11_GPIO_Port, DHT11_Pin, 100);//espra por alto o que pasen 100us
	 //espra que el sensor ponga un cero por 100 us sino retorna cero
	 if (waitforlow(DHT11_GPIO_Port, DHT11_Pin, 100)) return(1);
	 else return(0);

}


void  DHT11_Read()
{
 	unsigned int i=0,datar=0;


	 DHT11_TEMP=0;DHT11_HUM=0;DHT11_CHKSM=0;

	 Input_Pin(DHT11_GPIO_Port, DHT11_Pin);  // Configura como entrada

	 for(i=0;i<40;i++)
	  {
	  waitforhigh(DHT11_GPIO_Port, DHT11_Pin, 100);
	  delay_us(40);
	  if(HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin)==0)datar=0;
	  else{datar=1;waitforlow(DHT11_GPIO_Port, DHT11_Pin, 100);}
	  if(i<16){ DHT11_HUM|=datar; if(i<15){DHT11_HUM<<=1;}}
	  if(i>=16 && i<32){DHT11_TEMP|=datar; if(i<31)DHT11_TEMP<<=1;}
	  if(i>=32&& i<40){DHT11_CHKSM|=datar; if(i<39)DHT11_CHKSM<<=1;}
	  }

	   DHT11_TEMP=DHT11_TEMP>>8;
	    DHT11_HUM=DHT11_HUM>>8;
	    HAL_Delay(1);

}

