// Adaptada por Alcides Ramos

//main  is e suno el sensor esta conectado
//INA219_Init()

//en el while
//Voltaje en milivoltios dividir/1000
//corriente en mA
/*
  	uint16_t voltaje=INA219_ReadBusVoltage();
	uint16_t corriente=INA219_ReadCurrent();
*/

#include "main.h"
#include "INA219.h"


uint16_t ina219_calibrationValue;
int16_t ina219_currentDivider_mA;
int16_t ina219_powerMultiplier_mW;


uint16_t Read16( uint8_t Register)
{
	uint8_t Value[2];

	HAL_I2C_Mem_Read(&INA219_I2C, INA219_ADDRESS, Register, 1, Value, 2, 1000);

	return ((Value[0] << 8) | Value[1]);
}


void Write16(uint8_t Register, uint16_t Value)
{
	uint8_t addr[2];
	addr[0] = (Value >> 8) & 0xff;  // upper byte
	addr[1] = (Value >> 0) & 0xff; // lower byte
	HAL_I2C_Mem_Write(&INA219_I2C, INA219_ADDRESS, Register, 1, (uint8_t*)addr, 2, 1000);
}


uint16_t INA219_ReadBusVoltage()
{
	uint16_t result = Read16(INA219_REG_BUSVOLTAGE);

	return ((result >> 3  ) * 4);

}

int16_t INA219_ReadCurrent_raw()
{
	int16_t result = Read16( INA219_REG_CURRENT);

	return (result );
}

float INA219_ReadCurrent()
{
	int16_t result = INA219_ReadCurrent_raw();

	return (result /(float) ina219_currentDivider_mA );
}

uint16_t INA219_ReadShuntVolage()
{
	uint16_t result = Read16(INA219_REG_SHUNTVOLTAGE);

	return (result * 0.01 );
}

void INA219_Reset()
{
	Write16(INA219_REG_CONFIG, INA219_CONFIG_RESET);
	HAL_Delay(1);
}

void INA219_setCalibration(uint16_t CalibrationData)
{
	Write16(INA219_REG_CALIBRATION, CalibrationData);
}

uint16_t INA219_getConfig()
{
	uint16_t result = Read16(INA219_REG_CONFIG);
	return result;
}

void INA219_setConfig( uint16_t Config)
{
	Write16(INA219_REG_CONFIG, Config);
}

void INA219_setCalibration_32V_2A()
{
	uint16_t config = INA219_CONFIG_BVOLTAGERANGE_32V |
	             INA219_CONFIG_GAIN_8_320MV | INA219_CONFIG_BADCRES_12BIT |
	             INA219_CONFIG_SADCRES_12BIT_1S_532US |
	             INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;

	ina219_calibrationValue = 4096;
	ina219_currentDivider_mA = 10; // Current LSB = 100uA per bit (1000/100 = 10)
	ina219_powerMultiplier_mW = 2; // Power LSB = 1mW per bit (2/1)

	INA219_setCalibration(ina219_calibrationValue);
	INA219_setConfig(config);
}

void INA219_setCalibration_32V_1A()
{
	uint16_t config = INA219_CONFIG_BVOLTAGERANGE_32V |
	                    INA219_CONFIG_GAIN_8_320MV | INA219_CONFIG_BADCRES_12BIT |
	                    INA219_CONFIG_SADCRES_12BIT_1S_532US |
	                    INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;

	ina219_calibrationValue = 10240;
	ina219_currentDivider_mA = 25;    // Current LSB = 40uA per bit (1000/40 = 25)
	ina219_powerMultiplier_mW = 0.8f; // Power LSB = 800uW per bit

	INA219_setCalibration(ina219_calibrationValue);
	INA219_setConfig(config);
}

void INA219_setCalibration_16V_400mA()
{
	uint16_t config = INA219_CONFIG_BVOLTAGERANGE_16V |
	                    INA219_CONFIG_GAIN_1_40MV | INA219_CONFIG_BADCRES_12BIT |
	                    INA219_CONFIG_SADCRES_12BIT_1S_532US |
	                    INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;

	ina219_calibrationValue = 8192;
	ina219_currentDivider_mA = 20;    // Current LSB = 50uA per bit (1000/50 = 20)
	ina219_powerMultiplier_mW = 1.0f; // Power LSB = 1mW per bit

	INA219_setCalibration(ina219_calibrationValue);
	INA219_setConfig(config);
}

void INA219_setPowerMode(uint8_t Mode)
{
	uint16_t config = INA219_getConfig();

	switch (Mode) {
		case INA219_CONFIG_MODE_POWERDOWN:
			config = (config & ~INA219_CONFIG_MODE_MASK) | (INA219_CONFIG_MODE_POWERDOWN & INA219_CONFIG_MODE_MASK);
			INA219_setConfig(config);
			break;

		case INA219_CONFIG_MODE_SANDBVOLT_TRIGGERED:
			config = (config & ~INA219_CONFIG_MODE_MASK) | (INA219_CONFIG_MODE_SANDBVOLT_TRIGGERED & INA219_CONFIG_MODE_MASK);
			INA219_setConfig(config);
			break;

		case INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS:
			config = (config & ~INA219_CONFIG_MODE_MASK) | (INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS & INA219_CONFIG_MODE_MASK);
			INA219_setConfig(config);
			break;

		case INA219_CONFIG_MODE_ADCOFF:
			config = (config & ~INA219_CONFIG_MODE_MASK) | (INA219_CONFIG_MODE_ADCOFF & INA219_CONFIG_MODE_MASK);
			INA219_setConfig(config);
			break;
	}
}

uint8_t INA219_Init( )
{
	ina219_currentDivider_mA = 0;
	ina219_powerMultiplier_mW = 0;

	uint8_t ina219_isReady = HAL_I2C_IsDeviceReady(&INA219_I2C, INA219_ADDRESS , 3, 2);

	if(ina219_isReady == HAL_OK)
	{

		INA219_Reset();
		INA219_setCalibration_32V_2A();

		return 1;
	}

	else
	{
		return 0;
	}
}

