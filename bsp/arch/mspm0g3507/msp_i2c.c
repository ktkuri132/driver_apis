/*
    MSPM0 上的IIC通讯,分为硬件IIC和软件IIC两种方式。
    通过宏定义开关
*/
#include "msp_i2c.h"
#include <sysport.h>
#ifdef __SOFT_I2C__



#endif

#ifdef __HARD_I2C__
#include <ti/driverlib/dl_i2c.h>




#endif