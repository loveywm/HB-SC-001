

#include "app.h"
#include "system.h"

//#include "uiLCD_12864.h"

//显示6*8字体
void uiLcd6x8Char(UINT8 *pbyFontData, int row, int col, BOOL bIsReverse)
{//6x8 //pbyFontData[6], 0<=row<=7, 0<=col<=19
	//disp_str("2",2*8,0);
	//ulLcdChar(pbyFontData, row, col, bIsReverse, 6, 1, 1, 10, 0);	
	System.Device.Lcd_12864.ulLcdChar(pbyFontData, row, col, bIsReverse, 6, 1, 1, 10, 0);
}

void uiLcdSmallString(char* pStr, int row, int col, BOOL bIsReverse)
{
	//disp_str("0",0,0);
	while (*pStr)
	{
		//disp_str("1",1*8,0);
		uiLcd6x8Char((UINT8 *)&uiENfont68[(UINT8)*pStr - 0x20][0], row, col, bIsReverse);
		col++;
		pStr++;
		//if (col > 19)
			//break;
		if (col > 20)
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////////
//显示16*32字体
void uiLcd16x32Char(UINT8 * pbyFontData, int row, int col, BOOL bIsReverse)
{//16x32 //pbyFontData[64], 0<=row<=2, 0<=col<=7
	System.Device.Lcd_12864.ulLcdChar(pbyFontData, row, col, bIsReverse, 16, 4, 2, 4, 0);	
}

//////////////////////////////////////////////////////////////////////////
void uiLcdLargeString(char* pStr, int row, int col, BOOL bIsReverse)
{//for time uiLcdLargeString("12 :36", 1, 1, FALSE);

	while (*pStr)
	{
		uiLcd16x32Char((UINT8 *)&uiENfont1632[(UINT8)*pStr - 0x20][0], row, col, bIsReverse);

		col++;
		pStr++;

		if (col > 7)
			break;
	}
}
///////////////////////////////////////////////////////////////////////////////
//显示6*16字体
void uiLcd6x16Char(UINT8* pbyFontData, int row, int col, BOOL bIsReverse)
{//6x16 //pbyFontData[12], 0<=row<=3, 0<=col<=19
	System.Device.Lcd_12864.ulLcdChar(pbyFontData, row, col, bIsReverse, 6, 2, 2, 10, 4);	
}
////////////////////////////////////////////////////////////////////////////////
//显示8*16字体
void uiLcd8x16Char(UINT8* pbyFontData, int row, int col, BOOL bIsReverse)
{//8x16 //pbyFontData[16], 0<=row<=3, 0<=col<=15
	System.Device.Lcd_12864.ulLcdChar(pbyFontData, row, col, bIsReverse, 8, 2, 2, 8, 0);	
}

void uiLcdMediumString(char* pStr, int row, int col, BOOL bIsReverse)
{
	while( *pStr != 0x00)
	{
		uiLcd8x16Char((UINT8 *)&uiENfont816[(UINT8)*pStr - 0x20][0], row, col, bIsReverse);
		col++;
		pStr++;

		if (col > 15)
			break;
	}
}

//////////////////////////////////////////////////////////////////////////
void uiLcdLineErase6x8(int row, int col1, int col2, BOOL bIsReverse)
{
	int i;

	for (i=col1; i<col2; i++)
	{
		uiLcd6x8Char((UINT8 *)&uiENfont68[0][0], row, i, bIsReverse);
	}
}

//////////////////////////////////////////////////////////////////////////
// void uiLcdLineErase6x16(int row, int col1, int col2, BOOL bIsReverse)
// {
// 	int i;

// 	for (i=col1; i<col2; i++)
// 	{
 //		uiLcd6x16Char((UINT8 *)&uiENfont616[0][0], row, i, bIsReverse);
// 	}
// }

void uiLcdLineErase8x16(int row, int col1, int col2, BOOL bIsReverse)
{
	int i;

	for (i=col1; i<col2; i++)
	{
		uiLcd8x16Char((UINT8 *)&uiENfont816[0][0], row, i, bIsReverse);
	}
}

void uiLcdDecimal(int nValue, int row, int col, BOOL bIsReverse, int nZeroPadding)
{
	int i, nPadding;
	char str[32], str2[32];

	sprintf(str2, "%d", nValue);
	nPadding = nZeroPadding - strlen(str2);

	if (nPadding > 0)
	{
		for(i=0; i<nPadding;i++)
			str[i] = '0';
		str[nPadding] = 0;
	}
	else
	{
		str[0] = 0;
	}
	strcat(str, str2);
	uiLcdMediumString(str, row, col, bIsReverse);
	//uiLcdSmallString(str, row, col, bIsReverse);
}

void uiLcdDecimal_Small(int nValue, int row, int col, BOOL bIsReverse, int nZeroPadding)
{
	int i, nPadding;
	char str[32], str2[32];

	sprintf(str2, "%d", nValue);
	nPadding = nZeroPadding - strlen(str2);

	if (nPadding > 0)
	{
		for(i=0; i<nPadding;i++)
			str[i] = '0';
		str[nPadding] = 0;
	}
	else
	{
		str[0] = 0;
	}
	strcat(str, str2);
	//uiLcdMediumString(str, row, col, bIsReverse);
	uiLcdSmallString(str, row, col, bIsReverse);
}

//此函数是显示汉字的12*12
void uiLcdMultiChar(UINT8 * pbyFontData, int row, int col, BOOL bIsReverse)
{//12x16 //pbyFontData[24], 0<=row<=3, 0<=col<=9
	System.Device.Lcd_12864.ulLcdChar(pbyFontData, row, col, bIsReverse, 12, 2, 2, 5, 4);	
}



// void uiLcdBmp(UINT8 * cData)
// {
// 	//UINT8 * pby = cData;
// 	//int i, j;
// 	//UINT8 cTemp;
// 	T_LCDDAT lcd_data;
// 	
// 	//cTemp = 0x00;

// 	lcd_data.xpos_start=0;
// 	lcd_data.ypos_start=0;
// 	lcd_data.xpos_end=8;
// 	lcd_data.ypos_end=128;		
// 	lcd_data.pdata=cData;
// 	lcd_data.bIsReverse=0;
// 	//driver_ioctl(Lcd_Fd,IOCTL_LCD_BMP_SET,&lcd_data);
// 	ioctl_lcd_bmp_set(&lcd_data,1);	

// }
// //  0=<row<=7   4=<col<=124  
// //num 表示此次显示几个汉字
// //address表示显示汉字在汉字数组中的起始位置数
void uiLcd_1212_ch(u8 address,u8 row,u8 col,u8 num)
{
	int i,j;	
	for(j=0;j<num;j++)
	{	
		System.Device.Lcd_12864.ioctl_lcd_xpos_set (row,1);
		System.Device.Lcd_12864.ioctl_lcd_ypos_set (col+j*12,1);
		for(i=0;i<12;i++)
		{ 
			//LCDWritedata(charstring[i]);
			System.Device.Lcd_12864.LCD_Data_write(uichfont_1212[j+address][i]);		
		} 
		System.Device.Lcd_12864.ioctl_lcd_xpos_set (row+1,1);
		System.Device.Lcd_12864.ioctl_lcd_ypos_set (col+j*12,1);
		for(i=0;i<12;i++)
		{ 
			//LCDWritedata(charstring[i+16]);
			System.Device.Lcd_12864.LCD_Data_write(uichfont_1212[j+address][12+i]);		
		} 
	}
}

void uiLcd_1414_ch(u8 address,u8 row,u8 col,u8 num)
{
	int i,j;	
	for(j=0;j<num;j++)
	{	
		System.Device.Lcd_12864.ioctl_lcd_xpos_set (row,1);
		System.Device.Lcd_12864.ioctl_lcd_ypos_set (col+j*14,1);
		for(i=0;i<14;i++)
		{ 
			//LCDWritedata(charstring[i]);
			System.Device.Lcd_12864.LCD_Data_write(uichfont_1414[j+address][i]);		
		} 
		System.Device.Lcd_12864.ioctl_lcd_xpos_set (row+1,1);
		System.Device.Lcd_12864.ioctl_lcd_ypos_set (col+j*14,1);
		for(i=0;i<14;i++)
		{ 
			//LCDWritedata(charstring[i+16]);
			System.Device.Lcd_12864.LCD_Data_write(uichfont_1414[j+address][14+i]);		
		} 
	}
}

void uiLcd_1616_ch(u8 address,u8 row,u8 col,u8 num)
{
	int i,j;	
	for(j=0;j<num;j++)
	{	
		System.Device.Lcd_12864.ioctl_lcd_xpos_set (row,1);
		System.Device.Lcd_12864.ioctl_lcd_ypos_set (col+j*16,1);
		for(i=0;i<16;i++)
		{ 
			//LCDWritedata(charstring[i]);
			System.Device.Lcd_12864.LCD_Data_write(uichfont_1616[j+address][i]);		
		} 
		System.Device.Lcd_12864.ioctl_lcd_xpos_set (row+1,1);
		System.Device.Lcd_12864.ioctl_lcd_ypos_set (col+j*16,1);
		for(i=0;i<16;i++)
		{ 
			//LCDWritedata(charstring[i+16]);
			System.Device.Lcd_12864.LCD_Data_write(uichfont_1616[j+address][16+i]);		
		} 
	}
}


void uiLcdClear(void)
{
	System.Device.Lcd_12864.uiLcdClear();
}





