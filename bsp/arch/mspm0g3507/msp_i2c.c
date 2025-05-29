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

void MSP_I2C_Port_Init(void) {
    // 初始化I2C端口
    
}

void MSP_IIC_SCL_UP(void) {
    // 设置SCL引脚为高电平
    DL_GPIO_setPins(GPIOB, DL_GPIO_PIN_11);
}

void MSP_IIC_SCL_DOWN(){
    // 设置SCL引脚为低电平
    DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_11);
}

void MSP_IIC_SDA_SET_OUTPUT(void) {
    // 设置SDA引脚为输出模式
    DL_GPIO_initDigitalOutput(IOMUX_PINCM18);
    DL_GPIO_enableOutput(GPIOB, DL_GPIO_PIN_5);
}

void MSP_IIC_SDA_UP(void) {
    // 设置SDA引脚为高电平
    DL_GPIO_setPins(GPIOB, DL_GPIO_PIN_5);
}

void MSP_IIC_SDA_DOWN(void) {
    // 设置SDA引脚为低电平
    DL_GPIO_clearPins(GPIOB, DL_GPIO_PIN_5);
}

void MSP_IIC_SDA_SET_INPUT(void) {
    // 设置SDA引脚为输入模式
    DL_GPIO_initDigitalInput(IOMUX_PINCM18);
}

uint8_t MSP_IIC_SDA_READ(void) {
    // 读取SDA引脚的状态
    return DL_GPIO_readPins(GPIOB, DL_GPIO_PIN_5);
}




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