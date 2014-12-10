#ifndef		_UART_H_
#define		_UART_H_

typedef unsigned char byte;

#define         RELAY_UP_LEN         0
#define         RELAY_DOWN_LEN         0
#define         RELAY_STOP_LEN         0
#define		RELAY_LEN  		0
enum{

        CMD_RT_DATA=0x01,//实时上传重量和16路接口错误代码
        CMD_LEVELING_DATA,//平层传平层的值

        


        //继电器控制的上，下，停止三个转太
        CMD_RELAY_UP,
        CMD_RELAY_DOWN,
        CMD_RELAY_STOP,

};




void Usart2RxDataTOApp(byte data);
unsigned char   Rcv_Cmd(void);
void  FlushUart2(void);

void HB_Send_Realy_CMD(u8 cmd);


void HB_Send_Realy_UP(void);
void HB_Send_Realy_DOWN(void);
void HB_Send_Realy_STOP(void);
#endif

