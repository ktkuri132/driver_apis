#ifndef __INCLUDE_H
#define __INCLUDE_H


#define EnableInterrupts  __asm(" CPSIE i");//¿ª×ÜÖÐ¶Ï
#define DisableInterrupts __asm(" CPSID i");//¹Ø×ÜÖÐ¶Ï


//#define LQMT9V034    //Ñ¡ÔñÊ¹ÓÃÉñÑÛÉãÏñÍ·Ä£¿é
#define LQOV7725   //Á½ÕßÑ¡ÆäÒ»

#ifdef LQMT9V034 //LQMT9V034Ä£¿é
#define CAMERA_FPS  100
#define APP_CAMERA_WIDTH  (IMAGEW)
#define APP_CAMERA_HEIGHT (IMAGEH/2)  //Ò»¸öuint16_tÀïÃæ×°ÁËÁ½¸öÏñËØ£¬ Ò»ÐÐIMAGEW ÀïÃæÆäÊµ×°ÁËÁ½ÐÐµÄÏñËØµã£¬ËùÒÔ¸ß¶È/2
#define APP_BPP 2 //ÏñËØ¸ñÊ½£¬
#define  Use_ROWS  120    //Ê¹ÓÃÍ¼Ïñ¸ß¶È
#define  Use_Line  160    //Ê¹ÓÃÍ¼Ïñ¿í¶È
#else            //LQOV7725Ä£¿é
#define LQOV7725RGB   //Ê¹ÓÃ7725RGB   
//#define LQOV7725YUV     //Ê¹ÓÃ7725»Ò¶È
#define CAMERA_FPS  100
#define APP_CAMERA_WIDTH  320
#define APP_CAMERA_HEIGHT 240

#define APP_BPP 2 //ÏñËØ¸ñÊ½
#define  Use_ROWS  120    //Ê¹ÓÃÍ¼Ïñ¸ß¶È
#define  Use_Line  160    //Ê¹ÓÃÍ¼Ïñ¿í¶È
#endif

/* ÕâÀïÖ÷Òª´æ·ÅÒ»Ð©È«¾Öºê¶¨Òå */

/* ÏÔÊ¾ÆÁÄ» */
//#define OLED
#define TFT1_8


/* MPU6050   Ö»ÄÜÊ¹ÓÃ Ó²¼þIIC ºÍÈí¼þIIC */
/* ICM20602  ÄÜÊ¹ÓÃ Ó²¼þIIC ºÍÈí¼þIIC  Ó²¼þSPI ºÍÈí¼þSPI */
/* ÁúÇñ¾ÅÖá  Ö»ÄÜÊ¹ÓÃ Ó²¼þIIC ºÍÈí¼þIIC */
/* ºê¶¨Òå Ê¹ÓÃ²»Í¬µÄ½ÓÏß·½Ê½ */
#define USE_HAL_SPI  0
#define USE_SOFT_SPI 1
#define USE_SOFT_IIC 2
#define USE_HAL_IIC  3

/* Ñ¡ÔñÊ¹ÓÃÓ²¼þIIC */
#define IMU_USE_SELECT  USE_HAL_IIC

/* VL53 Ê¹ÓÃÓ²¼þIIC */
#define VL53_USE_SELECT USE_HAL_IIC

/* ·¢ËÍÄäÃûÉÏÎ»»úÊ¹ÓÃ ´®¿Ú DMA´«Êä·½Ê½ */
#define ANO_DT_USE_DMA  1

#endif
