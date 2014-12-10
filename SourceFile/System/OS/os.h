/*
********************************************************************************
*                            
********************************************************************************
*文件名   : os.h
*作用     : RTOS头文件
*创建时间 : 2013.7.21
********************************************************************************
*/

#ifndef __OS_H
#define __OS_H

/*以下三个宏定义，必须要跟实际配套，否则出现异常*/
#define TaskSum     2           /*任务总数*/
#define EventSum    1           /*事件块总数*/                            			 
#define QueueSum    1           /*消息队列总数*/

typedef struct                  //定义队列
{
    void    * * Start;          //指向队列开始
    void    * * End;            //指向队列结束
    void    * * In;             //插入一个消息
    void    * * Out;            //取出一个消息
    ushort      Size;           //队列长度
    ushort      Entries;        //消息长度
} Queue;

typedef struct                  //定义事件控制块
{                                               
    byte 	Type;               //事件类型
    ushort 	Counter;            //信号
    void  * MessagePointer;     //消息指针
    byte 	PriorityTable;      //等待任务优先级表
} Event;
  
extern void InitializeOs(void);

#endif






















