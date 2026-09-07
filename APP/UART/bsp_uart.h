#ifndef __BSP_UART__
#define __BSP_UART__

#include "bsp_system.h"

void uart_init(void);
void uart_proc(void);

extern volatile uint32_t uart1_dropped_bytes;

#endif
