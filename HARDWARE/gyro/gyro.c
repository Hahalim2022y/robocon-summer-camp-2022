#include "gyro.h"
#include "uart.h"
#include <stdio.h>

// 模块主动上传的数据(串口解析后)
attitude_t		attitude;		// 姿态角
quaternion_t	quaternion;
gyroAcc_t 		gyroAccData;
mag_t			magData;
baro_t			baroData;
ioStatus_t		iostatus;

/* 模块寄存器参数值 */
regValue_t  	imu901Param;

// 串口接收解析成功的数据包
GyroDataFrame_t gyroDataFrame;


/* 陀螺仪加速度量程表 */
const uint16_t gyroFsrTable[4] = {250, 500, 1000, 2000};
const uint8_t  accFsrTable[4] = {2, 4, 8, 16};

// 接收串口数据解包流程
static enum
{
    waitForStartByte1,
    waitForStartByte2,
    waitForMsgID,
    waitForDataLength,
    waitForData,
    waitForChksum1,
} rxState = waitForStartByte1;

//void gyro_read(void)
//{
//	u8 buff;
//	while(uart2_read(&buff))
//	{
//		if(gyro_unpack(buff))
//		{
//			atkpParsing(&gyroDataFrame);
//		}
//	}
//}

uint8_t gyro_unpack(uint8_t ch)
{
    static uint8_t cksum = 0, dataIndex = 0;

    switch (rxState)
    {
        case waitForStartByte1:
			
            if (ch == UP_BYTE1)
            {
                rxState = waitForStartByte2;
                gyroDataFrame.startByte1 = ch;
            }

            cksum = ch;
            break;

        case waitForStartByte2:
            if (ch == UP_BYTE2 || ch == UP_BYTE2_ACK)
            {
                rxState = waitForMsgID;
                gyroDataFrame.startByte2 = ch;
            }
            else
            {
                rxState = waitForStartByte1;
            }

            cksum += ch;
            break;

        case waitForMsgID:
            gyroDataFrame.msgID = ch;
            rxState = waitForDataLength;
            cksum += ch;
            break;

        case waitForDataLength:
            if (ch <= GYRO_DATA_FRAME_MAX_DATA_SIZE)
            {
                gyroDataFrame.dataLen = ch;
                dataIndex = 0;
                rxState = (ch > 0) ? waitForData : waitForChksum1;	/*ch=0,数据长度为0，校验1*/
                cksum += ch;
            }
            else
            {
                rxState = waitForStartByte1;
            }

            break;

        case waitForData:
            gyroDataFrame.data[dataIndex] = ch;
            dataIndex++;
            cksum += ch;

            if (dataIndex == gyroDataFrame.dataLen)
            {
				rxState = waitForChksum1;
            }

            break;

        case waitForChksum1:
            if (cksum == ch)	// 校准正确返回1
            {
				gyroDataFrame.checkSum = cksum;
				rxState = waitForStartByte1;
				return 1;
            }
            else	/*!< 校验错误 */
            {
                rxState = waitForStartByte1;
            }
           
            break;

        default:
            rxState = waitForStartByte1;
            break;
    }

    return 0;
}



/**
  * @brief  ATKP数据包解析
  * @param  packet: atkp数据包
  * @retval None
  */
void atkpParsing(GyroDataFrame_t *packet)
{
    /* 姿态角 */
    if (packet->msgID == UP_ATTITUDE)
    {
		//uart1_send("lalala");
        int16_t data = (int16_t) (packet->data[1] << 8) | packet->data[0];
        attitude.roll = (float) data / 32768 * 180;

        data = (int16_t) (packet->data[3] << 8) | packet->data[2];
        attitude.pitch = (float) data / 32768 * 180;

        data = (int16_t) (packet->data[5] << 8) | packet->data[4];
        attitude.yaw = (float) data / 32768 * 180;
    }

    /* 四元数 */
    else if (packet->msgID == UP_QUAT)
    {
        int16_t data = (int16_t) (packet->data[1] << 8) | packet->data[0];
        quaternion.q0 = (float) data / 32768;

        data = (int16_t) (packet->data[3] << 8) | packet->data[2];
        quaternion.q1 = (float) data / 32768;

        data = (int16_t) (packet->data[5] << 8) | packet->data[4];
        quaternion.q2 = (float) data / 32768;

        data = (int16_t) (packet->data[7] << 8) | packet->data[6];
        quaternion.q3 = (float) data / 32768;
    }

    /* 陀螺仪加速度数据 */
    else if (packet->msgID == UP_GYROACCDATA)
    {
        gyroAccData.acc[0] = (int16_t) (packet->data[1] << 8) | packet->data[0];
        gyroAccData.acc[1] = (int16_t) (packet->data[3] << 8) | packet->data[2];
        gyroAccData.acc[2] = (int16_t) (packet->data[5] << 8) | packet->data[4];

        gyroAccData.gyro[0] = (int16_t) (packet->data[7] << 8) | packet->data[6];
        gyroAccData.gyro[1] = (int16_t) (packet->data[9] << 8) | packet->data[8];
        gyroAccData.gyro[2] = (int16_t) (packet->data[11] << 8) | packet->data[10];

        gyroAccData.faccG[0] = (float)gyroAccData.acc[0] / 32768 * accFsrTable[imu901Param.accFsr]; 		/*!< 4代表4G，上位机设置好的量程 */
        gyroAccData.faccG[1] = (float)gyroAccData.acc[1] / 32768 * accFsrTable[imu901Param.accFsr];
        gyroAccData.faccG[2] = (float)gyroAccData.acc[2] / 32768 * accFsrTable[imu901Param.accFsr];

        gyroAccData.fgyroD[0] = (float)gyroAccData.gyro[0] / 32768 * gyroFsrTable[imu901Param.gyroFsr]; 	/*!< 2000代表2000°/S，上位机设置好的量程 */
        gyroAccData.fgyroD[1] = (float)gyroAccData.gyro[1] / 32768 * gyroFsrTable[imu901Param.gyroFsr];
        gyroAccData.fgyroD[2] = (float)gyroAccData.gyro[2] / 32768 * gyroFsrTable[imu901Param.gyroFsr];
    }

    /* 磁场数据 */
    else if (packet->msgID == UP_MAGDATA)
    {
        magData.mag[0] = (int16_t) (packet->data[1] << 8) | packet->data[0];
        magData.mag[1] = (int16_t) (packet->data[3] << 8) | packet->data[2];
        magData.mag[2] = (int16_t) (packet->data[5] << 8) | packet->data[4];

        int16_t data = (int16_t) (packet->data[7] << 8) | packet->data[6];
        magData.temp = (float) data / 100;
    }

    /* 气压计数据 */
    else if (packet->msgID == UP_BARODATA)
    {
        baroData.pressure = (int32_t) (packet->data[3] << 24) | (packet->data[2] << 16) |
                            (packet->data[1] << 8) | packet->data[0];

        baroData.altitude = (int32_t) (packet->data[7] << 24) | (packet->data[6] << 16) |
                            (packet->data[5] << 8) | packet->data[4];

        int16_t data = (int16_t) (packet->data[9] << 8) | packet->data[8];
        baroData.temp = (float) data / 100;
    }

    /* 端口状态数据 */
    else if (packet->msgID == UP_D03DATA)
    {
        iostatus.d03data[0] = (uint16_t) (packet->data[1] << 8) | packet->data[0];
        iostatus.d03data[1] = (uint16_t) (packet->data[3] << 8) | packet->data[2];
        iostatus.d03data[2] = (uint16_t) (packet->data[5] << 8) | packet->data[4];
        iostatus.d03data[3] = (uint16_t) (packet->data[7] << 8) | packet->data[6];
    }
}


///**
//  * @brief  写寄存器
//  * @param  reg: 寄存器列表地址
//  * @param  data: 数据
//  * @param  datalen: 数据的长度只能是 1或2
//  * @retval None
//  */
//void atkpWriteReg(enum regTable reg, uint16_t data, uint8_t datalen)
//{
//    uint8_t buf[7];

//    buf[0] = 0x55;
//    buf[1] = 0xAF;
//    buf[2] = reg;
//    buf[3] = datalen; 	/*!< datalen只能是1或者2 */
//    buf[4] = data;

//    if (datalen == 2)
//    {
//        buf[5] = data >> 8;
//        buf[6] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5];
//        imu901_uart_send(buf, 7);
//    }
//    else
//    {
//        buf[5] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4];
//        imu901_uart_send(buf, 6);
//    }
//}


///**
//  * @brief  发送读寄存器命令
//  * @param  reg: 寄存器列表地址
//  * @retval None
//  */
//static void atkpReadRegSend(enum regTable reg)
//{
//    uint8_t buf[7];

//    buf[0] = 0x55;
//    buf[1] = 0xAF;
//    buf[2] = reg | 0x80;
//    buf[3] = 1;
//    buf[4] = 0;
//    buf[5] = buf[0] + buf[1] + buf[2] + buf[3] + buf[4];
//    imu901_uart_send(buf, 6);
//}



///**
//  * @brief  读寄存器
//  * @param  reg: 寄存器地址
//  * @param  data: 读取到的数据
//  * @retval uint8_t: 0读取失败（超时） 1读取成功
//  */
//uint8_t atkpReadReg(enum regTable reg, int16_t *data)
//{
//    uint8_t ch;
//    uint16_t timeout = 0;

//	while (imu901_uart_receive(&ch, 1) != 0); /*  丢弃缓存数据  */
//    atkpReadRegSend(reg);

//    while (1)
//    {
//        if (imu901_uart_receive(&ch, 1)) 	/*!< 获取串口fifo一个字节 */
//        {
//            if (imu901_unpack(ch)) 			/*!< 有有效数据包 */
//            {
//                if (rxPacket.startByte2 == UP_BYTE2) /*!< 主动上传包 */
//                {
//                    atkpParsing(&rxPacket);
//                }
//                else if (rxPacket.startByte2 == UP_BYTE2_ACK) /*!< 读寄存器应答包 */
//                {
//                    if (rxPacket.dataLen == 1)
//                        *data = rxPacket.data[0];
//                    else if (rxPacket.dataLen == 2)
//                        *data = (rxPacket.data[1] << 8) | rxPacket.data[0];

//                    return 1;
//                }
//            }
//        }
//        else
//        {
//            delay_ms(1);
//            timeout++;

//            if (timeout > 200) /*!< 超时返回 */
//                return 0;
//        }
//    }
//}



/**
  * @brief  模块初始化
  * @param  None
  * @retval None
  */
//void gyro_init(void)
//{
//    int16_t data;

//    /**
//      *	 写入寄存器参数（测试）
//      *	 这里提供写入引用例子，用户可以在这写入一些默认参数，
//      *  如陀螺仪加速度量程、带宽、回传速率、PWM输出等。
//      */
//    atkpWriteReg(REG_GYROFSR, 3, 1);
//    atkpWriteReg(REG_ACCFSR, 1, 1);
//	atkpWriteReg(REG_SAVE, 0, 1); 	/* 发送保存参数至模块内部Flash，否则模块掉电不保存 */

//    /* 读出寄存器参数（测试） */
//    atkpReadReg(REG_GYROFSR, &data);
//    imu901Param.gyroFsr = data;

//    atkpReadReg(REG_ACCFSR, &data);
//    imu901Param.accFsr = data;

//    atkpReadReg(REG_GYROBW, &data);
//    imu901Param.gyroBW = data;

//    atkpReadReg(REG_ACCBW, &data);
//    imu901Param.accBW = data;
//}
