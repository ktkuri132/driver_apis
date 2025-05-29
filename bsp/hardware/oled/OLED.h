#ifndef __OLED_H_
#define __OLED_H_
#define __OLED_

#include "OLED_Data.h"
#include <stdarg.h>
#include <string.h>
// #define Peripheral_SPI      // 此处定义外设自带SPI
#define Peripheral_IIC   // 此处定义外设自带IIC

/* 片上IIC驱动头文件  */
// #include <hardi2c.h>     // 此处定义片上硬件IIC
// 此处定义片上软件IIC
#include <drivers/bsp_iic.h>
/*  片上SPI驱动头文件    */
// #include <softspi.h>     // 实测通用软件SPI不能用以OLED驱动

/* 定义1.3寸OLED地址及其寄存器  */
 //7位OLED地址  stm32上OLED的IIC地址为0x78

#define OLED_Data_Mode      0x40
#define OLED_Command_Mode   0x00

#ifdef On_Chip_IIC

#undef Peripheral_SPI
#undef Peripheral_IIC
/* 定义IIC端口  */
//#define OLED_I2C_PORT i2c1

#ifdef  __HARDI2C_
#define OLED_ADDRESS        0x78
/* 江科大OLED IIC操作接口   */
#define OLED_WriteCommand(Command) Hard_IIC_Send_Byte(OLED_ADDRESS,OLED_Command_Mode,Command)
#define OLED_WriteData(Data,Count) Hard_IIC_Wirter_Data(OLED_ADDRESS,OLED_Data_Mode,Data,Count)
#define OLED_GPIO_Init() Hard_IIC_Init()

#elif defined __SOFTI2C_
#define OLED_ADDRESS        0x78
#define OLED_WriteCommand(Command)  Soft_IIC_Write_Byte(OLED_ADDRESS,OLED_Command_Mode,Command)//Soft_IIC_WriteByte(OLED_ADDRESS,OLED_Command_Mode,Command)
#define OLED_WriteData(Data,Count)  Soft_IIC_Write_Len(OLED_ADDRESS,OLED_Data_Mode,Count,Data)//Soft_IIC_WriteData(OLED_ADDRESS,OLED_Data_Mode,Data,Count)
#define OLED_GPIO_Init()            Soft_IIC_Init()//Soft_IIC_Init()

#endif


#elif defined On_Chip_SPI
#undef Peripheral_SPI
#undef Peripheral_IIC

#define OLED_CS SOFT_SPI_CS_PIN


#endif
/*FontSize参数取值*/
/*此参数值不仅用于判断，而且用于计算横向字符偏移，默认值为字体像素宽度*/
#define OLED_8X16				8
#define OLED_6X8				6

/*IsFilled参数数值*/
#define OLED_UNFILLED			0
#define OLED_FILLED				1


/*初始化函数*/
void OLED_Init(void);

/*更新函数*/
void OLED_Update(void);
void OLED_UpdateArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/*显存控制函数*/
void OLED_Clear(void);
void OLED_ClearArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);
void OLED_Reverse(void);
void OLED_ReverseArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/*显示函数*/
void OLED_ShowChar(int16_t X, int16_t Y, char Char, uint8_t FontSize);
void OLED_ShowString(int16_t X, int16_t Y, char *String, uint8_t FontSize);
void OLED_ShowNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowSignedNum(int16_t X, int16_t Y, int32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowHexNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowBinNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowFloatNum(int16_t X, int16_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize);
void OLED_ShowImage(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image);
void OLED_Printf(int16_t X, int16_t Y, uint8_t FontSize, char *format, ...);

/*绘图函数*/
void OLED_DrawPoint(int16_t X, int16_t Y);
uint8_t OLED_GetPoint(int16_t X, int16_t Y);
void OLED_DrawLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1);
void OLED_DrawRectangle(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled);
void OLED_DrawTriangle(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint8_t IsFilled);
void OLED_DrawCircle(int16_t X, int16_t Y, uint8_t Radius, uint8_t IsFilled);
void OLED_DrawEllipse(int16_t X, int16_t Y, uint8_t A, uint8_t B, uint8_t IsFilled);
void OLED_DrawArc(int16_t X, int16_t Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled);

#endif