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
*文件名     : device_systick.c
*作用       : 系统节拍
*原理       : 
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/12/16       初始版本
********************************************************************************
*/



#include "drive.h"
#include "system.h"

//extern void KeySystick100Routine(void);
extern void MasterSystick100Routine(void); 
extern void RtcSystick100Routine(void);

//extern void TimerSystick1000Routine(void);

extern void UsartSystick1000Routine(void);

extern void Usart2Systick1000Routine(void);

//extern void AdcSystick10000Routine(void);

extern void SysTick_Handler(void);

//loveywm 20140923
//extern u32 gTimeCount;//在按键驱动中定义的

#define Systick10000Sum 2
#define Systick1000Sum  4
#define Systick100Sum   8

static void Dummy(void) {};

static function Systick10000RegisterPointerBlock[Systick10000Sum] =
{
    Dummy, Dummy
};

static function Systick1000RegisterPointerBlock[Systick1000Sum] =
{
    Dummy, Dummy, Dummy, Dummy
};

static function Systick100RegisterPointerBlock[Systick100Sum] =
{
    Dummy, Dummy, Dummy, Dummy, Dummy, Dummy, Dummy, Dummy
};

/*******************************************************************************
* 函数名	: RegisterSystick
* 描述	    : 系统节拍注册函数，用于注册回调函数到不同速度的系统节拍数组中。
* 输入参数  : systemTick 系统节拍速度类型 registerFunction 注册的函数指针
* 返回参数  : bool布尔变量，返回是否成功
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/11       初始版本
*******************************************************************************/
static bool RegisterSystick(SystickEnum systick, function registerFunction)
{
    static byte Systick100Counter = 0;
    static byte Systick1000Counter = 0;
    static byte Systick10000Counter = 0;

    Assert(registerFunction != (function)0);
    EnterCritical();//关中断,利用线程锁的知识，linux先的信号量
    switch(systick)
    {
        case Systick100:
            if (Systick100Counter == Systick100Sum)
                return(false);
            
            Systick100RegisterPointerBlock[Systick100Counter++] = registerFunction;  
            ExitCritical();
            return(true);
        case Systick1000:
            if (Systick1000Counter == Systick1000Sum)
                return(false);
            
            Systick1000RegisterPointerBlock[Systick1000Counter++] = registerFunction;   
            ExitCritical();
            return(true);
        case Systick10000:
            if (Systick10000Counter == Systick10000Sum)
                return(false);
            
            Systick10000RegisterPointerBlock[Systick10000Counter++] = registerFunction;   
            ExitCritical();
            return(true);
        default:
            ExitCritical();
            return(false);
    }
}

void InitializeSystick(void)
{
    System.Device.Systick.Register = RegisterSystick;
    SysTick_Config(SystemCoreClock / 10000);//2//72*10^6/1*10^4=7200.1/72*10^6 * 7200 =1/10000ms=0.1ms
}



/*******************************************************************************
* 函数名	: SysTick_Handler
* 描述	    : 系统节拍中断入口，非常重要，每秒10000次，即0.1mS一次
*           : 系统节拍通过switch细分为每秒1000次，每秒100次的例行节拍
*           : 同层直接调用，高层采用注册调用
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/07       初始版本
*******************************************************************************/
void SysTick_Handler(void)
{
    static unsigned char Counter = 0;
	
    if(++Counter == 100)
    {
        Counter = 0;
    }

    Systick10000RegisterPointerBlock[0]();
    Systick10000RegisterPointerBlock[1]();

    switch(Counter) 
    {
        case 0:
        case 10:
        case 20:
        case 30:
        case 40:
        case 50:
        case 60:
        case 70:
        case 80:
        case 90:
            Systick1000RegisterPointerBlock[0]();//1//1ms一次
            break;
        case 2:
        case 12:
        case 22:
        case 32:
        case 42:
        case 52:
        case 62:
        case 72:
        case 82:
        case 92:
            Systick1000RegisterPointerBlock[1]();
            break;
        case 4:
        case 14:
        case 24:
        case 34:
        case 44:
        case 54:
        case 64:
        case 74:
        case 84:
        case 94:
            Systick1000RegisterPointerBlock[2]();
            break;
        case 6:
        case 16:
        case 26:
        case 36:
        case 46:
        case 56:
        case 66:
        case 76:
        case 86:
        case 96:
            Systick1000RegisterPointerBlock[3]();//1///1ms一次
            break;
        case 8:
        case 18:
        case 28:
        case 38:
        case 48:
        case 58:
        case 68:
        case 78:
        case 88:
        case 98:
            	UsartSystick1000Routine();
		Usart2Systick1000Routine();
            //TimerSystick1000Routine();
            break;

        case 1:
            Systick100RegisterPointerBlock[0]();//10ms一次
            break;
        case 3:
            Systick100RegisterPointerBlock[1]();
	   break;
        case 5:
            Systick100RegisterPointerBlock[2]();
            break;
        case 7:
            Systick100RegisterPointerBlock[3]();
            break;
        case 9:
            Systick100RegisterPointerBlock[4]();
            break;
        case 11:
            Systick100RegisterPointerBlock[5]();
            break;
        case 13:
            Systick100RegisterPointerBlock[6]();
            break;
        case 15:
            Systick100RegisterPointerBlock[7]();
            break;  
	//loveywm 20140923
	case 17:
		//gTimeCount++;
		break;
            
        case 97:
            //RtcSystick100Routine();
            break;
        case 99:
            //KeySystick100Routine();
            MasterSystick100Routine();
            break; 
        default:
            break;
    }

   // AdcSystick10000Routine();//0.1ms一次
}





