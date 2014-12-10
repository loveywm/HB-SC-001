

#include "system.h"
#include "app.h"

//��Ҫ��Ϊ�˰�����������ӣ�ͬʱҲ�����������޵�����bug
u32 gTimeCount = 0;//���ڼ���ʱ�ӹ�����ϵͳʱ�Ӻ�����ÿ10ms����һ��


u32 uiTimeGetTickCount(void)
{
	return gTimeCount;
}


static u8 HB_GetPin(void)
{
	return System.Device.HB_Key.HB_ScanPin();
}

u8 uiKeyGetKey(void)
{
	
	static u8 nPrevKey = UIKEY_NONE;
	static u32 dwPrevTime = 0;
	static BOOL bPrevEqual = 0;
	u8 nKey1;
	u8 nKey2;

	nKey1 = HB_GetPin();
	if (nKey1 != UIKEY_NONE)
	{
		DelayMs(30);
		nKey2 = HB_GetPin();
		if(nKey1 == nKey2)
		{

			if (nPrevKey == nKey1)
			{
				if (uiTimeGetTickCount() - dwPrevTime < (bPrevEqual ? 500 : 4000))
				{
					return UIKEY_NONE;
				}

				bPrevEqual = 1;
			}
			else
			{
				bPrevEqual = 0;
			}
		
			Buzzer_Beep(1, 100);
			nPrevKey = nKey1;
			dwPrevTime = uiTimeGetTickCount();
		}
		else
		{
			nKey1 = UIKEY_NONE;
		}
	}
	else
	{
		nPrevKey = nKey1;
	}
	return nKey1;
}

