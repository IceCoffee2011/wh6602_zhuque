#pragma once
#include "Stdafx.h"

class CInitData
{
public:
	CInitData(void);
	~CInitData(void);

public:
	static void InitData();									//��ʼ������
	static void FreeData();									//�ͷ���Դ

	

public:
	static char				s_cLoginServer[128];						//��¼��������ַ
	static char				s_cLoadServer[128];							//��¼��������ַ
	static char				s_cHomePage[128];							//�ٷ���վ
	static char				s_cGameHelp[128];							//��Ϸ����
	static char				s_cClientCenter[128];						//�ͷ�����
	static char				s_cGetPassword[128];						//�һ�����
	static char				s_cErrorTickling[128];						//���ⷴ��
	static char				s_cAccountsManage[128];						//�˺Ź���
	static char				s_cLogonAdverTisement[128];					//��¼���
	static char				s_cLogonNotice[128];						//��¼����
	static char				s_cContactService[256];						//���߿ͷ�
	static char				s_cConfirmGetGold[128];						//��ȡ���
	static char				s_cLogonRegister[128];						//ע���ʺ�
	static char				s_cLoginHomePage[128];						//�ٷ���վ(��¼)
	static char				s_cLoginAccountsManage[128];				//�˺Ź���(��¼)
	static char				s_cLoginPayment[128];						//�ʺų�ֵ
	static char				s_cLoginExchange[128];						//��Ʒ�һ�
	static char				s_cLoginPropShop[128];						//�����̳�
	static char				s_cLoginPropShopOpen[128];					//�����̳�(����ʽ)
	static char				s_cLoginTaskNew[128];						//��������
	static char				s_cLoginUserInfo[128];						//�����Ϣ
	static char				s_cLoginSysMessage[128];					//ϵͳ�ʼ�
	static char				s_cLoginBank[128];							//����
	static char				s_cLoginBackpack[128];						//����
	static char				s_cGameHelpSingle[128];						//������Ϸ����
	static char				s_cXBMessage[128];							//ϲ����Ϣ(��Ϸ�б�������ҳ)
	static char				s_cNotice[128];								//�����
	static char				s_cAdverTisement[128];						//�ײ����
	static char				s_cTitleNotice[128];						//ͷ�����
};
