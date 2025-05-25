#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>
#include <ti/devices/msp/peripherals/hw_uart.h>
#include <ti/devices/msp/m0p/mspm0g350x.h>
#include <ti/driverlib/dl_uart.h>
#include <sysport.h>


/**
 * @brief 计算 UART 波特率分频值
 * @param clockFreq UART 时钟频率（单位：Hz）
 * @param baudRate 目标波特率（单位：bps）
 * @param ibrd 输出参数，用于存储计算得到的整数分频值
 * @param fbrd 输出参数，用于存储计算得到的小数分频值
 */
void calculateUARTDividers(uint32_t clockFreq, uint32_t baudRate, uint32_t *ibrd, uint32_t *fbrd) {
    if (baudRate == 0 || clockFreq == 0) {
        return;
    }

    // 计算分频值
    double divider = (double)clockFreq / (16.0 * baudRate);

    // 提取整数部分和小数部分
    *ibrd = (uint32_t)divider; // 整数部分
    double fractionalPart = divider - *ibrd;

    // 计算小数分频值（四舍五入到最近的整数）
    *fbrd = (uint32_t)(fractionalPart * 64.0 + 0.5); // 小数部分乘以 64
}

/*要是用C++就好了*/


void MSP_UART_Init(void *const Parameters) {
    uart_st *uart = (uart_st *)Parameters;

    if(uart->clockSel == NULL){  /*默认BUSCLK*/
        uart->clockSel = DL_UART_MAIN_CLOCK_LFCLK;
    }
    if(uart->divideRatio == NULL){ /*默认分频系数1*/
        uart->divideRatio = DL_UART_MAIN_CLOCK_DIVIDE_RATIO_1;
    }
    if(uart->mode == NULL){ /*默认普通模式*/
        uart->mode = DL_UART_MODE_NORMAL;
    }
    if(uart->direction == NULL){ /*默认收发模式*/
        uart->direction = DL_UART_DIRECTION_TX_RX;
    }
    if(uart->flowControl == NULL){ /*默认无流控*/
        uart->flowControl = DL_UART_FLOW_CONTROL_NONE;
    }
    if(uart->parity == NULL){ /*默认无奇偶校验*/
        uart->parity = DL_UART_PARITY_NONE;
    }
    if(uart->wordLength == NULL){ /*默认8位数据位*/
        uart->wordLength = DL_UART_WORD_LENGTH_8_BITS;
    }
    if(uart->stopBits == NULL){ /*默认1位停止位*/
        uart->stopBits = DL_UART_STOP_BITS_ONE;
    }
    if(uart->oversamplingRate == NULL){ /*默认3倍过采样*/
        uart->oversamplingRate = DL_UART_OVERSAMPLING_RATE_3X;
    }
    // /*设置串口的时钟和分频*/
    uart->UARTx->CLKSEL = uart->clockSel;
    uart->UARTx->CLKDIV = uart->divideRatio;

    /*配置基础串口功能*/
    DL_Common_updateReg(&uart->UARTx->CTL0,
        (uint32_t) uart->mode | (uint32_t) uart->direction |
            (uint32_t) uart->flowControl,
        UART_CTL0_RXE_MASK | UART_CTL0_TXE_MASK | UART_CTL0_MODE_MASK |
            UART_CTL0_RTSEN_MASK | UART_CTL0_CTSEN_MASK | UART_CTL0_FEN_MASK);

    DL_Common_updateReg(&uart->UARTx->LCRH,
        (uint32_t) uart->parity | (uint32_t) uart->wordLength |
            (uint32_t) uart->stopBits,
        UART_LCRH_PEN_ENABLE | UART_LCRH_EPS_MASK | UART_LCRH_SPS_MASK |
            UART_LCRH_WLEN_MASK | UART_LCRH_STP2_MASK);

    uint32_t ibrd, fbrd;
    // calcula teUARTDividers(32768,uart->baudrate, &ibrd, &fbrd);  /*计算整数部分和小数部分*/
    DL_UART_Main_setOversampling(uart->UARTx, uart->oversamplingRate);  /*设置过采样*/
    DL_UART_Main_setBaudRateDivisor(uart->UARTx, 1,9);    /*设置波特率*/
 
    /* 使能接受中断 */
    DL_UART_Main_enableInterrupt(uart->UARTx, DL_UART_MAIN_INTERRUPT_RX);
    /* 使能串口 */
    DL_UART_Main_enable(uart->UARTx);
}

void MSP_UART_SetBaudRate(uint32_t baudrate) {}