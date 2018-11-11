#include "StdAfx.h"
#include "TableFrameSink.h"
#include "..\..\������\Include\GlobalChannel.h"
//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//��ʼģʽ

//�˿�����
BYTE const n_cbCardDataArray[27]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
};



//Open sendcard tool
//#define			TOOL_SEND_CARD		

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	m_pRoomLog = NULL;

	//��Ϸ����
	m_wSiceCount=MAKEWORD(1,1);
	m_wMPIndex=0;
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));


	//������Ϣ
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//������Ϣ
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//���б���
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//״̬����
	m_bSendStatus=false;
	m_bGangStatus=false;
	ZeroMemory(m_bHearStatus,sizeof(m_bHearStatus));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//������Ϣ
	m_cbChiHuCard=0;
	m_wHaiDiUser=INVALID_CHAIR;
	ZeroMemory(&m_ChiHuResult,sizeof(m_ChiHuResult));

	//�������
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	//���Ͽ���
	memset(m_tagUserGSKUResult, 0, sizeof(m_tagUserGSKUResult));



	//osc update
	memset(m_bIsUserFirstOutCard, true, sizeof(m_bIsUserFirstOutCard));
	memset(m_bUserFirstIsTingPai, 0, sizeof(m_bUserFirstIsTingPai));
	memset(m_byUserHuCardType, 0, sizeof(m_byUserHuCardType));
	m_iSaveGameOutCardCount = 0;
	m_iSaveGameSendCardCount = 0;
	m_byGameEndType = 0;

	m_bIsPlayDice=false;

	//::srand((unsigned int)time(NULL));
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

						m_pRoomLog->AddMsg(0, 0, TRUE, "***** ���ǡ������齫�� ��Ϸ�������־");
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
	m_wSiceCount=MAKEWORD(1,1);
	m_wMPIndex=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));

	//������Ϣ
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//������Ϣ
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//���б���
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//״̬����
	m_bSendStatus=false;
	m_bGangStatus=false;
	ZeroMemory(m_bHearStatus,sizeof(m_bHearStatus));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//������Ϣ
	m_cbChiHuCard=0;
	m_wHaiDiUser=INVALID_CHAIR;
	ZeroMemory(&m_ChiHuResult,sizeof(m_ChiHuResult));

	//���Ͽ���
	memset(m_tagUserGSKUResult, 0, sizeof(m_tagUserGSKUResult));


	//osc update
	memset(m_bIsUserFirstOutCard, true, sizeof(m_bIsUserFirstOutCard));
	memset(m_bUserFirstIsTingPai, 0, sizeof(m_bUserFirstIsTingPai));
	memset(m_byUserHuCardType, 0, sizeof(m_byUserHuCardType));
	m_iSaveGameOutCardCount = 0;
	m_iSaveGameSendCardCount = 0;
	m_byGameEndType = 0;
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


//��ʼ������
void CTableFrameSink::InitData()
{
	//��Ϸ����
	m_wSiceCount=MAKEWORD(1,1);
	m_wMPIndex=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));

	//������Ϣ
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//������Ϣ
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//���б���
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//״̬����
	m_bSendStatus=false;
	m_bGangStatus=false;
	ZeroMemory(m_bHearStatus,sizeof(m_bHearStatus));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//����˿�
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//������Ϣ
	m_cbChiHuCard=0;
	m_wHaiDiUser=INVALID_CHAIR;
	ZeroMemory(&m_ChiHuResult,sizeof(m_ChiHuResult));

	//���Ͽ���
	memset(m_tagUserGSKUResult, 0, sizeof(m_tagUserGSKUResult));

	//osc update
	memset(m_bIsUserFirstOutCard, true, sizeof(m_bIsUserFirstOutCard));
	memset(m_bUserFirstIsTingPai, 0, sizeof(m_bUserFirstIsTingPai));
	memset(m_byUserHuCardType, 0, sizeof(m_byUserHuCardType));
	m_iSaveGameOutCardCount = 0;
	m_iSaveGameSendCardCount = 0;
	m_byGameEndType = 0;

	memset(m_cbKingCard, 0, sizeof(m_cbKingCard));
	memset(m_cbKingCount, 0, sizeof(m_cbKingCount));
	m_cbMoCard = 0;
	m_wCurrentUser = INVALID_CHAIR;


	memset(m_byGangCard, 0, sizeof(m_byGangCard));
	memset(m_byGangCardNums, 0, sizeof(m_byGangCardNums));
	memset(m_byBuCard, 0, sizeof(m_byBuCard));
	memset(m_byBuCardNums, 0, sizeof(m_byBuCardNums));

	m_bIsPlayDice=true;

}
//��Ϸ��ʼ
bool __cdecl CTableFrameSink::OnEventGameStart()
{
	//����״̬
	m_pITableFrame->SetGameStatus(GS_MJ_PLAY);

	InitData();

#ifdef TOOL_SEND_CARD
	char chDirWork[1024]={0};
	//���ù���Ŀ¼
	GetModuleFileName(GetModuleHandle(NULL),chDirWork,sizeof(chDirWork));
	CString strDirWorkPath="";
	strDirWorkPath.ReleaseBuffer();
	strDirWorkPath.Append(chDirWork);
	int nPos = 0; 
	nPos=strDirWorkPath.ReverseFind('\\'); 
	strDirWorkPath=strDirWorkPath.Left(nPos);
	strDirWorkPath.Append("\\MJCard.ini");

	m_SendCardTool.SetFilePath(strDirWorkPath);

#endif


	if (m_wBankerUser<0 || m_wBankerUser>=GAME_PLAYER)
	{
		m_wBankerUser = rand()%GAME_PLAYER;
	}

	//�����˿�
	int iNum1=0,iNum2=0;
#ifdef TOOL_SEND_CARD
	m_SendCardTool.GetSZNums(iNum1, iNum2);
#endif
	if (0==iNum1 || 0==iNum2)
	{
		iNum1 = rand()%6+1;
		iNum2 = rand()%6+1;
	}
	m_wSiceCount=MAKEWORD(iNum1,iNum2);//���������
	m_cbLeftCardCount=CountArray(m_cbRepertoryCard);
	m_GameLogic.RandCardData(m_cbRepertoryCard,CountArray(m_cbRepertoryCard));


	
//#ifdef _DEBUG
	//test 
    //m_cbRepertoryCard[0] = 0x08;
	//m_cbRepertoryCard[1] = 0x26;
	//m_cbRepertoryCard[2] = 0x26;
	//m_cbRepertoryCard[3] = 0x26;

	//m_cbRepertoryCard[MAX_REPERTORY - 73] = 0x08;
	//m_cbRepertoryCard[MAX_REPERTORY - 72] = 0x08;
	//m_cbRepertoryCard[MAX_REPERTORY - 71] = 0x08;
	//m_cbRepertoryCard[MAX_REPERTORY - 70] = 0x08;
	//m_cbRepertoryCard[MAX_REPERTORY - 69] = 0x08;
	//m_cbRepertoryCard[MAX_REPERTORY - 68] = 0x08;
	//m_cbRepertoryCard[MAX_REPERTORY - 67] = 0x08;
	//m_cbRepertoryCard[MAX_REPERTORY - 66] = 0x08;
	//m_cbRepertoryCard[MAX_REPERTORY - 65] = 0x08;
	//m_cbRepertoryCard[MAX_REPERTORY - 64] = 0x28;
 //   m_cbRepertoryCard[MAX_REPERTORY - 63] = 0x26;
	//m_cbRepertoryCard[MAX_REPERTORY - 62] = 0x26;
	//m_cbRepertoryCard[MAX_REPERTORY - 61] = 0x26;
	//m_cbRepertoryCard[MAX_REPERTORY - 60] = 0x26;
	//m_cbRepertoryCard[MAX_REPERTORY - 59] = 0x08;
	//m_cbRepertoryCard[MAX_REPERTORY - 58] = 0x08;
	//m_cbRepertoryCard[MAX_REPERTORY - 57] = 0x02;
	//m_cbRepertoryCard[MAX_REPERTORY - 56] = 0x02;
	//m_cbRepertoryCard[MAX_REPERTORY - 55] = 0x02;
	//m_cbRepertoryCard[MAX_REPERTORY - 54] = 0x02;
	//m_cbRepertoryCard[MAX_REPERTORY - 53] = 0x02;

	//m_cbRepertoryCard[MAX_REPERTORY - 52] = 0x18;
	//m_cbRepertoryCard[MAX_REPERTORY - 51] = 0x17;
	//m_cbRepertoryCard[MAX_REPERTORY - 50] = 0x16;
	//m_cbRepertoryCard[MAX_REPERTORY - 49] = 0x11;
	//m_cbRepertoryCard[MAX_REPERTORY - 48] = 0x11;
	//m_cbRepertoryCard[MAX_REPERTORY - 47] = 0x11;
	//m_cbRepertoryCard[MAX_REPERTORY - 46] = 0x05;
	//m_cbRepertoryCard[MAX_REPERTORY - 45] = 0x26;
	//m_cbRepertoryCard[MAX_REPERTORY - 44] = 0x26;
	//m_cbRepertoryCard[MAX_REPERTORY - 43] = 0x26;
	//m_cbRepertoryCard[MAX_REPERTORY - 42] = 0x06;
	//m_cbRepertoryCard[MAX_REPERTORY - 41] = 0x05;
	//m_cbRepertoryCard[MAX_REPERTORY - 40] = 0x04;

	//m_cbRepertoryCard[MAX_REPERTORY - 39] = 0x12;
	//m_cbRepertoryCard[MAX_REPERTORY - 38] = 0x12;
	//m_cbRepertoryCard[MAX_REPERTORY - 37] = 0x15;
	//m_cbRepertoryCard[MAX_REPERTORY - 36] = 0x28;
	//m_cbRepertoryCard[MAX_REPERTORY - 35] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 34] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 33] = 0x15;
	//m_cbRepertoryCard[MAX_REPERTORY - 32] = 0x28;
	//m_cbRepertoryCard[MAX_REPERTORY - 31] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 30] = 0x25;
	//m_cbRepertoryCard[MAX_REPERTORY - 29] = 0x08;
	//m_cbRepertoryCard[MAX_REPERTORY - 28] = 0x08;
	//m_cbRepertoryCard[MAX_REPERTORY - 27] = 0x08;

	//m_cbRepertoryCard[MAX_REPERTORY - 70] = 0x26;
	//m_cbRepertoryCard[MAX_REPERTORY - 69] = 0x25;
	//m_cbRepertoryCard[MAX_REPERTORY - 68] = 0x29;
	//m_cbRepertoryCard[MAX_REPERTORY - 67] = 0x23;
	//m_cbRepertoryCard[MAX_REPERTORY - 66] = 0x28;
	//m_cbRepertoryCard[MAX_REPERTORY - 65] = 0x24;
	//m_cbRepertoryCard[MAX_REPERTORY - 64] = 0x21;
	//m_cbRepertoryCard[MAX_REPERTORY - 63] = 0x19;
	//m_cbRepertoryCard[MAX_REPERTORY - 62] = 0x21;
	//m_cbRepertoryCard[MAX_REPERTORY - 61] = 0x17;
	//m_cbRepertoryCard[MAX_REPERTORY - 60] = 0x18;
	//m_cbRepertoryCard[MAX_REPERTORY - 59] = 0x24;
	//m_cbRepertoryCard[MAX_REPERTORY - 58] = 0x16;
	//m_cbRepertoryCard[MAX_REPERTORY - 57] = 0x15;
	//m_cbRepertoryCard[MAX_REPERTORY - 56] = 0x14;
	//m_cbRepertoryCard[MAX_REPERTORY - 55] = 0x13;
	//m_cbRepertoryCard[MAX_REPERTORY - 54] = 0x11;
	//m_cbRepertoryCard[MAX_REPERTORY - 53] = 0x09;

	//m_cbRepertoryCard[MAX_REPERTORY - 26] = 0x29;
	//m_cbRepertoryCard[MAX_REPERTORY - 25] = 0x29;
	//m_cbRepertoryCard[MAX_REPERTORY - 24] = 0x29;
	//m_cbRepertoryCard[MAX_REPERTORY - 23] = 0x28;
	//m_cbRepertoryCard[MAX_REPERTORY - 22] = 0x27;
	//m_cbRepertoryCard[MAX_REPERTORY - 21] = 0x26;
	//m_cbRepertoryCard[MAX_REPERTORY - 20] = 0x23;
	//m_cbRepertoryCard[MAX_REPERTORY - 19] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 18] = 0x21;
	//m_cbRepertoryCard[MAX_REPERTORY - 17] = 0x12;
	//m_cbRepertoryCard[MAX_REPERTORY - 16] = 0x13;
	//m_cbRepertoryCard[MAX_REPERTORY - 15] = 0x14;
	//m_cbRepertoryCard[MAX_REPERTORY - 14] = 0x02;

	//m_cbRepertoryCard[MAX_REPERTORY - 13] = 0x02;
	//m_cbRepertoryCard[MAX_REPERTORY - 12] = 0x02;
	//m_cbRepertoryCard[MAX_REPERTORY - 11] = 0x17;
	//m_cbRepertoryCard[MAX_REPERTORY - 10] = 0x17;
	//m_cbRepertoryCard[MAX_REPERTORY - 9] = 0x17;
	//m_cbRepertoryCard[MAX_REPERTORY - 8] = 0x18;
	//m_cbRepertoryCard[MAX_REPERTORY - 7] = 0x18;
	//m_cbRepertoryCard[MAX_REPERTORY - 6] = 0x18;
	//m_cbRepertoryCard[MAX_REPERTORY - 5] = 0x08;
	//m_cbRepertoryCard[MAX_REPERTORY - 4] = 0x08;
	//m_cbRepertoryCard[MAX_REPERTORY - 3] = 0x04;
	//m_cbRepertoryCard[MAX_REPERTORY - 2] = 0x04;
	//m_cbRepertoryCard[MAX_REPERTORY - 1] = 0x04;
//#endif


	//���Ͽ���
	memset(m_tagUserGSKUResult, 0, sizeof(m_tagUserGSKUResult));

	//ȷ������
	int num=rand()%27;
	BYTE byKindCardValue = 0;
#ifdef TOOL_SEND_CARD
	byKindCardValue = m_SendCardTool.GetKingCard();
	m_cbKingCard[0]=byKindCardValue;
#endif
	if (0==byKindCardValue || 255==byKindCardValue)
	{
		m_cbKingCard[0]=n_cbCardDataArray[num];
	}

    if (0 == num)
    {
		m_wMPIndex=8;
    }
	else if (9 == num)
	{
		m_wMPIndex=17;
	}
	else if (18 == num)
	{
		m_wMPIndex=26;
	}
	else
	{
	   m_wMPIndex=num-1;
	}
	//m_cbMoCard=n_cbCardDataArray[m_wMPIndex];
	//m_cbKingCard[0]=0x29;


	//�ַ��˿�
#ifdef TOOL_SEND_CARD
	for (WORD i=0;i<m_wPlayerCount;i++)
	{	
		m_cbLeftCardCount-=(MAX_COUNT-1);
		BYTE byCardData[MAX_COUNT-1]={0};
		int iCardNums = m_SendCardTool.GetCardData(i, byCardData, MAX_COUNT-1);
		if (iCardNums==MAX_COUNT-1)
		{
			m_GameLogic.SwitchToCardIndex(byCardData,MAX_COUNT-1,m_cbCardIndex[i]);
		}
		else 
		{
			m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_cbLeftCardCount],MAX_COUNT-1,m_cbCardIndex[i]);
		}
	}
#else
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbLeftCardCount-=(MAX_COUNT-1);
		m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_cbLeftCardCount],MAX_COUNT-1,m_cbCardIndex[i]);
	}
#endif


#ifdef TOOL_SEND_CARD
	//�ı���ǽ����
	BYTE byWallCard[MAX_REPERTORY]={0};
	int iCardNums = m_SendCardTool.GetCardWallData(byWallCard, MAX_REPERTORY);
	if (iCardNums>0)
	{
		::memcpy(&m_cbRepertoryCard[m_cbLeftCardCount-1], byWallCard, iCardNums);
	}
#endif

	//�����˿�
	BYTE byNextCard=0;
#ifdef TOOL_SEND_CARD
	byNextCard = m_SendCardTool.GetNextCard();
#endif
	--m_cbLeftCardCount;
	if (0==byNextCard || 255==byNextCard)
	{
		byNextCard = m_cbRepertoryCard[m_cbLeftCardCount];
	}
	m_cbSendCardCount++;
	m_cbSendCardData=byNextCard;
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//���ñ���
	m_cbProvideCard=0;
	m_wProvideUser=INVALID_CHAIR;
	m_wCurrentUser=m_wBankerUser;

	//��������
	tagGangCardResult GangCardResult;
	BYTE byGangCard[MAX_INDEX]={0};
	BYTE byGangCardNums = 0;
	//bool bAroseAction=false;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbUserAction[i] = WIK_NULL;
		m_byUserHuCardType[i] = WIK_NULL;
		m_bUserFirstIsTingPai[i] = false;
		m_bIsUserFirstOutCard[i] = true; 

		//ׯ���ж�
		if (i==m_wBankerUser)
		{
			memset(byGangCard, 0, sizeof(byGangCard));
			byGangCardNums = 0;
			//�����ж�
			memset(&GangCardResult, 0, sizeof(tagGangCardResult));
			m_GameLogic.AnalyseGangCard(m_cbCardIndex[i],NULL,0,GangCardResult, m_cbKingCard);
			if (GangCardResult.cbCardCount>=1)
			{
				m_GameLogic.EstimateTingCard(m_cbCardIndex[i], NULL, 0, byGangCard, byGangCardNums, m_cbKingCard);
			}

			if (byGangCardNums>0)
			{
				m_cbUserAction[i] |= WIK_TING;
			}
			if (GangCardResult.cbCardCount>=1)
			{
				m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
			}

			//�����ж�
			tagChiHuResult ChiHuResult;
			BYTE cbReCardIndex[MAX_INDEX];
			m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[i],NULL,0,0,0,ChiHuResult,m_cbKingCard,1,cbReCardIndex);
		}

		////״̬����
		//if ((bAroseAction==false)&&(i!=m_wBankerUser)&&(m_cbUserAction[i]!=WIK_NULL))
		//{
		//	bAroseAction=true;
		//	m_wResumeUser=m_wCurrentUser;
		//	m_wCurrentUser=INVALID_CHAIR;
		//}
	}

	//osc update
	m_wCurrentUser=m_wBankerUser;
	m_wProvideUser=m_wBankerUser;
	m_wResumeUser=m_wCurrentUser;

	//��������
	CMD_S_GameStart GameStart;
	memset(&GameStart, 0, sizeof(CMD_S_GameStart));
	GameStart.wSiceCount=m_wSiceCount;
	GameStart.wMPIndex=m_wMPIndex;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wCurrentUser=m_wCurrentUser;     
	CopyMemory(GameStart.bTrustee,m_bTrustee,sizeof(m_bTrustee));
	GameStart.cbKingCard = m_cbKingCard[0];

	//��������
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//���ñ���
		memset(GameStart.byGangCard, 0, sizeof(GameStart.byGangCard));
		memset(GameStart.byBuCard, 0, sizeof(GameStart.byBuCard));
		GameStart.byGangCardNums = 0;
		GameStart.byBuCardNums = 0;
		GameStart.cbUserAction=m_cbUserAction[i];

		//��ʼ������ֵ
		memset(m_byGangCard[i], 0, sizeof(m_byGangCard[i]));
		m_byGangCardNums[i] = 0;
		memset(m_byBuCard[i], 0, sizeof(m_byBuCard[i]));
		m_byBuCardNums[i] = 0;

		if (i == m_wBankerUser)
		{
			if (byGangCardNums>0 && byGangCardNums<=4)
			{
				memcpy(GameStart.byGangCard, byGangCard, sizeof(BYTE)*byGangCardNums);
				GameStart.byGangCardNums = byGangCardNums;

				memcpy(m_byGangCard[i], byGangCard, sizeof(BYTE)*byGangCardNums);
				m_byGangCardNums[i] = byGangCardNums;
			}
			if (GangCardResult.cbCardCount>=1)
			{
				memcpy(GameStart.byBuCard, GangCardResult.cbCardData, sizeof(BYTE)*GangCardResult.cbCardCount);
				GameStart.byBuCardNums = GangCardResult.cbCardCount;

				memcpy(m_byBuCard[i], GangCardResult.cbCardData, sizeof(BYTE)*GangCardResult.cbCardCount);
				m_byBuCardNums[i] = GangCardResult.cbCardCount;
			}
		}
		//����
		memset(GameStart.cbCardData, 0, sizeof(GameStart.cbCardData));
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameStart.cbCardData);

		//��������
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(CMD_S_GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(CMD_S_GameStart));
	}

	return true;
}

//��Ϸ����
bool __cdecl CTableFrameSink::OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	m_bIsPlayDice=false;
	::memset(m_bTrustee, 0, sizeof(m_bTrustee));
	switch (cbReason)
	{
	case GER_NORMAL:		//�������
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			LONGLONG lCellScore=m_pGameServiceOption->lCellScore;//��Ԫ����
			GameEnd.bIsHuangzhuang = false;

			//������Ϣ
			GameEnd.bKingCard=m_cbKingCard[0];
			GameEnd.wProvideUser=wChairID;
			GameEnd.cbChiHuCard=m_cbChiHuCard;
			GameEnd.cbHaiDiCard=m_cbRepertoryCard[0];
			GameEnd.iChiHuType=m_byGameEndType;
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				GameEnd.wChiHuKind[i]=m_ChiHuResult[i].wChiHuKind;  //��������
				GameEnd.wChiHuRight[i]=m_ChiHuResult[i].wChiHuRight; //����Ȩλ
				GameEnd.cbKingCount[i]=m_ChiHuResult[i].m_bKingCount;  //���Ƹ���
			}

			//��������
			WORD wChiHuUserCount=0;

			//cbChiHuOrder Ϊ����������еķ���

			//ͳ�ƻ���  ������ץ�ڣ��������ͣ������ƣ�
			if (m_wProvideUser!=INVALID_CHAIR && 0!=m_byGameEndType)
			{//���˺���
				//��������
				if ((1==m_byGameEndType || 3==m_byGameEndType) && (m_ChiHuResult[m_wProvideUser].wChiHuKind!=CHK_NULL))
				{//����,����������
					//��������
					BYTE cbChiHuOrder=m_GameLogic.GetChiHuActionRank(m_ChiHuResult[m_wProvideUser]);
					if(0==GameEnd.cbKingCount[m_wProvideUser] || 3==GameEnd.cbKingCount[m_wProvideUser])
					{//������
						cbChiHuOrder+=1;
					}
					else if(4==GameEnd.cbKingCount[m_wProvideUser])
					{//�ĸ���
						cbChiHuOrder+=2;
					}
					if (m_bUserFirstIsTingPai[m_wProvideUser])
					{//��һ�ֱ���
						cbChiHuOrder+=1;
					}
					if (0==cbChiHuOrder) cbChiHuOrder=1;
					cbChiHuOrder++;//������


					//ѭ���ۼ�
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						//Ӯ�ҹ���
						if (i==m_wProvideUser) continue;

						GameEnd.lGameScore[i]-=((i==m_wBankerUser) || (m_wProvideUser==m_wBankerUser))?(2*cbChiHuOrder*lCellScore) : (cbChiHuOrder*lCellScore);
						GameEnd.lGameScore[m_wProvideUser]+=(i==m_wBankerUser || (m_wProvideUser==m_wBankerUser))?(2*cbChiHuOrder*lCellScore) : (cbChiHuOrder*lCellScore);
                        //����ͳ��
						//GameEnd.cbTimes[i]=((i==m_wBankerUser) || (m_wProvideUser==m_wBankerUser))?(-2*cbChiHuOrder) : (-cbChiHuOrder);
                        //GameEnd.cbTimes[m_wProvideUser]+=(i==m_wBankerUser || (m_wProvideUser==m_wBankerUser))?(2*cbChiHuOrder) : (cbChiHuOrder);
						GameEnd.cbTimes[i]=GameEnd.lGameScore[i]/lCellScore;
						GameEnd.cbTimes[m_wProvideUser]=(GameEnd.lGameScore[m_wProvideUser]/lCellScore);
					}
					GameEnd.iChiHuKind[m_wProvideUser]=1;

					//������Ŀ
					wChiHuUserCount=1;

					//ׯ������
					m_wBankerUser=m_wProvideUser;
					
				}

				//׽������
				if (2==m_byGameEndType || 4==m_byGameEndType)
				{//����, m_wProvideUserΪ���ڵ����
					//��������
					WORD wNextBankerUser=INVALID_CHAIR;

					GameEnd.iChiHuKind[m_wProvideUser]=3;

					//ѭ���ۼ�
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						//��ҹ���
						if (m_ChiHuResult[i].wChiHuKind==CHK_NULL) continue;

						//��������
						BYTE cbChiHuOrder=m_GameLogic.GetChiHuActionRank(m_ChiHuResult[i]);
						if(0==GameEnd.cbKingCount[i] || 3==GameEnd.cbKingCount[i])
						{//������
							cbChiHuOrder+=1;
						}
						else if(4==GameEnd.cbKingCount[i])
						{//�ĸ���
							cbChiHuOrder+=2;
						}
						if (m_bUserFirstIsTingPai[i])
						{//��һ�ֱ���
							cbChiHuOrder+=1;
						}
						if (0==cbChiHuOrder) cbChiHuOrder=1;
						
						GameEnd.lGameScore[i]+=((i==m_wBankerUser)||(m_wProvideUser==m_wBankerUser))? (2*cbChiHuOrder*lCellScore) : (cbChiHuOrder*lCellScore);
						GameEnd.lGameScore[m_wProvideUser]-=((i==m_wBankerUser)||(m_wProvideUser==m_wBankerUser))?(2*cbChiHuOrder*lCellScore) : (cbChiHuOrder*lCellScore);
						//GameEnd.cbTimes[m_wProvideUser]=((i==m_wBankerUser)||(m_wProvideUser==m_wBankerUser))? (-2*cbChiHuOrder) : (-cbChiHuOrder);
						//GameEnd.cbTimes[i]=((i==m_wBankerUser)||(m_wProvideUser==m_wBankerUser))?(2*cbChiHuOrder) : (cbChiHuOrder);
						GameEnd.cbTimes[m_wProvideUser]=GameEnd.lGameScore[m_wProvideUser]/lCellScore;
						GameEnd.cbTimes[i]=(GameEnd.lGameScore[i]/lCellScore);
                        GameEnd.iChiHuKind[i]=2;
						//���ñ���
						wNextBankerUser=i;
						wChiHuUserCount++;
					}

					
					//ׯ������
					m_wBankerUser=(wChiHuUserCount>=2)?m_wProvideUser:wNextBankerUser;
					if ((m_wHaiDiUser!=INVALID_CHAIR)&&(wChiHuUserCount>1)) m_wBankerUser=m_wHaiDiUser;
				}

				//�����˿�
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
				}
			}
			else
			{
				//��ׯ����
				GameEnd.bIsHuangzhuang = true;
				GameEnd.cbChiHuCard=0;
				GameEnd.cbHaiDiCard=m_cbRepertoryCard[0];

				//����ׯ��
				m_wBankerUser=(m_wHaiDiUser!=INVALID_CHAIR)?m_wHaiDiUser:m_wResumeUser;

				//�����˿�
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					GameEnd.cbCardCount[i]=0;
				}
			}

			//��������
			LONGLONG lUserScore[GAME_PLAYER]={0};
			ZeroMemory(lUserScore,sizeof(lUserScore));
			CopyMemory(lUserScore,GameEnd.lGameScore,sizeof(lUserScore));

			//д��
			for(int i=0;i<GAME_PLAYER;i++)
			{
				if (lUserScore[i]>m_pGameServiceOption->lRestrictScore)
				{//
					lUserScore[i] = m_pGameServiceOption->lRestrictScore;
				}
				else if (lUserScore[i]<-m_pGameServiceOption->lRestrictScore)
				{
					lUserScore[i] = -m_pGameServiceOption->lRestrictScore;
				}

				GameEnd.iCellScore[i]=0;//˰��
				GameEnd.lGameScore[i]=0;//���
				//д�����
				LONGLONG lScore=lUserScore[i];
				enScoreKind	ScoreKind;
				if(lScore>0L)
				{
					ScoreKind=enScoreKind_Win;
					GameEnd.iCellScore[i] = lScore*(0.01*m_pGameServiceOption->wRevenue);
					GameEnd.lGameScore[i]=lUserScore[i]-GameEnd.iCellScore[i];
				}
				else if(lScore==0L) 	
				{
					    ScoreKind=enScoreKind_Draw;
						GameEnd.lGameScore[i]=lUserScore[i];
				}
				else
				{
					    ScoreKind=enScoreKind_Lost;
						GameEnd.lGameScore[i]=lUserScore[i];
				}
				m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],GameEnd.iCellScore[i],ScoreKind);
			}
			
			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(CMD_S_GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(CMD_S_GameEnd));
			//������Ϸ
			m_pITableFrame->ConcludeGame();

			if (m_wBankerUser>=GAME_PLAYER)
			{
				m_wBankerUser = INVALID_CHAIR;
			}
			return true;
		}
	case GER_USER_LEFT:		//�û�ǿ��
		{
			//��������
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			GameEnd.bIsHuangzhuang = false;

			//��������
			LONGLONG lCellScore=m_pGameServiceOption->lCellScore;	//��Ԫ����
            DOUBLE lRestrictScore=m_pGameServiceOption->lRestrictScore;	//�������
			LONGLONG Revenue = m_pGameServiceOption->wRevenue;	//˰�ձ���
			
			//���ñ���
			GameEnd.wProvideUser=INVALID_CHAIR;

			GameEnd.bKingCard=m_cbKingCard[0];

			//ͳ�ƻ���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.iCellScore[i] = 0;
				if (i==wChairID) 
				{
					GameEnd.lGameScore[i]=-lRestrictScore;
					GameEnd.IsUserLeft[i]=true;
				}
				else 
				{
					GameEnd.iCellScore[i] = (lRestrictScore/3)*(0.01*m_pGameServiceOption->wRevenue);
					GameEnd.lGameScore[i]=lRestrictScore/3 - GameEnd.iCellScore[i];
					GameEnd.IsUserLeft[i]=false;
				}
			}


			//֪ͨ��Ϣ
			TCHAR szMessage[128]=TEXT("");
			_snprintf(szMessage,CountArray(szMessage),TEXT("���� [ %s ] �뿪��Ϸ����Ϸ����"),pIServerUserItem->GetAccounts());
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				IServerUserItem * pISendUserItem=m_pITableFrame->GetServerUserItem(i);
				if (pISendUserItem!=NULL) m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_INFO);
			}
			WORD wIndex=0;
			do
			{
				IServerUserItem * pISendUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
				if (pISendUserItem==NULL) break;
				m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_INFO);
			} while (true);

						
			//�޸Ļ���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (wChairID==i)
				{
					m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],0L,enScoreKind_Flee);
				}
				else
				{
					m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],GameEnd.iCellScore[i],enScoreKind_Win);
				}
			}


			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//������Ϸ
			m_pITableFrame->ConcludeGame();

			m_wBankerUser = INVALID_CHAIR;
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
	case GS_MJ_FREE:	//����״̬
		{
			//��������
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//��������
			StatusFree.wBankerUser=m_wBankerUser;
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusFree.bTrustee,m_bTrustee,sizeof(m_bTrustee));

			//���ͳ���
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_MJ_PLAY:	//��Ϸ״̬
	case GS_MJ_HAI_DI:	//����״̬
		{
			//��������
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			bool bIsMeTingPai = false;
			if (m_bIsUserFirstOutCard[wChiarID] && m_wOutCardUser==wChiarID)
			{
				bool bIsTing = m_GameLogic.IsCanTingCard(m_cbCardIndex[wChiarID], m_WeaveItemArray[wChiarID], m_cbWeaveItemCount[wChiarID], m_cbKingCard);
				if (bIsTing)
				{
					bIsMeTingPai = true;
				}
			}
			StatusPlay.m_bTingCard=bIsMeTingPai;				

			//����
			StatusPlay.byKindCard = m_cbKingCard[0];

			StatusPlay.bIsPlayDice=m_bIsPlayDice;

			//��Ϸ����
			StatusPlay.wSiceCount=m_wSiceCount;
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;
			StatusPlay.bIsAlreadyOper = m_bResponse[wChiarID];

			//״̬����
			StatusPlay.cbActionCard=m_cbProvideCard;
			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;
			StatusPlay.cbHearStatus=(m_bHearStatus[wChiarID]==true)?TRUE:FALSE;
			StatusPlay.cbActionMask=(m_bResponse[wChiarID]==false)?m_cbUserAction[wChiarID]:WIK_NULL;

			//��ʷ��¼
			StatusPlay.wOutCardUser=m_wOutCardUser;
			StatusPlay.cbOutCardData=m_cbOutCardData;
			CopyMemory(StatusPlay.cbDiscardCard,m_cbDiscardCard,sizeof(StatusPlay.cbDiscardCard));
			CopyMemory(StatusPlay.cbDiscardCount,m_cbDiscardCount,sizeof(StatusPlay.cbDiscardCount));
			CopyMemory(StatusPlay.bTrustee,m_bTrustee,sizeof(m_bTrustee));

			//����˿�
			CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(m_WeaveItemArray));
			CopyMemory(StatusPlay.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

			//�˿�����
			StatusPlay.cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[wChiarID],StatusPlay.cbCardData);

			//�ܲ���Ϣ
			memcpy(StatusPlay.byGangCard, m_byGangCard[wChiarID], sizeof(BYTE)*m_byGangCardNums[wChiarID]);
			StatusPlay.byGangCardNums = m_byGangCardNums[wChiarID];
			memcpy(StatusPlay.byBuCard, m_byBuCard[wChiarID], sizeof(BYTE)*m_byBuCardNums[wChiarID]);
			StatusPlay.byBuCardNums = m_byBuCardNums[wChiarID];

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
	switch (wSubCmdID)
	{
	case SUB_C_OUT_CARD:		//������Ϣ
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(CMD_C_OutCard));
			if (wDataSize!=sizeof(CMD_C_OutCard)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			return OnUserOutCard(pUserData->wChairID,pOutCard->cbCardData);
		}
	case SUB_C_OPERATE_CARD:	//������Ϣ
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(CMD_C_OperateCard));
			if (wDataSize!=sizeof(CMD_C_OperateCard)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_OperateCard * pOperateCard=(CMD_C_OperateCard *)pDataBuffer;
			return OnUserOperateCard(pUserData->wChairID,pOperateCard->cbOperateCode,pOperateCard->cbOperateCard);
		}
	case SUB_C_OPERATE_HAI_DI:	//���ײ���
		{
			//Ч����Ϣ
			ASSERT(wDataSize==sizeof(CMD_C_OperateHaiDi));
			if (wDataSize!=sizeof(CMD_C_OperateHaiDi)) return false;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pUserData->cbUserStatus!=US_PLAY) return true;

			//��Ϣ����
			CMD_C_OperateHaiDi * pOperateHaiDi=(CMD_C_OperateHaiDi *)pDataBuffer;
			return OnUserOperateHaiDi(pUserData->wChairID,pOperateHaiDi->cbTakeHaiDi);
		}
	case SUB_C_TRUSTEE:			//�йܲ���
		{
			if(wDataSize != sizeof(CMD_C_Trustee)) return false;
			CMD_C_Trustee *pTrustee =(CMD_C_Trustee *)pDataBuffer;

			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();

			m_bTrustee[pUserData->wChairID]=pTrustee->bTrustee;
			CMD_S_Trustee Trustee;
			Trustee.bTrustee=pTrustee->bTrustee;
			Trustee.wChairID = pUserData->wChairID;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));

			return true;
		}
	case SUB_C_USERTINGCARD:	//���Ʋ���
		{
			if(wDataSize != sizeof(CMD_UserTingOperate)) return false;
			CMD_UserTingOperate *TingOperate =(CMD_UserTingOperate *)pDataBuffer;
			
			//�û�Ч��
			tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			return OnUserTingCard(pUserData->wChairID, TingOperate->bTingCard);
		}
	case SUB_C_SZSTART:
		{
			m_bIsPlayDice=false;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_C_SZSTART);
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_C_SZSTART);
		}
		break;
	default:
		break;
	}

	return false;
}

//�û����Ʋ���
bool CTableFrameSink::OnUserTingCard(WORD wChairID, bool bIsTing)
{
	if (wChairID>=GAME_PLAYER || !m_bIsUserFirstOutCard[wChairID])
	{
		return false;
	}
	
	bool bRetIsTing = m_GameLogic.IsCanTingCard(m_cbCardIndex[wChairID], m_WeaveItemArray[wChairID], m_cbWeaveItemCount[wChairID], m_cbKingCard);
	if (bRetIsTing)
	{
		m_bIsUserFirstOutCard[wChairID] = false;
		m_bUserFirstIsTingPai[wChairID] = bIsTing; //�ǲ��ǵ�һ������.
		m_bHearStatus[wChairID] = bIsTing;

		if (bIsTing)
		{
			//���߿ͻ����û�ĳ����������ˡ�
			CMD_S_GameTingCard userting;
			userting.wTingCardUser = wChairID;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_TINGCARD_RESULT,&userting,sizeof(CMD_S_GameTingCard));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_TINGCARD_RESULT,&userting,sizeof(CMD_S_GameTingCard));
		}
		//�û���ʼ����
		UserStartOperation();
	}
	
	return true;
}

//�����Ϣ����
bool __cdecl CTableFrameSink::OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	////ׯ������
	//if ((bLookonUser==false)&&(m_wBankerUser==INVALID_CHAIR))
	//{
	//	m_wBankerUser=pIServerUserItem->GetChairID();
	//}

	return true;
}

//�û�����
bool __cdecl CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	////ׯ������
	//if ((bLookonUser==false)&&(wChairID==m_wBankerUser))
	//{
	//	m_wBankerUser=INVALID_CHAIR;

	//	for (WORD i=0;i<m_wPlayerCount;i++)
	//	{
	//		if ((i!=wChairID)&&(m_pITableFrame->GetServerUserItem(i)!=NULL))
	//		{
	//			m_wBankerUser=i;
	//			m_bTrustee[wChairID]=false;
	//			CMD_S_Trustee Trustee;
	//			Trustee.bTrustee=false;
	//			Trustee.wChairID = wChairID;
	//			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
	//			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));


	//			break;
	//		}
	//	}
	//}

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData)
{
	//Ч��״̬
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) 
	{
		return true;
	}

	//Ч�����
	if (wChairID!=m_wCurrentUser)
	{
		return false;
	}
	if (m_GameLogic.IsValidCard(cbCardData)==false)
	{
		return false;
	}

	//ɾ���˿�
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//���ñ���
	m_bIsPlayDice=false;
	m_bSendStatus=true;
	m_cbUserAction[wChairID]=WIK_NULL;
	m_cbPerformAction[wChairID]=WIK_NULL;

	//���Ƽ�¼
	m_cbOutCardCount++;
	m_wOutCardUser=wChairID;
	m_cbOutCardData=cbCardData;

	//��������
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbOutCardData=cbCardData;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));

	//�û��л�
	m_wProvideUser=wChairID;
	m_cbProvideCard=cbCardData;
	m_wCurrentUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;

	if (m_bIsUserFirstOutCard[wChairID])
	{//�ǲ��ǵ�һȦ
		bool bIsTing = m_GameLogic.IsCanTingCard(m_cbCardIndex[wChairID], m_WeaveItemArray[wChairID], m_cbWeaveItemCount[wChairID], m_cbKingCard);
		if (bIsTing)
		{
			CMD_S_GameTingCard tingcard;
			tingcard.wTingCardUser = wChairID;
			m_pITableFrame->SendTableData(wChairID,SUB_S_TING_CARD,&tingcard,sizeof(CMD_S_GameTingCard));
			return true;
		}
		else
		{
			m_bIsUserFirstOutCard[wChairID] = false;
		}
	}

	//�û���ʼ����
	UserStartOperation();

	return true;
}

//�û�����
void CTableFrameSink::UserStartOperation()
{
	//��Ӧ�ж�
	bool bAroseAction=EstimateUserRespond(m_wProvideUser,m_cbProvideCard,EstimatKind_OutCard);

	//��������
	if (m_bGangStatus==true)
	{
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if ((m_cbUserAction[i]&WIK_CHI_HU)!=0) break;
		}
		if (i==m_wPlayerCount) m_bGangStatus=false;
	}

	//�ɷ��˿�
	if (bAroseAction==false) 
	{
        m_bSendStatus = true;
		DispatchCardData(m_wCurrentUser);
	}
}

//�û�����
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//Ч���û�
	//ASSERT((wChairID==m_wCurrentUser)||(m_wCurrentUser==INVALID_CHAIR));
	if ((wChairID!=m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR)) return true;

	if (0==cbOperateCode && 0==cbOperateCard)
	{//ȡ����
		m_tagUserGSKUResult[wChairID].bIsGSKU = false;
	}
	

	//��������
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//Ч��״̬
		//ASSERT(m_bResponse[wChairID]==false);
		//ASSERT((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)!=0));

		//Ч��״̬
		if (m_bResponse[wChairID]==true)
			return true;
		if ((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)==0)) 
			return true;

		//��������
		WORD wTargetUser=wChairID;
		BYTE cbTargetAction=cbOperateCode;

		//���ñ���
		m_bResponse[wChairID]=true;
		m_cbPerformAction[wChairID]=cbOperateCode;
		m_cbOperateCard[wChairID]=(cbOperateCard==0)?m_cbProvideCard:cbOperateCard;

		bool bIsOtherGangAction = false;
		//ִ���ж�
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//��ȡ����
			BYTE cbUserAction=(m_bResponse[i]==false)?m_cbUserAction[i]:m_cbPerformAction[i];

			if ((cbUserAction&WIK_TING) > 0)
			{
				bIsOtherGangAction = true;
			}

			//���ȼ���
			BYTE cbUserActionRank=m_GameLogic.GetUserActionRank(cbUserAction);
			BYTE cbTargetActionRank=m_GameLogic.GetUserActionRank(cbTargetAction);

			//�����ж�
			if (cbUserActionRank>cbTargetActionRank)
			{
				wTargetUser=i;//����Ȩ���������
				cbTargetAction=cbUserAction;
			}
		}
		if (m_bResponse[wTargetUser]==false) return true;

		//�Ժ��ȴ�
		if (cbTargetAction==WIK_CHI_HU)
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if ((m_bResponse[i]==false)&&(m_cbUserAction[i]&WIK_CHI_HU)) 
					return true;
			}
		}

		//��������
		if (cbTargetAction==WIK_NULL)
		{
			//�û�״̬
			ZeroMemory(m_bResponse,sizeof(m_bResponse));
			ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
			ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
			ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

			//��ʼ��
			for(int i=0; i<m_wPlayerCount; i++)
			{
				m_cbUserAction[i] = WIK_NULL;
				::memset(&m_byGangCard[i], 0, sizeof(m_byGangCard[i]));
				m_byGangCardNums[i] = 0;
				::memset(&m_byBuCard[i], 0, sizeof(m_byBuCard[i]));
				m_byBuCardNums[i] = 0;
			}

			//�����˿�
            m_bSendStatus = true;
			DispatchCardData(m_wResumeUser);
			return true;
		}
		
		//��������
		BYTE cbTargetCard=m_cbOperateCard[wTargetUser];
		//���Ʋ���
		if (cbTargetAction==WIK_CHI_HU)
		{
			//������Ϣ
			m_cbChiHuCard=cbTargetCard;

			//�����ж�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				BYTE bKingCount=0;
				bKingCount=m_GameLogic.GetKingCardCount(m_cbCardIndex[i],NULL,0,m_cbKingCard,4);
				if (0 != bKingCount)
				{
					continue;
				}
				//�����ж�
				if ((i==m_wProvideUser)||((m_cbPerformAction[i]&WIK_CHI_HU)==0)) continue;

				//����Ȩλ
				WORD wChiHuRight=0;
				if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==1)) m_byUserHuCardType[i]|=CHR_DI;
				if (m_tagUserGSKUResult[m_wOutCardUser].bIsGang 
					&& (m_iSaveGameOutCardCount==m_cbOutCardCount-1)
					&& (m_iSaveGameSendCardCount==m_cbSendCardCount))
				{//������
					m_byUserHuCardType[i]|=CHR_QIANG_GANG;
				}
				if (bIsOtherGangAction)
				{//���ܺ�
					m_byUserHuCardType[i]|=CHR_QIANG_GANG;
				}

				//��ͨ����
				if (m_cbChiHuCard!=0)
				{
					//�����ж�
					BYTE cbReCardIndex[MAX_INDEX];
					BYTE cbWeaveItemCount=m_cbWeaveItemCount[i];
					tagWeaveItem * pWeaveItem=m_WeaveItemArray[i];
					m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,m_byUserHuCardType[i],m_ChiHuResult[i],m_cbKingCard,4,cbReCardIndex);
					
			       //�����˿�
			        if (m_ChiHuResult[i].wChiHuKind!=CHK_NULL) m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbChiHuCard)]++;
				}
			}

			//������Ϸ
			ASSERT(m_ChiHuResult[wTargetUser].wChiHuKind!=CHK_NULL);
			m_byGameEndType = 2; //����
			OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);

			return true;
		}

		//���Ʊ���
		m_cbOutCardData=0;
		m_bSendStatus=true;
		m_wOutCardUser=INVALID_CHAIR;

		//�û�״̬
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction)); 
		ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
		ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

		//����˿�
		ASSERT(m_cbWeaveItemCount[wTargetUser]<4);
		WORD wIndex=m_cbWeaveItemCount[wTargetUser]++;
		m_WeaveItemArray[wTargetUser][wIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wTargetUser][wIndex].cbCenterCard=cbTargetCard;
		m_WeaveItemArray[wTargetUser][wIndex].cbWeaveKind=cbTargetAction;
		m_WeaveItemArray[wTargetUser][wIndex].wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;

		//ɾ���˿�
		switch (cbTargetAction)
		{
		case WIK_LEFT:		//���Ʋ���
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbTargetCard+1,cbTargetCard+2};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;
			}
		case WIK_RIGHT:		//���Ʋ���
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbTargetCard-2,cbTargetCard-1};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;
			}
		case WIK_CENTER:	//���Ʋ���
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbTargetCard-1,cbTargetCard+1};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;
			}
		case WIK_PENG:		//���Ʋ���
			{
				//ɾ���˿�
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

				break;
			}
		case WIK_FILL:		//���Ʋ���
		case WIK_GANG:		//���Ʋ���
		case WIK_TING:		//���Ʋ���
			{
				//ɾ���˿�
				if ((m_cbSendCardCount==1)&&(m_cbOutCardData==0))
				{
					BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard,cbTargetCard,cbTargetCard};
					m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));
				}
				else
				{
					BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard,cbTargetCard};
					m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));
				}

				break;
			}
		}

		//������
		CMD_S_OperateResult OperateResult;
		OperateResult.wOperateUser=wTargetUser;
		OperateResult.cbOperateCard=cbTargetCard;
		OperateResult.cbOperateCode=cbTargetAction;
		OperateResult.wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;

		//����״̬
		if ((cbTargetAction==WIK_GANG)||(cbTargetAction==WIK_FILL) ||(cbTargetAction==WIK_TING))
		{
			m_bGangStatus=true;
		}
		if (cbTargetAction==WIK_TING)
		{
			m_bHearStatus[wTargetUser]=true;
			m_bEnjoinChiPeng[wTargetUser]=true;
			m_tagUserGSKUResult[wTargetUser].bIsGang = true;

			if (m_bIsUserFirstOutCard[wChairID])
			{
				m_bUserFirstIsTingPai[wChairID] = true; //���Է���
			}
			m_bIsUserFirstOutCard[wChairID] = false;
		}

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

		BYTE byStartID = m_wProvideUser;
		while (byStartID<m_wPlayerCount)
		{
			m_bIsUserFirstOutCard[byStartID] = false;
			if (byStartID==wTargetUser)
			{
				break;
			}
			byStartID = (byStartID+m_wPlayerCount-1)%m_wPlayerCount;
		}

		//�����û�
		m_wCurrentUser=wTargetUser;

		//���ƴ���
		if ((cbTargetAction==WIK_GANG)||(cbTargetAction==WIK_FILL) ||(cbTargetAction==WIK_TING))
		{
			//�������ܣ������Ƶ���ҷ�һ����
			m_bSendStatus = true;
			DispatchCardData(wTargetUser);
		}

		
		return true;
	}

	//��������
	if (m_wCurrentUser==wChairID)
	{
		//Ч�����
		if ((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)==0)) return true;

		//�˿�Ч��
		if ((cbOperateCode!=WIK_NULL)&&(cbOperateCode!=WIK_CHI_HU)&&(m_GameLogic.IsValidCard(cbOperateCard)==false))
		{
			return false;  
		}
		
		//���ñ���
		m_bSendStatus=true;
		m_cbUserAction[wChairID]=WIK_NULL;
		m_cbPerformAction[wChairID]=WIK_NULL;

		//ִ�ж���
		switch (cbOperateCode)
		{
		case WIK_FILL:			//���Ʋ���
		case WIK_GANG:			//���Ʋ���
		case WIK_TING:			//���Ʋ���
			{
				//��������
				bool bAnGang = false;		//�ǲ��ǰ���
				BYTE cbWeaveIndex=0xFF;
				BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOperateCard);

				//���ƴ���
				if (m_cbCardIndex[wChairID][cbCardIndex]==1)
				{//�������
					//Ѱ�����
					for (BYTE i=0;i<m_cbWeaveItemCount[wChairID];i++)
					{
						BYTE cbWeaveKind=m_WeaveItemArray[wChairID][i].cbWeaveKind;
						BYTE cbCenterCard=m_WeaveItemArray[wChairID][i].cbCenterCard;
						if ((cbCenterCard==cbOperateCard)&&(cbWeaveKind==WIK_PENG))
						{
							cbWeaveIndex=i;
							break;
						}
					}

					//Ч�鶯��
					if (cbWeaveIndex==0xFF) return false;

					//����˿�
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=TRUE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;
				}
				else
				{//�ܵ����
					//�˿�Ч��
					ASSERT(m_cbCardIndex[wChairID][cbCardIndex]==4);
					if (m_cbCardIndex[wChairID][cbCardIndex]!=4) return false;

					//���ñ���
					cbWeaveIndex=m_cbWeaveItemCount[wChairID]++;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=FALSE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;
					bAnGang = true;
				}

				//ɾ���˿�
				m_cbCardIndex[wChairID][cbCardIndex]=0;

				//����״̬
				m_bGangStatus=true;
				if (WIK_TING == cbOperateCode)
				{//����
					//����״̬
					m_bHearStatus[wChairID]=true;
					m_bEnjoinChiPeng[wChairID]=true;
					m_tagUserGSKUResult[wChairID].bIsGang = true;
					if (m_bIsUserFirstOutCard[wChairID])
					{
						m_bUserFirstIsTingPai[wChairID] = true; //���Է���
					}
					m_bIsUserFirstOutCard[wChairID] = false;
				}

				//������
				CMD_S_OperateResult OperateResult;
				OperateResult.wOperateUser=wChairID;
				OperateResult.wProvideUser=wChairID;
				OperateResult.cbOperateCode=cbOperateCode;
				OperateResult.cbOperateCard=cbOperateCard;

				//������Ϣ
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

				//�����˿�
                m_bSendStatus = true;
				DispatchCardData(wChairID);
				return true;
			}
		case WIK_CHI_HU:		//�Ժ�����
			{
				//����Ȩλ
				if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==0)) m_byUserHuCardType[wChairID]|=CHR_TIAN;
				if ((wChairID == m_wProvideUser) && m_tagUserGSKUResult[wChairID].bIsGang
					&& (m_iSaveGameOutCardCount==m_cbOutCardCount) 
					&& (m_iSaveGameSendCardCount==m_cbSendCardCount))
				{//���Ͽ���
					m_tagUserGSKUResult[wChairID].bIsGSKU = true;
					m_byUserHuCardType[wChairID] |= CHR_QIANG_GANG;
				}

				//��ͨ����
				BYTE cbReCardIndex[MAX_INDEX];
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
				//m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,0,wChiHuRight,m_ChiHuResult[wChairID]);
				m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,0,m_byUserHuCardType[wChairID],m_ChiHuResult[wChairID],m_cbKingCard,4,cbReCardIndex);

				//������Ϣ
				m_cbChiHuCard=m_cbProvideCard;

				//������Ϸ
				m_byGameEndType = 1; //����
				OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);

				return true;
			}
		}

		return true;
	}

	return false;
}

//���ײ���
bool CTableFrameSink::OnUserOperateHaiDi(WORD wChairID, BYTE cbTakeHaiDi)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_HAI_DI);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_HAI_DI) return true;

	//Ч���û�
	ASSERT(wChairID==m_wCurrentUser);
	if (wChairID!=m_wCurrentUser) return false;

	//���״���
	if (cbTakeHaiDi==1)
	{
		//�����ж�
		BYTE cbReCardIndex[MAX_INDEX];
		BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
		tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
		m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,m_cbProvideCard,0,m_ChiHuResult[wChairID],m_cbKingCard,4,cbReCardIndex);


		//����ж�
		if (m_ChiHuResult[wChairID].wChiHuKind==CHK_NULL)
		{
			//�����ж�
			bool bOtherChiHu=false;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//�����ж�
				if (i==wChairID) continue;

				//�����ж�
				pWeaveItem=m_WeaveItemArray[i];
				cbWeaveItemCount=m_cbWeaveItemCount[i];
			    m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbProvideCard,0,m_ChiHuResult[i],m_cbKingCard,4,cbReCardIndex);	

				//�������
				if (m_ChiHuResult[i].wChiHuKind!=CHK_NULL)
				{
					bOtherChiHu=true;
					m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;
					m_ChiHuResult[i].wChiHuRight |= CHR_HAI_DI;
					m_ChiHuResult[i].wChiHuRight |= m_byUserHuCardType[i];
				}
			}

			//������Ϸ
			if (bOtherChiHu==false)
			{
				//��ׯ����
				m_cbChiHuCard=0;
				m_cbLeftCardCount=0;
				m_wHaiDiUser=wChairID;
				m_wProvideUser=INVALID_CHAIR;
				m_byGameEndType = 0; //��ׯ
				OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);
			}
			else
			{
				//���ڽ���
				m_cbLeftCardCount=0;
				m_wHaiDiUser=wChairID;
				m_wProvideUser=wChairID;
				m_cbChiHuCard=m_cbProvideCard;
				m_byGameEndType = 4; //���׷���
				OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);
			}

		}
		else
		{//��Һ���

			m_ChiHuResult[wChairID].wChiHuRight |= CHR_HAI_DI;
			m_ChiHuResult[wChairID].wChiHuRight |= m_byUserHuCardType[wChairID];

			//�����˿�
			m_cbCardIndex[wChairID][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;

			//��������
			m_cbLeftCardCount=0;
			m_wHaiDiUser=wChairID;
			m_wProvideUser=wChairID;
			m_cbChiHuCard=m_cbProvideCard;
			m_byGameEndType = 3; //��������
			OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);
		}
	}
	else
	{
		//�����û�
		m_wCurrentUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;

		//��ׯ�ж�
		if (m_wCurrentUser==m_wResumeUser)
		{
			m_cbChiHuCard=0;
			m_wCurrentUser=INVALID_CHAIR;
			m_wProvideUser=INVALID_CHAIR;
			m_byGameEndType = 0; //��ׯ
			OnEventGameEnd(m_wProvideUser,NULL,GER_NORMAL);

			return true;
		}

		//��������
		CMD_S_OperateHaiDi OperateHaiDi;
		OperateHaiDi.wCurrentUser=m_wCurrentUser;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_HAI_DI,&OperateHaiDi,sizeof(OperateHaiDi));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_HAI_DI,&OperateHaiDi,sizeof(OperateHaiDi));

		return true;
	}


	return true;
}

//���Ͳ���
bool CTableFrameSink::SendOperateNotify()
{
	//������ʾ
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbUserAction[i]!=WIK_NULL)
		{
			//��������
			CMD_S_OperateNotify OperateNotify;
			memset(&OperateNotify, 0, sizeof(CMD_S_OperateNotify));
			OperateNotify.wResumeUser=m_wResumeUser;
			OperateNotify.cbActionCard=m_cbProvideCard;
			OperateNotify.cbActionMask=m_cbUserAction[i];

			if (m_bHearStatus[i])
			{//������
				if ((OperateNotify.cbActionMask&WIK_CHI_HU)>0)
				{
					m_byGangCardNums[i] = 0;
					m_byBuCardNums[i] = 0;
					memset(m_byGangCard[i], 0, sizeof(m_byGangCard[i]));
					memset(m_byBuCard[i], 0, sizeof(m_byBuCard[i]));
					m_cbUserAction[i] = OperateNotify.cbActionMask = WIK_CHI_HU;
				}
			}

			if (m_byGangCardNums[i]>0 && m_byGangCardNums[i]<=4)
			{//�ܡ���
				memcpy(OperateNotify.byGangCard, m_byGangCard[i], sizeof(BYTE)*m_byGangCardNums[i]);
				OperateNotify.byGangCardNums = m_byGangCardNums[i];
			}
			if (m_byBuCardNums[i]>=1)
			{//��
				memcpy(OperateNotify.byBuCard, m_byBuCard[i], sizeof(BYTE)*m_byBuCardNums[i]);
				OperateNotify.byBuCardNums = m_byBuCardNums[i];
			}

			//��������
			m_pITableFrame->SendTableData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
			m_pITableFrame->SendLookonData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
		}
	}

	return true;
}

//�ɷ��˿�
bool CTableFrameSink::DispatchCardData(WORD wCurrentUser)
{
	//״̬Ч��
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR) 
	{
		return false;

	}

	//�����˿�
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		m_cbDiscardCount[m_wOutCardUser]++;
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_cbOutCardData;
	}

	//�����ж�
	if (m_cbLeftCardCount==1)
	{
		//���ñ���
		m_wResumeUser=wCurrentUser;
		m_wCurrentUser=wCurrentUser;
		m_wProvideUser=INVALID_CHAIR;
		m_cbProvideCard=m_cbRepertoryCard[0];

		//����״̬
		m_pITableFrame->SetGameStatus(GS_MJ_HAI_DI);

		//��������
		CMD_S_OperateHaiDi OperateHaiDi;
		OperateHaiDi.wCurrentUser=wCurrentUser;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_HAI_DI,&OperateHaiDi,sizeof(OperateHaiDi));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_HAI_DI,&OperateHaiDi,sizeof(OperateHaiDi));

		return true;
	}

	//��ʼ��
	for(int i=0; i<m_wPlayerCount; i++)
	{
		m_cbUserAction[i] = WIK_NULL;
		::memset(&m_byGangCard[i], 0, sizeof(m_byGangCard[i]));
		m_byGangCardNums[i] = 0;
		::memset(&m_byBuCard[i], 0, sizeof(m_byBuCard[i]));
		m_byBuCardNums[i] = 0;
	}

	//���ƴ���
	m_cbUserAction[wCurrentUser] = WIK_NULL;
	tagGangCardResult GangCardResult;
	BYTE byGangCard[MAX_INDEX]={0};
	BYTE byGangCardNums = 0;
	memset(byGangCard, 0, sizeof(byGangCard));
	memset(&GangCardResult, 0, sizeof(tagGangCardResult));
	if (m_bSendStatus==true)
	{
		//�����˿�
		BYTE byNextCard=0;
#ifdef TOOL_SEND_CARD
		byNextCard = m_SendCardTool.GetNextCard();
#endif
		--m_cbLeftCardCount;
		if (0==byNextCard || 255==byNextCard)
		{
			byNextCard = m_cbRepertoryCard[m_cbLeftCardCount];
		}
		m_cbSendCardCount++;
		m_cbSendCardData=byNextCard;
		m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

		//���ñ���
		m_wProvideUser=wCurrentUser;
		m_cbProvideCard=m_cbSendCardData;

		if (m_tagUserGSKUResult[wCurrentUser].bIsGang)
		{//���ڸ����ڣ������������������
			m_iSaveGameOutCardCount = m_cbOutCardCount;
			m_iSaveGameSendCardCount = m_cbSendCardCount;
		}
		
		//�����ж�
		if (!m_bHearStatus[wCurrentUser] && m_cbLeftCardCount>1)
		{
			//m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[wCurrentUser],
			//	m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],GangCardResult);

			m_GameLogic.AnalyseGangCard(m_cbCardIndex[wCurrentUser],m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],GangCardResult, m_cbKingCard);
			if (GangCardResult.cbCardCount>=1)
			{
				m_GameLogic.EstimateTingCard(m_cbCardIndex[wCurrentUser], m_WeaveItemArray[wCurrentUser], m_cbWeaveItemCount[wCurrentUser], byGangCard, byGangCardNums, m_cbKingCard);
			}

			if (byGangCardNums>0)
			{
				m_cbUserAction[wCurrentUser] |= WIK_TING;
			}
			if (GangCardResult.cbCardCount>=1)
			{
				m_cbUserAction[wCurrentUser] |= WIK_FILL|WIK_GANG;
			}
		}

		//����Ȩλ
		WORD wChiHuRight=0;
		if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;

		//�����ж�
		BYTE cbReCardIndex[MAX_INDEX];;
		tagChiHuResult ChiHuResult;
		//m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wCurrentUser],
		//m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],0,wChiHuRight,ChiHuResult);
		m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[wCurrentUser],m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],
			                         0,wChiHuRight,ChiHuResult,m_cbKingCard,4,cbReCardIndex);
		if (m_bGangStatus && (WIK_CHI_HU&m_cbUserAction[wCurrentUser]))
		{
			m_tagUserGSKUResult[wCurrentUser].bIsGang = true;
		}
	}

	//���ñ���
	m_cbOutCardData=0;
	m_wCurrentUser=wCurrentUser;
	m_wOutCardUser=INVALID_CHAIR;

	//��������
	CMD_S_SendCard SendCard;
	memset(&SendCard, 0, sizeof(CMD_S_SendCard));
	SendCard.wCurrentUser=wCurrentUser;
	SendCard.cbActionMask=m_cbUserAction[wCurrentUser];
	SendCard.cbCardData=(m_bSendStatus==true)?m_cbSendCardData:0x00;

	if (byGangCardNums>0 && byGangCardNums<=4)
	{
		memcpy(SendCard.byGangCard, byGangCard, sizeof(BYTE)*byGangCardNums);
		SendCard.byGangCardNums = byGangCardNums;

		memcpy(m_byGangCard[wCurrentUser], byGangCard, sizeof(BYTE)*byGangCardNums);
		m_byGangCardNums[wCurrentUser] = byGangCardNums;
	}
	if (GangCardResult.cbCardCount>=1)
	{
		memcpy(SendCard.byBuCard, GangCardResult.cbCardData, sizeof(BYTE)*GangCardResult.cbCardCount);
		SendCard.byBuCardNums = GangCardResult.cbCardCount;

		memcpy(m_byBuCard[wCurrentUser], GangCardResult.cbCardData, sizeof(BYTE)*GangCardResult.cbCardCount);
		m_byBuCardNums[wCurrentUser] = GangCardResult.cbCardCount;
	}

	//��������
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	return true;
}


//��Ӧ�ж�
bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind)
{
	//��������
	bool bAroseAction=false;
	

	//�û�״̬
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));



	//�����ж�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{

		//��ʼ��
		::memset(&m_byGangCard[i], 0, sizeof(m_byGangCard[i]));
		m_byGangCardNums[i] = 0;
		::memset(&m_byBuCard[i], 0, sizeof(m_byBuCard[i]));
		m_byBuCardNums[i] = 0;

		//�û�����
		if (wCenterUser==i) continue;

		//������͵Ļ�ɫ����Ŀ
		BYTE cbWeaveCount=m_cbWeaveItemCount[i];
		//for (WORD j=0;j<cbWeaveCount;j++)
		//{
		//	BYTE cbCardColor[i][j]=m_WeaveItemArray[i][j].cbCenterCard&MASK_COLOR;
		//}

		//tagGangCardResult GangCardResult;
		//BYTE bySaveUserShouPai[MAX_INDEX] = {0};
		//BYTE byGangCard[MAX_INDEX]={0};
		//BYTE byGangCardNums = 0;
		////�����ж�
		//memset(&GangCardResult, 0, sizeof(tagGangCardResult));
		//memcpy(bySaveUserShouPai, m_cbCardIndex[i], sizeof(BYTE)*MAX_INDEX);
		//BYTE byCenterCardIndex = m_GameLogic.SwitchToCardIndex(cbCenterCard);
		//if (byCenterCardIndex<MAX_INDEX)
		//{
		//	++bySaveUserShouPai[byCenterCardIndex];
		//}
		//m_GameLogic.AnalyseGangCard(bySaveUserShouPai,NULL,0,GangCardResult);
		//if (GangCardResult.cbCardCount>=1)
		//{
		//	m_GameLogic.EstimateTingCard(bySaveUserShouPai, byGangCard, byGangCardNums, m_cbKingCard);
		//}
        		
		//��������
		if (!m_bHearStatus[i] && EstimatKind==EstimatKind_OutCard )
		{
			if (cbWeaveCount == 0)
			{//û���¼�
				//�����ж�
				if (m_bEnjoinChiPeng[i]==false)
				{//�ܳ�����
					//�����ж�
					m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
					//�����ж�
					WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
					if (wEatUser==i)
					{//�����¼��ܳ�
						m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
					}
				}
				//�����ж�
				if (m_cbLeftCardCount>1) 
				{
					BYTE byCardType = WIK_NULL;
					byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
					if ((byCardType&(WIK_GANG|WIK_FILL))>0)
					{
						m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
						m_byBuCard[i][0] = cbCenterCard;
						m_byBuCardNums[i] = 1;
						bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], NULL, 0, cbCenterCard, m_cbKingCard);
						if (bIsTing)
						{
							m_byGangCard[i][0] = cbCenterCard;
							m_byGangCardNums[i] = 1;
							m_cbUserAction[i] |= WIK_TING;
						}
					}
				}
			}
			else if (cbWeaveCount == 1)
			{//����¼��˲�����һ����.
				BYTE cbCardColor=m_WeaveItemArray[i][0].cbCenterCard&MASK_COLOR;
				if (((m_WeaveItemArray[i][0].cbWeaveKind == WIK_LEFT) || (m_WeaveItemArray[i][0].cbWeaveKind == WIK_CENTER ) || (m_WeaveItemArray[i][0].cbWeaveKind == WIK_RIGHT) ))
				{//�������
					//�����ж�
					if (m_bEnjoinChiPeng[i]==false && (cbCenterCard&MASK_COLOR) == cbCardColor)
					{
						//�����ж�
						m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
						//�����ж�
						WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
						if (wEatUser==i)
						{//�����¼��ܳ�
							m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
						}
					}
					//�����ж�
					if (m_cbLeftCardCount>1 && (cbCenterCard&MASK_COLOR) == cbCardColor) 
					{
						BYTE byCardType = WIK_NULL;
						byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
						if ((byCardType&(WIK_GANG|WIK_FILL))>0)
						{
							m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
							m_byBuCard[i][0] = cbCenterCard;
							m_byBuCardNums[i] = 1;
							bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], cbCenterCard, m_cbKingCard);
							if (bIsTing)
							{
								m_byGangCard[i][0] = cbCenterCard;
								m_byGangCardNums[i] = 1;
								m_cbUserAction[i] |= WIK_TING;
							}
						}
					}
				}
				else if (m_WeaveItemArray[i][0].cbWeaveKind == WIK_PENG || ((m_WeaveItemArray[i][0].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
				{//�������
					//�����ж�
					if (m_bEnjoinChiPeng[i]==false)
					{
						//�����ж�
						m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
						if ((cbCenterCard&MASK_COLOR) == cbCardColor)
						{
							//�����ж�
							WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
							if (wEatUser==i)
							{//�����¼��ܳ�
								m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
							}
						}

					}
					//�����ж�
					if (m_cbLeftCardCount>1) 
					{
						BYTE byCardType = WIK_NULL;
						byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
						if ((byCardType&(WIK_GANG|WIK_FILL))>0)
						{
							m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
							m_byBuCard[i][0] = cbCenterCard;
							m_byBuCardNums[i] = 1;
							bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], cbCenterCard, m_cbKingCard);
							if (bIsTing)
							{
								m_byGangCard[i][0] = cbCenterCard;
								m_byGangCardNums[i] = 1;
								m_cbUserAction[i] |= WIK_TING;
							}
						}
					}
				}

			}
			else if (cbWeaveCount == 2)
			{//�¼�����
				BYTE cbCardColor=m_WeaveItemArray[i][1].cbCenterCard&MASK_COLOR;
                if(((m_WeaveItemArray[i][1].cbWeaveKind == WIK_LEFT) || (m_WeaveItemArray[i][1].cbWeaveKind == WIK_CENTER ) || (m_WeaveItemArray[i][1].cbWeaveKind == WIK_RIGHT) ))
                {//�Ե������
					//�����ж�
					if (m_bEnjoinChiPeng[i]==false && ((cbCenterCard&MASK_COLOR) == cbCardColor))
					{
						//�����ж�
						m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
						//�����ж�
						WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
						if (wEatUser==i)
						{//�����¼��ܳ�
							m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
						}
					}

					//�����ж�
					if (m_cbLeftCardCount>1 && ((cbCenterCard&MASK_COLOR) == cbCardColor)) 
					{
						BYTE byCardType = WIK_NULL;
						byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
						if ((byCardType&(WIK_GANG|WIK_FILL))>0)
						{
							m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
							m_byBuCard[i][0] = cbCenterCard;
							m_byBuCardNums[i] = 1;
							bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], cbCenterCard, m_cbKingCard);
							if (bIsTing)
							{
								m_byGangCard[i][0] = cbCenterCard;
								m_byGangCardNums[i] = 1;
								m_cbUserAction[i] |= WIK_TING;
							}
						}
					}
                }
				else if (m_WeaveItemArray[i][1].cbWeaveKind == WIK_PENG || ((m_WeaveItemArray[i][1].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
				{
					//�����ж�
					if (m_bEnjoinChiPeng[i]==false)
					{
						if (m_WeaveItemArray[i][0].cbWeaveKind == WIK_PENG || ((m_WeaveItemArray[i][0].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
						{
							//�����ж�
							m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
						}
						else if(((cbCenterCard&MASK_COLOR) == cbCardColor) && ((m_WeaveItemArray[i][0].cbWeaveKind == WIK_LEFT) || (m_WeaveItemArray[i][0].cbWeaveKind == WIK_CENTER ) || (m_WeaveItemArray[i][0].cbWeaveKind == WIK_RIGHT) )) 
						{
							//�����ж�
							m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
						}

						if ((m_WeaveItemArray[i][0].cbCenterCard&MASK_COLOR) == (m_WeaveItemArray[i][1].cbCenterCard&MASK_COLOR))
						{
							//�����ж�
							if ((cbCenterCard&MASK_COLOR) == cbCardColor)
							{
								WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
								if (wEatUser==i)
								{//�����¼��ܳ�
									m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
								}
							}
						}
					}
					//�����ж�
					if (m_cbLeftCardCount>1) 
					{
						if (m_WeaveItemArray[i][0].cbWeaveKind == WIK_PENG || ((m_WeaveItemArray[i][0].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
						{
							BYTE byCardType = WIK_NULL;
							byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
							if ((byCardType&(WIK_GANG|WIK_FILL))>0)
							{
								m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
								m_byBuCard[i][0] = cbCenterCard;
								m_byBuCardNums[i] = 1;
								bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], cbCenterCard, m_cbKingCard);
								if (bIsTing)
								{
									m_byGangCard[i][0] = cbCenterCard;
									m_byGangCardNums[i] = 1;
									m_cbUserAction[i] |= WIK_TING;
								}
							}
						}
						else if(((cbCenterCard&MASK_COLOR) == cbCardColor) && ((m_WeaveItemArray[i][0].cbWeaveKind == WIK_LEFT) || (m_WeaveItemArray[i][0].cbWeaveKind == WIK_CENTER ) || (m_WeaveItemArray[i][0].cbWeaveKind == WIK_RIGHT) )) 
						{
							BYTE byCardType = WIK_NULL;
							byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
							if ((byCardType&(WIK_GANG|WIK_FILL))>0)
							{
								m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
								m_byBuCard[i][0] = cbCenterCard;
								m_byBuCardNums[i] = 1;
								bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], cbCenterCard, m_cbKingCard);
								if (bIsTing)
								{
									m_byGangCard[i][0] = cbCenterCard;
									m_byGangCardNums[i] = 1;
									m_cbUserAction[i] |= WIK_TING;
								}
							}
						}
					}
				}
			}

			else if (cbWeaveCount == 3)
			{//����
				BYTE cbCardColor=m_WeaveItemArray[i][2].cbCenterCard&MASK_COLOR;
				if(((m_WeaveItemArray[i][2].cbWeaveKind == WIK_LEFT) || (m_WeaveItemArray[i][2].cbWeaveKind == WIK_CENTER ) || (m_WeaveItemArray[i][2].cbWeaveKind == WIK_RIGHT) ))
				{
					//�����ж�
					if (m_bEnjoinChiPeng[i]==false && ((cbCenterCard&MASK_COLOR) == cbCardColor))
					{
						//�����ж�
						m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
						//�����ж�
						WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
						if (wEatUser==i)
						{
							m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
						}
					}

					//�����ж�
					if (m_cbLeftCardCount>1 && ((cbCenterCard&MASK_COLOR) == cbCardColor)) 
					{
						BYTE byCardType = WIK_NULL;
						byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
						if ((byCardType&(WIK_GANG|WIK_FILL))>0)
						{
							m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
							m_byBuCard[i][0] = cbCenterCard;
							m_byBuCardNums[i] = 1;
							bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], cbCenterCard, m_cbKingCard);
							if (bIsTing)
							{
								m_byGangCard[i][0] = cbCenterCard;
								m_byGangCardNums[i] = 1;
								m_cbUserAction[i] |= WIK_TING;
							}
						}
					}
				}
				else if (m_WeaveItemArray[i][2].cbWeaveKind == WIK_PENG || ((m_WeaveItemArray[i][2].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
				{
					//�����ж�
					if (m_bEnjoinChiPeng[i]==false)
					{
						bool bIsTongSe = false;
						if ((m_WeaveItemArray[i][0].cbWeaveKind == WIK_PENG) && (m_WeaveItemArray[i][1].cbWeaveKind == WIK_PENG))
						{
							//�����ж�
							m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
							bIsTongSe = true;
						}
						else if ((m_WeaveItemArray[i][0].cbWeaveKind == WIK_PENG) && ((m_WeaveItemArray[i][1].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
						{
							//�����ж�
							m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
							bIsTongSe = true;
						}
						else if (((m_WeaveItemArray[i][0].cbWeaveKind&(WIK_GANG|WIK_FILL))>0) && (m_WeaveItemArray[i][1].cbWeaveKind == WIK_PENG))
						{
							//�����ж�
							m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
							bIsTongSe = true;
						}
						else if (((m_WeaveItemArray[i][0].cbWeaveKind&(WIK_GANG|WIK_FILL))>0) && ((m_WeaveItemArray[i][1].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
						{
							//�����ж�
							m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
							bIsTongSe = true;
						}
						else if((cbCenterCard&MASK_COLOR) == cbCardColor)
						{
							//�����ж�
							m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
						}

						if (bIsTongSe)
						{
							//�����ж�
							if ((((m_WeaveItemArray[i][0].cbCenterCard&MASK_COLOR)==(m_WeaveItemArray[i][1].cbCenterCard&MASK_COLOR) ) && 
								((m_WeaveItemArray[i][1].cbCenterCard&MASK_COLOR)==(m_WeaveItemArray[i][2].cbCenterCard&MASK_COLOR) )) && 
								(cbCenterCard&MASK_COLOR) == cbCardColor)
							{//ͬ��ɫ
								WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
								if (wEatUser==i)
								{
									m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
								}
							}
						}
					}
					//�����ж�
					if (m_cbLeftCardCount>1) 
					{
						bool bIsTongSe = false;
						if ((m_WeaveItemArray[i][0].cbWeaveKind == WIK_PENG) && (m_WeaveItemArray[i][1].cbWeaveKind == WIK_PENG))
						{
							bIsTongSe = true;
						}
						else if ((m_WeaveItemArray[i][0].cbWeaveKind == WIK_PENG) && ((m_WeaveItemArray[i][1].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
						{
							bIsTongSe = true;
						}
						else if (((m_WeaveItemArray[i][0].cbWeaveKind&(WIK_GANG|WIK_FILL))>0) && (m_WeaveItemArray[i][1].cbWeaveKind == WIK_PENG))
						{
							bIsTongSe = true;
						}
						else if (((m_WeaveItemArray[i][0].cbWeaveKind&(WIK_GANG|WIK_FILL))>0) && ((m_WeaveItemArray[i][1].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
						{
							bIsTongSe = true;
						}

						if (bIsTongSe)
						{
							BYTE byCardType = WIK_NULL;
							byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
							if ((byCardType&(WIK_GANG|WIK_FILL))>0)
							{
								m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
								m_byBuCard[i][0] = cbCenterCard;
								m_byBuCardNums[i] = 1;
								bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], cbCenterCard, m_cbKingCard);
								if (bIsTing)
								{
									m_byGangCard[i][0] = cbCenterCard;
									m_byGangCardNums[i] = 1;
									m_cbUserAction[i] |= WIK_TING;
								}
							}
						}
						else if((cbCenterCard&MASK_COLOR) == cbCardColor)
						{
							BYTE byCardType = WIK_NULL;
							byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
							if ((byCardType&(WIK_GANG|WIK_FILL))>0)
							{
								m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
								m_byBuCard[i][0] = cbCenterCard;
								m_byBuCardNums[i] = 1;
								bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], cbCenterCard, m_cbKingCard);
								if (bIsTing)
								{
									m_byGangCard[i][0] = cbCenterCard;
									m_byGangCardNums[i] = 1;
									m_cbUserAction[i] |= WIK_TING;
								}
							}
						}
					}
				}
			}
		}
	
        //�����ж�
		BYTE bKingCount=0;
		bKingCount=m_GameLogic.GetKingCardCount(m_cbCardIndex[i],NULL,0,m_cbKingCard,4);
		if (0==bKingCount)
		{//�����ˣ�����û�����Ʋ����жϺ���.
			//����Ȩλ
			WORD wChiHuRight=0;
			//�Ժ��ж�
			BYTE cbReCardIndex[MAX_INDEX]={0};
			tagChiHuResult ChiHuResult;
			//BYTE cbWeaveCount=m_cbWeaveItemCount[i];
			//m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,wChiHuRight,ChiHuResult,m_cbKingCard,4,cbReCardIndex);
			//������ֻ��������ֻ�����������Ʋ��ܽ���
			//ͳ��������Ŀ
			m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,wChiHuRight,ChiHuResult,m_cbKingCard,4,cbReCardIndex, true);
		}

		//����ж�
		if (m_cbUserAction[i]!=WIK_NULL) bAroseAction=true;
	}

	////����¼����еĸ���
	//WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
	//for (int k=0; k<m_wPlayerCount; k++)
	//{
	//	if (wEatUser==k && m_cbUserAction[k]!=WIK_NULL)
	//	{
	//		//��������
	//		tagGangCardResult GangCardResult;
	//		BYTE byGangCard[MAX_INDEX]={0};
	//		BYTE byGangCardNums = 0;
	//		//�����ж�
	//		memset(&GangCardResult, 0, sizeof(tagGangCardResult));
	//		m_GameLogic.AnalyseGangCard(m_cbCardIndex[k],m_WeaveItemArray[k], m_cbWeaveItemCount[k],GangCardResult, m_cbKingCard);
	//		if (GangCardResult.cbCardCount>=1)
	//		{
	//			m_GameLogic.EstimateTingCard(m_cbCardIndex[k], m_WeaveItemArray[k], m_cbWeaveItemCount[k], byGangCard, byGangCardNums, m_cbKingCard);
	//		}
	//		if (byGangCardNums>0)
	//		{//��Ӹ�
	//			m_cbUserAction[k] |= WIK_TING;
	//			memcpy(&m_byGangCard[k][m_byGangCardNums[k]], byGangCard, sizeof(BYTE)*byGangCardNums);
	//			m_byGangCardNums[k] += byGangCardNums;
	//		}
	//		if (GangCardResult.cbCardCount>=1)
	//		{//��Ӳ�
	//			m_cbUserAction[k] |= WIK_FILL|WIK_GANG;
	//			memcpy(&m_byBuCard[k][m_byBuCardNums[k]], GangCardResult.cbCardData, sizeof(BYTE)*GangCardResult.cbCardCount);
	//			m_byBuCardNums[k] += GangCardResult.cbCardCount;
	//		}

	//		//����ж�
	//		if (m_cbUserAction[k]!=WIK_NULL) bAroseAction=true;
	//		break;
	//	}
	//}

	//�������
	if (bAroseAction==true) 
	{
		//���ñ���
		m_wProvideUser=wCenterUser;
		m_cbProvideCard=cbCenterCard;
		m_wResumeUser=m_wCurrentUser;
		m_wCurrentUser=INVALID_CHAIR;

		//������ʾ
		SendOperateNotify();

		return true;
	}

	return false;
}


//cp add 4.11
//==============================================================================
//����ID
#define					Sparrow_TASK_ID_1			31001
#define					Sparrow_TASK_ID_2			31002
#define					Sparrow_TASK_ID_3			31003
#define					Sparrow_TASK_ID_4			31004
#define					Sparrow_TASK_ID_5			31005
#define					Sparrow_TASK_ID_6			31006
#define					Sparrow_TASK_ID_7			31007
#define					Sparrow_TASK_ID_8			31008
#define					Sparrow_TASK_ID_9			31009
#define					Sparrow_TASK_ID_10			31010
#define					Sparrow_TASK_ID_11			31011

//==============================================================================




//
////��������ж�
//void CTableFrameSink::OnUserTaskJudgement(WORD wChairID, BYTE btTag)
//{
//	//wChairID:���ID;  btTag:1:Ӯ,0:��.
//	if (wChairID>=GAME_PLAYER || btTag>1)
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
//	//pUserData->task.dwID:����ID
//	if (1==btTag)
//	{//wChairIDӮ��
//		bool bIsWin = false;
//		switch(pUserData->task.dwID)
//		{
//		case Sparrow_TASK_ID_1:	//����1: С��
//			{
//				if ( (CHK_JI_HU == m_ChiHuResult[wChairID].wChiHuKind) || (CHK_PING_HU == m_ChiHuResult[wChairID].wChiHuKind) )
//				{//�������.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_2:	//����2: ��ϲ
//			{   
//				if (( CHK_SIXI_HU & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
//				//if ( true == m_GameLogic.IsSiXi(m_cbCardIndex[wChairID]) )
//				{//�������.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_3:	//����3: ����
//			{   
//                if (( CHK_BANBAN_HU & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
//				//if ( true == m_GameLogic.IsBanBanHu(m_cbCardIndex[wChairID]) )
//				{//�������.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_4:	//����4: ȱһɫ
//			{   
//				if (( CHK_QUEYISE_HU & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
//				//if ( true == m_GameLogic.IsQueYiSe(m_cbCardIndex[wChairID]) )
//				{//�������.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_5:	//����5: ����˳
//			{   
//				if (( CHK_LIULIU_HU & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
//				//if ( true == m_GameLogic.IsLiuLiuShun(m_cbCardIndex[wChairID]) )
//				{//�������.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_6:	//����6: ������
//			{   
//				if (( CHK_PENG_PENG & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
//				//if ( CHK_PENG_PENG == m_ChiHuResult[wChairID].wChiHuKind )
//				{//�������.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_7:	//����7: ������
//			{  		
//				if (( CHK_JIANG_JIANG & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
//				{//�������.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_8:	//����8: ��С��
//			{   
//				if (( CHK_QI_XIAO_DUI & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
//				//if ( CHK_QI_XIAO_DUI == m_ChiHuResult[wChairID].wChiHuKind )
//				{//�������.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_9:	//����9: ���Ͽ���
//			{   
//				if ( true == m_tagUserGSKUResult[wChairID].bIsGang && true == m_tagUserGSKUResult[wChairID].bIsGSKU)
//				{//�������.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_10:	//����10: �غ�
//			{   
//				if (( CHR_DI & m_ChiHuResult[wChairID].wChiHuRight ) > 0)
//				//if ( CHR_DI == m_ChiHuResult[wChairID].wChiHuRight )
//				{//�������.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_11:	//����11: ���
//			{   
//				if (( CHR_TIAN & m_ChiHuResult[wChairID].wChiHuRight ) > 0)
//				//if ( CHR_TIAN == m_ChiHuResult[wChairID].wChiHuRight)
//				{//�������.
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
//		}
//	}
//}
//////////////////////////////////////////////////////////////////////////



