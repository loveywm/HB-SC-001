#ifndef		_UART_H_
#define		_UART_H_

typedef unsigned char byte;

#define         RELAY_UP_LEN         0
#define         RELAY_DOWN_LEN         0
#define         RELAY_STOP_LEN         0
#define		RELAY_LEN  		0

#define         CMD_CURRENT_COUNT_DATA_LEN         4

enum{

        CMD_RT_DATA=0x01,//实时上传重量和16路接口错误代码
        CMD_LEVELING_DATA,//平层传平层的值

        


        //继电器控制的上，下，停止三个转太
        CMD_RELAY_UP,
        CMD_RELAY_DOWN,
        CMD_RELAY_STOP,

	//平层相关的控制代码
        CMD_LEVEL_UPDATA_CURRENT_COUNT,//更新当前编码器计数值
        CMD_LEVEL_UPDATA_FLOOR,//更新楼层数据结构指令
        CMD_LEVEL_UPDATA_LAST_COUNT,//跟新上一次保留的计数器值

};

///////////////////////////////////帧描述
//帧头
#define   PROTOCOL_HEAD_1       0x5A
#define   PROTOCOL_HEAD_1_COD   0xA5
#define   PROTOCOL_HEAD_2       0x55

//帧尾
#define   PROTOCOL_TAIL_1       0x6A
#define   PROTOCOL_TAIL_1_COD   0x95
#define   PROTOCOL_TAIL_2       0x69

//转义符
#define   PROTOCOL_ESC_CHAR     0x99
#define   PROTOCOL_ESC_CHAR_COD 0x66


void Usart2RxDataTOApp(byte data);
unsigned char   Rcv_Cmd(void);
void  FlushUart2(void);

void HB_Send_Realy_CMD(u8 cmd);


void HB_Send_Realy_UP(void);
void HB_Send_Realy_DOWN(void);
void HB_Send_Realy_STOP(void);


void HB_Send_Current_Count(u32 count);
void HB_Send_Floor(Floor_Data   *floor);
void HB_Send_Last_Count(u32 count);
#endif

