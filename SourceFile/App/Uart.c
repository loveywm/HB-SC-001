#include "system.h"
#include "app.h"





//串口循环队列接受方式
#define UART2_RX_BUFFER_SIZE  256
#define UART2_RX_BUFFER_MASK  255
static unsigned char UART2_RxBuf[UART2_RX_BUFFER_SIZE];
static volatile unsigned char UART2_RxHead;
static volatile unsigned char UART2_RxTail;

//供其他层调用，基本上是串口接受中断后传递值参数给data
void Usart2RxDataTOApp(byte data)
{
	UART2_RxHead = ( UART2_RxHead + 1 ) & UART2_RX_BUFFER_MASK;
	if( UART2_RxHead == UART2_RxTail )
	{
		/* ERROR! Receive buffer overflow */
	}
	UART2_RxBuf[UART2_RxHead] = data; /* store received data in buffer */
}

unsigned char ReceiveByte2(char* pdata)
{

	if(UART2_RxHead == UART2_RxTail)
	    return 0;

	UART2_RxTail = ( UART2_RxTail + 1 )&UART2_RX_BUFFER_MASK;/* calculate buffer index and store new index*/
	*pdata = UART2_RxBuf[UART2_RxTail];
	return 1;

}


static void TransmitByte2( unsigned char data )
{	
        System.Device.Usart2.WriteByte(data);
}

void  FlushUart2(void)
{
    	UART2_RxTail = UART2_RxHead;//UART2_RxTail = UART2_RxHead;
}



/////////////////////////////////////////////
#define    R_DATA_BUFF_LEN  256    //之所以要定义256个长度的数组，就是为了能够让数组“首尾相接”。因为0 -1 = 255 ， 255+1 = 0。
#define    R_DATA_BUFF_MASK 255
unsigned char rmote_buff[R_DATA_BUFF_LEN];
unsigned int rmote_head_index = 0;//存储数据的指针
unsigned int rmote_rep_index  = R_DATA_BUFF_LEN + 1;//头位置
unsigned char is_find_head = 0;

unsigned char   Rcv_Cmd(void)
{
    	int  i = 0;
	//rmote_head_index = 0;
    	while(ReceiveByte2(&rmote_buff[rmote_head_index]))
  	{
		//第一次寻找头
		if((rmote_rep_index == R_DATA_BUFF_LEN+1)&&(rmote_buff[rmote_head_index] == PROTOCOL_HEAD_1))//找到头了
		{ 
		 	rmote_rep_index = rmote_head_index;
			//printf("111rmote_head_index==%d\r\n",rmote_head_index);
			//printf("111rmote_rep_index==%d\r\n",rmote_rep_index);
		}
		
		//第二次找头
		if((rmote_rep_index != rmote_head_index)&&(rmote_rep_index != (R_DATA_BUFF_LEN+1)))
		{
		    	if(is_find_head == 0)
		    	{
				//printf("222is_find_head==%d\r\n",is_find_head);
				if(rmote_buff[(rmote_rep_index+1)&R_DATA_BUFF_MASK] == PROTOCOL_HEAD_2)
			    		is_find_head = 1;
			    	else
					rmote_rep_index = R_DATA_BUFF_LEN+1;
			}
		}
		
		//两个头都找到后，就开始处理数据了
		if(is_find_head)//若已经找到报文头了
		{
			//printf("333is_find_head==%d\r\n",is_find_head);
			if((rmote_buff[rmote_head_index] == PROTOCOL_TAIL_2)&&
			   (rmote_buff[(rmote_head_index+R_DATA_BUFF_MASK)&R_DATA_BUFF_MASK] == PROTOCOL_TAIL_1))//找到第2尾了
			{
			    	is_find_head = 0;
				//printf("444is_find_head==%d\r\n",is_find_head);
				//printf("we find it!!\r\n");
				
				if(rmote_head_index>rmote_rep_index)//接受的值没有循环超过圆形队列值
				{
		   	        		memcpy(fram_data_buff,rmote_buff+rmote_rep_index+2,rmote_head_index-rmote_rep_index-3);
					fram_len = rmote_head_index-rmote_rep_index-3;
					rmote_rep_index = R_DATA_BUFF_LEN+1;
				}
				else if(rmote_head_index<rmote_rep_index)//接受值过了循环队列，需要处理
				{
				    	if(rmote_rep_index<(R_DATA_BUFF_MASK-1))
					{
   			            		memcpy(fram_data_buff,rmote_buff+((rmote_rep_index+2)&R_DATA_BUFF_MASK),R_DATA_BUFF_MASK-rmote_rep_index-1);
						if(rmote_head_index>1)
						{
							memcpy(fram_data_buff+R_DATA_BUFF_MASK-rmote_rep_index-1,rmote_buff,rmote_head_index-1);
						}
	        			    			
				 	    	fram_len = R_DATA_BUFF_MASK-rmote_rep_index+rmote_head_index-2;
						rmote_rep_index = R_DATA_BUFF_LEN+1;
					}
					else if(rmote_rep_index == (R_DATA_BUFF_MASK-1))
					{
        			    			memcpy(fram_data_buff,rmote_buff,rmote_head_index-1);
				 	   	fram_len = rmote_head_index-1;
						rmote_rep_index = R_DATA_BUFF_LEN+1;
					}
					else 
					{
	        					memcpy(fram_data_buff,rmote_buff+1,rmote_head_index-2);
				 	    	fram_len = rmote_head_index-2;
						rmote_rep_index = R_DATA_BUFF_LEN+1;
					}
				}


				//FlushUart2();
				return 1;//接受完成后退出
				
			}
			else
			{
				//当接受的数值超过最大数据包值还没找到结尾时，就丢失此包
				//这个长度是自己定义的，注意等式后面的数字
				if(((rmote_head_index - rmote_rep_index+R_DATA_BUFF_LEN)&R_DATA_BUFF_MASK) >32 )//MAX_FRAM_LEN
				{
					 //认为是无效报文头了，则重新找头
					is_find_head = 0;
					if(rmote_head_index>rmote_rep_index)
					{
					    	for(i=rmote_rep_index+1;i<=rmote_head_index;i++)
						{
						    	if(rmote_buff[i]==PROTOCOL_HEAD_1)
							{
					            		rmote_rep_index = i;
							    	if(i != rmote_head_index)
								{
								    	if(rmote_buff[i+1] == PROTOCOL_HEAD_2)
									{
									    	is_find_head = 1;
										break;
									}
									else
									{
							            		rmote_rep_index = R_DATA_BUFF_LEN+1;
									}
								}
								else
								{
								}
							}
							else
							{
							    	if(i == rmote_head_index) // 最后一个元素
								    	rmote_rep_index = (R_DATA_BUFF_LEN+1);
							}
						}
					}
					else if(rmote_head_index<rmote_rep_index)
					{
					    	for(i=rmote_rep_index+1;i<R_DATA_BUFF_LEN;i++)
						{
						    	if(rmote_buff[i]==PROTOCOL_HEAD_1)
							{
							    	rmote_rep_index = i;
							    	if(i != R_DATA_BUFF_MASK)
								{
								    	if(rmote_buff[i+1] == PROTOCOL_HEAD_2)
									{
									    	is_find_head = 1;
										break;
									}
									else
									{
							            		rmote_rep_index = R_DATA_BUFF_LEN+1;
									}
								}
								else
								{
								    	if(rmote_buff[0] == PROTOCOL_HEAD_2)
									{
									    	is_find_head = 1;
										break;
									}
									else
									{
							            		rmote_rep_index = R_DATA_BUFF_LEN+1;
									}
								}
							}
							else
							{
							    	if(i == R_DATA_BUFF_MASK)
								    	rmote_rep_index = (R_DATA_BUFF_LEN+1);
							}
						}
						if(rmote_rep_index == (R_DATA_BUFF_LEN+1))// 没找到，继续找
						{
					        		for(i=0;i<=rmote_head_index;i++)
						    	{
						        		if(rmote_buff[i]==PROTOCOL_HEAD_1)
							    	{
					                			rmote_rep_index = i;
							    		if(i != rmote_head_index)
									{
								        		if(rmote_buff[i+1] == PROTOCOL_HEAD_2)
										{
									        		is_find_head = 1;
											break;
									    	}
										else
										{
							                			rmote_rep_index = R_DATA_BUFF_LEN+1;
										}
									}
									else
									{
									}
								}
								else
								{
								}
						    	}
						}
					
			    		}
				}
			}
		}
		rmote_head_index++;
		rmote_head_index &= R_DATA_BUFF_MASK;
	}
	return 0;
}


/*
        协议格式:
        头+命令字+数据长度+数据+校验和+尾

        校验和:是从命令字到数据的和


        暂时不做应答部分，日后添加
*/
unsigned char  Ready_One_Byte_To_Send(unsigned char data)
{
	unsigned char sum = 0;
	//char buff[1];
	if(( data== PROTOCOL_HEAD_1)||
	   (data == PROTOCOL_TAIL_1)||
	   (data == PROTOCOL_ESC_CHAR))
	{
	    	sum += PROTOCOL_ESC_CHAR;
		sum += 0xff-data;
	    	TransmitByte2(PROTOCOL_ESC_CHAR);
	    	TransmitByte2(0xff-data);
	}
	else
	{
	    	TransmitByte2(data);
		sum += data;
	}	
	return sum;
}

//void HB_Send_Cmd(u8 CMD,u8 Data_Len,u8 *data){}
void HB_Send_Realy_UP(void)
{
        unsigned char  sum = 0;
        //头
        TransmitByte2(PROTOCOL_HEAD_1);
        TransmitByte2(PROTOCOL_HEAD_2);
        //命令字
        sum+=Ready_One_Byte_To_Send(CMD_RELAY_UP);
        //数据长度
        sum+=Ready_One_Byte_To_Send(RELAY_UP_LEN);
        //校验和
        TransmitByte2(sum);
        //尾
        TransmitByte2(PROTOCOL_TAIL_1);
        TransmitByte2(PROTOCOL_TAIL_2);
}

void HB_Send_Realy_DOWN(void)
{
        unsigned char  sum = 0;
        //头
        TransmitByte2(PROTOCOL_HEAD_1);
        TransmitByte2(PROTOCOL_HEAD_2);
        //命令字
        sum+=Ready_One_Byte_To_Send(CMD_RELAY_DOWN);
        //数据长度
        sum+=Ready_One_Byte_To_Send(RELAY_DOWN_LEN);
        //校验和
        TransmitByte2(sum);
        //尾
        TransmitByte2(PROTOCOL_TAIL_1);
        TransmitByte2(PROTOCOL_TAIL_2);
}
void HB_Send_Realy_STOP(void)
{
        unsigned char  sum = 0;
        //头
        TransmitByte2(PROTOCOL_HEAD_1);
        TransmitByte2(PROTOCOL_HEAD_2);
        //命令字
        sum+=Ready_One_Byte_To_Send(CMD_RELAY_STOP);
        //数据长度
        sum+=Ready_One_Byte_To_Send(RELAY_STOP_LEN);
        //校验和
        TransmitByte2(sum);
        //尾
        TransmitByte2(PROTOCOL_TAIL_1);
        TransmitByte2(PROTOCOL_TAIL_2);
}

//发送当前编码器计数器的值
void HB_Send_Current_Count(u32 count)
{
        unsigned char  sum = 0;

        //头
        TransmitByte2(PROTOCOL_HEAD_1);
        TransmitByte2(PROTOCOL_HEAD_2);

        //命令字
        sum+=Ready_One_Byte_To_Send(CMD_LEVEL_UPDATA_CURRENT_COUNT);

        //数据长度
        sum+=Ready_One_Byte_To_Send(CMD_CURRENT_COUNT_DATA_LEN);

          //32位重量
        sum+=Ready_One_Byte_To_Send(count);//低位在前
        sum+=Ready_One_Byte_To_Send(count>>8);//高位在后
        sum+=Ready_One_Byte_To_Send(count>>16);
        sum+=Ready_One_Byte_To_Send(count>>24);
        
        //校验和
        TransmitByte2(sum);
        
        //尾
        TransmitByte2(PROTOCOL_TAIL_1);
        TransmitByte2(PROTOCOL_TAIL_2);

}

//发送前一次关机前保存的编码器计数器的值
void HB_Send_Last_Count(u32 count)
{
        unsigned char  sum = 0;
        //头
        TransmitByte2(PROTOCOL_HEAD_1);
        TransmitByte2(PROTOCOL_HEAD_2);
        //命令字
        sum+=Ready_One_Byte_To_Send(CMD_LEVEL_UPDATA_LAST_COUNT);
        //数据长度
        sum+=Ready_One_Byte_To_Send(CMD_CURRENT_COUNT_DATA_LEN);
          //32位重量
        sum+=Ready_One_Byte_To_Send(count);//低位在前
        sum+=Ready_One_Byte_To_Send(count>>8);//高位在后
        sum+=Ready_One_Byte_To_Send(count>>16);
        sum+=Ready_One_Byte_To_Send(count>>24);
        //校验和
        TransmitByte2(sum);
        //尾
        TransmitByte2(PROTOCOL_TAIL_1);
        TransmitByte2(PROTOCOL_TAIL_2);

}

//发送楼层更新数据
void HB_Send_Floor(Floor_Data   *floor)
{
        //        char i;
        unsigned char  sum = 0;

        //头
        TransmitByte2(PROTOCOL_HEAD_1);
        TransmitByte2(PROTOCOL_HEAD_2);

        //命令字
        sum+=Ready_One_Byte_To_Send(CMD_LEVEL_UPDATA_FLOOR);

        //数据长度
        sum+=Ready_One_Byte_To_Send(CMD_CURRENT_COUNT_DATA_LEN+2);

        sum+=Ready_One_Byte_To_Send(floor->floor_flag);//低位在前
        sum+=Ready_One_Byte_To_Send(floor->floor_count);//高位在后3


        sum+=Ready_One_Byte_To_Send(floor->floor_num);//低位在前4
        sum+=Ready_One_Byte_To_Send((floor->floor_num)>>8);//高位在后5
        sum+=Ready_One_Byte_To_Send((floor->floor_num)>>16);//6
        sum+=Ready_One_Byte_To_Send((floor->floor_num)>>24);//7
        
        //校验和
        TransmitByte2(sum);
        
        //尾
        TransmitByte2(PROTOCOL_TAIL_1);
        TransmitByte2(PROTOCOL_TAIL_2);
}




void HB_Send_Realy_CMD(u8 cmd)
{
        unsigned char  sum = 0;
        //头
        TransmitByte2(PROTOCOL_HEAD_1);
        TransmitByte2(PROTOCOL_HEAD_2);
        //命令字
        sum+=Ready_One_Byte_To_Send(cmd);
        //数据长度
        sum+=Ready_One_Byte_To_Send(RELAY_LEN);
        //校验和
        TransmitByte2(sum);
        //尾
        TransmitByte2(PROTOCOL_TAIL_1);
        TransmitByte2(PROTOCOL_TAIL_2);
}



