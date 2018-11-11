#include "StdAfx.h"
#include "TableFrame.h"
#include "DataBaseSink.h"
#include "AttemperEngineSink.h"
#include "CGlobalData.h"

#define  MAX_DELAY   (3)
#define  TWO_MIN     (60*2)

//���ͷ���
bool __cdecl CAttemperEngineSink::SendUserScore(IServerUserItem * pIServerUserItem)
{
	__ENTER_FUNCTION_LOG_CATCH

	tagServerUserData * pUserData=pIServerUserItem->GetUserData();

	//��������
	CMD_GR_UserScore UserScore;
	UserScore.dwUserID=pUserData->dwUserID;
	UserScore.UserScore=pUserData->UserScoreInfo;
	UserScore.lLoveliness = pUserData->lLoveliness;
	UserScore.UserScore.lGold   = pUserData->UserScoreInfo.lGold+pUserData->lStorageGold;

	m_pITCPNetworkEngine->SendDataBatch(MDM_GR_USER,SUB_GR_USER_SCORE,&UserScore,sizeof(UserScore));
	m_AndroidUserManager.SendDataToClient(MDM_GR_USER,SUB_GR_USER_SCORE,&UserScore,sizeof(UserScore));

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//////////////////////////////////////////////////////////////////////////
void Debug(char *text,...)
{
	static DWORD num=0;
	char buf[1024];
	FILE *fp=NULL;

	va_list ap;

	va_start(ap,text);
	vsprintf(buf,text,ap);
	va_end(ap);	

	if(num == 0)
	{
		fp=fopen("debug.log","w");
	}
	else
	{

		fp=fopen("debug.log","a");
	}

	if(fp == NULL)
		return ;

	num++;

	SYSTEMTIME time;

	GetLocalTime(&time);

	fprintf(fp, "%d:%s - %d/%d/%d %d:%d:%d \n", num, buf, time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	fclose(fp);
} 

//���캯��
CAttemperEngineSink::CAttemperEngineSink()
{
	m_pRoomLog = NULL;		// ��־
	
	//��Ϸ����
	m_wMaxUserItem=0;
	m_pTableFrame=NULL;
	m_pNormalParameter=NULL;
	m_pAndroidParameter=NULL;
	m_UserAutoQueue.Clear();
	m_AndroidAutoQueue.Clear();

	//���ͳ���
	m_cbSendLocation = 0;

	//��Ϣ����
	m_cbRoomMsgIndex=0;
	m_cbGameMsgIndex=0;

	//���ñ���
	m_dwCenterServer=0L;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//���Ʊ���
	m_bShallClose=false;
	m_bAllowWisper=true;
	m_bAllowRoomChat=true;
	m_bAllowGameChat=true;
	m_bAllowEnterRoom=true;
	m_bAllowEnterGame=true;

	//��������
	m_wColumnInfoSize=0;
	m_wOnLineInfoSize=0;
	memset(&m_ColumnInfo,0,sizeof(m_ColumnInfo));
	memset(&m_OnLineCountInfo,0,sizeof(m_OnLineCountInfo));

	//��Ϸ���
	m_hDllInstance=NULL;
	m_pIGameServiceManager=NULL;

	//�ӿڱ���
	m_pITimerEngine=NULL;
	m_pIAttemperEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketCorrespond=NULL;

	return;
}

//��������
CAttemperEngineSink::~CAttemperEngineSink()
{
	//ɾ���ڴ�
	SafeDeleteArray(m_pTableFrame);
	SafeDeleteArray(m_pNormalParameter);
	SafeDeleteArray(m_pAndroidParameter);

	return;
}

//�ӿڲ�ѯ
void * __cdecl CAttemperEngineSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAttemperEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE(IGameServiceFrame,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink,Guid,dwQueryVer);
	return NULL;
}

//����ģ������
bool __cdecl CAttemperEngineSink::OnAttemperEngineStart(IUnknownEx * pIUnknownEx)
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч�����
	ASSERT(pIUnknownEx!=NULL);

	//���ñ���
	m_wColumnInfoSize=0;
	m_wOnLineInfoSize=0;
	memset(&m_ColumnInfo,0,sizeof(m_ColumnInfo));
	memset(&m_OnLineCountInfo,0,sizeof(m_OnLineCountInfo));
	m_UserAutoQueue.Clear();
	m_AndroidAutoQueue.Clear();

	//������������
	m_wMaxUserItem=m_pGameServiceOption->wMaxConnect;
	m_pNormalParameter=new tagConnectItemInfo [m_wMaxUserItem];
	m_pAndroidParameter=new tagConnectItemInfo [MAX_ANDROID];
	memset(m_pNormalParameter,0,sizeof(tagConnectItemInfo)*m_wMaxUserItem);
	memset(m_pAndroidParameter,0,sizeof(tagConnectItemInfo)*MAX_ANDROID);

	//���Ա���
	m_PropertyAttributeArrary.RemoveAll();
	m_FlowerAttributeArrary.RemoveAll();

	//������Ϸ���
	if (m_pIGameServiceManager->RectifyServiceOption(m_pGameServiceOption)==false) return false;

	//������Ϸ����
	ASSERT(m_pGameServiceOption->wTableCount!=0);
	m_pTableFrame=new CTableFrame [m_pGameServiceOption->wTableCount];

	//�������
	for (int i=0; i<m_pGameServiceOption->wTableCount; ++i)
	{
		(m_pTableFrame + i)->m_pAttemperEngineSink = this;
		(m_pTableFrame + i)->SetRoomLog(m_pRoomLog);
	}
	//��ӽ���
	if (m_pTableFrame==NULL) return false;

	//��ʼ������
	bool bSuccess=false;
	tagTableFrameParameter TableFrameParameter;
	TableFrameParameter.pGameServiceAttrib=m_pGameServiceAttrib;
	TableFrameParameter.pGameServiceOption=m_pGameServiceOption;
	TableFrameParameter.pIGameServiceFrame=QUERY_ME_INTERFACE(IUnknownEx);
	TableFrameParameter.pIGameServiceManager=QUERY_OBJECT_PTR_INTERFACE(m_pIGameServiceManager,IUnknownEx);
	for (WORD i=0;i<m_pGameServiceOption->wTableCount;i++)
	{
		bSuccess=(m_pTableFrame+i)->InitTableFrame(i,&TableFrameParameter);

		if(bSuccess && m_pRoomLog)
		{
			// �첽��־  add by HouGuoJiang 2011-11-25
			(m_pTableFrame+i)->m_pITableFrameSink->channel_to_game_service(CTGS_MAIN_LOG, CTGS_ASS_LOG_POINT, 
																														 (void*)m_pRoomLog, sizeof(m_pRoomLog));
			////��������
			//// @yexiuxuan add
			//(m_pTableFrame+i)->m_pITableFrameSink->channel_to_game_service(CTCS_MAIN_GOODRAND, CTCS_MAIN_GOODRAND_POINT, (void*)m_pGoodRand, sizeof(m_pGoodRand));
		}

		if (bSuccess==false) return false;
	}

	m_ServerList.SetRoomLog(m_pRoomLog);	//�����б�
	m_ServerUserManager.SetRoomLog(m_pRoomLog);	//�����б�

	//��������
	m_AndroidUserManager.SetRoomLog(m_pRoomLog);
	m_AndroidUserManager.m_pITimerEngine=m_pITimerEngine;
	m_AndroidUserManager.m_pGameServiceAttrib=m_pGameServiceAttrib;
	m_AndroidUserManager.m_pGameServiceOption=m_pGameServiceOption;
	m_AndroidUserManager.m_pIGameServiceManager=m_pIGameServiceManager;
	m_AndroidUserManager.m_pIServerUserManager=QUERY_OBJECT_INTERFACE(m_ServerUserManager,IServerUserManager);
	m_AndroidUserManager.m_pITCPNetworkEngineEvent=QUERY_OBJECT_PTR_INTERFACE(m_pIAttemperEngine,ITCPNetworkEngineEvent);

	//�����б����
	switch (m_pGameServiceOption->wServerType)
	{
	case GAME_GENRE_GOLD:	//�������
		{
			//��������
			tagColumnItem ColumnItem[]=
			{
				115,DTP_USER_ACCOUNTS,TEXT("�û���"),
					60,DTP_GAME_ID,TEXT("ID ����"),
					60,DTP_USER_SCORE,TEXT("���"),
					40,DTP_USER_TABLE,TEXT("����"),
					60,DTP_LOVE_LINESS,TEXT("����"),
					85,DTP_GAME_LEVEL,TEXT("��Ϸ����"),
					50,DTP_USER_EXPERIENCE,TEXT("����ֵ"),
					60,DTP_WIN_RATE,TEXT("ʤ��"),
					60,DTP_FLEE_RATE,TEXT("����"),
					60,DTP_USER_PLAY_COUNT,TEXT("�ܾ�"),
					60,DTP_USER_WIN,TEXT("Ӯ��"),
					60,DTP_USER_LOST,TEXT("���"),
					60,DTP_USER_DRAW,TEXT("�;�"),
					60,DTP_USER_FLEE,TEXT("�Ӿ�"),
					200,DTP_UNDER_WRITE,TEXT("����ǩ��"),
			};

			//���ñ���
			m_ColumnInfo.wColumnCount=CountArray(ColumnItem);
			CopyMemory(m_ColumnInfo.ColumnItem,ColumnItem,sizeof(ColumnItem));

			break;
		}
	case GAME_GENRE_MATCH:	//��������
		{
			//��������
			tagColumnItem ColumnItem[]=
			{
				115,DTP_USER_ACCOUNTS,TEXT("�û���"),
					60,DTP_GAME_ID,TEXT("ID ����"),
					60,DTP_USER_SCORE,TEXT("����"),
					40,DTP_USER_TABLE,TEXT("����"),
					60,DTP_LOVE_LINESS,TEXT("����"),
					85,DTP_GAME_LEVEL,TEXT("��Ϸ����"),
					50,DTP_USER_EXPERIENCE,TEXT("����ֵ"),
					60,DTP_WIN_RATE,TEXT("ʤ��"),
					60,DTP_FLEE_RATE,TEXT("����"),
					60,DTP_USER_PLAY_COUNT,TEXT("�ܾ�"),
					60,DTP_USER_WIN,TEXT("Ӯ��"),
					60,DTP_USER_LOST,TEXT("���"),
					60,DTP_USER_DRAW,TEXT("�;�"),
					60,DTP_USER_FLEE,TEXT("�Ӿ�"),
					200,DTP_UNDER_WRITE,TEXT("����ǩ��"),
			};

			//���ñ���
			m_ColumnInfo.wColumnCount=CountArray(ColumnItem);
			CopyMemory(m_ColumnInfo.ColumnItem,ColumnItem,sizeof(ColumnItem));

			break;
		}
	default:
		{
			//��������
			tagColumnItem ColumnItem[]=
			{
				115,DTP_USER_ACCOUNTS,TEXT("�û���"),
					60,DTP_GAME_ID,TEXT("ID ����"),
					60,DTP_USER_SCORE,TEXT("����"),
					40,DTP_USER_TABLE,TEXT("����"),
					60,DTP_LOVE_LINESS,TEXT("����"),
					85,DTP_GAME_LEVEL,TEXT("��Ϸ����"),
					50,DTP_USER_EXPERIENCE,TEXT("����ֵ"),
					60,DTP_WIN_RATE,TEXT("ʤ��"),
					60,DTP_FLEE_RATE,TEXT("����"),
					60,DTP_USER_PLAY_COUNT,TEXT("�ܾ�"),
					60,DTP_USER_WIN,TEXT("Ӯ��"),
					60,DTP_USER_LOST,TEXT("���"),
					60,DTP_USER_DRAW,TEXT("�;�"),
					60,DTP_USER_FLEE,TEXT("�Ӿ�"),
					200,DTP_UNDER_WRITE,TEXT("����ǩ��"),
			};

			//���ñ���
			m_ColumnInfo.wColumnCount=CountArray(ColumnItem);
			CopyMemory(m_ColumnInfo.ColumnItem,ColumnItem,sizeof(ColumnItem));

			break;
		}
	}

	//�����б�
	WORD wColumnInfoHandSize=sizeof(m_ColumnInfo)-sizeof(m_ColumnInfo.ColumnItem);
	m_wColumnInfoSize=wColumnInfoHandSize+m_ColumnInfo.wColumnCount*sizeof(m_ColumnInfo.ColumnItem[0]);

	//�������
	srand((DWORD)time(NULL));

	//�������ķ�����
	m_pITCPSocketCorrespond->Connect(ntohl(m_dwCenterServer),PORT_CENTER_SERVER);

	//ϵͳ��Ϣ
	m_pITimerEngine->SetTimer(IDI_SEND_SYSTEM_MESSAGE,300000L,(DWORD)(-1),0);

	//��������
	if (m_AndroidUserManager.StartService()==false) return false;

	//���ع���
	m_pITimerEngine->SetTimer(IDI_UPDATE_AFFICHE ,TIME_UPDATE_AFFICHE,(DWORD)(-1),NULL);
	//���·�æ����������
	m_pITimerEngine->SetTimer(IDI_UPDATE_BUSY_DESK, TIME_UPDATE_BUSY_DESK, (DWORD)(-1),NULL);
	//���»����˵��Զ�����
	m_pITimerEngine->SetTimer(IDI_UPDATE_ADD_ANDROID,TIME_UPDATE_ADD_ANDROID,(DWORD)(-1), NULL);
	//�����û�����
	m_pITimerEngine->SetTimer(IDI_UPDATE_ADD_USER, TIME_UPDATE_ADD_USER, (DWORD)(-1), NULL);
	//����վλ������
	m_pITimerEngine->SetTimer(IDI_KEEP_SITTING_ANDROID, TIME_KEEP_SITTING_ANDROID, (DWORD)(-1), NULL);


	//����ϲ��
	m_pITimerEngine->SetTimer(IDI_UPDATE_HAPPY_MESS, TIME_UPDATE_HAPPY_MESS, DWORD(-1), 0);
	//���µȼ���Ϣ
	m_pITimerEngine->SetTimer(IDI_LOAD_GRADE_INFO, TIME_LOAD_GRADE_INFO, DWORD(1), 0);  //ֻҪ����һ��
	//����ϲ��
	m_pITimerEngine->SetTimer(IDI_SEND_HAPPY_MESS, TIME_SEND_HAPPY_MESS, DWORD(-1), 0);
	//����ǿ��
	m_pITimerEngine->SetTimer(IDI_UPDATE_EXIT_USER, TIME_UPDATE_EXIT_USER, DWORD(-1), 0);
	//�������ݿ�����һֱ����	Author:<cxf>
	m_pITimerEngine->SetTimer(IDI_CONTINUE_CONNECT, TIME_CONTINUE_CONNECT, DWORD(-1), 0);
	//�������
	m_pITimerEngine->SetTimer(IDI_NEWGIFT, TIME_NEWGIFT, DWORD(-1), 0);

	if (m_pRoomLog)
		m_pRoomLog->AddMsg(0, 0, TRUE,"***** ����ģ������");

	//throw(11);	// �����ã������׳��쳣

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//����ģ��ر�
bool __cdecl CAttemperEngineSink::OnAttemperEngineStop(IUnknownEx * pIUnknownEx)
{
	try
	{
		m_UserAutoQueue.Clear();
		m_AndroidAutoQueue.Clear();

		//ע������
		CMD_CS_UnRegGameServer UnRegGameServer;
		memset(&UnRegGameServer,0,sizeof(UnRegGameServer));
		UnRegGameServer.wKindID=m_pGameServiceAttrib->wKindID;
		UnRegGameServer.wServerID=m_pGameServiceOption->wServerID;
		m_pITCPSocketCorrespond->SendData(MDM_CS_SERVER_MANAGER,SUB_CS_UNREG_GAME_SERVER,&UnRegGameServer,sizeof(UnRegGameServer));

		//�ر����
		m_AndroidUserManager.StopService();
		m_ServerUserManager.ResetUserManager();

		//ɾ������
		m_wMaxUserItem=0;
		SafeDeleteArray(m_pTableFrame);
		SafeDeleteArray(m_pNormalParameter);
		SafeDeleteArray(m_pAndroidParameter);

		//���Ʊ���
		m_bShallClose=false;
		m_bAllowWisper=true;
		m_bAllowRoomChat=true;
		m_bAllowGameChat=true;
		m_bAllowEnterRoom=true;
		m_bAllowEnterGame=true;

		//��Ϣ����
		m_cbRoomMsgIndex=0;
		m_cbGameMsgIndex=0;

		//���ñ���
		m_wColumnInfoSize=0;
		m_wOnLineInfoSize=0;
		memset(&m_ColumnInfo,0,sizeof(m_ColumnInfo));
		memset(&m_OnLineCountInfo,0,sizeof(m_OnLineCountInfo));

		if (m_pRoomLog)
			m_pRoomLog->AddMsg(0, 0, TRUE,"***** ����ģ��ر�");
	}
	catch (...)
	{
		int iTmp = 0;
		int iOther = 1;
	}

	return true;
}

//����״̬
bool __cdecl CAttemperEngineSink::SendTableStatus(WORD wTableID)
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч�����
	ASSERT(wTableID<m_pGameServiceOption->wTableCount);
	if (wTableID>=m_pGameServiceOption->wTableCount) return false;

	//�������
	CMD_GR_TableStatus TableStatus;
	memset(&TableStatus,0,sizeof(TableStatus));
	TableStatus.wTableID=wTableID;
	TableStatus.bTableLock=(m_pTableFrame+wTableID)->IsTableLocked()?TRUE:FALSE;
	TableStatus.bPlayStatus=(m_pTableFrame+wTableID)->IsGameStarted()?TRUE:FALSE;

	//��������
	m_pITCPNetworkEngine->SendDataBatch(MDM_GR_STATUS,SUB_GR_TABLE_STATUS,&TableStatus,sizeof(TableStatus));
	m_AndroidUserManager.SendDataToClient(MDM_GR_STATUS,SUB_GR_TABLE_STATUS,&TableStatus,sizeof(TableStatus));

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}


//����״̬
bool __cdecl CAttemperEngineSink::SendUserStatus(IServerUserItem * pIServerUserItem)
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	ASSERT(pIServerUserItem->IsActive()==true);
	if (pIServerUserItem->IsActive()==false) return false;

	//��������
	CMD_GR_UserStatus UserStatus;
	memset(&UserStatus,0,sizeof(UserStatus));
	tagServerUserData * pUserData=pIServerUserItem->GetUserData();

	//��������
	UserStatus.dwUserID=pUserData->dwUserID;
	UserStatus.wTableID=pUserData->wTableID;
	UserStatus.wChairID=pUserData->wChairID;
	UserStatus.cbUserStatus=pUserData->cbUserStatus;

	//��������
	m_pITCPNetworkEngine->SendDataBatch(MDM_GR_USER,SUB_GR_USER_STATUS,&UserStatus,sizeof(UserStatus));
	m_AndroidUserManager.SendDataToClient(MDM_GR_USER,SUB_GR_USER_STATUS,&UserStatus,sizeof(UserStatus));

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}


//�����û�����Ϸ����
bool __cdecl CAttemperEngineSink::UpdateUserItem(IServerUserItem * pIServerUserItem)
{
	__ENTER_FUNCTION_LOG_CATCH

	DBR_GR_WriteUserScore  writeUserScore;
	ZeroMemory(&writeUserScore,sizeof(writeUserScore));

	//���ñ���
	writeUserScore.dwUserID  =pIServerUserItem->GetUserID();
	writeUserScore.dwClientIP=pIServerUserItem->GetClientIP();
	pIServerUserItem->GetUserScoreInfo(writeUserScore.ScoreModifyInfo);//

	//������Ϣ
	writeUserScore.lRevenue=pIServerUserItem->GetRoundRevenue();
	writeUserScore.dwPlayTimeCount=pIServerUserItem->GetRoundPlayTime();
	//Ͷ������


	//����ʹ��

	tagServerUserData * pServerUserData=pIServerUserItem->GetUserData();
	if (writeUserScore.ScoreModifyInfo.lExperience > 999999 ||
		pIServerUserItem->GetUserRevenue() > 999999 )
	{

		char info[BUFSIZ] = {0};
		sprintf(info, "д�����ݿ⣬��ⷢ�־����쳣, id��%i, д��ȼ�:%i, д�뾭�飺%i, ��ҵ�˰��:%i,  ��ҵȼ�: %i�� ��Ҿ��飺%i", 
			pServerUserData->dwUserID,
			writeUserScore.ScoreModifyInfo.dwGrade,
			writeUserScore.ScoreModifyInfo.lExperience,
			pIServerUserItem->GetUserRevenue(),
			pServerUserData->UserScoreInfo.dwGrade,
			pServerUserData->UserScoreInfo.lExperience
			);
		CTraceService::TraceString(info,TraceLevel_Normal);
	}

	if ( pServerUserData->UserScoreInfo.lExperience < 0 && writeUserScore.ScoreModifyInfo.lExperience < 0)
	{
		char info[BUFSIZ] = {0};
		sprintf(info, "д�����ݿ⣬��ⷢ����Ҿ��鲻һ��, id��%i, д��ȼ�:%i, д�뾭�飺%i, ��ҵ�˰��:%i,  ��ҵȼ�: %i�� ��Ҿ��飺%i", 
			pServerUserData->dwUserID,
			writeUserScore.ScoreModifyInfo.dwGrade,
			writeUserScore.ScoreModifyInfo.lExperience,
			pIServerUserItem->GetUserRevenue(),
			pServerUserData->UserScoreInfo.dwGrade,
			pServerUserData->UserScoreInfo.lExperience
			);
	}
	//������������� ʱ�����
	int iOnlineTime=pIServerUserItem->GetUserData()->dwOnlineTime;
	int iOnlineGiftCount=pIServerUserItem->GetUserData()->wOnlineGiftCount;
	int dwMaxVal[]={600, 1800, 3600, 7200};
	for (int i=0;i<4;i++)
	{
		if (i==iOnlineGiftCount-1 && iOnlineTime<dwMaxVal[i])
		{
			int iTimeOn=dwMaxVal[i]-iOnlineTime+1;
			if (iTimeOn>0)
			{
				writeUserScore.dwPlayTimeCount+=iTimeOn;
				break;
			}
		}
	}
	//
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_WRITE_GAME_SCORE, 0, &writeUserScore, sizeof(writeUserScore));
	pIServerUserItem->ClearScorModify();

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//�����û�
bool  CAttemperEngineSink::UnLockUserItem(IServerUserItem * pIServerUserItem)
{
	//Ч�����
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->IsActive()==true));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->IsActive()==false)) return true;

	//Ч��״̬
	ASSERT(pIServerUserItem->GetTableID()==INVALID_TABLE);
	ASSERT(pIServerUserItem->GetChairID()==INVALID_CHAIR);

	//��ȡ�û�
	tagServerUserData *pServerUserData = pIServerUserItem->GetUserData();
	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_UNLOCK_USER,0,&(pServerUserData->dwUserID),sizeof(pServerUserData->dwUserID));
	return true;
}


//ɾ���û�
bool __cdecl CAttemperEngineSink::DeleteUserItem(IServerUserItem * pIServerUserItem)
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч�����
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->IsActive()==true));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->IsActive()==false)) return false;

	//Ч��״̬
	ASSERT(pIServerUserItem->GetTableID()==INVALID_TABLE);
	ASSERT(pIServerUserItem->GetChairID()==INVALID_CHAIR);

	//��ȡ�û�
	tagServerUserData *pServerUserData = pIServerUserItem->GetUserData();

	//д�����
	DBR_GR_LeaveGameServer LeaveGameServer;
	ZeroMemory(&LeaveGameServer,sizeof(LeaveGameServer));

	//���ñ���
	LeaveGameServer.dwUserID     = pIServerUserItem->GetUserID();
	LeaveGameServer.dwClientIP   = pIServerUserItem->GetClientIP();
	pIServerUserItem->GetUserScoreInfo(LeaveGameServer.ScoreModifyInfo);//

	//������Ϣ
	LeaveGameServer.lLoveliness  = pIServerUserItem->GetUserData()->lLoveliness;
	LeaveGameServer.lRevenue     = pIServerUserItem->GetUserRevenue();
	LeaveGameServer.dwPlayTimeCount  = pIServerUserItem->GetPlayTimeCount();
	LeaveGameServer.dwOnlineTimeCount=pIServerUserItem->GetOnlineTimeCount();

	//Ͷ������
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_LEAVE_GAME_SERVER,0,&LeaveGameServer,sizeof(LeaveGameServer));

	//����״̬
	CMD_GR_UserStatus UserStatus;
	ZeroMemory(&UserStatus,sizeof(UserStatus));
	tagServerUserData * pUserData=pIServerUserItem->GetUserData();
	UserStatus.dwUserID=pIServerUserItem->GetUserID();
	UserStatus.wTableID=INVALID_TABLE;
	UserStatus.wChairID=INVALID_CHAIR;
	UserStatus.cbUserStatus=US_NULL;
	m_pITCPNetworkEngine->SendDataBatch(MDM_GR_USER,SUB_GR_USER_STATUS,&UserStatus,sizeof(UserStatus));
	m_AndroidUserManager.SendDataToClient(MDM_GR_USER,SUB_GR_USER_STATUS,&UserStatus,sizeof(UserStatus));

	//��������
	WORD wBindIndex=pIServerUserItem->GetUserIndex();
	if (wBindIndex<m_pGameServiceOption->wMaxConnect)
	{
		(m_pNormalParameter+wBindIndex)->pIServerUserItem=NULL;
	}
	else if ((wBindIndex>=INDEX_ANDROID)&&(wBindIndex<(INDEX_ANDROID+MAX_ANDROID)))
	{
		(m_pAndroidParameter+(wBindIndex-INDEX_ANDROID))->pIServerUserItem=NULL;
	}

	//ɾ���û�
	m_ServerUserManager.DeleteUserItem(pIServerUserItem);

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//��������
bool __cdecl CAttemperEngineSink::SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID)
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	ASSERT(pIServerUserItem->IsActive()==true);
	if (pIServerUserItem->IsActive()==false) return false;

	//��������
	WORD wIndex=pIServerUserItem->GetUserIndex();
	if (wIndex!=INVALID_WORD)
	{
		tagConnectItemInfo * pConnectItemInfo=GetBindParameter(wIndex);
		if (wIndex>=INDEX_ANDROID)
		{
			DWORD dwID=pConnectItemInfo->dwSocketID;
			m_AndroidUserManager.SendDataToClient(dwID,wMainCmdID,wSubCmdID,NULL,0);
		}
		else
		{
			m_pITCPNetworkEngine->SendData(pConnectItemInfo->dwSocketID,wMainCmdID,wSubCmdID);
		}
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//��������
bool __cdecl CAttemperEngineSink::SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	ASSERT(pIServerUserItem->IsActive()==true);
	if (pIServerUserItem->IsActive()==false) return false;

	//��������
	WORD wIndex=pIServerUserItem->GetUserIndex();
	if (wIndex!=INVALID_WORD)
	{
		tagConnectItemInfo * pConnectItemInfo=GetBindParameter(wIndex);
		if (wIndex>=INDEX_ANDROID)
		{
			DWORD dwID=pConnectItemInfo->dwSocketID;
			m_AndroidUserManager.SendDataToClient(dwID,wMainCmdID,wSubCmdID,pData,wDataSize);
		}
		else
		{
			m_pITCPNetworkEngine->SendData(pConnectItemInfo->dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);
		}
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//���ö�ʱ��
bool __cdecl CAttemperEngineSink::SetTableTimer(WORD wTableID, DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM wBindParam)
{
	//Ч�����
	ASSERT(dwTimerID<RANGE_TABLE_TIME);
	ASSERT(wTableID<m_pGameServiceOption->wTableCount);

	//���ö�ʱ��
	if (wTableID>=m_pGameServiceOption->wTableCount) return false;
	DWORD dwTableTimeID=IDI_TABLE_FRAME_START+wTableID*RANGE_TABLE_TIME+dwTimerID;
	return m_pITimerEngine->SetTimer(dwTableTimeID,dwElapse,dwRepeat,wBindParam);
}

//ɾ����ʱ��
bool __cdecl CAttemperEngineSink::KillTableTimer(WORD wTableID, DWORD dwTimerID)
{
	//Ч�����
	ASSERT(dwTimerID<RANGE_TABLE_TIME);
	ASSERT(wTableID<m_pGameServiceOption->wTableCount);

	//ɾ����ʱ��
	if (wTableID>=m_pGameServiceOption->wTableCount) return false;
	DWORD dwTableTimeID=IDI_TABLE_FRAME_START+wTableID*RANGE_TABLE_TIME+dwTimerID;
	return m_pITimerEngine->KillTimer(dwTableTimeID);
}

//�����Ϣ
void __cdecl CAttemperEngineSink::ExportInformation(LPCTSTR pszString, enTraceLevel TraceLevel)
{
	CTraceService::TraceString(pszString,TraceLevel);
	return;
}

//���͵�¼ʧ��
bool CAttemperEngineSink::SendLogonFailed(DWORD dwSocketID, LPCTSTR pszMessage, LONG lErrorCode)
{
	__ENTER_FUNCTION_LOG_CATCH


	//��������
	CMD_GR_LogonError LogonError;
	LogonError.lErrorCode=lErrorCode;
	lstrcpyn(LogonError.szErrorDescribe,pszMessage,CountArray(LogonError.szErrorDescribe));
	WORD wDescribeSize=CountStringBuffer(LogonError.szErrorDescribe);

	//��������
	WORD wSendSize=sizeof(LogonError)-sizeof(LogonError.szErrorDescribe)+wDescribeSize;
	if (LOWORD(dwSocketID)>=INDEX_ANDROID)
	{
		m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_ERROR,&LogonError,wSendSize);
	}
	else m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_ERROR,&LogonError,wSendSize);

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//���ͷ���������
bool CAttemperEngineSink::SendLogonFull(DWORD dwSocketID, LPCTSTR pszMessage, DWORD userCount)
{
	__ENTER_FUNCTION_LOG_CATCH


	//��������
	CMD_GR_LogonFull LogonError;
	LogonError.dwCountUser = userCount;
	lstrcpyn(LogonError.szErrorDescribe,pszMessage,CountArray(LogonError.szErrorDescribe));
	WORD wDescribeSize=CountStringBuffer(LogonError.szErrorDescribe);

	//��������
	WORD wSendSize=sizeof(LogonError)-sizeof(LogonError.szErrorDescribe)+wDescribeSize;
	if (LOWORD(dwSocketID)>=INDEX_ANDROID)
	{
		m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_FULL,&LogonError,wSendSize);
	}
	else m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GR_LOGON, SUB_GR_LOGON_FULL,&LogonError,wSendSize);

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}


//���͵�����Ϣ
bool CAttemperEngineSink::SendProMessage(DWORD dwSocketID, LPCTSTR lpszMessage, WORD wMessageType)
{
	//tagConnectItemInfo * pConnectItemInfo=GetBindParameter(LOWORD(dwSocketID));
	//IServerUserItem *pIServerUserItem = pConnectItemInfo->pIServerUserItem;
	//if(pIServerUserItem->GetTableID()!=INVALID_TABLE)
	//	SendGameMessage(dwSocketID, lpszMessage, wMessageType);
	//else SendRoomMessage(dwSocketID, lpszMessage, wMessageType);

	//�жϳ���
	if(m_cbSendLocation==LOCATION_GAME_ROOM)
		SendGameMessage(dwSocketID, lpszMessage, wMessageType);
	else SendRoomMessage(dwSocketID, lpszMessage, wMessageType);

	return true;
}

//���ͷ�����Ϣ
bool CAttemperEngineSink::SendRoomMessage(DWORD dwSocketID, LPCTSTR lpszMessage, WORD wMessageType)
{
	__ENTER_FUNCTION_LOG_CATCH

	//�������ݰ�
	CMD_GR_Message Message;
	Message.wMessageType=wMessageType;
	lstrcpyn(Message.szContent,lpszMessage,CountArray(Message.szContent));
	Message.wMessageLength=CountStringBuffer(Message.szContent);

	//��������
	WORD wSendSize=sizeof(Message)-sizeof(Message.szContent)+Message.wMessageLength*sizeof(TCHAR);
	if (LOWORD(dwSocketID)==INVALID_WORD)
	{
		m_pITCPNetworkEngine->SendDataBatch(MDM_GR_SYSTEM,SUB_GR_MESSAGE,&Message,wSendSize);
		m_AndroidUserManager.SendDataToClient(MDM_GR_SYSTEM,SUB_GR_MESSAGE,&Message,wSendSize);
	}
	else
	{
		if (LOWORD(dwSocketID)>=INDEX_ANDROID)
		{
			m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_GR_SYSTEM,SUB_GR_MESSAGE,&Message,wSendSize);
		}
		else m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GR_SYSTEM,SUB_GR_MESSAGE,&Message,wSendSize);
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//������Ϸ��Ϣ
bool CAttemperEngineSink::SendGameMessage(DWORD dwSocketID, LPCTSTR lpszMessage, WORD wMessageType)
{
	__ENTER_FUNCTION_LOG_CATCH

	//�������ݰ�
	CMD_GF_Message Message;
	Message.wMessageType=wMessageType;
	lstrcpyn(Message.szContent,lpszMessage,CountArray(Message.szContent));
	Message.wMessageLength=CountStringBuffer(Message.szContent);

	//��������
	WORD wSendSize=sizeof(Message)-sizeof(Message.szContent)+Message.wMessageLength*sizeof(TCHAR);
	if (LOWORD(dwSocketID)==INVALID_WORD)
	{
		m_pITCPNetworkEngine->SendDataBatch(MDM_GF_FRAME,SUB_GF_MESSAGE,&Message,wSendSize);
		m_AndroidUserManager.SendDataToClient(MDM_GF_FRAME,SUB_GF_MESSAGE,&Message,wSendSize);
	}
	else
	{
		if (LOWORD(dwSocketID)>=INDEX_ANDROID)
		{
			m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_GF_FRAME,SUB_GF_MESSAGE,&Message,wSendSize);
		}
		else m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GF_FRAME,SUB_GF_MESSAGE,&Message,wSendSize);
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//���͵�����Ϣ
bool CAttemperEngineSink::SendProMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType)
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч���û�
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	WORD wIndex=pIServerUserItem->GetUserIndex();
	if (wIndex!=INVALID_WORD)
	{
		tagConnectItemInfo * pConnectItemInfo=GetBindParameter(wIndex);
		SendProMessage(pConnectItemInfo->dwSocketID,lpszMessage,wMessageType);
		return true;
	}

	__LEAVE_FUNCTION_LOG_CATCH


	return true;
}

//���ͷ�����Ϣ
bool CAttemperEngineSink::SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType)
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч���û�
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	WORD wIndex=pIServerUserItem->GetUserIndex();
	if (wIndex!=INVALID_WORD)
	{
		tagConnectItemInfo * pConnectItemInfo=GetBindParameter(wIndex);
		SendRoomMessage(pConnectItemInfo->dwSocketID,lpszMessage,wMessageType);
		return true;
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return false;
}

//������Ϸ��Ϣ
bool CAttemperEngineSink::SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wMessageType)
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч���û�
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//�û�����
	if ((pIServerUserItem->GetTableID()==INVALID_TABLE)||(pIServerUserItem->GetChairID()==INVALID_CHAIR)) return false;

	//��������
	WORD wIndex=pIServerUserItem->GetUserIndex();
	if (wIndex!=INVALID_WORD)
	{
		tagConnectItemInfo * pConnectItemInfo=GetBindParameter(wIndex);
		SendGameMessage(pConnectItemInfo->dwSocketID,lpszMessage,wMessageType);

		return true;
	}

	__LEAVE_FUNCTION_LOG_CATCH


	return false;
}

//��ʱ���¼�
bool __cdecl CAttemperEngineSink::OnEventTimer(DWORD dwTimerID, WPARAM wBindParam)
{
	__ENTER_FUNCTION_LOG_CATCH

	//�ں˶�ʱ��
	if ((dwTimerID>=IDI_ATTEMPER_START)&&(dwTimerID<=IDI_ATTEMPER_CONCLUDE))
	{
		switch (dwTimerID)
		{
			//�������
		case IDI_SEND_HAPPY_MESS: //����ϲ��������������������û�
			{
				SendHappyMess();
				return true;
			}
		case IDI_UPDATE_EXIT_USER: //����ǿ���û�
			{
				UserLeftDataSetIT it    = m_userLeftDataSet.begin();
				UserLeftDataSetIT endit = m_userLeftDataSet.end();
				for (; it != endit;)
				{
					UserLeftDataSetIT cpIt = it;
					++cpIt;
					if (time(NULL) - (*it).second > TWO_MIN)
					{
						m_userLeftDataSet.erase(it);
					}
					it = cpIt;
				}
				return true;
			}

		case IDI_UPDATE_AFFICHE:  //���¹���
			{
				m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_LOAD_AFFICHE, 0 , NULL, 0); //������¹���
				return true;
			}
		case IDI_UPDATE_HAPPY_MESS: //����ϲ����Ϣ
			{
				m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_LOAD_HAPPY_MESS, 0, NULL, 0);
				return true;
			}
		case IDI_LOAD_GRADE_INFO: //����ȼ���Ϣ
			{
				m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_READ_GRADE_INFO, 0, 0, 0);  //������µȼ���Ϣ
				return true;
			}

		case IDI_UPDATE_BUSY_DESK:  //���·�æ����������
			{
				UpdateBusyDesk();
				return true;
			}
		case IDI_UPDATE_ADD_USER: //���Խ��û����뵽��Ϸ ��Ҫ�Ǵ���2���û��ȴ�������һ���û�����Ϳ�����Ϸ�����
			{
				TryAddUser();
				return true;
			}
		case IDI_KEEP_SITTING_ANDROID: //���Լ���վλ������
			{
				AddKeepSittingAndroid();
				return true;
			}

		case IDI_UPDATE_ADD_ANDROID: //���������
			{
				UpDateAddAndroid();
				return true;
			}

			//��ӽ���
		case IDI_CONNECT_CENTER_SERVER:		//���ӷ�����
			{
				//��������
				m_pITCPSocketCorrespond->Connect(ntohl(m_dwCenterServer),PORT_CENTER_SERVER);

				//������ʾ
				CTraceService::TraceString(TEXT("���ڳ��������������ķ�����...."),TraceLevel_Normal);

				return true;
			}
		case IDI_UPDATE_SERVER_LIST:		//�����б�
			{
				m_pITCPSocketCorrespond->SendData(MDM_CS_SERVER_LIST,SUB_CS_GET_SERVER_LIST,NULL,0);
				return true;
			}
		case IDI_UPDATE_ONLINE_COUNT:		//��������
			{
				//��������
				CMD_CS_ServerOnLineCount ServerOnLineCount;
				ServerOnLineCount.wKindID=m_pGameServiceOption->wKindID;
				ServerOnLineCount.wServerID=m_pGameServiceOption->wServerID;
				ServerOnLineCount.dwOnLineCount=(m_ServerUserManager.GetOnLineCount()+m_ServerUserManager.GetOffLineCount());
				m_pITCPSocketCorrespond->SendData(MDM_CS_SERVER_MANAGER,SUB_CS_SERVER_ONLINE_COUNT,&ServerOnLineCount,sizeof(ServerOnLineCount));

				return true;
			}
		case IDI_SEND_SYSTEM_MESSAGE:		//ϵͳ��Ϣ
			{
				//��ȡĿ¼
				TCHAR szPath[MAX_PATH]=TEXT("");
				GetCurrentDirectory(sizeof(szPath),szPath);

				//��ȡ����
				TCHAR szFileName[MAX_PATH],szMessage[1024];
				_snprintf(szFileName,sizeof(szFileName),TEXT("%s\\GameService.ini"),szPath);

				//ѭ����Ϣ
				TCHAR szRoomMsgName[64],szGameMsgName[64];
				_snprintf(szRoomMsgName,sizeof(szRoomMsgName),TEXT("RoomMessage%d"),m_cbRoomMsgIndex++);
				_snprintf(szGameMsgName,sizeof(szGameMsgName),TEXT("GameMessage%d"),m_cbGameMsgIndex++);

				//ϵͳ��Ϣ
				GetPrivateProfileString(TEXT("Message"),szRoomMsgName,TEXT(""),szMessage,sizeof(szMessage),szFileName);
				if (szMessage[0]!=0) SendRoomMessage(INVALID_WORD,szMessage,SMT_INFO);
				else m_cbRoomMsgIndex=0;

				//��Ϸ��Ϣ
				GetPrivateProfileString(TEXT("Message"),szGameMsgName,TEXT(""),szMessage,sizeof(szMessage),szFileName);
				if (szMessage[0]!=0) SendGameMessage(INVALID_WORD,szMessage,SMT_INFO);
				else m_cbGameMsgIndex=0;

				return true;
			}
		case IDI_CONTINUE_CONNECT:		//�������ݿ����Ӵ���	Author:<cxf>
			{
				m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_CONTINUE_CONNECT, 0, 0, 0);
				return true;
			}
		case IDI_NEWGIFT:				//�������	Author<cxf>
			{
				SendNewGift();
				return true;
			}
		}
	}

	//���Ӷ�ʱ��
	if (dwTimerID>=IDI_TABLE_FRAME_START)
	{
		//��������
		WORD wTableID=(WORD)((dwTimerID-IDI_TABLE_FRAME_START)/RANGE_TABLE_TIME);
		DWORD dwTableTimeID=(DWORD)((dwTimerID-IDI_TABLE_FRAME_START)%RANGE_TABLE_TIME);

		//��ʱ������
		ASSERT(wTableID<m_pGameServiceOption->wTableCount);
		return (m_pTableFrame+wTableID)->OnEventTimer(dwTableTimeID,wBindParam);
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return false;
}

//���ݿ��¼�
bool __cdecl CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBR_GR_LOGON_SUCCESS:		//��¼�ɹ�
		{
			return OnDBLogonSuccess(dwContextID,pData,wDataSize);
		}
	case DBR_GR_LOGON_ERROR:		//��¼ʧ��
		{
			return OnDBLogonError(dwContextID,pData,wDataSize);
		}
	case DBR_GR_USER_RIGHT_RESULT:	//�û�Ȩ��
		{
			return OnDBUserRightResult(dwContextID,pData,wDataSize);
		}
		//case DBR_GR_PROPERTY_ATTRIBUTE:	//���ص���
		//	{
		//		return OnDBLoadProperty(dwContextID,pData,wDataSize);
		//	}
	case DBR_GR_USER_FIND_GEMS_TASK_LOAD: //��ȡѰ������
		{
			return OnDBFindGemsTaskLoad(pData,wDataSize);
		}
	case DBR_GR_RE_FIND_GEMS_TASK_PERFORM_FINISH: //Ѱ���������
		{
			return SendTaskFinish(pData, wDataSize );
		}

	case DBR_GR_AFFICHE_ONE: //���ع���
		{
			return OnDBLoadAffiche(dwContextID, pData, wDataSize);
		}
	case DBR_GR_HAPPY_MESS_ONE: //����ϲ��
		{
			return OnDBLoadHappyMess(dwContextID, pData, wDataSize);
		}

	case DBR_GR_AFFICHE_SYNC: //ͬ������
		{
			m_strAffiche = "";
			return true;
		}
	case DBR_GR_HAPPY_MESS_SYNC   : //ͬ��ϲ��
		{
			m_strHappyMess = "";
			return true;
		}
	default:
		return true;
	}

	return false;
}

//�û���¼�ɹ�
bool CAttemperEngineSink::OnDBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_LogonSuccess));
	if (wDataSize!=sizeof(DBR_GR_LogonSuccess)) 
		return true;

	//�ж�����
	tagConnectItemInfo * pConnectItemInfo=GetBindParameter(LOWORD(dwContextID));
	if (pConnectItemInfo==NULL)
		return true;
	if (pConnectItemInfo->dwSocketID!=dwContextID)
		return true;
	ASSERT(pConnectItemInfo->pIServerUserItem==NULL);

	//��Ϣ����
	DBR_GR_LogonSuccess * pLogonSuccess=(DBR_GR_LogonSuccess *)pData;

	//�����ж�
	IServerUserItem * pIServerUserItem=m_ServerUserManager.SearchOnLineUser(pLogonSuccess->dwUserID);
	if (pIServerUserItem!=NULL)
		return true;

	//��������
	WORD wIndex=LOWORD(dwContextID);

	//״̬�ж�
	if ((m_bAllowEnterRoom==false)&&(pLogonSuccess->dwMasterRight==0L))
	{
		//������Ϣ
		LPCTSTR pszMessage=TEXT("��Ǹ������ϵͳά����ԭ�򣬱���Ϸ���䲻�����κ���ҵ�¼���룡");
		SendRoomMessage(dwContextID,pszMessage,SMT_EJECT|SMT_INFO|SMT_INTERMIT_LINE|SMT_CLOSE_ROOM);

		//�������
		DBR_GR_LeaveGameServer LeaveGameServer;
		ZeroMemory(&LeaveGameServer,sizeof(LeaveGameServer));
		LeaveGameServer.dwUserID=pLogonSuccess->dwUserID;
		LeaveGameServer.dwClientIP=pConnectItemInfo->dwClientIP;
		LeaveGameServer.lLoveliness=pLogonSuccess->lLoveliness;
		m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_LEAVE_GAME_SERVER,0,&LeaveGameServer,sizeof(LeaveGameServer));

		return true;
	}

	//�Ƿ�ǿ�˳�ʱ ����
#if 1
	UserLeftDataSetIT it = m_userLeftDataSet.find(pLogonSuccess->dwUserID);
	if (it != m_userLeftDataSet.end())
	{
		time_t passTime = time(NULL) - (*it).second;
		if (passTime <=  TWO_MIN)
		{
			int minu = (TWO_MIN - passTime) / 60;
			int sec  = (TWO_MIN - passTime) % 60;

			DBR_GR_LogonError LogonError;
			LogonError.lErrorCode=0;
			if (minu > 0)
			{
				_snprintf(LogonError.szErrorDescribe,sizeof(LogonError.szErrorDescribe),TEXT("ǿ�˺�%i��%i���ڲ��ܵ�½��Ϸ��"), minu, sec);
			}else
			{
				_snprintf(LogonError.szErrorDescribe,sizeof(LogonError.szErrorDescribe),TEXT("ǿ�˺�%i���ڲ��ܵ�½��Ϸ��"), sec);
			}
		
			SendLogonFailed(dwContextID, LogonError.szErrorDescribe, LogonError.lErrorCode);
			//�������
			DBR_GR_LeaveGameServer LeaveGameServer;
			ZeroMemory(&LeaveGameServer,sizeof(LeaveGameServer));
			LeaveGameServer.dwUserID=pLogonSuccess->dwUserID;
			LeaveGameServer.dwClientIP=pConnectItemInfo->dwClientIP;
			LeaveGameServer.lLoveliness=pLogonSuccess->lLoveliness;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_LEAVE_GAME_SERVER,0,&LeaveGameServer,sizeof(LeaveGameServer));
			return true;
		}else 
		{
			m_userLeftDataSet.erase(it);
		}
	}
#endif


	if (pLogonSuccess->lGold < m_pGameServiceOption->lRestrictScore)
	{
		DBR_GR_LogonError LogonError;
		LogonError.lErrorCode=0;
		_snprintf(LogonError.szErrorDescribe,sizeof(LogonError.szErrorDescribe),TEXT("���Ľ�Ҳ���,���ܽ��뷿�䣡"));
		SendLogonFailed(dwContextID, LogonError.szErrorDescribe, LogonError.lErrorCode);
		//�������
		DBR_GR_LeaveGameServer LeaveGameServer;
		ZeroMemory(&LeaveGameServer,sizeof(LeaveGameServer));
		LeaveGameServer.dwUserID=pLogonSuccess->dwUserID;
		LeaveGameServer.dwClientIP=pConnectItemInfo->dwClientIP;
		LeaveGameServer.lLoveliness=pLogonSuccess->lLoveliness;
		m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_LEAVE_GAME_SERVER,0,&LeaveGameServer,sizeof(LeaveGameServer));
		return true;
	}


	//�ж���Ŀ
	DWORD dwUserCount=m_ServerUserManager.GetOnLineCount()+m_ServerUserManager.GetOffLineCount();
	if ((pLogonSuccess->cbMemberOrder==0)&&(pLogonSuccess->cbMasterOrder==0)&&((dwUserCount+LESS_CONNECT_COUNT)>=m_pGameServiceOption->wMaxConnect))
	{
		//������Ϣ
		LPCTSTR pszMessage=TEXT("��Ǹ������Ϸ�����Ѿ����ˣ���ֹ�ǻ�Ա��ҵ�¼���룡");
		SendRoomMessage(dwContextID,pszMessage,SMT_EJECT|SMT_INFO|SMT_INTERMIT_LINE|SMT_CLOSE_ROOM);

		//�������
		DBR_GR_LeaveGameServer LeaveGameServer;
		ZeroMemory(&LeaveGameServer,sizeof(LeaveGameServer));
		LeaveGameServer.dwUserID=pLogonSuccess->dwUserID;
		LeaveGameServer.dwClientIP=pConnectItemInfo->dwClientIP;
		LeaveGameServer.lLoveliness=pLogonSuccess->lLoveliness;
		m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_LEAVE_GAME_SERVER,0,&LeaveGameServer,sizeof(LeaveGameServer));

		return true;
	}

	//�����û���Ϣ
	tagServerUserData ServerUserData;
	memset(&ServerUserData,0,sizeof(ServerUserData));
	ServerUserData.wFaceID=pLogonSuccess->wFaceID;
	ServerUserData.dwCustomFaceVer=pLogonSuccess->dwCustomFaceVer;
	ServerUserData.cbGender=pLogonSuccess->cbGender;
	ServerUserData.dwUserID=pLogonSuccess->dwUserID;
	ServerUserData.dwGameID=pLogonSuccess->dwGameID;
	ServerUserData.dwGroupID=pLogonSuccess->dwGroupID;
	ServerUserData.dwUserRight=pLogonSuccess->dwUserRight;
	ServerUserData.lLoveliness=pLogonSuccess->lLoveliness;
	ServerUserData.dwMasterRight=pLogonSuccess->dwMasterRight;
	ServerUserData.cbMemberOrder=pLogonSuccess->cbMemberOrder;
	ServerUserData.cbMasterOrder=pLogonSuccess->cbMasterOrder;
	ServerUserData.UserScoreInfo.lWinCount=pLogonSuccess->lWinCount;
	ServerUserData.UserScoreInfo.lLostCount=pLogonSuccess->lLostCount;
	ServerUserData.UserScoreInfo.lDrawCount=pLogonSuccess->lDrawCount;
	ServerUserData.UserScoreInfo.lFleeCount=pLogonSuccess->lFleeCount;
	ServerUserData.UserScoreInfo.lExperience=pLogonSuccess->lExperience;

	//�������
	ServerUserData.UserScoreInfo.lGold      = pLogonSuccess->lGold;
	ServerUserData.UserScoreInfo.lGem      = pLogonSuccess->lGem;
	ServerUserData.UserScoreInfo.dwGrade     = pLogonSuccess->dwGrade;

	lstrcpyn(ServerUserData.szNickName,pLogonSuccess->szNickName,CountArray(ServerUserData.szNickName));
	ServerUserData.utUserType                   =  pLogonSuccess->utIsAndroid;
	//��ӽ���
	lstrcpyn(ServerUserData.szAccounts,pLogonSuccess->szAccounts,CountArray(ServerUserData.szAccounts));
	lstrcpyn(ServerUserData.szGroupName,pLogonSuccess->szGroupName,CountArray(ServerUserData.szGroupName));
	lstrcpyn(ServerUserData.szUnderWrite,pLogonSuccess->szUnderWrite,CountArray(ServerUserData.szUnderWrite));
	lstrcpyn(ServerUserData.szBankPassword,pLogonSuccess->szBankPassword,CountArray(ServerUserData.szBankPassword));

	//��ȡ����
	ServerUserData.lStorageGold=0L;


	//�洢����
	if ((m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)&&(m_pGameServiceOption->lRestrictScore>0L))
	{
		if (pLogonSuccess->lGold>m_pGameServiceOption->lRestrictScore && m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
		{
			ServerUserData.UserScoreInfo.lGold=m_pGameServiceOption->lRestrictScore;
			ServerUserData.lStorageGold = pLogonSuccess->lGold - m_pGameServiceOption->lRestrictScore;

		}
	}


	//���ѽ��
	ServerUserData.UserScoreInfo.lMoney=pLogonSuccess->lMoney;
	//״̬��Ϣ
	ServerUserData.cbUserStatus=US_FREE;
	ServerUserData.wTableID=INVALID_TABLE;
	ServerUserData.wChairID=INVALID_CHAIR;
	ServerUserData.dwOnlineTime=pLogonSuccess->lOnlineTime;				//�û�һ������ʱ��
	ServerUserData.wOnlineGiftCount=pLogonSuccess->wOnlineGiftCount;				//�û������������

	//Author<cxf>;DateTime<2012-03-02>
	ServerUserData.UserScoreInfo.lKindIDWinCount=pLogonSuccess->lKindIDWinCount;
	ServerUserData.UserScoreInfo.lKindIDLostCount=pLogonSuccess->lKindIDLostCount;
	ServerUserData.UserScoreInfo.lKindIDDrawCount=pLogonSuccess->lKindIDDrawCount;
	ServerUserData.UserScoreInfo.lKindIDFleeCount=pLogonSuccess->lKindIDFleeCount;
	//END

	//�����û�
	pConnectItemInfo->pIServerUserItem=m_ServerUserManager.ActiveUserItem(ServerUserData,pConnectItemInfo->dwClientIP,wIndex,pLogonSuccess->szPassWord,m_pGameServiceOption->wServerType);

	//���ͷ�����Ϣ
	SendGameServerInfo(pConnectItemInfo->pIServerUserItem,dwContextID);

	//�㲥�û���Ϣ
	SendUserItem(pConnectItemInfo->pIServerUserItem,INVALID_WORD);
	//���Ͷ������� //�������
	SendQueueCount(pConnectItemInfo->pIServerUserItem);

	//���Ĳ���
	//OnUserUpdateProp(12, pConnectItemInfo->pIServerUserItem, 0);

	SendAffiche(pConnectItemInfo->pIServerUserItem); //������ӣ�������Ϸ����
	//������Ϣ
	SendRoomMessage(dwContextID,TEXT("�װ����û�����ӭ�����ᱦ���飡"),SMT_INFO);

	//�����ж�
	if (m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)
	{
		LPCTSTR pszMessage=TEXT("��ע�⣬������Ϸ�������䣬�Ǳ���ѡ�ֽ������ڴ˷��������Ϸ��");
		SendRoomMessage(dwContextID,pszMessage,SMT_INFO);
	}

	//�����׷���
	if (m_pGameServiceOption->cbHideUserInfo==TRUE)
	{
		LPCTSTR pszMessage=TEXT("��ע�⣬���Ƿ�������Ϸ���䣬���� [ 1 ] �����ӽ��н�����Ϸ��");
		SendRoomMessage(dwContextID,pszMessage,SMT_INFO);
	}

	//������Ϣ
	if (m_bShallClose==true)
	{
		LPCTSTR pszMessage=TEXT("��ע�⣬������Ҫ������Ϸ����ά�����¹���������Ϸ���伴����ͣ����");
		SendRoomMessage(dwContextID,pszMessage,SMT_INFO);
	}


	//����Ⱥ��
	if (wIndex<INDEX_ANDROID) m_pITCPNetworkEngine->AllowBatchSend(dwContextID,true);

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//�û���¼ʧ��
bool CAttemperEngineSink::OnDBLogonError(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_LogonError));
	if (wDataSize!=sizeof(DBR_GR_LogonError))
		return true;

	//�ж�����
	tagConnectItemInfo * pConnectItemInfo=GetBindParameter(LOWORD(dwContextID));
	if (pConnectItemInfo==NULL)
		return true;
	if (pConnectItemInfo->dwSocketID!=dwContextID) 
		return true;

	//������Ϣ
	DBR_GR_LogonError * pLogonError=(DBR_GR_LogonError *)pData;
	pLogonError->szErrorDescribe[CountArray(pLogonError->szErrorDescribe)-1]=0;
	SendLogonFailed(dwContextID,pLogonError->szErrorDescribe,pLogonError->lErrorCode);

	//�ر�����
	if (LOWORD(dwContextID)>=INDEX_ANDROID)
	{
		m_AndroidUserManager.DeleteAndroidUserItem(dwContextID);
	}
	else
	{
		m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}



//�û�Ȩ��
bool CAttemperEngineSink::OnDBUserRightResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_UserRightResult));
	if (wDataSize!=sizeof(DBR_GR_UserRightResult)) return false;

	//��Ϣ����
	DBR_GR_UserRightResult * pUserRightResult=(DBR_GR_UserRightResult *)pData;

	//��ȡ���
	IServerUserItem * pSendIServerUserItem=m_ServerUserManager.SearchOnLineUser(pUserRightResult->dwMasterUserID);
	IServerUserItem * pRcvIServerUserItem=m_ServerUserManager.SearchOnLineUser(pUserRightResult->dwUserID);
	if(pSendIServerUserItem==NULL || pRcvIServerUserItem==NULL)return false;
	tagServerUserData * pSendUserData=pSendIServerUserItem->GetUserData();

	//�ɹ��ж�
	if (pUserRightResult->bGameSuccess || pUserRightResult->bAccountsSuccess)
	{
		//���͹���Ա
		WORD wEnumIndex=0;
		IServerUserItem * pIServerUserItemSend=NULL;
		while (true)
		{
			pIServerUserItemSend=m_ServerUserManager.EnumOnLineUser(wEnumIndex++);
			if (pIServerUserItemSend==NULL) break;
			if (pIServerUserItemSend->GetUserData()->cbMasterOrder==FALSE) continue;

			//��������
			CMD_GR_UserRight UserRight;
			memset(&UserRight,0,sizeof(UserRight));
			UserRight.dwUserRight = pUserRightResult->dwUserRight;
			UserRight.dwUserID = pRcvIServerUserItem->GetUserData()->dwUserID;

			//��������
			SendData(pIServerUserItemSend,MDM_GR_USER,SUB_GR_USER_RIGHT,&UserRight,sizeof(UserRight));
		}
	}
	else
	{
		//������Ϣ
		TCHAR szDescribe[128]=TEXT("");
		_snprintf(szDescribe,sizeof(szDescribe),TEXT("�����û�Ȩ��ʧ�ܣ�"));
		SendGameMessage(pSendIServerUserItem,szDescribe,SMT_EJECT|SMT_INFO);		
		return true;
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}





//����Ӧ���¼�
bool __cdecl CAttemperEngineSink::OnEventTCPNetworkBind(DWORD dwClientIP, DWORD dwSocketID)
{
#if 0	// �����ã�2012-03-29
	CString strPrompt;
	strPrompt.Format("OnEventTCPNetworkBind dwClientIP=%d ", dwClientIP);
	CTraceService::TraceString(strPrompt.GetBuffer(), TraceLevel_Exception);	
#endif

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagConnectItemInfo * pConnectItemInfo=GetBindParameter(wBindIndex);

	//���ñ���
	pConnectItemInfo->dwSocketID=dwSocketID;
	pConnectItemInfo->dwClientIP=dwClientIP;
	return true;
}

//�����¼�
bool CAttemperEngineSink::OnEventTCPNetworkRead(CMD_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (Command.wMainCmdID)
	{
	case MDM_GR_LOGON:	//��¼��Ϣ
		{
			return OnSocketMainLogon(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GR_USER:	//�û���Ϣ
		{
			return OnSocketMainUser(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GR_MANAGER://������Ϣ
		{
			return OnSocketMainManager(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GF_FRAME:	//�����Ϣ
		{
			return OnSocketMainFrame(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GF_GAME:	//��Ϸ��Ϣ
		{
			return OnSocketMainGame(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GF_VIDEO:	//��Ƶ��Ϣ
		{
			return OnSocketMainVideo(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GF_PRESENT://������Ϣ  �����˵��ߣ��ʻ�
		{
			return OnSocketPresent(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GF_BANK:	//������Ϣ
		{
			return OnSocketBank(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	default:
		return true;
	}

	return true;
}

//����ر��¼�
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientIP, DWORD dwActiveTime, DWORD dwSocketID)
{

	__ENTER_FUNCTION_LOG_CATCH

	CString strPrompt;
	strPrompt.Format("OnEventTCPNetworkShut dwClientIP=%d ", dwClientIP);
	//CTraceService::TraceString(strPrompt.GetBuffer(), TraceLevel_Exception);	// �����ã�2012-03-29

	//�û�����
	tagConnectItemInfo * pConnectItemInfo=GetBindParameter(LOWORD(dwSocketID));
	if (pConnectItemInfo==NULL) return true;
	IServerUserItem * pIServerUserItem=pConnectItemInfo->pIServerUserItem;
	if (pIServerUserItem!=NULL)
	{
		//�ж���Ϸ
		bool bOffLine=false;
		if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
		{
			//��������
			CTableFrame * pTableFrame=(m_pTableFrame+pIServerUserItem->GetTableID());
			//����ע��
			WORD wChairID=pIServerUserItem->GetChairID();
			if ((pTableFrame->IsUserPlaying(pIServerUserItem)==true)&&(pTableFrame->OnUserReqOffLine(wChairID)==true)) //����ɾ��
			{
				bOffLine=m_ServerUserManager.RegOffLineUserItem(pIServerUserItem);
			}

			//���ߴ���
			if (bOffLine==true) 
				pTableFrame->OnUserOffLine(wChairID);
			else 
				pTableFrame->PerformStandUpAction(pIServerUserItem);
		}

		//ɾ���û�
		if (bOffLine==false)//����ɾ��
		{
			DeleteUserItem(pIServerUserItem);
		}

		//ɾ���Ŷ�
		char info[BUFSIZ] = {0};
		tagServerUserData * pUserData=pIServerUserItem->GetUserData();
		if (pUserData != NULL)
		{
#if _SHOW_MESSAGE_WARN
			sprintf(info, _T("%s OnEventTCPNetworkShut �û������ˣ��˳�����: ���й���%i��"),  pUserData->szAccounts, m_UserAutoQueue.Size());
			CTraceService::TraceString(info, TraceLevel_Normal);
#endif		
		}
		m_UserAutoQueue.Erase(pIServerUserItem);
		m_AndroidAutoQueue.Erase(pIServerUserItem);
	}

	//��������
	ZeroMemory(pConnectItemInfo,sizeof(tagConnectItemInfo));

	__LEAVE_FUNCTION_LOG_CATCH


	return true;
}

//���������¼�
bool __cdecl CAttemperEngineSink::OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)
{
	__ENTER_FUNCTION_LOG_CATCH

	CTraceService::TraceString(TEXT("OnEventTCPSocketLink "),TraceLevel_Warning);

	//�����ж�
	if (nErrorCode!=0)
	{
		//����ʱ��
		m_pITimerEngine->SetTimer(IDI_CONNECT_CENTER_SERVER,TIME_RECONNECT,1,NULL);

		//������ʾ
		CTraceService::TraceString(TEXT("���ķ���������ʧ�ܣ��Ժ󽫻��ٴγ���...."),TraceLevel_Warning);

		return true;
	}
	else
	{
		//������ʾ
		CTraceService::TraceString(TEXT("���ķ��������ӳɹ�"),TraceLevel_Normal);
	}

	//ע�᷿��
	CMD_CS_RegGameServer RegGameServer;
	memset(&RegGameServer,0,sizeof(RegGameServer));
	RegGameServer.GameServer.wSortID=m_pGameServiceOption->wSortID;
	RegGameServer.GameServer.wKindID=m_pGameServiceOption->wKindID;
	RegGameServer.GameServer.wServerID=m_pGameServiceOption->wServerID;
	RegGameServer.GameServer.wStationID=m_pGameServiceOption->wNodeID;
	RegGameServer.GameServer.wServerPort=m_pGameServiceOption->wServerPort;
	//RegGameServer.GameServer.dwServerAddr=m_pGameServiceOption->dwServerAddr;

	RegGameServer.GameServer.dwServerAddrCNN=m_pGameServiceOption->dwServerAddrCNN;
	RegGameServer.GameServer.dwServerAddrCUN=m_pGameServiceOption->dwServerAddrCUN;


	RegGameServer.GameServer.dwOnLineCount=(m_ServerUserManager.GetOnLineCount()+m_ServerUserManager.GetOffLineCount());
	RegGameServer.GameServer.dwMaxCount   = m_pGameServiceOption->wMaxConnect - LESS_CONNECT_COUNT;	//�������
	//RegGameServer.GameServer.lLessScore   = m_pGameServiceOption->lLessScore;  //�������
	//RegGameServer.GameServer.lMaxScore    = m_pGameServiceOption->lMaxScore;   //�������
	RegGameServer.GameServer.lRestrictScore = m_pGameServiceOption->lRestrictScore; //�������
	RegGameServer.GameServer.lCellScore     = m_pGameServiceOption->lCellScore;  //�������
	RegGameServer.GameServer.lRestrictGrade = m_pGameServiceOption->lRestrictGrade;//�������
	RegGameServer.GameServer.lEnterMoney    = m_pGameServiceOption->lEnterMoney; //�������


	lstrcpyn(RegGameServer.GameServer.szServerName,m_pGameServiceOption->szGameRoomName,CountArray(RegGameServer.GameServer.szServerName));
	m_pITCPSocketCorrespond->SendData(MDM_CS_SERVER_MANAGER,SUB_CS_REG_GAME_SERVER,&RegGameServer,sizeof(RegGameServer));

	//��ȡ�б�
	m_pITCPSocketCorrespond->SendData(MDM_CS_SERVER_LIST,SUB_CS_GET_SERVER_LIST,NULL,0);

	//���ö�ʱ��
	m_pITimerEngine->SetTimer(IDI_UPDATE_SERVER_LIST,TIME_UPDATE_LIST,TIMES_INFINITY,NULL);
	m_pITimerEngine->SetTimer(IDI_UPDATE_ONLINE_COUNT,TIME_UPDATE_ONLINE,TIMES_INFINITY,NULL);

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//���Ķ�ȡ�¼�
bool __cdecl CAttemperEngineSink::OnEventTCPSocketRead(WORD wServiceID, CMD_Command Command, VOID * pData, WORD wDataSize)
{
	switch (Command.wMainCmdID)
	{
	case MDM_CS_SERVER_LIST:	//�б���Ϣ
		{
			return OnCenterMainServerList(Command,pData,wDataSize);
		}
	}
	return true;
}

//���Ĺر��¼�
bool __cdecl CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	__ENTER_FUNCTION_LOG_CATCH

	//��������
	m_pITimerEngine->KillTimer(IDI_UPDATE_SERVER_LIST);
	m_pITimerEngine->KillTimer(IDI_UPDATE_ONLINE_COUNT);
	m_pITimerEngine->SetTimer(IDI_CONNECT_CENTER_SERVER,TIME_RECONNECT,1,NULL);

	//������ʾ
	CTraceService::TraceString(TEXT("���ķ����������жϣ��Ժ󽫻��ٴγ���...."),TraceLevel_Warning);

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//��¼��Ϣ����
bool CAttemperEngineSink::OnSocketMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	__ENTER_FUNCTION_LOG_CATCH

	switch (wSubCmdID)
	{
	case SUB_GR_LOGON_USERID:		//ID ��¼
		{
			//CTraceService::TraceString("�����¼",TraceLevel_Exception);	// �����ã�2012-03-29

			//Ч�����
			ASSERT(wDataSize>=sizeof(CMD_GR_LogonByUserID));
			if (wDataSize<sizeof(CMD_GR_LogonByUserID)) 
				return true;

			//������Ϣ
			CMD_GR_LogonByUserID * pLogonByUserID=(CMD_GR_LogonByUserID *)pData;
			pLogonByUserID->szPassWord[CountArray(pLogonByUserID->szPassWord)-1]=0;   

			//�ظ��ж�
			tagConnectItemInfo * pConnectItemInfo=GetBindParameter(LOWORD(dwSocketID));
			if (pConnectItemInfo==NULL) 
				return true;
			IServerUserItem * pIServerUserItem=pConnectItemInfo->pIServerUserItem;
			if (pIServerUserItem!=NULL)
				return true;

			//�����ж�
			pIServerUserItem=m_ServerUserManager.SearchOnLineUser(pLogonByUserID->dwUserID);
			if (pIServerUserItem!=NULL)
			{
				//Ч������
				if (lstrcmp(pIServerUserItem->GetPassword(),pLogonByUserID->szPassWord)!=0)
				{
					SendLogonFailed(dwSocketID,TEXT("�ʻ������������֤���ٴγ��Ե�¼��"),0);

					if (LOWORD(dwSocketID)>=INDEX_ANDROID)
					{
						m_AndroidUserManager.DeleteAndroidUserItem(dwSocketID);
					}
					else
					{
						m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);
					}

					return true;
				}

				//��������//�޸�
				WORD wSourceIndex=pIServerUserItem->GetUserIndex();
				tagConnectItemInfo * pConnectItemSource=GetBindParameter(wSourceIndex);
				tagConnectItemInfo * pConnectItemTarger=GetBindParameter(LOWORD(dwSocketID));

				//�Ͽ��û�
				if (pConnectItemSource!=NULL)
				{
					//���ñ���
					pConnectItemSource->pIServerUserItem=NULL;

					//������Ϣ
					LPCTSTR pszMessage=TEXT("��ע�⣬�����ʺ�����һ�ط��������Ϸ�����ˣ��������뿪��");
					SendRoomMessage(pConnectItemSource->dwSocketID,pszMessage,SMT_INFO|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_ROOM);

					//������Ϣ
					if (wSourceIndex>=INDEX_ANDROID)
					{
						m_AndroidUserManager.DeleteAndroidUserItem(pConnectItemSource->dwSocketID);
					}
					else
					{
						m_pITCPNetworkEngine->ShutDownSocket(pConnectItemSource->dwSocketID);
					}
				}

				//�����û�
				pConnectItemTarger->pIServerUserItem=pIServerUserItem;
				m_ServerUserManager.SwitchOnLineUserItem(pIServerUserItem,pConnectItemTarger->dwClientIP,LOWORD(dwSocketID));

				//���ͷ�����Ϣ
				SendGameServerInfo(pIServerUserItem,dwSocketID);

				//����Ⱥ��
				if (LOWORD(dwSocketID)<INDEX_ANDROID)
				{
					m_pITCPNetworkEngine->AllowBatchSend(dwSocketID,true);
				}

				SendFindGemsTask(pIServerUserItem);
				return true;
			}

			//�����ж�
			pIServerUserItem=m_ServerUserManager.SearchOffLineUser(pLogonByUserID->dwUserID);
			if (pIServerUserItem!=NULL)
			{
				//Ч������
				if (lstrcmp(pIServerUserItem->GetPassword(),pLogonByUserID->szPassWord)!=0)
				{
					SendLogonFailed(dwSocketID,TEXT("�ʻ������������֤���ٴγ��Ե�¼��"),0);

					if (LOWORD(dwSocketID)>=INDEX_ANDROID)
					{
						m_AndroidUserManager.DeleteAndroidUserItem(dwSocketID);
					}
					else
					{
						m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);
					}

					return true;
				}

				//�����û�
				pConnectItemInfo->pIServerUserItem=pIServerUserItem;
				m_ServerUserManager.ActiveOffLineUserItem(pIServerUserItem,pConnectItemInfo->dwClientIP,LOWORD(dwSocketID));

				//��������
				WORD wTableID=pIServerUserItem->GetTableID();
				WORD wChairID=pIServerUserItem->GetChairID();
				(m_pTableFrame+wTableID)->OnUserReConnect(wChairID);

				//���ͷ�����Ϣ
				SendGameServerInfo(pIServerUserItem,dwSocketID);

				//����Ⱥ��
				if (LOWORD(dwSocketID)<INDEX_ANDROID)
				{
					m_pITCPNetworkEngine->AllowBatchSend(dwSocketID,true);
				}

				SendFindGemsTask(pIServerUserItem);
				return true;
			}

			//��������
			DBR_GR_LogonByUserID LogonByUserID;
			memset(&LogonByUserID,0,sizeof(LogonByUserID));
			LogonByUserID.dwUserID=pLogonByUserID->dwUserID;
			LogonByUserID.dwClientIP=pConnectItemInfo->dwClientIP;
			lstrcpyn(LogonByUserID.szPassWord,pLogonByUserID->szPassWord,sizeof(LogonByUserID.szPassWord));
			LogonByUserID.dwKindID =  pLogonByUserID->dwKindID; //���ġ���Ҫ����


			//�������� �������
			DWORD dwUserCount=m_ServerUserManager.GetOnLineCount()+m_ServerUserManager.GetOffLineCount();
			if ((dwUserCount+LESS_CONNECT_COUNT)>=m_pGameServiceOption->wMaxConnect)
			{
				//������Ϣ
				LPCTSTR pszMessage=TEXT("��Ǹ������Ϸ�����Ѿ����ˣ�");
				SendLogonFull(dwSocketID, pszMessage, dwUserCount);
				return true;
			}
			//��ӽ���

			//��������
			VOID * pData=NULL;
			tagDataDescribe DataDescribe;
			CRecvPacketHelper RecvPacket(pLogonByUserID+1,wDataSize-sizeof(CMD_GR_LogonByUserID));

			//��չ��Ϣ
			while (true)
			{
				//��ȡ����
				pData=RecvPacket.GetData(DataDescribe);
				if (DataDescribe.wDataDescribe==DTP_NULL) break;

				//���ݷ���
				switch (DataDescribe.wDataDescribe)
				{
				case DTP_COMPUTER_ID:		//������ʶ
					{
						ASSERT(pData!=NULL);
						ASSERT(DataDescribe.wDataSize==sizeof(tagClientSerial));
						if (DataDescribe.wDataSize==sizeof(tagClientSerial))
						{
							BYTE * pcbByte=(BYTE *)pData;
							for (INT i=0;i<sizeof(tagClientSerial)/sizeof(BYTE);i++)
							{
								ASSERT(CountArray(LogonByUserID.szComputerID)>((i*2)+1));
								_stprintf(&LogonByUserID.szComputerID[i*2],TEXT("%02X"),*(pcbByte+i));
							}
						}

						break;
					}
				}
			}

			//Ͷ�����ݿ�
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_LOGON_BY_USERID,dwSocketID,&LogonByUserID,sizeof(LogonByUserID));
			
			//CTraceService::TraceString("�����뿪��¼",TraceLevel_Exception);	// �����ã�2012-03-29

			return true;
		}
	}

	__LEAVE_FUNCTION_LOG_CATCH
	
	//CTraceService::TraceString("�쳣�뿪��¼",TraceLevel_Exception);	// �����ã�2012-03-29

	return true;
}

//�û���Ϣ����
bool CAttemperEngineSink::OnSocketMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	__ENTER_FUNCTION_LOG_CATCH

	switch (wSubCmdID)
	{
	case SUB_GR_USER_SIT_REQ:		//��������   
	case SUB_GR_USER_LOOKON_REQ:	//�Թ�����
		{
			//��������
			CMD_GR_UserSitReq * pUserSitReq=(CMD_GR_UserSitReq *)pData;
			WORD wHeadSize=sizeof(CMD_GR_UserSitReq)-sizeof(pUserSitReq->szTablePass);

			//Ч������
			ASSERT((wDataSize>=wHeadSize)&&(wDataSize<=sizeof(CMD_GR_UserSitReq)));
			if ((wDataSize<wHeadSize)||(wDataSize>sizeof(CMD_GR_UserSitReq))) 
				return true;
			ASSERT((wHeadSize+pUserSitReq->cbPassLen)==wDataSize);
			if ((wHeadSize+pUserSitReq->cbPassLen)!=wDataSize)
				return true;

			//�ж�λ��
			if ((pUserSitReq->wChairID>=m_pGameServiceAttrib->wChairCountMax)&&(pUserSitReq->wChairID!=INVALID_CHAIR)) 
				return true;
			if ((pUserSitReq->wTableID>=m_pGameServiceOption->wTableCount)&&(pUserSitReq->wTableID!=INVALID_TABLE)) 
				return true;

			//��ȡ�û�
			IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItem==NULL) 
				return true;
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();

			//�������  �������������ӣ���Ϊ�����Ŷ�ϵͳ
			if ((wSubCmdID==SUB_GR_USER_SIT_REQ))
			{
				//��������
				CMD_GR_SitFailed SitFailed;
				memset(&SitFailed,0,sizeof(SitFailed));
				_snprintf(SitFailed.szFailedDescribe, sizeof(SitFailed.szFailedDescribe), TEXT("�������Ŷӽ�����Ϸ��"));

				//��������
				WORD wSendSize=sizeof(SitFailed)-sizeof(SitFailed.szFailedDescribe)+CountStringBuffer(SitFailed.szFailedDescribe);
				SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_SIT_FAILED,&SitFailed,wSendSize);
				//������� ��֤������û������û�
				return true;
			}

			//�Թ�����
			if ((wSubCmdID==SUB_GR_USER_LOOKON_REQ)&&(m_pGameServiceOption->cbUnLookOnTag==TRUE))
			{
				//��������
				CMD_GR_SitFailed SitFailed;
				memset(&SitFailed,0,sizeof(SitFailed));
				_snprintf(SitFailed.szFailedDescribe,sizeof(SitFailed.szFailedDescribe),TEXT("�������������Թ���Ϸ!"),pUserData->wTableID+1);

				//��������
				WORD wSendSize=sizeof(SitFailed)-sizeof(SitFailed.szFailedDescribe)+CountStringBuffer(SitFailed.szFailedDescribe);
				SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_SIT_FAILED,&SitFailed,wSendSize);
				return true;
			}

			//�������
			if ((pUserData->wTableID==pUserSitReq->wTableID)&&(pUserData->wChairID==pUserSitReq->wChairID))
			{
				if ((pUserData->cbUserStatus!=US_LOOKON)&&(wSubCmdID==SUB_GR_USER_SIT_REQ)) return true;
				if ((pUserData->cbUserStatus==US_LOOKON)&&(wSubCmdID==SUB_GR_USER_LOOKON_REQ)) return true;
			}

			//�뿪λ��  
			if (pUserData->wTableID!=INVALID_TABLE)
			{   //���������
				CTableFrame * pTableFrame=(m_pTableFrame+pUserData->wTableID);
				//��������
				CMD_GR_SitFailed SitFailed;
				memset(&SitFailed,0,sizeof(SitFailed));
				_snprintf(SitFailed.szFailedDescribe,sizeof(SitFailed.szFailedDescribe),TEXT("�����ڵ� [ %d ] ����Ϸ����Ϸ����ʱ�����뿪λ�ã�"),pUserData->wTableID+1);
				//��������
				WORD wSendSize=sizeof(SitFailed)-sizeof(SitFailed.szFailedDescribe)+CountStringBuffer(SitFailed.szFailedDescribe);
				SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_SIT_FAILED,&SitFailed,wSendSize);
				return true;
			}


			//������ӣ��û��Ѿ��Ŷ���
			if (m_UserAutoQueue.IsExist(pIServerUserItem))
			{
				//��������
				CMD_GR_SitFailed SitFailed;
				memset(&SitFailed,0,sizeof(SitFailed));
				_snprintf(SitFailed.szFailedDescribe,sizeof(SitFailed.szFailedDescribe),TEXT("���Ѿ��Ŷ��ˣ���ʱ�����Թۣ�"));

				//��������
				WORD wSendSize=sizeof(SitFailed)-sizeof(SitFailed.szFailedDescribe)+CountStringBuffer(SitFailed.szFailedDescribe);
				SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_SIT_FAILED,&SitFailed,wSendSize);
				return true;
			}


			CTableFrame * pTableFrame=(m_pTableFrame+pUserSitReq->wTableID);
			pTableFrame->PerformLookonAction(pUserSitReq->wChairID,pIServerUserItem,pUserSitReq->szTablePass);
			return true;
		}
	case SUB_GR_USER_STANDUP_REQ:	//��������
	case SUB_GR_USER_LEFT_GAME_REQ:	//ǿ������
		{
			//��ȡ�û�
			IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItem==NULL) return true;

			//�������
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if ((pUserData->wTableID==INVALID_TABLE)||(pUserData->wChairID==INVALID_CHAIR)) 
				return true;

			//����λ��
			CTableFrame * pTableFrame=(m_pTableFrame+pUserData->wTableID);
			if ((wSubCmdID==SUB_GR_USER_STANDUP_REQ)&&(pTableFrame->IsUserPlaying(pIServerUserItem)==true))
			{
				TCHAR szMessage[256]=TEXT("");
				_snprintf(szMessage,sizeof(szMessage),TEXT("�����ڵ� [ %d ] ����Ϸ����Ϸ����ʱ�����뿪λ�ã�"),pUserData->wTableID+1);
				SendRoomMessage(dwSocketID,szMessage,SMT_EJECT);
				return true;
			}
#if _SHOW_MESSAGE_NOR
			char info[BUFSIZ] = {0};
			sprintf(info, _T("%s �û��뿪�����ˣ��˳�����: ���й���%i��"), pUserData->szAccounts, m_UserAutoQueue.Size());
			CTraceService::TraceString(info, TraceLevel_Normal);
#endif			
			m_UserAutoQueue.Erase(pIServerUserItem);
			m_AndroidAutoQueue.Erase(pIServerUserItem);		

			pTableFrame->PerformStandUpAction(pIServerUserItem);
			//�����˳����Ƶ���ʱ 601=���� Author<cxf>;DateTime<2012-03-31>
			if (601!=m_pGameServiceAttrib->wKindID)
			{
				m_userLeftDataSet.insert(UserLeftDataSet::value_type(pUserData->dwUserID,  time(NULL)));
			}
			//�����е��÷����Ͷ�������
			SendAllQueueCount();
			return true;
		}


	case SUB_GR_USER_CHAT:			//������Ϣ
		{
			//Ч�����
			CMD_GR_UserChat * pUserChat=(CMD_GR_UserChat *)pData;
			ASSERT(wDataSize>=(sizeof(CMD_GR_UserChat)-sizeof(pUserChat->szChatMessage)));
			ASSERT(wDataSize==(sizeof(CMD_GR_UserChat)-sizeof(pUserChat->szChatMessage)+pUserChat->wChatLength));
			if (wDataSize<(sizeof(CMD_GR_UserChat)-sizeof(pUserChat->szChatMessage))) 
				return true;
			if (wDataSize!=(sizeof(CMD_GR_UserChat)-sizeof(pUserChat->szChatMessage)+pUserChat->wChatLength)) 
				return true;

			//��ȡ�û�
			IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItem==NULL)
				return true;

			//Ѱ���û�
			IServerUserItem * pIServerUserItemRecv=NULL;
			if (pUserChat->dwTargetUserID!=0) 
			{
				pIServerUserItemRecv=m_ServerUserManager.SearchOnLineUser(pUserChat->dwTargetUserID);
				if (pIServerUserItemRecv==NULL) 
					return true;
			}

			//״̬�ж�
			if (m_bAllowRoomChat==false)
			{
				LPCTSTR pszMessage=TEXT("��Ǹ������Ϸ���䲻������д������ģ�");
				SendRoomMessage(pIServerUserItem,pszMessage,SMT_EJECT|SMT_INFO);
				return true;
			}

			//Ȩ���ж�
			if (CUserRight::CanRoomChat(pIServerUserItem->GetUserData()->dwUserRight)==false)
			{
				LPCTSTR pszMessage=TEXT("��Ǹ����û�д������ĵ�Ȩ�ޣ�����Ҫ����������ϵ��Ϸ�ͷ���ѯ��");
				SendRoomMessage(pIServerUserItem,pszMessage,SMT_EJECT|SMT_INFO);
				return true;
			}

			//ת����Ϣ
			m_pITCPNetworkEngine->SendDataBatch(MDM_GR_USER,SUB_GR_USER_CHAT,pUserChat,wDataSize);
			m_AndroidUserManager.SendDataToClient(MDM_GR_USER,SUB_GR_USER_CHAT,pUserChat,wDataSize);

			return true;
		}
	case SUB_GR_USER_WISPER:		//˽����Ϣ
		{
			//Ч�����
			CMD_GR_Wisper * pWisper=(CMD_GR_Wisper *)pData;
			ASSERT(wDataSize>=(sizeof(CMD_GR_Wisper)-sizeof(pWisper->szChatMessage)));
			ASSERT(wDataSize==(sizeof(CMD_GR_Wisper)-sizeof(pWisper->szChatMessage)+pWisper->wChatLength));
			if (wDataSize<(sizeof(CMD_GR_Wisper)-sizeof(pWisper->szChatMessage))) 
				return true;
			if (wDataSize!=(sizeof(CMD_GR_Wisper)-sizeof(pWisper->szChatMessage)+pWisper->wChatLength))
				return true;

			//��ȡ�û�
			IServerUserItem * pIServerUserItemSend=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItemSend==NULL) 
				return true;
			IServerUserItem * pIServerUserItemRecv=m_ServerUserManager.SearchOnLineUser(pWisper->dwTargetUserID);
			if (pIServerUserItemRecv==NULL) 
				return true;

			//״̬�ж�
			DWORD dwMasterRightSend=pIServerUserItemSend->GetUserData()->dwMasterRight;
			DWORD dwMasterRightRecv=pIServerUserItemRecv->GetUserData()->dwMasterRight;
			if ((m_bAllowWisper==false)&&(dwMasterRightSend==0L)&&(dwMasterRightRecv==0L))
			{
				LPCTSTR pszMessage=TEXT("��Ǹ������Ϸ���䲻�ܽ������˽�ģ�");
				SendRoomMessage(pIServerUserItemSend,pszMessage,SMT_EJECT|SMT_INFO);
				return true;
			}

			//Ȩ���ж�
			DWORD dwUserRightSend=pIServerUserItemSend->GetUserData()->dwUserRight;
			if ((dwMasterRightRecv==0L)&&(CUserRight::CanWisper(dwUserRightSend)==false))
			{
				TCHAR szMessage[256]=TEXT("����ʱû������ҷ���˽����Ϣ��Ȩ�ޣ�ֻ�������Ա˽�ģ�");
				SendRoomMessage(dwSocketID,szMessage,SMT_EJECT);
				return true;
			}

			//ת����Ϣ
			SendData(pIServerUserItemSend,MDM_GR_USER,SUB_GR_USER_WISPER,pWisper,wDataSize);
			SendData(pIServerUserItemRecv,MDM_GR_USER,SUB_GR_USER_WISPER,pWisper,wDataSize);

			return true;
		}
	case SUB_GR_USER_RULE:		//�û�����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_GR_UserRule));
			if (wDataSize!=sizeof(CMD_GR_UserRule)) return true;

			//�������ж�
			if (m_pGameServiceOption->cbHideUserInfo==TRUE) return true;

			//��Ϣ����
			CMD_GR_UserRule * pUserRule=(CMD_GR_UserRule *)pData;
			pUserRule->szPassword[CountArray(pUserRule->szPassword)-1]=0;

			//��ȡ�û�
			IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItem==NULL) return true;

			//���˰���
			if (m_pGameServiceAttrib->wChairCountMax<=MAX_CHAIR_NORMAL)
			{
				//�������
				tagUserRule UserRule;
				ZeroMemory(&UserRule,sizeof(UserRule));
				UserRule.bLimitWin=pUserRule->bLimitWin;
				UserRule.bLimitFlee=pUserRule->bLimitFlee;
				UserRule.bLimitScore=pUserRule->bLimitScore;
				UserRule.bCheckSameIP=pUserRule->bCheckSameIP;
				UserRule.wWinRate=pUserRule->wWinRate;
				UserRule.wFleeRate=pUserRule->wFleeRate;
				UserRule.lMaxScore=pUserRule->lMaxScore;
				UserRule.lLessScore=pUserRule->lLessScore;
				if(m_pGameServiceAttrib->wChairCountMax<=MAX_CHAIR_NORMAL)
				{
					UserRule.bPassword=pUserRule->bPassword;
					lstrcpyn(UserRule.szPassword,pUserRule->szPassword,CountArray(UserRule.szPassword));
				}

				//���ù���
				pIServerUserItem->SetUserRule(UserRule);
			}

			return true;
		}

	case SUB_GR_USER_INVITE_REQ:		//�����û�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_GR_UserInviteReq));
			if (wDataSize!=sizeof(CMD_GR_UserInviteReq)) return true;

			//��Ϣ����
			CMD_GR_UserInviteReq * pUserInviteReq=(CMD_GR_UserInviteReq *)pData;

			//��ȡ�û�
			IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItem==NULL) return true;

			//Ч��״̬
			if (pUserInviteReq->wTableID==INVALID_TABLE) return true;
			if (pIServerUserItem->GetTableID()!=pUserInviteReq->wTableID) return true;
			if (pIServerUserItem->GetUserStatus()==US_PLAY) return true;
			if (pIServerUserItem->GetUserStatus()==US_OFFLINE) return true;

			//Ŀ���û�
			IServerUserItem * pITargetUserItem=m_ServerUserManager.SearchOnLineUser(pUserInviteReq->dwUserID);
			if (pITargetUserItem==NULL) return true;
			if (pITargetUserItem->GetUserStatus()==US_PLAY) return true;

			//������Ϣ
			CMD_GR_UserInvite UserInvite;
			memset(&UserInvite,0,sizeof(UserInvite));
			UserInvite.wTableID=pUserInviteReq->wTableID;
			UserInvite.dwUserID=pIServerUserItem->GetUserID();
			SendData(pITargetUserItem,MDM_GR_USER,SUB_GR_USER_INVITE,&UserInvite,sizeof(UserInvite));

			return true;
		}

		//�û�ȡ���Ŷ�
	case SUB_GR_AUTO_QUEUE_CANCEL:
		{
			//��ȡ�û�
			IServerUserItem * pIItem=GetServerUserItem(LOWORD(dwSocketID));
			if (pIItem==NULL) return true;

			if (!m_UserAutoQueue.IsExist(pIItem))
			{
				SendData(pIItem,MDM_GR_USER, SUB_GR_AUTO_QUEUE_CANCEL_FALL);
				return true;
			}
			m_UserAutoQueue.Erase(pIItem);
			m_AndroidAutoQueue.Erase(pIItem);
			SendData(pIItem, MDM_GR_USER, SUB_GR_AUTO_QUEUE_CANCEL_SUCC);
			SendAllQueueCount();
			return true;
		}
	case SUB_GR_AUTO_QUEUE:	//�Զ��Ŷ�
		{
			return ArrangeQueue(dwSocketID);;
		}

	case SUB_SENDACTIONINFO:  //ת��������Ϣ
		{
			IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItem==NULL) return true;
			return SendUserActionInfo(pIServerUserItem, pData, wDataSize);
		}

	case SUB_GF_QUIT_GAME_SERVER: //�û������˳���Ϸ������
		{
			IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItem==NULL) return true;

			if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
			{
				//��������
				CTableFrame * pTableFrame=(m_pTableFrame+pIServerUserItem->GetTableID());
				pTableFrame->PerformStandUpAction(pIServerUserItem);
			}

			//SendData(pIServerUserItem, MDM_GR_USER, SUB_GF_QUIT_GAME_SERVER_SUCC);
			UnLockUserItem(pIServerUserItem);
			m_pITCPNetworkEngine->SendData(dwSocketID,  MDM_GR_USER, SUB_GF_QUIT_GAME_SERVER_SUCC);

#if _SHOW_MESSAGE_NOR
			char info[BUFSIZ] = {0};
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData != NULL)
			{
				sprintf(info, _T("%s SUB_GF_QUIT_GAME_SERVER �û������˳���Ϸ���������˳�����: ���й���%i��"),  pUserData->szAccounts, m_UserAutoQueue.Size());
				CTraceService::TraceString(info, TraceLevel_Normal);
			}
#endif
			m_UserAutoQueue.Erase(pIServerUserItem);
			m_AndroidAutoQueue.Erase(pIServerUserItem);
			return true;
		}
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

bool CAttemperEngineSink::ArrangeQueue(DWORD dwSocketID)
{
	__ENTER_FUNCTION_LOG_CATCH

	//��ȡ�û�
	IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
	if (pIServerUserItem==NULL) return true;

	if (!CheckUserPlaying(pIServerUserItem))
	{
		return true;
	}

	if (!JudgeUserFitThisRoom(pIServerUserItem))
	{
		return true;
	}

	//������Ŷ��û�
	if (!InsertUserInQueue(pIServerUserItem))
	{
		goto Finish;
	}


	//����λ��
	//if (m_pGameServiceAttrib->wChairCountMin <=1)  //���ֻ��һ���˵Ļ����˳�
	//{
	//	 CTraceService::TraceString(_T("һ���˿�������Ϸ��"),TraceLevel_Normal);
	//	return true;
	//}

	//unsigned int busyDesk = m_BusyDeskStat.GetAverage();
	//if (busyDesk < m_pGameServiceOption->wAndroidTableCount)
	//{
	//	
	//}


	//bool fixChair = (m_pGameServiceAttrib->wChairCountMin == m_pGameServiceAttrib->wChairCountMax ? true : false) ;
	/*if (fixChair)
	{*/
	if(!PerformSitDownChair())
	{
		return true;
	}
	/*}else
	{
	if(!PerformSitDownUnFixChair())
	{
	return true;
	}
	}*/

Finish:
	//��������
	CMD_GR_AutoQueue AutoQueue;
	ZeroMemory(&AutoQueue,sizeof(AutoQueue));
	AutoQueue.iAutoQueueNum = (int)m_UserAutoQueue.Size();

	//��������
	WORD wSendSize=sizeof(AutoQueue);
	SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_AUTO_QUEUE_SUCC,&AutoQueue,wSendSize);
	//�����е��÷����Ͷ�������
	SendAllQueueCount();

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//�������
//�ж��������Ƿ���Ϸ��������
bool CAttemperEngineSink::JudgeUserFitThisRoom(IServerUserItem * pIServerUserItem)
{
	__ENTER_FUNCTION_LOG_CATCH


	ASSERT(pIServerUserItem != NULL);
	tagServerUserData * pUserData=pIServerUserItem->GetUserData();
	//�����ж�
	if (m_pGameServiceOption->wServerType==GAME_GENRE_MATCH)
	{
		//����Ȩ��
		if (CUserRight::IsMatchUser(pUserData->dwUserRight)==false)
		{
			TCHAR szDescribe[128]=TEXT("");
			lstrcpyn(szDescribe,TEXT("������Ϸ�������䣬�����Ǳ���ѡ�֣�����������λ���ϣ� "),CountArray(szDescribe));
			SendWarningPacket(pIServerUserItem, MDM_GR_USER, SUB_GR_AUTO_QUEUE_FAIL, szDescribe);
			return false;
		}


		//�Ծ־���
		if (m_pGameServiceOption->lMatchDraw>0)
		{
			LONG lPlayCount=pUserData->UserScoreInfo.lWinCount+pUserData->UserScoreInfo.lLostCount+pUserData->UserScoreInfo.lDrawCount;
			if (lPlayCount>=m_pGameServiceOption->lMatchDraw)
			{
				TCHAR szDescribe[128]=TEXT("");
				lstrcpyn(szDescribe,TEXT("��ϲ�������ı��������Ѿ�����ˣ�����Ҫ�ټ��������������ĵȴ����������� "),CountArray(szDescribe));
				SendWarningPacket(pIServerUserItem, MDM_GR_USER, SUB_GR_AUTO_QUEUE_FAIL, szDescribe);
				return false;
			}
		}
	}
	else
	{
		//Ȩ���ж�
		if (CUserRight::CanPlay(pUserData->dwUserRight)==false)
		{
			//������Ϣ 
			TCHAR szDescribe[128] = TEXT("��Ǹ����û�н�����Ϸ��Ȩ�ޣ�����Ҫ����������ϵ��Ϸ�ͷ���ѯ��");
			SendWarningPacket(pIServerUserItem, MDM_GR_USER, SUB_GR_AUTO_QUEUE_FAIL, szDescribe);
			return false;
		}

		if (pUserData->UserScoreInfo.lGold < m_pGameServiceOption->lRestrictScore)
		{
			//������Ϣ
			TCHAR szDescribe[128]=TEXT("");
			if (m_pGameServiceOption->wServerType==GAME_GENRE_GOLD)
			{
				_snprintf(szDescribe,sizeof(szDescribe),TEXT("������Ϸ������Ҫ %i ����ң����Ľ�Ҳ��������ܼ��룡"),m_pGameServiceOption->lEnterMoney);
			}

			SendWarningPacket(pIServerUserItem, MDM_GR_USER, SUB_GR_NO_GOLD, szDescribe);
			return false;
		}
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//�������    
//���;�����Ϣ
void CAttemperEngineSink::SendWarningPacket(IServerUserItem * pIServerUserItem, int MDM_MES, int SUB_MES, TCHAR* pMes)
{
	CMD_GR_SendWarning SitFailed;
	memset(&SitFailed,0,sizeof(SitFailed));
	strncpy(SitFailed.szWarningMessage,  pMes, sizeof(SitFailed.szWarningMessage) - 1);
#if _SHOW_MESSAGE_NOR
	CTraceService::TraceString(SitFailed.szWarningMessage, TraceLevel_Normal);
#endif
	//��������
	WORD wSendSize=sizeof(SitFailed)-sizeof(SitFailed.szWarningMessage)+CountStringBuffer(SitFailed.szWarningMessage);
	SendData(pIServerUserItem,MDM_MES,SUB_MES,&SitFailed,wSendSize);
}


//�������
//ִ���Ŷ������Ӳ���
bool CAttemperEngineSink::PerformSitDownChair()
{
	__ENTER_FUNCTION_LOG_CATCH

	for (int i=1; i<=m_pGameServiceAttrib->wChairCountMax; ++i)
	{
		PerformSitDownOnDesk(i);
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//�������
//�����е��û������ŶӶ��е�����
void CAttemperEngineSink::SendAllQueueCount()
{
	__ENTER_FUNCTION_LOG_CATCH

	WORD wEnumIndex=0;
	while (true)
	{
		IServerUserItem*  pIServerUserItemSend=m_ServerUserManager.EnumOnLineUser(wEnumIndex++);
		if (pIServerUserItemSend==NULL) break;
		SendQueueCount(pIServerUserItemSend);
	}

	wEnumIndex=0;
	while (true)
	{
		IServerUserItem*  pIServerUserItemSend=m_ServerUserManager.EnumOffLineUser(wEnumIndex++);
		if (pIServerUserItemSend==NULL) break;
		SendQueueCount(pIServerUserItemSend);
	}

	__LEAVE_FUNCTION_LOG_CATCH
}

//���Ͷ�����������
void  CAttemperEngineSink::SendQueueCount(IServerUserItem * pIServerUserItem)
{
	__ENTER_FUNCTION_LOG_CATCH

	//��������
	CMD_GR_AutoQueue AutoQueue;
	ZeroMemory(&AutoQueue,sizeof(AutoQueue));
	AutoQueue.iAutoQueueNum = (int)m_UserAutoQueue.Size();

	//��������
	WORD wSendSize=sizeof(AutoQueue);
	SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_QUEUE_COUNT,&AutoQueue,wSendSize);

	__LEAVE_FUNCTION_LOG_CATCH
}



//������Ϣ����
bool CAttemperEngineSink::OnSocketMainManager(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	__ENTER_FUNCTION_LOG_CATCH

	//��Ϣ����
	switch (wSubCmdID)
	{
	case SUB_GR_SEND_WARNING:	//������Ϣ
		{
			//Ч�����
			CMD_GR_SendWarning * pSendWarning=(CMD_GR_SendWarning *)pData;
			ASSERT(wDataSize>=(sizeof(CMD_GR_SendWarning)-sizeof(pSendWarning->szWarningMessage)));
			ASSERT(wDataSize==(sizeof(CMD_GR_SendWarning)-sizeof(pSendWarning->szWarningMessage)+pSendWarning->wChatLength));
			if (wDataSize<(sizeof(CMD_GR_SendWarning)-sizeof(pSendWarning->szWarningMessage)))
				return true;
			if (wDataSize!=(sizeof(CMD_GR_SendWarning)-sizeof(pSendWarning->szWarningMessage)+pSendWarning->wChatLength))
				return true;

			//�����û�
			IServerUserItem * pIServerUserItemSend=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItemSend==NULL) return true;

			//Ȩ���ж�
			DWORD dwMasterRight=pIServerUserItemSend->GetUserData()->dwMasterRight;
			if (CUserRight::CanSendWarning(dwMasterRight)==false) return true;

			//Ŀ���û�
			IServerUserItem * pIServerUserItemRecv=m_ServerUserManager.SearchOnLineUser(pSendWarning->dwTargetUserID);
			if (pIServerUserItemRecv==NULL) return true;

			//������Ϣ
			SendGameMessage(pIServerUserItemRecv,pSendWarning->szWarningMessage,SMT_INFO|SMT_EJECT);
			//SendRoomMessage(pIServerUserItemRecv,pSendWarning->szWarningMessage,SMT_INFO|SMT_EJECT);

			return true;
		}
	case SUB_GR_LOOK_USER_IP:	//�鿴��ַ
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_GR_LookUserIP));
			if (wDataSize!=sizeof(CMD_GR_LookUserIP)) return true;

			//��Ϣ����
			CMD_GR_LookUserIP * pLookUserIP=(CMD_GR_LookUserIP *)pData;

			//�����û�
			IServerUserItem * pIServerUserItemSend=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItemSend==NULL) return true;

			//Ȩ���ж�
			DWORD dwMasterRight=pIServerUserItemSend->GetUserData()->dwMasterRight;
			if (CUserRight::CanSeeUserIP(dwMasterRight)==false) return true;

			//Ŀ���û�
			IServerUserItem * pIServerUserItemRecv=m_ServerUserManager.SearchOnLineUser(pLookUserIP->dwTargetUserID);
			if (pIServerUserItemRecv==NULL) return true;

			//��������
			DWORD dwUserIP=pIServerUserItemRecv->GetClientIP();

			//������Ϣ
			TCHAR szMessage[128]=TEXT("");
			BYTE * pcbUserIP=(BYTE *)(&dwUserIP);
			_snprintf(szMessage,CountArray(szMessage),TEXT("[ %s ]��%d.%d.%d.%d"),pIServerUserItemRecv->GetAccounts(),
				pcbUserIP[0],pcbUserIP[1],pcbUserIP[2],pcbUserIP[3]);

			//������Ϣ
			SendRoomMessage(pIServerUserItemSend,szMessage,SMT_INFO|SMT_EJECT);

			return true;
		}
	case SUB_GR_KILL_USER:		//�߳��û�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_GR_KillUser));
			if (wDataSize!=sizeof(CMD_GR_KillUser)) return true;

			//��Ϣ����
			CMD_GR_KillUser * pKillUser=(CMD_GR_KillUser *)pData;

			//�����û�
			IServerUserItem * pIServerUserItemSend=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItemSend==NULL) return true;

			//Ȩ���ж�
			DWORD dwMasterRight=pIServerUserItemSend->GetUserData()->dwMasterRight;
			if (CUserRight::CanCutUser(dwMasterRight)==false) return true;

			//Ŀ���û�
			IServerUserItem * pIServerUserItemRecv=m_ServerUserManager.SearchOnLineUser(pKillUser->dwTargetUserID);
			if (pIServerUserItemRecv==NULL) return true;

			if (pIServerUserItemRecv->IsAndroidUser()==FALSE)
			{
				//������Ϣ
				if (TRUE == pKillUser->bCutOnline)
				{
					LPCTSTR szMessage=TEXT("�װ�����ң�������Υ�������������ʱ��ϵ��Ϸ�ͷ���");
					SendRoomMessage(pIServerUserItemRecv,szMessage,SMT_INFO|SMT_EJECT|SMT_INTERMIT_LINE|SMT_CUSTOMER_FRAME);
				}
				else
				{
					LPCTSTR szMessage=TEXT("�װ�����ң�������Υ�������������ʱ��ϵ��Ϸ�ͷ���");
					SendRoomMessage(pIServerUserItemRecv,szMessage,SMT_INFO|SMT_EJECT|SMT_CLOSE_ROOM|SMT_CUSTOMER_FRAME);
				}
			}
			else
			{
				WORD wBindIndex=pIServerUserItemRecv->GetUserIndex();
				tagConnectItemInfo *pConnectItemInfo=m_pAndroidParameter+(wBindIndex-INDEX_ANDROID);
				if (pConnectItemInfo!=NULL&&pConnectItemInfo->pIServerUserItem!=NULL) m_AndroidUserManager.DeleteAndroidUserItem(pConnectItemInfo->dwSocketID);
			}

			return true;
		}
	case SUB_GR_LIMIT_ACCOUNS:	//�����ʻ�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_GR_LimitAccounts));
			if (wDataSize!=sizeof(CMD_GR_LimitAccounts)) return true;

			//��Ϣ����
			CMD_GR_LimitAccounts * pLimitAccounts=(CMD_GR_LimitAccounts *)pData;

			//�����û�
			IServerUserItem * pIServerUserItemSend=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItemSend==NULL) return true;

			//Ȩ���ж�
			DWORD dwMasterRight=pIServerUserItemSend->GetUserData()->dwMasterRight;
			if (CUserRight::CanForbidAccounts(dwMasterRight)==false) return true;

			//Ŀ���û�
			IServerUserItem * pIServerUserItemRecv=m_ServerUserManager.SearchOnLineUser(pLimitAccounts->dwTargetUserID);
			if (pIServerUserItemRecv==NULL) return true;

			//Ͷ�����ݿ�
			DBR_GR_LimitAccounts LimitAccounts;
			LimitAccounts.dwUserID=pLimitAccounts->dwTargetUserID;
			LimitAccounts.dwMasterUserID=pIServerUserItemSend->GetUserID();
			LimitAccounts.dwMasterClientIP=pIServerUserItemSend->GetClientIP();
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_LIMIT_ACCOUNTS,0,&LimitAccounts,sizeof(LimitAccounts));

			//������Ϣ
			LPCTSTR szMessage=TEXT("��Ǹ�������ʺű������ˣ������κ����ʣ�����ϵ��Ϸ�ͷ���");
			SendRoomMessage(pIServerUserItemRecv,szMessage,SMT_INFO|SMT_EJECT|SMT_CLOSE_ROOM|SMT_CUSTOMER_FRAME);

			return true;
		}
	case SUB_GR_SET_USER_RIGHT:	//�û�Ȩ��
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_GR_SetUserRight));
			if (wDataSize!=sizeof(CMD_GR_SetUserRight)) return true;

			//��Ϣ����
			CMD_GR_SetUserRight * pSetUserRight=(CMD_GR_SetUserRight *)pData;

			//�����û�
			IServerUserItem * pIServerUserItemSend=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItemSend==NULL) return true;

			//Ŀ���û�
			IServerUserItem * pIServerUserItemRecv=m_ServerUserManager.SearchOnLineUser(pSetUserRight->dwTargetUserID);
			if (pIServerUserItemRecv==NULL) return true;

			//Ȩ���ж�
			DWORD dwMasterRight=pIServerUserItemSend->GetUserData()->dwMasterRight;
			if (dwMasterRight==0L) return true;

			//��ȡȨ��
			tagServerUserData * pUserData=pIServerUserItemRecv->GetUserData();

			//Ȩ������
			if (CUserRight::CanLimitPlay(dwMasterRight)==true)
			{
				if (pSetUserRight->cbLimitPlayGame==TRUE) pUserData->dwUserRight|=UR_CANNOT_PLAY;
				else pUserData->dwUserRight&=~UR_CANNOT_PLAY;
			}
			if (CUserRight::CanLimitLookon(dwMasterRight)==true)
			{
				if (pSetUserRight->cbLimitLookonGame==TRUE) pUserData->dwUserRight|=UR_CANNOT_LOOKON;
				else pUserData->dwUserRight&=~UR_CANNOT_LOOKON;
			}
			if (CUserRight::CanLimitWisper(dwMasterRight)==true)
			{
				if (pSetUserRight->cbLimitSendWisper==TRUE) pUserData->dwUserRight|=UR_CANNOT_WISPER;
				else pUserData->dwUserRight&=~UR_CANNOT_WISPER;
			}
			if (CUserRight::CanLimitRoomChat(dwMasterRight)==true)
			{
				if (pSetUserRight->cbLimitRoomChat==TRUE) pUserData->dwUserRight|=UR_CANNOT_ROOM_CHAT;
				else pUserData->dwUserRight&=~UR_CANNOT_ROOM_CHAT;
			}
			if (CUserRight::CanLimitGameChat(dwMasterRight)==true)
			{
				if (pSetUserRight->cbLimitGameChat==TRUE) pUserData->dwUserRight|=UR_CANNOT_GAME_CHAT;
				else pUserData->dwUserRight&=~UR_CANNOT_GAME_CHAT;
			}

			//����Ȩ��
			DBR_GR_SetUserRight SetUserRight;
			SetUserRight.dwUserRight=pUserData->dwUserRight;
			SetUserRight.dwUserID=pSetUserRight->dwTargetUserID;
			SetUserRight.dwMasterUserID=pIServerUserItemSend->GetUserID();
			SetUserRight.dwMasterClientIP=pIServerUserItemSend->GetClientIP();
			SetUserRight.cbGame=(CUserRight::CanBindGame(dwMasterRight)==true)?TRUE:FALSE;
			SetUserRight.cbAccounts=(CUserRight::CanBindGlobal(dwMasterRight)==true)?TRUE:FALSE;

			//Ͷ������
			if ((SetUserRight.cbAccounts==TRUE)||(SetUserRight.cbGame==TRUE))
			{
				m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_SET_USER_RIGHT,0,&SetUserRight,sizeof(SetUserRight));
			}

			return true;
		}
	case SUB_GR_SEND_MESSAGE:	//������Ϣ
		{
			//Ч�����
			CMD_GR_SendMessage * pSendMessage=(CMD_GR_SendMessage *)pData;
			ASSERT(wDataSize>=(sizeof(CMD_GR_SendMessage)-sizeof(pSendMessage->szSystemMessage)));
			ASSERT(wDataSize==(sizeof(CMD_GR_SendMessage)-sizeof(pSendMessage->szSystemMessage)+pSendMessage->wChatLength));
			if (wDataSize<(sizeof(CMD_GR_SendMessage)-sizeof(pSendMessage->szSystemMessage))) return true;
			if (wDataSize!=(sizeof(CMD_GR_SendMessage)-sizeof(pSendMessage->szSystemMessage)+pSendMessage->wChatLength)) return true;

			//�����û�
			IServerUserItem * pIServerUserItemSend=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItemSend==NULL) return true;

			//Ȩ���ж�
			DWORD dwMasterRight=pIServerUserItemSend->GetUserData()->dwMasterRight;
			if (CUserRight::CanIssueMessage(dwMasterRight)==false) return true;

			//������Ϣ
			if (pSendMessage->cbGame==TRUE) SendGameMessage(INVALID_WORD,pSendMessage->szSystemMessage,SMT_INFO);
			if (pSendMessage->cbRoom==TRUE) SendRoomMessage(INVALID_WORD,pSendMessage->szSystemMessage,SMT_INFO);

			return true;
		}
	case SUB_GR_OPTION_SERVER:	//��������
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_GR_OptionServer));
			if (wDataSize!=sizeof(CMD_GR_OptionServer)) return true;

			//��Ϣ����
			CMD_GR_OptionServer * pOptionServer=(CMD_GR_OptionServer *)pData;

			//�����û�
			IServerUserItem * pIServerUserItemSend=GetServerUserItem(LOWORD(dwSocketID));
			if (pIServerUserItemSend==NULL) return true;

			//Ȩ���ж�
			DWORD dwMasterRight=pIServerUserItemSend->GetUserData()->dwMasterRight;
			if (CUserRight::CanServerOption(dwMasterRight)==false) return true;

			//��������
			if (pOptionServer->cbOptionFlags==OSF_ROOM_CHAT)
			{
				m_bAllowRoomChat=(pOptionServer->cbOptionValue==TRUE)?true:false;
			}

			//��Ϸ����
			if (pOptionServer->cbOptionFlags==OSF_GAME_CHAT)
			{
				m_bAllowGameChat=(pOptionServer->cbOptionValue==TRUE)?true:false;
			}

			//����˽��
			if (pOptionServer->cbOptionFlags==OSF_ROOM_WISPER)
			{
				m_bAllowWisper=(pOptionServer->cbOptionValue==TRUE)?true:false;
			}

			//������Ϸ
			if (pOptionServer->cbOptionFlags==OSF_ENTER_GAME)
			{
				m_bAllowEnterGame=(pOptionServer->cbOptionValue==TRUE)?true:false;
			}

			//���뷿��
			if (pOptionServer->cbOptionFlags==OSF_ENTER_ROOM)
			{
				m_bAllowEnterRoom=(pOptionServer->cbOptionValue==TRUE)?true:false;
			}

			//�����ر�
			if (pOptionServer->cbOptionFlags==OSF_SHALL_CLOSE)
			{
				m_bShallClose=(pOptionServer->cbOptionValue==TRUE)?true:false;
			}

			//������Ϣ
			LPCTSTR pszMessage=TEXT("�������������ò���ѡ�����óɹ���");
			SendRoomMessage(pIServerUserItemSend,pszMessage,SMT_INFO);

			return true;
		}
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//�����Ϣ����
bool CAttemperEngineSink::OnSocketMainFrame(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//��ȡ�û�
	IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
	if (pIServerUserItem==NULL) return true;

	//�������
	tagServerUserData * pUserData=pIServerUserItem->GetUserData();
	if ((pUserData->wTableID==INVALID_TABLE)||(pUserData->wChairID==INVALID_CHAIR)) return true;

	//��Ϣ���� 
	CTableFrame * pTableFrame=(m_pTableFrame+pUserData->wTableID);
	return pTableFrame->OnEventSocketFrame(wSubCmdID,pData,wDataSize,pIServerUserItem);
}

//��Ϸ��Ϣ����
bool CAttemperEngineSink::OnSocketMainGame(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	__ENTER_FUNCTION_LOG_CATCH

	//��ȡ�û�
	IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
	if (pIServerUserItem==NULL) return true;

	//�������
	tagServerUserData * pUserData=pIServerUserItem->GetUserData();
	if ((pUserData->wTableID==INVALID_TABLE)||(pUserData->wChairID==INVALID_CHAIR)) return true;

	//��Ϣ���� 
	CTableFrame * pTableFrame=(m_pTableFrame+pUserData->wTableID);
	//return pTableFrame->OnEventSocketGame(wSubCmdID,pData,wDataSize,pIServerUserItem); //�����޸�
	//�������
	pTableFrame->OnEventSocketGame(wSubCmdID,pData,wDataSize,pIServerUserItem); 

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//��Ƶ��Ϣ����
bool CAttemperEngineSink::OnSocketMainVideo(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	__ENTER_FUNCTION_LOG_CATCH

	//��ȡ�û�
	IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
	if (pIServerUserItem==NULL) return true;

	//�������
	tagServerUserData * pUserData=pIServerUserItem->GetUserData();
	if ((pUserData->wTableID==INVALID_TABLE)||(pUserData->wChairID==INVALID_CHAIR)) return true;

	//��Ϣ���� 
	CTableFrame * pTableFrame=(m_pTableFrame+pUserData->wTableID);
	pTableFrame->OnEventSocketVideo(wSubCmdID,pData,wDataSize,pIServerUserItem);

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//������Ϣ����
bool CAttemperEngineSink::OnSocketPresent(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	__ENTER_FUNCTION_LOG_CATCH

	//��ȡ�û�
	IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
	if (pIServerUserItem==NULL) return true;

	//��������
	if (m_pGameServiceOption->wServerType==GAME_GENRE_MATCH)
	{
		SendProMessage(pIServerUserItem,TEXT("�������䲻����ʹ�ô˹��ܣ�"),SMT_INFO|SMT_EJECT);
		return true;
	}


	//��Ϣ����
	bool bReturnValue = false;
	switch ( wSubCmdID )
	{
	case SUB_GF_PROP_BUGLE:				//�����¼�
		{
			CMD_GF_BugleProperty *pTemp = (CMD_GF_BugleProperty*)pData;
			m_cbSendLocation = pTemp->cbSendLocation;
			bReturnValue = OnEventBugle(pData,wDataSize,pIServerUserItem);
			break;
		}
	}

	//��ʼ����
	m_cbSendLocation = 0;

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//������Ϣ����
bool CAttemperEngineSink::OnSocketBank(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{

	__ENTER_FUNCTION_LOG_CATCH

	//��ȡ�û�
	IServerUserItem * pIServerUserItem=GetServerUserItem(LOWORD(dwSocketID));
	if (pIServerUserItem==NULL) return true;

	//��������
	if (m_pGameServiceOption->wServerType==GAME_GENRE_MATCH)
	{
		SendProMessage(pIServerUserItem,TEXT("�������䲻����ʹ�ô˹��ܣ�"),SMT_INFO|SMT_EJECT);
		return true;
	}

	//��Ϣ����
	switch ( wSubCmdID )
	{
	case SUB_GF_BANK_GET:					//��ȡ�¼�
		{
			OnEventBankDrawoutGold(pData,wDataSize,dwSocketID);
			return true;
		}
	case SUB_GF_BANK_STORAGE:				//�洢�¼�
		{
			OnEventBankStorage(pData,wDataSize,dwSocketID);
			return true;
		}
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//�����û�
bool CAttemperEngineSink::SendUserItem(IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч�����
	ASSERT(pIServerUserItem!=NULL);

	//��������
	BYTE cbBuffer[SOCKET_PACKET];
	tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)cbBuffer;
	tagServerUserData * pUserData=pIServerUserItem->GetUserData();

	//��д����
	memset(pUserInfoHead,0,sizeof(tagUserInfoHead));
	pUserInfoHead->wFaceID=pUserData->wFaceID;
	pUserInfoHead->dwCustomFaceVer=pUserData->dwCustomFaceVer;
	pUserInfoHead->wTableID=pUserData->wTableID;
	pUserInfoHead->wChairID=pUserData->wChairID;
	pUserInfoHead->cbGender=pUserData->cbGender;
	pUserInfoHead->cbUserStatus=pUserData->cbUserStatus;
	pUserInfoHead->dwUserID=pUserData->dwUserID;
	pUserInfoHead->dwGameID=pUserData->dwGameID;
	pUserInfoHead->dwGroupID=pUserData->dwGroupID;
	pUserInfoHead->dwUserRight=pUserData->dwUserRight;
	pUserInfoHead->lLoveliness=pUserData->lLoveliness;
	pUserInfoHead->dwMasterRight=pUserData->dwMasterRight;
	pUserInfoHead->cbGender=pUserData->cbGender;
	pUserInfoHead->cbMemberOrder=pUserData->cbMemberOrder;
	pUserInfoHead->cbMasterOrder=pUserData->cbMasterOrder;
	pUserInfoHead->UserScoreInfo=pUserData->UserScoreInfo;
	pUserInfoHead->UserScoreInfo.lGold=pUserData->UserScoreInfo.lGold + pUserData->lStorageGold;
	memcpy(pUserInfoHead->szAccounts, pUserData->szAccounts, sizeof(pUserInfoHead->szAccounts));//Author<cxf>;DateTime<2012-03-15>
	//��������
	CSendPacketHelper SendPacket(cbBuffer+sizeof(tagUserInfoHead),sizeof(cbBuffer)-sizeof(tagUserInfoHead));
	//����ɾ��
	//SendPacket.AddPacket(pUserData->szAccounts,CountStringBuffer(pUserData->szAccounts),DTP_USER_ACCOUNTS);
	//�����޸� ���û����ǳ���Ϊ�˺Ŵ�����  ��Ϊ�ͻ��˲���Ҫ�˺�ֻ��Ҫ�ǳƾͿ���
	SendPacket.AddPacket(pUserData->szNickName,CountStringBuffer(pUserData->szNickName),DTP_USER_ACCOUNTS);

	if (pUserData->szUnderWrite[0]!=0) SendPacket.AddPacket(pUserData->szUnderWrite,CountStringBuffer(pUserData->szUnderWrite),DTP_UNDER_WRITE);
	if (pUserData->szGroupName[0]!=0) SendPacket.AddPacket(pUserData->szGroupName,CountStringBuffer(pUserData->szGroupName),DTP_USER_GROUP_NAME);

	//��������
	WORD wSendLength=sizeof(tagUserInfoHead)+SendPacket.GetDataSize();
	if (LOWORD(dwSocketID)!=INVALID_WORD) 
	{
		if (LOWORD(dwSocketID)>=INDEX_ANDROID)
		{
			m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_GR_USER,SUB_GR_USER_COME,cbBuffer,wSendLength);
		}
		else m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GR_USER,SUB_GR_USER_COME,cbBuffer,wSendLength);
	}
	else m_pITCPNetworkEngine->SendDataBatch(MDM_GR_USER,SUB_GR_USER_COME,cbBuffer,wSendLength);

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//������Ϣ ���������﷢�͵�
bool CAttemperEngineSink::SendGameServerInfo(IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	__ENTER_FUNCTION_LOG_CATCH

	//���͵�¼�ɹ�
	CMD_GR_LogonSuccess LogonSuccess;
	memset(&LogonSuccess,0,sizeof(LogonSuccess));
	LogonSuccess.dwUserID=pIServerUserItem->GetUserID();

	if (LOWORD(dwSocketID)>=INDEX_ANDROID)
	{
		m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_SUCCESS,&LogonSuccess,sizeof(LogonSuccess));
	}
	else m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_SUCCESS,&LogonSuccess,sizeof(LogonSuccess));

	//����������Ϣ
	BYTE cbBuffer[SOCKET_PACKET];
	CMD_GR_ServerInfo * pServerInfo=(CMD_GR_ServerInfo *)cbBuffer;
	memset(pServerInfo,0,sizeof(CMD_GR_ServerInfo));
	pServerInfo->wKindID=m_pGameServiceAttrib->wKindID;
	pServerInfo->wGameGenre=m_pGameServiceOption->wServerType;
	pServerInfo->wTableCount=m_pGameServiceOption->wTableCount;
	pServerInfo->wChairCount=m_pGameServiceAttrib->wChairCountMax;
	pServerInfo->dwVideoAddr=m_pGameServiceOption->dwVideoServer;
	pServerInfo->cbHideUserInfo=m_pGameServiceOption->cbHideUserInfo;

	if (LOWORD(dwSocketID)>=INDEX_ANDROID)
	{
		m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_GR_INFO,SUB_GR_SERVER_INFO,cbBuffer,sizeof(CMD_GR_ServerInfo));
	}
	else m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GR_INFO,SUB_GR_SERVER_INFO,cbBuffer,sizeof(CMD_GR_ServerInfo));

	//�����б����
	if (LOWORD(dwSocketID)>=INDEX_ANDROID)
	{
		m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_GR_INFO,SUB_GR_COLUMN_INFO,&m_ColumnInfo,m_wColumnInfoSize);
	}
	else m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GR_INFO,SUB_GR_COLUMN_INFO,&m_ColumnInfo,m_wColumnInfoSize);

	//�����������
	if (LOWORD(dwSocketID)>=INDEX_ANDROID)
	{
		m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_GR_INFO,SUB_GR_CONFIG_FINISH,NULL,0);
	}
	else m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GR_INFO,SUB_GR_CONFIG_FINISH);

	//�����Լ���Ϣ
	SendUserItem(pIServerUserItem,dwSocketID);

	//���������û�
	WORD wEnumIndex=0;
	IServerUserItem * pIServerUserItemSend=NULL;
	while (true)
	{
		pIServerUserItemSend=m_ServerUserManager.EnumOnLineUser(wEnumIndex++);
		if (pIServerUserItemSend==NULL) break;
		if (pIServerUserItemSend==pIServerUserItem) continue;
		SendUserItem(pIServerUserItemSend,dwSocketID);
	}

	//���Ͷ����û�
	wEnumIndex=0;
	while (true)
	{
		pIServerUserItemSend=m_ServerUserManager.EnumOffLineUser(wEnumIndex++);
		if (pIServerUserItemSend==NULL) break;
		if (pIServerUserItemSend==pIServerUserItem) continue;
		SendUserItem(pIServerUserItemSend,dwSocketID);
	}

	//����������Ϣ
	CMD_GR_TableInfo TableInfo;
	TableInfo.wTableCount=m_pGameServiceOption->wTableCount;
	ASSERT(TableInfo.wTableCount<CountArray(TableInfo.TableStatus));
	for (WORD i=0;i<TableInfo.wTableCount;i++)
	{
		CTableFrame * pTableFrame=(m_pTableFrame+i);
		TableInfo.TableStatus[i].bTableLock=pTableFrame->IsTableLocked()?TRUE:FALSE;
		TableInfo.TableStatus[i].bPlayStatus=pTableFrame->IsGameStarted()?TRUE:FALSE;
	}
	WORD wSendSize=sizeof(TableInfo)-sizeof(TableInfo.TableStatus)+TableInfo.wTableCount*sizeof(TableInfo.TableStatus[0]);

	if (LOWORD(dwSocketID)>=INDEX_ANDROID)
	{
		m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_GR_STATUS,SUB_GR_TABLE_INFO,&TableInfo,wSendSize);
	}
	else m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GR_STATUS,SUB_GR_TABLE_INFO,&TableInfo,wSendSize);


	//�������
	if (LOWORD(dwSocketID)>=INDEX_ANDROID)
	{
		m_AndroidUserManager.SendDataToClient(dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_FINISH,NULL,0);
	}
	else m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_FINISH);

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}


//�б���Ϣ����
bool CAttemperEngineSink::OnCenterMainServerList(CMD_Command Command, VOID * pData, WORD wDataSize)
{
	__ENTER_FUNCTION_LOG_CATCH

	switch (Command.wSubCmdID)
	{
	case SUB_CS_LIST_INFO:		//�б���Ϣ
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_ListInfo));
			if (wDataSize!=sizeof(CMD_CS_ListInfo)) return true;

			//��Ϣ����
			m_ServerList.ResetServerListBuffer();

			return true;
		}
	case SUB_CS_LIST_TYPE:		//������Ϣ
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(tagGameType));
			ASSERT(wDataSize%sizeof(tagGameType)==0);
			if (wDataSize!=sizeof(tagGameType)) return true;

			//��Ϣ����
			DWORD dwCount=wDataSize/sizeof(tagGameType);
			tagGameType * pGameType=(tagGameType *)pData;
			m_ServerList.AppendGameTypeBuffer(pGameType,dwCount);

			return true;
		}
	case SUB_CS_LIST_KIND:		//������Ϣ
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(tagGameKind));
			ASSERT(wDataSize%sizeof(tagGameKind)==0);
			if (wDataSize!=sizeof(tagGameKind)) return true;


			//��Ϣ����
			DWORD dwCount=wDataSize/sizeof(tagGameKind);
			tagGameKind * pGameKind=(tagGameKind *)pData;
			m_ServerList.AppendGameKindBuffer(pGameKind,dwCount);

			return true;
		}
	case SUB_CS_LIST_STATION:	//վ����Ϣ
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(tagGameStation));
			ASSERT(wDataSize%sizeof(tagGameStation)==0);
			if (wDataSize!=sizeof(tagGameStation)) return true;
			//��Ϣ����
			DWORD dwCount=wDataSize/sizeof(tagGameStation);
			tagGameStation * pGameStation=(tagGameStation *)pData;
			m_ServerList.AppendGameStationBuffer(pGameStation,dwCount);

			return true;
		}
	case SUB_CS_LIST_SERVER:	//������Ϣ
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(tagGameServer));
			ASSERT(wDataSize%sizeof(tagGameServer)==0);
			if (wDataSize!=sizeof(tagGameServer)) return true;
			//��Ϣ����
			DWORD dwCount=wDataSize/sizeof(tagGameServer);
			tagGameServer* pGameServer=(tagGameServer *)pData;
			m_ServerList.AppendGameServerBuffer(pGameServer,dwCount);

			return true;
		}
	case SUB_CS_LIST_FINISH:	//�������
		{
			//�����
			m_ServerList.ActiveServerListBuffer();

			//��������
			WORD wInfoCount=m_ServerList.GetOnLineCountInfo(m_OnLineCountInfo,CountArray(m_OnLineCountInfo));
			m_wOnLineInfoSize=wInfoCount*sizeof(tagOnLineCountInfo);

			//������Ϣ
			if ((m_wOnLineInfoSize>0)&&(m_ServerUserManager.GetOnLineCount()>0))
			{
				m_pITCPNetworkEngine->SendDataBatch(MDM_GR_SERVER_INFO,SUB_GR_ONLINE_COUNT_INFO,&m_OnLineCountInfo,m_wOnLineInfoSize);
				m_AndroidUserManager.SendDataToClient(MDM_GR_SERVER_INFO,SUB_GR_ONLINE_COUNT_INFO,&m_OnLineCountInfo,m_wOnLineInfoSize);
			}
			return true;
		}
	}

	__LEAVE_FUNCTION_LOG_CATCH


	return true;
}

//�󶨲���
tagConnectItemInfo * CAttemperEngineSink::GetBindParameter(WORD wBindIndex)
{
	__ENTER_FUNCTION_LOG_CATCH

	//��Ч����
	if (wBindIndex==INVALID_WORD) return NULL;

	//��������
	if (wBindIndex<m_pGameServiceOption->wMaxConnect)
	{
		return m_pNormalParameter+wBindIndex;
	}

	//��������
	if ((wBindIndex>=INDEX_ANDROID)&&(wBindIndex<(INDEX_ANDROID+MAX_ANDROID)))
	{
		return m_pAndroidParameter+(wBindIndex-INDEX_ANDROID);
	}

	//�������
	ASSERT(FALSE);

	__LEAVE_FUNCTION_LOG_CATCH

	return NULL;
}

//���û�
IServerUserItem * CAttemperEngineSink::GetServerUserItem(WORD wBindIndex)
{
	__ENTER_FUNCTION_LOG_CATCH

	//��ȡ����
	tagConnectItemInfo * pBindParameter=GetBindParameter(wBindIndex);

	//��ȡ�û�
	if (pBindParameter!=NULL)
	{
		return pBindParameter->pIServerUserItem;
	}

	//�������
	ASSERT(FALSE);

	__LEAVE_FUNCTION_LOG_CATCH

	return NULL;
}

//�û�״��
WORD CAttemperEngineSink::GetAndroidUserStatus(tagAndroidUserStatus & AndroidUserStatus)
{
	//��������
	WORD wEnumIndex=0;
	IServerUserItem * pIServerUserItem=NULL;

	//���ñ���
	ZeroMemory(&AndroidUserStatus,sizeof(AndroidUserStatus));

	//�û���Ϣ
	do
	{
		//��ȡ�û�
		pIServerUserItem=m_ServerUserManager.EnumOnLineUser(wEnumIndex++);

		//ͳ���ж�
		if ((pIServerUserItem!=NULL)&&(pIServerUserItem->IsAndroidUser()==true))
		{
			//��ȡ����
			BYTE cbUserStatus=pIServerUserItem->GetUserStatus();

			//�û�ͳ��
			switch (pIServerUserItem->GetUserStatus())
			{
			case US_FREE:
				{
					ASSERT(AndroidUserStatus.wFreeUserCount<CountArray(AndroidUserStatus.pServerUserFree));
					AndroidUserStatus.pServerUserFree[AndroidUserStatus.wFreeUserCount++]=pIServerUserItem;
					break;
				}
			case US_PLAY:
				{
					ASSERT(AndroidUserStatus.wPlayUserCount<CountArray(AndroidUserStatus.pServerUserPlay));
					AndroidUserStatus.pServerUserPlay[AndroidUserStatus.wPlayUserCount++]=pIServerUserItem;
					break;
				}
			case US_SIT:
			case US_READY:
				{
					ASSERT(AndroidUserStatus.wSitdownUserCount<CountArray(AndroidUserStatus.pServerUserSitdown));
					AndroidUserStatus.pServerUserSitdown[AndroidUserStatus.wSitdownUserCount++]=pIServerUserItem;
					break;
				}
			}
		}

	} while (pIServerUserItem!=NULL);

	return AndroidUserStatus.wFreeUserCount+AndroidUserStatus.wPlayUserCount+AndroidUserStatus.wSitdownUserCount;
}

//����״��
WORD CAttemperEngineSink::GetTableUserStatus(CTableFrame * pTableFrame, tagTableUserStatus & TableUserStatus)
{
	//���ñ���
	ZeroMemory(&TableUserStatus,sizeof(TableUserStatus));

	//�û�����
	for (WORD i=0;i<m_pGameServiceAttrib->wChairCountMax;i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=pTableFrame->GetServerUserItem(i);
		if (pIServerUserItem==NULL) continue;

		//�û�����
		if (pIServerUserItem->IsAndroidUser()==false)
		{
			TableUserStatus.wTableUserCount++;
		}
		else
		{
			TableUserStatus.wTableAndroidCount++;
		}
	}

	//������Ŀ
	switch (pTableFrame->m_pITableFrameSink->GetGameStartMode())
	{
	case enStartMode_AllReady:	//����׼��
		{
			TableUserStatus.wLessUserCount=2;
			break;
		}
	default:					//Ĭ��ģʽ
		{
			TableUserStatus.wLessUserCount=m_pGameServiceAttrib->wChairCountMax;
			break;
		}
	}

	return TableUserStatus.wTableAndroidCount+TableUserStatus.wTableUserCount;
}


//�����¼�
bool CAttemperEngineSink::OnEventBugle(const void * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return true;
}

//��ȡ�¼�
bool CAttemperEngineSink::OnEventBankDrawoutGold(const void * pData, WORD wDataSize, DWORD dwSocketID)
{
	return true;
}

//�洢�¼�
bool CAttemperEngineSink::OnEventBankStorage(const void * pData, WORD wDataSize, DWORD dwSocketID)
{
	return true;
}

//ʹ��Ȩ��
bool CAttemperEngineSink::IsPropertyUseRight(INT nPropertyID,IServerUserItem *pIServerUserItem,IServerUserItem *pTargetUserItem)
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ŀ�����
	if ( pTargetUserItem == NULL )
	{
		SendProMessage(pIServerUserItem, TEXT("����ʧ�ܣ���Ҫ���͵�����Ѿ��뿪��"), SMT_EJECT|SMT_INFO);
		return false;
	}

	//��ȡ���
	ASSERT(pIServerUserItem!=NULL);
	tagServerUserData *pConsumeUserData = pIServerUserItem->GetUserData();
	tagServerUserData *pTargetUserData = pTargetUserItem->GetUserData();

	//�����ж�
	if ( (m_pGameServiceOption->wServerType == GAME_GENRE_GOLD || m_pGameServiceOption->wServerType == GAME_GENRE_MATCH )
		&& ( nPropertyID == PROP_NEGAGIVE || nPropertyID == PROP_DOUBLE || nPropertyID == PROP_FOURDOLD || nPropertyID == PROP_SHIELD) )
	{
		TCHAR szMessage[] = TEXT("�˷��䲻����ʹ�ô˵��ߣ�");
		SendProMessage(pIServerUserItem, szMessage, SMT_INFO|SMT_EJECT);
		return false;
	}

	//ʹ���ж�
	switch(nPropertyID)
	{
	case PROP_NEGAGIVE :			//��������
		{
			LONGLONG lNowScore = pTargetUserData->UserScoreInfo.lGold;
			if( 0 <= lNowScore)
			{
				TCHAR szMessage[128];			

				if ( pIServerUserItem == pTargetUserItem ) 
					_sntprintf(szMessage,CountArray(szMessage),TEXT("�����ڵĻ����Ѿ��ǷǸ���������Ҫʹ�ø���������ߣ�"));
				else
					_sntprintf(szMessage,CountArray(szMessage),TEXT("[ ��� ]���ڵĻ����Ѿ��ǷǸ���������Ҫʹ�ø���������ߣ�"), pTargetUserData->szAccounts);

				SendProMessage(pIServerUserItem, szMessage, SMT_INFO|SMT_EJECT);
				return false;
			}
			break;
		}
	case PROP_FLEE :				//��������
		{
			LONG lNowFleeCount = pTargetUserData->UserScoreInfo.lFleeCount;

			if ( lNowFleeCount == 0 )
			{
				TCHAR szMessage[128];			

				if ( pIServerUserItem == pTargetUserItem ) 
					_sntprintf(szMessage,CountArray(szMessage),TEXT("�����ڵ��������Ѿ�Ϊ0������Ҫʹ������������ߣ�"));
				else
					_sntprintf(szMessage,CountArray(szMessage),TEXT("[ ��� ]���ڵ��������Ѿ�Ϊ0������Ҫʹ������������ߣ�"), pTargetUserData->szAccounts);

				SendProMessage(pIServerUserItem, szMessage, SMT_INFO|SMT_EJECT);
				return false;
			}
			break;
		}
	case PROP_BUGLE :				//�����ж�
		{
			//��������
			bool bMatchServer=((m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)!=0);
			bool bControlStart=((bMatchServer==true)&&m_pGameServiceOption->cbControlStart==TRUE);

			//�����Թ�
			if (pConsumeUserData->dwMasterRight==0L)
			{
				if ((bMatchServer==true)&&(pIServerUserItem->GetUserStatus()==US_LOOKON))
				{
					LPCTSTR pszMessage=TEXT("Ϊ�˲�Ӱ�����ѡ�ֱ������Թ۵��û���ֹ���ԣ�");
					SendProMessage(pIServerUserItem,pszMessage,SMT_EJECT|SMT_INFO);
					return false;
				}
			}

			//Ȩ���ж�
			if (CUserRight::CanGameChat(pConsumeUserData->dwUserRight)==false)
			{
				LPCTSTR pszMessage=TEXT("����ʱû�з��ͷ�����Ϣ��Ȩ�ޣ�ֻ�������Ա˽�ģ�");
				SendProMessage(pIServerUserItem,pszMessage,SMT_EJECT|SMT_INFO);
				return false;
			}

			//״̬��ѯ
			if ((pConsumeUserData->dwMasterRight==0L)&&(IsAllowGameChat()==false))
			{
				LPCTSTR pszMessage=TEXT("��Ǹ������Ϸ���䲻������������Ϣ��");
				SendProMessage(pIServerUserItem,pszMessage,SMT_EJECT|SMT_INFO);
				return false;
			}
			break;
		}
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//�޸Ľ��  ����ʹ��
void CAttemperEngineSink::ModifyGameGold(IServerUserItem * pIServerUserItem,LONGLONG lGameGold)
{
	//�û���Ϣ
	return;
}

//��ȡ�ʿ�
LONGLONG CAttemperEngineSink::GetBankStorageGold(IServerUserItem * pIServerUserItem)
{
	//��ȡ�ṹ
	tagServerUserData *pServerUserData = pIServerUserItem->GetUserData();

	return pServerUserData->UserScoreInfo.lMoney;
}

//�޸��ʿ�
void CAttemperEngineSink::ModifyBankStorageGold(IServerUserItem * pIServerUserItem, LONGLONG lStorageCount)
{
	__ENTER_FUNCTION_LOG_CATCH

	//�޸Ľ��
	pIServerUserItem->ModifyBankStorageGold(lStorageCount);

	//����֪ͨ
	SendUserScore( pIServerUserItem );

	__LEAVE_FUNCTION_LOG_CATCH

	return ;
}

bool CAttemperEngineSink::OnDBFindGemsTaskLoad(VOID * pData, WORD wDataSize)
{

	__ENTER_FUNCTION_LOG_CATCH


	ASSERT(wDataSize==sizeof(DBR_FIND_GEMS_TASK));
	if (wDataSize!=sizeof(DBR_FIND_GEMS_TASK)) return false;

	//��Ϣ����
	DBR_FIND_GEMS_TASK * pUserMiss=(DBR_FIND_GEMS_TASK *)pData;
	//�������
	IServerUserItem *pServerUserItem = m_ServerUserManager.SearchOnLineUser(pUserMiss->dwUserID);
	if ( pServerUserItem == NULL ) pServerUserItem = m_ServerUserManager.SearchOffLineUser(pUserMiss->dwUserID);
	if ( pServerUserItem == NULL ) return true;

	tagServerUserData *pServerUserData = pServerUserItem->GetUserData();
	memcpy(&(pServerUserData->task),  pUserMiss, sizeof(DBR_FIND_GEMS_TASK));

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}



bool CAttemperEngineSink::OnDBLoadAffiche(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_affiche));
	if (wDataSize!=sizeof(DBR_GR_affiche)) return false;

	DBR_GR_affiche* pAffiche = (DBR_GR_affiche*)pData;
	if(pAffiche != NULL)
	{
		TCHAR buf[BUFSIZ] = {0};
		sprintf(buf, "����:%s|**|", pAffiche->szInfo);
		m_strAffiche += buf;
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//����ϲ��
bool CAttemperEngineSink::OnDBLoadHappyMess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_happyMess));
	if (wDataSize!=sizeof(DBR_GR_happyMess)) return false;

	DBR_GR_happyMess* pHappyMess = (DBR_GR_happyMess*)pData;
	if(pHappyMess != NULL)
	{
		TCHAR buf[BUFSIZ] = {0};
		sprintf(buf, "ϲ��:%s|**|", pHappyMess->szInfo);
		m_strHappyMess += buf;
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//�������ݿ���Ӧ�������
void CAttemperEngineSink::PerformTaskFinsh(const CMD_GF_TASK_tagTaskFinish& taskFinish)
{

	__ENTER_FUNCTION_LOG_CATCH


	IServerUserItem *pServerUserItem = m_ServerUserManager.SearchOnLineUser(taskFinish.dwUserID);
	if ( pServerUserItem == NULL ) pServerUserItem = m_ServerUserManager.SearchOffLineUser(taskFinish.dwUserID);
	if ( pServerUserItem == NULL ) return;

	DBR_TASK_Finish dtf;
	dtf.bFinish = taskFinish.bFinish;
	dtf.dwGems  = taskFinish.dwGems;
	dtf.dwKindID = taskFinish.dwKindID;
	dtf.dwUserID = taskFinish.dwUserID;
	dtf.iTaskID  = taskFinish.iTaskID;
	dtf.iTaskUserLogID = taskFinish.iTaskUserLogID;
	memcpy(dtf.strUserLogTable,taskFinish.strUserLogTable, sizeof(dtf.strUserLogTable));
	dtf.dwServerID = m_pGameServiceOption->wServerID;

	DWORD ip = pServerUserItem->GetClientIP();
	char strIP[24] = {0};	
	sprintf(strIP, TEXT("%d.%d.%d.%d"),  FOURTH_IPADDRESS(ip),THIRD_IPADDRESS(ip),
		SECOND_IPADDRESS(ip),FIRST_IPADDRESS(ip)); 

	memcpy(dtf.szClientIP, strIP, sizeof(dtf.szClientIP));
	dtf.dwSortID  = m_pGameServiceOption->wSortID;

	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_USER_FIND_GEMS_TASK_PERFORM_FINISH, (DWORD)0, 
		(void*)&dtf,   sizeof(DBR_TASK_Finish));

	__LEAVE_FUNCTION_LOG_CATCH

}


//����Ѱ������
bool CAttemperEngineSink::SendFindGemsTask(IServerUserItem * pIServerUserItem)
{
	__ENTER_FUNCTION_LOG_CATCH


	//��������
	tagServerUserData *pServerUserData = pIServerUserItem->GetUserData();
	//������Ϣ
	SendData(pIServerUserItem, MDM_GF_TASK , SUB_GF_FIND_GEMS_TASK_ACCEPT, &(pServerUserData->task), sizeof(CMD_GF_TASK_tagTask));

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//�������������Ϣ
bool CAttemperEngineSink::SendTaskFinish(VOID * pData, WORD wDataSize)
{
	__ENTER_FUNCTION_LOG_CATCH

	ASSERT(wDataSize==sizeof(CMD_GF_TASK_tagTaskFinish));
	if (wDataSize!=sizeof(CMD_GF_TASK_tagTaskFinish)) return false;

	CMD_GF_TASK_tagTaskFinish * pTaskFinish=(CMD_GF_TASK_tagTaskFinish *)pData;
	IServerUserItem *pServerUserItem = m_ServerUserManager.SearchOnLineUser(pTaskFinish->dwUserID);
	if ( pServerUserItem == NULL ) pServerUserItem = m_ServerUserManager.SearchOffLineUser(pTaskFinish->dwUserID);
	if ( pServerUserItem == NULL ) return true;

	tagServerUserData* pServerUserData = pServerUserItem->GetUserData();
	if(pServerUserData->wTableID!=INVALID_TABLE)
	{
		CTableFrame * pTableFrame=(m_pTableFrame+pServerUserData->wTableID);
		if (pTableFrame != NULL)
		{
			////������Ϣ���ͻ���
			pTableFrame->BroadcastMessage( MDM_GF_TASK,  SUB_GF_FIND_GEMS_TASK_ONCEEND, pData, wDataSize);
		}
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//Ⱥ��ϲ��
void CAttemperEngineSink:: SendHappyMess()
{
	__ENTER_FUNCTION_LOG_CATCH

	WORD    wEnumIndex=0;
	size_t  wSendSize = m_strHappyMess.size();
	while (true)
	{
		IServerUserItem*  pIServerUserItemSend=m_ServerUserManager.EnumOnLineUser(wEnumIndex++);
		if (pIServerUserItemSend==NULL) break;
		if (wSendSize > (size_t)0) SendData(pIServerUserItemSend, MDM_GF_FRAME, SUB_GR_HAPPY_MESS,   (char*)m_strHappyMess.c_str(), wSendSize);

	}

	__LEAVE_FUNCTION_LOG_CATCH

}

//���͹�����Ϣ
void CAttemperEngineSink::SendAffiche(IServerUserItem * pIServerUserItem)
{
	__ENTER_FUNCTION_LOG_CATCH

	size_t wSendSize = m_strAffiche.size();
	if (wSendSize > (size_t)0) SendData(pIServerUserItem, MDM_GR_SYSTEM, SUB_GR_LOGON_AFFICHE, (char*)m_strAffiche.c_str(), wSendSize);

	__LEAVE_FUNCTION_LOG_CATCH
}

//���Ͷ���
bool CAttemperEngineSink::SendUserActionInfo(IServerUserItem* pIServerUserItem, void* pData, DWORD wDataSize)
{
	__ENTER_FUNCTION_LOG_CATCH

	tagServerUserData * pUserData=pIServerUserItem->GetUserData();
	if (pUserData->wTableID!=INVALID_TABLE)
	{
		CTableFrame * pTableFrame=m_pTableFrame+pUserData->wTableID;
		pTableFrame->BroadcastMessage(MDM_GR_USER,  SUB_RECVACTIONINFO, pData,  wDataSize);
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//�õ�����������ӵĸ���  �������
unsigned int CAttemperEngineSink::CountPlayeringDesk()
{
	CTableFrame * pTableFrame = NULL;
	unsigned int count = 0;
	for (INT_PTR i=0;i<m_pGameServiceOption->wTableCount;i++)
	{
		//��ȡ��������
		pTableFrame = (m_pTableFrame + i);
		if (pTableFrame->IsGameStarted() || pTableFrame->GetSittingChairCount() >= m_pGameServiceAttrib->wChairCountMin) 
		{
			++count;
		}
	}
	return count;
}



//ִ��ȱ�����ӵ����²���
bool CAttemperEngineSink::PerformSitDownOnDesk(int lessUser)
{
	__ENTER_FUNCTION_LOG_CATCH


	char info[BUFSIZ]={0};
	int count = 0;
	while (m_UserAutoQueue.Size() > 0)
	{
		if(count++ > 5)  //��ֹ����ѭ��
		{
#if _SHOW_MESSAGE_NOR
			sprintf(info, _T("Ŀǰɨ��5�Σ������޷��ҵ����ʵ���λ�� ����%i���Ŷ��û�"), m_UserAutoQueue.Size());
			CTraceService::TraceString(info,TraceLevel_Normal);
#endif		
			break;
		}

		CTableFrame * pTableFrame  = NULL;
		int           needSetUser  = 0;
		DWORD        nowTime       = (DWORD)time(NULL);
		for (int i = 0;i<m_pGameServiceOption->wTableCount;i++)
		{
			//��ȡ��������
			pTableFrame   =  (m_pTableFrame+i);
			int totalUser =  (int)(pTableFrame->GetAndroidCountInDesk() + m_UserAutoQueue.Size() + pTableFrame->GetNormalCountInDesk());
			if (pTableFrame->IsGameStarted()      == false &&
				nowTime - pTableFrame->GetEndTime() > MAX_DELAY &&
				pTableFrame->GetNormalCountInDesk() == lessUser     &&  //��֤������������
				pTableFrame->GetNormalCountInDesk() < m_pGameServiceAttrib->wChairCountMax &&
				totalUser >= m_pGameServiceAttrib->wChairCountMin )  //Ŀǰ������ӵ���㹻�Ŀ�λ���ҿ������Ͽ�ʼ��Ϸ
			{
#if _SHOW_MESSAGE_NOR
				sprintf(info, _T("Ŀǰ����ͨ��ҵ�������ţ�%i"), i);
				CTraceService::TraceString(info,TraceLevel_Normal);
#endif				

				int needCancelAndroid = totalUser - m_pGameServiceAttrib->wChairCountMin;
				if (needCancelAndroid > 0)
				{
					pTableFrame->CancelAndroid(needCancelAndroid);
				}
				needSetUser = (int)m_UserAutoQueue.Size();
				break;
			}
			pTableFrame = NULL;
		}

		if (pTableFrame == NULL && lessUser == m_pGameServiceAttrib->wChairCountMax) //û���ҵ��������˵����� �����л����˵����ӻ��߿�����
		{
			for (int i = 0;i<m_pGameServiceOption->wTableCount;i++)
			{
				//��ȡ��������
				pTableFrame   =  (m_pTableFrame+i);
				int totalUser =  (int)(pTableFrame->GetAndroidCountInDesk() + m_UserAutoQueue.Size() + pTableFrame->GetNormalCountInDesk());
				if (pTableFrame->IsGameStarted()       == false && 
					nowTime - pTableFrame->GetEndTime() > MAX_DELAY &&
					pTableFrame->GetNormalCountInDesk() < m_pGameServiceAttrib->wChairCountMax &&
					totalUser >= m_pGameServiceAttrib->wChairCountMin)  //Ŀǰ������ӵ���㹻�Ŀ�λ���ҿ������Ͽ�ʼ��Ϸ
				{
#if _SHOW_MESSAGE_NOR
					sprintf(info, _T("Ŀǰû����ͨ��ҵ�������ţ�%i"), i);
					CTraceService::TraceString(info,TraceLevel_Normal);
#endif				

					int needCancelAndroid = totalUser - m_pGameServiceAttrib->wChairCountMin;
					if (needCancelAndroid > 0)
					{
						pTableFrame->CancelAndroid(needCancelAndroid);
					}
					needSetUser = (int)m_UserAutoQueue.Size();
					break;
				}
				pTableFrame = NULL;
			}
		}


		if (pTableFrame != NULL)
		{
/*Author<cxf>;DateTime<2012-03-30>;Description<����:���ڲ���һ����Ҳ������>
			if (m_UserAutoQueue.Size() == 1 &&  !pTableFrame->HasPlayer())  //���ֻ��һ���û��������ҵ��������Ǹ��յ�����
			{
#if _SHOW_MESSAGE_NOR
				CTraceService::TraceString(TEXT("ֻ��һ���û��������ҵ��������Ǹ��յ�����"),TraceLevel_Normal);
#endif				
				break;
			}
*/

			tagServerUserData * pUserData = NULL;
			for(BYTE j=0; j<needSetUser; j++)
			{
				//���´���
				WORD wChairID=pTableFrame->GetNullChairID(); 
				if (wChairID!=INVALID_CHAIR)
				{
					IServerUserItem*  pIServerUserItem=m_UserAutoQueue.GetFront();
					if (pIServerUserItem==NULL) 
					{
#if _SHOW_MESSAGE_NOR
						CTraceService::TraceString(TEXT("����Ϊ��"),TraceLevel_Normal);
#endif				
						m_UserAutoQueue.PoPFront();
						return false;
					}else
					{
#if _SHOW_MESSAGE_NOR
						pUserData=pIServerUserItem->GetUserData();
						sprintf(info, _T("�õ�Ŀǰ���û� %s"), pUserData->szAccounts);
						CTraceService::TraceString(info,TraceLevel_Normal);
#endif
					}


					if (pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem,NULL)==true)
					{
						pUserData=pIServerUserItem->GetUserData();
#if _SHOW_MESSAGE_NOR						
						sprintf(info, _T("%s �û��Ѿ�������"), pUserData->szAccounts);
						CTraceService::TraceString(info,TraceLevel_Normal);
#endif
						m_UserAutoQueue.PoPFront();
					}else
					{
#if _SHOW_MESSAGE_NOR
						pUserData=pIServerUserItem->GetUserData();
						sprintf(info, _T("%s ����ʧ��"), pUserData->szAccounts);
						CTraceService::TraceString(info,TraceLevel_Normal);
#endif				
						m_UserAutoQueue.PoPFront(); //���� ��ʱ��������,û�б�Ҫ�����;�����Ϣ���û�
						//��Ϊ��ִ��PerformSitDownAction���Ѿ�������
					}
				}
				else
				{
#if _SHOW_MESSAGE_NOR
					sprintf(info, _T("û��������"));
					CTraceService::TraceString(info,TraceLevel_Normal);
#endif				
					break;
				}
			}
		}
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}


//���û�����ȴ�������
bool CAttemperEngineSink::InsertUserInQueue(IServerUserItem * pIServerUserItem)
{
	__ENTER_FUNCTION_LOG_CATCH


	char info[BUFSIZ]={0};
	tagServerUserData * pUserData=pIServerUserItem->GetUserData();
	pUserData->m_ArrangeQueueTime = (DWORD)time(NULL);
	if (pUserData->utUserType == PERSON)
	{
		if (!m_UserAutoQueue.IsExist(pIServerUserItem))
		{
			m_UserAutoQueue.PushBack(pIServerUserItem);
#if _SHOW_MESSAGE_NOR		
			sprintf(info, _T("%s �û�û���Ŷӣ��������: ���й���%i��"), pUserData->szAccounts, m_UserAutoQueue.Size());
			CTraceService::TraceString(info, TraceLevel_Normal);
#endif			
			return true;
		}else
		{
#if _SHOW_MESSAGE_NOR
			//SendData(pIServerUserItem, MDM_GR_USER, SUB_GR_AUTO_QUEUE_ALREADY);
			sprintf(info, _T("%s �û��Ѿ��Ŷ���; ���й���%i��"), pUserData->szAccounts, m_UserAutoQueue.Size());
			CTraceService::TraceString(info,TraceLevel_Normal);
#endif			
			return false;
		}
	}else if (pUserData->utUserType == ANDROID)
	{
		if (!m_AndroidAutoQueue.IsExist(pIServerUserItem))
		{
			m_AndroidAutoQueue.PushBack(pIServerUserItem);
#if _SHOW_MESSAGE_NOR			
			sprintf(info, _T("%s ������û���Ŷӣ��������: ���й���%i������"), pUserData->szAccounts, m_AndroidAutoQueue.Size());
			CTraceService::TraceString(info, TraceLevel_Normal);
#endif			
			return true;
		}else
		{
			//SendData(pIServerUserItem, MDM_GR_USER, SUB_GR_AUTO_QUEUE_ALREADY);
#if _SHOW_MESSAGE_NOR		
			sprintf(info, _T("%s �������Ѿ��Ŷ���; ���й���%i������"), pUserData->szAccounts, m_AndroidAutoQueue.Size());
			CTraceService::TraceString(info,TraceLevel_Normal);
#endif			
			return false;
		}
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return false;
}

//����û��Ƿ�������
bool CAttemperEngineSink::CheckUserPlaying(IServerUserItem * pIServerUserItem)
{

	__ENTER_FUNCTION_LOG_CATCH


	tagServerUserData * pUserData=pIServerUserItem->GetUserData();
	//�뿪λ��
	if (pUserData->wTableID!=INVALID_TABLE)
	{
		CTableFrame * pTableFrame=(m_pTableFrame+pUserData->wTableID);
		if (pTableFrame->IsUserPlaying(pIServerUserItem)==true)
		{
			//��������
			TCHAR szDescribe[128]=TEXT("");
			_snprintf(szDescribe,sizeof(szDescribe),TEXT("�����ڵ� [ %d ] ����Ϸ����Ϸ����ʱ�����Ŷӣ�"),pUserData->wTableID+1);
			SendWarningPacket(pIServerUserItem, MDM_GR_USER, SUB_GR_AUTO_QUEUE_FAIL, szDescribe);
			return false;
		}
		pTableFrame->PerformStandUpAction(pIServerUserItem);
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//���¶�ʱ���ӻ����˳���
void CAttemperEngineSink::UpDateAddAndroid()
{
	__ENTER_FUNCTION_LOG_CATCH


	#if _SHOW_MESSAGE_NOR
		CTraceService::TraceString(_T("���������濪ʼ"), TraceLevel_Normal);
	#endif	

	char info[BUFSIZ]={0};
	int busyDesk       = CountPlayeringDesk();
	int needCancelDesk = busyDesk - (int)m_pGameServiceOption->wAndroidTableCount;
	if ( needCancelDesk > 0 ) //���Ŀǰ��æ�����Ӵ��ڻ�����վλ��������Ŀ�����һЩ����
	{
		for (INT_PTR i=0; i<m_pGameServiceOption->wTableCount && needCancelDesk > 0; i++) //�������������
		{
			//��ȡ��������OnlyAndroidInDesk
			CTableFrame * pTableFrame = (m_pTableFrame+i);
			WORD totalChairCount  = pTableFrame->GetSittingChairCount();
			if ( totalChairCount >= m_pGameServiceAttrib->wChairCountMin &&
				pTableFrame->HasPlayer()  && 
				pTableFrame->OnlyAndroidInDesk()
				) //�õ�һ��ֻ�л����˵�����
			{
#if _SHOW_MESSAGE_NOR
				sprintf(info, _T("������һ����Ҫ��ɢ��ֻ�л����˵����ӣ����Ͻ�ɢ"));
				CTraceService::TraceString(info,TraceLevel_Normal);
#endif				
				pTableFrame->CancelOnlyAndroidDesk();
				--needCancelDesk;
			}
		}
	}


	//�������ӻ����˺��û���
	IServerUserItem*  pIServerUserItem = m_UserAutoQueue.GetFront();
	if (pIServerUserItem != NULL)
	{
		tagServerUserData * pUserData      = pIServerUserItem->GetUserData();
		DWORD timeNow  = (DWORD)time(NULL);
		DWORD timeLeft = timeNow - pUserData->m_ArrangeQueueTime;
		if ( timeLeft > m_pGameServiceOption->wUserWaitUntilAndroid &&
			m_AndroidAutoQueue.Size() + m_UserAutoQueue.Size() >=  m_pGameServiceAttrib->wChairCountMin)
		{
			CTableFrame * pTableFrame = NULL;

			for (INT_PTR i=0;i<m_pGameServiceOption->wTableCount;i++)
			{
				//��ȡ��������
				pTableFrame         = (m_pTableFrame+i);
				WORD totalNullChairCount = pTableFrame->GetTatolNullChairCount();
				if (pTableFrame->IsGameStarted()==false && totalNullChairCount == m_pGameServiceAttrib->wChairCountMax) //�õ�һ���п�λ����
				{
#if _SHOW_MESSAGE_NOR
					sprintf(info, _T("��������ˣ� Ŀǰ�����ӹ���%i��λ��"), totalNullChairCount);
					CTraceService::TraceString(info,TraceLevel_Normal);
#endif				
					break;
				}
				pTableFrame = NULL;
			}

			if (pTableFrame  != NULL)
			{
				int needAnroid = (int)(m_pGameServiceAttrib->wChairCountMin) - (int)m_UserAutoQueue.Size();
				if (needAnroid > 0)
				{
					while (m_UserAutoQueue.Size() > 0)
					{
						//���´���
						WORD wChairID=pTableFrame->GetNullChairID(); 
						if (wChairID!=INVALID_CHAIR)
						{
							IServerUserItem*  pIServerUserItem= m_UserAutoQueue.GetFront();
							if (pIServerUserItem==NULL) 
							{
#if _SHOW_MESSAGE_NOR
								CTraceService::TraceString(TEXT("��Ҷ���Ϊ��"),TraceLevel_Normal);
#endif				
								m_UserAutoQueue.PoPFront();
							}else
							{
								pUserData=pIServerUserItem->GetUserData();
#if _SHOW_MESSAGE_NOR						
								sprintf(info, _T("���, �õ�Ŀǰ���û� %s"), pUserData->szAccounts);
								CTraceService::TraceString(info,TraceLevel_Normal);
#endif				
							}


							if (pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem,NULL)==true)
							{
								pUserData=pIServerUserItem->GetUserData();
#if _SHOW_MESSAGE_NOR						
								sprintf(info, _T("%s ����û��Ѿ�������"), pUserData->szAccounts);
								CTraceService::TraceString(info,TraceLevel_Normal);
#endif				
								m_UserAutoQueue.PoPFront();
							}else
							{
								pUserData=pIServerUserItem->GetUserData();
#if _SHOW_MESSAGE_NOR							
								sprintf(info, _T("%s �������ʧ��"), pUserData->szAccounts);
								CTraceService::TraceString(info,TraceLevel_Normal);
#endif				
								m_UserAutoQueue.PoPFront(); //���� ��ʱ��������,û�б�Ҫ�����;�����Ϣ���û�
							}
						}
					}


					for (int i=0; i<needAnroid; ++i)
					{	
						//���´���
						WORD wChairID=pTableFrame->GetNullChairID(); 
						if (wChairID!=INVALID_CHAIR)
						{
							IServerUserItem*  pIServerUserItem= m_AndroidAutoQueue.GetFront();

							if (pIServerUserItem != NULL)
							{
								pUserData=pIServerUserItem->GetUserData();
								if (pUserData != NULL  && pUserData->UserScoreInfo.lGold >= m_pGameServiceOption->lRestrictScore)
								{
#if _SHOW_MESSAGE_NOR						
									sprintf(info, _T("%s վλ��������������....."), pUserData->szAccounts);
									CTraceService::TraceString(info,TraceLevel_Normal);
#endif	
									pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem,NULL);						
								}
								m_AndroidAutoQueue.PoPFront(); //���û��ӻ����˶�����ɾ��
							}	
						}
					}
				}
			}
		}
	}
#if _SHOW_MESSAGE_NOR
	CTraceService::TraceString(_T("�������������"), TraceLevel_Normal);
#endif

	__LEAVE_FUNCTION_LOG_CATCH

}


void CAttemperEngineSink::UpdateBusyDesk()
{
	DWORD busyDeskCount = CountPlayeringDesk();
	m_BusyDeskStat.Insert(busyDeskCount );  //�������ӵ�ƽ������
}

void CAttemperEngineSink::TryAddUser()
{
	__ENTER_FUNCTION_LOG_CATCH


	char info[BUFSIZ]={0};
	for (INT_PTR i=0; i<m_pGameServiceOption->wTableCount; i++) //��������˿��е����ӣ�����2��������վλ��������Ϸ����Ҫһ����ҵ����
	{
		//��ȡ��������OnlyAndroidInDesk
		CTableFrame * pTableFrame = (m_pTableFrame+i);
		WORD totalChairCount  = pTableFrame->GetSittingChairCount();
		if (pTableFrame->IsGameStarted()==false && 
			totalChairCount < m_pGameServiceAttrib->wChairCountMin && 
			pTableFrame->HasPlayer()  &&
			pTableFrame->OnlyAndroidInDesk()) //�ж��ǲ��ǻ����˿��е�����
		{
			#if _SHOW_MESSAGE_NOR
				sprintf(info, _T("����һ��ֻ�л����˵Ŀ�����, ���Ͻ�ɢ�������"));
				CTraceService::TraceString(info,TraceLevel_Normal);
			#endif			
			pTableFrame->CancelOnlyAndroidDesk();
		}
	}

	PerformSitDownChair();

	__LEAVE_FUNCTION_LOG_CATCH

}

void CAttemperEngineSink::AddKeepSittingAndroid()
{
	__ENTER_FUNCTION_LOG_CATCH

#if _SHOW_MESSAGE_NOR
	CTraceService::TraceString(_T("������վλ��ʼ"), TraceLevel_Normal);
#endif
	DWORD aveBusyDesk   = m_BusyDeskStat.GetAverage();
	if(aveBusyDesk  < m_pGameServiceOption->wAndroidTableCount && m_AndroidAutoQueue.Size() > 0)
	{
		size_t androidCount   =  m_AndroidAutoQueue.Size();
		size_t needUserCount  = (m_pGameServiceOption->wAndroidTableCount -  m_BusyDeskStat.GetAverage()) * m_pGameServiceAttrib->wChairCountMin;
		size_t smallCount     = androidCount < needUserCount ? androidCount : needUserCount;
		size_t needDesk       = smallCount / m_pGameServiceAttrib->wChairCountMin;


		char info[BUFSIZ]={0};
		int totalNullChairCount=0;
		for (size_t count = 0; count < needDesk; ++count)
		{
			CTableFrame * pTableFrame = NULL;
			for (INT_PTR i=0;i<m_pGameServiceOption->wTableCount;i++)
			{
				//��ȡ��������
				pTableFrame         = (m_pTableFrame+i);
				totalNullChairCount = pTableFrame->GetTatolNullChairCount();
				if (pTableFrame->IsGameStarted()==false && totalNullChairCount == m_pGameServiceAttrib->wChairCountMax) //�Ҹ�������
				{
#if _SHOW_MESSAGE_NOR					
					sprintf(info, _T("վλ�����ˣ� Ŀǰ�����ӹ���%i��λ��"), totalNullChairCount);
					CTraceService::TraceString(info,TraceLevel_Normal);
#endif				
					break;
				}
				pTableFrame = NULL;
			}


			if (pTableFrame != NULL)
			{
				tagServerUserData * pUserData = NULL;
				for(BYTE j=0; j< m_pGameServiceAttrib->wChairCountMin; j++)
				{
					//���´���
					WORD wChairID=pTableFrame->GetNullChairID(); 
					if (wChairID!=INVALID_CHAIR)
					{
						IServerUserItem*  pIServerUserItem= m_AndroidAutoQueue.GetFront();
						if (pIServerUserItem != NULL)
						{
							pUserData=pIServerUserItem->GetUserData();
							if (pUserData != NULL  && pUserData->UserScoreInfo.lGold >= m_pGameServiceOption->lRestrictScore)
							{
								pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem,NULL);
#if _SHOW_MESSAGE_NOR						
								sprintf(info, _T("%s վλ��������������....."), pUserData->szAccounts);
								CTraceService::TraceString(info,TraceLevel_Normal);
#endif	
							}
							m_AndroidAutoQueue.PoPFront(); //����,û�б�Ҫ�����;�����Ϣ���û�	
						}							

					}
				}
			}
		}
	}
#if _SHOW_MESSAGE_NOR	
	CTraceService::TraceString(_T("������վλ����"), TraceLevel_Normal);
#endif

	__LEAVE_FUNCTION_LOG_CATCH

}

// ������־ָ��
void CAttemperEngineSink::SetRoomLog(cnd_ILogMgr*	pRoomLog)
{
	m_pRoomLog = pRoomLog;;	// ������־
}

// �����������	Author<cxf>; DateTime<2012-01-17>
void CAttemperEngineSink::SendNewGift()
{
	WORD    wEnumIndex=0;
	DWORD	dwTime=0;
	// ��������15��30��60��120��Χ
	// �������ͣ���������15���ӣ�������ͣ�120���ӣ���Сʱ��
	DWORD	dwMinVal[]={9*60+45, 29*60+45, 59*60+45, 119*60+45};
	DWORD	dwMaxVal[]={11*60-45, 31*60-45, 61*60-45, 121*60-45};
	DWORD	dwMaxTimer[]={600, 1800, 3600, 7200};
	int		iGold[]={200,500,1200,2500};//�������񣬶�Ӧ�Ľ�ҽ��� 200��500��1200��2500
	DWORD	dwRand[]={2,3,4,5,6,7,8};// �����ͱ�-�ȼ�
	int		iGem[]={5,8,12,18,25,36,50};// �����ͱ�-��ʯ
	LONG	lTmpExperience=0l;// ����ֵ
	int		iTmpGrade=0l;	// �ȼ�

	////Test
	//dwMinVal[0]=1*60;dwMinVal[1]=4*60;dwMinVal[2]=7*60;dwMinVal[3]=10*60;
	//dwMaxVal[0]=3*60;dwMaxVal[1]=6*60;dwMaxVal[2]=9*60;dwMaxVal[3]=12*60;
	////end Test

	CMD_GF_NEW_GIFT tagNewGift;
	while (TRUE)
	{
		// ö�������������
		IServerUserItem*  pIServerUserItemSend = m_ServerUserManager.EnumOnLineUser(wEnumIndex++);
		if (NULL == pIServerUserItemSend)
			break;

		if (US_PLAY == pIServerUserItemSend->GetUserStatus()  &&  PERSON==pIServerUserItemSend->GetUserData()->utUserType)
		{
			CTableFrame * pTableFrame = (m_pTableFrame+pIServerUserItemSend->GetTableID());
			if (m_pGameServiceOption->wTableCount <= pIServerUserItemSend->GetTableID())
				continue;
			if (NULL == pTableFrame)
				continue;

			// ��������
			// ���������������5�����µ���ң���ÿ��������Ͳ�����2Сʱ
			if (5 > pIServerUserItemSend->GetUserScore()->dwGrade)
			{
				/*for (int k=0;k<pIServerUserItemSend->GetUserData()->wOnlineGiftCount;k++)
				{
					pIServerUserItemSend->SetOnlineTimeGoldSign(k);
				}*/
				// ������ҽ�����ۻ���Ϸʱ��,(�û���������Ϸ��ʱ��+����ѹ�ȥ��ʱ��=��������Ϸʱ�䣨ʵʱ��ʱ�䣩��
				dwTime=pIServerUserItemSend->GetUserData()->dwOnlineTime + pTableFrame->GetPlayTimeCount();

				for (BYTE i=0; i<4; i++)
				{
					BOOL bResult = pIServerUserItemSend->GetOnlineTimeGolSign(i);
					TRACE("dwMinVal[] %ld,dwMaxVal[] %ld,dwTime %ld --%d\r\n", 
						dwMinVal[i], dwMaxVal[i], dwTime, pIServerUserItemSend->GetOnlineTimeGolSign(i));

					/*if (dwMinVal[i] <= dwTime && dwMaxVal[i] >= dwTime &&
						FALSE==pIServerUserItemSend->GetOnlineTimeGolSign(i))
					{*/
					if (dwMinVal[i] <= dwTime && dwMaxVal[i] >= dwTime &&
						i >= pIServerUserItemSend->GetUserData()->wOnlineGiftCount)
					{
						ZeroMemory(&tagNewGift,sizeof(CMD_GF_NEW_GIFT));
						tagNewGift.wGiftType = 2;
						tagNewGift.iGold = iGold[i];
						tagNewGift.iGiftTime = dwMaxTimer[i];
						SendData(pIServerUserItemSend, MDM_GF_PRESENT, SUB_GF_NEW_GIFT, &tagNewGift, sizeof(CMD_GF_NEW_GIFT));
						
						//д�����ݿ�
						tagNewGift.wUserID = pIServerUserItemSend->GetUserData()->dwUserID;
						tagNewGift.dwClientIP=pIServerUserItemSend->GetClientIP();
						NewGiftData(&tagNewGift);

						pIServerUserItemSend->WriteUserGold(iGold[i]);
						pIServerUserItemSend->GetUserData()->wOnlineGiftCount+=1;

						int		iSurplus = 0;
						for (int j=0;j<4;j++)
						{
							if ((dwTime+21)<dwMaxTimer[j])//21��������
							{
								iSurplus=dwMaxTimer[j];
								break;
							}
						}
						CMD_GF_ONLINE onlineContent;
						onlineContent.dwChairID = pIServerUserItemSend->GetUserData()->wChairID;
						onlineContent.bStart = TRUE;		//�Ƿ�ʼ��ʱ<TRUE:��ʼ��ʱ;False:��ͣ>
						onlineContent.iToDayOnlineTime = iSurplus-dwTime;
						SendData(pIServerUserItemSend,MDM_GF_PRESENT,SUB_GF_ONLINE_TIME,&onlineContent,sizeof(CMD_GF_ONLINE));

						break;
					}
				}
			}

			// �����ͱ�  -����ʵʱ����ʱ�����ж���û������
			// �����ͱ���������8�������£��Ż����ͱ�ʯ
			if (8 >= pIServerUserItemSend->GetUserScore()->dwGrade)
			{
				// ��ǰ�ľ���ֵ��ÿ�ֽ��㶼���ۼӣ�+���ֵľ���ֵ����ֻ�û���㣩=�ܾ���ֵ��ʵʱ��
				// ����ֵ = ��Ϸʱ�� / 10
				lTmpExperience = pIServerUserItemSend->GetUserScore()->lExperience + pTableFrame->GetPlayTimeCount()/10;
				iTmpGrade = CCGlobalData::CaluGrade(lTmpExperience);

				if (iTmpGrade != pIServerUserItemSend->GetUserScore()->dwGrade)
				{
					for (BYTE i=0; i<7; i++)
					{
						BOOL bResult = pIServerUserItemSend->GetUpgradeGemSign(i);
						TRACE("dwRand[i] %ld, dwTime %ld, GetUpgradeGemSign --%d\r\n", 
							dwRand[i], iTmpGrade, pIServerUserItemSend->GetUpgradeGemSign(i));

						if (dwRand[i] == iTmpGrade && FALSE==pIServerUserItemSend->GetUpgradeGemSign(i))
						{
							ZeroMemory(&tagNewGift,sizeof(CMD_GF_NEW_GIFT));
							tagNewGift.wGiftType = 3;
							tagNewGift.iGem = iGem[i];
							tagNewGift.iGrade = pIServerUserItemSend->GetUserScore()->dwGrade+1;
							SendData(pIServerUserItemSend, MDM_GF_PRESENT, SUB_GF_NEW_GIFT, &tagNewGift, sizeof(CMD_GF_NEW_GIFT));

							//д�����ݿ�
							tagNewGift.wUserID=pIServerUserItemSend->GetUserData()->dwUserID;
							tagNewGift.dwClientIP=pIServerUserItemSend->GetClientIP();
							NewGiftData(&tagNewGift);

							pIServerUserItemSend->WriteUserGem(iGem[i]);
							pIServerUserItemSend->SetUpgradeGemSign(i);
							break;
						}
					}//end for
				}
			}
		}//end if
	}//end while
}
//
void CAttemperEngineSink::NewGiftData(CMD_GF_NEW_GIFT *tagNewGift)
{
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GR_ONLINE_GIFT,0,tagNewGift,sizeof(CMD_GF_NEW_GIFT));
}
