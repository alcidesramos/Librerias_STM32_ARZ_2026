// en el archivo stm32f4xx_it.h

/*
// en la variables
extern uint16_t Timer1, Timer2;

// en el procedimiento SysTick_Handler(void)

if(Timer1 > 0)
			Timer1--;

		if(Timer2 > 0)
			Timer2--;



/*
 en el archivo user_diskio.c
 incluir #include "fatfs_sd.h"
 hacer los cambios en

   return SD_disk_initialize(pdrv);
   return SD_disk_status(pdrv);
   return (SD_disk_read(pdrv, buff, sector, count));
   return SD_disk_write(pdrv, buff, sector, count);
   return SD_disk_ioctl(pdrv, cmd, buff);

*/




/// en el main  se usa
/*

 SD_INIT();
 SD_CREA_FILE(file_name);

 */

//  en el while se usa ejemplo

/*

    //escribe en el archivo abierto
     SD_WRITE_TEXT(file_name,texto);


 */


#include "main.h"
#include "ff.h"
#include "stdio.h"


//definir lo necesario
#define SPI_TIMEOUT 100
extern SPI_HandleTypeDef 	hspi1;
#define HSPI_SDCARD		 	&hspi1
//sino se configura en el .ioc hacerlo aqui
//#define SD_CS_Pin GPIO_PIN_4
//#define SD_CS_GPIO_Port GPIOA

extern char file_name[];
extern char perdidos[];
extern FATFS fs;
extern FATFS *pfs;
extern FIL fil;
extern FRESULT fres;  // resultados de operaciones copn la memoria
extern uint32_t fre_clust;//
extern uint32_t totalSpace, freeSpace;
//#define SD_SIZE_BUFF  200 //PARA AHORRA MEMORIO NO FORMATEO
#define SD_SIZE_BUFF  _MAX_SS//SOLO SI SE QUIER FORMATEAR
extern char SD_BUFFER[];

#ifndef __FATFS_SD_H
#define __FATFS_SD_H

/* Definitions for MMC/SDC command */
#define CMD0     (0x40+0)     	/* GO_IDLE_STATE */
#define CMD1     (0x40+1)     	/* SEND_OP_COND */
#define CMD8     (0x40+8)     	/* SEND_IF_COND */
#define CMD9     (0x40+9)     	/* SEND_CSD */
#define CMD10    (0x40+10)    	/* SEND_CID */
#define CMD12    (0x40+12)    	/* STOP_TRANSMISSION */
#define CMD16    (0x40+16)    	/* SET_BLOCKLEN */
#define CMD17    (0x40+17)    	/* READ_SINGLE_BLOCK */
#define CMD18    (0x40+18)    	/* READ_MULTIPLE_BLOCK */
#define CMD23    (0x40+23)    	/* SET_BLOCK_COUNT */
#define CMD24    (0x40+24)    	/* WRITE_BLOCK */
#define CMD25    (0x40+25)    	/* WRITE_MULTIPLE_BLOCK */
#define CMD41    (0x40+41)    	/* SEND_OP_COND (ACMD) */
#define CMD55    (0x40+55)    	/* APP_CMD */
#define CMD58    (0x40+58)    	/* READ_OCR */

/* MMC card type flags (MMC_GET_TYPE) */
#define CT_MMC		0x01		/* MMC ver 3 */
#define CT_SD1		0x02		/* SD ver 1 */
#define CT_SD2		0x04		/* SD ver 2 */
#define CT_SDC		0x06		/* SD */
#define CT_BLOCK	0x08		/* Block addressing */

/* Functions */
DSTATUS SD_disk_initialize (BYTE pdrv);
DSTATUS SD_disk_status (BYTE pdrv);
DRESULT SD_disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count);
DRESULT SD_disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count);
DRESULT SD_disk_ioctl (BYTE pdrv, BYTE cmd, void* buff);

char SD_INIT();
char SD_CLOSE_FILE();
char SD_EXIT();
char SD_CREA_FILE(const TCHAR* nombre);
void SD_WRITE_TEXT(const TCHAR* nombre,const TCHAR* sd_datos);
uint8_t SD_READ_LINE(const TCHAR* nombre);
void SD_READ_LINE_N(TCHAR* sdbuff,uint32_t linen);
uint8_t SD_File_Exist(const char *filename);
FRESULT Sd_OpentoRead(const char *filename);





#endif
