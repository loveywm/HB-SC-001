/*
********************************************************************************

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
#define LED0 		PcOut(2)// DS0


static void LED_Status(bool status)
{
    	//BeepPin = status;
	LED0 = status;
}

void InitializeLed(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
    	LED0 = 0;    
    
    	//System.Device.IO.SetBeep = SetBeep;
	System.Device.LED.LED_Status = LED_Status;
}

