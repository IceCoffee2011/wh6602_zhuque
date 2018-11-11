#include "StdAfx.h"
#include "TableFrameSink.h"
#include "..\�ͻ������\Mathaide.h"

//////////////////////////////////////////////////////////////////////////
//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_TimeControl;//��ʼģʽ

//////////////////////////////////////////////////////////////////////////
//����ʱ��
#define IDI_BUILD_TRACE				1									//�����켣
#define IDI_CLEAR_TRACE				2									//���ٹ켣
#define IDI_CHANGE_SCENE			3									//�л�����
#define IDI_REGULAR_FISH			4									//������Ⱥ
#define IDI_BONUS					5									//�����ʽ�
#define IDI_SYS_MESSAGE				6									//ϵͳ��Ϣ

#ifndef _DEBUG

#define TIME_BUILD_TRACE			3									//�����켣
#define TIME_CLEAR_TRACE			15									//���ٹ켣
#define TIME_CHANGE_SCENE			10*60								//�л�����
#define TIME_REGULAR_FISH			2									//������Ⱥ
#define TIME_SYS_MESSAGE			5*60								//ϵͳ��Ϣ

#else

#define TIME_BUILD_TRACE			3									//�����켣
#define TIME_CLEAR_TRACE			15									//���ٹ켣
#define TIME_CHANGE_SCENE			10*60								//�л�����
#define TIME_REGULAR_FISH			2									//������Ⱥ
#define TIME_SYS_MESSAGE			60									//ϵͳ��Ϣ

#endif

#define FISH_ALIVE_TIME				20*1000								//���ʱ��
//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;
	m_pITableFrameControl=NULL;
	m_CureSeaSceneKind=enSeaSceneKind_1;
	m_bWaveLToR=true;
	
	return;
}

//��������
CTableFrameSink::~CTableFrameSink()
{
}

//��ʼģʽ
enStartMode __cdecl CTableFrameSink::GetGameStartMode()
{
	return m_GameStartMode;
}

//�ӿڲ�ѯ
VOID * __cdecl CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

// GameSerivce.dll ͨ����Ϸ����˵�ͨ�������Դ����κμ������ݻ��Աָ��
bool CTableFrameSink::channel_to_game_service(int iMainCode, int iAssistantCode, void* pData, int iDataLen)
{
	switch(iMainCode)
	{
	case CTGS_MAIN_LOG:	// ��־
		{
			if(CTGS_ASS_LOG_POINT==iAssistantCode)	// ��־�����ָ��
			{
				if(pData)
				{
					m_pRoomLog = (cnd_ILogMgr*)pData;

					// ����ÿ�����Ӷ��ᵽ�ˣ���ֻ��дһ��
					static bool bArrow = true;
					if(bArrow && m_pRoomLog)
					{
						bArrow = !bArrow;

						m_pRoomLog->AddMsg(0, 0, TRUE, "***** ���ǡ��������� ��Ϸ�������־");
						m_pRoomLog->AddMsg(0, 0, TRUE, "***** ���ļ�����������ʱ��ʼ��¼");
						m_pRoomLog->AddMsg(0, 0, TRUE, "***** ��ֹͣ������߹ҵ�ʱ��ֹ");
						m_pRoomLog->AddMsg(0, 0, TRUE, "***** ÿ����¼ǰ���Ǽ�¼ʱ��");
						m_pRoomLog->AddMsg(0, 0, TRUE, "***** ���濪ʼ����������ļ�¼");
					}
				}
			}

			break;
		}
	default:
		{
			break;
		}
	}

	return true;
}

//��������
bool __cdecl CTableFrameSink::InitTableFrameSink(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//���ƽӿ�
	m_pITableFrameControl=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrameControl);
	if (m_pITableFrameControl==NULL) return false;

	//��ȡ����
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//��Ϸ����
	m_dwFishID=0L;
	ZeroMemory(m_lBulletCount,sizeof(m_lBulletCount));
	ZeroMemory(m_lCoinCount,sizeof(m_lCoinCount));
	ZeroMemory(m_lWinCoinCount,sizeof(m_lWinCoinCount));
	ZeroMemory(m_lUserShootCount,sizeof(m_lUserShootCount));
	ZeroMemory(m_wCptrFishCount,sizeof(m_wCptrFishCount));
	ZeroMemory(m_dwPlayTime,sizeof(m_dwPlayTime));
	m_bySysIndex=0;

	//�����ļ���
	lstrcpyn(m_szGameRoomName,m_pGameServiceOption->szGameRoomName,CountArray(m_szGameRoomName));
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);	
	_sntprintf(m_szIniFileName,CountArray(m_szIniFileName),TEXT("%s\\Fish.ini"),szPath);
		
	return true;
}

//��λ����
VOID __cdecl CTableFrameSink::RepositTableFrameSink()
{
	return;
}

//��Ϸ��ʼ
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	//��ʼ����
	m_ServerFishTrace.FreeAllItem();
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID) 
	{
		//�ͷ��ӵ�
		POSITION ps=m_ShootBullet[wChairID].GetStartPosition();
		DWORD dwBulletID=0L;
		tagBulletInfo *pBulletInfo=NULL;
		while (NULL!=ps)
		{
			m_ShootBullet[wChairID].GetNextAssoc(ps, dwBulletID, pBulletInfo);
			if (NULL!=pBulletInfo) m_ShootBulletFactory.FreeItem(pBulletInfo);
		}
		m_ShootBullet[wChairID].RemoveAll();
	}

	//���ñ���
	m_dwFishID=0L;
	m_dwSharkAppearTime=GetTickCount();
	m_dwGoldSharkAppearTime=GetTickCount();
	m_byRegFishColCount=0;
	m_nRegularSign=-1;
	m_byRegularForm=0;

	//��ȡ����
	ReadConfig();
	
	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//������Ϸ
			m_pITableFrame->ConcludeGame();
		
			return true;
		}
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//�رն�ʱ
			m_pITableFrame->KillGameTimer(IDI_BONUS);
			m_pITableFrame->KillGameTimer(IDI_BUILD_TRACE);
			m_pITableFrame->KillGameTimer(IDI_REGULAR_FISH);
			m_pITableFrame->KillGameTimer(IDI_CHANGE_SCENE);
					
			//������Ϸ
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_USER_LEFT:		//�û�ǿ��
		{
			//������Ϸ
			//m_pITableFrame->ConcludeGame();

			return true;
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//���ͳ���
bool __cdecl CTableFrameSink::SendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_FREE:		//����״̬
	case GS_PLAYING:	//��Ϸ״̬
		{
			//��Ϣ����
			BYTE byBuffer[2048]={0};
			WORD wSendSize=0;
			CMD_S_FishTrace *pFishTrace=(CMD_S_FishTrace *)byBuffer;

			//ö�ٱ���
			INT_PTR nEnumIndex=0;
			tagServerFishTrace *pServerFishTrace=NULL;

			do
			{
				//ö�ٹ켣
				pServerFishTrace=m_ServerFishTrace.EnumActiveObject(nEnumIndex++);
				if (NULL==pServerFishTrace) break;
				if ((wSendSize+sizeof(CMD_S_FishTrace))>sizeof(byBuffer)) break;

				//��������
				CopyMemory(pFishTrace->nInitX,pServerFishTrace->nInitX,sizeof(pFishTrace->nInitX));
				CopyMemory(pFishTrace->nInitY,pServerFishTrace->nInitY,sizeof(pFishTrace->nInitY));
				pFishTrace->dwFishID=pServerFishTrace->dwFishID;
				pFishTrace->FishKind=pServerFishTrace->FishKind;
				pFishTrace->nInitCount=pServerFishTrace->nInitCount;
				pFishTrace->bRegular=false;
				wSendSize+=sizeof(CMD_S_FishTrace);
				++pFishTrace;

				//��ֹ�ж�
				if (nEnumIndex>=10) break;

			} while(true);

			//��������
			if (wSendSize>0) m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_TRACE_POINT,byBuffer,wSendSize);

			//������Ϣ
			if (US_LOOKON!=pIServerUserItem->GetUserStatus())
			{
				CMD_S_BulletCount BulletCount={0};
				CMD_S_CoinCount CoinCount={0};
				for (WORD i=0; i<GAME_PLAYER; ++i)
				{
					//�����ж�
					if (NULL==m_pITableFrame->GetServerUserItem(i)) continue;

					////add by Ivan_han
					////�����ӵ�
					//SendBulletCount(i, enBulletCountKind_100, true);
					//SendBulletCount(i, enBulletCountKind_600, true);
					//SendBulletCount(i, enBulletCountKind_800, true);
					m_lBulletCount[i][enBulletCountKind_100]=1000;
					m_lBulletCount[i][enBulletCountKind_600]=6000;
					m_lBulletCount[i][enBulletCountKind_800]=8000;

					//�����ӵ�
					BulletCount.wChairID=i;
					BulletCount.BulletCountKind=enBulletCountKind_100;
					BulletCount.lBulletCount=m_lBulletCount[i][enBulletCountKind_100];
					m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_BULLET_COUNT,&BulletCount,sizeof(BulletCount));

					BulletCount.BulletCountKind=enBulletCountKind_600;
					BulletCount.lBulletCount=m_lBulletCount[i][enBulletCountKind_600];
					m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_BULLET_COUNT,&BulletCount,sizeof(BulletCount));

					BulletCount.BulletCountKind=enBulletCountKind_800;
					BulletCount.lBulletCount=m_lBulletCount[i][enBulletCountKind_800];
					m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_BULLET_COUNT,&BulletCount,sizeof(BulletCount));

					//���ͽ��
					CoinCount.wChairID=i;
					CoinCount.lCoinCount=m_lCoinCount[i];

					//������Ϣ
					m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_COIN_COUNT,&CoinCount,sizeof(CoinCount));
				}

				//������Ⱥ
				for (enFishKind FishKind=enFishKind_1; FishKind<enFishKind_No; FishKind=enFishKind(FishKind+1))
				{
					if (m_wCptrFishCount[wChairID][FishKind]>0) SendFishCount(wChairID, FishKind);
				}
			}

			//�ʽ���Ϣ
			if (m_lBonusOrigin>0L)
			{
				CMD_S_BonusInfo BonusInfo={0};
				BonusInfo.lBonus=m_lBonusOrigin+m_lBonusAccumulate[wChairID];
				m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_BONUS_INFO,&BonusInfo,sizeof(BonusInfo));
			}

			//���ͳ���
			CMD_S_GameScene GameScene={0};
			CopyMemory(GameScene.lBulletCharge,m_lBulletCharge,sizeof(GameScene.lBulletCharge));
			GameScene.CureSeaSceneKind=m_CureSeaSceneKind;
			CopyMemory(GameScene.lCellScore,m_lCellScore,sizeof(GameScene.lCellScore));
			CopyMemory(GameScene.lUserCellScore,m_lUserCellScore,sizeof(GameScene.lUserCellScore));
			return m_pITableFrame->SendGameScene(pIServerUserItem,&GameScene,sizeof(GameScene));
		}
	}

	//�������
	ASSERT(FALSE);

	return false;
}

//��ʱ���¼�
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	//�¼�����
	switch (wTimerID)
	{
	case IDI_SYS_MESSAGE:				//ϵͳ��Ϣ
		{
			//�����ж�
			const static BYTE byMaxIndex=100;
			if (m_bySysIndex>byMaxIndex) m_bySysIndex=0;

			//��ȡ��Ϣ
			static TCHAR szSysMessage[512], szIndex[32];
			ZeroMemory(szSysMessage,sizeof(szSysMessage));
			while (true)
			{
				_sntprintf(szIndex,CountArray(szIndex),TEXT("Message%d"),m_bySysIndex++);
				GetPrivateProfileString(m_szGameRoomName,szIndex,NULL,szSysMessage,CountArray(szSysMessage),m_szIniFileName);
				if (0!=szSysMessage[0] || m_bySysIndex>byMaxIndex) break;
			}

			//������Ϣ
			if (0!=szSysMessage[0])
			{
				//������Ϣ
				SendGameMessage(NULL,szSysMessage);
			}
			
			return true;
		}
	case IDI_BONUS:						//�����ʽ�
		{
			//��Ϣ����
			CMD_S_BonusInfo BonusInfo={0};

			//�ۼƲʽ�
			for (WORD wIndex=0; wIndex<GAME_PLAYER; ++wIndex)
			{
				//��ȡ���
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wIndex);
				if (NULL==pIServerUserItem) continue;

				//���Ӳʽ�
				if (m_lShootBulletCount[wIndex]>=m_lBulletLimit && m_lBonusMultipleUser[wIndex]<=m_lBonusMultipleMax)
				{				
					//���ñ���
					++m_lBonusMultipleUser[wIndex];
					m_lBonusAccumulate[wIndex]=m_lBonusMultiple*m_lBonusMultipleUser[wIndex];

					//������Ŀ
					m_lShootBulletCount[wIndex]=0L;
				}

				//������Ϣ
				BonusInfo.lBonus=m_lBonusAccumulate[wIndex]+m_lBonusOrigin;
				m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_BONUS_INFO,&BonusInfo,sizeof(BonusInfo));
			}

			//��ȡ����
			tagServerFishTrace *pServerFishTrace=m_ServerFishTrace.ActiveItem();
			ASSERT(NULL!=pServerFishTrace);
			if (NULL==pServerFishTrace) return true;

			//�����켣
			bool bResult=BuildTracePointLinearBonus(pServerFishTrace);

			//�ɹ��ж�
			ASSERT(bResult);
			if (false==bResult) return true;

			//��Ϣ����
			CMD_S_FishTrace FishTrace={0};

			//��������
			CopyMemory(FishTrace.nInitX,pServerFishTrace->nInitX,sizeof(FishTrace.nInitX));
			CopyMemory(FishTrace.nInitY,pServerFishTrace->nInitY,sizeof(FishTrace.nInitY));
			FishTrace.dwFishID=pServerFishTrace->dwFishID;
			FishTrace.FishKind=pServerFishTrace->FishKind;
			FishTrace.nInitCount=pServerFishTrace->nInitCount;

			//��������
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRACE_POINT,&FishTrace,sizeof(FishTrace));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRACE_POINT,&FishTrace,sizeof(FishTrace));

			return true;
		}
	case IDI_BUILD_TRACE:			//�����켣
		{
			//��Ϣ����
			BYTE byBuffer[2048]={0};
			WORD wSendSize=0;
			CMD_S_FishTrace *pFishTrace=(CMD_S_FishTrace *)byBuffer;

			int nSameCount=rand()%6;
			tagServerFishTrace *pServerFishTraceLast=NULL;
			for (int i=0; i<15; ++i)
			{
				//�ж��ж�
				if ((wSendSize+sizeof(CMD_S_FishTrace))>sizeof(byBuffer)) break;

				//��ȡ����
				tagServerFishTrace *pServerFishTrace=m_ServerFishTrace.ActiveItem();
				ASSERT(NULL!=pServerFishTrace);
				if (NULL==pServerFishTrace) break;

				//�����켣
				bool bResult=BuildTracePointLinear(pServerFishTrace,pServerFishTraceLast);

				//�ɹ��ж�
				ASSERT(bResult);
				if (true==bResult) 
				{
					//��������
					CopyMemory(pFishTrace->nInitX,pServerFishTrace->nInitX,sizeof(pFishTrace->nInitX));
					CopyMemory(pFishTrace->nInitY,pServerFishTrace->nInitY,sizeof(pFishTrace->nInitY));
					pFishTrace->dwFishID=pServerFishTrace->dwFishID;
					pFishTrace->FishKind=pServerFishTrace->FishKind;
					pFishTrace->nInitCount=pServerFishTrace->nInitCount;
					wSendSize+=sizeof(CMD_S_FishTrace);
					++pFishTrace;
				}
				else
				{
					m_ServerFishTrace.FreeItem(pServerFishTrace);
					break;
				}

				//��ͬ�ж�
				if (nSameCount>0 && pServerFishTrace->FishKind<=enFishKind_2)
				{
					--nSameCount;
					pServerFishTraceLast=pServerFishTrace;
				}
				else
				{
					pServerFishTraceLast=NULL;
				}
			}

			//��������
			if (wSendSize>0)
			{
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRACE_POINT,byBuffer,wSendSize);
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRACE_POINT,byBuffer,wSendSize);
			}

			//����ʱ��
			if (0!=wBindParam) m_pITableFrame->SetGameTimer(IDI_BUILD_TRACE,TIME_BUILD_TRACE*1000,TIMES_INFINITY,0L);

			return true;
		}
	case IDI_CLEAR_TRACE:			//���ٹ켣
		{
			//ö�ٱ���
			INT_PTR nEnumIndex=0;
			tagServerFishTrace *pServerFishTrace=NULL;
			DWORD dwNowTime=GetTickCount();

			do
			{
				//ö�ٹ켣
				pServerFishTrace=m_ServerFishTrace.EnumActiveObject(nEnumIndex);
				if (NULL==pServerFishTrace) break;

				//�ж�ʱ��
				if (dwNowTime>=(pServerFishTrace->dwBuildTime+FISH_ALIVE_TIME))
				{
					m_ServerFishTrace.FreeItem(pServerFishTrace);
					continue;
				}

				//��������
				++nEnumIndex;
			} while(true);

			return true;
		}
	case IDI_CHANGE_SCENE:			//�л�����
		{
			//�ı䳡��
			m_CureSeaSceneKind=enSeaSceneKind((m_CureSeaSceneKind+1)%enSeaSceneKind_No);
			m_bWaveLToR=rand()%2?true:false;

			//��������
			CMD_S_ChangeScene ChangeScene;

			//���ñ���
			ChangeScene.bChange=true;
			ChangeScene.CureSeaSceneKind=m_CureSeaSceneKind;
			ChangeScene.bLToR=m_bWaveLToR;

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_SCENE,&ChangeScene,sizeof(ChangeScene));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_SCENE,&ChangeScene,sizeof(ChangeScene));

			//�ر�ʱ��(��ֹ���л������ͷ����µ���Ⱥ)
			m_pITableFrame->KillGameTimer(IDI_BUILD_TRACE);

			//������Ⱥ
			m_pITableFrame->SetGameTimer(IDI_REGULAR_FISH,10*1000,1,1L);

			//������Ŀ
			m_byRegFishColCount=rand()%5+8;

			//������
			m_nRegularSign=(rand()%2)?(1):(-1);

			//������ʽ
			m_byRegularForm=rand()%3;

			return true;
		}
	case IDI_REGULAR_FISH:			//������Ⱥ
		{
			//ö�ٱ���
			INT_PTR nEnumIndex=m_ServerFishTrace.GetActiveCount();//֮ǰ����Ⱥ����Ҫ�ظ�����
			tagServerFishTrace *pServerFishTrace=NULL;

			//����Ⱥ��
			BuildRegularFish((wBindParam<m_byRegFishColCount)?false:true);

			//��Ϣ����
			BYTE byBuffer[2048]={0};
			WORD wSendSize=0;
			CMD_S_FishTrace *pFishTrace=(CMD_S_FishTrace *)byBuffer;

			do
			{
				//ö�ٹ켣
				pServerFishTrace=m_ServerFishTrace.EnumActiveObject(nEnumIndex++);
				if (NULL==pServerFishTrace) break;
				if ((wSendSize+sizeof(CMD_S_FishTrace))>sizeof(byBuffer))
				{
					m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRACE_POINT,byBuffer,wSendSize);
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRACE_POINT,byBuffer,wSendSize);
					wSendSize=0;
					pFishTrace=(CMD_S_FishTrace *)byBuffer;
				}

				//��������
				CopyMemory(pFishTrace->nInitX,pServerFishTrace->nInitX,sizeof(pFishTrace->nInitX));
				CopyMemory(pFishTrace->nInitY,pServerFishTrace->nInitY,sizeof(pFishTrace->nInitY));
				pFishTrace->dwFishID=pServerFishTrace->dwFishID;
				pFishTrace->FishKind=pServerFishTrace->FishKind;
				pFishTrace->nInitCount=pServerFishTrace->nInitCount;
				pFishTrace->bRegular=true;
				wSendSize+=sizeof(CMD_S_FishTrace);
				++pFishTrace;

			} while(true);

			//��������
			if (wSendSize>0)
			{
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRACE_POINT,byBuffer,wSendSize);
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRACE_POINT,byBuffer,wSendSize);
			}

			//�����ж�
			if (wBindParam>=m_byRegFishColCount)
			{
				//����ʱ��
				m_pITableFrame->SetGameTimer(IDI_BUILD_TRACE,20*1000,TIMES_INFINITY,1L);

				//�ر�ʱ��
				m_pITableFrame->KillGameTimer(IDI_REGULAR_FISH);
			}
			else
			{
				m_pITableFrame->SetGameTimer(IDI_REGULAR_FISH,TIME_REGULAR_FISH*1000,1,wBindParam+1);
			}

			return true;
		}
	}
	return false;
}

//��Ϸ��Ϣ
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//��Ϣ����
	switch (wSubCmdID)
	{
	case SUB_C_BUY_BULLET:				//�����ӵ�
		{
			//�Ϸ��ж�
			ASSERT(wDataSize==sizeof(CMD_C_BuyBullet));
			if (wDataSize!=sizeof(CMD_C_BuyBullet)) return false;

			//����ת��
			CMD_C_BuyBullet *pBuyBullet=(CMD_C_BuyBullet*)pData;

			//�Ϸ��ж�
			ASSERT(pBuyBullet->BulletCountKind<enBulletCountKind_End && pBuyBullet->BulletCountKind>=enBulletCountKind_100);
			if (!(pBuyBullet->BulletCountKind<enBulletCountKind_End && pBuyBullet->BulletCountKind>=enBulletCountKind_100)) 
			{
				TraceString(TEXT("(!(pBuyBullet->BulletCountKind<enBulletCountKind_End && pBuyBullet->BulletCountKind>=enBulletCountKind_100))"));
				return false;
			}
			
			return OnSubBuyBullet(pIServerUserItem, pBuyBullet->BulletCountKind);
		}
	case SUB_C_USER_SHOOT:
		{
			//�Ϸ��ж�
			ASSERT(wDataSize==sizeof(CMD_C_UserShoot));
			if (wDataSize!=sizeof(CMD_C_UserShoot)) return false;

			//����ת��
			CMD_C_UserShoot *pUserShoot=(CMD_C_UserShoot*)pData;

			////�Ϸ��ж�
			//ASSERT(pUserShoot->fAngle>=ME_MIN_CAN_ANGLE && pUserShoot->fAngle<=ME_MAX_CAN_ANGLE);
			//if (!(pUserShoot->fAngle>=ME_MIN_CAN_ANGLE && pUserShoot->fAngle<=ME_MAX_CAN_ANGLE)) return false;

			//�����ڵ�
			return OnSubUserShoot(pIServerUserItem, pUserShoot->dwBulletID, pUserShoot->fAngle, pUserShoot->BulletCountKind, pUserShoot->byShootCount);
		}
	case SUB_C_HIT_FISH:
		{
			//�Ϸ��ж�
			ASSERT(wDataSize==sizeof(CMD_C_HitFish));
			if (wDataSize!=sizeof(CMD_C_HitFish)) return false;

			//����ת��
			CMD_C_HitFish *pHitFish=(CMD_C_HitFish*)pData;			

			//�����ڵ�
			return OnSubHitFish(pIServerUserItem, pHitFish->dwFishID, pHitFish->dwBulletID);
		}
	//case SUB_C_CAPTURE_FISH:
	//	{
	//		//�Ϸ��ж�
	//		ASSERT(wDataSize==sizeof(CMD_C_CaptureFish));
	//		if (wDataSize!=sizeof(CMD_C_CaptureFish)) return false;

	//		//����ת��
	//		CMD_C_CaptureFish *pCaptureFish=(CMD_C_CaptureFish*)pData;

	//		//�Ϸ��ж�
	//		ASSERT(pCaptureFish->byBulletCount<=5);
	//		if (!(pCaptureFish->byBulletCount<=5)) 
	//		{
	//			TraceString(TEXT("(!(pCaptureFish->byBulletCount<=5))"));
	//			return false;
	//		}

	//		//�����ڵ�
	//		return OnSubCaptureFish(pIServerUserItem, pCaptureFish->dwFishID, pCaptureFish->dwBulletID, pCaptureFish->byBulletCount, pCaptureFish->byCptrProbability);
	//	}
	case SUB_C_CALC_SCORE:
		{
			//�������
			return OnSubCalcScore(pIServerUserItem);
		}
	case SUB_C_GET_SCORE_INFO:
		{
			//��ȡ����
			return OnSubGetScoreInfo(pIServerUserItem);
		}		
	case SUB_C_SET_CELL_SCORE:
		{
			//�Ϸ��ж�
			ASSERT(wDataSize==sizeof(CMD_C_SetCellScore));
			if (wDataSize!=sizeof(CMD_C_SetCellScore)) return false;

			//����ת��
			CMD_C_SetCellScore *pSetCellScore=(CMD_C_SetCellScore*)pData;

			//���õ�Ԫ
			return OnSubSetCellScore(pIServerUserItem,pSetCellScore->lCellScore);
		}
	case SUB_C_SET_PROPABILITY:
		{
			//�Ϸ��ж�
			ASSERT(wDataSize==sizeof(CMD_C_SetProbability));
			if (wDataSize!=sizeof(CMD_C_SetProbability)) return false;

			//����ת��
			CMD_C_SetProbability *pSetProbability=(CMD_C_SetProbability*)pData;

			for (int i=0; i<5; ++i)
			{
				for (int j=0; j<enFishKind_No; ++j)
				{
					m_byCptrProbability[i][j]=pSetProbability->byCptrProbability[i][j];
				}
			}
			return true;
		}
	}
	return false;
}

//�����Ϣ
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʼ�ж�
	if (false==bLookonUser && m_pITableFrame->GetGameStatus()==GS_FREE)
	{
		//��ʼ��Ϸ
		m_pITableFrameControl->StartGame();

		//����ʱ��
		m_pITableFrame->SetGameTimer(IDI_BUILD_TRACE,TIME_BUILD_TRACE*1000,TIMES_INFINITY,0L);
		m_pITableFrame->SetGameTimer(IDI_CLEAR_TRACE,TIME_CLEAR_TRACE*1000,TIMES_INFINITY,0L);
		m_pITableFrame->SetGameTimer(IDI_CHANGE_SCENE,TIME_CHANGE_SCENE*1000,TIMES_INFINITY,0L);
						
		//����״̬
		m_pITableFrame->SetGameStatus(GS_PLAYING);
	}

	//����ʱ��
	if (false==bLookonUser)
	{
		m_dwPlayTime[wChairID]=(DWORD)time(NULL);
		m_lWinCoinCount[wChairID]=0L;
		m_lUserShootCount[wChairID]=0L;
	}

	return true;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//�����ж�
	if (false==bLookonUser)
	{
		//�������
		CalcScore(pIServerUserItem, wChairID);

		//�����Ŀ
		WORD wUserCount=0;
		for (WORD wIndex=0; wIndex<GAME_PLAYER; ++wIndex)
		{
			if (NULL!=m_pITableFrame->GetServerUserItem(wIndex)) ++wUserCount;
		}

		//û�����
		if (0==wUserCount) 
		{
			//������Ϸ
			m_pITableFrame->ConcludeGame();

			//ȡ��ʱ��
			m_pITableFrame->KillGameTimer(IDI_BUILD_TRACE);	
			m_pITableFrame->KillGameTimer(IDI_CLEAR_TRACE);
			m_pITableFrame->KillGameTimer(IDI_CHANGE_SCENE);
			
			//����״̬
			m_pITableFrame->SetGameStatus(GS_FREE);

			//�رն�ʱ
			m_pITableFrame->KillGameTimer(IDI_BONUS);
			m_pITableFrame->KillGameTimer(IDI_SYS_MESSAGE);			
		}

		//���ñ���
		m_lUserCellScore[wChairID]=0L;
		m_dwPlayTime[wChairID]=(DWORD)time(NULL);
		m_lBonusAccumulate[wChairID]=0L;
		m_lShootBulletCount[wChairID]=0L;
		m_lBonusMultipleUser[wChairID]=0L;
	}

	return true;
}

//�����ӵ�
bool CTableFrameSink::OnSubBuyBullet(IServerUserItem * pIServerUserItem, enBulletCountKind BulletCountKind)
{
	//�Ϸ��ж�
	ASSERT(NULL!=pIServerUserItem);
	if (NULL==pIServerUserItem) return false;

	//���Ӻ���
	WORD wChairID=pIServerUserItem->GetChairID();
	if (wChairID>=GAME_PLAYER) return false;

	//�����ж�
	LONG lBulletCount=0;
	LONGLONG lCharge=m_lBulletCharge[BulletCountKind]*m_lUserCellScore[wChairID];
	if (enBulletCountKind_100==BulletCountKind)
	{
		lBulletCount=100;
	}
	else if (enBulletCountKind_600==BulletCountKind)
	{
		lBulletCount=600;
	}
	else if (enBulletCountKind_800==BulletCountKind)
	{
		lBulletCount=800;
	}
	else
	{
		ASSERT(FALSE);
		TraceString(TEXT("OnSubBuyBullet else"));
		return false;
	}

	//�۷Ѵ���
	LONGLONG lGold=pIServerUserItem->GetUserScore()->lGold;//Update<cxf>;DateTime<2012-03-27>;Description<���½������>
	ASSERT(lGold>=lCharge && lCharge>0L);
	if (!(lGold>=lCharge && lCharge>0L))
	{
		//TraceString(TEXT("(!(lGold>=lCharge))"));
		return true;
	}
	m_pITableFrame->WriteUserScore(pIServerUserItem,-lCharge,0L,enScoreKind_Service,0L);

	//������Ŀ	
	m_lBulletCount[wChairID][BulletCountKind]+=lBulletCount;

	//�����ӵ�
	SendBulletCount(wChairID, BulletCountKind);

	return true;
}

//�����켣
bool CTableFrameSink::BuildTracePointLinearBonus(tagServerFishTrace *pServerFishTraceNew)
{
	//�Ϸ��ж�
	ASSERT(NULL!=pServerFishTraceNew);
	if (NULL==pServerFishTraceNew) return false;

	//��ʼ����
	ZeroMemory(pServerFishTraceNew,sizeof(pServerFishTraceNew));

	//��������
	pServerFishTraceNew->FishKind=enFishKind_12;

	//�����ؼ���
	int const nShootRoundW[enFishKind_No]={40,56,68,56,68,88,72,80,166,180,354,368};
	int const nExcursioinX=nShootRoundW[pServerFishTraceNew->FishKind]/2,nExcursioinY=nShootRoundW[pServerFishTraceNew->FishKind]/2;
	bool bLeftToRight=(rand()%2)?true:false;
	if (bLeftToRight)
	{
		pServerFishTraceNew->nInitX[0]=0-nExcursioinX;
		pServerFishTraceNew->nInitX[1]=VIEW_WIDTH+nExcursioinX;
		pServerFishTraceNew->nInitY[0]=rand()%VIEW_HEIGHT;
		pServerFishTraceNew->nInitY[1]=rand()%VIEW_HEIGHT;
		pServerFishTraceNew->nInitCount=2;
	}
	else
	{
		pServerFishTraceNew->nInitX[0]=rand()%VIEW_WIDTH;
		pServerFishTraceNew->nInitX[1]=rand()%VIEW_WIDTH;
		pServerFishTraceNew->nInitY[0]=0-nExcursioinY;
		pServerFishTraceNew->nInitY[1]=VIEW_HEIGHT+nExcursioinY;
		pServerFishTraceNew->nInitCount=2;
	}

	//�����ж�
	bool bChangePoint=(rand()%2)?true:false;
	if (bChangePoint)
	{
		int nTmp=pServerFishTraceNew->nInitX[0];
		pServerFishTraceNew->nInitX[0]=pServerFishTraceNew->nInitX[1];
		pServerFishTraceNew->nInitX[1]=nTmp;

		nTmp=pServerFishTraceNew->nInitY[0];
		pServerFishTraceNew->nInitY[0]=pServerFishTraceNew->nInitY[1];
		pServerFishTraceNew->nInitY[1]=nTmp;
	}

	//����λ��
	if (pServerFishTraceNew->nInitY[0]>=VIEW_HEIGHT) RectifyTracePosX(pServerFishTraceNew->nInitX[0]);
	if (pServerFishTraceNew->nInitY[1]>=VIEW_HEIGHT) RectifyTracePosX(pServerFishTraceNew->nInitX[1]);
	if (pServerFishTraceNew->nInitY[0]<=0) RectifyTracePosX(pServerFishTraceNew->nInitX[0]);
	if (pServerFishTraceNew->nInitY[1]<=0) RectifyTracePosX(pServerFishTraceNew->nInitX[1]);

	//����ʱ��
	pServerFishTraceNew->dwBuildTime=GetTickCount();

	//��Ⱥ��ʶ
	pServerFishTraceNew->dwFishID=GetNewFishID();

	return true;
}

//�����켣
bool CTableFrameSink::BuildTracePointLinear(tagServerFishTrace *pServerFishTraceNew, tagServerFishTrace *pServerFishTraceLast)
{
	//�Ϸ��ж�
	ASSERT(NULL!=pServerFishTraceNew);
	if (NULL==pServerFishTraceNew) return false;

	//��ʼ����
	ZeroMemory(pServerFishTraceNew,sizeof(pServerFishTraceNew));

	if (NULL==pServerFishTraceLast)
	{
		//��������
		pServerFishTraceNew->FishKind=enFishKind(enFishKind_1+rand()%enFishKind_12);

		//�����ж�
		DWORD dwNowTime=GetTickCount();
		if (enFishKind_11==pServerFishTraceNew->FishKind)
		{
			if ((m_dwSharkAppearTime+20*1000)>dwNowTime)
				pServerFishTraceNew->FishKind=enFishKind(enFishKind_1+rand()%enFishKind_11);
			else
				m_dwSharkAppearTime=dwNowTime;
		}
		/*else if (enFishKind_12==pServerFishTraceNew->FishKind)
		{
			if ((m_dwGoldSharkAppearTime+60*1000)>dwNowTime)
				pServerFishTraceNew->FishKind=enFishKind(enFishKind_1+rand()%enFishKind_11);
			else
				m_dwGoldSharkAppearTime=dwNowTime;
		}*/

		//�����ؼ���
		int const nShootRoundW[enFishKind_No]={40,56,68,56,68,88,72,80,166,180,354,368};
		int const nExcursioinX=nShootRoundW[pServerFishTraceNew->FishKind]/2,nExcursioinY=nShootRoundW[pServerFishTraceNew->FishKind]/2;
		bool bLeftToRight=(rand()%2)?true:false;
		if (bLeftToRight)
		{
			pServerFishTraceNew->nInitX[0]=0-nExcursioinX;
			pServerFishTraceNew->nInitX[1]=VIEW_WIDTH+nExcursioinX;
			pServerFishTraceNew->nInitY[0]=rand()%VIEW_HEIGHT;
			pServerFishTraceNew->nInitY[1]=rand()%VIEW_HEIGHT;
			pServerFishTraceNew->nInitCount=2;
		}
		else
		{
			pServerFishTraceNew->nInitX[0]=rand()%VIEW_WIDTH;
			pServerFishTraceNew->nInitX[1]=rand()%VIEW_WIDTH;
			pServerFishTraceNew->nInitY[0]=0-nExcursioinY;
			pServerFishTraceNew->nInitY[1]=VIEW_HEIGHT+nExcursioinY;
			pServerFishTraceNew->nInitCount=2;
		}

		//�����ж�
		bool bChangePoint=(rand()%2)?true:false;
		if (bChangePoint)
		{
			int nTmp=pServerFishTraceNew->nInitX[0];
			pServerFishTraceNew->nInitX[0]=pServerFishTraceNew->nInitX[1];
			pServerFishTraceNew->nInitX[1]=nTmp;

			nTmp=pServerFishTraceNew->nInitY[0];
			pServerFishTraceNew->nInitY[0]=pServerFishTraceNew->nInitY[1];
			pServerFishTraceNew->nInitY[1]=nTmp;
		}

		//����λ��
		if (pServerFishTraceNew->nInitY[0]>=VIEW_HEIGHT) RectifyTracePosX(pServerFishTraceNew->nInitX[0]);
		if (pServerFishTraceNew->nInitY[1]>=VIEW_HEIGHT) RectifyTracePosX(pServerFishTraceNew->nInitX[1]);
		if (pServerFishTraceNew->nInitY[0]<=0) RectifyTracePosX(pServerFishTraceNew->nInitX[0]);
		if (pServerFishTraceNew->nInitY[1]<=0) RectifyTracePosX(pServerFishTraceNew->nInitX[1]);
	}
	else
	{
		int const static nSpace=15;
		if (0==pServerFishTraceLast->nInitX[0] && VIEW_WIDTH==pServerFishTraceLast->nInitX[1])
		{
			pServerFishTraceNew->nInitX[0]=0;
			pServerFishTraceNew->nInitX[1]=VIEW_WIDTH;
			pServerFishTraceNew->nInitY[0]=pServerFishTraceLast->nInitY[0]+nSpace;
			pServerFishTraceNew->nInitY[1]=pServerFishTraceLast->nInitY[1]+nSpace;
			pServerFishTraceNew->nInitCount=2;
		}
		else
		{
			pServerFishTraceNew->nInitX[0]=pServerFishTraceLast->nInitX[0]+nSpace;
			pServerFishTraceNew->nInitX[1]=pServerFishTraceLast->nInitX[1]+nSpace;
			pServerFishTraceNew->nInitY[0]=0;
			pServerFishTraceNew->nInitY[1]=VIEW_HEIGHT;
			pServerFishTraceNew->nInitCount=2;
		}

		//��Ⱥ����
		pServerFishTraceNew->FishKind=pServerFishTraceLast->FishKind;
	}

	//����ʱ��
	pServerFishTraceNew->dwBuildTime=GetTickCount();

	//��Ⱥ��ʶ
	pServerFishTraceNew->dwFishID=GetNewFishID();

#ifdef _DEBUG
	//pServerFishTraceNew->FishKind=enFishKind_11;
#endif
	return true;
}

//�����ڵ�
bool CTableFrameSink::OnSubUserShoot(IServerUserItem * pIServerUserItem, DWORD dwBulletID, float fAngle, enBulletCountKind BulletCountKind, BYTE byShootCount)
{
	//�Ϸ��ж�
	ASSERT(NULL!=pIServerUserItem);
	ASSERT(0L!=dwBulletID);	
	//ASSERT(m_lBulletCount[pIServerUserItem->GetChairID()][BulletCountKind]>=byShootCount);
	if (NULL==pIServerUserItem) return false;
	if (0L==dwBulletID) 
	{
		TraceString(TEXT("(0L==dwBulletID)"));
		return false;
	}
	if (!(m_lBulletCount[pIServerUserItem->GetChairID()][BulletCountKind]>=byShootCount))
	{
		//�����ӵ�
		SendBulletCount(pIServerUserItem->GetChairID(), BulletCountKind);

		//TraceString(TEXT("if (!(m_lBulletCount[pIServerUserItem->GetChairID()][BulletCountKind]>=byShootCount))"));

		return true;
	}

	//�������
	WORD wChairID=pIServerUserItem->GetChairID();

	//�ݼ���Ŀ
	m_lBulletCount[wChairID][BulletCountKind]-=byShootCount;

	//������Ŀ
	m_lShootBulletCount[wChairID]+=byShootCount;
	m_lUserShootCount[wChairID]+=byShootCount;

	//������Ϣ
	CMD_S_UserShoot UserShoot={0};
	UserShoot.wChairID=wChairID;
	UserShoot.fAngle=fAngle;
	UserShoot.byShootCount=byShootCount;
	UserShoot.BulletCountKind=BulletCountKind;
	SendDataExcludeSourceUser(pIServerUserItem,SUB_S_USER_SHOOT,&UserShoot,sizeof(UserShoot));

	//�����ӵ�
	SendBulletCount(wChairID, BulletCountKind, false);

	//��¼ʱ��
	tagBulletInfo *pBulletInfo=m_ShootBulletFactory.ActiveItem();
	ZeroMemory(pBulletInfo, sizeof(tagBulletInfo));
	pBulletInfo->dwShootTime=GetTickCount();
	pBulletInfo->byBulletCount=byShootCount;
	m_ShootBullet[wChairID][dwBulletID]=pBulletInfo;

	return true;
}

//��׽��Ⱥ
bool CTableFrameSink::OnSubHitFish(IServerUserItem * pIServerUserItem, DWORD dwFishID, DWORD dwBulletID)
{
	//�Ϸ��ж�
	ASSERT(NULL!=pIServerUserItem);
	if (NULL==pIServerUserItem) return true;

	//�Ϸ��ж�
	ASSERT(0L!=dwFishID && 0L!=dwBulletID);
	if (!(0L!=dwFishID && 0L!=dwBulletID)) return true;

	//��Ⱥ����
	tagServerFishTrace *pServerFishTrace=GetFishTraceInfo(dwFishID);
	if (NULL==pServerFishTrace) return true;
	if (pServerFishTrace->FishKind>=enFishKind_No) return true;

	//�������
	WORD wChairID=pIServerUserItem->GetChairID();

	//�ӵ�����
	tagBulletInfo *pBulletInfo=NULL;
	if (FALSE==m_ShootBullet[wChairID].Lookup(dwBulletID,pBulletInfo)) return true;

	//�Ϸ��ж�
	ASSERT(pBulletInfo->byBulletCount<=MAX_SHOOT_BULLET_COUNT);
	if (!(pBulletInfo->byBulletCount<=MAX_SHOOT_BULLET_COUNT)) return true;

	//�Ϸ��ж�
	ASSERT(pBulletInfo->byBulletCount<=MAX_SHOOT_BULLET_COUNT);
	if (!(pBulletInfo->byBulletCount<=MAX_SHOOT_BULLET_COUNT)) return true;

	//�����ӵ�������Ŀ
	if (pBulletInfo->byCptrFishCount>=4) 
	{
		m_ShootBulletFactory.FreeItem(pBulletInfo);
		m_ShootBullet[wChairID].RemoveKey(dwBulletID);

		return true;
	}

	//�����ж�
	if (pBulletInfo->FishKind>=enFishKind_6)
	{
		m_ShootBulletFactory.FreeItem(pBulletInfo);
		m_ShootBullet[wChairID].RemoveKey(dwBulletID);

		return true;
	}

	//ʱ���ж�
	if ((pBulletInfo->dwShootTime+15000)<=GetTickCount()) 
	{
		m_ShootBulletFactory.FreeItem(pBulletInfo);
		m_ShootBullet[wChairID].RemoveKey(dwBulletID);		

		TraceString(pIServerUserItem->GetAccounts());

		return true;
	}
	
	//���и���
	BYTE byCptrProbability=m_byCptrProbability[pBulletInfo->byBulletCount-1][pServerFishTrace->FishKind];

	//��������
	if (true==m_bControlScore)
	{
		//�ʽ��ж�
		if (enFishKind_12==pServerFishTrace->FishKind && m_lWinCoinCount[wChairID]>=m_byWindMulLimitBonus*m_lUserShootCount[wChairID]) return true;

		bool bRectify=false;
		if (m_lUserShootCount[wChairID]>=100)
		{
			//���߸���
			if (m_lWinCoinCount[wChairID]<=((m_byWindMulLimitMin/100.f)*m_lUserShootCount[wChairID])) 
			{
				byCptrProbability+=(byCptrProbability/5);
				bRectify=true;
			}
			else if (m_lWinCoinCount[wChairID]>=5*m_lUserShootCount[wChairID]) 
			{
				TraceString(pIServerUserItem->GetAccounts());

				byCptrProbability=0;//�����ж�
				bRectify=true;
			}
			else if (m_lWinCoinCount[wChairID]>=m_byWindMulLimitMax*m_lUserShootCount[wChairID]) 
			{
				byCptrProbability-=(byCptrProbability/5);//���͸���
				bRectify=true;
			}			
		}

		if (false==bRectify) 
		{
			//��Ԫ����
			int nCellIndex=0;
			for (int i=0; i<CountArray(m_lCellScore); ++i)
			{
				if (m_lUserCellScore[wChairID]==m_lCellScore[i])
				{
					nCellIndex=i;
					break;
				}
			}

			if (nCellIndex<CountArray(m_byCellScoreRectify) && m_byCellScoreRectify[nCellIndex]>0)
			{
				float fPrecent=m_byCellScoreRectify[nCellIndex]/100.f;
				byCptrProbability-=BYTE((byCptrProbability*fPrecent));//���͸���
			}
		}
	}

	//���и���
	BYTE byRandPrecent=rand()%200+1;
	if (byRandPrecent>byCptrProbability) return true;

	//������Ŀ
	if (pBulletInfo->byBulletCount<255) ++pBulletInfo->byCptrFishCount;
	pBulletInfo->FishKind=max(pBulletInfo->FishKind, pServerFishTrace->FishKind);
	
	//������Ϣ
	CMD_S_CaptureFish CaptureFish={0};
	CaptureFish.dwFishID=dwFishID;
	CaptureFish.wChairID=wChairID;
	CaptureFish.FishKind=pServerFishTrace->FishKind;
	CaptureFish.lFishScore=(enFishKind_12==pServerFishTrace->FishKind)?(m_lBonusOrigin+m_lBonusAccumulate[wChairID]):(g_nFishScore[pServerFishTrace->FishKind]);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CAPTURE_FISH,&CaptureFish,sizeof(CaptureFish));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CAPTURE_FISH,&CaptureFish,sizeof(CaptureFish));

	//������Ⱥ
	m_ServerFishTrace.FreeItem(pServerFishTrace);

	//�����Ŀ
	if (enFishKind_12==pServerFishTrace->FishKind)
	{
		//���ý��
		m_lCoinCount[wChairID]+=(m_lBonusOrigin+m_lBonusAccumulate[wChairID]);
		m_lWinCoinCount[wChairID]+=(m_lBonusOrigin+m_lBonusAccumulate[wChairID]);

		//��Ϣ����
		CMD_S_BonusInfo BonusInfo={0};

		//������Ϣ
		BonusInfo.lBonus=m_lBonusOrigin;
		m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_BONUS_INFO,&BonusInfo,sizeof(BonusInfo));
		m_pITableFrame->SendLookonData(wChairID,SUB_S_BONUS_INFO,&BonusInfo,sizeof(BonusInfo));

		//���ñ���
		m_lBonusMultipleUser[wChairID]=0L;
		m_lBonusAccumulate[wChairID]=0L;
	}
	else
	{
		m_lCoinCount[wChairID]+=g_nFishScore[pServerFishTrace->FishKind];
		m_lWinCoinCount[wChairID]+=g_nFishScore[pServerFishTrace->FishKind];
	}			

	//������Ŀ
	m_wCptrFishCount[wChairID][pServerFishTrace->FishKind]+=1;

	//���ͽ��
	SendCoinCount(wChairID);

	//������Ⱥ
	SendFishCount(wChairID, pServerFishTrace->FishKind);

	return true;
}

////��׽��Ⱥ
//bool CTableFrameSink::OnSubCaptureFish(IServerUserItem * pIServerUserItem, DWORD dwFishID, DWORD dwBulletID, BYTE byBulletCount, BYTE byCptrProbability)
//{
//	//�Ϸ��ж�
//	ASSERT(NULL!=pIServerUserItem);
//	ASSERT(0L!=dwBulletID && 0L!=dwFishID);
//	if (!(0L!=dwBulletID && 0L!=dwFishID)) return 0L;
//	if (NULL==pIServerUserItem) return false;
//
//	//�������
//	WORD wChairID=pIServerUserItem->GetChairID();
//
//	//ʱ���ж�
//	DWORD dwShootTime=0L;
//	if (FALSE==m_ShootBullet[wChairID].Lookup(dwBulletID,dwShootTime))
//	{
//	/*	ASSERT(FALSE);
//		TraceString(TEXT("(FALSE==m_ShootBullet[wChairID].Lookup(dwBulletID,dwShootTime))"));*/
//
//		return true;
//	}
//	if ((dwShootTime+10000)<=GetTickCount()) return true;
//
//	//ö�ٱ���
//	INT_PTR nEnumIndex=0;
//	tagServerFishTrace *pServerFishTrace=NULL;
//	bool bFind=false;
//
//	//TraceString(TEXT("�յ���Ϣ"));
//
//	do
//	{
//		//ö�ٹ켣
//		pServerFishTrace=m_ServerFishTrace.EnumActiveObject(nEnumIndex++);
//		if (NULL==pServerFishTrace) break;
//
//		//����ж�
//		if (dwFishID==pServerFishTrace->dwFishID)
//		{
//			//TraceString(TEXT("�д���"));
//
//			////λ���ж�
//			//for (int i=0; i<pServerFishTrace->nTracePtCount; ++i)
//			//{
//			//	if (lFishPos==MAKELPARAM(pServerFishTrace->ptTrace[i].x,pServerFishTrace->ptTrace[i].y))
//			//	{
//			//		//���ñ�־
//			//		bFind=true;
//			//	}
//			//}
//
//			////�Ϸ��ж�
//			//ASSERT(bFind);
//			//if (false==bFind) return false;
//
//			//���ñ�־
//			bFind=true;
//
//			//�ж�ѭ��
//			break;
//		}	
//
//	} while(true);
//
//	//���д���
//	if (true==bFind && NULL!=pServerFishTrace)
//	{
//		//�����ж�
//		ASSERT(byCptrProbability==m_byCptrProbability[byBulletCount-1][pServerFishTrace->FishKind]);
//		if (!(byCptrProbability==m_byCptrProbability[byBulletCount-1][pServerFishTrace->FishKind])) 
//		{
//			TraceString(TEXT("(!(byCptrProbability==m_byCptrProbability[byBulletCount-1][pServerFishTrace->FishKind]))"));
//			return true;
//		}
//
//		//TraceString(TEXT("���д���"));
//
//		//������Ϣ
//		CMD_S_CaptureFish CaptureFish={0};
//		CaptureFish.dwFishID=dwFishID;
//		CaptureFish.wChairID=wChairID;
//		CaptureFish.FishKind=pServerFishTrace->FishKind;
//		CaptureFish.lFishScore=(enFishKind_12==pServerFishTrace->FishKind)?(m_lBonusOrigin+m_lBonusAccumulate[wChairID]):(g_nFishScore[pServerFishTrace->FishKind]);
//		SendDataExcludeSourceUser(pIServerUserItem,SUB_S_CAPTURE_FISH,&CaptureFish,sizeof(CaptureFish));		
//
//		//������Ⱥ
//		m_ServerFishTrace.FreeItem(pServerFishTrace);
//
//		//�����Ŀ
//		if (enFishKind_12==pServerFishTrace->FishKind)
//		{
//			//���ý��
//			m_lCoinCount[wChairID]+=(m_lBonusOrigin+m_lBonusAccumulate[wChairID]);
//
//			//��Ϣ����
//			CMD_S_BonusInfo BonusInfo={0};
//
//			//������Ϣ
//			BonusInfo.lBonus=m_lBonusOrigin;
//			m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_BONUS_INFO,&BonusInfo,sizeof(BonusInfo));
//			m_pITableFrame->SendLookonData(wChairID,SUB_S_BONUS_INFO,&BonusInfo,sizeof(BonusInfo));
//			/*m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BONUS_INFO,&BonusInfo,sizeof(BonusInfo));
//			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_BONUS_INFO,&BonusInfo,sizeof(BonusInfo));*/
//
//			//���ñ���
//			m_lBonusMultipleUser[wChairID]=0L;
//			m_lBonusAccumulate[wChairID]=0L;
//		}
//		else
//		{
//			m_lCoinCount[wChairID]+=g_nFishScore[pServerFishTrace->FishKind];
//		}			
//
//		//������Ŀ
//		m_wCptrFishCount[wChairID][pServerFishTrace->FishKind]+=1;
//
//		//���ͽ��
//		SendCoinCount(wChairID);
//
//		//������Ⱥ
//		SendFishCount(wChairID, pServerFishTrace->FishKind);
//	}
//
//	return true;
//}

//��ȡ����
bool CTableFrameSink::OnSubGetScoreInfo(IServerUserItem * pIServerUserItem)
{
	//�Ϸ��ж�
	ASSERT(NULL!=pIServerUserItem);
	if (NULL==pIServerUserItem) return false;
	
	//��ȡ����
	WORD wChairID=pIServerUserItem->GetChairID();
	if (wChairID>=GAME_PLAYER) return true;

	//��������
	CMD_S_BulletCount BulletCount={0};

	//���ñ���
	BulletCount.wChairID=wChairID;
	BulletCount.BulletCountKind=enBulletCountKind_100;
	BulletCount.lBulletCount=m_lBulletCount[wChairID][enBulletCountKind_100];
	m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_BULLET_COUNT,&BulletCount,sizeof(BulletCount));

	BulletCount.BulletCountKind=enBulletCountKind_600;
	BulletCount.lBulletCount=m_lBulletCount[wChairID][enBulletCountKind_600];
	m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_BULLET_COUNT,&BulletCount,sizeof(BulletCount));

	BulletCount.BulletCountKind=enBulletCountKind_800;
	BulletCount.lBulletCount=m_lBulletCount[wChairID][enBulletCountKind_800];
	m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_BULLET_COUNT,&BulletCount,sizeof(BulletCount));

	//��������
	CMD_S_CoinCount CoinCount={0};

	//���ñ���
	CoinCount.wChairID=wChairID;
	CoinCount.lCoinCount=m_lCoinCount[wChairID];

	//������Ϣ
	m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_COIN_COUNT,&CoinCount,sizeof(CoinCount));

	//������Ⱥ
	for (enFishKind FishKind=enFishKind_1; FishKind<enFishKind_No; FishKind=enFishKind(FishKind+1))
        SendFishCount(wChairID, FishKind);

	//�����Ϣ
	m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_SCORE_INFO,NULL,0);
	
	return true;
}

//���õ�Ԫ
bool CTableFrameSink::OnSubSetCellScore(IServerUserItem * pIServerUserItem, LONG lCellScore)
{
	//�Ϸ��ж�
	ASSERT(NULL!=pIServerUserItem);
	if (NULL==pIServerUserItem) return false;

	//��ȡ����
	WORD wChairID=pIServerUserItem->GetChairID();
	if (wChairID>=GAME_PLAYER) return true;

	//�����ж�
	ASSERT(0==m_lUserCellScore[wChairID]);
	if (!(0==m_lUserCellScore[wChairID]))
	{
		TraceString(TEXT("(!(0==m_lUserCellScore[wChairID]))"));
		TraceString(pIServerUserItem->GetAccounts());
		return true;
	}

	//ƥ���ж�
	bool bMatch=false;
	for (int i=0; i<CountArray(m_lCellScore); ++i)
	{
		if (lCellScore==m_lCellScore[i])
		{
			//���ñ���
			m_lUserCellScore[wChairID]=lCellScore;
			bMatch=true;

			//������Ϣ
			CMD_S_SetCellScore SetCellScore={0};
			SetCellScore.lCellScore=lCellScore;
			SetCellScore.wChairID=wChairID;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SET_CELL_SCORE,&SetCellScore,sizeof(SetCellScore));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SET_CELL_SCORE,&SetCellScore,sizeof(SetCellScore));

			break;
		}
	}

	return bMatch;
}

//�������
bool CTableFrameSink::OnSubCalcScore(IServerUserItem * pIServerUserItem)
{
	//�Ϸ��ж�
	ASSERT(NULL!=pIServerUserItem);
	if (NULL==pIServerUserItem) return false;
	
	//��ȡ����
	WORD wChairID=pIServerUserItem->GetChairID();

	//�������
	CalcScore(pIServerUserItem, wChairID);

	//�����ӵ�
	SendBulletCount(wChairID, enBulletCountKind_100, true);
	SendBulletCount(wChairID, enBulletCountKind_600, true);
	SendBulletCount(wChairID, enBulletCountKind_800, true);

	//���ͽ��
	SendCoinCount(wChairID);

	//������Ⱥ
	for (enFishKind FishKind=enFishKind_1; FishKind<enFishKind_No; FishKind=enFishKind(FishKind+1))
        SendFishCount(wChairID, FishKind);

	return true;
}

//������Ϣ
void CTableFrameSink::SendDataExcludeSourceUser(IServerUserItem * pIServerUserItemSource, WORD wSubCmdID,void * pData, WORD wDataSize)
{
	//�Ϸ��ж�
	ASSERT(NULL!=pIServerUserItemSource);
	if (NULL==pIServerUserItemSource) return ;

	//������Ϣ
	IServerUserItem *pIServerUserItemSend=NULL;
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		pIServerUserItemSend=m_pITableFrame->GetServerUserItem(wChairID);
		if (NULL==pIServerUserItemSend) continue;
		if (pIServerUserItemSend==pIServerUserItemSource) continue;
		m_pITableFrame->SendUserData(pIServerUserItemSend,wSubCmdID,pData,wDataSize);
	}
	m_pITableFrame->SendLookonData(INVALID_CHAIR,wSubCmdID,pData,wDataSize);
}

//�����ӵ�
void CTableFrameSink::SendBulletCount(WORD wChairID, enBulletCountKind BulletCountKind, bool bBroadcast)
{
	//�Ϸ��ж�
	ASSERT(wChairID<GAME_PLAYER);
	if (!(wChairID<GAME_PLAYER)) return ;

	//��������
	CMD_S_BulletCount BulletCount={0};

	//���ñ���
	BulletCount.wChairID=wChairID;
	BulletCount.BulletCountKind=BulletCountKind;
	BulletCount.lBulletCount=m_lBulletCount[wChairID][BulletCountKind];

	//������Ϣ
	if (false==bBroadcast)
	{
		IServerUserItem *pIServerUserItemSend=NULL;
		for (WORD i=0; i<GAME_PLAYER; ++i)
		{
			pIServerUserItemSend=m_pITableFrame->GetServerUserItem(i);
			if (NULL==pIServerUserItemSend) continue;
			if (i==wChairID) continue;
			m_pITableFrame->SendUserData(pIServerUserItemSend,SUB_S_BULLET_COUNT,&BulletCount,sizeof(BulletCount));
		}
	}
	else
	{
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BULLET_COUNT,&BulletCount,sizeof(BulletCount));
	}
	//m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_BULLET_COUNT,&BulletCount,sizeof(BulletCount));
}

//���ͽ��
void CTableFrameSink::SendCoinCount(WORD wChairID)
{
	//��������
	CMD_S_CoinCount CoinCount={0};

	//���ñ���
	CoinCount.wChairID=wChairID;
	CoinCount.lCoinCount=m_lCoinCount[wChairID];

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_COIN_COUNT,&CoinCount,sizeof(CoinCount));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_COIN_COUNT,&CoinCount,sizeof(CoinCount));
}

//������Ⱥ
void CTableFrameSink::SendFishCount(WORD wChairID, enFishKind FishKind)
{
	//�Ϸ��ж�
	ASSERT(wChairID<GAME_PLAYER);
	if (!(wChairID<GAME_PLAYER)) return ;

	//��ȡ���
	IServerUserItem *pIServerUserItem=m_pITableFrame->GetServerUserItem(wChairID);
	ASSERT(NULL!=pIServerUserItem);
	if (!(NULL!=pIServerUserItem)) return ;

	//��������
	CMD_S_CptrFishCount CptrFishCount;

	//���ñ���
	CptrFishCount.FishKind=FishKind;
	CptrFishCount.wFishCount=m_wCptrFishCount[wChairID][FishKind];

	//������Ϣ
	m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_FISH_COUNT,&CptrFishCount,sizeof(CptrFishCount));
}

//�����Ϣ
void CTableFrameSink::TraceString(LPCTSTR pszMessage)
{
//#ifdef _DEBUG
//	//������Ϣ
//	IServerUserItem *pIServerUserItemSend=NULL;
//	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
//	{
//		pIServerUserItemSend=m_pITableFrame->GetServerUserItem(wChairID);
//		if (NULL==pIServerUserItemSend) continue;
//		m_pITableFrame->SendGameMessage(pIServerUserItemSend,pszMessage,SMT_INFO);
//	}

	try
	{
		//�����ļ���
		TCHAR szPath[MAX_PATH]=TEXT("");
		TCHAR szFileName[MAX_PATH];
		GetCurrentDirectory(CountArray(szPath),szPath);
		_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\%s.txt"),szPath,m_pGameServiceOption->szGameRoomName);

		//���ļ�
		CFile FileRecord;
		if (FileRecord.Open(szFileName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
		{
			FileRecord.SeekToEnd();
			FileRecord.Write(pszMessage,lstrlen(pszMessage));
			FileRecord.Write(TEXT("\r\n"),lstrlen(TEXT("\r\n")));
			FileRecord.Close();
		}
	}
	catch(...){}

//#endif
}

//��Ⱥ��ʶ
DWORD CTableFrameSink::GetNewFishID()
{
	//������ʶ
	m_dwFishID+=1;
	if (0L==m_dwFishID) m_dwFishID=1;

	return m_dwFishID;
}

//����Ⱥ��
void CTableFrameSink::BuildRegularFish(bool bLastCol)
{
	//��ʼλ��
	int nStartPosX=(-1==m_nRegularSign)?(-100):(VIEW_WIDTH+100);
	int nEndPosX=(-1==m_nRegularSign)?(VIEW_WIDTH+100):(-100);

	//��Ⱥ���
	int const nFishWidth[enFishKind_No]={44,91,78,63,74,96,80,117,195,190,447};
	int const nFishHeight[enFishKind_No]={34/2,53/2,77/2,/*57/2*/64/2,54/2,69/2,51/2,111/2,102/2,212/2,329/2,382/2};

	//��Ⱥ����
	BYTE const byMaxRow=10;

	//ƫ��λ��
	int nSpaceXFish=20,nSpaceXRow=8;

	//������Ⱥ
	BYTE byLastKind=0;
	BYTE byLastCount=0;

	//��Ⱥ����
	enFishKind FishKindRow=enFishKind(enFishKind_1+rand()%enFishKind_11);

	//��Ⱥ����
	BYTE byFishCount=(FishKindRow<=enFishKind_3)?(rand()%6+10):(rand()%6+5);

	//�����ж�
	if (true==bLastCol && 0==(rand()%4))
	{
		if (byFishCount>4) byFishCount=rand()%4+1;
		FishKindRow=enFishKind_11;
	}

	//�߶ȵ���
	if (byFishCount*nFishHeight[FishKindRow]>VIEW_HEIGHT) byFishCount=VIEW_HEIGHT/nFishHeight[FishKindRow];

	//�����ж�
	switch (m_byRegularForm)
	{
	case 0:
		{
			//����λ��
			/*if (0!=byLastKind)
			{
				nStartPosX+=(m_nRegularSign*(nSpaceXFish*byLastCount/2));
			}*/

			int nStartPosY=(VIEW_HEIGHT-byFishCount*nFishHeight[FishKindRow]-43+24)/2+nFishHeight[FishKindRow]/2;//Ҫ��ȥ�ײ��������߶�
			for (int nPosIndex=0; nPosIndex<byFishCount; ++nPosIndex)
			{
				//��ȡ����
				tagServerFishTrace *pServerFishTrace=m_ServerFishTrace.ActiveItem();
				ASSERT(NULL!=pServerFishTrace);
				if (NULL==pServerFishTrace) return;

				//��ʼ����
				ZeroMemory(pServerFishTrace,sizeof(tagServerFishTrace));

				//������Ϣ
				pServerFishTrace->dwBuildTime=GetTickCount();
				pServerFishTrace->dwFishID=GetNewFishID();
				pServerFishTrace->FishKind=FishKindRow;
				pServerFishTrace->nInitCount=2;
				pServerFishTrace->nInitX[0]=nStartPosX;
				pServerFishTrace->nInitX[1]=nEndPosX;
				pServerFishTrace->nInitY[0]=nStartPosY+nPosIndex*nFishHeight[FishKindRow];
				pServerFishTrace->nInitY[1]=pServerFishTrace->nInitY[0];
			}

			//�޸�����
			nStartPosX+=(m_nRegularSign*(nFishWidth[FishKindRow]+nSpaceXRow));

			//���ñ���
			byLastKind=0;
			byLastCount=byFishCount;

			break;
		}
	case 1:
		{
			if (byFishCount%2!=0) byFishCount+=1;
			int nStartPosY=(VIEW_HEIGHT-byFishCount*nFishHeight[FishKindRow]-43+24)/2+nFishHeight[FishKindRow]/2;//Ҫ��ȥ�ײ��������߶�
			for (int nPosIndex=0; nPosIndex<byFishCount; ++nPosIndex)
			{
				//��ȡ����
				tagServerFishTrace *pServerFishTrace=m_ServerFishTrace.ActiveItem();
				ASSERT(NULL!=pServerFishTrace);
				if (NULL==pServerFishTrace) return;

				//��ʼ����
				ZeroMemory(pServerFishTrace,sizeof(tagServerFishTrace));

				//������Ϣ
				pServerFishTrace->dwBuildTime=GetTickCount();
				pServerFishTrace->dwFishID=GetNewFishID();
				pServerFishTrace->FishKind=FishKindRow;
				pServerFishTrace->nInitCount=2;
				if (nPosIndex<byFishCount/2) pServerFishTrace->nInitX[0]=nStartPosX+m_nRegularSign*nPosIndex*nSpaceXFish;
				else pServerFishTrace->nInitX[0]=nStartPosX+m_nRegularSign*(byFishCount-1-nPosIndex)*nSpaceXFish;
				pServerFishTrace->nInitX[1]=nEndPosX;
				pServerFishTrace->nInitY[0]=nStartPosY+nPosIndex*nFishHeight[FishKindRow];
				pServerFishTrace->nInitY[1]=pServerFishTrace->nInitY[0];
			}

			//�޸�����
			nStartPosX+=(m_nRegularSign*(nFishWidth[FishKindRow]+nSpaceXFish*(byFishCount/2-1)+nSpaceXRow));

			//���ñ���
			byLastKind=0;
			byLastCount=byFishCount;

			break;
		}
	case 2:
		{
			if (byFishCount%2!=0) byFishCount+=1;
			int nStartPosY=(VIEW_HEIGHT-byFishCount*nFishHeight[FishKindRow]-43+24)/2+nFishHeight[FishKindRow]/2;//Ҫ��ȥ�ײ��������߶�
			nStartPosX+=(m_nRegularSign*(nFishWidth[FishKindRow]+nSpaceXFish*(byFishCount/2-1)));
			for (int nPosIndex=0; nPosIndex<byFishCount; ++nPosIndex)
			{
				//��ȡ����
				tagServerFishTrace *pServerFishTrace=m_ServerFishTrace.ActiveItem();
				ASSERT(NULL!=pServerFishTrace);
				if (NULL==pServerFishTrace) return;

				//��ʼ����
				ZeroMemory(pServerFishTrace,sizeof(tagServerFishTrace));

				//������Ϣ
				pServerFishTrace->dwBuildTime=GetTickCount();
				pServerFishTrace->dwFishID=GetNewFishID();
				pServerFishTrace->FishKind=FishKindRow;
				pServerFishTrace->nInitCount=2;
				if (nPosIndex<byFishCount/2) pServerFishTrace->nInitX[0]=nStartPosX-m_nRegularSign*nPosIndex*nSpaceXFish;
				else pServerFishTrace->nInitX[0]=nStartPosX-m_nRegularSign*(byFishCount-1-nPosIndex)*nSpaceXFish;
				pServerFishTrace->nInitX[1]=nEndPosX;
				pServerFishTrace->nInitY[0]=nStartPosY+nPosIndex*nFishHeight[FishKindRow];
				pServerFishTrace->nInitY[1]=pServerFishTrace->nInitY[0];
			}

			////�޸�����
			//nStartPosX+=(m_nRegularSign*(nFishWidth[FishKindRow]+nSpaceXFish*(byFishCount-1)+nSpaceXRow));

			//���ñ���
			byLastKind=0;
			byLastCount=byFishCount;

			break;
		}
	}
}

//�������
void CTableFrameSink::CalcScore(IServerUserItem * pIServerUserItem, WORD wChairID)
{
	//�Ϸ��ж�
	ASSERT(NULL!=pIServerUserItem);
	if (NULL==pIServerUserItem) return ;

	//ʣ���ӵ�
	LONGLONG lLeftCoin=0L;
	LONG lBulletCount[enBulletCountKind_End]={100,600,800};
	for (enBulletCountKind BulletCountKind=enBulletCountKind_100; BulletCountKind<enBulletCountKind_End; BulletCountKind=enBulletCountKind(BulletCountKind+1))
		lLeftCoin+=((m_lBulletCharge[BulletCountKind]/lBulletCount[BulletCountKind])*m_lBulletCount[wChairID][BulletCountKind]);

	//�������
	LONGLONG lScore=(m_lCoinCount[wChairID]+lLeftCoin)*m_lUserCellScore[wChairID];

	//д�����
	DWORD dwNowTime=(DWORD)time(NULL);
	DWORD dwPlayTime=0L;
	if (m_lCoinCount[wChairID]>0L) dwPlayTime=dwNowTime-m_dwPlayTime[wChairID];
	enScoreKind ScoreKind=(m_lWinCoinCount[wChairID]>m_lUserShootCount[wChairID])?enScoreKind_Win:enScoreKind_Lost;
	if (lScore!=0L) m_pITableFrame->WriteUserScore(pIServerUserItem,lScore,0L,ScoreKind,dwPlayTime);

	//������Ϣ
	TraceGameInfo(pIServerUserItem, lScore, lLeftCoin, wChairID);
	
	//�������
	ZeroMemory(m_lBulletCount[wChairID],sizeof(m_lBulletCount[wChairID]));
	m_lCoinCount[wChairID]=0L;
	m_lUserShootCount[wChairID]=0L;
	ZeroMemory(m_wCptrFishCount[wChairID],sizeof(m_wCptrFishCount[wChairID]));
	m_dwPlayTime[wChairID]=dwNowTime;

	//�ͷ��ӵ�
	POSITION ps=m_ShootBullet[wChairID].GetStartPosition();
	DWORD dwBulletID=0L;
	tagBulletInfo *pBulletInfo=NULL;
	while (NULL!=ps)
	{
		m_ShootBullet[wChairID].GetNextAssoc(ps, dwBulletID, pBulletInfo);
		if (NULL!=pBulletInfo) m_ShootBulletFactory.FreeItem(pBulletInfo);
	}
	m_ShootBullet[wChairID].RemoveAll();
}

//����λ��
bool CTableFrameSink::RectifyTracePosX(int &nPosX)
{
	if (nPosX>=150 && nPosX<=370)
	{
		nPosX+=100;
		return true;
	}
	else if (nPosX>=640 && nPosX<=860)
	{
		nPosX+=100;
		return true;
	}

	return false;
}

//��ȡ����(��m_szIniFileName=fish.ini��ȡ���ص�������Ϣ)
void CTableFrameSink::ReadConfig()
{
	//ϵͳ��Ϣ(����ϵͳ��Ϣ��ʱ��)
	INT nTimeSysMessage=GetPrivateProfileInt(m_szGameRoomName,TEXT("Time"),5*60,m_szIniFileName);
#ifdef _DEBUG
	if (nTimeSysMessage>0) m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE,10*1000,TIMES_INFINITY,0L);
#else
	if (nTimeSysMessage>0) m_pITableFrame->SetGameTimer(IDI_SYS_MESSAGE,nTimeSysMessage*60*1000,TIMES_INFINITY,0L);
#endif

	//��ȡ����(�ӵ�����)
	TCHAR szBuffer[32];
	LONG lDefaultValue[enBulletCountKind_End]={100,1200,2400};	
	for (int i=0; i<enBulletCountKind_End; ++i)
	{
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("Charge%d"),i+1);
		m_lBulletCharge[i]=GetPrivateProfileInt(m_szGameRoomName,szBuffer,lDefaultValue[i],m_szIniFileName);
	}

	//���ñ���
	TCHAR szIndex[128];
	for (int i=0; i<CountArray(m_lCellScore); ++i)
	{
		_sntprintf(szIndex,CountArray(szIndex),TEXT("CellScore%d"),i+1);
		m_lCellScore[i]=GetPrivateProfileInt(m_szGameRoomName,szIndex,m_pGameServiceOption->lCellScore,m_szIniFileName);
	}
	ZeroMemory(m_lUserCellScore,sizeof(m_lUserCellScore));
	//add by Ivan_han
	//��Ԫ����
    m_lUserCellScore[0]=5;
    m_lUserCellScore[1]=5;
    m_lUserCellScore[2]=5;
    m_lUserCellScore[3]=5;

	//�������P(�ӵ��ͺ�1-5,�������1-12)
	BYTE const byCptrProbability[5][enFishKind_No]={{80,50,25,14,10,5,2,1,1,1,1,1},
	{80,50,50,28,20,10,6,5,4,3,2,1},
	{80,70,75,42,30,15,10,7,6,5,3,1},
	{80,70,60,57,40,20,13,10,8,6,4,2},
	{90,85,80,71,50,25,16,12,9,7,5,3}};


	//�������(�ӵ�����ĸ���)
	for (int i=0; i<5; ++i)
	{
		for (int j=0; j<enFishKind_No; ++j)
		{
			_sntprintf(szBuffer,CountArray(szBuffer),TEXT("Bullet_%d_Fish_%d"),i+1,j+1);
			m_byCptrProbability[i][j]=GetPrivateProfileInt(m_szGameRoomName,szBuffer,byCptrProbability[i][j],m_szIniFileName);
#ifdef _DEBUG
			TCHAR szInt[128];
			WritePrivateProfileString(m_szGameRoomName,szBuffer,_itoa(byCptrProbability[i][j],szInt,10),m_szIniFileName);
#endif
		}
	}

	#ifdef _DEBUG
		for (int i=0; i<5; ++i)
		{
			for (int j=0; j<enFishKind_No; ++j)
			{
				m_byCptrProbability[i][j]=200;
			}
		}
	#endif

	//�ʽ����
	//��ʼ��Ŀ
	m_lBonusOrigin=GetPrivateProfileInt(m_szGameRoomName,TEXT("Bonus_Origin"),0L,m_szIniFileName);
	//��Ԫ��Ŀ
	m_lBonusMultiple=GetPrivateProfileInt(m_szGameRoomName,TEXT("Bonus_Multiple"),0L,m_szIniFileName);
	//�ӵ�����
	m_lBulletLimit=GetPrivateProfileInt(m_szGameRoomName,TEXT("Bonus_BulletLimit"),m_lBonusMultiple,m_szIniFileName);	
	//�����
	m_lBonusMultipleMax=GetPrivateProfileInt(m_szGameRoomName,TEXT("Bonus_MultipleMax"),100L,m_szIniFileName);
	//��Ԫʱ��
	m_lBonusUnitTime=GetPrivateProfileInt(m_szGameRoomName,TEXT("Bonus_UnitTime"),5,m_szIniFileName);
	ZeroMemory(m_lBonusAccumulate,sizeof(m_lBonusAccumulate));
	ZeroMemory(m_lShootBulletCount,sizeof(m_lShootBulletCount));
	ZeroMemory(m_lBonusMultipleUser,sizeof(m_lBonusMultipleUser));

	//���Ʋ���
	//�Ƿ��������¿��Ʋ�����1Ϊ���ã�0=false=m_bControlScoreΪ����
	m_bControlScore=GetPrivateProfileInt(m_szGameRoomName,TEXT("ControlScore"),0,m_szIniFileName)?true:false;

	//����Ҵ򵽵Ľ�������ڹ�����ӵ����İٷ�֮WindMulLimitMinʱ������߰ٷ�֮��ʮ�Ļ�����
	m_byWindMulLimitMin=GetPrivateProfileInt(m_szGameRoomName,TEXT("WindMulLimitMin"),40,m_szIniFileName);
	//����Ҵ򵽵Ľ�������ڹ�����ӵ�����WindMulLimitMax��ʱ���ή�Ͱٷ�֮��ʮ�Ļ�����
	m_byWindMulLimitMax=GetPrivateProfileInt(m_szGameRoomName,TEXT("WindMulLimitMax"),3,m_szIniFileName);
	//����Ҵ򵽵Ľ�������ڹ�����ӵ�����WindMulLimitBonus��ʱ��һֱ�����ܻ��вʽ�
	m_byWindMulLimitBonus=GetPrivateProfileInt(m_szGameRoomName,TEXT("WindMulLimitBonus"),2,m_szIniFileName);
	//���ѡ��һ��ʱ�����и��ʻή�Ͱٷ�֮CellScoreRectify_1
	m_byCellScoreRectify[0]=GetPrivateProfileInt(m_szGameRoomName,TEXT("CellScoreRectify_1"),0,m_szIniFileName);
	//���ѡ�����ʱ�����и��ʻή�Ͱٷ�֮CellScoreRectify_2
	m_byCellScoreRectify[1]=GetPrivateProfileInt(m_szGameRoomName,TEXT("CellScoreRectify_2"),0,m_szIniFileName);
	//��������
	m_byCellScoreRectify[2]=GetPrivateProfileInt(m_szGameRoomName,TEXT("CellScoreRectify_3"),0,m_szIniFileName);
	m_byCellScoreRectify[3]=GetPrivateProfileInt(m_szGameRoomName,TEXT("CellScoreRectify_4"),0,m_szIniFileName);
	m_byCellScoreRectify[4]=GetPrivateProfileInt(m_szGameRoomName,TEXT("CellScoreRectify_5"),0,m_szIniFileName);
	//1Ϊ�����Ϸ��Ϣ��0�����
	m_bTraceGameInfo=GetPrivateProfileInt(m_szGameRoomName,TEXT("TraceGameInfo"),0,m_szIniFileName)?true:false;
	//���ڴ˷�������Ϸ��Ϣ�����
	m_lTraceScoreLevel=GetPrivateProfileInt(m_szGameRoomName,TEXT("TraceLevel"),50000,m_szIniFileName);
	
	//����ʱ��
	if (m_lBonusOrigin>0L) m_pITableFrame->SetGameTimer(IDI_BONUS,m_lBonusUnitTime*60*1000,TIMES_INFINITY,0L);
}

//������Ϸ��Ϣ
bool CTableFrameSink::SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage)
{
	//�������ݰ�
	CMD_S_GameMessage Message;
	lstrcpyn(Message.szContent,lpszMessage,CountArray(Message.szContent));
	Message.wMessageLength=CountStringBuffer(Message.szContent);
	WORD wSendSize=sizeof(Message)-sizeof(Message.szContent)+Message.wMessageLength*sizeof(TCHAR);

	//��������
	if (NULL==pIServerUserItem)
	{
		IServerUserItem *pIServerUserItemSend=NULL;
		for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
		{
			pIServerUserItemSend=m_pITableFrame->GetServerUserItem(wChairID);
			if (NULL==pIServerUserItemSend) continue;
			m_pITableFrame->SendUserData(pIServerUserItemSend,SUB_S_GAME_MESSAGE,&Message,wSendSize);
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_MESSAGE,&Message,wSendSize);
	}
	else
	{
		m_pITableFrame->SendUserData(pIServerUserItem,SUB_S_GAME_MESSAGE,&Message,wSendSize);
		m_pITableFrame->SendLookonData(pIServerUserItem->GetChairID(),SUB_S_GAME_MESSAGE,&Message,wSendSize);
	}

	return true;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return true;
}

//�����ж�
tagServerFishTrace *CTableFrameSink::GetFishTraceInfo(DWORD dwFishID)
{
	//ö�ٱ���
	INT_PTR nEnumIndex=0;
	tagServerFishTrace *pServerFishTrace=NULL;
	do
	{
		//ö�ٹ켣
		pServerFishTrace=m_ServerFishTrace.EnumActiveObject(nEnumIndex++);
		if (NULL==pServerFishTrace) break;

		//����ж�
		if (dwFishID==pServerFishTrace->dwFishID) return pServerFishTrace;

	} while(true);

	return NULL;
}

//������Ϣ
void CTableFrameSink::TraceGameInfo(IServerUserItem * pIServerUserItem, LONGLONG lScore, LONGLONG lLeftBullet, WORD wChairID)
{
	//�Ϸ��ж�
	if (NULL==pIServerUserItem) return;
	if (wChairID>=GAME_PLAYER) return;

	//�����ж�
	if (true==m_bTraceGameInfo && lScore>=m_lTraceScoreLevel)
	{
		try
		{
			//�����ļ���
			TCHAR szPath[MAX_PATH]=TEXT("");
			TCHAR szFileName[MAX_PATH];
			GetCurrentDirectory(CountArray(szPath),szPath);
			_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\FishGameInfo\\%ld.txt"),szPath,pIServerUserItem->GetUserID());

			//����Ŀ¼
			int nExcursion=0;
			TCHAR szDirectory[MAX_PATH]=TEXT("");
			_sntprintf(szDirectory,CountArray(szDirectory),TEXT("%s\\FishGameInfo"),szPath);
			do
			{
				if (szDirectory[nExcursion]==0) 
				{
					::CreateDirectory(szDirectory,NULL);
					break;
				}
				if (szDirectory[nExcursion]==TEXT('\\'))
				{
					szDirectory[nExcursion]=0;
					::CreateDirectory(szDirectory,NULL);
					szDirectory[nExcursion]=TEXT('\\');
				}
				nExcursion++;
			} while (true);

			//���ļ�
			CFile FileRecord;
			if (FileRecord.Open(szFileName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
			{
				//�ƶ��ļ�
				FileRecord.SeekToEnd();

				//��ȡʱ��
				TCHAR szGameInfo[1024]={0};
				SYSTEMTIME SystemTime;
				GetLocalTime(&SystemTime);
				
				//д�����
				_sntprintf(szGameInfo,CountArray(szGameInfo),TEXT("[%04d-%02d-%02d %02d:%02d:%02d:%02d]�˺ţ�%s���÷֣�%I64d�����%ld���ӵ�:%I64d��������%ld\r\n"),
					SystemTime.wYear,SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond,SystemTime.wMilliseconds,
					pIServerUserItem->GetAccounts(),lScore,m_lCoinCount[wChairID],lLeftBullet, m_lUserCellScore[wChairID]);
				FileRecord.Write(szGameInfo,lstrlen(szGameInfo));

				//������Ϣ
				LONG lNormalFishScore=0;
				for (enFishKind FishKind=enFishKind_1; FishKind<enFishKind_No; FishKind=enFishKind(FishKind+1))
				{
					if (m_wCptrFishCount[wChairID][FishKind]>0)
					{
						//�ʽ��ж�
						if (enFishKind_12==FishKind)
						{
							_sntprintf(szGameInfo,CountArray(szGameInfo),TEXT("�ʽ�÷֣�%ld��������%ld"),m_lCoinCount[wChairID]-lNormalFishScore, m_wCptrFishCount[wChairID][FishKind]);
						}
						else
						{
							_sntprintf(szGameInfo,CountArray(szGameInfo),TEXT("(%ld,%ld)��"),g_nFishScore[FishKind], m_wCptrFishCount[wChairID][FishKind]);
							lNormalFishScore+=(m_wCptrFishCount[wChairID][FishKind]*g_nFishScore[FishKind]);
						}
						FileRecord.Write(szGameInfo,lstrlen(szGameInfo));
					}
				}
				
				
				FileRecord.Write(TEXT("\r\n"),lstrlen(TEXT("\r\n")));

				//�ر��ļ�
				FileRecord.Close();
			}
		}
		catch(...){}

	}
}
//////////////////////////////////////////////////////////////////////////
