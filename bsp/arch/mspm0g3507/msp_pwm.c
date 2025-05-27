#include <sysport.h>


void MSP_Motor_Set(GPTIMER_Regs *TIMERx,uint8_t Motor,uint32_t Load){
	DL_TimerA_setCaptureCompareValue(TIMERx, Load, Motor);
}

uint32_t MSP_Motor_Get(GPTIMER_Regs *TIMERx,uint8_t Motor) {
	return DL_TimerA_getCaptureCompareValue(TIMERx, Motor);
}