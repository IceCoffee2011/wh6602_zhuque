#include "StdAfx.h"
#include ".\gameinfo.h"

CGameInfo::CGameInfo(void)
{
}

CGameInfo::~CGameInfo(void)
{
}

bool CGameInfo::GetFromFile( CString path )
{
	m_strPath=path;
	int nFound = m_strPath.Find("Games");
	if (nFound>-1)
	{
		m_strName=m_strPath.Mid(nFound+6);
		m_strName.Replace(".dll","");
		 
	}
	typedef WORD GETKINDID();
	GETKINDID * GetKindID;
	HINSTANCE hm;
	if (hm =::LoadLibrary(m_strPath)) 
	{
		GetKindID =(GETKINDID *)::GetProcAddress(hm,"GetKindID");              
		if (GetKindID)
		{
			m_wKindID= GetKindID();
			 
		}else
		{
			AfxMessageBox("��������");
		}
		::FreeLibrary(hm);
	}else
	{
		AfxMessageBox("Dll����ʧ��");
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
CGameRoomInfo::CGameRoomInfo(void)
{
	m_pListServer=NULL;
}

CGameRoomInfo::~CGameRoomInfo(void)
{
}