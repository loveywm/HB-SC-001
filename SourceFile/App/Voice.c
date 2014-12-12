

#include "system.h"
#include "app.h"


#define  WTV_CS_H  	System.Device.IO.HB_Gpio_Set_Value(WTV_CS,1)
#define  WTV_CS_L  	System.Device.IO.HB_Gpio_Set_Value(WTV_CS,0)

#define  WTV_CLK_H 	System.Device.IO.HB_Gpio_Set_Value(WTV_CLK,1)
#define  WTV_CLK_L  	System.Device.IO.HB_Gpio_Set_Value(WTV_CLK,0)

#define  WTV_DATA_H System.Device.IO.HB_Gpio_Set_Value(WTV_DATA,1)
#define  WTV_DATA_L  System.Device.IO.HB_Gpio_Set_Value(WTV_DATA,0)

#define  WTV_RST_H 	System.Device.IO.HB_Gpio_Set_Value(WTV_RST,1)
#define  WTV_RST_L  	System.Device.IO.HB_Gpio_Set_Value(WTV_RST,0)



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



void WTV_Voice(u8 addr)
{     
	char i;
      	WTV_RST_L;//rst=0;	
	//delay_ms(5);//wait_5ms( ); // 5ms  
	DelayMs(5 );
	WTV_RST_H;//rst=1;
	//delay_ms(8);//wait_8ms( ); //8ms 
	DelayMs(8 );
	WTV_CS_L;//cs=0;
	//delay_ms(5);//wait_5ms( ); // 5ms 
	DelayMs(5 );
	for(i=0;i<8;i++)
	{
		//_CLI();//TR0=0;
		WTV_CLK_L;//scl=0;
		if(addr & 0x01)
		{
			WTV_DATA_H;//sda=1;
		}
		else
		WTV_DATA_L;//sda=0;
		addr>>=1;
		//delay_us(150);//wait_150us( ); // 300us 
		DelayUs(150 );
		WTV_CLK_H;//scl=1;
		//delay_us(150);//wait_150us( );
		DelayUs(150 );
	}
	WTV_CS_H;//cs=1;
	//_SEI();//TR0=1
 }