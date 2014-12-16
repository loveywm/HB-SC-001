#ifndef __UIFONT_H__
#define __UIFONT_H__



typedef enum
{
	// 0,1,2,... = parent & title, UISTR_SPACE - top menu
	UIMENU_POPUP = -1,						/*有下级菜单*/
	UIMENU_HASVALUE = -2,
	UIMENU_CUSTOM = -3,					/*有需要用户确认界面*/
	UIMENU_CUSTOM_HASVALUE = -4,
	UIMENU_NOACTION = -5
} T_UI_MENUITEM_TYPE;

typedef struct 
{
	short int nMenuStr;
	T_UI_MENUITEM_TYPE nType;
} T_UI_MENUITEM;


extern const unsigned char uiENfont68[][6];
//extern const unsigned char uiENfont616[][12];
extern const unsigned char uiENfont816[][16];
extern const unsigned char uiENfont1632[][64];

extern const unsigned char asc2_1206[95][12];
extern const unsigned char asc2_1608[95][16];


//12*12的显示索引
enum
{
	UISTR_HENGBANGKEJI=0,
	UISTR_DEVICE_IS_OK=4,
	UISTR_DEVICE_IS_ERROR_UP_DOOR=8,//12
	UISTR_GET_PASSWD=13,//+8=20
	
	UISTR_CURRENT_LOU_CHENG = 21,//+4=24
	UISTR_AIM_LOU_CHENG = 25,//28
	UISTR_WEIGHT = 29,//31

	UISTR_PINGCHENG_SET =32,//平层设置
	UISTR_ZAIZHONG_SET = 36,//载重设置
	UISTR_MIMA_SET = 40,//密码设置
	UISTR_ZHAUNTAIZIJIAN_SET= 44,//状态自检

	UISTR_ZAIZHONG_SET_EDINGZHONGLIANG = 48,//额定重量
	UISTR_ZAIZHONG_SET_YUJINGZAIHE = 52,//预警载荷
	UISTR_ZAIZHONG_SET_BAOJINGZAIHE = 56,//报警载荷
	UISTR_ZAIZHONG_SET_ZHONGLIANGZHILING = 60,//重量置零

	UISTR_ZHUSHEZHICAIDAN = 64,//主设置菜单
	
	UISTR_INPUT_PASSWORD = 69,//请输入新密码：

	UISTR_INPUT_LOAD = 76,//请输入标定重量一//8

	UISTR_INPUT_LOAD_TWO = 84,//二


	UISTR_FLAG_IN_DOOR =85,//进料门未关
	UISTR_FLAG_OUT_DOOR =90,//出
	UISTR_FLAG_SKY_DOOR =91,//天窗


	UISTR_ENCODER_FLOOR = 93,//标定楼层

	
};

//16*16的索引
enum
{
	UISTR_MENU,
	UISTR_MENU_PINGCHENG_SET,//平层设置
	UISTR_MENU_ZAIZHONG_SET,//载重设置
	UISTR_MENU_MIMA_SET,//密码设置
	UISTR_MENU_ZIJIAN_SET,//自检状态

	UISTR_MENU_ZAIZHONG_SET_EDINGZHONGLIANG,//额定重量
	UISTR_MENU_ZAIZHONG_SET_YUJINGZAIHE,//预警载荷
	UISTR_MENU_ZAIZHONG_SET_BAOJINGZAIHE,//报警载荷
	UISTR_MENU_ZAIZHONG_SET_ZHONGLIANGZHILING,//重量置零
	UISTR_MENU_ZAIZHONG_SET_BIAODINGDIAN_ONE,//标定点一
	UISTR_MENU_ZAIZHONG_SET_BIAODINGDIAN_TWO,//标定点二

	UISTR_MASTER_UP,//图标上升
	UISTR_MASTER_DOWN,//图标下降


};

//14*14的索引
enum
{
	UISTR_XXX_ZAIZHONG_SET_EDINGZHONGLIANG = 0,//额定重量
	UISTR_XXX_ZAIZHONG_SET_YUJINGZAIHE = 4,//预警载荷
	UISTR_XXX_ZAIZHONG_SET_BAOJINGZAIHE = 8,//报警载荷
	UISTR_XXX_ZAIZHONG_SET_ZHONGLIANGZHILING = 12,//重量置零

	UISTR_XXX_OK = 16,	//确认
	UISTR_XXX_ESC = 18,	//退出

	UISTR_XXX_SAVE = 20,	//保存
	UISTR_XXX_DEL = 22,	//取消

	UISTR_XXX_ZAIZHONG_SET_BIAODINGDIAN_ONE = 24,///设置标定点一
	UISTR_XXX_ZAIZHONG_SET_BIAODINGDIAN_TWO = 30,//设置标定点二

	UISTR_XXX_SUCESS = 36,//确认
	UISTR_XXX_FAIL = 38,	//退出

};

#endif//__UIFONT_H__
