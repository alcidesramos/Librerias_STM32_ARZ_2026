/*
 * PWM_INUENCIA.C
 *
 *  Created on: Dec 25, 2024
 *      Author: ALCIDES_RAMOS
 */


#include <PWM_IN.h>

//incluir los timer usados
extern TIM_HandleTypeDef htim2;


//configura entradas de PWM_INuencia a usar
PWM_INS PWM_IN1 = {&htim2, &(TIM2->CCR1),TIM_CHANNEL_1,TIM_FLAG_CC1};
PWM_INS PWM_IN2 = {&htim2, &(TIM2->CCR2),TIM_CHANNEL_2,TIM_FLAG_CC2};
PWM_INS PWM_IN3 = {&htim2, &(TIM2->CCR3),TIM_CHANNEL_3,TIM_FLAG_CC3};
PWM_INS PWM_IN4 = {&htim2, &(TIM2->CCR4),TIM_CHANNEL_4,TIM_FLAG_CC4};

#define  nosignal 200 //  colocar valor para colocar cero cuando se pierda la señal mas  de ciertyo milisegundos





void PWM_IN_init(PWM_INS *PWM_IN)
{
	PWM_IN->caida=1;//inicia promera lectira en cero
    HAL_TIM_IC_Start_IT(PWM_IN->htim, PWM_IN->channel);
        PWM_IN->flag=0;
    PWM_IN->TICK_PWM=uwTick;

}

//analiza si la interrpcion de PWM a deseada se da
void PWM_IN_INT_Analiza(PWM_INS *PWM_IN)
{
	//  si la bandera del canal x del timer seleccionado esta activo
		if (__HAL_TIM_GET_FLAG(PWM_IN->htim, PWM_IN->PWM_IN_flag) ==1)
			{

			//si el flanco es de subida
					if (PWM_IN->caida == 1) {
						//CONFIGURA FLANCO DE SUBIDA
						__HAL_TIM_SET_CAPTUREPOLARITY(PWM_IN->htim, PWM_IN->channel,TIM_INPUTCHANNELPOLARITY_FALLING);
						PWM_IN->caida = 0;
						PWM_IN->pulso_s = *PWM_IN->ccr;//lee el contador;

					} //si

					//si capturo el flaco de bajada
					else {
						__HAL_TIM_SET_CAPTUREPOLARITY(PWM_IN->htim, PWM_IN->channel,TIM_INPUTCHANNELPOLARITY_RISING);
						PWM_IN->caida = 1;
						PWM_IN->pulso_b = *PWM_IN->ccr;//lee el contador;
						PWM_IN->flag=1;
						PWM_IN->TICK_PWM=uwTick;

						if (PWM_IN->pulso_b > PWM_IN->pulso_s)
							PWM_IN->pulso = (PWM_IN->pulso_b - PWM_IN->pulso_s);
						else
							PWM_IN->pulso = (PWM_IN->htim->Instance->ARR - PWM_IN->pulso_s) + PWM_IN->pulso_b;
					} //Fin else


	    		   	__HAL_TIM_CLEAR_FLAG(PWM_IN->htim, PWM_IN->PWM_IN_flag);//limpia la bandera


			}//fin if


}
void PWM_IN_Interrupt()
{
	//llama las PWM deseadas a analizar

	PWM_IN_INT_Analiza(&PWM_IN1);
    PWM_IN_INT_Analiza(&PWM_IN2);
    PWM_IN_INT_Analiza(&PWM_IN3);
    PWM_IN_INT_Analiza(&PWM_IN4);


}
//retorna valor del PWM
uint32_t Lee_PWM(PWM_INS *PWM_IN)
{
	uint32_t ancho;
	//miro si se dio interrpcion
	if (PWM_IN->flag==1)
	{
	ancho=PWM_IN->pulso;
	PWM_IN->flag=0;
	}

	//analiza si se pierde la señal  puse 200ms pero si quiere mas o menos ajuste
	if (uwTick-PWM_IN->TICK_PWM> nosignal)	ancho=0;


	return(ancho);
}


