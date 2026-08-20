/*
 * PASO_PASO_DRIVER.c
 *
 *  Created on: 9 may 2026
 *      Author: Alcides Ramos
 */
#include "PASO_PASO_DRIVER.h"

//MOTORES A USAR
        //motor_num,fac_tiempo(ms),modo
PASOS PASO_PASO1 ={1,10,modo_continuo};
PASOS PASO_PASO2 ={2,1,modo_continuo};



void PASO_Init(PASOS *pas)
{
pas->fact_aux=0;
pas->pulso_cont=0;

}

void PASO_Interrupcion(PASOS *pasi)
{
	switch (pasi->dir)
	{
	case DER:
		 switch(pasi->motor_num)
			  {
			 case 1:
			 	   #ifdef step1_Pin
			 			  PIN_ON(dir1);
			 	   #endif
			 	break;
			 case 2:
			 		   #ifdef step2_Pin
				 	 	 	 PIN_ON(dir2);
			 		   #endif
			 		break;
			 case 3:
			 		   #ifdef step3_Pin
			 				  PIN_ON(dir3);
			 		   #endif
			 		break;

			 case 4:
			 		   #ifdef step4_Pin
				       PIN_ON(dir4);
			 		   #endif
			 		break;

			 case 5:
			 		   #ifdef step5_Pin
				        PIN_ON(dir5);
			 		   #endif
			 		break;

			   }
		break;

	case IZQ:
		 switch(pasi->motor_num)
			  {
			 case 1:
			 	   #ifdef step1_Pin
			 			  PIN_OFF(dir1);
			 	   #endif
			 	break;
			 case 2:
			 		   #ifdef step2_Pin
				 	 	 	 PIN_OFF(dir2);
			 		   #endif
			 		break;
			 case 3:
			 		   #ifdef step3_Pin
			 				  PIN_OFF(dir3);
			 		   #endif
			 		break;

			 case 4:
			 		   #ifdef step4_Pin
				       PIN_OFF(dir4);
			 		   #endif
			 		break;

			 case 5:
			 		   #ifdef step5_Pin
				        PIN_OFF(dir5);
			 		   #endif
			 		break;

			   }
		   break;
	case QUI:

		   break;

	}//fin switch
//analizo si tiene que moverse
	if (pasi->dir!=QUI)
	{
       //doy el pulso a los mootres usados
		 switch(pasi->motor_num)
					  {
					 case 1:
						   #ifdef step1_Pin
								  PIN_BLINK(step1);
						   #endif
						break;
					 case 2:
							   #ifdef step2_Pin
									  PIN_BLINK(step2);
							   #endif
							break;
					 case 3:
							   #ifdef step3_Pin
									  PIN_BLINK(step3);
							   #endif
							break;

					 case 4:
							   #ifdef step4_Pin
									  PIN_BLINK(step4);
							   #endif
							break;

					 case 5:
							   #ifdef step5_Pin
									  PIN_BLINK(step5);
							   #endif
							break;

					  }//SWITCH

   if (pasi->modo==modo_pasos)
   {
		//incremento contador de pulso
		pasi->pulso_cont++;
		//multiplico [or 2 los pulsos para el alto y el bajo
		if((pasi->pulso_cont)>pasi->pulsos<<1)
		{
			pasi->dir=QUI;//lo deja quieto
			pasi->pulso_cont=0;

		}

   }//fin si es modo pasos
   pasi->fact_aux=0;
	}//fin del if si no es quieto

}


void PASO_Analisis(PASOS *pas)
{

	pas->fact_aux++;

  if(pas->fact_aux>=pas->fact_tiempo)
  {
    PASO_Interrupcion(pas);
    pas->fact_aux=0;

  }
}


