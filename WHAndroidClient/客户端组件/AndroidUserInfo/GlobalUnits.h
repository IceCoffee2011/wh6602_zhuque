#ifndef GLOBAL_UNITS_HEAD_FILE
#define GLOBAL_UNITS_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AndroidUserInfoEx.h"
#include "UserGameDataDef.h"



//////////////////////////////////////////////////////////////////////////


//ȫ����Ϣ��
 class ANDROID_USER_INFO_EX CGlobalUnits
{
	//ϵͳ����
protected:
	DWORD								m_dwPlazaVersion;				//�����汾
	TCHAR								m_szDirWork[MAX_PATH];			//����Ŀ¼

public:
	bool								m_bInitGameKindList;			//�Ѿ���ʼ��
	GameOption							m_GameOption;					//��Ϸ����
	std::vector<GameServerList>			m_GameKindList;					//��Ϸ�б�

	//��������
public:
	//���캯��
	CGlobalUnits(void);
	//��������
	virtual ~CGlobalUnits(void);

	//��ʼ������
	bool InitGlobalUnits();

	//���ܺ���
public:
	//��ȡ����
	void GetClientSerial(tagClientSerial & ClientSerial);

	//��ѯ����
public:
	//�����汾
	DWORD GetPlazaVersion() { return m_dwPlazaVersion; }
	//����Ŀ¼
	LPCTSTR GetWorkDirectory() { return m_szDirWork; }
	//��õ�ǰ�ɽ���ķ�����Ϣ
	bool GetCurCanEnterRoomInfo(DWORD& dwServerAddr, DWORD& dwServerPort);

	//�ڲ�����
private:
	//������ַ
	WORD GetMACAddress(BYTE cbMacBuffer[], WORD wBufferSize);
};

//ȫ����Ϣ 
ANDROID_USER_INFO_EX extern CGlobalUnits	g_GlobalUnits;					//��Ϣ���	
//////////////////////////////////////////////////////////////////////////

#endif