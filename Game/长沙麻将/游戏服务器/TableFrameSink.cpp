#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//��Ϸ����
const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//��ʼģʽ

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��Ϸ����
	m_wSiceCount=MAKEWORD(1,1);
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
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
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


	memset(m_byUserHuCardType, 0, sizeof(m_byUserHuCardType));
	m_iSaveGameOutCardCount = 0;
	m_iSaveGameSendCardCount = 0;
	m_byGameEndType = 0;
	m_bIsPlayDice=false;
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
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
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


	//cp update

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

	//cp update

	memset(m_byUserHuCardType, 0, sizeof(m_byUserHuCardType));
	m_iSaveGameOutCardCount = 0;
	m_iSaveGameSendCardCount = 0;
	m_byGameEndType = 0;


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

	if (m_wBankerUser<0 || m_wBankerUser>=GAME_PLAYER)
	{
		m_wBankerUser = rand()%GAME_PLAYER;
	}

	//�����˿�
	m_wSiceCount=MAKEWORD(rand()%6+1,rand()%6+1);
	m_cbLeftCardCount=CountArray(m_cbRepertoryCard);
	m_GameLogic.RandCardData(m_cbRepertoryCard,CountArray(m_cbRepertoryCard));


//#ifdef _DEBUG
	//test 
    //m_cbRepertoryCard[0] = 0x08;
	//m_cbRepertoryCard[1] = 0x08;
	//m_cbRepertoryCard[2] = 0x08;
	//m_cbRepertoryCard[3] = 0x08;

	//m_cbRepertoryCard[MAX_REPERTORY - 73] = 0x12;
	//m_cbRepertoryCard[MAX_REPERTORY - 72] = 0x12;
	//m_cbRepertoryCard[MAX_REPERTORY - 71] = 0x12;
	//m_cbRepertoryCard[MAX_REPERTORY - 70] = 0x12;
	//m_cbRepertoryCard[MAX_REPERTORY - 69] = 0x12;
	//m_cbRepertoryCard[MAX_REPERTORY - 68] = 0x12;
	//m_cbRepertoryCard[MAX_REPERTORY - 67] = 0x12;
	//m_cbRepertoryCard[MAX_REPERTORY - 66] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 65] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 64] = 0x22;
    //m_cbRepertoryCard[MAX_REPERTORY - 63] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 62] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 61] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 60] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 59] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 58] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 57] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 56] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 55] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 54] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 53] = 0x15;

	//m_cbRepertoryCard[MAX_REPERTORY - 52] = 0x05;
	//m_cbRepertoryCard[MAX_REPERTORY - 51] = 0x05;
	//m_cbRepertoryCard[MAX_REPERTORY - 50] = 0x05;
	//m_cbRepertoryCard[MAX_REPERTORY - 49] = 0x34;
	//m_cbRepertoryCard[MAX_REPERTORY - 48] = 0x23;
	//m_cbRepertoryCard[MAX_REPERTORY - 47] = 0x23;
	//m_cbRepertoryCard[MAX_REPERTORY - 46] = 0x23;
	//m_cbRepertoryCard[MAX_REPERTORY - 45] = 0x12;
	//m_cbRepertoryCard[MAX_REPERTORY - 44] = 0x12;
	//m_cbRepertoryCard[MAX_REPERTORY - 43] = 0x12;
	//m_cbRepertoryCard[MAX_REPERTORY - 42] = 0x01;
	//m_cbRepertoryCard[MAX_REPERTORY - 41] = 0x01;
	//m_cbRepertoryCard[MAX_REPERTORY - 40] = 0x01;

	//m_cbRepertoryCard[MAX_REPERTORY - 39] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 38] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 37] = 0x21;
	//m_cbRepertoryCard[MAX_REPERTORY - 36] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 35] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 34] = 0x23;
	//m_cbRepertoryCard[MAX_REPERTORY - 33] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 32] = 0x21;
	//m_cbRepertoryCard[MAX_REPERTORY - 31] = 0x12;
	//m_cbRepertoryCard[MAX_REPERTORY - 30] = 0x12;
	//m_cbRepertoryCard[MAX_REPERTORY - 29] = 0x01;
	//m_cbRepertoryCard[MAX_REPERTORY - 28] = 0x01;
	//m_cbRepertoryCard[MAX_REPERTORY - 27] = 0x01;

	//m_cbRepertoryCard[MAX_REPERTORY - 26] = 0x05;
	//m_cbRepertoryCard[MAX_REPERTORY - 25] = 0x05;
	//m_cbRepertoryCard[MAX_REPERTORY - 24] = 0x05;
	//m_cbRepertoryCard[MAX_REPERTORY - 23] = 0x03;
	//m_cbRepertoryCard[MAX_REPERTORY - 22] = 0x03;
	//m_cbRepertoryCard[MAX_REPERTORY - 21] = 0x03;
	//m_cbRepertoryCard[MAX_REPERTORY - 20] = 0x04;
	//m_cbRepertoryCard[MAX_REPERTORY - 19] = 0x04;
	//m_cbRepertoryCard[MAX_REPERTORY - 18] = 0x04;
	//m_cbRepertoryCard[MAX_REPERTORY - 17] = 0x26;
	//m_cbRepertoryCard[MAX_REPERTORY - 16] = 0x26;
	//m_cbRepertoryCard[MAX_REPERTORY - 15] = 0x26;
	//m_cbRepertoryCard[MAX_REPERTORY - 14] = 0x11;

	//m_cbRepertoryCard[MAX_REPERTORY - 52] = 0x18;
	//m_cbRepertoryCard[MAX_REPERTORY - 51] = 0x18;
	//m_cbRepertoryCard[MAX_REPERTORY - 50] = 0x15;
	//m_cbRepertoryCard[MAX_REPERTORY - 49] = 0x15;
	//m_cbRepertoryCard[MAX_REPERTORY - 48] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 47] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 46] = 0x02;
	//m_cbRepertoryCard[MAX_REPERTORY - 45] = 0x02;
	//m_cbRepertoryCard[MAX_REPERTORY - 44] = 0x28;
	//m_cbRepertoryCard[MAX_REPERTORY - 43] = 0x28;
	//m_cbRepertoryCard[MAX_REPERTORY - 42] = 0x28;
	//m_cbRepertoryCard[MAX_REPERTORY - 41] = 0x17;
	//m_cbRepertoryCard[MAX_REPERTORY - 40] = 0x12;

	//m_cbRepertoryCard[MAX_REPERTORY - 39] = 0x18;
	//m_cbRepertoryCard[MAX_REPERTORY - 38] = 0x18;
	//m_cbRepertoryCard[MAX_REPERTORY - 37] = 0x15;
	//m_cbRepertoryCard[MAX_REPERTORY - 36] = 0x15;
	//m_cbRepertoryCard[MAX_REPERTORY - 35] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 34] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 33] = 0x02;
	//m_cbRepertoryCard[MAX_REPERTORY - 32] = 0x02;
	//m_cbRepertoryCard[MAX_REPERTORY - 31] = 0x28;
	//m_cbRepertoryCard[MAX_REPERTORY - 30] = 0x28;
	//m_cbRepertoryCard[MAX_REPERTORY - 29] = 0x28;
	//m_cbRepertoryCard[MAX_REPERTORY - 28] = 0x17;
	//m_cbRepertoryCard[MAX_REPERTORY - 27] = 0x12;

	//m_cbRepertoryCard[MAX_REPERTORY - 26] = 0x18;
	//m_cbRepertoryCard[MAX_REPERTORY - 25] = 0x18;
	//m_cbRepertoryCard[MAX_REPERTORY - 24] = 0x15;
	//m_cbRepertoryCard[MAX_REPERTORY - 23] = 0x15;
	//m_cbRepertoryCard[MAX_REPERTORY - 22] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 21] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 20] = 0x02;
	//m_cbRepertoryCard[MAX_REPERTORY - 19] = 0x02;
	//m_cbRepertoryCard[MAX_REPERTORY - 18] = 0x28;
	//m_cbRepertoryCard[MAX_REPERTORY - 17] = 0x28;
	//m_cbRepertoryCard[MAX_REPERTORY - 16] = 0x28;
	//m_cbRepertoryCard[MAX_REPERTORY - 15] = 0x17;
	//m_cbRepertoryCard[MAX_REPERTORY - 14] = 0x12;

	//m_cbRepertoryCard[MAX_REPERTORY - 52] = 0x18;
	//m_cbRepertoryCard[MAX_REPERTORY - 51] = 0x17;
	//m_cbRepertoryCard[MAX_REPERTORY - 50] = 0x16;
	//m_cbRepertoryCard[MAX_REPERTORY - 49] = 0x11;
	//m_cbRepertoryCard[MAX_REPERTORY - 48] = 0x12;
	//m_cbRepertoryCard[MAX_REPERTORY - 47] = 0x13;
	//m_cbRepertoryCard[MAX_REPERTORY - 46] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 45] = 0x23;
	//m_cbRepertoryCard[MAX_REPERTORY - 44] = 0x24;
	//m_cbRepertoryCard[MAX_REPERTORY - 43] = 0x26;
	//m_cbRepertoryCard[MAX_REPERTORY - 42] = 0x26;
	//m_cbRepertoryCard[MAX_REPERTORY - 41] = 0x26;
	//m_cbRepertoryCard[MAX_REPERTORY - 40] = 0x22;

	//m_cbRepertoryCard[MAX_REPERTORY - 39] = 0x18;
	//m_cbRepertoryCard[MAX_REPERTORY - 38] = 0x17;
	//m_cbRepertoryCard[MAX_REPERTORY - 37] = 0x16;
	//m_cbRepertoryCard[MAX_REPERTORY - 36] = 0x11;
	//m_cbRepertoryCard[MAX_REPERTORY - 35] = 0x12;
	//m_cbRepertoryCard[MAX_REPERTORY - 34] = 0x13;
	//m_cbRepertoryCard[MAX_REPERTORY - 33] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 32] = 0x23;
	//m_cbRepertoryCard[MAX_REPERTORY - 31] = 0x24;
	//m_cbRepertoryCard[MAX_REPERTORY - 30] = 0x26;
	//m_cbRepertoryCard[MAX_REPERTORY - 19] = 0x26;
	//m_cbRepertoryCard[MAX_REPERTORY - 28] = 0x26;
	//m_cbRepertoryCard[MAX_REPERTORY - 27] = 0x22;

	//m_cbRepertoryCard[MAX_REPERTORY - 26] = 0x08;
	//m_cbRepertoryCard[MAX_REPERTORY - 25] = 0x07;
	//m_cbRepertoryCard[MAX_REPERTORY - 24] = 0x06;
	//m_cbRepertoryCard[MAX_REPERTORY - 23] = 0x11;
	//m_cbRepertoryCard[MAX_REPERTORY - 22] = 0x12;
	//m_cbRepertoryCard[MAX_REPERTORY - 21] = 0x13;
	//m_cbRepertoryCard[MAX_REPERTORY - 20] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 19] = 0x23;
	//m_cbRepertoryCard[MAX_REPERTORY - 18] = 0x24;
	//m_cbRepertoryCard[MAX_REPERTORY - 17] = 0x26;
	//m_cbRepertoryCard[MAX_REPERTORY - 16] = 0x26;
	//m_cbRepertoryCard[MAX_REPERTORY - 15] = 0x26;
	//m_cbRepertoryCard[MAX_REPERTORY - 14] = 0x15;

	//m_cbRepertoryCard[MAX_REPERTORY - 13] = 0x15;
	//m_cbRepertoryCard[MAX_REPERTORY - 12] = 0x23;
	//m_cbRepertoryCard[MAX_REPERTORY - 11] = 0x23;
	//m_cbRepertoryCard[MAX_REPERTORY - 10] = 0x23;
	//m_cbRepertoryCard[MAX_REPERTORY - 9] = 0x13;
	//m_cbRepertoryCard[MAX_REPERTORY - 8] = 0x12;
	//m_cbRepertoryCard[MAX_REPERTORY - 7] = 0x11;
	//m_cbRepertoryCard[MAX_REPERTORY - 6] = 0x03;
	//m_cbRepertoryCard[MAX_REPERTORY - 5] = 0x02;
	//m_cbRepertoryCard[MAX_REPERTORY - 4] = 0x01;
	//m_cbRepertoryCard[MAX_REPERTORY - 3] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 2] = 0x22;
	//m_cbRepertoryCard[MAX_REPERTORY - 1] = 0x22;
//#endif


	//���Ͽ���
	memset(m_tagUserGSKUResult, 0, sizeof(m_tagUserGSKUResult));

	//�ַ��˿�
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbLeftCardCount-=(MAX_COUNT-1);
		m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_cbLeftCardCount],MAX_COUNT-1,m_cbCardIndex[i]);
	}


	//�����˿�
	m_cbSendCardCount++;
	m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//���ñ���
	m_cbProvideCard=0;
	m_wProvideUser=INVALID_CHAIR;
	m_wCurrentUser=m_wBankerUser;

	//��������
	tagGangCardResult GangCardResult;
	BYTE byGangCard[MAX_INDEX]={0};
	BYTE byGangCardNums = 0;
	bool bAroseAction=false;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbUserAction[i] = WIK_NULL;
		m_byUserHuCardType[i] = WIK_NULL;

		//�������
		m_cbUserAction[i]|=m_GameLogic.EstimateChiHu(m_cbCardIndex[i]);

		//ׯ���ж�
		if (i==m_wBankerUser)
		{
			memset(byGangCard, 0, sizeof(byGangCard));
			byGangCardNums = 0;
			//�����ж�
			memset(&GangCardResult, 0, sizeof(tagGangCardResult));
			m_GameLogic.AnalyseGangCard(m_cbCardIndex[i],NULL,0,GangCardResult);
			if (GangCardResult.cbCardCount>=1)
			{
				m_GameLogic.EstimateTingCard(m_cbCardIndex[i], NULL, 0, byGangCard, byGangCardNums);
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
			m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],NULL,0,0,0,ChiHuResult);
		}

		//״̬����
		if ((bAroseAction==false)&&(i!=m_wBankerUser)&&(m_cbUserAction[i]!=WIK_NULL))
		{
			bAroseAction=true;
			m_wResumeUser=m_wCurrentUser;
			m_wCurrentUser=INVALID_CHAIR;
		}
	}

	//��������
	CMD_S_GameStart GameStart;
	memset(&GameStart, 0, sizeof(CMD_S_GameStart));
	GameStart.wSiceCount=m_wSiceCount;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wCurrentUser=m_wCurrentUser;     
	CopyMemory(GameStart.bTrustee,m_bTrustee,sizeof(m_bTrustee));

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
			LONGLONG lCellScore=m_pGameServiceOption->lCellScore;
			BYTE cbBirdValue=(m_cbRepertoryCard[0]&MASK_VALUE);
			GameEnd.bIsHuangzhuang = false;

			//������Ϣ
			GameEnd.iCellScore=lCellScore;
            GameEnd.iChiHuType=m_byGameEndType;
			GameEnd.wProvideUser=wChairID;
			GameEnd.cbChiHuCard=m_cbChiHuCard;
			GameEnd.cbBirdCard=m_cbRepertoryCard[0];
			GameEnd.wBirdHitUser=(m_wBankerUser+9-cbBirdValue)%m_wPlayerCount;
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
			 GameEnd.wChiHuKind[i]=m_ChiHuResult[i].wChiHuKind;   //��������
			 GameEnd.wChiHuRight[i]=m_ChiHuResult[i].wChiHuRight; //����Ȩλ
			}

			//��������
			WORD wChiHuUserCount=0;

			//ͳ�ƻ���
			if (m_wProvideUser!=INVALID_CHAIR)
			{
				//��������
				if (m_ChiHuResult[m_wProvideUser].wChiHuKind!=CHK_NULL)
				{
					//��������
					BYTE cbChiHuOrder=m_GameLogic.GetChiHuActionRank(m_ChiHuResult[m_wProvideUser]);

					//ѭ���ۼ�
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						//Ӯ�ҹ���
						if (i==m_wProvideUser) continue;

						//���ּ���
						if (cbChiHuOrder>=2)
						{
							//��������
							LONGLONG lTimes=(((i==GameEnd.wBirdHitUser)||(m_wProvideUser==GameEnd.wBirdHitUser))?2L:1L);
							GameEnd.lGameScore[i]-=((i==m_wBankerUser)||(m_wProvideUser==m_wBankerUser))?8*lCellScore*lTimes:6*lCellScore*lTimes;
							GameEnd.lGameScore[m_wProvideUser]+=((i==m_wBankerUser)||(m_wProvideUser==m_wBankerUser))?8*lCellScore*lTimes:6*lCellScore*lTimes;
						}
						else if (cbChiHuOrder==1)
						{
							//�������
							LONGLONG lTimes=(((i==GameEnd.wBirdHitUser)||(m_wProvideUser==GameEnd.wBirdHitUser))?2L:1L);
							GameEnd.lGameScore[i]-=((i==m_wBankerUser)||(m_wProvideUser==m_wBankerUser))?4*lCellScore*lTimes:3*lCellScore*lTimes;
							GameEnd.lGameScore[m_wProvideUser]+=((i==m_wBankerUser)||(m_wProvideUser==m_wBankerUser))?4*lCellScore*lTimes:3*lCellScore*lTimes;
						}
						else
						{
							//С������
							LONGLONG lTimes=((i==GameEnd.wBirdHitUser)||(m_wProvideUser==GameEnd.wBirdHitUser))?2L:1L;
							GameEnd.lGameScore[i]-=((i==m_wBankerUser)||(m_wProvideUser==m_wBankerUser))?2*lCellScore*lTimes:lCellScore*lTimes;
							GameEnd.lGameScore[m_wProvideUser]+=((i==m_wBankerUser)||(m_wProvideUser==m_wBankerUser))?2*lCellScore*lTimes:lCellScore*lTimes;
						}
						GameEnd.cbTimes[i]=GameEnd.lGameScore[i]/lCellScore;
						GameEnd.cbTimes[m_wProvideUser]=GameEnd.lGameScore[m_wProvideUser]/lCellScore;
					}
					GameEnd.iChiHuKind[m_wProvideUser]=1;

					//������Ŀ
					wChiHuUserCount=1;

					//ׯ������
					m_wBankerUser=m_wProvideUser;
				}

				//׽������
				if (m_ChiHuResult[m_wProvideUser].wChiHuKind==CHK_NULL)
				{
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

						//����ͳ��
						if (cbChiHuOrder>=2)
						{
							//���Ϸ���
							LONGLONG lTimes=(((i==GameEnd.wBirdHitUser)||(m_wProvideUser==GameEnd.wBirdHitUser))?2L:1L);
							GameEnd.lGameScore[i]+=((i==m_wBankerUser)||(m_wProvideUser==m_wBankerUser))?14*lCellScore*lTimes:12*lCellScore*lTimes;
							GameEnd.lGameScore[m_wProvideUser]-=((i==m_wBankerUser)||(m_wProvideUser==m_wBankerUser))?14*lCellScore*lTimes:12*lCellScore*lTimes;
						}
						else if (cbChiHuOrder==1)
						{
							//���׽��
							LONGLONG lTimes=(((i==GameEnd.wBirdHitUser)||(m_wProvideUser==GameEnd.wBirdHitUser))?2L:1L);
							GameEnd.lGameScore[i]+=((i==m_wBankerUser)||(m_wProvideUser==m_wBankerUser))?7*lCellScore*lTimes:6*lCellScore*lTimes;
							GameEnd.lGameScore[m_wProvideUser]-=((i==m_wBankerUser)||(m_wProvideUser==m_wBankerUser))?7*lCellScore*lTimes:6*lCellScore*lTimes;
						}
						else
						{
							//С��׽��
							LONGLONG lTimes=((i==GameEnd.wBirdHitUser)||(m_wProvideUser==GameEnd.wBirdHitUser))?2L:1L;
							GameEnd.lGameScore[i]+=((i==m_wBankerUser)||(m_wProvideUser==m_wBankerUser))?2*lCellScore*lTimes:lCellScore*lTimes;
							GameEnd.lGameScore[m_wProvideUser]-=((i==m_wBankerUser)||(m_wProvideUser==m_wBankerUser))?2*lCellScore*lTimes:lCellScore*lTimes;
						}
						GameEnd.cbTimes[i]=GameEnd.lGameScore[i]/lCellScore;
						GameEnd.cbTimes[m_wProvideUser]=GameEnd.lGameScore[m_wProvideUser]/lCellScore;

                        GameEnd.iChiHuKind[i]=2;
						//���ñ���
						wNextBankerUser=i;
						wChiHuUserCount++;
					}

					//ׯ������
					m_wBankerUser=(wChiHuUserCount>=2)?m_wProvideUser:wNextBankerUser;
					if ((m_wHaiDiUser!=INVALID_CHAIR)&&(wChiHuUserCount>1)) m_wBankerUser=m_wHaiDiUser;
				}
			}
			else if ((m_cbOutCardCount==0)&&(m_cbSendCardCount==1))
			{
				//��������
				WORD wNextBankerUser=INVALID_CHAIR;

				//������� 
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					//��ҹ���
					if (m_ChiHuResult[i].wChiHuKind==CHK_NULL) continue;

					//ѭ���ۼ�
					if ((m_ChiHuResult[i].wChiHuKind&0x00F0)!=0)
					{
						for (WORD j=0;j<m_wPlayerCount;j++)
						{
							if (m_ChiHuResult[j].wChiHuKind==CHK_NULL)
							{
								LONGLONG lTimes=((i==GameEnd.wBirdHitUser)||(j==GameEnd.wBirdHitUser))?2L:1L;
								GameEnd.lGameScore[i]+=((i==m_wBankerUser)||(j==m_wBankerUser))?2*lCellScore*lTimes:lCellScore*lTimes;
								GameEnd.lGameScore[j]-=((i==m_wBankerUser)||(j==m_wBankerUser))?2*lCellScore*lTimes:lCellScore*lTimes;
								GameEnd.cbTimes[i]=GameEnd.lGameScore[i]/lCellScore;
								GameEnd.cbTimes[j]=GameEnd.lGameScore[j]/lCellScore;
							}
						}
					}
					else
					{
						for (WORD j=0;j<m_wPlayerCount;j++)
						{
							if (m_ChiHuResult[j].wChiHuKind==CHK_NULL)
							{
								LONGLONG lTimes=((i==GameEnd.wBirdHitUser)||(j==GameEnd.wBirdHitUser))?4L:2L;
								GameEnd.lGameScore[i]+=((i==m_wBankerUser)||(j==m_wBankerUser))?2*lCellScore*lTimes:lCellScore*lTimes;
								GameEnd.lGameScore[j]-=((i==m_wBankerUser)||(j==m_wBankerUser))?2*lCellScore*lTimes:lCellScore*lTimes;
								GameEnd.cbTimes[i]=GameEnd.lGameScore[i]/lCellScore;
								GameEnd.cbTimes[j]=GameEnd.lGameScore[j]/lCellScore;
							}
						}
					}
					
					GameEnd.iChiHuKind[i]=1;

					//���ñ���
					wChiHuUserCount++;
					wNextBankerUser=i;
				}

				//ׯ������
				if (wChiHuUserCount==1) m_wBankerUser=wNextBankerUser;
			}
			else
			{
				//��ׯ����
				GameEnd.bIsHuangzhuang = true;
				GameEnd.cbBirdCard=0;
				GameEnd.cbChiHuCard=0;
				GameEnd.wBirdHitUser=INVALID_CHAIR;
				GameEnd.cbHaiDiCard=m_cbRepertoryCard[0];

				//����ׯ��
				m_wBankerUser=(m_wHaiDiUser!=INVALID_CHAIR)?m_wHaiDiUser:m_wResumeUser;
			}

			//�����˿�
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			//��������
			LONGLONG lRevenue=0L;  //�����
			LONGLONG lUserScore[GAME_PLAYER];
			ZeroMemory(lUserScore,sizeof(lUserScore));
			CopyMemory(lUserScore,GameEnd.lGameScore,sizeof(lUserScore));

			//д��
			for(int i=0;i<GAME_PLAYER;i++)
			{

				//if(lUserScore[i]>0)
				//{
					//lRevenue=lUserScore[i]*m_pGameServiceOption->wRevenue/100;				    lRevenue=lCellScore;
					//GameEnd.lGameScore[i]=lUserScore[i]-lRevenue;
				//}

                lRevenue=lCellScore;
				GameEnd.lGameScore[i]=lUserScore[i]-lRevenue;
				//д�����
				LONGLONG lScore=GameEnd.lGameScore[i];
				enScoreKind	ScoreKind;
				if(lScore>0L)
				{
					ScoreKind=enScoreKind_Win;
				}

				else if(lScore==0L) 	
				{
					ScoreKind=enScoreKind_Draw;
				}
				else
				{
				    ScoreKind=enScoreKind_Lost;
				}
				


				m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],lRevenue,ScoreKind);


				////��������ж�
				//if (enScoreKind_Win==ScoreKind)
				//{
				//	OnUserTaskJudgement(i, 1);
				//}

			}
			
			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));



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
			LONGLONG lCellScore=m_pGameServiceOption->lCellScore;
            DOUBLE lRestrictScore=m_pGameServiceOption->lRestrictScore;
			LONGLONG Revenue = m_pGameServiceOption->wRevenue;
			LONGLONG lRevenue=0L;
			//lRevenue = (lRestrictScore/3)*(Revenue/100.0f);
			lRevenue = lCellScore;
			


			//���ñ���
			GameEnd.wBirdHitUser=INVALID_CHAIR;
			GameEnd.wProvideUser=INVALID_CHAIR;

			GameEnd.iCellScore=lCellScore;
			//ͳ�ƻ���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (i==wChairID) 
				{
				GameEnd.lGameScore[i]=-lRestrictScore - lRevenue;
				GameEnd.IsUserLeft[i]=true;
				}
				else 
				{
				GameEnd.lGameScore[i]=lRestrictScore/3 - lRevenue;
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

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
						
			//�޸Ļ���
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (wChairID==i)m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],0L,enScoreKind_Flee);
				else m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],lRevenue,enScoreKind_Flee);

			}

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
			if (/*m_bIsUserFirstOutCard[wChiarID] &&*/ m_wOutCardUser==wChiarID)
			{
				bool bIsTing = m_GameLogic.IsCanTingCard(m_cbCardIndex[wChiarID], m_WeaveItemArray[wChiarID], m_cbWeaveItemCount[wChiarID]);
				if (bIsTing)
				{
					bIsMeTingPai = true;
				}
			}
			StatusPlay.m_bTingCard=bIsMeTingPai;				


			StatusPlay.bIsPlayDice=m_bIsPlayDice;

			//��Ϸ����
			StatusPlay.wSiceCount=m_wSiceCount;
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;

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
	case SUB_C_TRUSTEE:
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
	
	//���Ͽ���
	memset(m_tagUserGSKUResult, 0, sizeof(m_tagUserGSKUResult));

	//���ñ���
	m_bIsPlayDice=false;
	m_bSendStatus=true;
	m_bEnjoinChiHu[wChairID]=false;
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

	//��Ӧ�ж�
	bool bAroseAction=EstimateUserRespond(wChairID,cbCardData,EstimatKind_OutCard);

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
	if (bAroseAction==false) DispatchCardData(m_wCurrentUser);

	return true;
}

//�û�����
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard)
{
	//Ч��״̬
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//Ч���û�
	if ((wChairID!=m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR)) return true;

	if (0==cbOperateCode && 0==cbOperateCard)
	{//ȡ����
		m_tagUserGSKUResult[wChairID].bIsGSKU = false;
	}
	

	//��������
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//Ч��״̬
		if (m_bResponse[wChairID]==true) return true;
		if ((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)==0))
		{
			return true;
		}

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
				if ((m_bResponse[i]==false)&&(m_cbUserAction[i]&WIK_CHI_HU)) return true;
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
				//�����ж�
				if ((i==m_wProvideUser)||((m_cbPerformAction[i]&WIK_CHI_HU)==0)) continue;

				//����Ȩλ
				WORD wChiHuRight=0;
				if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==1)) m_byUserHuCardType[i]|=CHR_DI;
				if (m_tagUserGSKUResult[i].bIsGang 
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
					BYTE cbWeaveItemCount=m_cbWeaveItemCount[i];
					tagWeaveItem * pWeaveItem=m_WeaveItemArray[i];
					m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,wChiHuRight,m_ChiHuResult[i]);

					//�����˿�
					if (m_ChiHuResult[i].wChiHuKind!=CHK_NULL) m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbChiHuCard)]++;
				}

				//�������
				if ((m_cbOutCardCount==0)&&(m_cbSendCardCount==1))
				{
					if (m_GameLogic.IsSiXi(m_cbCardIndex[i])==true) m_ChiHuResult[i].wChiHuKind|=CHK_SIXI_HU;
					if (m_GameLogic.IsQueYiSe(m_cbCardIndex[i])==true) m_ChiHuResult[i].wChiHuKind|=CHK_QUEYISE_HU;
					if (m_GameLogic.IsBanBanHu(m_cbCardIndex[i])==true) m_ChiHuResult[i].wChiHuKind|=CHK_BANBAN_HU;
					if (m_GameLogic.IsLiuLiuShun(m_cbCardIndex[i])==true) m_ChiHuResult[i].wChiHuKind|=CHK_LIULIU_HU;
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


		}

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

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
		ASSERT((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)!=0));
		if ((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)==0)) return false;

		//�˿�Ч��
		ASSERT((cbOperateCode==WIK_NULL)||(cbOperateCode==WIK_CHI_HU)||(m_GameLogic.IsValidCard(cbOperateCard)==true));
		if ((cbOperateCode!=WIK_NULL)&&(cbOperateCode!=WIK_CHI_HU)&&(m_GameLogic.IsValidCard(cbOperateCard)==false)) return false;
		
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
				{
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
					ASSERT(cbWeaveIndex!=0xFF);
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
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
				m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,0,m_byUserHuCardType[wChairID],m_ChiHuResult[wChairID]);


				//�������  ������
				if ((m_cbOutCardCount==0)&&(m_cbSendCardCount==1))
				{
					if (m_GameLogic.IsSiXi(m_cbCardIndex[wChairID])==true) m_ChiHuResult[wChairID].wChiHuKind|=CHK_SIXI_HU;
					if (m_GameLogic.IsQueYiSe(m_cbCardIndex[wChairID])==true) m_ChiHuResult[wChairID].wChiHuKind|=CHK_QUEYISE_HU;
					if (m_GameLogic.IsBanBanHu(m_cbCardIndex[wChairID])==true) m_ChiHuResult[wChairID].wChiHuKind|=CHK_BANBAN_HU;
					if (m_GameLogic.IsLiuLiuShun(m_cbCardIndex[wChairID])==true) m_ChiHuResult[wChairID].wChiHuKind|=CHK_LIULIU_HU;
				}

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
		BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
		tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
		m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,m_cbProvideCard,CHR_HAI_DI,m_ChiHuResult[wChairID]);

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
				m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbProvideCard,CHR_HAI_DI,m_ChiHuResult[i]);

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
		m_cbSendCardCount++;
		m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];
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
			m_GameLogic.AnalyseGangCard(m_cbCardIndex[wCurrentUser],m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],GangCardResult);
			if (GangCardResult.cbCardCount>=1)
			{
				m_GameLogic.EstimateTingCard(m_cbCardIndex[wCurrentUser], m_WeaveItemArray[wCurrentUser], m_cbWeaveItemCount[wCurrentUser], byGangCard, byGangCardNums);
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
		tagChiHuResult ChiHuResult;
		m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wCurrentUser],
			m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],0,wChiHuRight,ChiHuResult);
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

		//��������
		if (!m_bHearStatus[i] && EstimatKind==EstimatKind_OutCard )
		{
			//�����ж�
			if (m_bEnjoinChiPeng[i]==false)
			{
				//�����ж�
				m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard);

				//�����ж�
				WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
				if (wEatUser==i) m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard);
			}

			//�����ж�
			if (m_cbLeftCardCount>1) 
			{		
				BYTE byCardType = WIK_NULL;
				byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard);
				if ((byCardType&(WIK_GANG|WIK_FILL))>0)
				{
					m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
					m_byBuCard[i][0] = cbCenterCard;
					m_byBuCardNums[i] = 1;
					bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], NULL, 0, cbCenterCard);
					if (bIsTing)
					{
					  m_byGangCard[i][0] = cbCenterCard;
				 	  m_byGangCardNums[i] = 1;
					  m_cbUserAction[i] |= WIK_TING;
					}
				}

				//m_cbUserAction[i]|=m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard);
			}
		}

		//�����ж�
		if (m_bEnjoinChiHu[i]==false)
		{
			//����Ȩλ
			WORD wChiHuRight=0;
			//if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;
			//if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==1)) wChiHuRight|=CHR_DI;
			//if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==0)) wChiHuRight|=CHR_TIAN;

			//�Ժ��ж�
			tagChiHuResult ChiHuResult;
			BYTE cbWeaveCount=m_cbWeaveItemCount[i];
			m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,wChiHuRight,ChiHuResult);

			//�Ժ�����
			if ((m_cbUserAction[i]&WIK_CHI_HU)!=0) m_bEnjoinChiHu[i]=true;
		}

		//����ж�
		if (m_cbUserAction[i]!=WIK_NULL) bAroseAction=true;
	}

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
#define					Sparrow_TASK_ID_1			30301
#define					Sparrow_TASK_ID_2			30302
#define					Sparrow_TASK_ID_3			30303
#define					Sparrow_TASK_ID_4			30304
#define					Sparrow_TASK_ID_5			30305
#define					Sparrow_TASK_ID_6			30306
#define					Sparrow_TASK_ID_7			30307
#define					Sparrow_TASK_ID_8			30308
#define					Sparrow_TASK_ID_9			30309
#define					Sparrow_TASK_ID_10			30310
#define					Sparrow_TASK_ID_11			30311

//==============================================================================



/*
//////////////////////////////////////����ȡ��//////////////////////////////////////////////
//��������ж�
void CTableFrameSink::OnUserTaskJudgement(WORD wChairID, BYTE btTag)
{
	//wChairID:���ID;  btTag:1:Ӯ,0:��.
	if (wChairID>=GAME_PLAYER || btTag>1)
	{
		return;
	}
	//���Ȼ�ȡwChairID����������.
	IServerUserItem *pUserItem = m_pITableFrame->GetServerUserItem( wChairID );
	if (NULL == pUserItem)
	{
		return;
	}
	tagServerUserData *pUserData = pUserItem->GetUserData();
	if (NULL == pUserData)
	{
		return;
	}
	//pUserData->task.dwID:����ID
	if (1==btTag)
	{//wChairIDӮ��
		bool bIsWin = false;
		switch(pUserData->task.dwID)
		{
		case Sparrow_TASK_ID_1:	//����1: С��
			{
				if ( (CHK_JI_HU == m_ChiHuResult[wChairID].wChiHuKind) || (CHK_PING_HU == m_ChiHuResult[wChairID].wChiHuKind) )
				{//�������.
					bIsWin = true;
				}
			}
			break;
		case Sparrow_TASK_ID_2:	//����2: ��ϲ
			{   
				if (( CHK_SIXI_HU & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
				//if ( true == m_GameLogic.IsSiXi(m_cbCardIndex[wChairID]) )
				{//�������.
					bIsWin = true;
				}
			}
			break;
		case Sparrow_TASK_ID_3:	//����3: ����
			{   
                if (( CHK_BANBAN_HU & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
				//if ( true == m_GameLogic.IsBanBanHu(m_cbCardIndex[wChairID]) )
				{//�������.
					bIsWin = true;
				}
			}
			break;
		case Sparrow_TASK_ID_4:	//����4: ȱһɫ
			{   
				if (( CHK_QUEYISE_HU & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
				//if ( true == m_GameLogic.IsQueYiSe(m_cbCardIndex[wChairID]) )
				{//�������.
					bIsWin = true;
				}
			}
			break;
		case Sparrow_TASK_ID_5:	//����5: ����˳
			{   
				if (( CHK_LIULIU_HU & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
				//if ( true == m_GameLogic.IsLiuLiuShun(m_cbCardIndex[wChairID]) )
				{//�������.
					bIsWin = true;
				}
			}
			break;
		case Sparrow_TASK_ID_6:	//����6: ������
			{   
				if (( CHK_PENG_PENG & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
				//if ( CHK_PENG_PENG == m_ChiHuResult[wChairID].wChiHuKind )
				{//�������.
					bIsWin = true;
				}
			}
			break;
		case Sparrow_TASK_ID_7:	//����7: ������
			{  		
				if (( CHK_JIANG_JIANG & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
				{//�������.
					bIsWin = true;
				}
			}
			break;
		case Sparrow_TASK_ID_8:	//����8: ��С��
			{   
				if (( CHK_QI_XIAO_DUI & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
				//if ( CHK_QI_XIAO_DUI == m_ChiHuResult[wChairID].wChiHuKind )
				{//�������.
					bIsWin = true;
				}
			}
			break;
		case Sparrow_TASK_ID_9:	//����9: ���Ͽ���
			{   
				if ( true == m_tagUserGSKUResult[wChairID].bIsGang && true == m_tagUserGSKUResult[wChairID].bIsGSKU)
				{//�������.
					bIsWin = true;
				}
			}
			break;
		case Sparrow_TASK_ID_10:	//����10: �غ�
			{   
				if (( CHR_DI & m_ChiHuResult[wChairID].wChiHuRight ) > 0)
				//if ( CHR_DI == m_ChiHuResult[wChairID].wChiHuRight )
				{//�������.
					bIsWin = true;
				}
			}
			break;
		case Sparrow_TASK_ID_11:	//����11: ���
			{   
				if (( CHR_TIAN & m_ChiHuResult[wChairID].wChiHuRight ) > 0)
				//if ( CHR_TIAN == m_ChiHuResult[wChairID].wChiHuRight)
				{//�������.
					bIsWin = true;
				}
			}
			break;
		default:
			break;
		}
		if (true == bIsWin)
		{//�������.
			//�ύ�������
			m_pITableFrame->PefermTaskFinish(wChairID);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
*/
