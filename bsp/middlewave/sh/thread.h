#ifndef __THREAD_H
#define __THREAD_H

#include <sysport.h>

/**
 * @brief 中断处理函数结构体
 * @param IRQ_Num 中断号
 * @param IRQ_Stare 中断状态
 * @param IRQ_Handler 中断处理函数指针
 * @note 该结构体用于存储中断处理函数的相关参数和函数指针
 */
typedef struct {
    uint8_t IRQ_Num;
    uint8_t IRQ_Stare;
    void (*IRQ_Handler)(int avc ,void **argv);
}__IRQ;

void IRQ_Thread_Init();
int IRQ_Deal(__IRQ *irq,int argc,void **argv);


/**
 * @brief 线程元素结构体
 * @param Thread_Function 线程函数指针
 * @param Thread_Stare 线程状态
 * @param Thread_Stack 线程栈顶指针
 * @param Thread_Stack_Size 线程栈大小
 * @param TID 线程ID
 */
typedef struct {
    uint8_t TID;
    uint8_t Priority;
    char* Thread_Name;
    void*(*Thread_Function)(int avc ,void **argv);
    param *Thread_Parameters;
    uint8_t Thread_Stare;
    uint32_t Thread_Stack_Size;
    uint8_t *Thread_Stack;
}Thread;

/**
 * @brief 队列管理结构体
 * @param Thread_Num 线程数量
 * @param Thread_Stare 线程状态
 * @param Thread_Stack 线程栈
 * @param Thread_Stack_Size 线程栈大小
 * @param Stack_head_Pointer 栈头指针
 * @note 该结构体用于管理线程的相关参数和函数指针
 */
typedef struct {
    uint8_t Thread_Num;     /*线程数量*/
    Thread **pools;   /*线程池*/
}Queue;







#endif