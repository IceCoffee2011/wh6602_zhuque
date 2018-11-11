#ifndef GLOBAL_PROPERTY_HEAD_FILE
#define GLOBAL_PROPERTY_HEAD_FILE

////////////////////////////////////////////////////////////////////////////////////////////////

//�ʻ�����
#define FLOWER_1					0
#define FLOWER_2					1
#define FLOWER_3					2
#define FLOWER_4					3
#define FLOWER_5					4
#define FLOWER_6					5
#define FLOWER_7					6
#define FLOWER_8					7
#define FLOWER_9					8
#define FLOWER_10					9
#define FLOWER_11					10

BYTE const g_FlowerTypeList[]=
{
	FLOWER_1,
	FLOWER_2,
	FLOWER_3,
	FLOWER_4,
	FLOWER_5,
	FLOWER_6,
	FLOWER_7,
	FLOWER_8,
	FLOWER_9,
	FLOWER_10,
	FLOWER_11,
};

//�ʻ���Ŀ
#define FLOWER_COUNT		(sizeof(g_FlowerTypeList)/sizeof(g_FlowerTypeList[0]))//������Ŀ

//////////////////////////////////////////////////////////////////////////
//�궨��
#define CHARM_EXCHANGE_RATE			50									//�һ�����
#define MAX_FLOWER_COUNT			100									//�����Ŀ

//�ʻ��ṹ
struct tagGiftInfo
{
	TCHAR							szName[64];							//�ʻ�����
	TCHAR							szAction[128];						//��������
	TCHAR							szQuantifier1[20];					//������
	TCHAR							szResult[128];						//�������
	TCHAR							szQuantifier2[6];					//������
	TCHAR							szCharmResult[128];					//�������
};

//�ʻ�����
tagGiftInfo const g_FlowerDescribe[FLOWER_COUNT]=
{
	{TEXT("����"),TEXT(" ��������� "),TEXT(" ����1��"),TEXT("����ϲ "),TEXT("�� "),TEXT(" ����ֵ���� %ld �㣡")},
	{TEXT("����"),TEXT(" ��������� "),TEXT(" ����1��"),TEXT("����ϲ "),TEXT("�� "),TEXT(" ����ֵ���� %ld �㣡")},
	{TEXT("�ʻ�"),TEXT(" ��������� "),TEXT(" �ʻ�1��"),TEXT("����ϲ "),TEXT("�� "),TEXT(" ����ֵ���� %ld �㣡")},
	{TEXT("ơ��"),TEXT(" ��������� "),TEXT(" ơ��1��"),TEXT("����ϲ "),TEXT("�� "),TEXT(" ����ֵ���� %ld �㣡")},
	{TEXT("����"),TEXT(" ��������� "),TEXT(" ����1��"),TEXT("����ϲ "),TEXT("�� "),TEXT(" ����ֵ���� %ld �㣡")},
	{TEXT("���"),TEXT(" ��������� "),TEXT(" ���1ö"),TEXT("����ϲ "),TEXT("ö "),TEXT(" ����ֵ���� %ld �㣡")},
	{TEXT("�γ�"),TEXT(" ��������� "),TEXT(" �γ�1��"),TEXT("����ϲ "),TEXT("�� "),TEXT(" ����ֵ���� %ld �㣡")},
	{TEXT("����"),TEXT(" ��������� "),TEXT(" ����1��"),TEXT("����ϲ "),TEXT("�� "),TEXT(" ����ֵ���� %ld �㣡")},
	{TEXT("����"),TEXT(" �ݺݵ����� "),TEXT(" ����1��"),TEXT("����Ǹ "),TEXT("�� "),TEXT(" ����ֵ���� %ld �㣡")},
	{TEXT("שͷ"),TEXT(" �ݺݵ����� "),TEXT(" שͷ1��"),TEXT("����Ǹ "),TEXT("�� "),TEXT(" ����ֵ���� %ld �㣡")},
	{TEXT("ը��"),TEXT(" �ݺݵ����� "),TEXT(" ը��1ö"),TEXT("����Ǹ "),TEXT("ö "),TEXT(" ����ֵ���� %ld �㣡")}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//��������
#define PROP_DOUBLE						0									//˫�����ֿ�
#define PROP_FOURDOLD					1									//�ı����ֿ�
#define PROP_NEGAGIVE					2									//��������
#define PROP_FLEE						3									//��������
#define PROP_BUGLE						4									//С����
#define PROP_KICK						5									//���߿�
#define PROP_SHIELD						6									//�����
#define PROP_MEMBER_1					7									//��Ա����
#define PROP_MEMBER_2					8									//��Ա����
#define PROP_MEMBER_3					9									//��Ա����
#define PROP_MEMBER_4					10									//��Ա����

//�������
//������
#define PROP_USER_LOW                   11     //��Ϳ��õ���
#define PROP_FSK                        11     //���� ������
#define PROP_JBK_S                      12     //���� ������(С)
#define PROP_MLSS                       13     //���� ����˫�տ�
#define PROP_JBK_M                      14     //���� ������(��)
#define PROP_JBK_L                      15     //���� ������(��
#define PROP_HSZFK                      16     //���� ����ף����
#define PROP_GOODLUCK                   17     //���� ����Ů��
#define PROP_GAME_RACE                  18     //���� �����ʸ�
#define PROP_USER_HIGH                  19     //��߿��õ���

//������
#define PROP_CRYSTAL_RING              19     //ˮ����ָ
#define PROP_GREEN_GEMS_RING           20     //����ʯ��ָ
#define PROP_CAT_EYE_GEMS_RING         21     //è��ʯ��ָ
#define PROP_BULE_GEMS_RING            22     //����ʯ��ָ
#define PROP_RED_GEMS_RING             23     //�챦ʯ��ָ
#define PROP_EMERALD_RING              24     //��ĸ�̽�ָ
#define PROP_DIAMOND_5C_RING           25     //5������ʯ
#define PROP_DIAMOND_10C_RING          26     //10������ʯ
#define PROP_DIAMOND_20C_RING          27     //20������ʯ
#define PROP_DIAMOND_50C_RING          28     //50������ʯ
#define PROP_DIAMOND_100C_RING         29     //100������ʯ

//��ӽ���

BYTE const g_PropTypeList[]=
{
	PROP_DOUBLE	,
	PROP_FOURDOLD,
	PROP_NEGAGIVE,
	PROP_FLEE	,
	PROP_BUGLE	,
	PROP_KICK	,
	PROP_SHIELD	,
	PROP_MEMBER_1,
	PROP_MEMBER_2,
	PROP_MEMBER_3,
	PROP_MEMBER_4,

	PROP_FSK,                       
	PROP_JBK_S,                   
	PROP_MLSS,                      
	PROP_JBK_M,                    
	PROP_JBK_L,                     
	PROP_HSZFK ,                  
	PROP_GOODLUCK,                
	PROP_GAME_RACE, 

	PROP_CRYSTAL_RING,             
	PROP_GREEN_GEMS_RING,           
	PROP_CAT_EYE_GEMS_RING,        
	PROP_BULE_GEMS_RING,           
	PROP_RED_GEMS_RING,            
	PROP_EMERALD_RING,        
	PROP_DIAMOND_5C_RING,           
	PROP_DIAMOND_10C_RING,          
	PROP_DIAMOND_20C_RING,       
	PROP_DIAMOND_50C_RING,      
	PROP_DIAMOND_100C_RING
};
#define PROPERTY_COUNT			(sizeof(g_PropTypeList)/sizeof(g_PropTypeList[0]))//������Ŀ

//���߶���
#define MAX_PROPERTY_COUNT				100									//�����Ŀ
#define BUGLE_MAX_CHAR					120									//���ȸ���
#define BULESSING_MAX_CHAR				100									//ף������

//��������
struct tagPropertyDescribe
{
	TCHAR							szName[32];							//��������
	TCHAR							szDescribe[255];					//���߼�ֵ
};

tagPropertyDescribe const g_PropertyDescribe[PROPERTY_COUNT] =
{
	{TEXT("˫�����ֿ�"),TEXT("ӵ����˫�����ֿ�������ϷӮ�ַ��������˲���ۣ�������Ч����ι���ʱ���ۼӡ���ע��ֻ���ڹ������Ϸ����ʹ�á���")},
	{TEXT("�ı����ֿ�"),TEXT("ӵ�����ı����ֿ�������ϷӮ�ַ��ı������˲���ۣ�������Ч����ι���ʱ���ۼӡ���ע��ֻ���ڹ������Ϸ����ʹ�á���")},
	{TEXT("��������"),TEXT("ʹ�øõ��ߺ�������Ϸ���ֽ��ָ���ʼ״̬������Ϊ���ֶ����գ�")},
	{TEXT("��������"),TEXT("ʹ�øõ��ߺ����������ʽ��ָ���ʼ״̬������Ϊ���ܶ����գ�")},
	{TEXT("С����"),TEXT("�����͵���Ϣ���ڷ��估�����ڵ�������Ϸ����������Ŀ�ķ�ʽ��ʾ��")},	
	{TEXT("���߿�"),TEXT("����õ��ߺ������ٵ��ı���Ա�߳���Ϸ����")},
	{TEXT("�����"),TEXT("����õ��ߺ�����ֲ��۷֣�����Ϊ���ֶ����գ���ע��ֻ���ڹ������Ϸ����ʹ�á���")},
	{TEXT("�����Ա��"),TEXT("��������/����ʱ������9���Żݣ�������ͨ��ң��к����Ա�����ǣ���Ա�ڼ䣬����ID�ڴ����������к�ɫ������ʾ,���Ƚ����������䣡")},
	{TEXT("�����Ա��"),TEXT("��������/����ʱ������9���Żݣ�������ͨ�������Ա���������Ա�����ǣ���Ա�ڼ䣬����ID�ڴ����������к�ɫ������ʾ�����Ƚ����������䣡")},
	{TEXT("�����Ա��"),TEXT("��������/����ʱ������9���Żݣ�������ͨ�����ꡢ������ң��л�Ա�����ǣ���Ա�ڼ䣬����ID�ڴ����������к�ɫ������ʾ�����Ƚ����������䣡")},
	{TEXT("�����Ա��"),TEXT("��������/����ʱ������9���Żݣ�������ͨ�����ꡢ���ꡢ������ң��л�Ա�����ǣ���Ա�ڼ䣬����ID�ڴ����������к�ɫ������ʾ�����Ƚ����������䣡")},

	{TEXT("������"),TEXT("ʹ�ú���Ի��˫�����飬����2��Сʱ��")},
	{TEXT("������(С)"),TEXT("ʹ�ú�����Ϸ�л�ʤ���Ի��˫����ʯ,���˲���ۣ�����2��Сʱ��")},
	{TEXT("����˫�տ�"),TEXT("ʹ�ú�����Ϸ�п��Ի��˫������,�����ʤ�����Ի��˫����ʯ,���˲���ۣ�����2��Сʱ��")},
	{TEXT("������(��)"),TEXT("ʹ�ú�����Ϸ�л�ʤ���˫����ʯ,���˲���ۣ�����4��Сʱ��")},
	{TEXT("������(��)"),TEXT("ʹ�ú�����Ϸ�л�ʤ���Ի��˫����ʯ,���˲���ۣ�����6��Сʱ��")},
	{TEXT("����ף����"),TEXT("����Ϸ�����˲���Ǯ������2��Сʱ")},
	{TEXT("����Ů��"),TEXT("�������Ϸ�л�ʤ��20%�ĸ��ʻ������Ľ�����������ˮ����ָ+1000��ʯ���߽�����(С)������2��Сʱ��")},
	{TEXT("�����ʸ�"),TEXT("���뾺����")},

	{TEXT("ˮ����ָ"),TEXT("1000��� ���� 1000 ��ʯ")},
	{TEXT("����ʯ��ָ"),TEXT("2500��� ���� 2500 ��ʯ")},
	{TEXT("è��ʯ��ָ"),TEXT("4000��� ���� 4000 ��ʯ")},
	{TEXT("����ʯ��ָ"),TEXT("6500��� ���� 6500 ��ʯ")},

	{TEXT("�챦ʯ��ָ"),TEXT("9000��� ���� 9000 ��ʯ")},
	{TEXT("��ĸ�̽�ָ"),TEXT("12000��� ���� 12000 ��ʯ")},
	{TEXT("��ʯ��ָ5Ct"),TEXT("50000��� ���� 50000 ��ʯ")},
	{TEXT("��ʯ��ָ10Ct"),TEXT("100000��� ���� 100000 ��ʯ")},

	{TEXT("��ʯ��ָ20Ct"),TEXT("200000��� ���� 200000 ��ʯ")},
	{TEXT("��ʯ��ָ50Ct"),TEXT("500000��� ���� 500000 ��ʯ")},
	{TEXT("��ʯ��ָ100Ct"),TEXT("1000000��� ���� 1000000 ��ʯ")},

	//{TEXT("�ٴ������������"),TEXT("150���� ���� 1500009��ʯ")},
	//{TEXT("��ʫ������ʯŮ��"),TEXT("150���� ���� 1500001 ��ʯ")},
	//{TEXT("R8"),TEXT("350���� ���� 1500007 ��ʯ")},
	//{TEXT("��Ӱ"),TEXT("500���� ���� 3500008 ��ʯ")}
};

//������Ϣ�ṹ
struct tagPropertyInfo
{
	int							nPropertyID;						//����ID
	DWORD						dwPropCount1;						//������Ŀ
	DWORD						dwPropCount2;						//������Ŀ
	DWORD						dwPropCount3;						//������Ŀ
	DWORD						dwPropCount4;						//������Ŀ
	DWORD						dwPropCount5;						//������Ŀ
	DWORD						dwPropCount6;						//������Ŀ
	DWORD						dwPropCount7;						//������Ŀ
	DWORD						dwPropCount8;						//������Ŀ
	DWORD						dwPropCount9;						//������Ŀ
	DWORD						dwPropCount10;						//������Ŀ
	LONG						lPrice1;							//���߼۸�
	LONG						lPrice2;							//���߼۸�
	LONG						lPrice3;							//���߼۸�
	LONG						lPrice4;							//���߼۸�
	LONG						lPrice5;							//���߼۸�
	LONG						lPrice6;							//���߼۸�
	LONG						lPrice7;							//���߼۸�
	LONG						lPrice8;							//���߼۸�
	LONG						lPrice9;							//���߼۸�
	LONG						lPrice10;							//���߼۸�
	BYTE						cbDiscount;							//��Ա�ۿ�
	//�������
    DWORD                       dwLastingTime;                      //���߳�����ʱ��
	//��ӽ���
	bool						bNullity;							//��ֹ��ʶ
};

//�ʻ���Ϣ�ṹ
struct tagFlowerInfo
{
	int							nFlowerID;							//�ʻ�ID
	LONG						lPrice;								//�ʻ��۸�
	LONG						lSendUserCharm;						//��������
	LONG						lRcvUserCharm;						//��������
	BYTE						cbDiscount;							//��Ա�ۿ�
	bool						bNullity;							//��ֹ��ʶ
};

//////////////////////////////////////////////////////////////////////////////

#endif