
#include "string.h"
#include "stdio.h"
#include "main.h"

//#define uartx huart1   //   colocar el puerto serial a usar

extern UART_HandleTypeDef uartx;


#ifdef uartx
int __io_putchar(int ch)
{
 //ITM_SendChar(ch);//  si se quiere usar por debbuger
HAL_UART_Transmit(&uartx, (uint8_t *)&ch, 1, 0xFFFF);// printf al puerto serial sefinico
return(ch);
}

#else  // sino se define el uartx entonces lo asocia al USB cDC
int _write(int file, char *ptr, int len)
		{
	while(CDC_Transmit_FS(ptr, len)==USBD_BUSY);// espera que el dato sea enviado
    return len;
}

#endif

