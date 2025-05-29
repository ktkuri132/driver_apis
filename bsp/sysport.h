#ifndef __NVIC_
#define __NVIC_

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <syscall.h>

#ifdef MDK
#undef STDLIB
#endif

/* 添加芯片头文件  */
#ifdef IMX_RT1052
#include "arch/rt1052/rt1052.h"
#include <core/rt1052/MIMXRT1052.h>
#include <core/rt1052/MIMXRT1052.h>
#endif
#ifdef __MSPM0G3507__
#include <arch/mspm0g3507/mspm0.h>
#include <ti/devices/msp/m0p/mspm0g350x.h>
#include <ti/driverlib/driverlib.h>
#endif
#ifdef STM32F10X_MD
#include <arch/stm32f1/f103.h>
#include <stm32f10x.h>
#endif


typedef void (*func)(void *const Parameters);
typedef void (*Bfunc)(int argc, void *argv[]);
/**
 * @brief GPIO接口结构体
 * @note  该结构体用于存储GPIO接口的相关参数和函数指针
 * @param  bsp_gpio_init: GPIO初始化函数指针
 * @param  bsp_gpio_af_set: GPIO复用函数指针
 * @param  bsp_gpio_pin_set: GPIO引脚设置函数指针
 * @param  bsp_gpio_pin_get: GPIO引脚读取函数指针
 * @param  GetGPIOPeriphClock: 获取GPIO外设时钟函数指针
 */
typedef struct {
    func bsp_gpio_init;
    func bsp_gpio_af_set;
    void (*bsp_gpio_pin_set)(void *const Parameters, uint32_t pin, uint8_t status);
    uint8_t (*bsp_gpio_pin_get)(void *const Parameters, uint32_t pin);
    uint32_t (*GetGPIOPeriphClock)(void *const Parameters);
} GPIO_Port;

/**
 * @brief I2C接口结构体
 * @note  该结构体用于存储I2C接口的相关参数和函数指针
 * @param  bsp_IIC_Init: I2C初始化函数指针
 * @param  bsp_IIC_Start: I2C开始函数指针
 * @param  bsp_IIC_Stop: I2C停止函数指针
 * @param  bsp_IIC_Send_Byte: I2C发送字节函数指针
 * @param  bsp_IIC_Receive_Byte: I2C接收字节函数指针
 * @param  bsp_IIC_Wait_Ack: I2C等待应答函数指针
 * @param  bsp_IIC_Ack: I2C应答函数指针
 * @param  bsp_IIC_NAck: I2C不应答函数指针
 */
typedef struct {
    func bsp_IIC_Init;
    func bsp_IIC_Start;
    func bsp_IIC_Stop;
    void (*bsp_IIC_Send_Byte)(uint8_t txd);
    uint8_t (*bsp_IIC_Receive_Byte)(unsigned char ack);
    uint8_t (*bsp_IIC_Wait_Ack)(void);
    void (*bsp_IIC_Ack)(void);
    void (*bsp_IIC_NAck)(void);
    uint8_t (*bsp_IIC_Write_Len)(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);
    uint8_t (*bsp_IIC_Read_Len)(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);
    uint8_t (*bsp_IIC_Write_Byte)(uint8_t addr, uint8_t reg, uint8_t data);
    uint8_t (*bsp_IIC_Read_Byte)(uint8_t addr, uint8_t reg);
} I2C_Port;


/**
 * @brief SPI接口结构体
 * 
 */
typedef struct {
    struct GPIO_Init_Parameters*  (*bsp_SPI_Init)(void);
    void (*bsp_SPI_SendByte)(void* const Parameters,uint8_t CS,uint8_t Byte);
    void (*bsp_SPI_SendData)(void* const Parameters,uint8_t CS,uint8_t *Data,uint8_t Count);
    uint8_t (*bsp_SPI_ReceiveByte)(void* const Parameters,uint8_t CS);
    void (*bsp_SPI_ReceiveData)(void* const Parameters,uint8_t CS,uint8_t *Data,uint8_t Count);
    uint8_t (*bsp_SPI_SwapByte)(void* const Parameters,uint8_t SData);
} SPI_Port;

/**
 * @brief USART接口结构体
 * @note  该结构体用于存储USART接口的相关参数和函数指针
 * @param  bsp_usart_x_inti: USART初始化函数指针
 * @param  bsp_usart_x_send: USART发送函数指针
 */
typedef struct {
    func bsp_usart_x_inti;
    void (*bsp_usart_x_send_byte)(void *const Parameters,uint8_t data);
    void (*bsp_usart_x_send_string)(void *const Parameters, uint8_t *str);
    uint8_t (*bsp_usart_x_receive)(void *const Parameters,uint8_t*data);
    uint8_t (*bsp_get_rxs)(void *const Parameters);
	uint8_t ENS;
} USART_Port;

/**
 * @brief TIM接口结构体
 * @note  该结构体用于存储TIM接口的相关参数和函数指针
 * @param  bsp_tim_x_inti: TIM初始化函数指针
 * @param  bsp_tim_x_start: TIM启动函数指针
 * @param  其他的额函数暂未用到
 */
typedef struct {
    func bsp_tim_x_inti;
    func bsp_tim_x_start;
} TIM_Port;

/**
 * @brief SYSCALL(系统调用)接口结构体
 * @note  该结构体用于存储SYSCALL接口的相关参数和函数指针
 * @param  bsp_systick_delay_us: SysTick延时函数指针
 * @param  bsp_systick_delay_ms: SysTick延时函数指针
 * @param  NVIC_Configuration: NVIC配置函数指针
 */
typedef struct {
    void (*bsp_systick_delay_us)(uint32_t us);
    void (*bsp_systick_delay_ms)(uint32_t ms);
    void (*NVIC_Configuration)(void);
} SYSCALL_Port;

/**
 * @brief RCC(时钟)接口结构体
 * @note  该结构体用于存储RCC接口的相关参数和函数指针
 * @param  bsp_rcc_periph_clock: RCC外设时钟函数指针
 */
typedef struct {
    func bsp_rcc_periph_clock;
} RCC_PeriphClock_Port;

/**
 * @brief SystemRunTime(系统运行时间)结构体
 * @note  该结构体用于存储系统运行时间的相关参数
 * @param  SysRunTime: 系统运行时间(不间断计时器)
 * @param  SysRunTimeSec: 系统运行秒数
 * @param  SysRunTimeMin: 系统运行分钟
 * @param  SysRunTimeBeat: 系统运行节拍
 */
typedef struct {
    uint64_t SysRunTime;      // 系统运行时间(不间断计时器)
    uint32_t SysRunTimeSec;   // 系统运行秒数
    uint32_t SysRunTimeMin;   // 系统运行分钟
    uint32_t SysRunTimeBeat;  // 系统运行节拍
} SystemRunTime_t;

/**
 * @brief 通用函数参数结构体
 * 
 */
typedef struct {
    void **argv;
    int argc;
}param;

/**
 * @brief Sysfpoint(系统函数指针)结构体
 * @note  该结构体用于存储系统函数指针和参数
 * @param  syspfunc: 系统函数指针
 * @param  Parameters: 系统函数指针参数
 * @param  argc: 参数个数
 */
typedef struct {
    void (*syspfunc)(int argc, void *argv[]);  // 系统函数指针
    void **Parameters;
    int argc;
} Sysfpoint;

/**
 * @brief SYS_Port(系统接口)结构体
 * @note  该结构体用于存储系统接口的相关参数和函数指针
 * @param  SRT: 系统运行时间
 * @param  SFP: 系统通用函数指针结构体
 * @param  syscall_port: SYSCALL接口结构体
 * @param  rcc_periph_port: RCC接口结构体
 * @param  gpio_port: GPIO接口结构体
 * @param  i2c_port: I2C接口结构体
 * @param  usart_port: USART接口结构体
 * @param  tim_port: TIM接口结构体
 * @param  System_Init: 系统初始化函数指针
 * @param  SysTick_Init: SysTick初始化函数指针
 */
typedef struct {
    SystemRunTime_t SRT;
    Sysfpoint SFP;
    SYSCALL_Port syscall_port;
    RCC_PeriphClock_Port rcc_periph_port;
    GPIO_Port gpio_port;
    I2C_Port i2c_port;
    SPI_Port spi_port;
    USART_Port usart_port;
    TIM_Port tim_port;
    void (*System_Init)(void);
    void (*SysTick_Init)(void);
} SYS_Port;

SYS_Port* SysPort_Init();


/**
 * @brief  系统接口结构体
 * @note   该结构体用于存储系统接口的相关参数和函数指针,如果没有定义sys_port,则会报错
 * @param  sys_port: 系统接口结构体
 */

/* 定义系统指针  */
#define srt sys_port->SRT              /* 系统时钟指针  */
#define sfp sys_port->SFP              /* 系统函数指针  */
#define syscall sys_port->syscall_port /* 系统调用指针  */
#define gpio sys_port->gpio_port       /* gpio指针  */
#define rcc sys_port->rcc_periph_port  /* 外设时钟指针  */
#define i2c sys_port->i2c_port         /* I2C指针  */
#define spi sys_port->spi_port         /* SPI指针  */
#define usart sys_port->usart_port     /* USART指针  */
#define tim sys_port->tim_port         /* TIM指针  */

#define gpio_st GPIO_Init_Parameters
#define uart_st USART_Parameters

extern SYS_Port *sys_port; // 声明系统接口指针

#endif