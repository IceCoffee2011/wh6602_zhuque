#include "StdAfx.h"
#include "TableFrameSink.h"
#include "TraceService.h"
#include "GameLogic.h"
#include ".\readfixcard.h"//add by Ivan_han 20111213
#include<assert.h>

#define SHOW_BRAINPOWER  0  // 0��ʾ����־�в���ʾ֪�ܷ��ƣ�1��ʾ����־����ʾ֪�ܷ���

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//��ʼģʽ

//�궨��
#define	GOOD_CARD_PERCENT				4									//���Ƹ���

//���캯��
CTableFrameSink::CTableFrameSink()
{
	m_pRoomLog = NULL;

	//��Ϸ����
	m_wBombTime=1;
	m_wBombNums = 0;	//ը������
	m_bLandScore=0;
	m_wFirstUser=rand()%GAME_PLAYER; 
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_bCircuit,0,sizeof(m_bCircuit));
	memset(m_bScoreInfo,0,sizeof(m_bScoreInfo));
	memset(m_bUserBeiShu,0,sizeof(m_bUserBeiShu));
	memset(m_OneBombNums,0,sizeof(m_OneBombNums));
	m_bSnatchCount=0;

	//��������Ϣ
	m_IsMissile=false;

	//������
	m_bBrightCard=false;
	m_bCallScorePhase=CSD_NORMAL;
	ZeroMemory( m_bGiveUpBanker, sizeof( m_bGiveUpBanker ) );
	ZeroMemory( m_bBrightStart, sizeof( m_bBrightStart ) );
	m_cbBrightCardTime = 1;
	m_cbDoubleCount=0;
	ZeroMemory( m_bDoubleUser, sizeof( m_bDoubleUser ) );
	m_bAllowDoubleScore = false;
	ZeroMemory(m_bFinishDoubleUser, sizeof(m_bFinishDoubleUser) );
	ZeroMemory(m_bUserBrightCard, sizeof(m_bUserBrightCard));
	m_cbBrightCardCount=0;

	//������Ϣ
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	memset(m_bUserTrustee, 0, sizeof( m_bUserTrustee ) );

	//�˿���Ϣ
	memset(m_bBackCard,0,sizeof(m_bBackCard));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bOutCardCount,0,sizeof(m_bOutCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));

	//��ʼ������
	//memset(m_UserTaskInfo, 0, sizeof(m_UserTaskInfo));

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	m_wOperateUser = INVALID_CHAIR;									//�����û�		Author<cxf>
	ZeroMemory( m_bGiveUpSnatch, sizeof( m_bGiveUpSnatch ) );		//��������ʶ	Author<cxf>
	//������ʯ

	//��������ӡ��Ϣ
	//CTraceService::TraceString(TEXT("asdfasdfsadfsdf"), TraceLevel_Normal);
	return;
}

//��������
CTableFrameSink::~CTableFrameSink(void)
{
}

//�ӿڲ�ѯ
void * __cdecl CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
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
	m_wBombTime=1;
	m_wBombNums = 0;
	m_bLandScore=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	memset(m_bCircuit,0,sizeof(m_bCircuit));
	memset(m_bScoreInfo,0,sizeof(m_bScoreInfo));
	memset(m_bUserBeiShu,0,sizeof(m_bUserBeiShu));
	m_bSnatchCount=0;

	//������
	m_bBrightCard=false;
	m_bCallScorePhase=CSD_NORMAL;
	ZeroMemory( m_bGiveUpBanker, sizeof( m_bGiveUpBanker ) );
	ZeroMemory( m_bBrightStart, sizeof( m_bBrightStart ) );
	m_cbBrightCardTime = 1;
	m_cbDoubleCount=0;
	ZeroMemory( m_bDoubleUser, sizeof( m_bDoubleUser ) );
	ZeroMemory(m_bUserBrightCard, sizeof(m_bUserBrightCard));
	m_cbBrightCardCount=0;
	ZeroMemory(m_bFinishDoubleUser, sizeof(m_bFinishDoubleUser) );

	//������Ϣ
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	memset(m_bUserTrustee, 0, sizeof( m_bUserTrustee ) );

	//�˿���Ϣ
	memset(m_bBackCard,0,sizeof(m_bBackCard));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bOutCardCount,0,sizeof(m_bOutCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));

	m_wOperateUser = INVALID_CHAIR;									//�����û�		Author<cxf>
	ZeroMemory( m_bGiveUpSnatch, sizeof( m_bGiveUpSnatch ) );		//��������ʶ	Author<cxf>
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
	return true;
}

//���ù̶��ƻ�����ƻ����
int CTableFrameSink::SetFixOrRandCardOrGoodCard()
{
	//////////////////////////////////////////////////////////
	//add by Ivan_han 20111213
	CReadFixCard	readFixCard(54, 3);
	BYTE			byTxtFileHead[3];	// �����ļ���ͷ��ֵ����� byTxtFileHead[1]==1 ʱ�� �����ƺ��������������ƣ�ֱ�ӳ��ƣ�����ʱ��
	memset(byTxtFileHead, 0, 3);
	bool IsFixCard=false;
	int nType=0;
	CString strLclPth, strFlPth;
	GetModuleFileName(NULL, strLclPth.GetBufferSetLength(256), 256);
	strFlPth = strLclPth.Left(strLclPth.ReverseFind('\\')+1)+ TEXT("LandCard.ini");
	if (readFixCard.fnReadFile((LPCTSTR)strFlPth)) 
	{
		readFixCard.fnGetHead(byTxtFileHead);
		nType=byTxtFileHead[1];
		if (1 == byTxtFileHead[1])//ȡ�̶���
		{
			BYTE bCard[54];
			int AllCardNumber=readFixCard.fnGetCards(bCard);  // ��LandCard.ini�ļ�����ȡ�̶�����
			ASSERT(AllCardNumber==54);
			IsFixCard=true;
			//printf("���̶���\n");
			//�ַ��˿�
			for ( WORD i = 0; i < m_wPlayerCount; i++ )
			{
				m_bCardCount[ i ] = 17;
				CopyMemory( &m_bHandCardData[ i ], &bCard[17*i], sizeof( BYTE ) * 17 );
			}
			CopyMemory( m_bBackCard, &bCard[ 51 ], sizeof( m_bBackCard ) );
			m_GameLogic.SortCardList( m_bBackCard, 3, ST_ORDER );
		}
		//add by Ivan_han 20120203
		else if (2 == byTxtFileHead[1]||3 == byTxtFileHead[1])//����ֻ����һ����
		{
			this->SetGoodCard1();
		}
		//else if (3 == byTxtFileHead[1])//����ֻ����������
		//{
		//     this->SetGoodCard2();

		//}
		else if(4 == byTxtFileHead[1])//�ۺ�0,2,3���ַ��Ʋ��Եķ��Ʋ���
		{
			int nret=0; 
			//for(;;)
			//{
			//nret=rand()%4;
			nret=CGameLogic::GetRand(0,5);//1/3�ļ��ʷ�������
			//	if(nret!=1)
			//		break;
			//}
			//if(nret==0)this->SetRandCard();
			if(nret==2||nret==3)
				this->SetGoodCard1();
			else
				this->SetRandCard();
			//if(nret==3)this->SetGoodCard2();
		}
		else
		{
			nType=0;
			//printf("�������\n");
		}
	}
	else
	{
		//printf("��ȡini�ļ�ʧ�ܣ��������\n");
	}
	if(nType==0)
	{
		this->SetRandCard();
	}
	return nType;
}

//�����
void CTableFrameSink::SetRandCard()
{
	////�����˿� 
	BYTE bRandCard[ 54 ];
	memset(bRandCard, 0, sizeof(bRandCard));
	m_GameLogic.RandCardList( bRandCard, sizeof( bRandCard ) / sizeof( bRandCard[ 0 ] ) );

	//�ַ��˿�
	for ( WORD i = 0; i < m_wPlayerCount; i++ )
	{
		m_bCardCount[ i ] = 17;
		CopyMemory( &m_bHandCardData[ i ], &bRandCard[ i * m_bCardCount[ i ] ], sizeof( BYTE ) * m_bCardCount[ i ] );
	}
	CopyMemory( m_bBackCard, &bRandCard[ 51 ], sizeof( m_bBackCard ) );
	m_GameLogic.SortCardList( m_bBackCard, 3, ST_ORDER );

#if SHOW_BRAINPOWER
	if(m_pRoomLog)
		m_pRoomLog->AddMsg(0, 0, TRUE, "�������");
#endif
}

//����ֻ����һ����
void CTableFrameSink::SetGoodCard1()
{
	//���ͺ���
	vector<BYTE>::iterator   Iter;
	vector<BYTE> the_vector;
	the_vector.clear();

	for( int i = 0; i < 54; i++ )
	{
		the_vector.push_back(CGameLogic::m_cbCardData[i]);
	}


	srand( (unsigned)time( NULL ) );
	vector <BYTE> V[3];
	vector<BYTE> Vback;

	//��˳�ӻ�ɫҪ����
	vector<BYTE> vec;
	int nSize=the_vector.size();
	for(int i=0;i<nSize;i++)
	{
		int Index=CGameLogic::GetRand(0,the_vector.size()-1);
		vec.push_back(the_vector[Index]);
		the_vector.erase(the_vector.begin()+Index);
	}
	assert(nSize==vec.size() && the_vector.size()==0 && "����ʧ����ʾ");
	the_vector.swap(vec);
	assert(nSize==the_vector.size() && "����ʧ����ʾ");

	vec=the_vector;
	std::sort(vec.begin(),vec.end());

	int nRCount=0;
	for(;;nRCount++)
	{
		int PeekFunIndex=CGameLogic::GetRand(0,CountArray(CGameLogic::PeekFun)-1);
		//int PeekFunIndex=CGameLogic::GetRand(0,13);
		vector <BYTE> v0;
		bool bret=CGameLogic::PeekFun[PeekFunIndex](the_vector,V[0]);
		if(bret)
		{
			break;
		}
	}
	for(int i=0;i<17;i++)
	{
		int Index=CGameLogic::GetRand(0,the_vector.size()-1);
		V[1].push_back(the_vector[Index]);
		the_vector.erase(the_vector.begin()+Index);
	}

	nRCount=0;
	bool bsucback=CGameLogic::PeekRandBackCard(the_vector,Vback);
	V[2].insert(V[2].end(),the_vector.begin(),the_vector.end());
	//V[2].swap(the_vector);

	//assert(V[0].size()==V[1].size()==V[2].size()==17);
	assert(V[0].size()==17);
	assert(V[1].size()==17);
	assert(V[2].size()==17);

	//��3�������е��˿˷ָ�3������
	for(int i=0;i<3;i++)
		m_bCardCount[i]=17;
	int FirstChairId=CGameLogic::GetRand(0,2);
	int SecondChairId=0;
	for(;;nRCount++)
	{
		SecondChairId=CGameLogic::GetRand(0,2);
		if(SecondChairId!=FirstChairId)
		{
			break;
		}
	}
	int LastChairId=3-FirstChairId-SecondChairId;

	memcpy(&m_bHandCardData[FirstChairId][0],&V[0][0],17);
	memcpy(&m_bHandCardData[SecondChairId][0],&V[1][0],17);
	memcpy(&m_bHandCardData[LastChairId][0],&V[2][0],17);
	memcpy(&m_bBackCard[0],&Vback[0],3);
	m_GameLogic.SortCardList( m_bBackCard, 3, ST_ORDER );

	#if SHOW_BRAINPOWER
		if(m_pRoomLog)
			m_pRoomLog->AddMsg(0, 0, TRUE, "��������");
	#endif

	//�����Ƶ�˳����һ��ʱ���ж�����ơ�ȱ�ٵ���
	vector<BYTE> vec1;
	vec1.insert(vec1.end(),V[0].begin(),V[0].end());
	vec1.insert(vec1.end(),V[1].begin(),V[1].end());
	vec1.insert(vec1.end(),V[2].begin(),V[2].end());
	vec1.insert(vec1.end(),Vback.begin(),Vback.end());
	std::sort(vec1.begin(),vec1.end());
	bool b=equal(vec.begin(),vec.end(), vec1.begin());
	char sz[100]={0};
	if(b)
	{
		sprintf(sz,"vec==vec1,FirstChairId=%d",FirstChairId);
	}
	else
	{
		sprintf(sz,"vec!=vec1,FirstChairId=%d",FirstChairId);
	}
	#if SHOW_BRAINPOWER
		if(m_pRoomLog)
			m_pRoomLog->AddMsg(0, 0, TRUE, sz);
	#endif

	for(int j=0;j<3;j++)
		for( int i = 0; i < 17; i++ )
		{
			sprintf(sz,"V[%d][%d]=%d(%s)",j,i,V[j][i],CGameLogic::GetCardName(V[j][i]));

			#if SHOW_BRAINPOWER
				if(m_pRoomLog)
					m_pRoomLog->AddMsg(0, 0, TRUE, sz);
			#endif
		}
		for( int i = 0; i < 3; i++ )
		{
			sprintf(sz,"Vback[%d]=%d(%s)",i,Vback[i],CGameLogic::GetCardName(Vback[i]));

			#if SHOW_BRAINPOWER
				if(m_pRoomLog)
					m_pRoomLog->AddMsg(0, 0, TRUE, sz);
			#endif
		}
}

////����ֻ����������
//void CTableFrameSink::SetGoodCard2()
//{
//	//���ͺ���
//	vector<BYTE>::iterator   Iter;
//	vector<BYTE> the_vector;
//	the_vector.clear();
//
//	for( int i = 0; i < 54; i++ )
//	{
//		the_vector.push_back(CGameLogic::m_cbCardData[i]);
//	}
//
//
//	srand( (unsigned)time( NULL ) );
//	vector <BYTE> V[3];
//	vector<BYTE> Vback;
//
//	int nRCount=0;
//	//int nMaxRcount=10000;
//	int nMaxRcount=10;
//	for(int i=0;i<2;i++)
//	{
//		nRCount=0;
//		for(;;nRCount++)
//		{
//			int PeekFunIndex=CGameLogic::GetRand(0,1);
//			//vector <BYTE> v0;
//			bool bret=CGameLogic::PeekFun[PeekFunIndex](the_vector,V[i]);
//			if(bret)
//			{
//				break;
//			}
//			if(nRCount>=nMaxRcount)
//			{
//				V[0].clear();
//				V[1].clear();
//				the_vector.clear();
//				for( int j = 0; j < 54; j++ )
//				{
//					the_vector.push_back(CGameLogic::m_cbCardData[j]);
//				}
//				i=0;
//				nRCount=0;
//				break;//break;
//			}
//		}
//	}
//
//	nRCount=0;
//	bool bsucback=CGameLogic::PeekRandBackCard(the_vector,Vback);
//	V[2].insert(V[2].end(),the_vector.begin(),the_vector.end());
//	//V[2].swap(the_vector);
//
//	//assert(V[0].size()==V[1].size()==V[2].size()==17);
//	assert(V[0].size()==17);
//	assert(V[1].size()==17);
//	assert(V[2].size()==17);
//
//	//��3�������е��˿˷ָ�3������
//	for(int i=0;i<3;i++)
//		m_bCardCount[i]=17;
//	int FirstChairId=CGameLogic::GetRand(0,2);
//	int SecondChairId=0;
//	for(;;nRCount++)
//	{
//		SecondChairId=CGameLogic::GetRand(0,2);
//		if(SecondChairId!=FirstChairId)
//		{
//			break;
//		}
//	}
//	int LastChairId=3-FirstChairId-SecondChairId;
//
//	memcpy(&m_bHandCardData[FirstChairId][0],&V[0][0],17);
//	memcpy(&m_bHandCardData[SecondChairId][0],&V[1][0],17);
//	memcpy(&m_bHandCardData[LastChairId][0],&V[2][0],17);
//	memcpy(&m_bBackCard[0],&Vback[0],3);
//	m_GameLogic.SortCardList( m_bBackCard, 3, ST_ORDER );
//}

////���ù̶��ƻ������
//bool CTableFrameSink::SetFixOrRandCard()
//{
//	//////////////////////////////////////////////////////////
//	//add by Ivan_han 20111213
//	CReadFixCard	readFixCard(54, 3);
//	BYTE			byTxtFileHead[3];	// �����ļ���ͷ��ֵ����� byTxtFileHead[1]==1 ʱ�� �����ƺ��������������ƣ�ֱ�ӳ��ƣ�����ʱ��
//	memset(byTxtFileHead, 0, 3);
//	bool IsFixCard=false;
//	CString strLclPth, strFlPth;
//	GetModuleFileName(NULL, strLclPth.GetBufferSetLength(256), 256);
//	strFlPth = strLclPth.Left(strLclPth.ReverseFind('\\')+1)+ TEXT("LandCard.ini");
//	if (readFixCard.fnReadFile((LPCTSTR)strFlPth)) 
//	{
//		readFixCard.fnGetHead(byTxtFileHead);
//		if (1 == byTxtFileHead[1])
//		{
//			BYTE bCard[54];
//			int AllCardNumber=readFixCard.fnGetCards(bCard);  // ��LandCard.ini�ļ�����ȡ�̶�����
//			ASSERT(AllCardNumber==54);
//			IsFixCard=true;
//			//printf("���̶���\n");
//			//�ַ��˿�
//			for ( WORD i = 0; i < m_wPlayerCount; i++ )
//			{
//				m_bCardCount[ i ] = 17;
//				CopyMemory( &m_bHandCardData[ i ], &bCard[17*i], sizeof( BYTE ) * 17 );
//			}
//			CopyMemory( m_bBackCard, &bCard[ 51 ], sizeof( m_bBackCard ) );
//			m_GameLogic.SortCardList( m_bBackCard, 3, ST_ORDER );
//		}
//		else
//		{
//			//printf("�������\n");
//		}
//	}
//	else
//	{
//		//printf("��ȡini�ļ�ʧ�ܣ��������\n");
//	}
//	if(!IsFixCard)
//	{
//		////�����˿� 
//		BYTE bRandCard[ 54 ];
//		memset(bRandCard, 0, sizeof(bRandCard));
//		m_GameLogic.RandCardList( bRandCard, sizeof( bRandCard ) / sizeof( bRandCard[ 0 ] ) );
//
//		//�ַ��˿�
//		for ( WORD i = 0; i < m_wPlayerCount; i++ )
//		{
//			m_bCardCount[ i ] = 17;
//			CopyMemory( &m_bHandCardData[ i ], &bRandCard[ i * m_bCardCount[ i ] ], sizeof( BYTE ) * m_bCardCount[ i ] );
//		}
//		CopyMemory( m_bBackCard, &bRandCard[ 51 ], sizeof( m_bBackCard ) );
//		m_GameLogic.SortCardList( m_bBackCard, 3, ST_ORDER );
//	}
//	return IsFixCard;
//}

//��Ϸ��ʼ
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	//osc add
	memset(m_tagTask, 0, sizeof(m_tagTask));

	//��һ�ֿ�ʼ֮ǰ����ʼ��������Ϣ
	//for (int i=0; i<GAME_PLAYER; i++)
	//{
	//	m_UserTaskInfo[i].bIsOutBombWin = false;	//�Ƿ���ը��Ӯ
	//	m_UserTaskInfo[i].bIsOutMissile = false;	//�Ƿ������Ӯ
	//	m_UserTaskInfo[i].bRegBombNums = 0;			//һ�ֵ��д����ը������
	//}
	//
	//������
	ZeroMemory( m_bGiveUpBanker, sizeof( m_bGiveUpBanker ) );
	ZeroMemory( m_bGiveUpSnatch, sizeof( m_bGiveUpSnatch ) );		//��������ʶ	Author<cxf>
	memset(m_bUserTrustee, 0, sizeof( m_bUserTrustee ) );

	//����״̬
	m_pITableFrame->SetGameStatus(GS_WK_SCORE);//���ýз�״̬

	/* Del:<cxf>; description:<ɾ����������>
	BYTE cbSendGoodCard = rand () % GOOD_CARD_PERCENT ;
	BYTE cbAndroidUserCount = 0 ;
	WORD wAndroidUser[ GAME_PLAYER ] = {0};*/

	//��Ϸ��ʼ���з�Ϊ��
	m_bLandScore = 0;
	memset(m_bCircuit,0,sizeof(m_bCircuit));
	m_bSnatchCount=0;

	//add by Ivan_han
	m_vecChairIDOffLine.clear();
	this->SetFixOrRandCardOrGoodCard();

	//�����û�
	if ( m_wCurrentUser == INVALID_CHAIR ) 
	{
		m_wCurrentUser = m_wFirstUser;
	}
	//�����û�
	m_wOperateUser = m_wCurrentUser;					//Author<cxf>
	//�����˿�
	CMD_S_SendAllCard SendAllCard ;
	SendAllCard.wCurrentUser = m_wCurrentUser ;

	//������Ϣ
	CopyMemory(SendAllCard.bUserBrightCard, m_bUserBrightCard, sizeof(SendAllCard.bUserBrightCard));

	//�����˿�
	CopyMemory( SendAllCard.bCardData, m_bHandCardData, sizeof( m_bHandCardData ) ) ;
	//�����˿�
	CopyMemory( SendAllCard.bBackCardData, m_bBackCard, 3 ) ;
	for ( WORD i = 0; i < m_wPlayerCount; i++ )
	{
		m_pITableFrame->SendTableData( i, SUB_S_SEND_CARD, &SendAllCard, sizeof( SendAllCard ) );
		m_pITableFrame->SendLookonData( i, SUB_S_SEND_CARD, &SendAllCard, sizeof( SendAllCard ) );
	}

	//�����˿�
	for ( WORD i = 0; i < m_wPlayerCount; i++ )
	{
		m_GameLogic.SortCardList( m_bHandCardData[ i ], m_bCardCount[ i ], ST_ORDER );
	}

	return true;
}

//add by Ivan_han
//�û�����
bool __cdecl CTableFrameSink::OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem)
{ 
	if(pIServerUserItem==NULL)
		return false;
	m_vecChairIDOffLine.push_back(wChairID);
	//�����3�����Ӷ��ߣ�����Ϸ����
	if(m_vecChairIDOffLine.size()==3)
	{
		m_pITableFrame->DismissGame();
		return true;
	}
	if(m_wCurrentUser==wChairID)
	{
		int a=0;
		this->OnAutoPassOrOutCard();
	}

	//if (m_pITableFrame->GetGameStatus()==GS_WK_SCORE)
	//{
	//	if (m_wCurrentUser==wChairID)
	//	{
	//		this->OnUserLandScore(wChairID,0);
	//	}
	//}
	//else if (m_pITableFrame->GetGameStatus()==GS_WK_SNATCH)
	//{
	//	if (m_wCurrentUser==wChairID)
	//	{
	//		this->OnUserLandScore(wChairID,1);
	//	}
	//}
	//else if (m_pITableFrame->GetGameStatus()==GS_WK_PLAYING)
	//{
	//	if(m_wCurrentUser==wChairID)
	//		this->OnAutoPassOrOutCard();
	//}

	////�ж��������������Ƿ�����ƣ������û���ƣ��������ұ����һ����
	//if(m_wCurrentUser==wChairID && m_wCurrentUser!=INVALID_CHAIR)
	//{
	//	if(m_wCurrentUser!=m_wTurnWiner)
	//	{	
	//	    bool bFlag=this->OnUserPassCard(wChairID);
	//	}
	//	else
	//	{
	//		
	//		BYTE bCardData[1]={0};
	//		BYTE bCardCount=1;
	//           m_GameLogic.SortCardList(m_bHandCardData[wChairID],m_bCardCount[wChairID],ST_ORDER);
	//           bCardData[0]=m_bHandCardData[wChairID][0];
	//		m_GameLogic.RemoveCard(bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID]);
	//		this->OnUserOutCard(wChairID,bCardData,bCardCount);
	//	}
	//}


	/*
	//��Ϸ���
	for ( WORD wUserIdx = 0; wUserIdx < GAME_PLAYER; ++wUserIdx )
	{
	IServerUserItem *pIServerUserItem= m_pITableFrame->GetServerUserItem( wUserIdx );
	tagServerUserData * pUserData=pIServerUserItem->GetUserData();
	//if (pUserData->cbUserStatus!=US_PLAY)
	if(pUserData!=NULL)
	{
	//�û�״̬����
	static char *szUserStatus[7]={"US_NULLû��״̬","US_FREEվ��״̬","US_SIT����״̬","US_READYͬ��״̬","US_LOOKON�Թ�״̬","US_PLAY��Ϸ״̬","US_OFFLINE����״̬"};
	//��������ӡ����
	char chData[128]={0};
	sprintf(chData, "wUserIdx=%d,%s���û�״̬=%d(%s)", wUserIdx,pUserData->szNickName,pUserData->cbUserStatus,szUserStatus[pUserData->cbUserStatus]);
	}
	}
	*/
	return true; 

}

//add by Ivan_han
//�û�����
bool __cdecl CTableFrameSink::OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	if(pIServerUserItem==NULL)
		return false;
	m_vecChairIDOffLine.erase(remove(m_vecChairIDOffLine.begin(),m_vecChairIDOffLine.end(), wChairID),m_vecChairIDOffLine.end());

	//CTraceService::TraceString("�û�����OnActionUserReConnect", TraceLevel_Normal);
	////��Ϸ���
	//for ( WORD wUserIdx = 0; wUserIdx < GAME_PLAYER; ++wUserIdx )
	//{
	//	IServerUserItem *pIServerUserItem= m_pITableFrame->GetServerUserItem( wUserIdx );
	//	tagServerUserData * pUserData=pIServerUserItem->GetUserData();
	//	//if (pUserData->cbUserStatus!=US_PLAY)
	//	if(pUserData!=NULL)
	//	{
	//		//�û�״̬����
	//		static char *szUserStatus[7]={"US_NULLû��״̬","US_FREEվ��״̬","US_SIT����״̬","US_READYͬ��״̬","US_LOOKON�Թ�״̬","US_PLAY��Ϸ״̬","US_OFFLINE����״̬"};
	//		//��������ӡ����
	//		char chData[128]={0};
	//		sprintf(chData, "wUserIdx=%d,%s���û�״̬=%d(%s)", wUserIdx,pUserData->szNickName,pUserData->cbUserStatus,szUserStatus[pUserData->cbUserStatus]);
	//		CTraceService::TraceString(chData, TraceLevel_Normal);
	//	}
	//}
	return true; 
}

//��Ϸ�������
bool __cdecl CTableFrameSink::OnEventGameNormalEnd(WORD wChairID,bool bLandWin/*, IServerUserItem * pIServerUserItem, BYTE cbReason*/)
{
	//�������
	CMD_S_GameEnd GameEnd;
	memset(&GameEnd,0,sizeof(GameEnd));

	//ʣ���˿ˣ�����bHandCardData�ֶκ�bCardData�ֶε�ֵ
	BYTE bCardPos=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		CopyMemory(&GameEnd.bHandCardData[i],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
		GameEnd.bCardCount[i]=m_bCardCount[i];
		CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
		bCardPos+=m_bCardCount[i];
	}

	////����û���˱�ʾ�ǵ���Ӯ��
	//bool bLandWin=(m_bCardCount[m_wBankerUser]==0)?true:false;



	//�����жϣ�����IsChunTian�ֶκ�IsFanChun�ֶε�ֵ��������ұ���m_bUserBeiShu
	bool bchuntian=false;
	bool bfanchun=false;
	if ( wChairID == m_wBankerUser )//ֻ�ǵ��������ƣ����죩
	{
		WORD wUser1 = ( m_wBankerUser + 1 ) % GAME_PLAYER;
		WORD wUser2 = ( m_wBankerUser + 2 ) % GAME_PLAYER;
		if ( ( m_bOutCardCount[ wUser1 ] == 0 ) && ( m_bOutCardCount[ wUser2 ] == 0 ) ) 
		{
			//cxf del 12-01-04 wDrawTimes *= 2;
			bchuntian=true;
			//GameEnd.nSpringTime*=2;
			for (int i=0;i<GAME_PLAYER;++i)
			{
				m_bUserBeiShu[i]*=2;
			}

		}
		GameEnd.IsChunTian=bchuntian;
	}
	else//����ֻ����һ���ƣ�������
	{
		if ( m_bOutCardCount[ m_wBankerUser ] == 1 ) 
		{
			//cxf del 12-01-04 wDrawTimes *= 2;
			bfanchun=true;
			//GameEnd.nSpringTime*=2;
			for (int i=0;i<GAME_PLAYER;++i)
			{
				m_bUserBeiShu[i]*=2;
			}
		}
		GameEnd.IsFanChun=bfanchun;
	}

	//��������=��������*��λ����
	LONGLONG lBaseScore= m_bUserBeiShu[m_wBankerUser]*(WORD)m_pGameServiceOption->lCellScore;

	//������֣�����lGameScore�ֶε�ֵ
	for (int i=0; i<GAME_PLAYER; ++i)
	{
		if (i==m_wBankerUser)//����Ӯ��������ֵ�˫��
		{
			GameEnd.lGameScore[i]+=(lBaseScore*((bLandWin==true)?2:-2));
		}
		else//ũ��Ӯ���������
		{
			GameEnd.lGameScore[i]+=(lBaseScore*((bLandWin==true)?-1:1));
		}
	}

	LONG lRestrictScore = m_pGameServiceOption->lRestrictScore; //������Ʒ���
	LONG lBankerScore = GameEnd.lGameScore[m_wBankerUser]; //��������

	//������ƣ�����lGameScore�ֶε�ֵ
	if (lBankerScore > lRestrictScore*2)//����Ӯ�����ޣ�ũ���������
	{
		GameEnd.lGameScore[m_wBankerUser] = lRestrictScore*2;
		GameEnd.lGameScore[(m_wBankerUser+1)%m_wPlayerCount] = -lRestrictScore;
		GameEnd.lGameScore[(m_wBankerUser+2)%m_wPlayerCount] = -lRestrictScore;
	}
	else if (lBankerScore < -lRestrictScore)//����������ޣ�ũ��Ӯ������
	{
		GameEnd.lGameScore[m_wBankerUser] = -lRestrictScore;
		GameEnd.lGameScore[(m_wBankerUser+1)%m_wPlayerCount] = lRestrictScore/2;
		GameEnd.lGameScore[(m_wBankerUser+2)%m_wPlayerCount] = lRestrictScore/2;
	}



	//ͳ�ƻ���
	LONGLONG lScore=0;
	LONGLONG lRevenue=0;
	enScoreKind ScoreKind;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//����˰�գ�����ѣ�������lGameTax�ֶε�ֵ������lGameScore�ֶε�ֵ
		lRevenue=0;
		if ( GameEnd.lGameScore[i]>0L)//Ӯ�ҿ۳������
		{
			lRevenue= LONGLONG((GameEnd.lGameScore[i]*(m_pGameServiceOption->wRevenue/100.0)));
			//GameEnd.lGameTax+=lRevenue;
			GameEnd.lGameTax[i]=lRevenue;
			GameEnd.lGameScore[i]=GameEnd.lGameScore[i]-lRevenue;
		}
		else
		{
			GameEnd.lGameTax[i]=0;
		}
		lScore = GameEnd.lGameScore[i];
		ScoreKind = lScore > 0 ? enScoreKind_Win : enScoreKind_Lost;

		//�����жϣ����������ʶλ
		if(GameEnd.lGameScore[i]>0)
		{//��ʤ
			m_tagTask[i] |=CT_TASK_WIN;
		}

		//���12���ϱ��������ƣ�
		if (m_bUserBeiShu[i]>=12)
		{
			m_tagTask[i] |=CT_TASK_BEI_12;
		}

		//���24�����ϲ������ƣ�
		if (m_bUserBeiShu[i]>=24)
		{
			m_tagTask[i] |=CT_TASK_BEI_24;
		}

		//���48�����ϲ������ƣ�
		if (m_bUserBeiShu[i]>=48)
		{
			m_tagTask[i] |=CT_TASK_BEI_48;
		}

		//���96�����ϲ������ƣ�
		if (m_bUserBeiShu[i]>=96)
		{
			m_tagTask[i] |=CT_TASK_BEI_96;
		}
		//����Ϊ�㣻
		if (0==m_bCardCount[i])
		{
			m_tagTask[i] |=CT_TASK_CARD_ZERO;
		}
		//���ý�����ʯiLargessCount�ֶ�
		GameEnd.iLargessCount[i] = OnUserTaskJudgement(i,m_wBankerUser);
		//�����ռ������Ϸ����lGameScore�ֶε�ֵ������ѣ��������ͣ���Ӯ�����д�����ݿ�
		m_pITableFrame->WriteUserScore(i, lScore, lRevenue, ScoreKind );	
	}
	//�������Ϣ��������Ϸ����GameBeiShu�ֶκͷ������RoomScore�ֶε�ֵ
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		GameEnd.GameBeiShu[i]=m_bUserBeiShu[i];
		GameEnd.RoomScore[i]=(m_pGameServiceOption->lCellScore)*m_bLandScore;
	}
	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

	//�л��û�
	m_wFirstUser=wChairID;

	//������Ϸ
	m_pITableFrame->ConcludeGame();

	memset(m_bUserTrustee, 0, sizeof( m_bUserTrustee ) );

	return true;
}

//��Ϸ���� 
bool __cdecl CTableFrameSink::OnEventGameEnd( WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason )
{
	switch ( cbReason )
	{  
	case GER_DISMISS:		//��Ϸ��ɢ
		{
			//Ч�����
			//ASSERT( pIServerUserItem != NULL );//by Ivan_han
			//ASSERT( wChairID < m_wPlayerCount );//by Ivan_han

			//��������
			CMD_S_GameEnd GameEnd;
			memset( &GameEnd, 0, sizeof( GameEnd ) );

			//ʣ���˿�
			BYTE bCardPos = 0;
			for ( WORD i = 0; i < m_wPlayerCount; i++ )
			{
				CopyMemory(&GameEnd.bHandCardData[i],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				GameEnd.bCardCount[ i ] = m_bCardCount[ i ];
				CopyMemory( &GameEnd.bCardData[ bCardPos ], m_bHandCardData[ i ], m_bCardCount[ i ] * sizeof( BYTE ) );
				bCardPos += m_bCardCount[ i ];
			}                     

			//��ʼ������
			//memset(m_UserTaskInfo, 0, sizeof(m_UserTaskInfo));
			memset(m_bUserTrustee, 0, sizeof( m_bUserTrustee ) );

			//������Ϣ
			m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof( GameEnd ) );
			m_pITableFrame->SendLookonData( INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof( GameEnd ) );

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			return true;
		}
	case GER_NORMAL:		//�������
		{
			//if(m_vecChairIDOffLine.size()==0)
			//	printf("m_vecChairIDOffLine.size()==0\n");
			//else if(m_vecChairIDOffLine.size()==1||m_vecChairIDOffLine.size()==2)
			//	//printf("%d\n",m_vecChairIDOffLine.back());
			//	printf("%d\n",m_vecChairIDOffLine[0]);
			//else if(m_vecChairIDOffLine.size()>2)
			//	printf("m_vecChairIDOffLine.size()>2\n");
			//char sz[100]={0};
			//sprintf(sz,"m_vecChairIDOffLine.size()=%d",m_vecChairIDOffLine.size());
			//CTraceService::TraceString(sz, TraceLevel_Normal);

			//add by Ivan_han
			//����û���˱�ʾ�ǵ���Ӯ��
			bool bLandWin=(m_bCardCount[m_wBankerUser]==0)?true:false;
			if(m_vecChairIDOffLine.size()==0)
				return this->OnEventGameNormalEnd(wChairID,bLandWin);
			if(m_vecChairIDOffLine.size()==1||m_vecChairIDOffLine.size()==2)
			{
				//��һ�����ߵ����     
				WORD wChairIDOffLine=m_vecChairIDOffLine[0];
				//�������ߣ���ũ��ʤ��������������
				if(wChairIDOffLine==m_wBankerUser)
					return this->OnEventGameNormalEnd(wChairID,false);

				//δ����ũ��
				WORD wChairIDOnLine=3-wChairIDOffLine-m_wBankerUser;
				CMD_S_GameEnd GameEnd;
				memset(&GameEnd,0,sizeof(GameEnd));

				//ʣ���˿ˣ�����bHandCardData�ֶκ�bCardData�ֶε�ֵ
				BYTE bCardPos=0;
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					CopyMemory(&GameEnd.bHandCardData[i],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
					GameEnd.bCardCount[i]=m_bCardCount[i];
					CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
					bCardPos+=m_bCardCount[i];
				}

				//�����жϣ�����IsChunTian�ֶκ�IsFanChun�ֶε�ֵ��������ұ���m_bUserBeiShu
				bool bchuntian=false;
				bool bfanchun=false;
				if ( wChairID == m_wBankerUser )//ֻ�ǵ��������ƣ����죩
				{
					WORD wUser1 = ( m_wBankerUser + 1 ) % GAME_PLAYER;
					WORD wUser2 = ( m_wBankerUser + 2 ) % GAME_PLAYER;
					if ( ( m_bOutCardCount[ wUser1 ] == 0 ) && ( m_bOutCardCount[ wUser2 ] == 0 ) ) 
					{
						//cxf del 12-01-04 wDrawTimes *= 2;
						bchuntian=true;
						//GameEnd.nSpringTime*=2;
						for (int i=0;i<GAME_PLAYER;++i)
						{
							m_bUserBeiShu[i]*=2;
						}

					}
					GameEnd.IsChunTian=bchuntian;
				}
				else//����ֻ����һ���ƣ�������
				{
					if ( m_bOutCardCount[ m_wBankerUser ] == 1 ) 
					{
						//cxf del 12-01-04 wDrawTimes *= 2;
						bfanchun=true;
						//GameEnd.nSpringTime*=2;
						for (int i=0;i<GAME_PLAYER;++i)
						{
							m_bUserBeiShu[i]*=2;
						}
					}
					GameEnd.IsFanChun=bfanchun;
				}

				//��������=��������*��λ����
				LONGLONG lBaseScore= m_bUserBeiShu[m_wBankerUser]*(WORD)m_pGameServiceOption->lCellScore;
				//������֣�����lGameScore�ֶε�ֵ
				//ũ����ߣ�����Ӯ
				if(wChairIDOffLine!=m_wBankerUser && bLandWin)
				{
					GameEnd.lGameScore[m_wBankerUser]+=(lBaseScore*2);
					GameEnd.lGameScore[wChairIDOffLine]-=(lBaseScore*2);
				}
				//ũ����ߣ�ũ��Ӯ
				if(wChairIDOffLine!=m_wBankerUser && !bLandWin)
				{
					GameEnd.lGameScore[wChairIDOnLine]+=(lBaseScore);
					GameEnd.lGameScore[wChairIDOffLine]-=(lBaseScore);
				}
				//if(GameEnd.lGameScore[wChairIDOffLine]<0)
				//	GameEnd.lGameScore[wChairIDOffLine]=0;
				//��ʱ������������ƣ���ʱ������lGameScore�ֶε�ֵ

				//ͳ�ƻ���
				LONGLONG lScore=0;
				LONGLONG lRevenue=0;
				enScoreKind ScoreKind;
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					//����˰�գ�����ѣ�������lGameTax�ֶε�ֵ������lGameScore�ֶε�ֵ
					lRevenue=0;
					if ( GameEnd.lGameScore[i]>0L)//Ӯ�ҿ۳������
					{
						lRevenue= LONGLONG((GameEnd.lGameScore[i]*(m_pGameServiceOption->wRevenue/100.0)));
						//GameEnd.lGameTax+=lRevenue;
						GameEnd.lGameTax[i]=lRevenue;
						GameEnd.lGameScore[i]=GameEnd.lGameScore[i]-lRevenue;
					}
					else
					{
						GameEnd.lGameTax[i]=0;
					}
					lScore = GameEnd.lGameScore[i];
					ScoreKind = lScore > 0 ? enScoreKind_Win : enScoreKind_Lost;

					//�����жϣ����������ʶλ
					if(GameEnd.lGameScore[i]>0)
					{//��ʤ
						m_tagTask[i] |=CT_TASK_WIN;
					}

					//���12���ϱ��������ƣ�
					if (m_bUserBeiShu[i]>=12)
					{
						m_tagTask[i] |=CT_TASK_BEI_12;
					}

					//���24�����ϲ������ƣ�
					if (m_bUserBeiShu[i]>=24)
					{
						m_tagTask[i] |=CT_TASK_BEI_24;
					}

					//���48�����ϲ������ƣ�
					if (m_bUserBeiShu[i]>=48)
					{
						m_tagTask[i] |=CT_TASK_BEI_48;
					}

					//���96�����ϲ������ƣ�
					if (m_bUserBeiShu[i]>=96)
					{
						m_tagTask[i] |=CT_TASK_BEI_96;
					}
					//����Ϊ�㣻
					if (0==m_bCardCount[i])
					{
						m_tagTask[i] |=CT_TASK_CARD_ZERO;
					}
					//���ý�����ʯiLargessCount�ֶ�
					GameEnd.iLargessCount[i] = OnUserTaskJudgement(i,m_wBankerUser);
					//�����ռ������Ϸ����lGameScore�ֶε�ֵ������ѣ��������ͣ���Ӯ�����д�����ݿ�
					m_pITableFrame->WriteUserScore(i, lScore, lRevenue, ScoreKind );	
				}
				//�������Ϣ��������Ϸ����GameBeiShu�ֶκͷ������RoomScore�ֶε�ֵ
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					GameEnd.GameBeiShu[i]=m_bUserBeiShu[i];
					GameEnd.RoomScore[i]=(m_pGameServiceOption->lCellScore)*m_bLandScore;
				}
				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

				//�л��û�
				m_wFirstUser=wChairID;

				//������Ϸ
				m_pITableFrame->ConcludeGame();

				memset(m_bUserTrustee, 0, sizeof( m_bUserTrustee ) );

				return true;
			}
			ASSERT(FALSE);
			return true;
		}
	case GER_USER_LEFT:		//�û�ǿ��
		{
			//Ч�����
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount);

			//��������
			CMD_S_GameEnd GameEnd;
			memset(&GameEnd,0,sizeof(GameEnd));

			//cxf �����������
			LONG lRestrictScore = m_pGameServiceOption->lRestrictScore; //������Ʒ���
			//ǿ���û�������������
			GameEnd.lGameScore[wChairID] = -lRestrictScore;//ǿ���û�
			GameEnd.lGameScore[(wChairID+1)%GAME_PLAYER] = abs(lRestrictScore/2);//���Ӯ��ǿ����ҷ���/2
			GameEnd.lGameScore[(wChairID+2)%GAME_PLAYER] = abs(lRestrictScore/2);//���Ӯ��ǿ����ҷ���/2
			GameEnd.IsUserLeft[wChairID]=true;
			GameEnd.IsUserLeft[(wChairID+1)%GAME_PLAYER]=false;
			GameEnd.IsUserLeft[(wChairID+2)%GAME_PLAYER]=false;
			//cxf end

			//ͳ�ƻ���
			LONGLONG lScore=0;
			LONGLONG lRevenue=0;
			enScoreKind ScoreKind;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//����˰��
				lRevenue=0;
				if ( GameEnd.lGameScore[i]>0L)
				{
					lRevenue=LONGLONG((GameEnd.lGameScore[i]*(m_pGameServiceOption->wRevenue/100.0)));
					GameEnd.lGameTax[i]=lRevenue;
					GameEnd.lGameScore[i]=GameEnd.lGameScore[i]-lRevenue;
				}
				else
				{
					GameEnd.lGameTax[i]=0;
				}
				lScore = GameEnd.lGameScore[i];
				ScoreKind = lScore > 0 ? enScoreKind_Win : enScoreKind_Flee;
				if (i!=wChairID) 
				{
					m_pITableFrame->WriteUserScore(i, lScore, lRevenue, ScoreKind );
				}

			}

			//ʣ���˿�
			BYTE bCardPos=0;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				CopyMemory(&GameEnd.bHandCardData[i],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				GameEnd.bCardCount[i]=m_bCardCount[i];
				CopyMemory(&GameEnd.bCardData[bCardPos],m_bHandCardData[i],m_bCardCount[i]*sizeof(BYTE));
				bCardPos+=m_bCardCount[i];
			}
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.GameBeiShu[i]=m_bUserBeiShu[i];
				GameEnd.RoomScore[i]=(m_pGameServiceOption->lCellScore)*m_bLandScore;
			}


			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//�޸Ļ���
			m_pITableFrame->WriteUserScore(wChairID,GameEnd.lGameScore[wChairID],0,enScoreKind_Draw);

			//������Ϸ
			m_pITableFrame->ConcludeGame();
			memset(m_bUserTrustee, 0, sizeof( m_bUserTrustee ) );
			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//���ͳ���
bool __cdecl CTableFrameSink::SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_WK_FREE:		//����״̬
		{
			//�ڿ���״̬�£���ʼ��������Ϣ.
			//��ʼ������
			//memset(m_UserTaskInfo, 0, sizeof(m_UserTaskInfo));

			//��������
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//���ñ���
			StatusFree.lBaseScore=m_pGameServiceOption->lCellScore;
			CopyMemory( StatusFree.bBrightStart, m_bBrightStart, sizeof( StatusFree.bBrightStart ) );

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_WK_SCORE:		//�з�״̬
		{
			//��������
			CMD_S_StatusScore StatusScore;
			memset(&StatusScore,0,sizeof(StatusScore));

			//���ñ���
			StatusScore.bLandScore=m_bLandScore;
			StatusScore.wCurrentUser=m_wCurrentUser;
			StatusScore.lBaseScore=m_pGameServiceOption->lCellScore;
			StatusScore.bCallScorePhase=m_bCallScorePhase;
			CopyMemory(StatusScore.bScoreInfo,m_bScoreInfo,sizeof(m_bScoreInfo));
			//		CopyMemory(StatusScore.bCardData,m_bHandCardData[wChiarID],m_bCardCount[wChiarID]);
			CopyMemory(StatusScore.bCardData,m_bHandCardData, sizeof(m_bHandCardData)*sizeof(BYTE));
			CopyMemory( StatusScore.bUserTrustee, m_bUserTrustee, sizeof( m_bUserTrustee ) );
			StatusScore.bBrightTime = m_cbBrightCardTime;
			CopyMemory( StatusScore.bUserBrightCard, m_bUserBrightCard, sizeof( StatusScore.bUserBrightCard ) );

			//�����ж�
			if ( m_bCallScorePhase == CSD_BRIGHTCARD && m_wCurrentUser!=wChiarID && m_bUserBrightCard[wChiarID]==false ) m_cbBrightCardCount++;

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusScore,sizeof(StatusScore));
		}
	case GS_WK_DOUBLE_SCORE:	//�ӱ�״̬
		{
			//������Ϣ
			CMD_S_StatusDoubleScore StatusDoubleScore;
			ZeroMemory( &StatusDoubleScore, sizeof( StatusDoubleScore ) );
			StatusDoubleScore.bAllowDouble = m_bAllowDoubleScore;
			CopyMemory(StatusDoubleScore.bBackCard, m_bBackCard, sizeof(m_bBackCard ) );
			CopyMemory(StatusDoubleScore.bCardCount, m_bCardCount, sizeof(m_bCardCount ) );
			CopyMemory(StatusDoubleScore.bCardData, m_bHandCardData, sizeof(StatusDoubleScore.bCardData ) );
			StatusDoubleScore.bLandScore = m_bLandScore;
			CopyMemory(StatusDoubleScore.bUserTrustee, m_bUserTrustee, sizeof(StatusDoubleScore.bUserTrustee ) );	
			StatusDoubleScore.lBaseScore = m_pGameServiceOption->lCellScore;
			StatusDoubleScore.wLandUser = m_wBankerUser;
			CopyMemory(StatusDoubleScore.bDoubleUser, m_bDoubleUser, sizeof(StatusDoubleScore.bDoubleUser ) );	
			CopyMemory( StatusDoubleScore.bUserBrightCard, m_bUserBrightCard, sizeof( StatusDoubleScore.bUserBrightCard ) );


			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusDoubleScore,sizeof(StatusDoubleScore));
		}
	case GS_WK_PLAYING:		//��Ϸ״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//���ñ���
			StatusPlay.wBombTime=m_wBombTime;
			StatusPlay.wBombNums = m_wBombNums;
			StatusPlay.wLandUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.wLastOutUser=m_wTurnWiner;
			StatusPlay.bLandScore=m_bLandScore;
			StatusPlay.bTurnCardCount=m_bTurnCardCount;
			StatusPlay.lBaseScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusPlay.bBackCard,m_bBackCard,sizeof(m_bBackCard));
			CopyMemory(StatusPlay.bCardCount,m_bCardCount,sizeof(m_bCardCount));
			CopyMemory(StatusPlay.bTurnCardData,m_bTurnCardData,sizeof(BYTE)*m_bTurnCardCount);
			//		CopyMemory(StatusPlay.bCardData,m_bHandCardData[wChiarID],sizeof(BYTE)*m_bCardCount[wChiarID]);
			CopyMemory(StatusPlay.bCardData,m_bHandCardData,sizeof(BYTE)*sizeof(m_bHandCardData));
			CopyMemory( StatusPlay.bUserTrustee, m_bUserTrustee, sizeof( m_bUserTrustee ) );
			StatusPlay.bBrightTime = m_cbBrightCardTime;
			CopyMemory( StatusPlay.bUserBrightCard, m_bUserBrightCard, sizeof( StatusPlay.bUserBrightCard ) );
			::memcpy(StatusPlay.bDoubleUser, m_bDoubleUser, sizeof(StatusPlay.bDoubleUser));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	return false;
}

//��ʱ���¼�
bool __cdecl CTableFrameSink::OnTimerMessage(WORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//��Ϸ��Ϣ����
bool __cdecl CTableFrameSink::OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	bool bFlag=false; //����״̬
	switch (wSubCmdID)
	{
	case SUB_C_LAND_SCORE:	//�û��е���
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_LandScore));

			if (wDataSize!=sizeof(CMD_C_LandScore)) 
			{
				bFlag = false;
				break;
			}

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY)
			{
				bFlag=false;
				break;
			}

			//��Ϣ����
			CMD_C_LandScore * pLandScore=(CMD_C_LandScore *)pDataBuffer;
			bFlag = OnUserLandScore(pUserData->wChairID,pLandScore->bLandScore);

			//add by Ivan_han
			this->OnAutoPassOrOutCard();

			break;
		}
	case SUB_C_SNATCH_SCORE:	//������
		{
			//У������
			ASSERT(wDataSize==sizeof(CMD_C_SnatchLand));
			if(wDataSize != sizeof(CMD_C_SnatchLand))
			{
				bFlag = false;
				break;
			}
			//�û�У��
			tagServerUserData * pUserData = pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus != US_PLAY)
			{
				bFlag = false;
				break;
			}
			//��Ϣ����
			CMD_C_SnatchLand * pSnatchLand=(CMD_C_SnatchLand *)pDataBuffer;
			if (NULL != pSnatchLand)
			{
				bFlag = OnUserSnatchLand(pUserData->wChairID,pSnatchLand->bSnatchLand);
			}

			//add by Ivan_han
			this->OnAutoPassOrOutCard();

			break;
		}
	case SUB_C_OUT_CART:		//�û�����
		{
			if (sizeof(CMD_C_OutCard)!=wDataSize)
			{
				bFlag=false;
				break;
			}
			//��������
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;

			if (NULL != pOutCard)
			{
				//�û�Ч��
				tagServerUserData * pUserData=pIServerUserItem->GetUserData();
				if (pUserData->cbUserStatus!=US_PLAY)
				{
					bFlag=false;
					break;
				}

				//��Ϣ����
				bFlag = OnUserOutCard(pUserData->wChairID,pOutCard->bCardData,pOutCard->bCardCount);

				//add by Ivan_han
				//char sz[100]={0};
				//sprintf(sz,"��ӦSUB_C_OUT_CART��Ϣʱ�Ķ��⴦��:m_wCurrentUser=%d",m_wCurrentUser);
				//CTraceService::TraceString(sz, TraceLevel_Normal);
				this->OnAutoPassOrOutCard();
				////��Ϸ��û�н���
				//if (m_wCurrentUser!=INVALID_CHAIR)
				//{
				//	WORD wCurrentUser=m_wCurrentUser;
				//	vector<WORD>::iterator p;
				//	p=find(m_vecChairIDOffLine.begin(),m_vecChairIDOffLine.end(),wCurrentUser);
				//	if(p!=m_vecChairIDOffLine.end())
				//	{
				//		char sz[100]={0};
				//		sprintf(sz,"��ӦSUB_C_OUT_CART��Ϣʱ�Ķ��⴦��:m_wCurrentUser=%d,wCurrentUser=%d",m_wCurrentUser,wCurrentUser);
				//		CTraceService::TraceString(sz, TraceLevel_Normal);
				//		OnUserPassCard(wCurrentUser);
				//	}
				//}

			}
			break;
		}
	case SUB_C_PASS_CARD:
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY)
			{
				bFlag=false;
				break;
			}

			//��Ϣ����
			bFlag = OnUserPassCard(pUserData->wChairID);

			//add by Ivan_han
			//char sz[100]={0};
			//sprintf(sz,"��ӦSUB_C_PASS_CARD��Ϣʱ�Ķ��⴦��:m_wCurrentUser=%d",m_wCurrentUser);
			//CTraceService::TraceString(sz, TraceLevel_Normal);
			this->OnAutoPassOrOutCard();

			break;
		}
	case SUB_C_TRUSTEE:			//����й�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_UserTrustee));
			if (wDataSize != sizeof(CMD_C_UserTrustee))
			{
				bFlag=false;
				break;
			}

			//��������
			CMD_C_UserTrustee *pUserTrustee = ( CMD_C_UserTrustee * )pDataBuffer;
			if (NULL != pUserTrustee)
			{
				//��Ϣ����
				bFlag = OnUserTrustee(pUserTrustee->wUserChairID, pUserTrustee->bTrustee);	
			}
			break;
		}
	case SUB_C_BRIGHT_START:	//���ƿ�ʼ
		{
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();

			//��Ϣ����
			if (NULL != pUserData)
			{
				bFlag = OnBrightStart( pUserData->wChairID, true );
			}
			break;
		}
	case SUB_C_BRIGHT:		//�������
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_BrightCard));
			if (wDataSize != sizeof(CMD_C_BrightCard))
			{
				bFlag=false;
				break;
			}

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY)
			{
				bFlag=false;
				break;
			}

			//��������
			CMD_C_BrightCard *pBrightCard = ( CMD_C_BrightCard * )pDataBuffer;

			if (NULL != pBrightCard)
			{
				//��Ϣ����
				bFlag = OnBrightCard( pUserData->wChairID, pBrightCard->cbBrightCardTime);
			}
			break;
		}
	case SUB_C_DOUBLE_SCORE:	//��Ҽӱ�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_DoubleScore));
			if (wDataSize != sizeof(CMD_C_DoubleScore))
			{
				bFlag=false;
				break;
			}

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY)
			{
				bFlag=false;
				break;
			}

			if (m_bDoubleUser[pUserData->wChairID])
			{//����Ѿ��ӱ�
				bFlag=true;
				break;
			}

			//��������
			CMD_C_DoubleScore *pDoubleScore = ( CMD_C_DoubleScore * )pDataBuffer;

			if (NULL != pDoubleScore)
			{
				//��Ϣ����
				bFlag = OnUserDoubleScore( pUserData->wChairID, pDoubleScore->bDoubleScore, pDoubleScore->bDoubleType);
			}
			break;
		}
	}
	if (!bFlag)
	{//����
		return true;
	}
	return true;
}

//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//Author:<cxf>; 
//CreateDate:<2011-12-29>;
//Description:<�е���>;
bool CTableFrameSink::OnUserLandScore(WORD wChairID, BYTE bLandScore)
{
	//Ч��״̬<�е���״̬>
	if (m_pITableFrame->GetGameStatus()!=GS_WK_SCORE) return false;
	//Ч������û�<�ǲ��Ƿ�����ָ�������û�>
	if (m_wOperateUser != wChairID) return false;

	//�е���״̬
	if (m_bCallScorePhase==CSD_NORMAL)
	{
		//��֤<1:�е���;0:����;>
		if(bLandScore != 1 && bLandScore != 0) 
		{
			return false;
		}

		//�л��û�<Ϊ�е����û�>
		m_wCurrentUser =(wChairID+1)%GAME_PLAYER;
		//�����û�
		m_wOperateUser = m_wCurrentUser;
		//�Ƿ�е���
		if (1==bLandScore)
		{//�е���
			m_wBankerUser = wChairID;										//ׯ��
			//�Ƿ��з����е����û�
			bool bStart=true;
			for (int i=0;i<m_wPlayerCount;++i)
			{
				if (wChairID == i) continue;
				if (!m_bGiveUpBanker[i])
				{//���л���  û������
					bStart=false;
					break;
				}
			}
			m_pITableFrame->SetGameStatus(GS_WK_SNATCH);					//����Ϊ�������׶�
			m_bCallScorePhase=CSD_SNATCHLAND;								//����Ϊ�������׶�
			m_wFirstUser = wChairID;										//�׽��û�

			m_bGiveUpBanker[wChairID]=false;
			for (int i=0;i<GAME_PLAYER;++i)
			{
				m_bUserBeiShu[i]=3;
			}
			//���ͽе�����Ϣ
			CMD_S_LandScore LandScore;
			ZeroMemory( &LandScore, sizeof( LandScore ) );
			LandScore.bLandUser=wChairID;									//�е����û�
			LandScore.bLandScore=bLandScore;								//�е���		(����<0:����; 1:�е���>��ʱ=1)
			LandScore.wCurrentUser= bStart?INVALID_CHAIR:m_wCurrentUser;	//�¸������û�
			LandScore.bCallScorePhase=CSD_SNATCHLAND;						//����״̬<������״̬>
			LandScore.bPreCallScorePhase = CSD_NORMAL;						//֮ǰ״̬<�е���״̬>
			CopyMemory(LandScore.bUserBeiShu,m_bUserBeiShu,sizeof(m_bUserBeiShu));  //������ҵı���
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_LAND_SCORE,&LandScore,sizeof(LandScore));		//�������:�з���Ϣ
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LAND_SCORE,&LandScore,sizeof(LandScore));	//�����Թ����:�з���Ϣ

			//��Ϸ��ʼ
			if (bStart)
			{
				//���͵���
				m_bCardCount[m_wBankerUser]=20;
				CopyMemory(&m_bHandCardData[m_wBankerUser][17],m_bBackCard,sizeof(m_bBackCard));
				m_GameLogic.SortCardList(m_bHandCardData[m_wBankerUser],m_bCardCount[m_wBankerUser],ST_ORDER);

				//���ÿ�ʼ��Ϸ״̬
				m_pITableFrame->SetGameStatus(GS_WK_PLAYING);	
				m_bCallScorePhase=CSD_GAMESTART;
				//������Ϣ
				m_bTurnCardCount=0;											//��ʼ��������Ŀ
				m_wTurnWiner=m_wBankerUser;									//ÿ��ʤ�����
				m_wCurrentUser=m_wBankerUser;								//��ǰ���

				//������Ϣ
				CMD_S_GameStart GameStart;
				GameStart.wLandUser=m_wBankerUser;							//ׯ��
				GameStart.bLandScore=bLandScore;							//
				GameStart.wCurrentUser=m_wBankerUser;						//��ǰ�������
				CopyMemory(GameStart.bBackCardData, m_bBackCard, sizeof(GameStart.bBackCardData));
				CopyMemory(GameStart.bUserBrightCard, m_bUserBrightCard, sizeof(GameStart.bUserBrightCard));
				//��Ϸ��ʼ��Ϣ
				for (int i=0; i<GAME_PLAYER; ++i)
				{
					::memset(GameStart.bCardData, 0, sizeof(GameStart.bCardData));
					::memcpy(GameStart.bCardData, m_bHandCardData[i], sizeof(GameStart.bCardData));
					GameStart.bCardNum = m_bCardCount[i];
					m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(CMD_S_GameStart));
				}
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
				m_bLandScore = 1;
			}
		}
		else
		{//����
			m_bGiveUpBanker[wChairID]=true;									//�����е�����ʶ

			if (m_bGiveUpBanker[m_wCurrentUser])
			{//<һȦû�˽е���>
				//add by Ivan_han 
				if(m_vecChairIDOffLine.size()>0)
				{//����ж��ߵ����,��ɢ��Ϸ.
				     m_pITableFrame->DismissGame();
				}
				else
				{//���·���
					m_bCallScorePhase = CSD_NORMAL;								//�е����׶�
					ZeroMemory( m_bGiveUpBanker, sizeof( m_bGiveUpBanker ) );	//������ʶ
					m_wCurrentUser=m_wFirstUser;								//�����û�
					//����
					this->SetFixOrRandCardOrGoodCard();
					//�����˿�
					CMD_S_SendAllCard SendAllCard ;
					ZeroMemory(&SendAllCard, sizeof(SendAllCard));
					SendAllCard.wCurrentUser = m_wCurrentUser ;					//��ǰ�û�
					CopyMemory(SendAllCard.bCardData, m_bHandCardData, sizeof(m_bHandCardData)) ;//�����˿�
					CopyMemory(SendAllCard.bBackCardData, m_bBackCard, 3);		//�����˿�
					for (WORD i=0;i<m_wPlayerCount;i++)
					{//������
						m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendAllCard,sizeof(SendAllCard));
						m_pITableFrame->SendLookonData(i,SUB_S_SEND_CARD,&SendAllCard,sizeof(SendAllCard));
					}
					//�����˿�
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						m_GameLogic.SortCardList(m_bHandCardData[i],m_bCardCount[i],ST_ORDER);
					}
				}
			}
			else
			{//��һλ�е���
				//���ͽе�����Ϣ
				CMD_S_LandScore LandScore;
				ZeroMemory( &LandScore, sizeof(LandScore));
				LandScore.bLandUser=wChairID;								//�е����û�
				LandScore.bLandScore=bLandScore;							//�е���		(����<0:����; 1:�е���>��ʱ=0)
				LandScore.wCurrentUser=m_wCurrentUser;						//�¸������û�
				LandScore.bCallScorePhase = CSD_NORMAL;						//����״̬<�е���״̬>
				LandScore.bPreCallScorePhase = CSD_NORMAL;					//֮ǰ״̬<�е���״̬>
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_LAND_SCORE,&LandScore,sizeof(LandScore));		//�������:�з���Ϣ
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LAND_SCORE,&LandScore,sizeof(LandScore));	//�����Թ����:�з���Ϣ
			}
		}
		return true;
	}

	return false;//����׶�
}

//Author:<cxf>; 
//CreateDate:<2011-12-29>;
//Description:<������>;
//�������¼�
bool CTableFrameSink::OnUserSnatchLand(WORD wChairID, BYTE bLandScore)
{
	//Ч��״̬<������״̬>
	if (m_pITableFrame->GetGameStatus()!=GS_WK_SNATCH) return false;
	//Ч������û�
	if (m_wOperateUser != wChairID) return false;

	//������
	if (CSD_SNATCHLAND == m_bCallScorePhase)
	{
		bool bContinue=false;											//�Ƿ����������<false:��ʼ; true:����������>
		//�Ƿ�������
		if (1==bLandScore)
		{//������
			++m_bSnatchCount;
			for (int i=0;i<GAME_PLAYER;++i)
			{
				m_bUserBeiShu[i]*=2;                                    //������ұ���
			}
			m_wFirstUser = wChairID;									//�������û�
			m_bGiveUpSnatch[wChairID] = 2;							    //��������ʶ
			if (m_wBankerUser==wChairID)
			{//�е����û��ٴ�������										
				m_bSnatchCount=3;										//����������
				m_wCurrentUser = wChairID;								//��ǰ�û�
				bContinue = false;
			}
			else
			{
				m_wCurrentUser = INVALID_CHAIR;							//��ǰ�û�
				//���˷����е����û�
				WORD wUser = (wChairID+1)%GAME_PLAYER;
				while (wUser != wChairID)
				{
					//û�з������е������л��������� �� ��û����������
					if (!m_bGiveUpBanker[wUser] && m_bGiveUpSnatch[wUser] == 0)
					{
						m_wCurrentUser=wUser;
						break;
					}
					if(wUser==wChairID) break;
					wUser = (wUser+1)%GAME_PLAYER;
				}

				//�Ƿ񻹿���������
				if (m_wCurrentUser == INVALID_CHAIR)
				{//û���ٿ���������
					m_wCurrentUser = wChairID;
					bContinue = false;
				}
				else
				{//��һ���������
					bContinue = true;
				}
			}
		}
		else
		{//����
			m_bGiveUpSnatch[wChairID] = 1;								//��������ʶ<0:û�в���; 1:����; 2:������>
			m_wCurrentUser = INVALID_CHAIR;								//��ǰ�û�

			//����Ƿ����������
			bool bSnatch=false;
			WORD wAccessUser = (m_wFirstUser+1)%GAME_PLAYER;
			while (wAccessUser != m_wFirstUser)
			{
				if (m_bGiveUpSnatch[wAccessUser] == 0 && !m_bGiveUpBanker[wAccessUser])
				{//��������������
					bSnatch =true;
					break;
				}
				wAccessUser = (wAccessUser+1)%GAME_PLAYER;
			}

			//���˷����е����û�
			if (bSnatch)
			{
				WORD wUser = (wChairID+1)%GAME_PLAYER;
				while (wUser != wChairID)
				{
					//û�з������е������л��������� �� ��û����������
					if (!m_bGiveUpBanker[wUser] && m_bGiveUpSnatch[wUser] == 0)
					{
						m_wCurrentUser=wUser;
						break;
					}
					if(wUser==wChairID) break;
					wUser = (wUser+1)%GAME_PLAYER;
				}
			}

			//�Ƿ񻹿���������
			if (m_wCurrentUser == INVALID_CHAIR)
			{//û���ٿ���������
				m_wCurrentUser = m_wFirstUser;							//�ϴ��������û�<>
				bContinue = false;
			}
			else
			{//��һ���������
				bContinue = true;
			}
		}

		//ָ���������
		m_wOperateUser = m_wCurrentUser;

		//�Ƿ����������<false:��ʼ; true:����������>
		if (!bContinue)
		{//���������
			m_wBankerUser = m_wCurrentUser;								//ׯ��
			//��������Ϣ
			CMD_S_SnatchLand SnatchLand;
			ZeroMemory(&SnatchLand, sizeof(CMD_S_SnatchLand));
			SnatchLand.wCurrentUser=INVALID_CHAIR;						//��ǰ�û�
			SnatchLand.wSnatchUser=wChairID;							//�������û�
			SnatchLand.bSnatchLand=bLandScore;							//������		(����<0:����; 1:������>��ʱ=1)
			SnatchLand.bPreCallScorePhase = CSD_SNATCHLAND;				//֮ǰ״̬<������״̬>
			SnatchLand.bCallScorePhase = CSD_GAMESTART;					//����״̬<������״̬>
			CopyMemory(SnatchLand.bUserBeiShu,m_bUserBeiShu,sizeof(m_bUserBeiShu));   //��������������
			SnatchLand.bSnatchLandCount=m_bSnatchCount;                 //����������
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_SNATCH_SCORE,&SnatchLand,sizeof(SnatchLand));		//�������:��������Ϣ
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_SNATCH_SCORE,&SnatchLand,sizeof(SnatchLand));		//�����Թ����:��������Ϣ

			//���͵���
			m_bCardCount[m_wBankerUser]=20;
			CopyMemory(&m_bHandCardData[m_wBankerUser][17],m_bBackCard,sizeof(m_bBackCard));
			m_GameLogic.SortCardList(m_bHandCardData[m_wBankerUser],m_bCardCount[m_wBankerUser],ST_ORDER);

			//���ÿ�ʼ��Ϸ״̬
			m_pITableFrame->SetGameStatus(GS_WK_PLAYING);	
			m_bCallScorePhase=CSD_GAMESTART;

			//������Ϣ
			m_bTurnCardCount=0;											//��ʼ��������Ŀ
			m_wTurnWiner=m_wBankerUser;									//ÿ��ʤ�����

			//������Ϣ
			CMD_S_GameStart GameStart;
			GameStart.wLandUser=m_wBankerUser;							//ׯ��
			GameStart.bLandScore=bLandScore;							//
			GameStart.wCurrentUser=m_wBankerUser;						//��ǰ�������
			CopyMemory(GameStart.bBackCardData, m_bBackCard, sizeof(GameStart.bBackCardData));
			//��Ϸ��ʼ��Ϣ
			for (int i=0; i<GAME_PLAYER; ++i)
			{
				::memset(GameStart.bCardData, 0, sizeof(GameStart.bCardData));
				::memcpy(GameStart.bCardData, m_bHandCardData[i], sizeof(GameStart.bCardData));
				GameStart.bCardNum = m_bCardCount[i];
				m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(CMD_S_GameStart));
			}
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
			m_bLandScore =1;
		}
		else
		{//��һ���������
			//��������Ϣ
			CMD_S_SnatchLand SnatchLand;
			ZeroMemory(&SnatchLand, sizeof(CMD_S_SnatchLand));
			SnatchLand.wCurrentUser=m_wCurrentUser;						//��ǰ�û�
			SnatchLand.wSnatchUser=wChairID;							//�������û�
			SnatchLand.bSnatchLand=bLandScore;							//������		(����<0:����; 1:������>��ʱ=1)
			SnatchLand.bPreCallScorePhase = CSD_SNATCHLAND;				//����״̬<������״̬>
			SnatchLand.bCallScorePhase = CSD_SNATCHLAND;				//֮ǰ״̬<������״̬>
			CopyMemory(SnatchLand.bUserBeiShu,m_bUserBeiShu,sizeof(m_bUserBeiShu));  //��������������
			SnatchLand.bSnatchLandCount=m_bSnatchCount;                 //����������
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_SNATCH_SCORE,&SnatchLand,sizeof(SnatchLand));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_SNATCH_SCORE,&SnatchLand,sizeof(SnatchLand));
		}
		return true;
	}
	return false;//����׶�
}

/* Del:<cxf>; DeleteDate:<2011-12-29>
//�з��¼�
bool CTableFrameSink::OnUserLandScore(WORD wChairID, BYTE bLandScore)
{
//Ч��״̬
if (m_pITableFrame->GetGameStatus()!=GS_WK_SCORE) return true;

//��������
bool bContinue=false;
BYTE cbPreCallScorePhase = m_bCallScorePhase;

//�зִ���
if (m_bCallScorePhase==CSD_NORMAL)
{
//Ч�����
ASSERT(((bLandScore<=3)||(bLandScore==255))&&(bLandScore>m_bLandScore));
if (((bLandScore>3)&&(bLandScore!=255))||(bLandScore<=m_bLandScore)) return false;

//���ñ���
if (bLandScore!=255)
{
m_bLandScore=bLandScore;
//m_bLandScore=1;
m_wBankerUser=m_wCurrentUser;
}
else
{
bContinue = true;
m_bGiveUpBanker[wChairID] = true;

//�л����
m_wCurrentUser=(wChairID+1)%m_wPlayerCount;

//���·���
if ( m_bGiveUpBanker[m_wCurrentUser] )
{
//���·���
if ( m_wBankerUser == INVALID_CHAIR )
{
//������
m_bBrightCard=false;
m_bCallScorePhase=CSD_NORMAL;
ZeroMemory( m_bGiveUpBanker, sizeof( m_bGiveUpBanker ) );

BYTE cbSendGoodCard = rand() % GOOD_CARD_PERCENT ;
BYTE cbAndroidUserCount=0 ;
WORD wAndroidUser[GAME_PLAYER]={0};

//���ñ���
ZeroMemory(m_bScoreInfo, sizeof(m_bScoreInfo)) ;

//�����˸���
for(WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
{
IServerUserItem *pServerUserItem = m_pITableFrame->GetServerUserItem(wChairID) ;
if(pServerUserItem->IsAndroidUser())
{
wAndroidUser[cbAndroidUserCount] = (wChairID) ;
++cbAndroidUserCount ;
}
}

WORD wHaveGoodCardAndroidUser = INVALID_CHAIR ;
if(cbAndroidUserCount>0) wHaveGoodCardAndroidUser = wAndroidUser[rand()%cbAndroidUserCount] ;

//���ͺ���
if( 0 == cbSendGoodCard && wHaveGoodCardAndroidUser!=INVALID_CHAIR)
{
//�����˿�
BYTE bRandCard[54];
m_GameLogic.RandCardList(bRandCard,sizeof(bRandCard)/sizeof(bRandCard[0]));

BYTE cbGoodCard[NORMAL_COUNT] ;
m_GameLogic.GetGoodCardData(cbGoodCard) ;

//��ȡ����
m_GameLogic.RemoveGoodCardData(cbGoodCard, NORMAL_COUNT, bRandCard, FULL_COUNT ) ;

//�ַ��˿�
CopyMemory(m_bHandCardData[wHaveGoodCardAndroidUser], cbGoodCard, NORMAL_COUNT) ;
m_bCardCount[wHaveGoodCardAndroidUser]=17;
for (WORD i=0, j=0;i<m_wPlayerCount;i++)
{
if(i!=wHaveGoodCardAndroidUser)
{
m_bCardCount[i]=17;
CopyMemory(&m_bHandCardData[i],&bRandCard[j*17],sizeof(BYTE)*17);
++j ;
}
}
CopyMemory(m_bBackCard,&bRandCard[34],sizeof(m_bBackCard));
m_GameLogic.SortCardList(m_bBackCard,3,ST_ORDER);
}
else
{
////�����˿�
//BYTE bRandCard[54];
//m_GameLogic.RandCardList(bRandCard,sizeof(bRandCard)/sizeof(bRandCard[0]));

////�ַ��˿�
//for (WORD i=0;i<m_wPlayerCount;i++)
//{
//	m_bCardCount[i]=17;
//	CopyMemory(&m_bHandCardData[i],&bRandCard[i*m_bCardCount[i]],sizeof(BYTE)*m_bCardCount[i]);
//}
//CopyMemory(m_bBackCard,&bRandCard[51],sizeof(m_bBackCard));
//m_GameLogic.SortCardList(m_bBackCard,3,ST_ORDER);
this->SetFixOrRandCard();
}

//�����û�
m_wCurrentUser=m_wFirstUser;

//�����˿�
CMD_S_SendAllCard SendAllCard ;
ZeroMemory(&SendAllCard, sizeof(SendAllCard));
SendAllCard.wCurrentUser = m_wCurrentUser ;

//�����˿�
CopyMemory(SendAllCard.bCardData, m_bHandCardData, sizeof(m_bHandCardData)) ;
//�����˿�
CopyMemory(SendAllCard.bBackCardData, m_bBackCard, 3) ;
for (WORD i=0;i<m_wPlayerCount;i++)
{
m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendAllCard,sizeof(SendAllCard));
m_pITableFrame->SendLookonData(i,SUB_S_SEND_CARD,&SendAllCard,sizeof(SendAllCard));
}

//�����˿�
for (WORD i=0;i<m_wPlayerCount;i++)
{
m_GameLogic.SortCardList(m_bHandCardData[i],m_bCardCount[i],ST_ORDER);
}

return true;
}
else
{
//�������
m_wCurrentUser = m_wBankerUser;

//�л�״̬
m_bCallScorePhase=CSD_GAMESTART;

//���ñ���
bContinue = false;
}
}
}

m_bScoreInfo[wChairID]=bLandScore;



//״̬�л�
if (m_bLandScore > 0 && m_bCallScorePhase!=CSD_GAMESTART)
{
bContinue=true;
//m_wFirstUser=wChairID;
//m_bCallScorePhase=CSD_SNATCHLAND;

//�л����
m_wCurrentUser = INVALID_CHAIR;
WORD wUser = (wChairID+1)%GAME_PLAYER;
if(m_bLandScore != 3)
{
while (  wUser != wChairID ) 
{
if ( m_bGiveUpBanker[wUser] == false )
{
m_wCurrentUser = wUser;
break;
}
wUser = (wUser+1)%GAME_PLAYER ;
}
}

if ( m_wCurrentUser == INVALID_CHAIR )
{
m_wCurrentUser = wChairID;
bContinue = false;
}
}
//��ʼ�ж�
if(m_wFirstUser == m_wCurrentUser)
{
//�������
m_wCurrentUser = m_wBankerUser;

//�л�״̬
m_bCallScorePhase=CSD_GAMESTART;

//���ñ���
bContinue = false;
}
}
else if (m_bCallScorePhase==CSD_SNATCHLAND)
{
//Ч�����
ASSERT((bLandScore==4)||(bLandScore==255));
if ((bLandScore!=4)&&(bLandScore!=255)) return false;

//���ñ���
if (bLandScore==4)
{
m_bLandScore++;
m_wBankerUser=wChairID;
}

//״̬�л�
if (m_wFirstUser==wChairID)
{
bContinue=true;
m_wFirstUser=m_wBankerUser;
m_wCurrentUser = m_wBankerUser;

//�����ж�
if ( m_cbBrightCardTime > 2 && m_bUserBrightCard[m_wCurrentUser] )
{
m_bCallScorePhase = CSD_GAMESTART;
bContinue = false;
}
else
m_bCallScorePhase=CSD_BRIGHTCARD;
}
else bContinue=true;

//�л����
if ( m_bCallScorePhase == CSD_SNATCHLAND )
{
m_wCurrentUser = INVALID_CHAIR;
WORD wUser = (wChairID+1)%GAME_PLAYER;
while (  wUser != wChairID && wUser != (m_wFirstUser+1)%GAME_PLAYER ) 
{
if ( m_bGiveUpBanker[wUser] == false )
{
m_wCurrentUser = wUser;
break;
}
wUser = (wUser+1)%GAME_PLAYER ;
}

if ( m_wCurrentUser == INVALID_CHAIR || ( m_wFirstUser == m_wBankerUser && m_wFirstUser == m_wCurrentUser ))
{
m_wCurrentUser = m_wBankerUser;
bContinue=true;
m_wFirstUser=m_wBankerUser;

//�����ж�
bool bAllUserBright = true;
for ( WORD wBrightIdx = 0; wBrightIdx < GAME_PLAYER; ++wBrightIdx )
if ( ! m_bUserBrightCard[wBrightIdx] ) bAllUserBright = false;

//�����ж�
if ( m_cbBrightCardTime > 2 && bAllUserBright )
{
m_bCallScorePhase = CSD_GAMESTART;
bContinue = false;
}
else
m_bCallScorePhase=CSD_BRIGHTCARD;
}
}
}
else if (m_bCallScorePhase==CSD_BRIGHTCARD)
{
//Ч�����
ASSERT((bLandScore==5)||(bLandScore==255));
if ((bLandScore!=5)&&(bLandScore!=255)) return false;

//���Ӵ���
m_cbBrightCardCount++;
if ( m_cbBrightCardCount == GAME_PLAYER )
{
bContinue = false;
m_bCallScorePhase = CSD_BRIGHTCARD;
}
else 
{
bContinue = true;
m_bCallScorePhase=CSD_GAMESTART;
}

//���ñ���
m_bBrightCard=(bLandScore==5)?true:false;
if ( bLandScore == 5 )
{
m_bLandScore++;
m_bUserBrightCard[wChairID] = true;
}
bContinue = true;
}

//��ʼ�ж�
//if ((bContinue==false)&&(m_bLandScore>=1 || m_wBankerUser != INVALID_CHAIR))
//{
//	//�л�״̬
//	//m_bCallScorePhase = CSD_BRIGHTCARD;
//}


////����״̬
//if ( m_bCallScorePhase == CSD_BRIGHTCARD )
//{
//	//���Ƹ���
//	for ( WORD wBrightIdx = 0; wBrightIdx < GAME_PLAYER; ++wBrightIdx ) 
//		if ( m_bUserBrightCard[wBrightIdx] ) ++m_cbBrightCardCount;
//}

//��ʼ�ж�
//if ( m_cbBrightCardCount == GAME_PLAYER  && CSD_BRIGHTCARD ==m_bCallScorePhase )
if ((bContinue==false)&&(m_bLandScore>=1 || m_wBankerUser != INVALID_CHAIR))
{
//cxf 
//����״̬
//m_pITableFrame->SetGameStatus(GS_WK_DOUBLE_SCORE);//�ӱ�״̬
// end

//��Ϸ��ʼ֮ǰ��¼������m_wBankerUser
//for (int i=0; i<GAME_PLAYER; i++)
//{
//	if (i == m_wBankerUser)
//	{//�ǵ�������1��
//		++m_UserTaskInfo[i].iRegJoineLandNums;
//	}
//	else
//	{//���ǵ��������㡣
//		m_UserTaskInfo[i].iRegJoineLandNums = 0;
//	}
//}
// 
//���ÿ�ʼ״̬ lq ADD JiaoFen
m_bCallScorePhase=CSD_GAMESTART;

//��ǰ��ұ���
for (int i=0; i<GAME_PLAYER; i++)
{
if (i == m_wBankerUser)
{
m_bUserBeiShu[i]=2*1;
}
else
{
m_bUserBeiShu[i]=1;
}	
}
BYTE bBackCard[3]={0};
bBackCard[0]=m_GameLogic.GetCardValue(m_bBackCard[0]);
bBackCard[1]=m_GameLogic.GetCardValue(m_bBackCard[1]);
bBackCard[2]=m_GameLogic.GetCardValue(m_bBackCard[2]);
if ((2==bBackCard[0]&&bBackCard[1]!=2&&bBackCard[2]!=2) || (2==bBackCard[1]&&bBackCard[0]!=2&&bBackCard[2]!=2) ||
(2==bBackCard[2]&&bBackCard[0]!=2&&bBackCard[1]!=2))  //lq ��������
{
m_tagTask[m_wBankerUser] |= CT_TASK_DP_TWO;
}
if (15==bBackCard[0] || 15==bBackCard[1] || 15==bBackCard[2])
{
m_tagTask[m_wBankerUser] |= CT_TASK_DP_KING;
}
if (bBackCard[0]==bBackCard[1] || bBackCard[1]==bBackCard[2])
{
m_tagTask[m_wBankerUser] |= CT_TASK_DP_SAME;
}

//������Ϣ
CMD_S_LandScore LandScore;
ZeroMemory( &LandScore, sizeof( LandScore ) );
LandScore.bLandUser=wChairID;
LandScore.bLandScore=bLandScore;
LandScore.wCurrentUser=INVALID_CHAIR;
LandScore.bCurrentScore=m_bLandScore;
LandScore.bCallScorePhase=m_bCallScorePhase;
LandScore.bPreCallScorePhase = cbPreCallScorePhase;
CopyMemory(LandScore.bBrightCardUser, m_bUserBrightCard, sizeof( LandScore.bBrightCardUser ) );
CopyMemory(LandScore.bUserBeiShu, m_bUserBeiShu, sizeof( LandScore.bUserBeiShu ) );
m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_LAND_SCORE,&LandScore,sizeof(LandScore));
m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LAND_SCORE,&LandScore,sizeof(LandScore));

//��ȡ·��
CHAR szDirectory[128];
GetCurrentDirectory(sizeof( szDirectory ), szDirectory );

//�ļ�����
CHAR szProfileName[128];
sprintf(szProfileName, TEXT("%s\\LandCrazy.ini"), szDirectory);

//��ȡ����
LONGLONG lDoubleLimit = GetPrivateProfileInt(TEXT("DoubleScore"), TEXT("Score"), 100, szProfileName );

bool bAllowDouble = true;
for ( WORD wUserID = 0; wUserID < GAME_PLAYER; ++wUserID )
{
//��ȡ���
IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem( wUserID );
if ( pIServerUserItem == NULL ) continue;

//�����ж�
LONGLONG lUserScore = pIServerUserItem->GetUserScore()->lGold;
//(���ܳ���)����ϷΪ��ʯ����Ӧ��lScore Ϊ��ʯ������ϷΪ���ֵ�Ӧ��lScore�ɻ���.  �ɷ�����������
if ( lUserScore < lDoubleLimit )
{
bAllowDouble = false;
break;
}
}
m_bAllowDoubleScore = bAllowDouble;

//���͵���
m_bCardCount[m_wBankerUser]=20;
CopyMemory(&m_bHandCardData[m_wBankerUser][17],m_bBackCard,sizeof(m_bBackCard));
m_GameLogic.SortCardList(m_bHandCardData[m_wBankerUser],m_bCardCount[m_wBankerUser],ST_ORDER);

//������Ϣ
CMD_S_DoubleScore DoubleScore;
ZeroMemory( &DoubleScore, sizeof( DoubleScore ) );
DoubleScore.bAllowDouble = bAllowDouble;
DoubleScore.wLandUser = m_wBankerUser;
DoubleScore.bCurrentScore = m_bLandScore;
CopyMemory(DoubleScore.bBackCard,m_bBackCard,sizeof(m_bBackCard));

//������Ϣ
m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_DOUBLE_SCORE, &DoubleScore, sizeof( DoubleScore ) );

//----cxf �з����
//����״̬
m_pITableFrame->SetGameStatus(GS_WK_PLAYING);	

//������Ϣ
m_bTurnCardCount=0;
m_wTurnWiner=m_wBankerUser;
m_wCurrentUser=m_wBankerUser;

//������Ϣ
CMD_S_GameStart GameStart;
GameStart.wLandUser=m_wBankerUser;
GameStart.bLandScore=m_bLandScore;
GameStart.bBrightCard=m_bBrightCard;
GameStart.wCurrentUser=m_wCurrentUser;
CopyMemory(GameStart.bBackCardData, m_bBackCard, sizeof(GameStart.bBackCardData));
CopyMemory(GameStart.bUserBrightCard, m_bUserBrightCard, sizeof(GameStart.bUserBrightCard));

for (int i=0; i<GAME_PLAYER; ++i)
{
::memset(GameStart.bCardData, 0, sizeof(GameStart.bCardData));
::memcpy(GameStart.bCardData, m_bHandCardData[i], sizeof(GameStart.bCardData));
GameStart.bCardNum = m_bCardCount[i];
m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(CMD_S_GameStart));
}

m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
//------cxf  end
//���ñ���
if (m_bLandScore==0) m_bLandScore=1;
if (m_wBankerUser==INVALID_CHAIR) m_wBankerUser=m_wFirstUser;

return true;
}

//������Ϣ
CMD_S_LandScore LandScore;
ZeroMemory( &LandScore, sizeof( LandScore ) );
LandScore.bLandUser=wChairID;
LandScore.bLandScore=bLandScore;
LandScore.wCurrentUser=m_wCurrentUser;
LandScore.bCurrentScore=m_bLandScore;
LandScore.bCallScorePhase=m_bCallScorePhase;
LandScore.bPreCallScorePhase = cbPreCallScorePhase;
CopyMemory(LandScore.bBrightCardUser, m_bUserBrightCard, sizeof( LandScore.bBrightCardUser ) );
m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_LAND_SCORE,&LandScore,sizeof(LandScore));
m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LAND_SCORE,&LandScore,sizeof(LandScore));

return true;
}
*/
//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_PLAYING) return true;
	if (wChairID!=m_wCurrentUser) 
		return false;

	//�����˿�
	m_GameLogic.SortCardList(bCardData,bCardCount,ST_ORDER);

	//�����ж�
	BYTE bCardType=m_GameLogic.GetCardType(bCardData,bCardCount);
	if (bCardType==CT_ERROR) 
		return false;
	//����
	if(CT_SINGLE_LINE==bCardType)
	{
		m_tagTask[wChairID] |=CT_TASK_SHUN;
		if ( 3==m_GameLogic.GetCardValue(bCardData[bCardCount-1])|| 3==m_GameLogic.GetCardValue(bCardData[bCardCount-2]))
		{
			m_tagTask[wChairID] |=CT_TASK_SHUN_3;
		}    
		if (1==m_GameLogic.GetCardValue(bCardData[bCardCount-1]))
		{
			m_tagTask[wChairID] |=CT_TASK_SHUN_A;
		}
		if (8==bCardCount)
		{
			m_tagTask[wChairID] |=CT_TASK_SHUN_8;
		}
		if (12==bCardCount)
		{
			m_tagTask[wChairID] |=CT_TASK_SHUN_12;
		}
	}
	if (CT_DOUBLE==bCardType)
	{
		m_tagTask[wChairID] |=CT_TASK_DOUBLE;
		if (4==m_GameLogic.GetCardValue(bCardData[0]))
		{
			m_tagTask[wChairID] |=CT_TASK_DOUBLE_4;
		}
		if (11==m_GameLogic.GetCardValue(bCardData[0]))
		{
			m_tagTask[wChairID] |=CT_TASK_DOUBLE_J;
		}
		if (12==m_GameLogic.GetCardValue(bCardData[0]))
		{
			m_tagTask[wChairID] |=CT_TASK_DOUBLE_Q;
		}
	}
	if (CT_DOUBLE_LINE==bCardType)
	{
		m_tagTask[wChairID] |=CT_TASK_LIANDUI;
		for (int i=0;i<=bCardCount; i+=2)
		{
			BYTE bLastCard=0;
			bLastCard=m_GameLogic.GetCardValue(bCardData[i]);
			if (8==bLastCard)
			{
				m_tagTask[wChairID] |=CT_TASK_LIANDUI_8;
				break;
			}
		}
	}
	if ((CT_THREE_LINE_TAKE_ONE==bCardType || CT_THREE_LINE_TAKE_TWO==bCardType) )
	{
		m_tagTask[wChairID] |=CT_TASK_THREE_N;
	}
	if ((CT_THREE_LINE_TAKE_ONE==bCardType || CT_THREE_LINE_TAKE_TWO==bCardType || 
		CT_THREE_LINE==bCardType) && bCardCount>5)
	{
		m_tagTask[wChairID] |=CT_TASK_FEIJI;
	}
	if (CT_MISSILE_CARD==bCardType)
	{
		m_tagTask[wChairID] |=CT_TASK_HUOJIAN;
	}
	if (CT_BOMB_CARD==bCardType)
	{
		m_tagTask[wChairID] |=CT_TASK_ZHADAN;
		if (12==m_GameLogic.GetCardValue(bCardData[0]))
		{
			m_tagTask[wChairID] |=CT_TASK_ZHADAN_Q;
		}
	}
	if (CT_THREE==bCardType && 3==m_GameLogic.GetCardValue(bCardData[0]))
	{
		m_tagTask[wChairID] |=CT_TASK_THREE_3;
	}
	if (CT_SINGLE==bCardType)
	{
		if (3==m_GameLogic.GetCardValue(bCardData[0]))
		{
			m_tagTask[wChairID] |=CT_TASK_SINGLE_3;
		}
		if (9==m_GameLogic.GetCardValue(bCardData[0]))
		{
			m_tagTask[wChairID] |=CT_TASK_SINGLE_9;
		}
		if (1==m_GameLogic.GetCardValue(bCardData[0]))
		{
			m_tagTask[wChairID] |=CT_TASK_SINGLE_A;
		}
		if (13==m_GameLogic.GetCardValue(bCardData[0]))
		{
			m_tagTask[wChairID] |=CT_TASK_SINGLE_K;
		}
		if (14==m_GameLogic.GetCardValue(bCardData[0]) || 15==m_GameLogic.GetCardValue(bCardData[0]))
		{
			m_tagTask[wChairID] |=CT_TASK_SINGLE_KING;
		}
	}

	//�������
	if (m_bTurnCardCount==0)
	{
		m_bTurnCardCount=bCardCount;
	}
	else if (m_GameLogic.CompareCard(m_bTurnCardData,bCardData,m_bTurnCardCount,bCardCount)==false) 
	{
		return false;
	}

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID])==false) 
	{
		return false;
	}
	m_bCardCount[wChairID]-=bCardCount;

	//���Ƽ�¼
	m_bTurnCardCount=bCardCount;
	m_bOutCardCount[wChairID]++;
	CopyMemory(m_bTurnCardData,bCardData,sizeof(BYTE)*bCardCount);

	//ը���ж�
	if ((bCardType==CT_BOMB_CARD)||(bCardType==CT_MISSILE_CARD))
	{
		m_wBombTime*=2;
		m_wBombNums++;
		//if (CT_BOMB_CARD == bCardType)
		//{//���ը��Ӯ��
		//	++m_UserTaskInfo[wChairID].bRegBombNums;
		//	m_UserTaskInfo[wChairID].bIsOutBombWin = true;
		//}
		//else if (CT_MISSILE_CARD == bCardType)
		//{//������Ӯ��
		//	m_UserTaskInfo[wChairID].bIsOutMissile = true;
		//}
		//��ұ���
		for (int i=0;i<GAME_PLAYER;++i)
		{
			m_bUserBeiShu[i]*=2;
			//m_OneBombNums[i]++;
		}
	}
	if (bCardType==CT_MISSILE_CARD)
	{
		m_IsMissile=true;
	}
	//�л��û�
	m_wTurnWiner=wChairID;
	if (m_bCardCount[wChairID]!=0)
	{
		if (bCardType!=CT_MISSILE_CARD)
		{//���ǻ��
			m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
		}
	}
	else
	{//�Ƴ��꣬��Ϸ����.
		m_wCurrentUser=INVALID_CHAIR;
	}

	//��������
	CMD_S_OutCard OutCard;
	OutCard.bCardCount=bCardCount;
	OutCard.wOutCardUser=wChairID;
	OutCard.wCurrentUser=m_wCurrentUser;
	OutCard.wBombNums = m_wBombNums;//ը������
	OutCard.bHandCardCount=m_bCardCount[wChairID];//���Ƹ�����
	CopyMemory(OutCard.bCardData,m_bTurnCardData,m_bTurnCardCount*sizeof(BYTE));
	CopyMemory(OutCard.bUserBeiShu,m_bUserBeiShu,sizeof(OutCard.bUserBeiShu));
	//---cxf
	//���ƴ���
	++m_bCircuit[wChairID];
	OutCard.wBankerUser = m_wBankerUser;//ׯ��
	ZeroMemory(OutCard.bIsDouble,sizeof(OutCard.bIsDouble));
	CopyMemory(OutCard.bIsDouble,m_bDoubleUser,sizeof(OutCard.bIsDouble));
	CopyMemory(OutCard.bCircuit,m_bCircuit,sizeof(OutCard.bCircuit));
	//---end

	//��������
	//WORD wSendSize=sizeof(CMD_S_OutCard)-sizeof(OutCard.bCardData)+OutCard.bCardCount*sizeof(BYTE);
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(CMD_S_OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(CMD_S_OutCard));

	//�������
	if (bCardType==CT_MISSILE_CARD)
	{//�����ǻ��
		m_bTurnCardCount=0;
	}

	//�����ж�
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		OnEventGameEnd(wChairID,NULL,GER_NORMAL);
	}
	//��������ӡ����
	//char chData[128]={0};
	//sprintf(chData, "%x,%x,%x,%x,%x", m_bTurnCardData[0], m_bTurnCardData[1],m_bTurnCardData[2], m_bTurnCardData[3], m_bTurnCardData[4]);
	//CTraceService::TraceString(chData, TraceLevel_Normal);

	return true;
}

//�û�����
bool CTableFrameSink::OnUserPassCard(WORD wChairID)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_WK_PLAYING) return true;
	if ((wChairID!=m_wCurrentUser)||(m_bTurnCardCount==0)) return false;

	//���ñ���
	m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
	if (m_wCurrentUser==m_wTurnWiner) m_bTurnCardCount=0;


	//��������
	CMD_S_PassCard PassCard;
	PassCard.wPassUser=wChairID;
	PassCard.wCurrentUser=m_wCurrentUser;
	PassCard.bNewTurn=(m_bTurnCardCount==0)?TRUE:FALSE;
	//---cxf
	//���ƴ���.
	++m_bCircuit[wChairID];
	ZeroMemory(PassCard.bIsDouble,sizeof(PassCard.bIsDouble));
	CopyMemory(PassCard.bIsDouble,m_bDoubleUser,sizeof(PassCard.bIsDouble));
	CopyMemory(PassCard.bCircuit,m_bCircuit,sizeof(PassCard.bCircuit));
	PassCard.wBankerUser = m_wBankerUser;//ׯ��
	//---end
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PASS_CARD,&PassCard,sizeof(PassCard));

	return true;
}

//add by Ivan_han
//���������������ҳ��ƻ򲻳���е�����������
bool CTableFrameSink::OnAutoPassOrOutCard()
{
	int a=0;
	//��Ϸ��û�н���
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		vector<WORD>::iterator p;
		p=find(m_vecChairIDOffLine.begin(),m_vecChairIDOffLine.end(),m_wCurrentUser);
		if(p!=m_vecChairIDOffLine.end())
		{
			BYTE b=m_pITableFrame->GetGameStatus();
			if(b==GS_WK_SCORE)//100
				this->OnUserLandScore(m_wCurrentUser,0);
			else if(b==GS_WK_SNATCH)//103
				this->OnUserSnatchLand(m_wCurrentUser,1);
			else if(b==GS_WK_PLAYING)//102
			{
				//�ж��������������Ƿ�����ƣ������û���ƣ��������ұ����һ����
				if(m_wCurrentUser!=m_wTurnWiner)
				{	
					bool bFlag=this->OnUserPassCard(m_wCurrentUser);
				}
				else
				{

					BYTE bCardData[1]={0};
					BYTE bCardCount=1;
					m_GameLogic.SortCardList(m_bHandCardData[m_wCurrentUser],m_bCardCount[m_wCurrentUser],ST_ORDER);
					bCardData[0]=m_bHandCardData[m_wCurrentUser][0];
					//m_GameLogic.RemoveCard(bCardData,bCardCount,m_bHandCardData[m_wCurrentUser],m_bCardCount[m_wCurrentUser]);
					this->OnUserOutCard(m_wCurrentUser,bCardData,bCardCount);
				}
			}

		}
	}
	return true;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	if ( ! bLookonUser )
	{
		//���ñ���
		m_bBrightStart[wChairID] = false;
		m_bUserBrightCard[wChairID] = false;
		m_cbBrightCardTime=1;
		for ( WORD wBrightUser = 0; wBrightUser < GAME_PLAYER; ++wBrightUser )
			if ( m_bBrightStart[wBrightUser] ) m_cbBrightCardTime = BRIGHT_START_TIME;

		//������Ϣ
		OnBrightStart( wChairID, false );
	}

	return true;
}

//����й�
bool CTableFrameSink::OnUserTrustee(WORD wChairID, bool bTrustee)
{
	m_bUserTrustee[ wChairID ] = bTrustee;

	//��������
	CMD_C_UserTrustee UserTrustee;
	ZeroMemory( &UserTrustee, sizeof( UserTrustee ) );
	UserTrustee.wUserChairID = wChairID;
	UserTrustee.bTrustee = bTrustee;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_C_TRUSTEE, &UserTrustee,sizeof(UserTrustee));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_C_TRUSTEE, &UserTrustee,sizeof(UserTrustee));

	return true;
}

//���ƿ�ʼ
bool CTableFrameSink::OnBrightStart( WORD wChairID , bool bBright )
{
	//���ñ���
	m_bBrightStart[wChairID] = bBright;
	m_bUserBrightCard[wChairID] = bBright;

	if ( bBright ) m_cbBrightCardTime=BRIGHT_START_TIME;

	//�������
	if ( bBright && m_wCurrentUser == INVALID_CHAIR )
	{
		m_wCurrentUser = wChairID;
		m_wBankerUser = wChairID;
	}

	//������Ϣ
	CMD_S_BrightStart BrightStart;
	ZeroMemory( &BrightStart, sizeof( BrightStart ) );
	BrightStart.wBrightUser = wChairID;
	BrightStart.bBright = bBright;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BRIGHT_START,&BrightStart,sizeof(BrightStart));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_BRIGHT_START,&BrightStart,sizeof(BrightStart));
	return true;
}


//�������
bool  CTableFrameSink::OnBrightCard( WORD wChairID, BYTE cbBrightCardTime)
{
	//������Ϣ
	if ( 0 < cbBrightCardTime && cbBrightCardTime != 255 ) 
	{
		CHAR szMsg[128] ;
		if (m_pGameServiceOption->cbHideUserInfo)
			sprintf(szMsg, TEXT("���ѡ�������ƣ����ַ�%d��"), cbBrightCardTime==5?1:cbBrightCardTime );
		else
			sprintf(szMsg, TEXT("[ %s ] ���ѡ�������ƣ����ַ�%d��"), m_pITableFrame->GetServerUserItem( wChairID )->GetAccounts(), cbBrightCardTime==5?1:cbBrightCardTime );

		SendGameMessage( INVALID_CHAIR, szMsg );
	}

	//��������
	if ( m_bCallScorePhase != CSD_BRIGHTCARD )
	{
		ASSERT( cbBrightCardTime == BRIGHT_CARD_TIME );
		if ( cbBrightCardTime != BRIGHT_CARD_TIME ) return true;

		//�����ж�
		m_cbBrightCardTime = m_cbBrightCardTime > cbBrightCardTime ? m_cbBrightCardTime : cbBrightCardTime;

		//���ñ���
		m_bUserBrightCard[wChairID] = cbBrightCardTime > 0 ? true:false;

		//������Ϣ
		CMD_S_BrightCard BrightCard;
		ZeroMemory( &BrightCard, sizeof( BrightCard ) );
		BrightCard.wBrightUser = wChairID;
		BrightCard.cbBrightTime = cbBrightCardTime;
		BrightCard.cbCurrenBrightTime = m_cbBrightCardTime;
		BrightCard.cbCallScorePhase = CSD_NORMAL;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BRIGHT_CARD,&BrightCard,sizeof(BrightCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_BRIGHT_CARD,&BrightCard,sizeof(BrightCard));
	}
	else
	{
		//Ч�����
		ASSERT((cbBrightCardTime==5)||(cbBrightCardTime==255));
		if ((cbBrightCardTime!=5)&&(cbBrightCardTime!=255)) return false;

		//�����ж�
		if ( m_bUserBrightCard[wChairID] ) return true;

		//���Ӵ���
		m_cbBrightCardCount++;
		if ( m_cbBrightCardCount == GAME_PLAYER )
		{
			m_bCallScorePhase = CSD_GAMESTART;
		}
		else 
		{
			m_bCallScorePhase = CSD_BRIGHTCARD;
		}

		//���ñ���
		if ( cbBrightCardTime == 5 )
		{
			m_bLandScore++;
			m_bUserBrightCard[wChairID] = true;
		}

		//������Ϣ
		CMD_S_BrightCard BrightCard;
		ZeroMemory( &BrightCard, sizeof( BrightCard ) );
		BrightCard.wBrightUser = wChairID;
		BrightCard.cbBrightTime = cbBrightCardTime;
		BrightCard.cbCurrenBrightTime = m_cbBrightCardTime;
		BrightCard.cbCallScorePhase = CSD_BRIGHTCARD;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_BRIGHT_CARD,&BrightCard,sizeof(BrightCard));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_BRIGHT_CARD,&BrightCard,sizeof(BrightCard));

		//�����ж�
		if ( m_bCallScorePhase==CSD_GAMESTART )
		{
			//����״̬
			m_pITableFrame->SetGameStatus(GS_WK_DOUBLE_SCORE);

			//��ȡ·��
			CHAR szDirectory[128];
			GetCurrentDirectory(sizeof( szDirectory ), szDirectory );

			//�ļ�����
			CHAR szProfileName[128];
			sprintf(szProfileName, TEXT("%s\\LandCrazy.ini"), szDirectory);

			//��ȡ����
			LONGLONG lDoubleLimit = GetPrivateProfileInt(TEXT("DoubleScore"), TEXT("Score"), 10000, szProfileName );

			bool bAllowDouble = true;
			for ( WORD wUserID = 0; wUserID < GAME_PLAYER; ++wUserID )
			{
				//��ȡ���
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem( wUserID );
				if ( pIServerUserItem == NULL ) continue;

				//�����ж�
				LONGLONG lUserScore = pIServerUserItem->GetUserScore()->lGold;
				if ( lUserScore < lDoubleLimit )
				{
					bAllowDouble = false;
					break;
				}
			}
			m_bAllowDoubleScore = bAllowDouble;

			//���͵���
			m_bCardCount[m_wBankerUser]=20;
			CopyMemory(&m_bHandCardData[m_wBankerUser][17],m_bBackCard,sizeof(m_bBackCard));
			m_GameLogic.SortCardList(m_bHandCardData[m_wBankerUser],m_bCardCount[m_wBankerUser],ST_ORDER);

			//������Ϣ
			CMD_S_DoubleScore DoubleScore;
			ZeroMemory( &DoubleScore, sizeof( DoubleScore ) );
			DoubleScore.bAllowDouble = bAllowDouble;
			DoubleScore.wLandUser = m_wBankerUser;
			DoubleScore.bCurrentScore = m_bLandScore;
			CopyMemory(DoubleScore.bBackCard,m_bBackCard,sizeof(m_bBackCard));

			//������Ϣ
			m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_DOUBLE_SCORE, &DoubleScore, sizeof( DoubleScore ) );

			//���ñ���
			if (m_bLandScore==0) m_bLandScore=1;
			if (m_wBankerUser==INVALID_CHAIR) m_wBankerUser=m_wFirstUser;

			return true;
		}

	}

	return true;
}

//��Ҽӱ�
bool CTableFrameSink::OnUserDoubleScore( WORD wChairID, bool bDoubleScore, WORD bDoubleType)
{
	//�ӱ��ж�
	//if ( bDoubleScore ) m_bLandScore++;

	////������Ϣ
	//if ( bDoubleScore ) 
	//{
	//	//CHAR szMsg[128] ;
	//	//if (m_pGameServiceOption->cbHideUserInfo)
	//	//	//sprintf(szMsg, TEXT("���ѡ���˷��������ַ�1��"));
	//	//else
	//	//{
	//	//	IServerUserItem *pUserItem = m_pITableFrame->GetServerUserItem( wChairID );
	//	//	if (NULL == pUserItem)
	//	//	{
	//	//		return true;
	//	//	}
	//	//	tagServerUserData *pUserData = pUserItem->GetUserData();
	//	//	if (NULL != pUserData)
	//	//	{
	//	//		//sprintf(szMsg, TEXT("[ %s ] ���ѡ���˷��������ַ�1��"), pUserData->szNickName );
	//	//	}
	//	//}

	//	//SendGameMessage( INVALID_CHAIR, szMsg );
	//}
	//if (bDoubleScore)
	//{
	//	if (m_wBankerUser==wChairID)
	//	{
	//		for (int i=0; i<GAME_PLAYER;++i)
	//		{
	//			if (i!=m_wBankerUser)
	//			{
	//				if (m_bDoubleUser[i])
	//				{
	//					m_bUserBeiShu[i] *= 2;
	//				}
	//			}
	//		}
	//	}
	//	else
	//	{
	//		m_bDoubleUser[wChairID]=bDoubleScore;
	//		m_bUserBeiShu[wChairID] *= 2;
	//	}
	//}

	//m_bUserBeiShu[m_wBankerUser] = 0;// ������������ũ��֮��
	//for (int i=0; i<GAME_PLAYER;++i)
	//{
	//	if (i!=m_wBankerUser)
	//	{
	//		m_bUserBeiShu[m_wBankerUser] += m_bUserBeiShu[i];
	//	}
	//}
	//���ñ���
	m_bFinishDoubleUser[wChairID] = true;
	m_bDoubleUser[wChairID] = false;
	if (bDoubleScore) 
	{
		m_bDoubleUser[wChairID] = true;
	}

	//������Ϣ 
	CMD_S_UserDouble UserDouble;
	ZeroMemory( &UserDouble, sizeof( UserDouble ) );
	UserDouble.bCurrentScore = m_bLandScore;
	UserDouble.bDoubleScore = bDoubleScore;
	UserDouble.wDoubleUser = wChairID;
	UserDouble.bDoubleType=bDoubleType;
	/*::memcpy(UserDouble.bBeiShu, m_bUserBeiShu, sizeof(m_bUserBeiShu));*/
	BYTE bNextUser = (wChairID+1)%GAME_PLAYER;
	UserDouble.bNextUser = bNextUser;
	if (wChairID==m_wBankerUser)
	{
		UserDouble.bState = 0; //�����ӱ�
	}
	else
	{
		if (bNextUser == m_wBankerUser)
		{
			UserDouble.bState = 3; //��
		}
		else
		{
			UserDouble.bState = 2; //��
		}
	}

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_DOUBLE,&UserDouble,sizeof(UserDouble));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_DOUBLE,&UserDouble,sizeof(UserDouble));

	/*cxf ������ ����ӱ�
	//ͳ�ƴ���
	m_cbDoubleCount++;

	//�����ж�
	if ( m_cbDoubleCount == GAME_PLAYER )
	{
	//����״̬
	m_pITableFrame->SetGameStatus(GS_WK_PLAYING);	

	//������Ϣ
	m_bTurnCardCount=0;
	m_wTurnWiner=m_wBankerUser;
	m_wCurrentUser=m_wBankerUser;

	//������Ϣ
	CMD_S_GameStart GameStart;
	GameStart.wLandUser=m_wBankerUser;
	GameStart.bLandScore=m_bLandScore;
	GameStart.bBrightCard=m_bBrightCard;
	GameStart.wCurrentUser=m_wCurrentUser;
	CopyMemory(GameStart.bBackCardData, m_bBackCard, 3);
	CopyMemory(GameStart.bUserBrightCard, m_bUserBrightCard, sizeof(GameStart.bUserBrightCard));
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}
	*/
	return true;
}

//������Ϣ
void CTableFrameSink::SendGameMessage( WORD wChairID, CHAR szMsg[] )
{
	if ( wChairID == INVALID_CHAIR )
	{
		//��Ϸ���
		for ( WORD wUserIdx = 0; wUserIdx < GAME_PLAYER; ++wUserIdx )
		{
			IServerUserItem *pUserItem = m_pITableFrame->GetServerUserItem( wUserIdx );
			m_pITableFrame->SendGameMessage( pUserItem, szMsg, SMT_INFO );
		}

		//�Թ����
		WORD wLookonIdx = 0;
		IServerUserItem *pLookonUserItem = m_pITableFrame->EnumLookonUserItem(wLookonIdx);
		while( pLookonUserItem != NULL )
		{
			wLookonIdx++;
			m_pITableFrame->SendGameMessage( pLookonUserItem, szMsg, SMT_INFO );
			pLookonUserItem = m_pITableFrame->EnumLookonUserItem(wLookonIdx);
		}

	}
	else
	{
		IServerUserItem *pUserItem = m_pITableFrame->GetServerUserItem( wChairID );
		m_pITableFrame->SendGameMessage( pUserItem, szMsg, SMT_INFO );
	}
}

////===============================================================================
//����ID
#define					LANDCRAZY_TASK_ID_1			11101                  //��ʤ��������
#define					LANDCRAZY_TASK_ID_2			11102                  //���˳�Ӳ�������
#define					LANDCRAZY_TASK_ID_3			11103                  //������Ӳ�������
#define					LANDCRAZY_TASK_ID_4			11104                  //���24�����ϲ�������
#define					LANDCRAZY_TASK_ID_5			11105                  //�������3��������
#define					LANDCRAZY_TASK_ID_6			11106                  //�������9��������
#define					LANDCRAZY_TASK_ID_7			11107                  //�������K��������
#define					LANDCRAZY_TASK_ID_8			11108                  //�������A��������
#define					LANDCRAZY_TASK_ID_9			11109                  //�����������������
#define					LANDCRAZY_TASK_ID_10		11110                  //���12�����ϲ�������
#define					LANDCRAZY_TASK_ID_11		11201                  //�����A��˳�Ӳ�������
#define					LANDCRAZY_TASK_ID_12		11202                  //���8�ŵ�˳�Ӳ�������
#define					LANDCRAZY_TASK_ID_13		11203                  //�����J��������
#define					LANDCRAZY_TASK_ID_14		11204                  //���3��N��������
#define					LANDCRAZY_TASK_ID_15		11205                  //��������������
#define					LANDCRAZY_TASK_ID_16		11206                  //���48�����ϲ�������
#define					LANDCRAZY_TASK_ID_17		11207                  //�����4��������
#define					LANDCRAZY_TASK_ID_18		11208                  //�����Q��������
#define					LANDCRAZY_TASK_ID_19		11209                  //�����3��˳�Ӳ�������
#define					LANDCRAZY_TASK_ID_20		11210                  //���3��3��������
#define					LANDCRAZY_TASK_ID_21		11301                  //����ץ����ͬ
#define					LANDCRAZY_TASK_ID_22		11302                  //����ץ����2
#define					LANDCRAZY_TASK_ID_23		11303                  //����ץ����
#define					LANDCRAZY_TASK_ID_24		11304                  //���12�ŵ�˳�Ӳ�������
#define					LANDCRAZY_TASK_ID_25		11305                  //������Բ�������
#define					LANDCRAZY_TASK_ID_26		11306                  //�����8�����Բ�������
#define					LANDCRAZY_TASK_ID_27		11307                  //����ɻ���������
#define					LANDCRAZY_TASK_ID_28		11308                  //���ը����������
#define					LANDCRAZY_TASK_ID_29		11309                  //�����Q��ը����������
#define					LANDCRAZY_TASK_ID_30		11310                  //���96�����ϲ�������
////===============================================================================
//
//
//
//
//
////��������ж�
//void CTableFrameSink::OnUserTaskJudgement(WORD wChairID, WORD wBandID, BYTE btTag)
//{
//	//wChairID:���ID; wBandID:��ǰׯ��ID; btTag:1:Ӯ,0:��.
//	if (wChairID>=GAME_PLAYER || wBandID>=GAME_PLAYER || btTag>1)
//	{
//		return;
//	}
//	//���Ȼ�ȡwChairID����������.
//	IServerUserItem *pUserItem = m_pITableFrame->GetServerUserItem( wChairID );
//	if (NULL == pUserItem)
//	{
//		return;
//	}
//	tagServerUserData *pUserData = pUserItem->GetUserData();
//	if (NULL == pUserData)
//	{
//		return;
//	}
//	//pUserData->task.iTaskID:����ID
//	if (1==btTag)
//	{//wChairIDӮ��
//		bool bIsWin = false;
//		switch(pUserData->task.iTaskID)
//		{
//		case LANDCRAZY_TASK_ID_1:	//����1: �õ������������ƾ���ʤ��
//			{
//				if (wChairID==wBandID)
//				{//�������.
//					bIsWin = true;
//				}
//			}
//			break;
//		case LANDCRAZY_TASK_ID_2:	//����2: ����Ϸ�д��ը������ʤ��
//			{
//				if (true==m_UserTaskInfo[wChairID].bIsOutBombWin)
//				{//�������.
//					bIsWin = true;
//				}
//			}
//			break;
//		case LANDCRAZY_TASK_ID_3:	//����3: ����Ϸ�д���������ʤ��
//			{
//				if (true==m_UserTaskInfo[wChairID].bIsOutMissile)
//				{//�������.
//					bIsWin = true;
//				}
//			}
//			break;
//		case LANDCRAZY_TASK_ID_4:	//����4: ��ͬһ����Ϸ�г�����������ը������ʤ��
//			{
//				if (m_UserTaskInfo[wChairID].bRegBombNums>=2)
//				{//�������
//					bIsWin = true;
//				}
//			}
//			break;
//		case LANDCRAZY_TASK_ID_5:	//����5: �ڶ���������������ʤ
//			{
//				++m_UserTaskInfo[wChairID].iRegJointWinNums;
//				pUserData->task.count = m_UserTaskInfo[wChairID].iRegJointWinNums;
//				m_pITableFrame->PefermTaskCount(wChairID);
//				if (m_UserTaskInfo[wChairID].iRegJointWinNums>=3)
//				{//�������
//					bIsWin = true;
//				}
//			}
//			break;
//		case LANDCRAZY_TASK_ID_6:	//����6: �ڶ����������������õ���������������ʤ
//			{
//				++m_UserTaskInfo[wChairID].iRegJointWinNums;
//				pUserData->task.count = m_UserTaskInfo[wChairID].iRegJointWinNums;
//				m_pITableFrame->PefermTaskCount(wChairID);
//				if (m_UserTaskInfo[wChairID].iRegJoineLandNums>=3 && m_UserTaskInfo[wChairID].iRegJointWinNums>=3)
//				{//�������
//					bIsWin = true;
//				}
//			}
//			break;
//		case LANDCRAZY_TASK_ID_7:	//����7: �ڶ�����������10�λ�ʤ
//			{
//				++m_UserTaskInfo[wChairID].iRegJointWinNums;
//				pUserData->task.count = m_UserTaskInfo[wChairID].iRegJointWinNums;
//				m_pITableFrame->PefermTaskCount(wChairID);
//				if (m_UserTaskInfo[wChairID].iRegJointWinNums>=10)
//				{//�������
//					bIsWin = true;
//				}
//			}
//			break;
//		default:
//			break;
//		}
//		if (true == bIsWin)
//		{//�������.
//			//�ύ�������
//			m_pITableFrame->PefermTaskFinish(wChairID);
//			//���������Ϣ
//			memset(&m_UserTaskInfo[wChairID], 0, sizeof(tag_Server_TaskInfo));
//		}
//	}
//	else
//	{//����
//		switch(pUserData->task.dwID)
//		{
//		case LANDCRAZY_TASK_ID_5:	//����5: �ڶ���������������ʤ
//		case LANDCRAZY_TASK_ID_6:	//����6: �ڶ����������������õ���������������ʤ
//		case LANDCRAZY_TASK_ID_7:	//����7: �ڶ�����������10�λ�ʤ
//			{
//				//������Ӯ�Ĵ������㡣
//				m_UserTaskInfo[wChairID].iRegJointWinNums = 0;
//				pUserData->task.count = 0;
//				m_pITableFrame->PefermTaskCount(wChairID);
//			}
//			break;
//		default:
//			break;
//		}
//	}
//	
//}
////////////////////////////////////////////////////////////////////////////
//��������ж�
int CTableFrameSink::OnUserTaskJudgement(WORD wChairID, WORD wBandID)
{
	//wChairID:���ID; wBandID:��ǰׯ��ID;
	if (wChairID>=GAME_PLAYER || wBandID>=GAME_PLAYER )
	{
		return 0;
	}

	//���Ȼ�ȡwChairID����������.
	IServerUserItem *pUserItem = m_pITableFrame->GetServerUserItem( wChairID );
	if (NULL == pUserItem)
	{
		return 0;
	}
	tagServerUserData *pUserData = pUserItem->GetUserData();
	if (NULL == pUserData)
	{
		return 0;
	}
	bool bIsFinish=false;
	switch (pUserData->task.iTaskID)
	{
	case LANDCRAZY_TASK_ID_22: //����ץ����2��
		{
			if (m_tagTask[wChairID] & CT_TASK_DP_TWO)
			{
				bIsFinish=true;
			}
		}
		break;
	case LANDCRAZY_TASK_ID_23: //����ץ������
		{
			if (m_tagTask[wChairID] & CT_TASK_DP_KING)
			{
				bIsFinish=true;
			}
		}
		break;
	case LANDCRAZY_TASK_ID_21: //����ץ����ͬ��
		{
			if (m_tagTask[wChairID] & CT_TASK_DP_SAME)
			{
				bIsFinish=true;
			}
		}

	case LANDCRAZY_TASK_ID_1: //��ʤ�������ƣ�
		{
			if (m_tagTask[wChairID] & CT_TASK_WIN && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
			{
				bIsFinish=true;
			}
		}
		break;
	case LANDCRAZY_TASK_ID_2: //���˳�Ӳ������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_SHUN && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	case LANDCRAZY_TASK_ID_19: //�����3��˳�Ӳ������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_SHUN_3 && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	case LANDCRAZY_TASK_ID_11: //�����A��˳�Ӳ������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_SHUN_A && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	case LANDCRAZY_TASK_ID_12://���8�ŵ�˳�Ӳ������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_SHUN_8 && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	case LANDCRAZY_TASK_ID_24://���12�ŵ�˳�Ӳ������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_SHUN_12 && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	case LANDCRAZY_TASK_ID_3: //������Ӳ������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_DOUBLE && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	case LANDCRAZY_TASK_ID_13: //�����J�������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_DOUBLE_J && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	case LANDCRAZY_TASK_ID_17: //�����4�������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_DOUBLE_4 && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	case LANDCRAZY_TASK_ID_18: //�����Q�������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_DOUBLE_Q && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	case LANDCRAZY_TASK_ID_25: //������Բ������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_LIANDUI && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	case LANDCRAZY_TASK_ID_26: //�����8�����Բ������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_LIANDUI_8 && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}

		break;
	case LANDCRAZY_TASK_ID_14: //���3��N�������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_THREE_N && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	case LANDCRAZY_TASK_ID_27: //����ɻ��������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_FEIJI && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	case LANDCRAZY_TASK_ID_28: //���ը���������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_ZHADAN && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	case LANDCRAZY_TASK_ID_15: //�������������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_HUOJIAN && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	case LANDCRAZY_TASK_ID_29: //�����Q��ը���������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_ZHADAN_Q && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	case LANDCRAZY_TASK_ID_20: //�������3�������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_THREE_3 && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	case LANDCRAZY_TASK_ID_10: //���12�����ϲ������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_BEI_12 && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	case LANDCRAZY_TASK_ID_4: //���24�����ϲ������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_BEI_24 && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	case LANDCRAZY_TASK_ID_16: //���48�����ϲ������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_BEI_48 && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	case LANDCRAZY_TASK_ID_30: //���96�����ϲ������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_BEI_96 && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	case LANDCRAZY_TASK_ID_5: //�������3�������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_SINGLE_3 && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	case LANDCRAZY_TASK_ID_6: //�������9�������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_SINGLE_9 && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	case LANDCRAZY_TASK_ID_8: //�������A�������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_SINGLE_A && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	case LANDCRAZY_TASK_ID_7: //�������K�������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_SINGLE_K && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	case LANDCRAZY_TASK_ID_9: //����������������ƣ�
		if (m_tagTask[wChairID] & CT_TASK_SINGLE_KING && m_tagTask[wChairID] &CT_TASK_CARD_ZERO)
		{
			bIsFinish=true;
		}
		break;
	default:
		break;
	}

	int iEncourageGem=0;
	bool iServerFinish=false;
	if (true == bIsFinish)
	{//�������.
		if (m_pGameServiceOption->wSortID==1)
		{//һ�����䱦ʯ����
			iEncourageGem=pUserData->task.iLargessCount1;
		}
		else if (m_pGameServiceOption->wSortID==2)
		{//�������䱦ʯ����
			iEncourageGem=pUserData->task.iLargessCount2;
		}
		else if (m_pGameServiceOption->wSortID==3)
		{//�������䱦ʯ����
			iEncourageGem=pUserData->task.iLargessCount3;
		}
		else //�ļ����䱦ʯ����
		{
			iEncourageGem=pUserData->task.iLargessCount4;
		}
	}
	//�ύ�������
	m_pITableFrame->QuestTaskFinish(wChairID,iEncourageGem);
	return iEncourageGem;	
}
