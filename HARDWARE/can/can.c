#include "can.h"
#include "led.h"
#include "uart.h"

//CAN初始化
//tsjw:重新同步跳跃时间单元.范围:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:时间段2的时间单元.   范围:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:时间段1的时间单元.   范围:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :波特率分频器.范围:1~1024;  tq=(brp)*tpclk1
//波特率=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,普通模式;CAN_Mode_LoopBack,回环模式;
//Fpclk1的时钟在初始化的时候设置为36M,如果设置CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);
//则波特率为:36M/((8+9+1)*4)=500Kbps
//返回值:0,初始化OK;
//    其他,初始化失败;

u8 can_motor_receive_databuff[3][8];
u8 can_motor_send_databuff[8];

void can_init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
   	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PORTA时钟	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); //使能CAN1时钟	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化IO
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化IO
	  
 	//CAN单元设置
	CAN_InitStructure.CAN_TTCM = DISABLE; //就是这样
	CAN_InitStructure.CAN_ABOM = ENABLE; //软件自动离线管理
  	CAN_InitStructure.CAN_AWUM = ENABLE; //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
  	CAN_InitStructure.CAN_NART = DISABLE; //自动重传功能
  	CAN_InitStructure.CAN_RFLM = DISABLE; //报文不锁定,新的覆盖旧的
  	CAN_InitStructure.CAN_TXFP = ENABLE;	 //按存入fifo的顺序发送
  	CAN_InitStructure.CAN_Mode = mode; //模式设置： mode:0,普通模式 1,回环模式
	
  	//设置波特率？？？
  	CAN_InitStructure.CAN_SJW = tsjw; //重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位
  	CAN_InitStructure.CAN_BS1 = tbs1; //Tbs1=tbs1+1个时间单位CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2 = tbs2; //Tbs2=tbs2+1个时间单位CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler = brp; //分频系数(Fdiv)为brp+1
  	CAN_Init(CAN1, &CAN_InitStructure); // 初始化CAN1 

	//必须设置过滤器，不然无法接收数据
	CAN_FilterInitStructure.CAN_FilterNumber = 0;	  //过滤器0
   	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
  	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0201;
  	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;//32位MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE; //激活过滤器0
  	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
	
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);//FIFO0有消息中断允许.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
}   

//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
u8 Can_Send_Msg(u8* msg,u8 len)
{	
	u8 mbox;
	u8 i = 0;
	CanTxMsg TxMessage;

	TxMessage.StdId = 0x200; // 标准标识符 
	TxMessage.ExtId = 0x12; // 设置扩展标示符 
	TxMessage.IDE = CAN_Id_Standard; // 标准帧
	TxMessage.RTR = CAN_RTR_Data; // 数据帧
	TxMessage.DLC = len; // 要发送的数据长度
	for(i = 0; i < len; i++)
	{
		TxMessage.Data[i] = msg[i];
	}
	
	mbox = CAN_Transmit(CAN1, &TxMessage);   
	
	i=0;
	while((CAN_TransmitStatus(CAN1, mbox) == CAN_TxStatus_Failed) && (i < 0Xff)) i++;	//等待发送结束
	if(i == 0Xff)return 1;
	
	return 0;		
}

u8 Can_Receive_Msg(u8 *buf)
{		   		   
	u8 i;
	CanRxMsg RxMessage;

	if(CAN_MessagePending(CAN1, CAN_FIFO0) == 0)
	{
		return 0;
	}		//没有接收到数据,直接退出

	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据	
	
	for(i = 0; i < 8; i++)
	{
		buf[i] = RxMessage.Data[i];
	}
	
	return RxMessage.DLC; //数据长度	
}
