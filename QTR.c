/*
 * QTR.C
 *
 *  Created on: Jul 25, 2023
 *      Author: alcid
 */
#include <QTR.h>
#include "string.h"
#include "DRV8833.h"
//define la lectura de los pines d elos sensores

#define SIR1_in  HAL_GPIO_ReadPin(SIR1_GPIO_Port, SIR1_Pin)
#define SIR2_in  HAL_GPIO_ReadPin(SIR2_GPIO_Port, SIR2_Pin)
#define SIR3_in  HAL_GPIO_ReadPin(SIR4_GPIO_Port, SIR3_Pin)
#define SIR4_in  HAL_GPIO_ReadPin(SIR4_GPIO_Port, SIR4_Pin)
#define SIR5_in  HAL_GPIO_ReadPin(SIR5_GPIO_Port, SIR5_Pin)
#define SIR6_in  HAL_GPIO_ReadPin(SIR6_GPIO_Port, SIR6_Pin)
#define SIR7_in  HAL_GPIO_ReadPin(SIR7_GPIO_Port, SIR7_Pin)
#define SIR8_in  HAL_GPIO_ReadPin(SIR8_GPIO_Port, SIR8_Pin)
#define SIR9_in  HAL_GPIO_ReadPin(SIR9_GPIO_Port, SIR9_Pin)
#define SIR10_in  HAL_GPIO_ReadPin(SIR10_GPIO_Port, SIR10_Pin)

//definir como externas en el main

//definir en el main el vector qwue guarda als lecturas

//banderas de lectura de relexion  // tipo banderas 8 bits
uint16_tb flags;

//PARA NORMALIZAR LAS LECTURAS
uint16_t  lecturasir[ n_sensores];
unsigned int minimos[]={65535,65535,65535,65535,65535,65535,65535,65535,65535,65535}; //Lectura minima   inicia con un valor alto
unsigned int maximos[]={0,0,0,0,0,0,0,0,0,0}; // Lectura de maximo  inicia con un valor bajo

float pesos[]={-4.5,-3.5,-2.5,-1.5,-0.5,0.5,1.5,2.5,3.5,4.5}; // Lectura normalizadas de 0 a 1
//float pesos[]={-16.0,-8.0,-4.0,-2.0,-0.5,0.5,2.0,4.0,8.0,16.0}; // Lectura normalizadas de 0 a 1

float peso_anterior;// almacena el sentido de giro anterior
uint8_t flag_fuera=0;

void QTR_Init()
{
delay_Init();
HAL_TIM_Base_Start(&hTIMQTR);//inicia el timer IR
}

void QTR_lectura()
{

memset(lecturasir,0,n_sensores*2);//limpia las lecturas

#ifdef LEDIR_Pin
HAL_GPIO_WritePin(LEDIR_GPIO_Port, LEDIR_Pin, 1);//ENCIENDE LED DE CALIBRACION
#endif


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
#ifdef SIR9_Pin
	 Output_Pin(SIR9_GPIO_Port, SIR9_Pin);
#endif
#ifdef SIR10_Pin
	 Output_Pin(SIR10_GPIO_Port, SIR10_Pin);
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
#ifdef SIR9_Pin
	 HAL_GPIO_WritePin(SIR9_GPIO_Port, SIR9_Pin, 1);
#endif
#ifdef SIR10_Pin
	 HAL_GPIO_WritePin(SIR10_GPIO_Port, SIR10_Pin, 1);
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

#ifdef SIR9_Pin
Input_Pin(SIR9_GPIO_Port,SIR9_Pin);
#endif

#ifdef SIR10_Pin
Input_Pin(SIR10_GPIO_Port,SIR10_Pin);
#endif


//limpia las banderas
	 	  flags.word=0;// borra las banderas

		  TIMQTR->CNT=0;// Resetea el timer

		  while (TIMQTR->CNT <muestro_ir)// ESPeRA LOS uS DESEADOS
			{
       //lectura sensor 1
        #ifdef SIR1_Pin
			  if ((SIR1_in==0) && (flags.b0==0))
	           {
				lecturasir[0]=TIMQTR->CNT;
	            flags.b0=1;
	           }
        #endif
      //lectura sensor 2
        #ifdef SIR2_Pin
		       if ((SIR2_in==0) && (flags.b1==0))
	          {
		    	 lecturasir[1]=TIMQTR->CNT;
			     flags.b1=1;
			    }
		 #endif
		  //lectura sensor 3
		  #ifdef SIR3_Pin
		    	   if ((SIR3_in==0) && (flags.b2==0))
		    	   {
		    		lecturasir[2]=TIMQTR->CNT;
		       	     flags.b2=1;
		       	   	}
				#endif

		           //lectura sensor 4
		            #ifdef SIR4_Pin
		    			  if ((SIR4_in==0) && (flags.b3==0))
		    	           {
		    				lecturasir[3]=TIMQTR->CNT;
		    	            flags.b3=1;
		    	           }
		            #endif
		          //lectura sensor 5
		            #ifdef SIR5_Pin
		    		       if ((SIR5_in==0) && (flags.b4==0))
		    	          {
		    		    	lecturasir[4]=TIMQTR->CNT;
		    			    flags.b4=1;
		    			    }
		    		 #endif
		    		  //lectura sensor 6
		    		  #ifdef SIR6_Pin
		    		    	   if ((SIR6_in==0) && (flags.b5==0))
		    		    	   {
		    		    		lecturasir[5]=TIMQTR->CNT;
		    		       	     flags.b5=1;
		    		       	   	}
		    				#endif

		    		    //lectura sensor 7
		    		      #ifdef SIR7_Pin
		    		        	  if ((SIR7_in==0) && (flags.b6==0))
		    		    	           {
		    		        		  lecturasir[6]=TIMQTR->CNT;
		    		    	            flags.b6=1;
		    		    	           }
		    		            #endif
		    		      //lectura sensor 8
	    		            #ifdef SIR8_Pin
		    		    		   if ((SIR8_in==0) && (flags.b7==0))
		    		    	        {
		    		    			 lecturasir[7]=TIMQTR->CNT;
		    		    			 flags.b7=1;
		    		    			 }
		    		    		 #endif


		    		 		      //lectura sensor 9
		    		 	    		            #ifdef SIR9_Pin
		    		 		    		    		   if ((SIR9_in==0) && (flags.b8==0))
		    		 		    		    	        {
		    		 		    		    			 lecturasir[8]=TIMQTR->CNT;
		    		 		    		    			 flags.b8=1;
		    		 		    		    			 }
		    		 		    		    		 #endif


		    		 		    				      //lectura sensor 10
													#ifdef SIR10_Pin
		    		 		    		    		   if ((SIR10_in==0) && (flags.b9==0))
		    		 		    		    	        {
		    		 		    		    			 lecturasir[9]=TIMQTR->CNT;
		    		 		    		    			 flags.b9=1;
		    		 		    		    			 }
		    		 		    		    		 #endif


			  }// fin del while de las lecturas

#ifdef LEDIR_Pin
   HAL_GPIO_WritePin(LEDIR_GPIO_Port, LEDIR_Pin, 0);//Apaga LED DE CALIBRACION
#endif
}

void QTR_Manda_Lecturas()
{
	char textoir[300];
	uartx_write_text(&QTR_UART, "CRUDO");
	sprintf(textoir,"\r\nIR1=%u\r\nIR2=%u\r\nIR3=%u\r\nIR4=%u\r\nIR5=%u\r\nIR6=%u\r\nIR7=%u\r\nIR8=%u\r\nIR9=%u\r\nIR10=%u\r\n\r\n",lecturasir[0],lecturasir[1],lecturasir[2],lecturasir[3],lecturasir[4],lecturasir[5],lecturasir[6],lecturasir[7],lecturasir[8],lecturasir[9]);
  uartx_write_text(&QTR_UART, textoir);
 }

void QTR_calibra(uint16_t veces)
{
uint16_t cuenta2;
uint8_t cont;
uint8_t sentido=0;
uint32_t milis_QTR=uwTick;
#define velo 4080

//MD_Adelante(velo);
//MI_Atras(velo);
while(uwTick-milis_QTR<400);
milis_QTR=uwTick;

    for (cuenta2=0; cuenta2<=veces;cuenta2++)
    {
     if (uwTick-milis_QTR>800)
     {
    	 if(sentido==0)
    	 {
    		// MD_Atras(velo);
    		// MI_Adelante(velo);
   		    sentido=1;
    	 }
    	 else
    	 {

    	//	 MD_Adelante(velo);
    		// MI_Atras(velo);
              sentido=0;

    	 }

    	 milis_QTR=uwTick;
     }
    for (cont=0; cont<n_sensores;cont++)
        {
    	QTR_lectura();
           if (lecturasir[cont]>=maximos[cont]) maximos[cont]=lecturasir[cont];
           if (lecturasir[cont]<=minimos[cont]) minimos[cont]=lecturasir[cont];
                          }  //

    } //fin for cuenta


//APAGA MOTORES
    MD_Stop();
    MI_Stop();

 }

void QTR_Manda_Limites()
{
	char textoir[300];
  sprintf(textoir,"min1=%u\r\nmin2=%u\r\nmin3=%u\r\nmin4=%u\r\nmin5=%u\r\nmin6=%u\r\nmin7=%u\r\nmin8=%u\r\nmin9=%u\r\nmin10=%u\r\n\r\n",minimos[0],minimos[1],minimos[2],minimos[3],minimos[4],minimos[5],minimos[6],minimos[7],minimos[8],minimos[9]);
  uartx_write_text(&QTR_UART, textoir);
  sprintf(textoir,"max1=%u\r\nmax2=%u\r\nmax3=%u\r\nmax4=%u\r\nmax5=%u\r\nmax6=%u\r\nmax7=%u\r\nmax8=%u\r\nmax9=%u\r\nmax10=%u\r\n\r\n",maximos[0],maximos[1],maximos[2],maximos[3],maximos[4],maximos[5],maximos[6],maximos[7],maximos[8],maximos[9]);
  uartx_write_text(&QTR_UART, textoir);
}
// normaliza el valor leido en un ranfo en este caso de 0 a 1
 void QTR_Normaliza()
 {
   uint8_t cuenta;
   float min_temp,max_temp;
   float promedio;


   for (cuenta=0;cuenta<n_sensores;cuenta++)
   {
    min_temp=minimos[cuenta];
    max_temp=maximos[cuenta];

    promedio=(max_temp-min_temp)/2;  // calcula la diferencia entre el maximo y minimo del sensor correspondiente

    if (lecturasir[cuenta]>=(min_temp+promedio))  lecturasir[cuenta]=ir_negro;
    else  lecturasir[cuenta]=ir_blanco;

   } // fin del for cuenta

 }

 void QTR_Normaliza_A()
  {
	 //analizando el minimo de los maximos
	 //se normaliza de 0 a 3000 la salida
    	 uint8_t cuenta;
    float min_temp,max_temp;
    float promedio;
    double ventana, salida; // VALOR ESTIMADO


    for (cuenta=0;cuenta<n_sensores;cuenta++)
    {
     min_temp=minimos[cuenta];
     max_temp=maximos[cuenta];
      ventana=max_temp-min_temp;
      salida=0;
       //blanco
      if(ventana != 0.0) salida = (((float)lecturasir[cuenta]) -  min_temp) * rango_normaliza / ventana;

      //negro
      //if(ventana != 0.0) salida = ((float)max_temp-(float)lecturasir[cuenta]) * rango_normaliza / ventana;


      if(salida < 0.0) salida = 0.0;
         else if(salida > rango_normaliza) salida = rango_normaliza;
         lecturasir[cuenta] = salida;

      //SI ES NEGRO LA PISTA
/*
         for (cuenta=0;cuenta<n_sensores;cuenta++)
         {
        	 int lee=rango_normaliza-lecturasir[cuenta];
        	if (lee<0) lee=0;
        	 lecturasir[cuenta]=lee;

         }*/

    } // fin del for cuenta


  }

 float QTR_Calcula_Pesos()
{
   char cuenta;
   float numerador=0,denominador=0;
   float valor;
   float peso;
   uint8_t linea=0;
   float peso_min=pesos[0];
   float peso_max=pesos[n_sensores-1];
   char textoir[100];
   for (cuenta=0;cuenta<n_sensores; cuenta++)
   {
     //linea blanca
	 peso=(float)lecturasir[cuenta];


	 //este valor depende de sus sensores
	 	 if(peso>lin_negro)  linea=1;  //indica que al menos un sensor esta en la linea

			if(peso>lin_blancolow)//elimina reflejos irrelevantes  es el umbral u offset
			{
			 numerador+=lecturasir[cuenta]*pesos[cuenta];
			 denominador+=lecturasir[cuenta];
			}

	    }//fin del for
   flag_fuera=0;
	if(!linea)//sino esta en la line  corrige
		//auna velocidad media
		{
			if (peso_anterior>0) return(4.5); // si estaba muy a la derecha antes retorna el maximo valor a la derecha
			else  if (peso_anterior<0) return(-4.5);  // si estaba muy a la izquierda antes retorna el maximo valor a la izquierda
		   flag_fuera=1;
		}
	//si tod esta en la linea
	//if (numerador<0.001) return(peso_anterior);



  //si esta en la linea
	if (denominador!=0) valor=numerador/denominador;
  	   peso_anterior=valor;//  almacena el valor por si se llega a salir de la linea
        return(valor);
}

 float QTR_Calcula_Pesos_N()
 {
    char cuenta;
    float numerador=0,denominador=0.0001;
    float valor;
    float peso;
    uint8_t linea=0;
    float peso_min=pesos[0];
    float peso_max=pesos[n_sensores-1];
    char textoir[100];
    for (cuenta=0;cuenta<n_sensores; cuenta++)
    {
      //linea blanca
 	 peso=(float)lecturasir[cuenta];


 	   //linea negra

 //	   peso=(float)(muestro_ir-lecturasir[cuenta]);


 	 //este valor depende de sus sensores
 	 	 if(peso==1)  linea=1;  //indica que al menos un sensor esta en la linea

 			//if(peso>20)//elimina reflejos irrelevantes  es el umbral u offset
 			{
 			 numerador+=lecturasir[cuenta]*pesos[cuenta];
 			 denominador+=lecturasir[cuenta];
 			}

 	    }//fin del for


 	if(numerador==0)//sino esta en la line  corrige
 		{
 			if (peso_anterior>0) return(50); // si estaba muy a la derecha antes retorna el maximo valor a la derecha
 			else  if (peso_anterior<0) return(-50);  // si estaba muy a la izquierda antes retorna el maximo valor a la izquierda
 		}
   //previene division por cero
     if (denominador!=0) valor=numerador/denominador;
   	   else valor=peso_anterior;
   	   peso_anterior=valor;//  almacena el valor por si se llega a salir de la linea
        return(valor);
 }


 float QTR_Calcula_Pesos_ARZ()
{
   char cuenta;
   float numerador=0,denominador=0.0001;
   float valor;
   uint8_t linea=0;

   for (cuenta=0;cuenta<n_sensores; cuenta++)
   {
     numerador=lecturasir[cuenta]*pesos[cuenta]+numerador;
     denominador=lecturasir[cuenta]+denominador;
   }// for cuenta


  /* if (denominador>=9.8) // indica que todo esta en negro, se recominda rectificar movimiento
     {
       if (peso_anterior>0) return pesos[n_sensores-1]; // si estaba muy a la derecha antes retorna el maximo valor a la derecha
       else  if (peso_anterior<0) return pesos[0];  // si estaba muy a la izquierda antes retorna el maximo valor a la izquierda
     }*/
     //
      if (denominador==0.00001) // indica que no esta en la linea , se salio  para evitar errores flotante
     {
       if (peso_anterior>0) return pesos[n_sensores-1]; // si estaba muy a la derecha antes retorna el maximo valor a la derecha
       else  if (peso_anterior<0) return pesos[0];  // si estaba muy a la izquierda antes retorna el maximo valor a la izquierda
       linea=1;
     }
 if (linea==0)  valor=numerador/denominador;
 else valor=peso_anterior;
  peso_anterior=valor;//  almacena el valor por si se lelga a salir de la linea
     return(valor);

}

 float analisis_pesos()
 {
	 uint16_tb codigo;
	 uint16_t peso1,peso1_;
	 char textoir[300];
  //arma codigo de sensores de la derecha

	 codigo.word=0;
	 codigo.b0=lecturasir[0];
	 codigo.b1=lecturasir[1];
	 codigo.b2=lecturasir[2];
	 codigo.b3=lecturasir[3];
	 codigo.b4=lecturasir[4];
	 codigo.b5=lecturasir[5];
	 codigo.b6=lecturasir[6];
	 codigo.b7=lecturasir[7];
	 codigo.b8=lecturasir[8];
	 codigo.b9=lecturasir[9];


   switch (codigo.word)
  {
   //caso en blanco
   //      321Cc123
     case 0b00000000:

     //  if( (peso_>0) && (peso_<16)) peso=16;
      // else if(peso_<0) peso=-16;
      // else peso=20;
      break;
          //centrales
            //321Cc123
       case 0b0000100000: peso1=0;break;
            //4321cc1234
       case 0b0000110000: peso1=0;break;
            //4321cc1234
       case 0b0000010000: peso1=0;break;
            //4321cc1234

       case 0b0000001000: peso1=1;break;
            //4321cc1234
       case 0b0000001100: peso1=1.5;break;
            //4321cc1234
       case 0b0000001110: peso1=2.0;break;
            //4321cc1234
       case 0b0000001111: peso1=2.5;break;
            //4321cc1234


       case 0b0000000100: peso1=3.0;break;
            //4321cc1234
       case 0b0000000110: peso1=3.5;break;
            //4321cc1234
       case 0b0000000111: peso1=4.0;break;
            //4321cc1234


       case 0b0000000010: peso1=4.5;break;
            //4321cc1234
       case 0b0000000011: peso1=5;break;
            //4321cc1234

       case 0b0000000001: peso1=5.5;break;
            //4321cc1234
       ///LOS DE LA IZQUIERDA

       case 0b0001000000: peso1=-1;break;
            //4321cc1234
       case 0b0011000000: peso1=-1.5;break;
            //4321cc1234
       case 0b0111000000: peso1=-2.0;break;
            //4321cc1234
       case 0b1111000000: peso1=-2.5;break;
            //4321cc1234

       case 0b0010000000: peso1=-3.0;break;
            //4321cc1234
       case 0b0110000000: peso1=-3.5;break;
            //4321cc1234
       case 0b1110000000: peso1=-4.0;break;
            //4321cc1234

       case 0b0100000000: peso1=-4.5;break;
            //4321cc1234
       case 0b1100000000: peso1=-5.0;break;
            //4321cc1234

       case 0b1000000000: peso1=-5.5;break;
            //4321cc1234

            default : peso1=50;
   }//fin del switch

   peso1_=peso1;
   return(peso1);
 }
 float calcularError()
 {
     float error = 0.0;
     int sumaLecturas = 0;
     float errorAnterior = 0.0; // Variable para recordar el error anterior

     // Calcular el error basado en las lecturas de los sensores
     for (int i = 0; i < n_sensores; i++)
     {
         if (lecturasir[i] == 1)
         {
             error += pesos[i];  // Suma el peso del sensor activo
             sumaLecturas++;     // Cuenta el número de sensores activos
         }

     }
     // Evitar división por cero: si todos los sensores son cero, usa el error anterior
         if (sumaLecturas == 0) {
             return errorAnterior;
         }

         // Calcula el error promedio
         error = error / sumaLecturas;

         // Actualizar el error anterior
         errorAnterior = error;

         return error;
}
