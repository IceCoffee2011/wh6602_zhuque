#include "stdafx.h"
#include "gameuserinfo.h"
#include "GlobalUnits.h"


CGameUserInfo*	CGameUserInfo::m_Instance = NULL;

CGameUserInfo::CGameUserInfo(void)
{
	m_ptrFreeGameClient = NULL;
	m_ptrNewGameClient = NULL;
	m_hGameDllInstance = NULL;
	m_vecUserGameInfo.clear();
	::memset(&m_GameServerInfo, 0, sizeof(m_GameServerInfo));
}

CGameUserInfo::~CGameUserInfo(void)
{
	m_ptrNewGameClient = NULL;
	m_ptrFreeGameClient = NULL;
	if (NULL != m_hGameDllInstance)
	{
		::FreeLibrary(m_hGameDllInstance);
	}
	if (NULL != m_Instance)
	{
		delete m_Instance;
	}
	m_Instance = NULL;
}

CGameUserInfo* CGameUserInfo::GetInstance()
{
	if (NULL == m_Instance)
	{
		m_Instance = new CGameUserInfo();
	}
	return m_Instance;
}

//����DLL
bool CGameUserInfo::LoadGameClientDll(const std::string& strDllName)
{
	if (!strDllName.empty())
	{
		m_ptrFreeGameClient = NULL;
		m_ptrNewGameClient = NULL;
		m_hGameDllInstance = NULL;

		m_hGameDllInstance = ::LoadLibrary(strDllName.c_str());
		if (NULL != m_hGameDllInstance)
		{
			m_ptrNewGameClient = (NewGameClient*)::GetProcAddress(m_hGameDllInstance, TEXT("NewGameClient"));
			if (NULL == m_ptrNewGameClient)
			{
				return false;
			}
			m_ptrFreeGameClient = (FreeGameClient*)::GetProcAddress(m_hGameDllInstance, TEXT("FreeGameClient"));
			if (NULL == m_ptrFreeGameClient)
			{
				return false;
			}
			return true;
		}
	}
	
	return false;
}

//��ʼ����Ϣ
WORD CGameUserInfo::InitGameUserInfo()
{
	m_vecUserGameInfo.clear();
	for (WORD i=0; i<g_GlobalUnits.m_GameOption.dwUserNums; i++)
	{
		strUserGameInfo* info = new strUserGameInfo();
		if (NULL==info)continue;
		::memset(info->chName, 0, sizeof(info->chName));
		::sprintf(info->chName, "%s_%d", g_GlobalUnits.m_GameOption.chUserID, g_GlobalUnits.m_GameOption.dwUserIndex+i+1);
		info->dwSocketID = g_GlobalUnits.m_GameOption.dwSockStartID+i;
		info->dwKindID = g_GlobalUnits.m_GameOption.dwGameID;
		info->dwGameServerIP = 0;
		info->dwGameServerport = 0;
		info->dwCountConnect = 0;
		info->dwLogonLinkState = TAG_LINK_FAIL;
		info->dwGameLinkState = TAG_LINK_FAIL;
		info->dwUserLogonState = USER_LOGON_SERVER_FAIL;
		info->dwUserGameState = USER_LOGON_SERVER_FAIL;
		info->dwUserID = 0;

		//������Ϸ�ͻ���
		bool bIsCreateSuc = false;
		info->pIGameClient = NULL;
		
		CWnd* pMain = AfxGetMainWnd();
		if (NULL != pMain)
		{
			if (NULL != m_ptrNewGameClient)
			{
				info->pIGameClient = m_ptrNewGameClient(pMain, info->dwSocketID);
				if (NULL != info->pIGameClient)
				{
					bIsCreateSuc = true;
				}
			}
		}
		
		if (!bIsCreateSuc)
		{//����ռ�ʧ��
			return i+1;
		}

		//����Socket
		if (!info->TcpSocketHelper.CreateInstance()) 
		{
			return i+1;
		}
		info->TcpSocketHelper->SetSocketID(g_GlobalUnits.m_GameOption.dwSockStartID+i);
		
		//������Ϣ��
		pair<DWORD, strUserGameInfo*> value(info->dwSocketID, info);
		m_vecUserGameInfo.insert(value);
	}

	return 0;
}

//�ͷų�ʼ����Ϣ
bool CGameUserInfo::FreeGameUserInfo()
{
	std::map<DWORD, strUserGameInfo*>::iterator iter = m_vecUserGameInfo.begin();
	for (; iter!=m_vecUserGameInfo.end(); ++iter)
	{
		if (NULL != (*iter).second)
		{
			//�ͷ�socket���.
			(*iter).second->TcpSocketHelper.CloseInstance();
			//�ͷ���Ϸ�ͻ���
			if (NULL != (*iter).second->pIGameClient)
			{
				if (NULL != m_ptrFreeGameClient)
				{
					m_ptrFreeGameClient((*iter).second->pIGameClient);
				}
				(*iter).second->pIGameClient = NULL;
			}
			delete (*iter).second;
		}
		(*iter).second=NULL;
	}
	m_vecUserGameInfo.clear();
	return true;
}


//��������˳���Ϸ
void CGameUserInfo::AllUserExitGame()
{
	std::map<DWORD, strUserGameInfo*>::iterator iter = m_vecUserGameInfo.begin();
	for (; iter!=m_vecUserGameInfo.end(); ++iter)
	{
		if ((NULL != (*iter).second->pIGameClient) && ((*iter).second->pIGameClient)->IsEnableWnd())
		{
			const tagUserData* pUserData = ((*iter).second->pIGameClient)->GetMeData();
			if (NULL != pUserData)
			{
				if (US_PLAY == pUserData->cbUserStatus)
				{//�������Ϸ�У���ǿ�ˡ�
					((*iter).second)->TcpSocketHelper->SendData(MDM_GR_USER,SUB_GR_USER_LEFT_GAME_REQ);
				}
				else
				{//�뿪��Ϸ,����������Ϣ��
					((*iter).second)->TcpSocketHelper->SendData(MDM_GR_USER,SUB_GR_USER_STANDUP_REQ);
				}
			}
		}
	}
}

//�ر�����socket
void CGameUserInfo::CloseAllSocket()
{//�˳���Ϸ������ʹ��.
	std::map<DWORD, strUserGameInfo*>::iterator iter = m_vecUserGameInfo.begin();
	for (; iter!=m_vecUserGameInfo.end(); ++iter)
	{
		((*iter).second)->dwGameLinkState = TAG_LINK_FAIL;
		((*iter).second)->dwUserGameState = USER_LOGON_SERVER_FAIL;
		((*iter).second)->TcpSocketHelper->CloseSocket();
	}
}

//��ղ��˳�������
void CGameUserInfo::ClearGameUserInfo()
{
	std::map<DWORD, strUserGameInfo*>::iterator iter = m_vecUserGameInfo.begin();
	for (; iter!=m_vecUserGameInfo.end(); ++iter)
	{
		((*iter).second)->dwLogonLinkState = TAG_LINK_FAIL;
		((*iter).second)->dwGameLinkState = TAG_LINK_FAIL;
		((*iter).second)->dwUserLogonState = USER_LOGON_SERVER_FAIL;
		((*iter).second)->dwUserGameState = USER_LOGON_SERVER_FAIL;
		((*iter).second)->TcpSocketHelper->CloseSocket();
	}
	FreeGameUserInfo();
}

//��������ص�����
bool CGameUserInfo::SetIUnknownEx(BYTE byTag, IUnknownEx * pIUnknownEx)
{
	std::map<DWORD, strUserGameInfo*>::iterator iter = m_vecUserGameInfo.begin();
	for (; iter!=m_vecUserGameInfo.end(); ++iter)
	{
		if (NULL == (*iter).second)
		{
			continue;
		}
		if (1 == byTag)
		{//��¼
			if (NULL == ((*iter).second)->TcpSocketHelper.GetInterface())
			{
				((*iter).second)->TcpSocketHelper.CreateInstance();
			}
			if (false == ((*iter).second)->TcpSocketHelper->SetTCPSocketSink(pIUnknownEx))
			{
				return false;
			}
		}
		else if ((2 == byTag) && (0 != (*iter).second->dwUserID) && (TAG_LINK_SUC == ((*iter).second)->dwLogonLinkState))
		{//��¼�ɹ�.
			if (NULL == ((*iter).second)->TcpSocketHelper.GetInterface())
			{
				((*iter).second)->TcpSocketHelper.CreateInstance();
			}
			if (false == ((*iter).second)->TcpSocketHelper->SetTCPSocketSink(pIUnknownEx))
			{
				return false;
			}
		}
	}
	return true;
}


//��ʼ���û���������
void CGameUserInfo::InitGameUserShareData()
{
	::memset(&m_GameServerInfo, 0, sizeof(m_GameServerInfo));
}

//��÷�����Ϣ
const Local_GR_ServerInfo* CGameUserInfo::GetLocalRoomInfo()
{
	return &m_GameServerInfo;
}

//���÷�����Ϣ
void CGameUserInfo::SetLocalRoomInfo(WORD wGameGenre, WORD wKindID, WORD wTableCount, WORD wChairCount)
{
	m_GameServerInfo.bIsInit = true;
	m_GameServerInfo.wGameGenre = wGameGenre;	//��Ϸ����
	m_GameServerInfo.wKindID = wKindID;		//��ϷID
	m_GameServerInfo.wTableCount = wTableCount;	//���Ӹ���
	m_GameServerInfo.wChairCount = wChairCount;	//���Ӹ���
}




