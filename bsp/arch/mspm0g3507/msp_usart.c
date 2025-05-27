#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>
#include <ti/devices/msp/peripherals/hw_uart.h>
#include <ti/devices/msp/m0p/mspm0g350x.h>
#include <ti/driverlib/dl_uart.h>
#include <sysport.h>


/*要是用C++就好了*/

void get_sbrosr(uint32_t bound,uint32_t srcClock_Hz,uint16_t *sbr,uint32_t *osr)
{
    uint16_t sbrTemp;
    uint32_t osrTemp, tempDiff, calculatedBaud, baudDiff;

    /* This LPUART instantiation uses a slightly different baud rate calculation
     * The idea is to use the best OSR (over-sampling rate) possible
     * Note, OSR is typically hard-set to 16 in other LPUART instantiations
     * loop to find the best OSR value possible, one that generates minimum baudDiff
     * iterate through the rest of the supported values of OSR */

    baudDiff = bound;
    *osr = 0;
    *sbr = 0;
    for (osrTemp = 4; osrTemp <= 32; osrTemp++)
    {
        /* calculate the temporary sbr value   */
        sbrTemp = (srcClock_Hz / (bound * osrTemp));
        /*set sbrTemp to 1 if the sourceClockInHz can not satisfy the desired baud rate*/
        if (sbrTemp == 0)
        {
            sbrTemp = 1;
        }
        /* Calculate the baud rate based on the temporary OSR and SBR values */
        calculatedBaud = (srcClock_Hz / (osrTemp * sbrTemp));

        tempDiff = calculatedBaud - bound;

        /* Select the better value between srb and (sbr + 1) */
        if (tempDiff > (bound - (srcClock_Hz / (osrTemp * (sbrTemp + 1)))))
        {
            tempDiff = bound - (srcClock_Hz / (osrTemp * (sbrTemp + 1)));
            sbrTemp++;
        }

        if (tempDiff <= baudDiff)
        {
            baudDiff = tempDiff;
            *osr = osrTemp; /* update and store the best OSR value calculated */
            *sbr = sbrTemp; /* update store the best SBR value calculated */
        }
    }
}

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
    get_sbrosr(uart->baudrate, 32768,&ibrd, &fbrd);  /*计算整数部分和小数部分*/
    DL_UART_Main_setOversampling(uart->UARTx, uart->oversamplingRate);  /*设置过采样*/
    DL_UART_Main_setBaudRateDivisor(uart->UARTx,1, 9);    /*设置波特率*/
 
    /* 使能接受中断 */
    DL_UART_Main_enableInterrupt(uart->UARTx, DL_UART_MAIN_INTERRUPT_RX);
    /* 使能串口 */
    DL_UART_Main_enable(uart->UARTx);
}

void MSP_SendString(void *const param,uint8_t *data){
    UART_Regs *ut = (UART_Regs *)param;
    while (*data != '\0') {  /*发送字符串*/
        DL_UART_Main_transmitDataBlocking(ut, *data++);  /*阻塞发送*/
    }
}

uint8_t MSP_ReceiveData(void *const param, uint8_t *data) {
    UART_Regs *ut = (UART_Regs *)param;
    *data = DL_UART_Main_receiveDataBlocking(ut);  /*阻塞接收*/
    return *data;  /*返回接收到的数据*/
}