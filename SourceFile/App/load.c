#include "system.h"
#include "app.h"

#define MAX_LOAD_BYTE_LENS		4	
#define LOAD_ROW	1
#define LOAD_COL		2

//设置标定点一
u8 Load_Set_One(void)
{
	u8 pcRet[MAX_LOAD_BYTE_LENS];
	u8 nKey;
	int i, nPos = 0;
	u8 nMaxLen = MAX_LOAD_BYTE_LENS;
	pcRet[0] = 0;
	uiLcdClear();
	uiLcd_1212_ch(UISTR_INPUT_LOAD,0,4,8);

	//uiLcdLineErase8x16(2, 5, 11, 1);
	uiLcdMediumString("____", LOAD_ROW, LOAD_COL,0);

	uiLcd_1414_ch(UISTR_XXX_SAVE, 6, 4,2);
	uiLcdMediumString("OK", 3 ,4,0);

	uiLcd_1414_ch(UISTR_XXX_DEL, 6,76,2);
	uiLcdMediumString("ESC", 3, 13,0);
	while(1)
	{
		//读取此时上传的重量
		if(Rcv_Cmd() == 1)
		{

			if(fram_data_buff[0] == CMD_RT_DATA)
			{
				//得到16位错误代码

				//得到重量的值
				
				App.Weight_Tmp =  fram_data_buff[5];
				
				App.Weight_Tmp = (App.Weight_Tmp << 8) |fram_data_buff[4];

				//printf("App.Weight_Tmp===%d\r\n",App.Weight_Tmp);

				uiLcdDecimal((u16)App.Weight_Tmp,LOAD_ROW, LOAD_COL+7,0,4);
			}
		}
		
        		nKey = uiKeyGetKey();
		if(nKey == UIKEY_ESC)
			break;
		if(nKey == UIKEY_NONE)
			continue;
		if(nKey == UIKEY_OK)
		{
			printf("pcRet[1-4]== %d\r\n",(pcRet[0]-'0')*1000+(pcRet[1]-'0')*100+(pcRet[2]-'0')*10+(pcRet[3]-'0'));

			if (nPos == 1)
			{
				printf("pcRet[1]== %d\r\n",(pcRet[0]-'0'));
				App.Weight.calibrate_one_suffer = (pcRet[0]-'0');
			}	
			else if (nPos == 2)
			{
				printf("pcRet[2]== %d\r\n",(pcRet[0]-'0')*10+(pcRet[1]-'0'));
				App.Weight.calibrate_one_suffer = (pcRet[0]-'0')*10+(pcRet[1]-'0');
			}
			else if (nPos == 3)
			{
				printf("pcRet[3]== %d\r\n",(pcRet[0]-'0')*100+(pcRet[1]-'0')*10+(pcRet[2]-'0'));
				App.Weight.calibrate_one_suffer = (pcRet[0]-'0')*100+(pcRet[1]-'0')*10+(pcRet[2]-'0');
			}
			else if (nPos == 4)
			{
				printf("pcRet[4]== %d\r\n",(pcRet[0]-'0')*1000+(pcRet[1]-'0')*100+(pcRet[2]-'0')*10+(pcRet[3]-'0'));
				App.Weight.calibrate_one_suffer = (pcRet[0]-'0')*1000+(pcRet[1]-'0')*100+(pcRet[2]-'0')*10+(pcRet[3]-'0');
			}
			else
			{
				printf("pcRet[0]== %d\r\n",(pcRet[0]-'0')*1000+(pcRet[1]-'0')*100+(pcRet[2]-'0')*10+(pcRet[3]-'0'));
				//提示没有输入
				continue;

			}
			
			App.Weight.calibrate_one_ad_suffer = (u16)App.Weight_Tmp;//将采集值赋给保存
			
		}
		
		
		if(nKey == UIKEY_MENU)//删除
		{
			if (nPos == 0)//当读取位置== 0时，
			{	
				nPos = 0;
				for (i=0; i<MAX_LOAD_BYTE_LENS; i++)
				{	
					pcRet[i] = '0';
				}
				uiLcdMediumString("____", LOAD_ROW, LOAD_COL,0);
				continue;
			}
			nPos--;
			//printf("pcRet[%d]== %d\r\n",nPos,pcRet[nPos]);
			if(nPos == 0)
			{
				uiLcdMediumString("____", LOAD_ROW, LOAD_COL,0);
				pcRet[0] = '0';
				pcRet[1] = '0';
				pcRet[2] = '0';
				pcRet[3] = '0';
				//uiLcdDecimal(pcRet[0]-'0',LOAD_ROW, LOAD_COL+3,0,1);//从右到左显示
			}else if(nPos == 1)
			{
				uiLcdMediumString("____", LOAD_ROW, LOAD_COL,0);
				uiLcdDecimal(pcRet[0]-'0',LOAD_ROW, LOAD_COL+3,0,1);//从右到左显示
				pcRet[1] = '0';
				pcRet[2] = '0';
				pcRet[3] = '0';
				//uiLcdDecimal(pcRet[1]-'0',LOAD_ROW, LOAD_COL+3,0,1);//从右到左显示
			}else if(nPos == 2)
			{
				uiLcdMediumString("____", LOAD_ROW, LOAD_COL,0);
				uiLcdDecimal(pcRet[0]-'0',LOAD_ROW, LOAD_COL+2,0,1);//从右到左显示
				uiLcdDecimal(pcRet[1]-'0',LOAD_ROW, LOAD_COL+3,0,1);//从右到左显示
				pcRet[2] = '0';
				pcRet[3] = '0';
				//uiLcdDecimal(pcRet[2]-'0',LOAD_ROW, LOAD_COL+3,0,1);//从右到左显示
			}else if(nPos == 3)
			{
				uiLcdMediumString("____", LOAD_ROW, LOAD_COL,0);
				uiLcdDecimal(pcRet[0]-'0',LOAD_ROW, LOAD_COL+1,0,1);//从右到左显示
				uiLcdDecimal(pcRet[1]-'0',LOAD_ROW, LOAD_COL+2,0,1);//从右到左显示
				uiLcdDecimal(pcRet[2]-'0',LOAD_ROW, LOAD_COL+3,0,1);//从右到左显示
				pcRet[3] = '0';
				//uiLcdDecimal(pcRet[3]-'0',LOAD_ROW, LOAD_COL+0,0,1);//从右到左显示
			}else
			{

			}

		}

		if (uiKeyIsDigit(nKey))
		{
			if (nPos == nMaxLen)//当读取位置大于4时，密码存储值清理
			{	
				//printf("pcRet[%d]== %d \r\n",nPos,pcRet[nPos-1]);
				//提示输入的数目超过定数，重新输入

				
				nPos = 0;
				for (i=0; i<MAX_LOAD_BYTE_LENS; i++)
				{	
					pcRet[i] = '0';
				}
				uiLcdMediumString("____", LOAD_ROW, LOAD_COL,0);
				continue;
			}
			
			pcRet[nPos] = nKey;
			//printf("pcRet[%d]== %d \r\n",nPos,pcRet[nPos]);
			nPos++;
			//printf("pcRet[%d]== %d\r\n",nPos,pcRet[nPos-1]);

			if(nPos == 1)
			{
				uiLcdDecimal(pcRet[0]-'0',LOAD_ROW, LOAD_COL+3,0,1);//从右到左显示
				pcRet[1] = '0';
				pcRet[2] = '0';
				pcRet[3] = '0';
				
			}else if(nPos == 2)
			{
				uiLcdDecimal(pcRet[0]-'0',LOAD_ROW, LOAD_COL+2,0,1);//从右到左显示
				uiLcdDecimal(pcRet[1]-'0',LOAD_ROW, LOAD_COL+3,0,1);//从右到左显示
				pcRet[2] = '0';
				pcRet[3] = '0';
			}else if(nPos == 3)
			{
				uiLcdDecimal(pcRet[0]-'0',LOAD_ROW, LOAD_COL+1,0,1);//从右到左显示
				uiLcdDecimal(pcRet[1]-'0',LOAD_ROW, LOAD_COL+2,0,1);//从右到左显示
				uiLcdDecimal(pcRet[2]-'0',LOAD_ROW, LOAD_COL+3,0,1);//从右到左显示
				pcRet[3] = '0';
			}else if(nPos == 4)
			{
				uiLcdDecimal(pcRet[0]-'0',LOAD_ROW, LOAD_COL+0,0,1);//从右到左显示
				uiLcdDecimal(pcRet[1]-'0',LOAD_ROW, LOAD_COL+1,0,1);//从右到左显示
				uiLcdDecimal(pcRet[2]-'0',LOAD_ROW, LOAD_COL+2,0,1);//从右到左显示
				uiLcdDecimal(pcRet[3]-'0',LOAD_ROW, LOAD_COL+3,0,1);//从右到左显示
			}else
			{

			}

		}
	}
	return FALSE;	
}


//设置标定点
u8 Load_Set_Calibration(u8 calibration)
{
	u8 pcRet[MAX_LOAD_BYTE_LENS];
	u8 nKey;
	int i, nPos = 0;
	u8 nMaxLen = MAX_LOAD_BYTE_LENS;
	pcRet[0] = 0;
	uiLcdClear();
	//uiLcd_1212_ch(UISTR_INPUT_LOAD,0,4,8);

	if(calibration == 1)
	{
		uiLcd_1212_ch(UISTR_INPUT_LOAD,0,4,8);
	}
	if(calibration == 2)
	{
		uiLcd_1212_ch(UISTR_INPUT_LOAD,0,4,7);
		uiLcd_1212_ch(UISTR_INPUT_LOAD_TWO,0,88,1);
	}

	//uiLcdLineErase8x16(2, 5, 11, 1);
	uiLcdMediumString("____", LOAD_ROW, LOAD_COL,0);

	uiLcd_1414_ch(UISTR_XXX_SAVE, 6, 4,2);
	uiLcdMediumString("OK", 3 ,4,0);

	uiLcd_1414_ch(UISTR_XXX_DEL, 6,76,2);
	uiLcdMediumString("ESC", 3, 13,0);
	while(1)
	{
		//读取此时上传的重量
		if(Rcv_Cmd() == 1)
		{

			if(fram_data_buff[0] == CMD_RT_DATA)
			{
				//得到16位错误代码

				//得到重量的值
				
				App.Weight_Tmp =  fram_data_buff[5];
				
				App.Weight_Tmp = (App.Weight_Tmp << 8) |fram_data_buff[4];

				//printf("App.Weight_Tmp===%d\r\n",App.Weight_Tmp);

				uiLcdDecimal((u16)App.Weight_Tmp,LOAD_ROW, LOAD_COL+7,0,4);
			}
		}
		
        		nKey = uiKeyGetKey();
		if(nKey == UIKEY_ESC)
			break;
		if(nKey == UIKEY_NONE)
			continue;
		if(nKey == UIKEY_OK)
		{
			//printf("pcRet[1-4]== %d\r\n",(pcRet[0]-'0')*1000+(pcRet[1]-'0')*100+(pcRet[2]-'0')*10+(pcRet[3]-'0'));

			if(calibration == 1)
			{
				if (nPos == 1)
				{
					printf("pcRet[1]== %d\r\n",(pcRet[0]-'0'));
					App.Weight.calibrate_one_suffer = (pcRet[0]-'0');
				}	
				else if (nPos == 2)
				{
					printf("pcRet[2]== %d\r\n",(pcRet[0]-'0')*10+(pcRet[1]-'0'));
					App.Weight.calibrate_one_suffer = (pcRet[0]-'0')*10+(pcRet[1]-'0');
				}
				else if (nPos == 3)
				{
					printf("pcRet[3]== %d\r\n",(pcRet[0]-'0')*100+(pcRet[1]-'0')*10+(pcRet[2]-'0'));
					App.Weight.calibrate_one_suffer = (pcRet[0]-'0')*100+(pcRet[1]-'0')*10+(pcRet[2]-'0');
				}
				else if (nPos == 4)
				{
					printf("pcRet[4]== %d\r\n",(pcRet[0]-'0')*1000+(pcRet[1]-'0')*100+(pcRet[2]-'0')*10+(pcRet[3]-'0'));
					App.Weight.calibrate_one_suffer = (pcRet[0]-'0')*1000+(pcRet[1]-'0')*100+(pcRet[2]-'0')*10+(pcRet[3]-'0');
				}
				else
				{
					printf("pcRet[0]== %d\r\n",(pcRet[0]-'0')*1000+(pcRet[1]-'0')*100+(pcRet[2]-'0')*10+(pcRet[3]-'0'));
					//提示没有输入
					continue;

				}
				App.Weight.calibrate_one_ad_suffer = (u16)App.Weight_Tmp;//将采集值赋给保存
				
			}
			
			if(calibration == 2)
			{
				if (nPos == 1)
				{
					printf("pcRet[1]== %d\r\n",(pcRet[0]-'0'));
					App.Weight.calibrate_two_suffer = (pcRet[0]-'0');
				}	
				else if (nPos == 2)
				{
					printf("pcRet[2]== %d\r\n",(pcRet[0]-'0')*10+(pcRet[1]-'0'));
					App.Weight.calibrate_two_suffer = (pcRet[0]-'0')*10+(pcRet[1]-'0');
				}
				else if (nPos == 3)
				{
					printf("pcRet[3]== %d\r\n",(pcRet[0]-'0')*100+(pcRet[1]-'0')*10+(pcRet[2]-'0'));
					App.Weight.calibrate_two_suffer = (pcRet[0]-'0')*100+(pcRet[1]-'0')*10+(pcRet[2]-'0');
				}
				else if (nPos == 4)
				{
					printf("pcRet[4]== %d\r\n",(pcRet[0]-'0')*1000+(pcRet[1]-'0')*100+(pcRet[2]-'0')*10+(pcRet[3]-'0'));
					App.Weight.calibrate_two_suffer = (pcRet[0]-'0')*1000+(pcRet[1]-'0')*100+(pcRet[2]-'0')*10+(pcRet[3]-'0');
				}
				else
				{
					printf("pcRet[0]== %d\r\n",(pcRet[0]-'0')*1000+(pcRet[1]-'0')*100+(pcRet[2]-'0')*10+(pcRet[3]-'0'));
					//提示没有输入
					continue;

				}
				App.Weight.calibrate_two_ad_suffer = (u16)App.Weight_Tmp;//将采集值赋给保存

			}
			return TRUE;
			
		}
		
		
		if(nKey == UIKEY_MENU)//删除
		{
			if (nPos == 0)//当读取位置== 0时，
			{	
				nPos = 0;
				for (i=0; i<MAX_LOAD_BYTE_LENS; i++)
				{	
					pcRet[i] = '0';
				}
				uiLcdMediumString("____", LOAD_ROW, LOAD_COL,0);
				continue;
			}
			nPos--;
			//printf("pcRet[%d]== %d\r\n",nPos,pcRet[nPos]);
			if(nPos == 0)
			{
				uiLcdMediumString("____", LOAD_ROW, LOAD_COL,0);
				pcRet[0] = '0';
				pcRet[1] = '0';
				pcRet[2] = '0';
				pcRet[3] = '0';
				//uiLcdDecimal(pcRet[0]-'0',LOAD_ROW, LOAD_COL+3,0,1);//从右到左显示
			}else if(nPos == 1)
			{
				uiLcdMediumString("____", LOAD_ROW, LOAD_COL,0);
				uiLcdDecimal(pcRet[0]-'0',LOAD_ROW, LOAD_COL+3,0,1);//从右到左显示
				pcRet[1] = '0';
				pcRet[2] = '0';
				pcRet[3] = '0';
				//uiLcdDecimal(pcRet[1]-'0',LOAD_ROW, LOAD_COL+3,0,1);//从右到左显示
			}else if(nPos == 2)
			{
				uiLcdMediumString("____", LOAD_ROW, LOAD_COL,0);
				uiLcdDecimal(pcRet[0]-'0',LOAD_ROW, LOAD_COL+2,0,1);//从右到左显示
				uiLcdDecimal(pcRet[1]-'0',LOAD_ROW, LOAD_COL+3,0,1);//从右到左显示
				pcRet[2] = '0';
				pcRet[3] = '0';
				//uiLcdDecimal(pcRet[2]-'0',LOAD_ROW, LOAD_COL+3,0,1);//从右到左显示
			}else if(nPos == 3)
			{
				uiLcdMediumString("____", LOAD_ROW, LOAD_COL,0);
				uiLcdDecimal(pcRet[0]-'0',LOAD_ROW, LOAD_COL+1,0,1);//从右到左显示
				uiLcdDecimal(pcRet[1]-'0',LOAD_ROW, LOAD_COL+2,0,1);//从右到左显示
				uiLcdDecimal(pcRet[2]-'0',LOAD_ROW, LOAD_COL+3,0,1);//从右到左显示
				pcRet[3] = '0';
				//uiLcdDecimal(pcRet[3]-'0',LOAD_ROW, LOAD_COL+0,0,1);//从右到左显示
			}else
			{

			}

		}

		if (uiKeyIsDigit(nKey))
		{
			if (nPos == nMaxLen)//当读取位置大于4时，密码存储值清理
			{	
				//printf("pcRet[%d]== %d \r\n",nPos,pcRet[nPos-1]);
				//提示输入的数目超过定数，重新输入

				
				nPos = 0;
				for (i=0; i<MAX_LOAD_BYTE_LENS; i++)
				{	
					pcRet[i] = '0';
				}
				uiLcdMediumString("____", LOAD_ROW, LOAD_COL,0);
				continue;
			}
			
			pcRet[nPos] = nKey;
			//printf("pcRet[%d]== %d \r\n",nPos,pcRet[nPos]);
			nPos++;
			//printf("pcRet[%d]== %d\r\n",nPos,pcRet[nPos-1]);

			if(nPos == 1)
			{
				uiLcdDecimal(pcRet[0]-'0',LOAD_ROW, LOAD_COL+3,0,1);//从右到左显示
				pcRet[1] = '0';
				pcRet[2] = '0';
				pcRet[3] = '0';
				
			}else if(nPos == 2)
			{
				uiLcdDecimal(pcRet[0]-'0',LOAD_ROW, LOAD_COL+2,0,1);//从右到左显示
				uiLcdDecimal(pcRet[1]-'0',LOAD_ROW, LOAD_COL+3,0,1);//从右到左显示
				pcRet[2] = '0';
				pcRet[3] = '0';
			}else if(nPos == 3)
			{
				uiLcdDecimal(pcRet[0]-'0',LOAD_ROW, LOAD_COL+1,0,1);//从右到左显示
				uiLcdDecimal(pcRet[1]-'0',LOAD_ROW, LOAD_COL+2,0,1);//从右到左显示
				uiLcdDecimal(pcRet[2]-'0',LOAD_ROW, LOAD_COL+3,0,1);//从右到左显示
				pcRet[3] = '0';
			}else if(nPos == 4)
			{
				uiLcdDecimal(pcRet[0]-'0',LOAD_ROW, LOAD_COL+0,0,1);//从右到左显示
				uiLcdDecimal(pcRet[1]-'0',LOAD_ROW, LOAD_COL+1,0,1);//从右到左显示
				uiLcdDecimal(pcRet[2]-'0',LOAD_ROW, LOAD_COL+2,0,1);//从右到左显示
				uiLcdDecimal(pcRet[3]-'0',LOAD_ROW, LOAD_COL+3,0,1);//从右到左显示
			}else
			{

			}

		}
	}
	return FALSE;	
}

//由两个校准值计算刻度和零点
void get_weight_clear_value(void)
{
	u32 operate_temp;
	u8 nKey;
	u32 ad_temp;
	//计算刻度（为了避免舍去误差，不是真正的刻度）
	if(App.Weight.calibrate_one_ad_suffer > App.Weight.calibrate_two_ad_suffer )
	{
		App.Weight.calibrate_ad_sub_value = App.Weight.calibrate_one_ad_suffer - App.Weight.calibrate_two_ad_suffer;
		App.Weight.calibrate_sub_value = App.Weight.calibrate_one_suffer - App.Weight.calibrate_two_suffer;
	}
	else if(App.Weight.calibrate_one_ad_suffer == App.Weight.calibrate_two_ad_suffer)
	{
		App.Weight.calibrate_ad_sub_value = 1;
		App.Weight.calibrate_sub_value = App.Weight.calibrate_one_suffer - App.Weight.calibrate_two_suffer;
	}
	else
	{
		App.Weight.calibrate_ad_sub_value = App.Weight.calibrate_two_ad_suffer - App.Weight.calibrate_one_ad_suffer;
		App.Weight.calibrate_sub_value = App.Weight.calibrate_two_suffer - App.Weight.calibrate_one_suffer;
	}

	printf("App.Weight.calibrate_sub_value== %d\r\n",App.Weight.calibrate_sub_value);
	printf("App.Weight.calibrate_ad_sub_value== %d\r\n",App.Weight.calibrate_ad_sub_value);
	
	//计算零点并保存
	operate_temp=App.Weight.calibrate_one_suffer;

	//App.Weight.calibrate_slope = 
	
	App.Weight.calibrate_suffer=(operate_temp*App.Weight.calibrate_ad_sub_value)/App.Weight.calibrate_sub_value;
	if(App.Weight.calibrate_one_ad_suffer >= App.Weight.calibrate_suffer )
	{	
		App.Weight.weight_clear_ad_value=App.Weight.calibrate_one_ad_suffer - App.Weight.calibrate_suffer;
		App.Weight.weight_clear_ad_value_sign=0;
	}
	else
	{
		App.Weight.weight_clear_ad_value= App.Weight.calibrate_suffer - App.Weight.calibrate_one_ad_suffer;
		App.Weight.weight_clear_ad_value_sign=1;
	}

	printf("operate_temp== %d\r\n",operate_temp);
	printf("App.Weight.calibrate_one_suffer== %d\r\n",App.Weight.calibrate_one_suffer);
	printf("App.Weight.calibrate_two_suffer== %d\r\n",App.Weight.calibrate_two_suffer);
	printf("App.Weight.calibrate_one_ad_suffer== %d\r\n",App.Weight.calibrate_one_ad_suffer);
	printf("App.Weight.calibrate_two_ad_suffer== %d\r\n",App.Weight.calibrate_two_ad_suffer);
	printf("App.Weight.calibrate_suffer== %d\r\n",App.Weight.calibrate_suffer);
	printf("App.Weight.weight_clear_ad_value== %d\r\n",App.Weight.weight_clear_ad_value);
	printf("App.Weight.weight_clear_ad_value_sign== %d\r\n",App.Weight.weight_clear_ad_value_sign);


	uiLcdClear();
	//测试使用
	while(1)
	{
		
		//读取此时上传的重量
		if(Rcv_Cmd() == 1)
		{

			if(fram_data_buff[0] == CMD_RT_DATA)
			{
				//得到16位错误代码

				//得到重量的值
				
				App.Weight_Tmp =  fram_data_buff[5];
				
				App.Weight_Tmp = (App.Weight_Tmp << 8) |fram_data_buff[4];

				//printf("App.Weight_Tmp===%d\r\n",App.Weight_Tmp);

				uiLcdDecimal((u16)App.Weight_Tmp,LOAD_ROW+2, LOAD_COL+7,0,4);
			}
		}

		uiLcdDecimal(App.Weight.calibrate_one_suffer,LOAD_ROW-1, LOAD_COL-2,0,4);
		uiLcdDecimal(App.Weight.calibrate_two_suffer,LOAD_ROW-1, LOAD_COL+3,0,4);
		uiLcdDecimal(App.Weight.calibrate_sub_value,LOAD_ROW-1, LOAD_COL+8,0,4);

		uiLcdDecimal(App.Weight.calibrate_one_ad_suffer,LOAD_ROW, LOAD_COL-2,0,4);
		uiLcdDecimal(App.Weight.calibrate_two_ad_suffer,LOAD_ROW, LOAD_COL+3,0,4);
		uiLcdDecimal(App.Weight.calibrate_ad_sub_value,LOAD_ROW, LOAD_COL+8,0,4);
	
		uiLcdDecimal(App.Weight.calibrate_suffer,LOAD_ROW+1, LOAD_COL-2,0,4);
		uiLcdDecimal(App.Weight.weight_clear_ad_value,LOAD_ROW+1, LOAD_COL+3,0,4);
		
		

		uiLcdDecimal(App.Weight.weight_clear_ad_value_sign,LOAD_ROW+2, LOAD_COL,0,4);


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
        		ad_temp=(operate_temp * App.Weight.calibrate_sub_value)/App.Weight.calibrate_ad_sub_value;//得到最终的重量值	

		uiLcdDecimal(ad_temp,LOAD_ROW+1, LOAD_COL+8,0,4);
		
		
        		nKey = uiKeyGetKey();
		if(nKey == UIKEY_ESC)
			break;
		if(nKey == UIKEY_NONE)
			continue;
		//if(nKey == UIKEY_OK)
			

	}
	
}



