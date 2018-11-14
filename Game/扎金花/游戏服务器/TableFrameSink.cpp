#include "StdAfx.h"
#include "TableFrameSink.h"
#include "TraceService.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_AllReady;	//��ʼģʽ

//��ʱ�� 0~30
#define IDI_GAME_COMPAREEND					1									//������ʱ��
#define IDI_GAME_OPENEND					2									//������ʱ��

#define TIME_GAME_COMPAREEND				6000								//������ʱ��
#define TIME_GAME_OPENEND					6000								//������ʱ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_bOperaCount=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_lCompareCount=0;
	m_bGameEnd=false;
	ZeroMemory(m_wFlashUser,sizeof(m_wFlashUser));

	//�û�״̬
	ZeroMemory(&m_StGameEnd,sizeof(m_StGameEnd));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	for(int i=0;i<m_wPlayerCount;i++)
	{
		m_bMingZhu[i]=false;
		m_wCompardUser[i].RemoveAll();
	}

	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//��ע��Ϣ
	m_lMaxCellScore=0L;
	m_lCellScore=0L;
	m_lCurrentTimes=0L;		
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lUserMaxScore,sizeof(m_lUserMaxScore));

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
}

//�ӿڲ�ѯ--��������Ϣ�汾
void * __cdecl CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��ʼ��
bool __cdecl CTableFrameSink::InitTableFrameSink(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	return true;
}

//��λ����
void __cdecl CTableFrameSink::RepositTableFrameSink()
{
	//��Ϸ����
	m_bOperaCount=0;
	m_wCurrentUser=INVALID_CHAIR;
	m_lCompareCount=0;
	ZeroMemory(m_wFlashUser,sizeof(m_wFlashUser));

	//�û�״̬
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	for(int i=0;i<m_wPlayerCount;i++)
	{
		m_bMingZhu[i]=false;	
		m_wCompardUser[i].RemoveAll();
	}

	//�˿˱���
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//��ע��Ϣ
	m_lMaxCellScore=0L;						
	m_lCellScore=0L;						
	m_lCurrentTimes=0L;		
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lUserMaxScore,sizeof(m_lUserMaxScore));

	return;
}

//��ʼģʽ
enStartMode __cdecl CTableFrameSink::GetGameStartMode()
{
	return m_GameStartMode;
}

//��Ϸ״̬
bool __cdecl CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	ASSERT(wChairID<m_wPlayerCount);   
	return (m_cbPlayStatus[wChairID]==TRUE)?true:false;
}

//��Ϸ��ʼ
bool __cdecl CTableFrameSink::OnEventGameStart()
{

	ZeroMemory(&m_StGameEnd,sizeof(m_StGameEnd));
	char chData[100]={0};
	sprintf(chData, "CTableFrameSink::OnEventGameStart");
	CTraceService::TraceString(chData, TraceLevel_Info);

	//����״̬
	m_pITableFrame->SetGameStatus(GS_PLAYING);

	m_bGameEnd=false;

	//�����ע
	//LONGLONG lTimes=6L;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbPlayStatus[i]=FALSE;
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) continue;

		const tagUserScore * pUserScore=pIServerUserItem->GetUserScore();
		ASSERT(pUserScore->lGem>=m_pGameServiceOption->lCellScore);

		//���ñ���
		m_cbPlayStatus[i]=TRUE;
		m_lUserMaxScore[i]=pUserScore->lGem;

		////�жϵ�ע
		//LONGLONG Temp=lTimes;
		//if(m_lUserMaxScore[i]<10001)Temp=1L;
		//else if(m_lUserMaxScore[i]<100001)Temp=2L;
		//else if(m_lUserMaxScore[i]<1000001)Temp=3L;
		//else if(m_lUserMaxScore[i]<10000001)Temp=4L;
		//else if(m_lUserMaxScore[i]<100000001)Temp=5L;
		//if(lTimes>Temp)lTimes=Temp;
	}

	//��ע����
	m_lCellScore=m_pGameServiceOption->lCellScore;
	//while((lTimes--)>0)m_lCellScore*=10;
	m_lMaxCellScore=m_lCellScore*5;
	m_lCurrentTimes=1;

	//�����ע(�������)
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_cbPlayStatus[i]==TRUE)
		{
			//m_lUserMaxScore[i] = __min(m_lUserMaxScore[i],m_lMaxCellScore*101);
			DOUBLE  lRestrictScore = m_pGameServiceOption->lRestrictScore;
            m_lUserMaxScore[i] = lRestrictScore;
		}
	}

	//�ַ��˿�
	m_GameLogic.RandCardList(m_cbHandCardData[0],sizeof(m_cbHandCardData)/sizeof(m_cbHandCardData[0][0]));

	//�û�����
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE)
		{
			m_lTableScore[i]=m_lCellScore;
		}
	}

	//����ׯ��
	if(m_wBankerUser==INVALID_CHAIR)m_wBankerUser=rand()%m_wPlayerCount;

	//ׯ���뿪
	if(m_wBankerUser<m_wPlayerCount && m_cbPlayStatus[m_wBankerUser]==FALSE)m_wBankerUser=rand()%m_wPlayerCount;

	//ȷ��ׯ��
	while(m_cbPlayStatus[m_wBankerUser]==FALSE)
	{
		m_wBankerUser=(m_wBankerUser+1)%m_wPlayerCount;
	}

	//��ǰ�û�
	m_wCurrentUser=(m_wBankerUser+1)%m_wPlayerCount;
	while(m_cbPlayStatus[m_wCurrentUser]==FALSE)
	{
		m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
	}

	//��������
	CMD_S_GameStart GameStart;
	GameStart.lMaxScore=m_lMaxCellScore;
	GameStart.lCellScore=m_lCellScore;
	GameStart.lCurrentTimes=m_lCurrentTimes;
	GameStart.wCurrentUser=m_wCurrentUser;
	GameStart.wBankerUser=m_wBankerUser;



	for (WORD k=0;k<m_wPlayerCount;k++)
	{
		GameStart.bPlayStatus[k] = false;
		if (m_cbPlayStatus[k])
		{
			GameStart.bPlayStatus[k] = true;
		}
	}
	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE)
		{
			GameStart.lUserMaxScore=m_lUserMaxScore[i];
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_COMPARECARD:	//���ƽ���
	case GER_NO_PLAYER:		//û�����
		{
			if(m_bGameEnd)return true;
			m_bGameEnd=true;

			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			for (WORD i=0;i<m_wPlayerCount;i++)
				for (WORD j=0;j<4;j++)
					GameEnd.wCompareUser[i][j]=INVALID_CHAIR;

			//Ψһ���
			WORD wWinner,wUserCount=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{	
				if(m_cbPlayStatus[i]==TRUE)
				{
					wUserCount++;
					wWinner=i;
					if(GER_COMPARECARD==cbReason)ASSERT(m_wBankerUser==i);
					m_wBankerUser=i;
				}
			}

			//ʤ����ǿ��
			if(wUserCount==0 /*&& GER_COMPARECARD==cbReason*/)
			{
				wWinner=m_wBankerUser;
			}

			//������ע
			LONGLONG lWinnerScore=0L;
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				if(i==wWinner)continue;
                if (m_lTableScore[i] > m_lUserMaxScore[i])
                {
					m_lTableScore[i] = m_lUserMaxScore[i];
                }
				GameEnd.lGameScore[i]=-m_lTableScore[i];
				lWinnerScore+=m_lTableScore[i];
			}

			//����˰��
			LONGLONG lRevenue=(LONGLONG)m_pGameServiceOption->wRevenue;
			GameEnd.lGameTax=lWinnerScore*lRevenue/100;
			GameEnd.lGameScore[wWinner]=lWinnerScore-GameEnd.lGameTax;

			CopyMemory(GameEnd.cbCardData,m_cbHandCardData,sizeof(m_cbHandCardData));

			//�˿�����
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				WORD wCount=0;
				while(m_wCompardUser[i].GetCount()>0)
				{
					GameEnd.wCompareUser[i][wCount++] = m_wCompardUser[i].GetAt(0);
					m_wCompardUser[i].RemoveAt(0);
				}
			}

			GameEnd.wEndState=0;

			//ʤ����ǿ��
			if(wUserCount==1)
			{
				//�޸Ļ���
				//д�����
				m_pITableFrame->WriteUserScore(wWinner,GameEnd.lGameScore[wWinner],GameEnd.lGameTax,enScoreKind_Win);
			}

			if(wChairID==GAME_PLAYER)
			{
				//��������
				CopyMemory(&m_StGameEnd,&GameEnd,sizeof(CMD_S_GameEnd));
				m_pITableFrame->SetGameTimer(IDI_GAME_COMPAREEND,TIME_GAME_COMPAREEND/2,1,0);
			}
			else
			{
				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}
			
			//������Ϸ
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_USER_LEFT:		//�û�ǿ��
		{
			if(m_bGameEnd)return true;

			//Ч�����
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount);

			//ǿ�˴���
			return OnUserGiveUp(wChairID,true);
		}
	case GER_OPENCARD:		//���ƽ���   
		{
			if(m_bGameEnd)return true;
			m_bGameEnd = true;

			//�������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			for (WORD i=0;i<m_wPlayerCount;i++)
				for (WORD j=0;j<4;j++)
					GameEnd.wCompareUser[i][j]=INVALID_CHAIR;

			//ʤ�����
			WORD wWinner=m_wBankerUser;

			//�������
			LONGLONG lWinnerScore=0L;
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				if(i==wWinner)continue;
				if (m_lTableScore[i] > m_lUserMaxScore[i])
				{
					m_lTableScore[i] = m_lUserMaxScore[i];
				}
				lWinnerScore+=m_lTableScore[i];
				GameEnd.lGameScore[i]=-m_lTableScore[i];
			}

			//����˰��
			LONGLONG lRevenue=(LONGLONG)m_pGameServiceOption->wRevenue;
			GameEnd.lGameTax=lWinnerScore*lRevenue/100;
			GameEnd.lGameScore[wWinner]=lWinnerScore-GameEnd.lGameTax;

			//���ƽ���
			GameEnd.wEndState=1;
			CopyMemory(GameEnd.cbCardData,m_cbHandCardData,sizeof(m_cbHandCardData));

			//�˿�����
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{				
				WORD wCount=0;
				while(m_wCompardUser[i].GetCount()>0)
				{
					GameEnd.wCompareUser[i][wCount++] = m_wCompardUser[i].GetAt(0);
					m_wCompardUser[i].RemoveAt(0);
				}
			}

			//�޸Ļ���
			tagScoreInfo ScoreInfo[m_wPlayerCount];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{				
				if(i==wWinner || m_cbPlayStatus[i]==FALSE)continue;
				ScoreInfo[i].lScore=GameEnd.lGameScore[i];
				ScoreInfo[i].ScoreKind=enScoreKind_Lost;
			}
			ScoreInfo[wWinner].lScore=GameEnd.lGameScore[wWinner];
			ScoreInfo[wWinner].ScoreKind=enScoreKind_Win;

			//д�����
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_cbPlayStatus[i]==TRUE)
				{
					LONGLONG lRevenueValue = ((i==wWinner)?(GameEnd.lGameTax):(0L));
					enScoreKind nScoreKind = ((i==wWinner)?(enScoreKind_Win):(enScoreKind_Lost));
					m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],lRevenueValue,nScoreKind);
				}
			}
			

			if(wChairID==GAME_PLAYER)
			{
				//��������
				CopyMemory(&m_StGameEnd,&GameEnd,sizeof(m_StGameEnd));
				//m_pITableFrame->SetGameTimer(IDI_GAME_OPENEND,TIME_GAME_OPENEND,1,0);
				m_pITableFrame->SetGameTimer(IDI_GAME_OPENEND,TIME_GAME_OPENEND/6,1,0);
			}
			else
			{
				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			return true;
		}
	}

	return false;
}

//���ͳ���
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_FREE:		//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//���ñ���
			StatusFree.lCellScore=0L;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_PLAYING:	//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//��ע��Ϣ
			StatusPlay.lMaxCellScore=m_lMaxCellScore;
			StatusPlay.lCellScore=m_lCellScore;
			StatusPlay.lCurrentTimes=m_lCurrentTimes;
			StatusPlay.lUserMaxScore=m_lUserMaxScore[wChiarID];

			//���ñ���
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			CopyMemory(StatusPlay.bMingZhu,m_bMingZhu,sizeof(m_bMingZhu));
			CopyMemory(StatusPlay.lTableScore,m_lTableScore,sizeof(m_lTableScore));

			for (int i=0; i<GAME_PLAYER; i++)
			{
				StatusPlay.cbPlayStatus[i] = false;
				if (m_cbPlayStatus[i])
				{
					StatusPlay.cbPlayStatus[i] = true;
				}
			}

			//��ǰ״̬
			StatusPlay.bCompareState=(m_lCompareCount<=0)?false:true;

			//�����˿�
			CopyMemory(&StatusPlay.cbHandCardData,&m_cbHandCardData[wChiarID],MAX_COUNT);

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	//Ч�����
	ASSERT(FALSE);

	return false;
}

//��ʱ���¼� 
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	//������ʱ
	if(wTimerID==IDI_GAME_COMPAREEND || IDI_GAME_OPENEND==wTimerID)
	{
		//ɾ��ʱ��
		if(wTimerID==IDI_GAME_COMPAREEND)m_pITableFrame->KillGameTimer(IDI_GAME_COMPAREEND);
		else m_pITableFrame->KillGameTimer(IDI_GAME_OPENEND);

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&m_StGameEnd,sizeof(CMD_S_GameEnd));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&m_StGameEnd,sizeof(CMD_S_GameEnd));

		//������Ϸ
		m_pITableFrame->ConcludeGame();
	}

	return false;
}

//��Ϸ��Ϣ����
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	bool bRet = false;
	switch (wSubCmdID)
	{
	case SUB_C_GIVE_UP:			//�û�����
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY)
			{
				bRet = false;
				break;
			}

			//״̬�ж�
			ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE);
			if (m_cbPlayStatus[pUserData->wChairID]==FALSE)
			{
				bRet = false;
				break;
			}

			//��Ϣ����
			bRet = OnUserGiveUp(pUserData->wChairID);
			break;
		}
	case SUB_C_LOOK_CARD:		//�û�����
	case SUB_C_OPEN_CARD:		//�û�����
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY)
			{
				bRet = false;
				break;
			}

			//״̬�ж�
			ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE);
			if (m_cbPlayStatus[pUserData->wChairID]==FALSE)
			{
				bRet = false;
				break;
			}

			//��Ϣ����
			if(wSubCmdID==SUB_C_LOOK_CARD)
			{//����
				bRet = OnUserLookCard(pUserData->wChairID);
			}
			else if(wSubCmdID==SUB_C_OPEN_CARD)
			{//����
				bRet = OnUserOpenCard(pUserData->wChairID);
			}
			break;
		}
	case SUB_C_COMPARE_CARD:	//�û�����
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_CompareCard));
			if (wDataSize!=sizeof(CMD_C_CompareCard))
			{
				bRet = false;
				break;
			}

			//��������
			CMD_C_CompareCard * pCompareCard=(CMD_C_CompareCard *)pDataBuffer;
			if (NULL != pCompareCard)
			{
				//�û�Ч��
				tagServerUserData * pUserData=pIServerUserItem->GetUserData();
				if (pUserData->cbUserStatus!=US_PLAY)
				{
					bRet = false;
					break;
				}

				//����Ч��
				ASSERT(pUserData->wChairID==m_wCurrentUser);
				if(pUserData->wChairID!=m_wCurrentUser)
				{
					bRet = false;
					break;
				}
				if (pCompareCard->wCompareUser >= GAME_PLAYER)
				{
					bRet = false;
					break;
				}

				//״̬�ж�
				ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE && m_cbPlayStatus[pCompareCard->wCompareUser]==TRUE);
				if(m_cbPlayStatus[pUserData->wChairID]==FALSE || m_cbPlayStatus[pCompareCard->wCompareUser]==FALSE)
				{
					bRet = false;
					break;
				}

				//��Ϣ����
				bRet = OnUserCompareCard(pUserData->wChairID,pCompareCard->wCompareUser);
			}
			break;
		}
	case SUB_C_ADD_SCORE:		//�û���ע
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_AddScore));
			if (wDataSize!=sizeof(CMD_C_AddScore))
			{
				bRet = false;
				break;
			}
			//��������
			CMD_C_AddScore * pAddScore=(CMD_C_AddScore *)pDataBuffer;
			if (NULL==pAddScore)
			{
				bRet = false;
				break;
			}

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (NULL!=pUserData && pUserData->cbUserStatus!=US_PLAY)
			{
				bRet = false;
				break;
			}

			//״̬�ж�
			ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE);
			if (m_cbPlayStatus[pUserData->wChairID]==FALSE)
			{
				bRet = false;
				break;
			}

			//��ǰ״̬
			//if(pAddScore->wState>0)
			//{
			//	m_lCompareCount=pAddScore->lScore;
			//}
			m_lCompareCount = pAddScore->wState;

			//��Ϣ����
			bRet = OnUserAddScore(pUserData->wChairID,pAddScore->lScore,false,((pAddScore->wState>0)?true:false));
			break;
		}
	case SUB_C_WAIT_COMPARE:	//�ȴ�����
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (NULL==pUserData || pUserData->cbUserStatus!=US_PLAY)
			{
				bRet = false;
				break;
			}
			//״̬�ж�
			ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE);
			if (m_cbPlayStatus[pUserData->wChairID]==FALSE)
			{
				bRet = false;
				break;
			}

			//��Ϣ����
			CMD_S_WaitCompare WaitCompare;
			WaitCompare.wCompareUser = pUserData->wChairID;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_WAIT_COMPARE,&WaitCompare,sizeof(CMD_S_WaitCompare));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_WAIT_COMPARE,&WaitCompare,sizeof(CMD_S_WaitCompare));
			bRet = true;
			break;
		}
	case SUB_C_FINISH_FLASH:	//��ɶ��� 
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (NULL == pUserData)
			{
				bRet = false;
				break;
			}
			if (!m_wFlashUser[pUserData->wChairID])
			{
				bRet = true;
				break;
			}

			//���ñ���
			m_wFlashUser[pUserData->wChairID] = FALSE;

			//������Ϸ
			WORD i = 0;
			for(;i<GAME_PLAYER;i++)
			{
				if(m_wFlashUser[i])
				{
					break;
				}
			}
			if(i==GAME_PLAYER)
			{
				//ɾ��ʱ��
				m_pITableFrame->KillGameTimer(IDI_GAME_COMPAREEND);
				m_pITableFrame->KillGameTimer(IDI_GAME_OPENEND);

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&m_StGameEnd,sizeof(m_StGameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&m_StGameEnd,sizeof(m_StGameEnd));

				//������Ϸ
				m_pITableFrame->ConcludeGame();
			}
			bRet = true;
			break;
		}
	}
	return true;

	//char chData[100]={0};
	//sprintf(chData, "CTableFrameSink::OnGameMessage--%d", wSubCmdID);
	//AfxMessageBox(chData);
	//return false;
}


/*
//��Ϸ��Ϣ����
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_GIVE_UP:			//�û�����
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//״̬�ж�
			ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE);
			if (m_cbPlayStatus[pUserData->wChairID]==FALSE) return false;

			//��Ϣ����
			return OnUserGiveUp(pUserData->wChairID);
		}
	case SUB_C_LOOK_CARD:		//�û�����
	case SUB_C_OPEN_CARD:		//�û�����
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//״̬�ж�
			ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE);
			if (m_cbPlayStatus[pUserData->wChairID]==FALSE) return false;

			//��Ϣ����
			if(wSubCmdID==SUB_C_LOOK_CARD)return OnUserLookCard(pUserData->wChairID);
			return OnUserOpenCard(pUserData->wChairID);
		}
	case SUB_C_COMPARE_CARD:	//�û�����
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_CompareCard));
			if (wDataSize!=sizeof(CMD_C_CompareCard)) return false;

			//��������
			CMD_C_CompareCard * pCompareCard=(CMD_C_CompareCard *)pDataBuffer;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//����Ч��
			ASSERT(pUserData->wChairID==m_wCurrentUser);
			if(pUserData->wChairID!=m_wCurrentUser)return false;

			//״̬�ж�
			ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE && m_cbPlayStatus[pCompareCard->wCompareUser]==TRUE);
			if(m_cbPlayStatus[pUserData->wChairID]==FALSE || m_cbPlayStatus[pCompareCard->wCompareUser]==FALSE)return false;

			//��Ϣ����
			return OnUserCompareCard(pUserData->wChairID,pCompareCard->wCompareUser);
		}
	case SUB_C_ADD_SCORE:		//�û���ע
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_AddScore));
			if (wDataSize!=sizeof(CMD_C_AddScore)) return false;

			//��������
			CMD_C_AddScore * pAddScore=(CMD_C_AddScore *)pDataBuffer;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//״̬�ж�
			ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE);
			if (m_cbPlayStatus[pUserData->wChairID]==FALSE) return false;

			//��ǰ״̬
			if(pAddScore->wState>0)m_lCompareCount=pAddScore->lScore;

			//��Ϣ����
			return OnUserAddScore(pUserData->wChairID,pAddScore->lScore,false,((pAddScore->wState>0)?true:false));
		}
	case SUB_C_WAIT_COMPARE:	//�ȴ�����
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//״̬�ж�
			ASSERT(m_cbPlayStatus[pUserData->wChairID]==TRUE);
			if (m_cbPlayStatus[pUserData->wChairID]==FALSE) return false;

			//��Ϣ����
			CMD_S_WaitCompare WaitCompare;
			WaitCompare.wCompareUser = pUserData->wChairID;
			if(m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_WAIT_COMPARE,&WaitCompare,sizeof(WaitCompare)) &&
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_WAIT_COMPARE,&WaitCompare,sizeof(WaitCompare)) )
				return true;
		}
	case SUB_C_FINISH_FLASH:	//��ɶ��� 
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//״̬�ж�
			ASSERT(m_wFlashUser[pUserData->wChairID]==TRUE);
			if (m_wFlashUser[pUserData->wChairID]==FALSE) return true;

			//���ñ���
			m_wFlashUser[pUserData->wChairID] = FALSE;

			//������Ϸ
			for(WORD i=0;i<GAME_PLAYER;i++)
			{
				if(m_wFlashUser[i] != FALSE)break;
			}
			if(i==GAME_PLAYER)
			{
				//ɾ��ʱ��
				m_pITableFrame->KillGameTimer(IDI_GAME_COMPAREEND);
				m_pITableFrame->KillGameTimer(IDI_GAME_OPENEND);

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&m_StGameEnd,sizeof(m_StGameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&m_StGameEnd,sizeof(m_StGameEnd));

				//������Ϸ
				m_pITableFrame->ConcludeGame();
			}
			return true;
		}
	}

	return false;
}
*/

//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�����¼�
bool CTableFrameSink::OnUserGiveUp(WORD wChairID,bool bExit)
{
	//��������
	m_cbPlayStatus[wChairID] = FALSE;

	//������Ϣ
	CMD_S_GiveUp GiveUp;
	GiveUp.wGiveUpUser=wChairID;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));

	//д�����
	m_pITableFrame->WriteUserScore(wChairID,-m_lTableScore[wChairID],0,(bExit?enScoreKind_Flee:enScoreKind_Lost));

	//����ͳ��
	WORD wPlayerCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE) wPlayerCount++;
	}

	//�жϽ���
	if (wPlayerCount>=2)
	{
		if (m_wCurrentUser==wChairID)
		{
			OnUserAddScore(wChairID,0L,true,false);
		}
	}
	else OnEventGameEnd(INVALID_CHAIR,NULL,GER_NO_PLAYER);

	return true;
}

//�����¼�
bool CTableFrameSink::OnUserLookCard(WORD wChairID)
{
	//״̬Ч��
	ASSERT(m_wCurrentUser==wChairID);
	if (m_wCurrentUser!=wChairID) return false;

	//����Ч��
	ASSERT(!m_bMingZhu[wChairID]);
	if (m_bMingZhu[wChairID]) return true;

	//���ò���
	m_bMingZhu[wChairID]=true;

	//��������
	CMD_S_LookCard LookCard;
	CopyMemory(LookCard.cbCardData,m_cbHandCardData[wChairID],sizeof(m_cbHandCardData[0]));
	LookCard.wLookCardUser=wChairID;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_LOOK_CARD,&LookCard,sizeof(LookCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LOOK_CARD,&LookCard,sizeof(LookCard));

	return true;
}

//�����¼�
bool CTableFrameSink::OnUserCompareCard(WORD wFirstChairID,WORD wNextChairID)
{
	//Ч�����
	ASSERT(m_lCompareCount>0);
	if(m_lCompareCount<=0)return true;
	//LONGLONG lTemp=(m_bMingZhu[m_wCurrentUser])?6:5;
	//ASSERT((m_lUserMaxScore[m_wCurrentUser]-m_lTableScore[m_wCurrentUser]+m_lCompareCount) >= (m_lMaxCellScore*lTemp));
	//if((m_lUserMaxScore[m_wCurrentUser]-m_lTableScore[m_wCurrentUser]+m_lCompareCount) < (m_lMaxCellScore*lTemp))return false;
	//ASSERT(m_wCurrentUser==m_wBankerUser || m_lTableScore[m_wCurrentUser]-m_lCompareCount>=2*m_lCellScore);
	//if(!(m_wCurrentUser==m_wBankerUser || m_lTableScore[m_wCurrentUser]-m_lCompareCount>=2*m_lCellScore))
	//{
	//	return true;
	//}

	//�Ƚϴ�С
	WORD wCompareCardStatus = m_GameLogic.CompareCard(m_cbHandCardData[wFirstChairID],m_cbHandCardData[wNextChairID],MAX_COUNT);

	//״̬����
	m_lCompareCount=0;

	//ʤ���û�
	WORD wLostUser=INVALID_CHAIR,wWinUser=INVALID_CHAIR;
	if(1==wCompareCardStatus)
	{//First win
		wWinUser=wFirstChairID;
		wLostUser=wNextChairID;
	}
	else
	{//
		wWinUser=wNextChairID;
		wLostUser=wFirstChairID;
	}

	//��������
	m_wCompardUser[wLostUser].Add(wWinUser);
	m_wCompardUser[wWinUser].Add(wLostUser);
	m_cbPlayStatus[wLostUser]=FALSE;

	//����ͳ��
	WORD wPlayerCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE) wPlayerCount++;
	}

	//������Ϸ
	if(wPlayerCount>=2)
	{
		//�û��л�
		WORD wNextPlayer=INVALID_CHAIR;
		for (WORD i=1;i<m_wPlayerCount;i++)
		{
			//���ñ���
			wNextPlayer=(m_wCurrentUser+i)%m_wPlayerCount;

			//�����ж�
			if (m_cbPlayStatus[wNextPlayer]==TRUE) break;
		}
		//�����û�
		m_wCurrentUser=wNextPlayer;
	}
	else m_wCurrentUser=INVALID_CHAIR;

	//��������
	CMD_S_CompareCard CompareCard;
	CompareCard.wCurrentUser = m_wCurrentUser;//next
	CompareCard.wLostUser = wLostUser;
	CompareCard.wCompareUser[0] = wFirstChairID;
	CompareCard.wCompareUser[1] = wNextChairID;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_COMPARE_CARD,&CompareCard,sizeof(CMD_S_CompareCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_COMPARE_CARD,&CompareCard,sizeof(CMD_S_CompareCard));

	//д�����
	m_pITableFrame->WriteUserScore(wLostUser,-m_lTableScore[wLostUser],0,enScoreKind_Lost);

	//������Ϸ
	if (wPlayerCount<2 || INVALID_CHAIR==m_wCurrentUser)
	{
		m_wBankerUser=wWinUser;
		m_wFlashUser[wNextChairID] = TRUE;
		m_wFlashUser[wFirstChairID] = TRUE;
		OnEventGameEnd(GAME_PLAYER,NULL,GER_COMPARECARD);
	}

	return true;
}

//�����¼�
bool CTableFrameSink::OnUserOpenCard(WORD wUserID)
{
	//Ч�����
	ASSERT(m_lCompareCount>0);
	if(m_lCompareCount<=0)return true;
	ASSERT(m_wCurrentUser==wUserID);
	if(m_wCurrentUser!=wUserID)
	{
		return false;
	}
	//LONGLONG lTemp=(m_bMingZhu[wUserID])?6:5;
	//ASSERT((m_lUserMaxScore[wUserID]-m_lTableScore[wUserID]+m_lCompareCount) < (m_lMaxCellScore*lTemp));
	//if((m_lUserMaxScore[wUserID]-m_lTableScore[wUserID]+m_lCompareCount) >= (m_lMaxCellScore*lTemp))return false;

	//��������
	m_wCurrentUser=INVALID_CHAIR;
	m_lCompareCount = 0;

	//�����˿�
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

	//�������
	WORD wWinner=wUserID;

	//����������
	for (WORD i=1;i<m_wPlayerCount;i++)
	{
		WORD w=(wUserID+i)%m_wPlayerCount;

		//�û�����
		if (m_cbPlayStatus[w]==FALSE) continue;

		//�Ա��˿�
		if (m_GameLogic.CompareCard(cbUserCardData[w],cbUserCardData[wWinner],MAX_COUNT)>=TRUE) 
		{
			wWinner=w;
		}
	}
	ASSERT(m_cbPlayStatus[wWinner]==TRUE);
	if(m_cbPlayStatus[wWinner]==FALSE)
	{
		return true;
	}

	//ʤ�����
	m_wBankerUser = wWinner;

	//��������
	CMD_S_OpenCard OpenCard;
	OpenCard.wWinner=wWinner;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPEN_CARD,&OpenCard,sizeof(CMD_S_OpenCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPEN_CARD,&OpenCard,sizeof(CMD_S_OpenCard));

	//������Ϸ
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_cbPlayStatus[i]==TRUE)
		{
			m_wFlashUser[i] = TRUE;
			OnEventGameEnd(GAME_PLAYER,NULL,GER_OPENCARD);
		}
	 
	}


	return true;
}

//��ע�¼�
bool CTableFrameSink::OnUserAddScore(WORD wChairID, LONGLONG lScore, bool bGiveUp, bool bCompareCard)
{
	if (bGiveUp==false)				//��������
	{
		//״̬Ч��
		ASSERT(m_wCurrentUser==wChairID);
		if (m_wCurrentUser!=wChairID) return false;

		//���Ч��
		ASSERT(lScore>=0 && lScore%m_lCellScore==0);
		ASSERT((lScore+m_lTableScore[wChairID])<=m_lUserMaxScore[wChairID]);
		if (lScore<0 || lScore%m_lCellScore!=0) return false;
		if ((lScore+m_lTableScore[wChairID])>m_lUserMaxScore[wChairID])
		{
			m_lTableScore[wChairID] = m_lUserMaxScore[wChairID];
		}

		//��ǰ����	bGiveUp������lScoreΪ0
		//�����ƺ�ѺעҪ�����������Ҫ���ƵĻ�������
		LONGLONG lTimes=(m_bMingZhu[wChairID] || bCompareCard)?2:1;
		if(m_bMingZhu[wChairID] && bCompareCard)lTimes=4;
		LONGLONG lTemp=lScore/m_lCellScore/lTimes;
		ASSERT(m_lCurrentTimes <= lTemp && m_lCurrentTimes <= m_lMaxCellScore/m_lCellScore);
		if(!(m_lCurrentTimes <= lTemp && m_lCurrentTimes <= m_lMaxCellScore/m_lCellScore))
		{
			return false;
		}
		m_lCurrentTimes = lTemp;

		//�û�ע��
		m_lTableScore[wChairID]+=lScore;
	}

	//�����û�
	if(!bCompareCard)
	{
		//�û��л�
		WORD wNextPlayer=INVALID_CHAIR;
		for (WORD i=1;i<m_wPlayerCount;i++)
		{
			//���ñ���
			wNextPlayer=(m_wCurrentUser+i)%m_wPlayerCount;

			//�����ж�
			if (m_cbPlayStatus[wNextPlayer]==TRUE) break;
		}
		m_wCurrentUser=wNextPlayer;
	}

	//��������
	CMD_S_AddScore AddScore;
	AddScore.lCurrentTimes=m_lCurrentTimes;
	AddScore.wCurrentUser=m_wCurrentUser;
	AddScore.wAddScoreUser=wChairID;
	AddScore.lAddScoreCount=lScore;

	//�ж�״̬
	AddScore.wCompareState=( bCompareCard )?TRUE:FALSE;

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(CMD_S_AddScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(CMD_S_AddScore));

	return true;
}

//////////////////////////////////////////////////////////////////////////
