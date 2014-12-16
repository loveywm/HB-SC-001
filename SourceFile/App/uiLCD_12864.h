

#ifndef __UILCD_12864_H
#define __UILCD_12864_H
//#include "system.h"


extern const unsigned char uiENfont68[][6];
//extern const unsigned char uiENfont616[][12];
extern const unsigned char uiENfont816[][16];
extern const unsigned char uiENfont1632[][64];
extern const unsigned char uichfont_1212[][24];
extern const unsigned char uichfont_1616[][32];
extern const unsigned char uichfont_1414[][28];

//6*8
//pbyFontData[6], 0<=row<=7, 0<=col<=19
void uiLcd6x8Char(UINT8 *pbyFontData, int row, int col, BOOL bIsReverse);
void uiLcdSmallString(char* pStr, int row, int col, BOOL bIsReverse);
//显示6*16字体
//pbyFontData[12], 0<=row<=3, 0<=col<=19
void uiLcd6x16Char(UINT8* pbyFontData, int row, int col, BOOL bIsReverse);
//8*16
//pbyFontData[16], 0<=row<=3, 0<=col<=15
void uiLcd8x16Char(UINT8* pbyFontData, int row, int col, BOOL bIsReverse);
void uiLcdMediumString(char* pStr, int row, int col, BOOL bIsReverse);
//16*32
//pbyFontData[64], 0<=row<=2, 0<=col<=7
void uiLcd16x32Char(UINT8 * pbyFontData, int row, int col, BOOL bIsReverse);
void uiLcdLargeString(char* pStr, int row, int col, BOOL bIsReverse);

void uiLcdLineErase6x8(int row, int col1, int col2, BOOL bIsReverse);
void uiLcdLineErase8x16(int row, int col1, int col2, BOOL bIsReverse);

void uiLcdDecimal(int nValue, int row, int col, BOOL bIsReverse, int nZeroPadding);
void uiLcdDecimal_Small(int nValue, int row, int col, BOOL bIsReverse, int nZeroPadding);

//pbyFontData[24], 0<=row<=3, 0<=col<=9
void uiLcdMultiChar(UINT8 * pbyFontData, int row, int col, BOOL bIsReverse);
//void uiLcdMenuStatus(int status);
//void uiLcdProgress(int row, int nPercent);
void uiLcdBmp(UINT8 * cData);

//  0=<row<=7   4=<col<=124  
//num 表示此次显示几个汉字
//address表示显示汉字在汉字数组中的起始位置数
void uiLcd_1212_ch(u8 address,u8 row,u8 col,u8 num);

void uiLcd_1616_ch(u8 address,u8 row,u8 col,u8 num);

void uiLcdClear(void);

void uiLcd_1414_ch(u8 address,u8 row,u8 col,u8 num);

#endif /*__UILCD_12864_H*/


