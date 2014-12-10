

#include "app.h"
#include "system.h"



//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
u16 POINT_COLOR1 = 0x0000;

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    	u8 temp,t1,t;
	u16 y0=y;
	u16 colortemp=POINT_COLOR1;      			     
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	if(!mode) //�ǵ��ӷ�ʽ
	{
	    	for(t=0;t<size;t++)
	    	{   
			if(size==12)
				temp=asc2_1206[num][t];  //����1206����
			else 
				temp=asc2_1608[num][t];		 //����1608���� 	                          
	        		for(t1=0;t1<8;t1++)
			{			    
		        		if(temp&0x80)
					POINT_COLOR1=colortemp;
				else 
					POINT_COLOR1= 0xFFFF;
				System.Device.Lcd.LCD_DrawPoint(x,y);
				//System.Device.LCD_DrawPoint(x,y);	
				temp<<=1;
				y++;
				//if(x>=lcddev.width)
				if(x>=320)
				{
					POINT_COLOR1=colortemp;
					return;
				}//��������
				if((y-y0)==size)
				{
					y=y0;
					x++;
					//if(x>=lcddev.width)
					if(x>=320)
					{
						POINT_COLOR1=colortemp;
						return;
					}//��������
					break;
				}
			}  	 
	    	}    
	}else//���ӷ�ʽ
	{
	    	for(t=0;t<size;t++)
	    	{   
			if(size==12)
				temp=asc2_1206[num][t];  //����1206����
			else 
				temp=asc2_1608[num][t];		 //����1608���� 	                          
	        		for(t1=0;t1<8;t1++)
			{			    
		        		if(temp&0x80)
					System.Device.Lcd.LCD_DrawPoint(x,y);
				temp<<=1;
				y++;
				//if(x>=lcddev.height)
				if(x>=480)
				{
					POINT_COLOR1=colortemp;
					return;
				}//��������
				if((y-y0)==size)
				{
					y=y0;
					x++;
					//if(x>=lcddev.width)
					if(x>=320)
					{
						POINT_COLOR1=colortemp;
						return;
					}//��������
					break;
				}
			}  	 
	    }     
	}
	POINT_COLOR1=colortemp;	    	   	 	  
}   



//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    	while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    	{       
        		if(x>=width){x=x0;y+=size;}
        		if(y>=height)break;//�˳�
       		 LCD_ShowChar(x,y,*p,size,1);
       		x+=size/2;
        		p++;
    	}  
}