#include "gameclient.h"

//��ʱ��
//���׼��
#define				IDI_GT_GAME_USER_READY					3001
//�����ע
#define				IDI_GT_GAME_USER_FIRSTXZ				3002
//��Ҽ�ע
#define				IDI_GT_GAME_USER_JIAZHU					3003
//��ҿ���
#define				IDI_GT_GAME_USER_LOOKCARD				3004


//ʱ����
#define				TIME_GAME_USER_READY_INTERVAL			5
#define				TIME_GAME_USER_FIRSTXZ_INTERVAL			2
#define				TIME_GAME_USER_JIAZHU_INTERVAL			4
#define				TIME_GAME_USER_LOOKCARD_INTERVAL		1



BEGIN_MESSAGE_MAP(CGameClient, CGameClientFrame)
	ON_WM_TIMER()
END_MESSAGE_MAP()



CGameClient::CGameClient(void)
{
	InitGameFrame();
	::srand((unsigned int)::time(NULL));
}

CGameClient::~CGameClient(void)
{
}


//��ʼ����
bool CGameClient::InitGameFrame()
{
	//��ǰ�û�
	m_wCurrentUser = INVALID_CHAIR;
	//ׯ���û�
	m_wBankerUser = INVALID_CHAIR;
	//����Ӯ��
	m_wCurrentUser = INVALID_CHAIR;
	//��Ҹ���
	m_wPlayerNums = 0;
	//�ⶥ��Ŀ
	m_lMaxScore = 0;
	//��Ԫ����
	m_lMaxCellScore = 0;
	//��Ԫ��ע
	m_lCellScore = 0;
	//��ǰ����
	m_lCurrentTimes = 1;
	//������
	m_lUserMaxScore = 0;
	//��ע����
	::memset(m_wUserXZCount, 0, sizeof(m_wUserXZCount));
	//�����עֵ
	::memset(m_lTableScore, 0, sizeof(m_lTableScore));
	//�Ƿ���
	::memset(m_bIsLookCard, 0, sizeof(m_bIsLookCard));
	//�Ƿ���Ч���
	::memset(m_cbPlayStatus, 0, sizeof(m_cbPlayStatus));
	//�������
	::memset(m_byUserCard, 0, sizeof(m_byUserCard));
	//���Ƶ�����б�
	m_vecLookCardUserList.clear();
	//���ƻ��������б�
	m_vecLostUserList.clear();
	m_vecWinUserList.clear();
	m_vecCancelUserList.clear();
	//���Ӹ�ע����
	m_wContinuouFollowNums = 0;
	//�����Ժ󲻲���
	m_bIsLookCardLaterNoOperation = false;
	//��������
	m_bIsFollowToOpenCard = false;
	//����Ƿ��ж���
	m_bIsUserAction = false;
	::srand((unsigned int)::time(NULL));
	return true;
}

//���ÿ��
void CGameClient::ResetGameFrame()
{
	InitGameFrame();
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
	case IDI_GT_GAME_USER_FIRSTXZ:
		{//��һ����ע
			if (0==nElapse|| nElapse>100)
			{
				FirstXZ(true);
			}
			break;
		}
	case IDI_GT_GAME_USER_JIAZHU:
		{//��ע��ʱ��
			if (0==nElapse|| nElapse>100)
			{
				AutoJudge();
			}
			break;
		}
	case IDI_GT_GAME_USER_LOOKCARD:
		{//���ƶ�ʱ��
			if (0==nElapse|| nElapse>100)
			{
				LookCardLaterOpera();
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
	case GS_FREE:	//����״̬
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
	case SUB_S_GAME_START:	//��Ϸ��ʼ
		{
			bRet = OnSubGameStart(pBuffer,wDataSize);
			break;
		}
	case SUB_S_GIVE_UP:		//�û�����
		{
			bRet = OnSubGiveUp(pBuffer, wDataSize);
			break;
		}
	case SUB_S_ADD_SCORE:	//�û���ע
		{
			bRet = OnSubAddScore(pBuffer,wDataSize);
			break;
		}
	case SUB_S_LOOK_CARD:	//������Ϣ
		{
			bRet = OnSubLookCard(pBuffer,wDataSize);
			break;
		}
	case SUB_S_COMPARE_CARD:	//������Ϣ
		{
			bRet = OnSubCompareCard(pBuffer, wDataSize);
			break;
		}
	case SUB_S_OPEN_CARD:	//������Ϣ
		{
			bRet = OnSubOpenCard(pBuffer, wDataSize);
			break;
		}
	case SUB_S_WAIT_COMPARE:	//�ȴ�����
		{
			bRet = true;
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
	return true;
}


//����֮ǰ��ʼ��
void CGameClient::InitGameData()
{
	//��ʼ������
	InitGameFrame();
	//ȡ�����ж�ʱ��
	CancelAllTimer();
}


//��Ϸ��ʼ
bool CGameClient::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GameStart))
	{
		return false;
	}

	//��ʼ����Ϸ����
	InitGameData();

	CMD_S_GameStart *pGameStart=(CMD_S_GameStart *)pBuffer;
	if (NULL != pGameStart)
	{
		//����״̬
		SetGameStatus(GS_PLAYING);

		//��Ԫ��ע
		m_lCellScore=pGameStart->lCellScore;
		//��Ԫ����
		m_lMaxCellScore=pGameStart->lMaxScore;
		//��ǰ����
		m_lCurrentTimes=pGameStart->lCurrentTimes;
		//��ǰ�û�
		m_wCurrentUser=pGameStart->wCurrentUser;
		//ׯ���û�
		m_wBankerUser=pGameStart->wBankerUser;
		//������
		m_lUserMaxScore=pGameStart->lUserMaxScore;
		//��Ϸ��Ҹ���
		for (int i=0; i<GAME_PLAYER; i++)
		{
			m_cbPlayStatus[i] = false;
			m_bIsLookCard[i] = false;
			if (pGameStart->bPlayStatus[i])
			{
				++m_wPlayerNums;	//��Ҹ���
				m_cbPlayStatus[i] = true; //��Чλ
				m_bIsLookCard[i] = false; //û�п���
				m_lTableScore[i] = m_lCellScore;//Ĭ���µ�ע
			}
		}

		//��һ�������ע
		WORD wMeChairID = GetMeChairID();
		if (wMeChairID>=0 && wMeChairID<GAME_PLAYER)
		{
			if (wMeChairID == m_wCurrentUser)
			{
				int iRand = rand()%9;
				if (iRand<0 || iRand>=8)
				{
					iRand = 8;
				}
				SetGameTimer(IDI_GT_GAME_USER_FIRSTXZ, TIME_GAME_USER_FIRSTXZ_INTERVAL+iRand);
			}
		}

	}
	return true;
}

//�û�����
bool CGameClient::OnSubGiveUp(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_GiveUp))
	{
		return false;
	}

	KillGameTimer(IDI_GT_GAME_USER_FIRSTXZ);
	KillGameTimer(IDI_GT_GAME_USER_JIAZHU);
	KillGameTimer(IDI_GT_GAME_USER_LOOKCARD);

	CMD_S_GiveUp *pGiveUp=(CMD_S_GiveUp *)pBuffer;
	if (NULL != pGiveUp)
	{
		if (pGiveUp->wGiveUpUser>=0 && pGiveUp->wGiveUpUser<GAME_PLAYER)
		{
			m_cbPlayStatus[pGiveUp->wGiveUpUser]=false;
			m_vecCancelUserList.push_back(pGiveUp->wGiveUpUser);
			BYTE byMeChairID = GetMeChairID();
			if (pGiveUp->wGiveUpUser == byMeChairID)
			{
				SetGameStatus(GS_FREE);
			}

			//����Ƿ��ж���
			m_bIsUserAction = true;
		}
		
	}
	return true;
}

//�û���ע
bool CGameClient::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	if (wDataSize!=sizeof(CMD_S_AddScore))
	{
		return false;
	}

	KillGameTimer(IDI_GT_GAME_USER_FIRSTXZ);
	KillGameTimer(IDI_GT_GAME_USER_JIAZHU);
	KillGameTimer(IDI_GT_GAME_USER_LOOKCARD);

	CMD_S_AddScore *pAddScore=(CMD_S_AddScore *)pBuffer;
	if (NULL != pAddScore)
	{
		if (pAddScore->wAddScoreUser>=GAME_PLAYER)
		{
			return false;
		}
		//��ǰ����
		m_lCurrentTimes=pAddScore->lCurrentTimes;
		//��ǰ�û�
		m_wCurrentUser=pAddScore->wCurrentUser;
		//��ע���
		m_lTableScore[pAddScore->wAddScoreUser]+=pAddScore->lAddScoreCount;
		//��¼��ע����
		++m_wUserXZCount[pAddScore->wAddScoreUser];

		WORD wMeChairID = GetMeChairID();
		if (wMeChairID>=0 && wMeChairID<GAME_PLAYER)
		{
			//�ؼ���Ϣ
			if(!pAddScore->wCompareState && m_cbPlayStatus[wMeChairID] && wMeChairID==m_wCurrentUser)
			{
				int iRand = rand()%7;
				if (iRand<0 || iRand>=7)
				{
					iRand = 7;
				}
				//������ע����ע�����ƣ����ƣ�������ע��:ׯ���ڵ�һ��û��עֻ�ܸ��ϼұ��ơ�
				SetGameTimer(IDI_GT_GAME_USER_JIAZHU, TIME_GAME_USER_JIAZHU_INTERVAL+iRand);
			}
		}
	}
	return true;
}

//�û�����
bool CGameClient::OnSubLookCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_LookCard))
	{
		return false;
	}

	KillGameTimer(IDI_GT_GAME_USER_FIRSTXZ);
	KillGameTimer(IDI_GT_GAME_USER_JIAZHU);
	KillGameTimer(IDI_GT_GAME_USER_LOOKCARD);

	CMD_S_LookCard * pLookCard=(CMD_S_LookCard *)pBuffer;
	if (NULL != pLookCard)
	{
		if (pLookCard->wLookCardUser>=0 && pLookCard->wLookCardUser<GAME_PLAYER)
		{
			m_bIsLookCard[pLookCard->wLookCardUser] = true;

			//����Ƿ��ж���
			m_bIsUserAction = true;

			BYTE byMeChairID = GetMeChairID();
			if (pLookCard->wLookCardUser == byMeChairID)
			{//�����Լ���������
				::memcpy(m_byUserCard, pLookCard->cbCardData, sizeof(m_byUserCard));

				if (m_bIsLookCardLaterNoOperation && m_cbPlayStatus[byMeChairID] && byMeChairID==m_wCurrentUser)
				{
					//������ע����ע�����ƣ�������ע��:ׯ���ڵ�һ��û��עֻ�ܸ��ϼұ��ơ�
					int iRand = rand()%5;
					if (iRand<0 || iRand>=5)
					{
						iRand = 5;
					}
					SetGameTimer(IDI_GT_GAME_USER_LOOKCARD, TIME_GAME_USER_LOOKCARD_INTERVAL+iRand);
				}
			}
			else
			{
				//����¼�Լ�����
				m_vecLookCardUserList.push_back(pLookCard->wLookCardUser);
			}
		}
	}
	return true;
}

//�û�����
bool CGameClient::OnSubCompareCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_CompareCard))
	{
		return false;
	}

	KillGameTimer(IDI_GT_GAME_USER_FIRSTXZ);
	KillGameTimer(IDI_GT_GAME_USER_JIAZHU);
	KillGameTimer(IDI_GT_GAME_USER_LOOKCARD);

	CMD_S_CompareCard * pCompareCard=(CMD_S_CompareCard *)pBuffer;
	if (NULL != pCompareCard)
	{
		BYTE byMeChairID = GetMeChairID();
		//��¼���
		if (pCompareCard->wLostUser>=0 && pCompareCard->wLostUser<GAME_PLAYER)
		{
			m_cbPlayStatus[pCompareCard->wLostUser]=false;
			m_vecLostUserList.push_back(pCompareCard->wLostUser);
			if (pCompareCard->wLostUser == byMeChairID)
			{
				SetGameStatus(GS_FREE);
			}
		}
		//��¼Ӯ��,���ܻ��¼��Ρ�
		if (pCompareCard->wCompareUser[0]!=pCompareCard->wLostUser)
		{
			m_vecWinUserList.push_back(pCompareCard->wCompareUser[0]);
		}
		else if (pCompareCard->wCompareUser[1]!=pCompareCard->wLostUser)
		{
			m_vecWinUserList.push_back(pCompareCard->wCompareUser[1]);
		}

		//����Ƿ��ж���
		m_bIsUserAction = true;

		//��ǰ�û�
		m_wCurrentUser=pCompareCard->wCurrentUser;
		if (m_cbPlayStatus[byMeChairID] && byMeChairID==m_wCurrentUser)
		{
			int iRand = rand()%7;
			if (iRand<0 || iRand>=7)
			{
				iRand = 7;
			}
			//������ע����ע�����ƣ����ƣ�������ע��:ׯ���ڵ�һ��û��עֻ�ܸ��ϼұ��ơ�
			SetGameTimer(IDI_GT_GAME_USER_JIAZHU, TIME_GAME_USER_JIAZHU_INTERVAL+iRand);
		}
	}
	return true;
}

//�û�����
bool CGameClient::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_OpenCard)) 
	{
		return false;
	}

	//ȡ�����ж�ʱ��
	CancelAllTimer();

	CMD_S_OpenCard* pOpenCard=(CMD_S_OpenCard *)pBuffer;
	if (NULL != pOpenCard)
	{
		//ʤ���û�
		m_wWinnerUser=pOpenCard->wWinner;
	}
	return true;
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
		SetGameStatus(GS_FREE);

		//������������
		InitGameData();
		
		//��ʼ׼��
		int iRand = rand()%10+5;
		if (iRand<0 || iRand>=15)
		{
			iRand = 15;
		}
		SetGameTimer(IDI_GT_GAME_USER_READY, iRand);
	}
	return true;
}


//ȡ�����ж�ʱ��
void CGameClient::CancelAllTimer()
{
	KillGameTimer(IDI_GT_GAME_USER_READY);
	KillGameTimer(IDI_GT_GAME_USER_FIRSTXZ);
	KillGameTimer(IDI_GT_GAME_USER_JIAZHU);
	KillGameTimer(IDI_GT_GAME_USER_LOOKCARD);
}


void CGameClient::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CGameClientFrame::OnTimer(nIDEvent);
}



//��һ����ע
void CGameClient::FirstXZ(bool bIsFirst)
{
	WORD wMeChairID = GetMeChairID();
	if (wMeChairID>=0 && wMeChairID<GAME_PLAYER)
	{
		if (wMeChairID == m_wCurrentUser)
		{
			//�ܲ��ܿ���
			bool bIsLook = false;
			if (!m_bIsLookCard[wMeChairID])
			{
				BYTE byValue = rand()%10;
				if (3==byValue || 7==byValue)
				{
					//���Կ���
					//������Ϣ
					SendData(SUB_C_LOOK_CARD,NULL,0);
					bIsLook = true;
					m_bIsLookCard[wMeChairID] = true;
				}
			}

			if (bIsLook)
			{//�����Ժ󲻲���
				m_bIsLookCardLaterNoOperation = false;
			}
			if (bIsFirst)
			{//��һ����ע
				//������Ϣ
				CMD_C_AddScore AddScore;
				AddScore.wState=0;
				AddScore.lScore=bIsLook?(2*m_lCellScore):(m_lCellScore);
				SendData(SUB_C_ADD_SCORE,&AddScore,sizeof(CMD_C_AddScore));
			}
			else
			{//��ע
				FollowCard();
			}
		}
	}
}

//��Ҳ���
void CGameClient::MeOperator()
{
	////������ע����ע�����ƣ����ƣ�������ע��:ׯ���ڵ�һ��û��עֻ�ܸ��ϼұ��ơ�
	WORD wMeChairID = GetMeChairID();
	if (wMeChairID>=GAME_PLAYER || !m_cbPlayStatus[wMeChairID] || wMeChairID!=m_wCurrentUser)
	{
		return;
	}

	int iCountNums = 0;
	for (int i=0; i<GAME_PLAYER; i++)
	{
		if (m_cbPlayStatus[i])
		{
			++iCountNums;
		}
	}
	if (iCountNums<= 1)
	{
		return;
	}
	//���������ƴ���Ϊ0.
	m_wContinuouFollowNums = 0;
	//�����Ժ󲻲���
	m_bIsLookCardLaterNoOperation = false;
	//����Ƿ��ж���
	m_bIsUserAction = false;
	//��������
	m_bIsFollowToOpenCard = false;

	//�жϿɲ����Կ���
	int iLookCount = m_vecLookCardUserList.size();
	int iLostCount = m_vecLostUserList.size();
	int iWinCount = m_vecWinUserList.size();
	int iCancelCount = m_vecCancelUserList.size();
	bool bIsSendLookMsg = false;
	if (!m_bIsLookCard[wMeChairID])
	{//����û�п��ƣ���Ҫ�ж�һ��Ҫ��Ҫ���ơ�
		if (iLookCount>0)
		{
			if (m_wPlayerNums<=3)
			{
				bIsSendLookMsg = true;
			}
			else if (iLookCount>=2)
			{
				bIsSendLookMsg = true;
			}
			else
			{
				if (m_vecLookCardUserList[0]>=0 && m_vecLookCardUserList[0]<GAME_PLAYER)
				{
					if (m_cbPlayStatus[m_vecLookCardUserList[0]])
					{
						if (m_wUserXZCount[m_vecLookCardUserList[0]]>=5)
						{//���Ƶ����������ע������ϣ����Կ����ˡ�
							bIsSendLookMsg = true;
						}
						else if (iWinCount>0)
						{
							int iCountWins = 0;
							for (int i=0; i<iWinCount; i++)
							{
								if (m_vecWinUserList[i] == m_vecLookCardUserList[0])
								{
									++iCountWins;
								}
							}
							if (iCountWins>=1)
							{
								bIsSendLookMsg = true;
							}
						}
					}
				}
			}
		}
		else
		{
			if (iLostCount==0 && iCancelCount==0)
			{//û�˷���Ҳû����.Ҳû�˿�,ֱ�Ӹ���
				if (m_wUserXZCount[wMeChairID]<=3)
				{//��û��ע,�������,��ע
					FirstXZ(false);
					return;
				}
				else if (m_wUserXZCount[wMeChairID]<10)
				{//���һ��ֵ����ע.
					m_wContinuouFollowNums = 2+rand()%2;
				}
				else if (m_wUserXZCount[wMeChairID]>=10)
				{
					bIsSendLookMsg = true;
				}
			}
			if (!bIsSendLookMsg && iLostCount>0)
			{
				if (m_wPlayerNums<=3)
				{
					bIsSendLookMsg = true;
				}
				else if (iLostCount>=2)
				{
					bIsSendLookMsg = true;
				}
			}
			if (!bIsSendLookMsg && iCancelCount>0)
			{
				if (m_wPlayerNums<=3)
				{
					bIsSendLookMsg = true;
				}
				else if (iCancelCount>=2)
				{
					bIsSendLookMsg = true;
				}
			}
		}

		if (bIsSendLookMsg)
		{//���ʿ���
			m_bIsLookCardLaterNoOperation = true;
			//������Ϣ
			SendData(SUB_C_LOOK_CARD,NULL,0);
		}
		else
		{//����ж�
			int iRandValue = rand()%10;
			if (m_wContinuouFollowNums || iRandValue<=6)
			{//��ע
				FollowCard();
			}
			else
			{//����
				UserCompareCard();
			}
		}
	}
	else
	{//���Ʋ���
		LookCardLaterOpera();
	}
}


//�Ƿ���Կ���
bool CGameClient::IsOpenCard()
{
	WORD wMeChairID = GetMeChairID();
	if (wMeChairID>=GAME_PLAYER || wMeChairID!=m_wCurrentUser)
	{
		return false;
	}

	int iCountNums = 0;
	for (int i=0; i<GAME_PLAYER; i++)
	{
		if (m_cbPlayStatus[i])
		{
			++iCountNums;
		}
	}
	if (iCountNums<= 1)
	{
		return false;
	}

	//�ж��Ƿ���Կ���
	LONGLONG lTemp=(m_bIsLookCard[wMeChairID])?2:1;
	if((m_lUserMaxScore-m_lTableScore[wMeChairID]) < (m_lMaxCellScore*lTemp))
	{//���Կ���

		if (m_lCurrentTimes<=0)
		{
			m_lCurrentTimes = 1;
		}
		LONGLONG lCurrentScore=(m_bIsLookCard[wMeChairID])?(m_lCurrentTimes*m_lCellScore*4):(m_lCurrentTimes*m_lCellScore*2);
		CMD_C_AddScore AddScore;
		AddScore.wState=(2==iCountNums)?1:2;
		AddScore.lScore = lCurrentScore;
		SendData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

		//����Ƕ�����ң���ֱ�ӱ��ơ�
		if (2 == iCountNums)
		{
			CMD_C_CompareCard CompareCard;
			CompareCard.wCompareUser = INVALID_CHAIR;
			//�����ϼ�
			BYTE iCompareUserID = (GAME_PLAYER+wMeChairID-1)%GAME_PLAYER;
			for (int i=0; i<GAME_PLAYER; i++)
			{
				BYTE byTemplateUserID = (GAME_PLAYER+iCompareUserID-i)%GAME_PLAYER;
				if (m_cbPlayStatus[byTemplateUserID])
				{
					CompareCard.wCompareUser=byTemplateUserID;
					break;
				}
			}
			//������Ϣ
			SendData(SUB_C_COMPARE_CARD,&CompareCard,sizeof(CMD_C_CompareCard));
		}
		else
		{//������ж���������ң���ֱ�ӿ��ơ�
			//������Ϣ
			SendData(SUB_C_OPEN_CARD,NULL,0);
		}
		return true;
	}

	return false;
}


//���Ʋ���
void CGameClient::UserCompareCard()
{
	WORD wMeChairID = GetMeChairID();
	if (wMeChairID>=GAME_PLAYER || wMeChairID!=m_wCurrentUser)
	{
		return;
	}

	int iCountNums = 0;
	std::vector<BYTE> vecCompareUser;
	vecCompareUser.clear();
	for (int i=0; i<GAME_PLAYER; i++)
	{
		if (m_cbPlayStatus[i])
		{
			if (i!=wMeChairID)
			{
				vecCompareUser.push_back(i);
			}
			++iCountNums;
		}
	}
	if (iCountNums<= 1 || vecCompareUser.size()<=0)
	{
		return;
	}

	if (m_lCurrentTimes<=0)
	{
		m_lCurrentTimes = 1;
	}

	LONGLONG lCurrentScore=(m_bIsLookCard[wMeChairID])?(m_lCurrentTimes*m_lCellScore*4):(m_lCurrentTimes*m_lCellScore*2);
	CMD_C_AddScore AddScore;
	AddScore.wState=1;
	AddScore.lScore = lCurrentScore;
	SendData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));


	//ע��:ׯ���ڵ�һ��û��עֻ�ܸ��ϼұ��ơ�
	CMD_C_CompareCard CompareCard;
	BYTE iCompareUserID = INVALID_CHAIR;
	if ((wMeChairID==m_wBankerUser && 0==m_wUserXZCount[wMeChairID]) || (2==iCountNums))
	{
		CompareCard.wCompareUser = INVALID_CHAIR;
		//�����ϼ�
		BYTE iCompareUserID = (GAME_PLAYER+wMeChairID-1)%GAME_PLAYER;
		for (int i=0; i<GAME_PLAYER; i++)
		{
			BYTE byTemplateUserID = (GAME_PLAYER+iCompareUserID-i)%GAME_PLAYER;
			if (m_cbPlayStatus[byTemplateUserID])
			{
				CompareCard.wCompareUser=byTemplateUserID;
				break;
			}
		}
		if (CompareCard.wCompareUser == INVALID_CHAIR)
		{
			CompareCard.wCompareUser = vecCompareUser[0];
		}
	}
	else
	{//���һ����ҽ��б���
		int iRandValue = rand()%vecCompareUser.size();
		if (iRandValue>=0 && iRandValue<vecCompareUser.size())
		{
			CompareCard.wCompareUser = vecCompareUser[iRandValue];
		}
		else
		{
			CompareCard.wCompareUser = vecCompareUser[0];
		}
	}
	//������Ϣ
	SendData(SUB_C_COMPARE_CARD,&CompareCard,sizeof(CMD_C_CompareCard));
}

//��ȡ������������
int CGameClient::GetCardMaxPoint()
{
	//ע��A:14
	int iPoint = 0;
	for (int i=0; i<MAX_COUNT; i++)
	{
		if (0 != m_byUserCard[i])
		{
			int iTempPoint = m_byUserCard[i]&LOGIC_MASK_VALUE;
			if (1 == iTempPoint)
			{
				iTempPoint = 14;
			}
			if (iTempPoint>iPoint)
			{
				iPoint = iTempPoint;
			}
		}
		
	}
	return iPoint;
}

//���ƺ�Ĳ���
void CGameClient::LookCardLaterOpera()
{
	WORD wMeChairID = GetMeChairID();
	if (wMeChairID>=GAME_PLAYER || wMeChairID!=m_wCurrentUser)
	{
		return;
	}

	//���Ӹ�ע����
	m_wContinuouFollowNums = 0;
	//��������
	m_bIsFollowToOpenCard = false;
	//����Ƿ��ж���
	m_bIsUserAction = false;

	//�����������
	bool bIsCancel = false;
	BYTE byCardType = m_GameLogic.GetCardType(m_byUserCard, MAX_COUNT);
	int iCardMaxPoint = GetCardMaxPoint();
	if (0 == iCardMaxPoint)
	{//����
		bIsCancel = true;
	}
	else if (CT_SINGLE==byCardType && iCardMaxPoint<10)
	{//���ǵ��Ų�����������С��10�㣬��ֱ�ӷ�����
		bIsCancel = true;
	}
	if (!bIsCancel)
	{//��һ������
		if (CT_SINGLE==byCardType)
		{
			m_wContinuouFollowNums = 1+rand()%2;
		}
		else if (CT_DOUBLE==byCardType)
		{
			if (iCardMaxPoint<=10)
			{
				m_wContinuouFollowNums = 2+rand()%3;
			}
			else
			{
				m_wContinuouFollowNums = 4+rand()%4;
			}
		}
		else if ((CT_SHUN_ZI<=byCardType) && (iCardMaxPoint<=10))
		{
			m_wContinuouFollowNums = 5+rand()%6;
		}
		else if (byCardType>=CT_SHUN_ZI)
		{//��������
			m_bIsFollowToOpenCard = true;
		}

		//��ע
		FollowCard();
	}
	else
	{//����
		//������Ϣ
		SendData(SUB_C_GIVE_UP,NULL,0);
	}
}

//��ע
void CGameClient::FollowCard()
{
	WORD wMeChairID = GetMeChairID();
	if (wMeChairID>=GAME_PLAYER || wMeChairID!=m_wCurrentUser)
	{
		return;
	}
	if (m_lCurrentTimes<=0)
	{
		m_lCurrentTimes = 1;
	}
	CMD_C_AddScore AddScore;
	AddScore.wState=0;
	AddScore.lScore=m_lCellScore*m_lCurrentTimes;
	if (m_bIsLookCard[wMeChairID])
	{
		AddScore.lScore*=2;
	}
	SendData(SUB_C_ADD_SCORE,&AddScore,sizeof(CMD_C_AddScore));
}

//�Զ��ж�
void CGameClient::AutoJudge()
{
	//1:�����жϿɲ����Կ���
	bool bIsOpenCard = IsOpenCard();
	if (bIsOpenCard)
	{//����
		return;
	}

	//2:�ж��Ƿ���Ը�����
	if (m_bIsFollowToOpenCard)
	{//��������
		FollowCard();
		return;
	}

	//3:�ж��Ƿ񻹿������Ӹ�ע
	if (!m_bIsUserAction && m_wContinuouFollowNums>0)
	{//������
		--m_wContinuouFollowNums;
		if (0==m_wContinuouFollowNums)
		{//�������Ĵ����Ѿ�Ϊ���ˣ����ڿ��Ա����ˡ�
			UserCompareCard();
		}
		else
		{
			FollowCard();
		}
		return;
	}

	//4:�жϲ���
	MeOperator();
}
