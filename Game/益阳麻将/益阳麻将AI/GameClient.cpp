#include "GameClient.h"
#include "AndroidAI.h"
#include "..\..\������\Include\GlobalChannel.h"

//��ʱ��
#define				IDI_GT_GAME_USER_READY					1000					//���׼��
#define				IDI_OPERATE_CARD						1001					//������ʱ��
#define				IDI_GT_GAME_USER_OUTCARD				1002					//��ҳ��ƶ�ʱ��
#define				IDI_USER_TING_CARD						1003					//���ƶ�ʱ��
#define				IDI_USER_HAI_DI_CARD					1004					//�û�������
#define             IDI_SZSTART                             1005                    //����ɸ�Ӷ�ʱ��


#define				TIME_GAME_READY							3						//׼��ʱ��
#define				TIME_OPERATE_CARD						2						//������ʱ��
#define				TIME_LESS								2						//����ʱ��
#define				TIME_GAME_USER_NO_OUTCARD_INTERVAL		5						//������ʱ��
#define				TIME_TING_CARD_INTERVAL					2						//���ƶ�ʱ��
#define				TIME_USER_HAI_DI_CARD_INTERVAL			2						//�����ƶ�ʱ��
#define             TIME_USER_SZ_INTERVAL                   5                       //����ɸ��ʱ��


BEGIN_MESSAGE_MAP(CGameClient, CGameClientFrame)
END_MESSAGE_MAP()


CGameClient::CGameClient(void)
{
	m_pRoomLog = NULL;		// ��־
	//��ʼ��
	InitData();
}

CGameClient::~CGameClient(void)
{
}

void CGameClient::InitSome(DWORD dwWndID)
{
		HRESULT hResult;
		hResult = ::CoCreateInstance(CLSID_DefLogMgr, NULL, CLSCTX_INPROC_SERVER, IID_ILogMgr, (void**)&m_pRoomLog);

		if(S_OK==hResult && m_pRoomLog)
		{
			CString strPath,strTmp; 
			GetModuleFileName(NULL, strPath.GetBufferSetLength(256), 256);
			strTmp.Format(".\\YYMJAndroidLog\\%d\\",dwWndID);
			strPath = strPath.Left(strPath.Find("AndroidLoad.exe")) + strTmp;//YYMJAndroid.dll

			m_pRoomLog->Init();
			m_pRoomLog->Start();
			m_pRoomLog->Register(strPath, 50000);
			m_pRoomLog->AddMsg(0, 0, TRUE,"***** �����齫��������־��ʼ����");
		}
}

void CGameClient::UninitSome()
{
	// ��־
	if (m_pRoomLog)
	{
		if (m_pRoomLog)
			m_pRoomLog->AddMsg(0, 0, TRUE,"***** �����齫��������־ֹͣ����");

		m_pRoomLog->Stop();
		m_pRoomLog->Uninit();
		m_pRoomLog = NULL; 
	}
}

//��ʼ��
void CGameClient::InitData()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;
	m_bHearStatus = false;
	m_bIsMeCanOutCard = false;

	::memset(m_byGangCard, 0, sizeof(m_byGangCard));
	::memset(m_byBuCard, 0, sizeof(m_byBuCard));
	m_byGangCardNums = 0;
	m_byBuCardNums = 0;

	//�˿˱���
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	m_TempKing = 0;
}

//��ʼ����
bool CGameClient::InitGameFrame()
{
	//��ʼ��
	InitData();

	return true;
}

//���ÿ��
void CGameClient::ResetGameFrame()
{
	//��ʼ��
	InitData();
}

//ȡ�����ж�ʱ��
void CGameClient::CancelAllTimer()
{
	KillGameTimer(IDI_GT_GAME_USER_READY);
	KillGameTimer(IDI_OPERATE_CARD);
	KillGameTimer(IDI_GT_GAME_USER_OUTCARD);
	KillGameTimer(IDI_USER_TING_CARD);
	KillGameTimer(IDI_USER_HAI_DI_CARD);
}

//����֮ǰ��ʼ��
void CGameClient::InitSendCard()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;
	m_bHearStatus = false;
	m_bIsMeCanOutCard = false;

	::memset(m_byGangCard, 0, sizeof(m_byGangCard));
	::memset(m_byBuCard, 0, sizeof(m_byBuCard));
	m_byGangCardNums = 0;
	m_byBuCardNums = 0;

	//�˿˱���
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	m_TempKing = 0;

	CancelAllTimer();//ɾ����ʱ��
}

//ϵͳ�¼�
//ʱ����Ϣ
bool CGameClient::OnTimerMessage(UINT nElapse, UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_SZSTART:     //����ɸ�Ӷ�ʱ��
		{
			if (0==nElapse)
			{
				SendData(SUB_C_SZSTART);
			}
			break;
		}
	case IDI_GT_GAME_USER_READY:
		{//��Ϸ׼����ʱ��
			if (0==nElapse|| nElapse>100)
			{
				SendUserReady(NULL, 0);
			}
			break;
		}
	case IDI_OPERATE_CARD:				//����
		{
			if (0 == nElapse)
			{
				BYTE byChairID = GetMeChairID();
				if (byChairID == m_wCurrentUser)
				{//��ʼ����
					OnUserOperateCard();
				}
			}
			break;
		}
	case IDI_GT_GAME_USER_OUTCARD:		//���ƶ�ʱ��
		{
			if (nElapse==0)
			{
				BYTE byChairID = GetMeChairID();
				if (byChairID == m_wCurrentUser)
				{//��ʼ����
					OnOutCard();
				}
			}
			break;
		}
	case IDI_USER_HAI_DI_CARD:			//������
		{
			if (0==nElapse)
			{
				BYTE byChairID = GetMeChairID();
				if (byChairID == m_wCurrentUser)
				{//������
					//������Ϣ
					CMD_C_OperateHaiDi OperateHaiDi;
					OperateHaiDi.cbTakeHaiDi=1;
					SendData(SUB_C_OPERATE_HAI_DI,&OperateHaiDi,sizeof(CMD_C_OperateHaiDi));
				}
			}
			break;
		}
	case IDI_USER_TING_CARD:			//�û�����
		{
			if (0==nElapse)
			{
				BYTE byChairID = GetMeChairID();
				if (byChairID == m_wCurrentUser)
				{//������
					CMD_UserTingOperate TingOperate;
					TingOperate.bTingCard = true;
					SendData(SUB_C_USERTINGCARD,&TingOperate,sizeof(TingOperate));
				}
			}
			break;
		}
	default:
		break;
	}
	char sz[100]={0};
	sprintf(sz,"CGameClient::OnTimerMessage nElapse=%d,nTimerID=%d",nElapse,nTimerID);
	if (m_pRoomLog)
		m_pRoomLog->AddMsg(0, 0, TRUE,sz);
	return true;
}

//��Ϸ����
bool CGameClient::OnGameSceneMessage(BYTE cbGameStatus, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_MJ_FREE:	//����״̬
		{
			WORD wTime = rand()%TIME_GAME_READY+TIME_LESS;
			SetGameTimer(IDI_GT_GAME_USER_READY,wTime);
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
	case SUB_C_SZSTART:			//����ɫ�Ӷ���
		{
			bRet = OnSubStartSZFlash();
			break;
		}
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
	case SUB_S_TING_CARD:		//���Ʊ��
		{
			bRet =  OnSubTingCard(pBuffer,wDataSize);
			break;
		}
	case SUB_S_USER_TINGCARD_RESULT:	//���ƵĽ��
		{
			bRet =  OnSubTingCardResult(pBuffer, wDataSize);
			break;
		}
	default:
		break;
	}

	return true;
}

//�Ƿ����Ʋ�����
bool CGameClient::bIsTingCardOpera()
{
	BYTE byChairID = GetMeChairID();
	if (m_wCurrentUser!=byChairID)
	{
		return false;
	}
	if (m_bHearStatus)
	{//������
		if ((m_cbActionMask&WIK_CHI_HU)>0 && 0!=m_cbActionCard)
		{//���Ժ�����
			OnCardOperate(WIK_CHI_HU, m_cbActionCard);
		}
		else
		{
			//ȡ��
			OnCardOperate(WIK_NULL, 0);
		}
		return true;
	}
	return false;
}

//�û�������
void CGameClient::OnUserOperateCard()
{
	bool bIsTingCard = bIsTingCardOpera();
	if (!bIsTingCard)
	{//û������
		if ((m_cbActionMask&WIK_CHI_HU)>0 && 0!=m_cbActionCard)
		{//���Ժ�����
			OnCardOperate(WIK_CHI_HU, m_cbActionCard);
			return;
		}
		else if ((m_cbActionMask&WIK_TING)>0)
		{//��ʼ����
			if (m_byGangCardNums>0 && 0!=m_byGangCard[0])
			{
				OnCardOperate(WIK_TING, m_byGangCard[0]);
				return;
			}
		}
		//ȡ��
		OnCardOperate(WIK_NULL, 0);
	}
}

//�˿˲���
void CGameClient::OnCardOperate(BYTE byOperaType, BYTE byCardData)
{
	KillGameTimer(IDI_OPERATE_CARD);
	KillGameTimer(IDI_GT_GAME_USER_OUTCARD);

	BYTE byChairID = GetMeChairID();
	if (m_wCurrentUser!=byChairID)
	{
		return;
	}

	//��������
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=byOperaType;
	OperateCard.cbOperateCard=byCardData;
	SendData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(CMD_C_OperateCard));

	if (0==byOperaType)
	{//���ȡ����
		if (m_bIsMeCanOutCard)
		{//�Լ������Գ���.
			//����
			m_wCurrentUser = byChairID;
			UINT nElapse = rand()%TIME_GAME_USER_NO_OUTCARD_INTERVAL+TIME_LESS;
			SetGameTimer(IDI_GT_GAME_USER_OUTCARD,nElapse);
		}
	}
	return;
}

//�Ƿ�ʼ���Ӷ���
bool CGameClient::OnSubStartSZFlash()
{
	KillGameTimer(IDI_SZSTART);

	BYTE byMeChairID = GetMeChairID();
	if (byMeChairID==m_wBankerUser)
	{
		if (WIK_NULL != m_cbActionMask)
		{//10�����
			m_bIsMeCanOutCard = true;
			UINT nElapse = rand()%TIME_OPERATE_CARD+TIME_LESS+12;
			SetGameTimer(IDI_OPERATE_CARD, nElapse);
		}
		else
		{//30�����
			UINT nElapse = rand()%TIME_GAME_USER_NO_OUTCARD_INTERVAL+TIME_LESS+10;
			SetGameTimer(IDI_GT_GAME_USER_OUTCARD,nElapse);
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
	if (NULL==pGameStart)
	{
		return false;
	}
	
	SetGameStatus(GS_MJ_PLAY);

	//��������
	m_TempKing = pGameStart->cbKingCard;

	BYTE byMeChairID = GetMeChairID();
	//���ñ���
	m_wBankerUser=pGameStart->wBankerUser;					//ׯ���û�
	m_wCurrentUser=pGameStart->wCurrentUser;				//��ǰ�����û�
	m_cbActionMask=pGameStart->cbUserAction;				//�Լ���������

	//�����˿�
	BYTE cbCardCount=(byMeChairID==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
	m_GameLogic.SwitchToCardIndex(pGameStart->cbCardData,cbCardCount,m_cbCardIndex);

	//��������
	if (m_wCurrentUser==byMeChairID)
	{
		::memcpy(m_byGangCard, pGameStart->byGangCard, sizeof(BYTE)*4);
		::memcpy(m_byBuCard, pGameStart->byBuCard, sizeof(BYTE)*4);
		m_byGangCardNums = pGameStart->byGangCardNums;
		m_byBuCardNums = pGameStart->byBuCardNums;

		UINT nElapse = rand()%TIME_USER_SZ_INTERVAL+1+2;
		SetGameTimer(IDI_SZSTART,nElapse);
	}

	if (m_pRoomLog)
		m_pRoomLog->AddMsg(0, 0, TRUE,"***** ��Ϸ��ʼCGameClient::OnSubGameStart");
	return true;
}


//�ж��Ƿ�������������
bool CGameClient::bIsHaveCard(BYTE byCard, BYTE& byLastCard)
{
	byLastCard = 0;
	BYTE byIndex = m_GameLogic.SwitchToCardIndex(byCard);
	if (0==m_cbCardIndex[byIndex])
	{//û��
		for (int i=MAX_INDEX; i>=0; i--)
		{
			if (m_cbCardIndex[i]>0)
			{
				byLastCard = m_GameLogic.SwitchToCardData(i);
				break;
			}
		}
		return false;
	}
	return true;
}

//����
void CGameClient::OnOutCard()
{
	//ɾ����ʱ��
	KillGameTimer(IDI_GT_GAME_USER_OUTCARD);
	KillGameTimer(IDI_OPERATE_CARD);

	BYTE byChairID = GetMeChairID();
	if (byChairID!=m_wCurrentUser)
	{
		return;
	}

	BYTE byOutCard = 0;
	if (m_bHearStatus)
	{//������
		if ((m_cbActionMask&WIK_CHI_HU)>0 && 0!=m_cbActionCard)
		{//���Ժ�����
			OnCardOperate(WIK_CHI_HU, m_cbActionCard);
			return;
		}
		else
		{
			byOutCard = m_cbActionCard;
		}
	}
	//��������
	if (0==byOutCard)
	{
		SearchOutCard(byOutCard);
	}
	if (0==byOutCard || !m_GameLogic.IsValidCard(byOutCard))
	{//����������������⣬��ѡ���������һ���ơ�
		BYTE byLastCard = 0;
		bool bIsHave = bIsHaveCard(byOutCard, byLastCard);
		if (!bIsHave)
		{
			byOutCard = byLastCard;
		}
	}
	CMD_C_OutCard OutCard;
	OutCard.cbCardData = byOutCard;
	SendData(SUB_C_OUT_CARD,&OutCard,sizeof(CMD_C_OutCard));

	//���ñ���
	m_wCurrentUser = INVALID_CHAIR;
	m_cbActionMask = WIK_NULL;
	m_cbActionCard = 0;
}

//�û�����
bool CGameClient::OnSubSendCard( const void *pBuffer,WORD wDataSize )
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	KillGameTimer(IDI_OPERATE_CARD);
	KillGameTimer(IDI_GT_GAME_USER_OUTCARD);

	//��������
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;
	if (NULL != pSendCard)
	{
		//���ñ���
		WORD wMeChairID=GetMeChairID();
		m_wCurrentUser=pSendCard->wCurrentUser;

		//���ñ���
		m_bIsMeCanOutCard = false;

		::memset(m_byGangCard, 0, sizeof(BYTE)*4);
		::memset(m_byBuCard, 0, sizeof(BYTE)*4);
		m_byGangCardNums = 0;
		m_byBuCardNums = 0;
		m_cbActionCard = 0;

		//����ʱ��
		if( wMeChairID == m_wCurrentUser )
		{
			//��������һ���˿�
			m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pSendCard->cbCardData)]++;	

			m_cbActionMask = pSendCard->cbActionMask;
			m_cbActionCard = pSendCard->cbCardData;

			if (pSendCard->cbActionMask != WIK_NULL)
			{//�в���
				::memcpy(m_byGangCard, pSendCard->byGangCard, sizeof(BYTE)*4);
				::memcpy(m_byBuCard, pSendCard->byBuCard, sizeof(BYTE)*4);
				m_byGangCardNums = pSendCard->byGangCardNums;
				m_byBuCardNums = pSendCard->byBuCardNums;

				m_bIsMeCanOutCard = true;
				UINT nElapse = rand()%TIME_OPERATE_CARD+TIME_LESS;
				SetGameTimer(IDI_OPERATE_CARD, nElapse);
			}
			else
			{//����
				UINT nElapse = rand()%TIME_GAME_USER_NO_OUTCARD_INTERVAL+TIME_LESS;
				SetGameTimer(IDI_GT_GAME_USER_OUTCARD,nElapse);
			}
		}
	}

	return true;
}


//�û�����
bool CGameClient::OnSubOutCard( const void *pBuffer,WORD wDataSize )
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard)) return false;

	KillGameTimer(IDI_OPERATE_CARD);
	KillGameTimer(IDI_GT_GAME_USER_OUTCARD);
	//��Ϣ����
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;
	if (NULL != pOutCard)
	{
		//��������
		WORD wMeChairID=GetMeChairID();

		//���ñ���
		m_wCurrentUser=INVALID_CHAIR;
		m_bIsMeCanOutCard = false;

		//ɾ���˿�
		if( wMeChairID == pOutCard->wOutCardUser )
		{
			m_GameLogic.RemoveCard(m_cbCardIndex,pOutCard->cbOutCardData);
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

	KillGameTimer(IDI_OPERATE_CARD);
	KillGameTimer(IDI_GT_GAME_USER_OUTCARD);

	//��������
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;
	//�û�����
	if (NULL!=pOperateNotify)
	{
		::memset(m_byGangCard, 0, sizeof(m_byGangCard));
		::memset(m_byBuCard, 0, sizeof(m_byBuCard));
		m_byGangCardNums = 0;
		m_byBuCardNums = 0;

		m_bIsMeCanOutCard = false;

		BYTE byChairID = GetMeChairID();
		m_wCurrentUser = byChairID;
		m_cbActionMask = pOperateNotify->cbActionMask;
		m_cbActionCard = pOperateNotify->cbActionCard;

		if (pOperateNotify->cbActionMask!=WIK_NULL)
		{//���Բ���
			::memcpy(m_byGangCard, pOperateNotify->byGangCard, sizeof(BYTE)*4);
			::memcpy(m_byBuCard, pOperateNotify->byBuCard, sizeof(BYTE)*4);
			m_byGangCardNums = pOperateNotify->byGangCardNums;
			m_byBuCardNums = pOperateNotify->byBuCardNums;

			UINT nElapse = rand()%TIME_OPERATE_CARD+TIME_LESS;
			SetGameTimer(IDI_OPERATE_CARD, nElapse);
		}
	}
	return true;
}


//�������֪ͨ
bool CGameClient::OnSubOperateResult( const void *pBuffer,WORD wDataSize )
{
	//Ч����Ϣ
	if (wDataSize!=sizeof(CMD_S_OperateResult)) return false;

	//��Ϣ����
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;
	if (NULL==pOperateResult)
	{
		return false;
	}
	KillGameTimer(IDI_OPERATE_CARD);
	KillGameTimer(IDI_GT_GAME_USER_OUTCARD);

	m_bIsMeCanOutCard = false;

	if (NULL != pOperateResult)
	{
		//���ñ���
		m_cbActionMask = WIK_NULL;
		m_cbActionCard = 0;

		//��������
		BYTE cbPublicCard=TRUE;
		WORD wOperateUser=pOperateResult->wOperateUser;
		BYTE cbOperateCard=pOperateResult->cbOperateCard;
		BYTE byChairID = GetMeChairID();
		//�������
		if ((pOperateResult->cbOperateCode&(WIK_GANG|WIK_FILL|WIK_TING))>0)
		{//�������,�����ǲ����ܡ���
			//Ϊ��ǰ�������
			m_wCurrentUser=INVALID_CHAIR;
			//���ñ���
			if ((pOperateResult->cbOperateCode==WIK_TING)&&(wOperateUser==byChairID)) 
			{
				m_bHearStatus = true;
			}

			//�˿�����
			if (byChairID==wOperateUser)
			{
				BYTE byIndex = m_GameLogic.SwitchToCardIndex(cbOperateCard);
				if (byIndex<MAX_INDEX)
				{
					m_cbCardIndex[byIndex]=0;
				}
			}
		}
		else if (pOperateResult->cbOperateCode!=WIK_NULL)
		{//������ң��ԡ��������
			//Ϊ��ǰ�������
			m_wCurrentUser=pOperateResult->wOperateUser;

			//��Ͻ���
			BYTE cbWeaveCard[4]={0,0,0,0},cbWeaveKind=pOperateResult->cbOperateCode;
			BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbOperateCard,cbWeaveCard);

			//�����˿�
			if (byChairID==wOperateUser)
			{
				//ɾ���˿�
				m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard,1);
				m_GameLogic.RemoveCard(m_cbCardIndex,cbWeaveCard,cbWeaveCardCount-1);
			}

			if (byChairID==m_wCurrentUser)
			{//���ǲ������,Ҫ���һ����
				//����ʱ��
				UINT nElapse = rand()%TIME_GAME_USER_NO_OUTCARD_INTERVAL+TIME_LESS;
				SetGameTimer(IDI_GT_GAME_USER_OUTCARD,nElapse);
			}
		}
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
	if (NULL==pOperateHaiDi)
	{
		return true;
	}

	CancelAllTimer();

	//����״̬
	SetGameStatus(GS_MJ_HAI_DI);

	//���ñ���
	m_bIsMeCanOutCard = false;
	m_wCurrentUser=INVALID_CHAIR;

	BYTE byChairID = GetMeChairID();
	if (NULL != pOperateHaiDi && byChairID==pOperateHaiDi->wCurrentUser)
	{
		m_wCurrentUser = pOperateHaiDi->wCurrentUser;
		WORD wTime = rand()%TIME_USER_HAI_DI_CARD_INTERVAL + TIME_LESS;
		SetGameTimer(IDI_USER_HAI_DI_CARD, wTime);
	}
	
	return true;
}


//���Ʋ���
bool CGameClient::OnSubTingCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameTingCard));
	if (wDataSize!=sizeof(CMD_S_GameTingCard)) return false;

	KillGameTimer(IDI_USER_TING_CARD);

	//��Ϣ����
	CMD_S_GameTingCard * pGameTingCard=(CMD_S_GameTingCard *)pBuffer;
	if (NULL == pGameTingCard)
	{
		return false;
	}
	BYTE byChairID = GetMeChairID();
	if (pGameTingCard->wTingCardUser == byChairID)
	{
		m_bHearStatus = false;
		m_wCurrentUser = pGameTingCard->wTingCardUser;

		//����
		WORD wTime = rand()%TIME_TING_CARD_INTERVAL + TIME_LESS;
		SetGameTimer(IDI_USER_TING_CARD, wTime);
	}

	return true;

}

//���ƽ��
bool CGameClient::OnSubTingCardResult(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameTingCard));
	if (wDataSize!=sizeof(CMD_S_GameTingCard)) return false;

	//��Ϣ����
	CMD_S_GameTingCard * pGameTingCard=(CMD_S_GameTingCard *)pBuffer;
	if (NULL != pGameTingCard)
	{
		BYTE byChairID = GetMeChairID();
		if (pGameTingCard->wTingCardUser == byChairID)
		{
			KillGameTimer(IDI_USER_TING_CARD);
			m_bHearStatus = true;
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
		
		SetGameStatus(GS_MJ_FREE);

		//׼����ʱ��
		WORD wTime = rand()%TIME_GAME_READY+TIME_LESS;
		SetGameTimer(IDI_GT_GAME_USER_READY,wTime);
	}
	if (m_pRoomLog)
		m_pRoomLog->AddMsg(0, 0, TRUE,"***** ��Ϸ����CGameClient::OnSubGameEnd");
	return true;
}


//��������
void CGameClient::SearchOutCard( BYTE& byOutCard)
{
	//��ʼ��
	byOutCard = 0;

	WORD wMeChairId = GetMeChairID();
	if( wMeChairId == INVALID_CHAIR )
	{
		return;
	}

	//ת������
	BYTE bIndex=m_GameLogic.SwitchToCardIndex(m_TempKing);
	BYTE byCard[MAX_COUNT]={0};
	BYTE byCardCount = 0;
	for( BYTE i = 0; i < MAX_INDEX; i++ )
	{
		if (bIndex==i) continue;
		for( BYTE j = 0; j < m_cbCardIndex[i]; j++ )
		{
			byCard[byCardCount++] = i;
		}
	}

	BYTE cbActionCard = m_cbActionCard;
	//����ԭʼ��
	CAndroidAI AndroidAi;
	AndroidAi.SetEnjoinOutCard(m_TempKing, 1);
	AndroidAi.SetCardData(byCard,byCardCount);
	AndroidAi.Think();

	//��������
	BYTE cbOutCardData = cbActionCard;
	BYTE byBadlyIndex = AndroidAi.GetBadlyCard();
	if( byBadlyIndex == 0xff )
	{
		byBadlyIndex = AndroidAi.GetBadlyIn2Card();
		if( byBadlyIndex == 0xff )
		{
			byBadlyIndex = AndroidAi.GetBadlyIn3Card();
		}
	}
	cbOutCardData = m_GameLogic.SwitchToCardData(byBadlyIndex);

	if (!m_GameLogic.IsValidCard(cbOutCardData))
	{
		byOutCard = m_cbActionCard;
	}
	else
	{
		byOutCard = cbOutCardData;
	}
}