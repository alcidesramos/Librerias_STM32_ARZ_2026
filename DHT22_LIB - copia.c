
#include "puertos.h"
#include "DHT22_LIB.h"


//DHT22
extern float DHT22_TMP;
extern float DHT22_HUM;
char DHT22_CHKSM;



// Envio una señal al sensor.
void DHT22_start (void)
{
	Output_Pin(DHT22_PORT, DHT22_PIN);   // configura salida
	HAL_GPIO_WritePin (DHT22_PORT, DHT22_PIN, 1);   // SACA 1
	  HAL_Delay(50);

    HAL_GPIO_WritePin (DHT22_PORT, DHT22_PIN, 0);   // saca un 0
    HAL_Delay(2); //saca cero por 18ms
    	Input_Pin(DHT22_PORT, DHT22_PIN);  // Configura como entrada
    	delay_us(40);

}

uint8_t DHT22_ok()
{

	 DHT22_start();
	 waitforhigh(DHT22_PORT, DHT22_PIN, 100);//espra por alto o que pasen 100us
	 //espra que el sensor ponga un cero por 100 us sino retorna cero
	 if (waitforlow(DHT22_PORT, DHT22_PIN, 100)) return(1);
	 else return(0);

}


void  DHT22_Read()
{
 	unsigned int i=0,datar=0,hume_aux,temp_aux;
    unsigned char temp_H,temp_L,hume_H,hume_L;

	 DHT22_TMP=0;DHT22_HUM=0;DHT22_CHKSM=0;

	 Input_Pin(DHT22_PORT, DHT22_PIN);  // Configura como entrada

	 for(i=0;i<40;i++)
	  {
			waitforhigh(DHT22_PORT, DHT22_PIN, 100);
			  delay_us(50);

			  if(HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN)==0)datar=0;
			  else{datar=1;waitforlow(DHT22_PORT, DHT22_PIN, 100);}

			 //humedad
			  if(i<8){ hume_H|=datar; if(i<7){hume_H<<=1;}}
			  if(i>=8 && i<16){hume_L|=datar; if(i<15)hume_L<<=1;}

			 //temperatura
			  if(i>=16 && i<24){temp_H|=datar; if(i<23)temp_H<<=1;}
			  if(i>=24 && i<32){temp_L|=datar; if(i<31)temp_L<<=1;}

			  //checksum
			  if(i>=32&& i<40){DHT22_CHKSM|=datar; if(i<39)DHT22_CHKSM<<=1;}



	  }
	   hume_aux=( (unsigned int)hume_H<<8) | hume_L;
	   temp_aux=( (unsigned int)temp_H<<8) | temp_L;

	    DHT22_TMP=(float)temp_aux/10.0;
	    DHT22_HUM=(float)hume_aux/10.0;
	    HAL_Delay(1);

}

