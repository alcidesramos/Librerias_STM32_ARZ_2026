
#define EEPROM_BASE_ADDRESS 0x08080000UL


HAL_StatusTypeDef writeEEPROMByte(uint32_t address, uint8_t data)
{
	HAL_StatusTypeDef  estado;
	address = address + EEPROM_BASE_ADDRESS;
  HAL_FLASHEx_DATAEEPROM_Unlock();  //desprotege la memoria
  estado = HAL_FLASHEx_DATAEEPROM_Program(TYPEPROGRAMDATA_BYTE, address, data);
  HAL_FLASHEx_DATAEEPROM_Lock();  // protege la memorioa
  return(estado);  //hal_ok  todo bien
}

HAL_StatusTypeDef writeEEPROM2Byte(uint32_t address, uint16_t data)
{
  HAL_StatusTypeDef  estado;
 address = address + EEPROM_BASE_ADDRESS;
  HAL_FLASHEx_DATAEEPROM_Unlock();  //desprotege la meoria
  estado = HAL_FLASHEx_DATAEEPROM_Program(TYPEPROGRAMDATA_HALFWORD, address, data);
  HAL_FLASHEx_DATAEEPROM_Lock();  // protege la memoria
  return(estado);  //hal_ok  todo bien
}

HAL_StatusTypeDef writeEEPROM4Byte(uint32_t address, uint32_t data)
{
	HAL_StatusTypeDef  estado;
	address = address + EEPROM_BASE_ADDRESS;
  HAL_FLASHEx_DATAEEPROM_Unlock();  //desprotege la meoria
  estado = HAL_FLASHEx_DATAEEPROM_Program(TYPEPROGRAMDATA_WORD, address, data);
  HAL_FLASHEx_DATAEEPROM_Lock();  // protege la memoria
  return(estado);  //hal_ok  todo bien
}


HAL_StatusTypeDef writeEEPROMFloat(uint32_t address, float data)
{
	HAL_StatusTypeDef  estado;
	address = address + EEPROM_BASE_ADDRESS;
  HAL_FLASHEx_DATAEEPROM_Unlock();  //desprotege la meoria
  estado = HAL_FLASHEx_DATAEEPROM_Program(TYPEPROGRAMDATA_WORD, address, data);
  HAL_FLASHEx_DATAEEPROM_Lock();  // protege la memoria
  return(estado);  //hal_ok  todo bien
    }



uint8_t readEEPROMByte(uint32_t address) {
  uint8_t data = 0;
  address = address + EEPROM_BASE_ADDRESS;
  data = *(volatile uint32_t*)address;
  return data;
  }


uint16_t readEEPROM2Byte(uint32_t address) {
  uint16_t data = 0;
  address = address + EEPROM_BASE_ADDRESS;
  data = *(volatile uint32_t*)address;
  return data;
  }


uint32_t readEEPROM4Byte(uint32_t address) {
  uint16_t data = 0;
  address = address + EEPROM_BASE_ADDRESS;
  data = *(volatile uint32_t*)address;
  return data;
  }

float readEEPROMFloat(uint32_t address) {
  float data = 0;
  address = address + EEPROM_BASE_ADDRESS;
  data = *(volatile float*)address;
  return data;
  }
