/**
 * @file bsp_iic.c
 * @author ktkuri132 (jg3457532@gmail.com)
 * @brief 典型iic驱动中间层文件.参照Github上的开源iic驱动:sf_i2c
 * @version 0.1
 * @date 2025-05-27
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "bsp_iic.h"

#define Low_Speed   0  // I2C低速模式
#define High_Speed  1  // I2C高速模式

#define INIT(p_dev)                 p_dev->dfo.Init()              // 端口初始化函数
#define SCL_UP(p_dev)               p_dev->dfo.scl_up()             // SCL引脚拉高
#define SCL_DOWN(p_dev)             p_dev->dfo.scl_down()           // SCL引脚拉低
#define SDA_SET_OUTPUT(p_dev)       p_dev->dfo.sda_set_output()     // 设置SDA引脚为输出
#define SDA_UP(p_dev)               p_dev->dfo.sda_up()             // SDA引脚拉高
#define SDA_DOWN(p_dev)             p_dev->dfo.sda_down()           // SDA引脚拉低
#define SDA_SET_INPUT(p_dev)        p_dev->dfo.sda_set_input()      // 设置SDA引脚为输入
#define SDA_READ(p_dev)             p_dev->dfo.sda_read()           // 读取SDA引脚状态
#define DELAY_US_Low(p_dev)         p_dev->dfo.delay_us(Low_Speed)         // 延时函数
#define DELAY_US_High(p_dev)        p_dev->dfo.delay_us(High_Speed)        // 延时函数

#define BUSY(p_dev)                 p_dev->ss.busy                 // I2C总线是否忙
#define ERROR(p_dev)                p_dev->ss.error                // I2C通信错误标志
#define ACK(p_dev)                  p_dev->ss.ack                  // I2C应答标志
#define DEVICE_SUCCESS(p_dev)       p_dev->ss.Device_Success       // I2C设备是否成功连接
#define DEVICE_FAIL(p_dev)          p_dev->ss.Device_Fail          // I2C设备连接失败标志

/**
 * @brief I2C设备初始化函数
 * 
 * @param dev 指向I2C设备结构体的指针
 * @return int 返回0表示成功，其他值表示错误
 */
int i2c_dev_init(i2c_t *dev){
    INIT(dev);  // 调用端口初始化函数
    BUSY(dev) = 0;  // 设置总线状态为空闲
    ERROR(dev) = 0;  // 清除通信错误标志
    ACK(dev) = 0;  // 清除应答标志
    DEVICE_SUCCESS(dev) = 0;  // 清除设备连接成功标志
    DEVICE_FAIL(dev) = 0;  // 清除设备连接失败标志
    SDA_SET_OUTPUT(dev);  // 设置SDA引脚为输出
    SCL_UP(dev);  // 拉高SCL引脚
    SDA_UP(dev);  // 拉高SDA引脚
    DELAY_US_Low(dev);  // 延时，确保引脚状态稳定
}

/**
 * @brief i2c_start - 发送I2C起始信号
 * 
 * @param dev 参数结构体
 */
void i2c_start(i2c_t *dev){
    SDA_SET_OUTPUT(dev);  // 设置SDA引脚为输出
    SCL_UP(dev);  // 拉高SCL引脚
    DELAY_US_Low(dev);  // 延时，确保引脚状态稳定
    SDA_DOWN(dev);  // 拉低SDA引脚
    DELAY_US_Low(dev);  // 延时，确保引脚状态稳定
    SCL_DOWN(dev);  // 拉低SCL引脚
}

void i2c_restart(i2c_t *dev){
    SDA_SET_OUTPUT(dev);  // 设置SDA引脚为输出
    SCL_UP(dev);  // 拉高SCL引脚
    DELAY_US_Low(dev);  // 延时，确保引脚状态稳定
    SDA_UP(dev);  // 拉高SDA引脚
    DELAY_US_Low(dev);  // 延时，确保引脚状态稳定
    SDA_DOWN(dev);  // 拉低SDA引脚
    DELAY_US_Low(dev);  // 延时，确保引脚状态稳定
    SCL_DOWN(dev);  // 拉低SCL引脚
}

void i2c_stop(i2c_t *dev){
    SDA_SET_OUTPUT(dev);  // 设置SDA引脚为输出
    SCL_DOWN(dev);  // 拉低SCL引脚
    DELAY_US_Low(dev);  // 延时，确保引脚状态稳定
    SDA_DOWN(dev);  // 拉低SDA引脚
    DELAY_US_Low(dev);  // 延时，确保引脚状态稳定
    SCL_UP(dev);  // 拉高SCL引脚
    DELAY_US_Low(dev);  // 延时，确保引脚状态稳定
    SDA_UP(dev);  // 拉高SDA引脚
}

void i2c_ack(i2c_t *dev){
    SDA_SET_OUTPUT(dev);  // 设置SDA引脚为输出
    SCL_DOWN(dev);  // 拉低SCL引脚
    DELAY_US_Low(dev);  // 延时，确保引脚状态稳定
    SDA_DOWN(dev);  // 拉低SDA引脚
    DELAY_US_Low(dev);  // 延时，确保引脚状态稳定
    SCL_UP(dev);  // 拉高SCL引脚
    DELAY_US_Low(dev);  // 延时，确保引脚状态稳定
    SCL_DOWN(dev);  // 拉低SCL引脚
}

void i2c_nack(i2c_t *dev){
    SDA_SET_OUTPUT(dev);  // 设置SDA引脚为输出
    SCL_DOWN(dev);  // 拉低SCL引脚
    DELAY_US_Low(dev);  // 延时，确保引脚状态稳定
    SDA_UP(dev);  // 拉高SDA引脚
    DELAY_US_Low(dev);  // 延时，确保引脚状态稳定
    SCL_UP(dev);  // 拉高SCL引脚
    DELAY_US_Low(dev);  // 延时，确保引脚状态稳定
    SCL_DOWN(dev);  // 拉低SCL引脚
}

i2c_ss_t i2c_wait_ack(i2c_t *dev){
    uint16_t wait_time = 0xFFF;  // 等待时间计数器

    SDA_SET_INPUT(dev);  // 设置SDA引脚为输入
    SCL_UP(dev);  // 拉高SCL引脚
    DELAY_US_Low(dev);  // 延时，确保引脚状态稳定

    while (SDA_READ(dev)) {  // 等待SDA引脚拉低
        if ((wait_time--) == 0) {
            SDA_UP(dev);  // 拉高SDA引脚
            SDA_SET_OUTPUT(dev);  // 设置SDA引脚为输出
            i2c_stop(dev);  // 发送停止信号
            ERROR(dev) = 1;  // 设置通信错误标志
            return dev->ss;  // 返回状态结构体
        }
    }

    DELAY_US_Low(dev);  // 延时，确保引脚状态稳定
    SCL_DOWN(dev);  // 拉低SCL引脚
    SDA_UP(dev);  // 拉高SDA引脚
    SDA_SET_OUTPUT(dev);  // 设置SDA引脚为输出

    return dev->ss;  // 返回状态结构体
}

i2c_ss_t i2c_send_byte(i2c_t *dev, uint8_t byte){
    uint8_t i;

    for (i = 0; i < 8; i++) {
        SCL_DOWN(dev);  // 拉低SCL引脚
        DELAY_US_Low(dev);  // 延时，确保引脚状态稳定

        if (byte & 0x80) {
            SDA_UP(dev);  // SDA引脚拉高
        } else {
            SDA_DOWN(dev);  // SDA引脚拉低
        }

        SCL_UP(dev);  // 拉高SCL引脚
        DELAY_US_Low(dev);  // 延时，确保引脚状态稳定
        byte <<= 1;  // 左移一位
    }

    return i2c_wait_ack(dev);  // 等待应答信号
}

uint8_t i2c_read_byte(i2c_t *dev, uint8_t ack,uint8_t *data){
    uint8_t i = 0;

    SDA_SET_INPUT(dev);  // 设置SDA引脚为输入
    for (i = 0; i < 8; i++) {
        *data <<= 1;  // 左移一位
        SCL_DOWN(dev);  // 拉低SCL引脚
        DELAY_US_Low(dev);  // 延时，确保引脚状态稳定
        SCL_UP(dev);  // 拉高SCL引脚

        if (SDA_READ(dev)) {
            *data |= 0x01;  // 如果SDA引脚为高，则设置最低位为1
        }

        DELAY_US_Low(dev);  // 延时，确保引脚状态稳定
    }

    if (ack) {
        i2c_ack(dev);  // 发送应答信号
    } else {
        i2c_nack(dev);  // 发送非应答信号
    }

    return *data;  // 返回读取的字节数据
}

i2c_ss_t i2c_send_bytes(i2c_t *dev, uint8_t *data, uint16_t len){
    uint16_t i;
    i2c_ss_t ret;
    for (i = 0; i < len; i++) {
        ret = i2c_send_byte(dev, data[i]);  // 逐字节发送数据
    }

    return ret;  // 返回状态结构体
}
