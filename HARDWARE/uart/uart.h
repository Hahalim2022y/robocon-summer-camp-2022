#ifndef UART_H
#define UART_H

#include <stm32f10x.h>

void usart1Init(u32 baud);
void usart2Init(u32 baud);
void uart1_send(char str[]);
void uart2_send(char str[]);
u8 uart2_read(u8 *buff);

#endif
