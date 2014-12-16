
#include "system.h"
#include "app.h"

#define HEAD_LEN  16	//�˵���Ŀÿ����ʾԤ��
#define FLAG_ROW	6
#define FLAG_COL_FOUR	44//�ĸ���ʱ�м���ʾ
#define FLAG_COL_FIVE	44//�����ʱ��ʾ
#define FLAG_COL_SIX	44//������ʱ��ʾ


extern const T_UI_MENUITEM g_uiMenuItemsSrc[11+2];

T_UI_MENUITEM	g_uiMenuItems[sizeof(g_uiMenuItemsSrc) / sizeof(g_uiMenuItemsSrc[0])];


u8 uiKeyIsDigit(u8 nKey)
{
	return (UIKEY_0 <= nKey && nKey <= UIKEY_9);
}


////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
int uiProcBoxNumber(BOOL *pbRet, int row, int col, int nInitial, int nMin, int nMax, int nMaxLen,
						BOOL bZeroPadding, BOOL b0isNo)
{
	int nRet = nInitial;
	int nPerRet = nInitial;
	int dwPrevTime = uiTimeGetTickCount() - 501;
	BOOL bPrevBlack = FALSE;
	UINT8 nKey;
	BOOL bStartNew = TRUE;
	char pnBuf[16];

	//printf("nInittial==%d\r\n",nInitial);
	
	*pbRet = FALSE;
	while(1)
	{
		if ((uiTimeGetTickCount() - dwPrevTime > 50) ||(nRet != nPerRet))
		//if(nRet != nPerRet)
		{
			//MinMax check
			if (nRet < nMin)
			{
				//uiBuzzerError();
				nRet = nMin;
				bStartNew = TRUE;
			}
			if (nRet > nMax)
			{
				//uiBuzzerError();
				nRet = nMax;
				bStartNew = TRUE;
			}
			sprintf(pnBuf, "%d", nRet);

			//draw
			dwPrevTime = uiTimeGetTickCount();
			bPrevBlack = !bPrevBlack;
			//bPrevBlack = TRUE;
			uiLcdLineErase8x16(row, col, col + nMaxLen, bPrevBlack);

			if (b0isNo && nRet == 0 && col + nMaxLen == 16)
			{
				//uiLcdMultiStringRightAlign(UISTR_NO, row, bPrevBlack);
			}
			else
			{
				if (bZeroPadding)
					uiLcdDecimal(nRet, row, col, bPrevBlack, nMaxLen);
				else
					uiLcdDecimal(nRet, row, col + nMaxLen - strlen(pnBuf), bPrevBlack, 0);
			}

			nPerRet = nRet;
		}
		else
		{
			//uiLcdDecimal(nRet, row, col, 1, nMaxLen);
		}

		nKey = uiKeyGetKey();
		if( nKey == UIKEY_ESC)
		{
			nRet = nInitial;
			break;
		}

		if (nKey == UIKEY_OK)
		{
			*pbRet = TRUE;
			break;
		}

		if (nKey == UIKEY_NONE)
			continue;

		if (nKey == UIKEY_UP)
		{
			nRet++;
			bStartNew = TRUE;
		}

		if (nKey == UIKEY_DOWN)
		{
			nRet--;
			bStartNew = TRUE;
		}

		if (uiKeyIsDigit(nKey))
		{
			if (bStartNew)
			{
				nRet = (int)nKey - '0';
				bStartNew = FALSE;
			}
			else
			{
				nRet = nRet * 10 + (int)nKey - '0';
			}

			sprintf(pnBuf, "%d", nRet);
			if (strlen(pnBuf) > nMaxLen)
			{
				nRet = nMax;
				bStartNew = TRUE;
			}
			
			//printf("nRet==%d\r\n",nRet);
			//printf("nPerRet==%d\r\n",nPerRet);
		}

		dwPrevTime = uiTimeGetTickCount();
		bPrevBlack = FALSE;
	}

	return nRet;
}








/////////////////////////////////////////////////////////////////
//��ʾ��ȡ������״ֵ̬�������ޣ����ص�
u8 uiProcShowFlag(u16 flag)
{
	u8 j;


	uiLcdLineErase8x16(3,5,14,0);//����ǰ״̬����
	
	if(flag == Input_invalid)
	{
		//������ʾ�豸״̬	
		uiLcd_1212_ch(UISTR_DEVICE_IS_OK,FLAG_ROW,FLAG_COL_FOUR,4);//�豸����
		return 0;
	}
	//
	if(flag&Input_invalid_1)//������δ��
	{
		uiLcd_1212_ch(UISTR_FLAG_IN_DOOR,FLAG_ROW,FLAG_COL_FOUR,5);

		
		return 1;
	}
	if(flag&Input_invalid_2)//������δ��
	{
		
		uiLcd_1212_ch(UISTR_FLAG_OUT_DOOR,FLAG_ROW,FLAG_COL_FOUR,1);
		uiLcd_1212_ch(UISTR_FLAG_IN_DOOR+1,FLAG_ROW,FLAG_COL_FOUR+12,4);
		return 1;
	}
	if(flag&Input_invalid_3)//�촰δ��
	{
		uiLcd_1212_ch(UISTR_FLAG_SKY_DOOR,FLAG_ROW,FLAG_COL_FOUR,2);
		uiLcd_1212_ch(UISTR_FLAG_IN_DOOR+3,FLAG_ROW,FLAG_COL_FOUR+24,2);
		return 1;
	}

	
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////

static void uiProcMenuBuild(void)
{
	int i, j=0;

	for(i=0; i<sizeof(g_uiMenuItemsSrc) / sizeof(g_uiMenuItemsSrc[0]); i++)
	{
		g_uiMenuItems[j] = g_uiMenuItemsSrc[i];
		j++;
	}
}
void uiProcMainDraw(BOOL Initialise,Menu_Parameter *Parameter)	/*�����������ʾ*/
{

	if (Initialise)
	{
		uiLcdClear();
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
		//uiLcd_1212_ch(UISTR_DEVICE_IS_OK,6,44,4);
		return ;
	}


	uiLcdDecimal(Target_F,1,11,1,2);
	
	if(Parameter->Parameter_Change_Flag == 1)
	{

		if((Parameter->Parameter_Change_Position)&(WEITHT))
		{
			uiLcdDecimal(Parameter->Weight_Integer,2,5,0,1);

			uiLcdMediumString(".",2,6,0);
	
			uiLcdDecimal(Parameter->Weight_Decimal,2,7,0,1);
			uiLcdDecimal(Parameter->Weight_Percentile,2,8,0,1);

			uiLcdDecimal(Parameter->Weight_Percentage,2,11,0,3);
		}
		
		if((Parameter->Parameter_Change_Position)&(SHOWFLAG))
		{
			//��ʱ������������
			//uiProcShowFlag(App.Input_Data);
		}

		if((Parameter->Parameter_Change_Position)&(SHOWFLOOR))
		{
			switch(Parameter->Show_Floor)
			{
				case MASTER_STOP:
					uiLcd_1616_ch(5,2,44,1);//��ʾ���
					break;
				case MASTER_DOWN:
					uiLcd_1616_ch(4,2,44,1);//��ʾ�½�ͼ��
					break;
				case MASTER_UP:
					uiLcd_1616_ch(3,2,44,1);//��ʾ����ͼ��
					break;
				default:
					break;
			}
		}

	}
	

}



u16 T_Get_Adc_Average(u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		//temp_val+=App.Data.Temperature;
		temp_val += Temperature_tmp;
		DelayMs(5);
	}
	return temp_val/times;
} 

static u8  Handle_Weight(Menu_Parameter *Parameter)
{
	u32 ad_temp,operate_temp;
	App.Weight_Tmp =  fram_data_buff[5];			
	App.Weight_Tmp = (App.Weight_Tmp << 8) |fram_data_buff[4];
	
	//printf("App.Weight_Tmp===%d\r\n",App.Weight_Tmp);
			
	if(App.Weight.weight_clear_ad_value_sign == 0)
	{
		if((u16)App.Weight_Tmp >= App.Weight.weight_clear_ad_value)
	         {
	                  ad_temp=(u16)App.Weight_Tmp-App.Weight.weight_clear_ad_value;
	         }
	         else
	         {
	               ad_temp=App.Weight.weight_clear_ad_value-(u16)App.Weight_Tmp;
	                  //weight_display_sign=1;
		}

	}
	if(App.Weight.weight_clear_ad_value_sign == 1)
	{

		ad_temp=(u16)App.Weight_Tmp+App.Weight.weight_clear_ad_value;
	}
				
	operate_temp=ad_temp;
	ad_temp=(operate_temp * App.Weight.calibrate_sub_value)/App.Weight.calibrate_ad_sub_value;//�õ����յ�����ֵ	

	//uiLcdDecimal(ad_temp,1+2, 0,0,4);

	Parameter->Weight_Integer = (u8)(ad_temp/1000);
	ad_temp=ad_temp%1000;
	Parameter->Weight_Decimal =(u8)(ad_temp/100);
	ad_temp=ad_temp%100;
	Parameter->Weight_Percentile =(u8)(ad_temp/10);

	Parameter->Parameter_Change_Flag = 1;
	Parameter->Parameter_Change_Position |= WEITHT;

	return TRUE;
}

static u8 Handle_ERR_Code(Menu_Parameter *Parameter)
{
	u16	Err_Code;
	Err_Code =  fram_data_buff[3];			
	Err_Code = (Err_Code << 8) |fram_data_buff[2];

	//printf("App.Input_Data==%x\r\n",App.Input_Data);
	//printf("Err_Code==%x\r\n",Err_Code);

	//��ʱ����
	uiLcdDecimal(Err_Code,3,9,0,5);
	//uiLcdDecimal(ad_temp,1+2, 0,0,4);
	
	if(App.Input_Data == Err_Code)
	{

		Parameter->Parameter_Change_Flag =1;
		Parameter->Parameter_Change_Position |= SHOWFLAG;
		return 0;
	}else
	{
		App.Input_Data = Err_Code;
		Parameter->Parameter_Change_Flag =1;
		Parameter->Parameter_Change_Position |= SHOWFLAG;
		
		return 1;
	}
	
}

//��������ɨ��
static u8  Handle_Master(Menu_Parameter *Parameter)
{

	u8 Scan_value ;
	App.Master_Flag = System.Device.IO.ScanPin();//ɨ������ֵ
	DelayMs(10);//ȥ��
	Scan_value = System.Device.IO.ScanPin();//ɨ������ֵ

	if(App.Master_Flag != Scan_value)
	{
		return 0;
	}
	else 
	{
		
	}

	if(master_pre_value ==  App.Master_Flag)
	{
		//return 0;//��������������Σ��������������������⣬��ʱֻ����һ��
		
		tmpxxx++;
		if(tmpxxx == 2)//���������ȣ��Ͳ�����
		{
			tmpxxx = 1;
			return 0;
		}
		//Send_Flag = 1;
	}
	else
	{
		//Send_Flag = 1;
		tmpxxx = 0;
		master_pre_value = App.Master_Flag;
	}
	Parameter->Parameter_Change_Flag = 1;
	Parameter->Parameter_Change_Position |= SHOWFLOOR;

	if(App.Master_Flag ==  0xfe)//��һλ�½�
	{
		
		HB_Send_Realy_CMD(CMD_RELAY_DOWN);
		Parameter->Show_Floor = MASTER_DOWN;
		uiLcdDecimal(1,3,15,0,1);
		WTV_Voice(MASTER_DOWM_FLAG);
		
	}
	else if(App.Master_Flag ==  0xfd)//�ڶ�λ����
	{
		HB_Send_Realy_CMD(CMD_RELAY_UP);
		Parameter->Show_Floor = MASTER_UP;
		uiLcdDecimal(2,3,15,0,1);


		WTV_Voice(MASTER_UP_FLAG);
		
	}
	else if(App.Master_Flag ==  0xfb)//����λֹͣ
	{
		HB_Send_Realy_CMD(CMD_RELAY_STOP);
		Parameter->Show_Floor = MASTER_STOP;
		uiLcdDecimal(3,3,15,0,1);
	}
	else
	{

	}

	

}
/////////////////////////////////////////////
//�жϱ����Ƿ�У�����ȷ,pRep����ı�������ͷȥβ
char   Cmd_Rep_valid(char*  pRep,char* plen)
{
	char i = 0,j = 0;
	//char sum = PROTOCOL_HEAD_1 + PROTOCOL_HEAD_2;
	char sum = 0;
	////�������ģ���ת��������
    	for(i=0;i<(*plen-1);)//���һ����У��ͣ������ж�
	{
		sum += pRep[i];
		if(pRep[i] == PROTOCOL_ESC_CHAR)
		{
			if(i == *plen-2)
			{
				return 0;
			}
			else
			{
				if((pRep[i+1]!= PROTOCOL_ESC_CHAR_COD)&&
					(pRep[i+1]!= PROTOCOL_TAIL_1_COD)&&
					(pRep[i+1]!= PROTOCOL_HEAD_1_COD))
					return 0;
				else
				{
					sum += pRep[i+1];
					fram_data_buff[j] = 0xff - pRep[i+1];
					j++;
					i+=2;
				}
			}
		}
		else
		{
			fram_data_buff[j] = pRep[i];
			j++;
			i++;
		}
	}
	fram_data_buff[j] = pRep[*plen-1];
	///�ж�У���
	if(sum != fram_data_buff[j])
		return 0;
	*plen = fram_data_buff[1]+3;//ͷ+���ݳ���+����+У���
    	return 1;
}



//////////////////////////////////////////////

void uiProcMenuHasValue(int nPopupMenuTitle, T_UI_MENUITEM *pUiMenuItem, int row,int nCurrentIndex)
{
	u32  nValue;
	u8 bRet = FALSE;
/*
	uiLcdLineErase6x16(row, 0, 20, FALSE);
	
	uiLcdMultiString(g_uiStrNumber[nCurrentIndex], row, 0, FALSE);
	if(g_uiLang != UILANG_EN)
	{	//˫�ֽ�����
		uiLcdMultiString(pUiMenuItem->nMenuStr, row, 1, FALSE);
	}
	else
	{
		//���ֽ�����
		uiLcdMultiString(pUiMenuItem->nMenuStr, row, 2, FALSE);
	}
*/
	switch(pUiMenuItem->nMenuStr)
	{
		case UISTR_MENU_ZAIZHONG_SET_EDINGZHONGLIANG:
			nValue = uiProcBoxNumber(&bRet, row, 10, 2000, 0, 5000, 4, 4, TRUE);
			if (bRet) 
			{
				//gsSetting.bLockEn = nValue;
				App.Weight.Rated_weight = nValue;
			}
                		break;
		case UISTR_MENU_ZAIZHONG_SET_YUJINGZAIHE:
			nValue = uiProcBoxNumber(&bRet, row, 12, 90, 0, 100, 3, 3, TRUE);
			if (bRet) 
			{
				//gsSetting.bLockEn = nValue;
				App.Weight.Warning_weight = (u8)nValue;
			}
                		break;
		case UISTR_MENU_ZAIZHONG_SET_BAOJINGZAIHE:
			nValue = uiProcBoxNumber(&bRet, row, 12, 110, 0, 150, 3, 3, TRUE);
			if (bRet) 
			{
				//gsSetting.bLockEn = nValue;
				App.Weight.Alarm_weight= (u8)nValue;
			}
                		break;
		default:
			//uiLcdFatalError("HAS_VALUE!");
			break;
	}
}


void uiProcMenuCustom(int nPopupMenuTitle, T_UI_MENUITEM *pUiMenuItem, int row)
{
	BOOL bDownloadRet, bDownloadEmpty;
	u8 cRet;
	u8 bRet;


	printf("row==%d\r\n",row);
	
	switch(pUiMenuItem->nMenuStr)
	{

              	case UISTR_MENU_MIMA_SET:
			Pwd_Enroll_Process();
			break;
		case UISTR_MENU_ZAIZHONG_SET_BIAODINGDIAN_ONE://���ñ궨��һ
			Load_Set_Calibration(1);
			break;
		case UISTR_MENU_ZAIZHONG_SET_BIAODINGDIAN_TWO://���ñ궨���
			if(Load_Set_Calibration(2) == 1)
			{
				get_weight_clear_value();
			}
		
			
			break;
		case UISTR_MENU_PINGCHENG_SET://ƽ��궨����
			Encoder_Demarcate();
			break;
		default:
			break;
	}
}

//////////////////////////////////////////////////////////////////////////
//����ֵҪ��ʾ�Ĳ���
void uiProcMenuDrawValue(T_UI_MENUITEM *pUiMenuItem, int row, BOOL bIsReverse)
{
	BOOL bIsMulti = FALSE;
	UINT32 dwValue = 0;
	UINT8 	cTemp[8];

	switch(pUiMenuItem->nMenuStr)
	{

		case UISTR_MENU_ZAIZHONG_SET_EDINGZHONGLIANG:
			//uiLcdMediumString("Kg", row, 15, 0);
			//uiLcdSmallString("T", row*2+1, 18, 0);
			
			//uiLcdDecimal(2, row, 12, 0, 1);
			//uiLcdMediumString(".", row, 13, 0);
			uiLcdDecimal(App.Weight.Rated_weight, row, 10, 0, 4);

			uiLcdSmallString("Kg", row*2+1, 19, 0);
			//uiLcdMediumString("Kg", row, 14, 0);
			//wa 
			
			break;
		case UISTR_MENU_ZAIZHONG_SET_YUJINGZAIHE:
			uiLcdDecimal(App.Weight.Warning_weight, row, 12, 0, 3);
			uiLcdMediumString("%", row, 15, 0);
			break;
		case UISTR_MENU_ZAIZHONG_SET_BAOJINGZAIHE:
			uiLcdDecimal(App.Weight.Alarm_weight, row, 12, 0, 3);
			uiLcdMediumString("%", row, 15, 0);
			break;
		case UISTR_MENU_ZAIZHONG_SET_ZHONGLIANGZHILING:
			//uiLcdMediumString("YES", row*2, 12, 0);
			uiLcdSmallString("YES", row*2+1, 17, 0);
			break;
		default:
			//uiLcdFatalError("DRAW_VALUE!");
			break;
	}

	if (bIsMulti)
	{
		//uiLcdMultiStringRightAlign(dwValue, row, bIsReverse);
	}
	else
	{
		//uiLcdDecimalRightAlign((int)dwValue, row, bIsReverse, 0);
	}
}

void uiProcMenuDraw(T_UI_MENUITEM *pUiStartMenuItem, int nCount,int nTopIndex, int nCurrentIndex)
{
	int i, nIndex;
	BOOL bSelected;

	uiLcdClear();

	printf("nCount==%d\r\n",nCount);
	printf("nTopIndex==%d\r\n",nTopIndex);
	printf("nCurrentIndex==%d\r\n",nCurrentIndex);

	//-----------------------------
	//��ʾ�˵�����
	switch((pUiStartMenuItem-1)->nMenuStr)//��type��Ӧ��ֵ
	{
		
		case UISTR_MENU:
			//i = UISTR_MENU_PINGCHENG_SET;
			uiLcd_1212_ch(UISTR_ZHUSHEZHICAIDAN, 0, 40, 5);
			
			break;
		case UISTR_MENU_ZAIZHONG_SET:
			uiLcd_1212_ch(UISTR_ZAIZHONG_SET, 0, 40, 4);
			//i = UISTR_MENU_PINGCHENG_SET;
			break;
		default:

			break;
	}
	//������ʾ���˵�
	//uiLcdMultiString(i, 0, 0, FALSE);

	
/*
	if (nCurrentIndex == 0)
		uiLcdMenuStatus(2);
	else
	if (nCurrentIndex == nCount - 1)
		uiLcdMenuStatus(1);
	else
		uiLcdMenuStatus(3);
*/

	for(i=0; i<3; i++)//ѭ����ʾһ�������µ�������Ŀ
	{
		nIndex = nTopIndex + i;
		if (nIndex >= nCount)
			break;

		if (nIndex == nCurrentIndex)
		{
			//uiLcdLineErase6x16(1+i, 0, 20, TRUE);
			//uiLcdLineErase8x16(1+i, 0, 16, TRUE);
			bSelected = TRUE;
			uiLcdDecimal_Small(nCurrentIndex+1, (1+i)*2, 0, 0, 1);
			uiLcdSmallString(">", (1+i)*2, 1,0);
			//uiLcdDecimal_Small(nCurrentIndex, (1+i)*2, 0, 0, 1);
		}
		else
		{
			bSelected = FALSE;
		}
		//uiLcdMultiString(g_uiStrNumber[nIndex], 1+i, 0, bSelected);


		
		//uiLcdMultiString(pUiStartMenuItem[nIndex].nMenuStr, 1+i, 1, bSelected);

		//uiLcdSmallString
		//uiLcdSmallString("1:",0,0,0);
		//��������Ӳ˵�Ŀ¼�Ļ滭
		switch(pUiStartMenuItem[nIndex].nMenuStr)//��type��Ӧ��ֵ
		{
			//���˵��µ���ʾ
			case UISTR_MENU_PINGCHENG_SET:
				//uiLcdSmallString("1:",0,0,1);
				uiLcd_1212_ch(UISTR_PINGCHENG_SET, (1+i)*2, HEAD_LEN, 4);
				break;
			case UISTR_MENU_ZAIZHONG_SET:
				//uiLcdSmallString("2:",2,0,1);
				uiLcd_1212_ch(UISTR_ZAIZHONG_SET, (1+i)*2, HEAD_LEN, 4);
				break;
			case UISTR_MENU_MIMA_SET:
				//uiLcdSmallString("3:",4,0,1);
				uiLcd_1212_ch(UISTR_MIMA_SET, (1+i)*2, HEAD_LEN, 4);
				break;
			case UISTR_MENU_ZIJIAN_SET:
				//uiLcdSmallString("4:",6,0,1);
				uiLcd_1212_ch(UISTR_ZHAUNTAIZIJIAN_SET, (1+i)*2, HEAD_LEN, 4);
				break;
				
			//���������µ���ʾ
			case UISTR_MENU_ZAIZHONG_SET_EDINGZHONGLIANG:
				//uiLcd_1212_ch(UISTR_ZAIZHONG_SET_EDINGZHONGLIANG, (1+i)*2, HEAD_LEN, 4);
				//uiLcd_1616_ch(UISTR_XXX_ZAIZHONG_SET_EDINGZHONGLIANG, (1+i)*2, HEAD_LEN, 4);
				uiLcd_1414_ch(UISTR_XXX_ZAIZHONG_SET_EDINGZHONGLIANG, (1+i)*2, HEAD_LEN, 4);
				break;
			case UISTR_MENU_ZAIZHONG_SET_YUJINGZAIHE:
				//uiLcd_1212_ch(UISTR_ZAIZHONG_SET_YUJINGZAIHE, (1+i)*2, HEAD_LEN, 4);
				uiLcd_1414_ch(UISTR_XXX_ZAIZHONG_SET_YUJINGZAIHE, (1+i)*2, HEAD_LEN, 4);
				break;
			case UISTR_MENU_ZAIZHONG_SET_BAOJINGZAIHE:
				//uiLcd_1212_ch(UISTR_ZAIZHONG_SET_BAOJINGZAIHE, (1+i)*2, HEAD_LEN, 4);
				uiLcd_1414_ch(UISTR_XXX_ZAIZHONG_SET_BAOJINGZAIHE, (1+i)*2, HEAD_LEN, 4);
				break;
				
			case UISTR_MENU_ZAIZHONG_SET_BIAODINGDIAN_ONE://���ñ궨��һ
				//uiLcd_1212_ch(UISTR_ZAIZHONG_SET_ZHONGLIANGZHILING, (1+i)*2, HEAD_LEN, 4);
				uiLcd_1414_ch(UISTR_XXX_ZAIZHONG_SET_BIAODINGDIAN_ONE, (1+i)*2, HEAD_LEN, 6);
				break;
			case UISTR_MENU_ZAIZHONG_SET_BIAODINGDIAN_TWO://���ñ궨���
				//uiLcd_1212_ch(UISTR_ZAIZHONG_SET_ZHONGLIANGZHILING, (1+i)*2, HEAD_LEN, 4);
				uiLcd_1414_ch(UISTR_XXX_ZAIZHONG_SET_BIAODINGDIAN_TWO, (1+i)*2, HEAD_LEN, 6);
				break;

				
			case UISTR_MENU_ZAIZHONG_SET_ZHONGLIANGZHILING:
				//uiLcd_1212_ch(UISTR_ZAIZHONG_SET_ZHONGLIANGZHILING, (1+i)*2, HEAD_LEN, 4);
				uiLcd_1414_ch(UISTR_XXX_ZAIZHONG_SET_ZHONGLIANGZHILING, (1+i)*2, HEAD_LEN, 4);
				break;


				
			default:
				//i = (pUiStartMenuItem-1)->nMenuStr;
				break;
		}
		//uiLcd_1212_ch(UISTR_PINGCHENG_SET, 1+i, 1, 4);
		

		if (pUiStartMenuItem[nIndex].nType == UIMENU_HASVALUE ||
			pUiStartMenuItem[nIndex].nType == UIMENU_CUSTOM_HASVALUE)
		{
			uiProcMenuDrawValue(&pUiStartMenuItem[nIndex], 1+i, bSelected);
		}
	}
}



void uiProcMenuSettingStart(int nPopupMenuTitle)
{
	
/*
	gsSetting.wDeviceNum = gsSystemInfo.sFshSystemInfo.wDeviceNum;
	gsSetting.wRepeatTime = gsSystemInfo.sFshSystemInfo.wRepeatTime;
	gsSetting.wRecordAlarm = gsSystemInfo.sFshSystemInfo.wRecordAlarm;
	gsSetting.wSpeakerPromptEn = gsSystemInfo.sFshSystemInfo.wSpeakerPromptEn;
	gsSetting.wBaudrate = gsSystemInfo.sFshSystemInfo.wBaudrate;
	gsSetting.wLCDContrast = gsSystemInfo.sFshSystemInfo.wLCDContrast;
	gsSetting.bLanguage = gsSystemInfo.sFshSystemInfo.bLanguage;
	gsSetting.bMaxAdminNum = gsSystemInfo.sFshSystemInfo.bMaxAdminNum;
	gsSetting.bAtendFunctionKeyEn = gsSystemInfo.sFshSystemInfo.bAtendFunctionKeyEn;
	gsSetting.bLockEn = gsSystemInfo.sFshSystemInfo.wLockEn;
	gsSetting.bWiegandIn = gsSystemInfo.sFshSystemInfo.bWiegandType;
	gsSetting.bVolume = gsSystemInfo.sFshSystemInfo.bVolume;

	gsSetting.wPortNum = gsSystemInfo.sFshSystemInfo.wPortNum;
	gsSetting.dwCommPassword = gsSystemInfo.sFshSystemInfo.dwCommPassword;
	gsSetting.ipAddress = gsSystemInfo.sFshSystemInfo.ipAddress;
	gsSetting.ipSubnetMask = gsSystemInfo.sFshSystemInfo.ipSubnetMask;
	gsSetting.ipDefaultGateway = gsSystemInfo.sFshSystemInfo.ipDefaultGateway;

*/

}


//����ϵͳ����
UINT8 SaveSystemInfo(void)
{
	UINT8 *pData;
	UINT16 wTemp;
	UINT32 i;
/*
		pPoint = (UINT8 *)&App;
		pPoint += 2;//ȥ��ǰ�������ӵ�2λ��ʼУ��
		for(i = 0x00;i < sizeof(App) -2;i++)
		{
			wCheckSum += *pPoint++;
		}
*/
	pData = (UINT8 *)&App;
	pData += 2;
	wTemp = 0x00;
	for(i = 0;i < sizeof(App) - 2;i++)
	{
		wTemp += *pData++;
	}
	App.wLicCheckSum = wTemp;


	memcpy(&App_Flash,&App,sizeof(App));
	
	Flash_WriteInfo((u8*)&App,APP_SETUP_ADD,sizeof(App));
	
	return TRUE;
}

void uiProcMenuSettingEnd(int nPopupMenuTitle)
{
	BOOL bSave0 = FALSE;

	//printf("nPopupMenuTitle===%d\r\n",nPopupMenuTitle);
	//printf("UISTR_MENU_ZAIZHONG_SET===%d\r\n",UISTR_MENU_ZAIZHONG_SET);
	if(nPopupMenuTitle == UISTR_MENU_ZAIZHONG_SET)
	{
		//printf("456\r\n");
		
		if(App.Weight.Rated_weight  != App_Flash.Weight.Rated_weight )
		{
			bSave0 = TRUE;
		}
		if(App.Weight.Warning_weight !=  App_Flash.Weight.Warning_weight)
		{
			bSave0 = TRUE;
		}
		if(App.Weight.Alarm_weight != App_Flash.Weight.Alarm_weight)
		{
			bSave0 = TRUE;
		}
		if(App.Weight.calibrate_two_suffer!= App_Flash.Weight.calibrate_two_suffer)
		{
			bSave0 = TRUE;
		}

		
		if(bSave0 == TRUE)
		{
			
			SaveSystemInfo();

		}
	}
	
	//if (nPopupMenuTitle == UISTR_MENU_TIP_SET)
	//{
		
	//}
}


void uiProcMenu(int nPopupMenuTitle)
{
	int nCount, nTopIndex = 0, nCurrentIndex = 0;
	T_UI_MENUITEM *pUiStartMenuItem = &g_uiMenuItems[0], *pUiEndMenuItem;

	u8 nKey;
	u8 bRet;

	
	uiProcMenuSettingStart(nPopupMenuTitle);

	//////////////////////////////////////////////////////////////////////////
	while (pUiStartMenuItem->nMenuStr != nPopupMenuTitle ||
		(int)pUiStartMenuItem->nType < 0)
	{
		pUiStartMenuItem++;
	}
	pUiStartMenuItem++;//��1

	pUiEndMenuItem = pUiStartMenuItem;
	while ((int)pUiEndMenuItem->nType < 0)//�ҵڽ���λ��
	{
		pUiEndMenuItem++;
	}

	nCount = ((UINT32)pUiEndMenuItem - (UINT32)pUiStartMenuItem)/sizeof(T_UI_MENUITEM);

	//////////////////////////////////////////////////////////////////////////
	//uiProcMenuSysInfoViewInit(pUiStartMenuItem);
	
	//////////////////////////////////////////////////////////////////////////
	uiProcMenuDraw(pUiStartMenuItem, nCount, nTopIndex, nCurrentIndex);

	while(1)
	{
		
		if( (nKey = uiKeyGetKey()) == UIKEY_ESC)
		{
			uiProcMenuSettingEnd(nPopupMenuTitle);
			break;
		}

		switch(nKey) 
		{
			case UIKEY_OK:
			/*	
				if (pUiStartMenuItem[nCurrentIndex].nType == UIMENU_POPUP &&
					uiProcMenuCheckPrivilege(pUiStartMenuItem[nCurrentIndex].nMenuStr))
				{
					

				}
				else if (pUiStartMenuItem[nCurrentIndex].nType == UIMENU_HASVALUE)
				{
					uiProcMenuHasValue(nPopupMenuTitle, &pUiStartMenuItem[nCurrentIndex], 
						1 + (nCurrentIndex - nTopIndex),nCurrentIndex);
				}
				else if (pUiStartMenuItem[nCurrentIndex].nType == UIMENU_CUSTOM_HASVALUE)
				{
					uiProcMenuSysInfoViewProcess(&pUiStartMenuItem[nCurrentIndex]);
				}
				else if (pUiStartMenuItem[nCurrentIndex].nType == UIMENU_CUSTOM)
				{
					uiProcMenuCustom(nPopupMenuTitle, &pUiStartMenuItem[nCurrentIndex],1 + (nCurrentIndex - nTopIndex));
				}
			*/
				if (pUiStartMenuItem[nCurrentIndex].nType == UIMENU_POPUP)//�����������˵���ѡ��
				{
					
					uiProcMenu(pUiStartMenuItem[nCurrentIndex].nMenuStr);
					
				}
				else if(pUiStartMenuItem[nCurrentIndex].nType == UIMENU_HASVALUE)//��ֵ�ı���������
				{

					uiProcMenuHasValue(nPopupMenuTitle, &pUiStartMenuItem[nCurrentIndex], 
						1 + (nCurrentIndex - nTopIndex),nCurrentIndex);
				}
				else if (pUiStartMenuItem[nCurrentIndex].nType == UIMENU_CUSTOM)
				{
					uiProcMenuCustom(nPopupMenuTitle, &pUiStartMenuItem[nCurrentIndex],1 + (nCurrentIndex - nTopIndex));
				}
				
				uiProcMenuDraw(pUiStartMenuItem, nCount, nTopIndex, nCurrentIndex);

				break;
			case UIKEY_UP:
				if (nCurrentIndex > 0)
					nCurrentIndex --;

				if (nCurrentIndex < nTopIndex)
					nTopIndex = nCurrentIndex;

				uiProcMenuDraw(pUiStartMenuItem, nCount, nTopIndex, nCurrentIndex);
				break;
			case UIKEY_DOWN:
				if (nCurrentIndex < nCount - 1)
					nCurrentIndex ++;

				if (nCurrentIndex > nTopIndex + 2)
					nTopIndex = nCurrentIndex - 2;

				uiProcMenuDraw(pUiStartMenuItem, nCount, nTopIndex, nCurrentIndex);
				break;
			default:
				break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////


void uiProcKey(u8 nKey,Menu_Parameter *Parameter)
{


	if(uiKeyIsDigit(nKey) )	/*�жϰ����Ƿ�������*/
	{
		
		//Target_F += (nKey - '0');
		Target_F = Target_F * 10 + (int)nKey - '0';
		if(Target_F > 99)
		{
			Target_F =  (nKey - '0');
		}
		if(Target_F < 1)
		{
			Target_F =  1;
		}
			
	}
	else if (nKey == UIKEY_UP)//Ŀ��¥���1
	{
		Target_F = Target_F+1;
		if(Target_F > 99)
		{
			Target_F =  99;
		}

		HB_Send_Realy_CMD(CMD_RELAY_UP);
	}
	else if (nKey == UIKEY_DOWN)//Ŀ��¥���1
	{
		Target_F = Target_F-1;
		if(Target_F < 1)
		{
			Target_F =  1;
		}
		HB_Send_Realy_CMD(CMD_RELAY_DOWN);
	}
	else if (nKey == UIKEY_ESC)//Ŀ��¥���˳�
	{
		Target_F = 0;
		//if(Target_F < 1)
		//{
			//Target_F =  1;
		//}
		HB_Send_Realy_CMD(CMD_RELAY_STOP);
	}
	else if (nKey == UIKEY_OK)//Ŀ��¥������
	{
		//����Ŀ��¥����������
		get_weight_clear_value();
		
	}
	else if (nKey == UIKEY_MENU)
	{			

		//��֤����
		if(Pwd_Identify_Process())
		{
			uiProcMenu(UISTR_MENU);
		}
		
		//��ʾ
		
		uiProcMainDraw(TRUE,Parameter);	
	}



}

void uiProcMain(void)
{
	u8 nKey;
	//u32 adcx,adcx1;
	//float Temperature;
	//float temp;
	//int i;
	//u8  xxxx;
	
	Menu_Parameter Parameter;
	Target_F = 1;//��ʼ��¥��
	//��������ʹ��
	nKey  = uiKeyGetKey();
	switch(nKey)
	{
		case UIKEY_2:		
			Pwd_Enroll_Process();
			break;
		default:
			break;
	}
	FlushUart2();
	uiProcMainDraw(TRUE,&Parameter);
	uiProcMenuBuild();//�����˵�Ŀ¼

	//for(xxxx= 0;xxxx<0x41;xxxx++)
	//{
		
		//WTV_Voice(xxxx);
		//DelayMs(1000*4);
	//}
	DelayMs(1000);
	HB_Send_Last_Count(20000);
	while(1)
	{
		nKey  = uiKeyGetKey();//��������¥��
		if(nKey != UIKEY_NONE)
		{
			uiProcKey(nKey,&Parameter);
		}
		//���ڴ����ڽ���Э�鴦��
		if(Rcv_Cmd() == 1)
		{
			//���ڿ�ʼУ��ͷ������ݺ���ûʵ�֣�������ݴ�����ʾ������
			//ͷ��β��ͬ������ʱ��ʾ���룬��ʾ�ܴ�����䣬��ʵ�ڿ��ƶ�
			//��û�б仯�ġ�����Ҫ�����£��Ժ���ʾ���ⶼ������������ֵ�����
			//������Ҫ��������
			if(Cmd_Rep_valid(fram_data_buff,&fram_len))
			{

				if(fram_data_buff[0] == CMD_RT_DATA)
				{
					//�õ�16λ�������
					Handle_ERR_Code(&Parameter);
					//�õ�������ֵ
					Handle_Weight(&Parameter);
				}
				if(fram_data_buff[0] == CMD_LEVEL_UPDATA_CURRENT_COUNT)
				{
					int adxxx = 0;
					//20141216 ������ʾ��ʱƽ�����������ֵ
					adxxx |= fram_data_buff[5]<<24;
					adxxx |= fram_data_buff[4]<<16;
					adxxx |= fram_data_buff[3]<<8;
					adxxx |= fram_data_buff[2];

					uiLcdDecimal(adxxx,3,0,0,7);
				}
				if(fram_data_buff[0] == CMD_LEVEL_UPDATA_LAST_COUNT)
				{
					App.Floor_Last_Count |= fram_data_buff[5]<<24;
					App.Floor_Last_Count |= fram_data_buff[4]<<16;
					App.Floor_Last_Count |= fram_data_buff[3]<<8;
					App.Floor_Last_Count |= fram_data_buff[2];
					
					if(App.Floor_Last_Count != App_Flash.Floor_Last_Count)
					{
						SaveSystemInfo();
					}

				}
				if(fram_data_buff[0] == CMD_LEVEL_UPDATA_FLOOR)
				{
					u8	floor_num = 0;
					floor_num = fram_data_buff[3];//¥����λ��
					
					//App.floor_tmp[floor_num-1].floor_num = floor_num;
					
					
					//App.floor_tmp[floor_num-1].floor_count |= fram_data_buff[7]<<24;
					//App.floor_tmp[floor_num-1].floor_count |= fram_data_buff[6]<<16;
					//App.floor_tmp[floor_num-1].floor_count |= fram_data_buff[5]<<8;
					//App.floor_tmp[floor_num-1].floor_count |= fram_data_buff[4];
					
				}


			}
			
		}

		Handle_Master(&Parameter);

		uiProcMainDraw(FALSE,&Parameter);//������ʾ
		Parameter.Parameter_Change_Flag =0;
		Parameter.Parameter_Change_Position=0;
	}

}



	




////////�����Ǳ��Ż�������Ĵ���
//20141203����������ȥ�����Ż���void uiProcKey(u8 nKey,Menu_Parameter *Parameter)
/*
			if(uiKeyIsDigit(nKey) )	//�жϰ����Ƿ�������
			{
				
				//Target_F += (nKey - '0');
				Target_F = Target_F * 10 + (int)nKey - '0';
				if(Target_F > 99)
				{
					Target_F =  (nKey - '0');
				}
				if(Target_F < 1)
				{
					Target_F =  1;
				}
					
			}
			else if (nKey == UIKEY_UP)//Ŀ��¥���1
			{
				Target_F = Target_F+1;
				if(Target_F > 99)
				{
					Target_F =  99;
				}
			}
			else if (nKey == UIKEY_DOWN)//Ŀ��¥���1
			{
				Target_F = Target_F-1;
				if(Target_F < 1)
				{
					Target_F =  1;
				}
			}
			else if (nKey == UIKEY_ESC)//Ŀ��¥���˳�
			{
				Target_F = 0;
				//if(Target_F < 1)
				//{
					//Target_F =  1;
				//}
			}
			else if (nKey == UIKEY_OK)//Ŀ��¥������
			{
				//����Ŀ��¥����������
				get_weight_clear_value();
				
			}
			else if (nKey == UIKEY_MENU)
			{			

				//��֤����
				if(Pwd_Identify_Process())
				{
					uiProcMenu(UISTR_MENU);
				}
				
				//��ʾ
				
				uiProcMainDraw(TRUE,&Parameter);	
			}
*/
/*


*/

