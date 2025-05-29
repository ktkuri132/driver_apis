#ifndef __MSP_IIC_H
#define __MSP_IIC_H

#include <syscall.h>
#include <sysport.h>
#include <ti/driverlib/dl_i2c.h>

#define __SOFT_I2C__  // 定义为软件IIC
// #define __HARD_I2C__  // 定义为硬件IIC
#ifdef __SOFT_I2C__



#endif

#ifdef __HARD_I2C__



void I2C_Init(void);
void IIC_SendByte(I2C_Regs *I2C_handle, uint16_t Addr, uint8_t cmd, uint8_t data);
void IIC_SendData(I2C_Regs *I2C_handle, uint16_t Addr, uint8_t *data, uint16_t len);

#endif

#endif