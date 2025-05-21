/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨������i.MX RT1052���İ�-���ܳ���
����    д��CHIUSIR
��E-mail  ��chiusir@163.com
������汾��V1.0
�������¡�2018��2��1��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://shop36265907.taobao.com
------------------------------------------------
��dev.env.��IAR8.20.1�����ϰ汾
��Target �� i.MX RT1052
��Crystal�� 24.000Mhz
��ARM PLL�� 1200MHz
��SYS PLL�� 528MHz
��USB PLL�� 480MHz
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef _LQ_GPT_H_
#define _LQ_GPT_H_


/**
  * @brief    ����GPT ��ʱ��
  *
  * @param    
  *
  * @return   
  *
  * @note     ʾ���� A7   500Hz��ת
  *
  * @example  
  *
  * @date     2019/6/10 ����һ
  */
void Test_GPTTimer(void);


/**
  * @brief    ����GPT ��ʱ
  *
  * @param    
  *
  * @return   
  *
  * @note     ʾ���� A7   500Hz��ת
  *
  * @example  
  *
  * @date     2019/6/10 ����һ
  */
void Test_GPTDelay(void);




/**
  * @brief    GPT ͨ�ö�ʱ����ʼ��
  *
  * @param    
  *
  * @return   
  *
  * @note     ��ʼ��GPT������  GPT1��������ʱ��  GPT2������ȷ��ʱ
  *
  * @example  
  *
  * @date     2019/6/10 ����һ
  */
void LQ_GPT_Init(void);



/**
  * @brief    gpt ��ʱ����ȷ��ʱ
  *
  * @param    ms �� ��ʱʱ�� ms
  *
  * @return   
  *
  * @note     GPT ��32Ϊ������ �����ʱ4294967295 / 5 000000 = 868s
  *
  * @example  
  *
  * @date     2019/6/10 ����һ
  */
void GPT_DelayMs(uint32_t ms);



/**
  * @brief    gpt ��ʱ����ȷ��ʱ
  *
  * @param    us �� ��ʱʱ�� us
  *
  * @return   
  *
  * @note     GPT ��32Ϊ������ �����ʱ4294967295 / 5 000000 = 868s
  *
  * @example  
  *
  * @date     2019/6/10 ����һ
  */
void GPT_DelayUs(uint32_t us);



/**
  * @brief    gpt ��ʱ����ʼ��ʱ
  *
  * @param   
  *
  * @return   
  *
  * @note     GPT ��32Ϊ������ �����ʱ4294967295 / 1 000000 = 4290s 
  *
  * @example  
  *
  * @date     2019/6/10 ����һ
  */
void GPT_TimeStart(void);



/**
  * @brief    gpt ��ʱ����ʼ��ʱ
  *
  * @param   
  *
  * @return   
  *
  * @note     GPT ��32Ϊ������ �����ʱ4294967295 / 1 000000 = 4290s 
  *
  * @example  
  *
  * @date     2019/6/10 ����һ
  */
uint32_t GPT_TimeGetUs(void);













#endif  
