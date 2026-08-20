// RETOCADA POR ALCIDES RAMOS

#include "main.h"



#define LTR390_ADDR       0x53<<1
#define LTR390_I2C_ADDR 0x53<<1

//EL I2C A USAR
extern I2C_HandleTypeDef hi2c2;
#define LTR390_I2C hi2c2

#define LTR390_MAIN_CTRL (0x00)// Main control register
#define LTR390_MEAS_RATE (0x04)// Resolution and data rate
#define LTR390_GAIN (0x05)// ALS and UVS gain range
#define LTR390_PART_ID (0x06)// Part id/revision register
#define LTR390_MAIN_STATUS (0x07)// Main status register
#define LTR390_ALSDATA (0x0D)// ALS data lowest byte, 3 byte
#define LTR390_UVSDATA (0x10)// UVS data lowest byte, 3 byte
#define LTR390_INT_CFG (0x19)// Interrupt configuration
#define LTR390_INT_PST (0x1A)// Interrupt persistance config
#define LTR390_THRESH_UP (0x21)// Upper threshold, low byte, 3 byte
#define LTR390_THRESH_LOW (0x24)// Lower threshold, low byte, 3 byte


//ALS/UVS measurement resolution, Gain setting, measurement rate
#define RESOLUTION_20BIT_TIME400MS (0X00)
#define RESOLUTION_19BIT_TIME200MS (0X10)
#define RESOLUTION_18BIT_TIME100MS (0X20)//default
#define RESOLUTION_17BIT_TIME50MS  (0x3)
#define RESOLUTION_16BIT_TIME25MS  (0x40)
#define RESOLUTION_13BIT_TIME12_5MS  (0x50)
#define RATE_25MS (0x0)
#define RATE_50MS (0x1)
#define RATE_100MS (0x2)//default
#define RATE_200MS (0x3)
#define RATE_500MS (0x4)
#define RATE_1000MS (0x5)
#define RATE_2000MS (0x6)

//measurement Gain Range.
#define GAIN_1  (0x0)
#define GAIN_3  (0x1)// default
#define GAIN_6 (0x2)
#define GAIN_9 (0x3)
#define GAIN_18 (0x4)



uint8_t LTR390_Init(void);
uint32_t LTR390_UV(void);
uint32_t LTR390_LUZ(void);


