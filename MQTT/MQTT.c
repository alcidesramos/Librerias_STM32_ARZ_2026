/*
 * MQTT.c
 *
 *  Created on: Oct 19, 2026
 *      Author: ALCIDES_RAMOS
 *
 *  Cliente MQTT 3.1.1 (QoS 0) sobre UART transparente (DT-06 -> thingsboard)
 */

#include "MQTT.h"
#include "uart.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#define MQTT_MAX_SUBS  4          // topics que se vuelven a suscribir al reconectar

uint8_t MQTT_conectado = 0;
uint16_t MQTT_reconexiones = 0;
uint8_t MQTT_flag_msg = 0;
char    MQTT_topic_rx[MQTT_TOPIC_SIZE];
char    MQTT_payload_rx[MQTT_PAYLOAD_SIZE];
char MQTT_TX_BUFF[MQTT_TX_SIZE ];


static uint8_t     tx[MQTT_TX_SIZE];          // buffer de armado de paquetes
static uint16_t    packet_id = 1;
static uint16_t    request_id = 1;            // id para consultas de atributos
static uint8_t     ultimo_tipo = 0;           // ultimo paquete de control recibido
static uint8_t     esperando_ping = 0;
static uint32_t    t_ping = 0;
static uint32_t    t_reintento = 0;
static const char *subs[MQTT_MAX_SUBS];
static uint8_t     num_subs = 0;


//*********************************************************
// funciones internas para armar paquetes

//escribe el "remaining length" codificado (1 a 4 bytes), retorna bytes usados
static uint8_t mqtt_largo(uint8_t *p, uint32_t largo)
{
	uint8_t n = 0;
	do
	{
		uint8_t dato = largo % 128;
		largo /= 128;
		if (largo > 0) dato |= 0x80;
		p[n++] = dato;
	} while (largo > 0 && n < 4);
	return n;
}

//escribe un string MQTT (2 bytes de largo + texto), retorna bytes usados
static uint16_t mqtt_string(uint8_t *p, const char *s)
{
	uint16_t len = strlen(s);
	p[0] = len >> 8;
	p[1] = len & 0xFF;
	memcpy(&p[2], s, len);
	return len + 2;
}

static void mqtt_enviar(uint8_t *buf, uint16_t len)
{
	uartx_write_buffer(MQTT_UARTRX.huart, buf, len);
}

static uint16_t mqtt_nuevo_id(void)
{
	packet_id++;
	if (packet_id == 0) packet_id = 1;
	return packet_id;
}


//*********************************************************
// recepcion: procesa los paquetes que llegan por MQTT_UARTRX

static void mqtt_procesa_paquete(uint8_t tipo, uint8_t *p, uint32_t largo)
{
	switch (tipo & 0xF0)
	{
	case 0x20:  // CONNACK  -> p[1] = codigo de retorno (0 = aceptado)
		MQTT_conectado = (largo >= 2 && p[1] == 0) ? 1 : 0;
		break;

	case 0xD0:  // PINGRESP
		esperando_ping = 0;
		break;

	case 0x30:  // PUBLISH desde el servidor
	{
		uint8_t  qos = (tipo >> 1) & 0x03;
		uint16_t tlen = (p[0] << 8) | p[1];
		uint32_t pos = 2 + tlen;
		if (pos > largo) break;

		uint16_t n = (tlen < MQTT_TOPIC_SIZE - 1) ? tlen : MQTT_TOPIC_SIZE - 1;
		memcpy(MQTT_topic_rx, &p[2], n);
		MQTT_topic_rx[n] = 0;

		if (qos > 0)  // trae packet id, responder PUBACK
		{
			uint8_t ack[4] = {0x40, 0x02, p[pos], p[pos + 1]};
			pos += 2;
			mqtt_enviar(ack, 4);
		}

		uint32_t plen = largo - pos;
		if (plen > MQTT_PAYLOAD_SIZE - 1) plen = MQTT_PAYLOAD_SIZE - 1;
		memcpy(MQTT_payload_rx, &p[pos], plen);
		MQTT_payload_rx[plen] = 0;
		MQTT_flag_msg = 1;
		break;
	}

	default:    // SUBACK, UNSUBACK, PUBACK... solo se registra el tipo
		break;
	}
	ultimo_tipo = tipo & 0xF0;
}

static void mqtt_procesa_rx(void)
{
	if (MQTT_UARTRX.flag_rx == 0) return;

	uint8_t *buf = MQTT_UARTRX.trama_rx;
	uint16_t n = MQTT_UARTRX.num_datos;
	uint16_t i = 0;

	//en una misma trama pueden venir varios paquetes MQTT
	while (i + 2 <= n)
	{
		uint8_t  tipo = buf[i];
		uint32_t largo = 0;
		uint32_t mult = 1;
		uint16_t j = i + 1;
		uint8_t  dato = 0;
		do
		{
			if (j >= n) { i = n; break; }
			dato = buf[j++];
			largo += (dato & 0x7F) * mult;
			mult *= 128;
		} while ((dato & 0x80) && mult <= 128 * 128 * 128);

		if (i >= n || j + largo > n) break;   // paquete incompleto o basura
		mqtt_procesa_paquete(tipo, &buf[j], largo);
		i = j + largo;
	}

	uartRX_DMA_Re_init(&MQTT_UARTRX);
}

//espera un tipo de paquete (CONNACK, SUBACK...) hasta MQTT_TIMEOUT
static uint8_t mqtt_esperar(uint8_t tipo)
{
	uint32_t inicio = HAL_GetTick();
	ultimo_tipo = 0;
	while ((HAL_GetTick() - inicio) < MQTT_TIMEOUT)
	{
		mqtt_procesa_rx();
		if (ultimo_tipo == tipo) return 1;
	}
	return 0;
}


//*********************************************************
// funciones publicas

//inicia la recepcion por DMA del uart del DT-06, llamar una vez antes del while
void MQTT_Init(void)
{
	uartRX_it_idle_dma_init(&MQTT_UARTRX);
	MQTT_conectado = 0;
	MQTT_flag_msg = 0;
}

//manda CONNECT con client id y token, retorna 1 si el servidor acepto
uint8_t MQTT_Connect(void)
{
	uint8_t  body[128];                           // client id + token caben de sobra
	uint16_t b = 0;

	b += mqtt_string(&body[b], "MQTT");          // protocolo
	body[b++] = 0x04;                            // nivel 4 = MQTT 3.1.1
	body[b++] = 0x02 | 0x80;                     // clean session + username
	body[b++] = MQTT_KEEPALIVE >> 8;
	body[b++] = MQTT_KEEPALIVE & 0xFF;
	b += mqtt_string(&body[b], MQTT_CLIENT_ID);
	b += mqtt_string(&body[b], MQTT_TOKEN);      // en thingsboard el token va como usuario

	uint16_t t = 0;
	tx[t++] = 0x10;
	t += mqtt_largo(&tx[t], b);
	memcpy(&tx[t], body, b);
	t += b;

	MQTT_conectado = 0;
	esperando_ping = 0;
	mqtt_enviar(tx, t);

	if (mqtt_esperar(0x20) == 0) MQTT_conectado = 0;
	t_ping = HAL_GetTick();
	return MQTT_conectado;
}

void MQTT_Disconnect(void)
{
	uint8_t pk[2] = {0xE0, 0x00};
	mqtt_enviar(pk, 2);
	MQTT_conectado = 0;
}

//publica un texto en un topic (QoS 0), retorna 1 si se envio
uint8_t MQTT_Publish(const char *topic, const char *payload)
{
	if (!MQTT_conectado) return 0;

	uint16_t tlen = strlen(topic);
	uint16_t plen = strlen(payload);
	uint32_t largo = 2 + tlen + plen;
	if (largo + 5 > MQTT_TX_SIZE) return 0;

	uint16_t t = 0;
	tx[t++] = 0x30;
	t += mqtt_largo(&tx[t], largo);
	t += mqtt_string(&tx[t], topic);
	memcpy(&tx[t], payload, plen);
	t += plen;

	mqtt_enviar(tx, t);
	return 1;
}

//suscribe a un topic (QoS 0). Pasar textos constantes (ej TB_RPC_REQUEST)
//porque se guarda el puntero para volver a suscribir al reconectar
uint8_t MQTT_Subscribe(const char *topic)
{
	uint8_t ya = 0;
	for (uint8_t k = 0; k < num_subs; k++) if (subs[k] == topic) ya = 1;
	if (!ya && num_subs < MQTT_MAX_SUBS) subs[num_subs++] = topic;

	if (!MQTT_conectado) return 0;

	uint16_t tlen = strlen(topic);
	uint32_t largo = 2 + 2 + tlen + 1;
	if (largo + 5 > MQTT_TX_SIZE) return 0;

	uint16_t id = mqtt_nuevo_id();
	uint16_t t = 0;
	tx[t++] = 0x82;
	t += mqtt_largo(&tx[t], largo);
	tx[t++] = id >> 8;
	tx[t++] = id & 0xFF;
	t += mqtt_string(&tx[t], topic);
	tx[t++] = 0x00;                              // QoS 0

	mqtt_enviar(tx, t);
	return mqtt_esperar(0x90);                   // SUBACK
}

//manda PINGREQ, la respuesta se revisa en MQTT_Loop
uint8_t MQTT_Ping(void)
{
	if (!MQTT_conectado) return 0;
	uint8_t pk[2] = {0xC0, 0x00};
	mqtt_enviar(pk, 2);
	esperando_ping = 1;
	t_ping = HAL_GetTick();
	return 1;
}

//llamar siempre en el while(1): procesa lo recibido, mantiene viva la
//conexion con ping y reconecta si se cae
void MQTT_Loop(void)
{
	uint32_t ahora = HAL_GetTick();

	mqtt_procesa_rx();

	if (MQTT_conectado)
	{
		if (esperando_ping && (ahora - t_ping) > MQTT_TIMEOUT)
		{
			MQTT_conectado = 0;                  // el servidor no responde
			t_reintento = ahora;
		}
		//ping periodico aunque se este publicando: con QoS 0 es la unica forma
		//de saber si el DT-06 perdio el socket y la sesion MQTT ya no existe
		else if (!esperando_ping && (ahora - t_ping) > MQTT_PING_MS)
		{
			MQTT_Ping();
		}
	}
	else if ((ahora - t_reintento) > 5000)
	{
		t_reintento = ahora;
		if (MQTT_Connect())
		{
			MQTT_reconexiones++;
			for (uint8_t k = 0; k < num_subs; k++) MQTT_Subscribe(subs[k]);
		}
		t_reintento = HAL_GetTick();
	}
}


//*********************************************************
// funciones para thingsboard

//manda telemetria en json, ej: {"temperatura":25.3,"humedad":60}
uint8_t MQTT_Telemetria(const char *json)
{
	return MQTT_Publish(TB_TELEMETRIA, json);
}

//manda una sola variable de telemetria
uint8_t MQTT_Telemetria_Float(const char *clave, float valor)
{
	char json[64];
	snprintf(json, sizeof(json), "{\"%s\":%.2f}", clave, valor);
	return MQTT_Publish(TB_TELEMETRIA, json);
}

//manda atributos de cliente en json, ej: {"firmware":"1.0"}
uint8_t MQTT_Atributos(const char *json)
{
	return MQTT_Publish(TB_ATRIBUTOS, json);
}

//consulta atributos al servidor, claves separadas por coma ("" si no se usan)
//la respuesta llega en MQTT_payload_rx (suscribirse antes a TB_ATRIB_RESPONSE)
uint8_t MQTT_Consultar_Atributos(const char *claves_cliente, const char *claves_compartidas)
{
	char topic[48];
	char json[200];
	int  n = 0;

	snprintf(topic, sizeof(topic), "%s%u", TB_ATRIB_REQUEST, request_id++);

	n += snprintf(&json[n], sizeof(json) - n, "{");
	if (claves_cliente && claves_cliente[0])
		n += snprintf(&json[n], sizeof(json) - n, "\"clientKeys\":\"%s\"", claves_cliente);
	if (claves_compartidas && claves_compartidas[0])
		n += snprintf(&json[n], sizeof(json) - n, "%s\"sharedKeys\":\"%s\"",
		              (n > 1) ? "," : "", claves_compartidas);
	snprintf(&json[n], sizeof(json) - n, "}");

	return MQTT_Publish(topic, json);
}

//responde un RPC usando el topic recibido (v1/devices/me/rpc/request/ID)
uint8_t MQTT_RPC_Responder(const char *topic_request, const char *json)
{
	char topic[64];
	const char *id = strrchr(topic_request, '/');
	if (id == NULL) return 0;
	snprintf(topic, sizeof(topic), "%s%s", TB_RPC_RESPONSE, id + 1);
	return MQTT_Publish(topic, json);
}

void MQTT_Interrupt( UART_HandleTypeDef *huart,uint16_t sizex)
{
	if ((MQTT_UARTRX.flag_rx==0)&& (huart->Instance == MQTT_UARTRX.usart_instance))//si es el uart de datos
		{
	
       HAL_UART_DMAStop(MQTT_UARTRX.huart);  //para la recepcion temporarmente
	     MQTT_UARTRX.num_datos=sizex;
	     MQTT_UARTRX.flag_rx=1;
		 }


}

void MQTT_Error(UART_HandleTypeDef *huart)
{
	
if (huart->Instance == MQTT_UARTRX.usart_instance)//si es el uart de datos
		{
		__HAL_UART_CLEAR_OREFLAG(MQTT_UARTRX.huart); // Limpia Overrun Error
        __HAL_UART_FLUSH_DRREGISTER(MQTT_UARTRX.huart); // Limpia buffer de entrada

		  if(MQTT_UARTRX.flag_rx==0)
		{
		HAL_UART_DMAStop(MQTT_UARTRX.huart);  //para la recepcion temporarmente
		HAL_UARTEx_ReceiveToIdle_DMA(MQTT_UARTRX.huart, (uint8_t*)MQTT_UARTRX.trama_rx, MQTT_UARTRX.sizeT);//inica la recepcion por idle
		__HAL_DMA_DISABLE_IT(MQTT_UARTRX.huart->hdmarx, DMA_IT_HT); // deshabilita HT, deja activo IDLE + buffer lleno

		}
	  }

}

//*********************************************************
// RPC automaticos: se pasa el nombre (ej "LedStatus") y la funcion
// atiende "getLedStatus" (responde el valor actual) y "setLedStatus"
// (lee el valor nuevo, responde y lo retorna). Si el mensaje no es para
// ese nombre, retorna el valor actual sin cambios.

//1 si el RPC pendiente es <prefijo><nombre>, ej: "set" + "LedStatus"
static uint8_t rpc_es(const char *prefijo, const char *nombre)
{
	char metodo[48];
	if (!MQTT_flag_msg || !strstr(MQTT_topic_rx, "rpc/request")) return 0;
	snprintf(metodo, sizeof(metodo), "\"%s%s\"", prefijo, nombre);
	return strstr(MQTT_payload_rx, metodo) != NULL;
}

//apunta al valor que sigue a "params": (salta espacios y comillas)
static const char *rpc_params(void)
{
	const char *p = strstr(MQTT_payload_rx, "\"params\":");
	if (p == NULL) return "";
	p += 9;
	while (*p == ' ' || *p == '"') p++;
	return p;
}

//responde y marca el mensaje como atendido
static void rpc_responder(const char *txt)
{
	MQTT_RPC_Responder(MQTT_topic_rx, txt);
	MQTT_flag_msg = 0;
}

uint8_t MQTT_RPC_Bool(const char *nombre, uint8_t actual)
{
	if (rpc_es("get", nombre))
	{
		rpc_responder(actual ? "true" : "false");
	}
	else if (rpc_es("set", nombre))
	{
		const char *p = rpc_params();
		actual = (*p == 't' || *p == '1') ? 1 : 0;   // true o 1
		rpc_responder(actual ? "true" : "false");
	}
	return actual;
}

int32_t MQTT_RPC_Int(const char *nombre, int32_t actual)
{
	char txt[16];
	if (rpc_es("get", nombre))
	{
		snprintf(txt, sizeof(txt), "%ld", (long)actual);
		rpc_responder(txt);
	}
	else if (rpc_es("set", nombre))
	{
		actual = atol(rpc_params());
		snprintf(txt, sizeof(txt), "%ld", (long)actual);
		rpc_responder(txt);
	}
	return actual;
}

float MQTT_RPC_Float(const char *nombre, float actual)
{
	char txt[24];
	if (rpc_es("get", nombre))
	{
		snprintf(txt, sizeof(txt), "%.2f", actual);
		rpc_responder(txt);
	}
	else if (rpc_es("set", nombre))
	{
		actual = atof(rpc_params());
		snprintf(txt, sizeof(txt), "%.2f", actual);
		rpc_responder(txt);
	}
	return actual;
}
