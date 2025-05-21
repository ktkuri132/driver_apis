#include "f407.h"


/*
    冷门的外设,不设置对应的结构体和函数指针来管理
*/

/**
 * @brief 随机数发生器初始化函数
 * 
 */
void F407_RNG_Init(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN; // 使能RNG时钟
    RNG->CR |= RNG_CR_RNGEN; // 使能RNG
    RNG->SR = 0; // 清除状态寄存器
}

/**
 * @brief 得到一个随机数
 * 
 * @return 随机数 
 */
uint32_t Get_Random_Number(void) {
    // 等待随机数准备好
    while (!(RNG->SR & RNG_SR_DRDY));
    // 返回随机数
    return RNG->DR;
}