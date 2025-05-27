#ifndef BSP_IIC_H
#define BSP_IIC_H

#include <stdint.h>

/**
 * @brief i2c message structure
 * 
 */
typedef struct {
    uint8_t address[3];  // I2C设备地址(最多支持3个地址)
    uint8_t speed;  // I2C通信速度
    uint8_t len;  // 数据长度
    uint8_t *buf;  // 数据缓冲区指针
    uint8_t flags;  // 标志位(0x01表示读操作, 0x00表示写操作)
} i2c_msg_t;

/**
 * @brief i2c State structure
 * 
 */
typedef struct{
    uint8_t busy;  // I2C总线是否忙
    uint8_t error;  // I2C通信错误标志
    uint8_t ack;  // I2C应答标志
    uint8_t Device_Success;  // I2C设备是否成功连接
    uint8_t Device_Fail;  // I2C设备连接失败标志
} i2c_ss_t;

/**
 * @brief i2c driver function opions structure
 * 
 */
typedef struct {
    void (*Init)();  // 初始化函数
    void (*scl_up)();  // SCL引脚拉高
    void (*scl_down)();  // SCL引脚拉低
    void (*sda_set_output)();  // 设置SDA引脚为输出
    void (*sda_up)();  // SDA引脚拉高
    void (*sda_down)();  // SDA引脚拉低
    void (*sda_set_input)();  // 设置SDA引脚为输入
    uint8_t (*sda_read)();  // 读取SDA引脚状态
    void (*delay_us)(uint32_t us);  // 延时函数
} i2c_dfo_t;


typedef struct {
    i2c_msg_t bcs;  // 基础配置
    i2c_ss_t ss;    // 状态
    i2c_dfo_t dfo;  // 驱动函数选项
} i2c_t;



#endif