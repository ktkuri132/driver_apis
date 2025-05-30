/*
    MSPM0 上的IIC通讯,分为硬件IIC和软件IIC两种方式。
    通过宏定义开关
*/
#include <drivers/bsp_iic.h>
#include <sysport.h>
#include <inc/stm32f10x_gpio.h>

#define OLED_I2C_ADDR 0x78
#define MPU6050_I2C_ADDR 0x68



#ifdef __SOFTI2C_

extern gpio_st i2c_scl;
extern SYS_Port *sys_port; // 系统接口指针

void Soft_IIC_GPIO_Port_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 初始化SCL引脚
    GPIO_InitStructure.GPIO_Pin = SOFT_IIC_SCL_PORT;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; // 设置为输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 设置速度
    GPIO_Init(SOFT_IIC_PORT, &GPIO_InitStructure);

    // 初始化SDA引脚
    GPIO_InitStructure.GPIO_Pin = SOFT_IIC_SDA_PORT;
    GPIO_Init(SOFT_IIC_PORT, &GPIO_InitStructure);
}

void delay_us(uint32_t nus)
{
	// syscall.bsp_systick_delay_us(nus); // 使用系统调用延时
    __delay_us(nus);
}

void delay_ms(uint32_t nms)
{
    // syscall.bsp_systick_delay_ms(nms); // 使用系统调用延时
    __delay_ms(nms);
}

void Soft_IIC_SCL(uint8_t state)
{
	if(state)
    {
        GPIO_WriteBit(SOFT_IIC_PORT, SOFT_IIC_SCL_PORT, Bit_SET);
    }
    else
    {
        GPIO_WriteBit(SOFT_IIC_PORT, SOFT_IIC_SCL_PORT, Bit_RESET);
    }
}

void Soft_IIC_SDA(uint8_t state)
{
	if(state)
    {
        GPIO_WriteBit(SOFT_IIC_PORT, SOFT_IIC_SDA_PORT, Bit_SET);
    }
    else
    {
        GPIO_WriteBit(SOFT_IIC_PORT, SOFT_IIC_SDA_PORT, Bit_RESET);
    }
}

void Soft_SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = SOFT_IIC_SDA_PORT;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 设置为浮空输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 设置速度
    GPIO_Init(SOFT_IIC_PORT, &GPIO_InitStructure);
}

void Soft_SDA_OUT(void)
{
	// DL_GPIO_initDigitalOutput(SOFT_IIC_SDA_PORT);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = SOFT_IIC_SDA_PORT;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; // 设置为输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 设置速度
    GPIO_Init(SOFT_IIC_PORT, &GPIO_InitStructure);
}

uint8_t Soft_READ_SDA(void)
{
	return GPIO_ReadInputDataBit(SOFT_IIC_PORT, SOFT_IIC_SDA_PORT);
}

SIAS i2c_Dev = {
    .Soft_IIC_GPIO_Port_Init = Soft_IIC_GPIO_Port_Init,
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