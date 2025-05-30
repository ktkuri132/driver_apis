#ifndef BSP_IIC_H
#define BSP_IIC_H

#include <stdint.h>

#ifndef On_Chip_IIC
#define On_Chip_IIC

#ifndef __SOFTI2C_
#define __SOFTI2C_
#include "bsp_iic.h"
#include <sysport.h>
#define SOFT_IIC_PORT       GPIOB
#define SOFT_IIC_SCL_PORT   I2C_SCL_PIN
#define SOFT_IIC_SDA_PORT   I2C_SDA_PIN
#endif


typedef struct Soft_IIC_APIs_Struct
{
    void (*Soft_IIC_GPIO_Port_Init)(void); //IIC GPIO端口初始化函数
    void (*delay_us)(uint32_t nus); //延时函数
    void (*Soft_IIC_SCL)(uint8_t state); //SCL线控制函数
    void (*Soft_IIC_SDA)(uint8_t state); //SDA线控制函数
    void (*Soft_SDA_IN)(void); //SDA线设置为输入
    void (*Soft_SDA_OUT)(void); //SDA线设置为输出
    uint8_t (*Soft_READ_SDA)(void); //读取SDA线状态
} SIAS;

//IIC所有操作函数
void Soft_IIC_Init(SIAS *i2c_dev);                //初始化IIC的IO口				 
void Soft_IIC_Start(SIAS *i2c_dev);				//发送IIC开始信号
void Soft_IIC_Stop(SIAS *i2c_dev);	  			//发送IIC停止信号
void Soft_IIC_Send_Byte(SIAS *i2c_dev,uint8_t txd);			//IIC发送一个字节
uint8_t Soft_IIC_Receive_Byte(SIAS *i2c_dev,unsigned char ack);//IIC读取一个字节
uint8_t Soft_IIC_Wait_Ack(SIAS *i2c_dev); 				//IIC等待ACK信号
void Soft_IIC_Ack(SIAS *i2c_dev);					//IIC发送ACK信号
void Soft_IIC_NAck(SIAS *i2c_dev);				//IIC不发送ACK信号

uint8_t Soft_IIC_Write_Len(SIAS *i2c_dev,uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);//IIC连续写
uint8_t Soft_IIC_Read_Len(SIAS *i2c_dev,uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf); //IIC连续读 
uint8_t Soft_IIC_Write_Byte(SIAS *i2c_dev,uint8_t addr,uint8_t reg,uint8_t data);				//IIC写一个字节
uint8_t Soft_IIC_Read_Byte(SIAS *i2c_dev,uint8_t addr,uint8_t reg);						//IIC读一个字节

#endif
#endif


#endif