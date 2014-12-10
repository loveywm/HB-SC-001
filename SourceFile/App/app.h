/*
********************************************************************************
********************************************************************************
*�ļ���     : app.h
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/13       ��ʼ�汾
********************************************************************************
*/


#ifndef __APP_H
#define __APP_H

/*******************************************************************************
* �ṹ����	: AppStruct
* ����	    : msOS������Ҫ��һ���ṹ�壬��װ��App����������ݼ����ڿؼ�
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
*******************************************************************************/
typedef unsigned short T_U16;
typedef unsigned short u16;
typedef unsigned char T_U8;
typedef unsigned char u8;
typedef unsigned char BOOL;
typedef unsigned char UINT8,u8;
typedef unsigned short UINT16;
typedef unsigned int UINT32,u32;
#define 	APP_SETUP_ADD 		0x100000  //ϵͳ������ŵ�flashλ��

#define   MAX_FRAM_LEN    32   /*��֡���ȴ��ڴ�ֵ������Ϊ��Ч����������֡ͷ*/


typedef struct
{

	u16 	wLicCheckSum;					/*�����(�������ݵ��ֽں�)*/

	struct Weight
	{
		u32	Rated_weight;//�����
		u32	Warning_weight;//Ԥ������
		u32	Alarm_weight;//��������
		u32	calibrate_sub_value;//����У׼����ֵ��ֵ
		u32	calibrate_ad_sub_value;//����У׼����ֵ��Ӧ��AD���ֵ��ֵ
		u32  weight_clear_ad_value;////����ʱ��������Ӧ��AD���ֵ��ʵ�����������Ӧ��AD���ֵ����ΪֻУ׼ʱҲ�����һ�����ֵ�������ֵ��
		u32  weight_clear_ad_value_sign;////����ʱ����ֵ�������Ŵ�������0Ϊ����1Ϊ����

		
		u32  calibrate_one_suffer;//�궨��һ
		u32  calibrate_one_ad_suffer;//�궨��һ��Ӧ��adֵ
		u32  calibrate_two_suffer;//�궨���
		u32  calibrate_two_ad_suffer;//�궨�����Ӧ��adֵ
		u32  calibrate_suffer;//ֱ��б��*�궨��һ,����

		//float	  calibrate_slope;//ֱ��б��
		
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

	u8	passwd[8];//�������ط�
    }Data;

	 u16  Input_Data;//16λ״ֵ̬���洢ֵ
	 u32  Weight_Tmp;//�������Ĳ�׽������ֵ
	 
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
#include "load.h"//������ش����ļ�

#endif /*__APP_H*/

