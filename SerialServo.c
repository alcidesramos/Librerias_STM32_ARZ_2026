/*
 * SerialServo.c
 */
#include "SerialServo.h"

static uint8_t tx_buf[INSTRUCTION_FRAME_BUFFER];
static uint8_t status_param[STATUS_PARAM_MAX];

/* ---------------------------------------------------------------- bajo nivel */

uint8_t getChecksum(const Instruction_Packet *ip)
{
	uint8_t sum = ip->Packet_ID + ip->Length + ip->Instruction;
	for (uint8_t i = 0; i < ip->Length - 2; i++)
		sum += ip->Param[i];
	return ~sum;
}

void int_to_hex(int decimalNumber, unsigned char *lowByte, unsigned char *highByte)
{
	uint16_t hex_value;

	if (decimalNumber < 0)
	{
		hex_value = (uint16_t) (-decimalNumber);
		hex_value |= 0x8000;            // bit 15 = direccion negativa
	}
	else
	{
		hex_value = (uint16_t) decimalNumber;
	}
	*lowByte = hex_value & 0xFF;
	*highByte = (hex_value >> 8) & 0xFF;
}

int32_t convertHexToInteger(unsigned char hexBytes[], int byteCount)
{
	int32_t result = 0;
	for (int i = 0; i < byteCount; i++)
		result |= ((int32_t) hexBytes[i] << (i * 8));
	return result;
}

/* Envia la instruccion y lee la respuesta (half-duplex: se conmuta TX/RX a mano). */
Status_Packet AxelFlow_fire(UART_HandleTypeDef *huart, Instruction_Packet ip)
{
	Status_Packet st;
	memset(&st, 0, sizeof(st));
	st.Param = status_param;

	// armar trama: FF FF ID LEN INST PARAMS... CHK
	tx_buf[0] = HEADER;
	tx_buf[1] = HEADER;
	tx_buf[2] = ip.Packet_ID;
	tx_buf[3] = ip.Length;
	tx_buf[4] = ip.Instruction;
	for (uint8_t i = 0; i < ip.Length - 2; i++)
		tx_buf[5 + i] = ip.Param[i];
	tx_buf[ip.Length + 3] = ip.Checksum;

	// limpiar basura pendiente en el receptor
	__HAL_UART_CLEAR_OREFLAG(huart);
	if (__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE))
		(void) huart->Instance->DR;

	HAL_HalfDuplex_EnableTransmitter(huart);
	HAL_StatusTypeDef err = HAL_UART_Transmit(huart, tx_buf, ip.Length + 4, 100);
	HAL_HalfDuplex_EnableReceiver(huart);
	if (err != HAL_OK || ip.Packet_ID == BROADCAST_ID)
		return st;                      // broadcast: el servo no responde

	// respuesta: FF FF ID LEN ERR PARAMS... CHK
	uint8_t hdr[4];
	if (HAL_UART_Receive(huart, hdr, 4, STATUS_PACKET_TIMEOUT) != HAL_OK)
		return st;
	if (hdr[0] != HEADER || hdr[1] != HEADER)
		return st;
	uint8_t len = hdr[3];
	if (len < 2 || len > STATUS_PARAM_MAX + 2)
		return st;

	uint8_t body[STATUS_PARAM_MAX + 3];    // ERR + params + CHK = len bytes
	if (HAL_UART_Receive(huart, body, len, STATUS_PACKET_TIMEOUT) != HAL_OK)
		return st;

	uint8_t sum = hdr[2] + hdr[3];
	for (uint8_t i = 0; i < len - 1; i++)
		sum += body[i];
	if ((uint8_t) ~sum != body[len - 1])
		return st;                      // checksum incorrecto

	st.Header_1 = hdr[0];
	st.Header_2 = hdr[1];
	st.Packet_ID = hdr[2];
	st.Length = len;
	st.Error = body[0];
	for (uint8_t i = 0; i < len - 2; i++)
		status_param[i] = body[1 + i];
	st.Checksum = body[len - 1];
	st.Valid = true;
	return st;
}

static Status_Packet send_cmd(int ID, uint8_t instruction, uint8_t *params, uint8_t nparams)
{
	Instruction_Packet p;
	p.Header_1 = HEADER;
	p.Header_2 = HEADER;
	p.Packet_ID = ID;
	p.Length = nparams + 2;
	p.Instruction = instruction;
	p.Param = params;
	p.Checksum = getChecksum(&p);
	return AxelFlow_fire(&servouart, p);
}

static void write_reg8(int ID, uint8_t addr, uint8_t value)
{
	uint8_t prm[2] = { addr, value };
	send_cmd(ID, COMMAND_WRITE_DATA, prm, 2);
}

static void write_reg16(int ID, uint8_t addr, int value)
{
	uint8_t lo, hi;
	int_to_hex(value, &lo, &hi);
	uint8_t prm[3] = { addr, lo, hi };
	send_cmd(ID, COMMAND_WRITE_DATA, prm, 3);
}

static int read_reg(int ID, uint8_t addr, uint8_t nbytes)
{
	uint8_t prm[2] = { addr, nbytes };
	Status_Packet st = send_cmd(ID, COMMAND_READ_DATA, prm, 2);
	if (!st.Valid || st.Length < nbytes + 2)
		return -1;
	return convertHexToInteger(st.Param, nbytes);
}

/* ------------------------------------------------------------------ escritura */

bool Ping(int ID)
{
	Status_Packet st = send_cmd(ID, COMMAND_PING, NULL, 0);
	return st.Valid && st.Error == 0;
}

int findID(void)
{
	for (int i = 0; i < BROADCAST_ID; i++)
		if (Ping(i))
			return i;
	return -1;
}

void Write_LockFlag(int ID, int flag)
{
	write_reg8(ID, RAM_LOCK_FLAG, flag);
}

void SetID(int Initial_ID, int New_ID)
{
	Write_LockFlag(Initial_ID, 0);          // desbloquear EEPROM
	write_reg8(Initial_ID, EEPROM_ID, New_ID);
	Write_LockFlag(New_ID, 1);              // bloquear con el ID nuevo
}

void Torque_enable(int ID, int enable)
{
	write_reg8(ID, RAM_TORQUE_ENABLE, enable);
}

void Operation_mode(int ID, int mode)
{
	Write_LockFlag(ID, 0);
	write_reg8(ID, OPERATION_MODE, mode);
	Write_LockFlag(ID, 1);
}

void Target_location(int ID, int pos)
{
	write_reg16(ID, TARGET_LOCATION, pos);
}

void Operation_time(int ID, int time)
{
	write_reg16(ID, OPERATION_TIME, time);
}

void Operation_speed(int ID, int speed)
{
	write_reg16(ID, OPERATION_SPEED, speed);
}

void Acceleration(int ID, int acc)
{
	write_reg8(ID, RAM_ACCELERATION, acc);
}

void Set_torque(int ID, int value)
{
	write_reg16(ID, RAM_TORQUE_LIMIT, value);
}

void Min_Max_Angle(int ID, int min, int max)
{
	Write_LockFlag(ID, 0);
	write_reg16(ID, EEPROM_ANGLE_LIMIT_MIN, min);
	write_reg16(ID, EEPROM_ANGLE_LIMIT_MAX, max);
	Write_LockFlag(ID, 1);
}

void POSITION_CORRECTION(int ID, int pos)
{
	Write_LockFlag(ID, 0);
	write_reg16(ID, EEPROM_POSITION_CORRECTION, pos);
	Write_LockFlag(ID, 1);
}

/* Posicion + tiempo + velocidad en una sola escritura (registros 0x2A..0x2F) */
void ID_loc_time_speed(int ID, int pos, int time, int speed)
{
	uint8_t lp, hp, lt, ht, ls, hs;
	int_to_hex(pos, &lp, &hp);
	int_to_hex(time, &lt, &ht);
	int_to_hex(speed, &ls, &hs);
	uint8_t prm[7] = { TARGET_LOCATION, lp, hp, lt, ht, ls, hs };
	send_cmd(ID, COMMAND_WRITE_DATA, prm, 7);
}

/* Mueve varios servos a la vez (pos, tiempo, velocidad por servo). Max 16 servos. */
void Sync_write(SyncWrite_Packet w)
{
	if (w.count == 0 || w.count > 16)
		return;

	uint8_t prm[2 + 16 * 7];
	uint8_t n = 0;
	prm[n++] = TARGET_LOCATION;
	prm[n++] = 0x06;                        // bytes por servo (sin contar el ID)
	for (uint8_t i = 0; i < w.count; i++)
	{
		uint8_t lp, hp, lt, ht, ls, hs;
		int_to_hex(w.pos[i], &lp, &hp);
		int_to_hex(w.time[i], &lt, &ht);
		int_to_hex(w.speed[i], &ls, &hs);
		prm[n++] = w.ID[i];
		prm[n++] = lp;
		prm[n++] = hp;
		prm[n++] = lt;
		prm[n++] = ht;
		prm[n++] = ls;
		prm[n++] = hs;
	}
	send_cmd(BROADCAST_ID, COMMAND_SYNC_WRITE, prm, n);
}

/* -------------------------------------------------------------------- lectura */

int GetMoveFlag(int ID)
{
	return read_reg(ID, RAM_MOVE_FLAG, 1);
}

int LockFlag(int ID)
{
	return read_reg(ID, RAM_LOCK_FLAG, 1);
}

int CurrentLocation(int ID)
{
	return read_reg(ID, RAM_CURRENT_LOCATION, 2);
}

/* ---------------------------------------------------------------------- debug */

HAL_StatusTypeDef AxelFlow_debug_println(char *st)
{
	char str[200];
	snprintf(str, sizeof(str), "%s\r\n", st);
	return HAL_UART_Transmit(&debuguart, (uint8_t*) str, strlen(str), HAL_MAX_DELAY);
}
