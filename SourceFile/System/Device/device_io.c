/*
********************************************************************************
*                          
********************************************************************************
*文件名     : device_io.c
*作用       : 输入输出设备
*原理       : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/08/13       初始版本
********************************************************************************
*/

#include "drive.h"
#include "system.h"



//#define BeepPin	PaOut(1)

//loveywm 20140906
#define BeepPin	PcOut(1)


static void SetBeep(bool status)
{
    BeepPin = status;
}

void InitializeIO(void)
{
    	//loveywm 20140906
    	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
    	BeepPin = 0;    

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
        	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
        	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10|GPIO_Pin_11;
        	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
        	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
    
    	System.Device.IO.SetBeep = SetBeep;
}

