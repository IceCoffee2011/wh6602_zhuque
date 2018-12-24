// GameLogon.cpp : ʵ���ļ�
//


#include "GameLogon.h"
#include ".\gamelogon.h"
#include "GlobalUnits.h"
#include "FlashEnterDlg.h"
#include <WinSock.h>


//���Ӷ�ʱ��
#define			IDI_TIMER_USER_LOGON_LOOK			4000
#define			TIME_USER_LOGON_LOOK_INTERVAL		1000

//һ��������
#define			LOGON_CONNECT_ONCE_NUMS				100

// CGameLogon

IMPLEMENT_DYNAMIC(CGameLogon, CWnd)
CGameLogon::CGameLogon()
{
	m_pMainWnd = NULL;
	m_pGameUserInfo = NULL;
	::memset(&m_LogonInfo, 0, sizeof(m_LogonInfo));
}

CGameLogon::~CGameLogon()
{
}


BEGIN_MESSAGE_MAP(CGameLogon, CWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//�ӿڲ�ѯ
void * __cdecl CGameLogon::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IViewItem,Guid,dwQueryVer);
	QUERYINTERFACE(ITCPSocketSink,Guid,dwQueryVer);
	//QUERYINTERFACE(IPlazaViewItem,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IViewItem,Guid,dwQueryVer);
	return NULL;
}

// CGameLogon ��Ϣ�������


int CGameLogon::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	return 0;
}


//���ӷ�����
bool CGameLogon::OnEventConnectServer()
{
	KillTimer(IDI_TIMER_USER_LOGON_LOOK);

	//���������ע�ᵽ��¼������
	m_pGameUserInfo = CGameUserInfo::GetInstance();
	::memset(&m_LogonInfo, 0, sizeof(m_LogonInfo));

	IUnknownEx * pIUnknownEx=(IUnknownEx *)QueryInterface(IID_IUnknownEx,VER_IUnknownEx);
	bool bRet = m_pGameUserInfo->SetIUnknownEx(1, pIUnknownEx);
	if (!bRet)
	{
		AfxMessageBox(TEXT("��������ص��ӿ�����ʧ�ܣ�"));
		return false;
	}
	
	if (m_pGameUserInfo->m_vecUserGameInfo.size()>0)
	{
		DWORD dwCountConnectNums = 0;
		std::map<DWORD, strUserGameInfo*>::iterator iter = m_pGameUserInfo->m_vecUserGameInfo.begin();
		for (; iter!=m_pGameUserInfo->m_vecUserGameInfo.end(); ++iter)
		{
			if ((NULL!=(*iter).second)&&(TAG_LINK_FAIL == ((*iter).second)->dwLogonLinkState))
			{
				((*iter).second)->dwCountConnect = 0;
				((*iter).second)->TcpSocketHelper->Connect(g_GlobalUnits.m_GameOption.chServerIP,g_GlobalUnits.m_GameOption.dwServerPort);
				((*iter).second)->dwLogonLinkState = TAG_LINKING;
				((*iter).second)->dwUserLogonState = USER_LOGON_SERVER_FAIL;
				++dwCountConnectNums;
				if (dwCountConnectNums>=LOGON_CONNECT_ONCE_NUMS)
				{
					break;
				}
			}
		}
	}
	

	//���ö�ʱ������¼ʧ�ܵ�.
	//�������ӳɹ�.
	SetTimer(IDI_TIMER_USER_LOGON_LOOK, TIME_USER_LOGON_LOOK_INTERVAL, NULL);
	return true;
}


//�����¼�
//�����¼�
bool __cdecl CGameLogon::OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode)
{
	std::map<DWORD, strUserGameInfo*>::iterator iter = m_pGameUserInfo->m_vecUserGameInfo.find(wSocketID);
	if (iter != m_pGameUserInfo->m_vecUserGameInfo.end())
	{
		if (nErrorCode!=0)
		{//����ʧ��.
			((*iter).second)->dwLogonLinkState = TAG_LINK_FAIL;
			++(((*iter).second)->dwCountConnect);
			if (NULL != m_pMainWnd)
			{
				char chData[128]={0};
				sprintf(chData, "���[%s]��TCP���¼�������Ͽ����ӣ�", ((*iter).second)->chName);
				m_pMainWnd->InsertListData(wSocketID, nErrorCode, chData);
			}
			return true;
		}
		else
		{//���ӳɹ�.
			UserLogonToLogonServer((*iter).second);
		}
	}
	return true;
}

//��¼����¼������
void CGameLogon::UserLogonToLogonServer(strUserGameInfo* pUserInfo)
{
	if (NULL != pUserInfo)
	{
		pUserInfo->dwUserID = 0;
		//��������
		TCHAR szPassword[33] = {0};
		BYTE cbBuffer[SOCKET_PACKET] = {0};
		CMD5Encrypt::EncryptData(g_GlobalUnits.m_GameOption.chUserPassWord,szPassword);

		//��������
		CMD_GP_LogonByAccounts * pLogonByAccounts=(CMD_GP_LogonByAccounts *)cbBuffer;
		memset(pLogonByAccounts,0,sizeof(CMD_GP_LogonByAccounts));
		pLogonByAccounts->dwPlazaVersion=g_GlobalUnits.GetPlazaVersion();
		CopyMemory(pLogonByAccounts->szPassWord,szPassword,sizeof(pLogonByAccounts->szPassWord));
		lstrcpyn(pLogonByAccounts->szAccounts,pUserInfo->chName,CountArray(pLogonByAccounts->szAccounts));

		//�������к�
		tagClientSerial ClientSerial;
		g_GlobalUnits.GetClientSerial(ClientSerial);

		//�������ݰ�
		CSendPacketHelper Packet(cbBuffer+sizeof(CMD_GP_LogonByAccounts),sizeof(cbBuffer)-sizeof(CMD_GP_LogonByAccounts));
		Packet.AddPacket(&ClientSerial,sizeof(ClientSerial),DTP_COMPUTER_ID);
		pUserInfo->TcpSocketHelper->SendData(MDM_GP_LOGON,SUB_GP_LOGON_ACCOUNTS,cbBuffer,sizeof(CMD_GP_LogonByAccounts)+Packet.GetDataSize());
		pUserInfo->dwLogonLinkState = TAG_LINK_SUC;
		pUserInfo->dwUserLogonState = USER_LOGON_SERVER_LINKING;
	}
}

//�ر��¼�
bool __cdecl CGameLogon::OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason)
{
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//�ر�����
		std::map<DWORD, strUserGameInfo*>::iterator iter = m_pGameUserInfo->m_vecUserGameInfo.find(wSocketID);
		if (iter != m_pGameUserInfo->m_vecUserGameInfo.end())
		{
			((*iter).second)->dwLogonLinkState = TAG_LINK_FAIL;
			++(((*iter).second)->dwCountConnect);
			if (NULL != m_pMainWnd)
			{
				char chData[128]={0};
				sprintf(chData, "���[%s]��TCP���¼�������Ͽ����ӣ�", ((*iter).second)->chName);
				m_pMainWnd->InsertListData(wSocketID, cbShutReason, chData);
			}
		}
	}
	return true;
}

//��ȡ�¼�
bool __cdecl CGameLogon::OnEventTCPSocketRead(WORD wSocketID, CMD_Command Command, VOID * pData, WORD wDataSize)
{
	switch (Command.wMainCmdID)
	{
	case MDM_GP_LOGON:				//��¼��Ϣ
		{
			return OnSocketMainLogon(wSocketID, Command,pData,wDataSize);		//�û���֤����¼�ɹ������Ϣ
		}
	case MDM_GP_SERVER_LIST:		//�б���Ϣ
		{
			return OnSocketMainServerList(wSocketID, Command,pData,wDataSize);	//������Ϸ�����б�
		}
	default:
		break;
	}
	return true;
}


//��¼��Ϣ
bool CGameLogon::OnSocketMainLogon(WORD wSocketID, CMD_Command Command, void * pData, WORD wDataSize)
{
	switch (Command.wSubCmdID)
	{
	case SUB_GP_LOGON_SUCCESS:		//��¼�ɹ�
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(CMD_GP_LogonSuccess));
			if (wDataSize<sizeof(CMD_GP_LogonSuccess))
			{
				return false;
			}
			
			CMD_GP_LogonSuccess * pLogonSuccess=(CMD_GP_LogonSuccess *)pData;
			if (NULL != pLogonSuccess)
			{
				std::map<DWORD, strUserGameInfo*>::iterator iter = m_pGameUserInfo->m_vecUserGameInfo.find(wSocketID);
				if (iter != m_pGameUserInfo->m_vecUserGameInfo.end())
				{
					(*iter).second->dwUserID = pLogonSuccess->dwUserID;
					(*iter).second->llUserGems = pLogonSuccess->lGold;
				}
			}
			return true;
		}
	case SUB_GP_LOGON_ERROR:		//��¼ʧ��
		{
			//Ч�����
			CMD_GP_LogonError *pLogonError = (CMD_GP_LogonError *)pData;
			ASSERT(wDataSize>=(sizeof(CMD_GP_LogonError)-sizeof(pLogonError->szErrorDescribe)));
			if (wDataSize<(sizeof(CMD_GP_LogonError)-sizeof(pLogonError->szErrorDescribe)))
			{
				return false;
			}
			
			//�ر�����
			std::map<DWORD, strUserGameInfo*>::iterator iter = m_pGameUserInfo->m_vecUserGameInfo.find(wSocketID);
			if (iter != m_pGameUserInfo->m_vecUserGameInfo.end())
			{
				if (NULL != m_pMainWnd)
				{
					char chData[128]={0};
					sprintf(chData, "���[%s]��¼����¼������ʧ�ܣ�", ((*iter).second)->chName);
					m_pMainWnd->InsertListData(wSocketID, 0, chData);
				}
				((*iter).second)->dwUserLogonState = USER_LOGON_SERVER_FAIL;
				++(((*iter).second)->dwCountConnect);

				++m_LogonInfo.dwLogonFailNums;
				if (NULL != m_pMainWnd)
				{
					m_pMainWnd->UpdateLogonInfo();
				}
			}
			return true;
		}
	case SUB_GP_LOGON_FINISH:		//��¼���
		{
			std::map<DWORD, strUserGameInfo*>::iterator iter = m_pGameUserInfo->m_vecUserGameInfo.find(wSocketID);
			if (iter != m_pGameUserInfo->m_vecUserGameInfo.end())
			{
				if (TAG_LINK_SUC == ((*iter).second)->dwLogonLinkState)
				{
					//�رյ�¼����
					((*iter).second)->TcpSocketHelper->CloseSocket();
					((*iter).second)->dwUserLogonState = USER_LOGON_SERVER_SUC;
					((*iter).second)->dwCountConnect = 0;
					
					++m_LogonInfo.dwLogonSucNums;
					if (NULL != m_pMainWnd)
					{
						m_pMainWnd->UpdateLogonInfo();
					}
				}
			}
			
			return true;
		}
	}
	return true;
}

bool CGameLogon::OnSocketMainServerList(WORD wSocketID, CMD_Command Command, void * pData, WORD wDataSize)
{
	ASSERT(Command.wMainCmdID==MDM_GP_SERVER_LIST);

	switch (Command.wSubCmdID)
	{
	case SUB_GP_LIST_SERVER:		//����������
		{
			if (g_GlobalUnits.m_bInitGameKindList)
			{
				return true;
			}

			//Ч�����
			ASSERT(wDataSize%sizeof(tagGameServer)==0);
			if (wDataSize%sizeof(tagGameServer)!=0)
			{
				return false;
			}
			
			//������Ϣ
			tagGameServer * pGameServer=(tagGameServer *)pData;
			WORD wItemCount=wDataSize/sizeof(tagGameServer);
			if (NULL != pGameServer)
			{
				for (WORD i=0; i<wItemCount; i++)
				{
					if (g_GlobalUnits.m_GameOption.dwGameID==pGameServer[i].wKindID && g_GlobalUnits.m_GameOption.dwRoomID==pGameServer[i].wSortID)
					{//�ĸ���Ϸ�ĸ�����
						GameServerList gsl;
						::memset(&gsl, 0, sizeof(GameServerList));
						gsl.wSortID = pGameServer[i].wSortID;
						gsl.wKindID = pGameServer[i].wKindID;
						//gsl.dwGameServerIP = pGameServer[i].dwServerAddrCNN;
						//gsl.dwGameServerport = pGameServer[i].wServerPort; 

						gsl.dwGameServerIP = inet_addr(g_GlobalUnits.m_GameOption.chGameServerIP);
						gsl.dwGameServerport = g_GlobalUnits.m_GameOption.dwGameServerPort; 

						gsl.dwOnLineCount = pGameServer[i].dwOnLineCount;
						gsl.dwMaxCount = pGameServer[i].dwMaxCount;
						gsl.lRestrictScore = pGameServer[i].lRestrictScore;
						g_GlobalUnits.m_GameKindList.push_back(gsl);
						
						if (NULL != m_pMainWnd)
						{
							char chData[256]={0};
							sprintf(chData, "��Ϸ�����б���Ϣ:KindID:%d, SortID:%d, IP:%d, Port:%d, OnLine:%d, MaxLine:%d.", gsl.wKindID, gsl.wSortID, 
								gsl.dwGameServerIP, gsl.dwGameServerport, gsl.dwOnLineCount, gsl.dwMaxCount);
							m_pMainWnd->InsertListData(0, 0, chData);
						}
					}
				}
			}
			g_GlobalUnits.m_bInitGameKindList = true;
			return true;
		}
	}

	return true;
}

//����������ָ��
void CGameLogon::SetMainWnd(CFlashEnterDlg* pMainWnd)
{
	if (NULL != pMainWnd)
	{
		m_pMainWnd = pMainWnd;
	}
}

//������Ӧֵ
void CGameLogon::GetLogonInfo(DWORD &dwLogonSucNums, DWORD &dwLogonFailNums, DWORD &dwTCPCoonectNums, DWORD &dwUserConnectNums)
{
	dwLogonSucNums = m_LogonInfo.dwLogonSucNums;
	dwLogonFailNums = m_LogonInfo.dwLogonFailNums;
	dwTCPCoonectNums = m_LogonInfo.dwTCPConnectNums;
	dwUserConnectNums = m_LogonInfo.dwUserConnectNums;
}

//�˳���¼������
bool CGameLogon::OnEventExitServer()
{
	KillTimer(IDI_TIMER_USER_LOGON_LOOK);
	m_pGameUserInfo->ClearGameUserInfo();
	::memset(&m_LogonInfo, 0, sizeof(m_LogonInfo));
	if (NULL != m_pMainWnd)
	{
		m_pMainWnd->UpdateLogonInfo();
		m_pMainWnd->UpdateLogonConnInfo();
	}
	return true;
}

void CGameLogon::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (IDI_TIMER_USER_LOGON_LOOK == nIDEvent)
	{//���TCP����״̬
		m_LogonInfo.dwTCPConnectNums = 0;
		m_LogonInfo.dwUserConnectNums = 0;
		std::map<DWORD, strUserGameInfo*>::iterator iter = m_pGameUserInfo->m_vecUserGameInfo.begin();
		for (; iter!=m_pGameUserInfo->m_vecUserGameInfo.end(); ++iter)
		{
			if (NULL==(*iter).second) continue;
			if (((*iter).second)->dwCountConnect >= I_CONNECT_NUMS) continue;
			if (TAG_LINK_FAIL == ((*iter).second)->dwLogonLinkState)
			{
				if (m_LogonInfo.dwTCPConnectNums>=LOGON_CONNECT_ONCE_NUMS)
				{
					continue;
				}
				if (NULL == ((*iter).second)->TcpSocketHelper.GetInterface())
				{
					((*iter).second)->TcpSocketHelper.CreateInstance();
				}
				IUnknownEx * pIUnknownEx=(IUnknownEx *)QueryInterface(IID_IUnknownEx,VER_IUnknownEx);
				if (false == ((*iter).second)->TcpSocketHelper->SetTCPSocketSink(pIUnknownEx))
				{
					KillTimer(IDI_TIMER_USER_LOGON_LOOK);
					break;
				}
				((*iter).second)->TcpSocketHelper->Connect(g_GlobalUnits.m_GameOption.chServerIP,g_GlobalUnits.m_GameOption.dwServerPort);
				((*iter).second)->dwLogonLinkState = TAG_LINKING;
				++m_LogonInfo.dwTCPConnectNums;
			}
			else if (TAG_LINK_SUC == ((*iter).second)->dwLogonLinkState)
			{//TCP���ӳɹ���ǰ��������.
				if (USER_LOGON_SERVER_FAIL == ((*iter).second)->dwUserLogonState)
				{//��¼ʧ��.
					if (m_LogonInfo.dwUserConnectNums>=LOGON_CONNECT_ONCE_NUMS)
					{
						continue;
					}
					UserLogonToLogonServer((*iter).second);
					++m_LogonInfo.dwUserConnectNums;
				}
			}
			if (m_LogonInfo.dwTCPConnectNums>=LOGON_CONNECT_ONCE_NUMS || m_LogonInfo.dwUserConnectNums>=LOGON_CONNECT_ONCE_NUMS)
			{
				break;
			}
		}
		m_LogonInfo.dwLogonFailNums -= m_LogonInfo.dwUserConnectNums;
		if (NULL != m_pMainWnd)
		{
			m_pMainWnd->UpdateLogonConnInfo();
		}
	}
	__super::OnTimer(nIDEvent);
}
