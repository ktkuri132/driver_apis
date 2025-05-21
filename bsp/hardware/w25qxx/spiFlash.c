
#include "spiFlash.h"
#include <sysport.h>
#include "f407.h"


/**
  * 函    数：W25Q64初始化
  * 参    数：无
  * 返 回 值：无
  */
// spi_init* W25Q64_Init(void)
// {
// 	spi.bsp_SPI_Init();			//初始化SPI引脚

// }

/**
  * 函    数：W25Q64读取ID号
  * 参    数：MID 工厂ID，使用输出参数的形式返回
  * 参    数：DID 设备ID，使用输出参数的形式返回
  * 返 回 值：无
  */
void W25Qxx_ReadID(spi_init *_spi,uint8_t *MID, uint16_t *DID)
{
	Soft_SPI_Start(_spi);								//SPI起始
	spi.bsp_SPI_SwapByte(_spi,W25Q64_JEDEC_ID);			//交换发送读取ID的指令
	*MID = spi.bsp_SPI_SwapByte(_spi,W25Q64_DUMMY_BYTE);	//交换接收MID，通过输出参数返回
	*DID = spi.bsp_SPI_SwapByte(_spi,W25Q64_DUMMY_BYTE);	//交换接收DID高8位
	*DID <<= 8;									//高8位移到高位
	*DID |= spi.bsp_SPI_SwapByte(_spi,W25Q64_DUMMY_BYTE);	//或上交换接收DID的低8位，通过输出参数返回
	Soft_SPI_Stop(_spi);								//SPI终止
}

/**
  * 函    数：W25Q64写使能
  * 参    数：无
  * 返 回 值：无
  */
void W25Qxx_WriteEnable(spi_init *_spi)
{
	Soft_SPI_Start(_spi);								//SPI起始
	spi.bsp_SPI_SwapByte(_spi,W25Q64_WRITE_ENABLE);		//交换发送写使能的指令
	Soft_SPI_Stop(_spi);								//SPI终止
}

/**
  * 函    数：W25Q64等待忙
  * 参    数：无
  * 返 回 值：无
  */
void W25Qxx_WaitBusy(spi_init *_spi)
{
	uint32_t Timeout;
	Soft_SPI_Start(_spi);								//SPI起始
	spi.bsp_SPI_SwapByte(_spi,W25Q64_READ_STATUS_REGISTER_1);				//交换发送读状态寄存器1的指令
	Timeout = 100000;							//给定超时计数时间
	while ((spi.bsp_SPI_SwapByte(_spi,W25Q64_DUMMY_BYTE) & 0x01) == 0x01)	//循环等待忙标志位
	{
		Timeout --;								//等待时，计数值自减
		if (Timeout == 0)						//自减到0后，等待超时
		{
			/*超时的错误处理代码，可以添加到此处*/
			break;								//跳出等待，不等了
		}
	}
	Soft_SPI_Stop(_spi);								//SPI终止
}

/**
  * 函    数：W25Q64页编程
  * 参    数：Address 页编程的起始地址，范围：0x000000~0x7FFFFF
  * 参    数：DataArray	用于写入数据的数组
  * 参    数：Count 要写入数据的数量，范围：0~256
  * 返 回 值：无
  * 注意事项：写入的地址范围不能跨页
  */
void W25Qxx_PageProgram(spi_init *_spi,uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
	uint16_t i;

	W25Qxx_WriteEnable(_spi);						//写使能

	Soft_SPI_Start(_spi);								//SPI起始
	spi.bsp_SPI_SwapByte(_spi,W25Q64_PAGE_PROGRAM);		//交换发送页编程的指令
	spi.bsp_SPI_SwapByte(_spi,Address >> 16);				//交换发送地址23~16位
	spi.bsp_SPI_SwapByte(_spi,Address >> 8);				//交换发送地址15~8位
	spi.bsp_SPI_SwapByte(_spi,Address);					//交换发送地址7~0位
	for (i = 0; i < Count; i ++)				//循环Count次
	{
		spi.bsp_SPI_SwapByte(_spi,DataArray[i]);			//依次在起始地址后写入数据
	}
	Soft_SPI_Stop(_spi);								//SPI终止

	W25Qxx_WaitBusy(_spi);							//等待忙
}

/**
  * 函    数：W25Q64扇区擦除（4KB）
  * 参    数：Address 指定扇区的地址，范围：0x000000~0x7FFFFF
  * 返 回 值：无
  */
void W25Qxx_SectorErase(spi_init *_spi,uint32_t Address)
{
	W25Qxx_WriteEnable(_spi);						//写使能

	Soft_SPI_Start(_spi);								//SPI起始
	spi.bsp_SPI_SwapByte(_spi,W25Q64_SECTOR_ERASE_4KB);	//交换发送扇区擦除的指令
	spi.bsp_SPI_SwapByte(_spi,Address >> 16);				//交换发送地址23~16位
	spi.bsp_SPI_SwapByte(_spi,Address >> 8);				//交换发送地址15~8位
	spi.bsp_SPI_SwapByte(_spi,Address);					//交换发送地址7~0位
	Soft_SPI_Stop(_spi);								//SPI终止

	W25Qxx_WaitBusy(_spi);							//等待忙
}

/**
  * 函    数：W25Q64读取数据
  * 参    数：Address 读取数据的起始地址，范围：0x000000~0x7FFFFF
  * 参    数：DataArray 用于接收读取数据的数组，通过输出参数返回
  * 参    数：Count 要读取数据的数量，范围：0~0x800000
  * 返 回 值：无
  */
void W25Qxx_ReadData(spi_init *_spi,uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint32_t i;
	Soft_SPI_Start(_spi);								//SPI起始
	spi.bsp_SPI_SwapByte(_spi,W25Q64_READ_DATA);			//交换发送读取数据的指令
	spi.bsp_SPI_SwapByte(_spi,Address >> 16);				//交换发送地址23~16位
	spi.bsp_SPI_SwapByte(_spi,Address >> 8);				//交换发送地址15~8位
	spi.bsp_SPI_SwapByte(_spi,Address);					//交换发送地址7~0位
	for (i = 0; i < Count; i ++)				//循环Count次
	{
		DataArray[i] = spi.bsp_SPI_SwapByte(_spi,W25Q64_DUMMY_BYTE);	//依次在起始地址后读取数据
	}
	Soft_SPI_Stop(_spi);								//SPI终止
}

