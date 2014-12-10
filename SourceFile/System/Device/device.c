/*
********************************************************************************
*                                嵌入式微系统
*                                   msOS
*
*                            硬件平台:msPLC DEMO
*                          主芯片:STM32F103R8T6/RBT6
*                           深圳市雨滴科技有限公司
*
*                                作者:王绍伟
*                                网名:凤舞天
*                                标识:Wangsw
*
*                                QQ:26033613
*                               QQ群:291235815
*                        淘宝店:http://52edk.taobao.com
*                      论坛:http://gongkong.eefocus.com/bbs/
*                博客:http://forum.eet-cn.com/BLOG_wangsw317_1268.HTM
********************************************************************************
*文件名       : device.c
*作用         : 设备接口
*原理         : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/07/21       初始版本
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
* 函数名	: InitializeDevice
* 描述	    : 初始化设备
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/08       初始版本
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
    
	NVIC_SetPriority(SysTick_IRQn, 0xFE);   //次优先级
 	NVIC_SetPriority(PendSV_IRQn, 0xFF);    //最低优先级
    
}


