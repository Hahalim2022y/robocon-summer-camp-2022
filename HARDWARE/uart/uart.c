#include "uart.h"

CycleArray uart1_fifo;
CycleArray uart2_fifo;
CycleArray uart4_fifo;

void usart1Init(u32 baud)
{
	//使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	//初始化PA9
	GPIO_InitTypeDef PA9;
	PA9.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	PA9.GPIO_Pin = GPIO_Pin_9;
	PA9.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &PA9);
	
	//初始化PA10
	GPIO_InitTypeDef PA10;
	PA9.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	PA9.GPIO_Pin = GPIO_Pin_10;
	PA9.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &PA10);
	
	//配置中断优先级
	NVIC_InitTypeDef NVIC_USART1;
	NVIC_USART1.NVIC_IRQChannel = USART1_IRQn;
	NVIC_USART1.NVIC_IRQChannelCmd = ENABLE;
	NVIC_USART1.NVIC_IRQChannelPreemptionPriority = 2; //抢占优先级
	NVIC_USART1.NVIC_IRQChannelSubPriority = 0; //响应优先级
	NVIC_Init(&NVIC_USART1);
	
	//串口初始化设置
	USART_InitTypeDef u1Init;
	u1Init.USART_BaudRate = baud;
	u1Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	u1Init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	u1Init.USART_Parity = USART_Parity_No; //奇偶校验设置
	u1Init.USART_StopBits = USART_StopBits_1;
	u1Init.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &u1Init);
	
	//开启串口接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	//使能串口
	USART_Cmd(USART1, ENABLE);
	
	cycleArray_init(&uart1_fifo);
}

void usart2Init(u32 baud)
{
	//使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//初始化PA2
	GPIO_InitTypeDef PA2;
	PA2.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	PA2.GPIO_Pin = GPIO_Pin_2;
	PA2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &PA2);
	
	//初始化PA3
	GPIO_InitTypeDef PA3;
	PA3.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
//PA3.GPIO_Mode = GPIO_Mode_IPD;
	PA3.GPIO_Pin = GPIO_Pin_3;
	PA3.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &PA3);
	
	//配置中断优先级
	NVIC_InitTypeDef NVIC_USART2;
	NVIC_USART2.NVIC_IRQChannel = USART2_IRQn;
	NVIC_USART2.NVIC_IRQChannelCmd = ENABLE;
	NVIC_USART2.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级
	NVIC_USART2.NVIC_IRQChannelSubPriority = 0; //响应优先级
	NVIC_Init(&NVIC_USART2);
	
	//串口初始化设置
	USART_InitTypeDef u2Init;
	u2Init.USART_BaudRate = baud;
	u2Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	u2Init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	u2Init.USART_Parity = USART_Parity_No; //奇偶校验设置
	u2Init.USART_StopBits = USART_StopBits_1;
	u2Init.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &u2Init);
	
	//开启串口接收中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	//使能串口
	USART_Cmd(USART2, ENABLE);
	
	cycleArray_init(&uart2_fifo);
}

void uart4Init(u32 baud)
{
	//使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	
	//初始化PA9
	GPIO_InitTypeDef PC10;
	PC10.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	PC10.GPIO_Pin = GPIO_Pin_10;
	PC10.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &PC10);
	
	//初始化PA10
	GPIO_InitTypeDef PC11;
	PC11.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	PC11.GPIO_Pin = GPIO_Pin_11;
	PC11.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &PC11);
	
	//配置中断优先级
	NVIC_InitTypeDef NVIC_UART4;
	NVIC_UART4.NVIC_IRQChannel = UART4_IRQn;
	NVIC_UART4.NVIC_IRQChannelCmd = ENABLE;
	NVIC_UART4.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级
	NVIC_UART4.NVIC_IRQChannelSubPriority = 0; //响应优先级
	NVIC_Init(&NVIC_UART4);
	
	//串口初始化设置
	USART_InitTypeDef u4Init;
	u4Init.USART_BaudRate = baud;
	u4Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	u4Init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	u4Init.USART_Parity = USART_Parity_No; //奇偶校验设置
	u4Init.USART_StopBits = USART_StopBits_1;
	u4Init.USART_WordLength = USART_WordLength_8b;
	USART_Init(UART4, &u4Init);
	
	//开启串口接收中断
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	
	//使能串口
	USART_Cmd(UART4, ENABLE);
	
	cycleArray_init(&uart4_fifo);
}

void uart1_send(char str[])
{
	int i;
	for(i = 0; str[i] != '\0'; i++)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
		USART_SendData(USART1, str[i]);
	}
}

void uart2_send(char str[])
{
	int i;
	for(i = 0; str[i] != '\0'; i++)
	{
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		USART_SendData(USART2, str[i]);
	}
}

void uart4_send(char str[])
{
	int i;
	for(i = 0; str[i] != '\0'; i++)
	{
		while(USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
		USART_SendData(UART4, str[i]);
	}
}

u8 uart2_read(u8 *buff)
{
	if(cycleArray_isEmpty(&uart2_fifo) == 0)
	{
		*buff = cycleArray_pop(&uart2_fifo);
		return 1;
	}
	else
	{
		return 0;
	}
}

u8 uart4_read(u8 *buff)
{
	if(cycleArray_isEmpty(&uart4_fifo) == 0)
	{
		*buff = cycleArray_pop(&uart4_fifo);
		return 1;
	}
	else
	{
		return 0;
	}
}

void USART1_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE)) //必须是RXNE才能继续
	{
		res = USART_ReceiveData(USART1);
		cycleArray_push(&uart1_fifo, res);
	}
}

void UART4_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(UART4, USART_IT_RXNE)) //必须是RXNE才能继续
	{
		res = USART_ReceiveData(UART4);
		cycleArray_push(&uart4_fifo, res);
	}
}
