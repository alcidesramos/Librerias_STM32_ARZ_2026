/*
 * RC522.C
 *
 *  Created on: Apr 19, 2024
 *      Author: alcid
 */


#include "RC522.h"
#include "delays.h"
extern SPI_HandleTypeDef hspi1;

uint8_t cont;   // para for

 //algoritmo has

uint8_t UID[10];  // almacena el buffer del codigo de rfid


// compara dos vector
uint8_t  verifica(const uint8_t  *vector1,uint8_t  *vector2,uint8_t size)
{
uint8_t  cu;

 for (cu=0;cu<size;cu++)
  {
    if (vector1[cu]!=vector2[cu])
    {
      return(0);
    }

  }

  return(1);

  }

//necesario para algoritmo hash
#define FNV_PRIME 16777619
#define FNV_BASIS  2166136261


uint32_t   Hash_algoritmo ()
{
	uint32_t   hash_acum = FNV_BASIS;

      for (cont=0;cont<=4;cont++)   // se guardan cinco datos del uid
          {
                 // Add value into the hash
                hash_acum = (hash_acum * FNV_PRIME) ^ UID[cont];
        }

        return (hash_acum);
}



static void MFRC522_Wr( uint8_t addr, uint8_t value )
{
     uint8_t dato;
     dato=(( addr << 1 ) & 0x7E);
	 HAL_GPIO_WritePin(RC522_CS_GPIO_Port, RC522_CS_Pin, 0);
     HAL_SPI_Transmit(&hspi1, &dato, 1, 100);
     HAL_SPI_Transmit(&hspi1, &value, 1, 100);

     HAL_GPIO_WritePin(RC522_CS_GPIO_Port, RC522_CS_Pin, 1);
}

 static uint8_t MFRC522_Rd( uint8_t addr )
{
uint8_t value;
uint8_t dato;
dato=((( addr << 1 ) & 0x7E) | 0x80);
   HAL_GPIO_WritePin(RC522_CS_GPIO_Port, RC522_CS_Pin, 0);
   HAL_SPI_Transmit(&hspi1, &dato, 1, 100);
   dato=0;
   HAL_SPI_TransmitReceive(&hspi1, &dato, &value, 1,100);

   HAL_GPIO_WritePin(RC522_CS_GPIO_Port, RC522_CS_Pin, 1);
   return(value);
}


static void MFRC522_Clear_Bit( uint8_t addr, uint8_t mask )
{
     MFRC522_Wr( addr, MFRC522_Rd( addr ) & (~mask) );
}



static void MFRC522_Set_Bit( uint8_t addr, uint8_t mask )
{
     MFRC522_Wr( addr, MFRC522_Rd( addr ) | mask );
}



void MFRC522_Reset()
{

MFRC522_Wr( COMMANDREG, PCD_RESETPHASE );
}
void MFRC522_AntennaOn()
{
 MFRC522_Set_Bit( TXCONTROLREG, 0x03 );
}
void MFRC522_AntennaOff()
{
 MFRC522_Clear_Bit( TXCONTROLREG, 0x03 );
}



void MFRC522_Init()
{
   delay_init();
     HAL_GPIO_WritePin(RC522_CS_GPIO_Port, RC522_CS_Pin, 1);

     //ACCION DE RESET
     HAL_GPIO_WritePin(RC522_RST_GPIO_Port, RC522_RST_Pin, 0);
     Delay_us(2);   //TIEMPO PARA COMPLETAR RESET
	 HAL_GPIO_WritePin(RC522_RST_GPIO_Port, RC522_RST_Pin, 1);//LISTO PARA INICIAR
     HAL_Delay(50);

     MFRC522_Reset();

     MFRC522_Wr( TMODEREG, 0x8D );      //Tauto=1; f(Timer) = 6.78MHz/TPreScaler
     MFRC522_Wr( TPRESCALERREG, 0x3E ); //TModeReg[3..0] + TPrescalerReg
     MFRC522_Wr( TRELOADREGL, 30 );
     MFRC522_Wr( TRELOADREGH, 0 );

     MFRC522_Wr( TXAUTOREG, 0x40 );    //100%ASK
     MFRC522_Wr( MODEREG, 0x3D );      // CRC valor inicial de 0x6363

     //MFRC522_Clear_Bit( STATUS2REG, 0x08 );//MFCrypto1On=0
     //MFRC522_Wr( RXSELREG, 0x86 );      //RxWait = RxSelReg[5..0]
     //MFRC522_Wr( RFCFGREG, 0x7F );     //RxGain = 48dB
     MFRC522_AntennaOn();

}
uint8_t MFRC522_ToCard( uint8_t command, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, unsigned *backLen )
{
  uint8_t _status = MI_ERR;
  uint8_t irqEn = 0x00;
  uint8_t waitIRq = 0x00;
  uint8_t lastBits;
  uint8_t n;
  unsigned i;

  switch (command)
  {
    case PCD_AUTHENT:       //Certification cards close
    {
      irqEn = 0x12;
      waitIRq = 0x10;
      break;
    }
    case PCD_TRANSCEIVE:    //Transmit FIFO data
    {
      irqEn = 0x77;
      waitIRq = 0x30;
      break;
    }
    default:
      break;
  }
  MFRC522_Wr( COMMIENREG, irqEn | 0x80 );  //Interrupt request
  MFRC522_Clear_Bit( COMMIRQREG, 0x80 );   //Clear all interrupt request bit
  MFRC522_Set_Bit( FIFOLEVELREG, 0x80 );   //FlushBuffer=1, FIFO Initialization
  MFRC522_Wr( COMMANDREG, PCD_IDLE );      //NO action; Cancel the current command???



  //Writing data to the FIFO
  for ( i=0; i < sendLen; i++ )
  {
    MFRC522_Wr( FIFODATAREG, sendData[i] );
  }
  //Execute the command
  MFRC522_Wr( COMMANDREG, command );
  if (command == PCD_TRANSCEIVE )
  {
    MFRC522_Set_Bit( BITFRAMINGREG, 0x80 ); //StartSend=1,transmission of data starts
  }
  //Waiting to receive data to complete
  //i according to the clock frequency adjustment, the operator M1 card maximum waiting time 25ms???
  i = 0xFFFF;
  do
  {
    //CommIrqReg[7..0]
    //Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
    n = MFRC522_Rd( COMMIRQREG );
    i--;
  }
  while ( i && !(n & 0x01) && !( n & waitIRq ) );
  MFRC522_Clear_Bit( BITFRAMINGREG, 0x80 );    //StartSend=0
  if (i != 0)
  {
    if( !( MFRC522_Rd( ERRORREG ) & 0x1B ) ) //BufferOvfl Collerr CRCErr ProtecolErr
    {
      _status = MI_OK;
      if ( n & irqEn & 0x01 )
      {
        _status = MI_NOTAGERR;       //??
      }
      if ( command == PCD_TRANSCEIVE )
      {
        n = MFRC522_Rd( FIFOLEVELREG );
        lastBits = MFRC522_Rd( CONTROLREG ) & 0x07;
        if (lastBits)
        {
          *backLen = (n-1) * 8 + lastBits;
        }
        else
        {
          *backLen = n * 8;
        }
        if (n == 0)
        {
          n = 1;
        }
        if (n > 16)
        {
          n = 16;
        }
        //Reading the received data in FIFO
        for (i=0; i < n; i++)
        {
          backData[i] = MFRC522_Rd( FIFODATAREG );
        }

  backData[i] = 0;
      }
    }
    else
    {
      _status = MI_ERR;
    }
  }
  //MFRC522_Set_Bit( CONTROLREG, 0x80 );
  //MFRC522_Wr( COMMANDREG, PCD_IDLE );
  return _status;
}
uint8_t MFRC522_Request( uint8_t reqMode, uint8_t *TagType )
{
  uint8_t _status;
  unsigned backBits;            //The received data bits
  MFRC522_Wr( BITFRAMINGREG, 0x07 ); //TxLastBists = BitFramingReg[2..0]   ???
  TagType[0] = reqMode;
  _status = MFRC522_ToCard( PCD_TRANSCEIVE, TagType, 1, TagType, &backBits );
  if ( (_status != MI_OK) || (backBits != 0x10) )
  {
    _status = MI_ERR;
  }
  return _status;
}
void MFRC522_CRC( uint8_t *dataIn, uint8_t length, uint8_t *dataOut )
{
uint8_t i, n;
    MFRC522_Clear_Bit( DIVIRQREG, 0x04 );
    MFRC522_Set_Bit( FIFOLEVELREG, 0x80 );

 //Escreve dados no FIFO
    for ( i = 0; i < length; i++ )
    {
        MFRC522_Wr( FIFODATAREG, *dataIn++ );
    }

    MFRC522_Wr( COMMANDREG, PCD_CALCCRC );

    i = 0xFF;
    //Espera a finaliza��o do Calculo do CRC
    do
    {
        n = MFRC522_Rd( DIVIRQREG );
        i--;
    }
    while( i && !(n & 0x04) );        //CRCIrq = 1

    dataOut[0] = MFRC522_Rd( CRCRESULTREGL );
    dataOut[1] = MFRC522_Rd( CRCRESULTREGM );
}
uint8_t MFRC522_SelectTag( uint8_t *serNum )
{
  uint8_t i;
  uint8_t _status;
  uint8_t size;
  unsigned recvBits;
  uint8_t buffer[9];

  //MFRC522_Clear_Bit( STATUS2REG, 0x08 );   //MFCrypto1On=0

  buffer[0] = PICC_SElECTTAG;
  buffer[1] = 0x70;

  for ( i=2; i < 7; i++ )
  {
    buffer[i] = *serNum++;
  }

  MFRC522_CRC( buffer, 7, &buffer[7] );

  _status = MFRC522_ToCard( PCD_TRANSCEIVE, buffer, 9, buffer, &recvBits );
  if ( (_status == MI_OK) && (recvBits == 0x18) )
  {
    size = buffer[0];
  }
  else
  {
    size = 0;
  }
  return size;
}
//hibernation
void MFRC522_Halt()
{
  unsigned unLen;
  uint8_t buff[4];

  buff[0] = PICC_HALT;
  buff[1] = 0;
  MFRC522_CRC( buff, 2, &buff[2] );
  MFRC522_Clear_Bit( STATUS2REG, 0x80 );
  MFRC522_ToCard( PCD_TRANSCEIVE, buff, 4, buff, &unLen );
  MFRC522_Clear_Bit( STATUS2REG, 0x08 );
}
uint8_t MFRC522_Auth( uint8_t authMode, uint8_t BlockAddr, uint8_t *Sectorkey, uint8_t *serNum )
{
  uint8_t _status;
  unsigned recvBits;
  uint8_t i;
  uint8_t buff[12];

  //Verify the command block address + sector + password + card serial number
  buff[0] = authMode;
  buff[1] = BlockAddr;

  for ( i = 2; i < 8; i++ )
  {
    buff[i] = Sectorkey[i-2];
  }

  for ( i = 8; i < 12; i++ )
  {
    buff[i] = serNum[i-8];
  }

  _status = MFRC522_ToCard( PCD_AUTHENT, buff, 12, buff, &recvBits );

  if ( ( _status != MI_OK ) || !( MFRC522_Rd( STATUS2REG ) & 0x08 ) )
  {
    _status = MI_ERR;
  }

  return _status;
}
uint8_t MFRC522_Write( uint8_t blockAddr, uint8_t *writeData )
{
  uint8_t _status;
  unsigned recvBits;
  uint8_t i;
  uint8_t buff[18];
  buff[0] = PICC_WRITE;
  buff[1] = blockAddr;

  MFRC522_CRC( buff, 2, &buff[2] );
  _status = MFRC522_ToCard( PCD_TRANSCEIVE, buff, 4, buff, &recvBits );
  if ( (_status != MI_OK) || (recvBits != 4) || ( (buff[0] & 0x0F) != 0x0A) )
  {
    _status = MI_ERR;
  }
  if (_status == MI_OK)
  {
    for ( i = 0; i < 16; i++ )                //Data to the FIFO write 16Byte
    {
      buff[i] = writeData[i];
    }

    MFRC522_CRC( buff, 16, &buff[16] );
    _status = MFRC522_ToCard( PCD_TRANSCEIVE, buff, 18, buff, &recvBits );
    if ( (_status != MI_OK) || (recvBits != 4) || ( (buff[0] & 0x0F) != 0x0A ) )
    {
      _status = MI_ERR;
    }
  }
  return _status;
}
uint8_t MFRC522_Read( uint8_t blockAddr, uint8_t *recvData )
{
  uint8_t _status;
  unsigned unLen;
  recvData[0] = PICC_READ;
  recvData[1] = blockAddr;

  MFRC522_CRC( recvData, 2, &recvData[2] );

  _status = MFRC522_ToCard( PCD_TRANSCEIVE, recvData, 4, recvData, &unLen );
  if ( (_status != MI_OK) || (unLen != 0x90) )
  {
    _status = MI_ERR;
  }
  return _status;
}
uint8_t MFRC522_AntiColl( uint8_t *serNum )
{
  uint8_t _status;
  uint8_t i;
  uint8_t serNumCheck = 0;
  unsigned unLen;
  MFRC522_Wr( BITFRAMINGREG, 0x00 );                //TxLastBists = BitFramingReg[2..0]
  serNum[0] = PICC_ANTICOLL;
  serNum[1] = 0x20;
  MFRC522_Clear_Bit( STATUS2REG, 0x08 );
  _status = MFRC522_ToCard( PCD_TRANSCEIVE, serNum, 2, serNum, &unLen );
  if (_status == MI_OK)
  {
    for ( i=0; i < 4; i++ )
    {
      serNumCheck ^= serNum[i];
    }

    if ( serNumCheck != serNum[4] )
    {
      _status = MI_ERR;
    }
  }
  return _status;
}
//0x0044 = Mifare_UltraLight
//0x0004 = Mifare_One (S50)
//0x0002 = Mifare_One (S70)
//0x0008 = Mifare_Pro (X)
//0x0344 = Mifare_DESFire
uint8_t MFRC522_isCard( uint8_t *TagType )
{
    if (MFRC522_Request( PICC_REQIDL, TagType ) == MI_OK)
        return 1;
    else
        return 0;
}
uint8_t MFRC522_ReadCardSerial( uint8_t *str )
{
uint8_t _status;
 _status = MFRC522_AntiColl( str );
 str[5] = 0;
 if (_status == MI_OK)
  return 1;
 else
  return 0;
}

