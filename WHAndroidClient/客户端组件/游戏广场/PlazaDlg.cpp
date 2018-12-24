// PlazaDlg.cpp : ʵ���ļ�
//

#include "stdafx.h" 
#include <math.h>
#include "GlobalUnits.h"
#include "Zip.h"
#include "GameInfo.h"
#include <afxtempl.h>
#include <afxctl.h>
#include "webpath.h"
#include "Mmsystem.h"
#include "GamePlaza.h"
#include "InitData.h"
#include "NewComerGuide.h"
#include "Active.h"
#include "PlazaDlg.h"
#include ".\plazadlg.h"

// CPlazaDlg �Ի���

#define INNO_ISS_MSG_CLOSE			1124			//Inno ��װ�ű���������Ϣ �رմ�����Ϣ
#define INNO_ISS_MSG_SELECT_GAME	1125			//Inno ��װ�ű���������Ϣ ѡ����Ϸ

#define IDI_WEB_PLAZA_ADVERTISEMENT	1109281405		//�������
#define IDI_WEB_PLAZA_NOTICE		1109281406		//��������
#define IDI_WEB_PLAZA_MESSAGE		1110241427		//������Ϣ

//͸����ʱ��
#define		IDI_TIME_SET_DLG_ALPHA_VALUE		20315
#define		IDI_TIME_SET_DLG_ALPHA_END			20316
#define		TIME_SET_DLG_ALPHA_INTERVAL			20
#define		TIME_SET_DLG_ALPHA_END_INTERVAL		5000

//���ű�������
#define		IDI_TIMER_PLAY_BACK_MUSIC			20317
#define		TIME_PLAY_BACK_MUSIC_INTERVAL		1000

//��Ϸ��Ϣ��ʱ��
#define		IDI_TIMER_GAME_MSG_FLASH			20318
#define		TIME_GAME_MSG_FLASH_INTERVAL		500
#define		TIMER_WEB_LOADING_PLAZA				1111031445			//������ҳ

//���°�ť��ʱ��
#define		TIMER_UPDATEUSERINFO_OPERATE		1001

//����ָ����ʱ��
#define		IDI_ITMER_MEWCOMER_GUIDE			4002

// �ȴ�����
#define IDI_TIMER_WAIT_END						4003

// �ȴ������·
#define TIME_WAIT_CHECK_LINE					4004

//��ֵ
#define		DEF_NUM_VALUE			0.95


IMPLEMENT_DYNAMIC(CPlazaDlg, CDialog)
CPlazaDlg::CPlazaDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPlazaDlg::IDD, pParent),m_pGradeInfo(NULL)
//CPlazaDlg::CPlazaDlg() : CDialog(CPlazaDlg::IDD)
{
	EnableAutomation();//������,����Ȼ���GetIDispatch()ʱ��ʧ��.

	m_bInitDialog	= false;
	m_bLogonPlaza	= false;
	m_bDownloadConnect=false;
	m_bStartDownloadFace=false;
	m_gameListManager.Init();
	//cxf m_gameListDlg.m_gameListManager=&m_gameListManager;//��Ϸ�б�
	//cxf 1022 m_roomListDlg.m_gameListManager=&m_gameListManager;//�����б�
	m_curTab = 0;	//��ǰҳ��
	m_pRoomViewItemCreate=NULL;
	m_bIsShowXJInfo = false;

	m_hUser32Instance = NULL;
	m_pFunShowLayer = NULL;
	m_uiCurAlphaValue = 0;
	m_hSaveNewCurWnd = NULL;
	m_hSaveOldCurWnd = NULL;
	m_byCurWndTag = 0;
	m_wTitlePos=0;

	m_memdc.m_hDC = NULL;
	m_pActivity = NULL;

	//m_bIsDrawBack = false;
	m_bIsSound = false;
	m_bIsUpdateUserInfo=false; //�Ƿ��ǵ�������û���Ϣ
	m_bIsGetGlod=false;//�����ȡ���
	m_bCheckLine=false;
	m_bDeleteMachineCode=false;
	/*1024
	for (int i=0; i<4; ++i)
	{
	m_TitleButtonList[i].BTImage.DestroyImage();
	m_TitleButtonList[i].rcRect.SetRect(0, 0, 0, 0);
	m_TitleButtonList[i].btStatus = 0;
	}
	*/
	m_iGradeInfoDiv = 0;
	m_bNewComerGuide=false;
	m_wNewComerGuideActionNumber=1;
	/*cxf m_bIsFlashMsg = false;
	m_dwGameMsgIndex = 0;
	m_byFlashMsgStatus = 0;*/

	m_bIsGameSoftUpdate = false;

   memset(m_hEventGameLoadOver, 0, sizeof(m_hEventGameLoadOver));
	m_strGameLoadOver = "SISEQIU_GAME_CLOSE";	// ע�⣺��ɫ����Ϸ�е����ֵҪ��˴�һ��
	m_hEventGameLoadOver[0] = CreateEvent(NULL, FALSE, FALSE, m_strGameLoadOver);

	m_pNewHandTask = NULL;

	m_dwLastConnectTime = 0l;
	m_dwConnectIntervalTime = 3l;

	CString strModuleFileName("");
	GetCurrentDirectoryPath(strModuleFileName);
	strModuleFileName.Append("\\config\\config.ini");
	m_dwConnectIntervalTime = GetPrivateProfileInt(_T("HallSet"), _T("RoomConnectIntervalTime"), 3, strModuleFileName);
	if (1 > m_dwConnectIntervalTime  ||  5 <m_dwConnectIntervalTime)
		m_dwConnectIntervalTime = 3;
}

CPlazaDlg::~CPlazaDlg()
{
	m_fontNickName.DeleteObject();
	m_fontXPValue.DeleteObject();
	m_fontDrawXJ.DeleteObject();

	SafeDelete(m_pRoomViewItemCreate);
	m_pRoomViewItemCreate = NULL;

	if (NULL != m_hUser32Instance)
	{
		FreeLibrary(m_hUser32Instance); 
	}
	m_hUser32Instance = NULL;
	m_pFunShowLayer = NULL;

	m_memdc.DeleteDC();
	//�ȼ���Ϣ
	if (NULL != m_pGradeInfo)
	{
		SafeDelete(m_pGradeInfo);
		m_pGradeInfo = NULL;
	}
	m_pGradeInfo = NULL;

	if ( m_CustomFace.pFaceData != NULL ) 
	{
		SafeDeleteArray(m_CustomFace.pFaceData);
	}

	if (NULL != m_pActivity)
	{
		SafeDelete(m_pActivity);
		m_pActivity = NULL;
	}
}

void CPlazaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	// 	DDX_Control(pDX,IDC_BT_SHOP,m_btnGameShop);
	// 	DDX_Control(pDX,IDC_BT_BEAUTY_SALON,m_btnBeautySalon);
	DDX_Control(pDX,IDC_BT_RE_LOGON,m_btnReLogon);
	DDX_Control(pDX,IDC_BT_RETURN,m_btnReturn);
	DDX_Control(pDX,IDC_BT_SET,m_btnSound);
	DDX_Control(pDX,IDCANCEL,m_btnClose);  
	DDX_Control(pDX,IDC_MINSIZE,m_btnMinsize); 
	DDX_Control(pDX,IDC_VISIT_WEB,m_btnVisitWeb);
	DDX_Control(pDX,IDC_ACCOUNTS_MANAGE,m_btnAccountsManage);
	DDX_Control(pDX,IDC_GAME_HELP,m_btnGameHelp);
	DDX_Control(pDX,IDC_QUESTION_FEEDBACK,m_btnQuestionFeedBack);
	DDX_Control(pDX,IDC_CONTACT_SERVISE,m_btnContactServise);
	DDX_Control(pDX,IDC_GAME_LIST,m_btnGameList);
	DDX_Control(pDX,IDC_TOUNAMENT,m_btnTounament);
	DDX_Control(pDX,IDC_ACCOUNTS_RECHARGE,m_btnAccountsRecharge);
	DDX_Control(pDX,IDC_PRIZE_EXCHARGE,m_btnPrizeExcharge);
	DDX_Control(pDX,IDC_PROPS_SHOP,m_btnPropsShop);
	DDX_Control(pDX,IDC_NOVICE_TASK,m_btnNoviceTask);
	DDX_Control(pDX, IDC_BACK_PACK, m_btnBackpack);
	DDX_Control(pDX, IDC_USER_INFO, m_btnUserInfo);
	DDX_Control(pDX, IDC_BANK, m_btnBank);
	DDX_Control(pDX, IDC_EMAIL_MESSAGE, m_btnEmailMessage);
	DDX_Control(pDX, IDC_PLAZA_ROLLBACK, m_btnRollback);
	DDX_Control(pDX, IDC_PLAZA_NEXT_PAGE, m_btnNextPage);
	DDX_Control(pDX, IDC_UPDATE_USERINFO, m_btnUpdateUserInfo);
	DDX_Control(pDX, IDC_GUIDE, m_btnNewComerGuide);
}


BEGIN_MESSAGE_MAP(CPlazaDlg, CDialog) 
	ON_COMMAND(IDM_CONNECT_SERVER,	OnCommandConnect)
	ON_COMMAND(IDM_CANCEL_CONNECT,	OnCommandCancelConnect)
	ON_COMMAND(IDM_LOGON_PLAZA,		OnCommandLogon)
	ON_COMMAND(IDM_CALL_ROOM_LIST,	OnCallRoomList)
	ON_COMMAND(IDM_CALL_GAME_LIST,	OnCallGameList)
	ON_COMMAND(IDM_CALL_TABLE_LIST,	OnCallTableList)
	ON_COMMAND(IDM_CALL_EXIT_GAME,  OnBnExitLogoUI)
	ON_COMMAND(IDM_CALL_CANCEL_GAME,  OnBnCancelGame)
	ON_COMMAND(IDB_CALL_UPDATE_USERIMAGE, OnCannUpdateUserImage)

	ON_WM_SYSCOMMAND()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//	ON_WM_WINDOWPOSCHANGED()
	ON_BN_CLICKED(IDC_BT_RETURN,&CPlazaDlg::OnBnClickedReturn)
	ON_BN_CLICKED(IDC_BT_SET,&CPlazaDlg::OnBnClickedSet)
	ON_BN_CLICKED(IDC_MINSIZE,&CPlazaDlg::OnBnClickedMinsize) 
	ON_BN_CLICKED(IDCANCEL,&CPlazaDlg::ConfirmClose)
	ON_BN_CLICKED(IDC_BT_RE_LOGON,&CPlazaDlg::OnBnClickedReLogon)
	ON_BN_CLICKED(IDC_VISIT_WEB,&CPlazaDlg::OnBnClickedVisitWeb)
	ON_BN_CLICKED(IDC_ACCOUNTS_MANAGE,&CPlazaDlg::OnBnClickedAccountsManage)
	ON_BN_CLICKED(IDC_GAME_HELP,&CPlazaDlg::OnBnClickedGameHelp)
	ON_BN_CLICKED(IDC_QUESTION_FEEDBACK,&CPlazaDlg::OnBnClickedQuestionFeedBack)
	ON_BN_CLICKED(IDC_CONTACT_SERVISE,&CPlazaDlg::OnBnClickedContactServise)
	ON_BN_CLICKED(IDC_BACK_PACK,&CPlazaDlg::OnBnClickedBackPack)
	ON_BN_CLICKED(IDC_USER_INFO,&CPlazaDlg::OnBnClickedUserInfo)
	ON_BN_CLICKED(IDC_BANK,&CPlazaDlg::OnBnClickedBank)
	ON_BN_CLICKED(IDC_EMAIL_MESSAGE,&CPlazaDlg::OnBnClickedEmailMessage)

	ON_BN_CLICKED(IDC_GAME_LIST,&CPlazaDlg::OnBnClickedGameList)
	ON_BN_CLICKED(IDC_TOUNAMENT,&CPlazaDlg::OnBnClickedTounament)
	ON_BN_CLICKED(IDC_ACCOUNTS_RECHARGE,&CPlazaDlg::OnBnClickedAccountsRecharge)
	ON_BN_CLICKED(IDC_PRIZE_EXCHARGE,&CPlazaDlg::OnBnClickedPrizeExcharge)
	ON_BN_CLICKED(IDC_PROPS_SHOP,&CPlazaDlg::OnBnClickedPropsShop)
	ON_BN_CLICKED(IDC_NOVICE_TASK,&CPlazaDlg::OnBnClickedNoviceTask)

	ON_BN_CLICKED(IDC_PLAZA_ROLLBACK,&CPlazaDlg::OnBnClickedRollBack)
	ON_BN_CLICKED(IDC_PLAZA_NEXT_PAGE,&CPlazaDlg::OnBnClickedNextPage)
	ON_BN_CLICKED(IDC_UPDATE_USERINFO,&CPlazaDlg::OnBnClickedUpdateUserInfo)
	ON_BN_CLICKED(IDC_GUIDE, &CPlazaDlg::OnBnClickedNewComerGuide)
	// 	ON_BN_CLICKED(IDC_BT_BEAUTY_SALON,&CPlazaDlg::OnBnClickedBeautySalon)
	// 	ON_BN_CLICKED(IDC_BT_SHOP,&CPlazaDlg::OnBnClickedGameShop)
	//ON_BN_CLICKED(IDB_BT_DEPOSITORY,&CPlazaDlg::OnBnClickedDepository)
	//ON_BN_CLICKED(IDB_BT_SHOP,&CPlazaDlg::OnBnClickedShop)
	//ON_BN_CLICKED(IDB_BT_TASK,&CPlazaDlg::OnBnClickedTask)
	//ON_BN_CLICKED(IDC_BT_RE_GAME_PLAZY, &CPlazaDlg::OnBnClickedGamePlazy)

	ON_MESSAGE(INNO_ISS_MSG_CLOSE,	OnCallInnoIssMsgClose)
	ON_MESSAGE(INNO_ISS_MSG_SELECT_GAME, OnCallInnoIssMsgSelectGame)
	ON_MESSAGE(IDM_CALL_SHOW_CUSTOMER, OnCallShowCustomer)
	ON_MESSAGE(IDM_CHECK_LINE, OnCallCheckConnect)

END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CPlazaDlg, CDialog)
	//DISP_FUNCTION_ID(CPlazaDlg, "oncontextmenu", DISPID_HTMLDOCUMENTEVENTS2_ONCONTEXTMENU, onHtmlContextMenu, VT_BOOL, VTS_DISPATCH)
END_DISPATCH_MAP()

BEGIN_EVENTSINK_MAP(CPlazaDlg, CDialog)
	ON_EVENT(CPlazaDlg, 23, 259, OnDocumentCompleteExplorer3, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

BOOL CPlazaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//cxf
	int width   =   GetSystemMetrics(SM_CXSCREEN); 
	int height   =   GetSystemMetrics(SM_CYSCREEN);

	SetWindowPos(NULL, (width-960)/2, (height-720)/2, 960, 720, 0);
	CRect rtClientRect;
	GetClientRect(&rtClientRect);
	//end
	m_hUser32Instance = NULL;
	m_pFunShowLayer = NULL;
	m_uiCurAlphaValue = 0;
	m_hSaveNewCurWnd = NULL;
	m_hSaveOldCurWnd = NULL;
	m_byCurWndTag = 0;
	m_bIsUpdateUserInfo=false;//�Ƿ��ǵ�������û���Ϣ
	m_bIsGetGlod=false;//�����ȡ���
	m_bCheckLine=false;
	m_bDeleteMachineCode=false;
	m_bNewComerGuide=false;
	m_wNewComerGuideActionNumber=1;

	// ��ӿ�
	m_hUser32Instance = LoadLibrary("User32.DLL");
	if(m_hUser32Instance)
	{
		// ��ȡ����ָ��
		m_pFunShowLayer = (ShowLayer)GetProcAddress(m_hUser32Instance, "SetLayeredWindowAttributes");
	}

	//��������
	BOOL bIsRet = m_fontNickName.CreateFont(20,0,0,0,200,0,0,0,134,3,2,1,2,TEXT("΢���ź�"));
	if (!bIsRet)
	{
		m_fontNickName.CreateFont(12,0,0,0,200,0,0,0,134,3,2,1,2,TEXT("����"));
	}
	bIsRet = m_fontXPValue.CreateFont(16,0,0,0,200,0,0,0,134,3,2,1,2,TEXT("΢���ź�"));
	if (!bIsRet)
	{
		m_fontXPValue.CreateFont(12,0,0,0,200,0,0,0,134,3,2,1,2,TEXT("����"));
	}
	m_fontDrawXJ.CreateFont(12,0,0,0,200,0,0,0,134,3,2,1,2,TEXT("����"));

	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);

	m_ServerItemView.InitServerItemView(/*this*/);
	g_GlobalUnits.m_ServerListManager.InitServerListManager(&m_ServerItemView);

	//��ʼ������ 	
	//cxf m_pDlgLogon->Create(IDD_LOGON, this);
	//cxf m_gameListDlg.Create(IDD_GAME_LIST, this);
	//cxf 1022 m_roomListDlg.Create(IDD_ROOM_LIST, this);
	//m_friendlistDlg.Create(IDD_FRIEND_LIST);
	//cxf 1022 m_TaskDlg.Create(CTaskDialog::IDD, this);
	//cxf 1022 m_shopDlg.Create(CShopDialog::IDD, this);
	//cxf 1022 m_doteyBoxDlg.Create(CDoteyBoxDialog::IDD, this);
	m_GameListHot.Create(CGameListHot::IDD, this);
	m_PlazaGameWeb.Create(CPlazaGameWeb::IDD, this);
	m_GameListCenterDlg.Create(IDD_GAME_LIST_CENTER,this);
	m_RoomListCenterDlg.Create(CRoomListCenterDlg::IDD,this);
	/*cxf for (int i=0; i<4; ++i)
	{
	m_TitleButtonList[i].BTImage.DestroyImage();
	m_TitleButtonList[i].rcRect.SetRect(0, 0, 0, 0);
	m_TitleButtonList[i].btStatus = 0;
	}*/

	//��ʼ��������Ϣ
	InitDlgBK();
	SetWindowText("�ر���");

	m_UserXJRect.left = 755;
	m_UserXJRect.top = 397;
	m_UserXJRect.right = m_UserXJRect.left + 60;
	m_UserXJRect.bottom = m_UserXJRect.top + 14;
	m_UserXJInfoRect.left = 753;
	m_UserXJInfoRect.top = 315;
	m_UserXJInfoRect.right = m_UserXJInfoRect.left + 114;
	m_UserXJInfoRect.bottom = m_UserXJInfoRect.top + 77;

	m_rcTitleArea.left=428;
	m_rcTitleArea.top=148;
	m_rcTitleArea.right=m_rcTitleArea.left+84;
	m_rcTitleArea.bottom=m_rcTitleArea.top+22;

	//����ָ������
	m_rcNewComerGuideArea.left = 705;
	m_rcNewComerGuideArea.top = 30;
	m_rcNewComerGuideArea.right = m_rcNewComerGuideArea.left + 94;
	m_rcNewComerGuideArea.bottom = m_rcNewComerGuideArea.top + 59;

	//cxf 
	//��ȡ��������
	UINT uiReturn = GetCommonConfig("Sound","EnablePlazeBGMusic",1);

	//��ȡ��ǰ·��
	m_strCurrentPathPlaza="";
	GetResPath("\\data\\plaza\\",m_strCurrentPathPlaza);

	if (1==uiReturn)
	{//��������
		m_bIsSound=true;
		m_btnSound.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTSoundOpen.png"),false);
	}else
	{//�ر�����
		m_bIsSound=false;
		m_btnSound.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTSoundClose.png"),false);
	}
	//��ť
	m_btnClose.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTClose.png"),false);
	m_btnMinsize.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTMinsize.png"),false);
	m_btnVisitWeb.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTVisitWeb.png"),false);
	m_btnAccountsManage.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTAccountsManage.png"),false);
	m_btnGameHelp.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTGameHelp.png"),false);
	m_btnQuestionFeedBack.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTQuestionFeedBack.png"),false);
	m_btnContactServise.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTContactServise.png"),false);
	m_btnGameList.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTGameList.png"),true);
	m_btnTounament.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTTounament.png"),true);
	m_btnAccountsRecharge.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTAccountsRecharge.png"),true);
	m_btnPrizeExcharge.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTPrizeExcharge.png"),true);
	m_btnPropsShop.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTPropsShop.png"),true);
	m_btnNoviceTask.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTNoviceTask.png"),true);
	m_btnBackpack.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTBackPack.png"),false);
	m_btnUserInfo.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTUserInfo.png"),false);
	m_btnBank.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTBank.png"),false);
	m_btnEmailMessage.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTEmailMessage.png"),false);
	m_btnRollback.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTRollBack.png"),false);
	m_btnNextPage.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTNextPage.png"),false);
	m_btnUpdateUserInfo.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTUpdateUserInfo.png"), false);
	m_btnNewComerGuide.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTGuide.png"), false);
	//��ͼ
	m_ImageTitle.LoadImage(TEXT(m_strCurrentPathPlaza+"ImageTitle.png"));
	m_ImageNewComerGuide.LoadImage(TEXT(m_strCurrentPathPlaza+"ImageNewComerGuide.png"));//����ָ��

	/*cxf HINSTANCE hInstance=g_GlobalUnits.m_PlatformResourceModule->GetResInstance();	
	//	m_btnGameShop.LoadRgnImage(hInstance,g_GlobalUnits.m_PlatformFrameImage.uFrameNavigation[3]);
	// 	m_btnBeautySalon.LoadRgnImage(hInstance,g_GlobalUnits.m_PlatformFrameImage.uFrameNavigation[7]);
	m_btnReLogon.LoadRgnImage(hInstance,g_GlobalUnits.m_PlatformFrameImage.uBtFrameRelogon);
	m_btnReturn.LoadRgnImage(hInstance,g_GlobalUnits.m_PlatformFrameImage.uBtFrameRetrue);
	m_btnSound.LoadRgnImage(hInstance,g_GlobalUnits.m_PlatformFrameImage.uBtFrameOption);
	m_btnClose.LoadRgnImage(hInstance,g_GlobalUnits.m_PlatformFrameImage.uBtFrameClose);
	m_btnMinsize.LoadRgnImage(hInstance,g_GlobalUnits.m_PlatformFrameImage.uBtFrameMin);

	m_ToolTipCtrl.AddTool(&m_btnReLogon,TEXT("���µ�½"));
	m_ToolTipCtrl.AddTool(&m_btnReturn,TEXT("����"));
	*/
	//��ʾ
	m_ToolTipCtrl.AddTool(&m_btnSound,TEXT("���ű�������"));
	m_ToolTipCtrl.AddTool(&m_btnClose,TEXT("�ر���Ϸ����"));
	m_ToolTipCtrl.AddTool(&m_btnMinsize,TEXT("��С������"));
	m_ToolTipCtrl.AddTool(&m_btnUserInfo,TEXT("�����Ϣ"));
	m_ToolTipCtrl.AddTool(&m_btnBank,TEXT("����"));
	m_ToolTipCtrl.AddTool(&m_btnEmailMessage,TEXT("ϵͳ��Ϣ"));
	m_ToolTipCtrl.AddTool(&m_btnBackpack,TEXT("����"));
	//��ҳ
	CString strPath("");
	GetResPath("\\Url\\plaza\\",strPath);
	m_webPlazaAdvertisement.Create(NULL,NULL,WS_VISIBLE | WS_CHILD,CRect(0,0,0,0),this,IDI_WEB_PLAZA_ADVERTISEMENT,NULL);
	m_webPlazaNotice.Create(NULL,NULL,WS_VISIBLE | WS_CHILD,CRect(0,0,0,0),this,IDI_WEB_PLAZA_NOTICE,NULL);
	m_webPlazaMessage.Create(NULL,NULL,WS_VISIBLE | WS_CHILD,CRect(0,0,0,0),this,IDI_WEB_PLAZA_MESSAGE,NULL);
	m_webPlazaTitle.Create(NULL,NULL,WS_VISIBLE | WS_CHILD,CRect(0,0,0,0),this,IDI_WEB_PLAZA_ADVERTISEMENT,NULL);
	m_webPlazaAdvertisement.Navigate(TEXT(strPath+"webPlazaAdvertisement.html"),NULL,NULL,NULL,NULL);
	m_webPlazaNotice.Navigate(TEXT(strPath+"webPlazaNotice.html"),NULL,NULL,NULL,NULL);
	m_webPlazaMessage.Navigate(TEXT(strPath+"webPlazaMessage.html"),NULL,NULL,NULL,NULL);
	m_webPlazaTitle.Navigate(TEXT(strPath+"webPlazaTitle.html"),NULL,NULL,NULL,NULL);
	//end

	m_GameFlash.Create(IDD_STATUS, this);
	m_GameFlash.InitFlash("data\\status\\BKStatus.png", 12, CPoint(560/2, 320/2), 100);

	//������Ļ
	LockWindowUpdate();

	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	//cxf DeferWindowPos(hDwp,m_btnReLogon,NULL,800,13,0,0,uFlags);
	//cxf DeferWindowPos(hDwp,m_btnReturn,NULL,826,13,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnSound,NULL,786,5,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnMinsize,NULL,838,5,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnClose,NULL,888,5,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnVisitWeb,NULL,350,7,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnAccountsManage,NULL,427,7,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnGameHelp,NULL,504,7,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnNewComerGuide,NULL,581,7,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnQuestionFeedBack,NULL,656,7,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnContactServise,NULL,rtClientRect.Width()-119,58,0,0,uFlags);

	DeferWindowPos(hDwp,m_btnGameList,NULL,160,90,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnTounament,NULL,292,90,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnAccountsRecharge,NULL,384,90,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnPrizeExcharge,NULL,476,90,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnPropsShop,NULL,568,90,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnNoviceTask,NULL,660,90,0,0,uFlags);

	DeferWindowPos(hDwp,m_btnUserInfo,NULL,758,512,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnEmailMessage,NULL,805,512,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnBank,NULL,852,512,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnBackpack,NULL,899,512,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnRollback,NULL,217,153,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnNextPage,NULL,701,153,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnUpdateUserInfo,NULL,909,392,0,0,uFlags);

	m_webPlazaMessage.MoveWindow(211,538,536,21);
	m_webPlazaAdvertisement.MoveWindow(15,578,727,128);
	m_webPlazaNotice.MoveWindow(761,605,185,103);
	m_webPlazaTitle.MoveWindow(204,32,480,58);

	EndDeferWindowPos(hDwp);
	ResizeFrame();
	//�������
	UnlockWindowUpdate();

	//cxf 1022 m_TaskDlg.ShowWindow(SW_HIDE);
	//cxf 1022 m_shopDlg.ShowWindow(SW_HIDE);
	//cxf 1022 m_doteyBoxDlg.ShowWindow(SW_HIDE);
	//cxf m_gameListDlg.ShowWindow(SW_HIDE);
	//cxf 1022 m_roomListDlg.ShowWindow(SW_HIDE);
	m_btnReturn.ShowWindow(SW_HIDE);
	m_btnReLogon.ShowWindow(SW_HIDE);
	m_GameListHot.ShowWindow(SW_SHOW);
	m_PlazaGameWeb.ShowWindow(SW_HIDE);
	m_btnSound.ShowWindow(SW_SHOW);
	m_btnClose.ShowWindow(SW_SHOW);
	m_btnMinsize.ShowWindow(SW_SHOW);
	m_btnVisitWeb.ShowWindow(SW_SHOW);
	m_btnAccountsManage.ShowWindow(SW_SHOW);
	m_btnGameHelp.ShowWindow(SW_SHOW);
	m_btnQuestionFeedBack.ShowWindow(SW_SHOW);
	m_btnContactServise.ShowWindow(SW_SHOW);
	m_btnBackpack.ShowWindow(SW_SHOW);
	m_btnUserInfo.ShowWindow(SW_SHOW);
	m_btnBank.ShowWindow(SW_SHOW);
	m_btnEmailMessage.ShowWindow(SW_SHOW);
	m_GameListCenterDlg.ShowWindow(SW_SHOW);
	m_RoomListCenterDlg.ShowWindow(SW_HIDE);
	m_btnRollback.ShowWindow(SW_HIDE);
	m_btnNextPage.ShowWindow(SW_HIDE);
	m_btnUpdateUserInfo.ShowWindow(SW_SHOW);
	m_btnNewComerGuide.ShowWindow(SW_SHOW);

	//cxf 1024 m_TitleButtonList[0].btStatus = 3;
	SetTimer(TIMER_WEB_LOADING_PLAZA,20,NULL);
	//cxf ShowTab(0);

	// �����·
	GetResPath("\\config\\",strPath);
	m_IpFinder.Open(strPath + _T("ipwry.dat"));
	return TRUE;
}

void CPlazaDlg::ResizeFrame()
{
	HDWP hDwp=BeginDeferWindowPos(32);
	//cxf if (::IsWindow(m_gameListDlg.GetSafeHwnd()))
	//{
	//	DeferWindowPos(hDwp, m_gameListDlg.GetSafeHwnd(), NULL, 287, 81, 659, 626, SWP_NOREDRAW|SWP_NOZORDER);
	//	//::MoveWindow(m_gameListDlg.GetSafeHwnd(), wndrect.left+287,wndrect.top+81, 659, 626, FALSE);
	//	//m_gameListDlg.SetWindowPos(NULL, wndrect.left+287,wndrect.top+81, 659, 626, SWP_NOSENDCHANGING | SWP_NOREDRAW);
	//}
	////cxf 1022  if (::IsWindow(m_roomListDlg.GetSafeHwnd()))
	//{
	//	DeferWindowPos(hDwp, m_roomListDlg.GetSafeHwnd(), NULL, 287, 81, 659, 626, SWP_NOREDRAW|SWP_NOZORDER);
	//	//::MoveWindow(m_roomListDlg.GetSafeHwnd(), wndrect.left+287,wndrect.top+81, 659, 626, FALSE);
	//	//m_roomListDlg.SetWindowPos(NULL, wndrect.left+287,wndrect.top+81, 659, 626, SWP_NOSENDCHANGING | SWP_NOREDRAW);
	//}
	if (m_pRoomViewItemCreate && ::IsWindow(m_pRoomViewItemCreate->GetSafeHwnd()))
	{
		DeferWindowPos(hDwp, m_pRoomViewItemCreate->GetSafeHwnd(), NULL, 0, 0, 960, 720, SWP_NOREDRAW|SWP_NOZORDER);
		//cxf 1022 DeferWindowPos(hDwp, m_pRoomViewItemCreate->GetSafeHwnd(), NULL, 287, 81, 659, 626, SWP_NOREDRAW|SWP_NOZORDER);
		//::MoveWindow(m_pRoomViewItemCreate->GetSafeHwnd(),wndrect.left+287,wndrect.top+81, 659, 626, FALSE);
		//m_pRoomViewItemCreate->SetWindowPos(NULL, wndrect.left+287,wndrect.top+81, 659, 626, SWP_NOSENDCHANGING | SWP_NOREDRAW);
	}
	////cxf 1022 if (::IsWindow(m_shopDlg.GetSafeHwnd()))
	//{
	//	DeferWindowPos(hDwp, m_shopDlg.GetSafeHwnd(), NULL, 287, 81, 659, 626, SWP_NOREDRAW|SWP_NOZORDER);
	//	//::MoveWindow(m_shopDlg.GetSafeHwnd(), wndrect.left+287,wndrect.top+81, 659, 626, FALSE);
	//	//m_shopDlg.SetWindowPos(NULL, wndrect.left+287,wndrect.top+81, 659, 626, SWP_NOSENDCHANGING | SWP_NOREDRAW);
	//}
	////cxf 1022 if (::IsWindow(m_doteyBoxDlg.GetSafeHwnd()))
	//{
	//	DeferWindowPos(hDwp, m_doteyBoxDlg.GetSafeHwnd(), NULL, 287, 81, 659, 626, SWP_NOREDRAW|SWP_NOZORDER);
	//	//::MoveWindow(m_doteyBoxDlg.GetSafeHwnd(), wndrect.left+287,wndrect.top+81, 659, 626, FALSE);
	//	//m_doteyBoxDlg.SetWindowPos(NULL, wndrect.left+287,wndrect.top+81, 659, 626, SWP_NOSENDCHANGING | SWP_NOREDRAW);
	//}
	////cxf 1022 if (::IsWindow(m_TaskDlg.GetSafeHwnd()))
	//{
	//	DeferWindowPos(hDwp, m_TaskDlg.GetSafeHwnd(), NULL, 287, 81, 659, 626, SWP_NOREDRAW|SWP_NOZORDER);
	//	//::MoveWindow(m_TaskDlg.GetSafeHwnd(), wndrect.left+287,wndrect.top+81, 659, 626, FALSE);
	//	//m_TaskDlg.SetWindowPos(NULL, wndrect.left+287,wndrect.top+81, 659, 626, SWP_NOSENDCHANGING | SWP_NOREDRAW);
	//}
	if (::IsWindow(m_GameListHot.GetSafeHwnd()))
	{
		DeferWindowPos(hDwp, m_GameListHot.GetSafeHwnd(), NULL, 15, 177, 189, 382, SWP_NOREDRAW|SWP_NOZORDER);
	}
	//�����м���ҳ
	if (::IsWindow(m_PlazaGameWeb.GetSafeHwnd()))
	{
		DeferWindowPos(hDwp, m_PlazaGameWeb.GetSafeHwnd(), NULL, 211, 177, 536, 361, SWP_NOREDRAW|SWP_NOZORDER);
	}
	//����Ϸ�б�
	if (::IsWindow(m_GameListCenterDlg.GetSafeHwnd()))
	{
		DeferWindowPos(hDwp, m_GameListCenterDlg.GetSafeHwnd(), NULL, 211, 177, 536, 361, SWP_NOREDRAW|SWP_NOZORDER);
	}
	//�����б�
	if (::IsWindow(m_RoomListCenterDlg.GetSafeHwnd()))
	{
		DeferWindowPos(hDwp, m_RoomListCenterDlg.GetSafeHwnd(), NULL, 211, 177, 536, 361, SWP_NOREDRAW|SWP_NOZORDER);
	}
	EndDeferWindowPos(hDwp);
}


//��ʼ��ͼƬ��Ϣ
void CPlazaDlg::InitDlgBK()
{
	HINSTANCE hInstance = AfxGetInstanceHandle();

	//cxf
	//װ��ͼƬ
	CString strPath("");
	GetResPath("\\data\\plaza\\",strPath);
	m_ImageBack.LoadImage(TEXT(strPath+"BKPlaza.png"));
	m_Exp2Image.LoadImage(TEXT(strPath+"experience.png"));//ǰ��������
	m_XJInfoBackImage.LoadImage(TEXT(strPath+"BKGrade.png"));
	//m_ImageBack.SetLoadInfo(IDB_PLAZA_BG, hInstance, false);
	//m_ImageNums.SetImageResource("PNG_NUM_IMAGE");

	//װ������ͼƬ
	CString szPath("");
	GetResPath("\\data\\",szPath);
	szPath.Append(TEXT("Number_Hei.png"));
	char* chData = szPath.GetBuffer();
	m_ImageNums.SetImageResource(chData);

	m_ImageNums.SetSeparator(false);
	m_ImageNums.SetSign(false);
	m_ImageNums.SetNumAlign(IA_CENTER);
	m_ImageNums.SetSeparatorWH(7, 14);

	//װ������ͼƬ
	szPath="";
	GetResPath("\\data\\",szPath);
	szPath.Append(TEXT("Number_Cai.png"));
	chData = szPath.GetBuffer();
	m_ImageNumsTel.SetImageResource(chData);

	//m_PngHat.LoadImage(hInstance, TEXT("PNG_IMAGE_HAT"));
	//m_UserInfo.LoadImage(hInstance, TEXT("PNG_USER_INFO_BACK_IMAGE"));

	//m_Exp1Image.LoadImage(hInstance, TEXT("PNG_EXPERIENCEL_IMAGE1"));//����������
	//m_Exp2Image.LoadImage(hInstance, TEXT("PNG_EXPERIENCEL_IMAGE2"));//ǰ��������
	//end

	/*cxf
	m_TitleButtonList[0].BTImage.LoadImage(hInstance, TEXT("BT_PNG_IMAGE_GAMEPLAZY"));
	m_TitleButtonList[1].BTImage.LoadImage(hInstance, TEXT("BT_PNG_IMAGE_TASK"));
	m_TitleButtonList[2].BTImage.LoadImage(hInstance, TEXT("BT_PNG_IMAGE_CBG"));
	m_TitleButtonList[3].BTImage.DestroyImage();
	m_TitleButtonList[3].BTImage.LoadImage(hInstance, TEXT("BT_PNG_IMAGE_SHOP"));

	m_TitleButtonList[0].rcRect.left = 327;
	m_TitleButtonList[0].rcRect.top = 24;
	m_TitleButtonList[0].rcRect.right = m_TitleButtonList[0].rcRect.left + m_TitleButtonList[0].BTImage.GetWidth()/4;
	m_TitleButtonList[0].rcRect.bottom = m_TitleButtonList[0].rcRect.top + m_TitleButtonList[0].BTImage.GetHeight();

	m_TitleButtonList[1].rcRect.left = 439;
	m_TitleButtonList[1].rcRect.top = 24;
	m_TitleButtonList[1].rcRect.right = m_TitleButtonList[1].rcRect.left + m_TitleButtonList[1].BTImage.GetWidth()/4;
	m_TitleButtonList[1].rcRect.bottom = m_TitleButtonList[1].rcRect.top + m_TitleButtonList[1].BTImage.GetHeight();

	m_TitleButtonList[2].rcRect.left = 551;
	m_TitleButtonList[2].rcRect.top = 24;
	m_TitleButtonList[2].rcRect.right = m_TitleButtonList[2].rcRect.left + m_TitleButtonList[2].BTImage.GetWidth()/4;
	m_TitleButtonList[2].rcRect.bottom = m_TitleButtonList[2].rcRect.top + m_TitleButtonList[2].BTImage.GetHeight();

	m_TitleButtonList[3].rcRect.left = 663;
	m_TitleButtonList[3].rcRect.top = 24;
	m_TitleButtonList[3].rcRect.right = m_TitleButtonList[3].rcRect.left + m_TitleButtonList[3].BTImage.GetWidth()/4;
	m_TitleButtonList[3].rcRect.bottom = m_TitleButtonList[3].rcRect.top + m_TitleButtonList[3].BTImage.GetHeight();*/


	//��Ϣ
	/*cxf m_byFlashMsgStatus = 0;
	m_bIsFlashMsg = false;
	m_GameMsgImage.LoadImage(hInstance, TEXT("PNG_IMAGE_GAME_MSG"));
	m_rcGameMsgArea.SetRect(0, 0, 0, 0);
	if (!m_GameMsgImage.IsNull())
	{
	m_rcGameMsgArea.left = 581;
	m_rcGameMsgArea.top = 460;
	m_rcGameMsgArea.right = m_rcGameMsgArea.left + m_GameMsgImage.GetWidth();
	m_rcGameMsgArea.bottom = m_rcGameMsgArea.top + m_GameMsgImage.GetHeight();
	}*/
}


//���ӷ�����
void CPlazaDlg::OnCommandConnect()
{
	//�������
	if (m_ClientSocket.GetInterface()==NULL)
	{
		try
		{
			IUnknownEx * pIUnknownEx=(IUnknownEx *)QueryInterface(IID_IUnknownEx,VER_IUnknownEx);
			if (m_ClientSocket.CreateInstance()==false) 
			{
				throw TEXT("�����������ʧ��");
			}
			if (m_ClientSocket->SetTCPSocketSink(pIUnknownEx)==false) 
			{
				throw TEXT("��������ص��ӿ�����ʧ��");
			}
		}
		catch (...)
		{
			ShowMessageBox(TEXT("�����������ʧ�ܣ�������������Ϸ������"),MB_ICONSTOP);
			PostMessage(WM_COMMAND,IDM_LOGON_PLAZA,0);
			return;
		}
	}

	//��ַ����
	CRegKey RegServer;
	LPCTSTR pszServerIP=m_pDlgLogon->GetLogonServer();
	TCHAR szRegServer[256]=TEXT(""),szServerAddr[64]=TEXT("");
	_snprintf(szRegServer,sizeof(szRegServer),TEXT("%s\\%s"),REG_LOGON_SERVER,pszServerIP);

	if (RegServer.Open(HKEY_CURRENT_USER,szRegServer,KEY_READ)==ERROR_SUCCESS)
	{
		TCHAR szReadData[1024]=TEXT("");
		DWORD dwReadData=0L,dwDataType=0L,dwDataSize=sizeof(szReadData);
		LONG lErrorCode=RegServer.QueryValue(TEXT("ServerAddr"),&dwDataType,szReadData,&dwDataSize);
		if (lErrorCode==ERROR_SUCCESS) 
		{
			CXOREncrypt::CrevasseData(szReadData,szServerAddr,sizeof(szServerAddr));
			pszServerIP=szServerAddr;
		}
	}

	//���ӷ�����
	try
	{
		//���ӷ�����
		m_ClientSocket->CloseSocket();

		//�����ж�
		if ( m_pDlgLogon->EnableProxy() == true )
		{
			//��ȡ����
			enProxyServerType ProxyServerType;
			tagProxyServerInfo ProxyServerInfo;
			m_pDlgLogon->GetProxyInfo(ProxyServerType, ProxyServerInfo);	

			//���ô���
			m_ClientSocket->SetProxyServerInfo(ProxyServerType,ProxyServerInfo);
		}

		if (m_ClientSocket->Connect(pszServerIP,PORT_LOGON_SERVER)!=CONNECT_SUCCESS)
		{
			throw TEXT("���������Ӵ��󣬿���������ϵͳ��û�гɹ����������磡");
		}
	}
	catch (LPCTSTR pszError)
	{
		ShowMessageBox(pszError,MB_ICONINFORMATION);
		PostMessage(WM_COMMAND,IDM_LOGON_PLAZA,0);
		return;
	}

	//��ʾ��ʾ��
	/*cxf 1116 CString strMessage=TEXT("�������ӷ����������Ժ�...");
	g_GlobalAttemper.ShowStatusMessage(strMessage,this);*/
	if (!m_bIsUpdateUserInfo  &&  !m_bCheckLine  && !m_bDeleteMachineCode)
	{
		m_GameFlash.Play(true, 0, 0);
	}

	return;
}
//ȡ������
void CPlazaDlg::OnCommandCancelConnect()
{
	g_GlobalAttemper.DestroyStatusWnd(this);
	m_ClientSocket->CloseSocket();
	SendLogonMessage();
	return;
}

//�ӿڲ�ѯ
void * __cdecl CPlazaDlg::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IViewItem,Guid,dwQueryVer);
	QUERYINTERFACE(ITCPSocketSink,Guid,dwQueryVer);
	QUERYINTERFACE(IPlazaViewItem,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IViewItem,Guid,dwQueryVer);
	return NULL;
}


int CPlazaDlg::ShowMessageBox(LPCTSTR pszMessage, UINT nType)
{			
	//�ر���ʾ
	g_GlobalAttemper.DestroyStatusWnd(this);
	m_GameFlash.Play(false);
	int nResult=ShowInformationEx(pszMessage,0,nType,TEXT("��Ϸ����"));

	return nResult;
}

//��¼��Ϣ
bool __cdecl CPlazaDlg::SendLogonMessage()
{
	m_bLogonPlaza=false;
	//((CGameFrame*)AfxGetMainWnd())->m_UserInfoView.ShowUserInfo(false);
	PostMessage(WM_COMMAND,IDM_LOGON_PLAZA,0);
	return true;
}

bool __cdecl CPlazaDlg::OnEventTCPSocketLink( WORD wSocketID, INT nErrorCode )
{
	//������
	if (nErrorCode!=0)
	{
		//cxf 1116 g_GlobalAttemper.DestroyStatusWnd(this);
		m_GameFlash.Play(false);

		ShowMessageBox(TEXT("��¼����������ʧ�ܣ����Ժ����Ի�������վ���棡"),MB_ICONINFORMATION);
		SendLogonMessage();
		m_pDlgLogon->m_edUserAccounts.SetFocus();
		return true;
	}
	
	if (m_bIsUpdateUserInfo)
	{//������°�ť
		m_bIsUpdateUserInfo=false;
		BYTE cbBuffer[SOCKET_PACKET];
		tagGlobalUserData & UserData = g_GlobalUnits.GetGolbalUserData();

		//��������
		CMD_GP_UpdateUserInfo * pUpdateUserInfo=(CMD_GP_UpdateUserInfo *)cbBuffer;
		memset(pUpdateUserInfo,0,sizeof(CMD_GP_UpdateUserInfo));
		pUpdateUserInfo->dwUserID = UserData.dwUserID;
		CopyMemory(pUpdateUserInfo->szPassWord, UserData.szPassWord, sizeof(UserData.szPassWord));

		//�������ݰ�
		CSendPacketHelper Packet(cbBuffer+sizeof(CMD_GP_UpdateUserInfo),sizeof(cbBuffer)-sizeof(CMD_GP_UpdateUserInfo));
		m_ClientSocket.GetInterface()->SendData(MDM_GP_USER,SUB_GP_UPDATE_USERINFO,cbBuffer,sizeof(CMD_GP_UpdateUserInfo)+Packet.GetDataSize());

		return true;
	}
	
	if (m_bIsGetGlod)
	{//�����ȡ���
		m_bIsGetGlod=false;
		BYTE cbBuffer[SOCKET_PACKET];
		tagGlobalUserData & UserData = g_GlobalUnits.GetGolbalUserData();

		//��������
		CMD_GP_GiftGold * pUpdateUserInfo=(CMD_GP_GiftGold *)cbBuffer;
		memset(pUpdateUserInfo,0,sizeof(CMD_GP_GiftGold));
		pUpdateUserInfo->dwUserID = UserData.dwUserID;
		CopyMemory(pUpdateUserInfo->szPassWord, UserData.szPassWord, sizeof(UserData.szPassWord));

		//�������ݰ�
		CSendPacketHelper Packet(cbBuffer+sizeof(CMD_GP_GiftGold),sizeof(cbBuffer)-sizeof(CMD_GP_GiftGold));
		m_ClientSocket.GetInterface()->SendData(MDM_GP_USER, SUB_GP_GET_GIFT_GOLD,cbBuffer,sizeof(CMD_GP_GiftGold)+Packet.GetDataSize());

		return true;
	}

	if (m_bCheckLine)
	{
		m_bCheckLine = false;

		BYTE cbBuffer[SOCKET_PACKET];
		tagGlobalUserData & UserData = g_GlobalUnits.GetGolbalUserData();

		//��������
		CMD_GP_CheckLine * pCheckLine=(CMD_GP_CheckLine *)cbBuffer;
		memset(pCheckLine,0,sizeof(CMD_GP_CheckLine));
		pCheckLine->dwUserID = UserData.dwUserID;

		//�������ݰ�
		CSendPacketHelper Packet(cbBuffer+sizeof(CMD_GP_CheckLine),sizeof(cbBuffer)-sizeof(CMD_GP_CheckLine));
		m_ClientSocket.GetInterface()->SendData(MDM_GP_USER, SUB_GP_CHECK_LINE,cbBuffer,sizeof(CMD_GP_CheckLine)+Packet.GetDataSize());
		return true;
	}

	if (m_bDeleteMachineCode)
	{
		BYTE cbBuffer[SOCKET_PACKET];
		m_bDeleteMachineCode=false;
		tagGlobalUserData & UserData = g_GlobalUnits.GetGolbalUserData();

		CMD_GP_DeleteMachineCode * pDeleteMachinCode=(CMD_GP_DeleteMachineCode *)cbBuffer;
		memset(pDeleteMachinCode,0,sizeof(CMD_GP_CheckLine));
		lstrcpy(pDeleteMachinCode->szAccounts, UserData.szAccounts);
		
		//�������к�
		g_GlobalUnits.GetClientSerial(pDeleteMachinCode->clientSerial);

		CSendPacketHelper Packet(cbBuffer+sizeof(CMD_GP_CheckLine),sizeof(cbBuffer)-sizeof(CMD_GP_CheckLine));
		m_ClientSocket.GetInterface()->SendData(MDM_GP_USER, SUB_GP_DELETE_MACHINE_CODE, &cbBuffer, sizeof(CMD_GP_DeleteMachineCode));
		return true;
	}
	 
	//���͵�¼��
	if (m_bLogonPlaza==false)
	{
		//cxf 1022 m_ShowGameMsg.ClearAllMsg();
		/*cxf m_bIsFlashMsg = false;
		m_dwGameMsgIndex = 0;*/
		m_pDlgLogon->SendLogonPacket(m_ClientSocket.GetInterface());
		//cxf 1116 g_GlobalAttemper.ShowStatusMessage(TEXT("������֤�û���¼��Ϣ..."),this);
	}

	//�Զ���ͷ��
	//	CGameFrame *pGameFrame = (CGameFrame *)AfxGetMainWnd() ;
	//	CDlgCustomFace &DlgCustomFace = pGameFrame->m_DlgCustomFace;
	//	if ( DlgCustomFace.m_hWnd != NULL && DlgCustomFace.IsWindowVisible() )
	//	{
	//		DlgCustomFace.SendData();
	//	}

	//�����ж�
	//if ( m_bDownloadConnect )
	//{
	//	//���ñ���
	//	m_bDownloadConnect = false;

	//	//Ͷ������
	//	tagDownloadInfo &DownloadInfo = m_DownloadInfoArrary[0];

	//	CMD_GP_DownloadFace DownloadFace;
	//	DownloadFace.dwUserID = DownloadInfo.dwUserID;
	//	m_ClientSocket->SendData(MDM_GP_USER, SUB_GP_USER_DOWNLOAD_FACE, &DownloadFace, sizeof(DownloadFace));
	//}
	return true;
}

bool __cdecl CPlazaDlg::OnEventTCPSocketShut( WORD wSocketID, BYTE cbShutReason )
{
	m_bIsUpdateUserInfo=false;
	m_bIsGetGlod=false;
	m_bCheckLine=false;
	m_bDeleteMachineCode=false;

	if (m_bLogonPlaza==false)
	{
		if (cbShutReason!=SHUT_REASON_NORMAL)
		{
			//cxf 1116 g_GlobalAttemper.DestroyStatusWnd(this);
			PostMessage(WM_COMMAND,IDM_LOGON_PLAZA,0);
			/*
			ShowMessageBox(TEXT("��¼����������ʧ�ܣ����Ժ����Ի�������վ���棡"),MB_ICONINFORMATION);
			SendLogonMessage();*/

			return true;
		}
	}
	m_GameFlash.Play(false);

	//�Զ���ͷ��
	//	CGameFrame *pGameFrame = (CGameFrame *)AfxGetMainWnd() ;
	//	CDlgCustomFace &DlgCustomFace = pGameFrame->m_DlgCustomFace;
	//	if ( DlgCustomFace.m_hWnd != NULL ) DlgCustomFace.SetStatus(enOperateStatus_NULL);

	//�ͷ��ڴ�

	//	if ( m_CustomFace.pFaceData != NULL ) m_CustomFace.Clear();

	return true;
}

bool __cdecl CPlazaDlg::OnEventTCPSocketRead( WORD wSocketID, CMD_Command Command, VOID * pData, WORD wDataSize )
{
	switch (Command.wMainCmdID)
	{
	case MDM_GP_LOGON:				//��¼��Ϣ
		{
			return OnSocketMainLogon(Command,pData,wDataSize);		//�û���֤����¼�ɹ������Ϣ
		}
	case MDM_GP_SERVER_LIST:		//�б���Ϣ
		{
			return OnSocketMainServerList(Command,pData,wDataSize);	//������Ϸ�����б�
		}
	case MDM_GP_SYSTEM:				//ϵͳ��Ϣ
		{
			return OnSocketMainSystem(Command,pData,wDataSize);		//������Ϸ�ͻ��˰汾������Ϣ
		}
	case MDM_GP_USER:				//�û���Ϣ
		{
			return OnSocketMainUser(Command,pData,wDataSize);		//�û�ͷ��������Ϣ
		}
	}

	return true;
}

bool __cdecl CPlazaDlg::SendConnectMessage()
{
	PostMessage(WM_COMMAND,IDM_CONNECT_SERVER,0);
	return true;
}

bool CPlazaDlg::OnSocketMainLogon(CMD_Command Command, void * pData, WORD wDataSize)
{
	switch (Command.wSubCmdID)
	{
	case SUB_GP_LOGON_SUCCESS:		//��¼�ɹ�
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(CMD_GP_LogonSuccess));
			if (wDataSize<sizeof(CMD_GP_LogonSuccess))
			{
				return false;
			}
			ShowWindow(SW_HIDE);
			//�ر���ʾ
			//g_GlobalAttemper.DestroyStatusWnd(this);

			//������Ϣ
			int iSize = sizeof(CMD_GP_LogonSuccess);

			CMD_GP_LogonSuccess * pLogonSuccess=(CMD_GP_LogonSuccess *)pData;
			if (NULL == pLogonSuccess)
			{
				return false;
			}
			if (ANDROID == pLogonSuccess->utIsAndroid)
			{//������
				m_ClientSocket->CloseSocket();
				ShowMessageBox(TEXT("�����˺Ų����ڻ���������������,���֤���ٴε�¼!"),MB_ICONQUESTION);
				m_pDlgLogon->m_edUserAccounts.SetFocus();
				////���ش���
				//SetAllWndShow(false);

				//KillTimer(TIME_SET_DLG_ALPHA_INTERVAL);
				//m_uiCurAlphaValue = 0;
				//m_hSaveNewCurWnd = NULL;
				//m_hSaveOldCurWnd = NULL;
				//m_byCurWndTag = 0;

				//CRect rect;
				//GetWindowRect(&rect);
				//::SetWindowPos(m_pDlgLogon->GetSafeHwnd(), 0, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
				//m_pDlgLogon->OnPopRegisterDlg();
				return true;
			}
			//cxf 1116 g_GlobalAttemper.ShowStatusMessage(TEXT("���ڵ�¼��Ϸ����..."),this);
			tagGlobalUserData & UserData=g_GlobalUnits.GetGolbalUserData();
			UserData.wFaceID=pLogonSuccess->wFaceID;
			UserData.cbGender=pLogonSuccess->cbGender;
			UserData.cbMember=pLogonSuccess->cbMember;
			UserData.dwUserID=pLogonSuccess->dwUserID;
			UserData.dwGameID=pLogonSuccess->dwGameID;
			UserData.dwExperience=pLogonSuccess->dwExperience;
			UserData.dwCustomFaceVer=pLogonSuccess->dwCustomFaceVer;
			UserData.dwLockServerID=pLogonSuccess->dwLockServerID;
			::memset(UserData.szNickName, 0, sizeof(UserData.szNickName));
			::memcpy(UserData.szNickName, pLogonSuccess->szNickName, strlen(pLogonSuccess->szNickName));
			UserData.lGold = pLogonSuccess->lGold;
			UserData.dwGrade = pLogonSuccess->dwGrade;
			UserData.lGem = pLogonSuccess->lGem;
			UserData.lMoney = pLogonSuccess->lMoney;
			//UserData.lHonor = pLogonSuccess->lHonor;
			UserData.lWinCount = pLogonSuccess->lWinCount;
			UserData.lLostCount = pLogonSuccess->lLostCount;
			UserData.lDrawCount = pLogonSuccess->lDrawCount;
			UserData.lFleeCount = pLogonSuccess->lFleeCount;

			::memcpy(UserData.szKey, pLogonSuccess->szKey, sizeof(pLogonSuccess->szKey));//��½��ҳKey
			UserData.wGiftCount=pLogonSuccess->lGiftNum;
			UserData.nMasterRight=pLogonSuccess->nMasterRight;//Ȩ�޹���
			UserData.nMasterOrder=pLogonSuccess->nMasterOrder;//Ȩ�޵ȼ�

			//��չ��Ϣ
			void * pDataBuffer=NULL;
			tagDataDescribe DataDescribe;
			CRecvPacketHelper RecvPacket(pLogonSuccess+1,wDataSize-sizeof(CMD_GP_LogonSuccess));
			while (true)
			{
				pDataBuffer=RecvPacket.GetData(DataDescribe);
				if (DataDescribe.wDataDescribe==DTP_NULL)
				{
					break;
				}

				switch (DataDescribe.wDataDescribe)
				{
				case DTP_USER_ACCOUNTS:		//�û��ʻ�
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize>0);
						ASSERT(DataDescribe.wDataSize<=sizeof(UserData.szAccounts));
						if (DataDescribe.wDataSize<=sizeof(UserData.szAccounts))
						{
							CopyMemory(UserData.szAccounts,pDataBuffer,DataDescribe.wDataSize);
							UserData.szAccounts[CountArray(UserData.szAccounts)-1]=0;
						}
						break;
					}
				case DTP_USER_PASS:			//�û�����
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize>0);
						ASSERT(DataDescribe.wDataSize<=sizeof(UserData.szPassWord));
						if (DataDescribe.wDataSize<=sizeof(UserData.szPassWord))
						{
							CopyMemory(UserData.szPassWord,pDataBuffer,DataDescribe.wDataSize);
							UserData.szPassWord[CountArray(UserData.szPassWord)-1]=0;
						}
						break;
					}
				case DTP_UNDER_WRITE:		//����ǩ��
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize>0);
						ASSERT(DataDescribe.wDataSize<=sizeof(UserData.szUnderWrite));
						if (DataDescribe.wDataSize<=sizeof(UserData.szUnderWrite))
						{
							CopyMemory(UserData.szUnderWrite,pDataBuffer,DataDescribe.wDataSize);
							UserData.szUnderWrite[CountArray(UserData.szUnderWrite)-1]=0;
						}
						break;
					}
				case DTP_USER_GROUP_NAME:	//��������
					{
						ASSERT(pDataBuffer!=NULL);
						ASSERT(DataDescribe.wDataSize>0);
						ASSERT(DataDescribe.wDataSize<=sizeof(UserData.szGroupName));
						if (DataDescribe.wDataSize<=sizeof(UserData.szGroupName))
						{
							CopyMemory(UserData.szGroupName,pDataBuffer,DataDescribe.wDataSize);
							UserData.szGroupName[CountArray(UserData.szGroupName)-1]=0;
						}
						break;
					}
				case DTP_STATION_PAGE:		//��Ϸ��վ
					{
						ASSERT(pDataBuffer!=NULL);
						if (pDataBuffer!=NULL) 
						{
							g_GlobalUnits.SetStationPage((LPCTSTR)pDataBuffer);
							//m_pHtmlBrower->Navigate(g_GlobalUnits.GetStationPage());
						}
						break;
					}
				default: { ASSERT(FALSE); }
				}
			}

			//����URL
			//cxf 1022 m_doteyBoxDlg.SetResetURL();
			//cxf 1022 m_TaskDlg.ResetDlgURL();
			//cxf 1022 m_shopDlg.SetResetURL();

			//m_bIsDrawBack = true;
			m_UserImage.DestroyImage();
			char chImageData[32] = {0};
			//cxf
			//::sprintf(chImageData, "PNG_USER_%d_IMAGE", UserData.wFaceID);
			//HINSTANCE hInstance = AfxGetInstanceHandle();
			//m_UserImage.LoadImage(hInstance, chImageData);
			CString szPath("");
			GetResPath("\\data\\image\\",szPath);
			if (0<UserData.wFaceID){
				::sprintf(chImageData, "Image_p_%d", UserData.wFaceID);
			}else{
				if (1==UserData.cbGender){
					::sprintf(chImageData, "Image_p_%d_w", UserData.wFaceID);
				}else{
					::sprintf(chImageData, "Image_p_%d_m", UserData.wFaceID);
				}
			}
			szPath.Append(chImageData);
			szPath.Append(TEXT(".png"));
			m_UserImage.LoadImage(szPath);

			//m_bIsFlashMsg = false;
			////ע����ַ�
			//CString strRegKey;
			//strRegKey.Format(TEXT("%s\\%ld"),REG_USER_INFO,UserData.dwUserID);
			////��ע���
			//CRegKey RegPassWord;
			//RegPassWord.Open(HKEY_CURRENT_USER,strRegKey,KEY_READ);
			//if (RegPassWord!=NULL)
			//{
			//	if (RegPassWord.QueryValue(m_dwGameMsgIndex, TEXT("GameMsgIndex"))!=ERROR_SUCCESS)
			//	{
			//		m_dwGameMsgIndex = 0;
			//	}
			//}
			//else m_dwGameMsgIndex = 0;

			//SetResetAlphaValue(m_gameListDlg.GetSafeHwnd());

			//			CWnd * pMainWnd=((CGamePlazaApp*)AfxGetApp())->m_pMainWnd;
			//			pMainWnd->ShowWindow(SW_SHOW);

			m_bIsGameSoftUpdate = CheckGameSoftHash(pLogonSuccess->szHashID);
			if (2 != g_GlobalUnits.GetApplicationCmdLine().GetSize())
			{
				m_pDlgLogon->EnableControlWindow(TRUE);
			}
			return true;
		}
	case SUB_GP_LOGON_ERROR:		//��¼ʧ��
		{
			//Ч�����
			CMD_GP_LogonError *pLogonError = (CMD_GP_LogonError *)pData;
			ASSERT(wDataSize>=(sizeof(CMD_GP_LogonError)-sizeof(pLogonError->szErrorDescribe)));
			if (wDataSize<(sizeof(CMD_GP_LogonError)-sizeof(pLogonError->szErrorDescribe))) return false;
			ShowWindow(SW_HIDE);
			//�ر�����
			//cxf 1116 g_GlobalAttemper.DestroyStatusWnd(this);
			m_GameFlash.Play(false);
			if (2 != g_GlobalUnits.GetApplicationCmdLine().GetSize())
			{
				m_pDlgLogon->EnableControlWindow(TRUE);
			}

			m_ClientSocket->CloseSocket();

			//��ʾ��Ϣ
			WORD wDescribeSize=wDataSize-(sizeof(CMD_GP_LogonError)-sizeof(pLogonError->szErrorDescribe));
			if (wDescribeSize>0)
			{
				pLogonError->szErrorDescribe[wDescribeSize-1]=0;
				ShowMessageBox(pLogonError->szErrorDescribe,MB_ICONINFORMATION);
			}

			//���͵�¼
			SendLogonMessage();
			m_pDlgLogon->m_edUserAccounts.SetFocus();
			return true;
		}
	case SUB_GP_LOGON_USER_GRADE_INFO:
		{
			m_iGradeInfoDiv = wDataSize / sizeof(CMD_GP_UserGradeInfo);
			int mod = wDataSize % sizeof(CMD_GP_UserGradeInfo);

			if (mod != 0) return false;
			if (NULL != pData)
			{
				if (NULL != m_pGradeInfo)
				{
					delete m_pGradeInfo;
				}
				m_pGradeInfo = NULL;
				m_pGradeInfo = new CMD_GP_UserGradeInfo[m_iGradeInfoDiv];
				memcpy(m_pGradeInfo, pData, sizeof(CMD_GP_UserGradeInfo)*m_iGradeInfoDiv);
			}
			return true;
		}
	case SUB_GP_LOGON_FINISH:		//��¼���
		{
			m_GameFlash.Play(false);

			// �����ж�
			if (m_bIsGameSoftUpdate)
			{
				if (2 == g_GlobalUnits.GetApplicationCmdLine().GetSize())
				{
					ShowMessageBox("��Ϸ���Զ�θ���ʧ�ܣ�����ϵ�ͷ���", MB_ICONSTOP);
				}
				else
				{
					CallExternalUpdateProgram();
				}

				m_ClientSocket->CloseSocket();
				OnCancel();
				return true;
			}
			

			//SetAllWndAlpha(0);
			//SetAllWndShow(true);
			//cxf 1116 g_GlobalAttemper.DestroyStatusWnd(this);

			m_pDlgLogon->EnableControlWindow(TRUE);
			ShowWindow(SW_SHOW);

			//cxf m_gameListDlg.SetHtml();
			//cxf m_hSaveOldCurWnd = m_gameListDlg.GetSafeHwnd();
			//������Ϸ�б� cxf
			m_GameListCenterDlg.SetGameList();

			//cxf m_pFunShowLayer(m_hSaveOldCurWnd, 0, 255, 2);
			m_curTab = 1;
			//cxf 1025 SetShowTab(1);
			//cxf m_gameListDlg.SetFocus();
			//cxf 1022 m_roomListDlg.SetResetHtml();
			m_bIsGamePlazyStatus = true;

			if (1 == m_bIsSound)
			{//��������
				SetTimer(IDI_TIMER_PLAY_BACK_MUSIC, TIME_PLAY_BACK_MUSIC_INTERVAL, NULL);
			}
			else
			{//��ֹ����
				::PlaySound(NULL, AfxGetInstanceHandle(),SND_PURGE);
			}

			/*cxfif (m_bIsFlashMsg)
			{
			tagGlobalUserData & UserData=g_GlobalUnits.GetGolbalUserData();
			//������Ϣ
			TCHAR szBuffer[256];
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%s\\%ld"),REG_USER_INFO,UserData.dwUserID);
			//д����Ϣ
			CRegKey RegUserInfo;
			if (RegUserInfo.Create(HKEY_CURRENT_USER,szBuffer)==ERROR_SUCCESS)
			{
			RegUserInfo.SetDWORDValue(TEXT("GameMsgIndex"),m_dwGameMsgIndex);
			}
			//���ö�ʱ��
			SetTimer(IDI_TIMER_GAME_MSG_FLASH, TIME_GAME_MSG_FLASH_INTERVAL, NULL);
			::PlaySound(TEXT("WAVE_NEW_MSG"), AfxGetInstanceHandle(),SND_ASYNC|SND_NODEFAULT|SND_RESOURCE);
			}
			else
			{
			m_bIsFlashMsg = true;
			}*/

			/*
			//չ������
			INT_PTR nIndex=0;
			CListType * pListType=NULL;
			do
			{
			pListType=g_GlobalUnits.m_ServerListManager.EnumTypeItem(nIndex++);
			if (pListType==NULL) break;
			g_GlobalUnits.m_ServerListManager.ExpandListItem(pListType);
			} while (true);

			//չ���б�
			nIndex=0;
			CListInside * pListInside=NULL;
			do
			{
			pListInside=g_GlobalUnits.m_ServerListManager.EnumInsideItem(nIndex++);
			if (pListInside==NULL) break;
			g_GlobalUnits.m_ServerListManager.ExpandListItem(pListInside);
			} while (true);
			*/


			//��¼��Ϣ
			m_bLogonPlaza=true;
			m_pDlgLogon->OnLogonSuccess();
			//m_pHtmlBrower->EnableBrowser(true);

			//��¼��Ϣ
			g_GlobalUnits.WriteUserCookie();

			//��ʾͷ�� TODO��
			//			((CGameFrame*)AfxGetMainWnd())->m_UserInfoView.ShowUserInfo(true);

			//�Զ����ж�
			//tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
			//if ( GlobalUserData.dwCustomFaceVer!=0)
			//{
			//	//ͷ������
			//	CString strDirName = CString(g_GlobalUnits.GetWorkDirectory()) + TEXT("\\CustomFace");
			//	CString strFileName;
			//	strFileName.Format(TEXT("\\%ld_%d.bmp"), GlobalUserData.dwUserID, GlobalUserData.dwCustomFaceVer);

			//	//��ȡ�ļ�
			//	CImage FaceImage;
			//	HRESULT hResult = FaceImage.Load(strDirName + strFileName);
			//	if (SUCCEEDED(hResult))
			//	{
			//		//�ر�����
			//		m_ClientSocket->CloseSocket();

			//		FaceImage.Destroy();
			//	}
			//	//����ͷ��
			//	else
			//	{
			//		PostMessage(WM_DOWN_LOAD_FACE, LPARAM(GlobalUserData.dwCustomFaceVer), WPARAM(GlobalUserData.dwUserID));
			//	}
			//}
			//else
			//{
			//�ر�����
			m_ClientSocket->CloseSocket();
			/*cxf HINSTANCE hInstance = AfxGetInstanceHandle();
			m_TitleButtonList[3].BTImage.DestroyImage();
			m_TitleButtonList[3].BTImage.LoadImage(hInstance, TEXT("BT_PNG_IMAGE_SHOP"));
			m_TitleButtonList[0].btStatus = 3;
			m_TitleButtonList[1].btStatus = 0;
			m_TitleButtonList[2].btStatus = 0;
			m_TitleButtonList[3].btStatus = 0;*/

			//}
			//�ر���ʾ
			tagGlobalUserData & UserData=g_GlobalUnits.GetGolbalUserData();
			if (UserData.dwExperience<=0)
			{//�����û�
				m_bNewComerGuide=true;
				SetTimer(IDI_ITMER_MEWCOMER_GUIDE,350,NULL);
				DoModalNewComerGuide();
			}
			if (0 != UserData.dwLockServerID)
			{//ֱ�ӵ�¼����
				OnFlashEnterRoom();
			}
			return true;
		}
		/*case SUB_GP_LOGON_USER_PLACARD:		//��Ϸ��Ϣ
		{
		if (wDataSize > sizeof(CMD_GP_UserPlacard))
		{
		return false;
		}

		CMD_GP_UserPlacard* pGameMsg = (CMD_GP_UserPlacard*)pData;
		if (NULL == pGameMsg)
		{
		return false;
		}

		m_ShowGameMsg.InsertGameMsg(pGameMsg->szTitle, pGameMsg->szDate, pGameMsg->szContex);
		if (0 == m_dwGameMsgIndex)
		{
		m_bIsFlashMsg = false;
		//ע����ַ�
		CString strRegKey;
		strRegKey.Format(TEXT("%s\\%ld"),REG_USER_INFO,pGameMsg->dwUserId);
		//��ע���
		CRegKey RegPassWord;
		RegPassWord.Open(HKEY_CURRENT_USER,strRegKey,KEY_READ);
		if (RegPassWord!=NULL)
		{
		if (RegPassWord.QueryValue(m_dwGameMsgIndex, TEXT("GameMsgIndex"))!=ERROR_SUCCESS)
		{
		m_dwGameMsgIndex = 0;
		}
		}
		else
		{
		m_dwGameMsgIndex = 0;
		}
		}
		if (pGameMsg->dwIndex > m_dwGameMsgIndex)
		{
		m_dwGameMsgIndex = pGameMsg->dwIndex;
		m_bIsFlashMsg = true;


		return true;
		}}*/
	}

	return true;
}


//ֱ�ӽ��뷿��
void CPlazaDlg::OnFlashEnterRoom()
{
	if (FALSE == IsCanIntervalConnect())
		return;

	tagGlobalUserData & UserData=g_GlobalUnits.GetGolbalUserData();
	INT_PTR nIndex=0;
	CListServer * pListServer = g_GlobalUnits.m_ServerListManager.SearchServerItem(UserData.dwLockServerID);
	WORD dwKind = pListServer->GetListKind()->m_GameKind.wKindID;
	if (0 != dwKind)
	{
		m_RoomListCenterDlg.m_wKindID = dwKind;
	}

	if (NULL != m_pRoomViewItemCreate)
	{
		SafeDelete(m_pRoomViewItemCreate);
	}
	//Ѱ�ҷ���
	CRoomViewItem * pRoomViewItem=m_pRoomViewItemCreate;
	//����������ͼ(�����б�)
	try
	{
		pRoomViewItem=new CRoomViewItem;
		if (pRoomViewItem==NULL) 
		{
			return;
		}
		pRoomViewItem->Create(IDD_TABLE_LIST,this);//���������б�Ի���

		//pRoomViewItem->m_gameListManager = &m_gameListManager;//�����б�
		if (pRoomViewItem->InitRoomViewItem(pListServer)==false) 
		{
			throw TEXT("��Ϸ�����ʼ��ʧ�ܣ�");
		}
	}
	catch (...) 
	{ 
		SafeDelete(pRoomViewItem); 
	}
	if (pRoomViewItem==NULL)
	{
		ShowMessageBox(TEXT("��Ϸ���䴴��ʧ��"),MB_ICONQUESTION);
		PostMessage(WM_COMMAND,IDM_LOGON_PLAZA,0);
		return;
	}

	//������Ϸ
	bool bSuccess=pRoomViewItem->ConnectGameServer();
	if (bSuccess==false || pRoomViewItem->GetLogonFull())//�жϷ����Ƿ�����
	{
		pRoomViewItem->DestroyWindow();
		SafeDelete(pRoomViewItem);
		return;
	}

	//���ñ���
	m_pRoomViewItemCreate=pRoomViewItem;
	CRect plazarect;
	GetWindowRect(&plazarect);
	::MoveWindow(m_pRoomViewItemCreate->GetSafeHwnd(), plazarect.left, plazarect.top, 960, 720, FALSE);
	//HWND hWnd=NULL;
	//hWnd=m_pRoomViewItemCreate->GetSafeHwnd();

	//������ʾ
	CString strBuffer;
	strBuffer.Format(TEXT("�������ӷ��䣬���Ժ�..."));
	g_GlobalAttemper.ShowStatusMessage(strBuffer,pRoomViewItem);

	/*cxf 1022 tagGlobalUserData & UserData=g_GlobalUnits.GetGolbalUserData();
	INT_PTR nIndex=0;
	CListServer * pListServer = g_GlobalUnits.m_ServerListManager.SearchServerItem(UserData.dwLockServerID);
	WORD dwKind = pListServer->GetListKind()->m_GameKind.wKindID;
	if (0 != dwKind)
	{
	m_roomListDlg.m_wKindID = dwKind;
	}

	//Ѱ�ҷ���
	CRoomViewItem * pRoomViewItem=m_pRoomViewItemCreate;
	//����������ͼ(�����б�)
	try
	{
	pRoomViewItem=new CRoomViewItem;
	if (pRoomViewItem==NULL) 
	{
	return;
	}
	pRoomViewItem->Create(IDD_TABLE_LIST,this);//���������б�Ի���
	//pRoomViewItem->m_gameListManager = &m_gameListManager;//�����б�
	if (pRoomViewItem->InitRoomViewItem(pListServer)==false) 
	{
	throw TEXT("��Ϸ�����ʼ��ʧ�ܣ�");
	}
	}
	catch (...) 
	{ 
	SafeDelete(pRoomViewItem); 
	}
	if (pRoomViewItem==NULL)
	{
	ShowMessageBox(TEXT("��Ϸ���䴴��ʧ��"),MB_ICONQUESTION);
	PostMessage(WM_COMMAND,IDM_LOGON_PLAZA,0);
	return;
	}

	//������Ϸ
	bool bSuccess=pRoomViewItem->ConnectGameServer();
	if (bSuccess==false || pRoomViewItem->GetLogonFull())//�жϷ����Ƿ�����
	{
	pRoomViewItem->DestroyWindow();
	SafeDelete(pRoomViewItem);
	return;
	}

	//���ñ���
	m_pRoomViewItemCreate=pRoomViewItem;

	::MoveWindow(m_pRoomViewItemCreate->GetSafeHwnd(),287,81, 659, 626, FALSE);
	ShowTab(3);
	//HWND hWnd=NULL;
	//hWnd=m_pRoomViewItemCreate->GetSafeHwnd();

	//������ʾ
	CString strBuffer;
	strBuffer.Format(TEXT("�������ӷ��䣬���Ժ�..."));
	g_GlobalAttemper.ShowStatusMessage(strBuffer,pRoomViewItem);
	*/
}


bool CPlazaDlg::OnSocketMainServerList(CMD_Command Command, void * pData, WORD wDataSize)
{
	ASSERT(Command.wMainCmdID==MDM_GP_SERVER_LIST);

	switch (Command.wSubCmdID)
	{
	case SUB_GP_LIST_TYPE:			//������Ϣ
		{
			//Ч�����
			ASSERT(wDataSize%sizeof(tagGameType)==0);
			if (wDataSize%sizeof(tagGameType)!=0) return false;

			//������Ϣ
			tagGameType * pGameType=(tagGameType *)pData;
			WORD wItemCount=wDataSize/sizeof(tagGameType);
			//TODO:�ṹҪ��
			g_GlobalUnits.m_ServerListManager.InsertTypeItem(pGameType,wItemCount);

			return true;
		}
	case SUB_GP_LIST_KIND:			//������Ϣ
		{
			//Ч�����
			ASSERT(wDataSize%sizeof(tagGameKind)==0);
			if (wDataSize%sizeof(tagGameKind)!=0) return false;

			//������Ϣ
			tagGameKind * pGameKind=(tagGameKind *)pData;
			WORD wItemCount=wDataSize/sizeof(tagGameKind);

			//TODO:�ṹҪ�� 
			g_GlobalUnits.m_ServerListManager.InsertKindItem(pGameKind,wItemCount);


			CString	sFileName("");
			CString sFilePath("");
			CString sFieldName("");
			TCHAR tchReturnVal[MAX_PATH]=_T("");
			GetModuleFileName(NULL, sFileName.GetBufferSetLength(MAX_PATH), MAX_PATH);
			sFilePath=sFileName.Left(sFileName.ReverseFind('\\') + 1) + TEXT("config\\roomlist.ini");

			for (WORD i=0; i<wItemCount; i++)
			{
				tagGameKind * pSiSeQiuKind = (tagGameKind *)(pGameKind+i);
				if (NULL == pSiSeQiuKind)
					continue;

				if (SISEQIU_ID == pSiSeQiuKind->wKindID)
				{
					tagGameServer GameServer;
					GameServer.wKindID = SISEQIU_ID;
					GameServer.wServerID = SISEQIU_ID;
					GameServer.dwMaxCount = 400;
					GameServer.dwOnLineCount = 100;
					_snprintf(GameServer.szServerName, sizeof(GameServer.szServerName), _T("��ɫ�򷿼�"));
					int iEnable = GetPrivateProfileInt(TEXT("siseqiu"), TEXT("Enable"), 1, sFilePath);
					if (0 == iEnable)
						continue;

					GameServer.wSortID = GetPrivateProfileInt(TEXT("siseqiu"), TEXT("SortID"), 1, sFilePath);
					GameServer.lCellScore = GetPrivateProfileInt(TEXT("siseqiu"), TEXT("lCellScore"), 0, sFilePath);
					GameServer.lRestrictScore = GetPrivateProfileInt(TEXT("siseqiu"), TEXT("lRestrictScore"), 0, sFilePath);
					GameServer.lEnterMoney = GetPrivateProfileInt(TEXT("siseqiu"), TEXT("lEnterMoney"), 0, sFilePath);

					g_GlobalUnits.m_ServerListManager.InsertServerItem(&GameServer, 1);
				}
				else if (ACTIVE_ID == pSiSeQiuKind->wKindID)
				{
					int iIndex = 1;
					while (1)
					{
						tagGameServer GameServer;
						sFieldName.Format(_T("Active_%d"), iIndex);
						GameServer.wKindID = ACTIVE_ID;
						GameServer.wServerID = GetPrivateProfileInt(sFieldName, TEXT("ServerID"), 1, sFilePath);
						GameServer.wSortID = GetPrivateProfileInt(sFieldName, TEXT("SortID"), 1, sFilePath);
						GetPrivateProfileString(sFieldName, _T("Name"), _T(""), tchReturnVal, sizeof(tchReturnVal), sFilePath);

						if (0 >= lstrlen(tchReturnVal))
							break;

						g_GlobalUnits.m_ServerListManager.InsertServerItem(&GameServer, 1);
						iIndex++;
					}
				}
			}
			return true;
		}
	case SUB_GP_LIST_STATION:		//վ����Ϣ
		{
			//Ч�����
			ASSERT(wDataSize%sizeof(tagGameStation)==0);
			if (wDataSize%sizeof(tagGameStation)!=0) return false;

			//������Ϣ
			tagGameStation * pGameStation=(tagGameStation *)pData;
			WORD wItemCount=wDataSize/sizeof(tagGameStation);
			//TODO:�ṹҪ��
			g_GlobalUnits.m_ServerListManager.InsertStationItem(pGameStation,wItemCount);

			return true;
		}
	case SUB_GP_LIST_SERVER:		//����������
		{
			//Ч�����
			ASSERT(wDataSize%sizeof(tagGameServer)==0);
			if (wDataSize%sizeof(tagGameServer)!=0) return false;

			//������Ϣ
			tagGameServer * pGameServer=(tagGameServer *)pData;
			WORD wItemCount=wDataSize/sizeof(tagGameServer);
			//TODO:�ṹҪ��
			g_GlobalUnits.m_ServerListManager.InsertServerItem(pGameServer,wItemCount);
			return true;
		}
	case SUB_GP_LIST_FINISH:		//�б������
		{
			//��������
			INT_PTR nIndex=0;
			DWORD dwAllOnLineCount=0L;
			CListKind * pListKind=NULL;
			do
			{
				pListKind=g_GlobalUnits.m_ServerListManager.EnumKindItem(nIndex++);
				if (pListKind==NULL) break;
				dwAllOnLineCount+=pListKind->GetItemInfo()->dwOnLineCount;
			} while (true);
			g_GlobalUnits.m_ServerListManager.UpdateGameOnLineCount(dwAllOnLineCount);

			return true;
		}
	case SUB_GP_LIST_CONFIG:		//�б�����
		{
			//Ч�����
			ASSERT(wDataSize%sizeof(CMD_GP_ListConfig)==0);
			if (wDataSize%sizeof(CMD_GP_ListConfig)!=0) return false;

			//������Ϣ
			CMD_GP_ListConfig * pListConfig=(CMD_GP_ListConfig *)pData;

			//			((CGameFrame*)AfxGetMainWnd())->m_ServerItemView.ShowOnLineCount(pListConfig->bShowOnLineCount?true:false);

			return true;
		}
	}

	return true;
}

bool CPlazaDlg::OnSocketMainSystem(CMD_Command Command, void * pData, WORD wDataSize)
{
	ASSERT(Command.wMainCmdID==MDM_GP_SYSTEM);
	switch (Command.wSubCmdID)
	{
	case SUB_GP_VERSION:			//�汾��Ϣ
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(CMD_GP_Version));
			if (wDataSize<sizeof(CMD_GP_Version)) return false;

			//��Ϣ����
			CMD_GP_Version * pVersion=(CMD_GP_Version *)pData;
			if (NULL==pVersion)
			{
				return false;
			}
			//cxf
			m_bLogonPlaza = false;
			m_pDlgLogon->m_DlgLogonLogo->ShowWindow(SW_HIDE);
			m_pDlgLogon->ShowWindow(SW_HIDE);
			this->ShowWindow(SW_HIDE);
			//end
			//cxf 1116 g_GlobalAttemper.DestroyStatusWnd(this);
			m_GameFlash.Play(false);

			m_ClientSocket->CloseSocket();
			tagGlobalUserData & GlobalUserData=g_GlobalUnits.GetGolbalUserData();
			memset(&GlobalUserData,0,sizeof(GlobalUserData));
			g_GlobalAttemper.DownLoadClient(TEXT("GamePlaza"),0,true,2);


			//if (pVersion->bAllowConnect)
			//{
			//	TCHAR szMessage[]=TEXT("��Ϸ�����汾�Ѿ����������ڵİ汾�����Լ���ʹ�ã������Ƿ����������°汾��");

			//	int iResult=ShowInformationEx(szMessage,0,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1,TEXT("��Ϸ����"));
			//	if (iResult==IDYES)
			//	{
			//		g_GlobalAttemper.DestroyStatusWnd(this);
			//		m_ClientSocket->CloseSocket();
			//		tagGlobalUserData & GlobalUserData=g_GlobalUnits.GetGolbalUserData();
			//		memset(&GlobalUserData,0,sizeof(GlobalUserData));
			//		g_GlobalAttemper.DownLoadClient(TEXT("GamePlaza"),0,true);
			//	}
			//}
			//else
			//{
			//	g_GlobalAttemper.DestroyStatusWnd(this);
			//	m_ClientSocket->CloseSocket();
			//	TCHAR szMessage[]=TEXT("��Ϸ�����汾�Ѿ����������ڵİ汾�����Լ���ʹ�ã������Ƿ����������°汾��");

			//	int iResult=ShowInformationEx(szMessage,0,MB_ICONSTOP|MB_YESNO|MB_DEFBUTTON1,TEXT("��Ϸ����"));
			//	if(iResult != IDYES)
			//	{
			//		tagGlobalUserData & GlobalUserData=g_GlobalUnits.GetGolbalUserData();
			//		memset(&GlobalUserData,0,sizeof(GlobalUserData));
			//		AfxGetMainWnd()->PostMessage(WM_CLOSE);
			//	}
			//	else 
			//	{
			//		g_GlobalAttemper.DownLoadClient(TEXT("GamePlaza"),0,true);
			//	}
			//}

			return true;
		}
	}
	return true;
}

bool CPlazaDlg::OnSocketMainUser( CMD_Command Command, void * pBuffer, WORD wDataSize )
{
	ASSERT(Command.wMainCmdID == MDM_GP_USER);
	switch(Command.wSubCmdID)
	{
	case SUB_GP_USER_DOWNLOAD_FACE:			//����ͷ��
		{
			return true;

			//����ת��
			CMD_GP_DownloadFaceSuccess *pDownloadFaceSuccess = (CMD_GP_DownloadFaceSuccess*)pBuffer;

			//������֤
			WORD wSendSize = WORD(pDownloadFaceSuccess->dwCurrentSize + sizeof(CMD_GP_DownloadFaceSuccess) - sizeof(pDownloadFaceSuccess->bFaceData));
			ASSERT(wDataSize == wSendSize);
			if ( wDataSize != wSendSize )
			{
				return false;
			}


			//��һ���ж�
			if ( m_CustomFace.pFaceData == NULL )
			{
				if ( m_CustomFace.pFaceData != NULL ) 
				{
					delete[] m_CustomFace.pFaceData;
				}
				m_CustomFace.pFaceData = new BYTE[pDownloadFaceSuccess->dwToltalSize];
				if ( m_CustomFace.pFaceData == NULL ) 
				{
					return true;
				}

				m_CustomFace.dwFaceSize = pDownloadFaceSuccess->dwToltalSize;
				m_CustomFace.dwUserID = pDownloadFaceSuccess->dwUserID;
			}

			//��������
			CopyMemory(m_CustomFace.pFaceData+m_CustomFace.dwCurrentSize, pDownloadFaceSuccess->bFaceData, pDownloadFaceSuccess->dwCurrentSize);
			m_CustomFace.dwCurrentSize += pDownloadFaceSuccess->dwCurrentSize;

			//�������
			if ( m_CustomFace.dwFaceSize == m_CustomFace.dwCurrentSize )
			{
				tagDownloadInfo &DownloadInfo = m_DownloadInfoArrary[0];

				//����Ŀ¼
				CString strDirName = CString(g_GlobalUnits.GetWorkDirectory()) + TEXT("\\CustomFace");
				CreateDirectory(strDirName, NULL) ;

				//д���ļ�
				CFile fileFace;
				CString strZipFileName, strBmpFileName;
				strZipFileName.Format(TEXT("\\%ld_%d.zip"), DownloadInfo.dwUserID, DownloadInfo.bFaceVer);
				strBmpFileName.Format(TEXT("\\%ld_%d.bmp"), DownloadInfo.dwUserID, DownloadInfo.bFaceVer);
				if ( fileFace.Open(strDirName + strZipFileName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary) )
				{
					fileFace.Write(m_CustomFace.pFaceData, m_CustomFace.dwFaceSize);
					fileFace.Close();

					//��ѹ�ļ�
					CUnzip oUnzip( strDirName + strZipFileName );
					oUnzip.SwapSize( strDirName + strBmpFileName );

					//ɾ���ļ�
					CFile::Remove(strDirName + strZipFileName);
				}

				//�Լ��ж�
				tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
				if ( GlobalUserData.dwUserID ==  DownloadInfo.dwUserID )
				{
					//���½���
					//					((CGameFrame*)AfxGetMainWnd())->m_UserInfoView.UpdateUserInfo();
				}

				//���ñ���
				m_CustomFace.Clear();

				//ɾ��Ԫ��
				m_DownloadInfoArrary.RemoveAt(0);

				//�����ж�
				if ( 0 < m_DownloadInfoArrary.GetCount() )
				{
					//״̬�ж�
					if ( m_ClientSocket->GetSocketStatus() != SOCKET_STATUS_CONNECT )
					{
						//��������
						if ( ! ConnectServer() )
						{
							return true;
						}


						//���ñ�ʶ
						m_bDownloadConnect = true;
					}

					//Ͷ������
					tagDownloadInfo &DownloadInfo = m_DownloadInfoArrary[0];

					CMD_GP_DownloadFace DownloadFace;
					DownloadFace.dwUserID = DownloadInfo.dwUserID;
					m_ClientSocket->SendData(MDM_GP_USER, SUB_GP_USER_DOWNLOAD_FACE, &DownloadFace, sizeof(DownloadFace));
				}
				else
				{
					//�ر�����
					m_ClientSocket->CloseSocket();

					//���ñ���
					m_bStartDownloadFace=false;
				}
			}
			return true;
		}
	case SUB_GP_UPLOAD_FACE_RESULT:		//�ϴ����
		{
			return true;

			ASSERT(sizeof(CMD_GP_UploadFaceResult) == wDataSize);
			if ( sizeof(CMD_GP_UploadFaceResult) != wDataSize)
			{
				return true;
			}


			//��ʾ��Ϣ
			CMD_GP_UploadFaceResult *pUploadFaceResult = (CMD_GP_UploadFaceResult*)pBuffer;
			ShowMessageBox(pUploadFaceResult->szDescribeMsg, MB_ICONINFORMATION);

			//�ɹ��ж�
			if ( pUploadFaceResult->bOperateSuccess )
			{
				//ͷ��Ŀ¼
				CString strDirName = CString(g_GlobalUnits.GetWorkDirectory()) + TEXT("\\CustomFace");
				CString strOldFileName;
				strOldFileName = TEXT("\\MyFace.bmp");

				//�����ļ�
				tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
				CString strNemFileName;
				strNemFileName.Format(TEXT("\\%ld_%d.bmp"), GlobalUserData.dwUserID, pUploadFaceResult->dwFaceVer);

				//ɾ���ļ�
				try
				{
					CFile::Remove(strDirName + strNemFileName);
				}
				catch(...){}

				//ɾ���ļ�
				CString strOldFile;
				strOldFile.Format(TEXT("\\%ld_%ld.bmp"), GlobalUserData.dwUserID, GlobalUserData.dwCustomFaceVer);
				try
				{
					CFile::Remove(strDirName + strOldFile);
				}
				catch(...){}

				try
				{
					CFile::Rename(strDirName + strOldFileName, strDirName + strNemFileName);

					//���ð汾��
					GlobalUserData.dwCustomFaceVer = pUploadFaceResult->dwFaceVer;
					ASSERT(GlobalUserData.dwCustomFaceVer!=0);

					//Ͷ����Ϣ
					for ( INT nIndex = 0; nIndex < MAX_SERVER; nIndex++ ) 
					{
						//		CRoomViewItem *pRoomViewItem = ((CGameFrame*)AfxGetMainWnd())->m_pRoomViewItem[nIndex];
						//		if ( pRoomViewItem == NULL ) continue;
						//		pRoomViewItem->SendMessage(WM_UPDATE_FACE, GlobalUserData.dwCustomFaceVer, GlobalUserData.dwUserID);
					}
				}
				catch(...)
				{
					ShowMessageBox(TEXT("ͷ���ļ�����ʧ�ܣ���Ҫ���µ�¼�ſ�����ʾ��ͷ��"), MB_ICONINFORMATION);
				}

				//���½���
				//				((CGameFrame*)AfxGetMainWnd())->m_UserInfoView.UpdateUserInfo();

				//���ý���
				//	CGameFrame *pGameFrame = (CGameFrame *)AfxGetMainWnd() ;
				//	CDlgCustomFace &DlgCustomFace = pGameFrame->m_DlgCustomFace;
				//	if ( DlgCustomFace.m_hWnd != NULL && DlgCustomFace.IsWindowVisible() )
				{
					//		DlgCustomFace.UpdateControls();
					//		DlgCustomFace.SetFocus();
				}
			}

			//�ر�����
			m_ClientSocket->CloseSocket();

			return true;
		}
	case SUB_GP_DELETE_FACE_RESULT:		//ɾ�����
		{
			return true;

			ASSERT(sizeof(CMD_GP_DeleteFaceResult) == wDataSize);
			if ( sizeof(CMD_GP_DeleteFaceResult) != wDataSize)
			{
				return true;
			}


			//��ʾ��Ϣ
			CMD_GP_DeleteFaceResult *pDeleteFaceResult = (CMD_GP_DeleteFaceResult*)pBuffer;
			ShowMessageBox(pDeleteFaceResult->szDescribeMsg, MB_ICONINFORMATION);

			//�ɹ��ж�
			if ( pDeleteFaceResult->bOperateSuccess )
			{
				//��ȡ��Ϣ
				tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();

				//�ļ�Ŀ¼
				CString strDirName = CString(g_GlobalUnits.GetWorkDirectory()) + TEXT("\\CustomFace");

				//�ļ�����
				CString strFileName;
				strFileName.Format(TEXT("\\%ld_%d.bmp"), GlobalUserData.dwUserID, pDeleteFaceResult->dwFaceVer);

				//ɾ���ļ�
				try
				{
					CFile::Remove(strDirName + strFileName);
				}
				catch(...){}

				//���½���
				//				((CGameFrame*)AfxGetMainWnd())->m_UserInfoView.UpdateUserInfo();

				//���ý���
				//				CGameFrame *pGameFrame = (CGameFrame *)AfxGetMainWnd() ;
				//				CDlgCustomFace &DlgCustomFace = pGameFrame->m_DlgCustomFace;
				//				if ( DlgCustomFace.m_hWnd != NULL && DlgCustomFace.IsWindowVisible() )
				{
					//					DlgCustomFace.UpdateControls();
					//					DlgCustomFace.SetFocus();
				}

				//Ͷ����Ϣ
				for ( INT nIndex = 0; nIndex < MAX_SERVER; nIndex++ ) 
				{
					//					CRoomViewItem *pRoomViewItem = ((CGameFrame*)AfxGetMainWnd())->m_pRoomViewItem[nIndex];
					//					if ( pRoomViewItem == NULL ) continue;
					//					pRoomViewItem->SendMessage(WM_UPDATE_FACE, GlobalUserData.dwCustomFaceVer, GlobalUserData.dwUserID);
				}
			}

			//�ر�����
			m_ClientSocket->CloseSocket();

			return true;
		}
	case SUB_GP_MODIFY_INDIVIDUAL_RESULT:	//�޸Ľ��
		{
			return true;
		}
	case SUB_GP_UPDATE_USERINFO_SUCC:		//�����û���Ϣ
		{
			if (wDataSize != sizeof(CMD_GP_UserInfoSucc)) return true;

			CMD_GP_UserInfoSucc * pUserInfoSucc = (CMD_GP_UserInfoSucc *) pBuffer;
			if (NULL != pUserInfoSucc)
			{
				tagGlobalUserData & UserData = g_GlobalUnits.GetGolbalUserData();
				UserData.cbGender = pUserInfoSucc->cbGender;
				UserData.lGem = pUserInfoSucc->lGem;
				UserData.lMoney = pUserInfoSucc->lMoney;
				UserData.lGold = pUserInfoSucc->lGold;
				UserData.wFaceID = pUserInfoSucc->wFaceID;
				CopyMemory(UserData.szNickName, pUserInfoSucc->szNickName, sizeof(pUserInfoSucc->szNickName));

				m_UserImage.DestroyImage();
				char chImageData[32] = {0};
				CString szPath("");
				GetResPath("\\data\\image\\",szPath);
				if (0<UserData.wFaceID){
					::sprintf(chImageData, "Image_p_%d", UserData.wFaceID);
				}else{
					if (1==UserData.cbGender){
						::sprintf(chImageData, "Image_p_%d_w", UserData.wFaceID);
					}else{
						::sprintf(chImageData, "Image_p_%d_m", UserData.wFaceID);
					}
				}
				szPath.Append(chImageData);
				szPath.Append(TEXT(".png"));
				m_UserImage.LoadImage(szPath);
				//�����û�����
				UpdateUserImage();
			}

			//�ر�����
			m_ClientSocket->CloseSocket();

			return true;
		}
	case SUB_GP_GET_GIFT_GOLD_RESULT:
		{
			if (wDataSize != sizeof(CMD_GP_GiftGoldResult)) return true;
			CMD_GP_GiftGoldResult * pGiftGoldResult = (CMD_GP_GiftGoldResult *) pBuffer;
			if (NULL != pGiftGoldResult)
			{
				if (1==pGiftGoldResult->lRet)
				{
					tagGlobalUserData & UserData = g_GlobalUnits.GetGolbalUserData();
					UserData.lGold = UserData.lGold+pGiftGoldResult->dwGold;
					UserData.wGiftCount++;
					//�����û�����
					UpdateUserImage();
				}
				ShowMessageBox(TEXT(pGiftGoldResult->szDescribe),MB_ICONINFORMATION);
			}
			//�ر�����
			m_ClientSocket->CloseSocket();
			m_GameFlash.Play(false);
			return true;
		}
	case SUB_GP_CHECK_LINE:		// �����·
		{
			ASSERT(wDataSize == sizeof(CMD_GP_CheckLine));
			if (wDataSize != sizeof(CMD_GP_CheckLine)) 
				return true;

			KillTimer(TIME_WAIT_CHECK_LINE);
			CMD_GP_CheckLine * pCheckLine = (CMD_GP_CheckLine *) pBuffer;

			if (NULL != pCheckLine  &&  NULL != m_pDlgLogon  &&  NULL != m_pDlgLogon->GetSafeHwnd())
			{
				//ת����ַ
				TCHAR szClientIP[16]=TEXT("");
				BYTE * pClientIP=(BYTE *)&pCheckLine->dwClientIP;
				_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

				string strCountry, strLocation;
				m_IpFinder.GetAddressByIp(szClientIP, strCountry, strLocation);
				int iIpType = m_IpFinder.GetIpType(strLocation.c_str());
				m_pDlgLogon->SetCheckLineResult(iIpType);
			}

			//�ر�����
			m_ClientSocket->CloseSocket();
			return true;
		}
	case SUB_GP_DELETE_MACHINE_CODE:		// ɾ��������ɹ�
		{
			//�ر�����
			m_ClientSocket->CloseSocket();
			// �ɹ�ɾ������Ӧ��Ϸ�˳�
			KillTimer(IDI_TIMER_WAIT_END);
			SetTimer(IDI_TIMER_WAIT_END, 10, NULL);
			return true;
		}
	default:
		{
			ASSERT(FALSE);
			return false;
		}
	}
	return true;
}

bool CPlazaDlg::ConnectServer()
{
	//�Ϸ��ж�
	if ( m_ClientSocket.GetInterface() == NULL ) 
	{
		return false;
	}

	//��ַ����
	CRegKey RegServer;
	LPCTSTR pszServerIP=m_pDlgLogon->GetLogonServer();
	TCHAR szRegServer[256]=TEXT(""),szServerAddr[64]=TEXT("");
	_snprintf(szRegServer,sizeof(szRegServer),TEXT("%s\\%s"),REG_LOGON_SERVER,pszServerIP);

	if (RegServer.Open(HKEY_CURRENT_USER,szRegServer,KEY_READ)==ERROR_SUCCESS)
	{
		TCHAR szReadData[1024]=TEXT("");
		DWORD dwReadData=0L,dwDataType=0L,dwDataSize=sizeof(szReadData);
		LONG lErrorCode=RegServer.QueryValue(TEXT("ServerAddr"),&dwDataType,szReadData,&dwDataSize);
		if (lErrorCode==ERROR_SUCCESS) 
		{
			CXOREncrypt::CrevasseData(szReadData,szServerAddr,sizeof(szServerAddr));
			pszServerIP=szServerAddr;
		}
	}

	//���ӷ�����
	try
	{
		//���ӷ�����
		m_ClientSocket->CloseSocket();
		if (m_ClientSocket->Connect(pszServerIP,PORT_LOGON_SERVER)!=ERROR_SUCCESS)
		{
			throw TEXT("���������Ӵ��󣬿���������ϵͳ��û�гɹ����������磡");
		}
	}
	catch (LPCTSTR pszError)
	{
		UNREFERENCED_PARAMETER(pszError);
		return false;
	}

	return true;
}

void CPlazaDlg::OnCommandLogon()
{//ֱ�ӷ��ص���¼����
	//m_pDlgLogon->ShowWindow(SW_SHOW);
	if (3 != m_curTab)
	{//����ֱ�ӷ��ص���¼����
		m_bLogonPlaza=false;
		ShowTab(0);
	}
	else
	{//�Ѿ��������б����ˣ���Ҫ��������������˻ص���¼����.
		if (NULL != m_pRoomViewItemCreate && ::IsWindow(m_pRoomViewItemCreate->GetSafeHwnd()))
		{
			m_pRoomViewItemCreate->SendRequestCloseRoomMsg(0);
		}
	}
}

//���ص���¼����
void CPlazaDlg::OnBnExitLogoUI()
{
	tagGlobalUserData & GlobalUserData=g_GlobalUnits.GetGolbalUserData();
	//ɾ����¼
	g_GlobalUnits.DeleteUserCookie();

	//���ñ���
	memset(&GlobalUserData,0,sizeof(GlobalUserData));

	if (NULL != m_pRoomViewItemCreate)
	{
		m_pRoomViewItemCreate->ClearMemDC();
		delete m_pRoomViewItemCreate;
		m_pRoomViewItemCreate = NULL;
	}
	m_bLogonPlaza=false;
	ShowTab(0);
}

void CPlazaDlg::ShowTab( int nPage )
{
	//�ж�״̬
	tagGlobalUserData & GlobalUserData=g_GlobalUnits.GetGolbalUserData();
	if (GlobalUserData.dwUserID==0L && nPage >= 1)
	{
		ShowMessageBox(TEXT("����û�е�¼�����ȵ�¼��Ϸ������"),MB_ICONQUESTION);
		nPage=0;
	}

	HINSTANCE hInstance=AfxGetInstanceHandle();
	switch(nPage)
	{
	case 0:
		{//���ص�¼����
			//m_TaskDlg.ShowWindow(SW_HIDE);
			//m_shopDlg.ShowWindow(SW_HIDE);
			//m_doteyBoxDlg.ShowWindow(SW_HIDE);
			//		m_pDlgLogon->ShowWindow(SW_SHOW);

			//���ش���
			SetAllWndShow(false);

			//���ش���
			ShowWindow(SW_HIDE);

			//��ʾ��ˢ��
			m_pDlgLogon->ShowWindow(SW_SHOW);
			m_pDlgLogon->m_DlgLogonLogo->ShowWindow(SW_SHOW);
			//cxf 1025 m_pDlgLogon->InvalidateRectControl();

			KillTimer(IDI_TIMER_GAME_MSG_FLASH);
			KillTimer(TIME_SET_DLG_ALPHA_INTERVAL);
			m_uiCurAlphaValue = 0;
			m_hSaveNewCurWnd = NULL;
			m_hSaveOldCurWnd = NULL;
			m_byCurWndTag = 0;

			/*cxf CRect rect;
			GetWindowRect(&rect);
			::SetWindowPos(m_pDlgLogon->GetSafeHwnd(), 0, rect.left, rect.top, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
			*/
			m_pDlgLogon->OnPopRegisterDlg();
			m_pDlgLogon->SetActiveWindow();
			//m_gameListDlg.ShowWindow(SW_HIDE);
			//m_roomListDlg.ShowWindow(SW_HIDE);
			//if (m_pRoomViewItemCreate && ::IsWindow(m_pRoomViewItemCreate->GetSafeHwnd()))
			//{
			//	::ShowWindow(m_pRoomViewItemCreate->GetSafeHwnd(), SW_HIDE);
			//}
			m_pDlgLogon->SetFocus();
			//cxf 1022 m_ShowGameMsg.ClearAllMsg();
			UpdateMusicSound(false);
		}
		break;
	case 1:
		{//��Ϸ�б�
			//cxf 1024 m_TitleButtonList[3].BTImage.DestroyImage();
			//cxf 1024 m_TitleButtonList[3].BTImage.LoadImage(hInstance, TEXT("BT_PNG_IMAGE_SHOP"));
			//cxf 1024 InvalidateRect(&m_TitleButtonList[3].rcRect);
			m_pDlgLogon->ShowWindow(SW_HIDE);
			m_pDlgLogon->m_keydlg.ShowWindow(SW_HIDE);
			//m_roomListDlg.ShowWindow(SW_HIDE);
			ShowWindow(SW_SHOW);
			//if (m_pRoomViewItemCreate && ::IsWindow(m_pRoomViewItemCreate->GetSafeHwnd()))
			//{
			//	::ShowWindow(m_pRoomViewItemCreate->GetSafeHwnd(), SW_HIDE);
			//}
			m_bIsGamePlazyStatus = true;
			//::SetWindowPos(m_gameListDlg.GetSafeHwnd(), HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
			SetShowTab(1);
			//SetResetAlphaValue(m_gameListDlg.GetSafeHwnd());
		}
		break;
	case 2:	
		{//���ط����б�
			//cxf 1024 m_TitleButtonList[3].BTImage.DestroyImage();
			//cxf 1024 m_TitleButtonList[3].BTImage.LoadImage(hInstance, TEXT("BT_PNG_IMAGE_SHOP"));
			//cxf 1024 InvalidateRect(&m_TitleButtonList[3].rcRect);
			m_pDlgLogon->ShowWindow(SW_HIDE);
			//m_gameListDlg.ShowWindow(SW_HIDE);
			ShowWindow(SW_SHOW);
			//if (m_pRoomViewItemCreate && ::IsWindow(m_pRoomViewItemCreate->GetSafeHwnd()))
			//{ 
			//	::ShowWindow(m_pRoomViewItemCreate->GetSafeHwnd(), SW_HIDE);
			//}
			//::SetWindowPos(m_roomListDlg.GetSafeHwnd(), HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
			SetShowTab(2);
			m_bIsGamePlazyStatus = true;
			//SetResetAlphaValue(m_roomListDlg.GetSafeHwnd());
		}
		break;
	case 3:
		{//���������б�
			//m_TaskDlg.ShowWindow(SW_HIDE);
			//m_shopDlg.ShowWindow(SW_HIDE);
			//m_doteyBoxDlg.ShowWindow(SW_HIDE);
			if (m_pRoomViewItemCreate && ::IsWindow(m_pRoomViewItemCreate->GetSafeHwnd()))
			{
				//cxf 1024 m_TitleButtonList[3].BTImage.DestroyImage();
				//cxf 1024 m_TitleButtonList[3].BTImage.LoadImage(hInstance, TEXT("BT_PNG_IMAGE_HELP"));
				//cxf 1024 InvalidateRect(&m_TitleButtonList[3].rcRect);

				//CRect rect;
				//GetWindowRect(&rect);
				//m_pRoomViewItemCreate->SetWindowPos(NULL, rect.left+287,rect.top+81, 659, 626, 0);

				//m_roomListDlg.ShowWindow(SW_HIDE);
				//m_pDlgLogon->ShowWindow(SW_HIDE);
				//m_gameListDlg.ShowWindow(SW_HIDE);
				//::SetFocus(m_pRoomViewItemCreate->GetSafeHwnd());
				//::SetWindowPos(m_pRoomViewItemCreate->GetSafeHwnd(), HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
				//SetResetAlphaValue(m_pRoomViewItemCreate->GetSafeHwnd());
				SetShowTab(4);
				ShowWindow(SW_SHOW);
			}
			m_bIsGamePlazyStatus = true;
		}
		break;
	}
	m_curTab = nPage;
	//cxf 1024 m_TitleButtonList[0].btStatus = 3;
	//cxf 1024 InvalidateRect(&m_TitleButtonList[0].rcRect);
	// 	m_btnBeautySalon.ShowWindow(nPage==1?SW_SHOW:SW_HIDE);
	// 	m_btnGameShop.ShowWindow(nPage==1?SW_SHOW:SW_HIDE);
}

void CPlazaDlg::OnCallRoomList()
{//������뵥����Ϸ������
	if (m_RoomListCenterDlg.m_wKindID!=0) 
	{
		INT_PTR nIndex=0;
		CListServer * pListServer=NULL;
		WORD wSortID[50]={0},wSortCount=0;

		//�������
		m_RoomListCenterDlg.m_arrayGameRoomInfo.RemoveAll();
		do
		{ 
			pListServer=g_GlobalUnits.m_ServerListManager.EnumServerItem(nIndex++);
			if (pListServer==NULL)
			{
				//���뷿��
				m_RoomListCenterDlg.EnterRoom();
				SetShowHideInfo(3);//��ʾ���ش���

				break;
			}
			if(pListServer->m_pListKind->m_GameKind.wKindID==m_RoomListCenterDlg.m_wKindID) 
			{
				bool bExist=false;
				for (int i=0;i<wSortCount;++i)
				{
					if (pListServer->m_GameServer.wSortID==wSortID[i])
					{//��������Ѿ�������
						bExist=true;
						break;
					}
				}
				if (!bExist)
				{//����û����ͬ���� ��ӷ���������
					wSortID[wSortCount++]=pListServer->m_GameServer.wSortID;

					//������Ϣ��
					CGameRoomInfo GameRoomInfo;
					GameRoomInfo.m_pListServer=pListServer;
					m_RoomListCenterDlg.m_arrayGameRoomInfo.Add(GameRoomInfo);

					ASSERT(pListServer);
				}
			}			 
		} while (true); 
		//��ʾ����/��һҳ��ť
		m_btnRollback.ShowWindow(SW_SHOW);
		m_btnNextPage.ShowWindow(SW_SHOW);
	}

	/*cxf
	if (m_roomListDlg.m_wKindID!=0) 
	{
	INT_PTR nIndex=0;		 
	CListKind * pListKind=NULL;
	CListServer * pListServer=NULL;

	//�������
	m_roomListDlg.m_gameListManager->m_arrayGameRoomInfo.RemoveAll();
	do
	{ 
	pListServer=g_GlobalUnits.m_ServerListManager.EnumServerItem(nIndex++);
	if (pListServer==NULL)
	{
	if(m_roomListDlg.m_gameListManager->m_arrayGameRoomInfo.GetCount() != 0)
	{
	//���뷿��
	m_roomListDlg.EnterRoom();
	ShowTab(2);
	}
	break;
	}
	if(pListServer->m_pListKind->m_GameKind.wKindID==m_roomListDlg.m_wKindID) 
	{
	//������Ϣ��
	//pListKind->CListKind
	//pListKind->CListItem.m_ListItemArray

	CGameRoomInfo GameRoomInfo;
	GameRoomInfo.m_pListServer=pListServer;
	m_roomListDlg.m_gameListManager->m_arrayGameRoomInfo.Add(GameRoomInfo);

	ASSERT(pListServer);
	}			 
	//dwAllOnLineCount+=pListKind->GetItemInfo()->dwOnLineCount;
	} while (true); 
	}
	*/
}

void CPlazaDlg::OnCallGameList()
{//������Ϸ����.����Ϸ�б�
	ShowTab(1);

	//�����û�����
	UpdateUserImage();
}
LRESULT CPlazaDlg::OnCallInnoIssMsgClose(WPARAM wParam, LPARAM lParam)
{
	OnCancel();

	return 1;
}

LRESULT CPlazaDlg::OnCallCheckConnect(WPARAM wParam, LPARAM lParam)
{
	if (1 == (int)wParam  &&  1==(int)lParam)
	{
		m_bCheckLine = true;
	}

	SetTimer(TIME_WAIT_CHECK_LINE, 5000, NULL);
	OnCommandConnect();
	return 1;
}

//Inno ��װ�ű���������Ϣ ѡ����Ϸ
LRESULT CPlazaDlg::OnCallInnoIssMsgSelectGame(WPARAM wParam, LPARAM lParam)
{
	CListKind * pListKind=NULL;
	CListItem * pListItem=NULL;
	INT_PTR nListIndex=0;

	int iKind = (int)wParam;
	CString str;
	if ( m_RoomListCenterDlg.m_wKindID==iKind && m_RoomListCenterDlg.m_wSortID != -1) 
	{
		//�����б�
		CListServer * pListServer=NULL;
		//������֤
		if(g_GlobalUnits.m_ServerListManager.EnumServerItem(0) != NULL)
		{
			//Ѱ�ҷ���
			do
			{
				pListServer=g_GlobalUnits.m_ServerListManager.EnumServerItem(nListIndex++);
				if(pListServer != NULL)
				{
					if(m_RoomListCenterDlg.m_wKindID == pListServer->m_pListKind->m_GameKind.wKindID)
					{	
						pListKind=pListServer->m_pListKind;
						//Ŀ�귿��
						if(m_RoomListCenterDlg.m_wSortID != -1)
						{
							if(m_RoomListCenterDlg.m_wSortID == pListServer->m_GameServer.wSortID)
							{
								//��������
								if(pListServer->m_GameServer.dwMaxCount <= pListServer->m_GameServer.dwOnLineCount)
									continue;
								//���뷿��
								if(pListServer != NULL && !pListKind->m_bInstall)
								{
									CWinFileInfo WinFileInfo;
									tagGameKind * pGameKind=pListKind->GetItemInfo();
									pListKind->m_bInstall=WinFileInfo.OpenWinFile(pGameKind->szProcessName);
								}
								break;
							}
						}
						if(pListServer->m_pListKind->GetItemCount() == 0)break;
					}
				}
				else
					break;
			}while(true);
		}
		OnCallTableList();
		return 1;
	}
	return 1;

	/*cxf 1022
	CListKind * pListKind=NULL;
	CListItem * pListItem=NULL;
	INT_PTR nListIndex=0;

	int iKind = (int)wParam;
	CString str;
	if ( m_roomListDlg.m_wKindID==iKind && m_roomListDlg.m_nRoomNum != -1) 
	{
	//�����б�
	CListServer * pListServer=NULL;
	//������֤
	if(g_GlobalUnits.m_ServerListManager.EnumServerItem(0) != NULL)
	{
	//Ѱ�ҷ���
	do
	{
	pListServer=g_GlobalUnits.m_ServerListManager.EnumServerItem(nListIndex++);
	if(pListServer != NULL)
	{
	if(m_roomListDlg.m_wKindID == pListServer->m_pListKind->m_GameKind.wKindID)
	{	
	pListKind=pListServer->m_pListKind;
	//Ŀ�귿��
	if(m_roomListDlg.m_nRoomNum != -1)
	{
	if(m_roomListDlg.m_nRoomNum == pListServer->m_GameServer.wSortID)
	{
	//��������
	if(pListServer->m_GameServer.dwMaxCount <= pListServer->m_GameServer.dwOnLineCount)
	continue;
	//���뷿��
	if(pListServer != NULL && !pListKind->m_bInstall)
	{
	CWinFileInfo WinFileInfo;
	tagGameKind * pGameKind=pListKind->GetItemInfo();
	pListKind->m_bInstall=WinFileInfo.OpenWinFile(pGameKind->szProcessName);
	}
	break;
	}
	}
	if(pListServer->m_pListKind->GetItemCount() == 0)break;
	}
	}
	else
	break;
	}while(true);
	}
	OnCallTableList();
	return 1;
	}
	return 1;
	*/
}

void CPlazaDlg::OnCallTableList()
{//�л��������б�

	if (SISEQIU_ID == m_RoomListCenterDlg.m_wKindID)
	{
		if (!StartSiSeQiuProgram()) 
		{
			ShowMessageBox(_T("��ɫ������ʧ�ܣ�����ϵ����Ա��"), MB_ICONINFORMATION);
		}
		return;
	}
	else if (ACTIVE_ID == m_RoomListCenterDlg.m_wKindID)
	{
		if (!StartActivity(m_RoomListCenterDlg.m_wSortID))
		{
			ShowMessageBox(_T("�ר����ʧ�ܣ�����ϵ����Ա��"), MB_ICONINFORMATION);
		}
		return;
	}


	CListKind * pListKind=NULL;
	CListItem * pListItem=NULL;
	INT_PTR nListIndex=0;
	bool bIsLogonSuc = false;

	if (m_RoomListCenterDlg.m_wKindID!=0)
	{
		//�����б�
		CListServer * pListServer=NULL;
		//������֤
		if(g_GlobalUnits.m_ServerListManager.EnumServerItem(0) != NULL)
		{
			//�����Ϸ�Ƿ����
			g_GlobalUnits.m_ServerListManager.UpdateGameKind(m_RoomListCenterDlg.m_wKindID);
			//Ѱ�ҷ���
			do
			{
				pListServer=g_GlobalUnits.m_ServerListManager.EnumServerItem(nListIndex++);
				if(pListServer != NULL)
				{
					if(m_RoomListCenterDlg.m_wKindID == pListServer->m_pListKind->m_GameKind.wKindID)
					{	
						pListKind=pListServer->m_pListKind;

						//Ŀ�귿��
						if(m_RoomListCenterDlg.m_wSortID != -1)
						{
							if(m_RoomListCenterDlg.m_wSortID == pListServer->m_GameServer.wSortID)
							{
								//��������
								if((pListServer->m_GameServer.dwMaxCount <= pListServer->m_GameServer.dwOnLineCount)
									|| (pListServer->m_GameServer.dwOnLineCount >= pListServer->m_GameServer.dwMaxCount*DEF_NUM_VALUE))
								{
									//������������������������������������������������İٷ�֮95����ֱ�ӷ��ء�
									continue;
								}

								//���뷿��
								if(pListServer != NULL && pListKind->m_bInstall)
								{
									if(pListKind->m_GameKind.wKindID == pListServer->m_pListKind->m_GameKind.wKindID)
									{
										bIsLogonSuc = true;
										//�ڴ��жϸ��˵ı�ʯ�������������������������
										if(CreateTableList(pListServer))
										{
											//cxf ShowTab(3);
											break;
										}
										else
										{
											return;
										}
									}
								}
								else
								{
									//��������
									tagGameKind * pGameKind=pListKind->GetItemInfo();

									//��װ�ж�
									if ((!pListKind->m_bInstall)&&(pListKind->m_GameKind.dwMaxVersion!=0L))
									{
										TCHAR szProcessName[MODULE_LEN];
										CopyMemory(szProcessName, pGameKind->szProcessName, sizeof(szProcessName));
										TCHAR * pCh = strrchr(szProcessName, '.');
										if (pCh) *pCh = '\0';
										g_GlobalAttemper.DownLoadClient(szProcessName,pGameKind->wKindID,true,1);
										return;

										//������ʾ
										TCHAR szBuffer[512]=TEXT("");
										_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%s����û�а�װ�������Ƿ����أ�"),pGameKind->szKindName);
										//��ʾ��Ϣ
										if (ShowMessageBox(szBuffer,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1)==IDYES)
										{
											TCHAR szProcessName[MODULE_LEN];
											CopyMemory(szProcessName, pGameKind->szProcessName, sizeof(szProcessName));
											TCHAR * pCh = strrchr(szProcessName, '.');
											if (pCh) *pCh = '\0';
											g_GlobalAttemper.DownLoadClient(szProcessName,pGameKind->wKindID,true,1);
											//g_GlobalAttemper.DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,true);
										}
									}

									return;
								}
								break;
							}
						}

						if(pListServer->m_pListKind->GetItemCount() == 0)break;
					}
				}
				else
					break;
			}while(true);

			if (!bIsLogonSuc)
			{
				//��ʾ:������������
				char szBuffer[100]={0};
				sprintf(szBuffer, "�˷���������������ѡ�������������Ϸ��");
				ShowMessageBox(szBuffer, MB_ICONINFORMATION);
			}
		}

		/* cxf 
		//��������
		CListKind * pListKind=NULL;
		CListItem * pListItem=NULL;
		INT_PTR nListIndex=0;
		bool bIsLogonSuc = false;

		if (m_roomListDlg.m_wKindID!=0)
		{
		//�����б�
		CListServer * pListServer=NULL;
		//������֤
		if(g_GlobalUnits.m_ServerListManager.EnumServerItem(0) != NULL)
		{
		//Ѱ�ҷ���
		do
		{
		pListServer=g_GlobalUnits.m_ServerListManager.EnumServerItem(nListIndex++);
		if(pListServer != NULL)
		{
		if(m_roomListDlg.m_wKindID == pListServer->m_pListKind->m_GameKind.wKindID)
		{	
		pListKind=pListServer->m_pListKind;

		//Ŀ�귿��
		if(m_roomListDlg.m_nRoomNum != -1)
		{
		if(m_roomListDlg.m_nRoomNum == pListServer->m_GameServer.wSortID)
		{
		//��������
		if((pListServer->m_GameServer.dwMaxCount <= pListServer->m_GameServer.dwOnLineCount)
		|| (pListServer->m_GameServer.dwOnLineCount >= pListServer->m_GameServer.dwMaxCount*DEF_NUM_VALUE))
		{
		//������������������������������������������������İٷ�֮95����ֱ�ӷ��ء�
		continue;
		}

		//���뷿��
		if(pListServer != NULL && pListKind->m_bInstall)
		{
		if(pListKind->m_GameKind.wKindID == pListServer->m_pListKind->m_GameKind.wKindID)
		{
		bIsLogonSuc = true;
		//�ڴ��жϸ��˵ı�ʯ�������������������������
		if(CreateTableList(pListServer))
		{
		ShowTab(3);
		break;
		}
		else
		{
		return;
		}
		}
		}
		else
		{
		//��������
		tagGameKind * pGameKind=pListKind->GetItemInfo();

		//��װ�ж�
		if ((!pListKind->m_bInstall)&&(pListKind->m_GameKind.dwMaxVersion!=0L))
		{
		TCHAR szProcessName[MODULE_LEN];
		CopyMemory(szProcessName, pGameKind->szProcessName, sizeof(szProcessName));
		TCHAR * pCh = strrchr(szProcessName, '.');
		if (pCh) *pCh = '\0';
		g_GlobalAttemper.DownLoadClient(szProcessName,pGameKind->wKindID,true);
		return;

		//������ʾ
		TCHAR szBuffer[512]=TEXT("");
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%s����û�а�װ�������Ƿ����أ�"),pGameKind->szKindName);
		//��ʾ��Ϣ
		if (ShowMessageBox(szBuffer,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1)==IDYES)
		{
		TCHAR szProcessName[MODULE_LEN];
		CopyMemory(szProcessName, pGameKind->szProcessName, sizeof(szProcessName));
		TCHAR * pCh = strrchr(szProcessName, '.');
		if (pCh) *pCh = '\0';
		g_GlobalAttemper.DownLoadClient(szProcessName,pGameKind->wKindID,true);
		//g_GlobalAttemper.DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,true);
		}
		}

		return;
		}
		break;
		}
		}

		if(pListServer->m_pListKind->GetItemCount() == 0)break;
		}
		}
		else
		break;
		}while(true);

		if (!bIsLogonSuc)
		{
		//��ʾ:������������
		char szBuffer[100]={0};
		sprintf(szBuffer, "�˷���������������ѡ�������������Ϸ��");
		ShowMessageBox(szBuffer, MB_ICONINFORMATION);
		}
		}
		*/

		////���뷿��
		//if(pListServer != NULL && pListKind->m_bInstall)
		//{
		//	if(pListKind->m_GameKind.wKindID == pListServer->m_pListKind->m_GameKind.wKindID)
		//	{
		//		if(CreateTableList(pListServer))
		//			ShowTab(3);
		//	}
		//}
		//else
		//	{
		//		//��������
		//		tagGameKind * pGameKind=pListKind->GetItemInfo();

		//		//��װ�ж�
		//		if ((pListKind->m_bInstall==false)&&(pListKind->m_GameKind.dwMaxVersion!=0L))
		//		{
		//			//������ʾ
		//			TCHAR szBuffer[512]=TEXT("");
		//			_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%s����û�а�װ�������Ƿ����أ�"),pGameKind->szKindName);

		//			//��ʾ��Ϣ
		//			if (ShowMessageBox(szBuffer,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1)==IDYES)
		//			{
		//				g_GlobalAttemper.DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,true);
		//			}
		//		}

		//		return;
		//	}
	}
}

LRESULT CPlazaDlg::OnCallShowCustomer(WPARAM wParam, LPARAM lParam)
{
	CMD_GR_Message * pMessage=(CMD_GR_Message *)wParam;
	TCHAR szContent[1024] = _T("");
	lstrcpy(szContent, pMessage->szContent);

	if (SMT_CUSTOMER_FRAME&pMessage->wMessageType)
	{
		int iResult = ShowMessageBox(szContent, MB_ICONINFORMATION|MB_CUSTOMER);
		if (IDOK == iResult)
		{
			ShellExecute(NULL,"open",CInitData::s_cContactService,"",NULL,SW_SHOW);
		}
	}
	else
	{
		ShowMessageBox(szContent, MB_ICONINFORMATION|MB_OK);
	}

	return 1;
}

int CPlazaDlg::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	//m_ServerItemView.Create(WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,IDC_SERVER_ITEM_VIEW);
	//m_ServerItemView.InitServerItemView(/*this*/);
	//g_GlobalUnits.m_ServerListManager.InitServerListManager(&m_ServerItemView);
	return 0;
}

BOOL CPlazaDlg::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
	/*cxf 1024	if (m_bIsDrawBack)
	{
	CRect rtClientRect;
	GetClientRect(&rtClientRect);
	CDC pMemDC;
	pMemDC.CreateCompatibleDC(pDC);
	CBitmap hBitmap;
	hBitmap.CreateCompatibleBitmap(pDC, rtClientRect.Width(), rtClientRect.Height());
	pMemDC.SelectObject(&hBitmap);
	pMemDC.SetBkMode(TRANSPARENT);
	pMemDC.FillSolidRect(0,0,960, 720,RGB(0,0,0));

	//CImageHandle handle(&m_ImageBack);
	//m_ImageBack.AlphaDrawImage(&pMemDC, 0, 0, RGB(255, 0, 255));
	if (!m_ImageBack.IsNull())
	{
	m_ImageBack.DrawImage(&pMemDC,0,0);
	}

	pDC->BitBlt(287,81, 659, 626, &pMemDC, 287,81, SRCCOPY);
	DeleteObject(&hBitmap);
	pMemDC.DeleteDC();
	}
	*/
}

//λ����Ϣ
void CPlazaDlg::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	m_bIsShowXJInfo = false;
	CPaintDC dc(this);
	m_memdc.DeleteDC();
	CBitmap bmp;
	m_memdc.CreateCompatibleDC(&dc);
	bmp.CreateCompatibleBitmap(&dc, cx, cy);
	m_memdc.SelectObject(bmp);
	InvalidateRect(NULL);
	bmp.DeleteObject();
	return;
}


void CPlazaDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	__super::OnMouseMove(nFlags, point);

	if (m_UserXJRect.PtInRect(point))
	{
		if (!m_bIsShowXJInfo)
		{
			m_bIsShowXJInfo = true;
			InvalidateRect(&m_UserXJInfoRect);
		}
	}
	else
	{
		if (m_bIsShowXJInfo)
		{
			InvalidateRect(&m_UserXJInfoRect);
		}
		m_bIsShowXJInfo = false;
	}
	/*cxf 
	if (m_UserXJRect.PtInRect(point))
	{
	WORD wClass = 0;
	tagGlobalUserData & UserData=g_GlobalUnits.GetGolbalUserData();
	CountXJClass(UserData.lWinCount, (UserData.lWinCount+UserData.lLostCount+UserData.lDrawCount+UserData.lFleeCount), wClass);
	wClass =1 ;
	if (wClass>=1 && wClass<=5)
	{
	m_bIsShowXJInfo = true;
	InvalidateRect(&m_UserXJInfoRect);
	}
	}
	else
	{
	if (m_bIsShowXJInfo)
	{
	InvalidateRect(&m_UserXJInfoRect);
	}
	m_bIsShowXJInfo = false;
	}

	if (m_rcGameMsgArea.PtInRect(point))
	{
	m_byFlashMsgStatus = 0;
	if (m_bIsFlashMsg)
	{
	m_GameMsgImage.DestroyImage();
	HINSTANCE hInstance = AfxGetInstanceHandle();
	m_GameMsgImage.LoadImage(hInstance, TEXT("PNG_GAME_MSG_MSG_MOVE_BACK"));
	m_byFlashMsgStatus = 1;
	}
	InvalidateRect(&m_rcGameMsgArea);
	}
	else
	{
	if (0 != m_byFlashMsgStatus)
	{
	m_GameMsgImage.DestroyImage();
	HINSTANCE hInstance = AfxGetInstanceHandle();
	m_GameMsgImage.LoadImage(hInstance, TEXT("PNG_IMAGE_GAME_MSG"));
	m_byFlashMsgStatus = 0;
	InvalidateRect(&m_rcGameMsgArea);
	}
	}*/
	/*cxf 1024
	BYTE bySaveIndex = 255;
	for (int i=0; i<4; i++)
	{
	if (m_TitleButtonList[i].rcRect.PtInRect(point))
	{
	if (3 != m_TitleButtonList[i].btStatus)
	{
	bySaveIndex = i;
	break;
	}
	}
	else
	{
	if (3 != m_TitleButtonList[i].btStatus)
	{
	if (0 != m_TitleButtonList[i].btStatus)
	{
	m_TitleButtonList[i].btStatus = 0;
	InvalidateRect(&m_TitleButtonList[i].rcRect);
	}
	}
	}
	}
	if (bySaveIndex>=0 && bySaveIndex<4)
	{
	for (int i=0; i<4; i++)
	{
	if (3 != m_TitleButtonList[i].btStatus)
	{
	if (i==bySaveIndex)
	{
	if (2!=m_TitleButtonList[i].btStatus)
	{
	if (1 != m_TitleButtonList[i].btStatus)
	{
	m_TitleButtonList[i].btStatus = 1;
	InvalidateRect(&m_TitleButtonList[i].rcRect);
	}
	}
	}
	else
	{
	if (0 != m_TitleButtonList[i].btStatus)
	{
	m_TitleButtonList[i].btStatus = 0;
	InvalidateRect(&m_TitleButtonList[i].rcRect);
	}
	}
	}
	}
	}
	*/
}

void CPlazaDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bNewComerGuide && m_rcNewComerGuideArea.PtInRect(point))
	{//����ָ��
		OnBnClickedNewComerGuide();
		return;
	}
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	/*cxf  1024
	BYTE bySaveIndex = 255;
	bool bIsMove = true;
	for (int i=0; i<4; i++)
	{
	if (m_TitleButtonList[i].rcRect.PtInRect(point))
	{
	bIsMove = false;
	if (3 != m_TitleButtonList[i].btStatus)
	{
	bySaveIndex = i;
	break;
	}
	}
	}
	if (bySaveIndex>=0 && bySaveIndex<4)
	{
	for (int i=0; i<4; i++)
	{
	if (3 != m_TitleButtonList[i].btStatus)
	{
	m_TitleButtonList[i].btStatus = (i==bySaveIndex)?2:0;
	InvalidateRect(&m_TitleButtonList[i].rcRect);
	}
	}
	}

	if (bIsMove)
	{
	if (point.y < 80)
	{
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	}
	}
	*/
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	/*cxf

	if (m_rcGameMsgArea.PtInRect(point))
	{
	KillTimer(IDI_TIMER_GAME_MSG_FLASH);
	m_bIsFlashMsg = true;
	InvalidateRect(&m_rcGameMsgArea);
	m_ShowGameMsg.DoModal();
	}*/

	__super::OnLButtonDown(nFlags, point);
}

void CPlazaDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	__super::OnLButtonUp(nFlags, point);

	/*cxf BYTE bySaveIndex = 255;
	for (int i=0; i<4; i++)
	{
	if (m_TitleButtonList[i].rcRect.PtInRect(point))
	{
	if (2 == m_TitleButtonList[i].btStatus)
	{
	bySaveIndex = i;
	break;
	}
	}
	}
	if (bySaveIndex>=0 && bySaveIndex<4)
	{
	for (int i=0; i<4; i++)
	{
	if (i==bySaveIndex)
	{
	if (3 != m_TitleButtonList[i].btStatus)
	{
	m_TitleButtonList[i].btStatus = 3;
	ExeButtonEvent(i);
	}
	}
	else
	{
	m_TitleButtonList[i].btStatus = 0;
	}
	InvalidateRect(&m_TitleButtonList[i].rcRect);
	}
	}
	*/

	
}

//��������
bool CPlazaDlg::CreateTableList(CListServer * pListServer)
{
	if (FALSE == IsCanIntervalConnect())
		return false;

	//Ч�����
	ASSERT(pListServer!=NULL);
	CListKind * pListKind=pListServer->GetListKind();
	tagGameKind * pGameKind=pListKind->GetItemInfo();
	tagGameServer * pGameServer=pListServer->GetItemInfo();

	////���뷿����Ŀ
	//if (m_pRoomViewItem[CountArray(m_pRoomViewItem)-1]!=NULL)
	//{
	//	ShowMessageBox(TEXT("�������Ϸ����̫���ˣ���رղ�����Ϸ�������ԣ�"),MB_ICONINFORMATION);
	//	return NULL;
	//}

	if (NULL != m_pRoomViewItemCreate)
	{
		SafeDelete(m_pRoomViewItemCreate);
	}

	//Ѱ�ҷ���
	CRoomViewItem * pRoomViewItem=m_pRoomViewItemCreate;

	//��ȡ�汾
	CWinFileInfo WinFileInfo;
	if (WinFileInfo.OpenWinFile(pGameKind->szProcessName)==false)
	{

		TCHAR szProcessName[MODULE_LEN];
		CopyMemory(szProcessName, pGameKind->szProcessName, sizeof(szProcessName));
		TCHAR * pCh = strrchr(szProcessName, '.');
		if (pCh) *pCh = '\0';
		g_GlobalAttemper.DownLoadClient(szProcessName,pGameKind->wKindID,true, 1);

		return false;

		TCHAR szBuffer[512]=TEXT("");
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%s����û�а�װ�������Ƿ����أ�"),pGameKind->szKindName);
		int nResult=ShowMessageBox(szBuffer,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1);
		if (nResult==IDYES) 
		{
			TCHAR szProcessName[MODULE_LEN];
			CopyMemory(szProcessName, pGameKind->szProcessName, sizeof(szProcessName));
			TCHAR * pCh = strrchr(szProcessName, '.');
			if (pCh) *pCh = '\0';
			g_GlobalAttemper.DownLoadClient(szProcessName,pGameKind->wKindID,true,1);
			//g_GlobalAttemper.DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,true);
		}
		return false;
	}

	//��ȡ�汾
	DWORD dwFileVerMS=0L,dwFileVerLS=0L;
	WinFileInfo.GetFileVersion(dwFileVerMS,dwFileVerLS);

	//�汾����
	BYTE cbFileVer1=(BYTE)(HIWORD(dwFileVerMS));
	BYTE cbFileVer2=(BYTE)(LOWORD(dwFileVerMS));
	BYTE cbFileVer3=(BYTE)(HIWORD(dwFileVerLS));
	BYTE cbFileVer4=(BYTE)(LOWORD(dwFileVerLS));
	BYTE cbListVer1=(BYTE)(LOWORD(LOWORD(pGameKind->dwMaxVersion)));
	BYTE cbListVer2=(BYTE)(HIBYTE(LOWORD(pGameKind->dwMaxVersion)));
	BYTE cbListVer3=(BYTE)(LOBYTE(HIWORD(pGameKind->dwMaxVersion)));
	BYTE cbListVer4=(BYTE)(HIBYTE(HIWORD(pGameKind->dwMaxVersion)));

	//�жϰ汾
	if ( (cbFileVer1!=cbListVer1) || (cbFileVer2!=cbListVer2) || (cbFileVer3!=cbListVer3) )
	{
		TCHAR szProcessName[MODULE_LEN];
		CopyMemory(szProcessName, pGameKind->szProcessName, sizeof(szProcessName));
		TCHAR * pCh = strrchr(szProcessName, '.');
		if (pCh) *pCh = '\0';
		g_GlobalAttemper.DownLoadClient(szProcessName,pGameKind->wKindID,true,1);

		return false;

		TCHAR szBuffer[512]=TEXT("");
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("��%s���Ѿ�����Ϊ %ld.%ld.%ld.%ld �汾�����İ汾���ܼ���ʹ�ã������Ƿ����أ�"),pGameKind->szKindName,
			cbListVer1,cbListVer2,cbListVer3,cbListVer4);
		int nResult=ShowMessageBox(szBuffer,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1);
		if (nResult==IDYES) 
		{
			TCHAR szProcessName[MODULE_LEN];
			CopyMemory(szProcessName, pGameKind->szProcessName, sizeof(szProcessName));
			TCHAR * pCh = strrchr(szProcessName, '.');
			if (pCh) *pCh = '\0';
			g_GlobalAttemper.DownLoadClient(szProcessName,pGameKind->wKindID,true,1);
			//g_GlobalAttemper.DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,true);
		}
		return false;
	}

	//���ݰ汾
	if (cbFileVer4!=cbListVer4)
	{
		TCHAR szProcessName[MODULE_LEN];
		CopyMemory(szProcessName, pGameKind->szProcessName, sizeof(szProcessName));
		TCHAR * pCh = strrchr(szProcessName, '.');
		if (pCh) *pCh = '\0';
		g_GlobalAttemper.DownLoadClient(szProcessName,pGameKind->wKindID,true,2);

		return false;

		TCHAR szBuffer[512]=TEXT("");
		_snprintf(szBuffer,
			sizeof(szBuffer),
			TEXT("��%s���Ѿ�����Ϊ %ld.%ld.%ld.%ld �汾�������Ƿ�����������"),
			pGameKind->szKindName,
			cbListVer1,
			cbListVer2,
			cbListVer3,
			cbListVer4);

		int nResult=ShowMessageBox(szBuffer,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON1);
		if (nResult==IDYES) 
		{
			TCHAR szProcessName[MODULE_LEN];
			CopyMemory(szProcessName, pGameKind->szProcessName, sizeof(szProcessName));
			TCHAR * pCh = strrchr(szProcessName, '.');
			if (pCh) *pCh = '\0';
			g_GlobalAttemper.DownLoadClient(szProcessName,pGameKind->wKindID,true,2);
			//g_GlobalAttemper.DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,true);
			return false;
		}
	}

	//����������ͼ(�����б�)
	try
	{
		pRoomViewItem=new CRoomViewItem;
		if (pRoomViewItem==NULL) 
		{
			return false;
		}
		pRoomViewItem->Create(IDD_TABLE_LIST,this);//���������б�Ի���
		pRoomViewItem->ShowWindow(SW_HIDE);
		//pRoomViewItem->m_gameListManager = &m_gameListManager;//�����б�
		if (pRoomViewItem->InitRoomViewItem(pListServer)==false) 
		{
			throw TEXT("��Ϸ�����ʼ��ʧ�ܣ�");
		}
	}
	catch (...) 
	{ 
		SafeDelete(pRoomViewItem); 
	}
	if (pRoomViewItem==NULL)
	{
		ShowMessageBox(TEXT("��Ϸ���䴴��ʧ��"),MB_ICONQUESTION);
		PostMessage(WM_COMMAND,IDM_LOGON_PLAZA,0);
		return false;
	}

	//osc update
	//�ж��Լ���ʯ���������
	tagGlobalUserData& tagUserInfo = g_GlobalUnits.GetGolbalUserData();
	if (tagUserInfo.lGold < pGameServer->lEnterMoney)
	{
		if (tagUserInfo.dwGrade<6 && tagUserInfo.wGiftCount<1)
		{//���ֱ������ͽ��
			if (tagUserInfo.lGold<3000)
			{
				m_ConfirmGold.SetMode(1);
				m_ConfirmGold.DoModal();
				m_bIsGetGlod=true;
				OnCommandConnect();
			}
			else
			{
				ShowMessageBox(TEXT("���Ľ�Ҳ���,���ܽ���˷��䣡"),MB_ICONQUESTION);
			}
		}
		else
		{
			m_ConfirmGold.SetMode(0);
			m_ConfirmGold.DoModal();
			//��ʾ:������������
			/*char szBuffer[128]={0};
			sprintf(szBuffer, "���Ľ�Ҳ���,���ܽ���˷��䣡");
			ShowMessageBox(szBuffer, MB_ICONINFORMATION);*/
		}
		return false;
	}

	//�ж��Լ��ȼ�̫��
	if (tagUserInfo.dwGrade > pGameServer->lRestrictGrade)
	{
		m_ConfirmGrade.DoModal();
		//��ʾ:�ȼ�̫��
		/*char szBuffer[128]={0};
		sprintf(szBuffer, "���ĵȼ�̫��,���ܽ���˷��䣡");
		ShowMessageBox(szBuffer, MB_ICONINFORMATION);*/
		return false;
	}

	//������Ϸ
	bool bSuccess=pRoomViewItem->ConnectGameServer();
	if (bSuccess==false || pRoomViewItem->GetLogonFull())//�жϷ����Ƿ�����
	{
		pRoomViewItem->DestroyWindow();
		SafeDelete(pRoomViewItem);
		return false;
	}

	//���ñ���
	m_pRoomViewItemCreate=pRoomViewItem;
	CRect plazarect;
	GetWindowRect(&plazarect);
	::MoveWindow(m_pRoomViewItemCreate->GetSafeHwnd(),plazarect.left,plazarect.top, 960, 720, FALSE);
	//cxf ::MoveWindow(m_pRoomViewItemCreate->GetSafeHwnd(),287,81, 659, 626, FALSE);

	//HWND hWnd=NULL;
	//hWnd=m_pRoomViewItemCreate->GetSafeHwnd();

	//������ʾ
	CString strBuffer;
	strBuffer.Format(TEXT("�������ӷ��䣬���Ժ�..."));
	g_GlobalAttemper.ShowStatusMessage(strBuffer,pRoomViewItem);

	return true;

}

void CPlazaDlg::OnMove(int x, int y)
{
	__super::OnMove(x, y);

	if (::IsWindow(m_pRoomViewItemCreate->GetSafeHwnd()) && ::IsWindowVisible(m_pRoomViewItemCreate->GetSafeHwnd()))
	{
		CRect rcClientRect;
		GetWindowRect(&rcClientRect);
		m_pRoomViewItemCreate->SetWindowPos(NULL,rcClientRect.left,rcClientRect.top,960,720,SWP_SHOWWINDOW);
		//m_pRoomViewItemCreate->MoveWindow(rcClientRect,false);
	}

	if (NULL != m_pActivity  &&  ::IsWindow(m_pActivity->GetSafeHwnd())  &&  ::IsWindowVisible(m_pActivity->GetSafeHwnd()))
	{
		CRect rcClientRect;
		GetWindowRect(&rcClientRect);
		m_pActivity->SetWindowPos(&wndTop,rcClientRect.left+6,rcClientRect.top+30, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW);
	}

	// TODO: �ڴ˴������Ϣ����������
	/*cxf if (::IsWindow(m_pDlgLogon->GetSafeHwnd()) && ::IsWindowVisible(m_strCurrentPathPlaza->GetSafeHwnd()))
	{
	m_pDlgLogon->UpdateKeyPos();
	}*/
}

void CPlazaDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	//AfxMessageBox(TEXT("CPlazaDlg::OnOK"));
	__super::OnOK();
}

void CPlazaDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
#if !defined(_DEBUG)
	m_bDeleteMachineCode = true;
	OnCommandConnect();
	SetTimer(IDI_TIMER_WAIT_END, 5000, NULL);
#else
	SetTimer(IDI_TIMER_WAIT_END, 10, NULL);
#endif
}

void CPlazaDlg::OnPaint()
{
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� __super::OnPaint()
	if (NULL != m_memdc.m_hDC)
	{
		CPaintDC dc(this); // device context for painting
		//cxf
		////���Ʊ���
		if (!m_ImageBack.IsNull())
		{
			m_ImageBack.DrawImage(&m_memdc,0,0);
		}
		//CImageHandle handle(&m_ImageBack);
		//m_ImageBack.AlphaDrawImage(&m_memdc, 0, 0, RGB(255, 0, 255));

		//���Ƹ�������
		tagGlobalUserData & UserData=g_GlobalUnits.GetGolbalUserData();
		if (UserData.wFaceID>=0)
		{
			if (!m_UserImage.IsNull())
			{
				//cxf m_UserImage.DrawImage(&m_memdc, 14, 82);
				m_UserImage.DrawImage(&m_memdc, 756, 181);
			}
		}

		//������Ϸ�б����
		if (!m_ImageTitle.IsNull())
		{
			WORD wWidth=m_ImageTitle.GetWidth()/16;
			m_ImageTitle.DrawImage(&m_memdc,430,148,wWidth,m_ImageTitle.GetHeight(),m_wTitlePos*wWidth,0);
		}
		//�滭����ָ������
		if (m_bNewComerGuide && !m_ImageNewComerGuide.IsNull())
		{
			WORD wWidth=m_ImageNewComerGuide.GetWidth()/2;
			m_ImageNewComerGuide.DrawImage(&m_memdc,m_rcNewComerGuideArea.left,m_rcNewComerGuideArea.top,wWidth,m_ImageNewComerGuide.GetHeight(),
				m_wNewComerGuideActionNumber*wWidth,0);
		}
		/*cxf
		//���Ƹ�����Ϣ������
		if (!m_UserInfo.IsNull())
		{
		m_UserInfo.DrawImage(&m_memdc, 13, 497);
		}

		//������Ϣ
		if (!m_GameMsgImage.IsNull() && m_bIsFlashMsg)
		{
		m_GameMsgImage.DrawImage(&m_memdc, m_rcGameMsgArea.left, m_rcGameMsgArea.top);
		}*/

		m_memdc.SetBkMode(TRANSPARENT);

		//���� DC
		CFont* pOldFont= m_memdc.SelectObject(&m_fontNickName);
		CRect rcRect;
		rcRect.left = 780+(150-m_memdc.GetTextExtent(UserData.szNickName).cx)/2;
		rcRect.top = 365+(23-m_memdc.GetTextExtent(UserData.szNickName).cy)/2;
		rcRect.right = rcRect.left + 150;
		rcRect.bottom = rcRect.top + 20;
		//�����ǳ�
		DrawTextString(&m_memdc, UserData.szNickName, RGB(109, 209, 48), RGB(0, 0, 0), rcRect.left, rcRect.top);
		m_memdc.SelectObject(&pOldFont);
		pOldFont = NULL;

		//���Ƶȼ�
		rcRect.left = 770;
		rcRect.top = 397;
		rcRect.right = rcRect.left + 50;
		rcRect.bottom = rcRect.top + 14;

		m_ImageNums.SetNums(UserData.dwGrade);
		m_ImageNums.SetShowRect(rcRect);
		m_ImageNums.DrawImageNums(&m_memdc);

		m_ImageNums.SetNumAlign(IA_LEFT);
		//�ر���
		rcRect.left = 840;
		rcRect.top = 432;
		rcRect.right = rcRect.left + 100;
		rcRect.bottom = rcRect.top + 14;
		m_ImageNums.SetNums(UserData.lMoney);
		m_ImageNums.SetShowRect(rcRect);
		m_ImageNums.DrawImageNums(&m_memdc);

		//���ƽ��
		rcRect.left = 840;
		rcRect.top = 457;
		rcRect.right = rcRect.left + 100;
		rcRect.bottom = rcRect.top + 14;
		m_ImageNums.SetNums(UserData.lGold);
		m_ImageNums.SetShowRect(rcRect);
		m_ImageNums.DrawImageNums(&m_memdc);

		//��ʯ
		rcRect.left = 840;
		rcRect.top = 482;
		rcRect.right = rcRect.left + 100;
		rcRect.bottom = rcRect.top + 14;
		m_ImageNums.SetNums(UserData.lGem);
		m_ImageNums.SetShowRect(rcRect);
		m_ImageNums.DrawImageNums(&m_memdc);

		m_ImageNums.SetNumAlign(IA_CENTER);

		//����,��ǰ����ֵ���ܾ���ֵ�����ж���
		rcRect.left = 814;
		rcRect.top = 394;
		rcRect.right = rcRect.left + 92;
		rcRect.bottom = rcRect.top + 18;

		/*cxf
		//���Ʊ���
		if (!m_Exp1Image.IsNull())
		{
		m_Exp1Image.DrawImage(&m_memdc, rcRect.left, rcRect.top);
		}*/

		//UserData.dwClass = 4;
		//�����ܾ���ֵ.
		/*DWORD dwTotalExp=m_pGradeInfo[UserData.dwGrade].iMaxExp;
		DWORD dwTotalValue = 0;
		for (DWORD i=1; i<=UserData.dwGrade; i++)
		{
		dwTotalValue += i;
		}
		DWORD dwTotalExp = sqrt((float)(UserData.dwGrade+1))*dwTotalValue+200;
		if (UserData.dwExperience>=dwTotalExp)
		{
		dwTotalExp = UserData.dwExperience;
		}
		if (0==dwTotalExp)
		{
		dwTotalExp = 1;
		}*/
		int dwTotalExp=m_pGradeInfo[UserData.dwGrade-1].iMaxExp;
		//����ǰ��
		if (!m_Exp2Image.IsNull())
		{
			WORD wExp2ImageW = m_Exp2Image.GetWidth();
			WORD wExp2ImageH = m_Exp2Image.GetHeight();
			DWORD dwDrawWidth = UserData.dwExperience*(wExp2ImageW/(float)dwTotalExp);
			m_Exp2Image.DrawImage(&m_memdc, rcRect.left, rcRect.top, dwDrawWidth, wExp2ImageH, 0, 0, dwDrawWidth, wExp2ImageH);
		}
		//��������
		char chExpValue[32]={0};
		::sprintf(chExpValue, "%d\/%d", UserData.dwExperience, dwTotalExp);
		//���� DC
		pOldFont= m_memdc.SelectObject(&m_fontXPValue);

		rcRect.left = 814+(92-m_memdc.GetTextExtent(chExpValue).cx)/2;
		rcRect.top = 395;
		rcRect.right = rcRect.left + 92;
		rcRect.bottom = rcRect.top + 20;
		DrawTextString(&m_memdc, chExpValue, RGB(206, 255, 1), RGB(0, 0, 0), rcRect.left, rcRect.top);
		m_memdc.SelectObject(&pOldFont);
		pOldFont = NULL;

		//�����Ǽ�
		DrawXJInfo(&m_memdc);

		//���ư�ť
		//cxf DrawTitleButton(&m_memdc);

		//��������
		rcRect.left = 799;
		rcRect.top = 97;
		rcRect.right = rcRect.left + 12;
		rcRect.bottom = rcRect.top + 14;
		m_ImageNumsTel.SetSign(false);
		m_ImageNumsTel.SetNumAlign(IA_RIGHT);
		m_ImageNumsTel.SetNums(0);
		m_ImageNumsTel.SetShowRect(rcRect);
		m_ImageNumsTel.DrawImageNums(&m_memdc);
		rcRect.left = 811;
		rcRect.top = 97;
		rcRect.right = rcRect.left + 32;
		rcRect.bottom = rcRect.top + 14;
		m_ImageNumsTel.SetNumAlign(IA_LEFT);
		m_ImageNumsTel.SetNums(755);
		m_ImageNumsTel.SetShowRect(rcRect);
		m_ImageNumsTel.DrawImageNums(&m_memdc);
		//���Ƶ绰
		rcRect.left = 843;
		rcRect.top = 97;
		rcRect.right = rcRect.left + 80;
		rcRect.bottom = rcRect.top + 14;
		m_ImageNumsTel.SetSign(true);
		m_ImageNumsTel.SetNums(-86056703);
		m_ImageNumsTel.SetShowRect(rcRect);
		m_ImageNumsTel.DrawImageNums(&m_memdc);

		CRect rect;
		GetClientRect(&rect);
		dc.BitBlt(0, 0, rect.Width(), rect.Height(), &m_memdc, 0, 0, SRCCOPY);
	}
}

//�����Ǽ�
void CPlazaDlg::DrawXJInfo(CDC* pDC)
{
	if (NULL == pDC)
	{
		return;
	}

	tagGlobalUserData & UserData=g_GlobalUnits.GetGolbalUserData();
	DWORD dwTotalGameNums = (UserData.lWinCount+UserData.lLostCount+UserData.lDrawCount+UserData.lFleeCount);
	HINSTANCE hInstance = AfxGetInstanceHandle();

	DWORD dwGrade = UserData.dwGrade-1;//�ȼ�
	if (m_bIsShowXJInfo && !m_XJInfoBackImage.IsNull())
	{
		m_XJInfoBackImage.DrawImage(pDC, m_UserXJInfoRect.left, m_UserXJInfoRect.top);
		//���� DC
		CFont* pOldFont= pDC->SelectObject(&m_fontDrawXJ);
		//���Ƽ������
		char chXJData[64]={0};

		if (dwGrade<0 || dwGrade>m_iGradeInfoDiv)
		{
			return;
		}
		pDC->SetTextColor(RGB(255, 0, 255));
		CopyMemory(chXJData,m_pGradeInfo[dwGrade].strName,sizeof(m_pGradeInfo[dwGrade].strName));

		pDC->TextOut(m_UserXJInfoRect.left+43, m_UserXJInfoRect.top+8, chXJData);


		//������Ϸ�ܴ���
		pDC->SetTextColor(RGB(0, 0, 0));
		::memset(chXJData, 0, sizeof(chXJData));
		::sprintf(chXJData, "��Ϸ�ܴ�����");
		pDC->TextOut(m_UserXJInfoRect.left+4, m_UserXJInfoRect.top+25, chXJData);
		pDC->SetTextColor(RGB(255, 0, 255));
		::memset(chXJData, 0, sizeof(chXJData));
		::sprintf(chXJData, "%d", dwTotalGameNums);
		pDC->TextOut(m_UserXJInfoRect.left+78, m_UserXJInfoRect.top+25, chXJData);

		//����ʤ������
		pDC->SetTextColor(RGB(0, 0, 0));
		::memset(chXJData, 0, sizeof(chXJData));
		::sprintf(chXJData, "ʤ��������");
		pDC->TextOut(m_UserXJInfoRect.left+15, m_UserXJInfoRect.top+42, chXJData);
		pDC->SetTextColor(RGB(255, 0, 255));
		::memset(chXJData, 0, sizeof(chXJData));
		::sprintf(chXJData, "%d", UserData.lWinCount);
		pDC->TextOut(m_UserXJInfoRect.left+78, m_UserXJInfoRect.top+42, chXJData);

		//����ʤ������
		pDC->SetTextColor(RGB(0, 0, 0));
		::memset(chXJData, 0, sizeof(chXJData));
		::sprintf(chXJData, "ʤ�����ʣ�");
		pDC->TextOut(m_UserXJInfoRect.left+15, m_UserXJInfoRect.top+58, chXJData);
		pDC->SetTextColor(RGB(255, 0, 255));
		::memset(chXJData, 0, sizeof(chXJData));
		if (dwTotalGameNums>0)
		{
			float fTotalValue = ((float)dwTotalGameNums);
			DWORD dwValue = (UserData.lWinCount/fTotalValue)*100;
			::sprintf(chXJData, "%d\%%", dwValue);
			pDC->TextOut(m_UserXJInfoRect.left+78, m_UserXJInfoRect.top+58, chXJData);
		}
		else
		{
			::sprintf(chXJData, "0\%%");
			pDC->TextOut(m_UserXJInfoRect.left+78, m_UserXJInfoRect.top+58, chXJData);
		}
		pDC->SelectObject(&pOldFont);
		pOldFont = NULL;
	}

	/*cxf	WORD wClass = 0;
	tagGlobalUserData & UserData=g_GlobalUnits.GetGolbalUserData();
	DWORD dwTotalGameNums = (UserData.lWinCount+UserData.lLostCount+UserData.lDrawCount+UserData.lFleeCount);
	CountXJClass(UserData.lWinCount, dwTotalGameNums, wClass);
	if (wClass>=0 && wClass<=5)
	{
	HINSTANCE hInstance = AfxGetInstanceHandle();
	char chXJInfo[32] = {0};
	if (0 == wClass)
	{//����
	::sprintf(chXJInfo, "PNG_IMAGE_XJ_NEW");
	}
	else if (wClass>=1 && wClass<=5)
	{//�Ǽ����
	::sprintf(chXJInfo, "PNG_IMAGE_XJ_%d", wClass);
	}
	m_UserXJImage.DestroyImage();
	m_UserXJImage.LoadImage(hInstance, chXJInfo);
	//cxf m_UserXJImage.DrawImage(pDC, m_UserXJRect.left, m_UserXJRect.top);

	if (m_bIsShowXJInfo && !m_XJInfoBackImage.IsNull())
	{
	m_XJInfoBackImage.DrawImage(pDC, m_UserXJInfoRect.left, m_UserXJInfoRect.top);
	//���� DC
	CFont* pOldFont= pDC->SelectObject(&m_fontDrawXJ);
	//���Ƽ������
	char chXJData[64]={0};
	if (0 == wClass)
	{
	pDC->SetTextColor(RGB(0, 0, 0));
	::memset(chXJData, 0, sizeof(chXJData));
	::sprintf(chXJData, "�±�");
	pDC->TextOut(m_UserXJInfoRect.left+48, m_UserXJInfoRect.top+8, chXJData);
	}
	else
	{
	pDC->SetTextColor(RGB(255, 0, 255));
	::sprintf(chXJData, "%d", wClass);
	pDC->TextOut(m_UserXJInfoRect.left+35, m_UserXJInfoRect.top+8, chXJData);
	pDC->SetTextColor(RGB(0, 0, 0));
	::memset(chXJData, 0, sizeof(chXJData));
	if (2 == wClass)
	{
	::sprintf(chXJData, "����ξ");
	}
	else if (3 == wClass)
	{
	::sprintf(chXJData, "����У");
	}
	else if (4 == wClass)
	{
	::sprintf(chXJData, "����У");
	}
	else if (5 == wClass)
	{
	::sprintf(chXJData, "���Ͻ�");
	}
	else
	{
	::sprintf(chXJData, "����ʿ");
	}
	pDC->TextOut(m_UserXJInfoRect.left+43, m_UserXJInfoRect.top+8, chXJData);
	}


	//������Ϸ�ܴ���
	pDC->SetTextColor(RGB(0, 0, 0));
	::memset(chXJData, 0, sizeof(chXJData));
	::sprintf(chXJData, "��Ϸ�ܴ�����");
	pDC->TextOut(m_UserXJInfoRect.left+4, m_UserXJInfoRect.top+25, chXJData);
	pDC->SetTextColor(RGB(255, 0, 255));
	::memset(chXJData, 0, sizeof(chXJData));
	::sprintf(chXJData, "%d", dwTotalGameNums);
	pDC->TextOut(m_UserXJInfoRect.left+78, m_UserXJInfoRect.top+25, chXJData);

	//����ʤ������
	pDC->SetTextColor(RGB(0, 0, 0));
	::memset(chXJData, 0, sizeof(chXJData));
	::sprintf(chXJData, "ʤ��������");
	pDC->TextOut(m_UserXJInfoRect.left+15, m_UserXJInfoRect.top+42, chXJData);
	pDC->SetTextColor(RGB(255, 0, 255));
	::memset(chXJData, 0, sizeof(chXJData));
	::sprintf(chXJData, "%d", UserData.lWinCount);
	pDC->TextOut(m_UserXJInfoRect.left+78, m_UserXJInfoRect.top+42, chXJData);

	//����ʤ������
	pDC->SetTextColor(RGB(0, 0, 0));
	::memset(chXJData, 0, sizeof(chXJData));
	::sprintf(chXJData, "ʤ�����ʣ�");
	pDC->TextOut(m_UserXJInfoRect.left+15, m_UserXJInfoRect.top+58, chXJData);
	pDC->SetTextColor(RGB(255, 0, 255));
	::memset(chXJData, 0, sizeof(chXJData));
	if (dwTotalGameNums>0)
	{
	float fTotalValue = ((float)dwTotalGameNums);
	DWORD dwValue = (UserData.lWinCount/fTotalValue)*100;
	::sprintf(chXJData, "%d\%%", dwValue);
	pDC->TextOut(m_UserXJInfoRect.left+78, m_UserXJInfoRect.top+58, chXJData);
	}
	else
	{
	::sprintf(chXJData, "0\%%");
	pDC->TextOut(m_UserXJInfoRect.left+78, m_UserXJInfoRect.top+58, chXJData);
	}
	pDC->SelectObject(&pOldFont);
	pOldFont = NULL;
	}
	}
	*/
}

//�������а�ť
void CPlazaDlg::DrawTitleButton(CDC *pDC)
{
	if (NULL == pDC)
	{
		return;
	}
	/*cxf 1024
	for (int i=0; i<4; ++i)
	{
	if (!m_TitleButtonList[i].BTImage.IsNull())
	{
	int iImageW = m_TitleButtonList[i].BTImage.GetWidth()/4;
	int iImageH = m_TitleButtonList[i].BTImage.GetHeight();
	int iSrcPos = 0;
	if (0 == m_TitleButtonList[i].btStatus)
	{
	iSrcPos = 0;
	}
	else if (1 == m_TitleButtonList[i].btStatus)
	{
	iSrcPos = 3*iImageW;
	}
	else if (2 == m_TitleButtonList[i].btStatus)
	{
	iSrcPos = iImageW;
	}
	else if (3 == m_TitleButtonList[i].btStatus)
	{
	iSrcPos = 2*iImageW;
	}
	m_TitleButtonList[i].BTImage.DrawImage(pDC, m_TitleButtonList[i].rcRect.left, m_TitleButtonList[i].rcRect.top, iImageW, iImageH,
	iSrcPos, 0, iImageW, iImageH);
	}
	}
	*/
	/*cxf if (!m_PngHat.IsNull())
	{
	m_PngHat.DrawImage(pDC, m_TitleButtonList[0].rcRect.left-32, m_TitleButtonList[0].rcRect.top-8);
	}*/
}


//�����Ǽ�
void CPlazaDlg::CountXJClass(DWORD dwWinNums, DWORD dwTotalNums, WORD& wClass)
{
	wClass = 0;
	if ((0 == dwTotalNums) || (dwWinNums>dwTotalNums))
	{
		return;
	}
	DWORD wRate = (dwWinNums/(float)dwTotalNums)*100;
	if ((dwWinNums>=50)&&(wRate>=50))
	{//һ��
		wClass = 1;
	}
	else if ((dwWinNums>=150)&&(wRate>=55))
	{//����
		wClass = 2;
	}
	else if ((dwWinNums>=300)&&(wRate>=60))
	{//����
		wClass = 3;
	}
	else if ((dwWinNums>=500)&&(wRate>=65))
	{//����
		wClass = 4;
	}
	else if ((dwWinNums>=800)&&(wRate>=75))
	{//����
		wClass = 5;
	}
}

//��������
void CPlazaDlg::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
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

void CPlazaDlg::OnBnCancelGame()
{
	if (NULL != m_pRoomViewItemCreate && ::IsWindow(m_pRoomViewItemCreate->GetSafeHwnd()))
	{
		m_pRoomViewItemCreate->CloseRoomViewItem();
	}
	OnCancel();
}
void CPlazaDlg::ConfirmClose()
{
	if (NULL != m_pActivity  &&  ::IsWindow(m_pActivity->GetSafeHwnd())  &&  ::IsWindowVisible(m_pActivity->GetSafeHwnd()))
	{
		m_pActivity->ShowWindow(SW_HIDE);
		m_pActivity->SendMessage(WM_CLOSE);
		SafeDelete(m_pActivity);
		return;
	}

	//Description<��GM����������ر��򷵻���Ϸ�б�>;Author<cxf>;DateTime<2012-03-20>
	if (NULL!=m_pRoomViewItemCreate && NULL!=m_pRoomViewItemCreate->GetSafeHwnd() && m_pRoomViewItemCreate->IsWindowVisible()==TRUE)
	{
		m_pRoomViewItemCreate->SendRequestCloseRoomMsg(1);
		return;
	}

	m_PlazaCloseConfirm.DoModal();

	BYTE byCloseType = m_PlazaCloseConfirm.GetCloseType();
	if (0 == byCloseType)
	{//�رմ���
		OnBnClickedClose();
	}
	else if (1 == byCloseType)
	{
#if !defined(_DEBUG)
		m_bDeleteMachineCode = true;
		OnCommandConnect();
		SetTimer(IDI_TIMER_WAIT_END, 5000, NULL);
#else
		SetTimer(IDI_TIMER_WAIT_END, 10, NULL);
#endif
	}
}
//���µ�¼
void CPlazaDlg::ResetLogon()
{
	m_ServerItemView.InitServerItemView(/*this*/);
	g_GlobalUnits.m_ServerListManager.InitServerListManager(&m_ServerItemView);
	m_GameListCenterDlg.SetWndHide();
	OnBnClickedRollBack();
	//�ر�����Socket
	if (m_pRoomViewItemCreate->GetSafeHwnd())
	{//��ֹ���������Ϸ��ť������Socket����ʱ��������µ�¼��ť��(���ر�Socket����ִ����͵�¼��ͬʱ��ʾ)
		m_pRoomViewItemCreate->m_ClientSocket->CloseSocket();
		m_pRoomViewItemCreate->ShowWindow(SW_HIDE);
	}
	
	m_bLogonPlaza = false;
	m_ClientSocket->CloseSocket();
	m_pDlgLogon->m_DlgLogonLogo->ShowWindow(SW_SHOW);
	m_pDlgLogon->ShowWindow(SW_SHOW);
	this->ShowWindow(SW_HIDE);
}
//�رճ���
void CPlazaDlg::OnBnClickedClose()
{
	if (NULL != m_pRoomViewItemCreate && ::IsWindow(m_pRoomViewItemCreate->GetSafeHwnd()))
	{
		m_pRoomViewItemCreate->SendRequestCloseRoomMsg(3);
	}
	else
	{
		OnCancel();
	}
	return;
}

void CPlazaDlg::OnBnClickedReLogon()
{
	//Ͷ����Ϣ
	PostMessage(WM_COMMAND,IDM_LOGON_PLAZA,0);
	return ;
}

// �ҵı��� IDC_BT_BEAUTY_SALON
// void CPlazaDlg::OnBnClickedBeautySalon()
// {
// 	ShellExecute(NULL,   "open",   "http://www.acetechcn.com ",     ""   ,NULL,   SW_SHOW);
// 
// }
// ��Ϸ�̳� IDC_BT_SHOP
// void CPlazaDlg::OnBnClickedGameShop()
// {
// 	ShellExecute(NULL,   "open",   "http://www.acetechcn.com ",     ""   ,NULL,   SW_SHOW);
// 
// }

//����     IDC_BT_RETURN
void CPlazaDlg::OnBnClickedReturn()
{
	if (!m_bIsGamePlazyStatus)
	{//���������Ϸ����״̬����ֱ�ӷ��ء�
		return;
	}
	switch (m_curTab)
	{
	case 0:
		{
			return;
		}
		break;
	case 1:
		{//������ص���¼����
			OnBnClickedReLogon();
		}
		break;
	case 2:
		{//�ӷ����б��ص���Ϸ����
			break;
		}
	case 3:
		{//�������б��ص������б�
			if (m_pRoomViewItemCreate && ::IsWindow(m_pRoomViewItemCreate->GetSafeHwnd()))
			{
				m_pRoomViewItemCreate->SendRequestCloseRoomMsg(1);
			}
			return;
		}
	}
	ShowTab(m_curTab-1);
	//�����û�����
	UpdateUserImage();
}				
//��������
void CPlazaDlg::UpdateMusicSound(bool bIsEnable)
{
	if (bIsEnable)
	{
		if (1==m_bIsSound)
		{
			CString strPath("");
			GetCurrentDirectoryPath(strPath);
			strPath.Append(_T("\\data\\plaza\\sound\\BackMusic.wav"));
			::PlaySound(strPath, AfxGetInstanceHandle(),SND_LOOP|SND_ASYNC|SND_NODEFAULT|SND_NOSTOP|SND_FILENAME);
		}
		else
		{
			::PlaySound(NULL, AfxGetInstanceHandle(),SND_PURGE);
		}
	}
	else
	{
		::PlaySound(NULL, AfxGetInstanceHandle(),SND_PURGE);
	}
}

//������ر�����
void CPlazaDlg::SetMusicSound(bool bIsEnable)
{
	if (bIsEnable)
	{//��������
		CString strPath("");
		GetCurrentDirectoryPath(strPath);
		strPath.Append(_T("\\data\\plaza\\sound\\BackMusic.wav"));
		::PlaySound(strPath, AfxGetInstanceHandle(),SND_LOOP|SND_ASYNC|SND_NODEFAULT|SND_NOSTOP|SND_FILENAME);
	}
	else
	{//��ֹ����
		::PlaySound(NULL, AfxGetInstanceHandle(),SND_PURGE);
	}
	/*cxf
	g_GlobalOption.m_bEnableBackMusic = bIsEnable;
	//���ű�������
	if (g_GlobalOption.m_bEnableBackMusic)
	{//��������
	CString strPath("");
	GetCurrentDirectoryPath(strPath);
	strPath.Append(_T("\\data\\plaza\\sound\\BackMusic.wav"));
	::PlaySound(strPath, AfxGetInstanceHandle(),SND_LOOP|SND_ASYNC|SND_NODEFAULT|SND_NOSTOP|SND_FILENAME);
	}
	else
	{//��ֹ����
	::PlaySound(NULL, AfxGetInstanceHandle(),SND_PURGE);
	}

	tagGlobalUserData & UserData=g_GlobalUnits.GetGolbalUserData();
	//������Ϣ
	TCHAR szBuffer[256];
	_snprintf(szBuffer,sizeof(szBuffer),TEXT("%s\\%ld"),REG_USER_INFO,UserData.dwUserID);

	//д����Ϣ
	CRegKey RegUserInfo;
	if (RegUserInfo.Create(HKEY_CURRENT_USER,szBuffer)==ERROR_SUCCESS)
	{
	RegUserInfo.SetDWORDValue(TEXT("BackMusicTag"),g_GlobalOption.m_bEnableBackMusic?1:0);
	}
	*/
}

//��С��   IDCMINSIZE
void CPlazaDlg::OnBnClickedMinsize()
{
	::PlaySound(NULL, AfxGetInstanceHandle(),SND_PURGE);
	::ShowWindow(GetSafeHwnd(), SW_MINIMIZE);
}

BOOL CPlazaDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (m_ToolTipCtrl.m_hWnd!=NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	if(pMsg->message==WM_KEYDOWN)
	{
		if (VK_SPACE == pMsg->wParam)
		{
			return TRUE;
		}
		else if (VK_RETURN == pMsg->wParam)
		{
			if (m_pDlgLogon->IsWindowVisible())
			{
				m_pDlgLogon->OnOK();
			}
			return TRUE;
		}

		if (::GetKeyState(VK_CONTROL)<0)
		{
			if (70==pMsg->wParam || 102==pMsg->wParam)
			{
				if (NULL != m_pRoomViewItemCreate && ::IsWindow(m_pRoomViewItemCreate->GetSafeHwnd()) &&
					TRUE == m_pRoomViewItemCreate->IsWindowVisible())
				{
					m_pRoomViewItemCreate->SetCtrolAddF();
				}
				return TRUE;
			}
		}

		/*cxf
		else if (VK_BACK == pMsg->wParam)
		{
		if (IsWindowVisible() && m_bIsGamePlazyStatus && m_btnReturn.IsWindowEnabled())
		{
		OnBnClickedReturn();
		}
		}*/
	}
	return __super::PreTranslateMessage(pMsg);
}

//ִ����Ӧ��ť
void  CPlazaDlg::ExeButtonEvent(BYTE byEventID)
{
	if (byEventID>=0 && byEventID<4)
	{
		if (0 == byEventID)
		{//��Ϸ����
			OnBnClickedGamePlazy();
		}
		else if (1 == byEventID)
		{//����
			OnBnClickedTask();
		}
		else if (2 == byEventID)
		{//�ر���
			OnBnClickedDepository();
		}
		else if (3 == byEventID)
		{//�̳�
			OnBnClickedShop();
		}
	}
}

void CPlazaDlg::OnBnClickedShop()
{//�̳�
	/* cxf 1022
	if (3 != m_curTab)
	{//����ȥ�̳������
	SetShowTab(32);
	m_bIsGamePlazyStatus = false;
	}
	else
	{//�����������
	TCHAR szRuleUrl[128]={0};
	_snprintf(szRuleUrl,sizeof(szRuleUrl),WEB_MY_GAMEROOM_HELP,m_roomListDlg.m_wKindID);
	ShellExecute(NULL,TEXT("open"),szRuleUrl,NULL,NULL,SW_SHOWDEFAULT);

	if (m_bIsGamePlazyStatus)
	{
	m_TitleButtonList[3].btStatus = 0;
	m_TitleButtonList[0].btStatus = 3;
	InvalidateRect(&m_TitleButtonList[0].rcRect);
	InvalidateRect(&m_TitleButtonList[3].rcRect);
	}

	}
	*/
}

void CPlazaDlg::OnBnClickedTask()
{//����
	SetShowTab(8);
	m_bIsGamePlazyStatus = false;
}

void CPlazaDlg::OnBnClickedGamePlazy()
{//��Ϸ����
	m_bIsGamePlazyStatus = true;
	ShowTab(m_curTab);
}

void CPlazaDlg::OnBnClickedDepository()
{//�ر���
	SetShowTab(16);
	m_bIsGamePlazyStatus = false;
}
BOOL CPlazaDlg::onHtmlContextMenu(IHTMLEventObj *pEvtObj)
{ 
	//�ڳɹ��������¼�Դ��,ÿ���û��һ���������������,����Ը���pEvtObj���жϵ�ǰ���λ�õ�,Ȼ��������Լ������˵�,��IE�����˵�,����ʲô������...
	return S_OK;
}

void CPlazaDlg::OnDocumentCompleteExplorer3(LPDISPATCH pDisp, VARIANT FAR* URL) 
{
	//����WebBrowser�ؼ���DocumentComplete�¼�,����ʼ��pDoc2ָ������ӵ��¼�Դ
	//HRESULT hr=m_pExUserInfo->get_Document()->QueryInterface(IID_IHTMLDocument2,(void**)&pDoc2);
	//BOOL Ret = AfxConnectionAdvise( 
	//	pDoc2,  //�����Ӷ���Ľӿ�ָ��
	//	DIID_HTMLDocumentEvents2, //���ӽӿ�ID
	//	GetIDispatch(FALSE), //����Ƕ��IDispatchʵ�����һ������ʵ��m_xDispatch���˳�ȥ
	//	FALSE,   //donod addref
	//	&m_dwCookie );  //cookie to break connection later...
	//if(Ret){
	//		//AfxMessageBox("�ɹ��ҽ���");
	//}
}

void CPlazaDlg::CloseWebWindow(int nIDD)
{
	/*cxf 1022
	switch (nIDD)
	{
	//����
	case 0:
	{
	//cxf 1022  ::ShowWindow(m_TaskDlg.GetSafeHwnd(), SW_HIDE);
	}
	break;
	//�̳�
	case 1:
	{
	UpdateUserImage();
	::ShowWindow(m_shopDlg.GetSafeHwnd(), SW_HIDE);
	}
	break;
	//�ر���
	case 2:
	{
	::ShowWindow(m_doteyBoxDlg.GetSafeHwnd(), SW_HIDE);
	}
	break;
	}
	*/
}

//�����û�����
void CPlazaDlg::OnCannUpdateUserImage()
{
	UpdateUserImage();
}

//��������
void CPlazaDlg::UpdateUserImage()
{
	/*cxf 1022 CRect rect;
	rect.left = 14;
	rect.top = 82;
	rect.right = rect.left + 230;
	rect.bottom = rect.top + 673;*/
	CRect rect;
	rect.left = 750;
	rect.top = 170;
	rect.right = rect.left + 200;
	rect.bottom = rect.top + 340;
	InvalidateRect(&rect);
}
//�����û����
void CPlazaDlg::UpdateUserGold(DWORD dwGold)
{
	tagGlobalUserData & UserData=g_GlobalUnits.GetGolbalUserData();
	UserData.lGold = dwGold;
	CRect rcRect;
	rcRect.left = 103;
	rcRect.top = 574;
	rcRect.right = rcRect.left + 150;
	rcRect.bottom = rcRect.top + 14;
	InvalidateRect(&rcRect);
}
//�����û��ر���
void CPlazaDlg::UpdateUserMoney(DWORD dwMoney)
{
	tagGlobalUserData & UserData=g_GlobalUnits.GetGolbalUserData();
	UserData.lMoney = dwMoney;
	CRect rcRect;
	rcRect.left = 103;
	rcRect.top = 607;
	rcRect.right = rcRect.left + 150;
	rcRect.bottom = rcRect.top + 14;
	InvalidateRect(&rcRect);
}

//�����û�������Ϣ
void CPlazaDlg::UpdateUserDJInfo(int iDJID)
{
	if (NULL != m_pRoomViewItemCreate)
	{
		m_pRoomViewItemCreate->UpdateUserDJInfo(iDJID);
	}
	//��������
	UpdateUserImage();
}

//void CPlazaDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
//{
//	CDialog::OnWindowPosChanged(lpwndpos);
//	// TODO: �ڴ˴������Ϣ����������
//
//	////������Ļ
//	//LockWindowUpdate();
//
//	//ResizeFrame();
//
//	////�������
//	//UnlockWindowUpdate();
//}

//����alphaֵ
void CPlazaDlg::SetResetAlphaValue(HWND hWnd)
{
	return;
	if (!::IsWindow(hWnd))
	{
		return;
	}
	if (0==m_byCurWndTag && m_hSaveOldCurWnd==hWnd)
	{
		return;
	}
	else if ((1==m_byCurWndTag || 2==m_byCurWndTag) && m_hSaveNewCurWnd==hWnd)
	{
		return;
	}
	KillTimer(IDI_TIME_SET_DLG_ALPHA_END);
	KillTimer(IDI_TIME_SET_DLG_ALPHA_VALUE);
	if (0 != m_byCurWndTag)
	{//�����ǰ�Ķ�����δ���,��ֱ�����ض����еĴ���
		//::ShowWindow(m_hSaveNewCurWnd, SW_HIDE);
		//::ShowWindow(m_hSaveOldCurWnd, SW_HIDE);
		m_pFunShowLayer(m_hSaveNewCurWnd, 0, 0, 2);
		m_pFunShowLayer(m_hSaveOldCurWnd, 0, 0, 2);
	}
	if (!::IsWindow(m_hSaveOldCurWnd))
	{
		m_hSaveOldCurWnd = hWnd;
		m_byCurWndTag = 2;
		m_uiCurAlphaValue = 0;
		m_pFunShowLayer(m_hSaveNewCurWnd, 0, m_uiCurAlphaValue, 2);
	}
	else
	{
		m_byCurWndTag = 1;
		m_uiCurAlphaValue = 255;
		m_pFunShowLayer(m_hSaveOldCurWnd, 0, m_uiCurAlphaValue, 2);
	}
	m_hSaveNewCurWnd = hWnd;
	//::ShowWindow(m_hSaveNewCurWnd, SW_HIDE);
	//::ShowWindow(m_hSaveOldCurWnd, SW_HIDE);
	SetTimer(IDI_TIME_SET_DLG_ALPHA_VALUE, TIME_SET_DLG_ALPHA_INTERVAL, NULL);
	SetTimer(IDI_TIME_SET_DLG_ALPHA_END, TIME_SET_DLG_ALPHA_END_INTERVAL, NULL);
}

//ʱ����Ϣ
void CPlazaDlg::OnTimer(UINT nIDEvent)
{
	if (IDI_TIMER_PLAY_BACK_MUSIC == nIDEvent)
	{//���ű�������
		KillTimer(IDI_TIMER_PLAY_BACK_MUSIC);
		CString strPath("");
		GetCurrentDirectoryPath(strPath);
		strPath.Append(_T("\\data\\plaza\\sound\\BackMusic.wav"));
		::PlaySound(strPath, AfxGetInstanceHandle(),SND_LOOP|SND_ASYNC|SND_NODEFAULT|SND_NOSTOP|SND_FILENAME);
	}
	else if (TIMER_WEB_LOADING_PLAZA == nIDEvent)
	{
		KillTimer(TIMER_WEB_LOADING_PLAZA);
		m_webPlazaAdvertisement.Navigate(CInitData::s_cAdverTisement,NULL,NULL,NULL,NULL);
		m_webPlazaNotice.Navigate(CInitData::s_cNotice,NULL,NULL,NULL,NULL);
		m_webPlazaMessage.Navigate(CInitData::s_cXBMessage, NULL,NULL,NULL,NULL);
		m_webPlazaTitle.Navigate(CInitData::s_cTitleNotice,NULL,NULL,NULL,NULL);
	}
	else if (TIMER_UPDATEUSERINFO_OPERATE == nIDEvent)
	{
		KillTimer(TIMER_UPDATEUSERINFO_OPERATE);
		m_btnUpdateUserInfo.EnableWindow(TRUE);
	}
	else if (IDI_ITMER_MEWCOMER_GUIDE == nIDEvent)
	{//����ָ������֡��
		m_wNewComerGuideActionNumber = m_wNewComerGuideActionNumber==0?1:0;
		InvalidateRect(m_rcNewComerGuideArea);
	}
	else if (IDI_TIMER_WAIT_END == nIDEvent)
	{
		KillTimer(IDI_TIMER_WAIT_END);
		m_ClientSocket->CloseSocket();
		BYTE byCloseType = m_PlazaCloseConfirm.GetCloseType();
		
		if (0 == byCloseType)
		{
			if (NULL!=m_pDlgLogon && ::IsWindow(m_pDlgLogon->GetSafeHwnd()))
			{
				m_pDlgLogon->ExitLogon();
			}
			((CGamePlazaApp*)AfxGetApp())->ExitGamePlaza();
		}
		else
		{
			//���µ�¼
			ResetLogon();
		}
	}
	else if (TIME_WAIT_CHECK_LINE == nIDEvent) // �����·��ʱ
	{
		KillTimer(TIME_WAIT_CHECK_LINE);
		m_pDlgLogon->SetCheckLineResult(0);
	}

	/*cxf 
	else if (IDI_TIMER_GAME_MSG_FLASH == nIDEvent)
	{//��Ϸ��Ϣ����
	m_bIsFlashMsg = !m_bIsFlashMsg;
	InvalidateRect(&m_rcGameMsgArea);
	}*/

	//if (IDI_TIME_SET_DLG_ALPHA_VALUE == nIDEvent)
	//{
	//	if (1 == m_byCurWndTag)
	//	{//�����ɴ���
	//		if (::IsWindow(m_hSaveOldCurWnd))
	//		{
	//			if (m_uiCurAlphaValue<=100)
	//			{
	//				m_uiCurAlphaValue = 0;
	//				m_pFunShowLayer(m_hSaveOldCurWnd, 0, 0, 2);
	//				//::ShowWindow(m_hSaveOldCurWnd, SW_HIDE);
	//				if (::IsWindow(m_hSaveNewCurWnd))
	//				{
	//					m_pFunShowLayer(m_hSaveNewCurWnd, 0, 0, 2);
	//					m_byCurWndTag = 2;
	//					m_uiCurAlphaValue = 0;
	//				}
	//				else
	//				{
	//					KillTimer(IDI_TIME_SET_DLG_ALPHA_VALUE);
	//				}
	//			}
	//			else
	//			{
	//				//::ShowWindow(m_hSaveOldCurWnd, SW_SHOW);
	//				m_pFunShowLayer(m_hSaveOldCurWnd, 0, m_uiCurAlphaValue, 2);
	//				m_uiCurAlphaValue -= 50;
	//			}
	//		}
	//		else
	//		{
	//			if (::IsWindow(m_hSaveNewCurWnd))
	//			{
	//				m_pFunShowLayer(m_hSaveNewCurWnd, 0, 0, 2);
	//				m_byCurWndTag = 2;
	//				m_uiCurAlphaValue = 0;
	//			}
	//			else
	//			{
	//				KillTimer(IDI_TIME_SET_DLG_ALPHA_VALUE);
	//			}
	//		}
	//	}
	//	else
	//	{//�����´���
	//		if (::IsWindow(m_hSaveNewCurWnd))
	//		{
	//			//::ShowWindow(m_hSaveNewCurWnd, SW_SHOW);
	//			if (m_uiCurAlphaValue>=250)
	//			{
	//				KillTimer(IDI_TIME_SET_DLG_ALPHA_VALUE);
	//				KillTimer(IDI_TIME_SET_DLG_ALPHA_END);
	//				m_pFunShowLayer(m_hSaveNewCurWnd, 0, 255, 2);
	//				::SetWindowPos(m_hSaveNewCurWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
	//				if (m_hSaveNewCurWnd!=m_hSaveOldCurWnd && ::IsWindow(m_hSaveOldCurWnd))
	//				{
	//					m_pFunShowLayer(m_hSaveOldCurWnd, 0, 0, 2);
	//				}
	//				m_uiCurAlphaValue = 0;
	//				m_byCurWndTag = 0;
	//				m_hSaveOldCurWnd = m_hSaveNewCurWnd;
	//				m_hSaveNewCurWnd = NULL;
	//			}
	//			else
	//			{
	//				m_pFunShowLayer(m_hSaveNewCurWnd, 0, m_uiCurAlphaValue, 2);
	//				m_uiCurAlphaValue += 30;
	//			}
	//		}
	//		else
	//		{
	//			KillTimer(IDI_TIME_SET_DLG_ALPHA_VALUE);
	//		}
	//	}
	//}
	//else if (IDI_TIME_SET_DLG_ALPHA_END == nIDEvent)
	//{
	//	KillTimer(IDI_TIME_SET_DLG_ALPHA_END);
	//	m_pFunShowLayer(m_hSaveNewCurWnd, 0, 255, 2);
	//}
}


//�������д�����ʾ״̬
void CPlazaDlg::SetAllWndShow(bool bIsShow)
{
	if (bIsShow)
	{
		//cxf 1022 m_TaskDlg.ShowWindow(SW_SHOW);	//����
		//cxf 1022 m_shopDlg.ShowWindow(SW_SHOW);	//�̳�
		//cxf 1022 m_doteyBoxDlg.ShowWindow(SW_SHOW);	//�ر���
		//cxf m_gameListDlg.ShowWindow(SW_SHOW);	//��Ϸ�б�
		//cxf 1022 m_roomListDlg.ShowWindow(SW_SHOW);	//�����б�
		if (m_pRoomViewItemCreate && ::IsWindow(m_pRoomViewItemCreate->GetSafeHwnd()))	//�����б�
		{
			::ShowWindow(m_pRoomViewItemCreate->GetSafeHwnd(), SW_SHOW);
		}
	}
	else
	{
		//cxf 1022 m_TaskDlg.ShowWindow(SW_HIDE);	//����
		//cxf 1022 m_shopDlg.ShowWindow(SW_HIDE);	//�̳�
		//cxf 1022 m_doteyBoxDlg.ShowWindow(SW_HIDE);	//�ر���
		//cxf m_gameListDlg.ShowWindow(SW_HIDE);	//��Ϸ�б�
		//cxf 1022 m_roomListDlg.ShowWindow(SW_HIDE);	//�����б�
		if (m_pRoomViewItemCreate && ::IsWindow(m_pRoomViewItemCreate->GetSafeHwnd()))	//�����б�
		{
			::ShowWindow(m_pRoomViewItemCreate->GetSafeHwnd(), SW_HIDE);
		}
	}
}

//�������д���͸����
void CPlazaDlg::SetAllWndAlpha(BYTE byAlpha)
{
	//cxf 1022 m_pFunShowLayer(m_TaskDlg.GetSafeHwnd(), 0, byAlpha, 2);	//����
	//cxf 1022 m_pFunShowLayer(m_shopDlg.GetSafeHwnd(), 0, byAlpha, 2);	//�̳�
	//cxf 1022 m_pFunShowLayer(m_doteyBoxDlg.GetSafeHwnd(), 0, byAlpha, 2);	//�ر���
	//cxf m_pFunShowLayer(m_gameListDlg.GetSafeHwnd(), 0, byAlpha, 2);	//��Ϸ�б�
	//cxf 1022 m_pFunShowLayer(m_roomListDlg.GetSafeHwnd(), 0, byAlpha, 2);	//�����б�
	//cxf 1022 if (m_pRoomViewItemCreate && ::IsWindow(m_pRoomViewItemCreate->GetSafeHwnd()))	//�����б�
	//cxf 1022 {
	//cxf 1022 	m_pFunShowLayer(m_pRoomViewItemCreate->GetSafeHwnd(), 0, byAlpha, 2);
	//cxf 1022 }
}

//��ʾ��Ӧ��ť
void CPlazaDlg::SetShowTab(BYTE byValue)
{
	SetAllWndShow(false);
	if (1 == byValue)
	{//��ʾ��Ϸ�б�
		//cxf m_gameListDlg.ShowWindow(SW_SHOW);	//��Ϸ�б�
		//cxf m_btnReturn.EnableWindow(TRUE);
	}
	else if (2 == byValue)
	{//��ʾ�����б�
		//cxf 1022 m_roomListDlg.ShowWindow(SW_SHOW);	//�����б�
		//cxf m_btnReturn.EnableWindow(TRUE);
	}
	else if (4 == byValue)
	{//��ʾ�����б�
		if (m_pRoomViewItemCreate && ::IsWindow(m_pRoomViewItemCreate->GetSafeHwnd()))	//�����б�
		{
			::ShowWindow(m_pRoomViewItemCreate->GetSafeHwnd(), SW_SHOW);
			//cxf m_btnReturn.EnableWindow(TRUE);
		}
		else
		{
			//cxf m_btnReturn.EnableWindow(FALSE);
		}
	}
	else if (8 == byValue)
	{//��ʾ����
		//cxf 1022 m_TaskDlg.ShowWindow(SW_SHOW);	//����
		//cxf m_btnReturn.EnableWindow(FALSE);
	}
	else if (16 == byValue)
	{//��ʾ�ر���
		//cxf 1022 m_doteyBoxDlg.ShowWindow(SW_SHOW);	//�ر���
		//cxf m_btnReturn.EnableWindow(FALSE);
	}
	else if (32 == byValue)
	{//��ʾ�̳�
		//cxf 1022 m_shopDlg.ShowWindow(SW_SHOW);	//�̳�
		//cxf m_btnReturn.EnableWindow(FALSE);
	}
}

BOOL CPlazaDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	return CDialog::OnCommand(wParam, lParam);
}

void CPlazaDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if(nID == SC_MINIMIZE)
	{
		::PlaySound(NULL, AfxGetInstanceHandle(),SND_PURGE);
	}
	else if (SC_RESTORE == nID)
	{
		UpdateMusicSound(true);
	}
	else if (SC_MAXIMIZE == nID)
	{

	}
	else if (SC_CLOSE == nID)
	{

	}
	CDialog::OnSysCommand(nID, lParam);
}

//����	   IDC_BT_SET
void CPlazaDlg::OnBnClickedSet()
{
	/*cxf CSetDialog setdlg;
	setdlg.DoModal();*/

	UINT uiReturn = GetCommonConfig("Sound","EnablePlazeBGMusic",1);

	if (0==uiReturn)
	{//��������
		m_bIsSound=true;
		m_btnSound.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTSoundOpen.png"),false);
	}else
	{//�ر�����
		m_bIsSound=false;
		m_btnSound.LoadRgnImage(TEXT(m_strCurrentPathPlaza+"BTSoundClose.png"),false);
	}
	SetMusicSound(m_bIsSound);//�Ƿ񲥷�����
	SetCommonConfig("Sound","EnablePlazeBGMusic",((true == m_bIsSound)? TEXT("1"): TEXT("0")));
	m_btnSound.Invalidate(FALSE);
}
//��¼����
void CPlazaDlg::SetDlgLogonPtr(CDlgLogon * pDlgLogon)
{
	if (NULL != pDlgLogon)
	{
		m_pDlgLogon = pDlgLogon;
	}
}
//��ǰ·��
void CPlazaDlg::GetCurrentDirectoryPath(CString &szPath)
{
	szPath="";
	TCHAR szPathTemp[MAX_PATH]={0};
	GetCurrentDirectory(MAX_PATH, szPathTemp);
	szPath.Append(szPathTemp);
}
//������Դ·��
void CPlazaDlg::GetResPath(CString strClipName, CString& strPath)
{
	CString strModuleFileName("");
	GetCurrentDirectoryPath(strModuleFileName);
	strModuleFileName.Append(strClipName);
	strPath = strModuleFileName;
}
//��ȡ����
UINT CPlazaDlg::GetCommonConfig(CString strTitle,CString strName, int uiDefault)
{
	CString strLclPth, strFlPth;
	GetModuleFileName(NULL, strLclPth.GetBufferSetLength(256), 256);
	strFlPth = strLclPth.Left(strLclPth.ReverseFind('\\')+1)+ TEXT("config\\Common.ini");
	return GetPrivateProfileInt(TEXT(strTitle), TEXT(strName), uiDefault, strFlPth); 
}
//��������
void CPlazaDlg::SetCommonConfig(CString strTitle,CString strName, CString bIsSound)
{
	CString strLclPth, strFlPth;
	GetModuleFileName(NULL, strLclPth.GetBufferSetLength(256), 256);
	strFlPth = strLclPth.Left(strLclPth.ReverseFind('\\')+1)+ TEXT("config\\Common.ini");
	WritePrivateProfileString(TEXT(strTitle), TEXT(strName),bIsSound, strFlPth);
}
//��Ϸ����
void CPlazaDlg::OnBnClickedVisitWeb()
{
	tagGlobalUserData & UserData=g_GlobalUnits.GetGolbalUserData();
	CString strUrl("");
	strUrl.Format(CInitData::s_cLoginHomePage, UserData.dwUserID, UserData.szKey);
	ShellExecute(NULL,"open",strUrl,"",NULL,SW_SHOW);
}
//�ʺŹ���
void CPlazaDlg::OnBnClickedAccountsManage()
{
	tagGlobalUserData & UserData=g_GlobalUnits.GetGolbalUserData();
	CString strUrl("");
	strUrl.Format(CInitData::s_cLoginAccountsManage, UserData.dwUserID, UserData.szKey);
	ShellExecute(NULL,"open",strUrl,"",NULL,SW_SHOW);
}
//��Ϸ����
void CPlazaDlg::OnBnClickedGameHelp()
{
	ShellExecute(NULL,"open",CInitData::s_cGameHelp,"",NULL,SW_SHOW);
}
//���ⷴ��
void CPlazaDlg::OnBnClickedQuestionFeedBack()
{
	tagGlobalUserData & UserData=g_GlobalUnits.GetGolbalUserData();
	CString strUrl("");
	strUrl.Format(CInitData::s_cErrorTickling, UserData.dwUserID, UserData.szKey);
	ShellExecute(NULL,"open",strUrl,"",NULL,SW_SHOW);
}
//���߿ͷ�
void CPlazaDlg::OnBnClickedContactServise()
{
	ShellExecute(NULL,"open",CInitData::s_cContactService,"",NULL,SW_SHOW);
}
//�����û���Ϣ
void CPlazaDlg::OnBnClickedUpdateUserInfo()
{
	m_btnUpdateUserInfo.EnableWindow(FALSE);
	SetTimer(TIMER_UPDATEUSERINFO_OPERATE, 60000, NULL);//����һ���ӵ��һ��
	UpdateUserInfo();
}
void CPlazaDlg::UpdateUserInfo()
{
	//���ӷ�����
	m_bIsUpdateUserInfo=true;
	OnCommandConnect();
}
//��Ϸ�б�
void CPlazaDlg::OnBnClickedGameList()
{
	SetShowHideInfo(1);//��ʾ����
	SwitchTitle(0);//�л�����
	//����������Ϸ��־λ����ʾ
	HideFlag();
}
//����ר��
void CPlazaDlg::OnBnClickedTounament()
{
	SetShowHideInfo(0);//��ʾ����
	SwitchTitle(1);//�л�����
	//����������Ϸ��־λ����ʾ
	HideFlag();
}
//�ʺų�ֵ
void CPlazaDlg::OnBnClickedAccountsRecharge()
{
	LoadPlazaGameWeb(1, 2);
}
//��Ʒ�һ�
void CPlazaDlg::OnBnClickedPrizeExcharge()
{
	//LoadPlazaGameWeb(2, 3);
	tagGlobalUserData UserData=g_GlobalUnits.GetGolbalUserData();
	CString strUrl("");
	strUrl.Format(CInitData::s_cLoginExchange, UserData.dwUserID, UserData.szKey);
	ShellExecute(NULL,"open",strUrl,"",NULL,SW_SHOW);
}
//�����̳�
void CPlazaDlg::OnBnClickedPropsShop()
{
	LoadPlazaGameWeb(3, 4);
}
//��������
void CPlazaDlg::OnBnClickedNoviceTask()
{
	//LoadPlazaGameWeb(4, 5);

#if 0
	m_pNewHandTask = new CNewHandTask;
	if (!m_pNewHandTask->Create(IDD_NEWHANDTASK, this))
		return;
	m_pNewHandTask->ShowWindow(SW_SHOW);
#else
	CNewHandTask newhandtask;
	newhandtask.DoModal();
#endif
}
//����
void CPlazaDlg::OnBnClickedBackPack()
{
	LoadPlazaGameWeb(5, 13);
}
//�û���Ϣ
void CPlazaDlg::OnBnClickedUserInfo()
{
	LoadPlazaGameWeb(6, 10);
}
//����
void CPlazaDlg::OnBnClickedBank()
{
	LoadPlazaGameWeb(7, 12);
}
//�ʼ�
void CPlazaDlg::OnBnClickedEmailMessage()
{
	LoadPlazaGameWeb(8, 11);
}
//���ذ�ť
void CPlazaDlg::OnBnClickedRollBack()
{
	SetShowHideInfo(1);//��ʾ����
	SwitchTitle(0);//�л�����
	//����������Ϸ��־λ����ʾ
	HideFlag();
}
//��һҳ��ť
void CPlazaDlg::OnBnClickedNextPage()
{
}
//����������Ϸ��־
void CPlazaDlg::HideFlag()
{
	m_GameListHot.SetFlag(false);
	m_GameListHot.SetInvalidateRect();
}
//�л�����
void CPlazaDlg::SwitchTitle(WORD wPos)
{
	m_wTitlePos=wPos;
	SetInvalidateRect();
}
//����Ƕ����ҳ::wWebModeҳ����ʽ,wTitlePos����λ��
void CPlazaDlg::LoadPlazaGameWeb(WORD wWebMode,WORD wTitlePos)
{
	bool bReturn=false;
	bReturn = m_PlazaGameWeb.LoadPlazaGameWeb(wWebMode);
	if (bReturn)
	{
		SetShowHideInfo(2);//��ʾ����
		SwitchTitle(wTitlePos);//�л�����
		//����������Ϸ��־λ����ʾ
		HideFlag();
	}
}

BOOL CPlazaDlg::CheckGameSoftHash(LPCTSTR pNewHash)
{
	CString strLclPth, strFlPth;
	TCHAR chResult[MAX_PATH],chResultURL[MAX_PATH];	
	TCHAR szNamePath[MAX_PATH]=TEXT(""); //���������ļ�·��
	memset(chResult, 0, sizeof(chResult));
	GetModuleFileName(NULL, strLclPth.GetBufferSetLength(256), 256);

	_snprintf(szNamePath, sizeof(szNamePath), TEXT("%sconfig\\update.ini"), strLclPth.Left(strLclPth.ReverseFind('\\')+1));
	GetPrivateProfileString(_T("UpdateAllFile"), _T("GameHaseList"), _T(""), chResult, MAX_PATH, szNamePath);
	GetPrivateProfileString(_T("UpdateAllFile"), _T("URL"), _T("http://www.cbwgame.com/update/hallfile/"), chResultURL, MAX_PATH, szNamePath);

	BOOL bReturn = TRUE;
	if (0 == lstrcmp(pNewHash, chResult))
	{
		bReturn = FALSE;
	}
	else if (32 == lstrlen(pNewHash))
	{
		bReturn = TRUE;
	}
	else
	{
		m_GameFlash.Play(false);
		CString strBuffer;
		strBuffer.Format("����ϵ�ͷ�����Ϸ�汾У��ʧ�ܣ����ҽ�����ʾ��ͼ�����ͷ���лл������\r\nԶ�̣�[%s]!=���أ�[%s](%ld!=32)", pNewHash, chResult, lstrlen(pNewHash));
		ShowMessageBox(strBuffer, MB_ICONSTOP);
		bReturn = FALSE;
	}

	return bReturn;
}

BOOL CPlazaDlg::CallExternalUpdateProgram()
{
	// ����������
	CString strCommonLine;	
	char acBuffer[MAX_PATH] = {0};
	GetModuleFileName(AfxGetInstanceHandle(), acBuffer, sizeof(acBuffer));
	char *pBackSlash = strrchr(acBuffer, '\\');
	TCHAR szPassword[33];
	if (2 == g_GlobalUnits.GetApplicationCmdLine().GetSize())
	{
		_snprintf(szPassword, sizeof(szPassword), "%s", g_GlobalUnits.GetApplicationCmdLine().GetAt(1));
	}
	else
	{
		CMD5Encrypt::EncryptData(m_pDlgLogon->m_szPassword, szPassword);
	}
	strCommonLine.Format(TEXT("%s /modulefilename:%s /account:%s /pw:%s"), UPDATE_PROJECT_NAME, ++pBackSlash, m_pDlgLogon->m_szAccounts, szPassword);

	// ������Ϸ�ͻ���
	STARTUPINFO StartInfo;
	memset(&StartInfo,0,sizeof(StartInfo));
	StartInfo.cb=sizeof(StartInfo);
	StartInfo.wShowWindow=SW_SHOWMAXIMIZED;
	BOOL bSuccess = FALSE;
	PROCESS_INFORMATION m_GameProcessInfo;
	memset(&m_GameProcessInfo, 0, sizeof(m_GameProcessInfo));

	try
	{
		bSuccess = CreateProcess(NULL,strCommonLine.GetBuffer(),NULL,NULL,FALSE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&StartInfo,&m_GameProcessInfo);
		if (FALSE == bSuccess)
			throw 0;
	}
	catch (int iResult)
	{
		if (0 == iResult)
		{
			CString strBuffer;
			DWORD dwError = GetLastError();
			if (740 == dwError)
			{
				TCHAR tchBuffer[MAX_PATH] = {0}; 
				GetModuleFileName(AfxGetInstanceHandle(),tchBuffer,sizeof(tchBuffer));
				strBuffer.Format("����ִ���ļ���\r\n%s\r\n\r\nCreateProcess ʧ�� ���� %d��\r\n����Ĳ�����Ҫ������\r\n\r\n���������\r\n�ɳ����Ҽ����Թ���Ա������С���", tchBuffer, dwError);
				AfxMessageBox(strBuffer, MB_OK|MB_ICONSTOP);
			}
		}
	}
	catch (...) 
	{ 
		DWORD dwError = GetLastError();
		ASSERT(0); 
		return FALSE;
	}
	strCommonLine.ReleaseBuffer();

	if (FALSE == bSuccess)
	{
		DWORD dwError = GetLastError();
		return FALSE;
	}

	return TRUE;
}

//������Ч
void CPlazaDlg::SetInvalidateRect()
{
	InvalidateRect(m_rcTitleArea,FALSE);
}
//����
void CPlazaDlg::SetShowHideInfo(WORD dwType)
{
	m_btnRollback.ShowWindow(SW_HIDE);
	m_btnNextPage.ShowWindow(SW_HIDE);
	switch (dwType)
	{
	case 0:
		{
			m_RoomListCenterDlg.ShowWindow(SW_HIDE);
			m_PlazaGameWeb.ShowWindow(SW_HIDE);
			m_GameListCenterDlg.ShowWindow(SW_HIDE);
			break;
		}
	case 1:
		{//��Ϸ�б�
			m_RoomListCenterDlg.ShowWindow(SW_HIDE);
			m_PlazaGameWeb.ShowWindow(SW_HIDE);
			m_GameListCenterDlg.ShowWindow(SW_SHOW);
			break;
		}
	case 2:
		{//����Ƕ����ҳ
			m_GameListCenterDlg.ShowWindow(SW_HIDE);
			m_RoomListCenterDlg.ShowWindow(SW_HIDE);
			m_PlazaGameWeb.ShowWindow(SW_SHOW);
			break;
		}
	case 3:
		{//�����б�
			m_GameListCenterDlg.ShowWindow(SW_HIDE);
			m_PlazaGameWeb.ShowWindow(SW_HIDE);
			m_RoomListCenterDlg.ShowWindow(SW_SHOW);
			break;
		}
	}
}
//����ָ��
void CPlazaDlg::OnBnClickedNewComerGuide()
{
	KillTimer(IDI_ITMER_MEWCOMER_GUIDE);
	m_wNewComerGuideActionNumber = 1;
	m_bNewComerGuide=false;
	InvalidateRect(m_rcNewComerGuideArea);
	DoModalNewComerGuide();
}
//��������ָ������
void CPlazaDlg::DoModalNewComerGuide()
{
	CNewComerGuide newComerGuide;
	newComerGuide.DoModal();
}
//������ɫ��
bool CPlazaDlg::StartSiSeQiuProgram()
{
	tagGlobalUserData & UserData = g_GlobalUnits.GetGolbalUserData();
	// ����������
	CString strCommonLine;	
	strCommonLine.Format(TEXT("%s *%d *%s *%s *%d"), "SiSeQiu.exe", UserData.dwUserID, UserData.szPassWord, UserData.szKey, g_GlobalOption.m_byNetWorkTag);
	// ������Ϸ�ͻ���
	STARTUPINFO StartInfo;
	memset(&StartInfo,0,sizeof(StartInfo));
	StartInfo.cb=sizeof(StartInfo);
	StartInfo.wShowWindow=SW_SHOWMAXIMIZED;
	BOOL bSuccess = FALSE;
	PROCESS_INFORMATION m_GameProcessInfo;
	memset(&m_GameProcessInfo, 0, sizeof(m_GameProcessInfo));

	try
	{
		bSuccess = CreateProcess(NULL,strCommonLine.GetBuffer(),NULL,NULL,FALSE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&StartInfo,&m_GameProcessInfo);
		if (FALSE == bSuccess)
			throw 0;
	}
	catch (int iResult)
	{
		if (0 == iResult)
		{
			CString strBuffer;
			DWORD dwError = GetLastError();
			if (740 == dwError)
			{
				TCHAR tchBuffer[MAX_PATH] = {0}; 
				GetModuleFileName(AfxGetInstanceHandle(),tchBuffer,sizeof(tchBuffer));
				strBuffer.Format("����ִ���ļ���\r\n%s\r\n\r\nCreateProcess ʧ�� ���� %d��\r\n����Ĳ�����Ҫ������\r\n\r\n", tchBuffer, dwError);
				AfxMessageBox(strBuffer, MB_OK|MB_ICONSTOP);
			}
		}
	}
	catch (...) 
	{ 
		DWORD dwError = GetLastError();
		ASSERT(0); 
		return FALSE;
	}
	strCommonLine.ReleaseBuffer();

	if (FALSE == bSuccess)
	{
		DWORD dwError = GetLastError();
		return FALSE;
	}
	this->ShowWindow(SW_HIDE);
	
	SetMusicSound(false);//�Ƿ񲥷�����
	
	createThreads();

	return TRUE;

}
// �������߳�
bool CPlazaDlg::createThreads()
{
	if(m_hEventGameLoadOver[0])
	{
		m_hThread = (HANDLE)_beginthreadex( NULL, 0, &thread_game_close, (void*)this, 0, &m_dwThreadID);

		if(!m_hThread)
		{
			printf("�����߳�ʧ�� \n");
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}
//�̹߳�������(��ɫ��ͻ��˹ر�,��������)
unsigned __stdcall  CPlazaDlg::thread_game_close(void* pParam) 	
{
	CPlazaDlg* pThis = (CPlazaDlg*)pParam;
	DWORD  iWaitTime = 7200000;	// ms;��ĵȴ�ʱ��.
	DWORD dwResult;

	dwResult = WaitForMultipleObjects(1,  pThis->m_hEventGameLoadOver,  FALSE,  iWaitTime);	
	switch(dwResult)
	{
	case WAIT_TIMEOUT:			// ��ʱ
	case WAIT_OBJECT_0 +0:	    // �ӵ���ɫ��ͻ��˹رյ�֪ͨ
		{
			if (pThis && pThis->GetSafeHwnd())
			{
				pThis->ShowWindow(SW_SHOW);		// ��ʾ����
				pThis->UpdateUserInfo();//�����û���Ϣ
			}
			break;
		}
	}
	return 0;
}

bool CPlazaDlg::StartActivity(WORD wID)
{
	if (NULL != m_pActivity)
	{
		SafeDelete(m_pActivity);
	}

	m_pActivity = new CActive;
	if (NULL != m_pActivity)
	{
		int iIndex = 1;
		CString	sFileName("");
		CString sFilePath("");
		CString sFieldName("");
		TCHAR tchURL[MAX_PATH]=_T("");
		GetModuleFileName(NULL, sFileName.GetBufferSetLength(MAX_PATH), MAX_PATH);
		sFilePath=sFileName.Left(sFileName.ReverseFind('\\') + 1) + TEXT("config\\roomlist.ini");

		for (int iIndex = 1; ; iIndex++)
		{
			sFieldName.Format(_T("Active_%d"), iIndex);
			WORD wIniID = GetPrivateProfileInt(sFieldName, TEXT("ServerID"), -1, sFilePath);
			if (0 >= wIniID)
			{
				break;
			}

			if (wID == wIniID)
			{
				TCHAR tchReturnVal[MAX_PATH]=_T("");
				GetPrivateProfileString(sFieldName, _T("Url"), _T(""), tchReturnVal, sizeof(tchReturnVal), sFilePath);

				if (0 < lstrlen(tchReturnVal))
				{
					tagGlobalUserData UserData=g_GlobalUnits.GetGolbalUserData();
					_snprintf(tchURL, sizeof(tchURL), tchReturnVal, UserData.dwUserID, UserData.szKey);
					break;
				}
			}
		}

		m_pActivity->Create(IDD_ACTIVE, this);
		m_pActivity->SetURL(tchURL);
		m_pActivity->ShowWindow(SW_SHOW);
		return true;
	}

	return false;
}

BOOL CPlazaDlg::IsCanIntervalConnect()
{
	if (GetTickCount() < m_dwLastConnectTime)
		m_dwLastConnectTime = GetTickCount();
	
	TRACE3("���ʱ�����ã�%ld, %ld, %ld \r\n", GetTickCount(),  m_dwLastConnectTime, GetTickCount() - m_dwLastConnectTime);
	if (m_dwConnectIntervalTime*1000 >= GetTickCount() - m_dwLastConnectTime)
	{
		return FALSE;
	}

	m_dwLastConnectTime = GetTickCount();
	return TRUE;
}