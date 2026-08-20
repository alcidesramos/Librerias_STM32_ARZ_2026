/*
 * QTR.C
 *
 *  Created on: Jul 25, 2023
 *      Author: alcid
 */


#include <QTR.h>
#include "string.h"
//define la lectura de los pines d elos sensores

#define SIR1_in  HAL_GPIO_ReadPin(SIR1_GPIO_Port, SIR1_Pin)
#define SIR2_in  HAL_GPIO_ReadPin(SIR2_GPIO_Port, SIR2_Pin)
#define SIR3_in  HAL_GPIO_ReadPin(SIR4_GPIO_Port, SIR3_Pin)
#define SIR4_in  HAL_GPIO_ReadPin(SIR4_GPIO_Port, SIR4_Pin)
#define SIR5_in  HAL_GPIO_ReadPin(SIR5_GPIO_Port, SIR5_Pin)
#define SIR6_in  HAL_GPIO_ReadPin(SIR6_GPIO_Port, SIR6_Pin)
#define SIR7_in  HAL_GPIO_ReadPin(SIR7_GPIO_Port, SIR7_Pin)
#define SIR8_in  HAL_GPIO_ReadPin(SIR8_GPIO_Port, SIR8_Pin)

//definir como externas en el main

//definir en el main el vector qwue guarda als lecturas

//banderas de lectura de relexion  // tipo banderas 8 bits
uint8_tb flags;

//PARA NORMALIZAR LAS LECTURAS
uint16_t lecturasir[8];
unsigned int minimos[]={65535,65535,65535,65535,65535,65535,65535,65535}; //Lectura minima   inicia con un valor alto
unsigned int maximos[]={0,0,0,0,0,0,0,0}; // Lectura de maximo  inicia con un valor bajo

void QTR_Init()
{
delay_Init();
HAL_TIM_Base_Start(&hTIMIR);//inicia el timer IR
}

void QTR_lectura(uint16_t *lectu_QTR)
{

memset(lectu_QTR,0,8);//limpia las lecturas
//configura como salida
#ifdef SIR1_Pin
	 Output_Pin(SIR1_GPIO_Port, SIR1_Pin);
#endif
#ifdef SIR2_Pin
	 Output_Pin(SIR2_GPIO_Port, SIR2_Pin);
#endif
#ifdef SIR3_Pin
	 Output_Pin(SIR3_GPIO_Port, SIR3_Pin);
#endif
#ifdef SIR4_Pin
	 Output_Pin(SIR4_GPIO_Port, SIR4_Pin);
#endif
#ifdef SIR5_Pin
	 Output_Pin(SIR5_GPIO_Port, SIR5_Pin);
#endif
#ifdef SIR6_Pin
	 Output_Pin(SIR6_GPIO_Port, SIR6_Pin);
#endif
#ifdef SIR7_Pin
	 Output_Pin(SIR7_GPIO_Port, SIR7_Pin);
#endif
#ifdef SIR8_Pin
	 Output_Pin(SIR8_GPIO_Port, SIR8_Pin);
#endif


	 //saca 1
#ifdef SIR1_Pin
	 HAL_GPIO_WritePin(SIR1_GPIO_Port, SIR1_Pin, 1);
#endif
#ifdef SIR2_Pin
	 HAL_GPIO_WritePin(SIR2_GPIO_Port, SIR2_Pin, 1);
#endif
#ifdef SIR3_Pin
	 HAL_GPIO_WritePin(SIR3_GPIO_Port, SIR3_Pin, 1);
#endif
#ifdef SIR4_Pin
	 HAL_GPIO_WritePin(SIR4_GPIO_Port, SIR4_Pin, 1);
#endif
#ifdef SIR5_Pin
	 HAL_GPIO_WritePin(SIR5_GPIO_Port, SIR5_Pin, 1);
#endif
#ifdef SIR6_Pin
	 HAL_GPIO_WritePin(SIR6_GPIO_Port, SIR6_Pin, 1);
#endif
#ifdef SIR7_Pin
	 HAL_GPIO_WritePin(SIR7_GPIO_Port, SIR7_Pin, 1);
#endif
#ifdef SIR8_Pin
	 HAL_GPIO_WritePin(SIR8_GPIO_Port, SIR8_Pin, 1);
#endif

	 delay_us(10);


   //se configura como entrada
#ifdef SIR1_Pin
Input_Pin(SIR1_GPIO_Port,SIR1_Pin);
#endif
#ifdef SIR2_Pin
Input_Pin(SIR2_GPIO_Port,SIR2_Pin);
#endif
#ifdef SIR3_Pin
Input_Pin(SIR3_GPIO_Port,SIR3_Pin);
#endif
#ifdef SIR4_Pin
Input_Pin(SIR4_GPIO_Port,SIR4_Pin);
#endif
#ifdef SIR5_Pin
Input_Pin(SIR5_GPIO_Port,SIR5_Pin);
#endif
#ifdef SIR6_Pin
Input_Pin(SIR6_GPIO_Port,SIR6_Pin);
#endif
#ifdef SIR7_Pin
Input_Pin(SIR7_GPIO_Port,SIR7_Pin);
#endif
#ifdef SIR8_Pin
Input_Pin(SIR8_GPIO_Port,SIR8_Pin);
#endif

//limpia las banderas
	 	  flags.byte=0;// borra las banderas

		  TIMIR->CNT=0;// Resetea el timer

		  while (TIMIR->CNT < 6000)// ESPeRA LOS uS DESEADOS
			{
       //lectura sensor 1
        #ifdef SIR1_Pin
			  if ((SIR1_in==0) && (flags.b0==0))
	           {
				lectu_QTR[0]=TIMIR->CNT;
	            flags.b0=1;
	           }
        #endif
      //lectura sensor 2
        #ifdef SIR2_Pin
		       if ((SIR2_in==0) && (flags.b1==0))
	          {
		    	 lectu_QTR[1]=TIMIR->CNT;
			     flags.b1=1;
			    }
		 #endif
		  //lectura sensor 3
		  #ifdef SIR3_Pin
		    	   if ((SIR3_in==0) && (flags.b2==0))
		    	   {
		    		lectu_QTR[2]=TIMIR->CNT;
		       	     flags.b2=1;
		       	   	}
				#endif

		           //lectura sensor 4
		            #ifdef SIR4_Pin
		    			  if ((SIR4_in==0) && (flags.b3==0))
		    	           {
		    				lectu_QTR[3]=TIMIR->CNT;
		    	            flags.b3=1;
		    	           }
		            #endif
		          //lectura sensor 5
		            #ifdef SIR5_Pin
		    		       if ((SIR5_in==0) && (flags.b4==0))
		    	          {
		    		    	lectu_QTR[4]=TIMIR->CNT;
		    			    flags.b4=1;
		    			    }
		    		 #endif
		    		  //lectura sensor 6
		    		  #ifdef SIR6_Pin
		    		    	   if ((SIR6_in==0) && (flags.b5==0))
		    		    	   {
		    		    		lectu_QTR[5]=TIMIR->CNT;
		    		       	     flags.b5=1;
		    		       	   	}
		    				#endif

		    		    //lectura sensor 7
		    		      #ifdef SIR7_Pin
		    		        	  if ((SIR7_in==0) && (flags.b6==0))
		    		    	           {
		    		        		  lectu_QTR[6]=TIMIR->CNT;
		    		    	            flags.b6=1;
		    		    	           }
		    		            #endif
		    		      //lectura sensor 8
	    		            #ifdef SIR8_Pin
		    		    		   if ((SIR8_in==0) && (flags.b7==0))
		    		    	        {
		    		    			 lectu_QTR[7]=TIMIR->CNT;
		    		    			 flags.b7=1;
		    		    			 }
		    		    		 #endif

			  }// fin del while de las lecturas


}

void QTR_calibra(uint16_t veces)
{
uint16_t cuenta2;
uint8_t cont;

#ifdef LEDIR_Pin
HAL_GPIO_WritePin(LEDIR_GPIO_Port, LEDIR_Pin, 1);//ENCIENDE LED DE CALIBRACION
#endif

    //toma 80 muestras del terreno  para calibrar los sensores
    for (cuenta2=0; cuenta2<=veces;cuenta2++)
    {
    for (cont=0; cont<8;cont++)
        {
    	QTR_lectura(lecturasir);
           if (lecturasir[cont]>=maximos[cont]) maximos[cont]=lecturasir[cont];
           if (lecturasir[cont]<=minimos[cont]) minimos[cont]=lecturasir[cont];
        }  //

    } //fin for cuenta
#ifdef LEDIR_Pin
   HAL_GPIO_WritePin(LEDIR_GPIO_Port, LEDIR_Pin, 0);//Enciende LED DE CALIBRACION
#endif



 }

void QTR_Manda_Limites()
{
	char textoir[100];
  sprintf(textoir,"min1=%u   min2=%u  min3=%u   min4=%u   min5=%u    min6=%u    min7=%u   min8=%u\r\n",minimos[0],minimos[1],minimos[2],minimos[3],minimos[4],minimos[5],minimos[6],minimos[7]);
  uartx_write_text(&huart1, textoir);
  sprintf(textoir,"max1=%u   max2=%u  max3=%u   max4=%u   max5=%u   max6=%u   max7=%u   max8=%u\r\n\r\n",maximos[0],maximos[1],maximos[2],maximos[3],maximos[4],maximos[5],maximos[6],maximos[7]);
  uartx_write_text(&huart1, textoir);


}

// normaliza el valor leido en un ranfo en este caso de 0 a 1
 void QTR_Normaliza(uint16_t *lectu_QTR)
 {
   uint8_t cuenta;
   float min_temp,max_temp;
   float promedio;


   for (cuenta=0;cuenta<8;cuenta++)
   {
    min_temp=minimos[cuenta];
    max_temp=maximos[cuenta];
    promedio=(max_temp-min_temp)/2;  // calcual la diferencia entre el maximo y minimo del sensor correspondiente
     if (lectu_QTR[cuenta]>=(min_temp+promedio))  lectu_QTR[cuenta]=blanco;
     else  lectu_QTR[cuenta]=negro;

   } // fin del for cuenta

 }



