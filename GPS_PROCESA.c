/*
 * GPS_PROCESA.c
 *
 *  Created on: Jun 11, 2024
 *      Author: Alcides Ramos
 */

#include "GPS_PROCESA.h"
float latitud, longitud,velocidad;
uint8_t min_gps,seg_gps,dia_gps,mes_gps,an_gps;
int8_t hor_gps;//

#define  hor_utc -5  // define la hora ne colombia utc -5

uint8_t GPS_RMC( char *trama_gps)
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
		        // Encontrar el primer "$GNRMC"
		           start = strstr(trama_gps, "$GNRMC");
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
		        //busca primero si e svalido el dato
		           strcpy(captura, strtok(GPS_buffer, ","));  //inicia captura de tokens
		            strcpy(captura, strtok(0, ","));  //captura hasta segunda coma
		            strcpy(captura, strtok(0, ","));  //captura hasta 3 coma

		            if (captura[0]!=65) return(0);//  si el dato no es valido sale


		       		//si es valido vuelve a capurar
		          //arma el buffer de nuevo
		            strncpy(GPS_buffer, start, length);
        		   GPS_buffer[length] = '\0'; // Añadir  cero al final
        		    strcpy(captura, strtok(GPS_buffer, ","));  //inicia captura de tokens
		     		 strcpy(captura, strtok(0, ","));  //captura hasta segunda coma
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
    		                 minutos=atof(&captura[2])/60.0;  //lo pasa a grados
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
                      //captura dia mes año
    		            memset(info,0,sizeof(info));//limpía el  buffer
    		            strcpy(captura, strtok(0, ","));  //captura hasta 5 coma
                       strcpy(captura, strtok(0, ","));  //captura hasta 6 coma
                       strcpy(captura, strtok(0, ","));  //captura hasta 7 coma
                       strncpy(info,captura,2);//captura la dia
                      dia_gps=atoi(info)+diamas;
                     strncpy(info,&captura[2],2);//captura mes
                      mes_gps=atoi(info);
                      strncpy(info,&captura[4],2);//captura año
                      an_gps=atoi(info);





                     return(1);


}
