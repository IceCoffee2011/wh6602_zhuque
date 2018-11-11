#pragma once

#include "ServerItemView.h"

class CGameInfo
{
public:
	CGameInfo(void);
	~CGameInfo(void);
	bool GetFromFile(CString path);

	//����
public:
	CString					m_strPath;//�ļ�·��
	CString					m_strName;
	WORD					m_wKindID;//��ϷID
	//int						m_nSort;
	
public:
	

};

//������Ϣ
class CGameRoomInfo
{
public:
	CListServer * m_pListServer;

public:
	CGameRoomInfo();
	~CGameRoomInfo();

	WORD GetKindID(){return m_pListServer->m_pListKind->m_GameKind.wKindID;};
	WORD GetSortId(){return m_pListServer->m_GameServer.wSortID;};
};
