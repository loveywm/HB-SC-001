
#include "system.h"
#include "app.h"


#define MAX_PWD_BYTE_LENS		6	

#define MAX_PWD_GET_LENS		52	

#define PWD_MAP_ADD			0x300000





//��֤�ɹ�����1��ʧ�ܷ���0
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

	for(i=0;i<MAX_PWD_BYTE_LENS;i++)//����ȶԣ���һ����һ����ʧ�ܷ���
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
	//������ʵ��Ҫ����Ȼ��ȡ�����ȶ��£���ȷ���Ƿ���ȷ
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
				//������ȶ�
				if(PwdSave(pcRet))
				{
					//�ȶԳɹ�
					return TRUE;

				}

				//ʧ��
				nPos = 0;
				for (i=0; i<MAX_PWD_BYTE_LENS; i++)
				{	
					pcRet[i] = 0;
				}
				uiLcdMediumString("______", PWD_ROW, PWD_COL,0);

				//��ʾ�������

				
				continue;
				
			}
			printf("nPos== %d \r\n",nPos);
			//��ʾ�������Ŀ����
			
			continue;
		}

		if (nKey == UIKEY_NONE)
			continue;

		if(nKey == UIKEY_MENU)//ɾ��
		{
			if (nPos == 0)//����ȡλ��== 0ʱ������洢ֵ����
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
			if (nPos == nMaxLen)//����ȡλ�ô���6ʱ������洢ֵ����
			{	
				printf("pcRet[%d]== %d \r\n",nPos,pcRet[nPos-1]);
				//��ʾ�������Ŀ������������������

				
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
			if(nPos <MAX_PWD_BYTE_LENS)//���ܳ������ֵ����ֹ����й¶
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




//��֤�ɹ�����1��ʧ�ܷ���0
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
				//������ȶ�
				if(PwdVerify(pcRet))
				{
					//�ȶԳɹ�
					return TRUE;

				}

				//ʧ��
				nPos = 0;
				for (i=0; i<MAX_PWD_BYTE_LENS; i++)
				{	
					pcRet[i] = 0;
				}
				uiLcdMediumString("______", 2, 5,0);

				//��ʾ�������

				
				continue;
				
			}
			printf("nPos== %d \r\n",nPos);
			//��ʾ�������Ŀ����
			
			continue;
		}

		if (nKey == UIKEY_NONE)
			continue;

		if(nKey == UIKEY_MENU)//ɾ��
		{
			if (nPos == 0)//����ȡλ��== 0ʱ������洢ֵ����
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
			if (nPos == nMaxLen)//����ȡλ�ô���6ʱ������洢ֵ����
			{	
				printf("pcRet[%d]== %d \r\n",nPos,pcRet[nPos-1]);
				//��ʾ�������Ŀ������������������

				
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
			if(nPos <MAX_PWD_BYTE_LENS)//���ܳ������ֵ����ֹ����й¶
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





