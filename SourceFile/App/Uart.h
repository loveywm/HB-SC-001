#ifndef		_UART_H_
#define		_UART_H_

typedef unsigned char byte;

#define         RELAY_UP_LEN         0
#define         RELAY_DOWN_LEN         0
#define         RELAY_STOP_LEN         0
#define		RELAY_LEN  		0
enum{

        CMD_RT_DATA=0x01,//ʵʱ�ϴ�������16·�ӿڴ������
        CMD_LEVELING_DATA,//ƽ�㴫ƽ���ֵ

        


        //�̵������Ƶ��ϣ��£�ֹͣ����ת̫
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

