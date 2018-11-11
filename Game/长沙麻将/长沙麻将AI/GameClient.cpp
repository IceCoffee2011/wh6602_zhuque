#include "GameClient.h"
#include "AndroidAI.h"

//��ʱ��
#define				IDI_GT_GAME_USER_READY					1000					//���׼��
#define				IDI_OPERATE_CARD						1001					//������ʱ��
#define				IDI_GT_GAME_USER_NO_OUTCARD				1002					//��Ҳ�����ʱ��

#define				TIME_OPERATE_CARD						4						//������ʱ��
#define				TIME_LESS								2						//����ʱ��
#define				TIME_GAME_USER_NO_OUTCARD_INTERVAL		30						//������ʱ��

BEGIN_MESSAGE_MAP(CGameClient, CGameClientFrame)
	ON_WM_TIMER()
END_MESSAGE_MAP()


CGameClient::CGameClient(void)
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	return;
}

CGameClient::~CGameClient(void)
{
}

//��ʼ����
bool CGameClient::InitGameFrame()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;
	m_cbLeftCardCount=0;
	m_cbOutCardData=0;
	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	return true;
}

//���ÿ��
void CGameClient::ResetGameFrame()
{
}

//ȡ�����ж�ʱ��
void CGameClient::CancelAllTimer()
{
	KillGameTimer(IDI_GT_GAME_USER_READY);
	KillGameTimer(IDI_OPERATE_CARD);
	KillTimer(IDI_GT_GAME_USER_NO_OUTCARD);
}

//����֮ǰ��ʼ��
void CGameClient::InitSendCard()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;
	m_cbLeftCardCount=0;
	m_cbOutCardData=0;
	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	CancelAllTimer();//ɾ����ʱ��
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
	case IDI_OPERATE_CARD:		//������ʱ��
		{
			if (nElapse==0)
			{
				KillGameTimer(IDI_OPERATE_CARD);
				tagOutCardResult OutCardResult;
				WORD wMeChairId = GetMeChairID();

				try
				{
					//��������
					if( SearchOutCard(OutCardResult) )
					{
						if( OutCardResult.cbOperateCode != WIK_NULL )
						{
							//Ч��
							ASSERT( OutCardResult.cbOperateCode&m_cbActionMask );
							if( !(OutCardResult.cbOperateCode&m_cbActionMask) ) throw 0;

							//��Ӧ����
							OnOperateCard(OutCardResult.cbOperateCode,OutCardResult.cbOperateCard);
						}
						else
						{
							//Ч��
							ASSERT( m_cbCardIndex[wMeChairId][m_GameLogic.SwitchToCardIndex(OutCardResult.cbOperateCard)] > 0 );
							if( m_cbCardIndex[wMeChairId][m_GameLogic.SwitchToCardIndex(OutCardResult.cbOperateCard)] == 0 ) throw 0;

							//����
							OnOutCard(OutCardResult.cbOperateCard);
						}
					}else
					{
						//Ч��
						ASSERT( wMeChairId != m_wCurrentUser );
						if( wMeChairId == m_wCurrentUser ) throw 0;

						//��Ӧ����
						OnOperateCard(WIK_NULL,0);
					}
				}catch(...)
				{
					ASSERT(FALSE);
					//�쳣����
					if( wMeChairId == m_wCurrentUser )
					{
						BYTE cbRand = rand()%MAX_INDEX;
						for( BYTE j = 0; j < MAX_INDEX; j++ )
						{
							BYTE i = (cbRand+j)%MAX_INDEX;
							if( m_cbCardIndex[wMeChairId][i] > 0 )
							{
								OnOutCard( m_GameLogic.SwitchToCardData(i) );
								return true;
							}
						}
					}
					else OnOperateCard( WIK_NULL,0 );
				}
			}
			break;
		}
	}
	return true;
}

//��ʱ��
void CGameClient::OnTimer(UINT nIDEvent)
{
	CGameClientFrame::OnTimer(nIDEvent);
	switch (nIDEvent)
	{
	case IDI_GT_GAME_USER_NO_OUTCARD:
		{
			KillTimer(IDI_GT_GAME_USER_NO_OUTCARD);
			if (m_wCurrentUser == GetMeChairID())
			{
				BYTE cbRand = rand()%MAX_INDEX;
				for( BYTE j = 0; j < MAX_INDEX; j++ )
				{
					BYTE i = (cbRand+j)%MAX_INDEX;
					if( m_cbCardIndex[m_wCurrentUser][i] > 0 )
					{
						OnOutCard( m_GameLogic.SwitchToCardData(i) );
						break;
					}
				}
			}
			break;
		}
	default:
		break;
	}
}

//��Ϸ����
bool CGameClient::OnGameSceneMessage(BYTE cbGameStatus, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_MJ_FREE:	//����״̬
		{
			SetGameTimer(IDI_GT_GAME_USER_READY,rand()%TIME_OPERATE_CARD+TIME_LESS);
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
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			bRet = OnSubGameStart(pBuffer,wDataSize);
			break;
		}
	case SUB_S_OUT_CARD:		//�û�����
		{
			bRet = OnSubOutCard(pBuffer,wDataSize);
			break;
		}
	case SUB_S_SEND_CARD:		//������Ϣ
		{
			bRet = OnSubSendCard(pBuffer,wDataSize);
			break;
		}
	case SUB_S_OPERATE_NOTIFY:	//������ʾ
		{
			bRet = OnSubOperateNotify(pBuffer,wDataSize);
			break;
		}
	case SUB_S_OPERATE_RESULT:	//�������
		{
			bRet = OnSubOperateResult(pBuffer,wDataSize);
			break;
		}
	case SUB_S_OPERATE_HAI_DI:	//���ײ���
		{
			bRet = OnSubOperateHaiDi(pBuffer,wDataSize);
			break;
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			bRet = OnSubGameEnd(pBuffer,wDataSize);
			break;
		}
	case SUB_S_TRUSTEE:			//�û��й�
		{
			bRet = true;
			break;
		}
	}

	return true;
}

//��Ϸ��ʼ
bool CGameClient::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart))
	{
		return false;
	}
	
	//��ʼ��
	InitSendCard();

	//��������
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	if (NULL != pGameStart)
	{
		BYTE byMeChairID = GetMeChairID();
		//���ñ���
		m_wBankerUser=pGameStart->wBankerUser;					//ׯ���û�
		m_wCurrentUser=pGameStart->wCurrentUser;				//��ǰ�����û�
		m_cbActionMask=pGameStart->cbUserAction;				//��������
		m_cbLeftCardCount=MAX_REPERTORY-GAME_PLAYER*(MAX_COUNT-1)-1;
		//������Ϣ
		m_wOutCardUser=INVALID_CHAIR;							//�����û�
		ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));	//����������
		ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));	//����������

		//����˿�
		ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
		ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

		//�����˿�
		BYTE cbCardCount=(byMeChairID==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
		m_GameLogic.SwitchToCardIndex(pGameStart->cbCardData,cbCardCount,m_cbCardIndex[byMeChairID]);

		/*
		BYTE bIndex = 0;
		for( WORD i = 0; i < GAME_PLAYER; i++ )
		{
			if( i == byMeChairID ) continue;
			cbCardCount=(i==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
			m_GameLogic.SwitchToCardIndex(&pGameStart->cbCardData[MAX_COUNT*bIndex++],cbCardCount,m_cbCardIndex[i]);
		}
		*/
		//��������
		if ((m_cbActionMask!=WIK_NULL) || m_wCurrentUser==byMeChairID)
		{
			UINT nElapse = rand()%TIME_OPERATE_CARD+TIME_LESS;
			SetGameTimer(IDI_OPERATE_CARD,nElapse);
			SetTimer(IDI_GT_GAME_USER_NO_OUTCARD, TIME_GAME_USER_NO_OUTCARD_INTERVAL, NULL);
			return true;
		}
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
		//������������
		InitSendCard();

		//��ʼ׼��
		int iTimeRand = rand()%4 + 1;
		SetGameTimer(IDI_GT_GAME_USER_READY, iTimeRand);
	}
	return true;
}

//���ײ���
bool CGameClient::OnSubOperateHaiDi(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	if (wDataSize!=sizeof(CMD_S_OperateHaiDi)) return false;

	//��Ϣ����
	CMD_S_OperateHaiDi * pOperateHaiDi=(CMD_S_OperateHaiDi *)pBuffer;

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;

	if (NULL != pOperateHaiDi && GetMeChairID()==pOperateHaiDi->wCurrentUser)
	{
		//������Ϣ
		CMD_C_OperateHaiDi OperateHaiDi;
		OperateHaiDi.cbTakeHaiDi=1;
		SendData(SUB_C_OPERATE_HAI_DI,&OperateHaiDi,sizeof(OperateHaiDi));
	}
	
	return true;
}

//����
void CGameClient::OnOutCard( BYTE cbOutCard )
{
	//ɾ����ʱ��
	KillGameTimer(IDI_OPERATE_CARD);

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	m_cbActionMask = WIK_NULL;
	m_cbActionCard = 0;

	//ɾ����
	m_GameLogic.RemoveCard(m_cbCardIndex[GetMeChairID()],cbOutCard);

	//��������
	CMD_C_OutCard OutCard;
	OutCard.cbCardData=cbOutCard;
	if( !SendData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard)) )
	{
		ASSERT( FALSE );
		return ;
	}

	return ;
}

//�û�����
bool CGameClient::OnSubOutCard( const void *pBuffer,WORD wDataSize )
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard)) return false;

	//��Ϣ����
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;

	if (NULL != pOutCard)
	{
		//��������
		WORD wMeChairID=GetMeChairID();

		//���ñ���
		m_wCurrentUser=INVALID_CHAIR;
		m_wOutCardUser=pOutCard->wOutCardUser;
		ASSERT( pOutCard->cbOutCardData != 0 );
		m_cbOutCardData=pOutCard->cbOutCardData;

		//ɾ���˿�
		if( wMeChairID != pOutCard->wOutCardUser )
			m_GameLogic.RemoveCard(m_cbCardIndex[pOutCard->wOutCardUser],pOutCard->cbOutCardData);
	}
	
	return true;
}

//�û�����
bool CGameClient::OnSubSendCard( const void *pBuffer,WORD wDataSize )
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//��������
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	if (NULL != pSendCard)
	{
		//���ñ���
		WORD wMeChairID=GetMeChairID();
		m_wCurrentUser=pSendCard->wCurrentUser;

		//�����˿�
		if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
		{
			//�����˿�
			m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]++] = m_cbOutCardData;
			//���ñ���
			m_cbOutCardData=0;
			m_wOutCardUser=INVALID_CHAIR;
		}

		//���ƴ���
		if (pSendCard->cbCardData!=0)
		{
			m_cbCardIndex[pSendCard->wCurrentUser][m_GameLogic.SwitchToCardIndex(pSendCard->cbCardData)]++;	
			//�۳��˿�
			m_cbLeftCardCount--;
		}

		//����ʱ��
		if( wMeChairID == m_wCurrentUser )
		{
			m_cbActionMask = pSendCard->cbActionMask;
			m_cbActionCard = pSendCard->cbCardData;

			//����ʱ��
			UINT nElapse=rand()%TIME_OPERATE_CARD+TIME_LESS;
			SetGameTimer(IDI_OPERATE_CARD,nElapse);
		}
	}
	
	return true;
}

//�������֪ͨ
bool CGameClient::OnSubOperateResult( const void *pBuffer,WORD wDataSize )
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult)) return false;

	//��Ϣ����
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;

	KillGameTimer( IDI_OPERATE_CARD );

	if (NULL != pOperateResult)
	{
		//��������
		BYTE cbPublicCard=TRUE;
		WORD wOperateUser=pOperateResult->wOperateUser;
		BYTE cbOperateCard[3]={pOperateResult->cbOperateCard,pOperateResult->cbOperateCard,pOperateResult->cbOperateCard};

		//���Ʊ���
		if (pOperateResult->cbOperateCode!=WIK_NULL)
		{
			m_cbOutCardData=0;
			m_wOutCardUser=INVALID_CHAIR;
		}

		//���ñ���
		m_cbActionMask = WIK_NULL;
		m_cbActionCard = 0;

		//�������
		if ((pOperateResult->cbOperateCode&WIK_GANG)!=0)
		{
			//���ñ���
			m_wCurrentUser=INVALID_CHAIR;

			//����˿�
			BYTE cbWeaveIndex=0xFF;
			for (BYTE i=0;i<m_cbWeaveCount[wOperateUser];i++)
			{
				BYTE cbWeaveKind=m_WeaveItemArray[wOperateUser][i].cbWeaveKind;
				BYTE cbCenterCard=m_WeaveItemArray[wOperateUser][i].cbCenterCard;
				if ((cbCenterCard==cbOperateCard[0])&&(cbWeaveKind==WIK_PENG))
				{
					cbWeaveIndex=i;
					m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
					m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
					m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser = pOperateResult->wProvideUser;
					break;
				}
			}

			//����˿�
			if (cbWeaveIndex==0xFF)
			{
				//�����ж�
				cbPublicCard=(pOperateResult->wProvideUser==wOperateUser)?FALSE:TRUE;

				//�����˿�
				cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=cbPublicCard;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
			}

			//�˿�����
			m_cbCardIndex[wOperateUser][m_GameLogic.SwitchToCardIndex(cbOperateCard[0])]=0;
		}
		else if (pOperateResult->cbOperateCode!=WIK_NULL&&pOperateResult->cbOperateCode!=WIK_CHI_HU)
		{
			//���ñ���
			m_wCurrentUser=pOperateResult->wOperateUser;

			//�������
			BYTE cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;

			//��Ͻ���
			BYTE cbWeaveCard[4]={0,0,0,0},cbWeaveKind=pOperateResult->cbOperateCode;
			BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbOperateCard[0],cbWeaveCard);

			//ɾ���˿�
			m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard[0],1);
			m_GameLogic.RemoveCard(m_cbCardIndex[wOperateUser],cbWeaveCard,cbWeaveCardCount-1);
		}

		//����ʱ��
		if (m_wCurrentUser==GetMeChairID())
		{
			//����ʱ��
			UINT nElapse=rand()%TIME_OPERATE_CARD+TIME_LESS;
			//����ʱ��
			SetGameTimer(IDI_OPERATE_CARD,nElapse);
		}
	}
	
	return true;
}

//����֪ͨ
bool CGameClient::OnSubOperateNotify( const void *pBuffer,WORD wDataSize )
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) return false;

	//��������
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

	//�û�����
	if (NULL != pOperateNotify)
	{
		if (pOperateNotify->cbActionMask!=WIK_NULL)
		{
			//��ȡ����
			WORD wMeChairID=GetMeChairID();
			m_cbActionMask = pOperateNotify->cbActionMask;
			m_cbActionCard = pOperateNotify->cbActionCard;

			//����ʱ��
			UINT nElapse = rand()%TIME_OPERATE_CARD+TIME_LESS;
			SetGameTimer(IDI_OPERATE_CARD,nElapse);
		}
	}
	return true;
}

//������
void CGameClient::OnOperateCard( BYTE cbOperateCode,BYTE cbOperateCard )
{
	//ɾ��ʱ��
	KillGameTimer(IDI_OPERATE_CARD);

	//���ñ���
	m_cbActionMask = WIK_NULL;
	m_cbActionCard = 0;

	//��������
	BYTE cbCard[3] = {cbOperateCard,0,0};

	//����ǳ���
	if( cbOperateCode & WIK_LEFT )
	{
		cbCard[1] = cbOperateCard+1;
		cbCard[2] = cbOperateCard+2;
	}
	else if( cbOperateCode & WIK_CENTER )
	{
		cbCard[1] = cbOperateCard-1;
		cbCard[2] = cbOperateCard+1;
	}
	else if( cbOperateCode & WIK_RIGHT )
	{
		cbCard[1] = cbOperateCard-2;
		cbCard[2] = cbOperateCard-1;
	}

	//��������
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=cbOperateCode;
	OperateCard.cbOperateCard=cbCard[0];
	//CopyMemory( OperateCard.cbOperateCard,cbCard,sizeof(cbCard) );
	if( !SendData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard)) )
	{
		ASSERT( FALSE );
		return ;
	}

	return ;
}

//��������
bool CGameClient::SearchOutCard( tagOutCardResult &OutCardResult )
{
	//��ʼ��
	ZeroMemory(&OutCardResult,sizeof(OutCardResult));

	WORD wMeChairId = GetMeChairID();
	ASSERT( wMeChairId != INVALID_CHAIR );
	if( wMeChairId == INVALID_CHAIR ) return false;

	//�жϺ�
	if( (m_cbActionMask&WIK_CHI_HU) != WIK_NULL )
	{
		OutCardResult.cbOperateCode = WIK_CHI_HU;
		OutCardResult.cbOperateCard = m_cbActionCard;
		return true;
	}

	//ת������
	BYTE byCard[MAX_COUNT],byCardCount = 0;
	for( BYTE i = 0; i < MAX_INDEX; i++ )
	{
		for( BYTE j = 0; j < m_cbCardIndex[wMeChairId][i]; j++ )
		{
			byCard[byCardCount++] = i;
		}
	}

	BYTE cbActionCard = m_cbActionCard;

	//������ֲ����÷�
	BYTE cbOperateCode[] = { WIK_TING,WIK_PENG,WIK_LEFT,WIK_CENTER,WIK_RIGHT,WIK_NULL };
	int nOperateScore[] = { 0,0,0,0,0,0 };
	//����ԭʼ��
	CAndroidAI AndroidAi;
	AndroidAi.SetEnjoinOutCard( NULL,0 );
	AndroidAi.SetCardData(byCard,byCardCount);
	AndroidAi.Think();
	int nOrgScore = AndroidAi.GetMaxScore();
	//�жϸ�
	if( m_cbActionMask & WIK_TING )
	{
		if( m_wCurrentUser == wMeChairId )
		{
			tagGangCardResult GangCardResult;
			m_GameLogic.AnalyseGangCard(m_cbCardIndex[wMeChairId],m_WeaveItemArray[wMeChairId],m_cbWeaveCount[wMeChairId],GangCardResult);

			//Ѱ����߷ָ���
			BYTE cbGangCard = 0;
			int nMaxGangScore = -1;
			for( BYTE i = 0; i < GangCardResult.cbCardCount; i++ )
			{
				ASSERT( m_cbCardIndex[wMeChairId][m_GameLogic.SwitchToCardIndex(GangCardResult.cbCardData[i])] > 0 );
				if( m_cbCardIndex[wMeChairId][m_GameLogic.SwitchToCardIndex(GangCardResult.cbCardData[i])] == 0 ) throw 0;

				//����ܺ�÷�
				AndroidAi.SetCardData(byCard,byCardCount);
				AndroidAi.SetAction(WIK_TING,m_GameLogic.SwitchToCardIndex(GangCardResult.cbCardData[i]));
				AndroidAi.Think();

				int nScore = AndroidAi.GetMaxScore();
				if( nScore > nMaxGangScore )
				{
					nMaxGangScore = nScore;
					cbGangCard = GangCardResult.cbCardData[i];
				}
			}

			ASSERT(nMaxGangScore!=-1&&cbGangCard!=0);
			cbActionCard = cbGangCard;
			nOperateScore[0] = nMaxGangScore-nOrgScore;
		}
		else
		{
			ASSERT( m_cbCardIndex[wMeChairId][m_GameLogic.SwitchToCardIndex(cbActionCard)] > 0 );
			if( m_cbCardIndex[wMeChairId][m_GameLogic.SwitchToCardIndex(cbActionCard)] == 0 ) throw 0;

			//����ܺ�÷�
			AndroidAi.SetCardData(byCard,byCardCount);
			AndroidAi.SetAction(WIK_GANG,m_GameLogic.SwitchToCardIndex(cbActionCard));
			AndroidAi.Think();
			nOperateScore[0] = AndroidAi.GetMaxScore()-nOrgScore;
		}
	}
	//�ж���
	if( m_cbActionMask & WIK_PENG )
	{
		AndroidAi.SetCardData(byCard,byCardCount);
		AndroidAi.SetAction(WIK_PENG,m_GameLogic.SwitchToCardIndex(cbActionCard));
		AndroidAi.Think();
		BYTE byBadlyIndex = AndroidAi.GetBadlyCard();
		if( byBadlyIndex == 0xff )
		{
			byBadlyIndex = AndroidAi.GetBadlyIn2Card();
			if( byBadlyIndex == 0xff )
				byBadlyIndex = AndroidAi.GetBadlyIn3Card();
		}
		if( byBadlyIndex != 0xff )
		{
			AndroidAi.RemoveCardData( byBadlyIndex );
			AndroidAi.Think();
			nOperateScore[1] = AndroidAi.GetMaxScore()-nOrgScore;
		}
	}
	//���
	if( m_cbActionMask & WIK_LEFT )
	{
		AndroidAi.SetCardData(byCard,byCardCount);
		AndroidAi.SetAction(WIK_LEFT,m_GameLogic.SwitchToCardIndex(cbActionCard));
		AndroidAi.Think();
		BYTE byBadlyIndex = AndroidAi.GetBadlyCard();
		if( byBadlyIndex == 0xff )
		{
			byBadlyIndex = AndroidAi.GetBadlyIn2Card();
			if( byBadlyIndex == 0xff )
				byBadlyIndex = AndroidAi.GetBadlyIn3Card();
		}
		if( byBadlyIndex != 0xff )
		{
			AndroidAi.RemoveCardData( byBadlyIndex );
			AndroidAi.Think();
			nOperateScore[2] = AndroidAi.GetMaxScore()-nOrgScore;
		}
	}
	//�г�
	if( m_cbActionMask & WIK_CENTER )
	{
		AndroidAi.SetCardData(byCard,byCardCount);
		AndroidAi.SetAction(WIK_CENTER,m_GameLogic.SwitchToCardIndex(cbActionCard));
		AndroidAi.Think();
		BYTE byBadlyIndex = AndroidAi.GetBadlyCard();
		if( byBadlyIndex == 0xff )
		{
			byBadlyIndex = AndroidAi.GetBadlyIn2Card();
			if( byBadlyIndex == 0xff )
				byBadlyIndex = AndroidAi.GetBadlyIn3Card();
		}
		if( byBadlyIndex != 0xff )
		{
			AndroidAi.RemoveCardData( byBadlyIndex );
			AndroidAi.Think();
			nOperateScore[3] = AndroidAi.GetMaxScore()-nOrgScore;
		}
	}
	//�ҳ�
	if( m_cbActionMask & WIK_RIGHT )
	{
		AndroidAi.SetCardData(byCard,byCardCount);
		AndroidAi.SetAction(WIK_RIGHT,m_GameLogic.SwitchToCardIndex(cbActionCard));
		AndroidAi.Think();
		BYTE byBadlyIndex = AndroidAi.GetBadlyCard();
		if( byBadlyIndex == 0xff )
		{
			byBadlyIndex = AndroidAi.GetBadlyIn2Card();
			if( byBadlyIndex == 0xff )
				byBadlyIndex = AndroidAi.GetBadlyIn3Card();
		}
		if( byBadlyIndex != 0xff )
		{
			AndroidAi.RemoveCardData( byBadlyIndex );
			AndroidAi.Think();
			nOperateScore[4] = AndroidAi.GetMaxScore()-nOrgScore;
		}
	}

	//��������
	BYTE cbOutCardData = cbActionCard;
	if( m_wCurrentUser == wMeChairId )
	{
		AndroidAi.SetCardData(byCard,byCardCount);
		AndroidAi.Think();
		BYTE byBadlyIndex = AndroidAi.GetBadlyCard();
		if( byBadlyIndex == 0xff )
		{
			byBadlyIndex = AndroidAi.GetBadlyIn2Card();
			if( byBadlyIndex == 0xff )
			{
				byBadlyIndex = AndroidAi.GetBadlyIn3Card();
				ASSERT( byBadlyIndex != 0xff );
				if( byBadlyIndex == 0xff ) throw 0;
			}
		}
		AndroidAi.RemoveCardData( byBadlyIndex );
		AndroidAi.Think();
		nOperateScore[5] = AndroidAi.GetMaxScore()-nOrgScore;
		cbOutCardData = m_GameLogic.SwitchToCardData(byBadlyIndex);
	}

	//��ȡ��߷ֲ���
	BYTE cbIndex = 0;
	for( BYTE i = 1; i < CountArray(nOperateScore); i++ )
	{
		if( nOperateScore[cbIndex] < nOperateScore[i] )
			cbIndex = i;
	}
	if( (cbOperateCode[cbIndex]&m_cbActionMask) && 
		( cbOperateCode[cbIndex]==WIK_GANG || nOperateScore[cbIndex] > 0 ) )
	{
		OutCardResult.cbOperateCode = cbOperateCode[cbIndex];
		OutCardResult.cbOperateCard = cbActionCard;
		return true;
	}
	else
	{
		if( m_wCurrentUser == wMeChairId )
		{
			OutCardResult.cbOperateCard = cbOutCardData;
			return true;
		}
		else return false;
	}

	ASSERT( FALSE );
	throw 0;
}

