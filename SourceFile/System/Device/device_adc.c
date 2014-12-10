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
*�ļ���     : device_adc.c
*����       : ADC�豸
*ԭ��       : Adc�豸���ܿ飬����SystemTick����ʵ��ʵʱ���ݲ���
*           : ����ע�ắ���������������ݸ��ϲ�Ӧ�ñ���                  
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/01       ��ʼ�汾
********************************************************************************
*/

#include "drive.h"
#include "system.h"



static ushort NullValue;

static ushort * RegisterPointerBlock[4] =
{
    &NullValue, &NullValue, &NullValue, &NullValue
};


/*******************************************************************************
* ������	: Register
* ����	    : ��Ӧ�ò����ָ��ע�ᵽ��Ӧ��ͨ�������У�ʵ�ֵײ������򶥲㴫��
* �������  : adcChannel: ADCͨ����
*           : dataPoint: Ӧ�ò����ָ��
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/08       ��ʼ�汾
*******************************************************************************/
static void Register(AdcChannelEnum adcChannel, ushort * dataPointer)
{
    RegisterPointerBlock[adcChannel] = dataPointer;
}


/*******************************************************************************
* ������	: AdcSystick10000Routine
* ����	    : ϵͳʱ��10000/S����0.1mS�������һ�����У���ȡADCֵ��App�����
* �������  : ��
* ���ز���  : ��
********************************************************************************
*�汾     ����            ����            ˵��
*V0.1    Wangsw        2013/09/10       ��ʼ�汾
*******************************************************************************/
void AdcSystick10000Routine(void)
{
/*
	static byte Counter = 0;
    
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    *RegisterPointerBlock[Counter] = ADC_GetConversionValue(ADC1);

    if (++Counter > 3) Counter = 0;
    
    ADC_RegularChannelConfig(ADC1,Counter + 10, 1 , ADC_SampleTime_239Cycles5);

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
*/

	ADC_RegularChannelConfig(ADC1, 16, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��3,��һ��ת��,����ʱ��Ϊ239.5����	  			    
 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
	*RegisterPointerBlock[0] = ADC_GetConversionValue(ADC1);
	
	//return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}

void InitializeAdc(void)
{

	ADC_InitTypeDef ADC_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��GPIOA,ADC1ͨ��ʱ��
  
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //��Ƶ����6ʱ��Ϊ72M/6=12MHz

   	ADC_DeInit(ADC1);  //������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���

	ADC_TempSensorVrefintCmd(ENABLE); //�����ڲ��¶ȴ�����
	
 
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1

	ADC_ResetCalibration(ADC1);	//����ָ����ADC1�ĸ�λ�Ĵ���

    	while(ADC_GetResetCalibrationStatus(ADC1));	//��ȡADC1����У׼�Ĵ�����״̬,����״̬��ȴ�

	ADC_StartCalibration(ADC1);	 //

	while(ADC_GetCalibrationStatus(ADC1));		//��ȡָ��ADC1��У׼����,����״̬��ȴ�


	System.Device.Adc.Register = Register;
/*
	GPIO_InitTypeDef GPIO_InitStructure;
    	ADC_InitTypeDef ADC_InitStructure;
    
    	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1, ENABLE);
    	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    	GPIO_Init(GPIOC, &GPIO_InitStructure);

    	ADC_DeInit(ADC1);

    	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    	ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);

    while(ADC_GetResetCalibrationStatus(ADC1));

    ADC_StartCalibration(ADC1);

    while(ADC_GetCalibrationStatus(ADC1));

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    System.Device.Adc.Register = Register;

    */
}

