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
*�ļ���     : logic.c
*����       : ҵ���߼������ļ�
*ԭ��       : ���������Ϣ���簴�����������ȣ���׼��һЩ�����Ĵ���ģʽ
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/07/21       ��ʼ�汾
********************************************************************************
*/


#include "system.h"
#include "app.h"

//loveywm 20140923
#include "uilcd_12864.h"
#include "uifont.h"


/*******************************************************************************
* ������	: FormFocusLongKeyProcess
* ����	    : FormFocusLongKey����ҳ�潹���л���������л�����ҳ��
* �������  : ��
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
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
* ������	: SaveKeyProcess
* ����	    : �洢�����ı��ؼ�������ֵ
* �������  : ��
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
*******************************************************************************/
void SaveKeyProcess(void)
{
    if (App.Menu.FocusFormPointer->FocusTextBoxDataPointer == null) return;
   
    //System.Device.Storage.Parameter.Write((uint *)App.Menu.FocusFormPointer->FocusTextBoxDataPointer);
}


/*******************************************************************************
* ������	: SaveLongKeyProcess
* ����	    : �洢���в���
* �������  : ��
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
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
* ������	: StopKeyDefaultProcess
* ����	    : StopKeyȱʡ������
* �������  : ��
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
*******************************************************************************/
static void StopKeyDefaultProcess(void)
{
    App.Menu.FocusFormPointer->FocusTextBoxDataPointer = null;
    App.Menu.FocusFormPointer = &App.Menu.WorkForm;
}

static byte Counter = 0;
/*******************************************************************************
* ������	: KeyProcess
* ����	    : ������Ϣ������
* �������  : key�����ⰴ��ֵ
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
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

            
        case FormFocusLongKey:  //ҳ�泤�����л�
            FormFocusLongKeyProcess();
            break;   
        case TextBoxFocusKey:   //�ı��ؼ�����л�
            System.Gui.Form.SwitchTextBoxFocus();
            break;
        case AddKey:
        case AddLongKey:
        //case SubKey:
        case SubLongKey:
        //case AssistUpKey:
        case AssistUpLongKey:
        case AssistDownKey:
        case AssistDownLongKey: //���ְ�������
            System.Gui.Form.SetTextBoxData(key);
            break;
        //case SaveKey:           //�����ݴ洢
            //SaveKeyProcess();     
            //break;
        case SaveLongKey:       //ȫ�����ݴ洢
            SaveLongKeyProcess();
            break;
        case StartKey:          //��ʼ����,������־д
            System.Device.Storage.Log.Write("%d\n", Counter++);
            break;
        case StartLongKey:      //��ʼ����,������־��
            //printf(System.Device.Storage.Log.Read(-1));
            break;
        case StopKey:           //ֹͣ����
            StopKeyDefaultProcess();
            break;    
            */     
   	 }
}

/*******************************************************************************
* ������	: LogicTask
* ����	    : �߼�ҵ�����񣬸�����Ϣ�簴������������Ϣ�������
* �������  : ��
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/03       ��ʼ�汾
*******************************************************************************/
void LogicTask(void)
{
    //byte i;
    uint message;
    uint messageData; 
        
    System.OS.DelayTimeTick(2000);//2000
    //loveywm 20140906
/*
    //��Ƶ��Ӧ�����豸ɨƵ�Լ죬��ȡг���
    App.Menu.FocusFormPointer = &App.Menu.CheckForm;   
    for(i = 0; i < 16; i++)
    {
        System.OS.DelayTimeTick(100);//100
        App.Menu.CheckForm.ChartPointer->Column[i] = i % 5;
    }
    App.Menu.FocusFormPointer = &App.Menu.WorkForm;   
*/
    //�߼�ҵ�������ȡ��Ϣ�����䴦��
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
		uiLcd_1212_ch(UISTR_CURRENT_LOU_CHENG,0,4,4);//��ʾ����ǰ¥�㡱
		uiLcd_1212_ch(UISTR_AIM_LOU_CHENG,0,76,4);//��ʾ��Ŀ��¥�㡱
		uiLcdMediumString("00",1,3,0);//��ʾ����ǰ¥�㡱����
		uiLcdMediumString("11",1,11,1);//��ʾ��Ŀ��¥�㡱����
		
		//uiLcd_1212_ch(UISTR_WEIGHT,4,4,3);//��ʾ�����أ���
		uiLcd_1616_ch(0,4,0,3);//��ʾ�����أ���
		uiLcdMediumString("0.00",2,5,0);//�����ֵ�����ʵʱ����
		uiLcdMediumString("T",2,9,0);
		uiLcdMediumString("100",2,11,0);//�����ֵ����ذٷֱ�
		uiLcdMediumString("%",2,14,0);
		
		//������ʾ�豸״̬
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
            case KeyMessageType:                    //������Ϣ
                //KeyProcess((KeyEnum)messageData);
                break;
            default:                                //�����ʱ����Ϣ
                Function(message);
                break;
        }

        */
    }
}

