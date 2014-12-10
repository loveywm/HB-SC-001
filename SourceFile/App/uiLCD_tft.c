

#include "app.h"
#include "system.h"



//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16
//mode:叠加方式(1)还是非叠加方式(0)
u16 POINT_COLOR1 = 0x0000;

void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    	u8 temp,t1,t;
	u16 y0=y;
	u16 colortemp=POINT_COLOR1;      			     
	//设置窗口		   
	num=num-' ';//得到偏移后的值
	if(!mode) //非叠加方式
	{
	    	for(t=0;t<size;t++)
	    	{   
			if(size==12)
				temp=asc2_1206[num][t];  //调用1206字体
			else 
				temp=asc2_1608[num][t];		 //调用1608字体 	                          
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
				}//超区域了
				if((y-y0)==size)
				{
					y=y0;
					x++;
					//if(x>=lcddev.width)
					if(x>=320)
					{
						POINT_COLOR1=colortemp;
						return;
					}//超区域了
					break;
				}
			}  	 
	    	}    
	}else//叠加方式
	{
	    	for(t=0;t<size;t++)
	    	{   
			if(size==12)
				temp=asc2_1206[num][t];  //调用1206字体
			else 
				temp=asc2_1608[num][t];		 //调用1608字体 	                          
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
				}//超区域了
				if((y-y0)==size)
				{
					y=y0;
					x++;
					//if(x>=lcddev.width)
					if(x>=320)
					{
						POINT_COLOR1=colortemp;
						return;
					}//超区域了
					break;
				}
			}  	 
	    }     
	}
	POINT_COLOR1=colortemp;	    	   	 	  
}   



//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    	while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    	{       
        		if(x>=width){x=x0;y+=size;}
        		if(y>=height)break;//退出
       		 LCD_ShowChar(x,y,*p,size,1);
       		x+=size/2;
        		p++;
    	}  
}