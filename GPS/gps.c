/*
 * GPS.c
 *
 *  Created on: Agosto 4 2026
 *      Author: ALCIDES_RAMOS
 */


#include "GPS.h"

char GPS_buffer[500];  //tamaño buffer para  caprota d edatos gps



#define trama_gps  GPS_UARTRX  //define la variable asociada al puero serial

#define radio_tierra 6371000.0 	//radio de la tierra en metros
#define DEG_TO_RAD (M_PI / 180.0)


double latitud, longitud,velocidad;
uint8_t min_gps,seg_gps,dia_gps,mes_gps,an_gps;
int8_t hor_gps;//
float gps_vel_nudos,gps_vel_kph,gps_rumbo,gps_desv_mag;

//gga
int8_t gps_modo,gps_satelites;
float gps_hor_dilu,gps_altura;

// Separa una cadena por delimitador SIN saltar campos vacios
// (a diferencia de strtok, que colapsa delimitadores consecutivos).
// Las tramas NMEA traen campos vacios entre comas (ej: ,,,) cuando
// un dato no esta disponible (sin fix, sin GGA valido, etc.), y usar
// strtok en ese caso desalinea la captura de los campos siguientes y
// puede terminar devolviendo NULL -> strcpy(dest,NULL) -> HardFault.
static char *nmea_next_field(char **str)
{
	char *start = *str;
	char *sep;
	if (start == NULL) return NULL; // ya no quedan mas campos en la trama
	sep = strchr(start, ',');
	if (sep != NULL)
	{
		*sep = '\0';
		*str = sep + 1;
	}
	else
	{
		*str = NULL; // este era el ultimo campo de la trama
	}
	return start;
}

// Copia el siguiente campo NMEA a "captura" de forma segura.
// Si la trama esta incompleta (no quedan mas campos) retorna 0
// desde la funcion que la invoque, en vez de arriesgar un strcpy(NULL).
#define NMEA_FIELD(saveptr, dst) \
	do { \
		char *_tok = nmea_next_field(&(saveptr)); \
		if (_tok == NULL) return(0); \
		strncpy((dst), _tok, sizeof(dst) - 1); \
		(dst)[sizeof(dst) - 1] = '\0'; \
	} while (0)

uint8_t GPS_RMC()
{
	char captura[100];
	char info[20];
	char *saveptr;
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
		           start = strstr((char*)trama_gps.trama_rx, "$GPRMC");
		           if (start == NULL) return(0); // no se encontro la trama, sale sin procesar

		               // el enter o final d ela trama RMC
		               end = strstr(start, "\r");
		               if (end == NULL) return(0); // trama incompleta (sin CR), sale sin procesar

							   // Calcular la longitud de la sentencia
							   length = end - start;
							   if (length >= sizeof(GPS_buffer)) return(0); // evita desbordar GPS_buffer
							   // Copiar la sentencia completa al buffer de salida
							   strncpy(GPS_buffer, start, length);
							   GPS_buffer[length] = '\0'; // Añadir  cero al final
		        //busca primero si es valido el dato
		           saveptr = GPS_buffer;
		           NMEA_FIELD(saveptr, captura);  //inicia captura de tokens
		           NMEA_FIELD(saveptr, captura);  //captura
		           NMEA_FIELD(saveptr, captura);  //captura
		           if (captura[0]!=65) return(0);//  si el dato no es valido sale

		       		//si es valido vuelve a capurar
		          //arma el buffer de nuevo
		          strncpy(GPS_buffer, start, length);
        		   GPS_buffer[length] = '\0'; // Añadir  cero al final

        		   saveptr = GPS_buffer;
        		   NMEA_FIELD(saveptr, captura);  //inicia captura de tokens
	     		 NMEA_FIELD(saveptr, captura);  //captura

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
    	        NMEA_FIELD(saveptr, captura);  //captura hasta 3 coma

    	        //      captura la latitud
    	            NMEA_FIELD(saveptr, captura);  //captura hasta 4 coma
	     		    memset(info,0,sizeof(info));//limpía los grados
    	               strncpy(info,captura,2);//captura los grados

    	               grados=atof(info);      // pasa de alfanumerico o cadena a flotante

    	               // apunta a los minutos
    	                 minutos=atof(&captura[2])/60.0;  //lo pasa a grados

    	                 latitud=grados+minutos;
    	                 NMEA_FIELD(saveptr, captura);  //captura siguiente coma  //orientacion o signo de la lat
    	                 if (captura[0]=='S') latitud=-latitud;


    	                 //captura longitud
    	                 NMEA_FIELD(saveptr, captura);  //captura siguiente coma la longitud
    	                  memset(info,0,sizeof(info));//limpía los grados
    	                   strncpy(info,captura,3);//captura los grados  3 posiciones
    	                    grados=atof(info);
    	                     // apunta a los minutos
    	                      minutos=atof(&captura[3])/60.0;  //lo pasa a grados
    	                       longitud=grados+minutos;
    	                        NMEA_FIELD(saveptr, captura);  //captura siguiente coma  //orientacion o signo de la long
    	                        if (captura[0]=='W') longitud=-longitud;

    	           //captura velocidad

    	             NMEA_FIELD(saveptr, captura);  //captura siguenti coma
    	             gps_vel_nudos=atof(captura);
    	             gps_vel_kph = gps_vel_nudos * 1.852;

    	             //captura rumbo
    	             NMEA_FIELD(saveptr, captura);  //captura siguenti coma
    	             gps_rumbo=atof(captura);


    	           //captura dia mes año
    	            memset(info,0,sizeof(info));//limpía el  buffer
    	            NMEA_FIELD(saveptr, captura);  //
                       strncpy(info,captura,2);//captura la dia
                      dia_gps=atoi(info)+diamas;
                     strncpy(info,&captura[2],2);//captura mes
                      mes_gps=atoi(info);
                      strncpy(info,&captura[4],2);//captura año
                      an_gps=atoi(info);

                      //captura deviacion magnetica
     	             NMEA_FIELD(saveptr, captura);  //captura siguenti coma
     	            gps_desv_mag=atof(captura);

                         NMEA_FIELD(saveptr, captura);  //captura siguiente coma  //orientacion o signo de la long
                        if (captura[0]=='W')  gps_desv_mag=- gps_desv_mag;




                     return(1);


}

uint8_t GPS_GGA()
{
	char captura[100];
	char info[20];
	char *saveptr;
	float grados,minutos;
	//punteros para detectar inicia y fin d ela trama RMC
		const char *start;
	    const char *end;
	//             limpia buffer
	         	 memset(GPS_buffer,0,sizeof(GPS_buffer));

		        size_t length;
		           // Encontrar el primer "$GPGGA"
		           start = strstr((char*)trama_gps.trama_rx, "$GPGGA");
		           if (start == NULL) return(0); // no se encontro la trama, sale sin procesar

		               // el enter o final d ela trama RMC
		               end = strstr(start, "\r");
		               if (end == NULL) return(0); // trama incompleta (sin CR), sale sin procesar

						   // Calcular la longitud de la sentencia
						   length = end - start;
						   if (length >= sizeof(GPS_buffer)) return(0); // evita desbordar GPS_buffer
						   // Copiar la sentencia completa al buffer de salida
						   strncpy(GPS_buffer, start, length);
						   GPS_buffer[length] = '\0'; // Añadir  cero al final

                      //PROCESA LA TRAMA GGA

		           //busca primero si es valido el dato
		          saveptr = GPS_buffer;
		          NMEA_FIELD(saveptr, captura);  //inicia captura de tokens
		          NMEA_FIELD(saveptr, captura);  //captura
		          NMEA_FIELD(saveptr, captura);  //captura
		          NMEA_FIELD(saveptr, captura);  //captura
		      	  NMEA_FIELD(saveptr, captura);  //captura
		      	  NMEA_FIELD(saveptr, captura);
		      	  NMEA_FIELD(saveptr, captura);
		          if (captura[0]=='0') return(0);//  si el dato no es valido sale
                       gps_modo=atoi(captura);

		          //si es valido vuelve a capurar
        	          //arma el buffer de nuevo
        	          strncpy(GPS_buffer, start, length);
                 	   GPS_buffer[length] = '\0'; // Añadir  cero al final
                        //   uartx_write_text(&huart2, GPS_buffer);

                           //inica captora datos validos

                           //hora
                	   saveptr = GPS_buffer;
                	   NMEA_FIELD(saveptr, captura);  //inicia captura de tokens
        	     	    NMEA_FIELD(saveptr, captura);  //captura

        		     		 //comienza la decodificacion
        		       		strncpy(info,captura,2);//captura la hora
         	                 hor_gps=atoi(info);
        		                hor_gps = hor_gps + hor_utc;

        		               if (hor_gps < 0) {
        		            	   hor_gps += 24;// si es negatriva sumo 24
        		               } else if (hor_gps >= 24) // si pasa de 24 le retso 24
        		               {
        		            	   hor_gps -= 24;
        		               }

        		            strncpy(info,&captura[2],2);//captura min
            		        min_gps=atoi(info);
            		        strncpy(info,&captura[4],2);//captura seg
            		        seg_gps=atoi(info);

            		        //      captura la latitud
            	            NMEA_FIELD(saveptr, captura);  //captura hasta 4 coma
        	     		    memset(info,0,sizeof(info));//limpía los grados
            	               strncpy(info,captura,2);//captura los grados
                          //   uartx_write_text(&huart2, info);
            	               grados=atof(info);      // pasa de alfanumerico o cadena a flotante

            	               // apunta a los minutos
            	                 minutos=atof(&captura[2])/60.0;  //lo pasa a grados

            	                 latitud=grados+minutos;
            	                 NMEA_FIELD(saveptr, captura);  //captura siguiente coma  //orientacion o signo de la lat
            	                 if (captura[0]=='S') latitud=-latitud;


            	                 //captura longitud
            	                 NMEA_FIELD(saveptr, captura);  //captura siguiente coma la longitud
            	                  memset(info,0,sizeof(info));//limpía los grados
            	                   strncpy(info,captura,3);//captura los grados  3 posiciones
            	                    grados=atof(info);
            	                     // apunta a los minutos
            	                      minutos=atof(&captura[3])/60.0;  //lo pasa a grados
            	                       longitud=grados+minutos;
            	                        NMEA_FIELD(saveptr, captura);  //captura siguiente coma  //orientacion o signo de la long
            	                        if (captura[0]=='W') longitud=-longitud;

            	                        NMEA_FIELD(saveptr, captura);
            	                         gps_modo=atoi(captura);// calidas gps

             	                        NMEA_FIELD(saveptr, captura);
             	                         gps_satelites=atoi(captura);// #de satelites

              	                        NMEA_FIELD(saveptr, captura);
              	                         gps_hor_dilu=atof(captura);// dilucion horizontal

               	                        NMEA_FIELD(saveptr, captura);
               	                         gps_altura=atof(captura);// altura
                	                      //si la unidad es Kilometros
               	                         NMEA_FIELD(saveptr, captura);  //captura siguiente coma  //orientacion o signo de la long
               		                         if (captura[0]=='K')  gps_altura=1000*gps_altura;


                           return(1);
}

void GPS_Interrupt( UART_HandleTypeDef *huart,uint16_t sizex)
{
	if ((GPS_UARTRX.flag_rx==0)&& (huart->Instance == GPS_UARTRX.usart_instance))//si es el uart de datos
		{
	
       HAL_UART_DMAStop(GPS_UARTRX.huart);  //para la recepcion temporarmente
	     GPS_UARTRX.num_datos=sizex;
	     GPS_UARTRX.flag_rx=1;
		 }


}

void GPS_Error(UART_HandleTypeDef *huart)
{
if (huart->Instance == GPS_UARTRX.usart_instance)//si es el uart de datos
		{
		__HAL_UART_CLEAR_OREFLAG(GPS_UARTRX.huart); // Limpia Overrun Error
        __HAL_UART_FLUSH_DRREGISTER(GPS_UARTRX.huart); // Limpia buffer de entrada

		  if(GPS_UARTRX.flag_rx==0)
		{
		HAL_UART_DMAStop(GPS_UARTRX.huart);  //para la recepcion temporarmente
		HAL_UARTEx_ReceiveToIdle_DMA(GPS_UARTRX.huart, (uint8_t*)GPS_UARTRX.trama_rx, GPS_UARTRX.sizeT);//inica la recepcion por idle
		__HAL_DMA_DISABLE_IT(GPS_UARTRX.huart->hdmarx, DMA_IT_HT); // deshabilita HT, deja activo IDLE + buffer lleno

		}
	  }

}

// Calcula la distancia (en metros) entre dos puntos GPS
 // usando la fórmula de Haversine.
 
double calcularDistanciaGPS(double lat1, double lon1, double lat2, double lon2)
{
    double dLat = (lat2 - lat1) * DEG_TO_RAD;
    double dLon = (lon2 - lon1) * DEG_TO_RAD;

    double lat1_rad = lat1 * DEG_TO_RAD;
    double lat2_rad = lat2 * DEG_TO_RAD;

    double a = sinf(dLat / 2) * sinf(dLat / 2) +
              cosf(lat1_rad) * cosf(lat2_rad) *
              sinf(dLon / 2) * sinf(dLon / 2);

    double c = 2.0f * atan2f(sqrtf(a), sqrtf(1.0f - a));

    return (radio_tierra * c);  // Distancia en metros
}

 // Verifica si la posición actual (leída del GPS)
 // está dentro del radio de operación permitido.
 // Retorna 1 si está dentro, 0 si está fuera.
 
uint8_t estaDentroDelRadio(double lat1, double lon1,double lat_gps, double lon_gps)
{
    double distancia = calcularDistanciaGPS(lat1, lon1, lat_gps, lon_gps);

    if (distancia <= presicion)
    {
        return 1;  // Dentro del radio de operación
    }
    else
    {
        return 0;  // Fuera del radio de operación
    }
}