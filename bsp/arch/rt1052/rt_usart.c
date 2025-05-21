#include <sysport.h>
#ifdef CPU_MIMXRT1052DVL6B
#include <MIMXRT1052.h>
#include <stdint.h>

#include "rt1052.h"

uint8_t USART_GETRXS(void *const Parameters) {
    uart_st* uart_p = (uart_st*)Parameters;
    if(uart_p->USARTx->STAT & LPUART_STAT_RDRF(1)) {  // 判断接收中断
        return 1;  // 接收中断
    } else {
        return 0;  // 非接收中断
    }
}

uint8_t USART_GETTXS(){
    uint8_t TXE = 0;
    return TXE;
}

uint8_t USART_GETRE(){
    uint8_t RE = 0;
    return RE;
}

uint8_t lpuart_get_sbrosr(uint32_t clk,uint32_t baudrate,uint16_t *sbr,uint16_t *osr)
{
	uint32_t lpuartclk=clk;		
    uint16_t tempsbr;				
    uint32_t temposr;				
	uint32_t bauddiff;				
	uint32_t tempdiff;				
	uint32_t tempbaud;				 
    bauddiff=baudrate;											
	for(temposr=4;temposr<=32;temposr++)						
    {
        tempsbr=lpuartclk/(baudrate*temposr);					
        if(tempsbr==0)tempsbr=1;								
        tempbaud=lpuartclk/(temposr*tempsbr);					
		tempdiff=tempbaud-baudrate;								
        if(tempdiff>(baudrate-lpuartclk/(temposr*(tempsbr+1))))	
        {
            tempdiff=baudrate-lpuartclk/(temposr*(tempsbr+1));	
            tempsbr++;											
        } 
        if(tempdiff<=bauddiff)									
        {
            bauddiff=tempdiff;									
            *osr=temposr;										
            *sbr=tempsbr; 										
        }
    } 
    if(bauddiff>((baudrate/100)*3)||(*sbr>8191))return 1;		
	return 0; 
}

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


void RT_UART_Init(void* const Parameters){
    uart_st* uart_p = (uart_st*)Parameters;
    uint32_t __gpio;
    if (uart_p->GPIOx == GPIO1) {
        __gpio = 1;
    } else if (uart_p->GPIOx == GPIO2) {
        __gpio = 2;
    } else if (uart_p->GPIOx == GPIO3) {
        __gpio = 3;
    } else if (uart_p->GPIOx == GPIO4) {
        __gpio = 4;
    } else {
        printf(FG_RED "Error: Invalid GPIOx\n" RESET_ALL);
    }
    int i         = 0;
    uint8_t __pin = 0;
    if (uart_p->GPIO_Mode_x == 2) {  // 如果被设置成复用功能
        for (; i < 32; i++) {
            if (uart_p->GPIO_Pin_Source[i]) {
                // 引脚映射
                __pin = (__gpio - 1) * 32 + i;
                IOMUXC->SW_MUX_CTL_PAD[__pin] |=
                    IOMUXC_SW_MUX_CTL_PAD_MUX_MODE(uart_p->GPIO_AF);
                IOMUXC->SW_PAD_CTL_PAD[uart_p->GPIO_Pin_Source[i]] |= 0x10B0u;
            }
        }
    } else {
        printf(FG_RED "Error: Invalid Mode x\n" RESET_ALL);
    }
    uint16_t sbr;
    uint32_t osr;
//    lpuart_get_sbrosr(80000000,uart_p->baudrate,&sbr,&osr);
    get_sbrosr(uart_p->baudrate,80000000,&sbr,&osr);
    uart_p->USARTx->GLOBAL |= LPUART_GLOBAL_RST(1);    // 复位LPUART
    uart_p->USARTx->GLOBAL &= ~(1<<1);    // 解除复位
    // uart_p->USARTx->PINCFG |= LPUART_PINCFG_TRGSEL(1); // 允许RX接受触发
    uart_p->USARTx->BAUD = LPUART_BAUD_SBR(sbr) | LPUART_BAUD_OSR(osr-1);
    if((osr>3)&&(osr<8)){   // 4~7采取双边沿采集
        uart_p->USARTx->BAUD |=LPUART_BAUD_BOTHEDGE(1);
    }
    uart_p->USARTx->CTRL |= LPUART_CTRL_RIE(1); // 使能接收中断
    uart_p->USARTx->CTRL |= LPUART_CTRL_TE(1)| LPUART_CTRL_RE(1); // 使能收发
}

uint8_t RT_USART_ReceiveData(void* const Parameters,uint8_t *data) {
    uart_st *uart_p = (uart_st *)Parameters;
    uint8_t _data = 0;
    while (!(uart_p->USARTx->STAT & LPUART_STAT_RDRF_MASK));  // 等待接收缓冲区非空
    _data = uart_p->USARTx->DATA;   
    if(data != NULL) {
        *data = _data;  // 将接收到的数据存储到data中
    }
    return *data;
}

void RT_USART_SendData(void* const Parameters, uint8_t data) {
    uart_st *uart_p = (uart_st *)Parameters;
    while (!(uart_p->USARTx->STAT & LPUART_STAT_TDRE_MASK));  // 等待发送缓冲区空
    uart_p->USARTx->DATA = data;                              // 发送数据
}

void RT_USART_SendString(void* const Parameters, uint8_t *str) {
    uart_st *uart_p = (uart_st *)Parameters;
    while (*str) {
        RT_USART_SendData(uart_p, *str++);
    }
}

















#endif