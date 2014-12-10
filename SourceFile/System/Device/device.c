/*
********************************************************************************
*                                Ƕ��ʽ΢ϵͳ
*                                   msOS
*
*                            Ӳ��ƽ̨:msPLC DEMO
*                          ��оƬ:STM32F103R8T6/RBT6
*                           ��������οƼ����޹�˾
*
*                                ����:����ΰ
*                                ����:������
*                                ��ʶ:Wangsw
*
*                                QQ:26033613
*                               QQȺ:291235815
*                        �Ա���:http://52edk.taobao.com
*                      ��̳:http://gongkong.eefocus.com/bbs/
*                ����:http://forum.eet-cn.com/BLOG_wangsw317_1268.HTM
********************************************************************************
*�ļ���       : device.c
*����         : �豸�ӿ�
*ԭ��         : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/07/21       ��ʼ�汾
********************************************************************************
*/



#include "drive.h"
#include "system.h"

extern void InitializeKey(void);

//extern void InitializeLcd(void);

extern void InitializeRtc(void);

extern void InitializeTimer(void);

extern void InitializeUsart(void);
extern void InitializeUsart2(void) ;

extern void InitializeSystick(void);

extern void InitializeIO(void);

//extern void InitializeAdc(void);

extern void InitializeStorage(void);

extern void InitializeDevice(void);

//////by loveywm 20140906
extern void InitializeLed(void);

extern void InitializeLcd_12864(void);

//by loveywm 20140922
extern void InitializeW25q64(void);


/*******************************************************************************
* ������	: InitializeDevice
* ����	    : ��ʼ���豸
* �������  : ��
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/08       ��ʼ�汾
*******************************************************************************/
void InitializeDevice(void)
{
    RCC->APB2ENR |=  RCC_APB2Periph_GPIOA \
                    |RCC_APB2Periph_GPIOB \
                    |RCC_APB2Periph_GPIOC \
                    |RCC_APB2Periph_GPIOD \
                    |RCC_APB2Periph_GPIOE \
                    |RCC_APB2Periph_GPIOF \
                    |RCC_APB2Periph_GPIOG\
                    |RCC_APB2Periph_AFIO;

	//loveywm 20140911
    	InitializeUsart();	
	InitializeUsart2() ;

    	InitializeKey();	

   

    	InitializeRtc();

    	InitializeTimer();

    	InitializeIO();

    	//InitializeAdc();

   	//InitializeStorage();

    	InitializeSystick();

    	///loveywm 20140906
	InitializeLed();
	InitializeLcd_12864();

	//loveywm 20140922
	InitializeW25q64();

	 //InitializeLcd();

    	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
	NVIC_SetPriority(SysTick_IRQn, 0xFE);   //�����ȼ�
 	NVIC_SetPriority(PendSV_IRQn, 0xFF);    //������ȼ�
    
}


