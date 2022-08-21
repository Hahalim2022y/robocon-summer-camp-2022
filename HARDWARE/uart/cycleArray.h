#ifndef CYCLEARRAY_H
#define CYCLEARRAY_H

#include <stm32f10x.h>

#define UART_REC_LEN 256
#define FCYCLEARRAY_SIZE 401

typedef struct CycleArray_
{
	u8 data[UART_REC_LEN];
	u8 head, tail;
} CycleArray;

typedef struct FCycleArray_
{
	float data[FCYCLEARRAY_SIZE];
	u8 head, tail;
} FCycleArray;

void cycleArray_init(CycleArray *fifo);
u8 cycleArray_isEmpty(CycleArray *fifo);
void cycleArray_push(CycleArray *fifo, u8 data);
u8 cycleArray_pop(CycleArray *fifo);

void fCycleArray_init(FCycleArray *ca);
u8 fCycleArray_isEmpty(FCycleArray *ca);
void fCycleArray_push(FCycleArray *ca, float data);
float fCycleArray_pop(FCycleArray *ca);
int fCycleArray_len(FCycleArray *ca);

#endif
