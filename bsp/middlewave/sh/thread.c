#include <stdint.h>
#include "thread.h"
#include "Serial.h"
#include <stdbool.h>
#include <string.h>
#include <sysport.h>
#include <stdlib.h>
#include <time.h>
__IRQ irqs[10];

/**
 * @brief 中断处理函数初始化
 * 
 */
void IRQ_Thread_Init(){

}

/**
 * @brief 中断回调函数处理
 * @note 处理中断发生时的回调信号
 * @param irq 中断处理结构体指针
 * @param argc 参数数量
 * @param argv 参数数组
 * @return -1表示错误，0表示成功
 */
int IRQ_Deal(__IRQ *irq,int argc,void **argv){
    if(irq->IRQ_Handler == NULL){
        if(usart.ENS == 1){
            printf(FG_RED "Error: IRQ_Handler is NULL\n" RESET_ALL);
        }
        return -1;
    }
    sfp.syspfunc = irq->IRQ_Handler;
    sfp.argc = argc;  // 设置参数个数
    sfp.Parameters = argv;  // 设置参数
}


/**
 * @brief 初始化队列
 * @note 初始化线程池和线程数量
 * @param queue 队列指针
 * @return 队列指针
 */
Queue* Queue_Init(Queue *queue){
    queue->Thread_Num = 0; // 初始化线程数量
    /*线程池初始化*/
    if(queue->pools == NULL){
        queue->pools = (Thread **)malloc(sizeof(Thread *) * 20); // 分配线程池内存,最多20个线程
        if(queue->pools == NULL){
            if (usart.ENS == 1) {
                printf(FG_RED "Error: Thread pool allocation failed\n" RESET_ALL);
            }
            while(1); // 阻塞
        }
    }
    return 0;
}

/**
 * @brief 入队函数
 * @note 将线程添加到队列中
 * @param queue 全局队列指针
 * @param thr 线程指针
 * @return -1表示错误，0表示成功
 */
int Queue_Push(Queue *queue,Thread *thr){
    if(thr == NULL || queue == NULL){
        if (usart.ENS == 1) {
            printf(FG_RED "Error: Thread or queue is NULL\n" RESET_ALL);
        }
        return -1;
    }
    queue->Thread_Num++; // 增加线程数量
    if(queue->Thread_Num > 20){
        queue->Thread_Num--;
        if (usart.ENS == 1) {
            printf(FG_RED "Error: Thread pool is full\n" RESET_ALL);
        }
        return -1; // 队列已满
    } else {
        queue->pools[queue->Thread_Num] = thr; // 将线程添加到队列
        queue->pools[queue->Thread_Num]->Thread_Stare = 1; // 设置线程状态为就绪态
    }
    return 0;
}

/**
 * @brief 出队函数
 * @在队列中移除线程
 * @param queue 队列
 * @param thr 线程
 * @return -1表示错误，0表示成功
 */
int Queue_Pop(Queue *queue,Thread *thr){
    if(thr == NULL || queue == NULL){
        if (usart.ENS == 1) {
            printf(FG_RED "Error: Thread or queue is NULL\n" RESET_ALL);
        }
        return -1;
    }
    queue->pools[queue->Thread_Num] = NULL; // 将线程从队列中移除
    queue->Thread_Num--; // 减少线程数量
    return 0;
}


int Thread_schedul(Queue *queue){
    for(int i = 0; i < 20; i++){
        if(queue->pools[i] != NULL){
            
        }
    }
    return 0;
}


#define MAX_THREAD_ID 20 // 最大线程号
static bool thread_id_map[MAX_THREAD_ID] = {0}; // 用于记录线程号是否已分配

uint32_t Get_Random_Number(){
    // 这里可以使用硬件随机数生成器或其他方法生成随机数
    // 例如，使用系统时钟作为种子生成伪随机数
//    srand((unsigned int)time(NULL)); // 使用当前时间作为种子
    return rand(); // 返回一个随机数
}
/**
 * @brief 生成唯一的线程号
 * @return 返回生成的线程号
 */
static uint8_t Generate_Thread_ID() {
    uint8_t thread_id;
    while(thread_id_map[thread_id]) { // 检查线程号是否已分配
        uint32_t Temp_Random_Number_32bit = Get_Random_Number();  /*生成一个32位的随机数*/
        thread_id = (Temp_Random_Number_32bit % 20) + 1; /* 将随机数限制在 1 到 20 */
    }
    thread_id_map[thread_id] = true; // 标记线程号为已分配
    return thread_id;
}

/**
 * @brief 线程注册函数(动态)
 * @note 注册线程到线程池
 * @return -1表示错误，0表示成功
 */
int Thread_Registration(Thread *thr,void* (*Thread_Function)(int argc ,void **argv),char* const Thread_Name,void **argv, uint32_t Thread_Stack_Size){
    
    thr = (Thread *)malloc(sizeof(Thread)+Thread_Stack_Size); 
    if (thr == NULL) {
        if (usart.ENS == 1) {
            printf(FG_RED "Error: Thread allocation failed\n" RESET_ALL);
        }
        return -1; // 返回错误
    }    /*分配线程空间*/
    thr->TID = Generate_Thread_ID(); // 生成唯一的线程号
    thr->Thread_Name = Thread_Name;
    thr->Thread_Function = Thread_Function;
    thr->Thread_Stack = (uint8_t *)(thr + 1); // 设置线程栈指针
    return 0;
}

/**
 * @brief 线程注销函数
 * @note 注销已注册的线程,释放内存资源
 * @param queue 
 * @return int 
 */
int Thread_Deregistration(Thread *thr){
    if(thr == NULL){
        return -1;
    }
    thread_id_map[thr->TID] = false; // 释放线程号
    free(thr);    /*释放线程空间*/
    return 0;
}
