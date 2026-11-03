/*
 * MQTT.h
 *
 *  Created on: Oct 19, 2026
 *      Author: ALCIDES_RAMOS
 *
 *  Cliente MQTT 3.1.1 minimo para modulo DT-06 (ESP8266) en modo
 *  transparente UART <-> TCP. El DT-06 ya abre el socket TCP contra
 *  thingsboard.cloud:1883, el STM32 solo arma y lee los paquetes MQTT.
 *
 *  Usa el UART definido en UARTRX.c  ->  UARTRXS MQTT_UARTRX
 */

#ifndef LIBRERIAS_MQTT_H_
#define LIBRERIAS_MQTT_H_

#include "main.h"
#include "UARTRX.h"

//********************* CONFIGURACION *********************
#define MQTT_CLIENT_ID     "CHARLA"                 // nombre del device
#define MQTT_TOKEN         "S7F0xjp2xvB8Fj3MlLSc"          // access token del device en thingsboard
#define MQTT_KEEPALIVE     60                       // segundos
#define MQTT_TIMEOUT       3000                     // ms de espera de respuestas (CONNACK, SUBACK...)
#define MQTT_PING_MS       15000                    // cada cuanto se verifica la conexion con PINGREQ
#define MQTT_TX_SIZE       512                      // tamaño maximo de un paquete a enviar
#define MQTT_TOPIC_SIZE    128                      // tamaño maximo del topic recibido
#define MQTT_PAYLOAD_SIZE  512                      // tamaño maximo del payload recibido

//topics de thingsboard
#define TB_TELEMETRIA      "v1/devices/me/telemetry"
#define TB_ATRIBUTOS       "v1/devices/me/attributes"
#define TB_ATRIB_REQUEST   "v1/devices/me/attributes/request/"
#define TB_ATRIB_RESPONSE  "v1/devices/me/attributes/response/+"
#define TB_RPC_REQUEST     "v1/devices/me/rpc/request/+"
#define TB_RPC_RESPONSE    "v1/devices/me/rpc/response/"
//*********************************************************

//estado de la conexion  1=conectado al broker
extern uint8_t MQTT_conectado;
extern uint16_t MQTT_reconexiones;                  // veces que se cayo y se volvio a conectar

//ultimo mensaje recibido (PUBLISH del broker)
extern uint8_t MQTT_flag_msg;                       // 1 = llego mensaje nuevo, ponerlo en 0 al procesar
extern char    MQTT_topic_rx[MQTT_TOPIC_SIZE];
extern char    MQTT_payload_rx[MQTT_PAYLOAD_SIZE];
extern char MQTT_TX_BUFF[];


//funciones
void    MQTT_Init(void);
uint8_t MQTT_Connect(void);
void    MQTT_Disconnect(void);
uint8_t MQTT_Publish(const char *topic, const char *payload);
uint8_t MQTT_Subscribe(const char *topic);
uint8_t MQTT_Ping(void);
void    MQTT_Loop(void);

//funciones para thingsboard
uint8_t MQTT_Telemetria(const char *json);
uint8_t MQTT_Telemetria_Float(const char *clave, float valor);
uint8_t MQTT_Atributos(const char *json);
uint8_t MQTT_Consultar_Atributos(const char *claves_cliente, const char *claves_compartidas);
uint8_t MQTT_RPC_Responder(const char *topic_request, const char *json);

//RPC automaticos (get<nombre> / set<nombre>), retornan el valor nuevo o el actual
//ej: led = MQTT_RPC_Bool("LedStatus", led);   -> atiende getLedStatus y setLedStatus
uint8_t MQTT_RPC_Bool(const char *nombre, uint8_t actual);
int32_t MQTT_RPC_Int(const char *nombre, int32_t actual);
float   MQTT_RPC_Float(const char *nombre, float actual);
void MQTT_Interrupt( UART_HandleTypeDef *huart,uint16_t sizex);
void MQTT_Error(UART_HandleTypeDef *huart);
#endif /* LIBRERIAS_MQTT_H_ */

/*  COMO USAR 

 colocar en MQTT.h
    
     MQTT_CLIENT_ID  -> nombre del device
     MQTT_TOKEN      -> access token del device en thingsboard

   en UARTRX.c incluir MQTT.h 
//inciar uart de recepcion por interrupcion  y tamaño del buffer
UARTRXS MQTT_UARTRX= {&huart1,USART1,MQTT_PAYLOAD_SIZE };
Y EN LO PROcEDIMEINTOS
void uartRX_INTERRUPT(UART_HandleTypeDef *huart,uint16_t sizex)
{
AGREGAR ESTO
MQTT_Interrupt(huart,sizex);   //uart del DT-06: marca flag_rx, los paquetes se procesan en MQTT_Loop

Y EN EL
 void uartRX_Errores(UART_HandleTypeDef *huart)
 {
AGRAGAR ESTO
MQTT_Error(huart);  

 En el main incluir
      #include "MQTT.h"

  Antes del while(1)
      MQTT_Init();                       //inicia la recepcion por DMA del MQTT_UARTRX
      HAL_Delay(3000);                   //da tiempo al DT-06 de conectarse al wifi y al servidor
      MQTT_Connect();                    //si falla, MQTT_Loop reintenta cada 5 s
      MQTT_Subscribe(TB_RPC_REQUEST);    //recibir comandos RPC desde thingsboard

  En el while(1)
      MQTT_Loop();                       //OBLIGATORIO: procesa lo recibido, ping y reconexion

    ENVIAR telemetria (usar un tiempo, no mandar en cada vuelta del while)
      sprintf(MQTT_TX_BUFF, "{\"temperatura\":%.2f,\"humedad\":%.2f}", t, h);
      MQTT_Telemetria(MQTT_TX_BUFF);
      MQTT_Telemetria_Float("voltaje", v);          //o una sola variable

    RECIBIR desde thingsboard con RPC automaticos: se pasa el nombre y la
    variable, la funcion atiende get<nombre> (responde el valor actual) y
    set<nombre> (retorna el valor nuevo). En el widget del dashboard poner
    los metodos getLedStatus / setLedStatus, etc.
      led_estado   = MQTT_RPC_Bool ("LedStatus",  led_estado);    //switch
      servo_angulo = MQTT_RPC_Float("ServoAngle", servo_angulo);  //slider / knob
      contador     = MQTT_RPC_Int  ("Contador",   contador);

    mensajes que ninguna funcion RPC atendio (otros metodos o atributos):
    quedan en MQTT_topic_rx / MQTT_payload_rx, limpiar la bandera DESPUES
    de las funciones RPC para que no bloquee los mensajes siguientes
      if (MQTT_flag_msg == 1)
      {
          MQTT_flag_msg = 0;
      }

 6) Opcional: consultar atributos al servidor (la respuesta llega en
    MQTT_payload_rx con topic v1/devices/me/attributes/response/<id>)
      MQTT_Subscribe(TB_ATRIB_RESPONSE);           //una sola vez, antes del while
      MQTT_Consultar_Atributos("", "setpoint,modo");

 */
