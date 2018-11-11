#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"
#include "GameClientDlg.h"
#include ".\gameclientview.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////////
//��ť��ʶ 

#define IDC_START						100								//��ʼ��ť
#define IDC_HAI_DI						101								//���װ�ť
#define IDC_NO_HAI_DI					102								//������ť
#define IDC_TRUSTEE_CONTROL				104								//�йܿ���
#define IDC_AUTOPLAY_ON                 111                             //�йܰ�ť
#define IDC_AUTOPLAY_OFF                112                             //ȡ���й�
 
//������ʶ  
#define DICE_EFFECT_COUNT               9                               //������Ŀ
#define START_EFFECT_COUNT              4                               //��ʼ��Ŀ
#define BIRD_EFFECT_COUNT               12                              //������Ŀ

#define IDI_DICE_EFFECT					106							    //���ӱ�ʶ
#define IDI_BIRD_EFFECT				    108							    //�����ʶ
#define IDI_START_EFFECT                109                             //��ʼ��ʶ



//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_HAI_DI, OnHaiDi)
	ON_BN_CLICKED(IDC_NO_HAI_DI, OnNoHaiDi)
	ON_BN_CLICKED(IDC_AUTOPLAY_ON, OnAutoPlayerOn)
	ON_BN_CLICKED(IDC_AUTOPLAY_OFF,OnAutoPlayerOff)
	ON_BN_CLICKED(IDC_TRUSTEE_CONTROL,OnStusteeControl)
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientView::CGameClientView()
{
	//��־����
	m_cbHaiDiCard=0;
	m_bOutCard=false;
	m_bWaitOther=false;


	m_nDiceCount1=0;
	m_nDiceCount2=0;

	//��Ϸ����
	m_lCellScore=0L;
	m_wBankerUser=INVALID_CHAIR;

	//�û�״̬
	m_tDice=0;
	m_tStart=0;
	m_cbCardData=0;
	m_cbBirdCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));


	m_iSaveCheckPaiIndex = 0;
	m_pGameClientDlg  = NULL;

	//����
	m_bDiceEffect=false;
	m_bBirdEffect=false;
	m_cbFrameIndex=0;

	//����λͼ
	HINSTANCE hInstance=AfxGetInstanceHandle();

	CString strPath="";
	GetPicPath(strPath);
    m_ImageBack.LoadImage(TEXT(strPath+"VIEW_BACK.png"));
	m_ImageWait.LoadImage(TEXT(strPath+"WAIT_TIP.png"));
	m_ImageUserFlag.LoadImage(TEXT(strPath+"USER_FLAG.png"));
	m_ImageDirectionBack.LoadImage(TEXT(strPath+"DIRECTIONBACK.png"));
	m_ImageUserBankerFlag.LoadImage(TEXT(strPath+"BANKER.png"));
	m_ImageHuangZhuang.LoadImage(TEXT(strPath+"HUANG_ZHUANG.png"));
	m_ImageTrustee.LoadImage(TEXT(strPath+"TRUSTEE.png"));
	m_ImageUserGradeNum.LoadImage(TEXT(strPath+"GRADENUM.png"));
	m_ImageOutCardBack.LoadImage(TEXT(strPath+"OUTCARDBACK.png"));
	

	//�������������Ϣ��
	for (int i=0; i<4; ++i)
	{
		m_ImageUserInfo[i].LoadImage(TEXT(strPath+"USER_INFO.png"));
	}
	
	////////////////////////////////////////////
	////////////////////////////////////////////
	//����-����
	m_ActionDice1.LoadImage(TEXT(strPath+"Dice.png"));
	m_csActionDice1.cx=m_ActionDice1.GetWidth()/9;
	m_csActionDice1.cy=m_ActionDice1.GetHeight();

    m_ActionDice2.LoadImage(TEXT(strPath+"Dice2.png"));
	m_csActionDice2.cx=m_ActionDice2.GetWidth()/6;
	m_csActionDice2.cy=m_ActionDice2.GetHeight();
    ///////////////////////////////////////////
	//���񶯻�
	m_ActionBird[0].LoadImage(TEXT(strPath+"Cartoon\\ActBirdTop.png"));
	m_ActionBird[1].LoadImage(TEXT(strPath+"Cartoon\\ActBirdRight.png"));
	m_ActionBird[2].LoadImage(TEXT(strPath+"Cartoon\\ActBirdBottom.png"));
	m_ActionBird[3].LoadImage(TEXT(strPath+"Cartoon\\ActBirdLeft.png"));
	///////////////////////////////////////////

	return;
}

//��ȡ·��
void CGameClientView::GetPicPath(CString& strPicPath)
{
	strPicPath="";
	TCHAR szResourcePath[MAX_PATH]={0};
	GetCurrentDirectory(MAX_PATH,szResourcePath);
	strPicPath.Append(szResourcePath);
	strPicPath.Append(TEXT("\\SparrowCS\\Picture\\PNG\\"));
}
//��������
CGameClientView::~CGameClientView(void)
{
}

//������Ϣ
int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//��������
	enDirection Direction[]={Direction_North,Direction_East,Direction_South,Direction_West};

	//���ÿؼ�
	for (WORD i=0;i<4;i++)
	{
		//�û��˿�
		m_HeapCard[i].SetDirection(Direction[i]);
		m_TableCard[i].SetDirection(Direction[i]);
		m_DiscardCard[i].SetDirection(Direction[i]);

		//����˿�
		m_WeaveCard[i][0].SetDisplayItem(true);
		m_WeaveCard[i][1].SetDisplayItem(true);
		m_WeaveCard[i][2].SetDisplayItem(true);
		m_WeaveCard[i][3].SetDisplayItem(true);
		m_WeaveCard[i][0].SetDirection(Direction[i]);
		m_WeaveCard[i][1].SetDirection(Direction[i]);
		m_WeaveCard[i][2].SetDirection(Direction[i]);
		m_WeaveCard[i][3].SetDirection(Direction[i]);
	}

	//���ÿؼ�
	m_UserCard[0].SetDirection(Direction_North);
	m_UserCard[1].SetDirection(Direction_East);
	m_UserCard[2].SetDirection(Direction_West);

	//�����ؼ�
	CRect rcCreate(0,0,0,0);
	m_ControlWnd.Create(NULL,NULL,WS_CHILD|WS_CLIPCHILDREN,rcCreate,this,10,NULL);

	//�����ؼ�
	CString strPath="";
	GetPicPath(strPath);
	m_btStart.Create(NULL,WS_CHILD,rcCreate,this,IDC_START);
	m_btAutoPlayOn.Create(NULL,WS_CHILD,rcCreate,this,IDC_AUTOPLAY_ON);
	m_btAutoPlayOff.Create(NULL,WS_CHILD,rcCreate,this,IDC_AUTOPLAY_OFF);
	m_btHaiDi.Create(NULL,WS_CHILD,rcCreate,this,IDC_HAI_DI);
	m_btNoHaiDi.Create(NULL,WS_CHILD,rcCreate,this,IDC_NO_HAI_DI);

	m_btStart.SetButtonImage(TEXT(strPath+"BEGIN.png"),false,5);
	m_btAutoPlayOn.SetButtonImage(TEXT(strPath+"AUTOPLAYON.png"),false,5);
	m_btAutoPlayOff.SetButtonImage(TEXT(strPath+"AUTOPLAYOFF.png"),false,5);
	m_btHaiDi.SetButtonImage(TEXT(strPath+"HAIDI.png"),false,5);
	m_btNoHaiDi.SetButtonImage(TEXT(strPath+"NO_HAIDI.png"),false,5);


	//bool bRect = m_CardHuPaiList.LoadImage("SparrowCS\\Picture\\PNG\\");
	//if (!bRect)
	//{
	//	AfxMessageBox("sadfasdf");
	//}

	//BYTE cbData[3]={0x01,0x21,0x18};
	//SetCanHuPaiData(cbData, 3, 300);
	//UpdateGameView(NULL);

	return 0;
}

//���ý���
void CGameClientView::ResetGameView()
{
	//��־����
	m_cbHaiDiCard=0;
	m_bOutCard=false;
	m_bWaitOther=false;
	m_bDiceEffect=false;
	m_bBirdEffect=false;

	m_nDiceCount1=0;
	m_nDiceCount2=0;

	//��Ϸ����
	m_lCellScore=0l;
	m_wBankerUser=INVALID_CHAIR;

	//�û�״̬
	m_cbCardData=0;
	m_cbBirdCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));

	//��������
	m_btStart.ShowWindow(SW_HIDE);
	m_ControlWnd.ShowWindow(SW_HIDE);
	
	//���ÿؼ�
	m_btAutoPlayOn.ShowWindow(TRUE);
	m_btAutoPlayOff.ShowWindow(SW_HIDE);

	//�˿�����
	m_UserCard[0].SetCardData(0,false);
	m_UserCard[1].SetCardData(0,false);
	m_UserCard[2].SetCardData(0,false);
	m_HandCardControl.SetPositively(false);
	m_HandCardControl.SetDisplayItem(false);
	m_HandCardControl.SetCardData(NULL,0,0);

	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_HeapCard[i].SetCardData(0,0,0);
		m_TableCard[i].SetCardData(NULL,0);
		m_DiscardCard[i].SetCardData(NULL,0);
		m_WeaveCard[i][0].SetCardData(NULL,0);
		m_WeaveCard[i][1].SetCardData(NULL,0);
		m_WeaveCard[i][2].SetCardData(NULL,0);
		m_WeaveCard[i][3].SetCardData(NULL,0);
	}

	return;
}

//�����ؼ�
void CGameClientView::RectifyGameView(int nWidth, int nHeight)
{
	//��������
    //��������
	m_ptBirdPos[0].x=155;
	m_ptBirdPos[0].y=198;
	m_ptBirdPos[1].x=33;
	m_ptBirdPos[1].y=137;
	m_ptBirdPos[2].x=242;
	m_ptBirdPos[2].y=87;
	m_ptBirdPos[3].x=246;
	m_ptBirdPos[3].y=138;

	//��������
	m_ptChiHuFlash[0].x=304;
	m_ptChiHuFlash[0].y=50;
	m_ptChiHuFlash[1].x=591;
	m_ptChiHuFlash[1].y=187;
	m_ptChiHuFlash[2].x=300;
	m_ptChiHuFlash[2].y=380;
	m_ptChiHuFlash[3].x=12;
	m_ptChiHuFlash[3].y=182;

	//������λ��
	m_ptSayPos[0].x = m_ptFace[0].x+50;
	m_ptSayPos[0].y = m_ptFace[0].y+30;
	m_ptSayPos[1].x = m_ptFace[1].x+50;
	m_ptSayPos[1].y = m_ptFace[1].y+30;
	m_ptSayPos[2].x = 480;
	m_ptSayPos[2].y = 650;
	m_ptSayPos[3].x = m_ptFace[3].x+50;
	m_ptSayPos[3].y = m_ptFace[3].y+30;

	//��Ϣ������
	m_Info[0].x=305;
	m_Info[0].y=134-27-5;
	m_Info[1].x=843;
	m_Info[1].y=334-29;
	m_Info[3].x=6;
	m_Info[3].y=334-29;

    //ͷ����������    
	m_ptFace[0].x=nWidth/2+220;
	m_ptFace[0].y=m_nYBorder+8;
	m_ptName[0].x=nWidth/2+270;
	m_ptName[0].y=m_nYBorder+112;
	m_ptFace[1].x=nWidth-m_nXBorder-125;
	m_ptFace[1].y=nHeight/2-119;
	m_ptName[1].x=nWidth-m_nXBorder-80;
	m_ptName[1].y=nHeight/2-135;
	m_ptFace[2].x=8;
	m_ptFace[2].y=nHeight-m_nYBorder-187;
	m_ptName[2].x=40;
	m_ptName[2].y=nHeight-m_nYBorder-82;
	m_ptFace[3].x=m_nXBorder+8;
	m_ptFace[3].y=nHeight/2-119;
	m_ptName[3].x=38;
	m_ptName[3].y=nHeight/2-135;

	//׼������
	m_ptReady[0].x=nWidth/2+10;
	m_ptReady[0].y=m_nYBorder+m_nYTimer/2+220;
	m_ptReady[1].x=nWidth-m_nXBorder-m_nXTimer-155;
	m_ptReady[1].y=nHeight/2-35;
	m_ptReady[2].x=nWidth/2+10;
	m_ptReady[2].y=nHeight-m_nYBorder-m_nYTimer/2-250;
	m_ptReady[3].x=m_nXBorder+m_nXTimer+155;
	m_ptReady[3].y=nHeight/2-35;


	//ʱ������
	m_ptTimer[0].x=555;
	m_ptTimer[0].y=120;
	m_ptTimer[1].x=835;
	m_ptTimer[1].y=249;
	m_ptTimer[2].x=876;
	m_ptTimer[2].y=530;
	m_ptTimer[3].x=115;
	m_ptTimer[3].y=249;

	//�й�����
	m_PointTrustee[0].x=549;
	m_PointTrustee[0].y=120-28;	
	m_PointTrustee[1].x=768;
	m_PointTrustee[1].y=212-28;	
	m_PointTrustee[2].x=138;
	m_PointTrustee[2].y=479-20;	
	m_PointTrustee[3].x=82;
	m_PointTrustee[3].y=212-28;

   //��λ����  ������
	//m_UserFlagPos[0].x=402;
	//m_UserFlagPos[0].y=50;
	//m_UserFlagPos[1].x=864;
	//m_UserFlagPos[1].y=137;
	//m_UserFlagPos[2].x=250;
	//m_UserFlagPos[2].y=660;
	//m_UserFlagPos[3].x=54;
	//m_UserFlagPos[3].y=137;

	
	//ׯ������
	m_UserBankerFlagPos[0].x=517;
	m_UserBankerFlagPos[0].y=26;
	m_UserBankerFlagPos[1].x=900;
	m_UserBankerFlagPos[1].y=110;
	m_UserBankerFlagPos[2].x=9;
	m_UserBankerFlagPos[2].y=490;
	m_UserBankerFlagPos[3].x=10;
	m_UserBankerFlagPos[3].y=110;

	////���Ʊ������
	//m_UserTingFlagPos[0].x=m_PointTrustee[0].x+89;
	//m_UserTingFlagPos[0].y=m_PointTrustee[0].y-45;
	//m_UserTingFlagPos[1].x=m_PointTrustee[1].x+90;
	//m_UserTingFlagPos[1].y=m_PointTrustee[1].y-47;
	//m_UserTingFlagPos[2].x=m_PointTrustee[2].x+90;
	//m_UserTingFlagPos[2].y=m_PointTrustee[2].y-47;
	//m_UserTingFlagPos[3].x=m_PointTrustee[3].x+88;
	//m_UserTingFlagPos[3].y=m_PointTrustee[3].y-46;


   
	//�û��˿�(����)
	m_UserCard[0].SetControlPoint(nWidth/2-180,m_nYBorder+155);
	m_UserCard[1].SetControlPoint(nWidth-m_nXBorder-230,nHeight/2-150);
	m_UserCard[2].SetControlPoint(m_nXBorder+102,nHeight/2-125);
	m_HandCardControl.SetBenchmarkPos(nWidth/2-30,nHeight-m_nYBorder-135,enXCenter,enYBottom); 

	//�������ļ����ưڵ������˿�(��ʱ�� ������2)
	m_TableCard[0].SetControlPoint(nWidth/2-40,m_nYBorder+175);
	m_TableCard[1].SetControlPoint(nWidth-m_nXBorder-110,nHeight/2-130);
	m_TableCard[2].SetControlPoint(nWidth/2+30,nHeight-m_nYBorder-195);
	m_TableCard[3].SetControlPoint(m_nXBorder+50,nHeight/2-80);

	//�ѻ��˿�(��ʼ��)
	int nXCenter=nWidth/2;
	int nYCenter=nHeight/2;
	m_HeapCard[0].SetControlPoint(nXCenter-165,nYCenter-160);
	m_HeapCard[1].SetControlPoint(nXCenter+185,nYCenter-140);
	m_HeapCard[2].SetControlPoint(nXCenter-260,nYCenter+80);
	m_HeapCard[3].SetControlPoint(nXCenter-240,nYCenter-135);

	////////////////////////////////////////////////////////////////////////////
	//BYTE cbCardData[ 20 ] = { 0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01 };
 //   m_HandCardControl[ 1 ].SetCardData( cbCardData, sizeof( cbCardData ) ); 
	//m_HandCardControl[ 0 ].SetCardData( cbCardData, sizeof( cbCardData ) );
	//m_HandCardControl[ 2 ].SetCardData( cbCardData, sizeof( cbCardData ) );
	//m_UserCardControl[ 1 ].SetCardData( cbCardData, 8 ); 
	//m_UserCardControl[ 0 ].SetCardData( cbCardData, 8 );
	//m_UserCardControl[ 2 ].SetCardData( cbCardData, 8 );
	//for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID ){
	//	m_bPass[ wChairID ] = true; 
	//	m_HandCardControl[ wChairID ].ShowWindow( SW_SHOW );
	//}
	//if ( m_ScoreView.m_hWnd == NULL ) m_ScoreView.Create(IDD_GAME_SCORE,this);
	//m_ScoreView.ShowWindow( SW_SHOW );
	//for ( WORD wChairID = 0; wChairID < GAME_PLAYER; ++wChairID ) {
	//	m_ScoreView.SetGameScore( wChairID, "dkfjdkfjdf", 00 );
	//}
	//////////////////////////////////////////////////////////////////////////////

	//�����˿�
	m_DiscardCard[0].SetControlPoint(nXCenter-90,nYCenter-116);
	m_DiscardCard[1].SetControlPoint(nXCenter+220,nYCenter-50);
	m_DiscardCard[2].SetControlPoint(nXCenter-125,nYCenter+42);
	m_DiscardCard[3].SetControlPoint(nXCenter-148,nYCenter+50);

	//����˿�
	m_WeaveCard[0][0].SetControlPoint(0, nWidth/2+200,m_nYBorder+175);
	m_WeaveCard[0][1].SetControlPoint(1, nWidth/2+145,m_nYBorder+175);
	m_WeaveCard[0][2].SetControlPoint(2, nWidth/2+90,m_nYBorder+175);
	m_WeaveCard[0][3].SetControlPoint(3, nWidth/2+35,m_nYBorder+175);

	//����˿�
	m_WeaveCard[1][0].SetControlPoint(0, nWidth-m_nXBorder-115,nHeight/2+63);
	m_WeaveCard[1][1].SetControlPoint(1, nWidth-m_nXBorder-152,nHeight/2-22);
	m_WeaveCard[1][2].SetControlPoint(2, nWidth-m_nXBorder-185,nHeight/2-97);
	m_WeaveCard[1][3].SetControlPoint(3, nWidth-m_nXBorder-207,nHeight/2-157);

	//����˿�
	m_WeaveCard[2][0].SetControlPoint(0, nWidth/2-370,nHeight-m_nYBorder-195);
	m_WeaveCard[2][1].SetControlPoint(1, nWidth/2-280,nHeight-m_nYBorder-195);
	m_WeaveCard[2][2].SetControlPoint(2, nWidth/2-190,nHeight-m_nYBorder-195);
	m_WeaveCard[2][3].SetControlPoint(3, nWidth/2-100,nHeight-m_nYBorder-195);

	//����˿�
	m_WeaveCard[3][0].SetControlPoint(0, m_nXBorder+203,nHeight/2-115);
	m_WeaveCard[3][1].SetControlPoint(1, m_nXBorder+182,nHeight/2-60);
	m_WeaveCard[3][2].SetControlPoint(2, m_nXBorder+155,nHeight/2+10);
	m_WeaveCard[3][3].SetControlPoint(3, m_nXBorder+125,nHeight/2+90);


	//test
	//BYTE byCard = 0x23;
	//BYTE cbActionMask = 1;
	//tagGangCardResult GangCardResult;
	//ZeroMemory(&GangCardResult,sizeof(GangCardResult));
	//GangCardResult.cbCardCount = 4;
	//GangCardResult.cbCardData[0] = 0x12;
	//GangCardResult.cbCardData[1] = 0x12;
	//GangCardResult.cbCardData[2] = 0x12;
	//GangCardResult.cbCardData[3] = 0x12;
    //m_ControlWnd.SetControlInfo(byCard,cbActionMask,GangCardResult);

	//���ƴ���
	m_ControlWnd.SetBenchmarkPos(nWidth/2+150,nHeight-m_nYBorder-210);

	//�ƶ���ť
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(32);
	m_btStart.GetWindowRect(&rcButton);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	//�ƶ�����
	DeferWindowPos(hDwp,m_btStart,NULL,403,324-29,0,0,uFlags);
	DeferWindowPos(hDwp,m_btHaiDi,NULL,595,487-29,0,0,uFlags);
	DeferWindowPos(hDwp,m_btNoHaiDi,NULL,661,487-29,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAutoPlayOn,NULL,571,616-29,0,0,uFlags);
	DeferWindowPos(hDwp,m_btAutoPlayOff,NULL,571,616-29,0,0,uFlags);
	//�ƶ�����
	//DeferWindowPos(hDwp,m_btStusteeControl,NULL,nWidth-m_nXBorder-(rcButton.Width()+5),nHeight-m_nYBorder-rcButton.Height()-180,0,0,uFlags);

	//�����ƶ�
	EndDeferWindowPos(hDwp);

	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		if(i!=3&&i!=2) SetFlowerControlInfo(i,m_ptFace[i].x, m_ptFace[i].y);
		else if(i==2) SetFlowerControlInfo(i,m_ptFace[i].x, m_ptFace[i].y-110);
		else SetFlowerControlInfo(i,m_ptFace[i].x-70, m_ptFace[i].y);
	}

	BYTE cbCardData[MAX_INDEX]={0x12,0x18,0x27,0x27,0x27,0x03,0x04,0x05,0x11,0x12,0x13,0x16,0x17};
	//m_HandCardControl.SetCardData(cbCardData,13,0x29,0x01);
	//m_HandCardControl.SetDisplayItem(true);
	//m_HandCardControl.SetPositively(true);

#define  MJ_H_FULL_COUNT (28)
#define  MJ_V_FULL_COUNT (26)
	//m_HeapCard[0].SetCardData(0,8,34);//top
	//m_HeapCard[1].SetCardData(0,6,34);//right
	//m_HeapCard[2].SetCardData(0,8,34);//bottom
	//m_HeapCard[3].SetCardData(0,6,34);//left

#define   mCardCount  13
	//m_UserCard[0].SetCardData(mCardCount,true);
	//m_UserCard[1].SetCardData(mCardCount,true);
	//m_UserCard[2].SetCardData(mCardCount,true);

	BYTE cbDCardData[18]={0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x12,0x11,0x25};
	//m_DiscardCard[0].SetCardData(cbDCardData,18);
	//m_DiscardCard[1].SetCardData(cbDCardData,18);
	//m_DiscardCard[2].SetCardData(cbDCardData,18);
	//m_DiscardCard[3].SetCardData(cbDCardData,18);


	BYTE cbWCardData[4]={0x01,0x12,0x01};
	//m_WeaveCard[0][0].SetCardData(cbWCardData,4);
	//m_WeaveCard[0][1].SetCardData(cbWCardData,4);
	//m_WeaveCard[0][2].SetCardData(cbWCardData,4);
	//m_WeaveCard[0][3].SetCardData(cbWCardData,4);

	//m_WeaveCard[1][0].SetCardData(cbWCardData,3);
	//m_WeaveCard[1][1].SetCardData(cbWCardData,3);
	//m_WeaveCard[1][2].SetCardData(cbWCardData,4);
	//m_WeaveCard[1][3].SetCardData(cbWCardData,4);

	//m_WeaveCard[2][0].SetCardData(cbWCardData,4);
	//m_WeaveCard[2][1].SetCardData(cbWCardData,4);
	//m_WeaveCard[2][2].SetCardData(cbWCardData,4);
	//m_WeaveCard[2][3].SetCardData(cbWCardData,4);

	//m_WeaveCard[3][0].SetCardData(cbWCardData,4);
	//m_WeaveCard[3][1].SetCardData(cbWCardData,4);
	//m_WeaveCard[3][2].SetCardData(cbWCardData,4);
	//m_WeaveCard[3][3].SetCardData(cbWCardData,4);


	#define   mTCardCount  14
	BYTE cbTCardData[8]={0x03,0x04,0x02,0x12,0x11,0x25,0x02,0x12};

	//m_TableCard[0].SetCardData(cbTCardData,8);
	//m_TableCard[1].SetCardData(cbTCardData,7);
	//m_TableCard[2].SetCardData(cbTCardData,2);
	//m_TableCard[3].SetCardData(cbTCardData,7);
	//m_btTingCard.ShowWindow(SW_SHOW);
	//m_btBuTing.ShowWindow(SW_SHOW);
	return;
}


////////����/////////
//

//���񶯻�
void CGameClientView::ActionPlayBird(CDC *pDC,int nFrame,BYTE byViewID, CPoint pt, BYTE cbBirdCardData)
{
	m_ActionBird[byViewID].DrawImage( pDC, pt.x, pt.y, 356, 297, 356*nFrame, 0);
	if (nFrame>12)
	{
        nFrame=12;
	}
	if (nFrame>6)
	{
		//�滭�˿�
		BYTE byIndex = m_CardControl.GetImageIndex(cbBirdCardData);
		m_CardControl.m_ImageOutCardNum.DrawImage(pDC,pt.x+m_ptBirdPos[byViewID].x, pt.y+m_ptBirdPos[byViewID].y,43,58,43*byIndex,0,43,58);
	}
}

////��ʼ����
//void CGameClientView::ActionStart( CDC *pDC, int nFrame )
//{
//	LONG lWidth, lHeight;
//	lWidth =m_ActionStart.GetWidth()/4;
//	lHeight = m_ActionStart.GetHeight();
//
//	m_ActionStart.DrawImage( pDC, 150, 150, lWidth, lHeight, lWidth*nFrame, 0);
//}

//���Ӷ���1
void CGameClientView::ActionDice1( CDC *pDC, int nFrame )
{
	LONG lWidth, lHeight;
	lWidth = m_csActionDice1.cx;
	lHeight = m_csActionDice1.cy;
	m_ActionDice1.DrawImage( pDC, PS_BX_DICE, PS_BY_DICE, lWidth, lHeight, lWidth*nFrame, 0);
}


//���Ӷ���2
void CGameClientView::ActionDice2( CDC *pDC,int pos1, int pos2 )
{
	LONG lWidth, lHeight;
	lWidth = m_csActionDice2.cx;
	lHeight = m_csActionDice2.cy;

	m_ActionDice2.DrawImage( pDC, PS_BX_DICE1, PS_BY_DICE1, lWidth, lHeight, lWidth*pos1, 0);
	m_ActionDice2.DrawImage( pDC, PS_BX_DICE2, PS_BY_DICE2, lWidth, lHeight, lWidth*pos2, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//�滭����
void CGameClientView::DrawGameView(CDC * pDC, int nWidth, int nHeight)
{
	//�滭����
	m_ImageBack.DrawImage(pDC,0,0,960,691,0,0);

	//�滭�û�
	pDC->SetTextColor(RGB(255,255,0));
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		const tagUserData * pUserData=GetUserInfo(i);
		if (pUserData!=NULL)
		{
			char chUserDataInfo[100]={0};
			//::memset(chUserDataInfo, 0, sizeof(chUserDataInfo));
			//sprintf(chUserDataInfo, "%d", pUserData->wFaceID);
			//DrawUserFace(pDC,pUserData,m_ptFace[i].x,m_ptFace[i].y,pUserData->cbUserStatus==US_OFFLINE);


			//�����û���Ϣ��
			if (2!=i)
			{
				m_ImageUserInfo[i].DrawImage(pDC,m_Info[i].x,m_Info[i].y);
			}
            
			//�û�����
			//DrawTextString(pDC,pUserData->szName,RGB(240,240,240),RGB(50,50,50),m_ptName[i].x-10,m_ptName[i].y);

			//ʱ��
			DrawUserTimer(pDC, i);

			//�ȼ���ν
			if (i!=2)
			{
				pDC->TextOut(m_Info[i].x+37,m_Info[i].y+28,pUserData->chGradeName,strlen(pUserData->chGradeName));
			}

			//�ȼ�
			if (i!=2)
			{
				INT GrandWith=m_ImageUserGradeNum.GetWidth()/12;
				INT GrandHeighe=m_ImageUserGradeNum.GetHeight();
				INT GrandCount=0;
				INT Grand=pUserData->dwGrade;
				INT Fan=Grand;
				while(1<=Grand)
				{
					GrandCount++;
					Grand/=10;
				}
				if (1<=GrandCount)
				{
					INT xPos=m_Info[i].x+40;
					INT yPos=m_Info[i].y+7;
					while (0<GrandCount)
					{
						xPos+=GrandWith;
						--GrandCount;
						INT n_Mutiple=(INT)pow(10,GrandCount);
						INT DrawGrand=(INT)(Fan/n_Mutiple);
						m_ImageUserGradeNum.DrawImage(pDC,xPos,yPos,GrandWith,GrandHeighe,DrawGrand*GrandWith,0);
						Fan-=(n_Mutiple*DrawGrand);
					}
				}
			}

			if (pUserData->cbUserStatus==US_READY)
			{
				DrawUserReady(pDC,m_ptReady[i].x,m_ptReady[i].y);
			}

		}

		//�й�
		if(m_bTrustee[i])
		{
			m_ImageTrustee.DrawImage(pDC,m_PointTrustee[i].x,m_PointTrustee[i].y);
		}

		//��������ʾ
		if((m_cbUserAction[i]&WIK_CHI_HU) && (m_wChiHuRight[i] & CHR_HAI_DI))
		{
			//��������
			int nXImagePos=0;
			int nItemWidth=m_ImageHuangZhuang.GetWidth();
			int nItemHeight=m_ImageHuangZhuang.GetHeight()/2;

			//�滭����
			BYTE byIndex = m_CardControl.GetImageIndex(m_cbHaiDiCard);
			m_CardControl.m_ImageUserBottom.DrawImage(pDC,450,450,48,75,byIndex*48,0,48,75);
			m_ImageHuangZhuang.DrawImage(pDC,390,410,nItemWidth,45,nXImagePos,0,nItemWidth, 45);
		}
	}

	//�û���־
	if (m_wBankerUser!=INVALID_CHAIR)
	{
		//����λͼ
		int nImageWidth=m_ImageUserFlag.GetWidth()/3;
		int nImageHeight=m_ImageUserFlag.GetHeight();

		int nBankerWidth = m_ImageUserBankerFlag.GetWidth();
	    int nBankerHeight = m_ImageUserBankerFlag.GetHeight();

		//�滭��־
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			WORD wIndex=((i+GAME_PLAYER)-m_wBankerUser)%GAME_PLAYER;
			if (0==wIndex)
			{//ׯ�ұ�ʶ
				m_ImageUserBankerFlag.DrawImage(pDC, m_UserBankerFlagPos[i].x,m_UserBankerFlagPos[i].y,nBankerWidth,nBankerHeight,0,0);
			}
			else
			{
				m_ImageDirectionBack.DrawImage(pDC, m_UserBankerFlagPos[i].x,m_UserBankerFlagPos[i].y);
				m_ImageUserFlag.DrawImage(pDC, m_UserBankerFlagPos[i].x+7,m_UserBankerFlagPos[i].y+15,nImageWidth,nImageHeight,nImageWidth*(wIndex-1),0,nImageWidth,nImageHeight);
			}
			
		}
	}


	//�����˿�
	for (WORD i=0;i<4;i++)
	{
		m_TableCard[i].DrawCardControl(pDC);
		m_DiscardCard[i].DrawCardControl(pDC);
		m_WeaveCard[i][0].DrawCardControl(pDC);
		m_WeaveCard[i][1].DrawCardControl(pDC);
		m_WeaveCard[i][2].DrawCardControl(pDC);
		m_WeaveCard[i][3].DrawCardControl(pDC);
	}

	//�ѻ��˿�
	m_HeapCard[3].DrawCardControl(pDC);
	m_HeapCard[0].DrawCardControl(pDC);
	m_HeapCard[1].DrawCardControl(pDC);
	m_HeapCard[2].DrawCardControl(pDC);

	//�û��˿�
	m_UserCard[0].DrawCardControl(pDC);
	m_UserCard[1].DrawCardControl(pDC);
	m_UserCard[2].DrawCardControl(pDC);
	m_HandCardControl.DrawCardControl(pDC);


	//���Ӷ���
	if (m_nDiceCount1<0 || m_nDiceCount1>5)
	{
		m_nDiceCount1=0;
	}
	if (m_nDiceCount2<0 || m_nDiceCount2>5)
	{
		m_nDiceCount2=0;
	}
	if (m_bDiceEffect==true)
	{
		if( m_cbFrameIndex>=0 && m_cbFrameIndex<9)
		{
			ActionDice1( pDC,m_cbFrameIndex);
		}
		else if( m_cbFrameIndex>= 9)
		{
			ActionDice2( pDC,m_nDiceCount1,m_nDiceCount2);
		}
	}


	//���񶯻�
	if (m_bBirdEffect==true)
	{
		if (m_cbBirdCardData!=0)
		{
			if( m_tPlayBirdIndex>=0 && m_tPlayBirdIndex<15)
			{
				CPoint pt;
				pt.x=m_ptChiHuFlash[m_bViewBirdID].x;
				pt.y=m_ptChiHuFlash[m_bViewBirdID].y;
				ActionPlayBird( pDC,m_tPlayBirdIndex,m_bViewBirdID,pt,m_cbBirdCardData);
				m_tPlayBirdIndex++;
			}	
		}

	}

	//��ׯ��־
	if (true == m_bIsHuangZhuang && m_cbHaiDiCard!=0)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
	{
        BYTE byIndex = m_CardControl.GetImageIndex(m_cbHaiDiCard);
		m_CardControl.m_ImageUserBottom.DrawImage(pDC,450,450,48,75,byIndex*48,0,48,75);
		m_ImageHuangZhuang.DrawImage(pDC,390,411,180,45,0,0,180, 45);
	}

	//�û�״̬
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
	    if (m_wOutCardUser==i)   //�����û�
		{
			//����λ��
			int nXPos=0,nYPos=0;
			switch (i)
			{
			case 0:	//����
				{
					nXPos=415;
					nYPos=180;
					break;
				}
			case 1:	//����
				{
					nXPos=680;
					nYPos=250;
					break;
				}
			case 2:	//����
				{
					nXPos=410;
					nYPos=380;
					break;
				}
			case 3:	//����
				{
					nXPos=150;
					nYPos=250;
					break;
				}
			}

			//�滭����
			if (m_cbCardData>0)
			{//��������
				m_ImageOutCardBack.DrawImage(pDC,nXPos,nYPos,m_ImageOutCardBack.GetWidth(),m_ImageOutCardBack.GetHeight(),0,0);
				//�滭�˿�
				BYTE byIndex = m_CardControl.GetImageIndex(m_cbCardData);
				m_CardControl.m_ImageOutCardNum.DrawImage(pDC,nXPos+53,nYPos+50,43,58,43*byIndex,0,43,58);
			}
		}
		if (m_cbUserAction[i]!=0)    //�����ܶ���
		{
			//����λ��
			int nXPos=0,nYPos=0;
			int nXPost=0,nYPost=0;
			switch (i)
			{
			case 0:	//����
				{
					nXPos=475;
					nYPos=89-28;
					nXPost=nWidth/2+280;
					nYPost=m_nYBorder-15;
					break;
				}
			case 1:	//����
				{
					nXPos=793;
					nYPos=138-28;
					nXPost=nWidth-m_nXBorder-235;
					nYPost=nHeight/2-120;
					break;
				}
			case 2:	//����
				{
					nXPos=110;
					nYPos=419-28;
					nXPost=nWidth/2-385;
					nYPost=nHeight-m_nYBorder-240;
					break;
				}
			case 3:	//����
				{
					nXPos=64;
					nYPos=153-28;
					nXPost=m_nXBorder;
					nYPost=nHeight/2-120;
					break;
				}
			}
		}
	}

	return;
}

//��������
void CGameClientView::SetCellScore(LONGLONG lCellScore)
{
	//�����˿�
	if (lCellScore!=m_lCellScore)
	{
		//���ñ���
		m_lCellScore=lCellScore;

		//���½���
		UpdateGameView(NULL);
	}

	return;
}

//�����˿�
void CGameClientView::SetHaiDiCard(BYTE cbHaiDiCard)
{
	//�����˿�
	if (cbHaiDiCard!=m_cbHaiDiCard)
	{
		//���ñ���
		m_cbHaiDiCard=cbHaiDiCard;

		//���½���
		UpdateGameView(NULL);
	}

	return;
}

//ׯ���û�
void CGameClientView::SetBankerUser(WORD wBankerUser)
{
	//�����û�
	if (wBankerUser!=m_wBankerUser)
	{
		//���ñ���
		m_wBankerUser=wBankerUser;

		//���½���
		UpdateGameView(NULL);
	}

	return;
}

//״̬��־
void CGameClientView::SetStatusFlag(bool bOutCard, bool bWaitOther)
{
	//���ñ���
	m_bOutCard=bOutCard;
	m_bWaitOther=bWaitOther;

	//���½���
	UpdateGameView(NULL);

	return;
}

//������Ϣ
void CGameClientView::SetOutCardInfo(WORD wViewChairID, BYTE cbCardData)
{
	//���ñ���
	m_cbCardData=cbCardData;
	m_wOutCardUser=wViewChairID;

	//���½���
	UpdateGameView(NULL);

	return;
}

//������Ϣ
void CGameClientView::SetUserAction(WORD wViewChairID, BYTE bUserAction)
{
	//���ñ���
	if (wViewChairID<GAME_PLAYER) m_cbUserAction[wViewChairID]=bUserAction;
	else ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));

	//���½���
	UpdateGameView(NULL);

	return;
}
//�����й�
void CGameClientView::SetTrustee(WORD wTrusteeUser,bool bTrustee)
{
	//У������ 
	ASSERT(wTrusteeUser>=0&&wTrusteeUser<GAME_PLAYER);

	if(m_bTrustee[wTrusteeUser] !=bTrustee)	
	{
		//��������
		m_bTrustee[wTrusteeUser]=bTrustee;

		//���½���
		UpdateGameView(NULL);
	}
	return;

}
//��������
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//�滭����
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	return;
}

//�����Ϣ
BOOL CGameClientView::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//��ȡ���
	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�������
	bool bRePaint=false;
	bool bHandle=m_HandCardControl.OnEventSetCursor(MousePoint,bRePaint);

	//�ػ�����
	if (bRePaint==true)
	{
		CRect rect;
		rect.left = 100;
		rect.top = 500;
		rect.right = rect.left + 700;
		rect.bottom = rect.top + 90;
		UpdateGameView(&rect);
	}

	//������
	if (bHandle==false) __super::OnSetCursor(pWnd,nHitTest,uMessage);

	return TRUE;
}

//�����Ϣ
void CGameClientView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	//��ȡ�˿�
	BYTE cbHoverCard=m_HandCardControl.GetHoverCard();
	if (cbHoverCard!=0) AfxGetMainWnd()->PostMessage(IDM_OUT_CARD,cbHoverCard,cbHoverCard);

	m_CardHuPaiList.ShowWindow(SW_HIDE);


	return;
}



//���Ӷ���
bool CGameClientView::SetDiceEffect(bool bDiceEffect)
{
	if (bDiceEffect==true)
	{
		//���ñ���
		m_bDiceEffect=true;
		m_cbFrameIndex=0;

		//����ʱ��
		SetTimer(IDI_DICE_EFFECT,200,NULL);
	}
	else
	{
		//ֹͣ����
		if (m_bDiceEffect==true)
		{
			//ɾ��ʱ��
			KillTimer(IDI_DICE_EFFECT);

			//���ñ���
			m_bDiceEffect=false;
			m_cbFrameIndex=0;

			//���½���
			UpdateGameView(NULL);
		}
	}

	return true;
}

//���񶯻�
bool CGameClientView::SetBirdEffect(bool bBirdEffect,BYTE bViewID)
{
	if (bBirdEffect==true)
	{
		//���ñ���
		m_bViewBirdID=bViewID;
		m_bBirdEffect=true;
		m_cbFrameIndex=0;

		//����ʱ��
		SetTimer(IDI_BIRD_EFFECT,200,NULL);
	}
	else
	{
		//ֹͣ����
		if (m_bBirdEffect==true)
		{
			//ɾ��ʱ��
			KillTimer(IDI_BIRD_EFFECT);

			//���ñ���
			m_bBirdEffect=false;
			m_cbFrameIndex=0;

			//���½���
			UpdateGameView(NULL);
		}
	}

	return true;
}

//ʱ����Ϣ 2011.8.3
VOID CGameClientView::OnTimer(UINT nIDEvent)
{
	//���Ӷ���
	if (nIDEvent==IDI_DICE_EFFECT)
	{

		//���ñ���
		if ((m_cbFrameIndex+1)>=DICE_EFFECT_COUNT+6)
		{
			//ɾ��ʱ��
			KillTimer(IDI_DICE_EFFECT);

			//���ñ���
			m_bDiceEffect=false;
			m_cbFrameIndex=0;
			AfxGetMainWnd()->SendMessage(IDM_SHOW_SZ,0,0);
		}
		else 
		{
			m_cbFrameIndex++;
		}

		//���½���
		UpdateGameView(NULL);

		return;
	}

	//���񶯻�
	if (nIDEvent==IDI_BIRD_EFFECT)
	{

		//���ñ���
		if ((m_cbFrameIndex+1)>=IDI_BIRD_EFFECT+12)
		{
			//ɾ��ʱ��
			KillTimer(IDI_BIRD_EFFECT);

			//���ñ���
			m_bBirdEffect=false;
			m_cbFrameIndex=0;
		}
		else 
		{
			m_cbFrameIndex++;
		}

		//���½���
		UpdateGameView(NULL);

		return;
	}



	__super::OnTimer(nIDEvent);
}

//��ʼ��ť
void CGameClientView::OnStart()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_START,0,0);

	return;
}

//���װ�ť
void CGameClientView::OnHaiDi()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_OPERATE_HAI_DI,1,1);

	return;
}

//������ť
void CGameClientView::OnNoHaiDi()
{
	//������Ϣ
	AfxGetMainWnd()->SendMessage(IDM_OPERATE_HAI_DI,0,0);

	return;
}
//�Ϲܿ���
void CGameClientView::OnStusteeControl()
{
	AfxGetMainWnd()->SendMessage(IDM_TRUSTEE_CONTROL,0,0);
	return;
}


// �Զ��й�
void CGameClientView::OnAutoPlayerOn()
{
	m_btAutoPlayOn.ShowWindow(SW_HIDE);
	m_btAutoPlayOff.ShowWindow(SW_SHOW);
	AfxGetMainWnd()->SendMessage(IDM_TRUSTEE_CONTROL,1,1);
	return;
}

// ȡ���й�
void CGameClientView::OnAutoPlayerOff()
{
	m_btAutoPlayOn.ShowWindow(SW_SHOW);
	m_btAutoPlayOff.ShowWindow(SW_HIDE);
	AfxGetMainWnd()->SendMessage(IDM_TRUSTEE_CONTROL,0,0);
	return;
}

//��ȡ��������
void CGameClientView::SetCanHuPaiData(const BYTE* pCardData, int iCardNums, int iX)
{
	if (NULL != pCardData &&  iCardNums>0)
	{
		CRect rect;
		POINT ptLocal;
		ptLocal.x = iX;
		ptLocal.y = 480;
		m_CardHuPaiList.SetCardData(pCardData, iCardNums);
		m_CardHuPaiList.ShowWin(ptLocal);
		m_CardHuPaiList.ShowWindow(SW_SHOW);
	}
	else
	{
		m_CardHuPaiList.SetCardData(NULL, 0);
		m_CardHuPaiList.ShowWindow(SW_HIDE);
	}
}

void CGameClientView::SetGameClientDlgPtr(CGameClientDlg* pGameClient)
{
	if (NULL != pGameClient)
	{
		m_pGameClientDlg = pGameClient;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CGameClientView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CPoint MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(&MousePoint);

	//�������
	int ibHandle=m_HandCardControl.CheckIndex(MousePoint);

	if (m_iSaveCheckPaiIndex == ibHandle)
	{
		CGameFrameView::OnMouseMove(nFlags, point);
		return;
	}
	m_iSaveCheckPaiIndex = ibHandle;
	if (ibHandle < 0)
	{
		m_HandCardControl.InitCurCardDataX();
		m_CardHuPaiList.ShowWindow(SW_HIDE);
		UpdateGameView(NULL);
	}
	else if (ibHandle>=0 && ibHandle<=13)
	{
		BYTE byCardData=0;
		int iX = 0;
		m_HandCardControl.GetCurCardDataAndX(byCardData, iX);
		if (0 != byCardData)
		{
			m_pGameClientDlg->CheckCanHuPai(byCardData, iX);
		}
	}
	CGameFrameView::OnMouseMove(nFlags, point);
}


void CGameClientView::PlayOperationFlash(BYTE byViewID)
{
	m_GameOperationFlash.Play(false);

	CString strPath="";
	GetPicPath(strPath);
	strPath.Append("Cartoon\\");
	CPoint pt;
	pt.x = 0;
	pt.y = 0;
	pt.x=m_ptChiHuFlash[byViewID].x;
	pt.y=m_ptChiHuFlash[byViewID].y;
    if (WIK_PENG&m_cbUserAction[byViewID])
	{
		if (0==byViewID)
		{
			pt.x=293;
			pt.y=109;
		}
		else if (1==byViewID)
		{
			pt.x=569;
			pt.y=158;
		}
		else if (2==byViewID)
		{
			pt.x=290;
			pt.y=387;
		}
		else
		{
			pt.x=63;
			pt.y=134;
		}
		m_GameOperationFlash.InitFlash(strPath+"ActPeng.png", 12, pt);
	}
	else if ((WIK_LEFT&m_cbUserAction[byViewID])||(WIK_CENTER&m_cbUserAction[byViewID])||(WIK_RIGHT&m_cbUserAction[byViewID]))
	{
		if (0==byViewID)
		{
			pt.x=293;
			pt.y=109;
		}
		else if (1==byViewID)
		{
			pt.x=602;
			pt.y=251;
		}
		else if (2==byViewID)
		{
			pt.x=290;
			pt.y=438;
		}
		else
		{
			pt.x=79;
			pt.y=238;
		}
		m_GameOperationFlash.InitFlash(strPath+"ActChi.png", 12, pt);
	}
	//else if ((m_cbUserAction[byViewID]&WIK_FILL) || (m_cbUserAction[byViewID]&WIK_GANG))
	//{
	//	//m_GameOperationFlash.InitFlash(strPath+"ActBu.png", 10, pt);
	//}
	else if (m_cbUserAction[byViewID]&WIK_TING) 
	{
		if (0==byViewID)
		{
			pt.x=373;
			pt.y=32;
		}
		else if (1==byViewID)
		{
			pt.x=678;
			pt.y=191;
		}
		else if (2==byViewID)
		{
			pt.x=357;
			pt.y=377;
		}
		else
		{
			pt.x=74;
			pt.y=182;
		}
		m_GameOperationFlash.InitFlash(strPath+"ActGang.png", 13, pt);
	}
	m_GameOperationFlash.Play(true, 1);

}


void CGameClientView::PlayChiHuFlash(BYTE byViewID)
{
	m_ChiHuKindFlash.Play(false);
	
	CString strPath="";
	GetPicPath(strPath);
	strPath.Append("Cartoon\\");
	CPoint pt;
	pt.x=m_ptChiHuFlash[byViewID].x;
	pt.y=m_ptChiHuFlash[byViewID].y;

	if (m_cbUserAction[byViewID]&WIK_CHI_HU)
	{
		if(m_wChiHuRight[byViewID] & CHR_TIAN)
		{//���
			//MessageBox("���");
			m_ChiHuKindFlash.InitFlash(strPath+"ActTian.png", 21, pt);
		}
		else  if(m_wChiHuRight[byViewID] & CHR_DI)
		{//�غ�
			//MessageBox("�غ�");
			m_ChiHuKindFlash.InitFlash(strPath+"ActDi.png", 13, pt);
		}
		else  if(m_wChiHuKind[byViewID] & CHK_SIXI_HU)
		{//��ϲ
			//MessageBox("��ϲ");
			m_ChiHuKindFlash.InitFlash(strPath+"ActSiXi.png", 8, pt);
		}
		else  if(m_wChiHuKind[byViewID] & CHK_BANBAN_HU)
		{//����
			//MessageBox("����");
			m_ChiHuKindFlash.InitFlash(strPath+"ActBanBanHu.png", 11, pt);
		}
		else  if(m_wChiHuKind[byViewID] & CHK_LIULIU_HU)
		{//����˳
			//MessageBox("����˳");
			m_ChiHuKindFlash.InitFlash(strPath+"ActLiuLiuShun.png", 13, pt);
		}
		else  if(m_wChiHuKind[byViewID] & CHK_QUEYISE_HU)
		{//ȱһɫ
			//MessageBox("ȱһɫ");
			m_ChiHuKindFlash.InitFlash(strPath+"ActQueYiSe.png", 16, pt);
		}
		else 
		if (3==m_iChiHuNums)
		{//ͨ�ڣ�һ�����죩
			m_ChiHuKindFlash.InitFlash(strPath+"ActTongPao.png", 11, CPoint(376,242));
		}
		else  if(m_wChiHuRight[byViewID] & CHR_HAI_DI)
		{//����
			if (4==m_wChiHuType)
			{//������
				m_ChiHuKindFlash.InitFlash(strPath+"ActHaiDiPao.png", 8, CPoint(308,260));
			}
			else if(3==m_wChiHuType)
			{//��������(����)
				m_ChiHuKindFlash.InitFlash(strPath+"ActHaiDiLaoYue.png", 8, CPoint(200,110));
			}
		}
		else  if((2==m_wChiHuType)&&(m_wChiHuRight[byViewID] & CHR_QIANG_GANG))
		{//���ܺ��������ڣ�
			if (0==byViewID)
			{
				pt.x=328;
				pt.y=100;
			}
			else if (1==byViewID)
			{
				pt.x=664;
				pt.y=255;
			}
			else if (2==byViewID)
			{
				pt.x=300;
				pt.y=463;
			}
			else
			{
				pt.x=72;
				pt.y=238;
			}
			m_ChiHuKindFlash.InitFlash(strPath+"ActQiangGangHu.png", 7, pt);
		}
		else  if((1==m_wChiHuType)&&(m_wChiHuRight[byViewID] & CHR_QIANG_GANG))
		{//���Ͽ�����������
			if (0==byViewID)
			{
				pt.x=334;
				pt.y=43;
			}
			else if (1==byViewID)
			{
				pt.x=646;
				pt.y=200;
			}
			else if (2==byViewID)
			{
				pt.x=305;
				pt.y=378;
			}
			else
			{
				pt.x=75;
				pt.y=186;
			}
			m_ChiHuKindFlash.InitFlash(strPath+"ActGangShangHua.png", 23, pt);
		}
		else if(m_wChiHuKind[byViewID] & CHK_PENG_PENG)
		{//������
			if ((0==byViewID)||(2==byViewID))
			{
				if (0==byViewID)
				{
					pt.x=133;
					pt.y=81;
				}
				else
				{
					pt.x=131;
					pt.y=442;
				}
				m_ChiHuKindFlash.InitFlash(strPath+"ActPengPengHuH.png", 13, pt);
			}
			else
			{
				if (1==byViewID)
				{
					pt.x=564;
					pt.y=222;
				}
				else
				{
					pt.x=28;
					pt.y=215;
				}
				m_ChiHuKindFlash.InitFlash(strPath+"ActPengPengHuV.png", 13, pt);
			}
			
		}
		else  if(m_wChiHuKind[byViewID] & CHK_QI_XIAO_DUI)
		{//��С��
			if (0==byViewID)
			{
				pt.x=288;
				pt.y=100;
			}
			else if (1==byViewID)
			{
				pt.x=589;
				pt.y=247;
			}
			else if (2==byViewID)
			{
				pt.x=274;
				pt.y=461;
			}
			else
			{
				pt.x=44;
				pt.y=238;
			}
			m_ChiHuKindFlash.InitFlash(strPath+"ActQiXiaoDui.png", 13, pt);
		}
		else  if(m_wChiHuRight[byViewID] & CHR_QING_YI_SE)
		{//��һɫ
			if (0==byViewID)
			{
				pt.x=271;
				pt.y=29;
			}
			else if (1==byViewID)
			{
				pt.x=511;
				pt.y=197;
			}
			else if (2==byViewID)
			{
				pt.x=261;
				pt.y=367;
			}
			else
			{
				pt.x=24;
				pt.y=157;
			}
			m_ChiHuKindFlash.InitFlash(strPath+"ActQingYiSe.png", 24, pt);
		}
		else  if(m_wChiHuKind[byViewID] & CHK_JIANG_JIANG)
		{//������
			if (0==byViewID)
			{
				pt.x=267;
				pt.y=28;
			}
			else if (1==byViewID)
			{
				pt.x=555;
				pt.y=218;
			}
			else if (2==byViewID)
			{
				pt.x=272;
				pt.y=408;
			}
			else
			{
				pt.x=1;
				pt.y=197;
			}
			m_ChiHuKindFlash.InitFlash(strPath+"ActJiangJiangHu.png", 12, pt);
		}
		else  if(1==m_wChiHuType)
		{//ƽ������
			//MessageBox("ƽ������");
			if (0==byViewID)
			{
				pt.x=302;
				pt.y=69;
			}
			else if (1==byViewID)
			{
				pt.x=603;
				pt.y=205;
			}
			else if (2==byViewID)
			{
				pt.x=274;
				pt.y=396;
			}
			else
			{
				pt.x=54;
				pt.y=205;
			}
			m_ChiHuKindFlash.InitFlash(strPath+"ActZiMo.png", 9, pt);
		}
		else  if(2==m_wChiHuType)
		{//ƽ������
			if (0==byViewID)
			{
				pt.x=355;
				pt.y=26;
			}
			else if (1==byViewID)
			{
				pt.x=696;
				pt.y=240;
			}
			else if (2==byViewID)
			{
				pt.x=328;
				pt.y=410;
			}
			else
			{
				pt.x=63;
				pt.y=240;
			}
			m_ChiHuKindFlash.InitFlash(strPath+"ActPingHu.png", 15, pt);
		}		
	}	

	m_ChiHuKindFlash.Play(true, 1);

	if(0==m_wChiHuType)
	{//��ׯ
		m_ChiHuKindFlash.InitFlash(strPath+"LiuJu.png", 6, CPoint(375,300));
	}
}


void CGameClientView::InitViewFrame()
{

	m_CardHuPaiList.Create(IDD_AI_PROMPT,this);
	m_GameOperationFlash.Create(IDD_AI_PROMPT, this);
	m_ChiHuKindFlash.Create(IDD_AI_PROMPT, this);
	bool bRect = m_CardHuPaiList.LoadImage("SparrowCS\\Picture\\PNG\\");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////