/*
********************************************************************************
*               
********************************************************************************
*文件名   : system.h
*作用     : 系统库全局头文件
*创建时间 : 2013.7.21
********************************************************************************
*/


#ifndef __SYSTEM_H
#define __SYSTEM_H


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>



#define DEMO_NEW

//#define DEMO_OLD





/*
1、类型定义兼容C#，尽可能用C#标准，常用的为int
2、char在C#中为16bit的unicode，keil C下为8bit
*/
//8bit
typedef char                sbyte;	    /*char 不能用于负数*/
typedef unsigned char       byte;       /*实际测试byte = char*/
//16bit
//typedef short             short
typedef unsigned short      ushort;
//32bit
//typedef int               int;
typedef unsigned int        uint;

//字符串
typedef char *              string;

typedef unsigned char       bool;

typedef void (*function)(void);
#define Function(address)   (*(function)address)()

typedef unsigned short T_U16;
typedef unsigned short u16;
typedef unsigned char T_U8;
typedef unsigned char u8;
typedef unsigned char BOOL;
typedef unsigned char UINT8,u8;
typedef unsigned short UINT16;
typedef unsigned int UINT32,u32;


#define true                1
#define false               0

#define TRUE		1
#define FALSE	0

#define null                ((void *) 0)
#define invalid             0xFF
#define MainClock           72000000

#define SetBit(data, offset)         (data |= 1U << offset)
#define ResetBit(data, offset)       (data &= ~(1U << offset))
#define GetBit(data, offset)         ((data >> offset) &0x01)

#define UintToByte3(data)               ((byte *)(&(data)))[3]
#define UintToByte2(data)               ((byte *)(&(data)))[2]
#define UintToByte1(data)               ((byte *)(&(data)))[1]
#define UintToByte0(data)               ((byte *)(&(data)))[0]

#define UintToUshort1(data)             ((ushort *)(&(data)))[1]
#define UintToUshort0(data)             ((ushort *)(&(data)))[0]

#define GetMessageType(data)            UintToByte3(data)
#define GetMessageData(data)            (data & 0x00FFFFFF)

#define GetByteFrom(address)                *((byte *)(address)) 
#define GetUshortFrom(address)              *((ushort *)(address)) 
#define GetUintFrom(address)                *((uint *)(address)) 

#define Assert(express)  if (!(express)){printf("\nASSERT: " #express "\n");}   //(void)0

//loveywm 20140906  图片界面标志
#define UIKEY_NONE		0
#define UIKEY_ESC         	'-'		
#define UIKEY_MENU		'*'		
#define UIKEY_UP         	'>'		
#define UIKEY_DOWN      	'<'		
#define UIKEY_OK         	'!'		
#define UIKEY_0          	'0'		
#define UIKEY_1          	'1'		
#define UIKEY_2          	'2'		
#define UIKEY_3          	'3'	
#define UIKEY_4          	'4'		
#define UIKEY_5          	'5'		
#define UIKEY_6          	'6'	
#define UIKEY_7          	'7'		
#define UIKEY_8          	'8'		
#define UIKEY_9          	'9'		
#define UIKEY_POWER	'#'		
#define UIKEY_F1			'A'
#define UIKEY_F2			'B'
#define UIKEY_F3			'C'
#define UIKEY_F4			'D'
#define UIKEY_F5			'E'
#define UIKEY_F6			'F'
#define UIKEY_DEL           'G'
#define UIKEY_LUN           'H'
#define UIKEY_YEAR           'I'
#define UIKEY_MONTH           'J'
#define UIKEY_DATE          'K'
#define UIKEY_HOUR           'L'
#define UIKEY_MIN           'M'
#define UIKEY_SEC           'N'
#define UIKEY_POINT           'O'

//////////////////////////////////////////////
//tft显示屏的头

//LCD的画笔颜色和背景色	   
//u16 POINT_COLOR=0x0000;	//画笔颜色

//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左	 

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u8	wramcmd;		//开始写gram指令
	u8  setxcmd;		//设置x坐标指令
	u8  setycmd;		//设置y坐标指令	 
}_lcd_dev; 	  

//LCD参数
//-----------------LCD端口定义---------------- 
#define	LCD_LED PbOut(0) //LCD背光    		 PB0 	    
//LCD地址结构体
typedef struct
{
	u16 LCD_REG;
	u16 LCD_RAM;
} LCD_TypeDef;
//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A10作为数据命令区分线 
//注意设置时STM32内部会右移一位对其! 111110=0X3E			    
#define LCD_BASE        ((u32)(0x6C000000 | 0x000007FE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)


////////////////////////////////////////////////

//输出IO口的对外定义
enum{

	WTV_CS,
	WTV_CLK,
	WTV_DATA,
	WTV_RST,
};



typedef enum
{
    KeyMessageType      = 0xFF,
    /*请填充类型*/
    TimerMessageType    = 0xF0  /*最小值,不允许有值小于它*/
}MessageType;
 				   
#include "device.h"  
#include "os.h"
#include "gui.h"


extern Event * LogicTaskMessageQueuePointer;

extern ushort CriticalNesting;
#define	EnterCritical()		__disable_irq(); CriticalNesting++;
#define ExitCritical()		if(--CriticalNesting == 0){__enable_irq();}

extern void Delay(uint delayClock);

extern void DelayUs(uint delayTime);

extern void DelayMs(uint delayTime);

extern byte HexToAscii(byte hex);

//extern void PostMessageToLogicTask(MessageType messageType, uint data);

/*******************************************************************************
* 结构体名	: SystemStruct
* 描述	    : msOS中最重要的一个结构体，封装了System层的所有接口
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/11       初始版本
*******************************************************************************/
typedef struct
{
    	void (*Initialize)(void);
    
    struct Device
    {      
        RtcStruct Rtc;

        struct IO
        {
            	void (*SetBeep)(bool status);
		void (*HB_Gpio_Set_Value)(u8 HB_Gpio_type,u8 HB_Gpio_Value);
		u8 (*ScanPin)(void);
        }IO;

	//loveywm 20140923
	struct HB_Key
	{

		//u8 (*uiKeyGetKey)(void);
		u8 (*HB_ScanPin)(void);

	}HB_Key;
	
        struct Adc
        {
            void (*Register)(AdcChannelEnum adcChannel, ushort * dataPointer);
        }Adc;
        
        	struct Lcd
        	{
            	//void (* DisplayString)(byte y, string string);
		//void (*LCD_Fill)(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
		//void (*LCD_Clear)(u16 color);
		//void LCD_Init(void);													   	//初始化
		void (*LCD_DisplayOn)(void);					//开显示
		void (*LCD_DisplayOff)(void);					//关显示
		void (*LCD_Clear)(u16 Color);	 				//清屏
		void (*LCD_SetCursor)(u16 Xpos, u16 Ypos);		//设置光标
		void (*LCD_DrawPoint)(u16 x,u16 y);				//画点
		void (*LCD_Fast_DrawPoint)(u16 x,u16 y,u16 color);	//快速画点
		u16  (*LCD_ReadPoint)(u16 x,u16 y); 				//读点 
		void (*Draw_Circle)(u16 x0,u16 y0,u8 r);				//画圆
		void (*LCD_DrawLine)(u16 x1, u16 y1, u16 x2, u16 y2);	//画线
		void (*LCD_DrawRectangle)(u16 x1, u16 y1, u16 x2, u16 y2);//画矩形
		void (*LCD_Fill)(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);	//填充单色
		void (*LCD_Color_Fill)(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);//填充指定颜色
		//void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);						//显示一个字符
		//void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  						//显示一个数字
		//void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				//显示 数字
		//void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);		//显示一个字符串,12/16字体

		void (*LCD_WriteReg)(u8 LCD_Reg, u16 LCD_RegValue);
		u16  (*LCD_ReadReg)(u8 LCD_Reg);
		void (*LCD_WriteRAM_Prepare)(void);
		void (*LCD_WriteRAM)(u16 RGB_Code);		  
		void (*LCD_Scan_Dir)(u8 dir);					//设置屏扫描方向
		void (*LCD_Display_Dir)(u8 dir);					//设置屏幕显示方向
		
        	}Lcd;

        ////loveywm 20140906
	    struct LED{
		    void (*LED_Status)(bool status);
	    }LED;
		
	    struct Lcd_12864{
		    //void (*uiLcdSmallString)(char* pStr, int row, int col, BOOL bIsReverse);
		    //void (*uiLcd_1212_ch)(u8 address,u8 row,u8 col,u8 num);
		    void (*ulLcdChar)(UINT8* pbyFontData, int row, int col, BOOL bIsReverse,
			        int nFontCx, int nFontCyBytes, int nRowBytes, int nColHarfNum, int nMargin);
		    void (*ioctl_lcd_xpos_set)(int data, int devid );
		    void (*ioctl_lcd_ypos_set)( int data, int devid );
		    void (*LCD_Data_write)(u8 dat);
		    void (*uiLcdClear)(void);
	    }Lcd_12864;
        
        struct Usart1
        {
            void (*Open)(void);
            void (*Close)(void);
            void (*RxdRegister)(void * registerFunction);
            bool (*WriteByte)(byte data);
            void (*Write)(byte * dataPointer, int sum);
        }Usart1;

	struct Usart2
        {
            void (*Open)(void);
            void (*Close)(void);
            void (*RxdRegister)(void * registerFunction);
            bool (*WriteByte)(byte data);
            //void (*Write)(byte * dataPointer, int sum);
        }Usart2;
        
        struct Timer
        {
            byte (*Start)(TimerhandleModeEnum mode, uint delay, function registerFunction);
            void (*Stop)(byte id); 
        }Timer;

/*
	struct Storage
        {
            struct Parameter
            {
                ParameterStruct (*Profile)(void);
                void (*Erase)(void);
                bool (*Read)(void * dataPointer);
                bool (*Write)(void * dataPointer);  
                void (*Clean)(ParameterStruct parameter);
            }Parameter;
            struct Log
            {
                LogStruct (*Profile)(void);
                void (*Erase)(void);
                char * (*Read)(int sequence);
                bool (*Write)(char *fmt, ...);
            }Log;
        }Storage;
*/
	struct W25q64{

		void (*SPI_Flash_Read)(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);
		void (*SPI_Flash_Write)(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
		u16 (*SPI_Flash_ReadID)(void);

	}W25q64;

        struct Systick
        {
            bool (*Register)(SystickEnum systemTick, function registerFunction);
        }Systick;
        
    }Device;


/*
    struct OS
    {
        void (* Start)(void);
        void (* CreateTask)(void (*taskPointer)(void), uint *stackTopPointer);
        Event * (* CreateMessageQueue)(void **start, ushort size);
        void * (* PendMessageQueue)(Event *eventPointer, uint timeout);
        bool (* PostMessageQueue)(Event *eventPointer, void *messagePointer);
        void (* DelayTimeTick)(uint timeTick); 
    }OS;
*/
/*
    struct Gui
    {
        void (*Parse)(Form * formPointer);
	//loveywm 20140910
	uint Globle_Key;//全局按键值显示

        struct
        {
            void (* AddLabel)(Form * formPointer, Label *labelPointer);
            void (* AddTextBox)(Form * formPointer, TextBox *textBoxPointer);
            void (* Initialize)(Form *formPointer);

            void (*SwitchTextBoxFocus)(void);
            void (*SetTextBoxData)(KeyEnum key);
            //loveywm 20140906
		    void (*drawxxx)(Form * formPointer);
		
        }Form;
    }Gui;

    */
}SystemStruct;

extern SystemStruct System;

#endif 
