#ifndef		_VOICE_H_
#define		_VOICE_H_

//����ģ�鲥�Ŵ����Ӧ��ö�ٺ�
enum{

	MASTER_UP_FLAG = 0x00,//����������ע�ⰲȫ
	MASTER_DOWM_FLAG,//�������½�ע�ⰲȫ
	DIAOLONGDAOZHAN,//������վ
	QINGTINGWENHOUZAIKAIMEN,//��ͣ�Ⱥ��ٿ���///�����ʱû��
	JINGLIAOMENWEIGUAN,//������δ��
	CHULIAOMENWEIGUAN=0x05,//������δ��
	TIANCHUANWEIGUAN,//�촰δ��

	SHENGJIANGJICAOZAI = 0x08,//����������

	ONE_FLAG= 0x17,//1//1
	TWO_FLAG,
	THREE_FLAG,
	FOUR_FLAG,
	FIVE_FLAG,
	SIX_FLAG,
	SEVEN_FLAG,
	EGHIT_FLAG,
	NINE_FLAG,
	TEN_FLAG,
	
	DINGDONG = 0x21,//����
	CENG_FALG,//��

	FU_FLAG = 0x25,//��
	DI_FALG,//��

	LINGSHENGYI=0x27,//����һ
	LINGSHENGER=0x28,//������
	LINGSHENGSAN=0x29,//������

	PINGCENGSHANGXIANBAOHU = 0xa,
	PINGCENGXIAXIANBAOHU,
	XITONGGUZHANG,//ϵͳ����
	PINGCENGGUZHANG,//ƽ�����

	HUANYINGSHIYONG = 0x32,//��ӭʹ��
	SHIGONGSHENGJIANGJI,//ʩ��������

	
	QINGSHIYONGZHULINGKAIGUAN=0x41,//��ʹ�������
	


};

void Buzzer_Beep(u8 nBeepTimes, u16 nMiliSecond);


void WTV_Voice(u8 addr);
								
#endif		//_VOICE_H_
