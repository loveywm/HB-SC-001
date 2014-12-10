
#ifndef __UI_PROSS_H__
#define __UI_PROSS_H__

#define Input_invalid                 0x0

#define Input_invalid_1             0x0001
#define Input_invalid_2             0x0002
#define Input_invalid_3             0x0004
#define Input_invalid_4             0x0008

#define Input_invalid_5             0x0010
#define Input_invalid_6             0x0020
#define Input_invalid_7             0x0040
#define Input_invalid_8             0x0080

#define Input_invalid_9             0x0100
#define Input_invalid_10             0x0200
#define Input_invalid_11             0x0400
#define Input_invalid_12             0x0800

#define Input_invalid_13             0x1000
#define Input_invalid_14             0x2000
#define Input_invalid_15             0x4000
#define Input_invalid_16             0x8000


//界面参数改变位置
enum{
	
	CURRENTFLOOR =1,
	TARGERFLOOR = 2,
	WEITHT = 4,
	SHOWFLAG =8,
};


typedef struct Menu_Parameter
{
	u8	Parameter_Change_Flag;//主界面参数改变的标志
	u8	Parameter_Change_Position;//主界面参数改变的位置
	
	u8	Current_Floor;
	u8	Target_Floor;
	u8	Weight_Integer;
	u8	Weight_Decimal;
	u8	Weight_Percentile;
	
	u8	Weight_Percentage;

	u8	Show_Flag;
	
}Menu_Parameter;

void uiProcMain(void);
u8 uiKeyIsDigit(u8 nKey);


#endif

