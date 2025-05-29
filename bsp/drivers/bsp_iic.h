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

#ifdef __STM32F4xx_GPIO_H
#define SCL_GPIO            GPIO_Pin_8
#define SDA_GPIO            GPIO_Pin_9
#endif




//IIC所有操作函数
void Soft_IIC_Init(void);                //初始化IIC的IO口				 
void Soft_IIC_Start(void);				//发送IIC开始信号
void Soft_IIC_Stop(void);	  			//发送IIC停止信号
void Soft_IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t Soft_IIC_Receive_Byte(unsigned char ack);//IIC读取一个字节
uint8_t Soft_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void Soft_IIC_Ack(void);					//IIC发送ACK信号
void Soft_IIC_NAck(void);				//IIC不发送ACK信号

uint8_t Soft_IIC_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);//IIC连续写
uint8_t Soft_IIC_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf); //IIC连续读 
uint8_t Soft_IIC_Write_Byte(uint8_t addr,uint8_t reg,uint8_t data);				//IIC写一个字节
uint8_t Soft_IIC_Read_Byte(uint8_t addr,uint8_t reg);						//IIC读一个字节

#endif
#endif


#endif