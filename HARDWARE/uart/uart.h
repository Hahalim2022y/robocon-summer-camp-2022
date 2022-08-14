#ifndef UART_H
#define UART_H

#include <stm32f10x.h>
#include "cycleArray.h"

extern CycleArray uart1_fifo;
extern CycleArray uart2_fifo;
extern CycleArray uart4_fifo;

void usart1Init(u32 baud);
void usart2Init(u32 baud);
void uart4Init(u32 baud);

void uart1_send(char str[]);
void uart2_send(char str[]);
void uart4_send(char str[]);

u8 uart2_read(u8 *buff);
u8 uart4_read(u8 *buff);

#endif
