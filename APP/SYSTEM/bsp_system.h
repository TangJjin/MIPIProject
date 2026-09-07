#ifndef BSP_SYSTEM__
#define BSP_SYSTEM__

#include "main.h"
#include "usart.h"
#include "dsihost.h"
#include "dma2d.h"
#include "i2c.h"
#include "ltdc.h"
#include "lvgl.h"
#include "FreeRTOS.h"
#include "task.h"

#include <stdint.h>
#include "string.h"
#include "stdio.h"
#include "stdarg.h"
#include "stdbool.h"

#include "bsp_led.h"
#include "bsp_uart.h"
#include "ringbuffer.h"
#include "bsp_sdram.h"
#include "bsp_mipi_lcd.h"
#include "bsp_touch_i2c.h"
#include "bsp_touch.h"
#include "gui_guider.h"
#include "custom.h"
#include "bsp_rtos.h"

typedef struct {
    uint8_t cmd;                /* 命令 */
    const uint8_t *data;        /* 参数数据 */
    uint8_t data_bytes;         /* 参数字节数 */
} mipi_lcd_init_cmd_t;

extern uint8_t uart_rx_dma_buffer[128];

extern const mipi_lcd_init_cmd_t ili9881c10_init_cmd_tbl[];
extern const uint16_t ili9881c10_init_cmd_count;

#endif

