#ifndef CAN_H
#define CAN_H

#include <stm32f10x.h>

//CAN接收RX0中断使能
#define CAN_RX0_INT_ENABLE	0		//0,不使能;1,使能.								    

extern u8 can_motor_receive_databuff[3][8];
extern u8 can_motor_send_databuff[8];

void can_init(u8 tsjw, u8 tbs2, u8 tbs1, u16 brp, u8 mode);//CAN初始化
 
u8 Can_Send_Msg(u8* msg,u8 len);						//发送数据

u8 Can_Receive_Msg(u8 *buf);							//接收数据
#endif
