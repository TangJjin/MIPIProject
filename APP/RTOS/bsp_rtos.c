#include "bsp_rtos.h"

void freertos_test_task(void *argument)
{
    (void)argument;

    for(;;)
    {
        green_led(2);                       // 翻转绿灯
        vTaskDelay(pdMS_TO_TICKS(500));     // 500ms
    }
}

void lvgl_task(void *argument)
{
    (void)argument;

    for (;;)
    {
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}
