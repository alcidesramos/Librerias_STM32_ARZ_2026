#include "stdint.h"

void usb_uart_write(char *trama_usb_tx)
{
	CDC_Transmit_FS(trama_usb_tx, strlen(trama_usb_tx));
}

void usb_uart_writen(char *trama_usb_tx,uint16_t num)
{
	CDC_Transmit_FS(trama_usb_tx, num);
}
