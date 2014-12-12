/*
********************************************************************************
*                          
********************************************************************************
*�ļ���     : device_io.c
*����       : ��������豸
*ԭ��       : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/08/13       ��ʼ�汾
********************************************************************************
*/

#include "drive.h"
#include "system.h"


#define 	ScanPin1	PcIn(12)
#define 	ScanPin2	PaIn(10)
#define 	ScanPin3	PaIn(11)
#define 	ScanPin4	PaIn(12)
#define 	ScanPin5	PcIn(9)

static byte ScanCounter = 0;
static unsigned char master_on_flg=0;   //ʵ�����޼����±�־

//loveywm 20140906
#define BeepPin	PcOut(1)


static void SetBeep(bool status)
{
    BeepPin = status;
}

static byte ScanPin(void)
{
    	byte scanValue;
	
    	scanValue = invalid;
    	if(ScanPin1 == 0)
        		scanValue &= 0xFE;

    	if(ScanPin2 == 0)
        		scanValue &= 0xFD;

    	if(ScanPin3 == 0)
        		scanValue &= 0xFB;

    	if(ScanPin4 == 0)
        		scanValue &= 0xF7;
		
	if(ScanPin5 == 0)
        		scanValue &= 0xEF;

    	return(scanValue);
}


/*******************************************************************************
* ������	: MasterSystick100Routine
* ����	    : ϵͳ����100/S����10mSһ��ɨ���ȡ��ֵ,��ȡ����ֵ
* �������  : ��
* ���ز���  : ��
********************************************************************************
*******************************************************************************/
void MasterSystick100Routine(void) 
{
    	byte scanValue;

    	scanValue = ScanPin();
	
    	if (scanValue != invalid)//������պ� 
	{
        		//App.Master_Flag = scanValue;
	} 
    	else 
    	{
        	
    	}
}


static void HB_Gpio_Set_Value(u8 HB_Gpio_type,u8 HB_Gpio_Value)
{
        switch(HB_Gpio_type)
        {
                //�ĸ�led��
                case    WTV_RST:
                        PcOut(0) = HB_Gpio_Value; 
                        break;
                case    WTV_CS:
                        PeOut(4) = HB_Gpio_Value; 
                        break;
                case    WTV_DATA:
                        PeOut(6) = HB_Gpio_Value; 
                        break;
                case    WTV_CLK:
                        PeOut(5) = HB_Gpio_Value; 
                        break;
                    default:
                        break;
        }
}


void InitializeIO(void)
{
    	//loveywm 20140906
    	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
    	BeepPin = 0;    

	//����ص����ɨ�谴��
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_9;
        	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
        	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
        	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
        	GPIO_Init(GPIOA, &GPIO_InitStructure);


	//�������ĸ����ƶ˿�
	//RST����
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
        	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//�����������ƽ�
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
        	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	
	
    
    	System.Device.IO.SetBeep = SetBeep;
	System.Device.IO.ScanPin = ScanPin;
	System.Device.IO.HB_Gpio_Set_Value = HB_Gpio_Set_Value;
}

