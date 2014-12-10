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
*文件名     : logic.c
*作用       : 业务逻辑处理文件
*原理       : 处理各类消息，如按键、传感器等，标准化一些按键的处理模式
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/07/21       初始版本
********************************************************************************
*/


#include "system.h"
#include "app.h"

//loveywm 20140923
#include "uilcd_12864.h"
#include "uifont.h"


/*******************************************************************************
* 函数名	: FormFocusLongKeyProcess
* 描述	    : FormFocusLongKey窗体页面焦点切换处理程序，切换窗体页面
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/11       初始版本
*******************************************************************************/
static void FormFocusLongKeyProcess(void)
{   
/*
    if (App.Menu.FocusFormPointer == &App.Menu.ServiceForm)
        App.Menu.FocusFormPointer = &App.Menu.WorkForm;
    else
        App.Menu.FocusFormPointer ++;
*/
	if(App.Menu.FocusFormPointer == &App.Menu.SetupForm)
	{
		App.Menu.FocusFormPointer = &App.Menu.LogoForm;
	}

	

}

/*******************************************************************************
* 函数名	: SaveKeyProcess
* 描述	    : 存储焦点文本控件的数据值
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/11       初始版本
*******************************************************************************/
void SaveKeyProcess(void)
{
    if (App.Menu.FocusFormPointer->FocusTextBoxDataPointer == null) return;
   
    //System.Device.Storage.Parameter.Write((uint *)App.Menu.FocusFormPointer->FocusTextBoxDataPointer);
}


/*******************************************************************************
* 函数名	: SaveLongKeyProcess
* 描述	    : 存储所有参数
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/11       初始版本
*******************************************************************************/
void SaveLongKeyProcess(void)
{
/*
	System.Device.Storage.Parameter.Write(&App.Data.Frequency);
    System.Device.Storage.Parameter.Write(&App.Data.PowerPercent);
    
    System.Device.Storage.Parameter.Write(&App.Data.MaxPower);
    System.Device.Storage.Parameter.Write((uint *)(&App.Data.MaxTemperature));
    System.Device.Storage.Parameter.Write(&App.Data.MaxFrequency);
    System.Device.Storage.Parameter.Write(&App.Data.MaxFrequencyOffset);
    System.Device.Storage.Parameter.Write(&App.Data.MinStream);
    
    System.Device.Storage.Parameter.Write(&App.Data.SerialNumber);
    System.Device.Storage.Parameter.Write(&App.Data.ProductionDate);
*/
}


/*******************************************************************************
* 函数名	: StopKeyDefaultProcess
* 描述	    : StopKey缺省处理函数
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/11       初始版本
*******************************************************************************/
static void StopKeyDefaultProcess(void)
{
    App.Menu.FocusFormPointer->FocusTextBoxDataPointer = null;
    App.Menu.FocusFormPointer = &App.Menu.WorkForm;
}

static byte Counter = 0;
/*******************************************************************************
* 函数名	: KeyProcess
* 描述	    : 按键消息处理函数
* 输入参数  : key，任意按键值
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/11       初始版本
*******************************************************************************/
void KeyProcess(KeyEnum key)
{

	u8 tmp[8];
	u8 i;
	
	printf("\nKey: = %x", key);
	
	System.Gui.Globle_Key = key;
    	switch(key)
    	{
		//loveywm 20140910
		case UIKEY_0:
			//App.Menu.FocusFormPointer = &App.Menu.WorkForm;
			System.Device.W25q64.SPI_Flash_Write("whatbook",0x00000004,8);
			
			break;
		case UIKEY_1:
			//App.Menu.FocusFormPointer = &App.Menu.LogoForm;
			System.Device.W25q64.SPI_Flash_Read(tmp,0x00000004,8);
			for(i=0;i<8;i++)
			{
				printf("%c",tmp[i]);
			}
			printf("\r\n");
			break;
		//case UIKEY_2:
			//uiLcdSmallString("wocha", 4, 4, 1);
			//break;
		case UIKEY_MENU:
			App.Menu.FocusFormPointer = &App.Menu.SetupForm;
			break;
		case UIKEY_ESC:
			//System.Gui.Form.SwitchTextBoxFocus();
			FormFocusLongKeyProcess();
			break;
		default:
			break;
/*
        //loveywm 20140906
        case 1://KEY_RIGHT
			App.Menu.FocusFormPointer = &App.Menu.WorkForm;
			break;
		case 2://KEY_DOWN
			break;

		case 4://KEY_LEFT
			App.Menu.FocusFormPointer = &App.Menu.LogoForm;
			break;
		case 8://KEY_UP
			break;

            
        case FormFocusLongKey:  //页面长按键切换
            FormFocusLongKeyProcess();
            break;   
        case TextBoxFocusKey:   //文本控件光标切换
            System.Gui.Form.SwitchTextBoxFocus();
            break;
        case AddKey:
        case AddLongKey:
        //case SubKey:
        case SubLongKey:
        //case AssistUpKey:
        case AssistUpLongKey:
        case AssistDownKey:
        case AssistDownLongKey: //数字按键操作
            System.Gui.Form.SetTextBoxData(key);
            break;
        //case SaveKey:           //单数据存储
            //SaveKeyProcess();     
            //break;
        case SaveLongKey:       //全部数据存储
            SaveLongKeyProcess();
            break;
        case StartKey:          //开始运行,测试日志写
            System.Device.Storage.Log.Write("%d\n", Counter++);
            break;
        case StartLongKey:      //开始运行,测试日志读
            //printf(System.Device.Storage.Log.Read(-1));
            break;
        case StopKey:           //停止运行
            StopKeyDefaultProcess();
            break;    
            */     
   	 }
}

/*******************************************************************************
* 函数名	: LogicTask
* 描述	    : 逻辑业务任务，各种消息如按键、传感器消息处理入口
* 输入参数  : 无
* 返回参数  : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    Wangsw        2013/09/03       初始版本
*******************************************************************************/
void LogicTask(void)
{
    //byte i;
    uint message;
    uint messageData; 
        
    System.OS.DelayTimeTick(2000);//2000
    //loveywm 20140906
/*
    //高频感应加热设备扫频自检，获取谐振点
    App.Menu.FocusFormPointer = &App.Menu.CheckForm;   
    for(i = 0; i < 16; i++)
    {
        System.OS.DelayTimeTick(100);//100
        App.Menu.CheckForm.ChartPointer->Column[i] = i % 5;
    }
    App.Menu.FocusFormPointer = &App.Menu.WorkForm;   
*/
    //逻辑业务任务获取消息，分配处理
    while(true)
    {     
	//loveywm 20140923
	u8 key;
	u8 i;
	key = System.Device.HB_Key.uiKeyGetKey();
		//printf("\r\nkey==%x\r\n",key);
	//KeyProcess(key);

		//uiLcdClear();
		//uiLcdSmallString("whatbook",0,0,0);
		uiLcd_1212_ch(UISTR_CURRENT_LOU_CHENG,0,4,4);//显示“当前楼层”
		uiLcd_1212_ch(UISTR_AIM_LOU_CHENG,0,76,4);//显示“目标楼层”
		uiLcdMediumString("00",1,3,0);//显示“当前楼层”数字
		uiLcdMediumString("11",1,11,1);//显示“目标楼层”数字
		
		//uiLcd_1212_ch(UISTR_WEIGHT,4,4,3);//显示“吊重：”
		uiLcd_1616_ch(0,4,0,3);//显示“吊重：”
		uiLcdMediumString("0.00",2,5,0);//这个数值填吊重实时数据
		uiLcdMediumString("T",2,9,0);
		uiLcdMediumString("100",2,11,0);//这个数值填吊重百分比
		uiLcdMediumString("%",2,14,0);
		
		//用于显示设备状态
		uiLcd_1212_ch(UISTR_DEVICE_IS_OK,6,44,4);

	switch(key)
    	{
		//loveywm 20140923
		case UIKEY_0:
			
			System.Device.W25q64.SPI_Flash_Write("whatbook",0x00000004,8);
			
			break;
		case UIKEY_1:
			
			//System.Device.W25q64.SPI_Flash_Read(tmp,0x00000004,8);
			for(i=0;i<8;i++)
			{
				//printf("%c",tmp[i]);
			}
			printf("\r\n");
			break;
		//case UIKEY_2:
			//uiLcdSmallString("wocha", 4, 4, 1);
			//break;
		case UIKEY_MENU:
			//App.Menu.FocusFormPointer = &App.Menu.SetupForm;
			uiLcdClear();
			while(1)
			{
				
				key = System.Device.HB_Key.uiKeyGetKey();

				if(key ==UIKEY_ESC )
				{
					uiLcdClear();
					break;
				}
				//uiLcdSmallString
				uiLcdSmallString("1:",0,0,0);
				uiLcd_1212_ch(UISTR_PINGCHENG_SET, 0, 15, 4);

				uiLcdSmallString("2:",2,0,0);
				uiLcd_1212_ch(UISTR_ZAIZHONG_SET, 2, 15, 4);

				uiLcdSmallString("3:",4,0,0);
				uiLcd_1212_ch(UISTR_MIMA_SET, 4, 15, 4);

				uiLcdSmallString("4:",6,0,0);
				uiLcd_1212_ch(UISTR_ZHAUNTAIZIJIAN_SET, 6, 15, 4);

			}
			break;
		case UIKEY_ESC:
			//System.Gui.Form.SwitchTextBoxFocus();
			//FormFocusLongKeyProcess();
			break;
		default:
			break;	

	}




	
	/*
	message = (uint)System.OS.PendMessageQueue(LogicTaskMessageQueuePointer, 0);
        
        messageData = GetMessageData(message);

	
		
        switch(GetMessageType(message))
        {
            case KeyMessageType:                    //按键消息
                //KeyProcess((KeyEnum)messageData);
                break;
            default:                                //软件定时器消息
                Function(message);
                break;
        }

        */
    }
}

