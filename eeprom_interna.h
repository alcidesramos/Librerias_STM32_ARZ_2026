HAL_StatusTypeDef writeEEPROMByte(uint32_t address, uint8_t data);
HAL_StatusTypeDef writeEEPROM2Byte(uint32_t address, uint16_t data);
HAL_StatusTypeDef writeEEPROM4Byte(uint32_t address, uint32_t data);
HAL_StatusTypeDef writeEEPROMFloat(uint32_t address, float data);
uint8_t readEEPROMByte(uint32_t address) 
uint16_t readEEPROM2Byte(uint32_t address);
uint32_t readEEPROM4Byte(uint32_t address); 
float readEEPROMFloat(uint32_t address); 
