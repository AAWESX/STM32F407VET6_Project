/*****************************
编写核心代码，尽量做到不依赖硬件



*****************************/

#ifndef __CORE_H__
#define __CORE_H__

#include "main.h"

#define DEBUG_EN 1


#define STOP_CHAR ','
#define STEP_STRING_MAX_LEN 1024

#define  XK_Status_True		    ( 0 )
#define  XK_Status_False	    ( 1 )




//   格式化可变变量字符串
char *WS_Format(char *p,...);

//   调试输出
void XK_printf(char *p,...);
//   循环发送调试信息
void XK_Debug_Loop_Transmit(void);


char WS_Find_Index_String(char *p_stepString , int index, char *p_RxBuf);



#endif



