
#include "system.h"
#include "app.h"


#define MAX_PWD_BYTE_LENS		6	

#define MAX_PWD_GET_LENS		52	

#define PWD_MAP_ADD			0x300000





//验证成功返回1，失败返回0
u8 PwdVerify(u8* pcPwd)
{
	//u16 	wPwd = 0;
	int	i;
	u8 Temp_Pwd[MAX_PWD_BYTE_LENS];
	
	//Flash_ReadInfo((UINT8 *)&gsPwdMapTmp,PWD_MAP_ADD,sizeof(gsPwdMapTmp));
	//store_all_int();
	//spiFlashRead(PWD_MAP_ADD,sizeof(gsPwdMapTmp),(UINT8 *)&gsPwdMapTmp);
	
	Flash_ReadInfo(Temp_Pwd,PWD_MAP_ADD,MAX_PWD_BYTE_LENS);
	//System.

	for(i=0;i<MAX_PWD_BYTE_LENS;i++)//逐个比对，有一个不一样就失败返回
	{
		printf("Temp_Pwd[%d]==%d\r\n",i,Temp_Pwd[i]);
		printf("pcPwd[%d]==%d\r\n",i,pcPwd[i]);
		if(Temp_Pwd[i] != pcPwd[i])
		{
			
			return FALSE;
		}

	}
	return TRUE;
}

u8 PwdSave(u8* pcPwd)
{
	//这里其实需要保存然后取出来比对下，再确定是否正确
	Flash_WriteInfo(pcPwd,PWD_MAP_ADD,MAX_PWD_BYTE_LENS);
	return TRUE;
}

#define PWD_ROW	1
#define PWD_COL		4

u8 Pwd_Enroll_Process(void)
{
	char pcRet[MAX_PWD_BYTE_LENS];
	u8 nKey;
	int i, nPos = 0;
	u8 nMaxLen = MAX_PWD_BYTE_LENS;

	pcRet[0] = 0;
		
	uiLcdClear();
	uiLcd_1212_ch(UISTR_INPUT_PASSWORD,0,4,7);

	//uiLcdLineErase8x16(2, 5, 11, 1);
	uiLcdMediumString("______", PWD_ROW, PWD_COL,0);

	uiLcd_1414_ch(UISTR_XXX_SAVE, 6, 4,2);
	uiLcdMediumString("OK", 3 ,4,0);


	uiLcd_1414_ch(UISTR_XXX_DEL, 6,76,2);
	uiLcdMediumString("ESC", 3, 13,0);
	while(1)
	{
	
        		nKey = uiKeyGetKey();
		if(nKey == UIKEY_ESC)
			break;
		
		if(nKey == UIKEY_OK)
		{
			if (nPos == 6 )
			{

				printf("pcRet== %d \r\n",nPos);
				//在这里比对
				if(PwdSave(pcRet))
				{
					//比对成功
					return TRUE;

				}

				//失败
				nPos = 0;
				for (i=0; i<MAX_PWD_BYTE_LENS; i++)
				{	
					pcRet[i] = 0;
				}
				uiLcdMediumString("______", PWD_ROW, PWD_COL,0);

				//提示密码错误

				
				continue;
				
			}
			printf("nPos== %d \r\n",nPos);
			//提示输入的数目不够
			
			continue;
		}

		if (nKey == UIKEY_NONE)
			continue;

		if(nKey == UIKEY_MENU)//删除
		{
			if (nPos == 0)//当读取位置== 0时，密码存储值清理
			{	
				nPos = 0;
				for (i=0; i<MAX_PWD_BYTE_LENS; i++)
				{	
					pcRet[i] = 0;
				}
				uiLcdMediumString("______", PWD_ROW, PWD_COL,0);
				continue;
			}
			nPos--;
			printf("pcRet[%d]== %d\r\n",nPos,pcRet[nPos]);

			for (i=0; i<nPos; i++)
			{
				//uiLcdMediumString("*", PWD_ROW, PWD_COL+i,0);
				uiLcdDecimal(pcRet[i]-'0',PWD_ROW, PWD_COL+i,0,1);
			}
			for (i=nPos; i<MAX_PWD_BYTE_LENS; i++)
			{
				//uiLcdMediumString("_", PWD_ROW, PWD_COL+i,0);
				uiLcdDecimal(pcRet[i]-'0',PWD_ROW, PWD_COL+i,0,1);
			}
			//uiLcdLineErase8x16(2, 4+nPos, 4+nPos+1, 1);

		}

		if (uiKeyIsDigit(nKey))
		{
			if (nPos == nMaxLen)//当读取位置大于6时，密码存储值清理
			{	
				printf("pcRet[%d]== %d \r\n",nPos,pcRet[nPos-1]);
				//提示输入的数目超过定数，重新输入

				
				nPos = 0;
				for (i=0; i<MAX_PWD_BYTE_LENS; i++)
				{	
					pcRet[i] = 0;
				}
				//uiLcdLineErase8x16(2, 5, 11, 1);
				//uiLcdLineErase8x16(2, 5, 6, 1);
				uiLcdMediumString("______", PWD_ROW, PWD_COL,0);
				continue;
			}

			pcRet[nPos] = nKey;

			printf("pcRet[%d]== %d \r\n",nPos,pcRet[nPos]);
			nPos++;
			if(nPos <MAX_PWD_BYTE_LENS)//不能超过最大值，防止数组泄露
			{
				pcRet[nPos] = 0;
			}
			

			printf("pcRet[%d]== %d\r\n",nPos,pcRet[nPos-1]);

			for (i=0; i<nPos; i++)
			{
				//uiLcdMediumString("*",PWD_ROW, PWD_COL+i,0);
				uiLcdDecimal(pcRet[i]-'0',PWD_ROW, PWD_COL+i,0,1);
			}
			//uiLcdLineErase8x16(2, 4+nPos, 4+nPos+1, 1);
		}
	}
	return FALSE;
}




//验证成功返回1，失败返回0
u8 Pwd_Identify_Process(void)
{
	char pcRet[MAX_PWD_BYTE_LENS];
	u8 nKey;
	int i, nPos = 0;
	u8 nMaxLen = MAX_PWD_BYTE_LENS;

	pcRet[0] = 0;
		
	uiLcdClear();
	uiLcd_1212_ch(UISTR_GET_PASSWD,1,4,8);

	//uiLcdLineErase8x16(2, 5, 11, 1);
	uiLcdMediumString("______", 2, 5,0);
	
	while(1)
	{
	
        		nKey = uiKeyGetKey();
		if(nKey == UIKEY_ESC)
			break;
		
		if(nKey == UIKEY_OK)
		{
			if (nPos == 6 )
			{
				//uiLcdLineErase8x16(2, 5, 10, 0);
				//for (i=0; i<nPos; i++)
				//uiLcdMediumPrintf(row, col1 + i, FALSE, "*");
				//uiLcdMediumString("*", 2, 5+i,0);

				printf("pcRet== %d \r\n",nPos);
				//在这里比对
				if(PwdVerify(pcRet))
				{
					//比对成功
					return TRUE;

				}

				//失败
				nPos = 0;
				for (i=0; i<MAX_PWD_BYTE_LENS; i++)
				{	
					pcRet[i] = 0;
				}
				uiLcdMediumString("______", 2, 5,0);

				//提示密码错误

				
				continue;
				
			}
			printf("nPos== %d \r\n",nPos);
			//提示输入的数目不够
			
			continue;
		}

		if (nKey == UIKEY_NONE)
			continue;

		if(nKey == UIKEY_MENU)//删除
		{
			if (nPos == 0)//当读取位置== 0时，密码存储值清理
			{	
				nPos = 0;
				for (i=0; i<MAX_PWD_BYTE_LENS; i++)
				{	
					pcRet[i] = 0;
				}
				uiLcdMediumString("______", 2, 5,0);
				continue;
			}
			nPos--;
			printf("pcRet[%d]== %d\r\n",nPos,pcRet[nPos]);

			for (i=0; i<nPos; i++)
			{
				uiLcdMediumString("*", 2, 5+i,0);
			}
			for (i=nPos; i<MAX_PWD_BYTE_LENS; i++)
			{
				uiLcdMediumString("_", 2, 5+i,0);
			}
			//uiLcdLineErase8x16(2, 4+nPos, 4+nPos+1, 1);

		}

		if (uiKeyIsDigit(nKey))
		{
			if (nPos == nMaxLen)//当读取位置大于6时，密码存储值清理
			{	
				printf("pcRet[%d]== %d \r\n",nPos,pcRet[nPos-1]);
				//提示输入的数目超过定数，重新输入

				
				nPos = 0;
				for (i=0; i<MAX_PWD_BYTE_LENS; i++)
				{	
					pcRet[i] = 0;
				}
				//uiLcdLineErase8x16(2, 5, 11, 1);
				//uiLcdLineErase8x16(2, 5, 6, 1);
				uiLcdMediumString("______", 2, 5,0);
				continue;
			}

			pcRet[nPos] = nKey;

			printf("pcRet[%d]== %d \r\n",nPos,pcRet[nPos]);
			nPos++;
			if(nPos <MAX_PWD_BYTE_LENS)//不能超过最大值，防止数组泄露
			{
				pcRet[nPos] = 0;
			}
			

			printf("pcRet[%d]== %d\r\n",nPos,pcRet[nPos-1]);

			for (i=0; i<nPos; i++)
			{
				uiLcdMediumString("*", 2, 5+i,0);
			}
			//uiLcdLineErase8x16(2, 4+nPos, 4+nPos+1, 1);
		}
	}
	return FALSE;
}




u8 Pwd_Del_Process(void);





