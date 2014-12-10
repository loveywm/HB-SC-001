/*
********************************************************************************
*                                嵌入式微系统
*                                   msOS

********************************************************************************
*文件名     : device_Lcd_12864.c
*作用       : 输入输出设备
*原理       : 无
********************************************************************************
*版本     作者            日期            说明
*V0.1    whatbook       2014/08/27       初始版本
********************************************************************************
*/

#include "drive.h"
#include "system.h"

typedef struct 
{
    T_U16    xpos_start;  //???;?????(0~7),??;x??????       
    T_U16    ypos_start;  //???;?????(0~127),??;y??????  
    T_U16    xpos_end;    //???;?????(0~7),??;x??????       
    T_U16    ypos_end;    //???;?????(0~127),??;y??????  	
    void*    pdata;        //??   
    //T_U16    xlen;           
    //T_U16    ylen;         
    T_U8     bIsReverse;  //??????        
} T_LCDDAT;   


#define	 Set_LEDK	  	 GPIO_SetBits (GPIOD, GPIO_Pin_6)

#define	 Set_CS	  		 GPIO_SetBits (GPIOD, GPIO_Pin_2)
#define	 Reset_CS	  	 GPIO_ResetBits  (GPIOD, GPIO_Pin_2)

#define	 SetR_S	  		 GPIO_SetBits (GPIOD, GPIO_Pin_3)
#define	 Reset_R_S	  	 GPIO_ResetBits  (GPIOD, GPIO_Pin_3)

#define	 Set_RES	  	 GPIO_SetBits (GPIOD, GPIO_Pin_4)
#define	 Reset_RES	  	 GPIO_ResetBits  (GPIOD, GPIO_Pin_4)

#define	 Set_SCK  		 GPIO_SetBits (GPIOD, GPIO_Pin_1)
#define	 Reset_SCK	  	 GPIO_ResetBits  (GPIOD, GPIO_Pin_1)

#define	 Set_SDA 		 GPIO_SetBits (GPIOD, GPIO_Pin_0)
#define	 Reset_SDA	  	 GPIO_ResetBits  (GPIOD, GPIO_Pin_0)


//set page addr 0~7
extern  void ioctl_lcd_xpos_set( int data, int devid );
//set column addr 1~128
extern void ioctl_lcd_ypos_set( int data, int devid );
extern void ioctl_lcd_xypos_set( T_LCDDAT *data, int devid );
//extern void ioctl_lcd_windows_set( T_LCDDAT *data, int devid );
extern void lcd_data_write( u8 *Pdata, int size, int devid ); 
extern void ioctl_lcd_datafast_set( T_LCDDAT * data, int devid );
extern void ioctl_lcd_data_clear( T_LCDDAT * data, int devid );
extern void ioctl_lcd_bmp_set( T_LCDDAT * data, int devid );

void LCD_Init(void);
void LCD_clear(void);
void LCD_set_xpos(int dat);
void LCD_set_ypos(int dat);
void LCD_set_xypos(int x,int y);
void LCD_Cmd_write(u8 cmd);
void LCD_Data_write(u8 dat);

static void uiLcdClear(void);
void ulLcdChar(UINT8* pbyFontData, int row, int col, BOOL bIsReverse,
			   int nFontCx, int nFontCyBytes, int nRowBytes, int nColHarfNum, int nMargin);



void InitializeLcd_12864(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOC |RCC_APB2Periph_GPIOD |RCC_APB2Periph_AFIO, ENABLE);        
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    	/* GPIOA Configuration: Channel 1, 2 and 3 as alternate function push-pull */
    	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    	//GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_6;
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//System.Device.Lcd_12864.uiLcdSmallString = uiLcdSmallString;
	//System.Device.Lcd_12864.uiLcd_1212_ch = uiLcd_1212_ch;
	System.Device.Lcd_12864.ulLcdChar = ulLcdChar;
	System.Device.Lcd_12864.ioctl_lcd_xpos_set = ioctl_lcd_xpos_set;
	System.Device.Lcd_12864.ioctl_lcd_ypos_set = ioctl_lcd_ypos_set;
	System.Device.Lcd_12864.LCD_Data_write = LCD_Data_write;
	System.Device.Lcd_12864.uiLcdClear = uiLcdClear;
	
	LCD_Init();
	
}


void Delayms(u16 ms)
{	
	u8 j,k;
	while(ms--)
	  for(j=0;j<=20;j++) for(k=0;k<=200;k++);   
	                   
}		
void Comwrite(u8 cmd)
{
	u8 i,j;
	Reset_CS;		//CS1=0;
	Reset_R_S;		//R_S=0;  
	Set_SCK;		//SCK=1;
	for(i=0;i<8;i++)
	{ 
		j=cmd;
		Reset_SCK;		// SCK=0;
		if(cmd&0x80)
			Set_SDA;		//SDA=cmd&0x80;
		else Reset_SDA;
			Set_SCK;		//SCK=1;	     
		cmd=j<<1;
	}
	Set_CS;			// CS1=1;
		//SetR_S;			//R_S=1;
}

void Datwrite(u8 dat)	
{							
    u8 i,j;
	Reset_CS;			// CS1=0;
	SetR_S;		 		 //R_S=1;  
    Set_SCK;			// SCK=1;
	for(i=0;i<8;i++)
	{ 
		j=dat;
		Reset_SCK;		// SCK=0;
		if(dat&0x80)
			Set_SDA;		//SDA=cmd&0x80;
		else 
			Reset_SDA;
		Set_SCK;		//SCK=1;
		dat=j<<1;
	}
	Set_CS;			//CS1=1;
	//Reset_R_S;			//	R_S=0;
}

void LCD_Data_write(u8 dat)
{
	u8 i,j;
	Reset_CS;			// CS1=0;
	SetR_S;		 		 //R_S=1;  
    Set_SCK;			// SCK=1;
	for(i=0;i<8;i++)
	{ 
		j=dat;
		Reset_SCK;		// SCK=0;
		if(dat&0x80)
			Set_SDA;		//SDA=cmd&0x80;
		else 
			Reset_SDA;
		Set_SCK;		//SCK=1;
		dat=j<<1;
	}
	Set_CS;			//CS1=1;
	//Reset_R_S;			//	R_S=0;
}
void LCD_Cmd_write(u8 cmd)
{
	u8 i,j;
	Reset_CS;		//CS1=0;
	Reset_R_S;		//R_S=0;  
	Set_SCK;		//SCK=1;
	for(i=0;i<8;i++)
	{ 
		j=cmd;
		Reset_SCK;		// SCK=0;
		if(cmd&0x80)
			Set_SDA;		//SDA=cmd&0x80;
		else 
			Reset_SDA;
		Set_SCK;		//SCK=1;	     
		cmd=j<<1;
	}
	Set_CS;			// CS1=1;
		//SetR_S;			//R_S=1;
}

void LCD_Init(void)
{

	Set_LEDK;

	Reset_RES;//RES=0;
	Delayms(50);
	Set_RES;		//RES=1;
	Delayms(20);
		LCD_Cmd_write(0xe2);//复位  14 reset
		LCD_Cmd_write(0xAe);// 1 显示关
		LCD_Cmd_write(0x60);
		
		LCD_Cmd_write(0xa5);//10 set all pixel on 
		LCD_Cmd_write(0xa0);//a1//8  Set scan direction of SEG  MX=1, reverse direction  MX=0, normal direction
		LCD_Cmd_write(0xc8);//c0//15 Set output direction of COM MY=1, reverse direction MY=0, normal direction
		LCD_Cmd_write(0xa3);//a2//11 Select bias setting 0=1/9; 1=1/7(at 1/65 duty)
		LCD_Cmd_write(0xa6);//a7//9 INV =1, inverse display INV =0, normal display
		LCD_Cmd_write(0x26);//20-27//17 Select regulation resistor ratio 变压比调节
		//LCD_SendCmd(0x81);
		//LCD_SendCmd(data);
	
		LCD_Cmd_write(0xf8);//(19) Set Booster Double command!!
		LCD_Cmd_write(0x00);
	
		//LCD_Cmd_write(0x0e);
		//LCD_Cmd_write(0xe0);
		//LCD_Cmd_write(0xee);
		
	
		LCD_Cmd_write(0xa4);//10 nomal display
		LCD_Cmd_write(0xaf);// 1 显示开
		LCD_Cmd_write(0x2f);//16 Control built-in power circuit ON/OFF  电源全开
		//LCD_Cmd_write(0x40);//起始行0
		//LCD_Cmd_write(0x60);
		//LCD_Cmd_write(0x5c);
		LCD_clear();
}

void LCD_clear(void)
{
	 u8  i,j;
   for(i=0;i<8;i++)   
   {
			//Setadd(0,i); 
		 LCD_set_xypos(i,0);
		 //LCD_set_xypos(0,i);
     for(j=0;j<128;j++) 
				LCD_Data_write(0x00);
   }
}

void LCD_set_xpos(int x)
{
	LCD_Cmd_write(x|0xB0);
}
void LCD_set_ypos(int y)
{
	LCD_Cmd_write(y&0x0f);
	LCD_Cmd_write(0x10|((y>> 4) & 0x0F));
}
void LCD_set_xypos(int x,int y)
{
	LCD_set_xpos(x);
	LCD_set_ypos(y);
}

////////////////////////////////////////////////
void spi_lcd_xpos_set(int data)
{
	Comwrite(data|0xB0);
}
void spi_lcd_ypos_set(int data)
{
	Comwrite(data & 0x0f);
	Comwrite(0x10 | ((data>> 4) & 0x0F));
}

void spi_lcd_data_set(u8 data)
{
	Comwrite(data);
}
////////////////////////////////////////////////
//set page addr 0~7
 void ioctl_lcd_xpos_set( int data, int devid )
{
		spi_lcd_xpos_set(data);
}
//set column addr 1~128
 void ioctl_lcd_ypos_set( int data, int devid )
{
		spi_lcd_ypos_set(data);
	
}
 void ioctl_lcd_xypos_set( T_LCDDAT *data, int devid )
{
		spi_lcd_xpos_set(data->xpos_start);
		spi_lcd_ypos_set(data->ypos_start);
		
}
 void ioctl_lcd_datafast_set( T_LCDDAT * data, int devid )
{
	int i,j,dData;
	u8 *pdata;
	//u16 *pdata16;
	{
		pdata=(u8 *)data->pdata;
		for (j=0; j<data->xpos_end; j++)
		{			
			spi_lcd_xpos_set(data->xpos_start+j);
			spi_lcd_ypos_set(data->ypos_start);
			for (i=0; i<data->ypos_end; i++)
			{
				if (!data->bIsReverse)
				{
					dData = (u8)pdata[j*data->ypos_end + i];					
				}
				else
				{
					dData = (~(u8)pdata[j*data->ypos_end + i]);
				}			    
				LCD_Data_write(dData);
			}
		}
	}	
}
void ioctl_lcd_data_clear( T_LCDDAT * data, int devid )
{
	int i,j,dData;
	u8 *pdata;
	//u16 *pdata16;
	
	{
		pdata=(u8 *)data->pdata;
		for (j=0; j<data->xpos_end; j++)
		{			
			spi_lcd_xpos_set(data->xpos_start+j);
			spi_lcd_ypos_set(data->ypos_start);
			for (i=0; i<data->ypos_end; i++)
			{
				if (!data->bIsReverse)
				{
					dData = (u8)pdata[0];					
				}
				else
				{
					dData = (~(u8)pdata[0]);
				}			    
				LCD_Data_write(dData);
			}
		}		
	}	
} 
 void lcd_data_write( u8 *Pdata, int size, int devid )
{
	int i;
	//u16 *data;
	{
		for(i=0;i<size;i++)
		{
			//SPI_WriteByte(*Pdata);	
			//spi_lcd_data_set(*Pdata);
			LCD_Data_write(*Pdata);
			Pdata++;
		}
	}		
}


void ioctl_lcd_bmp_set( T_LCDDAT * data, int devid )
{
	int i,j,dData;
//	u16 *pdata16;
	
	//if(devid==LCD_BW_SUBID)
	{
		u8 * pby = (u8*)data->pdata;
		for (j=0; j<data->xpos_end; j++)
		{
			spi_lcd_xpos_set(data->xpos_start+j);
			spi_lcd_ypos_set(data->ypos_start);
			for (i=0; i<data->ypos_end; i++)
			{
				if (!data->bIsReverse)
				{
					dData = *pby++;					
				}
				else
				{
					dData = (~(*pby++));
				}			    
				//spi_lcd_data_set(dData);
				LCD_Data_write(dData);
			}
		}
	}		
}


void ulLcdChar(UINT8* pbyFontData, int row, int col, BOOL bIsReverse,
			   int nFontCx, int nFontCyBytes, int nRowBytes, int nColHarfNum, int nMargin)
{
	//int i, j;
	UINT8 dData;
	T_LCDDAT lcd_data;
	//disp_str("3",3*8,0);
	//COG,??
	if (nMargin > 0 && col == 0)	/*??????0??,????4?,??4???????*/
	{
		dData =0;

		lcd_data.xpos_start=row * nRowBytes;
		lcd_data.ypos_start=0;
		lcd_data.xpos_end=nFontCyBytes;
		lcd_data.ypos_end=nMargin;		
		lcd_data.pdata=&dData;	
		lcd_data.bIsReverse=bIsReverse;
		//driver_ioctl(Lcd_Fd,IOCTL_LCD_DAT_CLEAR,&lcd_data);	
		ioctl_lcd_data_clear((T_LCDDAT *)&lcd_data,1);
	}
	
	lcd_data.xpos_start=row * nRowBytes;
	lcd_data.ypos_start=nMargin + col * nFontCx;
	lcd_data.xpos_end=nFontCyBytes;
	lcd_data.ypos_end=nFontCx;		
	lcd_data.pdata=pbyFontData;
	lcd_data.bIsReverse=bIsReverse;
	//driver_ioctl(Lcd_Fd,IOCTL_LCD_DAT_FASTSET,&lcd_data);
	ioctl_lcd_datafast_set(&lcd_data,1);
	//disp_str("4",4*8,0);

	if (nMargin > 0 && col == nColHarfNum * 2 - 1)
	{
		dData =0;
		
		lcd_data.xpos_start=row * nRowBytes;
		lcd_data.ypos_start=128 - nMargin;
		lcd_data.xpos_end=nFontCyBytes;
		lcd_data.ypos_end=nMargin;			
		lcd_data.pdata=&dData;
		lcd_data.bIsReverse=bIsReverse;
		//driver_ioctl(Lcd_Fd,IOCTL_LCD_DAT_CLEAR,&lcd_data);
		ioctl_lcd_data_clear((T_LCDDAT *)&lcd_data,1);

	}
}

static void uiLcdClear(void)
{ 
	//int i,j;
	UINT8 cTemp;
	//UINT8 cTemp1;
	T_LCDDAT lcd_data;
	cTemp = 0x00;
	lcd_data.xpos_start=0;
	lcd_data.ypos_start=0;
	lcd_data.xpos_end=8;
	lcd_data.ypos_end=129;			
	lcd_data.pdata=&cTemp;
	lcd_data.bIsReverse=0;
	//driver_ioctl(Lcd_Fd,IOCTL_LCD_DAT_CLEAR,&lcd_data);	
	ioctl_lcd_data_clear(&lcd_data,1);
}


