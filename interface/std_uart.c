/* SPDX-License-Identifier: MPL-2.0 */
/* Copyright (c) 2026 stdrsrs */

#include "std_uart.h"
STD_UART_ID std_uart_id = STD_UART_LOG;
volatile uint32_t std_tim_high=0;
int _write(int fd, char *ptr, int len)
{
    (void) fd; //避免未使用参数警告
    STD_UART_Transmit(std_uart_id, (uint8_t *)ptr, len);
    return len;
}
