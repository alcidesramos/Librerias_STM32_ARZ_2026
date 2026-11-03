/*
 * SerialServo.h
 *
 *  Servos de bus serie tipo Feetech STS/SCS (cabecera FF FF, registro 0x2A = posicion objetivo)
 *  Bus half-duplex en USART1 (PA9) a 1 Mbps. USART2 se usa solo para depuracion.
 */

#ifndef INC_SERIALSERVO_H_
#define INC_SERIALSERVO_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "string.h"
#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

extern UART_HandleTypeDef huart1;   // bus del servo (half-duplex)
extern UART_HandleTypeDef huart2;   // consola de depuracion
#define servouart                       huart1
#define debuguart                       huart2

#define HEADER                          0xFF
#define BROADCAST_ID                    0xFE
#define STATUS_PACKET_TIMEOUT           50      // ms
#define INSTRUCTION_FRAME_BUFFER        128
#define STATUS_PARAM_MAX                32

// EEPROM
#define EEPROM_MODEL_NUMBER_L           0x03
#define EEPROM_MODEL_NUMBER_H           0x04
#define EEPROM_ID                       0x05
#define EEPROM_BAUD_RATE                0x06
#define EEPROM_RETURN_DELAY_TIME        0x07
#define EEPROM_RESPONSE_STATUS_LEVEL    0x08
#define EEPROM_ANGLE_LIMIT_MIN          0x09
#define EEPROM_ANGLE_LIMIT_MAX          0x0B
#define EEPROM_LIMIT_TEMPERATURE_MAX    0x0D
#define EEPROM_HIGH_LIMIT_VOLTAGE       0x0E
#define EEPROM_LOW_LIMIT_VOLTAGE        0x0F
#define EEPROM_MAX_TORQUE               0x10
#define EEPROM_POSITION_CORRECTION      0x1F
#define OPERATION_MODE                  0x21    // 0 posicion, 1 velocidad, 2 PWM, 3 paso

// RAM
#define RAM_TORQUE_ENABLE               0x28
#define RAM_ACCELERATION                0x29
#define TARGET_LOCATION                 0x2A
#define OPERATION_TIME                  0x2C
#define OPERATION_SPEED                 0x2E
#define RAM_TORQUE_LIMIT                0x30
#define RAM_LOCK_FLAG                   0x37
#define RAM_CURRENT_LOCATION            0x38
#define RAM_MOVE_FLAG                   0x42

// Instrucciones
#define COMMAND_PING                    0x01
#define COMMAND_READ_DATA               0x02
#define COMMAND_WRITE_DATA              0x03
#define COMMAND_REG_WRITE_DATA          0x04
#define COMMAND_ACTION                  0x05
#define COMMAND_RESET                   0x06
#define COMMAND_SYNC_READ               0x82
#define COMMAND_SYNC_WRITE              0x83

// Errores del status packet
#define VOLTAGE_ERROR                   0x01

typedef struct
{
	uint8_t Header_1;
	uint8_t Header_2;
	uint8_t Packet_ID;
	uint8_t Length;
	uint8_t Instruction;
	uint8_t *Param;
	uint8_t Checksum;
} Instruction_Packet;

typedef struct
{
	uint8_t Header_1;
	uint8_t Header_2;
	uint8_t Packet_ID;
	uint8_t Length;
	uint8_t Error;
	uint8_t *Param;      // apunta a un buffer interno (valido hasta la siguiente transaccion)
	uint8_t Checksum;
	bool    Valid;       // true si llego un paquete completo con checksum correcto
} Status_Packet;

typedef struct
{
	uint8_t count;       // numero de servos
	uint8_t *ID;
	int *pos;
	int *time;
	int *speed;
} SyncWrite_Packet;

// Bajo nivel
uint8_t getChecksum(const Instruction_Packet *ip);
Status_Packet AxelFlow_fire(UART_HandleTypeDef *huart, Instruction_Packet ip);
void int_to_hex(int decimalNumber, unsigned char *lowByte, unsigned char *highByte);
int32_t convertHexToInteger(unsigned char hexBytes[], int byteCount);
HAL_StatusTypeDef AxelFlow_debug_println(char *st);

// Escritura
bool Ping(int ID);
int  findID(void);                                   // -1 si no encuentra ninguno
void SetID(int Initial_ID, int New_ID);
void Write_LockFlag(int ID, int flag);
void Torque_enable(int ID, int enable);
void Operation_mode(int ID, int mode);
void Target_location(int ID, int pos);
void Operation_time(int ID, int time);
void Operation_speed(int ID, int speed);
void Acceleration(int ID, int acc);
void Set_torque(int ID, int value);
void Min_Max_Angle(int ID, int min, int max);
void POSITION_CORRECTION(int ID, int pos);
void ID_loc_time_speed(int ID, int pos, int time, int speed);
void Sync_write(SyncWrite_Packet write_packet);

// Lectura (-1 si falla la comunicacion)
int GetMoveFlag(int ID);
int LockFlag(int ID);
int CurrentLocation(int ID);

#ifdef __cplusplus
}
#endif

#endif /* INC_SERIALSERVO_H_ */
