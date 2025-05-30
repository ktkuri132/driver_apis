/*
    MSPM0 上的IIC通讯,分为硬件IIC和软件IIC两种方式。
    通过宏定义开关
*/
#include "msp_i2c.h"
#include <drivers/bsp_iic.h>
#include <sysport.h>


#define OLED_I2C_ADDR 0x78
#define MPU6050_I2C_ADDR 0x68

#ifdef __SOFT_I2C__

extern gpio_st i2c_scl;
extern SYS_Port *sys_port; // 系统接口指针



void delay_us(uint32_t nus)
{
	DL_Common_delayCycles(nus * 16); // 1us = 16 cycles
}

void Soft_IIC_SCL(uint8_t state)
{
	if(state)
    {
        // GPIO_WriteBit(SOFT_IIC_PORT, SOFT_IIC_SCL_PORT, Bit_SET);
        DL_GPIO_setPins(SOFT_IIC_PORT, SOFT_IIC_SCL_PORT);
    }
    else
    {
        // GPIO_WriteBit(SOFT_IIC_PORT, SOFT_IIC_SCL_PORT, Bit_RESET);
        DL_GPIO_clearPins(SOFT_IIC_PORT, SOFT_IIC_SCL_PORT);
    }
}

void Soft_IIC_SDA(uint8_t state)
{
	if(state)
    {
        // GPIO_WriteBit(SOFT_IIC_PORT, SOFT_IIC_SDA_PORT, Bit_SET);
        DL_GPIO_setPins(SOFT_IIC_PORT, SOFT_IIC_SDA_PORT);
    }
    else
    {
        // GPIO_WriteBit(SOFT_IIC_PORT, SOFT_IIC_SDA_PORT, Bit_RESET);
        DL_GPIO_clearPins(SOFT_IIC_PORT, SOFT_IIC_SDA_PORT);
    }
}

void Soft_SDA_IN(void)
{
	DL_GPIO_initDigitalInput(SOFT_IIC_SDA_PORT);
}

void Soft_SDA_OUT(void)
{
	DL_GPIO_initDigitalOutput(SOFT_IIC_SDA_PORT);
}

uint8_t Soft_READ_SDA(void)
{
	return DL_GPIO_readPins(SOFT_IIC_PORT, SOFT_IIC_SDA_PORT);
}

SIAS i2c_Dev = {
    .Soft_IIC_GPIO_Port_Init = NULL,
    .delay_us = delay_us,
    .Soft_IIC_SCL = Soft_IIC_SCL,
    .Soft_IIC_SDA = Soft_IIC_SDA,
    .Soft_SDA_IN = Soft_SDA_IN,
    .Soft_SDA_OUT = Soft_SDA_OUT,
    .Soft_READ_SDA = Soft_READ_SDA
};


#endif

#ifdef __HARD_I2C__

#include <ti/driverlib/dl_i2c.h>
#include <ti/comm_modules/i2c/controller/i2c_comm_controller.h>


void I2C_Init(void) {
    DL_I2C_enableController(I2C_MPU6050_INST);
    // DL_I2C_enableController(I2C_OLED_INST);
}

void IIC_SendByte(I2C_Regs *I2C_handle, uint16_t Addr,uint8_t cmd, uint8_t data) {
    DL_I2C_startControllerTransfer(I2C_handle, Addr, DL_I2C_CONTROLLER_DIRECTION_TX, 1);
    DL_I2C_transmitTargetDataCheck(I2C_handle, cmd);
    DL_I2C_transmitTargetDataCheck(I2C_handle, data);
}


void IIC_SendData(I2C_Regs *I2C_handle, uint16_t Addr, uint8_t *data, uint16_t len) {
    DL_I2C_startControllerTransfer(I2C_handle, Addr, DL_I2C_CONTROLLER_DIRECTION_TX, len);
    for (uint16_t i = 0; i < len; i++) {
        DL_I2C_transmitTargetDataCheck(I2C_handle, data[i]);
    }
}


#endif