#ifndef __MSPM0G3507_H
#define __MSPM0G3507_H

#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/dl_gpio.h>
#include <ti/driverlib/dl_uart.h>


typedef enum {
    GPIO_DIR_INPUT = 0,
    GPIO_DIR_OUTPUT = 1,
    GPIO_DIR_Analog = 2,
    GPIO_DIR_DIGITAL = 3,
} GPIO_Direction;

typedef struct {
    GPIO_Regs *GPIOx;       /*GPIO Port*/
    uint8_t IO_Dir;         /*GPIO方向 @param[1] GPIO_DIR_INPUT @param[2] GPIO_DIR_OUTPUT @param[3] GPIO_DIR_Analog @param[3] GPIO_DIR_DIGITAL*/
    uint32_t PINCM;         /*MSP的GPIO索引号: 见于mspm0g350x.h文件 @param IOMUX_PINCM*/
    uint32_t GPIO_Pin;       /*原理图上的GPIO Pin*/
    DL_GPIO_INVERSION INVERSION;      /*GPIO输出反转功能*/
    DL_GPIO_RESISTOR RESISTOR;       /*GPIO内部上拉下拉电阻功能*/
    DL_GPIO_DRIVE_STRENGTH DRIVE_STRENGTH; /*GPIO的驱动能力*/
    DL_GPIO_HYSTERESIS HYSTERESIS;     /*GPIO的迟滞比较功能*(输入模式下)*/
    DL_GPIO_HIZ HIZ;            /*GPIO高阻态功能*/
    DL_GPIO_WAKEUP WAKEUP;         /*GPIO唤醒功能*/
    DL_GPIO_EVENT_ROUTE EVENT_ROUTE;    /*GPIO事件路由功能*/
    DL_GPIO_IIDX IIDX;           /*GPIO中断功能*(外部中断)*/
    uint32_t GPIO_AF;        /*GPIO复用功能*/
}GPIO_Init_Parameters;

typedef struct{
    GPIO_Regs *GPIOx;
    uint32_t PINCM;
    uint8_t GPIO_Pin;
    uint8_t GPIO_AF;
}GPIO_Pin_Parameters;

void MSP_GPIO_init(void *const Parameters);
void MSP_GPIO_setPin(void *const Parameters,uint32_t pin,uint8_t value);
uint8_t MSP_GPIO_getPin(void *const Parameters,uint32_t pin);
void MSP_IOMUX(void *const Parameters);

typedef struct{
    UART_Regs *UARTx;       /*UART port*/
    uint32_t baudrate;      /*波特率*/
    DL_UART_CLOCK clockSel;      /*时钟*/
    DL_UART_CLOCK_DIVIDE_RATIO divideRatio;   /*时钟分频系数*/
    DL_UART_CLOCK_DIVIDE2_RATIO divide2Ratio; /*时钟分频系数2*/
    DL_UART_MODE mode;                  /*模式配置*/
    DL_UART_DIRECTION direction;        /*方向(收发)*/
    DL_UART_FLOW_CONTROL flowControl;   /*流控配置*/
    DL_UART_PARITY parity;          /*奇偶校验配置*/
    DL_UART_WORD_LENGTH wordLength; /*配置帧长度*/
    DL_UART_STOP_BITS stopBits;     /*配置停止位*/
    DL_UART_OVERSAMPLING_RATE oversamplingRate; /*过采样速率*/
    DL_UART_IIDX IIDX;            /*中断索引*/
    DL_UART_TXD_OUT TXD;           /*TXD引脚配置*/
    DL_UART_RTS RTS;           /*RTS引脚配置*/
}USART_Parameters;

void MSP_UART_Init(void *const Parameters);


#endif