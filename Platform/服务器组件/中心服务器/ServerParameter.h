#ifndef SERVER_PARAMETER_HEAD_FILE
#define SERVER_PARAMETER_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//���ò���
class CServerParameter
{
public:
	//��ַ��Ϣ
	struct tagAddressInfo
	{
		TCHAR							szAddress[32];						//�����ַ
	};

	//������Ϣ
	struct tagDataBaseParameter
	{
		WORD							wDataBasePort;						//���ݿ�˿�
		TCHAR							szDataBaseAddr[32];					//���ݿ��ַ
		TCHAR							szDataBaseUser[32];					//���ݿ��û�
		TCHAR							szDataBasePass[32];					//���ݿ�����
		TCHAR							szDataBaseName[32];					//���ݿ�����
	};

	//ϵͳ����
public:
	WORD							m_wMaxConnect;						//�������
	WORD							m_wServicePort;						//����˿�
	TCHAR							m_szServerName[32];			        //��������

	//���ʱ��
public:
	WORD							m_wConnectTime;						//����ʱ��
	WORD							m_wCollectTime;						//ͳ��ʱ��
	WORD                            m_wLoadVCountTime;					//����ʱ�� 
	WORD							m_wLoadListTime;					//�б�ʱ��
	WORD							m_wReLoadListTime;					//�б�ʱ��

	//�ͻ�ʱ��
public:
	WORD							m_wIntermitTime;					//�ж�ʱ��
	WORD							m_wOnLineCountTime;					//����ʱ��

	DWORD							m_dwMainVer;						//�������汾��   ��һλ
	DWORD							m_dwSubVer;							//�������汾��		�ڶ�λ
	DWORD							m_dwProductVer;						//�������汾��		����λ
	//Э����Ϣ
public:
	WORD							m_wCorrespondPort;					//Э���˿�
	tagAddressInfo					m_CorrespondAddress;				//Э����ַ

	//�����ַ
public:
	tagAddressInfo					m_ServiceAddress;					//�����ַ
	tagDataBaseParameter			m_AccountsDBParameter;				//���ӵ�ַ
	tagDataBaseParameter			m_TreasureDBParameter;				//���ӵ�ַ
	tagDataBaseParameter			m_PlatformDBParameter;				//���ӵ�ַ
	tagDataBaseParameter			m_GameSSCDBParameter;				//���ӵ�ַ

	//��������
public:
	//���캯��
	CServerParameter();
	//��������
	virtual ~CServerParameter();

	//���ܺ���
public:
	//��ʼ��
	VOID ServerParameter();
	//��������
	VOID LoadServerParameter();

	//����Ŀ¼
	static bool GetWorkDirectory(TCHAR szWorkDirectory[], WORD wBufferCount);
	//6603����ת��Ϊ6602����
	static tagDataBaseInfo wh6603_to_2(tagDataBaseParameter * p);
};

//////////////////////////////////////////////////////////////////////////////////

#endif
