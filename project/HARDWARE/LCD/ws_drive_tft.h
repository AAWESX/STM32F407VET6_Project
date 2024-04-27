
#ifndef __LCD9486_DRV_H__
#define __LCD9486_DRV_H__

#include "main.h"
#include "fsmc.h"


//      常用颜色
#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F  
#define BRED        0XF81F
#define GRED 			 	0XFFE0
#define GBLUE			 	0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 			0XBC40 //棕色
#define BRRED 			0XFC07 //棕红色
#define GRAY  			0X8430 //灰色
//GUI颜色


#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	0X841F //浅绿色
#define LGRAY 			 		0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE      	0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE          0X2B12 //浅棕蓝色(选择条目的反色)



#define LCD_DIR_MODE	       0           //0，竖屏；1，横屏。	


#define LCD9486_BL_CLOSE	    ->ODR &= ~LCD_BL_Pin  ; 
#define LCD9486_BL_OPEN      LCD_BL_GPIO_Port->ODR |=  LCD_BL_Pin  ;


#if LCD_DIR_MODE == 0
	  #define TFT_X_MAX	240
	  #define TFT_Y_MAX	320
#else
		#define TFT_X_MAX	320
		#define TFT_Y_MAX	240
#endif



void WS_TFT_Init(void);
void WS_TFT_Set_Windows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd);



void WS_TFT_Dis_5X7_String   (int x,int y,  char     *string , uint16_t backColor  ,uint16_t forColor) ;
void WS_TFT_Dis_8X16_String  (int x,int y,  char     *string , uint16_t backColor  ,uint16_t forColor)  ;
void WS_TFT_Dis_12X24_String (int x,int y,  char     *string , uint16_t backColor  ,uint16_t forColor);
void WS_TFT_Dis_16X32_String (int x,int y,  char     *string , uint16_t backColor  ,uint16_t forColor)   ;



void WS_TFT_Clear(uint16_t color);
void WS_TFT_Draw_Point       (uint16_t x,uint16_t y,uint16_t color) ;
void WS_TFT_Draw_Rectangle(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd,uint16_t color);


 
void WS_TFT_Dis_16X16_Date (int x, int y, uint8_t  *buffer , uint16_t n, uint16_t backColor  ,uint16_t forColor);
void WS_TFT_Dis_24X24_Date (int x, int y, uint8_t  *buffer , uint16_t n, uint16_t backColor  ,uint16_t forColor);
void WS_TFT_Dis_32X32_Date (int x, int y, uint8_t  *buffer , uint16_t n, uint16_t backColor  ,uint16_t forColor);

//    在TFT上打印 “8X16” 调试信息
void  WS_TFT_Dis_Debug(int x,int y, uint16_t forColor , char *p,...);

////   由ADC值转换位位置坐标
////   返回值高16位存X值，低16位存y值
//uint32_t WS_TFT_Touth_Get_Point(SPI_HandleTypeDef *hspi);





#endif

