//  Alcides Ramos  2024

//variables

//LTR390 UV
//uint32_t uv, luz;

//en el main
//1 si esta el sensor conectado
//LTR390_Init()

// en el while
/*
uv =LTR390_UV();
HAL_Delay(100);// RETARDO POR LA RESOLUCON DE 18BIT
luz = LTR390_LUZ();
*/
#include "LTR390.h"
//PARA CALCULAR LAS FORMULAS
float gainuv,timuv;

//ESCRIBE UN BYTE I2C
void I2C_Write(uint8_t dir,uint8_t val)
{
	HAL_I2C_Mem_Write(&LTR390_I2C, LTR390_ADDR, dir, I2C_MEMADD_SIZE_8BIT, &val, 1, 500);
}
//LEE UN BYTE I2C
uint8_t I2C_Read(uint8_t dir)
{
	uint8_t Buf;
	HAL_I2C_Mem_Read(&LTR390_I2C, LTR390_ADDR, dir, I2C_MEMADD_SIZE_8BIT, &Buf, 1, 500);
   return(Buf);

}

uint8_t LTR390_ID()
  {
    uint8_t reg = I2C_Read(LTR390_PART_ID);
    if (reg==0xb2) return (1); else return (0);

  }
//configura la ganancia
//GAIN_1   GAIN_3    GAIN_6  GAIN_18
void LTR390_Gain(uint8_t gain1 )
{

 I2C_Write(LTR390_GAIN, gain1);
  gainuv = gain1;

}
//ejemplo
//RESOLUTION_13BIT_TIME12_5MS ,RATE_25MS
void LTR390_Rate(uint8_t resol, uint8_t rata )
{
	uint16_t dat = (resol) | rata;
	I2C_Write(LTR390_MEAS_RATE, dat);
	    timuv = 2.0; //6 o 7 es 2ms
	    if (rata == 0) timuv = 0.025;
	    if (rata == 1) timuv = 0.05;
	    if (rata == 2) timuv = 0.1;
	    if (rata == 3) timuv = 0.2;
	    if (rata == 4) timuv = 0.5;
	    if (rata== 5) timuv = 1.0;
}

//Modo Luz
void LTR390_ALSMode()
{
//	I2C_Write(LTR390_INT_CFG, 0x14);
	I2C_Write(LTR390_MAIN_CTRL,0x02);
}

//modo UV
void LTR390_UVMode()
{

	I2C_Write(LTR390_MAIN_CTRL, 0x0A);
}

void  LTR390_Reset()
{
	I2C_Write(LTR390_MAIN_CTRL, 0x10);
}



uint8_t LTR390_Init()
  {
    uint8_t reg = I2C_Read(LTR390_PART_ID);
    if (reg!=0xb2) return (0);
  //inicia a la ganacia deseada
    LTR390_Reset();
    HAL_Delay(10);
    I2C_Write(LTR390_INT_CFG, 0x34);

    LTR390_Gain( GAIN_3 );
    //coloqie la  resolucion y rata
    //RESOLUTION_13BIT_TIME12_5MS ,RATE_25MS
    LTR390_Rate(RESOLUTION_18BIT_TIME100MS, RATE_25MS);

     return (1);

  }





uint32_t LTR390_UV(void)
{
	//LTR390_Reset();
	LTR390_UVMode();//modo UV
	uint32_t Data1 = I2C_Read(LTR390_UVSDATA);
    uint32_t Data2 = I2C_Read(LTR390_UVSDATA + 1);
    uint32_t Data3 = I2C_Read(LTR390_UVSDATA + 2);
    uint32_t UV;
    UV =  (Data3<<16)| (Data2<<8) | Data1;
    HAL_Delay(1);
    return UV;
}

uint32_t LTR390_LUZ(void)
{
	//LTR390_Reset();
	LTR390_ALSMode();//modo luz
    uint32_t Data1 = I2C_Read(LTR390_ALSDATA);
    uint32_t Data2 = I2C_Read(LTR390_ALSDATA + 1);
    uint32_t Data3 = I2C_Read(LTR390_ALSDATA + 2);
    uint32_t LUZ;
    LUZ =  (Data3<<16)| (Data2<<8) | Data1;
    HAL_Delay(1);
    return LUZ;
}
