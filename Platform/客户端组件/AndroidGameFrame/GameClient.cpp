#include "gameclient.h"

#define _DDZ_ANDROID_DEBUG_

#ifdef _DDZ_ANDROID_DEBUG_
#include "FlashEnterDlg.h"
#include ".\gameclient.h"
#endif


//��ʱ��
//���׼��
#define				IDI_GT_GAME_USER_READY					3001
//��ҽз�
#define				IDI_GT_GAME_USER_JIAOFEN				3002
//��Ҽӱ�
#define				IDI_GT_GAME_USER_JIABEI					3003
//��ҳ���
#define				IDI_GT_GAME_USER_OUTCARD				3004
//��Ҳ�����ʱ��
#define				IDI_GT_GAME_USER_NO_OUTCARD				3010


//ʱ����
#define				TIME_GAME_USER_READY_INTERVAL			5
#define				TIME_GAME_USER_JIAOFEN_INTERVAL			5
#define				TIME_GAME_USER_JIABEI_INTERVAL			3
#define				TIME_GAME_USER_OUTCARD_INTERVAL			3
#define				TIME_GAME_USER_NO_OUTCARD_INTERVAL		12000



BEGIN_MESSAGE_MAP(CGameClient, CGameClientFrame)
	ON_WM_TIMER()
END_MESSAGE_MAP()



CGameClient::CGameClient(void)
{
	m_bBankerUserID = -1;
	m_bCurrentUserID = -1;
	m_bHandCardCount = 0;
	m_bJiaBeiTag = -1;
	m_cbCallScorePhase = CSD_NORMAL;
	::memset(m_bHandCardData, 0, sizeof(m_bHandCardData));
	m_bIsNewTurn = false;
	::srand((unsigned int)::time(NULL));
}

CGameClient::~CGameClient(void)
{
}


//��ʼ����
bool CGameClient::InitGameFrame()
{
	m_bBankerUserID = -1;
	m_bCurrentUserID = -1;
	m_bHandCardCount = 0;
	::memset(m_bHandCardData, 0, sizeof(m_bHandCardData));
	m_bJiaBeiTag = -1;
	m_cbCallScorePhase = CSD_NORMAL;
	m_bMeCanOutCard = false;
	m_bIsNewTurn = false;
	return true;
}

//���ÿ��
void CGameClient::ResetGameFrame()
{
	m_bBankerUserID = -1;
	m_bCurrentUserID = -1;
	m_bHandCardCount = 0;
	::memset(m_bHandCardData, 0, sizeof(m_bHandCardData));
	m_bJiaBeiTag = -1;
	m_cbCallScorePhase = CSD_NORMAL;
	m_bMeCanOutCard = false;
	m_bIsNewTurn = false;
}

//ϵͳ�¼�
//ʱ����Ϣ
bool CGameClient::OnTimerMessage(UINT nElapse, UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_GT_GAME_USER_READY:
		{//��Ϸ׼����ʱ��
			if (0==nElapse|| nElapse>100)
			{
				SendUserReady(NULL, 0);
			}
			break;
		}
	case IDI_GT_GAME_USER_JIAOFEN:
		{//�зֶ�ʱ��
			if (0==nElapse|| nElapse>100)
			{
				CMD_C_LandScore LandScore;
				if (m_GameLogic.GetLandTag())
				{
					LandScore.bLandScore=3;	//ֱ�ӽ�3��
				}
				else
				{
					LandScore.bLandScore = 255;	//����
				}
				SendData(SUB_C_LAND_SCORE,&LandScore,sizeof(LandScore));
			}
			break;
		}
	case IDI_GT_GAME_USER_OUTCARD:
		{//���ƶ�ʱ��
			if (0==nElapse|| nElapse>100)
			{
				GameUserAutoOutCard();
			}
			break;
		}
	}
	return true;
}


//��Ϸ����
bool CGameClient::OnGameSceneMessage(BYTE cbGameStatus, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_WK_FREE:	//����״̬
		{
			//��ʼ׼��
			int iTimeRand = rand()%5+2;
			SetGameTimer(IDI_GT_GAME_USER_READY, iTimeRand);

			break;
		}
	default:
		{
			break;
		}
	}
	return true;
}

//������Ϣ
bool CGameClient::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	bool bRet = false;
	switch (wSubCmdID)
	{
	case SUB_S_SEND_CARD:	//��Ϸ��ʼ����
		{
			bRet = OnSubSendCard(pBuffer,wDataSize);
			break;
		}
	case SUB_S_LAND_SCORE:	//�зֹ���
		{
			bRet = OnSubGameUserLandScore(pBuffer,wDataSize);
			break;
		}
	case SUB_S_DOUBLE_SCORE:	//�з����
		{
			bRet = OnSubJiaoFenFin(pBuffer,wDataSize);
			break;
		}
	case SUB_S_USER_DOUBLE:		//��Ҽӱ�
		{
			bRet = OnSubUserDoubleResult(pBuffer, wDataSize);
			break;
		}
	case SUB_S_GAME_START:	//��Ϸ��ʼ����
		{
			bRet = OnSubGameStartOutCard(pBuffer, wDataSize);
			break;
		}
	case SUB_S_OUT_CARD:		//�û����ƽ��
		{
			bRet = OnSubGameUserOutCardResult(pBuffer, wDataSize);
			break;
		}
	case SUB_S_PASS_CARD:		//��������
		{
			bRet = OnSubGameUserPassCard(pBuffer, wDataSize);
			break;
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			bRet = OnSubGameEnd(pBuffer,wDataSize);
			break;
		}
	default:
		{
			return true;
			break;
		}
	}
#ifdef _DDZ_ANDROID_DEBUG_
	if (!bRet)
	{
		CWnd* pMain = AfxGetMainWnd();
		if (NULL != pMain)
		{
			char chData[32]={0};
			::sprintf(chData, "��ϢID:%d ����ȷ����δ����", wSubCmdID);
			((CFlashEnterDlg*)pMain)->InsertListData(0, 0, chData);
		}
	}
#endif
	return true;
}


//����֮ǰ��ʼ��
void CGameClient::InitSendCard()
{
	m_bBankerUserID = -1;
	m_bCurrentUserID = -1;
	m_bHandCardCount = 0;
	::memset(m_bHandCardData, 0, sizeof(m_bHandCardData));
	m_bJiaBeiTag = -1;
	m_bMeCanOutCard = false;
	m_cbCallScorePhase = CSD_NORMAL;
	m_bIsNewTurn = false;

	//ȡ�����ж�ʱ��
	CancelAllTimer();
	
	BYTE btMeChairID = GetMeChairID();
	if (btMeChairID>=0 && btMeChairID<GAME_PLAYER)
	{
		m_GameLogic.InitLogicData(btMeChairID);
	}
}


//�����˿�
bool CGameClient::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendAllCard));
	if (wDataSize!=sizeof(CMD_S_SendAllCard))
	{
		return false;
	}

	//��ʼ������
	InitSendCard();

	//��������
	CMD_S_SendAllCard * pSendCard=(CMD_S_SendAllCard *)pBuffer;
	if (NULL != pSendCard)
	{
		WORD byMeChairID = GetMeChairID();
		for (BYTE i=0; i<GAME_PLAYER; i++)
		{
			if (i == byMeChairID)
			{
				m_bHandCardCount = 17;
				::memcpy(m_bHandCardData, pSendCard->bCardData[i], sizeof(BYTE)*m_bHandCardCount);
				break;
			}
		}
		if (m_bHandCardCount < 17)
		{
			return false;
		}

#ifdef _DDZ_ANDROID_DEBUG_
		//��ӡ������Ϣ
		CWnd* pMain = AfxGetMainWnd();
		if (NULL != pMain)
		{
			std::string strValue = "��������Ϊ: ";
			char chData[100] = {0};
			for (int i=0; i<m_bHandCardCount; i++)
			{
				::memset(chData, 0, sizeof(chData));
				::sprintf(chData, "%x,", m_bHandCardData[i]);
				strValue.append(chData);
			}
			::sprintf(chData, "%s", strValue.c_str());
			((CFlashEnterDlg*)pMain)->InsertListData(0, 0, chData);
		}
#endif
		m_GameLogic.SetHandCard(m_bHandCardData, m_bHandCardCount, false);

		//���浱ǰ���
		m_bCurrentUserID = pSendCard->wCurrentUser;

		if (byMeChairID == m_bCurrentUserID)
		{//������ǵ�ǰ��ң���ֱ�ӽз֡�
			int iTimeRand = rand()%5+1;
			SetGameTimer(IDI_GT_GAME_USER_JIAOFEN, iTimeRand);
		}
	}

	return true;
}

//�зֹ���
bool CGameClient::OnSubGameUserLandScore(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_LandScore));
	if (wDataSize!=sizeof(CMD_S_LandScore))
	{
		return false;
	}

	//��Ϣ����
	CMD_S_LandScore * pLandScore=(CMD_S_LandScore *)pBuffer;
	if (NULL != pLandScore)
	{
		KillGameTimer(IDI_GT_GAME_USER_JIAOFEN);

		//���ñ���
		m_cbCallScorePhase = pLandScore->bCallScorePhase;
		
		WORD wMeChairID = GetMeChairID();
		if (pLandScore->wCurrentUser==wMeChairID)
		{
			int iTimeRand = rand()%4+1;
			SetGameTimer(IDI_GT_GAME_USER_JIAOFEN, iTimeRand);
		}
	}
	return true;
}


//��Ҽӱ����
bool CGameClient::OnSubUserDoubleResult(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_UserDouble));
	if (wDataSize != sizeof(CMD_S_UserDouble)) 
	{
		return false;
	}

	KillTimer(IDI_GT_GAME_USER_JIABEI);
	//��������
	CMD_S_UserDouble *pUserDouble = ( CMD_S_UserDouble * )pBuffer;
	if (NULL != pUserDouble)
	{
		WORD byMeChairID = GetMeChairID();
		if (byMeChairID == pUserDouble->bNextUser)
		{//�ӱ�ѡ��
			bool bIsAddBei = false;
			if (byMeChairID == m_bBankerUserID)
			{
				if (m_GameLogic.GetLandTag())
				{
					bIsAddBei = true;
				}
			}
			else
			{
				int iTag = rand()%2;
				if (iTag)
				{
					bIsAddBei = true;
				}
			}
			if (bIsAddBei)
			{
				m_bJiaBeiTag = pUserDouble->bState;
				int iTimeRand = rand()%3+1;
				SetTimer(IDI_GT_GAME_USER_JIABEI, iTimeRand*1000, NULL);
			}
		}
	}
	return true;
}

//�з����
bool CGameClient::OnSubJiaoFenFin(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_DoubleScore));
	if (wDataSize != sizeof(CMD_S_DoubleScore))
	{
		return false;
	}

	KillGameTimer(IDI_GT_GAME_USER_JIAOFEN);
	m_bBankerUserID = -1;	//����
	m_bCurrentUserID = -1;	//��ǰ���
	m_bMeCanOutCard = false;

	//��������
	CMD_S_DoubleScore *pDoubleScore = ( CMD_S_DoubleScore * )pBuffer;
	if (NULL != pDoubleScore)
	{
		m_bBankerUserID = pDoubleScore->wLandUser;
		m_bCurrentUserID = pDoubleScore->wLandUser;

		WORD byMeChairID = GetMeChairID();
		if (byMeChairID == m_bBankerUserID)
		{//���ǵ���
			//�������ŵ���
			CopyMemory(&m_bHandCardData[17],pDoubleScore->bBackCard,sizeof(BYTE)*3);
			m_bHandCardCount+=3;
			m_GameLogic.SetHandCard(m_bHandCardData, m_bHandCardCount, true);
		}
		else if (byMeChairID == (pDoubleScore->wLandUser+1)%GAME_PLAYER)
		{//�������ׯ�ҵ���һ�ң����ҿ��Ե���
			int iTag = rand()%2;
			if (iTag)
			{
				m_bJiaBeiTag = 1;
				int iTimeRand = rand()%3+1;
				SetTimer(IDI_GT_GAME_USER_JIABEI, iTimeRand*1000, NULL);
			}
		}
	}

	return true;
}

//��Ϸ��ʼ����
bool CGameClient::OnSubGameStartOutCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart))
	{
		return false;
	}

	KillTimer(IDI_GT_GAME_USER_JIABEI);
	KillGameTimer(IDI_GT_GAME_USER_JIAOFEN);
	KillTimer(IDI_GT_GAME_USER_NO_OUTCARD);

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	if (NULL != pGameStart)
	{
		m_bIsNewTurn = true;
		m_bMeCanOutCard = false;
		m_bBankerUserID = pGameStart->wLandUser;
		m_bCurrentUserID = pGameStart->wLandUser;
		
#ifdef _DDZ_ANDROID_DEBUG_
		CWnd* pMain = AfxGetMainWnd();
		if (NULL != pMain)
		{
			char chData[100] = {0};
			::sprintf(chData, "��Ϸ��ʼ����; ����:%d", m_bBankerUserID);
			((CFlashEnterDlg*)pMain)->InsertListData(0, 0, chData);
		}
#endif
		m_GameLogic.SetLandTag(m_bCurrentUserID);
		m_GameLogic.SetWhoOutCard(m_bCurrentUserID, true);
		WORD byMeChairID = GetMeChairID();
		if (byMeChairID == m_bCurrentUserID)
		{//������ǵ�ǰ��ң�����ơ�
			m_bMeCanOutCard = true;

			int iTimeRand = rand()%6 + 1;
			SetGameTimer(IDI_GT_GAME_USER_OUTCARD, iTimeRand);

			SetTimer(IDI_GT_GAME_USER_NO_OUTCARD, TIME_GAME_USER_NO_OUTCARD_INTERVAL, NULL);

#ifdef _DDZ_ANDROID_DEBUG_
			CWnd* pMain = AfxGetMainWnd();
			if (NULL != pMain)
			{
				((CFlashEnterDlg*)pMain)->InsertListData(0, 0, TEXT("0��Ϸ��ʼ����״̬--�ñ��˳����ˣ�"));
			}
#endif
			
		}
	}

	return true;
}

//�Զ������һ����
void CGameClient::GameAutoLastOneCard()
{
	CMD_C_OutCard OutCard;
	::memset(&OutCard, 0, sizeof(CMD_C_OutCard));
	OutCard.bCardCount = 1;
	OutCard.bCardData[0] = m_bHandCardData[m_bHandCardCount];//�����һ����
	//���ͳ�����Ϣ
	SendData(SUB_C_OUT_CART,&OutCard,sizeof(CMD_C_OutCard));
	m_bMeCanOutCard = false;

#ifdef _DDZ_ANDROID_DEBUG_
	//��ӡ������Ϣ
	CWnd* pMain = AfxGetMainWnd();
	if (NULL != pMain)
	{
		std::string strValue = "2���ط��ͳ�������Ϊ: ";
		char chData[100] = {0};
		for (int i=0; i<OutCard.bCardCount; i++)
		{
			::memset(chData, 0, sizeof(chData));
			::sprintf(chData, "%x,", OutCard.bCardData[i]);
			strValue.append(chData);
		}
		::sprintf(chData, "%s", strValue.c_str());
		((CFlashEnterDlg*)pMain)->InsertListData(0, 0, chData);
	}
#endif
}

//�Զ�����
void CGameClient::GameUserAutoOutCard()
{
	if (m_bCurrentUserID != GetMeChairID() || !m_bMeCanOutCard)
	{//���ǵ�ǰ���
		return;
	}
	CMD_C_OutCard OutCard;
	::memset(&OutCard, 0, sizeof(CMD_C_OutCard));
	m_GameLogic.OutCard(OutCard.bCardData, OutCard.bCardCount);
	if (OutCard.bCardCount > 0 )
	{
		//���ͳ�����Ϣ
		SendData(SUB_C_OUT_CART,&OutCard,sizeof(CMD_C_OutCard));
		m_bMeCanOutCard = false;

#ifdef _DDZ_ANDROID_DEBUG_
		//��ӡ������Ϣ
		CWnd* pMain = AfxGetMainWnd();
		if (NULL != pMain)
		{
			std::string strValue = "1���ط��ͳ�������Ϊ: ";
			char chData[100] = {0};
			for (int i=0; i<OutCard.bCardCount; i++)
			{
				::memset(chData, 0, sizeof(chData));
				::sprintf(chData, "%x,", OutCard.bCardData[i]);
				strValue.append(chData);
			}
			::sprintf(chData, "%s", strValue.c_str());
			((CFlashEnterDlg*)pMain)->InsertListData(0, 0, chData);
		}
#endif
		
	}
	else
	{
		KillTimer(IDI_GT_GAME_USER_NO_OUTCARD);
		m_bMeCanOutCard = false;
		if (m_bIsNewTurn)
		{//�����ׯ�һ�Ӯ�ң����ܲ�����
			GameAutoLastOneCard();
		}
		else
		{
			//��������
			SendData(SUB_C_PASS_CARD);
#ifdef _DDZ_ANDROID_DEBUG_
			CWnd* pMain = AfxGetMainWnd();
			if (NULL != pMain)
			{
				((CFlashEnterDlg*)pMain)->InsertListData(0, 0, TEXT("����ѷ��Ͳ�����Ϣ!"));
			}
#endif
		}
	}
}


//��ҳ��ƽ��
bool CGameClient::OnSubGameUserOutCardResult(const void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard))
	{
		return false;
	}
	//��������
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;
	if (NULL != pOutCard)
	{
		KillGameTimer(IDI_GT_GAME_USER_OUTCARD);
		KillTimer(IDI_GT_GAME_USER_JIABEI);
		KillTimer(IDI_GT_GAME_USER_NO_OUTCARD);
		m_bMeCanOutCard = false;
		m_bIsNewTurn = false;

		WORD byMeChairID = GetMeChairID();
		if (byMeChairID == pOutCard->wOutCardUser)
		{//ɾ�����е���
			bool bIsSuc = m_GameLogic.RemoveCard(pOutCard->bCardData,pOutCard->bCardCount,m_bHandCardData,m_bHandCardCount);
			if (bIsSuc)
			{//ɾ���ɹ�.
				m_bHandCardCount-=pOutCard->bCardCount;
			}

#ifdef _DDZ_ANDROID_DEBUG_
			//��ӡ������Ϣ
			CWnd* pMain = AfxGetMainWnd();
			if (NULL != pMain)
			{
				std::string strValue = "�������������Ϣ: ";
				char chData[100] = {0};
				::memset(chData, 0, sizeof(chData));
				::sprintf(chData, "�������%d, ��ǰ���:%d, ", pOutCard->wOutCardUser, pOutCard->wCurrentUser);
				strValue.append(chData);

				for (int i=0; i<pOutCard->bCardCount; i++)
				{
					::memset(chData, 0, sizeof(chData));
					::sprintf(chData, "%x,", pOutCard->bCardData[i]);
					strValue.append(chData);
				}
				::memset(chData, 0, sizeof(chData));
				::sprintf(chData, "%s", strValue.c_str());
				((CFlashEnterDlg*)pMain)->InsertListData(0, 0, chData);
			}
#endif

		}
		m_GameLogic.OutCardResult(pOutCard->wOutCardUser, true, pOutCard->bCardData,pOutCard->bCardCount);

		//���õ�����
		SetGameUserJiaBei(pOutCard->wBankerUser, pOutCard->wCurrentUser, pOutCard->bCircuit, pOutCard->bIsDouble);

		//��¼��ǰ���
		m_bCurrentUserID = pOutCard->wCurrentUser;

		//��ǰ��Ҿ���Ҫ���Ƶ����
		if (pOutCard->wCurrentUser==pOutCard->wOutCardUser)
		{
			m_bIsNewTurn = true;
			m_GameLogic.SetWhoOutCard(pOutCard->wCurrentUser, true);
		}
		else
		{
			m_GameLogic.SetWhoOutCard(pOutCard->wCurrentUser, false);
		}
		
		if (byMeChairID == pOutCard->wCurrentUser)
		{//������ǵ�ǰ��ң�����ơ�
			int iTimeRand = rand()%8 + 1;
			SetGameTimer(IDI_GT_GAME_USER_OUTCARD, iTimeRand);

			SetTimer(IDI_GT_GAME_USER_NO_OUTCARD, TIME_GAME_USER_NO_OUTCARD_INTERVAL, NULL);

			m_bMeCanOutCard = true;
#ifdef _DDZ_ANDROID_DEBUG_
			CWnd* pMain = AfxGetMainWnd();
			if (NULL != pMain)
			{
				((CFlashEnterDlg*)pMain)->InsertListData(0, 0, TEXT("1����״̬--�ñ��˳����ˣ�"));
			}
#endif
		}
	}
	return true;
}


//��ҷ�������
bool CGameClient::OnSubGameUserPassCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_PassCard));
	if (wDataSize!=sizeof(CMD_S_PassCard))
	{
		return false;
	}
	
	CMD_S_PassCard * pPassCard=(CMD_S_PassCard *)pBuffer;
	if (NULL != pPassCard)
	{
		KillGameTimer(IDI_GT_GAME_USER_OUTCARD);
		KillTimer(IDI_GT_GAME_USER_JIABEI);
		KillTimer(IDI_GT_GAME_USER_NO_OUTCARD);
		m_bMeCanOutCard = false;
		m_bIsNewTurn = false;

#ifdef _DDZ_ANDROID_DEBUG_
		//��ӡ������Ϣ
		CWnd* pMain = AfxGetMainWnd();
		if (NULL != pMain)
		{
			std::string strValue = "���粻����������Ϣ: ";
			char chData[100] = {0};
			::memset(chData, 0, sizeof(chData));
			::sprintf(chData, "�������%d, ��ǰ���:%d, ", pPassCard->wPassUser, pPassCard->wCurrentUser);
			strValue.append(chData);
			::memset(chData, 0, sizeof(chData));
			::sprintf(chData, "%s", strValue.c_str());
			((CFlashEnterDlg*)pMain)->InsertListData(0, 0, chData);
		}
#endif

		m_GameLogic.OutCardResult(pPassCard->wPassUser, false, NULL,0);

		//���õ�ǰ���
		m_bCurrentUserID = pPassCard->wCurrentUser;

		//���õ�����
		SetGameUserJiaBei(pPassCard->wBankerUser, pPassCard->wCurrentUser, pPassCard->bCircuit, pPassCard->bIsDouble);

		//һ���ж�
		if (pPassCard->bNewTurn)
		{
			m_bIsNewTurn = true;
			m_GameLogic.SetWhoOutCard(pPassCard->wCurrentUser, true);
		}
		else
		{
			m_GameLogic.SetWhoOutCard(pPassCard->wCurrentUser, false);
		}

		WORD byMeChairID = GetMeChairID();
		if (byMeChairID == pPassCard->wCurrentUser)
		{//������ǵ�ǰ��ң�����ơ�
			int iTimeRand = rand()%8 + 1;
			SetGameTimer(IDI_GT_GAME_USER_OUTCARD, iTimeRand);

			SetTimer(IDI_GT_GAME_USER_NO_OUTCARD, TIME_GAME_USER_NO_OUTCARD_INTERVAL, NULL);

			m_bMeCanOutCard = true;
#ifdef _DDZ_ANDROID_DEBUG_
			CWnd* pMain = AfxGetMainWnd();
			if (NULL != pMain)
			{
				((CFlashEnterDlg*)pMain)->InsertListData(0, 0, TEXT("2����״̬--�ñ��˳����ˣ�"));
			}
#endif

		}
	}

	return true;
}

//�ӱ��ж�
void CGameClient::SetGameUserJiaBei(WORD wBankerID, WORD wCurrentID, const BYTE *pCircuit, const bool *pIsDouble)
{
	if (wBankerID>=GAME_PLAYER || wCurrentID>=GAME_PLAYER || NULL==pCircuit || NULL==pIsDouble)
	{
		return;
	}

	WORD byMeChairID = GetMeChairID();
	m_bJiaBeiTag = 0;
	if (wCurrentID==byMeChairID) 
	{//��һ��ң��������ʱ����һ����ң�
		if (byMeChairID==wBankerID)
		{//ׯ��
			if (1 == pCircuit[wCurrentID])
			{//���ƴ��� С��2
				if ((pIsDouble[(wBankerID +1)%GAME_PLAYER] 
				|| pIsDouble[(wBankerID + 2)%GAME_PLAYER]) 
					&& !pIsDouble[wBankerID])
				{//ũ���� "��"��"��" ����ׯ�һ�û��"����",���Է���
					m_bJiaBeiTag = 3;
				}
			}
		}
		else
		{//ũ��
			if (pCircuit[wCurrentID] < 1)
			{//���ƴ��� С��1
				if (wCurrentID == (wBankerID+1)%GAME_PLAYER)
				{//��һ��ũ��
					if (!pIsDouble[(wBankerID+1)%GAME_PLAYER])
					{//֮ǰû�е������Ե���
						m_bJiaBeiTag = 1;
					}
				}
				else if ((wBankerID+2)%GAME_PLAYER == wCurrentID)
				{//�ڶ���ũ��.
					if (pIsDouble[(wCurrentID+2)%GAME_PLAYER] 
					&& !pIsDouble[(wCurrentID)%GAME_PLAYER])
					{//�ӱ���,���Ը�.
						m_bJiaBeiTag = 2;
					}
					else if(!pIsDouble[(wCurrentID)%GAME_PLAYER])
					{//û�ӱ�,���Ե�.
						m_bJiaBeiTag = 1;
					}
				}
			}
		}
	}
	if (m_bJiaBeiTag>0)
	{//�ӱ�
		WORD byMeChairID = GetMeChairID();
		bool bIsAddBei = false;
		if (byMeChairID == m_bBankerUserID)
		{
			if (m_GameLogic.GetLandTag())
			{
				bIsAddBei = true;
			}
		}
		else
		{
			int iTag = rand()%2;
			if (iTag)
			{
				bIsAddBei = true;
			}
		}
		if (bIsAddBei)
		{
			KillTimer(IDI_GT_GAME_USER_JIABEI);
			int iTimeRand = rand()%3+1;
			SetTimer(IDI_GT_GAME_USER_JIABEI, iTimeRand*1000, NULL);
		}
	}
}

//��Ϸ����
bool CGameClient::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd))
	{
		return false;
	}

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	if (NULL != pGameEnd)
	{
		//������������
		InitSendCard();

		//��ʼ׼��
		int iTimeRand = rand()%4 + 1;
		SetGameTimer(IDI_GT_GAME_USER_READY, iTimeRand);
	}
	return true;
}


//ȡ�����ж�ʱ��
void CGameClient::CancelAllTimer()
{
	KillGameTimer(IDI_GT_GAME_USER_READY);
	KillGameTimer(IDI_GT_GAME_USER_JIAOFEN);
	KillGameTimer(IDI_GT_GAME_USER_OUTCARD);
	KillTimer(IDI_GT_GAME_USER_NO_OUTCARD);
	KillTimer(IDI_GT_GAME_USER_JIABEI);
}


void CGameClient::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CGameClientFrame::OnTimer(nIDEvent);

#ifdef _DDZ_ANDROID_DEBUG_
	CWnd* pMain = AfxGetMainWnd();
	if (NULL != pMain)
	{
		char chData[100]={0};
		::sprintf(chData, "����ʱ���¼�ID:%d", nIDEvent);
		((CFlashEnterDlg*)pMain)->InsertListData(0, 0, chData);
	}
#endif

	switch (nIDEvent)
	{
	case IDI_GT_GAME_USER_NO_OUTCARD:
		{
			KillTimer(IDI_GT_GAME_USER_NO_OUTCARD);
			if ((m_bMeCanOutCard) && (m_bCurrentUserID == GetMeChairID()))
			{
				if (m_bIsNewTurn)
				{//�����ׯ�һ�Ӯ�ң����ܲ�����
					GameAutoLastOneCard();
				}
				else
				{
					//��������
					m_bMeCanOutCard = false;
					SendData(SUB_C_PASS_CARD);

#ifdef _DDZ_ANDROID_DEBUG_
					CWnd* pMain = AfxGetMainWnd();
					if (NULL != pMain)
					{
						char chData[100]={0};
						::sprintf(chData, "����ʱ���¼�ID:%d,%d��ҳ�ʱ�ѷ��ͷ�����Ϣ", nIDEvent, GetMeChairID());
						((CFlashEnterDlg*)pMain)->InsertListData(0, 0, chData);
					}
#endif
				}
			}
			break;
		}
	case IDI_GT_GAME_USER_JIABEI:
		{//����������
			KillTimer(IDI_GT_GAME_USER_JIABEI);
			if (m_bJiaBeiTag>=1 && m_bJiaBeiTag<=3)
			{
				//������Ϣ
				CMD_C_DoubleScore DoubleScore;
				ZeroMemory( &DoubleScore, sizeof( DoubleScore ) );
				DoubleScore.bDoubleScore = true;
				//������Ϣ
				SendData( SUB_C_DOUBLE_SCORE, &DoubleScore, sizeof( DoubleScore ) );

#ifdef _DDZ_ANDROID_DEBUG_
				CWnd* pMain = AfxGetMainWnd();
				if (NULL != pMain)
				{
					char chData[100]={0};
					::sprintf(chData, "����ʱ���¼�ID:%d,��ʼ��������", nIDEvent);
					((CFlashEnterDlg*)pMain)->InsertListData(0, 0, chData);
				}
#endif
			}
			break;
		}
	default:
		break;
	}
}
