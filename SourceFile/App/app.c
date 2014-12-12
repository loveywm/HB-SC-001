/*
********************************************************************************
*                     
********************************************************************************
*版本     作者            日期            说明
*V0.1    whatbook        2014/09/25       初始版本
********************************************************************************
*/
/*
********************************************************************************
//进度汇
//1:完成界面架构的布局和搭建
//2:完成存储部分代码
//3:完成密码功能的验证和存储
//4:实现目标楼层的调整
//5:





*********************************************************************************
*/
#include "system.h"
#include "app.h"


//loveywm 20140923
extern u32 gTimeCount;//在按键驱动中定义的

//#define IdentifyNumberAddress  0x1FFFF7E8

//#define LogicTaskStackSum   400             //业务逻辑任务栈深度
//#define MenuTaskStackSum    100             //菜单界面任务栈深度

//uint  LogicTaskStack[LogicTaskStackSum];    //业务逻辑任务栈数组
//uint  MenuTaskStack[MenuTaskStackSum];      //菜单界面任务栈数组

//void * LogicTaskMessageQueue[40];           //业务逻辑消息队列

AppStruct App;
AppStruct App_Flash;

u16 Temperature_tmp;
u32	Target_F;//目标楼层的全局变量
unsigned char  fram_data_buff[MAX_FRAM_LEN];////这个数组保存得到的一个掐头去尾的报文帧
unsigned char  fram_len = 0;


u32 tmpxxx;//用于计数
u8 master_pre_value;//扫描按键值的前面

/*******************************************************************************
* 函数名	: SystemTickRoutine
* 描述	    : 应用层系统节拍，用于执行固定间隔功能，间隔时间取决于注册函数
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/03       初始版本
*******************************************************************************/
static void SystickRoutine(void) 
{
    //回调函数，10ms一次被系统时钟调用

	gTimeCount++;
}


/*******************************************************************************
* 函数名	: Usart1RxdFunction
* 描述	    : Usart1接收处理回调函数，需要通过注册才能使用
* 输入参数  : data Usart接收值
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/13       初始版本
*******************************************************************************/
static void Usart1RxdFunction(byte data)
{
   	//PostMessageToLogicTask(KeyMessageType, data);  
   	//printf("data==%d\r\n",data);
	//printf("data==%x\r\n",data);

	//将data中的值传给应用层
	//Usart1RxDataTOApp(data);
}
static void Usart2RxdFunction(byte data)
{
	//将data中的值传给应用层
	Usart2RxDataTOApp(data);
}

/////////////////////////////////////////////////////////////////

/*******************************************************************************
* 函数名	: InitializeData
* 描述	    : 初始化默认参数
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/08       初始版本
*******************************************************************************/


void InitializeData(void)
{

	UINT16 wCheckSum = 0x00;
	UINT8 *pPoint;
	UINT16	i;
	BOOL 	cRet = FALSE;
	BOOL 	bCheckSumErr = FALSE;
	BOOL	bLoadDefaultSettings = FALSE;
	UINT8	ReadCheckSumCounts = 0;
	UINT8 	uiKey = UIKEY_NONE;
	

	while(!bCheckSumErr && (ReadCheckSumCounts < 5))
	{
		Flash_ReadInfo((UINT8 *)&App, APP_SETUP_ADD, sizeof(App));

		pPoint = (UINT8 *)&App;
		pPoint += 2;//去掉前两个，从第2位开始校验
		for(i = 0x00;i < sizeof(App) -2;i++)
		{
			wCheckSum += *pPoint++;
		}
		if(App.wLicCheckSum != wCheckSum)
		{
			bCheckSumErr = TRUE;
		}
		else
		{
			bCheckSumErr = FALSE;
			break;
		}
		ReadCheckSumCounts++;
	}
	if(bCheckSumErr)
	{

		App.Weight.Rated_weight = 2000;
		App.Weight.Warning_weight= 90;
		App.Weight.Alarm_weight = 110;

		App.Data.Frequency = 900000;
    		App.Data.PowerPercent = 20;   	
   		App.Data.MaxPower = 6000; 	
    		App.Data.MaxTemperature = 50;
    		App.Data.MaxFrequency = 980000;
    		App.Data.MaxFrequencyOffset = 100000;
    		App.Data.MinStream = 50;
    		App.Data.Power = 6000;
    		App.Data.Voltage = 99;
    		App.Data.Current = 101;
    		App.Data.Temperature = 25;
    		App.Data.State = 1;

		App.Input_Data = 0xff;

		wCheckSum = 0x00;
		pPoint = (UINT8 *)&App;
		pPoint += 2;
		for(i = 0x00;i < sizeof(App) -2;i++)
		{
			wCheckSum += *pPoint++;
		}
		App.wLicCheckSum = wCheckSum;

		Flash_WriteInfo((u8*)&App,APP_SETUP_ADD,sizeof(App));

		 printf("123\r\n");

	}

	memcpy(&App_Flash,&App,sizeof(App));

	 printf("\n芯片ID号 = %d", System.Device.W25q64.SPI_Flash_ReadID());
	 printf("\n芯片ID号 = 0x%x", System.Device.W25q64.SPI_Flash_ReadID());


	//测试写入默认密码
	//Flash_WriteInfo("123456", 0x300000, 6);


}

/*******************************************************************************
* 函数名	: InitializeApp
* 描述	    : 初始化App
*           : InitializeMmi:初始化界面
*           : System.Device.SystemTick.Register:注册应用层节拍
*           : System.Device.Adc.Register:注册ADC参数设备变量
*           : System.Device.Usart1.RxdRegister Usart1接收函数注册
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/03       初始版本
*******************************************************************************/
static void InitializeApp(void)
{
    	//InitializeData();
    
    	//InitializeMenu();

    	System.Device.Systick.Register(Systick100, SystickRoutine);//备用

    	//System.Device.Adc.Register(AdcChannel0, (ushort *)(&Temperature_tmp));

   	 //System.Device.Adc.Register(AdcChannel1, (ushort *)(&App.Data.Current));

    	//System.Device.Usart1.RxdRegister(Usart1RxdFunction);

	 System.Device.Usart2.RxdRegister(Usart2RxdFunction);
}


/*******************************************************************************
* 函数名	: main
* 描述	    : msOS整个程序入口
*           : InitializeData:初始化存储数据区
*           : System.Initialize:System初始化，实现底层系统初始化 
*           : 任务初始化优先的，优先级也优先，TaskSum要配套设置
*           : LogicTask: 业务逻辑任务，初始化为最高优先级 
*           : MenuTask: 界面任务，初始化为最低优先级,注意，它必须为最低优先级
*           : LogicTaskMessageQueuePoint:初始化一个逻辑任务消息队列
* 输入参数  : 无
* 返回参数  : int
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/03       初始版本
*******************************************************************************/
int main(void) 
{            

	System.Initialize();                                                //初始化系统层
        
    	InitializeApp();                                                    //初始化应用层


	uiProcMain();//主界面循环处理函数

	
                                                        //创建业务逻辑任务，最高优先级0 
    //System.OS.CreateTask(LogicTask, &LogicTaskStack[LogicTaskStackSum - 1]);
                                                        //创建菜单界面任务，最低优先级1
    //System.OS.CreateTask(MenuTask, &MenuTaskStack[MenuTaskStackSum - 1] );
                                                        //创建业务逻辑任务的消息队列
    //LogicTaskMessageQueuePointer = System.OS.CreateMessageQueue(LogicTaskMessageQueue, 40);

    //System.OS.Start();                                                  //RTOS启动
}



