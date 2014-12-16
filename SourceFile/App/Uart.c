#include "system.h"
#include "app.h"





//����ѭ�����н��ܷ�ʽ
#define UART2_RX_BUFFER_SIZE  256
#define UART2_RX_BUFFER_MASK  255
static unsigned char UART2_RxBuf[UART2_RX_BUFFER_SIZE];
static volatile unsigned char UART2_RxHead;
static volatile unsigned char UART2_RxTail;

//����������ã��������Ǵ��ڽ����жϺ󴫵�ֵ������data
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
#define    R_DATA_BUFF_LEN  256    //֮����Ҫ����256�����ȵ����飬����Ϊ���ܹ������顰��β��ӡ�����Ϊ0 -1 = 255 �� 255+1 = 0��
#define    R_DATA_BUFF_MASK 255
unsigned char rmote_buff[R_DATA_BUFF_LEN];
unsigned int rmote_head_index = 0;//�洢���ݵ�ָ��
unsigned int rmote_rep_index  = R_DATA_BUFF_LEN + 1;//ͷλ��
unsigned char is_find_head = 0;

unsigned char   Rcv_Cmd(void)
{
    	int  i = 0;
	//rmote_head_index = 0;
    	while(ReceiveByte2(&rmote_buff[rmote_head_index]))
  	{
		//��һ��Ѱ��ͷ
		if((rmote_rep_index == R_DATA_BUFF_LEN+1)&&(rmote_buff[rmote_head_index] == PROTOCOL_HEAD_1))//�ҵ�ͷ��
		{ 
		 	rmote_rep_index = rmote_head_index;
			//printf("111rmote_head_index==%d\r\n",rmote_head_index);
			//printf("111rmote_rep_index==%d\r\n",rmote_rep_index);
		}
		
		//�ڶ�����ͷ
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
		
		//����ͷ���ҵ��󣬾Ϳ�ʼ����������
		if(is_find_head)//���Ѿ��ҵ�����ͷ��
		{
			//printf("333is_find_head==%d\r\n",is_find_head);
			if((rmote_buff[rmote_head_index] == PROTOCOL_TAIL_2)&&
			   (rmote_buff[(rmote_head_index+R_DATA_BUFF_MASK)&R_DATA_BUFF_MASK] == PROTOCOL_TAIL_1))//�ҵ���2β��
			{
			    	is_find_head = 0;
				//printf("444is_find_head==%d\r\n",is_find_head);
				//printf("we find it!!\r\n");
				
				if(rmote_head_index>rmote_rep_index)//���ܵ�ֵû��ѭ������Բ�ζ���ֵ
				{
		   	        		memcpy(fram_data_buff,rmote_buff+rmote_rep_index+2,rmote_head_index-rmote_rep_index-3);
					fram_len = rmote_head_index-rmote_rep_index-3;
					rmote_rep_index = R_DATA_BUFF_LEN+1;
				}
				else if(rmote_head_index<rmote_rep_index)//����ֵ����ѭ�����У���Ҫ����
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
				return 1;//������ɺ��˳�
				
			}
			else
			{
				//�����ܵ���ֵ����������ݰ�ֵ��û�ҵ���βʱ���Ͷ�ʧ�˰�
				//����������Լ�����ģ�ע���ʽ���������
				if(((rmote_head_index - rmote_rep_index+R_DATA_BUFF_LEN)&R_DATA_BUFF_MASK) >32 )//MAX_FRAM_LEN
				{
					 //��Ϊ����Ч����ͷ�ˣ���������ͷ
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
							    	if(i == rmote_head_index) // ���һ��Ԫ��
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
						if(rmote_rep_index == (R_DATA_BUFF_LEN+1))// û�ҵ���������
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
        Э���ʽ:
        ͷ+������+���ݳ���+����+У���+β

        У���:�Ǵ������ֵ����ݵĺ�


        ��ʱ����Ӧ�𲿷֣��պ����
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
        //ͷ
        TransmitByte2(PROTOCOL_HEAD_1);
        TransmitByte2(PROTOCOL_HEAD_2);
        //������
        sum+=Ready_One_Byte_To_Send(CMD_RELAY_UP);
        //���ݳ���
        sum+=Ready_One_Byte_To_Send(RELAY_UP_LEN);
        //У���
        TransmitByte2(sum);
        //β
        TransmitByte2(PROTOCOL_TAIL_1);
        TransmitByte2(PROTOCOL_TAIL_2);
}

void HB_Send_Realy_DOWN(void)
{
        unsigned char  sum = 0;
        //ͷ
        TransmitByte2(PROTOCOL_HEAD_1);
        TransmitByte2(PROTOCOL_HEAD_2);
        //������
        sum+=Ready_One_Byte_To_Send(CMD_RELAY_DOWN);
        //���ݳ���
        sum+=Ready_One_Byte_To_Send(RELAY_DOWN_LEN);
        //У���
        TransmitByte2(sum);
        //β
        TransmitByte2(PROTOCOL_TAIL_1);
        TransmitByte2(PROTOCOL_TAIL_2);
}
void HB_Send_Realy_STOP(void)
{
        unsigned char  sum = 0;
        //ͷ
        TransmitByte2(PROTOCOL_HEAD_1);
        TransmitByte2(PROTOCOL_HEAD_2);
        //������
        sum+=Ready_One_Byte_To_Send(CMD_RELAY_STOP);
        //���ݳ���
        sum+=Ready_One_Byte_To_Send(RELAY_STOP_LEN);
        //У���
        TransmitByte2(sum);
        //β
        TransmitByte2(PROTOCOL_TAIL_1);
        TransmitByte2(PROTOCOL_TAIL_2);
}

//���͵�ǰ��������������ֵ
void HB_Send_Current_Count(u32 count)
{
        unsigned char  sum = 0;

        //ͷ
        TransmitByte2(PROTOCOL_HEAD_1);
        TransmitByte2(PROTOCOL_HEAD_2);

        //������
        sum+=Ready_One_Byte_To_Send(CMD_LEVEL_UPDATA_CURRENT_COUNT);

        //���ݳ���
        sum+=Ready_One_Byte_To_Send(CMD_CURRENT_COUNT_DATA_LEN);

          //32λ����
        sum+=Ready_One_Byte_To_Send(count);//��λ��ǰ
        sum+=Ready_One_Byte_To_Send(count>>8);//��λ�ں�
        sum+=Ready_One_Byte_To_Send(count>>16);
        sum+=Ready_One_Byte_To_Send(count>>24);
        
        //У���
        TransmitByte2(sum);
        
        //β
        TransmitByte2(PROTOCOL_TAIL_1);
        TransmitByte2(PROTOCOL_TAIL_2);

}

//����ǰһ�ιػ�ǰ����ı�������������ֵ
void HB_Send_Last_Count(u32 count)
{
        unsigned char  sum = 0;
        //ͷ
        TransmitByte2(PROTOCOL_HEAD_1);
        TransmitByte2(PROTOCOL_HEAD_2);
        //������
        sum+=Ready_One_Byte_To_Send(CMD_LEVEL_UPDATA_LAST_COUNT);
        //���ݳ���
        sum+=Ready_One_Byte_To_Send(CMD_CURRENT_COUNT_DATA_LEN);
          //32λ����
        sum+=Ready_One_Byte_To_Send(count);//��λ��ǰ
        sum+=Ready_One_Byte_To_Send(count>>8);//��λ�ں�
        sum+=Ready_One_Byte_To_Send(count>>16);
        sum+=Ready_One_Byte_To_Send(count>>24);
        //У���
        TransmitByte2(sum);
        //β
        TransmitByte2(PROTOCOL_TAIL_1);
        TransmitByte2(PROTOCOL_TAIL_2);

}

//����¥���������
void HB_Send_Floor(Floor_Data   *floor)
{
        //        char i;
        unsigned char  sum = 0;

        //ͷ
        TransmitByte2(PROTOCOL_HEAD_1);
        TransmitByte2(PROTOCOL_HEAD_2);

        //������
        sum+=Ready_One_Byte_To_Send(CMD_LEVEL_UPDATA_FLOOR);

        //���ݳ���
        sum+=Ready_One_Byte_To_Send(CMD_CURRENT_COUNT_DATA_LEN+2);

        sum+=Ready_One_Byte_To_Send(floor->floor_flag);//��λ��ǰ
        sum+=Ready_One_Byte_To_Send(floor->floor_count);//��λ�ں�3


        sum+=Ready_One_Byte_To_Send(floor->floor_num);//��λ��ǰ4
        sum+=Ready_One_Byte_To_Send((floor->floor_num)>>8);//��λ�ں�5
        sum+=Ready_One_Byte_To_Send((floor->floor_num)>>16);//6
        sum+=Ready_One_Byte_To_Send((floor->floor_num)>>24);//7
        
        //У���
        TransmitByte2(sum);
        
        //β
        TransmitByte2(PROTOCOL_TAIL_1);
        TransmitByte2(PROTOCOL_TAIL_2);
}




void HB_Send_Realy_CMD(u8 cmd)
{
        unsigned char  sum = 0;
        //ͷ
        TransmitByte2(PROTOCOL_HEAD_1);
        TransmitByte2(PROTOCOL_HEAD_2);
        //������
        sum+=Ready_One_Byte_To_Send(cmd);
        //���ݳ���
        sum+=Ready_One_Byte_To_Send(RELAY_LEN);
        //У���
        TransmitByte2(sum);
        //β
        TransmitByte2(PROTOCOL_TAIL_1);
        TransmitByte2(PROTOCOL_TAIL_2);
}



