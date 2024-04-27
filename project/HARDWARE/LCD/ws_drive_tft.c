#include "ws_drive_tft.h"
#include "drive_dzcode.h"
#include <stdarg.h>		  
#include <stdio.h>
#include "string.h"

      

__IO uint16_t *FSMC_TFT_Dat_Addr;       //  写数据地址
__IO uint16_t *FSMC_TFT_Cmd_Addr;       //  写命令地址


#define WS_TFT_Write_Cmd(d)	      (*FSMC_TFT_Cmd_Addr) = (d)		
#define WS_TFT_Write_Dat(d)	      (*FSMC_TFT_Dat_Addr) = (d)		
#define WS_TFT_Write_Cmd_Dat(r,d) (*FSMC_TFT_Cmd_Addr) = (r),(*FSMC_TFT_Dat_Addr) = (d)



//   设置TFT对应的内存地址
void WS_TFT_FSMC_Addr_Init(void)
{                               // 16bit位宽 ， LCD_RS->A23(NE1) , 对应第24个地址位    
	  FSMC_TFT_Dat_Addr = (uint16_t *) (((uint32_t)(0x60000000UL | 1<<24)));
	  FSMC_TFT_Cmd_Addr = (uint16_t *) (((uint32_t)(0x60000000UL | 1<<24))-2);
}


//LCD重要参数集
typedef struct  
{										    
	uint16_t width;			  //LCD 宽度
	uint16_t height;			//LCD 高度
	uint16_t id;				  //LCD ID
	uint8_t  dir;			    //横屏还是竖屏控制：0，竖屏；1，横屏。	
	uint16_t	wramcmd;		//开始写gram指令
	uint16_t  setxcmd;		//设置x坐标指令
	uint16_t  setycmd;		//设置y坐标指令	 
}_lcd_dev; 	

_lcd_dev lcddev;

#define USE_HORIZONTAL LCD_DIR_MODE   	//	0//1

void WS_TFT_Set_Param(void)
{ 
	lcddev.setxcmd=0x2A;
	lcddev.setycmd=0x2B;
	lcddev.wramcmd=0x2C;
#if USE_HORIZONTAL==1	//使用横屏	  
	lcddev.dir=1;//横屏
	lcddev.width=TFT_X_MAX;
	lcddev.height=TFT_Y_MAX;			
	WS_TFT_Write_Cmd_Dat(0x36,(1<<3)|(1<<7)|(1<<5));//BGR==1,MY==1,MX==0,MV==1
#else//竖屏
	lcddev.dir=0;//竖屏				 	 		
	lcddev.width =TFT_X_MAX;
	lcddev.height=TFT_Y_MAX;	
	WS_TFT_Write_Cmd_Dat(0x36,(1<<3)|(0<<6)|(0<<7));//BGR==1,MY==0,MX==0,MV==0
#endif
}



//   按照指定颜色清屏
void WS_TFT_Clear(uint16_t color)
{	
	uint32_t index=0;    
	WS_TFT_Set_Windows(0,0,lcddev.width-1,lcddev.height-1);	
	for(index=0;index<TFT_X_MAX * TFT_Y_MAX ;index++)
	{
		WS_TFT_Write_Dat(color);		  
	}
} 

const  unsigned char   test_16x16_code[] = 
{
0x00,0x00,0x1F,0xF0,0x10,0x10,0x10,0x10,0x1F,0xF0,0x10,0x10,0x10,0x10,0x1F,0xF0,0x04,0x40,0x44,0x44,0x24,0x44,0x14,0x48,0x14,0x50,0x04,0x40,0xFF,0xFE,0x00,0x00,/*"显",0*/
0x00,0x00,0x3F,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFE,0x01,0x00,0x01,0x00,0x11,0x10,0x11,0x08,0x21,0x04,0x41,0x02,0x81,0x02,0x05,0x00,0x02,0x00,/*"示",1*/
0x00,0x04,0x27,0xC4,0x14,0x44,0x14,0x54,0x85,0x54,0x45,0x54,0x45,0x54,0x15,0x54,0x15,0x54,0x25,0x54,0xE5,0x54,0x21,0x04,0x22,0x84,0x22,0x44,0x24,0x14,0x08,0x08,/*"测",2*/
0x00,0x28,0x20,0x24,0x10,0x24,0x10,0x20,0x07,0xFE,0x00,0x20,0xF0,0x20,0x17,0xE0,0x11,0x20,0x11,0x10,0x11,0x10,0x15,0x10,0x19,0xCA,0x17,0x0A,0x02,0x06,0x00,0x02/*"试",3*/
};

const  unsigned char   test_12x24_code[] = 
{
0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xFF,0xE0,0x03,0x00,0x70,0x03,0x00,0x60,0x03,0x00,0x60,0x03,0xFF,0xE0,0x03,0x00,0x60,0x03,0x00,0x60,0x03,0x00,0x60,0x03,0xFF,0xE0,0x03,0xC0,0x60,0x00,0x73,0x00,0x00,0x63,0x1C,0x18,0x63,0x18,0x0E,0x63,0x30,0x07,0x63,0x60,0x07,0x63,0xC0,0x03,0x63,0x80,0x00,0x63,0x0C,0x3F,0xFF,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"显",0*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x0F,0xFF,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x7F,0xFF,0xFE,0x00,0x1C,0x00,0x00,0x1C,0x00,0x01,0x9D,0x80,0x03,0x9C,0xC0,0x03,0x9C,0x60,0x07,0x1C,0x30,0x0E,0x1C,0x38,0x0C,0x1C,0x1C,0x18,0x1C,0x0E,0x30,0x1C,0x0C,0x61,0xFC,0x00,0x00,0x78,0x00,0x00,0x30,0x00,0x00,0x00,0x00,/*"示",1*/
0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x0C,0x0D,0xFF,0x0C,0x0F,0xC7,0x0C,0x01,0xC7,0x0C,0x01,0xFF,0x7C,0x61,0xFF,0x6C,0x3B,0xFF,0x6C,0x1B,0xFF,0x6C,0x1B,0xFF,0x6C,0x07,0xFF,0x6C,0x07,0xFF,0x6C,0x07,0xFF,0x6C,0x0D,0xF7,0x6C,0x0D,0xF7,0x6C,0x3D,0xF6,0x6C,0x1D,0xF0,0x0C,0x1C,0x7C,0x0C,0x1C,0xE6,0x0C,0x1C,0xC7,0x0C,0x1F,0x83,0x3C,0x06,0x00,0x1C,0x00,0x00,0x00,/*"测",2*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x18,0x01,0xF8,0x0C,0x00,0xDC,0x0E,0x00,0xCC,0x06,0x00,0xC6,0x06,0xFF,0xFF,0x00,0x00,0xC0,0x00,0x00,0xC0,0x7F,0x03,0xC0,0x06,0xFF,0xC0,0x06,0x0C,0xC0,0x06,0x0C,0xE0,0x06,0x0C,0x60,0x06,0x0C,0x60,0x06,0x6C,0x70,0x06,0xCD,0xB3,0x07,0x8F,0x3B,0x07,0x78,0x1B,0x0F,0x60,0x1E,0x06,0x00,0x0F,0x00,0x00,0x03,0x00,0x00,0x00 /*"试",3*/
};

const  unsigned char   test_32x32_code[] = 
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x01,0x80,0x00,0xFF,0xFF,0xC0,0x00,0xC0,0x01,0xC0,0x00,0xC0,0x01,0xC0,0x00,0xC0,0x01,0xC0,0x00,0xC0,0x01,0xC0,0x00,0xFF,0xFF,0xC0,0x00,0xC0,0x01,0xC0,0x00,0xC0,0x01,0xC0,0x00,0xC0,0x01,0xC0,0x00,0xC0,0x01,0xC0,0x00,0xFF,0xFF,0xC0,0x00,0xC0,0x01,0xC0,0x01,0x80,0x00,0x00,0x00,0x0E,0x38,0x00,0x00,0x0E,0x38,0x30,0x0C,0x0E,0x38,0x78,0x07,0x0E,0x38,0x70,0x07,0x8E,0x38,0xE0,0x03,0xCE,0x39,0xC0,0x01,0xCE,0x3B,0x80,0x01,0xCE,0x3B,0x00,0x00,0xCE,0x3E,0x00,0x00,0x0E,0x3C,0x00,0x00,0x0E,0x38,0x38,0x3F,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"显",0*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0xE0,0x03,0xFF,0xFF,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x3F,0xFF,0xFF,0xFE,0x00,0x01,0xC0,0x00,0x00,0x01,0xC0,0x00,0x00,0x01,0xC0,0x00,0x00,0x71,0xCC,0x00,0x00,0xF9,0xC6,0x00,0x00,0xE1,0xC3,0x80,0x01,0xE1,0xC1,0xC0,0x01,0xC1,0xC0,0xE0,0x03,0x81,0xC0,0xF0,0x07,0x01,0xC0,0x78,0x0E,0x01,0xC0,0x3C,0x0C,0x01,0xC0,0x1C,0x18,0x01,0xC0,0x18,0x30,0x71,0xC0,0x00,0x00,0x1F,0xC0,0x00,0x00,0x07,0x80,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,/*"示",1*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x18,0x0E,0x30,0x00,0x1C,0x07,0x3F,0xF8,0x1C,0x03,0xB8,0x38,0x1C,0x03,0xB8,0x38,0x1C,0x00,0x38,0x3B,0x9C,0x00,0x3B,0x3B,0xDC,0x00,0x3B,0xFB,0x9C,0x38,0x3B,0xBB,0x9C,0x1C,0xFB,0xBB,0x9C,0x1E,0xFB,0xBB,0x9C,0x0E,0xFB,0xBB,0x9C,0x0D,0xBB,0xBB,0x9C,0x01,0xBB,0xBB,0x9C,0x01,0xBB,0x3B,0x9C,0x01,0xBB,0x3B,0x9C,0x03,0x3B,0x3B,0x9C,0x03,0x3B,0x3B,0x9C,0x07,0x3B,0x3B,0x9C,0x7F,0x3F,0x33,0x9C,0x1E,0x37,0x03,0x9C,0x0E,0x07,0x83,0x9C,0x0E,0x0E,0xC0,0x1C,0x0E,0x0C,0x60,0x1C,0x0E,0x1C,0x78,0x1C,0x0E,0x38,0x38,0x1C,0x0E,0x70,0x39,0xF8,0x00,0xC0,0x18,0x78,0x01,0x80,0x00,0x30,0x00,0x00,0x00,0x00,/*"测",2*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x07,0xE0,0x0E,0x00,0x07,0x78,0x07,0x00,0x07,0x38,0x03,0x80,0x07,0x18,0x03,0x80,0x07,0x1C,0x03,0x80,0x07,0x0C,0x00,0x3F,0xFF,0xFE,0x00,0x00,0x07,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x07,0x00,0x01,0x80,0x07,0x00,0x7F,0xC0,0x1F,0x00,0x03,0x9F,0xFF,0x00,0x03,0x81,0xC3,0x00,0x03,0x81,0xC3,0x80,0x03,0x81,0xC3,0x80,0x03,0x81,0xC3,0x80,0x03,0x81,0xC1,0x80,0x03,0x81,0xC1,0xC0,0x03,0x8D,0xC1,0xC0,0x03,0x99,0xCE,0xE6,0x03,0xB1,0xF8,0xE6,0x03,0xE3,0xE0,0x76,0x03,0xFF,0x00,0x7E,0x03,0xCC,0x00,0x3E,0x03,0xC0,0x00,0x1E,0x01,0x80,0x00,0x0E,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00/*"试",3*/
	
};


//   初始化 TFT
void WS_TFT_Init(void) 
{  										 
	WS_TFT_FSMC_Addr_Init();
//*************   2.8inch ILI9341   **********//	
	WS_TFT_Write_Cmd(0xCF);  
	WS_TFT_Write_Dat(0x00); 
	WS_TFT_Write_Dat(0xC9); //C1 
	WS_TFT_Write_Dat(0X30); 
	WS_TFT_Write_Cmd(0xED) ;  
	WS_TFT_Write_Dat(0x64); 
	WS_TFT_Write_Dat(0x03); 
	WS_TFT_Write_Dat(0X12); 
	WS_TFT_Write_Dat(0X81); 
	WS_TFT_Write_Cmd(0xE8);  
	WS_TFT_Write_Dat(0x85); 
	WS_TFT_Write_Dat(0x10); 
	WS_TFT_Write_Dat(0x7A); 
	WS_TFT_Write_Cmd(0xCB);  
	WS_TFT_Write_Dat(0x39); 
	WS_TFT_Write_Dat(0x2C); 
	WS_TFT_Write_Dat(0x00); 
	WS_TFT_Write_Dat(0x34); 
	WS_TFT_Write_Dat(0x02); 
	WS_TFT_Write_Cmd(0xF7);  
	WS_TFT_Write_Dat(0x20); 
	WS_TFT_Write_Cmd(0xEA);  
	WS_TFT_Write_Dat(0x00); 
	WS_TFT_Write_Dat(0x00); 
	WS_TFT_Write_Cmd(0xC0);    //Power control 
	WS_TFT_Write_Dat(0x1B);   //VRH[5:0] 
	WS_TFT_Write_Cmd(0xC1);    //Power control 
	WS_TFT_Write_Dat(0x00);   //SAP[2:0];BT[3:0] 01 
	WS_TFT_Write_Cmd(0xC5);    //VCM control 
	WS_TFT_Write_Dat(0x30); 	 //3F
	WS_TFT_Write_Dat(0x30); 	 //3C
	WS_TFT_Write_Cmd(0xC7);    //VCM control2 
	WS_TFT_Write_Dat(0XB7); 
	WS_TFT_Write_Cmd(0x36);    // Memory Access Control 
	WS_TFT_Write_Dat(0x08); 
	WS_TFT_Write_Cmd(0x3A);   
	WS_TFT_Write_Dat(0x55); 
	WS_TFT_Write_Cmd(0xB1);   
	WS_TFT_Write_Dat(0x00);   
	WS_TFT_Write_Dat(0x1A); 
	WS_TFT_Write_Cmd(0xB6);    // Display Function Control 
	WS_TFT_Write_Dat(0x0A); 
	WS_TFT_Write_Dat(0xA2); 
	WS_TFT_Write_Cmd(0xF2);    // 3Gamma Function Disable 
	WS_TFT_Write_Dat(0x00); 
	WS_TFT_Write_Cmd(0x26);    //Gamma curve selected 
	WS_TFT_Write_Dat(0x01); 
	WS_TFT_Write_Cmd(0xE0);     //Set Gamma 
	WS_TFT_Write_Dat(0x0F); 
	WS_TFT_Write_Dat(0x2A); 
	WS_TFT_Write_Dat(0x28); 
	WS_TFT_Write_Dat(0x08); 
	WS_TFT_Write_Dat(0x0E); 
	WS_TFT_Write_Dat(0x08); 
	WS_TFT_Write_Dat(0x54); 
	WS_TFT_Write_Dat(0XA9); 
	WS_TFT_Write_Dat(0x43); 
	WS_TFT_Write_Dat(0x0A); 
	WS_TFT_Write_Dat(0x0F); 
	WS_TFT_Write_Dat(0x00); 
	WS_TFT_Write_Dat(0x00); 
	WS_TFT_Write_Dat(0x00); 
	WS_TFT_Write_Dat(0x00); 		 
	WS_TFT_Write_Cmd(0XE1);    //Set Gamma 
	WS_TFT_Write_Dat(0x00); 
	WS_TFT_Write_Dat(0x15); 
	WS_TFT_Write_Dat(0x17); 
	WS_TFT_Write_Dat(0x07); 
	WS_TFT_Write_Dat(0x11); 
	WS_TFT_Write_Dat(0x06); 
	WS_TFT_Write_Dat(0x2B); 
	WS_TFT_Write_Dat(0x56); 
	WS_TFT_Write_Dat(0x3C); 
	WS_TFT_Write_Dat(0x05); 
	WS_TFT_Write_Dat(0x10); 
	WS_TFT_Write_Dat(0x0F); 
	WS_TFT_Write_Dat(0x3F); 
	WS_TFT_Write_Dat(0x3F); 
	WS_TFT_Write_Dat(0x0F); 
	WS_TFT_Write_Cmd(0x2B); 
	WS_TFT_Write_Dat(0x00);
	WS_TFT_Write_Dat(0x00);
	WS_TFT_Write_Dat(0x01);
	WS_TFT_Write_Dat(0x3f);
	WS_TFT_Write_Cmd(0x2A); 
	WS_TFT_Write_Dat(0x00);
	WS_TFT_Write_Dat(0x00);
	WS_TFT_Write_Dat(0x00);
	WS_TFT_Write_Dat(0xef);	 
	WS_TFT_Write_Cmd(0x11);  //Exit Sleep
	HAL_Delay(120);
	WS_TFT_Write_Cmd(0x29);  //display on	

  
	WS_TFT_Set_Param();// 

	//   打开背光 
	LCD9486_BL_OPEN   ;
	
	
	//   全屏幕刷色测试 ，R,G,B 
	WS_TFT_Clear(RED)    ;
	HAL_Delay(500)       ;
	WS_TFT_Clear(GREEN)  ;
	HAL_Delay(500)       ;
	WS_TFT_Clear(BLUE)   ;
	HAL_Delay(500)       ;
	WS_TFT_Clear(WHITE)  ;
	
//	//   字符显示测试
//	WS_TFT_Dis_8X16_String (0,0,  "2020/11/14 09:08:20 ",WHITE, RED);
//	WS_TFT_Dis_5X7_String  (160,0,  "TFT 5x7 ASCII",WHITE, RED);
//	WS_TFT_Dis_5X7_String  (160,8,  "TFT 5x7 ASCII",BLACK, WHITE);
//	WS_TFT_Dis_8X16_String (0,16,"16x16 font:字库测试",WHITE,GREEN);	
//	WS_TFT_Dis_12X24_String(0,32,"24x24 font:字库测试",WHITE,BLUE);
//	WS_TFT_Dis_16X32_String(0,54,"32x32 font:字库测试",WHITE,BROWN);
	
//	 32_code,4,BLACK,RED);
}


//   选择需要更新的区域
void WS_TFT_Set_Windows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
{	
	WS_TFT_Write_Cmd(lcddev.setxcmd);	
	WS_TFT_Write_Dat(xStar>>8);
	WS_TFT_Write_Dat(0x00FF&xStar);		
	WS_TFT_Write_Dat(xEnd>>8);
	WS_TFT_Write_Dat(0x00FF&xEnd);

	WS_TFT_Write_Cmd(lcddev.setycmd);	
	WS_TFT_Write_Dat(yStar>>8);
	WS_TFT_Write_Dat(0x00FF&yStar);		
	WS_TFT_Write_Dat(yEnd>>8);
	WS_TFT_Write_Dat(0x00FF&yEnd);

	//   开始跟新选择区域内的GRAM
	WS_TFT_Write_Cmd(lcddev.wramcmd);	
}  


//   画点
void WS_TFT_Draw_Point(uint16_t x,uint16_t y,uint16_t color)
{
	//  设置区域
	WS_TFT_Set_Windows(x,y,x+1,y+1);
	//   写入数据
	WS_TFT_Write_Dat(color)         ; 
}

//   画实心矩形
void WS_TFT_Draw_Rectangle(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd,uint16_t color)
{
	uint16_t       i = 0;
	//  设置区域
	WS_TFT_Set_Windows(xStar,yStar,xEnd,yEnd);
	//   写入数据
	for(;yStar < yEnd ; yStar ++   )
	{
	  for(i = xStar; i <= xEnd; i ++ )
	     WS_TFT_Write_Dat(color)    ;  
	}
}


//   刷16*16点区域
void WS_TFT_Dis_16X16_Date (int x, int y, uint8_t * buffer , uint16_t n, uint16_t backColor  ,uint16_t forColor)
{
	     int i , j;
	
	     uint16_t   dat = 0;
	     while(n -- > 0)
			 {
				 WS_TFT_Set_Windows(x,y,x+15,y+15);	
				 for(i=0;i<32;i+=2)
				 {
						dat   = buffer[i] <<8 ;
						dat  |= buffer[i+1] ;
						for(j=15;j>=0;j--)
						{
								if((dat & 1<<j)  != 0 )
								{
										WS_TFT_Write_Dat(forColor);						
								}
								else
								{
										WS_TFT_Write_Dat(backColor); 
								} 
						}
						
				 } 
				 x += 16;
				 buffer += 32;
         if(x  >  TFT_X_MAX - 16) return; 		 
			 }
}

//   刷24*24点区域
void WS_TFT_Dis_24X24_Date (int x, int y, uint8_t  *buffer ,uint16_t n, uint16_t backColor  ,uint16_t forColor)
{
	     int i , j;
	     uint8_t   dat = 0;
	     while(n -- > 0)
			 {
				 WS_TFT_Set_Windows(x,y,x+23,y+23);	
				 for(i=0;i<72;i++)
				 {
						dat   = buffer[i]  ;
						for(j=7;j>=0;j--)
						{
								if((dat & 1<<j)  != 0 )
								{
										WS_TFT_Write_Dat(forColor);						
								}
								else
								{
										WS_TFT_Write_Dat(backColor); 
								} 
						}	
				 }
				 x += 24;
				 buffer += 72;
         if(x  >  TFT_X_MAX - 24) return; 		 
			 }				 
}

//   刷32*32点区域
void WS_TFT_Dis_32X32_Date (int x, int y, uint8_t  *buffer ,uint16_t n, uint16_t backColor  ,uint16_t forColor)
{
	     int i , j;
	     uint8_t   dat = 0;
	     while(n -- > 0)
			 {
				 WS_TFT_Set_Windows(x,y,x+31,y+31);	
				 for(i=0;i<128;i++)
				 {
						dat   = buffer[i]  ;
						for(j=7;j>=0;j--)
						{
								if((dat & 1<<j)  != 0 )
								{
										WS_TFT_Write_Dat(forColor);						
								}
								else
								{
										WS_TFT_Write_Dat(backColor); 
								} 
						}	
				 }
				 x += 32;
				 buffer += 128;
         if(x  >  TFT_X_MAX - 32) return; 		 
			 }				 
}


//   显示5X7字符串
void WS_TFT_Dis_5X7_String (int x,int y,char  *string , uint16_t backColor  ,uint16_t forColor)
{ 
	int  i , j ;
	uint8_t len=strlen((char *)string);
	uint8_t idex=0 , *p_dat , dat ;
	for(idex=0;idex<len;idex++)
	{
		if((uint8_t)string[idex]<0x80)  //   判断 ASCII 、汉字
		{
			 WS_TFT_Set_Windows(x,y,x+5,y+7);	
       p_dat = (uint8_t *)(string8X8Code + (string[idex] - 0x20)*8);	
			 for(i=0;i<8;i++)
			 {
					dat = p_dat[i]  ;
					for(j=7;j>=2;j--)
					{
							if((dat & (1<<j))  != 0 )
							{
							    WS_TFT_Write_Dat(forColor);						
							}
							else
							{
							    WS_TFT_Write_Dat(backColor); 
							} 
					}
			 }
      		
		}
	  else  //  
		{
		   idex ++;   
		}
    x +=  6;	
		if(x  >=  TFT_X_MAX - 6) return;    //
	}
}

//   显示8X16字符串
void WS_TFT_Dis_8X16_String (int x,int y, char  *string , uint16_t backColor  ,uint16_t forColor)
{ 
	uint16_t i , j ;
	uint8_t len=strlen((char *)string);
	uint8_t idex=0 , *p_dat , dat , buffer[32] ;
	
	for(idex=0;idex<len;)
	{
		if((uint8_t)string[idex]<0x80)  // 判断 ASCII 、汉字
		{
			 WS_TFT_Set_Windows(x,y,x+7,y+15);	
       p_dat = (uint8_t *)(string8X16Code + (string[idex] - 0x20)*16);	
			 for(i=0;i<16;i++)
			 {
					dat = p_dat[i]  ;
					for(j=0;j<8;j++)
					{
							if((dat & 1<<(7-j))  != 0 )
							{
							    WS_TFT_Write_Dat(forColor);          						
							}
							else
							{
							    WS_TFT_Write_Dat(backColor); 
							} 
					}
			 }
			 x +=  8;
       idex ++;			 
		}
	  else  // 
		{
			#if WS_Peripheral_W25Qxx
					if(WS_W25Qxx_Font16_Read_Date(buffer,(uint8_t *)&string[idex]) == WS_Status_OK)
					{
					    WS_TFT_Dis_16X16_Date(x,y,buffer,1,backColor,forColor);
						  x +=  16;
					}						
			#endif			
		  idex += 2; 						
		}
		if(x  >  TFT_X_MAX - 8) return;      
	}
}

//   显示12X24字符串
void WS_TFT_Dis_12X24_String (int x,int y, char  *string , uint16_t backColor  ,uint16_t forColor)
{ 
	uint16_t i , j, dat ;
	uint8_t len=strlen((char *)string);
	uint8_t idex=0 , *p_dat ,  buffer[72] ;

	for(idex=0;idex<len;)
	{
		if((uint8_t)string[idex]<0x80)  //   判断 ASCII 、汉字
		{
			 WS_TFT_Set_Windows(x,y,x+11,y+23);	
       p_dat = (uint8_t *)(ascii12X24Code + (string[idex] - 0x20)*48);	
			 for(i=0;i<48;i+=2)
			 {
					dat  = p_dat[i]<<8  ;
				  dat |= p_dat[i+1]   ;
					for(j=15;j>=4;j--)
					{
							if((dat & 1<<j)  != 0 )
							{
							    WS_TFT_Write_Dat(forColor);          						
							}
							else
							{
							    WS_TFT_Write_Dat(backColor); 
							} 
					}
			 }
			 x +=  12;
       idex ++;			 
		}
	  else  //  
		{
			#if WS_Peripheral_W25Qxx
					if(WS_W25Qxx_Font24_Read_Date(buffer,(uint8_t *)&string[idex]) == WS_Status_OK)
					{
					    WS_TFT_Dis_24X24_Date(x,y,buffer,backColor	,forColor);
						  x +=  24;
					}						
			#endif			
		  idex += 2; 						
		}
		if(x  >  TFT_X_MAX - 12) return;      //???????????????????????		
	}
}

//   显示16X32字符串
void WS_TFT_Dis_16X32_String (int x,int y, char  *string , uint16_t backColor  ,uint16_t forColor)
{ 
  int	i , j ;
	uint16_t  dat ;
	uint8_t len=strlen((char *)string);
	uint8_t idex=0 , *p_dat ,  buffer[128] ;

	for(idex=0;idex<len;)
	{
		if((uint8_t)string[idex]<0x80)  //   判断 ASCII 、汉字
		{
			 WS_TFT_Set_Windows(x,y,x+15,y+31);	
       p_dat = (uint8_t *)(ascii16X32Code + (string[idex] - 0x20)*64);	
			 for(i=0;i<64;i+=2)
			 {
					dat  = p_dat[i]<<8  ;
				  dat |= p_dat[i+1]   ;
					for(j=15;j>=0;j--)
					{
							if((dat & 1<<j)  != 0 )
							{
							    WS_TFT_Write_Dat(forColor);          						
							}
							else
							{
							    WS_TFT_Write_Dat(backColor); 
							} 
					}
			 }
			 x +=  16;
       idex ++;			 
		}
	  else  // 
		{
			#if WS_Peripheral_W25Qxx
					if(WS_W25Qxx_Font32_Read_Date(buffer,(uint8_t *)&string[idex]) == WS_Status_OK)
					{
					    WS_TFT_Dis_32X32_Date(x,y,buffer,backColor	,forColor);
						  x +=  32;
					}						
			#endif			
		  idex += 2; 						
		}
		if(x  >  TFT_X_MAX - 12) return;      //???????????????????????		
	}
}


//    在TFT上打印 “8X16” 调试信息
void  WS_TFT_Dis_Debug(int x,int y, uint16_t forColor , char *p,...)
{
	  char formatBuf[48] ;
    va_list ap;
		va_start(ap,p);
		vsprintf(formatBuf,p,ap);
		va_end(ap);
	  WS_TFT_Dis_8X16_String(x,y,formatBuf,WHITE,forColor);
}


////   计算 X、Y轴 一元一次函数的k、b
////   实测ADC值
//#define TOUTH_X_LITF_ADC  3875  
//#define TOUTH_X_RIGHT_ADC 215
//#define TOUTH_Y_UP_ADC    3865  
//#define TOUTH_Y_DOWN_ADC  400

////  k = (y2-y1)/(x2-x1)
////  b = 
//#define touchX_k   (float)(240-10) / (TOUTH_X_RIGHT_ADC -  TOUTH_X_LITF_ADC )
//#define touchX_b   (5*TOUTH_X_RIGHT_ADC - 235*TOUTH_X_LITF_ADC) / (TOUTH_X_RIGHT_ADC -  TOUTH_X_LITF_ADC)

//#define touchY_k   (float)(320-10) / (TOUTH_Y_DOWN_ADC  - TOUTH_Y_UP_ADC    )
//#define touchY_b   ((5*TOUTH_Y_DOWN_ADC - 315*TOUTH_Y_UP_ADC) / (TOUTH_Y_DOWN_ADC -  TOUTH_Y_UP_ADC))



////  读取XPT2046  X、Y数据
////  hspi: spi外设句柄
////  cmd : 读取控制字节， 0xd0读x轴，0x90读y轴。
//uint16_t  WS_TFT_Touth_Read_XPT2046(SPI_HandleTypeDef *hspi,uint16_t  cmd)
//{
//	 uint16_t num = 0;
//	
////  以片选定义为条件控制编译， 防止在没有定义下编译报错。
//#if defined( 	LCD_T_CS_GPIO_Port )  
//   uint8_t spi_r[2] , spi_t ;
//	
//	 //  片选使能
//	 HAL_GPIO_WritePin(LCD_T_CS_GPIO_Port,LCD_T_CS_Pin,GPIO_PIN_RESET);
//	
//	 spi_t = cmd;  // 写命令
//	 HAL_SPI_TransmitReceive(hspi,&spi_t,spi_r,1,1);
//	
//	 //  延时等待转换
//	 num = 50;
//	 while(num --);
//	 spi_t = 0 ;
//	
//	 //  读取16位数据
//	 HAL_SPI_TransmitReceive(hspi,&spi_t,spi_r,2,1);
//	
//	 //  数据转换 ， 保留中间12位
//	 num  = (spi_r[0] << 8) | spi_r[1];
//	 num &= ~(1<<15);
//	 num >>= 3;
//	
//	 //  片选失能
//	 HAL_GPIO_WritePin(LCD_T_CS_GPIO_Port,LCD_T_CS_Pin,GPIO_PIN_SET);
//#endif

//	 return num;
//}


////  排序函数
//void sort(uint16_t  arr[], int size)
//{
//	int j,i,tem;
//	for (i = 0; i < size-1;i ++)//size-1是因为不用与自己比较，所以比的数就少一个
//	{
//		int count = 0;
//		for (j = 0; j < size-1 - i; j++)	//size-1-i是因为每一趟就会少一个数比较
//		{
//			if (arr[j] > arr[j+1])//这是升序排法，前一个数和后一个数比较，如果前数大则与后一个数换位置
//			{
//				tem = arr[j];
//				arr[j] = arr[j+1];
//				arr[j+1] = tem;
//				count = 1;
//				
//			}
//		}
//		if (count == 0)			//如果某一趟没有交换位置，则说明已经排好序，直接退出循环
//				break;	
//	}
// 
//}
////   读取触摸芯片数据，并转化位位置数据，周期调用
////   10ms 调用一次 ， 统计5次转换数据，排序后去大去小，在求平均
////   返回值高16位存X值，低16位存y值
//uint32_t WS_TFT_Touth_Get_Point(SPI_HandleTypeDef *hspi)
//{  
//	 uint16_t adc_x , adc_y   ;
//	 static uint16_t  x_buf[5] , y_buf[5], count = 0;
//   uint32_t  num = 0;

////  以XPT2046中断输出定义为条件控制编译， 防止在没有定义下编译报错。	
//#if defined( 	LCD_T_PEN_GPIO_Port )  	
//	 if(HAL_GPIO_ReadPin(LCD_T_PEN_GPIO_Port,LCD_T_PEN_Pin) == GPIO_PIN_RESET)
//	 {
//		   if(count < 5)
//			 {
//				 x_buf[count] = WS_TFT_Touth_Read_XPT2046(hspi,0xd0); //  获取X轴数据
//				 y_buf[count] = WS_TFT_Touth_Read_XPT2046(hspi,0x90); //  获取Y轴数据
//				 count ++;
//			 }
//			 else
//			 {
//			   count = 0;
//				 sort (x_buf,5) ;
//         sort (y_buf,5) ;			
//         adc_x = (x_buf[1] + x_buf[2] + x_buf[3])	/ 3 ;	
//         adc_y = (y_buf[1] + y_buf[2] + y_buf[3])	/ 3 ;					 
//			 }
//		 
//			 //  合成数据
//			 num   =  (uint16_t)(touchX_k * adc_x + touchX_b) ;  //  存高16位
//			 num <<= 16;
//			 num  |=  (uint16_t)(touchY_k * adc_y + touchY_b) ;  //  存低16位
//	 }	
//#endif
//	 return num;
//}




