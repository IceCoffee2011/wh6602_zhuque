#include "Stdafx.h"
#include "GameOption.h"
#include "GameClientDlg.h"
#include <fstream>

#include "TraceCatch.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////

//std::ofstream out("client-info.txt", std::ios::trunc); 
//
//static void WriteInfo( char* p)
//{
//	out << p << std::endl;
//}
//
//void PrintArray(const BYTE* byData, int iNums)
//{
//	char chData[256]={0};
//	for (int i=0; i<iNums; i++)
//	{
//		sprintf(chData, "%s-%d-", chData, byData[i]);
//	}
//	WriteInfo(chData);
//}

//////////////////////////////////////////////////////////////////////////
//�궨��

//��Ϸ��ʱ��
#define IDI_OUT_CARD					200								//���ƶ�ʱ��
#define IDI_MOST_CARD					201								//���ʱ��
#define IDI_START_GAME					202								//��ʼ��ʱ��
#define IDI_LAND_SCORE					203								//�е�����ʱ��
#define IDI_SNATCH_LAND                 204                             //��������ʱ��
#define IDI_DISPATCH_CARD				205							    //���ƶ�ʱ��
#define IDI_DOUBLE_SCORE				206								//�ӱ���ʱ��
#define IDI_ENDSTART_GAME               301                             //��Ϸ������ʼ��ʱ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_PASS_CARD,OnPassCard)
	ON_MESSAGE(IDM_CALL_LAND,OnCallLand)
	ON_MESSAGE(IDM_SNATCH_LAND,OnSnatchLand)
	ON_MESSAGE(IDM_AUTO_OUTCARD,OnAutoOutCard)
	ON_MESSAGE(IDM_LEFT_HIT_CARD,OnLeftHitCard)
	ON_MESSAGE(IDM_RIGHT_HIT_CARD,OnRightHitCard)
	ON_MESSAGE(IDM_LAND_AUTOPLAY,OnAutoPlay)
	ON_MESSAGE(IDM_SORT_HAND_CARD,OnMessageSortCard)
	ON_MESSAGE(IDM_BRIGHT_START,OnBrightStart)
	ON_MESSAGE(IDM_DOUBLE_SCORE, OnDoubleScore)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView, &m_ScoreView)
{	
	__ENTER_FUNCTION

	
	//��Ϸ����
	m_wBombTime=1;
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	memset(m_bHandCardCount,0,sizeof(m_bHandCardCount));
	m_wLandUser=INVALID_CHAIR;
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	m_bAutoPlay = false ;
	m_cbSortType=ST_COUNT;

	//���ñ���
	m_dwCardHSpace=DEFAULT_PELS;

	//���Ʊ���
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//��������
	m_wTimeOutCount=0;
	m_wMostUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_bSystemTrustee=false;
	m_wLastMostUser=INVALID_CHAIR;
    
	//��������
	m_cbRemnantCardCount=0;
	m_cbDispatchCardCount=0;
	ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));

	//������
	m_cbCallScorePhase = CSD_NORMAL;
	m_sVernacular="GY_";//����

	//��ʾ�Ʊ���
	ZeroMemory(&m_tagNewCard,sizeof(m_tagNewCard));
	ZeroMemory(&m_tagOldCard,sizeof(m_tagOldCard));

	__LEAVE_FUNCTION
}

//��������
CGameClientDlg::~CGameClientDlg()
{
	__ENTER_FUNCTION
	__LEAVE_FUNCTION
}

//��ʼ����
bool CGameClientDlg::InitGameFrame()
{
	__ENTER_FUNCTION

	//���ñ���
	//SetWindowText(TEXT("��������Ϸ  --  Ver��6.0.1.0"));	
	//m_sVernacular.Format("%s_",m_strLanguage.c_str()); 

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//��ȡ����
	//m_dwCardHSpace=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("CardSpace"),DEFAULT_PELS);
	m_bDeasilOrder=AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),FALSE)?true:false;
	m_bAllowLookon = AfxGetApp()->GetProfileInt(TEXT("GameOption"),TEXT("AllowLookon"),FALSE)?true:false;

	//��������
	if ((m_dwCardHSpace>MAX_PELS)||(m_dwCardHSpace<LESS_PELS)) m_dwCardHSpace=DEFAULT_PELS;

	//���ÿؼ�
	//m_GameClientView.m_HandCardControl[1].SetCardSpace(17.5,0,20);

	//������Դ
	//m_GameClientView.m_btCardDao.ShowWindow(SW_SHOW);
	SetChatShowTag(true, true);
    m_GameClientView.SetCardBackDisPlay(false);
	m_GameClientView.SetCardBackData(NULL,0);
	//ShowScoreView(true);
	SetTaskWndShow(false);
    //������Ϸ��������
    PlayMusic("BACKGROUND_MUSIC.mp3");

	//��ʼ����������
	m_GameClientView.InitializeAction();

    __LEAVE_FUNCTION
	return true;	
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	__ENTER_FUNCTION
		
		
	//��Ϸ����
	m_wBombTime=1;
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	memset(m_bHandCardCount,0,sizeof(m_bHandCardCount));
	m_wLandUser=INVALID_CHAIR;
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	m_bAutoPlay = false ;
	m_cbSortType=ST_COUNT;

	//���Ʊ���
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));

	//��������
	m_wTimeOutCount=0;
	m_wMostUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_bSystemTrustee=false;
    m_wLastMostUser=INVALID_CHAIR;
	//��������
	m_cbRemnantCardCount=0;
	m_cbDispatchCardCount=0;
	ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));

	//������
	m_cbCallScorePhase = CSD_NORMAL;

	//����ͷ���
	m_IsFanSpring=false;
	m_IsSpring=false;
	memset(m_LargessCount,0,sizeof(m_LargessCount));
    m_bchuntian=false;
    m_bfanchun=false;
    m_bbaoshi=false;
    m_bjieshu=false;

	//ɾ����ʱ
	KillGameTimer(0);
	KillTimer(IDI_MOST_CARD);
	KillTimer(IDI_DISPATCH_CARD);

	__LEAVE_FUNCTION
}

//��Ϸ����(�ϴ����Ķ���)9-23
void CGameClientDlg::OnGameOptionSet()
{
	//��������
	CGameOption GameOption;
//	GameOption.m_dwCardHSpace=m_dwCardHSpace;
	GameOption.m_bEnableSound=IsEnableSound();
	//GameOption.m_bDeasilOrder=m_GameClientView.IsDeasilOrder();
	GameOption.m_bAllowLookOn=IsAllowUserLookon();

	//��������
	if (GameOption.DoModal()==IDOK)
	{
		//��ȡ����
//		m_bDeasilOrder=GameOption.m_bDeasilOrder;
//		m_dwCardHSpace=GameOption.m_dwCardHSpace;

		//���ÿؼ�
		EnableSound(GameOption.m_bEnableSound);
//		m_GameClientView.SetUserOrder(GameOption.m_bDeasilOrder);
		//m_GameClientView.m_HandCardControl[1].SetCardSpace(m_dwCardHSpace+50,0,20);
		AllowUserLookon(0,GameOption.m_bAllowLookOn);

		//��������
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("CardSpace"),m_dwCardHSpace);
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("DeasilOrder"),m_bDeasilOrder?TRUE:FALSE);
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("AllowLookon"),m_bDeasilOrder?TRUE:FALSE);
		AfxGetApp()->WriteProfileInt(TEXT("GameOption"),TEXT("CardSpace"),m_dwCardHSpace);
	}
	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse,UINT nTimerID )
{
	__ENTER_FUNCTION
		
	switch (nTimerID)
	{
        case IDI_OUT_CARD:			//�û�����
		{
			WORD wMeChairID=GetMeChairID();
			//�Զ����� 
			if ((((nElapse==0)||((m_bAutoPlay==true)&&(nElapse<15))))&&(wChairID==wMeChairID))
			{
				//��ʱ�ж�
				if ((IsLookonMode()==false)&&(m_bAutoPlay==false)&&((++m_wTimeOutCount)>=3)) 
				{
					//״̬�л�
					m_wTimeOutCount=0;
					m_bSystemTrustee=true;
					
					//�йܹ���
					SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOn, SW_HIDE);
					SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOff, SW_SHOW);
					InsertSystemString( TEXT( "��������γ�ʱ���л�Ϊ��ϵͳ�йܡ�ģʽ" ) );
					m_bAutoPlay = true;
			
					CMD_C_UserTrustee UserTrustee;
					ZeroMemory( &UserTrustee, sizeof( UserTrustee ) );
					UserTrustee.bTrustee = true;
					UserTrustee.wUserChairID = wMeChairID;

					SendData( SUB_C_TRUSTEE, &UserTrustee, sizeof( UserTrustee ));
				}
		        if (m_bSystemTrustee==true && m_wLastMostUser !=wChairID)
				{
					OnPassCard(0,0);
				}
				else if(m_wLastMostUser != wChairID && m_bAutoPlay==false)
				{
					OnPassCard(0,0);
				}
				else
				{
					//�Զ�����
					OnAutoOutCard( 0, 0 ) ;
					OnOutCard( 0, 0 ) ;
				}
				return false;
			}

			//��������
			const tagUserData *pUserData = GetUserData(wChairID);//�û���Ϣ
			const tagUserData *pUserData2 = GetUserData((wChairID+1)%3);//�û���Ϣ
			if (IsLookonMode() ==false && m_bHandCardCount[wMeChairID]<m_bTurnCardCount) return true;
			if ((nElapse<=5)&&(wChairID==wMeChairID)&&(IsLookonMode()==false))
			{
				PlaySound("GAME_WARN",false);
			}

			return true;
		}
	case IDI_START_GAME:		//��ʼ��Ϸ
		{
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) 
				{
					PostMessage(WM_CLOSE,0,0);
				}
				return false;
			}
			if ((nElapse<=5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) 
			{
				PlaySound("GAME_WARN",false);
			}

			return true;
		}
	case IDI_ENDSTART_GAME:         //��Ϸ������ʼ��ʱ��
		{
			BYTE byChairID = GetMeChairID();
			if (nElapse==29)
			{
				for (WORD i=0;i<GAME_PLAYER;i++) 
				{
					m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
				}
			}
			else if (nElapse==28)
			{
				if (m_IsSpring)
				{//����
					CString strPath ="";
					GetPicPath(strPath);
					strPath.Append("SPRING_EFFECT.png");
					m_GameClientView.m_SpringAction.SetActionImage(strPath,false,15);
					m_GameClientView.SetSpring();
					PlaySound("TSPRING",false);
					m_GameClientView.UpdateGameView(NULL);
					m_bchuntian=true;
				}
				else if (m_IsFanSpring)
				{//����
					CString strPath = "";
					GetPicPath(strPath);
					strPath.Append("FANSPRING_EFFECT.png");
					m_GameClientView.m_SpringAction.SetActionImage(strPath,false,15);
					m_GameClientView.SetSpring();
					PlaySound("TSPRING",false);	
					m_GameClientView.UpdateGameView(NULL);
					m_bfanchun=true;
				}
				else if (m_LargessCount[byChairID] >0)
				{
					PlayTaskFinishFlash(true,m_LargessCount[byChairID]);
					m_bbaoshi=true;
				}
				else
				{
					KillGameTimer(IDI_ENDSTART_GAME);
					ShowScoreView(true);
					m_ScoreView.GetWinLose(true); 
					m_ScoreView.SetCountDown(true);
					m_bjieshu=true;
				}		
			}
			else if (nElapse==25)
			{
			    if (m_LargessCount[byChairID] >0 && m_bbaoshi==false)
			     {
				    PlayTaskFinishFlash(true,m_LargessCount[byChairID]);	
			     }
			     else if(m_bjieshu==false)
			       {
					   KillGameTimer(IDI_ENDSTART_GAME);
				      ShowScoreView(true);
				      m_ScoreView.GetWinLose(true);
					  m_ScoreView.SetCountDown(true);
					  m_bjieshu=true;
			       }
			}
			else if (nElapse==23 && m_bjieshu==false)
			{
				KillGameTimer(IDI_ENDSTART_GAME);
				ShowScoreView(true);
				m_ScoreView.GetWinLose(true);
				m_ScoreView.SetCountDown(true);
				m_bjieshu=true;
			}
			return true;
		}
	case IDI_LAND_SCORE:		//�е���
		{
			WORD wMeChairID=GetMeChairID();
			if ( nElapse==0  )
			{//ʱ�䵽,Ĭ��Ϊ���е���
				if ((IsLookonMode()==false)&&(wChairID==wMeChairID)) OnCallLand(0,0);
				return false;
			}
			if ((nElapse<=5)&&(wChairID==wMeChairID)&&(IsLookonMode()==false)) 
			{
				PlaySound("GAME_WARN",false);
			}

			return true;
		}
	case IDI_SNATCH_LAND:        //������
		{
			WORD wMeChairID=GetMeChairID();
			if ( nElapse==0  )
			{//ʱ�䵽,Ĭ��Ϊ��������
				if ((IsLookonMode()==false)&&(wChairID==wMeChairID)) OnSnatchLand(0,0);
				return false;
			}
			if ((nElapse<=5)&&(wChairID==wMeChairID)&&(IsLookonMode()==false)) 
			{
				PlaySound("GAME_WARN",false);
			}

			return true;
		}
	case IDI_DOUBLE_SCORE:		//�ӱ���ʱ
		{
			if (nElapse==0 || ( nElapse <= 0 && m_bAutoPlay ))
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) 
				{//ʱ�䵽��Ĭ��Ϊ���ӱ���
					//������Ϣ
					SendMessage( IDM_DOUBLE_SCORE, 1, 1 );
				}
				return false;
			}
			if ((nElapse<=5)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));

			return true;
		}
	}

	__LEAVE_FUNCTION
	return false;
}

//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
	__ENTER_FUNCTION
		
	//�Թ�����
	//m_GameClientView.m_HandCardControl[1].SetDisplayFlag( ( IsLookonMode() == true ) && ( IsAllowLookon() == true ) );
	//m_GameClientView.m_GameCard.SetCardShow( true );

	__LEAVE_FUNCTION
}

//������Ϣ
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	__ENTER_FUNCTION
		
	switch (wSubCmdID)
	{
	case SUB_S_SEND_CARD:		//�����˿�
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_LAND_SCORE:	//�û��е���
		{
			return OnSubLandScore(pBuffer,wDataSize);
		}
	case SUB_S_USER_SNATCH_SCORE: //�û�������
		{
			return onSubSnatchLand(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_OUT_CARD:		//�û�����
		{
			return OnSubOutCard(pBuffer,wDataSize);
		}
	case SUB_S_PASS_CARD:		//��������
		{
			return OnSubPassCard(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_BRIGHT_START:	//���ƿ�ʼ
		{
			return OnSubBrightStart(pBuffer,wDataSize);			
		}
	case SUB_S_BRIGHT_CARD:	//�������
		{
			return OnSubBrightCard(pBuffer,wDataSize);
		}
	case SUB_C_TRUSTEE:			//����й�
		{
			return OnSubPlayerTrustee(pBuffer,wDataSize);
		}
	case SUB_S_DOUBLE_SCORE:	//�ӱ���Ϣ
		{
			return OnSubDoubleScore(pBuffer, wDataSize);
		}
	case SUB_S_USER_DOUBLE:		//��Ҽӱ�
		{
			return OnSubUserDouble(pBuffer, wDataSize);
		}
	}

   __LEAVE_FUNCTION
	return false;		
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	__ENTER_FUNCTION
		
	switch (cbGameStation)
	{
	case GS_WK_FREE:	//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//���ý���
			//
			m_GameClientView.SetBaseScore(pStatusFree->lBaseScore);
			for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID)
			{
				WORD wViewID = SwitchViewChairID(wChairID) ;
				bool bBrightStart = pStatusFree->bBrightStart[wChairID];
				m_GameClientView.SetBrightStart(wViewID, bBrightStart );
				if ( bBrightStart )
				{
					m_GameClientView.SetBrightTime( BRIGHT_START_TIME );
					m_GameClientView.ShowLandTime( true );
				}
			}

			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				SafeShowOrHideWnd(&m_GameClientView.m_btStart, SW_SHOW);
				//lq
				//m_GameClientView.m_btBrightStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
			}
			//SafeShowOrHideWnd(&m_GameClientView.m_btStart, SW_SHOW);
			//��������
			m_GameClientView.SetResetShouPaiNums();
			//�����˿�
			//m_GameClientView.m_HandCardControl[1].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));
			m_GameClientView.m_GameCard.SetCardShow( true );

			//����ʱ��
			SetGameTimer(GetMeChairID(),IDI_START_GAME,15);

			m_GameClientView.UpdateGameView(NULL);
			return true;
		}
	case GS_WK_SCORE:	//�з�״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusScore)) return false;
			CMD_S_StatusScore * pStatusScore=(CMD_S_StatusScore *)pBuffer;

			SetTaskWndShow(true);
			//���ñ���
			for (WORD i=0;i<GAME_PLAYER;i++) 
			{
				m_bCardCount[i]=17;
				m_bHandCardCount[i]=17;
				CopyMemory(m_bHandCardData[i],pStatusScore->bCardData[i],sizeof(BYTE)*m_bHandCardCount[i]);

				//�����˿�
				m_GameLogic.SortCardList(m_bHandCardData[i], 17, ST_ORDER);
			}
			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)	
			{
				WORD wViewChairID=SwitchViewChairID(i);
				if(IsLookonMode()==false)
				{
					m_GameClientView.SetCardCount(wViewChairID,m_bCardCount[i]);
				}
				m_GameClientView.SetLandScore(wViewChairID,pStatusScore->bScoreInfo[i],pStatusScore->bCallScorePhase,pStatusScore->bCallScorePhase);
			}

			m_GameClientView.ShowLandTitle(true);
			m_GameClientView.SetBaseScore(pStatusScore->lBaseScore);
			m_GameClientView.SetBrightTime(pStatusScore->bBrightTime);
			m_GameClientView.ShowLandTime(true);

			//��ť����
			if ((IsLookonMode()==false)&&(pStatusScore->wCurrentUser==GetMeChairID()))
			{
				/*SafeShowOrHideWnd(&m_GameClientView.m_btGiveUpScore, SW_SHOW);*/
				//m_GameClientView.m_btGiveUpScore.LoadRgnImage(AfxGetInstanceHandle(), IDB_GIVE_UP,RGB(255,0,255));

				if (pStatusScore->bCallScorePhase==CSD_NORMAL)
				{
			/*		SafeShowOrHideWnd(&m_GameClientView.m_btOneScore, SW_SHOW);
					SafeShowOrHideWnd(&m_GameClientView.m_btTwoScore, SW_SHOW);
					SafeShowOrHideWnd(&m_GameClientView.m_btThreeScore, SW_SHOW);

					SafeEnableWnd(&m_GameClientView.m_btOneScore, pStatusScore->bLandScore==0?TRUE:FALSE);
					SafeEnableWnd(&m_GameClientView.m_btTwoScore, pStatusScore->bLandScore<=1?TRUE:FALSE);
					SafeEnableWnd(&m_GameClientView.m_btThreeScore, pStatusScore->bLandScore<=2?TRUE:FALSE);*/
				}
				else if (pStatusScore->bCallScorePhase==CSD_SNATCHLAND)
				{
				/*	m_GameClientView.m_btSnatchLand.ShowWindow(SW_HIDE);*/
					//m_GameClientView.m_btGiveUpScore.LoadRgnImage(AfxGetInstanceHandle(),IDB_NOT_SNATCH_LAND,RGB(255,0,255));
				}
				//lq
				else if (pStatusScore->bCallScorePhase==CSD_BRIGHTCARD)
				{
					//m_GameClientView.m_btBrightCard.ShowWindow(SW_SHOW);
					//m_GameClientView.m_btGiveUpScore.SetButtonImage(IDB_NOT_BRIGHT_CARD,AfxGetInstanceHandle(),false);
				}
				//lq
			}

			//�����˿�
			BYTE bCardData[3]={0,0,0};
			WORD wMeChairID=GetMeChairID();
			m_GameClientView.SetCardBackData(bCardData,3);
			//m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData[wMeChairID],m_bHandCardCount[wMeChairID]);
			//m_GameClientView.m_HandCardControl[1].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));
			m_GameClientView.m_GameCard.SetCardData(m_bHandCardData[wMeChairID],m_bHandCardCount[wMeChairID]);
			
			m_GameClientView.m_GameCard.SetCardShow( true );

			for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
			{
				m_GameClientView.SetUserTrustee( SwitchViewChairID( wChairID ), pStatusScore->bUserTrustee[ wChairID ] );
				
				//�Լ��ж�
				if ( ! IsLookonMode() && wChairID == GetMeChairID() && pStatusScore->bUserTrustee[ wChairID ] )
				{
					//�йܹ���
					SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOn, SW_HIDE);
					SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOff, SW_SHOW);
					OnAutoPlay( 1, 1 );
				}
			}

			//�����˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID = SwitchViewChairID(i);
				m_GameClientView.m_GameCard.SetCardData(m_bHandCardData[i],m_bHandCardCount[i]);
			}

			//�����ж�
			//for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
			//{
			//	WORD wViewChairID = SwitchViewChairID( wChairID );

			//	//��ʾ�˿�
			//	if ( pStatusScore->bUserBrightCard[wChairID] )
			//	{
			//		m_GameClientView.m_HandCardControl[wViewChairID].ShowWindow(SW_SHOW);
			//		m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(true);
			//	}
			//}

			if ( IsLookonMode() )
			{
				SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOff, SW_HIDE);
				SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOn, SW_HIDE);
				SafeEnableWnd(&m_GameClientView.m_btAutoPlayOff, FALSE);
				SafeEnableWnd(&m_GameClientView.m_btAutoPlayOn, FALSE);
				SafeShowOrHideWnd(&m_GameClientView.m_btSortCard, SW_HIDE);
			}
			else 
			{
				SafeShowOrHideWnd(&m_GameClientView.m_btSortCard, SW_SHOW);
				SafeEnableWnd(&m_GameClientView.m_btSortCard, TRUE);
				SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOff, SW_HIDE);
				SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOn, SW_SHOW);
				SafeEnableWnd(&m_GameClientView.m_btAutoPlayOff, FALSE);
				SafeEnableWnd(&m_GameClientView.m_btAutoPlayOn, FALSE);
			}
			//�Թ����
			if (IsLookonMode()==true)
			{
				for (WORD wChairID=0; wChairID<GAME_PLAYER;++wChairID)
				{
					WORD wViewChairID = SwitchViewChairID( wChairID );
					if (1!=wViewChairID)
					{
						m_GameClientView.m_HandCardControl[wViewChairID].ShowWindow(SW_SHOW);
						m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(true);
						m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_bHandCardData[wViewChairID],m_bHandCardCount[wViewChairID]);
					}       
				}
			}
			//����ʱ��
			if ( pStatusScore->bCallScorePhase == CSD_NORMAL )
				SetGameTimer(pStatusScore->wCurrentUser,IDI_LAND_SCORE,10);
			else
				SetGameTimer(pStatusScore->wCurrentUser,IDI_LAND_SCORE,10);

			return true;
		}
	case GS_WK_DOUBLE_SCORE:	//�ӱ�״̬
			{
				//Ч������
				if (wDataSize!=sizeof(CMD_S_StatusDoubleScore)) return false;
				CMD_S_StatusDoubleScore * pStatusDoubleScore=(CMD_S_StatusDoubleScore *)pBuffer;

				SetTaskWndShow(true);
				//cxf ɾ���ӱ���ť  ��(��-��-��)����
				//���ð�ť
				//if ( ! IsLookonMode() )
				//{
					//bool bAllowDouble = pStatusDoubleScore->bAllowDouble;
					//m_GameClientView.m_btDoubleScore.ShowWindow( SW_SHOW );
					//m_GameClientView.m_btDoubleScore.EnableWindow( bAllowDouble ? TRUE : FALSE );
					//m_GameClientView.m_btNotDoubleScore.ShowWindow( SW_SHOW );
				//}
				//cxf end
				//��������
				//m_wLandUser = pStatusDoubleScore->wLandUser;
				m_GameClientView.SetLandUser(SwitchViewChairID(pStatusDoubleScore->wLandUser), pStatusDoubleScore->bLandScore );
				m_GameClientView.ShowLandTime(true);

				//״̬����
				m_GameClientView.SetLandScore(INVALID_CHAIR,0,CSD_GAMESTART,CSD_GAMESTART);

				//��������
				m_GameClientView.SetBaseScore(pStatusDoubleScore->lBaseScore);

				//�����˿�
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					m_bHandCardCount[i]=pStatusDoubleScore->bCardCount[i];
					CopyMemory(m_bHandCardData[i],pStatusDoubleScore->bCardData[i],m_bHandCardCount[i]);
					
					//�����˿�
					m_GameLogic.SortCardList(m_bHandCardData[i], m_bHandCardCount[i], ST_ORDER);

					WORD wViewChairID = SwitchViewChairID(i);
					if (IsLookonMode()==false)
					{
						m_GameClientView.SetCardCount(wViewChairID, m_bHandCardCount[i]);
					}	
				}
				m_GameClientView.SetCardBackData(pStatusDoubleScore->bBackCard,3);
				m_GameClientView.SetCardBackDisPlay(true);
				WORD wMeChairID = GetMeChairID();
				//m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData[wMeChairID],m_bHandCardCount[wMeChairID]);
			
				m_GameClientView.m_GameCard.SetCardData(m_bHandCardData[wMeChairID],m_bHandCardCount[wMeChairID]);
				
				if ( ! IsLookonMode() )
				{
					m_GameClientView.m_GameCard.SetCardShow(true);
					m_GameClientView.m_GameCard.SetCardPositively(true);

					//m_GameClientView.m_HandCardControl[1].SetDisplayFlag(true);
					//m_GameClientView.m_HandCardControl[1].SetPositively(true);
				}
				
				//�ӱ����
				for ( WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++ )
				{
					WORD wViewChairID = SwitchViewChairID( wChairID );
					m_GameClientView.SetDoubleScoreUser( wViewChairID, pStatusDoubleScore->bDoubleUser[wChairID] );
				}

				//�й�����
				for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
				{
					m_GameClientView.SetUserTrustee( SwitchViewChairID( wChairID ), pStatusDoubleScore->bUserTrustee[ wChairID ] );
					
					//�Լ��ж�
					if ( ! IsLookonMode() && wChairID == GetMeChairID() && pStatusDoubleScore->bUserTrustee[ wChairID ] )
					{
						//�йܹ���
						SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOn, SW_HIDE);
						SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOff, SW_SHOW);
						OnAutoPlay( 1, 1 );
					}
				}

				//������Ϣ
				//if ( ! IsLookonMode() ) SetGameTimer(GetMeChairID(), IDI_DOUBLE_SCORE, 30);

				//�����˿�
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					WORD wViewChairID = SwitchViewChairID(i);
					//m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_bHandCardData[i],m_bHandCardCount[i]);
					m_GameClientView.m_GameCard.SetCardData(m_bHandCardData[i],m_bHandCardCount[i]);
				}

				//�����ж�
				//for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
				//{
				//	WORD wViewChairID = SwitchViewChairID( wChairID );

				//	//��ʾ�˿�
				//	if ( pStatusDoubleScore->bUserBrightCard[wChairID] )
				//	{
				//		m_GameClientView.m_HandCardControl[wViewChairID].ShowWindow(SW_SHOW);
				//		m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(true);
				//	}
				//}

				if ( IsLookonMode() )
				{
					SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOff, SW_HIDE);
					SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOn, SW_HIDE);
					SafeEnableWnd(&m_GameClientView.m_btAutoPlayOff, FALSE);
					SafeEnableWnd(&m_GameClientView.m_btAutoPlayOn, FALSE);
					SafeShowOrHideWnd(&m_GameClientView.m_btSortCard, SW_HIDE);
				}
				else 
				{
					SafeShowOrHideWnd(&m_GameClientView.m_btSortCard, SW_SHOW);
					SafeEnableWnd(&m_GameClientView.m_btSortCard, TRUE);
					SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOff, SW_HIDE);
					SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOn, SW_SHOW);
					SafeEnableWnd(&m_GameClientView.m_btAutoPlayOff, FALSE);
					SafeEnableWnd(&m_GameClientView.m_btAutoPlayOn, FALSE);
				}
				//�Թ����
				if (IsLookonMode()==true)
				{
					for (WORD wChairID=0; wChairID<GAME_PLAYER;++wChairID)
					{
						WORD wViewChairID = SwitchViewChairID( wChairID );
						if (1!=wViewChairID)
						{
							m_GameClientView.m_HandCardControl[wViewChairID].ShowWindow(SW_SHOW);
							m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(true);
							m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_bHandCardData[wViewChairID],m_bHandCardCount[wViewChairID]);
						}       
					}
				}
				return true;
			}
	case GS_WK_PLAYING:		//��Ϸ״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			SetTaskWndShow(true);

			//���ñ���
			m_bTurnCardCount=pStatusPlay->bTurnCardCount;
			m_bTurnOutType=m_GameLogic.GetCardType(pStatusPlay->bTurnCardData,m_bTurnCardCount);
			CopyMemory(m_bTurnCardData,pStatusPlay->bTurnCardData,pStatusPlay->bTurnCardCount);

			//�����˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_bHandCardCount[i]=pStatusPlay->bCardCount[i];
				CopyMemory(m_bHandCardData[i],pStatusPlay->bCardData[i],m_bHandCardCount[i]);

				//�����˿�
				m_GameLogic.SortCardList(m_bHandCardData[i], m_bHandCardCount[i], ST_ORDER);
			}

			//���ý���
			for (BYTE i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_bCardCount[i]=pStatusPlay->bCardCount[i];
				if (IsLookonMode()==false)
				{
					m_GameClientView.SetCardCount(wViewChairID,pStatusPlay->bCardCount[i]);
				}			
				m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_bHandCardData[i],m_bHandCardCount[i]);
			}

			m_GameClientView.m_cbCallScorePhase = CSD_GAMESTART;
			//for (int k=0; k<GAME_PLAYER; k++)
			//{
			//	//m_GameClientView.SetDoubleScoreUser(k, pStatusPlay->bDoubleUser[k]);
			//}
			m_GameClientView.SetBombTime(pStatusPlay->wBombNums);
			m_GameClientView.SetBaseScore(pStatusPlay->lBaseScore);
			m_GameClientView.SetCardBackData(pStatusPlay->bBackCard,3);
			m_GameClientView.SetCardBackDisPlay(true);
			WORD wMeChairID = GetMeChairID();
			//m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData[wMeChairID],m_bHandCardCount[wMeChairID]);
			m_GameClientView.m_GameCard.SetCardData(m_bHandCardData[wMeChairID],m_bHandCardCount[wMeChairID]);
			m_GameClientView.SetLandUser(SwitchViewChairID(pStatusPlay->wLandUser),pStatusPlay->bLandScore);
			m_GameClientView.SetBrightTime(pStatusPlay->bBrightTime);
			m_GameClientView.ShowLandTime(false);

			//�������
			if ((IsLookonMode()==false)&&(pStatusPlay->wCurrentUser==GetMeChairID()))
			{
				SafeEnableWnd(&m_GameClientView.m_btOutCard, FALSE);
				SafeShowOrHideWnd(&m_GameClientView.m_btOutCard, SW_SHOW);
				SafeShowOrHideWnd(&m_GameClientView.m_btPassCard, SW_SHOW);
				SafeShowOrHideWnd(&m_GameClientView.m_btAutoOutCard, SW_SHOW);
				SafeEnableWnd(&m_GameClientView.m_btPassCard, (m_bTurnCardCount!=0)?TRUE:FALSE);
				SafeEnableWnd(&m_GameClientView.m_btAutoOutCard, TRUE);
			}

			//��������
			if (m_bTurnCardCount!=0)
			{
				WORD wViewChairID=SwitchViewChairID(pStatusPlay->wLastOutUser);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(m_bTurnCardData,m_bTurnCardCount);
			}

			if ( IsLookonMode() )
			{
				SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOff, SW_HIDE);
				SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOn, SW_HIDE);
				SafeEnableWnd(&m_GameClientView.m_btAutoPlayOff, FALSE);
				SafeEnableWnd(&m_GameClientView.m_btAutoPlayOn, FALSE);
				SafeShowOrHideWnd(&m_GameClientView.m_btSortCard, SW_HIDE);
			}
			else
			{
				SafeShowOrHideWnd(&m_GameClientView.m_btSortCard, SW_SHOW);
				SafeEnableWnd(&m_GameClientView.m_btSortCard, TRUE);
				SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOff, SW_HIDE);
				SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOn, SW_SHOW);
				SafeEnableWnd(&m_GameClientView.m_btAutoPlayOff, FALSE);
				SafeEnableWnd(&m_GameClientView.m_btAutoPlayOn, FALSE);
			}

			for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
			{
				m_GameClientView.SetUserTrustee( SwitchViewChairID( wChairID ), pStatusPlay->bUserTrustee[ wChairID ] );
				
				//�Լ��ж�
				if ( ! IsLookonMode() && wChairID == GetMeChairID() &&pStatusPlay->bUserTrustee[ wChairID ] )
				{
					//�йܹ���
					SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOn, SW_HIDE);
					SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOff, SW_SHOW);
					OnAutoPlay( 1, 1 );
				}
			}

			//���ö�ʱ��
			SetGameTimer(pStatusPlay->wCurrentUser,IDI_OUT_CARD,15);

			//�����˿�
			//m_GameClientView.m_HandCardControl[1].SetPositively(IsLookonMode()==false);
			//m_GameClientView.m_HandCardControl[1].SetDisplayFlag((IsLookonMode()==false)||(IsAllowLookon()==true));
	     	m_GameClientView.m_GameCard.SetCardShow(true);
			m_GameClientView.m_GameCard.SetCardPositively(IsLookonMode()==false);
			
			//�����ж�
			for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
			{
				WORD wViewChairID = SwitchViewChairID( wChairID );

				//��ʾ�˿�
				if ( pStatusPlay->bUserBrightCard[wChairID] )
				{
					/*m_GameClientView.m_HandCardControl[wViewChairID].ShowWindow(SW_SHOW);
					m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(true);*/
				}
				else if ( wViewChairID != 1 ) m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(false);
			}
			//�Թ����
			if (IsLookonMode()==true)
			{
				for (WORD wChairID=0; wChairID<GAME_PLAYER;++wChairID)
				{
					WORD wViewChairID = SwitchViewChairID( wChairID );
					if (1!=wViewChairID)
					{
						m_GameClientView.m_HandCardControl[wViewChairID].ShowWindow(SW_SHOW);
						m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(true);
						m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(m_bHandCardData[wViewChairID],m_bHandCardCount[wViewChairID]);
					}       
				}
			}

			return true;
		}
	}

	__LEAVE_FUNCTION
	return false;
}

//�����˿�
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	__ENTER_FUNCTION
			
	PlayWaitFlash(false);

	//����ǰ����ʾ�ȴ��з�����

	KillGameTimer(IDI_LAND_SCORE);
	m_GameClientView.ShowLandTitle(false);
    //����ǰ�Լҵ��Ʋ���ʾ
	m_GameClientView.SetCardCount(INVALID_CHAIR,0);
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendAllCard));
	if (wDataSize!=sizeof(CMD_S_SendAllCard)) return false;

	//��������
	CMD_S_SendAllCard * pSendCard=(CMD_S_SendAllCard *)pBuffer;

	
	ShowScoreView(false);

	////�Թ���� �Լ��˿�������ʾ
	if (IsLookonMode()==true)
	{
		m_GameClientView.m_HandCardControl[0].ShowWindow(SW_SHOW);
		m_GameClientView.m_HandCardControl[0].SetDisplayFlag(true);
		m_GameClientView.m_HandCardControl[2].ShowWindow(SW_SHOW);
		m_GameClientView.m_HandCardControl[2].SetDisplayFlag(true);
	}
	//��������
	m_wCurrentUser=pSendCard->wCurrentUser;
	m_bAutoPlay = false ;

	//��������
	for (WORD i=0;i<GAME_PLAYER;i++) 
	{
		m_bCardCount[i]=17;
		m_bHandCardCount[i]=17;
		CopyMemory(m_bHandCardData[i],pSendCard->bCardData[i],sizeof(pSendCard->bCardData[i]));

		//�����˿�
		m_GameLogic.SortCardList(m_bHandCardData[i], 17, ST_ORDER);
	}
	

	//���ý���
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetLandScore(i,0,CSD_NORMAL,CSD_NORMAL);
		m_GameClientView.SetPassFlag(i,false);
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_HandCardControl[i].SetCardData(NULL,0);
	}
	m_GameClientView.m_GameCard.SetCardData();
	WORD wMeChairID = GetMeChairID();
	//if ( ! IsLookonMode() && ! pSendCard->bUserBrightCard[wMeChairID] ) 
	//m_GameClientView.m_btBrightCard.ShowWindow( SW_SHOW );
	//else m_GameClientView.m_btBrightCard.ShowWindow( SW_HIDE );

	//���ý���
	m_GameClientView.SetBombTime(0);
	m_GameClientView.SetLandUser(INVALID_CHAIR,0);

	//�����˿�
	BYTE bBackCard[3]={0,0,0};
	m_GameClientView.SetCardBackDisPlay(false);//cxf ���õ���Ϊ������ʾ
	m_GameClientView.SetCardBackData(bBackCard,CountArray(bBackCard));
	//m_GameClientView.m_HandCardControl[1].SetDisplayFlag(((IsLookonMode()==false)||(IsAllowLookon()==true)));
	//m_GameClientView.m_HandCardControl[1].ShowWindow(SW_SHOW);
	

	//�����˿�
	//wMeChairID = GetMeChairID();
	
	DispatchUserCard(m_bHandCardData[wMeChairID],m_bHandCardCount[wMeChairID]);
	m_GameLogic.SortCardList(m_bHandCardData[wMeChairID],m_bHandCardCount[wMeChairID],ST_ORDER);

	////�����ж�
	//for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
	//{
	//	WORD wViewChairID = SwitchViewChairID( wChairID );

	//	//��ʾ�˿�
	//	if ( pSendCard->bUserBrightCard[wChairID] )
	//	{
	//		m_GameClientView.m_HandCardControl[wViewChairID].ShowWindow(SW_SHOW);
	//		m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(true);
	//	}
	//	else if ( wChairID != GetMeChairID() )
	//	{
	////		m_GameClientView.m_HandCardControl[wViewChairID].ShowWindow(SW_HIDE);
	//		m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(false);
	//	}
	//}
    m_GameClientView.UpdateGameView(NULL);

	__LEAVE_FUNCTION
	return true;	
}
//�û�������
bool CGameClientDlg::onSubSnatchLand(const void *pBuffer,WORD wDataSize)
{
	KillGameTimer(IDI_SNATCH_LAND);
	//У������
	ASSERT(wDataSize==sizeof(CMD_S_SnatchLand));
	if (wDataSize!=sizeof(CMD_S_SnatchLand)) return false;
	//��Ϣ����
	CMD_S_SnatchLand *pSnatchLand=(CMD_S_SnatchLand *)pBuffer;
    //��ȡ���ID
	WORD WmechairID=GetMeChairID();
	WORD wViewChairID=SwitchViewChairID(pSnatchLand->wSnatchUser);
	const tagUserData *pUserData = GetUserData(pSnatchLand->wSnatchUser);
	//�������
	m_GameClientView.SetMeBeiShu(pSnatchLand->bUserBeiShu[WmechairID]); //���ñ���
	//��ť��ʾ
	SafeShowOrHideWnd(&m_GameClientView.m_btSnatchLand,SW_HIDE);
	SafeEnableWnd(&m_GameClientView.m_btSnatchLand,FALSE);
	SafeShowOrHideWnd(&m_GameClientView.m_btGiveUpSnatchLand,SW_HIDE);
	SafeEnableWnd(&m_GameClientView.m_btGiveUpSnatchLand,FALSE);
	if (pSnatchLand->wCurrentUser != INVALID_CHAIR)
	{
		//�����Թ���Ҳ��ҡ���
		if (IsLookonMode()==false && pSnatchLand->wCurrentUser==WmechairID)
		{//��ť��ʾ
			SafeShowOrHideWnd(&m_GameClientView.m_btSnatchLand,SW_SHOW);
			SafeEnableWnd(&m_GameClientView.m_btSnatchLand,TRUE);
			SafeShowOrHideWnd(&m_GameClientView.m_btGiveUpSnatchLand,SW_SHOW);
			SafeEnableWnd(&m_GameClientView.m_btGiveUpSnatchLand,TRUE);
		}
		//����ʱ��
		SetGameTimer(pSnatchLand->wCurrentUser,IDI_SNATCH_LAND,10);
	}
	if (0==pSnatchLand->bSnatchLand)
	{
		//������������ʶ
		m_GameClientView.SetLandScore(wViewChairID,3,pSnatchLand->bPreCallScorePhase,pSnatchLand->bCallScorePhase);
		//��������
		PlayGameSoundLanguage(pUserData->cbGender, TEXT("GIVEUP_SNATCH"),false);
	}
	else
	{
		//������������ʶ
		m_GameClientView.SetLandScore(wViewChairID,4,pSnatchLand->bPreCallScorePhase,pSnatchLand->bCallScorePhase);
		//���Ŷ���
		CString strPath ="";
		GetPicPath(strPath);
	    PlaySound("BEI",false);
		strPath.Append("DOUBLE.png");
		m_GameClientView.m_DoubleAction.SetActionImage(strPath,true,15);
		m_GameClientView.PlayDoubelFlash();
		if(pSnatchLand->bSnatchLandCount==1)
		{
			PlayGameSoundLanguage(pUserData->cbGender,TEXT("SNATCH_1"),false);
		}
		else if(pSnatchLand->bSnatchLandCount==2)
		{
			PlayGameSoundLanguage(pUserData->cbGender,TEXT("SNATCH_2"),false);
		}
		else if (pSnatchLand->bSnatchLandCount==3)
		{
			PlayGameSoundLanguage(pUserData->cbGender,TEXT("SNATCH_3"),false);
		}
	}
	m_GameClientView.UpdateGameView(NULL);
	return true;
}
//�û��е���
bool CGameClientDlg::OnSubLandScore(const void * pBuffer, WORD wDataSize)
{
	KillGameTimer(IDI_LAND_SCORE);
	//У������
	ASSERT(wDataSize==sizeof(CMD_S_LandScore));
	if (wDataSize!=sizeof(CMD_S_LandScore)) return false;
	//��Ϣ����
	CMD_S_LandScore * pLandScore=(CMD_S_LandScore *)pBuffer;
	WORD wViewChairID=SwitchViewChairID(pLandScore->bLandUser);
	const tagUserData *pUserData = GetUserData(pLandScore->bLandUser);
	//���ñ���
	m_GameClientView.SetMeBeiShu(pLandScore->bUserBeiShu[GetMeChairID()]);
	if (pLandScore->wCurrentUser!= INVALID_CHAIR)
	{
		if (IsLookonMode()==false  && pLandScore->wCurrentUser==GetMeChairID())
		{//��ť��ʾ
			if (0==pLandScore->bLandScore)
			{
				SafeShowOrHideWnd(&m_GameClientView.m_btCallLand,SW_SHOW);
				SafeEnableWnd(&m_GameClientView.m_btCallLand,TRUE);
				SafeShowOrHideWnd(&m_GameClientView.m_btGiveUpCallLand,SW_SHOW);
				SafeEnableWnd(&m_GameClientView.m_btGiveUpCallLand,TRUE);
			}
			else
			{
				SafeShowOrHideWnd(&m_GameClientView.m_btSnatchLand,SW_SHOW);
				SafeEnableWnd(&m_GameClientView.m_btSnatchLand,TRUE);
				SafeShowOrHideWnd(&m_GameClientView.m_btGiveUpSnatchLand,SW_SHOW);
				SafeEnableWnd(&m_GameClientView.m_btGiveUpSnatchLand,TRUE);
			}
		}
		if(0==pLandScore->bLandScore)
		{
			//����ʱ��
			SetGameTimer(pLandScore->wCurrentUser,IDI_LAND_SCORE,10);
			//���ýе�����ʶ
			m_GameClientView.SetLandScore(wViewChairID,1,pLandScore->bPreCallScorePhase,pLandScore->bCallScorePhase);
			//��������
			PlayGameSoundLanguage(pUserData->cbGender, TEXT("GIVEUP_CALLLAND"),false);
		}
		else
		{
			//����ʱ��
			SetGameTimer(pLandScore->wCurrentUser,IDI_SNATCH_LAND,10);
			//���ýе�����ʶ
			m_GameClientView.SetLandScore(wViewChairID,2,pLandScore->bPreCallScorePhase,pLandScore->bCallScorePhase);
			//��������
			PlayGameSoundLanguage(pUserData->cbGender, TEXT("CALLLAND"),false);
		}
		
		m_GameClientView.UpdateGameView(NULL);
	}

	return true;
}
//�û��з�
//bool CGameClientDlg::OnSubLandScore(const void * pBuffer, WORD wDataSize)
//{
//	__ENTER_FUNCTION
//		
//	KillGameTimer(IDI_LAND_SCORE);
//	//Ч������
//	ASSERT(wDataSize==sizeof(CMD_S_LandScore));
//	if (wDataSize!=sizeof(CMD_S_LandScore)) return false;
//
//	//��Ϣ����
//	CMD_S_LandScore * pLandScore=(CMD_S_LandScore *)pBuffer;
//	//���ý���
//	WORD wViewChairID=SwitchViewChairID(pLandScore->bLandUser);
//
//	m_GameClientView.SetLandScore(wViewChairID,pLandScore->bLandScore,pLandScore->bCallScorePhase,pLandScore->bPreCallScorePhase);
//	m_GameClientView.SetLandUser(wViewChairID, pLandScore->bCurrentScore);
//
//	//��ǰ��ұ���
//	m_GameClientView.SetMeBeiShu(pLandScore->bUserBeiShu[GetMeChairID()]);
//	/*char chData[128]={0};
//	sprintf(chData, "%d", pLandScore->bUserBeiShu[GetMeChairID()]);
//	InsertSystemString(chData);*/
//	//m_GameClientView.SetBrightStart(INVALID_CHAIR, false);
//
//	//���ñ���
//	m_cbCallScorePhase = pLandScore->bCallScorePhase;
//
//	//�������
//	bool bUserBrightCard=false;
//	//if (pLandScore->bPreCallScorePhase==CSD_BRIGHTCARD && pLandScore->bCallScorePhase==CSD_BRIGHTCARD) 
//	//{
//	//	bUserBrightCard=true;
//	//}
//
//	//�������
//	if (bUserBrightCard==false)
//	{
//		if ((IsLookonMode()==false)&&( ( pLandScore->wCurrentUser == GetMeChairID() && pLandScore->bCallScorePhase!=CSD_BRIGHTCARD) || ( pLandScore->bCallScorePhase==CSD_BRIGHTCARD && 
//			! pLandScore->bBrightCardUser[GetMeChairID()] )))
//		{
//			ActiveGameFrame();
//			SafeShowOrHideWnd(&m_GameClientView.m_btGiveUpScore, SW_SHOW);
//			//m_GameClientView.m_btGiveUpScore.LoadRgnImage(AfxGetInstanceHandle(),IDB_GIVE_UP,RGB(255,0,255));
//
//			if (pLandScore->bCallScorePhase==CSD_NORMAL)
//			{
//				SafeShowOrHideWnd(&m_GameClientView.m_btOneScore, SW_SHOW);
//				SafeShowOrHideWnd(&m_GameClientView.m_btTwoScore, SW_SHOW);
//				SafeShowOrHideWnd(&m_GameClientView.m_btThreeScore, SW_SHOW);
//
//				SafeEnableWnd(&m_GameClientView.m_btOneScore, pLandScore->bCurrentScore==0?TRUE:FALSE);
//				SafeEnableWnd(&m_GameClientView.m_btTwoScore, pLandScore->bCurrentScore<=1?TRUE:FALSE);
//				SafeEnableWnd(&m_GameClientView.m_btThreeScore, pLandScore->bCurrentScore<=2?TRUE:FALSE);
//			}
//			//else if (pLandScore->bCallScorePhase==CSD_SNATCHLAND)
//			//{
//			//	m_GameClientView.m_btSnatchLand.ShowWindow(SW_SHOW);
//			//	m_GameClientView.m_btGiveUpScore.SetButtonImage(IDB_NOT_SNATCH_LAND,AfxGetInstanceHandle(),false);
//			//}
//			//else if (pLandScore->bCallScorePhase==CSD_BRIGHTCARD)
//			//{
//			//	//m_GameClientView.m_btBrightCard.ShowWindow(SW_SHOW);
//			//	//m_GameClientView.m_btGiveUpScore.SetButtonImage(IDB_NOT_BRIGHT_CARD,AfxGetInstanceHandle(),false);
//			//}
//		}
//
//		//����ʱ��
//		if ( pLandScore->bCallScorePhase==CSD_BRIGHTCARD )
//			SetGameTimer(GetMeChairID(),IDI_LAND_SCORE,10);
//		else if ( pLandScore->bCallScorePhase != CSD_NORMAL )
//			SetGameTimer(pLandScore->wCurrentUser,IDI_LAND_SCORE,10);
//		else 
//			SetGameTimer(pLandScore->wCurrentUser,IDI_LAND_SCORE,15);
//	}
//
//	//��������
//	const tagUserData *pUserData = GetUserData(pLandScore->bLandUser);
//
//	if ( pLandScore->bLandScore == 1 )
//	{
//		PlayGameSoundLanguage(pUserData->cbGender, TEXT("JIAOFEN_1"),false);
//	}
//	else if ( pLandScore->bLandScore == 2 )
//	{
//		PlayGameSoundLanguage(pUserData->cbGender, TEXT("JIAOFEN_2"),false);
//	}
//	else if ( pLandScore->bLandScore == 3 )
//	{
//		PlayGameSoundLanguage(pUserData->cbGender, TEXT("JIAOFEN_3"),false);
//	}
//	else
//	{
//		PlayGameSoundLanguage(pUserData->cbGender, TEXT("JIAOFEN_0"),false);
//	}
//	m_GameClientView.UpdateGameView(NULL);
//
//	__LEAVE_FUNCTION
//	return true;
//}

//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	__ENTER_FUNCTION
			
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;

	//��Ϣ����
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	SetTaskWndShow(true);

	//���ñ���
	m_wBombTime=1;
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	m_wLandUser=pGameStart->wLandUser;
	m_bCardCount[pGameStart->wLandUser]=20; 
	ZeroMemory(m_bTurnCardData,sizeof(m_bTurnCardData));
	//m_GameClientView.ShowLandTime(false);
	//m_GameClientView.SetBrightStart(INVALID_CHAIR, false);

	KillTimer(IDI_DISPATCH_CARD);
	m_GameClientView.ShowLandTitle(false);
	m_wLastMostUser=pGameStart->wLandUser;

	//���ý���
	m_GameClientView.SetLandScore(INVALID_CHAIR,0,CSD_GAMESTART,CSD_GAMESTART);
	m_GameClientView.SetDoubleScoreUser(INVALID_CHAIR, false);
	m_GameClientView.SetCardBackData(pGameStart->bBackCardData,CountArray(pGameStart->bBackCardData));
	m_GameClientView.SetCardBackDisPlay(true);
	if (IsLookonMode()==false)
	{
		m_GameClientView.SetCardCount(SwitchViewChairID(pGameStart->wLandUser),20);
	}
	SafeShowOrHideWnd(&m_GameClientView.m_btCallLand,SW_HIDE);
	SafeShowOrHideWnd(&m_GameClientView.m_btGiveUpCallLand,SW_HIDE);
	SafeShowOrHideWnd(&m_GameClientView.m_btSnatchLand,SW_HIDE);
	SafeShowOrHideWnd(&m_GameClientView.m_btGiveUpSnatchLand,SW_HIDE);

    //���õ�����������
	WORD wLandUser=pGameStart->wLandUser;
	m_bHandCardCount[wLandUser]=20;
	CopyMemory(&m_bHandCardData[wLandUser][17],pGameStart->bBackCardData,sizeof(pGameStart->bBackCardData));
	m_GameLogic.SortCardList(m_bHandCardData[wLandUser],m_bHandCardCount[wLandUser],ST_ORDER);
	m_GameClientView.m_HandCardControl[SwitchViewChairID(wLandUser)].SetCardData(m_bHandCardData[wLandUser],m_bHandCardCount[wLandUser]);

	m_GameClientView.SetLandUser(SwitchViewChairID(pGameStart->wLandUser),pGameStart->bLandScore);
	if(pGameStart->wLandUser == GetMeChairID())
	{
		m_GameClientView.m_GameCard.SetCardData(m_bHandCardData[wLandUser],m_bHandCardCount[wLandUser]);
	}

	//�������
	if (IsLookonMode()==false) 
	{
	   m_GameClientView.m_GameCard.SetCardShow(true);

	   BYTE byMeViewID = SwitchViewChairID(GetMeChairID());
	   m_GameClientView.m_GameCard.SetCardData(pGameStart->bCardData,pGameStart->bCardNum);
	}
	//��ǰ���
	if ((IsLookonMode()==false)&&(pGameStart->wCurrentUser==GetMeChairID()))
	{
		ActiveGameFrame();
		SafeEnableWnd(&m_GameClientView.m_btOutCard, FALSE);
		SafeShowOrHideWnd(&m_GameClientView.m_btOutCard, SW_SHOW);
		SafeEnableWnd(&m_GameClientView.m_btPassCard, FALSE);
		SafeShowOrHideWnd(&m_GameClientView.m_btPassCard, SW_SHOW);
		SafeShowOrHideWnd(&m_GameClientView.m_btAutoOutCard, SW_SHOW);
		SafeEnableWnd(&m_GameClientView.m_btAutoOutCard,FALSE);
	}
	//���ó���ʱ��
	SetGameTimer(pGameStart->wCurrentUser,IDI_OUT_CARD,15);

	//��������
	m_GameClientView.SetResetShouPaiNums();

	m_GameClientView.UpdateGameView(NULL);

	__LEAVE_FUNCTION
	return true;
}

//�û�����
bool CGameClientDlg::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	__ENTER_FUNCTION
		
	//////////////////////////////
	//char str[10]={'a'};
	//InsertSystemString(str);
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard))
	{
		return false;
	}
	//��������
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;
	WORD wHeadSize=sizeof(CMD_S_OutCard)-sizeof(pOutCard->bCardData);
	//Ч������
	//ASSERT(wDataSize>=wHeadSize);
	//if (wDataSize<wHeadSize) return false;
	//ASSERT(wDataSize==(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0])));
	//if (wDataSize!=(wHeadSize+pOutCard->bCardCount*sizeof(pOutCard->bCardData[0]))) return false;

	//ɾ����ʱ��
	KillTimer(IDI_MOST_CARD);
	KillGameTimer(IDI_OUT_CARD);

	//���ֳ����û�
	m_wLastMostUser=pOutCard->wOutCardUser;
	/*char chData[128]={0};
	sprintf(chData, "%d,%d", m_wLastMostUser ,pOutCard->wCurrentUser);
	InsertSystemString(chData);*/

	//���ñ���
	m_GameClientView.SetResetSYPaiNums();

	//��������
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);
	m_bCardCount[pOutCard->wOutCardUser]-=pOutCard->bCardCount;
	if (IsLookonMode()==false)
	{
		m_GameClientView.SetCardCount(wOutViewChairID,m_bCardCount[pOutCard->wOutCardUser]);
	}
	m_GameClientView.SetLandScore(INVALID_CHAIR,0,CSD_GAMESTART,CSD_GAMESTART);

	//SafeShowOrHideWnd(&m_GameClientView.m_btCardDao, SW_HIDE);
	//SafeShowOrHideWnd(&m_GameClientView.m_btCardGen, SW_HIDE);
	//SafeShowOrHideWnd(&m_GameClientView.m_btCardFan, SW_HIDE);
	
	//���õ�ǰ����
	m_GameClientView.SetMeBeiShu(pOutCard->bUserBeiShu[GetMeChairID()]);

	//С�ڵ���������ʱ��ʾ�йܰ�ť lq;
	if (pOutCard->bHandCardCount<=4 && GetMeChairID()==pOutCard->wOutCardUser)
	{
		SafeEnableWnd(&m_GameClientView.m_btAutoPlayOn,TRUE);
	}

	WORD wCurrentUser=pOutCard->wCurrentUser;
	WORD wMechairID=GetMeChairID();
	////cxf ���أ���-��-������ť
	//if (IsLookonMode() == false && wCurrentUser==wMechairID) 
	//{//��һ��ң��������ʱ����һ����ң�
	//	if (wMechairID==pOutCard->wBankerUser)
	//	{//ׯ��
	//		if (1 == pOutCard->bCircuit[wCurrentUser])
	//		{//���ƴ��� С��2
	//			if ((pOutCard->bIsDouble[(pOutCard->wBankerUser +1)%GAME_PLAYER] 
	//					|| pOutCard->bIsDouble[(pOutCard->wBankerUser + 2)%GAME_PLAYER]) 
	//					&& !pOutCard->bIsDouble[pOutCard->wBankerUser])
	//			{//ũ���� "��"��"��" ����ׯ�һ�û��"����"                       
	//				SafeShowOrHideWnd(&m_GameClientView.m_btCardFan, SW_SHOW);
	//			}
	//		}
	//	}
	//	else
	//	{//ũ��
	//		if (pOutCard->bCircuit[wCurrentUser] < 1)
	//		{//���ƴ��� С��1
	//			if (wCurrentUser == (pOutCard->wBankerUser+1)%GAME_PLAYER)
	//			{//��һ��ũ��
	//				if (!pOutCard->bIsDouble[(pOutCard->wBankerUser+1)%GAME_PLAYER])
	//				{//֮ǰû�е������Ե���
	//					SafeShowOrHideWnd(&m_GameClientView.m_btCardDao, SW_SHOW);
	//				}
	//			}
	//			else if ((pOutCard->wBankerUser+2)%GAME_PLAYER == wCurrentUser)
	//			{//�ڶ���ũ��.
	//				if (pOutCard->bIsDouble[(wCurrentUser+2)%GAME_PLAYER] 
	//						&& !pOutCard->bIsDouble[(wCurrentUser)%GAME_PLAYER])
	//				{//�ӱ���,���Ը�.
	//					SafeShowOrHideWnd(&m_GameClientView.m_btCardGen, SW_SHOW);
	//				}
	//				else if(!pOutCard->bIsDouble[(wCurrentUser)%GAME_PLAYER])
	//				{//û�ӱ�,���Ե�.
	//					SafeShowOrHideWnd(&m_GameClientView.m_btCardDao, SW_SHOW);
	//				}
	//			}
	//		}
	//	}
	//}
	//if (2 == pOutCard->bCircuit[pOutCard->wBankerUser])
	//{
		for (BYTE i=0; i<GAME_PLAYER; i++)
		{
			m_GameClientView.SetDoubleScoreUser(SwitchViewChairID(i), false);
		}
	//}
	//---end
	//�Ƴ�����
    /*if (2 == pOutCard->bCircuit[pOutCard->wBankerUser])
    {
		m_GameClientView.SetCardBackData(NULL,0);
    }*/
	//��������
	if ((IsLookonMode()==true)||(wMechairID!=pOutCard->wOutCardUser))
	{
		m_GameClientView.m_UserCardControl[wOutViewChairID].SetCardData(pOutCard->bCardData,pOutCard->bCardCount);
	}

	//��������
	if (m_bTurnCardCount==0)
	{
		for (WORD i=0;i<GAME_PLAYER;i++) 
		{
			if (i==pOutCard->wOutCardUser) continue;
			m_GameClientView.m_UserCardControl[SwitchViewChairID(i)].SetCardData(NULL,0);
		}
		m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
	}

	//������Ϣ
	const tagUserData *pUserData = GetUserData(pOutCard->wOutCardUser);//�û���Ϣ

	bool bWarmingCard = false;
	/*if(m_bCardCount[pOutCard->wOutCardUser]<=2) 
	{
		WORD wViewChairID = SwitchViewChairID(pOutCard->wOutCardUser) ;

		if ( pOutCard->wOutCardUser!=GetMeChairID() )
		{
			PlayGameSound(AfxGetInstanceHandle(),TEXT("WARMING_CARD"));
			bWarmingCard = true;
		}

		m_GameClientView.SetShowWarming(wViewChairID, true) ;
	}*/
	if (m_bCardCount[pOutCard->wOutCardUser]<=2&&wCurrentUser!=INVALID_CHAIR)
	{//������ʾ
		WORD wViewChairID = SwitchViewChairID(pOutCard->wOutCardUser) ;
		/*m_GameClientView.SetUserShouPaiNums(wViewChairID, m_bCardCount[pOutCard->wOutCardUser]);*/
		switch (m_bCardCount[pOutCard->wOutCardUser])
		{
		case 1://1��
			PlayGameSoundLanguage(pUserData->cbGender, TEXT("BAOJING_1"),false);
			break;
		case 2://2��
			PlayGameSoundLanguage(pUserData->cbGender, TEXT("BAOJING_2"),false);
			break;
		}	
		bWarmingCard = true;
		m_GameClientView.SetShowWarming(wViewChairID, true) ;
	}

	//��¼����
	m_bTurnCardCount=pOutCard->bCardCount;
	m_bTurnOutType=m_GameLogic.GetCardType(pOutCard->bCardData,pOutCard->bCardCount);
	CopyMemory(m_bTurnCardData,pOutCard->bCardData,sizeof(BYTE)*pOutCard->bCardCount);

	m_tagOldCard.cbCardCount = m_bTurnCardCount;
    CopyMemory(m_tagOldCard.cbResultCard, m_bTurnCardData, m_bTurnCardCount);

	//ը���ж�
	if ((m_bTurnOutType==CT_BOMB_CARD)||(m_bTurnOutType==CT_MISSILE_CARD))
	{
		/*if(m_wBombTime > 1)
		{
			PlayGameSoundLanguage(pUserData->cbGender, TEXT("ZHADAN_2"),false);
			bWarmingCard = true;
		}*/
		m_wBombTime*=2;
		m_GameClientView.SetBombTime(pOutCard->wBombNums);
	}

	//�Լ������˿�(�Թ۽���)
	if (IsLookonMode()==true/*||(pOutCard->wOutCardUser!=wMechairID)*/)
	{
		//ɾ���˿�
		WORD wOutCardUser=pOutCard->wOutCardUser;
		BYTE bSourceCount=m_bHandCardCount[wOutCardUser];
		m_bHandCardCount[wOutCardUser]-=pOutCard->bCardCount;
		m_GameLogic.RemoveCard(pOutCard->bCardData,pOutCard->bCardCount,m_bHandCardData[wOutCardUser],bSourceCount);

		//���ý���
		m_GameClientView.m_HandCardControl[SwitchViewChairID(wOutCardUser)].SetCardData(m_bHandCardData[wOutCardUser],m_bHandCardCount[wOutCardUser]);
		if (wOutCardUser==wMechairID)
		{
			m_GameClientView.m_GameCard.SetCardData(m_bHandCardData[wMechairID],m_bHandCardCount[wMechairID]);
		}
	}

	//����ж�(���)
	if (wCurrentUser==pOutCard->wOutCardUser)
	{
		//���ñ���
		m_bTurnCardCount=0;
		m_bTurnOutType=CT_ERROR;
		m_wMostUser=wCurrentUser;
		memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
		m_tagOldCard.cbCardCount=0;
		memset(m_tagOldCard.cbResultCard, 0,sizeof(m_tagOldCard.cbResultCard));

		//���ý���
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			if (i!=pOutCard->wOutCardUser)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.SetPassFlag(wViewChairID,true);
				m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
			}
		}

		//��������
		if ( !bWarmingCard )
		{
			CString strPath = "";
			GetPicPath(strPath);
			PlayGameSoundLanguage(pUserData->cbGender, TEXT("HUOJIAN"),false);
			PlaySound("THUOJIAN",false);
			strPath.Append("HUOJIAN.png");
			m_GameClientView.m_HuoJianAction.SetActionImage(strPath,false,1,false);
			m_GameClientView.SetHuoJian(true);
		}
		
		//���ö�ʱ��
		SetTimer(IDI_MOST_CARD,3000,NULL);

		m_GameClientView.UpdateGameView(NULL);
		return true;
	}
	//��ʾ����ͼƬ  lq
	if (wCurrentUser==wMechairID && wCurrentUser != pOutCard->wOutCardUser)
	{//��ǰ�����û�
		tagOutCardResult OutCardResult;
		WORD wMeChairID = GetMeChairID();
		m_GameLogic.SearchOutCard(m_bHandCardData[wMeChairID],m_bHandCardCount[wMeChairID],m_bTurnCardData,m_bTurnCardCount,OutCardResult);

		//���ý���
		if (OutCardResult.cbCardCount<=0)
		{//û�д�����
			m_GameClientView.SetShowBigness(true);
		}
		else
		{//����
			m_GameClientView.SetShowBigness(false);
		}
	}
	
	//
	//��������
	if (!bWarmingCard&& wCurrentUser!=INVALID_CHAIR )
	{
		BYTE cbOutCardType = m_GameLogic.GetCardType(pOutCard->bCardData, pOutCard->bCardCount );
		BYTE cbOutCardData=m_GameLogic.GetCardValue(pOutCard->bCardData[0]);
		CString strPath = "";
		GetPicPath(strPath);
		switch (cbOutCardType)
		{
		case CT_BOMB_CARD://ը��
			PlayGameSoundLanguage(pUserData->cbGender, TEXT("ZHADAN_1"),false);
			PlaySound("TZHADAN",false);
			strPath.Append("BOMB_EFFECT.png");
			m_GameClientView.m_ActionWnd[wOutViewChairID].SetActionImage(strPath,true,8);
			m_GameClientView.PlayActionFlash(wOutViewChairID,MARK_ACTION_ZHADAN);
			break;
		case CT_DOUBLE://����
			{
				switch (cbOutCardData)
				{
				case 1:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_DB_1"),false);
					break;
				case 2:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_DB_2"),false);
					break;
				case 3:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_DB_3"),false);
					break;
				case 4:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_DB_4"),false);
					break;
				case 5:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_DB_5"),false);
					break;
				case 6:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_DB_6"),false);
					break;
				case 7:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_DB_7"),false);
					break;
				case 8:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_DB_8"),false);
					break;
				case 9:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_DB_9"),false);
					break;
				case 10:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_DB_10"),false);
					break;
				case 11:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_DB_11"),false);
					break;
				case 12:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_DB_12"),false);
					break;
				case 13:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_DB_13"),false);
					break;
				}

			}
			break;
		case CT_THREE://����
			PlayGameSoundLanguage(pUserData->cbGender, TEXT("SANZHANG"),false);
			break;
		case CT_SINGLE_LINE://˳��
			PlayGameSoundLanguage(pUserData->cbGender, TEXT("SHUN"),false);
			PlaySound("TSHUNLIAN",false);
			if (pOutCard->bCardCount>=7)
			{
				strPath.Append("CHANGSHUNZI.png");
                m_GameClientView.m_ActionWnd[wOutViewChairID].SetActionImage(strPath,true,15);
			}
			else
			{
				strPath.Append("SHUNZI.png");
				m_GameClientView.m_ActionWnd[wOutViewChairID].SetActionImage(strPath,true,15);
			}			
            m_GameClientView.PlayActionFlash(wOutViewChairID,MARK_ACTION_SHUNLIAN);
			break;
		case CT_DOUBLE_LINE://����
			PlayGameSoundLanguage(pUserData->cbGender, TEXT("LIANDUI"),false);
			PlaySound("TSHUNLIAN",false);
			strPath.Append("LIANDUI.png");
			m_GameClientView.m_ActionWnd[wOutViewChairID].SetActionImage(strPath,true,15);
			m_GameClientView.PlayActionFlash(wOutViewChairID,MARK_ACTION_SHUNLIAN);
			break;
		case CT_THREE_LINE://����(�ɻ�)
			if(4<pOutCard->bCardCount)
			{
				PlayGameSoundLanguage(pUserData->cbGender, TEXT("FEIJIDAICHIBANG"),false);
				PlaySound("TFEIJI",false);
				strPath.Append("FEIJI.png");
				m_GameClientView.m_BActionWnd.SetActionImage(strPath,true,1,false);
				m_GameClientView.SetFeiJi(true);
			}		
			break;
		case CT_THREE_LINE_TAKE_ONE://���Ŵ�һ��
			if(4<pOutCard->bCardCount)
			{
				PlayGameSoundLanguage(pUserData->cbGender, TEXT("FEIJIDAICHIBANG"),false);
				PlaySound("TFEIJI",false);
				strPath.Append("FEIJI.png");
				m_GameClientView.m_BActionWnd.SetActionImage(strPath,true,1,false);
				m_GameClientView.SetFeiJi(true);
			}
			else
			{
				PlayGameSoundLanguage(pUserData->cbGender, TEXT("SANDAIYI"),false);
			}
			break;
		case CT_THREE_LINE_TAKE_TWO://���Ŵ�һ��
			if(5<pOutCard->bCardCount)
			{
				PlayGameSoundLanguage(pUserData->cbGender, TEXT("FEIJIDAICHIBANG"),false);
				PlaySound("TFEIJI",false);
				strPath.Append("FEIJI.png");
				m_GameClientView.m_BActionWnd.SetActionImage(strPath,true,1,false);
				m_GameClientView.SetFeiJi(true);
			}
			else
			{
				PlayGameSoundLanguage(pUserData->cbGender, TEXT("SANDAIRE"),false);
			}
			break;
		case CT_FOUR_LINE_TAKE_ONE://���Ŵ�һ��
			PlayGameSoundLanguage(pUserData->cbGender, TEXT("SIDAIYI"),false);
			break;
		case CT_FOUR_LINE_TAKE_TWO://���Ŵ�����
			PlayGameSoundLanguage(pUserData->cbGender, TEXT("SIDAIRE"),false);
			break;
		case CT_SINGLE:
			{
				switch (cbOutCardData)
				{
				case 1:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_1"),false);
					break;
				case 2:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_2"),false);
					break;
				case 3:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_3"),false);
					break;
				case 4:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_4"),false);
					break;
				case 5:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_5"),false);
					break;
				case 6:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_6"),false);
					break;
				case 7:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_7"),false);
					break;
				case 8:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_8"),false);
					break;
				case 9:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_9"),false);
					break;
				case 10:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_10"),false);
					break;
				case 11:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_11"),false);
					break;
				case 12:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_12"),false);
					break;
				case 13:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_13"),false);
					break;
				case 14:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_14"),false);
					break;
				case 15:
					PlayGameSoundLanguage(pUserData->cbGender,TEXT("MALE_15"),false);
					break;
				}				
			}
			break;
		default://Ĭ����ͨ��������
			PlaySound("OUT_CARD",false);
			break;
		}
	}

	//�������
	if (wCurrentUser!=INVALID_CHAIR)
	{
		WORD wViewChairID=SwitchViewChairID(wCurrentUser);
		m_GameClientView.SetPassFlag(wViewChairID,false);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
	}
	//�������
	if ((IsLookonMode()==false)&&(wCurrentUser==wMechairID))
	{
		if(m_bSystemTrustee==true && m_wLastMostUser!=wMechairID)
		{
			ActiveGameFrame();
			SafeShowOrHideWnd(&m_GameClientView.m_btOutCard, SW_HIDE);
			SafeShowOrHideWnd(&m_GameClientView.m_btPassCard, SW_HIDE);
			SafeShowOrHideWnd(&m_GameClientView.m_btAutoOutCard, SW_HIDE);
		    NoHitPassOutCard();
			m_GameClientView.UpdateGameView(NULL);
			return true;
		}
		else
		{
			ActiveGameFrame();
			SafeEnableWnd(&m_GameClientView.m_btPassCard,TRUE);
			SafeShowOrHideWnd(&m_GameClientView.m_btOutCard, SW_SHOW);
			SafeShowOrHideWnd(&m_GameClientView.m_btPassCard, SW_SHOW);
			SafeEnableWnd(&m_GameClientView.m_btAutoOutCard,TRUE);
			SafeShowOrHideWnd(&m_GameClientView.m_btAutoOutCard, SW_SHOW);
			SafeEnableWnd(&m_GameClientView.m_btOutCard,(VerdictOutCard()==true)?TRUE:FALSE);
		}
		::memset(&m_tagNewCard, 0, sizeof(m_tagNewCard));
		//lq
	}
	//����ʱ��
	if (wCurrentUser!=INVALID_CHAIR)
	{
		SetGameTimer(pOutCard->wCurrentUser,IDI_OUT_CARD,15);
	}
	m_GameClientView.UpdateGameView(NULL);

	__LEAVE_FUNCTION
	return true;
}

//��������
bool CGameClientDlg::OnSubPassCard(const void * pBuffer, WORD wDataSize)
{
	__ENTER_FUNCTION
		
	//Ч������
	if (wDataSize!=sizeof(CMD_S_PassCard)) return false;
	CMD_S_PassCard * pPassCard=(CMD_S_PassCard *)pBuffer;

	//ɾ����ʱ��
	KillGameTimer(IDI_OUT_CARD);

	WORD wMechairID=GetMeChairID();
	//�������
	if ((IsLookonMode()==true)||(pPassCard->wPassUser!=wMechairID))
	{
		WORD wViewChairID=SwitchViewChairID(pPassCard->wPassUser);
		m_GameClientView.SetPassFlag(wViewChairID,true);
		m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);
	}
	//һ���ж�
	if (pPassCard->bNewTurn==TRUE)
	{
		m_bTurnCardCount=0;
		m_bTurnOutType=CT_ERROR;
		memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
		m_tagOldCard.cbCardCount=0;
		memset(m_tagOldCard.cbResultCard, 0,sizeof(m_tagOldCard.cbResultCard));
	}
	//���ý���
	WORD wViewChairID=SwitchViewChairID(pPassCard->wCurrentUser);
	m_GameClientView.SetPassFlag(wViewChairID,false);
	m_GameClientView.m_UserCardControl[wViewChairID].SetCardData(NULL,0);

	//SafeShowOrHideWnd(&m_GameClientView.m_btCardDao, SW_HIDE);
	//SafeShowOrHideWnd(&m_GameClientView.m_btCardGen, SW_HIDE);
	//SafeShowOrHideWnd(&m_GameClientView.m_btCardFan, SW_HIDE);

	//cxf ���أ���-��-������ť
	//if (IsLookonMode() == false && pPassCard->wCurrentUser==wMechairID) 
	//{//��һ��ң��������ʱ����һ����ң�
	//	if (GetMeChairID()==pPassCard->wBankerUser)
	//	{//ׯ��
	//		if (1 == pPassCard->bCircuit[pPassCard->wCurrentUser])
	//		{//���ƴ��� С��2
	//			if ((pPassCard->bIsDouble[(pPassCard->wBankerUser +1)%GAME_PLAYER] 
	//			|| pPassCard->bIsDouble[(pPassCard->wBankerUser + 2)%GAME_PLAYER]) 
	//				&& !pPassCard->bIsDouble[pPassCard->wBankerUser])
	//			{//ũ���� "��"��"��" ����ׯ�һ�û��"����"
	//				SafeShowOrHideWnd(&m_GameClientView.m_btCardFan, SW_SHOW);
	//			}
	//		}
	//	}
	//	else
	//	{//ũ��
	//		if (pPassCard->bCircuit[pPassCard->wCurrentUser] < 1)
	//		{//���ƴ��� С��1
	//			if (pPassCard->wCurrentUser == (pPassCard->wBankerUser+1)%GAME_PLAYER)
	//			{//��һ��ũ�񣬿��Ե���
	//				SafeShowOrHideWnd(&m_GameClientView.m_btCardDao, SW_SHOW);
	//			}
	//			else if ((pPassCard->wBankerUser+2)%GAME_PLAYER == pPassCard->wCurrentUser)
	//			{//�ڶ���ũ��.
	//				if (pPassCard->bIsDouble[(pPassCard->wCurrentUser+2)%GAME_PLAYER] 
	//				&& !pPassCard->bIsDouble[(pPassCard->wCurrentUser)%GAME_PLAYER])
	//				{//�ӱ���,���Ը�.
	//					SafeShowOrHideWnd(&m_GameClientView.m_btCardGen, SW_SHOW);
	//				}
	//				else if(!pPassCard->bIsDouble[(pPassCard->wCurrentUser)%GAME_PLAYER])
	//				{//û�ӱ�,���Ե�.
	//					SafeShowOrHideWnd(&m_GameClientView.m_btCardDao, SW_SHOW);
	//				}

	//			}
	//		}
	//	}
	//}
	/*if (2 == pPassCard->bCircuit[pPassCard->wBankerUser])
	{*/
		for (BYTE i=0; i<GAME_PLAYER; i++)
		{
			m_GameClientView.SetDoubleScoreUser(SwitchViewChairID(i), false);
		}
	/*}*/
	//---end

	//�������
	if ((IsLookonMode()==false)&&(pPassCard->wCurrentUser==wMechairID))
	{
		tagOutCardResult OutCardResult;
		m_GameLogic.SearchOutCard(m_bHandCardData[wMechairID],m_bHandCardCount[wMechairID],m_bTurnCardData,m_bTurnCardCount,OutCardResult);

		//���ý���
		if (OutCardResult.cbCardCount<=0)
		{//û�д�����
			m_GameClientView.SetShowBigness(true);
		}
		else
		{//����
			m_GameClientView.SetShowBigness(false);
		}
		if(m_bSystemTrustee==true && m_wLastMostUser!=wMechairID)
		{
			ActiveGameFrame();
			SafeShowOrHideWnd(&m_GameClientView.m_btOutCard, SW_HIDE);
			SafeShowOrHideWnd(&m_GameClientView.m_btPassCard, SW_HIDE);
			SafeShowOrHideWnd(&m_GameClientView.m_btAutoOutCard, SW_HIDE);
			NoHitPassOutCard();
			m_GameClientView.UpdateGameView(NULL);
			return true;
		}
		else
		{
			ActiveGameFrame();
			SafeShowOrHideWnd(&m_GameClientView.m_btOutCard, SW_SHOW);
			SafeShowOrHideWnd(&m_GameClientView.m_btPassCard, SW_SHOW);
			SafeShowOrHideWnd(&m_GameClientView.m_btAutoOutCard, SW_SHOW);
			SafeEnableWnd(&m_GameClientView.m_btPassCard,(m_bTurnCardCount>0)?TRUE:FALSE);
			SafeEnableWnd(&m_GameClientView.m_btOutCard,(VerdictOutCard()==true)?TRUE:FALSE);
			SafeEnableWnd(&m_GameClientView.m_btAutoOutCard,TRUE);
		}
	}

	//���ñ���
	m_GameClientView.SetResetSYPaiNums();

	//��������
	//if (IsLookonMode()==true||(pPassCard->wPassUser!=GetMeChairID()))	
	//{
		const tagUserData *pUserData = GetUserData(pPassCard->wPassUser);//�û���Ϣ
		PlayGameSoundLanguage(pUserData->cbGender, TEXT("BUYAO"),true);
	//}

	//����ʱ��
	if (m_bTurnCardCount!=0)
	{
		SetGameTimer(pPassCard->wCurrentUser,IDI_OUT_CARD,15);
	}
	else SetGameTimer(pPassCard->wCurrentUser,IDI_OUT_CARD,15);

	m_GameClientView.UpdateGameView(NULL);

	__LEAVE_FUNCTION
	return true;
}

//��Ϸ����
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	__ENTER_FUNCTION
	
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	 
	KillTimer(IDI_MOST_CARD);
	KillGameTimer(IDI_OUT_CARD);
	KillGameTimer(IDI_LAND_SCORE);
	KillTimer(IDI_DISPATCH_CARD);
	KillGameTimer(IDI_SNATCH_LAND);

	//������
	m_cbCallScorePhase = CSD_NORMAL;
	//m_GameClientView.m_bDouble=1;

	//ֹͣ����
	DoncludeDispatchCard();

	//���ؿؼ�
	SafeShowOrHideWnd(&m_GameClientView.m_btOutCard, SW_HIDE);
	SafeShowOrHideWnd(&m_GameClientView.m_btPassCard, SW_HIDE);
	SafeShowOrHideWnd(&m_GameClientView.m_btCallLand, SW_HIDE);
	SafeShowOrHideWnd(&m_GameClientView.m_btGiveUpCallLand, SW_HIDE);
	SafeShowOrHideWnd(&m_GameClientView.m_btSnatchLand, SW_HIDE);
	SafeShowOrHideWnd(&m_GameClientView.m_btAutoOutCard, SW_HIDE);
	SafeShowOrHideWnd(&m_GameClientView.m_btGiveUpSnatchLand, SW_HIDE);
	SafeEnableWnd(&m_GameClientView.m_btSortCard, FALSE);

	//SafeShowOrHideWnd(&m_GameClientView.m_btSnatchLand, SW_HIDE);

	SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOff,  SW_HIDE ) ;
	SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOn, SW_HIDE);

	/*SafeShowOrHideWnd(&m_GameClientView.m_btCardDao, SW_HIDE);
	SafeShowOrHideWnd(&m_GameClientView.m_btCardGen, SW_HIDE);
	SafeShowOrHideWnd(&m_GameClientView.m_btCardFan, SW_HIDE);*/
    
	//���ÿؼ�
	SafeEnableWnd(&m_GameClientView.m_btOutCard, FALSE);
	SafeEnableWnd(&m_GameClientView.m_btPassCard, FALSE);

	//��������
	m_GameClientView.SetShowWarming(INVALID_CHAIR, false) ;
	m_GameClientView.SetLandScore(INVALID_CHAIR,0,CSD_NORMAL,CSD_NORMAL);

	InsertShtickString( TEXT( "���ֽ���,�ɼ�ͳ��:" ), RGB( 255, 0, 0 ), true );

	//���û���
	CString strScore;
	WORD IdMe=GetMeChairID();
	WORD landchair=m_wLandUser;
	m_ScoreView.GetMeID(IdMe,landchair);
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_bUserTrustee[i]=false;//��Ϸ����,ȡ���й�.
		const tagUserData * pUserData=GetUserData(i);
		m_ScoreView.SetGameScore(i,pUserData->szName,pGameEnd->lGameScore[i],pGameEnd->GameBeiShu[i],pGameEnd->RoomScore[i]);
		strScore.Format( TEXT( "%s:" ),pUserData->szName);

		//add by Ivan_han
		char sz[200]={0};
		sprintf(sz,"��Ϸ����OnSubGameEnd:chairID=%d,�û�����=%s,��Ϸ����=%I64d,��Ϸ����=%d,�������=%I64d\n",i,pUserData->szName,pGameEnd->lGameScore[i],pGameEnd->GameBeiShu[i],pGameEnd->RoomScore[i]);
        OutputDebugStringA(sz);

		if (i==IdMe)
		{
			InsertShtickString( strScore, RGB( 0, 255, 0 ), false );		
		}
		else
		{
			InsertShtickString(TEXT("���:"),RGB(0,255,0),false );
		}		
		strScore.Format( TEXT( " %I64d" ),pGameEnd->lGameScore[i]);
		InsertShtickString( strScore, RGB( 255, 255, 0 ), true );
		//���ñ�ʯ
		m_ScoreView.SetGem(i,pGameEnd->iLargessCount[i]);
		//��������
		m_ScoreView.SetUserLeft(i,pGameEnd->IsUserLeft[i]);
		//����˰��
		m_ScoreView.SetGameTax(i,pGameEnd->lGameTax[i]);

		sprintf(sz,"������ʯ=%d,�û��Ƿ�ǿ��=%d,��Ϸ˰��=%d\n",pGameEnd->iLargessCount[i],pGameEnd->IsUserLeft[i],pGameEnd->lGameTax[i]);
		OutputDebugStringA(sz);
	}

	//��Ϸ����
	IClientKernel *pIClientKernel = ( IClientKernel * )GetClientKernel( IID_IClientKernel, VER_IClientKernel );
	const tagServerAttribute *pServerAttribute  = pIClientKernel->GetServerAttribute();
	if ( pServerAttribute->wGameGenre == GAME_GENRE_GOLD )
		{
			strScore.Format( TEXT( "����ѣ�%I64d" ), pGameEnd->lGameTax[IdMe]);
		    InsertShtickString( strScore, RGB( 255, 0, 0 ), true );	
		}
	//�Թ���Ҳ���ʾ-���ش���-������Ϸ-���ط���
	/*if(true==IsLookonMode())
	{
	}*/		
	//��ʾ�˿�
	BYTE cbCardPos=0 ;
	for(BYTE i=0; i<GAME_PLAYER; ++i)
	{
		WORD wViewID = SwitchViewChairID(i);
		if(1 == wViewID)
		{
			m_GameClientView.m_HandCardControl[wViewID].SetCardData(pGameEnd->bHandCardData[i], pGameEnd->bCardCount[i]) ;
			m_GameClientView.m_HandCardControl[wViewID].SetDisplayFlag(true) ;
			SafeShowOrHideWnd(&m_GameClientView.m_HandCardControl[wViewID], SW_SHOW);
			//cbCardPos += pGameEnd->bCardCount[i];
		}
		else
		{//�Լ�����
			m_GameClientView.SetHandCardData(pGameEnd->bHandCardData[i], pGameEnd->bCardCount[i], wViewID);
		}	
	}
	//for(BYTE i=0; i<GAME_PLAYER; ++i)
	//{
	//	WORD wViewID = SwitchViewChairID(i);
	//	m_GameClientView.m_HandCardControl[wViewID].SetCardData(pGameEnd->bCardData+cbCardPos, pGameEnd->bCardCount[i]) ;
	//	m_GameClientView.m_HandCardControl[wViewID].SetDisplayFlag(true);
	//	m_GameClientView.m_HandCardControl[wViewID].ShowWindow(SW_SHOW);
	//	cbCardPos += pGameEnd->bCardCount[i];
	//}

	//��ʾ�Լ��˿�
	//m_GameClientView.m_HandCardControl[SwitchViewChairID(GetMeChairID())].SetDisplayFlag(true) ;
	//m_GameClientView.m_HandCardControl[SwitchViewChairID(GetMeChairID())].ShowWindow(SW_SHOW);

	//��������
	WORD wMeChairID=GetMeChairID();
	LONGLONG lMeScore=pGameEnd->lGameScore[wMeChairID];

	if (lMeScore>0L)
	{
		PlaySound("GAME_WIN",false);
	}
	else if(lMeScore<0L)
	{
		PlaySound("GAME_LOST",false);
	}
	else
	{
		PlaySound("GAME_END",false);
	}
	//��ʯ
	for (int i=0;i<GAME_PLAYER;++i)
	{
		m_LargessCount[i]=pGameEnd->iLargessCount[i];
	}
	//����ͷ���
	m_IsSpring=pGameEnd->IsChunTian;
	m_IsFanSpring=pGameEnd->IsFanChun;
	
	//���ý���
	if (IsLookonMode()==false)
	{
		//����,����,��ʯ,����
		m_bchuntian=false;
		m_bfanchun=false;
		m_bbaoshi=false;
		m_bjieshu=false;
		SetGameTimer(wMeChairID,IDI_ENDSTART_GAME,30);

		//�йܹ���
		SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOn, SW_HIDE);
		SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOff, SW_HIDE);
		SafeShowOrHideWnd(&m_GameClientView.m_btSortCard,  SW_HIDE );
		//OnAutoPlay( 0, 0 );
	}

	//��������
	m_GameClientView.SetResetShouPaiNums();
	ZeroMemory(&m_tagOldCard,sizeof(m_tagOldCard));
	m_GameClientView.SetShowBigness(false);

	m_GameClientView.ShowLandTitle(false);
	m_GameClientView.SetMeBeiShu(0);
	m_GameClientView.SetCardCount(INVALID_CHAIR,0);
	m_GameClientView.SetLandUser(INVALID_CHAIR,0);
	for (int i=0;i<GAME_PLAYER;++i)
	{
		m_GameClientView.SetUserTrustee( i, FALSE);
	}
	
	if (IsLookonMode()==true)
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.SetHandCardData(NULL, 0, i);
		}
	}
//#ifdef VIDEO_GAME
//	//��ʾ�˿�
//	m_GameClientView.m_HandCardControl[ 0 ].ShowWindow( SW_HIDE );
//	m_GameClientView.m_HandCardControl[ 2 ].ShowWindow( SW_HIDE );
//#endif

	m_GameClientView.UpdateGameView(NULL);

	__LEAVE_FUNCTION
	return true;
}

//�����ж�
bool CGameClientDlg::VerdictOutCard()
{
	__ENTER_FUNCTION
		
	//״̬�ж�
	if (m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE) return false;

	//��ȡ�˿�
	BYTE bCardData[20];
	//BYTE bShootCount=(BYTE)m_GameClientView.m_HandCardControl[1].GetShootCard(bCardData,CountArray(bCardData));
     BYTE  bShootCount=(BYTE)m_GameClientView.m_GameCard.GetShootCard(bCardData,CountArray(bCardData));
	
	//�����ж�
	if (bShootCount>0L)
	{
		//�����˿�
		m_GameLogic.SortCardList(bCardData,bShootCount,ST_ORDER);

		//��������
		BYTE bCardType=m_GameLogic.GetCardType(bCardData,bShootCount);

		//�����ж�
		if (bCardType==CT_ERROR) return false;

		//�����ж�
		if (m_bTurnCardCount==0) return true;
		return m_GameLogic.CompareCard(m_bTurnCardData,bCardData,m_bTurnCardCount,bShootCount);
	}

	__LEAVE_FUNCTION
	return false;
}

//�Զ�����
bool CGameClientDlg::AutomatismOutCard()
{
	__ENTER_FUNCTION
	
	//�ȳ�����
	if (m_bTurnCardCount==0)
	{
		//���ƽ���
		KillGameTimer(IDI_OUT_CARD);
		SafeShowOrHideWnd(&m_GameClientView.m_btOutCard, SW_HIDE);
		SafeEnableWnd(&m_GameClientView.m_btOutCard,FALSE);
		SafeShowOrHideWnd(&m_GameClientView.m_btPassCard, SW_HIDE);
		SafeEnableWnd(&m_GameClientView.m_btPassCard,FALSE);
		SafeShowOrHideWnd(&m_GameClientView.m_btAutoOutCard, SW_HIDE);
		SafeEnableWnd(&m_GameClientView.m_btAutoOutCard,FALSE);

		WORD wMeChairID=GetMeChairID();

		//��������
		CMD_C_OutCard OutCard;
		OutCard.bCardCount=1;
		OutCard.bCardData[0]=m_bHandCardData[wMeChairID][m_bHandCardCount[wMeChairID]-1];
		SendData(SUB_C_OUT_CART,&OutCard,sizeof(CMD_C_OutCard));

		//Ԥ�ȴ���
		PlaySound("OUT_CARD",false);
		m_GameClientView.m_UserCardControl[1].SetCardData(OutCard.bCardData,OutCard.bCardCount);

		//Ԥ��ɾ��
		BYTE bSourceCount=m_bHandCardCount[wMeChairID];
		m_bHandCardCount[wMeChairID]-=OutCard.bCardCount;
		m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData[wMeChairID],bSourceCount);
		//m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData[wMeChairID],m_bHandCardCount[wMeChairID]);
		m_GameClientView.m_GameCard.SetCardData(m_bHandCardData[wMeChairID],m_bHandCardCount[wMeChairID]);

	}
	else OnPassCard(1,1);

	__LEAVE_FUNCTION
	return true;
}

//ֹͣ����
bool CGameClientDlg::DoncludeDispatchCard()
{
	__ENTER_FUNCTION
	
	//ֹͣ����
	if (m_cbRemnantCardCount>0)
	{
		//ɾ��ʱ��
		KillTimer(IDI_DISPATCH_CARD);

		//���ý���
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			WORD wViewChairID = SwitchViewChairID(i) ;
			if (wViewChairID!=1)
			{
				BYTE cbCardData[20] ;
				ZeroMemory(cbCardData, sizeof(cbCardData)) ;
				m_GameClientView.m_HandCardControl[wViewChairID].SetCardData(cbCardData,17);
			}
			else{
				WORD wMeChairID = GetMeChairID();
				//m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData[wMeChairID],m_bHandCardCount[wMeChairID]); 
				m_GameClientView.m_GameCard.SetCardData(m_bHandCardData[wMeChairID],m_bHandCardCount[wMeChairID]);
			}
		}

		//���ñ���
		m_cbRemnantCardCount=0;
		m_cbDispatchCardCount=0;
		ZeroMemory(m_cbDispatchCardData,sizeof(m_cbDispatchCardData));
	}

	__LEAVE_FUNCTION
	return true;
}

//�ɷ��˿�
bool CGameClientDlg::DispatchUserCard(BYTE cbCardData[], BYTE cbCardCount)
{
	__ENTER_FUNCTION
		
	//���ñ���
	m_cbDispatchCardCount=0;
	m_cbRemnantCardCount=cbCardCount;
	CopyMemory(m_cbDispatchCardData,cbCardData,sizeof(BYTE)*cbCardCount);

	//���ö�ʱ��
	SetTimer(IDI_DISPATCH_CARD,120,NULL);

	__LEAVE_FUNCTION
	return true;
}

//��ʱ����Ϣ
void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	__ENTER_FUNCTION
		
	if ((nIDEvent==IDI_MOST_CARD)&&(m_wMostUser!=INVALID_CHAIR))
	{
		//��������
		WORD wCurrentUser=m_wMostUser;
		m_wMostUser=INVALID_CHAIR;

		//ɾ����ʱ��
		KillTimer(IDI_MOST_CARD);

		//���ý���
		m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
		for (WORD i=0;i<GAME_PLAYER;i++) m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);

		//�������
		if ((IsLookonMode()==false)&&(wCurrentUser==GetMeChairID()))
		{
			ActiveGameFrame();
			SafeShowOrHideWnd(&m_GameClientView.m_btOutCard, SW_SHOW);
			SafeShowOrHideWnd(&m_GameClientView.m_btPassCard, SW_SHOW);
			SafeEnableWnd(&m_GameClientView.m_btPassCard,FALSE);
			SafeShowOrHideWnd(&m_GameClientView.m_btAutoOutCard, SW_SHOW);
			SafeEnableWnd(&m_GameClientView.m_btAutoOutCard,FALSE);
			SafeEnableWnd(&m_GameClientView.m_btOutCard,(VerdictOutCard()==true)?TRUE:FALSE);
		}

		//����ʱ��
		SetGameTimer(wCurrentUser,IDI_OUT_CARD,15);

		return;
	}

	//�ɷ��˿�
	if (nIDEvent==IDI_DISPATCH_CARD)
	{
		//�ɷ�����
		if (m_cbRemnantCardCount>0)
		{
			//�˿�����
			BYTE cbUserCard[20];
			ZeroMemory(cbUserCard,sizeof(cbUserCard));

			//�ɷ��˿�
			m_cbRemnantCardCount--;
			m_cbDispatchCardCount++;
			m_GameLogic.SortCardList(m_cbDispatchCardData,m_cbDispatchCardCount,ST_ORDER);

			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewID = SwitchViewChairID(i) ;
				if (wViewID!=1)
				{
					m_GameClientView.m_HandCardControl[wViewID].SetCardData(m_bHandCardData[i],m_cbDispatchCardCount);
					//m_GameClientView.m_HandCardControl[wViewID].ShowWindow(SW_HIDE);
				}
				else
				{
					m_GameClientView.m_GameCard.SetCardData(m_cbDispatchCardData,m_cbDispatchCardCount); 
					m_GameClientView.m_GameCard.SetCardShow(true);
				}
			}

			//��������
			PlaySound("DISPATCH_CARD",false);
		}

		//��ֹ�ж�
		if (m_cbRemnantCardCount==0)
		{
			//ɾ����ʱ��
			KillTimer(IDI_DISPATCH_CARD);

			SetTaskWndShow(true);

			//�����ƺ���ʾ�ȴ��з�����
			m_GameClientView.ShowLandTitle(true);

			//�˿���Ŀ
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if (IsLookonMode()==false)
				{
					m_GameClientView.SetCardCount(SwitchViewChairID(i),m_bCardCount[i]);
				}		
			}

			//��ǰ���
			if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
			{
				ActiveGameFrame();
				SafeEnableWnd(&m_GameClientView.m_btCallLand,TRUE);
				SafeShowOrHideWnd(&m_GameClientView.m_btCallLand, SW_SHOW);
				SafeEnableWnd(&m_GameClientView.m_btGiveUpCallLand,TRUE);
				SafeShowOrHideWnd(&m_GameClientView.m_btGiveUpCallLand, SW_SHOW);
				//m_GameClientView.m_btGiveUpScore.LoadRgnImage(AfxGetInstanceHandle(),IDB_GIVE_UP,RGB(255,0,255));
			}
			//���ý���
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewID = SwitchViewChairID(i) ;
				if (wViewID!=1)
				{
					BYTE cbCardData[20] ;
					ZeroMemory(cbCardData, sizeof(cbCardData)) ;
					m_GameClientView.m_HandCardControl[wViewID].SetCardData(m_bHandCardData[i],17);
				}
				else 
				{
					WORD wMeChairID = GetMeChairID();
					m_GameClientView.m_HandCardControl[wViewID].SetCardData(m_bHandCardData[wMeChairID],m_bHandCardCount[wMeChairID]); 
					if(false==IsLookonMode())
					{
						m_GameClientView.m_HandCardControl[wViewID].SetDisplayFlag(true) ;
						m_GameClientView.m_HandCardControl[wViewID].SetPositively(true) ;
					}
				}
			}
			if(IsLookonMode()==false) 
			{
				SafeShowOrHideWnd(&m_GameClientView.m_btSortCard, SW_SHOW);
				SafeEnableWnd(&m_GameClientView.m_btSortCard,TRUE);
				SafeShowOrHideWnd(&m_GameClientView.m_btAutoPlayOn, SW_SHOW);
				SafeEnableWnd(&m_GameClientView.m_btAutoPlayOn,FALSE);
				//m_GameClientView.m_btBrightCard.ShowWindow( SW_HIDE );
			}
		   //��������
			PlaySound("GAME_START",false);

			//����ʱ��
			if (m_wCurrentUser!=INVALID_CHAIR) SetGameTimer(m_wCurrentUser,IDI_LAND_SCORE,10);
		}
		return;
	}
	__super::OnTimer(nIDEvent);
	__LEAVE_FUNCTION
}

//��ʼ��ť
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	__ENTER_FUNCTION
			
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
        m_GameClientView.SetHandCardData(NULL, 0, i);
	}	
	//���ñ���
	m_bAutoPlay=false;
	m_wBombTime=1;
	m_wTimeOutCount=0;
	m_bSystemTrustee=false;
	m_bTurnCardCount=0;
	m_bTurnOutType=CT_ERROR;
	m_wMostUser=INVALID_CHAIR;
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	memset(m_bHandCardCount,0,sizeof(m_bHandCardCount));

	//���ý���
	KillGameTimer(IDI_START_GAME);
	KillGameTimer(IDI_ENDSTART_GAME);

	m_GameClientView.ShowLandTitle(false);
	m_GameClientView.SetBombTime(0);
	m_GameClientView.SetCardCount(INVALID_CHAIR,0);
	m_GameClientView.SetLandUser(INVALID_CHAIR,0);
	m_GameClientView.SetLandScore(INVALID_CHAIR,0,CSD_NORMAL,CSD_NORMAL);
	m_GameClientView.SetPassFlag(INVALID_CHAIR,false);
	m_GameClientView.ShowLandTime(true);
	m_GameClientView.SetMeBeiShu(0);

	//���ؿؼ�
	SafeShowOrHideWnd(&m_GameClientView.m_btStart, SW_HIDE);

	//if(m_GameClientView.m_btBrightStart.GetSafeHwnd())	// add by HouGuoJiang 2011-11-25
	//	m_GameClientView.m_btBrightStart.ShowWindow(SW_HIDE);
	/*m_ScoreView.ShowWindow(SW_HIDE);*/

	//�����˿�
//	m_GameClientView.m_HandCardControl[ 0 ].ShowWindow( SW_HIDE );
//	m_GameClientView.m_HandCardControl[ 2 ].ShowWindow( SW_HIDE );
	m_GameClientView.SetCardBackDisPlay(false);

	//�����˿�
	m_GameClientView.SetCardBackData(NULL,0);
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_HandCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
	}
    //����Լ���������
	m_GameClientView.m_GameCard.SetCardData(NULL,0);

	m_IsFanSpring=false;
	m_IsSpring=false;
	memset(m_LargessCount,0,sizeof(m_LargessCount));
	m_bchuntian=false;
	m_bfanchun=false;
	m_bbaoshi=false;
	m_bjieshu=false;

	//������Ϣ
	SendUserReady(NULL,0);


	int iCount = GetCurGameUserNums();
	PlayWaitFlash(false);
	if (GAME_PLAYER != iCount)
	{
		PlayWaitFlash(true);
	}

	SetTaskWndShow(false);

	__LEAVE_FUNCTION
	return 0;
}

//������Ϣ
LRESULT CGameClientDlg::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	__ENTER_FUNCTION
	
	//״̬�ж�
	if ((m_GameClientView.m_btOutCard.IsWindowEnabled()==FALSE)||
		(m_GameClientView.m_btOutCard.IsWindowVisible()==FALSE)) return 0;

	//���ý���
	KillGameTimer(IDI_OUT_CARD);
	SafeShowOrHideWnd(&m_GameClientView.m_btOutCard, SW_HIDE);
	SafeEnableWnd(&m_GameClientView.m_btOutCard,FALSE);
	SafeShowOrHideWnd(&m_GameClientView.m_btPassCard, SW_HIDE);
	SafeEnableWnd(&m_GameClientView.m_btPassCard,FALSE);
	SafeShowOrHideWnd(&m_GameClientView.m_btAutoOutCard, SW_HIDE);
	SafeEnableWnd(&m_GameClientView.m_btAutoOutCard,FALSE);

	if (1 == (BYTE)wParam)
	{
		//���ñ���
		m_wTimeOutCount=0;
	}

	//��������
	CMD_C_OutCard OutCard;
	//OutCard.bCardCount=(BYTE)m_GameClientView.m_HandCardControl[1].GetShootCard(OutCard.bCardData,CountArray(OutCard.bCardData));
	OutCard.bCardCount=(BYTE)m_GameClientView.m_GameCard.GetShootCard(OutCard.bCardData,CountArray(OutCard.bCardData));
	SendData(SUB_C_OUT_CART,&OutCard,sizeof(CMD_C_OutCard));

	//Ԥ����ʾ
	//BYTE cbOutCardType = m_GameLogic.GetCardType(OutCard.bCardData, OutCard.bCardCount );
	//if ( cbOutCardType == CT_BOMB_CARD )
	//	PlayGameSound(AfxGetInstanceHandle(),TEXT("BOMB_CARD"));
	//else if ( cbOutCardType == CT_MISSILE_CARD )
	//	PlayGameSound(AfxGetInstanceHandle(),TEXT("MOST_CARD"));
	//else
	//	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	m_GameClientView.m_UserCardControl[1].SetCardData(OutCard.bCardData,OutCard.bCardCount);

	//Ԥ��ɾ��
	WORD wMeChairID=GetMeChairID();
	BYTE bSourceCount=m_bHandCardCount[wMeChairID];
	m_bHandCardCount[wMeChairID]-=OutCard.bCardCount;
	m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData[wMeChairID],bSourceCount);
	//m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData[wMeChairID],m_bHandCardCount[wMeChairID]);
	m_GameClientView.m_GameCard.SetCardData(m_bHandCardData[wMeChairID],m_bHandCardCount[wMeChairID]);
   
	m_GameClientView.UpdateGameView(NULL);

	__LEAVE_FUNCTION
	return 0;
}

//�����PASS��ť,ֱ�ӳ��ơ�
void CGameClientDlg::NoHitPassOutCard()
{
	__ENTER_FUNCTION

	//���ý���
	KillGameTimer(IDI_OUT_CARD);
	SafeShowOrHideWnd(&m_GameClientView.m_btOutCard, SW_HIDE);
	SafeEnableWnd(&m_GameClientView.m_btOutCard,FALSE);
	SafeShowOrHideWnd(&m_GameClientView.m_btPassCard, SW_HIDE);
	SafeEnableWnd(&m_GameClientView.m_btPassCard,FALSE);
	SafeShowOrHideWnd(&m_GameClientView.m_btAutoOutCard, SW_HIDE);
	SafeEnableWnd(&m_GameClientView.m_btAutoOutCard,FALSE);

	//��������
	SendData(SUB_C_PASS_CARD);

	//Ԥ����ʾ
	m_GameClientView.SetPassFlag(1,true);
	m_GameClientView.SetShowBigness(false);
	m_GameClientView.UpdateGameView(NULL);
	__LEAVE_FUNCTION
}

//��������
LRESULT CGameClientDlg::OnPassCard(WPARAM wParam, LPARAM lParam)
{
	__ENTER_FUNCTION

	//״̬�ж�
	if (m_GameClientView.m_btPassCard.IsWindowEnabled()==FALSE) return 0;
    WORD wMeChairID = GetMeChairID();

	if (1 == (BYTE)wParam)
	{
		//���ñ���
		m_wTimeOutCount=0;
	}

	//���ý���
	KillGameTimer(IDI_OUT_CARD);
	SafeShowOrHideWnd(&m_GameClientView.m_btOutCard, SW_HIDE);
	SafeEnableWnd(&m_GameClientView.m_btOutCard,FALSE);
	SafeShowOrHideWnd(&m_GameClientView.m_btPassCard, SW_HIDE);
	SafeEnableWnd(&m_GameClientView.m_btPassCard,FALSE);
	SafeShowOrHideWnd(&m_GameClientView.m_btAutoOutCard, SW_HIDE);
	SafeEnableWnd(&m_GameClientView.m_btAutoOutCard,FALSE);

	//��������
	SendData(SUB_C_PASS_CARD);

	//Ԥ����ʾ
	m_GameClientView.SetPassFlag(1,true);
	//PlayGameSound(AfxGetInstanceHandle(),TEXT("PASS_CARD"));
	
	//m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData[wMeChairID],m_bHandCardCount[wMeChairID]);
    m_GameClientView.m_GameCard.SetCardData(m_bHandCardData[wMeChairID],m_bHandCardCount[wMeChairID]);
	m_GameClientView.SetShowBigness(false);
	m_GameClientView.UpdateGameView(NULL);

	__LEAVE_FUNCTION
	return 0;
}

//�е�����Ϣ
LRESULT CGameClientDlg::OnCallLand(WPARAM wParam, LPARAM lParam)
{
	__ENTER_FUNCTION

	//���ö�ʱ��
	KillGameTimer(IDI_LAND_SCORE);

	//���ý���	
	SafeShowOrHideWnd(&m_GameClientView.m_btCallLand, SW_HIDE);
	SafeShowOrHideWnd(&m_GameClientView.m_btGiveUpCallLand, SW_HIDE);

	//��������
	CMD_C_LandScore LandScore;
	LandScore.bLandScore=(BYTE)wParam;
	SendData(SUB_C_LAND_SCORE,&LandScore,sizeof(LandScore));

	__LEAVE_FUNCTION
	return 0;
}
//��������Ϣ
LRESULT CGameClientDlg::OnSnatchLand(WPARAM wParam,LPARAM lParam)
{
	//���ö�ʱ��
	KillGameTimer(IDI_SNATCH_LAND);

	//���ý���
	SafeShowOrHideWnd(&m_GameClientView.m_btSnatchLand,SW_HIDE);
	SafeShowOrHideWnd(&m_GameClientView.m_btGiveUpSnatchLand,SW_HIDE);

	//��������
	CMD_C_SnatchLand SnatchLand;
	SnatchLand.bSnatchLand=(BYTE)wParam;
	SendData(SUB_C_SNATCH_SCORE,&SnatchLand,sizeof(SnatchLand));

	return 0;
}

//������ʾ
//LRESULT CGameClientDlg::OnAutoOutCard(WPARAM wParam, LPARAM lParam)
//{
//	//�������
//	tagOutCardResult OutCardResult;
//	WORD wMeChairID = GetMeChairID();
//	m_GameLogic.SearchOutCard(m_bHandCardData[wMeChairID],m_bHandCardCount[wMeChairID],m_bTurnCardData,m_bTurnCardCount,OutCardResult);
	//���ý���
//	if (OutCardResult.cbCardCount>0)
//	{
//		//���ý���
//		m_GameClientView.m_HandCardControl[1].SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);
//
//		//���ÿؼ�
//		bool bOutCard=VerdictOutCard();
//		if(false == bOutCard) OnPassCard(0,0);
//		m_GameClientView.m_btOutCard.EnableWindow((bOutCard==true)?TRUE:FALSE);
//	}
//	else 
//	{
//		//��������
//		OnPassCard(0,0);
//	}
//
//	return 0;
//}
//������ʾ
LRESULT CGameClientDlg::OnAutoOutCard(WPARAM wParam, LPARAM lParam)
{
	__ENTER_FUNCTION
	
	bool bIsSearchOutCard = false;
	tagOutCardResult OutCardResult;
	WORD wMeChairID = GetMeChairID();
	WORD wViewChairID = SwitchViewChairID(wMeChairID);
	m_GameLogic.SearchOutCard(m_bHandCardData[wMeChairID],m_bHandCardCount[wMeChairID],m_tagOldCard.cbResultCard,m_tagOldCard.cbCardCount,OutCardResult);


	if (OutCardResult.cbCardCount>0)
	{
		bIsSearchOutCard = true;
	}
	else if (m_tagNewCard.cbCardCount > 0)
	{
		m_tagOldCard.cbCardCount = m_bTurnCardCount;
		CopyMemory(m_tagOldCard.cbResultCard, m_bTurnCardData, m_bTurnCardCount);
		::memset(&m_tagNewCard, 0, sizeof(m_tagNewCard));

		m_GameLogic.SearchOutCard(m_bHandCardData[wMeChairID],m_bHandCardCount[wMeChairID],m_tagOldCard.cbResultCard,m_tagOldCard.cbCardCount,OutCardResult);
		bIsSearchOutCard = true;
	}
	else
	{
		OnPassCard(1,1);
	}

	if (bIsSearchOutCard)
	{
		m_tagNewCard=OutCardResult;
		m_tagOldCard=m_tagNewCard;
		//���ý���
		m_GameClientView.m_GameCard.SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);

		//���ÿؼ�
		bool bOutCard=VerdictOutCard();
		if(false == bOutCard)
		{
            OnPassCard(1,1);
		}
		SafeEnableWnd(&m_GameClientView.m_btOutCard,(bOutCard==true)?TRUE:FALSE);
	}

	__LEAVE_FUNCTION
	return 0;
}


//����˿�
LRESULT CGameClientDlg::OnLeftHitCard(WPARAM wParam, LPARAM lParam)
{
	__ENTER_FUNCTION

	//���ÿؼ�
	bool bOutCard=VerdictOutCard();
	SafeEnableWnd(&m_GameClientView.m_btOutCard,bOutCard?TRUE:FALSE);

	__LEAVE_FUNCTION
	return 0;
}

//�Ҽ��˿�
LRESULT CGameClientDlg::OnRightHitCard(WPARAM wParam, LPARAM lParam)
{		
	__ENTER_FUNCTION

	//�û�����
	OnOutCard(1,1);

	__LEAVE_FUNCTION
	return 0;
}

//////////////////////////////////////////////////////////////////////////

LRESULT CGameClientDlg::OnAutoPlay(WPARAM wParam, LPARAM lParam)
{
	__ENTER_FUNCTION

	if (IsLookonMode())
	{
		return 0;
	}
    bool bAutoPlay = (wParam == 1);
    if (bAutoPlay == m_bAutoPlay)
        return 0;

    m_bAutoPlay = bAutoPlay;
	//m_bSystemTrustee=bAutoPlay;

    if (m_bAutoPlay)
	{
		InsertSystemString("ʹ���йܹ���, ϵͳ�����й�״̬!");
	}	
    else
	{
		m_bSystemTrustee=false;
		InsertSystemString("ȡ���йܹ���. ");
	}
	//������Ϣ
	CMD_C_UserTrustee UserTrustee;
	ZeroMemory( &UserTrustee, sizeof( UserTrustee ) );
	UserTrustee.bTrustee = m_bAutoPlay;
	UserTrustee.wUserChairID = GetMeChairID();

	SendData( SUB_C_TRUSTEE, &UserTrustee, sizeof( UserTrustee ) );

	__LEAVE_FUNCTION
    return 0;
}


LRESULT CGameClientDlg::OnMessageSortCard(WPARAM wParam, LPARAM lParam)
{
	__ENTER_FUNCTION

	//�����˿�
	WORD wMeChairID=GetMeChairID();
	m_GameLogic.SortCardList(m_bHandCardData[wMeChairID],m_bHandCardCount[wMeChairID],m_cbSortType);

	//��ȡ�˿�
	//BYTE cbShootCount=(BYTE)m_GameClientView.m_HandCardControl[1].SetCardData(m_bHandCardData[wMeChairID],m_bHandCardCount[wMeChairID]);
	BYTE cbShootCount=(BYTE)m_GameClientView.m_GameCard.SetCardData(m_bHandCardData[wMeChairID],m_bHandCardCount[wMeChairID]);


	//���ñ���
	m_cbSortType=(m_cbSortType==ST_ORDER)?ST_COUNT:ST_ORDER;

	//���ð�ť
	UINT uImageID=(m_cbSortType==ST_ORDER)?IDB_ORDER_SORT:IDB_COUNT_SORT;

	//m_GameClientView.m_btSortCard.LoadRgnImage(AfxGetInstanceHandle(),uImageID,RGB(255,0,255));

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	__LEAVE_FUNCTION
	return 0;
}

//���ƿ�ʼ
LRESULT CGameClientDlg::OnBrightStart(WPARAM wParam, LPARAM lParam)
{
	//��������
	m_GameClientView.ShowLandTime(true);

	//������Ϣ
	SendData( SUB_C_BRIGHT_START);

	//��ʼ��Ϣ
	SendMessage(IDM_START, 0, 0 );

	//��������
	//PlayGameSound(AfxGetInstanceHandle(),TEXT("BRIGHT_CARD"));

	return 0;
}

bool CGameClientDlg::OnSubBrightStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_BrightStart));
	if (wDataSize != sizeof(CMD_S_BrightStart)) return false;

	//��������
	CMD_S_BrightStart *pBrightStart = ( CMD_S_BrightStart * )pBuffer;

	//���ý���
	WORD wViewID = SwitchViewChairID(pBrightStart->wBrightUser) ;
	m_GameClientView.SetBrightStart(wViewID, pBrightStart->bBright);
	if ( pBrightStart->bBright ) m_GameClientView.SetBrightTime(BRIGHT_START_TIME);

	return true;
}

//�������
bool CGameClientDlg::OnSubBrightCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_BrightCard));
	if (wDataSize != sizeof(CMD_S_BrightCard)) return false;

	//��������
	CMD_S_BrightCard *pBrightCard = ( CMD_S_BrightCard * )pBuffer;

	if ( pBrightCard->cbCallScorePhase == CSD_BRIGHTCARD )
	{
		//����ת��
		WORD wViewChairID = SwitchViewChairID(pBrightCard->wBrightUser);

		//�������
		m_GameClientView.m_bScore[wViewChairID] = 0 ;
		m_GameClientView.m_bPass[wViewChairID] = false;
		m_GameClientView.m_bDoubleScore[wViewChairID] = false;

		if ( pBrightCard->cbBrightTime != 255 )
		{
			//���ý���
			m_GameClientView.SetBrightTime(pBrightCard->cbCurrenBrightTime);
			m_GameClientView.SetBrightStart(wViewChairID, true);

			//�����˿�
			m_GameClientView.m_HandCardControl[wViewChairID].ShowWindow(SW_SHOW);
			m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(true);
		}
		else
		{
			m_GameClientView.m_cbUserPhase[wViewChairID] = CSD_BRIGHTCARD;	
			m_GameClientView.m_bScore[wViewChairID] = 255 ;
		}

		m_GameClientView.UpdateGameView(NULL);

		return true;
	}
	else
	{
		//���ý���
		m_GameClientView.SetBrightTime(pBrightCard->cbCurrenBrightTime);
		WORD wViewChairID = SwitchViewChairID(pBrightCard->wBrightUser);
		m_GameClientView.SetBrightStart(wViewChairID, true);

		//�����˿�
		m_GameClientView.m_HandCardControl[wViewChairID].ShowWindow(SW_SHOW);
		m_GameClientView.m_HandCardControl[wViewChairID].SetDisplayFlag(true);

		return true;
	}
	return true;
}

//����й�
bool CGameClientDlg::OnSubPlayerTrustee(const void * pBuffer, WORD wDataSize)
{
	__ENTER_FUNCTION
	
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_C_UserTrustee));
	if (wDataSize != sizeof(CMD_C_UserTrustee)) return false;

	CMD_C_UserTrustee *pUserTrustee = ( CMD_C_UserTrustee * )pBuffer;

	m_GameClientView.SetUserTrustee( SwitchViewChairID( pUserTrustee->wUserChairID ), pUserTrustee->bTrustee );

	//��ʾ�й�
	if ( pUserTrustee->wUserChairID != GetMeChairID() || IsLookonMode() ) {

		CString strMsg;
		if ( pUserTrustee->bTrustee )
			strMsg.Format(TEXT("���ѡ�����й�"));
		else
			strMsg.Format(TEXT("���ȡ�����й�"));
		InsertSystemString( strMsg );
	}
	m_GameClientView.UpdateGameView(NULL);

	__LEAVE_FUNCTION
	return true;
}

//�ӱ���Ϣ (�з����)
bool CGameClientDlg::OnSubDoubleScore(const void * pBuffer, WORD wDataSize)
{
	__ENTER_FUNCTION
	
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_DoubleScore));
	if (wDataSize != sizeof(CMD_S_DoubleScore)) return false;

	//��������
	CMD_S_DoubleScore *pDoubleScore = ( CMD_S_DoubleScore * )pBuffer;

	//�رհ�ť
	//SafeShowOrHideWnd(&m_GameClientView.m_btGiveUpScore, SW_HIDE);
	//SafeShowOrHideWnd(&m_GameClientView.m_btOneScore, SW_HIDE);
	//SafeShowOrHideWnd(&m_GameClientView.m_btTwoScore, SW_HIDE);
	//SafeShowOrHideWnd(&m_GameClientView.m_btThreeScore, SW_HIDE);

	//cxf ɾ���ӱ���ť  ��(��-��-��)����
	//���ð�ť
	//bool bAllowDouble = pDoubleScore->bAllowDouble;
	//m_GameClientView.m_btDoubleScore.ShowWindow( SW_SHOW );
	//m_GameClientView.m_btDoubleScore.EnableWindow( bAllowDouble ? TRUE : FALSE );
	//m_GameClientView.m_btNotDoubleScore.ShowWindow( SW_SHOW );
	//if ((pDoubleScore->wLandUser+1)%GAME_PLAYER == GetMeChairID())
	//{
	//	SafeShowOrHideWnd(&m_GameClientView.m_btCardDao, SW_SHOW);
	//}
	//cxf end
	//���ÿؼ�
	m_GameClientView.SetCardBackData(pDoubleScore->bBackCard,CountArray(pDoubleScore->bBackCard));
	m_GameClientView.SetCardBackDisPlay(true);
	m_GameClientView.SetBrightStart(INVALID_CHAIR, false);

	//���ý���
	m_bCardCount[pDoubleScore->wLandUser]=20;
	//m_GameClientView.SetLandScore(INVALID_CHAIR,0,CSD_GAMESTART,CSD_GAMESTART);
	if (IsLookonMode()==false)
	{
		m_GameClientView.SetCardCount(SwitchViewChairID(pDoubleScore->wLandUser),m_bCardCount[pDoubleScore->wLandUser]);
	}
	WORD wViewChairID=SwitchViewChairID(pDoubleScore->wLandUser);
	//m_GameClientView.SetLandUser(wViewChairID, pDoubleScore->bCurrentScore);

	//��������
	WORD wLandUser=pDoubleScore->wLandUser;
	BYTE bCardCound=m_bHandCardCount[wLandUser];
	m_bHandCardCount[wLandUser]+=CountArray(pDoubleScore->bBackCard);
	CopyMemory(&m_bHandCardData[wLandUser][bCardCound],pDoubleScore->bBackCard,sizeof(pDoubleScore->bBackCard));
	m_GameLogic.SortCardList(m_bHandCardData[wLandUser],m_bHandCardCount[wLandUser],ST_ORDER);
	//m_GameClientView.m_HandCardControl[SwitchViewChairID(wLandUser)].SetCardData(m_bHandCardData[wLandUser],m_bHandCardCount[wLandUser]);
	if(pDoubleScore->wLandUser == GetMeChairID())
	{
		m_GameClientView.m_GameCard.SetCardData(m_bHandCardData[wLandUser],m_bHandCardCount[wLandUser]);
	}

	//������Ϣ
	//SetGameTimer(GetMeChairID(), IDI_DOUBLE_SCORE, 30);
	m_GameClientView.UpdateGameView(NULL);

	__LEAVE_FUNCTION
	return true;
}

//�ӱ���Ϣ 
LRESULT CGameClientDlg::OnDoubleScore(WPARAM wParam, LPARAM lParam)
{
	__ENTER_FUNCTION

	//cxf ɾ���ӱ���ť  ��(��-��-��)����
	//�ӱ��ж�
	//bool bDoubleScore = (wParam == 0) ? true : false;
	bool bDoubleScore = (wParam > 0) ? true : false;

	//���ý���
	//m_GameClientView.m_btDoubleScore.ShowWindow( SW_HIDE );
	//m_GameClientView.m_btNotDoubleScore.ShowWindow( SW_HIDE );
	/*SafeShowOrHideWnd(&m_GameClientView.m_btCardDao, SW_HIDE);
	SafeShowOrHideWnd(&m_GameClientView.m_btCardGen, SW_HIDE);
	SafeShowOrHideWnd(&m_GameClientView.m_btCardFan, SW_HIDE);*/

	if(IsLookonMode()) return 0;
    //cxf end

	//������Ϣ
	CMD_C_DoubleScore DoubleScore;
	ZeroMemory( &DoubleScore, sizeof( DoubleScore ) );
	DoubleScore.bDoubleScore = bDoubleScore;
	DoubleScore.bDoubleType=lParam;

	//������Ϣ
	SendData( SUB_C_DOUBLE_SCORE, &DoubleScore, sizeof( DoubleScore ) );

	//�رն�ʱ��
	//KillGameTimer(IDI_DOUBLE_SCORE);

	__LEAVE_FUNCTION
	return 0;
}

//��Ҽӱ�
bool CGameClientDlg::OnSubUserDouble(const void * pBuffer, WORD wDataSize)
{
	__ENTER_FUNCTION

	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_UserDouble));
	if (wDataSize != sizeof(CMD_S_UserDouble)) return false;

	//��������
	CMD_S_UserDouble *pUserDouble = ( CMD_S_UserDouble * )pBuffer;

	//SafeShowOrHideWnd(&m_GameClientView.m_btCardDao, SW_HIDE);
	//SafeShowOrHideWnd(&m_GameClientView.m_btCardGen, SW_HIDE);
	//SafeShowOrHideWnd(&m_GameClientView.m_btCardFan, SW_HIDE);

	//cxf  ��ʾ����()
	//if (IsLookonMode() == false && pUserDouble->bNextUser == GetMeChairID())
	//{
	//	switch (pUserDouble->bState)
	//	{
	//	case 1 : //����
	//		{
	//			SafeShowOrHideWnd(&m_GameClientView.m_btCardDao, SW_SHOW);
	//			break;
	//		}
	//	case 2 : //����
	//		{
	//			SafeShowOrHideWnd(&m_GameClientView.m_btCardGen, SW_SHOW);
	//			break;
	//		}
	//	case 3 : //��������
	//		{
	//			SafeShowOrHideWnd(&m_GameClientView.m_btCardFan, SW_SHOW);
	//			break;
	//		}
	//	default :
	//		break;
	//	}
	//}

	const tagUserData *pUserData = GetUserData(pUserDouble->wDoubleUser);//�û���Ϣ
	WORD wMeChairID=GetMeChairID();
	CString strPath ="";
	GetPicPath(strPath);
	//if (1==pUserDouble->bDoubleType)
	//{//��
	//	PlayGameSoundLanguage(pUserData->cbGender, TEXT("DAO"),false);
	//	PlaySound("BEI",false);
	////���Ŷ���
	//	strPath.Append("IMAGE_DAO.png");
	//	m_GameClientView.m_DoubleAction.SetActionImage(strPath,true,15);
	//	m_GameClientView.PlayDoubelFlash();
	//	
	//} 
	//else if(2==pUserDouble->bDoubleType)
	//{//��
	//	PlayGameSoundLanguage(pUserData->cbGender, TEXT("GEN"),false);
	//	PlaySound("BEI",false);
	//  //���Ŷ���
	//	strPath.Append("IMAGE_GEN.png");
	//	m_GameClientView.m_DoubleAction.SetActionImage(strPath,true,15);
	//	m_GameClientView.PlayDoubelFlash();
	//	
	//}
	//else if(3==pUserDouble->bDoubleType)
	//{//��
	//	PlayGameSoundLanguage(pUserData->cbGender, TEXT("FAN"),false);
	//	PlaySound("BEI",false);
	//	//���Ŷ���
	//	strPath.Append("IMAGE_FAN.png");
	//	m_GameClientView.m_DoubleAction.SetActionImage(strPath,true,15);
	//	m_GameClientView.PlayDoubelFlash();
	//}

	//���ý���
	WORD wViewChairID=SwitchViewChairID(m_wLandUser);
	//m_GameClientView.SetLandUser(wViewChairID, pUserDouble->bCurrentScore);
	m_GameClientView.SetDoubleScoreUser( SwitchViewChairID(pUserDouble->wDoubleUser), pUserDouble->bDoubleScore );
	m_GameClientView.SetBrightStart(wViewChairID, false);

	//����ʵʱ����
    /*m_GameClientView.SetMeBeiShu(pUserDouble->bBeiShu[GetMeChairID()]);*/
	
	m_GameClientView.UpdateGameView(NULL);

	__LEAVE_FUNCTION
	return true;
}

//���������Ϸ
void CGameClientDlg::OnEventContinueGame()
{
	OnStart(0, 0);
}

//������Ϸ����
void CGameClientDlg::PlayGameSoundLanguage(BYTE cbGender, CString strSound,bool bSoundType)
{
	__ENTER_FUNCTION

	CString strGameSound="";
	//�Ա𣨲�����Ů������
	if (1==cbGender)
	{//Ů
		strGameSound=m_sVernacular+strSound+TEXT("_W");
	}
	else
	{//��
		strGameSound=m_sVernacular+strSound+TEXT("_M");
	}
	if(bSoundType)
	{//��������
		TCHAR cBuffer[10]={""};
		int iRand=rand()%4+1;
		::_snprintf(cBuffer,sizeof(cBuffer),"%d",iRand);
		strGameSound=strGameSound+"_"+cBuffer;
	}
	PlaySound(strGameSound,false);
	__LEAVE_FUNCTION
}

//������Ϸ���� 9-22
void CGameClientDlg::PlaySound(CString szSoundRes, bool bLoop)
{
    __ENTER_FUNCTION

	if (!szSoundRes.IsEmpty())
	{
		TCHAR szResourcePath[MAX_PATH]={0};
		GetCurrentDirectory(MAX_PATH,szResourcePath);
		CString strPath(szResourcePath);
		strPath.Append("\\LandCrazy\\sound\\");
		strPath.Append(szSoundRes);
		strPath.Append(".mp3");

		PlayDirSound(strPath, true, bLoop);	//������Ϸ��Ч
	}
	__LEAVE_FUNCTION
}

//���ű�������
void CGameClientDlg::PlayMusic(CString szSoundRes)
{
	__ENTER_FUNCTION

	if (!szSoundRes.IsEmpty())
	{
		TCHAR szResourcePath[MAX_PATH];
		GetCurrentDirectory(MAX_PATH,szResourcePath);
		CString strPath(szResourcePath);
		strPath.Append("\\LandCrazy\\sound\\");
		strPath.Append(szSoundRes);

		PlayBGMusic(strPath, 0, true);
	}
	__LEAVE_FUNCTION
}

//��ȡ��ǰ·��
void CGameClientDlg::GetPicPath(CString& strPath)
{
	__ENTER_FUNCTION

	strPath = "";
	TCHAR szResourcePath[MAX_PATH]={0};
	GetCurrentDirectory(MAX_PATH,szResourcePath);
	strPath.Append(szResourcePath);
	strPath.Append("\\LandCrazy\\picture\\");
    __LEAVE_FUNCTION
}

//ͳ������
int CGameClientDlg::GetCurGameUserNums()
{
	int iCount = 0;
	__ENTER_FUNCTION

	for (int i=0; i<GAME_PLAYER; ++i)
	{
		const tagUserData *pUserData = GetUserData(i);
		if (NULL != pUserData)
		{
			++iCount;
		}
	}

	__LEAVE_FUNCTION
	return iCount;
}

//�û�����
void __cdecl CGameClientDlg::OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	__ENTER_FUNCTION

	CGameFrameDlg::OnEventUserEnter(pUserData, wChairID, bLookonUser);
	//int iCount = GetCurGameUserNums();
	//if (GAME_PLAYER == iCount)
	//{
	//	PlayWaitFlash(false);
	//}
	__LEAVE_FUNCTION
}
//�û��뿪
void __cdecl CGameClientDlg::OnEventUserLeave(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	__ENTER_FUNCTION

	CGameFrameDlg::OnEventUserLeave(pUserData, wChairID, bLookonUser);

	int iCount = GetCurGameUserNums();
	PlayWaitFlash(false);
	if (GAME_PLAYER != iCount)
	{
		PlayWaitFlash(true);
	}
	__LEAVE_FUNCTION
}