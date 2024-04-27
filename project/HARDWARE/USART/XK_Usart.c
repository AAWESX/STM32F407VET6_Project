#include "USART\XK_Usart.h"
#include "string.h"
#include <stdarg.h>		  //  标准头文件
#include <stdio.h>
#include "usart.h"


#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
}

//重定义fputc函数 
int fputc(int ch, FILE *f)
{  
//		HAL_UART_Transmit(&huart1,(uint8_t *)ch,sizeof(ch));
	while((USART1->SR&0X40) == 0); //循环发送,直到发送完毕   
    USART1->DR = (uint8_t) ch;
	return ch;
}

#endif 


//   格式化可变变量字符串缓存
char  formatBuf[256];
//   格式化可变变量字符串
char *XK_Format(char *p,...)
{
    va_list ap;
		va_start(ap,p);
		vsprintf(formatBuf,p,ap);
		va_end(ap);
	  return formatBuf;
}


//   格式化可变变量字符串缓存
#define   DEBUG_LOOP_LEN 10
char      debugBuf[DEBUG_LOOP_LEN][128];
uint16_t  loopCount   = 0 ;
uint16_t  loopTxCount = 0 ;
//   格式化输出的调试函数
void XK_printf(char *p,...)   // 与 printf  一摸一样的功能 
{
	  /*  格式化数据  */
    va_list ap;
		va_start(ap,p);
		vsprintf(debugBuf[loopCount],p,ap);
		va_end(ap);
	  if(++loopCount == DEBUG_LOOP_LEN ) loopCount = 0;	
//  编译控制	
#ifdef DEBUG_EN    
	  if(huart1.gState == HAL_UART_STATE_READY) 
		{
		  XK_Debug_Loop_Transmit(); 
		}			
#endif	 
}

//   循环发送调试信息
void XK_Debug_Loop_Transmit(void)
{
    if(loopTxCount != loopCount)
		{
		    HAL_UART_Transmit_IT(&huart1,(uint8_t *)debugBuf[loopTxCount],strlen(debugBuf[loopTxCount])); //  信息输出接口
		    
			  if(++ loopTxCount == DEBUG_LOOP_LEN)
					loopTxCount = 0;
		}
}


//  查找字符串内的在指定数字字符串
//  p_stepString : 被查找字符串
//  index        : 需要查找的字符串下标,1~XXXX;
//  p_RxBuf      ; 缓存查找到的字符串
char XK_Find_Index_String(char *p_stepString , int index, char *p_RxBuf)
{
	   int n = 0      ;     //  记录查找字符数
	   int strnum = 0 ;     //  记录查找字符串数
	   p_RxBuf[0]  = 0;     //  清空
     while(n < STEP_STRING_MAX_LEN && *p_stepString != 0) 
		 {  
				if((*p_stepString  == STOP_CHAR) ||  n == 0)
				{
				   strnum ++;
					 if(strnum == index) //  找到匹配下标
						{
							 if(n > 0) p_stepString ++;
							 while(*p_stepString != 0 && *p_stepString != STOP_CHAR)
							 {
							   *p_RxBuf = *p_stepString;
								  p_stepString ++        ;
								  p_RxBuf ++;
							 }
							 *p_RxBuf = 0;
							 return XK_Status_True;
						}
				}
			  n ++           ;
				p_stepString ++;
		 }
	 *p_RxBuf = 0; 
	 return XK_Status_False;	 
}





