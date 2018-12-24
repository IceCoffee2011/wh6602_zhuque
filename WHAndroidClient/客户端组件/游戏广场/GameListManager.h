#pragma once
 
#include "GameInfo.h"
#include <afxtempl.h>
#include "SystemOption.h"

class CGameListManager
{
public:
	CGameListManager(void);
	~CGameListManager(void);
public:  
	CArray<CGameInfo,CGameInfo>		m_arrayGameInfoCollection;
	CArray<CGameRoomInfo,CGameRoomInfo>		m_arrayGameRoomInfo;
	CString							m_strHtml;
public:
	//��ʼ������ȡ���е���Ϸ��Ϣ
	bool Init();
	//��Ϸ�б�
	CString GetGameListHtml();
	//�����б�
	CString GetGameRoomHtml();
	//���÷���
	void SetHtmlRoom();
	//���ô���
	void SetHtmlPlazz();
};
