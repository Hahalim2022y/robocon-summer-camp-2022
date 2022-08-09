#ifndef CYCLEARRAY_H
#define CYCLEARRAY_H

#include <stm32f10x.h>

#define UART_REC_LEN 256

typedef struct CycleArray_
{
	u8 data[UART_REC_LEN];
	u8 head, tail;
} CycleArray;

void cycleArray_init(CycleArray *fifo);
u8 cycleArray_isEmpty(CycleArray *fifo);
void cycleArray_push(CycleArray *fifo, u8 data);
u8 cycleArray_pop(CycleArray *fifo);

#endif
