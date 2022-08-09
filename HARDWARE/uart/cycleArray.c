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
