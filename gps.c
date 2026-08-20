/*
 * GPS.c
 *
 *  Created on: Apr 19, 2025
 *      Author: ALCIDES_RAMOS
 */


#include "GPS.h"

char GPS_buffer[600];  //tamaño buffer para  caprota d edatos gps
#define  hor_utc -5  // define la hora de colombia utc -5


#define trama_gps  GPS_UARTRX  //define la variable asociada al puero serial


double latitud, longitud,velocidad;
uint8_t min_gps,seg_gps,dia_gps,mes_gps,an_gps;
int8_t hor_gps;//
float gps_vel_nudos,gps_vel_kph,gps_rumbo,gps_desv_mag;

//gga
int8_t gps_modo,gps_satelites;
float gps_hor_dilu,gps_altura;



uint8_t GPS_RMC()
{
	uint8_t captura[100];
	uint8_t info[20];
	float grados,minutos;
	//punteros para detectar inicia y fin d ela trama RMC
	const char *start;
    const char *end;
	int8_t diamas=0;// ajusta el dia al UTC
//             limpia buffer
         	 memset(GPS_buffer,0,sizeof(GPS_buffer));

		        size_t length;
		       //   uartx_write_text(&huart2, trama_gps.trama_rx);

		        // Encontrar el primer "$GPRMC"
		           start = strstr(trama_gps.trama_rx, "$GPRMC");
		           if (start != NULL)
		           {
		               // el enter o final d ela trama RMC
		               end = strstr(start, "\r");
						   if (end != NULL)
						   {
							   // Calcular la longitud de la sentencia
							   length = end - start;
							   // Copiar la sentencia completa al buffer de salida
							   strncpy(GPS_buffer, start, length);
							   GPS_buffer[length] = '\0'; // Añadir  cero al final
				           }
		           }
		        //busca primero si es valido el dato
		           strcpy(captura, strtok(GPS_buffer, ","));  //inicia captura de tokens
		           strcpy(captura, strtok(0, ","));  //captura
		           strcpy(captura, strtok(0, ","));  //captura
		           if (captura[0]!=65) return(0);//  si el dato no es valido sale

		       		//si es valido vuelve a capurar
		          //arma el buffer de nuevo
		          strncpy(GPS_buffer, start, length);
        		   GPS_buffer[length] = '\0'; // Añadir  cero al final

        		   strcpy(captura, strtok(GPS_buffer, ","));  //inicia captura de tokens
	     		 strcpy(captura, strtok(0, ","));  //captura

		     		 //comienza la decodificacion
		       		strncpy(info,captura,2);//captura la hora
 	                 hor_gps=atoi(info);
		                hor_gps = hor_gps + hor_utc;

		               if (hor_gps < 0) {
		            	   diamas=-1;  //  es un dia antes al UTC
		            	   hor_gps += 24;// si es negatriva sumo 24
		               } else if (hor_gps >= 24) // si pasa de 24 le retso 24
		               {
		            	   diamas=+1;//  es el dia siguiente
		            	   hor_gps -= 24;
		               }

		            strncpy(info,&captura[2],2);//captura min
    		        min_gps=atoi(info);
    		        strncpy(info,&captura[4],2);//captura seg
    		        seg_gps=atoi(info);

    		        // ya se sabe que es valido solo  que toca capturar de nuevo
    		        strcpy(captura, strtok(0, ","));  //captura hasta 3 coma

    		        //      captura la latitud
    		            strcpy(captura, strtok(0, ","));  //captura hasta 4 coma
		     		    memset(info,0,sizeof(info));//limpía los grados
    		               strncpy(info,captura,2);//captura los grados

    		               grados=atof(info);      // pasa de alfanumerico o cadena a flotante

    		               // apunta a los minutos
    		                 minutos=atoff(&captura[2])/60.0;  //lo pasa a grados

    		                 latitud=grados+minutos;
    		                 strcpy(captura, strtok(0, ","));  //captura siguiente coma  //orientacion o signo de la lat
    		                 if (captura[0]=='S') latitud=-latitud;


    		                 //captura longitud
    		                 strcpy(captura, strtok(0, ","));  //captura siguiente coma la longitud
    		                  memset(info,0,sizeof(info));//limpía los grados
    		                   strncpy(info,captura,3);//captura los grados  3 posiciones
    		                    grados=atof(info);
    		                     // apunta a los minutos
    		                      minutos=atof(&captura[3])/60.0;  //lo pasa a grados
    		                       longitud=grados+minutos;
    		                        strcpy(captura, strtok(0, ","));  //captura siguiente coma  //orientacion o signo de la long
    		                        if (captura[0]=='W') longitud=-longitud;

    		           //captura velocidad

    		             strcpy(captura, strtok(0, ","));  //captura siguenti coma
    		             gps_vel_nudos=atof(captura);
    		             gps_vel_kph = gps_vel_nudos * 1.852;

    		             //captura rumbo
    		             strcpy(captura, strtok(0, ","));  //captura siguenti coma
    		             gps_rumbo=atof(captura);


    		           //captura dia mes año
    		            memset(info,0,sizeof(info));//limpía el  buffer
    		            strcpy(captura, strtok(0, ","));  //
                       strncpy(info,captura,2);//captura la dia
                      dia_gps=atoi(info)+diamas;
                     strncpy(info,&captura[2],2);//captura mes
                      mes_gps=atoi(info);
                      strncpy(info,&captura[4],2);//captura año
                      an_gps=atoi(info);

                      //captura deviacion magnetica
     		             strcpy(captura, strtok(0, ","));  //captura siguenti coma
     		            gps_desv_mag=atof(captura);

                         strcpy(captura, strtok(0, ","));  //captura siguiente coma  //orientacion o signo de la long
                        if (captura[0]=='W')  gps_desv_mag=- gps_desv_mag;




                     return(1);


}

uint8_t GPS_GGA()
{
	uint8_t captura[100];
	uint8_t info[20];
	float grados,minutos;
	//punteros para detectar inicia y fin d ela trama RMC
		const char *start;
	    const char *end;
		int8_t diamas=0;// ajusta el dia al UTC
	//             limpia buffer
	         	 memset(GPS_buffer,0,sizeof(GPS_buffer));

			        size_t length;
			           // Encontrar el primer "$GPGGA"
			           start = strstr(trama_gps.trama_rx, "$GPGGA");
			           if (start != NULL)
			           {
			               // el enter o final d ela trama RMC
			               end = strstr(start, "\r");
							   if (end != NULL)
							   {
								   // Calcular la longitud de la sentencia
								   length = end - start;
								   // Copiar la sentencia completa al buffer de salida
								   strncpy(GPS_buffer, start, length);
								   GPS_buffer[length] = '\0'; // Añadir  cero al final

							   }
			           }

                      //PROCESA LA TRAMA GGA

			           //busca primero si es valido el dato
			          strcpy(captura, strtok(GPS_buffer, ","));  //inicia captura de tokens
			          strcpy(captura, strtok(0, ","));  //captura
			          strcpy(captura, strtok(0, ","));  //captura
			          strcpy(captura, strtok(0, ","));  //captura
			      	  strcpy(captura, strtok(0, ","));  //captura
			      	  strcpy(captura, strtok(0, ","));
			      	  strcpy(captura, strtok(0, ","));
			          if (captura[0]=='0') return(0);//  si el dato no es valido sale
                       gps_modo=atoi(captura);

			          //si es valido vuelve a capurar
        		          //arma el buffer de nuevo
        		          strncpy(GPS_buffer, start, length);
                 		   GPS_buffer[length] = '\0'; // Añadir  cero al final
                        //   uartx_write_text(&huart2, GPS_buffer);

                           //inica captora datos validos

                           //hora
                		   strcpy(captura, strtok(GPS_buffer, ","));  //inicia captura de tokens
        	     		    strcpy(captura, strtok(0, ","));  //captura

        		     		 //comienza la decodificacion
        		       		strncpy(info,captura,2);//captura la hora
         	                 hor_gps=atoi(info);
        		                hor_gps = hor_gps + hor_utc;

        		               if (hor_gps < 0) {
        		            	   diamas=-1;  //  es un dia antes al UTC
        		            	   hor_gps += 24;// si es negatriva sumo 24
        		               } else if (hor_gps >= 24) // si pasa de 24 le retso 24
        		               {
        		            	   diamas=+1;//  es el dia siguiente
        		            	   hor_gps -= 24;
        		               }

        		            strncpy(info,&captura[2],2);//captura min
            		        min_gps=atoi(info);
            		        strncpy(info,&captura[4],2);//captura seg
            		        seg_gps=atoi(info);

            		        //      captura la latitud
            		            strcpy(captura, strtok(0, ","));  //captura hasta 4 coma
        		     		    memset(info,0,sizeof(info));//limpía los grados
            		               strncpy(info,captura,2);//captura los grados
                          //   uartx_write_text(&huart2, info);
            		               grados=atof(info);      // pasa de alfanumerico o cadena a flotante

            		               // apunta a los minutos
            		                 minutos=atoff(&captura[2])/60.0;  //lo pasa a grados

            		                 latitud=grados+minutos;
            		                 strcpy(captura, strtok(0, ","));  //captura siguiente coma  //orientacion o signo de la lat
            		                 if (captura[0]=='S') latitud=-latitud;


            		                 //captura longitud
            		                 strcpy(captura, strtok(0, ","));  //captura siguiente coma la longitud
            		                  memset(info,0,sizeof(info));//limpía los grados
            		                   strncpy(info,captura,3);//captura los grados  3 posiciones
            		                    grados=atof(info);
            		                     // apunta a los minutos
            		                      minutos=atof(&captura[3])/60.0;  //lo pasa a grados
            		                       longitud=grados+minutos;
            		                        strcpy(captura, strtok(0, ","));  //captura siguiente coma  //orientacion o signo de la long
            		                        if (captura[0]=='W') longitud=-longitud;

            		                        strcpy(captura, strtok(0, ","));
            		                         gps_modo=atoi(captura);// calidas gps

             		                        strcpy(captura, strtok(0, ","));
             		                         gps_satelites=atoi(captura);// calidas gps

              		                        strcpy(captura, strtok(0, ","));
              		                         gps_hor_dilu=atof(captura);// calidas gps

               		                        strcpy(captura, strtok(0, ","));
               		                         gps_altura=atof(captura);// calidas gps
                		                      //si la unidad es Kilometros
               		                         strcpy(captura, strtok(0, ","));  //captura siguiente coma  //orientacion o signo de la long

               		                         if (captura[0]=='K')  gps_altura=1000*gps_altura;


                           return(1);
}

void GPS_Interrupt( UART_HandleTypeDef *huart,uint16_t sizex)
{
	if ((GPS_UARTRX .flag_rx==0)&& (huart->Instance == GPS_UARTRX .usart_instance))//si es el uart de datos
			{
			HAL_UART_DMAStop(GPS_UARTRX .huart);  //para la recepcion temporarmente
			GPS_UARTRX .num_datos=sizex;
			GPS_UARTRX .flag_rx=1;
			 }


}
