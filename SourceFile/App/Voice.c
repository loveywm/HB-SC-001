

#include "system.h"
#include "app.h"




static void BUZZER_OFF(void)
{
	//gpio_setportval(GPIO_PORTB, GPIO_BELL, 0); 
	System.Device.IO.SetBeep(0);
}

static void BUZZER_ON(void)
{
	//gpio_setportval(GPIO_PORTB, GPIO_BELL, GPIO_BELL); 
	System.Device.IO.SetBeep(1);

}

void Buzzer_Beep(u8 nBeepTimes, u16 nMiliSecond)
{
	while( nBeepTimes > 0)
	{
		BUZZER_ON();
		DelayMs(nMiliSecond);
		BUZZER_OFF();
		nBeepTimes--;
		
		if ( nBeepTimes )
		{
			DelayMs(nMiliSecond);
		}		
	}
}




