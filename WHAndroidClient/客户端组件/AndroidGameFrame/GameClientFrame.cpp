// GameClient.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GameClientFrame.h"
#include "GameUserInfo.h"
#include ".\gameclientframe.h"

// CGameClient

IMPLEMENT_DYNAMIC(CGameClientFrame, CWnd)
CGameClientFrame::CGameClientFrame()
{
	::memset(&m_MeUserInfo, 0, sizeof(m_MeUserInfo));
	m_MeUserInfo.wChairID = -1;
	m_wSocketID = 0;
	m_wTimeID = 0;
	m_nElapseCount = 0;

	m_bGameStatus = GS_FREE;
}

CGameClientFrame::~CGameClientFrame()
{
}


BEGIN_MESSAGE_MAP(CGameClientFrame, CWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()



//����SocketID
void __cdecl CGameClientFrame::SetMeSocketID(WORD wSocketID)
{
	m_wSocketID = wSocketID;
}

//�Ƿ񴰿�
bool __cdecl CGameClientFrame::IsEnableWnd()
{
	return ::IsWindow(m_hWnd);
}

//�رմ���
void __cdecl CGameClientFrame::CloseWnd()
{
	SendMessage(WM_DESTROY, 0, 0);
}

// CGameClient ��Ϣ�������

bool CGameClientFrame::OnSocketMainGameFrame(CMD_Command Command, void * pBuffer, WORD wDataSize)
{
	bool bHandle=false;
	if (wDataSize==0)
	{
		bHandle=OnEventSocket(Command,NULL,0);
	}
	else
	{
		bHandle=OnEventSocket(Command,pBuffer,wDataSize);
	}

	//�����Ϣ
	if ((bHandle==false)&&(Command.wMainCmdID==MDM_GF_FRAME))
	{
		switch (Command.wSubCmdID)
		{
		case SUB_GF_OPTION:			//��Ϸ����
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(CMD_GF_Option));
				if (wDataSize!=sizeof(CMD_GF_Option)) return false;

				//��Ϣ����
				CMD_GF_Option * pOption=(CMD_GF_Option *)pBuffer;
				if (NULL != pOption)
				{
					m_bGameStatus=pOption->bGameStatus;
				}
				return true;
			}
		case SUB_GF_SCENE:			//��Ϸ����
			{
				return OnGameSceneMessage(m_bGameStatus,pBuffer,wDataSize);
			}
		default:
			break;
		}
	}
	
	return true;
}

//�¼�Socket
bool CGameClientFrame::OnEventSocket(CMD_Command Command, void * pBuffer, WORD wDataSize)
{
	switch (Command.wMainCmdID)
	{
	case MDM_GF_FRAME:	//�����Ϣ
		{
			return OnFrameMessage(Command.wSubCmdID,pBuffer,wDataSize);
		}
	case MDM_GF_GAME:	//��Ϸ��Ϣ
		{
			return OnGameMessage(Command.wSubCmdID,pBuffer,wDataSize);
		}
	}
	return false;
}

//�����Ϣ
bool CGameClientFrame::OnFrameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	return false;
}


//�û�����
void __cdecl CGameClientFrame::OnEventUserEnter(const tagUserData* pUserData)
{
	if (NULL != pUserData)
	{
		::memcpy(&m_MeUserInfo, pUserData, sizeof(tagUserData));
	}
}

//�û��뿪
void __cdecl CGameClientFrame::OnEventUserLeave()
{
}

//�û�����
void __cdecl CGameClientFrame::OnEventUserScore(const IPC_UserScore* pUserScore)
{
	if (NULL != pUserScore)
	{
		m_MeUserInfo.lLoveliness = pUserScore->lLoveliness;
		m_MeUserInfo.lMoney = pUserScore->UserScore.lMoney;
		m_MeUserInfo.lGameGold = pUserScore->UserScore.lGold;
		//m_MeUserInfo.lInsureScore = pUserScore->UserScore.lInsureScore;
		m_MeUserInfo.lWinCount = pUserScore->UserScore.lWinCount;
		m_MeUserInfo.lLostCount = pUserScore->UserScore.lLostCount;
		m_MeUserInfo.lDrawCount = pUserScore->UserScore.lDrawCount;
		m_MeUserInfo.lFleeCount = pUserScore->UserScore.lFleeCount;
		m_MeUserInfo.lExperience = pUserScore->UserScore.lExperience;
		m_MeUserInfo.lGem = pUserScore->UserScore.lGem;
		//m_MeUserInfo.lHonor = pUserScore->UserScore.lHonor;
		//m_MeUserInfo.lCredit = pUserScore->UserScore.lCredit;
		m_MeUserInfo.dwGrade = pUserScore->UserScore.dwGrade;
		//m_MeUserInfo.dwOrder = pUserScore->UserScore.dwOrder;
		//m_MeUserInfo.dwWinGemsToday = pUserScore->UserScore.dwWinGemsToday;
	}
}


//�û�״̬
void __cdecl CGameClientFrame::OnEventUserStatus(WORD wTableID, WORD wChairID, BYTE cbUserStatus)
{
	m_MeUserInfo.wTableID = wTableID;
	m_MeUserInfo.wChairID = wChairID;
	m_MeUserInfo.cbUserStatus = cbUserStatus;

	if (m_MeUserInfo.cbUserStatus < US_SIT)
	{
		//������Ϸ���
		ResetGameFrame();
		
		m_bGameStatus=US_FREE;
		m_wTimeID = 0;
		m_nElapseCount = 0;
	}
	else if (US_SIT == m_MeUserInfo.cbUserStatus)
	{//����״̬
		//��ȡ����
		CMD_GF_Info Info;
		ZeroMemory(&Info,sizeof(Info));
		Info.bAllowLookon=FALSE;
		SendData(MDM_GF_FRAME,SUB_GF_INFO,&Info,sizeof(CMD_GF_Info));
	}
}


//����Լ���Ϣ
const tagUserData* CGameClientFrame::GetMeData()
{
	return &m_MeUserInfo;
}

//�Լ�λ��
WORD CGameClientFrame::GetMeChairID()
{
	return m_MeUserInfo.wChairID;
}

//���ͺ���
bool CGameClientFrame::SendData(WORD wSubCmdID)
{
	CGameUserInfo* pGameUserInfo = CGameUserInfo::GetInstance();
	if (NULL != pGameUserInfo)
	{
		strUserGameInfo* pGameInfo = pGameUserInfo->m_vecUserGameInfo[m_wSocketID];
		if (NULL != pGameInfo)
		{
			pGameInfo->TcpSocketHelper->SendData(MDM_GF_GAME,wSubCmdID);
			return true;
		}
		//std::map<DWORD, strUserGameInfo*>::iterator iter = pGameUserInfo->m_vecUserGameInfo.find(m_wSocketID);
		//if (iter != pGameUserInfo->m_vecUserGameInfo.end())
		//{
		//	(*iter).second->TcpSocketHelper->SendData(MDM_GF_GAME,wSubCmdID);
		//	return true;
		//}
	}
	return false;
}

//���ͺ���
bool CGameClientFrame::SendData(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	CGameUserInfo* pGameUserInfo = CGameUserInfo::GetInstance();
	if (NULL != pGameUserInfo)
	{
		strUserGameInfo* pGameInfo = pGameUserInfo->m_vecUserGameInfo[m_wSocketID];
		if (NULL != pGameInfo)
		{
			pGameInfo->TcpSocketHelper->SendData(MDM_GF_GAME,wSubCmdID, pData, wDataSize);
			return true;
		}
		//std::map<DWORD, strUserGameInfo*>::iterator iter = pGameUserInfo->m_vecUserGameInfo.find(m_wSocketID);
		//if (iter != pGameUserInfo->m_vecUserGameInfo.end())
		//{
		//	(*iter).second->TcpSocketHelper->SendData(MDM_GF_GAME,wSubCmdID, pData, wDataSize);
		//	return true;
		//}
	}
	return false;
}

//���ͺ���
bool CGameClientFrame::SendData(WORD wMainCmdID, WORD wSubCmdID)
{
	CGameUserInfo* pGameUserInfo = CGameUserInfo::GetInstance();
	if (NULL != pGameUserInfo)
	{
		strUserGameInfo* pGameInfo = pGameUserInfo->m_vecUserGameInfo[m_wSocketID];
		if (NULL != pGameInfo)
		{
			pGameInfo->TcpSocketHelper->SendData(wMainCmdID,wSubCmdID);
		}
	}
	return false;
}
//���ͺ���
bool CGameClientFrame::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	CGameUserInfo* pGameUserInfo = CGameUserInfo::GetInstance();
	if (NULL != pGameUserInfo)
	{
		strUserGameInfo* pGameInfo = pGameUserInfo->m_vecUserGameInfo[m_wSocketID];
		if (NULL != pGameInfo)
		{
			pGameInfo->TcpSocketHelper->SendData(wMainCmdID,wSubCmdID, pData, wDataSize);
		}
	}
	return false;
}

//����׼��
bool CGameClientFrame::SendUserReady(void * pBuffer, WORD wDataSize)
{
	return SendData(MDM_GF_FRAME,SUB_GF_USER_READY, pBuffer, wDataSize);
}

//���ö�ʱ��
bool CGameClientFrame::SetGameTimer(UINT nTimerID, UINT nElapse)
{
	ASSERT(NULL!=m_hWnd);
	if (NULL==m_hWnd)
	{
		return false;
	}
	if (0!=nTimerID) KillTimer(m_wTimeID);
	if (nElapse > 0)
	{
		m_wTimeID = nTimerID;
		m_nElapseCount = nElapse;
		
		SetTimer(m_wTimeID, 1000, NULL);
	}
	
	return true;
}

//ɾ����ʱ��
bool CGameClientFrame::KillGameTimer(UINT nTimerID)
{
	if (0!=nTimerID && 0!=m_wTimeID && nTimerID==m_wTimeID)
	{
		KillTimer(nTimerID);
		m_wTimeID = 0;
		m_nElapseCount = 0;
	}
	return true;
}


void CGameClientFrame::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == m_wTimeID)
	{
		if (m_nElapseCount > 0)
		{
			--m_nElapseCount;
			if (m_nElapseCount<=0)
			{
				m_nElapseCount = 0;
			}
		}
		OnTimerMessage(m_nElapseCount, m_wTimeID);
		if (0==m_nElapseCount)
		{
			KillTimer(nIDEvent);
			m_wTimeID = 0;
			m_nElapseCount = 0;
		}
	}
	
	__super::OnTimer(nIDEvent);
}


//��Ϸ״̬
BYTE CGameClientFrame::GetGameStatus()
{
	return m_bGameStatus;
}

//��Ϸ״̬
void CGameClientFrame::SetGameStatus(BYTE bGameStatus)
{
	m_bGameStatus = bGameStatus;
}

int CGameClientFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	InitGameFrame();
	return 0;
}
