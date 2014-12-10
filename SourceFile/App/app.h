/*
********************************************************************************
********************************************************************************
*文件名     : app.h
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/13       初始版本
********************************************************************************
*/


#ifndef __APP_H
#define __APP_H

/*******************************************************************************
* 结构体名	: AppStruct
* 描述	    : msOS中最重要的一个结构体，封装了App层的所有数据及窗口控件
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/11       初始版本
*******************************************************************************/
typedef unsigned short T_U16;
typedef unsigned short u16;
typedef unsigned char T_U8;
typedef unsigned char u8;
typedef unsigned char BOOL;
typedef unsigned char UINT8,u8;
typedef unsigned short UINT16;
typedef unsigned int UINT32,u32;
#define 	APP_SETUP_ADD 		0x100000  //系统参数存放的flash位置

#define   MAX_FRAM_LEN    32   /*若帧长度大于此值，则认为无效，重新搜索帧头*/


typedef struct
{

	u16 	wLicCheckSum;					/*检验和(后面数据的字节和)*/

	struct Weight
	{
		u32	Rated_weight;//额定重量
		u32	Warning_weight;//预警重量
		u32	Alarm_weight;//报警重量
		u32	calibrate_sub_value;//两个校准重量值差值
		u32	calibrate_ad_sub_value;//两个校准重量值对应的AD检测值差值
		u32  weight_clear_ad_value;////清零时重量所对应的AD检测值；实际是零点所对应的AD检测值，因为只校准时也会计算一个零点值替代清零值；
		u32  weight_clear_ad_value_sign;////清零时重量值的正负号存贮区，0为正，1为负；

		
		u32  calibrate_one_suffer;//标定点一
		u32  calibrate_one_ad_suffer;//标定点一对应的ad值
		u32  calibrate_two_suffer;//标定点二
		u32  calibrate_two_ad_suffer;//标定点二对应的ad值
		u32  calibrate_suffer;//直线斜率*标定点一,待定

		//float	  calibrate_slope;//直线斜率
		
	}Weight;

	
	
    struct Data
    {
        int Frequency;                 //Frq
        int PowerPercent;
        int Power;
        int Voltage;
        int Current;
        int  Temperature;               
        int State;

        int MaxPower;
        int  MaxTemperature;
        int MaxFrequency;
        int MaxFrequencyOffset;
        int MinStream;

	u8	passwd[8];//存放密码地方
    }Data;

	 u16  Input_Data;//16位状态值检测存储值
	 u32  Weight_Tmp;//传感器的捕捉的重量值
	 
}AppStruct;

extern AppStruct App;
extern AppStruct App_Flash;
extern u16 Temperature_tmp;
extern u32	Target_F;
extern unsigned char fram_len;
extern unsigned char  fram_data_buff[MAX_FRAM_LEN];


#include "logic.h"

#include "menu.h"

#include 	"Voice.h"
#include "KeyBoard.h"

#include "uiPross.h"
#include "uiLCD_12864.h"
#include "uiFont.h"

#include "password.h"
#include "SFlashApi.h"
#include "Uart.h"

#include "uiLCD_tft.h"
#include "load.h"//载重相关代码文件

#endif /*__APP_H*/

