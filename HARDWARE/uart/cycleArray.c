#include "cycleArray.h"

void cycleArray_init(CycleArray *fifo)
{
    fifo->head = 0;
	fifo->tail = 0;
}

u8 cycleArray_isEmpty(CycleArray *fifo)
{
	if(fifo->head == fifo->tail)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void cycleArray_push(CycleArray *fifo, u8 data)
{
	fifo->data[fifo->tail] = data;
	fifo->tail++;
	fifo->tail = fifo->tail % UART_REC_LEN;
	if(fifo->tail == fifo->head) 
	{
		fifo->head++;
		fifo->head = fifo->head % UART_REC_LEN;
	}
}

u8 cycleArray_pop(CycleArray *fifo)
{
	u8 data = 0;
	if(cycleArray_isEmpty(fifo) == 0)
	{
		data = fifo->data[fifo->head];
		fifo->head++;
		fifo->head = fifo->head % UART_REC_LEN;
	}
	return data;
}

//uint16_t ringbuffer_getRemainSize(ringbuffer_t *fifo)
//{
//    return (fifo->size - ringbuffer_getUsedSize(fifo) - 1);
//}

//uint16_t ringbuffer_getUsedSize(ringbuffer_t *fifo)
//{
//    if (fifo->in >= fifo->out)
//        return (fifo->in - fifo->out);
//    else
//        return (fifo->size - fifo->out + fifo->in);
//}

void fCycleArray_init(FCycleArray *ca)
{
    ca->head = 0;
	ca->tail = 0;
}

u8 fCycleArray_isEmpty(FCycleArray *ca)
{
	if(ca->head == ca->tail)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void fCycleArray_push(FCycleArray *ca, float data)
{
	ca->data[ca->tail] = data;
	ca->tail++;
	ca->tail = ca->tail % FCYCLEARRAY_SIZE;
	if(ca->tail == ca->head) 
	{
		ca->head++;
		ca->head = ca->head % FCYCLEARRAY_SIZE;
	}
}

float fCycleArray_pop(FCycleArray *ca)
{
	float data = 0;
	if(fCycleArray_isEmpty(ca) == 0)
	{
		data = ca->data[ca->head];
		ca->head++;
		ca->head = ca->head % FCYCLEARRAY_SIZE;
	}
	return data;
}

int fCycleArray_len(FCycleArray *ca)
{
	int res = ca->tail - ca->head;
	while(res < 0) res += FCYCLEARRAY_SIZE;
	return res;
}
