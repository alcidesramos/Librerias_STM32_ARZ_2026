
#include "HAL2_PERIFERICOS.h"
#include "mx_hal_def.h"
#include "mx_i2c1.h"
#include "mx_spi1.h"
#include "stm32c5xx_hal_spi.h"


//define los uart usados
#ifdef HAL2
#ifdef HUART1
hal_uart_handle_t *uart1;
#endif
#ifdef HUART2
hal_uart_handle_t *uart2;
#endif
#ifdef HUART3
hal_uart_handle_t *uart3;
#endif
#ifdef HUART4
hal_uart_handle_t *uart4;
#endif
#ifdef HUART5
hal_uart_handle_t *uart5;
#endif
#ifdef HUART6
hal_uart_handle_t *uart6;
#endif
#ifdef HUART7
hal_uart_handle_t *uart7;
#endif
#ifdef HUART8
hal_uart_handle_t *uart8;
#endif
#ifdef HUART9
hal_uart_handle_t *uart9;
#endif
#ifdef HUART10
hal_uart_handle_t *uart10;
#endif

#ifdef HI2C1
hal_i2c_handle_t *i2c1;
#endif
#ifdef HI2C2
hal_i2c_handle_t *i2c2;
#endif
#ifdef HI2C3
hal_i2c_handle_t *i2c3;
#endif
#ifdef HI2C4
hal_i2c_handle_t *i2c4;
#endif

#ifdef HSPI1
hal_spi_handle_t *spi1;
#endif

#ifdef HSPI2
hal_uart_handle_t *spi2;
#endif

#ifdef HSPI3
hal_uart_handle_t *spi3;
#endif

#ifdef HSPI4
hal_uart_handle_t *spi4;
#endif

#endif





#ifdef HAL2
//inicia los uartt deseado
#ifdef HUART1
void uart1_init()
{	 //uart1 = mx_usart1_uart_gethandle();
     uart1 = mx_usart1_uart_init();
}
#endif
#ifdef HUART2
void uart2_init()
{	 //uart2 = mx_usart2_uart_gethandle();
     uart2 = mx_usart2_uart_init();
}
#endif

#ifdef HUART3
void uart3_init()
{	 //uart3 = mx_usart3_uart_gethandle();
     uart3 = mx_usart3_uart_init();
}
#endif

#ifdef HUART4
void uart4_init()
{	 //uart4 = mx_usart4_uart_gethandle();
     uart4 = mx_usart4_uart_init();
}
#endif
#ifdef HUART5
void uart5_init()
{	 //uart5 = mx_usart5_uart_gethandle();
     uart5 = mx_usart5_uart_init();
}
#endif
#ifdef HUART6
void uart6_init()
{	 //uart6 = mx_usart6_uart_gethandle();
     uart6 = mx_usart6_uart_init();
}
#endif
#ifdef HUART7
void uart7_init()
{	 //uart7 = mx_usart7_uart_gethandle();
     uart7 = mx_usart7_uart_init();
}
#endif
#ifdef HUART8
void uart8_init()
{	 //uart8 = mx_usart8_uart_gethandle();
     uart8 = mx_usart8_uart_init();
}
#endif
#ifdef HUART9
void uart9_init()
{	 //uart9 = mx_usart9_uart_gethandle();
     uart9 = mx_usart9_uart_init();
}
#endif
#ifdef HUART10
void uart10_init()
{	 //uart10 = mx_usart10_uart_gethandle();
     uart10 = mx_usart10_uart_init();
}
#endif


#ifdef HI2C1
void i2c1_init()
{	 //uart10 = mx_usart10_uart_gethandle();
     i2c1 = mx_i2c1_i2c_init();
     
}
#endif

#ifdef HI2C2
void i2c2_init()
{i2c2 = mx_i2c2_i2c_init();
     
}
#endif

#ifdef HI2C3
void i2c3_init()
{    i2c3 = mx_i2c3_i2c_init();
     
}
#endif

#ifdef HI2C4
void i2c4_init()
{i2c4 = mx_i2c4_i2c_init();
     
}
#endif

#ifdef HSPI1
void spi1_init()
{	
     spi1 = mx_spi1_init();
         
}
#endif

#ifdef HSPI2
void spi2_init()
{	
     spi2 = mx_spi2_init();
    
}
#endif
#ifdef HSPI3
void spi3_init()
{	
     spi3 = mx_spi3_init();
    
}
#endif
#ifdef HSPI4
void spi4_init()
{	
     spi4 =mx_spi1_init();
    
}
#endif


#endif
