/*
********************************************************************************
*                     
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    whatbook        2014/09/25       ��ʼ�汾
********************************************************************************
*/
/*
********************************************************************************
//���Ȼ�
//1:��ɽ���ܹ��Ĳ��ֺʹ
//2:��ɴ洢���ִ���
//3:������빦�ܵ���֤�ʹ洢
//4:ʵ��Ŀ��¥��ĵ���
//5:





*********************************************************************************
*/
#include "system.h"
#include "app.h"


//loveywm 20140923
extern u32 gTimeCount;//�ڰ��������ж����

//#define IdentifyNumberAddress  0x1FFFF7E8

//#define LogicTaskStackSum   400             //ҵ���߼�����ջ���
//#define MenuTaskStackSum    100             //�˵���������ջ���

//uint  LogicTaskStack[LogicTaskStackSum];    //ҵ���߼�����ջ����
//uint  MenuTaskStack[MenuTaskStackSum];      //�˵���������ջ����

//void * LogicTaskMessageQueue[40];           //ҵ���߼���Ϣ����

AppStruct App;
AppStruct App_Flash;

u16 Temperature_tmp;
u32	Target_F;//Ŀ��¥���ȫ�ֱ���
unsigned char  fram_data_buff[MAX_FRAM_LEN];////������鱣��õ���һ����ͷȥβ�ı���֡
unsigned char  fram_len = 0;


u32 tmpxxx;//���ڼ���
u8 master_pre_value;//ɨ�谴��ֵ��ǰ��

/*******************************************************************************
* ������	: SystemTickRoutine
* ����	    : Ӧ�ò�ϵͳ���ģ�����ִ�й̶�������ܣ����ʱ��ȡ����ע�ắ��
* �������  : ��
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/03       ��ʼ�汾
*******************************************************************************/
static void SystickRoutine(void) 
{
    //�ص�������10msһ�α�ϵͳʱ�ӵ���

	gTimeCount++;
}


/*******************************************************************************
* ������	: Usart1RxdFunction
* ����	    : Usart1���մ���ص���������Ҫͨ��ע�����ʹ��
* �������  : data Usart����ֵ
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/13       ��ʼ�汾
*******************************************************************************/
static void Usart1RxdFunction(byte data)
{
   	//PostMessageToLogicTask(KeyMessageType, data);  
   	//printf("data==%d\r\n",data);
	//printf("data==%x\r\n",data);

	//��data�е�ֵ����Ӧ�ò�
	//Usart1RxDataTOApp(data);
}
static void Usart2RxdFunction(byte data)
{
	//��data�е�ֵ����Ӧ�ò�
	Usart2RxDataTOApp(data);
}

/////////////////////////////////////////////////////////////////

/*******************************************************************************
* ������	: InitializeData
* ����	    : ��ʼ��Ĭ�ϲ���
* �������  : ��
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/08       ��ʼ�汾
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
		pPoint += 2;//ȥ��ǰ�������ӵ�2λ��ʼУ��
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

	 printf("\nоƬID�� = %d", System.Device.W25q64.SPI_Flash_ReadID());
	 printf("\nоƬID�� = 0x%x", System.Device.W25q64.SPI_Flash_ReadID());


	//����д��Ĭ������
	//Flash_WriteInfo("123456", 0x300000, 6);


}

/*******************************************************************************
* ������	: InitializeApp
* ����	    : ��ʼ��App
*           : InitializeMmi:��ʼ������
*           : System.Device.SystemTick.Register:ע��Ӧ�ò����
*           : System.Device.Adc.Register:ע��ADC�����豸����
*           : System.Device.Usart1.RxdRegister Usart1���պ���ע��
* �������  : ��
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/03       ��ʼ�汾
*******************************************************************************/
static void InitializeApp(void)
{
    	//InitializeData();
    
    	//InitializeMenu();

    	System.Device.Systick.Register(Systick100, SystickRoutine);//����

    	//System.Device.Adc.Register(AdcChannel0, (ushort *)(&Temperature_tmp));

   	 //System.Device.Adc.Register(AdcChannel1, (ushort *)(&App.Data.Current));

    	//System.Device.Usart1.RxdRegister(Usart1RxdFunction);

	 System.Device.Usart2.RxdRegister(Usart2RxdFunction);
}


/*******************************************************************************
* ������	: main
* ����	    : msOS�����������
*           : InitializeData:��ʼ���洢������
*           : System.Initialize:System��ʼ����ʵ�ֵײ�ϵͳ��ʼ�� 
*           : �����ʼ�����ȵģ����ȼ�Ҳ���ȣ�TaskSumҪ��������
*           : LogicTask: ҵ���߼����񣬳�ʼ��Ϊ������ȼ� 
*           : MenuTask: �������񣬳�ʼ��Ϊ������ȼ�,ע�⣬������Ϊ������ȼ�
*           : LogicTaskMessageQueuePoint:��ʼ��һ���߼�������Ϣ����
* �������  : ��
* ���ز���  : int
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/03       ��ʼ�汾
*******************************************************************************/
int main(void) 
{            

	System.Initialize();                                                //��ʼ��ϵͳ��
        
    	InitializeApp();                                                    //��ʼ��Ӧ�ò�


	uiProcMain();//������ѭ��������

	
                                                        //����ҵ���߼�����������ȼ�0 
    //System.OS.CreateTask(LogicTask, &LogicTaskStack[LogicTaskStackSum - 1]);
                                                        //�����˵���������������ȼ�1
    //System.OS.CreateTask(MenuTask, &MenuTaskStack[MenuTaskStackSum - 1] );
                                                        //����ҵ���߼��������Ϣ����
    //LogicTaskMessageQueuePointer = System.OS.CreateMessageQueue(LogicTaskMessageQueue, 40);

    //System.OS.Start();                                                  //RTOS����
}



