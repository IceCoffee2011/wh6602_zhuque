// GameServer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FlashEnterDlg.h"
#include "GameServer.h"
#include "GlobalUnits.h"

//��ʱ��
#define			IDI_TIMER_GAME_SERVER_LOOK				3021
#define			TIMER_GAME_SERVER_LOOK_INTERVAL			10000

//һ��������
#define			GAME_CONNECT_ONCE_NUMS				100

// CGameServer

IMPLEMENT_DYNAMIC(CGameServer, CWnd)
CGameServer::CGameServer()
{
	m_pMainWnd = NULL;
	m_pGameUserInfo = NULL;
	::memset(&m_LogonInfo, 0, sizeof(m_LogonInfo));
	m_dwQueueSuccNums = 0;
	m_dwQueueFailNums = 0;
}

CGameServer::~CGameServer()
{
}


BEGIN_MESSAGE_MAP(CGameServer, CWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

//�ӿڲ�ѯ
void * __cdecl CGameServer::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IViewItem,Guid,dwQueryVer);
	QUERYINTERFACE(ITCPSocketSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IViewItem,Guid,dwQueryVer);
	return NULL;
}

//������Ϸ������֮ǰ��ʼ������
bool CGameServer::InitGameLocalData()
{
	KillTimer(IDI_TIMER_GAME_SERVER_LOOK);

	m_dwQueueSuccNums = 0;
	m_dwQueueFailNums = 0;
	::memset(&m_LogonInfo, 0, sizeof(m_LogonInfo));

	m_pGameUserInfo = CGameUserInfo::GetInstance();
	if (NULL == m_pGameUserInfo)
	{
		return false;
	}
	m_pGameUserInfo->InitGameUserShareData();
	return true;
}

//��ұ�ʯ����,���ܵ�¼��Ϸ������
bool CGameServer::UserCanLogonGame(LONG lRestrictScore)
{
	if (lRestrictScore <=0)
	{
		return false;
	}
	if (g_GlobalUnits.m_GameOption.dwRoomID==0 || g_GlobalUnits.m_GameOption.dwRoomID>6)
	{
		return false;
	}
	DWORD dwGameServerListNums = g_GlobalUnits.m_GameKindList.size();
	if (dwGameServerListNums <= 0 || dwGameServerListNums>100)
	{
		return false;
	}

	for (int i=0; i<dwGameServerListNums; i++)
	{
		if (g_GlobalUnits.m_GameKindList[i].wSortID == g_GlobalUnits.m_GameOption.dwRoomID)
		{
			return (lRestrictScore>=g_GlobalUnits.m_GameKindList[i].lRestrictScore)?true:false;
		}
		
	}
	return false;
}

//��ʼ���������IP�Ͷ˿ں�
bool CGameServer::StartAllotServerIPAndPort()
{
	if (m_pGameUserInfo->m_vecUserGameInfo.size()<=0)
	{
		return false;
	}
	DWORD dwCountConnect = 0;
	std::map<DWORD, strUserGameInfo*>::iterator iter = m_pGameUserInfo->m_vecUserGameInfo.begin();
	for (; iter!=m_pGameUserInfo->m_vecUserGameInfo.end(); ++iter)
	{
		if ((NULL!=(*iter).second) && (TAG_LINK_SUC == ((*iter).second)->dwLogonLinkState))
		{//��¼�������ɹ���.
			++dwCountConnect;
		}
	}
	DWORD dwGameServerListNums = g_GlobalUnits.m_GameKindList.size();
	if (dwGameServerListNums <= 0 || dwGameServerListNums>100)
	{
		return false;
	}
	//�ѵ�¼�ɹ������ƽ����������Ϸ������.
	DWORD dwOnceServerUserNums = dwCountConnect/dwGameServerListNums;
	if (dwOnceServerUserNums<=0 || dwOnceServerUserNums>dwCountConnect)
	{
		return false;
	}
	iter = m_pGameUserInfo->m_vecUserGameInfo.begin();
	for (int i=0; i<dwGameServerListNums; i++)
	{
		for (int j=0; j<dwOnceServerUserNums; j++)
		{
			for (; iter!=m_pGameUserInfo->m_vecUserGameInfo.end(); ++iter)
			{
				if ((NULL!=(*iter).second) && (TAG_LINK_SUC == ((*iter).second)->dwLogonLinkState))
				{//��¼�������ɹ���.
					DWORD aa = g_GlobalUnits.m_GameKindList[i].dwGameServerIP;
					DWORD bb = g_GlobalUnits.m_GameKindList[i].dwGameServerport;
					((*iter).second)->dwGameServerIP = g_GlobalUnits.m_GameKindList[i].dwGameServerIP;
					((*iter).second)->dwGameServerport = g_GlobalUnits.m_GameKindList[i].dwGameServerport;
					++iter;
					break;
				}
			}
		}
	}
	return true;
}

//���ӵ���Ϸ������
void CGameServer::OnConnectGameServer()
{
	//��ʼ������
	bool bRet = InitGameLocalData();
	if (!bRet)
	{
		MessageBox("��������Ϸ������ǰ��ʼ��ʧ�ܣ�");
		return;
	}
	
	//�������
	IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IUnknownEx);
	bRet = m_pGameUserInfo->SetIUnknownEx(2, pIUnknownEx);
	if (!bRet)
	{
		AfxMessageBox(TEXT("��������ص��ӿ�����ʧ�ܣ�"));
		return;
	}

	//��ʼ���������IP�Ͷ˿ں�
	bool bIsSuc = StartAllotServerIPAndPort();
	if (!bIsSuc)
	{
		AfxMessageBox(TEXT("���������IP�Ͷ˿ں�ʧ�ܣ�"));
		return;
	}

	if (m_pGameUserInfo->m_vecUserGameInfo.size()>0)
	{
		DWORD dwCountConnect = 0;
		std::map<DWORD, strUserGameInfo*>::iterator iter = m_pGameUserInfo->m_vecUserGameInfo.begin();
		for (; iter!=m_pGameUserInfo->m_vecUserGameInfo.end(); ++iter)
		{
			if ((NULL != (*iter).second)&&(TAG_LINK_SUC == ((*iter).second)->dwLogonLinkState && TAG_LINK_FAIL == ((*iter).second)->dwGameLinkState))
			{//��¼�ɹ���.
				if (0!=(*iter).second->dwUserID)
				{
					if (0!=(*iter).second->dwGameServerIP && 0!=(*iter).second->dwGameServerport)
					{
						if (UserCanLogonGame((*iter).second->llUserGems))
						{
							((*iter).second)->dwCountConnect = 0;
							((*iter).second)->TcpSocketHelper->SetTCPSocketSink(pIUnknownEx);
							((*iter).second)->TcpSocketHelper->Connect((*iter).second->dwGameServerIP,(*iter).second->dwGameServerport);
							((*iter).second)->dwGameLinkState = TAG_LINKING;
							((*iter).second)->dwUserGameState = USER_LOGON_SERVER_FAIL;
							++dwCountConnect;
							if (dwCountConnect>=GAME_CONNECT_ONCE_NUMS)
							{
								break;
							}
						}
						else
						{
							char chData[256]={0};
							sprintf(chData, "CGameServer::OnConnectGameServer->���[%s]������IP:%d,�˿ں�:%d,���(%d)������", \
								(((*iter).second)->chName), ((*iter).second)->dwGameServerIP, ((*iter).second)->dwGameServerport, ((*iter).second)->llUserGems);
							m_pMainWnd->InsertListData((*iter).second->dwSocketID, 0, chData);
						}
					}
				}
			}
		}
	}
	
	SetTimer(IDI_TIMER_GAME_SERVER_LOOK, TIMER_GAME_SERVER_LOOK_INTERVAL, NULL);
}


//�����¼�
bool __cdecl CGameServer::OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode)
{
	//���ӳɹ�.
	std::map<DWORD, strUserGameInfo*>::iterator iter = m_pGameUserInfo->m_vecUserGameInfo.find(wSocketID);
	if (iter != m_pGameUserInfo->m_vecUserGameInfo.end())
	{
		if (0==nErrorCode)
		{//���ӳɹ�.
			UserLogonToGameServer((*iter).second);
		}
		else if (nErrorCode!=0)
		{//����ʧ��.
			((*iter).second)->dwGameLinkState = TAG_LINK_FAIL;
			++(((*iter).second)->dwCountConnect);
			if (NULL != m_pMainWnd)
			{
				char chData[256]={0};
				sprintf(chData, "CGameServer::OnEventTCPSocketLink->���[%s]��TCP������Ϸ������[IP:%d,PORT:%d]�Ͽ����ӣ�", ((*iter).second)->chName, ((*iter).second)->dwGameServerIP, ((*iter).second)->dwGameServerport);
				m_pMainWnd->InsertListData(wSocketID, nErrorCode, chData);
			}
			return true;
		}
	}
	return true;
}

//��¼����Ϸ������
void CGameServer::UserLogonToGameServer(strUserGameInfo* pUserInfo)
{
	if (NULL != pUserInfo)
	{
		if (UserCanLogonGame(pUserInfo->llUserGems))
		{
			//��ȡ��Ϣ
			BYTE cbBuffer[SOCKET_PACKET] = {0};
			TCHAR szPassword[33] = {0};
			CMD5Encrypt::EncryptData(g_GlobalUnits.m_GameOption.chUserPassWord,szPassword);

			//��¼���ݰ�
			CMD_GR_LogonByUserID * pLogonByUserID=(CMD_GR_LogonByUserID *)cbBuffer;
			pLogonByUserID->dwUserID=pUserInfo->dwUserID;
			pLogonByUserID->dwPlazaVersion=g_GlobalUnits.GetPlazaVersion();
			pLogonByUserID->dwKindID = g_GlobalUnits.m_GameOption.dwGameID;
			lstrcpyn(pLogonByUserID->szPassWord,szPassword,sizeof(pLogonByUserID->szPassWord));

			//�������к�
			tagClientSerial ClientSerial;
			g_GlobalUnits.GetClientSerial(ClientSerial);

			//�������ݰ�
			CSendPacketHelper Packet(cbBuffer+sizeof(CMD_GR_LogonByUserID),sizeof(cbBuffer)-sizeof(CMD_GR_LogonByUserID));
			Packet.AddPacket(&ClientSerial,sizeof(ClientSerial),DTP_COMPUTER_ID);
			pUserInfo->TcpSocketHelper->SendData(MDM_GR_LOGON,SUB_GR_LOGON_USERID,cbBuffer,sizeof(CMD_GR_LogonByUserID)+Packet.GetDataSize());
			pUserInfo->dwGameLinkState = TAG_LINK_SUC;
			pUserInfo->dwUserGameState = USER_LOGON_SERVER_LINKING;
		}
	}
}

//�ر��¼�
bool __cdecl CGameServer::OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason)
{
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		//�ر�����
		std::map<DWORD, strUserGameInfo*>::iterator iter = m_pGameUserInfo->m_vecUserGameInfo.find(wSocketID);
		if (iter!=m_pGameUserInfo->m_vecUserGameInfo.end())
		{
			if (NULL != ((*iter).second))
			{
				((*iter).second)->dwGameLinkState = TAG_LINK_FAIL;
				++(((*iter).second)->dwCountConnect);
				if (NULL != m_pMainWnd)
				{
					char chData[256]={0};
					sprintf(chData, "CGameServer::OnEventTCPSocketShut->���[%s]������Sock����Ϸ������[IP:%d,PORT:%d]�Ͽ����ӣ�", ((*iter).second)->chName, ((*iter).second)->dwGameServerIP, ((*iter).second)->dwGameServerport);
					m_pMainWnd->InsertListData(wSocketID, cbShutReason, chData);
				}
			}
		}
	}

	return true;
}

//��ȡ�¼�
bool __cdecl CGameServer::OnEventTCPSocketRead(WORD wSocketID, CMD_Command Command, VOID * pData, WORD wDataSize)
{
	bool bRet = true;
	switch (Command.wMainCmdID)
	{
	case MDM_GF_GAME:			//��Ϸ��Ϣ
	case MDM_GF_FRAME:			//�����Ϣ
		{
			bRet =  OnSocketMainGameFrame(wSocketID,Command,pData,wDataSize);
		}
		break;
	default:
		{
			std::map<DWORD, strUserGameInfo*>::iterator iter = m_pGameUserInfo->m_vecUserGameInfo.find(wSocketID);
			if (iter != m_pGameUserInfo->m_vecUserGameInfo.end())
			{
				switch (Command.wMainCmdID)
				{
				case MDM_GR_LOGON:			//��¼��Ϣ
					{
						bRet =  OnSocketMainLogon(wSocketID,Command,pData,wDataSize);
						break;
					}
				case MDM_GR_USER:			//�û���Ϣ
					{
						bRet =  OnSocketMainUser(wSocketID,Command,pData,wDataSize);
						break;
					}
				case MDM_GR_INFO:			//������Ϣ
					{
						bRet =  OnSocketMainInfo(wSocketID,Command,pData,wDataSize);
						break;
					}
				case MDM_GR_STATUS:			//״̬��Ϣ
					{
						//return true;
						//return OnSocketMainStatus(wSocketID,Command,pData,wDataSize);
						break;
					}
				case MDM_GR_SYSTEM:			//ϵͳ��Ϣ
					{
						//return true;
						//return OnSocketMainSystem(wSocketID,Command,pData,wDataSize);
						break;
					}
				case MDM_GR_SERVER_INFO:	//������Ϣ
					{
						//return true;
						//return OnSocketMainServerInfo(wSocketID,Command,pData,wDataSize);
						break;
					}
				default:
					break;
				}
			}
		}
		break;
	}

	if (!bRet)
	{
		if (NULL != m_pMainWnd)
		{
			char chData[256]={0};
			sprintf(chData, "��Ϣ��ID:%d,����ϢID:%d,����%d,����Size:%d, ��Ϣ���ܲ���ȷ����δ������", Command.wMainCmdID, Command.wSubCmdID, pData, wDataSize);
			m_pMainWnd->InsertListData(wSocketID, 0, chData);
		}
	}

	return true;
}

//��¼��Ϣ
bool CGameServer::OnSocketMainLogon(WORD wSocketID, CMD_Command Command, void * pBuffer, WORD wDataSize)
{
	strUserGameInfo* pUserGameInfo = m_pGameUserInfo->m_vecUserGameInfo[wSocketID];
	if (NULL == pUserGameInfo)
	{
		return true;
	}
	switch (Command.wSubCmdID)
	{
	case SUB_GR_LOGON_SUCCESS://��¼�ɹ�
		{
			return true;
		}
	case SUB_GR_LOGON_ERROR://��¼ʧ��
	case SUB_GR_LOGON_FULL://��������
		{
			//Ч�����
			//CMD_GR_LogonError * pLogonError=(CMD_GR_LogonError *)pBuffer;
			//ASSERT(wDataSize>=(sizeof(CMD_GR_LogonError)-sizeof(pLogonError->szErrorDescribe)));
			//if (wDataSize<(sizeof(CMD_GR_LogonError)-sizeof(pLogonError->szErrorDescribe))) return false;

			////��ʾ��Ϣ
			//WORD wDescribeSize=wDataSize-(sizeof(CMD_GR_LogonError)-sizeof(pLogonError->szErrorDescribe));
			//if (wDescribeSize>0)
			//{
			//	pLogonError->szErrorDescribe[wDescribeSize-1]=0;
			//	AfxMessageBox(pLogonError->szErrorDescribe);
			//}

			//�ر�����
			if (NULL != pUserGameInfo)
			{
				if (NULL != m_pMainWnd)
				{
					char chData[128]={0};
					sprintf(chData, "���[%s]��¼����Ϸ������ʧ�ܣ�", pUserGameInfo->chName);
					m_pMainWnd->InsertListData(wSocketID, 0, chData);
				}
				pUserGameInfo->dwUserGameState = USER_LOGON_SERVER_FAIL;

				++m_LogonInfo.dwLogonFailNums;
				if (NULL != m_pMainWnd)
				{
					m_pMainWnd->UpdateGameInfo();
				}
			}
			return true;
		} 
	case SUB_GR_LOGON_FINISH://��¼���
		{
			if (TAG_LINK_SUC == pUserGameInfo->dwGameLinkState)
			{//TCP���ӳɹ���.
				pUserGameInfo->dwUserGameState = USER_LOGON_SERVER_SUC;
				++m_LogonInfo.dwLogonSucNums;
				if (NULL != m_pMainWnd)
				{
					m_pMainWnd->UpdateGameInfo();
				}
			}
			//���ý���
			return true;
		}
	}
	return true;
}

//�û���Ϣ
bool CGameServer::OnSocketMainUser(WORD wSocketID, CMD_Command Command, void * pBuffer, WORD wDataSize)
{
	strUserGameInfo* pUserGameInfo = m_pGameUserInfo->m_vecUserGameInfo[wSocketID];
	if (NULL == pUserGameInfo)
	{
		return true;
	}
	switch (Command.wSubCmdID)
	{
	case SUB_GR_USER_COME:			//�û�����
		{
			return OnSocketSubUserCome(pUserGameInfo, pBuffer, wDataSize);
		}
	case SUB_GR_USER_STATUS:		//�û�״̬
		{
			return OnSocketSubStatus(pUserGameInfo, pBuffer, wDataSize);
		}
	case SUB_GR_USER_SCORE:			//�û�����
		{
			return OnSocketSubScore(pUserGameInfo, pBuffer, wDataSize);
		}
	case SUB_GR_USER_RIGHT:			//�û�Ȩ��
		{
			return true;
		}
	case SUB_GR_MEMBER_ORDER:		//��Ա�ȼ�
		{
			return true;
		}
	case SUB_GR_SIT_FAILED:			//����ʧ��
		{
			return true;
		}
	case SUB_GR_USER_CHAT:			//�û�����
		{
			return true;
		}
	case SUB_GR_USER_WISPER:		//�û�˽��
		{
			return true;
		}
	case SUB_GR_USER_INVITE:		//�������
		{
			return true;
		}
		//�Ŷӳɹ�      �������   
	case SUB_GR_AUTO_QUEUE_SUCC:
	case SUB_GR_AUTO_QUEUE:			//�Զ��Ŷ�
		{
			//��Ϣ����
			CMD_GR_AutoQueue * pAutoQueue=(CMD_GR_AutoQueue *)pBuffer;
			if (NULL != pAutoQueue)
			{
				++m_dwQueueSuccNums;
				
				if (NULL != m_pMainWnd)
				{
					m_pMainWnd->UpdateQueueInfo();
				}
			}
			return true;
		}
	case SUB_GR_AUTO_QUEUE_FAIL:	//�Ŷ�ʧ��
		{
			++m_dwQueueFailNums;

			std::map<DWORD, strUserGameInfo*>::iterator iter = m_pGameUserInfo->m_vecUserGameInfo.find(wSocketID);
			if (iter != m_pGameUserInfo->m_vecUserGameInfo.end())
			{
				((*iter).second)->TcpSocketHelper->SendData(MDM_GR_USER,SUB_GR_AUTO_QUEUE);
			}

			if (NULL != m_pMainWnd)
			{
				m_pMainWnd->UpdateQueueInfo();
			}
			return true;
		}
	case SUB_GR_QUEUE_COUNT:	//�Ŷ�����
		{
			return true;
		}
	case SUB_RECVACTIONINFO:		//���ܶ���
		{
			return true;
		}
	case SUB_GR_NO_GOLD:		//��Ҳ��㣬�˻ط����б�
		{
			pUserGameInfo->TcpSocketHelper->CloseSocket();
			pUserGameInfo->dwLogonLinkState = TAG_LINK_FAIL;
			pUserGameInfo->dwUserGameState = USER_LOGON_SERVER_FAIL;
			return true;
		}
	case SUB_GR_AUTO_QUEUE_CANCEL_FALL:		//ȡ���Ŷ�ʧ��
		{
			return true;
		}
	case SUB_GR_AUTO_QUEUE_CANCEL_SUCC:		//ȡ���Ŷӳɹ�
		{
			return true;
		}
	case SUB_GF_QUIT_GAME_SERVER_SUCC:		//���ص������б�
		{
			return true;
		}
	default:
		break;
	}

	if (NULL != m_pMainWnd)
	{
		m_pMainWnd->UpdateQueueInfo();
	}
	return true;
}

//�û�����
bool CGameServer::OnSocketSubUserCome(strUserGameInfo* pUserGameInfo, void * pBuffer, WORD wDataSize)
{
	if (NULL == pUserGameInfo)
	{
		return false;
	}

	//Ч�����
	ASSERT(wDataSize>=sizeof(tagUserInfoHead));
	if (wDataSize<sizeof(tagUserInfoHead))
	{
		return false;
	}

	//��ȡ������Ϣ
	tagUserData UserData;
	memset(&UserData,0,sizeof(UserData));
	tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)pBuffer;
	if (NULL == pUserInfoHead)
	{
		return false;
	}
	if (pUserGameInfo->dwUserID != pUserInfoHead->dwUserID)
	{//���Ǳ���Ҫ���ܵ���Ϣ
		return true;
	}

	//��ȡ��Ϣ
	UserData.dwUserID=pUserInfoHead->dwUserID;
	UserData.wTableID=pUserInfoHead->wTableID;
	UserData.wChairID=pUserInfoHead->wChairID;
	UserData.cbUserStatus=pUserInfoHead->cbUserStatus;
	UserData.dwUserRight=pUserInfoHead->dwUserRight;
	UserData.dwMasterRight=pUserInfoHead->dwMasterRight;
	//UserData.lInsureScore = pUserInfoHead->lInsureScore;

	//��ȡ��Ϣ
	UserData.wFaceID=pUserInfoHead->wFaceID;
	UserData.dwCustomFaceVer=pUserInfoHead->dwCustomFaceVer;
	UserData.cbGender=pUserInfoHead->cbGender;
	UserData.cbMemberOrder=pUserInfoHead->cbMemberOrder;
	UserData.cbMasterOrder=pUserInfoHead->cbMasterOrder;
	UserData.dwGameID=pUserInfoHead->dwGameID;
	UserData.dwGroupID=pUserInfoHead->dwGroupID;
	UserData.lLoveliness=pUserInfoHead->lLoveliness;
	UserData.lMoney=pUserInfoHead->UserScoreInfo.lMoney;
	UserData.lGameGold=pUserInfoHead->UserScoreInfo.lGold;
	//UserData.lInsureScore=pUserInfoHead->UserScoreInfo.lInsureScore;
	UserData.lWinCount=pUserInfoHead->UserScoreInfo.lWinCount;
	UserData.lLostCount=pUserInfoHead->UserScoreInfo.lLostCount;
	UserData.lDrawCount=pUserInfoHead->UserScoreInfo.lDrawCount;
	UserData.lFleeCount=pUserInfoHead->UserScoreInfo.lFleeCount;
	UserData.lExperience=pUserInfoHead->UserScoreInfo.lExperience;

	UserData.dwGrade = pUserInfoHead->UserScoreInfo.dwGrade;
	//UserData.dwOrder = pUserInfoHead->UserScoreInfo.dwOrder;

	UserData.lGem = pUserInfoHead->UserScoreInfo.lGem;   //�û��ı�ʯ
	//UserData.lHonor = pUserInfoHead->UserScoreInfo.lHonor;  //�û�������
	//UserData.lCredit = pUserInfoHead->UserScoreInfo.lCredit; //�û�������

	//��ҽ���
	pUserGameInfo->pIGameClient->OnEventUserEnter(&UserData);
	return true;
}


//�û�״̬
bool CGameServer::OnSocketSubStatus(strUserGameInfo* pUserGameInfo, void * pBuffer, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize>=sizeof(CMD_GR_UserStatus));
	if (wDataSize<sizeof(CMD_GR_UserStatus))
	{
		return false;
	}

	//��������
	CMD_GR_UserStatus * pUserStatus=(CMD_GR_UserStatus *)pBuffer;
	if (NULL != pUserStatus)
	{
		if (pUserGameInfo->dwUserID != pUserStatus->dwUserID)
		{//���Ǳ���Ҫ���ܵ���Ϣ
			return true;
		}

		BYTE cbNowStatus=pUserStatus->cbUserStatus;
		if (cbNowStatus<US_SIT)
		{//�û��뿪
			pUserGameInfo->pIGameClient->OnEventUserLeave();
		}
		else
		{//�û�״̬����
			pUserGameInfo->pIGameClient->OnEventUserStatus(pUserStatus->wTableID, pUserStatus->wChairID, pUserStatus->cbUserStatus);
		}
	}

	return true;
}


//�û�����
bool CGameServer::OnSocketSubScore(strUserGameInfo* pUserGameInfo, void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize>=sizeof(CMD_GR_UserScore));
	if (wDataSize<sizeof(CMD_GR_UserScore))
	{
		return false;
	}

	//��������
	CMD_GR_UserScore * pUserScore=(CMD_GR_UserScore *)pBuffer;
	if (NULL != pUserScore)
	{
		if (pUserGameInfo->dwUserID != pUserScore->dwUserID)
		{//���Ǳ���Ҫ���ܵ���Ϣ
			return true;
		}

		IPC_UserScore UserScore;
		memset(&UserScore,0,sizeof(UserScore));
		UserScore.dwUserID=pUserScore->dwUserID;
		UserScore.UserScore=pUserScore->UserScore;
		UserScore.lLoveliness = pUserScore->lLoveliness;
		pUserGameInfo->pIGameClient->OnEventUserScore(&UserScore);
	}

	return true;
}


//������Ϣ
bool CGameServer::OnSocketMainInfo(WORD wSocketID, CMD_Command Command, void * pBuffer, WORD wDataSize)
{
	strUserGameInfo* pUserGameInfo = m_pGameUserInfo->m_vecUserGameInfo[wSocketID];
	if (NULL == pUserGameInfo)
	{
		return true;
	}
	ASSERT(Command.wMainCmdID==MDM_GR_INFO);
	switch (Command.wSubCmdID)
	{
	case SUB_GR_SERVER_INFO:	//������Ϣ
		{
			const Local_GR_ServerInfo* pLocalInfo = m_pGameUserInfo->GetLocalRoomInfo();
			if (pLocalInfo->bIsInit) 
			{//�ѳ�ʼ�����ˡ�
				return true;
			}

			//Ч������
			ASSERT(wDataSize>=sizeof(CMD_GR_ServerInfo));
			if (wDataSize<sizeof(CMD_GR_ServerInfo))
			{
				return false;
			}

			//��Ϣ����
			CMD_GR_ServerInfo * pServerInfo=(CMD_GR_ServerInfo *)pBuffer;
			if (NULL != pServerInfo)
			{
				m_pGameUserInfo->SetLocalRoomInfo(pServerInfo->wGameGenre, pServerInfo->wKindID, pServerInfo->wTableCount, pServerInfo->wChairCount);
			}
			return true;
		}
	case SUB_GR_COLUMN_INFO:	//�б����
		{
			return true;
		}
	case SUB_GR_CONFIG_FINISH:	//�������
		{
			return true;
		}
	}
	return true;
}

//״̬��Ϣ
bool CGameServer::OnSocketMainStatus(WORD wSocketID, CMD_Command Command, void * pBuffer, WORD wDataSize)
{
	strUserGameInfo* pUserGameInfo = m_pGameUserInfo->m_vecUserGameInfo[wSocketID];
	if (NULL == pUserGameInfo)
	{
		return true;
	}
	ASSERT(Command.wMainCmdID==MDM_GR_STATUS);
	switch (Command.wSubCmdID)
	{
	case SUB_GR_TABLE_INFO:		//������Ϣ
		{
			return true;
		}
	case SUB_GR_TABLE_STATUS:	//����״̬
		{
			return true;
		}
	}
	return true;
}

//ϵͳ��Ϣ
bool CGameServer::OnSocketMainSystem(WORD wSocketID, CMD_Command Command, void * pBuffer, WORD wDataSize)
{
	strUserGameInfo* pUserGameInfo = m_pGameUserInfo->m_vecUserGameInfo[wSocketID];
	if (NULL == pUserGameInfo)
	{
		return true;
	}
	ASSERT(Command.wMainCmdID==MDM_GR_SYSTEM);
	switch (Command.wSubCmdID)
	{
	case SUB_GR_MESSAGE:		//ϵͳ��Ϣ
		{
			//Ч�����
			CMD_GR_Message * pMessage=(CMD_GR_Message *)pBuffer;
			ASSERT(wDataSize>(sizeof(CMD_GR_Message)-sizeof(pMessage->szContent)));
			if (wDataSize<=(sizeof(CMD_GR_Message)-sizeof(pMessage->szContent)))
			{
				return false;
			}

			//��Ϣ����
			WORD wHeadSize=sizeof(CMD_GR_Message)-sizeof(pMessage->szContent);
			ASSERT(wDataSize==(wHeadSize+pMessage->wMessageLength*sizeof(TCHAR)));
			if (wDataSize!=(wHeadSize+pMessage->wMessageLength*sizeof(TCHAR))) 
			{
				return false;
			}

			//��Ϣ����
			pMessage->szContent[pMessage->wMessageLength-1]=0;

			bool bIntermet=false;
			if (pMessage->wMessageType&SMT_INTERMIT_LINE) 
			{//�ر�����
				bIntermet=true;
			}
			else if (pMessage->wMessageType&SMT_CLOSE_ROOM)
			{//�رշ���
				bIntermet=true;
			}

			if (bIntermet) 
			{
			}
			return true;
		}
	case SUB_GR_LOGON_AFFICHE:	//ϵͳ����
		{
			return true;
		}
	}
	return true;
}

//������Ϣ
bool CGameServer::OnSocketMainServerInfo(WORD wSocketID, CMD_Command Command, void * pBuffer, WORD wDataSize)
{
	strUserGameInfo* pUserGameInfo = m_pGameUserInfo->m_vecUserGameInfo[wSocketID];
	if (NULL == pUserGameInfo)
	{
		return true;
	}
	ASSERT(Command.wMainCmdID==MDM_GR_SERVER_INFO);
	switch (Command.wSubCmdID)
	{
	case SUB_GR_ONLINE_COUNT_INFO:		//������Ϣ
		{
			return true;
		}
	}

	return true;
}

//��Ϸ��Ϣ
bool CGameServer::OnSocketMainGameFrame(WORD wSocketID, CMD_Command Command, void * pBuffer, WORD wDataSize)
{
	strUserGameInfo* pUserGameInfo = m_pGameUserInfo->m_vecUserGameInfo[wSocketID];
	if (NULL == pUserGameInfo)
	{
		return true;
	}

	if (0==wDataSize)
	{//������
		pUserGameInfo->pIGameClient->OnSocketMainGameFrame(Command, NULL, 0);
	}
	else
	{
		pUserGameInfo->pIGameClient->OnSocketMainGameFrame(Command, pBuffer, wDataSize);
	}
	
	return true;
}



// CGameServer ��Ϣ�������


int CGameServer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	return 0;
}

void CGameServer::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (IDI_TIMER_GAME_SERVER_LOOK == nIDEvent)
	{
		m_LogonInfo.dwTCPConnectNums = 0;
		m_LogonInfo.dwUserConnectNums = 0;
		//int iCountConnect = 0;
		std::map<DWORD, strUserGameInfo*>::iterator iter = m_pGameUserInfo->m_vecUserGameInfo.begin();
		for (; iter!=m_pGameUserInfo->m_vecUserGameInfo.end(); ++iter)
		{
			if (NULL==(*iter).second) continue;
			if (((*iter).second)->dwCountConnect >= I_CONNECT_NUMS) continue;
			if (TAG_LINK_SUC == (*iter).second->dwLogonLinkState && TAG_LINK_FAIL==(*iter).second->dwGameLinkState)
			{//��¼�ɹ��ˣ�����Ϸ��������û�е�¼�ɹ���
				if (0 != (*iter).second->dwUserID)
				{
					if (m_LogonInfo.dwTCPConnectNums>=GAME_CONNECT_ONCE_NUMS)
					{
						continue;
					}
					if (0!=(*iter).second->dwGameServerIP && 0!=(*iter).second->dwGameServerport)
					{
						if (UserCanLogonGame((*iter).second->llUserGems))
						{
							//�������
							if (NULL == ((*iter).second)->TcpSocketHelper.GetInterface())
							{
								((*iter).second)->TcpSocketHelper.CreateInstance();
							}
							IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IUnknownEx);
							((*iter).second)->TcpSocketHelper->SetTCPSocketSink(pIUnknownEx);
							((*iter).second)->TcpSocketHelper->Connect((*iter).second->dwGameServerIP,(*iter).second->dwGameServerport);
							(*iter).second->dwGameLinkState = TAG_LINKING;
							++m_LogonInfo.dwTCPConnectNums;
							//++iCountConnect;
						}
					}
				}
			}
			else if (TAG_LINK_SUC==(*iter).second->dwGameLinkState)
			{//TCP�ѵ�¼����Ϸ������
				if (USER_LOGON_SERVER_FAIL==(*iter).second->dwUserGameState)
				{//�û���¼ʧ��.
					if (m_LogonInfo.dwUserConnectNums>=GAME_CONNECT_ONCE_NUMS)
					{
						continue;
					}
					UserLogonToGameServer((*iter).second);
					++m_LogonInfo.dwUserConnectNums;
					//++iCountConnect;
				}
			}
			if (m_LogonInfo.dwTCPConnectNums>=GAME_CONNECT_ONCE_NUMS || m_LogonInfo.dwUserConnectNums>=GAME_CONNECT_ONCE_NUMS)
			{
				break;
			}
		}
		m_LogonInfo.dwLogonFailNums -= m_LogonInfo.dwUserConnectNums;
		if (NULL != m_pMainWnd)
		{
			m_pMainWnd->UpdateGameConnInfo();
		}
		//if (0 == iCountConnect)
		//{
		//	KillTimer(IDI_TIMER_GAME_SERVER_LOOK);
		//}
	}
	__super::OnTimer(nIDEvent);
}

//����������ָ��
void CGameServer::SetMainWnd(CFlashEnterDlg* pMainWnd)
{
	if (NULL != pMainWnd)
	{
		m_pMainWnd = pMainWnd;
	}
}

//������Ӧֵ
void CGameServer::GetLogonInfo(DWORD &dwLogonSucNums, DWORD &dwLogonFailNums, DWORD &dwTCPCoonectNums, DWORD &dwUserConnectNums)
{
	dwLogonSucNums = m_LogonInfo.dwLogonSucNums;
	dwLogonFailNums = m_LogonInfo.dwLogonFailNums;
	dwTCPCoonectNums = m_LogonInfo.dwTCPConnectNums;
	dwUserConnectNums = m_LogonInfo.dwUserConnectNums;
}

void CGameServer::GetQueueInfo(DWORD &dwQueueSucNums, DWORD &dwQueueFailNums)
{
	dwQueueSucNums = m_dwQueueSuccNums;
	dwQueueFailNums = m_dwQueueFailNums;
}

//��������Ŷ�
void CGameServer::SetAllUserQueue()
{
	m_dwQueueFailNums = 0;
	m_dwQueueSuccNums = 0;
	std::map<DWORD, strUserGameInfo*>::iterator iter = m_pGameUserInfo->m_vecUserGameInfo.begin();
	for (; iter!=m_pGameUserInfo->m_vecUserGameInfo.end(); ++iter)
	{
		if (TAG_LINK_SUC == (*iter).second->dwGameLinkState && USER_LOGON_SERVER_SUC==(*iter).second->dwUserGameState)
		{
			((*iter).second)->TcpSocketHelper->SendData(MDM_GR_USER,SUB_GR_AUTO_QUEUE);
		}
	}
	if (NULL != m_pMainWnd)
	{
		m_pMainWnd->UpdateQueueInfo();
	}
}

//�˳���Ϸ������
bool CGameServer::OnEventExitServer()
{
	KillTimer(IDI_TIMER_GAME_SERVER_LOOK);
	m_pGameUserInfo->CloseAllSocket();
	::memset(&m_LogonInfo, 0, sizeof(m_LogonInfo));
	m_dwQueueSuccNums = 0;
	m_dwQueueFailNums = 0;
	if (NULL != m_pMainWnd)
	{
		m_pMainWnd->UpdateGameInfo();
		m_pMainWnd->UpdateGameConnInfo();
	}
	return true;
}