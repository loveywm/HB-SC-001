/*
********************************************************************************
*                                Ƕ��ʽ΢ϵͳ
*                                   msOS
*
*                            Ӳ��ƽ̨:msPLC DEMO
*                          ��оƬ:STM32F103R8T6/RBT6
*                           ��������οƼ����޹�˾
*
*                                ����:����ΰ
*                                ����:������
*                                ��ʶ:Wangsw
*
*                                QQ:26033613
*                               QQȺ:291235815
*                        �Ա���:http://52edk.taobao.com
*                      ��̳:http://gongkong.eefocus.com/bbs/
*                ����:http://forum.eet-cn.com/BLOG_wangsw317_1268.HTM
********************************************************************************
*�ļ���     : gui.c
*����       : ͼ�ο��ļ�
*ԭ��       : ���������ƣ���һ��Menu��Ϊ���Form,һ��Form��������ؼ�����Chart��
*           : Label��TextBox�ȣ�����һ��������ȼ�����MenuTaskȥ����ִ����Щ�ؼ���Ϣ
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/07/21       ��ʼ�汾
********************************************************************************
*/

#include "system.h"
//loveywm 20140906
#include "uilcd_12864.h"
#include "uifont.h"
static char GuiBuffer[4][16];
static char DisplayBuffer[4][16];
static byte TextBoxFocusFlash;

static Form * FocusFormPointer;

/*******************************************************************************
* ������	: ParseBackText
* ����	    : ��������ɨ��ˢ��
* �������  : formPointer ��ǰ����ָ��
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
*******************************************************************************/
static void ParseBackText(Form * formPointer)
{
    if(formPointer->BackTextPointer == (byte *)0)
        memset(GuiBuffer, ' ', 64);  
    else
        memcpy(GuiBuffer, formPointer->BackTextPointer, 64);
}


/*******************************************************************************
* ������	: ParseChartParse
* ����	    : Chartͼ�α�ؼ�ɨ��ˢ��
* �������  : formPointer ��ǰ����ָ��
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
*******************************************************************************/
static void ParseChart(Form * formPointer)
{
    byte i, j;
    char character;
    byte column;

    if(formPointer->ChartPointer == (Chart *)0) return;
        
    character = formPointer->ChartPointer->Character;
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 16; j++)
        {
            column = formPointer->ChartPointer->Column[j];
            Assert(column < 5);
            
            if(column >= (4 - i))
                GuiBuffer[i][j] = character;
            else
                GuiBuffer[i][j] = ' ';
        }
    }
}


/*******************************************************************************
* ������	: ParseLabel
* ����	    : Label�ؼ�ɨ��ˢ�£�Labelһ�������ı���ʾ�������ܰ����������
* �������  : formPointer ��ǰ����ָ��
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
*******************************************************************************/
static void ParseLabel(Form * formPointer)
{
    byte i, x, y,remainder;
    uint data;
    RtcStruct * rtcPointer;
    bool plusFlag;
    string string;
    Label * labelPointer;
    
    labelPointer = formPointer->LabelPointer;
    x = labelPointer->X;
    y = labelPointer->Y;

    while(labelPointer != (Label *)0)
    {
        switch(labelPointer->Type)
        {
            case GuiDataTypeIntDec:
                data = *((uint *)(labelPointer->DataPointer));
                
                if(GetBit(data, 31) == 1)
                    plusFlag = false;
                else
                    plusFlag = true;

            case GuiDataTypeUintDec:
                Assert(labelPointer->DataPointer != null);
                data = *((uint *)(labelPointer->DataPointer));
                
                if(plusFlag == false) data = (~data) + 1;

                data = data + labelPointer->Offset;
                
    			for (i = 0; i < labelPointer->Shift; i++) 
                   data = data / 10;

                if(data == 0) GuiBuffer[y][x] = '0';
                
    			while (data) 
                {
    				remainder = data % 10;
    				data =  data / 10;
                    
    				if (data ||remainder) 
                        GuiBuffer[y][x] = HexToAscii(remainder);

                    if(x) x--;
                }
                
                if(plusFlag == false) GuiBuffer[y][x] = '-';

                plusFlag = true;
                break;

            case GuiDataTypeHex:
                Assert(labelPointer->DataPointer != null);
                data = *((uint *)(labelPointer->DataPointer));
                data = data >> (labelPointer->Shift);

                i = labelPointer->Flexible;
                if (i)
                {
                    while(i--)
                    {
                        GuiBuffer[y][x] = HexToAscii(data & 0x0000000F);
                        data = data >> 4;
                        if(x) x--;
                    }
                }
                else
                {
                    if (data == 0) GuiBuffer[y][x] = '0';
                    
                    while(data)
                    {
                        GuiBuffer[y][x] = HexToAscii(data & 0x0000000F);
                        data = data >> 4;
                        if(x) x--;
                    }
                }
                break;

            case GuiDataTypeString:
                Assert(labelPointer->DataPointer != null);
                data = *((uint *)(labelPointer->DataPointer));
                string = labelPointer->StringBlockPointer[data];
                i = strlen(string);
                if(labelPointer->Align == GuiDataAlignRight)
                {
                    Assert(x > i);
                    x = x - i + 1;
                }
                while(i--)
                {
                    GuiBuffer[y][x] = *string++;
                    if(x < 15) x++;
                }
                break;
                
            case GuiDataTypeDate:
                Assert(labelPointer->DataPointer != null);
                data = *((uint *)(labelPointer->DataPointer));
//��
                i = UintToByte0(data);
                Assert(i < 32);
                GuiBuffer[y][x] = HexToAscii(i % 10);
                Assert(x > 0);
                x--;
                GuiBuffer[y][x] = HexToAscii(i / 10);
                Assert(x > 0);
                x--;
                GuiBuffer[y][x] = '-';
                Assert(x > 0);
                x--;
//��
                i = UintToByte1(data);
                Assert(i < 13);
                GuiBuffer[y][x] = HexToAscii(i % 10);
                Assert(x > 0);
                x--;
                GuiBuffer[y][x] = HexToAscii(i / 10);
                Assert(x > 0);
                x--;
                GuiBuffer[y][x] = '-';
                Assert(x > 0);
                x--;
//��
                data = data >> 16;
                Assert(data < 2100);
                
                if(data == 0) GuiBuffer[y][x] = '0';
                    
    			while (data) 
                {
    				remainder = data % 10;
    				data =  data / 10;
    				if (data ||remainder)
    					GuiBuffer[y][x] = HexToAscii(remainder);

                    if(x) x--;
                }
                break;
                
            case GuiDataTypeTime:
                Assert(labelPointer->DataPointer != null);
                rtcPointer = (RtcStruct *)(labelPointer->DataPointer);
//��
                i = rtcPointer->Second;
                Assert(i < 60);
                GuiBuffer[y][x] = HexToAscii(i % 10);
                Assert(x > 0);
                x--;
                GuiBuffer[y][x] = HexToAscii(i / 10);
                Assert(x > 0);
                x--;
                GuiBuffer[y][x] = ':';
                Assert(x > 0);
                x--;
//��
                i = rtcPointer->Minute;
                Assert(i < 60);
                GuiBuffer[y][x] = HexToAscii(i % 10);
                Assert(x > 0);
                x--;
                GuiBuffer[y][x] = HexToAscii(i / 10);
                Assert(x > 0);
                x--;
                GuiBuffer[y][x] = ':';
                Assert(x > 0);
                x--;
//ʱ
                i = rtcPointer->Hour;
                Assert(i < 24);
                GuiBuffer[y][x] = HexToAscii(i % 10);
                Assert(x > 0);
                x--;
                GuiBuffer[y][x] = HexToAscii(i / 10);
                Assert(x > 0);
                x--;
                GuiBuffer[y][x] = ':';
                Assert(x > 0);
                x--;
//��
                data = rtcPointer->Day;   
                if(data == 0) GuiBuffer[y][x] = '0';

                while (data) 
                {
    				remainder = data % 10;
    				data =  data / 10;
    				if (data ||remainder)
    					GuiBuffer[y][x] = HexToAscii(remainder);

                    if(x) x--;
                }
                break;
        }
        labelPointer = labelPointer->NextLabelPointer;
        x = labelPointer->X;
        y = labelPointer->Y;
    }
}


/*******************************************************************************
* ������	: ParseTextBox
* ����	    : TextBox�ؼ�ɨ��ˢ�£�TextBox���������ı���ʾ�����ܰ��������޸Ĳ���
* �������  : formPointer ��ǰ����ָ��
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
*******************************************************************************/
static void ParseTextBox(Form * formPointer)
{
    byte i, x, y,remainder;
    uint data;
    RtcStruct * rtcPointer;
    bool plusFlag;
    string string;
    TextBox * textBoxPointer;

    
    textBoxPointer = formPointer->TextBoxPointer;
    x = textBoxPointer->X;
    y = textBoxPointer->Y;

    while(textBoxPointer != (TextBox *)0)
    {
        if(formPointer->FocusTextBoxDataPointer == textBoxPointer->DataPointer)
        {
            if((TextBoxFocusFlash++ & 0x04) != false)
            {
                textBoxPointer = textBoxPointer->NextTextBoxPointer;
                x = textBoxPointer->X;
                y = textBoxPointer->Y;
                continue;
            }
        }
        switch(textBoxPointer->Type)
        {
            case GuiDataTypeIntDec:
                data = *((uint *)(textBoxPointer->DataPointer));
                
                if(GetBit(data, 31) == 1)
                    plusFlag = false;
                else
                    plusFlag = true;

            case GuiDataTypeUintDec:
                Assert(textBoxPointer->DataPointer != null);
                data = *((uint *)(textBoxPointer->DataPointer));
                
                if(plusFlag == false) data = (~data) + 1;
                
                data = data + textBoxPointer->Offset;
                
    			for (i = 0; i < textBoxPointer->Shift; i++) 
                   data = data / 10;
    				
                if(data == 0) GuiBuffer[y][x] = '0';

    			while (data) 
                {
    				remainder = data % 10;
    				data =  data / 10;
    				if (data ||remainder)
    					GuiBuffer[y][x] = HexToAscii(remainder);

                    if(x) x--;
                }
                
                if(plusFlag == false) GuiBuffer[y][x] = '-';

                plusFlag = true;
                break;

            case GuiDataTypeHex:
                Assert(textBoxPointer->DataPointer != null);
                data = *((uint *)(textBoxPointer->DataPointer));
                data = data >> (textBoxPointer->Shift);
                
                i = textBoxPointer->Flexible;
                if (i)
                {
                    while(i--)
                    {
                        GuiBuffer[y][x] = HexToAscii(data & 0x0000000F);
                        data = data >> 4;
                        if(x) x--;
                    }
                }
                else
                {
                    if (data == 0) GuiBuffer[y][x] = '0';
                    
                    while(data)
                    {
                        GuiBuffer[y][x] = HexToAscii(data & 0x0000000F);
                        data = data >> 4;
                        if(x) x--;
                    }
                }
                break;

            case GuiDataTypeString:
                Assert(textBoxPointer->DataPointer != null);
                data = *((uint *)(textBoxPointer->DataPointer));
                string = textBoxPointer->StringBlockPointer[data];
                i = strlen(string);
                if(textBoxPointer->Align == GuiDataAlignRight)
                {
                    Assert(x > i);
                    x = x - i + 1;
                }
                while(i--)
                {
                    GuiBuffer[y][x] = *string++;
                    if(x < 15) x++;
                }
                break;
            case GuiDataTypeDate:
                Assert(textBoxPointer->DataPointer != null);
                data = *((uint *)(textBoxPointer->DataPointer));
//��
                i = UintToByte0(data);
                Assert(i < 32);
                GuiBuffer[y][x] = HexToAscii(i % 10);
                Assert(x > 0);
                x--;
                GuiBuffer[y][x] = HexToAscii(i / 10);
                Assert(x > 0);
                x--;
                GuiBuffer[y][x] = '-';
                Assert(x > 0);
                x--;
//��
                i = UintToByte1(data);
                Assert(i < 13);
                GuiBuffer[y][x] = HexToAscii(i % 10);
                Assert(x > 0);
                x--;
                GuiBuffer[y][x] = HexToAscii(i / 10);
                Assert(x > 0);
                x--;
                GuiBuffer[y][x] = '-';
                Assert(x > 0);
                x--;
//��
                data = data >> 16;
                Assert(data < 2100);
                
                if(data == 0) GuiBuffer[y][x] = '0';

    			while (data) 
                {
    				remainder = data % 10;
    				data =  data / 10;
    				if (data ||remainder)
    					GuiBuffer[y][x] = HexToAscii(remainder);
                    
                    if(x) x--;
                }
                break;
            case GuiDataTypeTime:
                Assert(textBoxPointer->DataPointer != null);
                rtcPointer = (RtcStruct *)(textBoxPointer->DataPointer);
//��
                i = rtcPointer->Second;
                Assert(i < 60);
                GuiBuffer[y][x] = HexToAscii(i % 10);
                Assert(x > 0);
                x--;
                GuiBuffer[y][x] = HexToAscii(i / 10);
                Assert(x > 0);
                x--;
                GuiBuffer[y][x] = ':';
                Assert(x > 0);
                x--;
//��
                i = rtcPointer->Minute;
                Assert(i < 60);
                GuiBuffer[y][x] = HexToAscii(i % 10);
                Assert(x > 0);
                x--;
                GuiBuffer[y][x] = HexToAscii(i / 10);
                Assert(x > 0);
                x--;
                GuiBuffer[y][x] = ':';
                Assert(x > 0);
                x--;
//ʱ
                i = rtcPointer->Hour;
                Assert(i < 24);
                GuiBuffer[y][x] = HexToAscii(i % 10);
                Assert(x > 0);
                x--;
                GuiBuffer[y][x] = HexToAscii(i / 10);
                Assert(x > 0);
                x--;
                GuiBuffer[y][x] = ':';
                Assert(x > 0);
                x--;
//��
                data = rtcPointer->Day;   

                if(data == 0) GuiBuffer[y][x] = '0';

                while (data) 
                {
    				remainder = data % 10;
    				data =  data / 10;
    				if (data ||remainder)
    					GuiBuffer[y][x] = HexToAscii(remainder);

                    if(x) x--;
                }
                break;
        }
        textBoxPointer = textBoxPointer->NextTextBoxPointer;
        x = textBoxPointer->X;
        y = textBoxPointer->Y;
    }
}


/*******************************************************************************
* ������	: Update
* ����	    : LCD�����ݸ��£�������ʾ�����б仯�����и���LCD������
* �������  : ��
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
*******************************************************************************/
static void Update(void)
{
    byte i, j;
    bool update,updateAll;
    byte * bufferPointer;

    updateAll = false;
    for (i = 0; i < 4; i++)
	{
        update = false;
        
        for (j = 0; j < 16; j++)
        {
            if(DisplayBuffer[i][j] != GuiBuffer[i][j])
            {
                DisplayBuffer[i][j] = GuiBuffer[i][j];
                update = true;
                updateAll = true;
            }
        }
        if (update)
        {
            System.Device.Lcd.DisplayString(i, DisplayBuffer[i]);
        }
    }
    if (updateAll == true)
    {
        System.Device.Usart1.WriteByte(0x02);
        for (i = 0; i < 64; i+=16)
        {
            bufferPointer = (byte *)DisplayBuffer + i;
            System.Device.Usart1.Write(bufferPointer, 16);
            System.Device.Usart1.WriteByte(0x0D);
            System.Device.Usart1.WriteByte(0x0A);
        }
        System.Device.Usart1.WriteByte(0x03);
    }
}


/*******************************************************************************
* ������	: AddLabel
* ����	    : ����Form����Label������ʱ���Label���ݣ����غ���Ҫ��Label��ʼ������
* �������  : formPointer :����Formָ��
*           : labelPointer: �ؼ�Labelָ��
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
*******************************************************************************/
static void AddLabel(Form * formPointer, Label *labelPointer)
{
    labelPointer->DataPointer = null;
    labelPointer->Shift = 0;
    labelPointer->Offset = 0;
    labelPointer->Flexible = 0;
    labelPointer->StringBlockPointer = (const string *)0;
    
    labelPointer->NextLabelPointer = formPointer->LabelPointer;
    formPointer->LabelPointer = labelPointer;
}


/*******************************************************************************
* ������	: AddTextBox
* ����	    : ����Form����TextBox������ʱ���TextBox���ݣ����غ���Ҫ��TextBox��ʼ������
* �������  : formPointer :����Formָ��
*           : textBoxPointer: �ؼ�TextBoxָ��
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
*******************************************************************************/
static void AddTextBox(Form * formPointer, TextBox *textBoxPointer)
{
    textBoxPointer->DataPointer = null;
    textBoxPointer->DataMax = 0;
    textBoxPointer->DataMin = 0;
    textBoxPointer->DataStep = 0;
    textBoxPointer->DataBigStep = 0;
    
    textBoxPointer->Shift = 0;
    textBoxPointer->Offset = 0;
    textBoxPointer->Flexible = 0;
    textBoxPointer->StringBlockPointer = (const string *)0;
    
    textBoxPointer->NextTextBoxPointer = formPointer->TextBoxPointer;
    formPointer->TextBoxPointer = textBoxPointer;
}

static void InitializeForm(Form *formPointer)
{
    formPointer->BackTextPointer = (const byte *)0;
    formPointer->ChartPointer = (Chart *)0;
    formPointer->LabelPointer = (Label *)0;
    formPointer->TextBoxPointer = (TextBox *)0;
    formPointer->FocusTextBoxDataPointer = (void *)0;

	//formPointer->form_num = 0;
}

/*******************************************************************************
* ������	: SwitchTextBoxFocus
* ����	    : �ı��ؼ������л���������л��ı��ؼ�����
* �������  : ��
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
*******************************************************************************/
static void SwitchTextBoxFocus(void)
{
	//loveywm 20140910
	if(FocusFormPointer->form_num == SetupForm)
	{
		//App.Menu.FocusFormPointer = &App.Menu.LogoForm;
		//FocusFormPointer
	}


/*	
    void * lastDataPointer;
    void * endDataPointer;
    TextBox * textBoxPointer;

    textBoxPointer = FocusFormPointer->TextBoxPointer;
//û��TextBox����
    if (textBoxPointer == (TextBox *)0)
    {
        return;
    }
//û�н��㣬�趨���㣬��Ϊ���ݲ����ǵ���˳������Ҫȡ�����һ�� 
    if (FocusFormPointer->FocusTextBoxDataPointer == null)
    {
        while(textBoxPointer != (TextBox *)0)
        {
            lastDataPointer = textBoxPointer->DataPointer;
            textBoxPointer = textBoxPointer->NextTextBoxPointer;            
        }
        FocusFormPointer->FocusTextBoxDataPointer = lastDataPointer;
        return;
    }
//���ڽ���
    else
    {
    //���������Ҫ����
        endDataPointer = textBoxPointer->DataPointer;
        if(FocusFormPointer->FocusTextBoxDataPointer == endDataPointer)
        {
            FocusFormPointer->FocusTextBoxDataPointer = null;
            return;
        }
    //ѭ������
        while(textBoxPointer != (TextBox *)0)
        {
            if(FocusFormPointer->FocusTextBoxDataPointer != textBoxPointer->DataPointer)
            {
                lastDataPointer = textBoxPointer->DataPointer;
            }
            else
            {
                FocusFormPointer->FocusTextBoxDataPointer = lastDataPointer;
                return;
            }
            textBoxPointer = textBoxPointer->NextTextBoxPointer;            
        }
    }
    */
}


/*******************************************************************************
* ������	: SetTextBoxData
* ����	    : �ı��ؼ���ֵ�޸Ĵ����޸ĳ���
* �������  : key: AddKey��AddLongKey��SubKey��SubLongKey
*           :      AssistUpKey��AssistUpLongKey��AssistDownKey��AssistDownLongKey
* ���ز���  : 
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
*******************************************************************************/
static void SetTextBoxData(KeyEnum key)
{
    TextBox * textBoxPointer;
    void * dataPointer;
    
    int intData;
    int intDataMax;
    int intDataMin;
    int intDataStep;
    int intDataBigStep;
    
    uint uintData;
    uint uintDataMax;
    uint uintDataMin;
    uint uintDataStep;
    uint uintDataBigStep;

    RtcStruct rtc;
    
    DataType dataType;

    textBoxPointer = FocusFormPointer->TextBoxPointer;
    dataPointer = FocusFormPointer->FocusTextBoxDataPointer;

    if (dataPointer == null)
    {
        return;
    }

    while(textBoxPointer != (TextBox *)0)
    {
        if(textBoxPointer->DataPointer == dataPointer)
        {
            break;
        }
        textBoxPointer = textBoxPointer->NextTextBoxPointer;   
    }
    
    Assert(textBoxPointer != (TextBox *)0);

    dataType = textBoxPointer->Type;

    switch(dataType)
    {
        case GuiDataTypeIntDec:
            intData = * (int *)(textBoxPointer->DataPointer);
            intDataMax = (int)(textBoxPointer->DataMax);
            intDataMin = (int)(textBoxPointer->DataMin);
            intDataStep = (int)(textBoxPointer->DataStep);        
            intDataBigStep = (int)(textBoxPointer->DataBigStep);
            
            switch(key)
            {
                case AddKey:
                    if(intData + intDataStep > intDataMax)
                        intData = intDataMax;
                    else
                        intData += intDataStep;
                    break;
                case AddLongKey:
                    if(intData + intDataBigStep > intDataMax)
                        intData = intDataMax;
                    else
                        intData += intDataBigStep;
                    break;
                case SubKey:
                    if(intData - intDataStep < intDataMin)
                        intData = intDataMin;
                    else
                        intData -= intDataStep;
                    break;
                case SubLongKey:
                    if(intData - intDataBigStep < intDataMin)
                        intData = intDataMin;
                    else
                        intData -= intDataBigStep;
                    break;
            }
            *(int *)dataPointer = intData;
            break;
        case GuiDataTypeUintDec:
        case GuiDataTypeHex:
        case GuiDataTypeString:
            uintData = * (uint *)(textBoxPointer->DataPointer);
            uintDataMax = (uint)(textBoxPointer->DataMax);
            uintDataMin = (uint)(textBoxPointer->DataMin);
            uintDataStep = (uint)(textBoxPointer->DataStep);        
            uintDataBigStep = (uint)(textBoxPointer->DataBigStep);
            
            switch(key)
            {
                case AddKey:
                    if((uintData + uintDataStep > uintDataMax) ||(uintData + uintDataStep < uintData)||(uintData + uintDataStep < uintDataStep))
                        uintData = uintDataMax;
                    else
                        uintData += uintDataStep;
                    
                    break;
                case AddLongKey:
                    if((uintData + uintDataBigStep > uintDataMax) ||(uintData + uintDataBigStep < uintData)||(uintData + uintDataBigStep < uintDataBigStep))
                        uintData = uintDataMax;
                    else
                        uintData += uintDataBigStep;
                    break;
                case SubKey:
                    if((uintData - uintDataStep < uintDataMin) || (uintData < uintDataStep))
                        uintData = uintDataMin;
                    else
                        uintData -= uintDataStep;
                    break;
                case SubLongKey:
                    if((uintData - uintDataBigStep < uintDataMin) || (uintData < uintDataBigStep))
                        uintData = uintDataMin;
                    else
                        uintData -= uintDataBigStep;
                    break;
            }  
            *(uint *)dataPointer = uintData;
            break;
        case GuiDataTypeDate:
            uintData = *(uint *)(textBoxPointer->DataPointer);
            switch(key)
            {
                case AddKey:
                    UintToByte0(uintData)++;
                    break;
                case AddLongKey:
                    UintToByte1(uintData)++;
                    break;
                case AssistUpKey:
                    UintToUshort1(uintData)++;
                    break;
                case SubKey:
                    UintToByte0(uintData)--;
                    break;
                case SubLongKey:
                    UintToByte1(uintData)--;
                    break;
                case AssistDownKey:
                    UintToUshort1(uintData)--;
                    break;                    
            }
            *(uint *)dataPointer = uintData;
            break;
        case GuiDataTypeTime:
            rtc = *(RtcStruct *)(textBoxPointer->DataPointer);
            switch(key)
            {
                case AddKey:
                    if(rtc.Second < 59)
                        rtc.Second++;
                    break;
                case AddLongKey:
                    if(rtc.Minute < 59)
                        rtc.Minute++;
                    break;
                case AssistUpKey:
                    if(rtc.Hour < 23)
                        rtc.Hour++;
                    break;
                case AssistUpLongKey:
                        rtc.Day++;
                    break;
                case SubKey:
                    if(rtc.Second > 0)
                        rtc.Second--;
                    break;
                case SubLongKey:
                    if(rtc.Minute > 0)
                        rtc.Minute--;
                    break;
                case AssistDownKey:
                    if(rtc.Hour > 0)
                        rtc.Hour--;
                    break; 
                case AssistDownLongKey:
                    if(rtc.Day > 0)
                        rtc.Day--;
                    break;
            }
            *(RtcStruct *)dataPointer = rtc;
            break;
        default:
            break;
    }

}

void Adddraw(Form * formPointer)
{
	
	//uiLcdSmallString("whatbook",0,0,0);


	//uiLcdSmallString("whatbook",1,0,0);
	//uiLcdSmallString("whatbook",2,0,0);
	//uiLcdSmallString("whatbook",3,0,0);

	//uiLcd_1212_ch(0,4,6,4);
	u8 form_num;
	form_num = formPointer->form_num;

	//if(for){}

	if(form_num == LogoRorm)
	{
		uiLcdClear();
		//uiLcdSmallString("whatbook",0,0,0);
		uiLcd_1212_ch(UISTR_CURRENT_LOU_CHENG,0,4,4);//��ʾ����ǰ¥�㡱
		uiLcd_1212_ch(UISTR_AIM_LOU_CHENG,0,76,4);//��ʾ��Ŀ��¥�㡱
		uiLcdMediumString("00",1,3,0);//��ʾ����ǰ¥�㡱����
		uiLcdMediumString("11",1,11,1);//��ʾ��Ŀ��¥�㡱����
		
		//uiLcd_1212_ch(UISTR_WEIGHT,4,4,3);//��ʾ�����أ���
		uiLcd_1616_ch(0,4,0,3);//��ʾ�����أ���
		uiLcdMediumString("0.00",2,5,0);//�����ֵ�����ʵʱ����
		uiLcdMediumString("T",2,9,0);
		uiLcdMediumString("100",2,11,0);//�����ֵ����ذٷֱ�
		uiLcdMediumString("%",2,14,0);
		
		//������ʾ�豸״̬
		uiLcd_1212_ch(UISTR_DEVICE_IS_OK,6,44,4);
			
	}
	else if(form_num == CheckForm)
	{
		//uiLcdSmallString("whatbook",1,0,0);
	}
	else if(form_num == WorkForm)
	{
		uiLcdClear();
		//uiLcdSmallString("whatbook",1,0,0);
		//uiLcdSmallString("whatbook",2,0,0);
		//uiLcdSmallString("whatbook",3,0,0);
		uiLcd_1212_ch(UISTR_GET_PASSWD,1,4,8);
		uiLcdLineErase8x16(2, 5, 11, 1);
	}
	else if(form_num ==SetupForm)
	{
		//uiLcdSmallString("whatbook",3,0,0);
		
		uiLcdClear();
		while(1)
		{
			
			
			uiLcd_1212_ch(UISTR_GET_PASSWD,1,4,8);
			uiLcdLineErase8x16(2, 5, 11, 1);


			


			//if(System.Gui.Globle_Key== UIKEY_ESC)
			if(System.Device.HB_Key.uiKeyGetKey() == UIKEY_ESC)	
			{
				//break;
				//App.Menu.FocusFormPointer = &App.Menu.LogoForm;
				return;
			}
		}
		

		uiLcdClear();
		
		//uiLcdSmallString
		uiLcdSmallString("1:",0,0,0);
		uiLcd_1212_ch(UISTR_PINGCHENG_SET, 0, 15, 4);

		uiLcdSmallString("2:",2,0,0);
		uiLcd_1212_ch(UISTR_ZAIZHONG_SET, 2, 15, 4);

		uiLcdSmallString("3:",4,0,0);
		uiLcd_1212_ch(UISTR_MIMA_SET, 4, 15, 4);

		uiLcdSmallString("4:",6,0,0);
		uiLcd_1212_ch(UISTR_ZHAUNTAIZIJIAN_SET, 6, 15, 4);

		
	}
	else if(form_num ==ServiceForm)
	{
		//uiLcdSmallString("whatbook",4,0,0);
	}

}

/*******************************************************************************
* ������	: Parse
* ����	    : ����ҳ��
* �������  : ��
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/11       ��ʼ�汾
*******************************************************************************/
static void Parse(Form * formPointer)
{           
    	FocusFormPointer = formPointer;
    	//loveywm 20140906
	Adddraw(formPointer);
	
    //ParseBackText(formPointer);         //�������ֿؼ�ɨ�����

    //ParseChart(formPointer);            //��ؼ�ɨ�����

    //ParseLabel(formPointer);            //��ǩ�ؼ�ɨ�����

    //ParseTextBox(formPointer);          //�ı��ؼ�ɨ�����

    //Update();                           //����LCD��ʾ����
}

void InitializeGui(void)
{
    byte i;
    for (i = 0; i < 64; i++)
    {
        ((byte *)GuiBuffer)[i] = ' ';
        ((byte *)DisplayBuffer)[i] = ' ';
    }
    
    System.Gui.Parse = Parse;
    
    System.Gui.Form.AddLabel = AddLabel;

    System.Gui.Form.AddTextBox = AddTextBox;

    System.Gui.Form.Initialize = InitializeForm;    

    System.Gui.Form.SwitchTextBoxFocus = SwitchTextBoxFocus;

    System.Gui.Form.SetTextBoxData = SetTextBoxData;

    //loveywm 20140906
	System.Gui.Form.drawxxx= Adddraw;
	System.Gui.Globle_Key = 0;
}




