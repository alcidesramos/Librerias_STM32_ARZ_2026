/*
 * Pasopaso.c
 *
 *  Created on: May 22, 2024
 *      Author: Alcides Ramos
 */
#include "Pasopaso.h"
#include "main.h"

#define M2  //si se va usar otro motor

//paso simple
uint32_t codigo1[]={0b0001,0b0010,0b0100,0b1000};
//codigo motor2
uint32_t codigo2[]={0b0001,0b0010,0b0100,0b1000};


//paso doble
//char codigo1[]={0b1100,0b0110,0b0011,0b1001};
//codigo motor2
//char codigo2[]={0b1100,0b0110,0b0011,0b1001};

//medio paso
//uint8_t codigo1[]={0b1000,0b1100,0b0100,0b0110,0b0010,0b0011,0b0001,0b1001};
//codigo motor2
//uint8_t codigo2[]={0b1000,0b1100,0b0100,0b0110,0b0010,0b0011,0b0001,0b1001};

const uint8_t n_pasos_m=sizeof(codigo1); //colocar numero de secuencias del motor

//acumulan la posicion de la secuencia
signed char secuencia1=0;
signed char secuencia2=0;

//variable para retardo  extra dentro de la interrupcion
uint16_t velo1=0,velo2=0;

const uint8_t n_pasos=n_pasos_m-1;//como ararnca en cero resta uno para no hacer operacones en la interrpcion

//variables del sentido de giro d elos motores
uint8_t giro_m1,giro_m2;
//variable de la velocidad de los motores de 0 - vel_max
uint16_t velo_m1=0,velo_m2=0;
//variable almacena numero pasos a dar en modo pasos
uint32_t  n_pasos1,n_pasos2;uint32_t  pasos1=0,pasos2=0;

uint8_t estado_giro1=quieto,estado_giro2=quieto;
uint8_t estado_modo1=continuo,estado_modo2=continuo;


void saca_secuenciaM1()
{
	uint8_t lee1;
	pasos1++;

	lee1= LEEBIT (&codigo1[secuencia1],0);
	HAL_GPIO_WritePin(M1B1_GPIO_Port, M1B1_Pin, lee1);

	lee1= LEEBIT (&codigo1[secuencia1],1);
	HAL_GPIO_WritePin(M1B2_GPIO_Port, M1B2_Pin, lee1);

	lee1= LEEBIT (&codigo1[secuencia1],2);
   HAL_GPIO_WritePin(M1B3_GPIO_Port, M1B3_Pin, lee1);

   lee1= LEEBIT (&codigo1[secuencia1],3);
   	HAL_GPIO_WritePin(M1B4_GPIO_Port, M1B4_Pin, lee1);

}


 void izquierdaM1()
 {
  secuencia1++;
  if (secuencia1>n_pasos) secuencia1=0;
  saca_secuenciaM1();
 }

void derechaM1()
 {
  secuencia1--;
  if (secuencia1<0) secuencia1=n_pasos;
  saca_secuenciaM1();
 }


void pasopaso_int()
{
	  //aumenta variable para velocidad
	  velo1++;
	  velo2++;
	    //analiza si le toca dar un paso al motor1
	        if (velo1>=vel_max+1-velo_m1)// para que la menor sea mas lenta
	        {

	        	velo1=0;
	            if (estado_modo1==pasos)  //si esta en modo de pasos
	               {
	                 if (pasos1>=n_pasos1)
	                 {
	                   estado_giro1=quieto;//deja quieto al motor
	                   pasos1=0;    //reinicia numero de pasos
	                 }
	               }
	              //analiza el sentido de giro del motor
	             switch (estado_giro1)
	             {
	               case derecha: derechaM1(); break;
	               case izquierda:izquierdaM1();break;

	             }//fin del switch del sentido de giro

	        }//fin de analisis del motor 1


#ifdef M2

	        //analiza si le toca dar un paso al motor2
	               if (velo2>=vel_max+1-velo_m2)// para que la menor sea mas lenta
	               {

	            	   velo2=0;
	                   if (estado_modo2==pasos)  //si esta en modo de pasos
	                      {
	                        if (pasos2>=n_pasos2)
	                        {
	                          estado_giro2=quieto;//deja quieto al motor
	                          pasos2=0;    //reinicia nuemro de pasos
	                        }
	                      }
	                     //analiza el sesntido de giro del motor
	                    switch (estado_giro2)
	                    {
	                      case derecha: derechaM2(); break;
	                      case izquierda:izquierdaM2();break;

	                    }//fin del switch del sentido de giro

	               }//fin de analisis del motor 2
#endif



}


#ifdef M2


void saca_secuenciaM2()
{

	uint8_t lee1;
	pasos2++;

	   lee1= LEEBIT (&codigo2[secuencia2],0);
		HAL_GPIO_WritePin(M2B1_GPIO_Port, M2B1_Pin, lee1);

		lee1= LEEBIT (&codigo2[secuencia2],1);
		HAL_GPIO_WritePin(M2B2_GPIO_Port, M2B2_Pin, lee1);

		lee1= LEEBIT (&codigo2[secuencia2],2);
	   HAL_GPIO_WritePin(M2B3_GPIO_Port, M2B3_Pin, lee1);

	   lee1= LEEBIT (&codigo2[secuencia2],3);
	   	HAL_GPIO_WritePin(M2B4_GPIO_Port, M2B4_Pin, lee1);
    }


 void izquierdaM2()
 {
  secuencia2++;
  if (secuencia2>n_pasos) secuencia2=0;
  saca_secuenciaM2();
 }

void derechaM2()
 {
  secuencia2--;
  if (secuencia2<0) secuencia2=n_pasos;
  saca_secuenciaM2();
 }
#endif
