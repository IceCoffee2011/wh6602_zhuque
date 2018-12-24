#ifndef GLOBAL_SERVICE_HEAD_FILE
#define GLOBAL_SERVICE_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////

//���ݿ�����
const TCHAR szGameUserDB[]=TEXT("QPGameUserDB");						//�û����ݿ�
const TCHAR	szTreasureDB[]=TEXT("QPTreasureDB");						//�Ƹ����ݿ�
const TCHAR szServerInfoDB[]=TEXT("QPServerInfoDB");					//�������ݿ�

//////////////////////////////////////////////////////////////////////////

//���ݿ���Ϣ
struct tagDataBaseInfo
{
	WORD							wDataBasePort;						//���ݿ�˿�
	DWORD							dwDataBaseAddr;						//���ݿ��ַ
	TCHAR							szDataBaseUser[32];					//���ݿ��û�
	TCHAR							szDataBasePass[32];					//���ݿ�����
	TCHAR							szDataBaseName[32];					//���ݿ�����
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ��������
struct tagGameServiceAttrib
{
	WORD							wKindID;							//���ƺ���
	WORD							wChairCountMin; //�����޸�    WORD   wChairCountMin; //������Ŀ
	WORD                            wChairCountMax; //�������

	BYTE							cbJoinInGame;						//��Ϸ����
	TCHAR							szDataBaseName[32];					//��Ϸ����
	TCHAR							szDescription[128];					//ģ������
	TCHAR							szKindName[KIND_LEN];				//��������
	TCHAR							szServerModuleName[MODULE_LEN];		//ģ������
	TCHAR							szClientModuleName[MODULE_LEN];		//ģ������
};

//��Ϸ��������
struct tagGameServiceOption
{
	//��������
	WORD							wKindID;							//���ͱ�ʶ
	WORD							wSortID;							//�����ʶ
	WORD							wNodeID;							//վ�����
	WORD							wServerID;							//�������
	WORD							wTableCount;						//������Ŀ

	WORD							wServerPort;						//����˿�
	WORD							wServerType;						//��������
	WORD							wMaxConnect;						//�������
	DWORD							dwServerAddrCNN;				    //���ŷ����ַ �������
	DWORD                           dwServerAddrCUN;                    //��ͬ�����ַ �������

	DWORD							dwVideoServer;						//��Ƶ��ַ
	DWORD							dwDataBaseAddr;						//���ݵ�ַ
	TCHAR							szDataBaseName[SERVER_LEN];			//��������
	TCHAR							szGameRoomName[SERVER_LEN];			//��������

	//��������
	WORD							wRevenue;							//��Ϸ˰��
	LONG							lCellScore;							//��λ����
	LONG							lLessScore;							//��ͻ���
	LONG							lMaxScore;							//��߻���
	LONG							lRestrictScore;						//���ƻ���
	LONG                            lRestrictGrade;                     //���Ƶȼ�
	LONG                            lEnterMoney;                        //������Ϸ��ͽ��

	//��������
	LONG							lMatchDraw;							//��������
	BYTE							cbControlStart;						//���ƿ�ʼ

	//��չ����
	BYTE							cbHideUserInfo;						//������Ϣ
	BYTE							cbUnLookOnTag;						//�����Թ�
	BYTE							cbUnSameIPPlay;						//����ͬIP
	
	DWORD                             wAndroidTableCount;                 //���������վλ�������� ������� 
	                                                                      //������������� wAndroidTableCount = 10��ζ��������30�������˻�վλ
	DWORD                             wUserWaitUntilAndroid;              //�û��ȴ�ʱ��
};

//����������
#define	LESS_CONNECT_COUNT			(50L)								//��������
//////////////////////////////////////////////////////////////////////////

#endif