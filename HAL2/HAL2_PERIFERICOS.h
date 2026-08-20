/*
HECA POR ALCIDES RAMOS  25/07/2026

*/


#include "main.h"
#include "mx_gpio_default.h"
//#include "stm32c5xx_hal_uart.h"


//DEFINE PERIFERICOS USADOS
#define  HUART1
#define  HUART2
#define  HI2C1
#define  HSPI1

#define HAL2

#ifdef  HAL2
//uart
#if defined (HUART1) || defined(HUART2) || defined(HUART3)|| defined(HUART3)|| defined(HUART5) || defined(HUART6)|| defined(HUART7)|| defined(HUART8)|| defined(HUART9)|| defined(HUART10)
#define UART_HandleTypeDef hal_uart_handle_t
#endif
//i2c
#if defined(HI2C1) || defined(HI2C2) || defined(HI2C3)|| defined(HI2C4)
#define I2C_HandleTypeDef hal_i2c_handle_t
#define HAL_StatusTypeDef hal_status_t
#define HAL_I2C_Master_Transmit HAL_I2C_MASTER_Transmit
#define HAL_I2C_Master_Receive HAL_I2C_MASTER_Receive
#endif
//delay
#define HAL_RCC_GetSysClockFreq() HAL_RCC_GetSYSCLKFreq()
//spi
#if defined(HSPI1) || defined(HSPI2) || defined(HSPI3)|| defined(HSPI4)
#define SPI_HandleTypeDef hal_spi_handle_t
#endif
//PUERTOS
#define GPIO_Port PORT 
#endif



#ifdef  HAL2
#ifdef  HUART1
extern hal_uart_handle_t *uart1;
#define huart1  (*uart1)   
#endif
#ifdef  HUART2
extern hal_uart_handle_t *uart2;
#define huart2  (*uart2)   
#endif
#ifdef  HUART3
extern hal_uart_handle_t *uart3;
#define huart3  (*uart3)   
#endif
#ifdef  HUART4
extern hal_uart_handle_t *uart4;
#define huart4  (*uart4)   
#endif
#ifdef  HUART5
extern hal_uart_handle_t *uart5;
#define huart5  (*uart5)   
#endif
#ifdef HUART6
extern hal_uart_handle_t *uart6;
#define huart6  (*uart6)   
#endif
#ifdef  HUART7
extern hal_uart_handle_t *uart7;
#define huart7  (*uart7)   
#endif
#ifdef  HUART8
extern hal_uart_handle_t *uart8;
#define huart8  (*uart8)   
#endif
#ifdef  HUART9
extern hal_uart_handle_t *uart9;
#define huart9  (*uart9)   
#endif
#ifdef  HUART10
extern hal_uart_handle_t *uart10;
#define huart10  (*uart10)   
#endif

#ifdef  HI2C1
extern hal_i2c_handle_t *i2c1;
#define hi2c1  (*i2c1)
#endif

#ifdef  HI2C2
extern hal_i2c_handle_t *i2c2;
#define hi2c2  (*i2c2)
#endif

#ifdef  HI2C3
extern hal_i2c_handle_t *i2c3;
#define hi2c3  (*i2c3)
#endif

#ifdef  HI2C4
extern hal_i2c_handle_t *i2c4;
#define hi2c4  (*i2c4)
#endif

#ifdef  HSPI1
extern hal_spi_handle_t *spi1;
#define hspi1  (*spi1)
#endif

#ifdef  HSPI2
extern hal_spi_handle_t *spi2;
#define hspi2  (*spi2)
#endif

#ifdef  HSPI3
extern hal_spi_handle_t *spi3;
#define hspi3  (*spi3)
#endif

#ifdef  HSPI4
extern hal_spi_handle_t *spi4;
#define hspi4  (*spi4)
#endif

#endif


#ifdef HAL2
//inicia los uartt deseado
#ifdef HUART1
void uart1_init();
#endif
#ifdef HUART2
void uart2_init();
#endif
#ifdef HUART3
void uart3_init();
#endif
#ifdef HUART4
void uart4_init();
#endif
#ifdef HUART5
void uart5_init();
#endif
#ifdef HUART6
void uart6_init();
#endif
#ifdef HUART7
void uart7_init();
#endif
#ifdef HUART8
void uart8_init();
#endif
#ifdef HUART9
void uart9_init();
#endif
#ifdef HUART10
void uart10_init();
#endif


#ifdef HI2C1
void i2c1_init();
#endif
#ifdef HI2C2
void i2c2_init();
#endif
#ifdef HI2C3
void i2c3_init();
#endif
#ifdef HI2C4
void i2c4_init();
#endif


#ifdef HSPI1
void spi1_init();
#endif

#ifdef HSPI2
void spi2_init();
#endif
#ifdef HSPI3
void spi3_init();
#endif
#ifdef HSPI4
void spi4_init();
#endif

#endif