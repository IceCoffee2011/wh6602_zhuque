#include "Stdafx.h"
#include "Resource.h"
#include "DlgLogon.h"
#include "DlgOption.h"
#include "Mmsystem.h"
#include "GlobalUnits.h"
#include "RoomViewItem.h"
#include "PlazaDlg.h"
#include "webpath.h"
#include "InitData.h"
#include ".\roomviewitem.h"

//#include "TableListDlg.h"

//////////////////////////////////////////////////////////////////////////

//��ť����
#define	BUTTON_AREA_WIDTH			200									//��ť����


//��ǰ����
#define GAME_USER_CURRENT_SORT_W		10000							//����



#define IDI_GAME_FRAME_IS_EXIST			20001							//���ͻ����Ƿ����
#define IDI_REQUEST_SERVER_EXIT_MSG		20002							//��������˳�
#define IDI_LOADING_ANIMACTION			20003							//Loading����

#define TIMER_GAMEFRAME_EXIST			10000							//
#define TIMER_REQUEST_SERVER_INTER		10000							

//////////////////////////////////////////////////////////////////////////

//�������
#define	SR_CREATE_ERROR				0									//��������
#define	SR_CREATE_SUCCESS			1									//�����ɹ�
#define	SR_ALREADY_EXIST			2									//�Ѿ�����

//////////////////////////////////////////////////////////////////////////
//�˵�����

//����˵�
#define	IDM_UM_WISPER				WM_USER+100							//˽�Ĳ˵�
#define	IDM_UM_COPY_NAME			WM_USER+101							//��������
#define	IDM_UM_SET_CHAT				WM_USER+102							//��������
#define	IDM_UM_CANECL_CHAT			WM_USER+103							//ȡ������
#define	IDM_UM_LOOK_LOCATION		WM_USER+104							//�鿴λ��
#define	IDM_UM_INVITE_PLAY			WM_USER+105							//������Ϸ
#define	IDM_UM_SEARCH_USER			WM_USER+106							//Ѱ���û�
#define	IDM_UM_SET_FRIEND			WM_USER+107							//��Ϊ����
#define	IDM_UM_SET_DETEST			WM_USER+108							//��Ϊ���

//��������
#define IDM_UM_MANAGER_USER			WM_USER+150							//Ȩ�޹���
#define IDM_UM_ISSUE_MESSAGE		WM_USER+151							//������Ϣ
#define IDM_IM_SYSTEM_OPTION		WM_USER+152							//ϵͳ����
#define IDM_UM_SEND_WARNNING		WM_USER+153							//���;���
#define IDM_UM_LOOK_USER_IP			WM_USER+154							//�鿴��ַ
#define IDM_UM_CUT_USER				WM_USER+155							//�û�����
#define IDM_UM_LIMIT_ACCOUN			WM_USER+156							//��ֹ�ʻ�
#define IDM_UM_EXIT_TABLE			WM_USER+157							//�˳�����
#define IDM_UM_DISABLE_USER_IP		WM_USER+158							//�������IP


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CRoomViewItem, CDialog)
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_COPYDATA()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_HIT_EXPMESSTION,	OnHitExpression)
	ON_MESSAGE(WM_UPDATE_FACE, OnUpdateFace)
	ON_MESSAGE(WM_FLASH_CHAIR, OnFlashChair)

	//ON_BN_CLICKED(IDC_BANK,	OnBnClickedBank)
	//ON_BN_CLICKED(IDC_AUTO_SIT,	OnBnClickedAutoSit)
	//ON_BN_CLICKED(IDC_FIND_USER, OnBnClickedFindUser)
	//ON_BN_CLICKED(IDC_QUIT_ROOM,	OnBnClickedQuitRoom)
	ON_BN_CLICKED(IDB_SEND_MSG, OnBnClickedSendChat)
	ON_BN_CLICKED(IDC_COLOR_SET, OnBnClickedColorSet)
	ON_BN_CLICKED(IDB_FACE, OnBnClickedExpression)
	ON_BN_CLICKED(IDC_CLEAN_SCREEN,	OnBnClickedCleanScreen)
	//ON_BN_CLICKED(IDC_BUGLE,	OnBnClickedBugle)
	// 	ON_BN_CLICKED(IDC_CLOSE,OnBnClose)
	// 	ON_BN_CLICKED(IDC_RETURN,OnBnReturn)
	ON_BN_CLICKED(IDC_PGDWON,OnBnDwon)
	ON_BN_CLICKED(IDC_PGUP,OnBnUp)
	// 	ON_NOTIFY(NM_DBLCLK, IDC_USER_LIST, OnNMDblclkUserList)
	// 	ON_NOTIFY(NM_RCLICK, IDC_USER_LIST, OnNMRclickUserList)
	// 	ON_NOTIFY(NM_CLICK, IDC_USER_LIST, OnNMclickUserList)
	ON_BN_CLICKED(IDC_AUTO_QUEUE, OnAutoQueue)

	// 	ON_BN_CLICKED(IDC_BT_CLOSE,&CRoomViewItem::OnBnClickedClose)
	// 	ON_BN_CLICKED(IDC_BT_RETURN,&CRoomViewItem::OnBnClickedReturn)
	// 	ON_BN_CLICKED(IDC_BT_RE_LOGON,&CRoomViewItem::OnBnClickedReLogon)
	// 	ON_BN_CLICKED(IDC_BT_SET,&CRoomViewItem::OnBnClickedSet)
	// 	ON_BN_CLICKED(IDC_MINSIZE,&CRoomViewItem::OnBnClickedMinsize) 
	ON_BN_CLICKED(IDB_BT_FRIEND,&CRoomViewItem::OnBnClickedFindUser) 

	ON_COMMAND(IDM_CANCEL_CONNECT,	OnCommandCancelConnect)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

CRoomViewItem::CRoomViewItem() : CDialog(IDD_GAME_ROOM)
,m_hGameLoadOver(m_hEventGameLoadOver[0])
, m_hClosePromtWndr(m_hEventGameLoadOver[1])
{
	//��������
	m_wGameGenre=0;
	m_dwVideoAddr=0L;
	m_bRectifyHor=false;
	m_bRectifyVor=false;
	m_bInitDialog=false;
	m_bHideUserList=false;
	m_bQueueState=true;
	m_dwLimitChatTimes=0;
	m_dwMenuUserID=0L;
	m_wReqTableID=INVALID_TABLE;
	m_wReqChairID=INVALID_CHAIR;
	m_wFindTableID=INVALID_TABLE;
	m_nListIndex=0;
	memset(&m_ListColumnInfo,0,sizeof(m_ListColumnInfo));
	m_QueueNum=0;
	m_bLogonFull=false;
	m_btExitTag = 0;
	m_bRequestServerExit = false;

	//��������
	m_cbHideUserInfo=FALSE;
	//m_gameListManager = NULL;

	//���ñ���
	m_pGameOption=NULL;
	m_pServerOption=NULL;

	//�����û�
	m_dwChatUserID=0L;
	m_szChatName[0]=0;

	//������Ϣ
	m_szShareName[0]=0;
	m_hWndChannel=NULL;
	m_hShareMemory=NULL;
	m_pShareMemory=NULL;
	memset(&m_GameProcessInfo,0,sizeof(m_GameProcessInfo));
	::memset(&m_tagGameFrameStatus, 0, sizeof(m_tagGameFrameStatus));

	//���ݻ���
	m_wPacketTableID=INVALID_TABLE;
	m_wPacketChairID=INVALID_CHAIR;

	//���ñ���
	m_pMeUserItem=NULL;
	m_pListServer=NULL;
	m_ServiceStatus=ServiceStatus_Null;
	memset(&m_OptionBuffer,0,sizeof(m_OptionBuffer));
	m_pFindUserDlg=NULL;

	//cxfm_dwWinGemsToday = 0;
	//ZeroMemory(&m_ptOrder, sizeof(m_ptOrder));
	//ZeroMemory(&m_ptWinGemsToday, sizeof(m_ptWinGemsToday));


	//��ʼ��������Ϣ
	m_strSaveGameAffiche.clear();

	m_memdc.m_hDC = NULL;

	//cxf 11-26 m_RoomViewAffiche = NULL;

	m_wAnimAction=0;

	m_bIsDestroyStatus = false;

	m_wCloseState=0;
	
	// add by HouGuoJiang 2011-11-25
	memset(m_hEventGameLoadOver, 0, sizeof(m_hEventGameLoadOver));
	m_strGameLoadOver = "MY_GAME_LOAD_OVER";	// ע�⣺��������Ϸ�е����ֵҪ��˴�һ��
	m_hEventGameLoadOver[0] = CreateEvent(NULL, FALSE, FALSE, m_strGameLoadOver);
	m_hEventGameLoadOver[1] = CreateEvent(NULL, FALSE, FALSE, NULL);

	//Author<cxf>;DateTime<2012-03-01>;Description<����>
	//m_bMasterRight=false;//Author<cxf>;DateTime<2012-02-27>

	return;
}

CRoomViewItem::~CRoomViewItem()
{

	//�����ڴ�
	m_fontGameTitleInfo.DeleteObject();

	/*cxf 11-26 if (NULL != m_RoomViewAffiche)
	{
	m_RoomViewAffiche->DestroyWindow();
	delete m_RoomViewAffiche;
	m_RoomViewAffiche = NULL;
	}*/
	if (m_pShareMemory!=NULL)
	{
		UnmapViewOfFile(m_pShareMemory);
		m_pShareMemory=NULL;
	}
	if (m_hShareMemory!=NULL)
	{
		CloseHandle(m_hShareMemory);
		m_hShareMemory=NULL;
	}

	/*
	for (INT_PTR i=0;i<m_ShortMessage.GetCount();i++) 
	{
	delete ((CShortMessage *)m_ShortMessage.GetAt(i));
	m_ShortMessage.SetAt(i,NULL);
	}
	*/

	SafeDelete(m_pFindUserDlg);
	//m_PropertyBar.DestroyBar();

	return;
}

//�ӿڲ�ѯ
void * __cdecl CRoomViewItem::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IViewItem,Guid,dwQueryVer);
	QUERYINTERFACE(IRoomViewItem,Guid,dwQueryVer);
	//QUERYINTERFACE(IPurchaseInfo,Guid,dwQueryVer);
	QUERYINTERFACE(ITCPSocketSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(IUserManagerSink,Guid,dwQueryVer);
	QUERYINTERFACE(IChannelMessageSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IViewItem,Guid,dwQueryVer);
	return NULL;
}

//��������
WORD CRoomViewItem::GetGameGenre()
{
	return m_wGameGenre;
}

//ö�����
tagUserData * CRoomViewItem::EnumLookonUser(WORD wIndex)
{
	return NULL;
}

//�����û�
tagUserData * CRoomViewItem::SearchUserItem(DWORD dwUserID)
{
	IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(dwUserID);

	return (pIUserItem!=NULL)?(pIUserItem->GetUserData()):NULL;
}

//�����û�
tagUserData * CRoomViewItem::SearchUserItem(LPCTSTR pszUserName)
{
	WORD wEnumIndex=0;
	while (true)
	{
		//��ȡ�û�
		IUserItem * pIUserItem=m_ClientUserManager.EnumUserItem(wEnumIndex++);
		if (pIUserItem==NULL) break;
		tagUserData * pUserData=pIUserItem->GetUserData();
		if(pUserData==NULL)continue;

		//�Ƚ�����
		CString strName,strFineUser;
		strName.Format(TEXT("%s"),pUserData->szName);
		strFineUser.Format(TEXT("%s"),pszUserName);
		if (strName==strFineUser) return pUserData;
	};
	return NULL;
}

//��ȡ���
const tagUserData * CRoomViewItem::GetUserInfo(WORD wChairID)
{
	return NULL;
}

//��ȡ�Լ�
const tagUserData * CRoomViewItem::GetMeUserInfo()
{
	return (m_pMeUserItem!=NULL)?(m_pMeUserItem->GetUserData()):NULL;
}

//������Ϣ
void CRoomViewItem::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	m_ClientSocket->SendData(wMainCmdID, wSubCmdID, pData, wDataSize);

	return;
}

//�ؼ���
void CRoomViewItem::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_BUGLE, m_btBugle);
	////DDX_Control(pDX, IDC_PHRASE, m_btPhrase);
	//DDX_Control(pDX, IDC_COLOR_SET, m_btColorSet);
	//DDX_Control(pDX, IDC_SEND_CHAT, m_btSendChat);
	//DDX_Control(pDX, IDC_EXPRESSION, m_btExpression);
	//DDX_Control(pDX, IDC_CLEAN_SCREEN, m_btCleanScreen);
	//DDX_Control(pDX, IDC_CHAT_INPUT, m_ChatInput);
	//DDX_Control(pDX, IDC_USER_LIST, m_UserListView);
	//DDX_Control(pDX, IDC_VOR_SPLITER, m_VorSplitter);
	//DDX_Control(pDX, IDC_CHAT_MESSAGE, m_ChatMessage);
	//DDX_Control(pDX, IDC_AUTO_SIT, m_btAutoSit);
	//DDX_Control(pDX, IDC_BANK,m_btBank);
	//DDX_Control(pDX, IDC_FIND_USER, m_btFindUser);
	//DDX_Control(pDX, IDC_CHAT_OBJECT, m_ChatObject);
	//DDX_Control(pDX, IDC_QUIT_ROOM, m_btQuitRoom);
	//DDX_Control(pDX, IDC_HOR_SPLITER, m_HorSplitter);
	// 	DDX_Control(pDX,IDC_BT_RE_LOGON,m_btnReLogon);
	// 	DDX_Control(pDX,IDC_BT_RETURN,m_btnReturn);
	// 	DDX_Control(pDX,IDC_BT_SET,m_btnSet);
	// 	DDX_Control(pDX,IDC_BT_CLOSE,m_btnClose);  
	// 	DDX_Control(pDX,IDC_MINSIZE,m_btnMinsize); 
	// 	DDX_Control(pDX,IDB_BT_DEPOSITORY,m_btnDepository);
	// 	DDX_Control(pDX,IDB_BT_FRIEND,m_btnFriend);
	// 	DDX_Control(pDX,IDB_BT_SHOP,m_btnShop);
	// 	DDX_Control(pDX,IDB_BT_TASK,m_btnTask);
	// 	DDX_Control(pDX,IDB_BTN_MY_TOOL,m_btnTool);
	DDX_Control(pDX,IDC_PGDWON,m_btnPgDn);
	DDX_Control(pDX,IDC_PGUP,m_btnPgUp);
	DDX_Control(pDX,IDC_RICHEDIT2_OUTPUT,m_ChatMessage);
	DDX_Control(pDX,IDC_EDIT_INPUT,m_editInput);
	DDX_Control(pDX,IDC_COMBO1,m_ChatObject);
	DDX_Control(pDX,IDB_FACE,m_btExpression);
	DDX_Control(pDX,IDB_SEND_MSG,m_btSendChat);
	//DDX_Control(pDX,IDB_BT_DESPOKEN_TABLE,m_btnDespokenTable);
}
void CRoomViewItem::OnOK()
{
	//AfxMessageBox(TEXT("CRoomViewItem::OnOK"));
}
//��ʼ������
BOOL CRoomViewItem::OnInitDialog()
{
	__super::OnInitDialog();

	// add by HouGuoJiang 2011-11-25
	// �������̣߳���Ӧ��Ϸ�ĳ�ʼ������¼��Ա���������ʾ����
	// ��Ե����⣺��ʾ��������̫���ˣ���Ϸ���ػ�δ��ɣ�������ͬͻȻ��ʧ�ˣ��ͻ���֪����
	bool bResult = createThreads();

	//SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE)^0x80000);
	//���ÿؼ�
	//m_UserListView.InitUserListView();
	//m_VorSplitter.SetSplitterSink(this);
	//m_HorSplitter.SetSplitterSink(this);

	//m_ChatInput.LimitText(MAX_CHAT_LEN-1);
	//m_ChatMessage.SetBackgroundColor(FALSE,RGB(230,249,255));

	//�����ؼ�
	//m_BrowerAD.Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,100,NULL);
	//m_BrowerAD.Navigate(TEXT("http://qp.foxuc.net/AD/GameRoomAD.asp"),NULL,NULL,NULL,NULL);

	//���߿ؼ�
	// 	m_PropertyBar.SetSkinResource(GetResInstanceHandle(),&g_GlobalUnits.m_PropertyViewImage);
	// 	m_PropertyBar.CreateBar(this);
	// 	m_PropertyBar.SetPurchaseInfoSink(this);

	////������ʾ�ؼ�
	//m_ToolTipCtrl.Create(this);
	//m_ToolTipCtrl.Activate(TRUE);
	//m_ToolTipCtrl.AddTool(GetDlgItem(IDC_AUTO_SIT),TEXT("�Զ�Ѱ����Ϸλ��"));
	//m_ToolTipCtrl.AddTool(GetDlgItem(IDC_FIND_USER),TEXT("�����û�"));
	//m_ToolTipCtrl.AddTool(GetDlgItem(IDC_QUIT_ROOM),TEXT("�˳�����"));
	//m_ToolTipCtrl.AddTool(GetDlgItem(IDC_EXPRESSION),TEXT("����"));
	//m_ToolTipCtrl.AddTool(GetDlgItem(IDC_COLOR_SET),TEXT("��ɫ"));
	//m_ToolTipCtrl.AddTool(GetDlgItem(IDC_CLEAN_SCREEN),TEXT("����"));
	//m_ToolTipCtrl.AddTool(GetDlgItem(IDC_SEND_CHAT),TEXT("����"));
	//m_ToolTipCtrl.AddTool(GetDlgItem(IDC_BUGLE),TEXT("С����"));

	////�������
	//m_ChatInput.AddString(TEXT("�����ҵȵĻ���Ҳл��"));
	//m_ChatInput.AddString(TEXT("��Һã��ܸ��˼�����λ��"));
	//m_ChatInput.AddString(TEXT("��ô�ֶ����ˣ�����̫����..."));
	//m_ChatInput.AddString(TEXT("��Ҫ�ߣ���ս��������"));
	//m_ChatInput.AddString(TEXT("������������̫�����..."));
	//m_ChatInput.AddString(TEXT("���ǽ������ѣ��ܸ�����������ϵ��ʽ��"));
	//m_ChatInput.AddString(TEXT("����MM,����GG?"));
	//m_ChatInput.AddString(TEXT("��㣬��������л�ˣ�"));
	//m_ChatInput.AddString(TEXT("ʹ�죬ʹ�죬��ʮ��ţ�⣬����þƣ�"));
	//m_ChatInput.AddString(TEXT("��Ҫ���ˣ���ʲô�ó���..."));
	//m_ChatInput.AddString(TEXT("�ټ��ˣ��һ������ҵģ�BYE"));

	////�������
	//m_ChatObject.AddString(TEXT("������"));
	//m_ChatObject.SetCurSel(0);

	//cxf
	CString strPath;
	GetImagePlazaPath(strPath);
	bool bAdministration = IsAdministrationHavePurview();

	if (bAdministration)
	{
		m_ImageBack.LoadImage(TEXT(strPath+"\\Tablelist\\table_bg.bmp"));
		//m_ImageBackGM.LoadImage(TEXT(strPath+"GM_TABLE_LIST.png"));

		m_btnPgDn.ShowWindow(SW_SHOW);
		m_btnPgUp.ShowWindow(SW_SHOW);
	}
	else 
	{
		m_ImageBack.LoadImage(TEXT(strPath+"ImageLoading.png"));
		m_ImgAnimAction.LoadImage(TEXT(strPath+"ImageLoadingAnimation.png"));
		m_ImgClose.LoadImage(TEXT(strPath+"BTCloseTableList.png"));

		m_btnPgDn.ShowWindow(SW_HIDE);
		m_btnPgUp.ShowWindow(SW_HIDE);
	}

	HINSTANCE hInstance=g_GlobalUnits.m_PlatformResourceModule->GetResInstance();	
	// 	m_btnReLogon.LoadRgnImage(hInstance,g_GlobalUnits.m_PlatformFrameImage.uBtFrameRelogon);
	// 	m_btnReturn.LoadRgnImage(hInstance,g_GlobalUnits.m_PlatformFrameImage.uBtFrameRetrue);
	// 	m_btnSet.LoadRgnImage(hInstance,g_GlobalUnits.m_PlatformFrameImage.uBtFrameOption);
	// 	m_btnClose.LoadRgnImage(hInstance,g_GlobalUnits.m_PlatformFrameImage.uBtFrameClose);
	// 	m_btnMinsize.LoadRgnImage(hInstance,g_GlobalUnits.m_PlatformFrameImage.uBtFrameMin);
	// 	m_btnDepository.LoadRgnImage(hInstance,g_GlobalUnits.m_PlatformFrameImage.uBtDepository);
	// 	m_btnFriend.LoadRgnImage(hInstance,g_GlobalUnits.m_PlatformFrameImage.uBtFriend);
	// 	m_btnShop.LoadRgnImage(hInstance,g_GlobalUnits.m_PlatformFrameImage.uBtShop);
	// 	m_btnTask.LoadRgnImage(hInstance,g_GlobalUnits.m_PlatformFrameImage.uBtTask);
	// 	m_btnTool.LoadRgnImage(hInstance,g_GlobalUnits.m_PlatformFrameImage.uBtTool);

	//cxf 1108 m_btExpression.LoadRgnImage(hInstance,g_GlobalUnits.m_ServerViewImage.uBtExpression);
	//cxf 1108 m_btSendChat.LoadRgnImage(hInstance,g_GlobalUnits.m_ServerViewImage.uBtChatChannel);

	//������Ϸ���洰��

	/*cxf 11-26 m_RoomViewAffiche = new CRoomViewAffiche();
	if (NULL == m_RoomViewAffiche)
	{
	AfxMessageBox("��Ϸ���洰�ڴ���ʧ�ܣ�");
	return false;
	}
	CRect wndRect(0, 0, 0, 0);
	m_RoomViewAffiche->Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,wndRect,this,1929);
	m_RoomViewAffiche->SetWindowPos(NULL, 150, 5, 659, 34, SWP_SHOWWINDOW);*/


	//�����ؼ�
	//cxf 1108 CRect CreateRect(0,0,0,0);
	//cxf 1108 m_AutoQueue.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CreateRect,this,IDC_AUTO_QUEUE);

	//��ť����
	m_bQueueState=true;
	hInstance=AfxGetInstanceHandle();
	//cxf 1108 m_AutoQueue.LoadRgnImage(hInstance,IDB_AUTO_QUEUE);
	//cxf 1108 m_AutoQueue.EnableWindow(FALSE);
	//m_btnDespokenTable.LoadRgnImage(hInstance, IDB_DESPOKEN_TABLE);
	m_btnPgDn.LoadRgnImage(hInstance, IDB_TABLE_LIST_PGDN);
	m_btnPgUp.LoadRgnImage(hInstance, IDB_TABLE_LIST_PGUP);
	//����
	//cxf 1108 m_pngNumber.LoadImage(hInstance, TEXT("ROOM_NUMBER"));
	//cxf 1108 m_pngText1.LoadImage(hInstance, TEXT("ROOM_TEXT_1"));
	//cxf 1108 m_pngText2.LoadImage(hInstance, TEXT("ROOM_TEXT_2"));
	//��
	//cxf 1108 m_pngImageHou.LoadImage(hInstance, TEXT("ROOM_IMAGE_HOU"));
	//��ǰ����
	//cxf 1108 m_pngImageCurSort.LoadImage(hInstance, TEXT("ROOM_IMAGE_CURSORT"));
	//����Ӯ��
	//cxf 1108 m_pngImageDayWin.LoadImage(hInstance, TEXT("ROOM_IMAGE_DAYWIN"));

	m_ChatMessage.SetBackgroundColor(FALSE, RGB(193,187,163));
	m_editInput.SetBackgroundColor(FALSE, RGB(60,23,1));

	//cxf 2011-10-22
	//cxf 1108 m_AutoQueue.ShowWindow(SW_HIDE);
	

	CFont font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	lf.lfHeight=12;
	lstrcpy(lf.lfFaceName, TEXT("����"));   // Request a face name "Arial".
	VERIFY(font.CreateFontIndirect(&lf));  // Create the font.
	m_ChatMessage.SetFont(&font);
	m_editInput.LimitText(MAX_CHAT_LEN-1);
	m_editInput.SetFont(&font);
	font.DeleteObject();

	//��Ϸ��������
	m_fontGameTitleInfo.CreateFont(20,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("����"));

	CString strChatObject[] = {TEXT("����"), TEXT("˽��"), TEXT("����")};
	for (int i=0; i<CountArray(strChatObject); i++)
	{
		m_ChatObject.AddString(strChatObject[i]);
	}
	m_ChatObject.SetCurSel(0);

	//����λ��
	CRect rcButton;

	//HDWP hDwp=BeginDeferWindowPos(32);
	//const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	// // 	DeferWindowPos(hDwp,m_btnReLogon,NULL,821,680,0,0,uFlags);
	// // 	DeferWindowPos(hDwp,m_btnReturn,NULL,847,680,0,0,uFlags);
	// // 	DeferWindowPos(hDwp,m_btnMinsize,NULL,872,680,0,0,uFlags);
	// // 	DeferWindowPos(hDwp,m_btnSet,NULL,897,680,0,0,uFlags);
	// // 	DeferWindowPos(hDwp,m_btnClose,NULL,921,680,0,0,uFlags);
	// // 	DeferWindowPos(hDwp,m_btnFriend,NULL,292,29,0,0,uFlags);
	// // 	DeferWindowPos(hDwp,m_btnTool,NULL,423,29,0,0,uFlags);
	// // 	DeferWindowPos(hDwp,m_btnTask,NULL,554,29,0,0,uFlags);
	// // 	DeferWindowPos(hDwp,m_btnDepository,NULL,684,29,0,0,uFlags);
	// // 	DeferWindowPos(hDwp,m_btnShop,NULL,812,29,0,0,uFlags);
	// 	DeferWindowPos(hDwp,m_ChatMessage,NULL,19,511,263,161,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS);
	// 	DeferWindowPos(hDwp,m_editInput,NULL,22,677,169,19,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS);
	// 	DeferWindowPos(hDwp,m_btSendChat,NULL,194,676,0,0,uFlags);
	// 	DeferWindowPos(hDwp,m_btExpression,NULL,218,676,0,0,uFlags);
	// 	DeferWindowPos(hDwp,m_ChatObject,NULL,242,676,38,21,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS);
	// 	DeferWindowPos(hDwp,m_AutoQueue,NULL,516,126,0,0,uFlags);
	// 	DeferWindowPos(hDwp,m_btnDespokenTable,NULL,516,177,0,0,uFlags);
	//EndDeferWindowPos(hDwp);


	//���ñ���
	m_bInitDialog=true;
	//������Դ
	UpdateSkinResource();



	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	// 	m_ToolTipCtrl.AddTool(&m_btnDepository,TEXT("�ر���"));
	// 	m_ToolTipCtrl.AddTool(&m_btnFriend,TEXT("����"));
	// 	m_ToolTipCtrl.AddTool(&m_btnShop,TEXT("�̳�"));
	// 	m_ToolTipCtrl.AddTool(&m_btnTool,TEXT("����"));
	// 	m_ToolTipCtrl.AddTool(&m_btnTask,TEXT("����"));
	// 	m_ToolTipCtrl.AddTool(&m_btnReLogon,TEXT("���µ�½"));
	// 	m_ToolTipCtrl.AddTool(&m_btnReturn,TEXT("����"));
	// 	m_ToolTipCtrl.AddTool(&m_btnSet,TEXT("��������"));
	// 	m_ToolTipCtrl.AddTool(&m_btnClose,TEXT("�ر���Ϸ����"));
	// 	m_ToolTipCtrl.AddTool(&m_btnMinsize,TEXT("��С������"));
	//cxf 1108 m_ToolTipCtrl.AddTool(&m_btExpression,TEXT("����ͷ��"));
	//cxf 1108 m_ToolTipCtrl.AddTool(&m_btSendChat,TEXT("����"));
	//cxf 1108 m_ToolTipCtrl.AddTool(&m_AutoQueue,TEXT("�Զ��Ŷ�"));
	//	m_ToolTipCtrl.AddTool(&m_btnDespokenTable,TEXT("Ԥ������"));

	m_bIsDestroyStatus = false;
	m_wAnimAction=0;				//����֡��
	//����λ��
	m_rcAnimAction.left=395;
	m_rcAnimAction.top=379;
	m_rcAnimAction.right=m_rcAnimAction.left+m_ImgAnimAction.GetWidth()/12;
	m_rcAnimAction.bottom=m_rcAnimAction.top+m_ImgAnimAction.GetHeight();

	m_rcCloseRect.left=908;
	m_rcCloseRect.top=8;
	m_rcCloseRect.right=m_rcCloseRect.left+m_ImgClose.GetWidth()/5;
	m_rcCloseRect.bottom=m_rcCloseRect.top+m_ImgClose.GetHeight();
	//��ʼ��������Ϣ
	m_strSaveGameAffiche.clear();
	//Author<cxf>;DateTime<2012-03-01>;Description<����>
	//m_bMasterRight=false;//Author<cxf>;DateTime<2012-02-27>

	return TRUE;
}

//��Ϣ����
BOOL CRoomViewItem::PreTranslateMessage(MSG * pMsg)
{
	if (WM_KEYDOWN == pMsg->message)
	{
		if (VK_RETURN == pMsg->wParam)
		{
			if (ServiceStatus_Serviceing == m_ServiceStatus)
			{
				OnBnClickedSendChat();
				return TRUE;
			}
		}
		else if (::GetKeyState(VK_CONTROL)<0)
		{
			if (70==pMsg->wParam || 102==pMsg->wParam)
			{
				SetCtrolAddF();
				return TRUE;
			}
		}
	}

	if (m_ToolTipCtrl.m_hWnd != NULL)
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	return __super::PreTranslateMessage(pMsg);
}

//�˵�����
BOOL CRoomViewItem::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_UM_WISPER:			//˽����Ϣ
		{
			//Ѱ�����
			if (m_dwMenuUserID==0L) return TRUE;
			IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(m_dwMenuUserID);
			if (pIUserItem==NULL) return TRUE;

			//����˽��
			if (m_dwMenuUserID!=m_pMeUserItem->GetUserID()) SendUserWhisper(pIUserItem);

			return TRUE;
		}
	case IDM_UM_COPY_NAME:		//��������
		{
			//Ѱ�����
			if (m_dwMenuUserID==0L) return TRUE;
			IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(m_dwMenuUserID);
			if (pIUserItem==NULL) return TRUE;

			//�򿪼��а�
			if (OpenClipboard()==FALSE) return TRUE;
			if (EmptyClipboard()==FALSE) { CloseClipboard(); return TRUE; }

			//��������
			HANDLE hData=GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,NAME_LEN);
			if (hData==NULL) 
			{
				CloseClipboard();
				return TRUE;
			}
			LPTSTR szMemName=(LPTSTR)GlobalLock(hData);
			lstrcpy(szMemName,pIUserItem->GetUserName());
			SetClipboardData(CF_TEXT,hData);
			GlobalUnlock(hData);
			CloseClipboard();

			//��ʾ����
			// 			m_ChatInput.Paste();
			// 			m_ChatInput.SetFocus();
			m_editInput.Paste();
			m_editInput.SetFocus();
			return TRUE;
		}
	case IDM_UM_SET_CHAT:		//��������
		{
			//Ѱ�����
			if (m_dwMenuUserID==0L) return TRUE;
			IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(m_dwMenuUserID);
			if (pIUserItem==NULL) return TRUE;

			//���ö���
			if (m_dwMenuUserID!=m_pMeUserItem->GetUserID())
			{
				SetChatObject(pIUserItem);
				//m_ChatInput.SetFocus();
				m_editInput.SetFocus();
			}

			return TRUE;
		}
	case IDM_UM_CANECL_CHAT:	//ȡ������
		{
			//���ö���
			SetChatObject(NULL);
			//m_ChatInput.SetFocus();
			m_editInput.SetFocus();
			return TRUE;
		}
	case IDM_UM_LOOK_LOCATION:	//�鿴λ��
		{
			//Ѱ�����
			if (m_dwMenuUserID==0L) return TRUE;
			IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(m_dwMenuUserID);
			if (pIUserItem==NULL) return TRUE;

			//��λλ��
			tagUserData * pUserData=pIUserItem->GetUserData();
			if ((pUserData->wTableID!=INVALID_TABLE)&&(pUserData->wChairID!=INVALID_CHAIR))
			{
				m_TableFrame.EnsureVisibleTable(pUserData->wTableID);
				if (pUserData->cbUserStatus==US_LOOKON)
				{
					m_TableFrame.FlashTable(pUserData->wTableID);
				}
				else
				{
					m_TableFrame.FlashChair(pUserData->wTableID,pUserData->wChairID);
				}

			}

			return TRUE;
		}
		//case IDM_UM_SEARCH_USER:	//Ѱ�����
		//	{
		//		//�����ť
		//		OnBnClickedFindUser();

		//		return TRUE;
		//	}
	case IDM_UM_INVITE_PLAY:	//�������
		{
			//״̬�ж�
			if (m_ServiceStatus!=ServiceStatus_Serviceing)
			{
				return TRUE;
			}


			//������Ϣ
			tagUserData * pUserData=m_pMeUserItem->GetUserData();
			if (pUserData->cbUserStatus==US_PLAY)
			{
				return TRUE;
			}

			if (pUserData->wTableID==INVALID_TABLE) 
			{
				return TRUE;
			}


			//Ѱ�����
			if (m_dwMenuUserID==0L) 
			{
				return TRUE;
			}

			IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(m_dwMenuUserID);
			if (pIUserItem==NULL) 
			{
				return TRUE;
			}


			//��������
			CMD_GR_UserInviteReq UserInviteReq;
			UserInviteReq.wTableID=pUserData->wTableID;
			UserInviteReq.dwUserID=pIUserItem->GetUserID();
			m_ClientSocket->SendData(MDM_GR_USER,SUB_GR_USER_INVITE_REQ,&UserInviteReq,sizeof(UserInviteReq));

			//��ʾ��Ϣ
			m_MessageProxyHelper->InsertSystemString(TEXT("�ɹ������û���������"),0);

			return TRUE;
		}
	case IDM_UM_SET_FRIEND:		//��Ϊ����
		{
			//Ѱ�����
			if (m_dwMenuUserID==0L) return TRUE;
			IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(m_dwMenuUserID);
			if (pIUserItem==NULL) return TRUE;

			//��������
			tagCompanionItem CompanionItem;
			memset(&CompanionItem,0,sizeof(CompanionItem));
			tagUserData * pUserData=pIUserItem->GetUserData();

			//���ù�ϵ
			CompanionItem.wFaceID=pUserData->wFaceID;
			CompanionItem.dwUserID=pUserData->dwUserID;
			CompanionItem.dwGameID=pUserData->dwGameID;
			if (pUserData->cbCompanion==enCompanion_Friend)
			{
				pUserData->cbCompanion=enCompanion_UnKnow;
				CompanionItem.Companion=enCompanion_UnKnow;
			}
			else
			{
				pUserData->cbCompanion=enCompanion_Friend;
				CompanionItem.Companion=enCompanion_Friend;
			}
			lstrcpyn(CompanionItem.szAccounts,pUserData->szName,CountArray(CompanionItem.szAccounts));
			g_GlobalUnits.m_CompanionManager->InsertCompanionItem(CompanionItem);
			//((CGameFrame*)AfxGetMainWnd())->m_UserCompanionList.OnCompanionUpdate(&CompanionItem);

			//�ı�֪ͨ
			OnUserItemUpdate(pIUserItem);

			return TRUE;
		}
	case IDM_UM_SET_DETEST:		//��Ϊ���
		{
			//Ѱ�����
			if (m_dwMenuUserID==0L) return TRUE;
			IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(m_dwMenuUserID);
			if (pIUserItem==NULL) return TRUE;

			//��������
			tagCompanionItem CompanionItem;
			memset(&CompanionItem,0,sizeof(CompanionItem));
			tagUserData * pUserData=pIUserItem->GetUserData();

			//���ù�ϵ
			CompanionItem.wFaceID=pUserData->wFaceID;
			CompanionItem.dwUserID=pUserData->dwUserID;
			CompanionItem.dwGameID=pUserData->dwGameID;
			if (pUserData->cbCompanion==enCompanion_Detest)
			{
				pUserData->cbCompanion=enCompanion_UnKnow;
				CompanionItem.Companion=enCompanion_UnKnow;
			}
			else
			{
				pUserData->cbCompanion=enCompanion_Detest;
				CompanionItem.Companion=enCompanion_Detest;
			}
			lstrcpyn(CompanionItem.szAccounts,pUserData->szName,CountArray(CompanionItem.szAccounts));
			g_GlobalUnits.m_CompanionManager->InsertCompanionItem(CompanionItem);
			//			((CGameFrame*)AfxGetMainWnd())->m_UserCompanionList.OnCompanionUpdate(&CompanionItem);

			//�ı�֪ͨ
			OnUserItemUpdate(pIUserItem);

			//��ʾ��Ϣ
			if (pUserData->cbCompanion==enCompanion_Detest)
			{
				TCHAR szMessage[256]=TEXT("");
				_sntprintf(szMessage,CountArray(szMessage),TEXT("[ %s ] ����Ϊ�����ң�ϵͳ����������һ��������Ϣ"),pUserData->szName);
				m_MessageProxyHelper->InsertSystemString(szMessage,0);
			}

			return TRUE;
		}
	case IDM_UM_SEND_WARNNING:	//���;���
		{
			//Ѱ�����
			if (m_dwMenuUserID==0L) return TRUE;
			IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(m_dwMenuUserID);
			if (pIUserItem==NULL) return TRUE;

			//Ȩ���ж�
			tagUserData * pMeUserData=m_pMeUserItem->GetUserData();
			if (CUserRight::CanSendWarning(pMeUserData->dwMasterRight)==false) return TRUE;

			//��������
			if (m_ManagerSendWarning.m_hWnd==NULL) 
			{
				m_ManagerSendWarning.Create(IDD_SEND_WARNING,this);
				m_ManagerSendWarning.SetClientUserManager(QUERY_OBJECT_INTERFACE(m_ClientUserManager,IUnknownEx));
				m_ManagerSendWarning.SetClientSocket(QUERY_OBJECT_PTR_INTERFACE(m_ClientSocket.GetInterface(),IUnknownEx));
			}

			//���ñ���
			m_ManagerSendWarning.SetUserItem(pIUserItem);

			//��ʾ����
			m_ManagerSendWarning.ShowWindow(SW_SHOW);
			m_ManagerSendWarning.BringWindowToTop();

			return TRUE;
		}
	case IDM_UM_LOOK_USER_IP:	//�鿴��ַ
		{
			//Ѱ�����
			if (m_dwMenuUserID==0L) return TRUE;
			IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(m_dwMenuUserID);
			if (pIUserItem==NULL) return TRUE;

			//Ȩ���ж�
			tagUserData * pMeUserData=m_pMeUserItem->GetUserData();
			if (CUserRight::CanSeeUserIP(pMeUserData->dwMasterRight)==false) return TRUE;

			//��������
			CMD_GR_LookUserIP LookUserIP;
			LookUserIP.dwTargetUserID=pIUserItem->GetUserID();
			m_ClientSocket->SendData(MDM_GR_MANAGER,SUB_GR_LOOK_USER_IP,&LookUserIP,sizeof(LookUserIP));

			return TRUE;
		}
	case IDM_UM_EXIT_TABLE: // �߳�����
	case IDM_UM_CUT_USER:		//�߳��û�
		{
			//Ѱ�����
			if (m_dwMenuUserID==0L) return TRUE;
			IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(m_dwMenuUserID);
			if (pIUserItem==NULL) return TRUE;

			//Ȩ���ж�
			tagUserData * pMeUserData=m_pMeUserItem->GetUserData();
			if (CUserRight::CanCutUser(pMeUserData->dwMasterRight)==false) return TRUE;

			//��������
			CMD_GR_KillUser KillUser;
			KillUser.dwTargetUserID=pIUserItem->GetUserID();
			KillUser.bCutOnline = (IDM_UM_EXIT_TABLE==LOWORD(wParam)) ? FALSE : TRUE;
			m_ClientSocket->SendData(MDM_GR_MANAGER,SUB_GR_KILL_USER,&KillUser,sizeof(KillUser));

			return TRUE;
		}
	case IDM_UM_LIMIT_ACCOUN:	//�����ʻ�
		{
			//Ѱ�����
			if (m_dwMenuUserID==0L) return TRUE;
			IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(m_dwMenuUserID);
			if (pIUserItem==NULL) return TRUE;

			//Ȩ���ж�
			tagUserData * pMeUserData=m_pMeUserItem->GetUserData();
			if (CUserRight::CanForbidAccounts(pMeUserData->dwMasterRight)==false) return TRUE;

			//ȷ����ʾ
			TCHAR szMessage[128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("ȷʵҪ���� [ %s ] ���ʺ���"),pIUserItem->GetUserName());
			if (ShowMessageBox(szMessage,MB_ICONQUESTION|MB_YESNO)!=IDYES) return TRUE;

			//�û��ж�
			if (pIUserItem->GetUserID()!=m_dwMenuUserID) 
			{
				_sntprintf(szMessage,CountArray(szMessage),TEXT("�û� [ %s ] �Ѿ��뿪����Ϸ���䣬�ʺŶ���ʧ��"),pIUserItem->GetUserName());
				m_MessageProxyHelper->InsertSystemString(szMessage,0);

				return TRUE;
			}

			//��������
			CMD_GR_LimitAccounts LimitAccounts;
			LimitAccounts.dwTargetUserID=pIUserItem->GetUserID();
			m_ClientSocket->SendData(MDM_GR_MANAGER,SUB_GR_LIMIT_ACCOUNS,&LimitAccounts,sizeof(LimitAccounts));

			return TRUE;
		}
	case IDM_UM_MANAGER_USER:	//Ȩ�޹���
		{
			//Ѱ�����
			if (m_dwMenuUserID==0L) return TRUE;
			IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(m_dwMenuUserID);
			if (pIUserItem==NULL) return TRUE;

			//��������
			if (m_ManagerUserRight.m_hWnd==NULL) 
			{
				m_ManagerUserRight.Create(IDD_MANAGER_RIGHT,this);
				m_ManagerUserRight.SetMasterRight(m_pMeUserItem->GetUserData()->dwMasterRight);
				m_ManagerUserRight.SetClientUserManager(QUERY_OBJECT_INTERFACE(m_ClientUserManager,IUnknownEx));
				m_ManagerUserRight.SetClientSocket(QUERY_OBJECT_PTR_INTERFACE(m_ClientSocket.GetInterface(),IUnknownEx));
			}

			//���ñ���
			m_ManagerUserRight.SetUserItem(pIUserItem);

			//��ʾ����
			m_ManagerUserRight.ShowWindow(SW_SHOW);
			m_ManagerUserRight.BringWindowToTop();

			return TRUE;
		}
	case IDM_UM_ISSUE_MESSAGE:	//������Ϣ
		{
			//Ȩ���ж�
			tagUserData * pMeUserData=m_pMeUserItem->GetUserData();
			if (CUserRight::CanIssueMessage(pMeUserData->dwMasterRight)==false)
			{
				return TRUE;
			}


			//��������
			if (m_ManagerMessage.m_hWnd==NULL) 
			{
				m_ManagerMessage.Create(IDD_MANAGER_MESSAGE,this);
				m_ManagerMessage.SetClientSocket(QUERY_OBJECT_PTR_INTERFACE(m_ClientSocket.GetInterface(),IUnknownEx));
			}

			//��ʾ����
			m_ManagerMessage.ShowWindow(SW_SHOW);
			m_ManagerMessage.BringWindowToTop();

			return TRUE;
		}
	case IDM_IM_SYSTEM_OPTION:	//�������
		{
			//Ȩ���ж�
			tagUserData * pMeUserData=m_pMeUserItem->GetUserData();
			if (CUserRight::CanServerOption(pMeUserData->dwMasterRight)==false)
			{
				return TRUE;
			}


			//��������
			if (m_ManagerServer.m_hWnd==NULL) 
			{
				m_ManagerServer.Create(IDD_MANAGER_SERVER,this);
				m_ManagerServer.SetClientSocket(QUERY_OBJECT_PTR_INTERFACE(m_ClientSocket.GetInterface(),IUnknownEx));
			}

			//��ʾ����
			m_ManagerServer.ShowWindow(SW_SHOW);
			m_ManagerServer.BringWindowToTop();

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//�������ֵ�λ��
WORD CRoomViewItem::GetNumsBitNums(ULONG ulNums)
{
	if (ulNums < 10)
	{
		return 1;
	}
	else if (ulNums < 100)
	{
		return 2;
	}
	else if (ulNums < 1000)
	{
		return 3;
	}
	else
	{
		TCHAR tc[NAME_LEN]=TEXT("");
		_sntprintf(tc,sizeof(tc),TEXT("%d"),ulNums);
		LPCTSTR pszString = tc;
		WORD iCountBit = 0;
		while (*pszString != '\0')
		{
			++iCountBit;
			pszString++;
		}
		return iCountBit;
	}
}
CPoint CRoomViewItem::DrawNumber(CDC * pDC, CPoint pt, LONG number)
{
	if (NULL == pDC || m_pngNumber.IsNull())
		return CSize(0, 0);

	TCHAR tc[NAME_LEN]=TEXT("");
	_sntprintf(tc,sizeof(tc),TEXT("%d"),number);
	LPCTSTR pszString = tc;
	int nImgWidth = m_pngNumber.GetWidth()/10;
	int nImgHeight =  m_pngNumber.GetHeight();
	int nOffsetX = pt.x, nOffsetY = pt.y;
	int nLen = strlen(pszString);
	CRect rc(0, 0, 0, 0);
	rc.bottom = rc.top + nImgHeight;
	rc.right = rc.left + (nLen * nImgWidth);

	int n = 0;
	while (*pszString != '\0')
	{
		CString str;
		str =  *pszString;
		n = atoi((LPCTSTR)str);
		m_pngNumber.DrawImage(pDC, nOffsetX, nOffsetY,nImgWidth,nImgHeight,n*nImgWidth,0);
		nOffsetX += nImgWidth;
		pszString++;
	}
	return CPoint(nOffsetX, nOffsetY);
}

//���Ƶ�ǰ����
void CRoomViewItem::DrawCurSort(DWORD wSort)
{
	if (0 == wSort) return;
	//���Ƶ�ǰ����
	CRect sortarea(20, 593, 205, 621);
	ULONG ulCountWidth = 0;
	if (!m_pngImageCurSort.IsNull())
	{
		ulCountWidth = m_pngImageCurSort.GetWidth();
	}
	int nImgNumWidth = 0;
	int nImgNumHeight = 0;
	if (!m_pngNumber.IsNull())
	{
		nImgNumWidth = m_pngNumber.GetWidth()/10;
		nImgNumHeight =  m_pngNumber.GetHeight();
	}
	if (wSort > GAME_USER_CURRENT_SORT_W)
	{//��ʾ10000��
		//�������ֿ��
		WORD wBit = GetNumsBitNums(GAME_USER_CURRENT_SORT_W);
		ulCountWidth += wBit*nImgNumWidth;

		if (!m_pngImageHou.IsNull())
		{
			ulCountWidth += m_pngImageHou.GetWidth();
		}
	}
	else
	{
		WORD wBit = GetNumsBitNums(wSort);
		ulCountWidth += wBit*nImgNumWidth;
		if (!m_pngText1.IsNull())
		{//λ
			ulCountWidth += m_pngText1.GetWidth();
		}
	}
	CPoint sortPoint;
	sortPoint.x = sortarea.left + (sortarea.Width() - ulCountWidth)/2;
	sortPoint.y = sortarea.top + 7;
	if (!m_pngImageCurSort.IsNull())
	{
		m_pngImageCurSort.DrawImage(&m_memdc, sortPoint.x, sortPoint.y);
		sortPoint.x += m_pngImageCurSort.GetWidth();
	}
	if (wSort > GAME_USER_CURRENT_SORT_W)
	{
		CPoint numpoint;
		numpoint = sortPoint;
		numpoint.y += 3;
		numpoint =  DrawNumber(&m_memdc, numpoint, GAME_USER_CURRENT_SORT_W);
		if (!m_pngImageHou.IsNull())
		{
			sortPoint.x = numpoint.x;
			m_pngImageHou.DrawImage(&m_memdc, sortPoint.x, sortPoint.y);
		}
	}
	else
	{
		CPoint numpoint;
		numpoint = sortPoint;
		numpoint.y += 3;
		numpoint =  DrawNumber(&m_memdc, numpoint, wSort);
		if (!m_pngText1.IsNull())
		{
			sortPoint.x = numpoint.x;
			m_pngText1.DrawImage(&m_memdc, sortPoint.x, sortPoint.y);
		}
	}
}

//���ƽ���Ӯȡ
void CRoomViewItem::DrawDayWin(DWORD wWinNums)
{
	CRect sortarea(205, 593, 466, 621);
	ULONG ulCountWidth = 0;
	if (!m_pngImageDayWin.IsNull())
	{
		ulCountWidth = m_pngImageDayWin.GetWidth();
	}
	int nImgNumWidth = 0;
	int nImgNumHeight = 0;
	if (!m_pngNumber.IsNull())
	{
		nImgNumWidth = m_pngNumber.GetWidth()/10;
		nImgNumHeight =  m_pngNumber.GetHeight();
	}
	WORD wBit = GetNumsBitNums(wWinNums);
	ulCountWidth += wBit*nImgNumWidth;
	if (!m_pngText2.IsNull())
	{//��ʯ
		ulCountWidth += m_pngText2.GetWidth();
	}

	CPoint sortPoint;
	sortPoint.x = sortarea.left + (sortarea.Width() - ulCountWidth)/2;
	sortPoint.y = sortarea.top + 7;
	if (!m_pngImageDayWin.IsNull())
	{
		m_pngImageDayWin.DrawImage(&m_memdc, sortPoint.x, sortPoint.y);
		sortPoint.x += m_pngImageDayWin.GetWidth();
	}
	CPoint numpoint;
	numpoint = sortPoint;
	numpoint.y += 3;
	numpoint =  DrawNumber(&m_memdc, numpoint, wWinNums);
	if (!m_pngText2.IsNull())
	{
		sortPoint.x = numpoint.x;
		m_pngText2.DrawImage(&m_memdc, sortPoint.x, sortPoint.y);
	}
}
//�滭��Ϣ
void CRoomViewItem::OnPaint()
{
	CPaintDC dc(this);

	if (m_memdc.m_hDC)
	{
		//��������
		//m_TableFrame.OnPaint(&m_memdc);

		//������Ϸͼ��
		if (!m_pngGameICO.IsNull())
		{
			CRect rcico(260, 46, 390, 70);
			CPoint ptIco;
			ptIco.x = rcico.left+(rcico.Width()-m_pngGameICO.GetWidth())/2;
			ptIco.y = rcico.top;
			m_pngGameICO.DrawImage(&m_memdc, ptIco.x, ptIco.y);
		}

		/* cxf 2011-10-09 if (m_pMeUserItem)
		{
		//��ȡ�û�
		tagUserData * pUserData=m_pMeUserItem->GetUserData();
		if (NULL != pUserData)
		{
		DWORD dwOrder = 0;
		if (pUserData->dwOrder < 0)
		{
		dwOrder = 100001;
		}
		else
		{
		dwOrder = pUserData->dwOrder;
		}
		//���Ƶ�ǰ����
		DrawCurSort(dwOrder);

		if (m_dwWinGemsToday < 0)
		{
		m_dwWinGemsToday = 0;
		}
		//���ƽ���Ӯȡ
		DrawDayWin(m_dwWinGemsToday);

		}
		}*/
		//===================================Author<cxf>;Datetime<2012-02-23>

		//Author<cxf>;DateTime<2012-03-01>;Description<����>
		if (!IsAdministrationHavePurview()) // �����ͼ
		{
			if (!m_ImageBack.IsNull())
			{
					m_ImageBack.DrawImage(&m_memdc, 0, 0);
			}

			if (!m_ImgAnimAction.IsNull())
			{//����
				WORD wWidth=m_ImgAnimAction.GetWidth()/12;
				m_ImgAnimAction.DrawImage(&m_memdc,m_rcAnimAction.left,m_rcAnimAction.top,wWidth,
					m_ImgAnimAction.GetHeight(),m_wAnimAction*wWidth,0);
			}

			if (!m_ImgClose.IsNull())
			{//�رհ�ť
				WORD wWidth=m_ImgClose.GetWidth()/5;
				m_ImgClose.DrawImage(&m_memdc,m_rcCloseRect.left,m_rcCloseRect.top,wWidth,
					m_ImgClose.GetHeight(),m_wCloseState*wWidth,0);
			}

			if (!m_ImageBack.IsNull())
			{
				m_ImageBack.DrawImage(&m_memdc, 0, 0);
			}
			if (!m_ImgAnimAction.IsNull())
			{//����
				WORD wWidth=m_ImgAnimAction.GetWidth()/12;
				m_ImgAnimAction.DrawImage(&m_memdc,m_rcAnimAction.left,m_rcAnimAction.top,wWidth,
					m_ImgAnimAction.GetHeight(),m_wAnimAction*wWidth,0);
			}
			if (!m_ImgClose.IsNull())
			{//�رհ�ť
				WORD wWidth=m_ImgClose.GetWidth()/5;
				m_ImgClose.DrawImage(&m_memdc,m_rcCloseRect.left,m_rcCloseRect.top,wWidth,
					m_ImgClose.GetHeight(),m_wCloseState*wWidth,0);
			}
		}
		else  // ����Ա��ͼ
		{
			CRect rcWnd;
			GetClientRect(&rcWnd);
			if (!m_ImageBack.IsNull())
			{
				for (int i=0; i<rcWnd.Width(); i+=m_ImageBack.GetWidth())
				{
					for (int j=0; j<rcWnd.Height(); j+=m_ImageBack.GetHeight())
					{
						m_ImageBack.DrawImage(&m_memdc, i, j);
					}
				}
			}

			/*if (!m_ImageBackGM.IsNull())
			{
				m_ImageBackGM.DrawImage(&m_memdc, 0, 0);
			}*/

			//��������
			m_TableFrame.OnPaint(&m_memdc);

			//������Ϸͼ��
			if (!m_pngGameICO.IsNull())
			{
				CRect rcico(260, 46, 390, 70);
				CPoint ptIco;
				ptIco.x = rcico.left+(rcico.Width()-m_pngGameICO.GetWidth())/2;
				ptIco.y = rcico.top;
				m_pngGameICO.DrawImage(&m_memdc, ptIco.x, ptIco.y);
			}

			//�������
			CFont* def_font = m_memdc.SelectObject(&m_fontGameTitleInfo);
			m_memdc.SetTextColor(RGB(255,203,104));
			TCHAR szRoomTitle[128]=TEXT("");
			_sntprintf(szRoomTitle,sizeof(szRoomTitle),TEXT("����%d���Ŷ�"),m_QueueNum);
			m_memdc.DrawText(szRoomTitle,lstrlen(szRoomTitle),&m_rcQueue,DT_END_ELLIPSIS|DT_CENTER|DT_VCENTER|DT_SINGLELINE);

			_sntprintf(szRoomTitle,sizeof(szRoomTitle),TEXT("%d\/%d"), m_TableFrame.GetPage()+1, m_TableFrame.GetMaxPage());
			m_memdc.DrawText(szRoomTitle, lstrlen(szRoomTitle), &CRect(380,590,500,620), DT_END_ELLIPSIS|DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_memdc.SelectObject(def_font);
		}

		CRect rect;
		GetClientRect(&rect);
		dc.BitBlt(0, 0, rect.Width(), rect.Height(), &m_memdc, 0, 0, SRCCOPY);
	}

	return;
}

//�滭����
BOOL CRoomViewItem::OnEraseBkgnd(CDC * pDC)
{
	//RectifyControl(960,720);
	//m_TableFrame.OnPaint();
	/*
	CRect rect;
	GetClientRect(&rect);

	DrawPic(pDC,IDB_TABLE_LIST_BG,&rect);

	Invalidate(FALSE);
	UpdateWindow();*/


	return TRUE;
}

//λ����Ϣ
void CRoomViewItem::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//���ñ���
	m_bRectifyVor=false;

	CPaintDC dc(this);
	m_memdc.DeleteDC();
	CBitmap bmp;
	m_memdc.CreateCompatibleDC(&dc);
	bmp.CreateCompatibleBitmap(&dc, cx, cy);
	m_memdc.SelectObject(bmp);	
	m_memdc.SetBkMode(TRANSPARENT);

	//�����ؼ�
	RectifyControl(cx,cy);
	m_TableFrame.OnSize(nType, cx, cy);

	bmp.DeleteObject();

	return;
}

//��ʾ��Ϣ
void CRoomViewItem::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	//���ñ���
	//if (bShow) 
	//{
	//	UpdateTitleText();
	//}
	return;
}

void CRoomViewItem::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	if (IsAdministrationHavePurview())
	{
		lpwndpos->x += 6;
		lpwndpos->y += 30;
		lpwndpos->cx -= 12;
		lpwndpos->cy -= 35;
	}

	return 	__super::OnWindowPosChanging(lpwndpos);
}

/*
//˫����������
void CRoomViewItem::OnNMDblclkUserList(NMHDR * pNMHDR, LRESULT * pResult)
{
NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
if ((pListNotify->iItem!=-1)&&(m_cbHideUserInfo==FALSE))
{

IUserItem * pIUserItem=(IUserItem *)m_UserListView.GetItemData(pListNotify->iItem);
if ((pIUserItem!=NULL)&&(pIUserItem!=m_pMeUserItem))
{
SendUserWhisper(pIUserItem);
}



}

*pResult=0;
}

//�Ҽ�����û��б�
void CRoomViewItem::OnNMRclickUserList(NMHDR * pNMHDR, LRESULT *pResult)
{
NMITEMACTIVATE * pListNotify = (NMITEMACTIVATE *)pNMHDR;

if (pListNotify->iItem != -1)
{
CPoint MousePoint;
GetCursorPos(&MousePoint);

IUserItem * pIUserItem=(IUserItem *)m_UserListView.GetItemData(pListNotify->iItem);
if (pIUserItem!=NULL) ShowUserInfoMenu(pIUserItem,MousePoint);
}

*pResult=0;

return ;
}

//����û��б�
void CRoomViewItem::OnNMclickUserList(NMHDR * pNMHDR, LRESULT * pResult)
{
NMITEMACTIVATE * pListNotify = (NMITEMACTIVATE *)pNMHDR;

if (pListNotify->iItem != -1)
{
CPoint MousePoint;
GetCursorPos(&MousePoint);

//ѡ���û�
IUserItem * pIUserItem=(IUserItem *)m_UserListView.GetItemData(pListNotify->iItem);
if (pIUserItem!=NULL)
{
m_PropertyBar.SetUserDataInfo(pIUserItem->GetUserData());
}


}

*pResult=0;
return ;
}
*/
//����ͷ��
LRESULT CRoomViewItem::OnUpdateFace(WPARAM wParam, LPARAM lParam)
{
	DWORD dwUserID = DWORD(lParam);
	DWORD cbFaceVer = DWORD(wParam);

	//������Ϣ
	tagUserData *pUserData = m_ClientUserManager.SearchUserByUserID(dwUserID)->GetUserData();
	if(pUserData!=NULL)pUserData->dwCustomFaceVer = cbFaceVer;

	//����ͷ��
	if (pUserData->wTableID!=INVALID_TABLE)
	{
		IPC_UpdateFace UpdateFace;
		memset(&UpdateFace,0,sizeof(UpdateFace));
		UpdateFace.dwCustomFace = cbFaceVer;
		SendProcessData(IPC_MAIN_USER,IPC_SUB_UPDATE_FACE,&UpdateFace,sizeof(UpdateFace));
	}

	return 0;
}

//IPC ��Ϣ
BOOL CRoomViewItem::OnCopyData(CWnd *pWnd, COPYDATASTRUCT *pCopyDataStruct)
{
	if (m_IPCRecvCopyData.OnCopyData(pWnd->GetSafeHwnd(), pCopyDataStruct)==true) return TRUE;
	return __super::OnCopyData(pWnd, pCopyDataStruct);
}

//������Ϣ
LRESULT CRoomViewItem::OnHitExpression(WPARAM wParam, LPARAM lParam)
{
	CExpressionItem * pExpressionItem=m_Expression.GetExpressItem((WORD)wParam);
	if (pExpressionItem!=NULL)
	{
		long nStartChar=0, nEndChar;
		CString strBuffer=TEXT(""), strBufLeft, strBufRight;
		m_editInput.GetSel(nStartChar, nEndChar);

		m_editInput.GetWindowText(strBuffer);
		strBufLeft=strBuffer.Left(nStartChar);
		strBufRight=strBuffer.Right(strBuffer.GetLength()-nStartChar);

		strBufLeft+=pExpressionItem->m_szTrancelate;
		m_editInput.SetWindowText(strBufLeft+strBufRight);
		m_editInput.SetFocus();
		m_editInput.SetSel(strBufLeft.GetLength(),strBufLeft.GetLength());
	}

	return 0;
}

LRESULT CRoomViewItem::OnFlashChair(WPARAM wParam, LPARAM lParam)
{
	DWORD wUserID = (DWORD)wParam;
	if (0L==wUserID) 
		return TRUE;

	IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(wUserID);
	if (pIUserItem==NULL) 
		return TRUE;

	//��λλ��
	tagUserData * pUserData=pIUserItem->GetUserData();
	if ((INVALID_TABLE!=pUserData->wTableID)  &&  (INVALID_CHAIR!=pUserData->wChairID))
	{
		m_TableFrame.EnsureVisibleTable(pUserData->wTableID);
		if (US_LOOKON == pUserData->cbUserStatus)
		{
			m_TableFrame.FlashTable(pUserData->wTableID);
		}
		else
		{
			m_TableFrame.FlashChair(pUserData->wTableID,pUserData->wChairID);
		}

	}
}

//���Ͱ�ť
void CRoomViewItem::OnBnClickedSendChat()
{
	//������Ϣ
	CString strBuf;
	//m_ChatInput.GetWindowText(szChatMessage,CountArray(szChatMessage));
	//m_editInput.GetWindowText(szChatMessage,CountArray(szChatMessage));
	m_editInput.GetWindowText(strBuf);
	if (!strBuf.IsEmpty())
	{
		TCHAR szChatMessage[MAX_CHAT_LEN*4]=TEXT("");
		m_editInput.GetWindowText(szChatMessage,CountArray(szChatMessage));
		szChatMessage[CountArray(szChatMessage)-1]='\0';
		if (lstrlen(szChatMessage) < MAX_CHAT_LEN )
		{
			//��ʱ����
			DWORD dwCurTimes = GetTickCount();
			if(m_dwLimitChatTimes==0 || dwCurTimes-m_dwLimitChatTimes>=LIMIT_CHAT_TIMES)
			{

				m_dwLimitChatTimes = dwCurTimes;
				//m_ChatInput.SetWindowText(TEXT(""));
				m_editInput.SetWindowText(TEXT(""));
				SendChatPacket(m_dwChatUserID,szChatMessage,g_GlobalOption.m_crChatTX);
			}else
			{
				m_MessageProxyHelper->InsertGeneralString(TEXT("��˵��̫����,���Ժ�!"),RGB(255,0,0),MS_NORMAL,true);
			}
		}else
		{
			m_MessageProxyHelper->InsertGeneralString(TEXT("��˵�Ļ�̫����!"),RGB(255,0,0),MS_NORMAL,true);
		}

	}else
	{
		m_MessageProxyHelper->InsertGeneralString(TEXT("�������ݲ���Ϊ��!"),RGB(255,0,0),MS_NORMAL,true);
	}

	//���ý���
	//m_ChatInput.SetFocus();
	m_editInput.SetFocus();
	return;
}

//���鰴ť
void CRoomViewItem::OnBnClickedExpression()
{
	//�������鴰��
	if (m_Expression.GetSafeHwnd()==NULL) m_Expression.CreateExpression(this);

	//�ƶ�����
	CRect rcButton;
	CSize ExpressSize;
	m_Expression.GetFixSize(ExpressSize);
	//cxf 1108 m_btExpression.GetWindowRect(&rcButton);
	m_Expression.MoveWindow(rcButton.right-ExpressSize.cx,rcButton.top-ExpressSize.cy,ExpressSize.cx,ExpressSize.cy);
	m_Expression.ShowWindow(SW_SHOW);
	m_Expression.SetFocus();

	return;
}

//��ɫ��ť
void CRoomViewItem::OnBnClickedColorSet()
{
	//������ɫ
	CColorDialog ColorDialog(g_GlobalOption.m_crChatTX,CC_FULLOPEN);
	if (ColorDialog.DoModal()==IDOK) g_GlobalOption.m_crChatTX = ColorDialog.GetColor();

	//���ý���
	//m_ChatInput.SetFocus();
	m_editInput.SetFocus();
	return;
}

//������ť
void CRoomViewItem::OnBnClickedCleanScreen()
{
	m_MessageProxyHelper->CleanScreen();
	return;
}

//���Ȱ�ť
//void CRoomViewItem::OnBnClickedBugle()
//{
//	if(m_ClientSocket.GetInterface()==NULL)return;
//
//	//��ȡ�û�
//	tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
//	IUserItem *pMeUserItem = m_ClientUserManager.SearchUserByUserID(GlobalUserData.dwUserID);
//	tagUserData *pMeUserData = pMeUserItem->GetUserData();
//
//	//���öԻ���
//	ShowBugleDlg(NULL,m_ClientSocket.GetInterface(),pMeUserData);
//
//	return ;
//}

//������Ϣ
tagGameKind * __cdecl CRoomViewItem::GetKindInfo()
{
	if (m_pListServer!=NULL)
	{
		CListKind * pListKind=m_pListServer->GetListKind();
		ASSERT(pListKind!=NULL);
		if (pListKind!=NULL)
		{
			return pListKind->GetItemInfo();
		}
	}
	return NULL;
}

//���а�ť
//void CRoomViewItem::OnBnClickedBank()
//{
//	//��ȡ�û�
//	tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
//	IUserItem *pMeUserItem = m_ClientUserManager.SearchUserByUserID(GlobalUserData.dwUserID);
//	tagUserData *pMeUserData = pMeUserItem->GetUserData();
//
//	//��ʾ����
//	ShowBankStorageDlg(NULL,m_ClientSocket.GetInterface(),pMeUserData);
//
//	return;
//}

//���밴ť
//void CRoomViewItem::OnBnClickedAutoSit()
//{
//	//�ж�״̬
//	if (m_ServiceStatus!=ServiceStatus_Serviceing) return;
//
//	if ((m_wReqTableID!=INVALID_TABLE)||(m_wReqChairID!=INVALID_CHAIR))
//	{
//		TCHAR szMessage[]=TEXT("�ϴ�����û���յ���������Ӧ,���Ժ�");
//		ShowMessageBox(szMessage,MB_ICONINFORMATION);
//		return;
//	}
//
//	//�ж�״̬
//	tagUserData * pUserData=m_pMeUserItem->GetUserData();
//	if (pUserData->cbUserStatus==US_PLAY)
//	{
//		TCHAR szMessage[]=TEXT("��������Ϸ�У����ܻ�λ�ã����Ƚ�����Ϸ��");
//		ShowMessageBox(szMessage,MB_ICONINFORMATION);
//		return;
//	}
//
//	//�����׷�
//	if(m_cbHideUserInfo==TRUE)
//	{
//		//ģ�����
//		m_TableFrame.EnsureVisibleTable(0);
//		m_TableFrame.FlashChair(0,rand()%m_TableFrame.GetChairCount());
//
//		//�Զ�����
//		SendSitDownPacket(INVALID_TABLE,INVALID_CHAIR,NULL);
//		return;
//	}
//
//	//���������Ӳ�ȫ�յ�
//	tagFindTable FindInfo;
//	FindInfo.bAllNull=false;
//	FindInfo.bFilterPass=true;
//	FindInfo.bNotFull=true;
//	FindInfo.bOneNull=true;
//	FindInfo.bTwoNull=(m_TableFrame.GetChairCount()!=2);
//	FindInfo.wBeginTableID=m_wFindTableID+1;
//	FindInfo.wResultTableID=INVALID_TABLE;
//	FindInfo.wResultChairID=INVALID_CHAIR;
//	bool bSuccess=FindGameTable(FindInfo);
//	m_wFindTableID=FindInfo.wResultTableID;
//
//	//����ȫ����Ϸ��
//	if (bSuccess==false)
//	{
//		FindInfo.bAllNull=true;
//		FindInfo.bFilterPass=true;
//		FindInfo.bNotFull=true;
//		FindInfo.bOneNull=true;
//		FindInfo.bTwoNull=true;
//		FindInfo.wBeginTableID=m_wFindTableID+1;
//		FindInfo.wResultTableID=INVALID_TABLE;
//		FindInfo.wResultChairID=INVALID_CHAIR;
//		bSuccess=FindGameTable(FindInfo);
//		m_wFindTableID=FindInfo.wResultTableID;
//	}
//
//	//����ж�
//	if (bSuccess==true)
//	{
//		//Ч������
//		ASSERT(FindInfo.wResultTableID!=INVALID_TABLE);
//		ASSERT(FindInfo.wResultChairID!=INVALID_CHAIR);
//
//		//��������
//		WORD wChairID=FindInfo.wResultChairID;
//		m_TableFrame.EnsureVisibleTable(m_wFindTableID);
//		m_TableFrame.FlashChair(m_wFindTableID,wChairID);
//
//		//�Զ�����
//		PerformSitDownAction(m_wFindTableID,wChairID);
//	}
//	else
//	{
//		TCHAR szMessage[]=TEXT("�Ҳ���������������Ϸ����");
//		ShowMessageBox(szMessage,MB_ICONINFORMATION);
//	}
//
//	return;
//}

//���Ұ�ť
void CRoomViewItem::OnBnClickedFindUser()
{
	if (m_pFindUserDlg==NULL) 
	{
		m_pFindUserDlg=new CFindUserDlg(this,0);
		m_pFindUserDlg->m_UserList.m_wGameGenre=m_wGameGenre;
		m_pFindUserDlg->m_UserList.m_wKindID=m_pListServer->GetItemInfo()->wKindID;
	}
	if (m_pFindUserDlg->GetSafeHwnd()==NULL)
	{
		m_pFindUserDlg->Create(IDD_FIND_USER,this);
		m_pFindUserDlg->m_UserList.SetColumnDescribe(m_ListColumnInfo.ColumnItem,m_ListColumnInfo.wColumnCount);
	}
	
	m_pFindUserDlg->ShowWindow(SW_SHOW);
	return;
}

//�˳���ť
//void CRoomViewItem::OnBnClickedQuitRoom()
//{
////	((CGameFrame*)AfxGetMainWnd())->CloseRoomViewItem(this);
//	return;
//}

//���ذ�ť
void CRoomViewItem::OnBnReturn()
{
	SendRequestCloseRoomMsg(1);
}

//�رհ�ť
void CRoomViewItem::OnBnClose()
{
	CloseRoomViewItem();
	DestroyWindow();
	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
}
//��һҳ
void CRoomViewItem::OnBnUp()
{
	m_TableFrame.OnBnUp();
	// 	int iPage=m_TableFrame.GetPage();
	// 	int temp=m_TableFrame.GetPageNum();
	// 
	// 	//��ҳ�ж�
	// 	if(m_TableFrame.GetPageNum() == 0 && iPage == 0)return;
	// 
	// 	if(iPage > 0)
	// 	{
	// 		
	// 		iPage--;
	// 		m_TableFrame.SetPage(iPage);
	// 
	// // 		CRect rect;
	// // 		m_TableFrame.GetWindowRect(&rect);
	// // 		//ClientToScreen(&rect);
	// // 		InvalidateRect(rect, FALSE);
	// 		Invalidate(FALSE);
	// 		//m_TableFrame.Invalidate(FALSE);
	// //		UpdateWindow();
	// 	}
	// 	m_btnPgUp.EnableWindow(iPage > 0 ? TRUE: FALSE);
	//else
	//{
	//	WORD wTableCount=0;						//��Ϸ����

	//	//���ӷ���
	//	CRoomViewItem * pRoomViewItem=NULL;
	//	CListServer * pListServer=NULL;

	//	try
	//	{
	//		do 
	//		{
	//			if(m_nListIndex > 0)
	//				m_nListIndex--;
	//			else
	//			{
	//				ASSERT(FALSE);
	//				break;
	//			}

	//			pListServer=g_GlobalUnits.m_ServerListManager.EnumServerItem(m_nListIndex);
	//			if(pListServer == NULL)return;

	//			if(m_pListServer->m_GameServer.wSortID == pListServer->m_GameServer.wSortID && 
	//				m_pListServer->m_GameServer.wServerPort != pListServer->m_GameServer.wServerPort)
	//			{
	//				//�ر�����
	//				if (m_ClientSocket.GetInterface()!=NULL) m_ClientSocket->CloseSocket();

	//				//�ر���Ϸ
	//				CloseGameClient();

	//				//�����ڴ�
	//				if (m_pShareMemory!=NULL)
	//				{
	//					UnmapViewOfFile(m_pShareMemory);
	//					m_pShareMemory=NULL;
	//				}
	//				if (m_hShareMemory!=NULL)
	//				{
	//					CloseHandle(m_hShareMemory);
	//					m_hShareMemory=NULL;
	//				}

	//				wTableCount=m_TableFrame.GetAllLine();
	//				this->m_TableFrame.DestroyTableFrame();

	//				//��ʼ������
	//				if (this->InitRoomViewItem(pListServer)==false) 
	//					throw TEXT("��Ϸ�����ʼ��ʧ�ܣ�");
	//				else
	//					break;
	//			}
	//		} while (true);
	//	}
	//	catch (...) { SafeDelete(pRoomViewItem); }

	//	//������Ϸ
	//	bool bSuccess=this->ConnectGameServer();
	//	if (bSuccess==false)
	//	{
	//		//DestroyWindow();
	//		//SafeDelete(this);
	//		ASSERT(FALSE);
	//		return;
	//	}

	//	//��������
	//	int iPageNum=m_TableFrame.GetPageNum();

	//	if(iPageNum > 0)
	//		iPageNum--;

	//	if(iPageNum-(wTableCount-8)/9 > 0)
	//		m_TableFrame.SetPageNum(iPageNum-(wTableCount-8)/9);
	//	else
	//		m_TableFrame.SetPageNum(0);

	//	m_TableFrame.SetPage((wTableCount-8)/9);
	//}

	//Invalidate(FALSE);
	//UpdateWindow();
}
//��һҳ
void CRoomViewItem::OnBnDwon()
{
	m_TableFrame.OnBnDwon();
	// 	int iPage=m_TableFrame.GetPage();
	// 	if((m_TableFrame.GetAllLine()-8)/9 > iPage)
	// 	{
	// 		iPage++;
	// 		m_TableFrame.SetPage(iPage);
	// 	}
	// 
	// 	m_btnPgUp.EnableWindow(iPage > 0 ? TRUE: FALSE);

	//else
	//{
	//	//���ӷ���
	//	CRoomViewItem * pRoomViewItem=NULL;
	//	CListServer * pListServer=NULL;

	//	try
	//	{
	//		do 
	//		{
	//			pListServer=g_GlobalUnits.m_ServerListManager.EnumServerItem(m_nListIndex++);
	//			if(pListServer == NULL)return;

	//			if(m_pListServer->m_GameServer.wSortID == pListServer->m_GameServer.wSortID && 
	//				m_pListServer->m_GameServer.wServerPort != pListServer->m_GameServer.wServerPort)
	//			{
	//				//�ر�����
	//				if (m_ClientSocket.GetInterface()!=NULL) m_ClientSocket->CloseSocket();

	//				//�ر���Ϸ
	//				CloseGameClient();

	//				//�����ڴ�
	//				if (m_pShareMemory!=NULL)
	//				{
	//					UnmapViewOfFile(m_pShareMemory);
	//					m_pShareMemory=NULL;
	//				}
	//				if (m_hShareMemory!=NULL)
	//				{
	//					CloseHandle(m_hShareMemory);
	//					m_hShareMemory=NULL;
	//				}

	//				this->m_TableFrame.DestroyTableFrame();

	//				//��ʼ������
	//				if (this->InitRoomViewItem(pListServer)==false) 
	//					throw TEXT("��Ϸ�����ʼ��ʧ�ܣ�");
	//				else
	//					break;
	//			}

	//		} while (true);
	//	}
	//	catch (...) { SafeDelete(pRoomViewItem); }

	//	//������Ϸ
	//	bool bSuccess=this->ConnectGameServer();
	//	if (bSuccess==false)
	//	{
	//		//DestroyWindow();
	//		//SafeDelete(this);
	//		ASSERT(FALSE);
	//		return;
	//	}

	//	//��������
	//	iPage++;
	//	m_TableFrame.SetPageNum(iPage);
	//}

	//���½���
	//CRect rect;
	//m_TableFrame.GetWindowRect(&rect);
	//ClientToScreen(&rect);
	//m_TableFrame.Invalidate(FALSE);
	//UpdateWindow();
	Invalidate(FALSE);
}

//����Զ��Ŷ�
void CRoomViewItem::OnAutoQueue()
{
	//cxf 1108 if (!m_AutoQueue.IsWindowEnabled())
	//cxf 1108 {
	//cxf 1108 	return;
	//cxf 1108 }
	if (m_bQueueState)
	{
		m_ClientSocket->SendData(MDM_GR_USER,SUB_GR_AUTO_QUEUE);
	}
	else
	{
		m_ClientSocket->SendData(MDM_GR_USER,SUB_GR_AUTO_QUEUE_CANCEL);
	}
	//cxf 1108 m_AutoQueue.EnableWindow(FALSE);
	//cxf 1108 m_AutoQueue.Invalidate(FALSE);
	m_bQueueState = !m_bQueueState;

}

//������Ϣ
tagGameServer * __cdecl CRoomViewItem::GetServerInfo()
{
	if (m_pListServer==NULL) return NULL;
	return m_pListServer->GetItemInfo();
}

//�ϰ��¼�
void CRoomViewItem::NotifyBossCome(bool bBossCome)
{
	return;
	//������Ϣ
	if (m_pShareMemory==NULL) return;
	if (bBossCome==true) 
	{
		::ShowWindow(m_pShareMemory->hWndGameFrame,SW_HIDE);
	}
	else 
	{
		::ShowWindow(m_pShareMemory->hWndGameFrame,SW_SHOW);
	}

	return;
}

//�ر�ѯ��
bool __cdecl CRoomViewItem::QueryCloseRoom()
{
	//״̬�ж�
	if (m_pMeUserItem==NULL) return true;
	if (m_ServiceStatus!=ServiceStatus_Serviceing) return true;
	if (m_pMeUserItem->GetUserData()->cbUserStatus!=US_PLAY) return true;

#ifndef _DEBUG

	//��ʾ��Ϣ
	int iRet=ShowInformation(TEXT("��������Ϸ�У�ǿ���˳������۷֣�ȷʵҪǿ����"),0,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2);

	//�����ж�
	if (m_pMeUserItem==NULL) return true;
	if (m_ServiceStatus!=ServiceStatus_Serviceing) return true;
	if (m_pMeUserItem->GetUserData()->cbUserStatus!=US_PLAY) return true;

	//����ж�
	if (iRet==IDYES)
	{
		SendLeftGamePacket();
		return true;
	}

	return false;

#else

	return true;

#endif

}

//�ر�֪ͨ
void __cdecl CRoomViewItem::CloseRoomViewItem()
{
	//�ر�����
	if (m_ClientSocket.GetInterface()!=NULL) 
	{
		m_ClientSocket->CloseSocket();
	}

	CloseGameClient();

	//�����ڴ�
	if (m_pShareMemory!=NULL)
	{
		UnmapViewOfFile(m_pShareMemory);
		m_pShareMemory=NULL;
	}
	if (m_hShareMemory!=NULL)
	{
		CloseHandle(m_hShareMemory);
		m_hShareMemory=NULL;
	}

	//�ر�����ʱ��
	m_bRequestServerExit = false;
	KillTimer(IDI_REQUEST_SERVER_EXIT_MSG);

	//�رմ���
	DestroyWindow();

	return;
}

//��ť��Ϣ
bool CRoomViewItem::OnSplitterButton(CSkinSplitter * pSkinSplitter, CSkinButton * pSplitterButton)
{
	/*
	//���ñ���
	m_bHideUserList=!m_bHideUserList;

	//���ư�ť
	UINT uControlId = ((!m_bHideUserList)?GetServerViewRes().uBtHideUserList:GetServerViewRes().uBtShowUserList);
	m_VorSplitter.ShowSplitterButton(uControlId,GetResInstanceHandle());

	//��������
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());
	*/

	return true;
}

//�������Ϣ
bool CRoomViewItem::OnSplitterEvent(CSkinSplitter * pSkinSplitter, UINT uSplitID, int nXPos, int nYPos)
{
	/*
	//��ʾ�ؼ�
	if (m_bHideUserList==true)
	{
	//���ñ���
	m_bRectifyHor=true;
	m_bHideUserList=false;

	//���ð�ť
	UINT uControlId = ((!m_bHideUserList)?GetServerViewRes().uBtHideUserList:GetServerViewRes().uBtShowUserList);
	m_VorSplitter.ShowSplitterButton(uControlId,GetResInstanceHandle());
	}

	//��������
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());
	*/

	return true;
}

//��ʼ��
bool CRoomViewItem::InitRoomViewItem(CListServer * pListServer)
{
	//���ñ���
	ASSERT(pListServer!=NULL);
	m_pListServer=pListServer;
	//��Ϣ����
	if (m_MessageProxyHelper.CreateInstance()==false) 
	{
		return false;
	}
	if (m_MessageProxyHelper->SetRichEditHwnd(&m_ChatMessage)==false) 
	{
		return false;
	}

	//�������
	IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IUnknownEx);
	/*
	if (m_UserListView.m_GameRankManager.CreateInstance()==false)
	{
	return false;
	}
	*/

	if (m_ClientSocket.CreateInstance()==false) 
	{
		return false;
	}
	if (m_ClientSocket->SetTCPSocketSink(pIUnknownEx)==false)
	{
		return false;
	}
	if (m_ClientUserManager.SetUserManagerSink(pIUnknownEx)==false)
	{
		return false;
	}
	if (m_IPCRecvCopyData.SetChannelMessageSink(pIUnknownEx)==false) 
	{
		return false;
	}


	//������ɫ
	tagMessageOption MessageOption;
	memset(&MessageOption,0,sizeof(MessageOption));
	MessageOption.crName=g_GlobalOption.m_crMsgName;
	MessageOption.crSystemHead=g_GlobalOption.m_crMsgSysHead;
	MessageOption.crSystemString=g_GlobalOption.m_crMsgSysString;
	m_MessageProxyHelper->SetMessageOption(MessageOption);

	//���������ڴ�
	tagGameServer * pGameServer=m_pListServer->GetItemInfo();
	_sntprintf(m_szShareName,sizeof(m_szShareName),TEXT("0x8%ld%ld%ld%ld"),pGameServer->dwServerAddrCNN,pGameServer->wServerPort,time(NULL),rand()%100);
	m_hShareMemory=(HANDLE)CreateFileMapping((HANDLE)0xFFFFFFFFFFFF,NULL,PAGE_READWRITE,0,sizeof(tagShareMemory),m_szShareName);
	if ((m_hShareMemory==NULL)||(GetLastError()==ERROR_ALREADY_EXISTS))
	{
		return false;
	}
	m_pShareMemory=(tagShareMemory *)MapViewOfFile(m_hShareMemory,FILE_MAP_ALL_ACCESS,0,0,0);

	//���ù����ڴ�
	memset(m_pShareMemory,0,sizeof(tagShareMemory));
	m_pShareMemory->wDataSize=sizeof(tagShareMemory);
	m_pShareMemory->hWndGameServer=m_hWnd;
	m_pShareMemory->hWndGamePlaza=AfxGetMainWnd()->m_hWnd;

	//������Ϣ
	m_pServerOption=g_GlobalOption.GetServerOption(pListServer);
	m_pGameOption=g_GlobalOption.GetGameOption(pListServer->GetListKind());
	ASSERT(m_pGameOption!=NULL);
	ASSERT(m_pServerOption!=NULL);

	//CArray<CGameRoomInfo,CGameRoomInfo> & m_arrayGameRoomInfo = m_gameListManager->m_arrayGameRoomInfo;
	//CArray<CGameInfo,CGameInfo> & m_arrayGameInfoCollection = m_gameListManager->m_arrayGameInfoCollection;
	//int nCount = m_arrayGameRoomInfo.GetCount();
	//CString strGamename="";
	//WORD wKindID=0;

	//for(BYTE j=0;j<m_arrayGameInfoCollection.GetCount()&&nCount>0;j++)
	//{
	//	//���� ��Ϸ�����Դ��Ϣ
	//	if(m_arrayGameInfoCollection[j].m_wKindID == m_arrayGameRoomInfo[0].GetKindID())
	//	{
	//		strGamename = TEXT("Games/") + m_arrayGameInfoCollection[j].m_strName+TEXT(".dll");
	//		break;
	//	}
	//}
	//HINSTANCE  hInstance = LoadLibrary(strGamename);
	//if (hInstance)
	//{
	//	if (!m_pngGameICO.IsNull())
	//	{
	//		m_pngGameICO.DestroyImage();
	//	}
	//	m_pngGameICO.LoadImage(hInstance, TEXT("DESK_UNIT_ICO"));
	//}
	//FreeLibrary(hInstance);

	// 	//���Ա���
	// 	m_PropertyAttributeArrary.RemoveAll();
	// 	m_FlowerAttributeArrary.RemoveAll();



	return true;
}

//���ӷ�����
bool CRoomViewItem::ConnectGameServer()
{
	//��������IP��ַת����ʮ����pGameServer->dwServerAddr.
	m_ServiceStatus=ServiceStatus_Connecting;
	tagGameServer * pGameServer=m_pListServer->GetItemInfo();

	// 0���ţ�2�Զ�
	if (0 == g_GlobalOption.m_byNetWorkTag  || 2 == g_GlobalOption.m_byNetWorkTag)
	{
		return (m_ClientSocket->Connect(pGameServer->dwServerAddrCNN,pGameServer->wServerPort)==CONNECT_SUCCESS);
	}
	return (m_ClientSocket->Connect(pGameServer->dwServerAddrCUN,pGameServer->wServerPort)==CONNECT_SUCCESS);
}

//���͹���
bool CRoomViewItem::SendUserRule()
{
	if (m_ServiceStatus!=ServiceStatus_Serviceing) return false;
	return SendUserRulePacket();
}

//˽����Ϣ
bool CRoomViewItem::SendUserWhisper(IUserItem * pIUserItem)
{
	//��ȡ�û�
	ASSERT(pIUserItem!=NULL);
	if (pIUserItem==NULL) return false;

	//�����
	CShortMessage *pShortMessageWnd = ActiveShortWnd(pIUserItem->GetUserData()->dwUserID,pIUserItem,true);
	if (pShortMessageWnd==NULL)	
	{
		ShowMessageBox(TEXT("����Ϣ����̫���ˣ���رղ��ֶ���Ϣ�������ԣ�"),MB_ICONINFORMATION);
	}


	return true;
}

//�û��˵�
bool CRoomViewItem::ShowUserInfoMenu(IUserItem * pIUserItem, CPoint Point)
{
	//�ж�״̬
	if (m_ServiceStatus!=ServiceStatus_Serviceing) return false;

	//�û��ж�
	if (pIUserItem==NULL) 
	{
		m_MessageProxyHelper->InsertGeneralString(TEXT("���û��Ѿ��뿪������ "),RGB(0,0,0),MS_NORMAL,true);
		return false;
	}

	//����˵�
	CMenu UserInfoMenu;
	if (UserInfoMenu.CreatePopupMenu()==FALSE) return false;

	//��ȡ���
	tagUserData * pUserData=pIUserItem->GetUserData();
	tagUserData * pMeUserData=m_pMeUserItem->GetUserData();

	//�û���Ϣ
	WORD wTableID=pUserData->wTableID;
	WORD wChiarID=pUserData->wChairID;
	BYTE cbUserStatus=pUserData->cbUserStatus;

	//�����Ϣ
	WORD wMeTableID=pMeUserData->wTableID;
	WORD wMeChiarID=pMeUserData->wChairID;
	BYTE cbMeUserStatus=pMeUserData->cbUserStatus;

	//���ñ���
	TCHAR szBuffer[1024]=TEXT("");
	m_dwMenuUserID=pUserData->dwUserID;
	bool bMySelf=(pIUserItem==m_pMeUserItem);

	////���ò˵�
	//if ((bMySelf==false)&&(m_cbHideUserInfo==FALSE)) 
	//{
	//	UserInfoMenu.AppendMenu(0,IDM_UM_WISPER,TEXT("����˽����Ϣ..."));
	//	UserInfoMenu.SetDefaultItem(IDM_UM_WISPER,MF_BYCOMMAND);
	//}
	//UserInfoMenu.AppendMenu(0,IDM_UM_COPY_NAME,TEXT("�����û���"));

	////�������
	//if ((bMySelf==false)&&(m_cbHideUserInfo==FALSE)&&(m_dwChatUserID!=pUserData->dwUserID))
	//{
	//	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("�� [ %s ] ��̸"),pUserData->szName);
	//	UserInfoMenu.AppendMenu(0,IDM_UM_SET_CHAT,szBuffer);
	//}
	//if (m_szChatName[0]!=0)
	//{
	//	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("ȡ���� [ %s ] ��̸"),m_szChatName);
	//	UserInfoMenu.AppendMenu(0,IDM_UM_CANECL_CHAT,szBuffer);
	//}

	////�鿴λ��
	//UserInfoMenu.AppendMenu(MF_SEPARATOR);
	//if (bMySelf==false) UserInfoMenu.AppendMenu(0,IDM_UM_LOOK_LOCATION,TEXT("�鿴��ҵ�λ��"));
	//else UserInfoMenu.AppendMenu(0,IDM_UM_LOOK_LOCATION,TEXT("�鿴�ҵ�λ��"));
	//if (pUserData->wTableID==INVALID_TABLE) UserInfoMenu.EnableMenuItem(IDM_UM_LOOK_LOCATION,MF_BYCOMMAND|MF_GRAYED);

	////������Ϸ
	//if ((bMySelf==false)&&(m_cbHideUserInfo==FALSE)&&(cbMeUserStatus!=US_PLAY)&&(wMeTableID!=INVALID_TABLE)&&(cbUserStatus<US_PLAY))
	//{
	//	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("���� [ %s ] һ����Ϸ"),pUserData->szName);
	//	UserInfoMenu.AppendMenu(0,IDM_UM_INVITE_PLAY,szBuffer);
	//}

	////���ܲ˵�
	//UserInfoMenu.AppendMenu(MF_SEPARATOR);
	//UserInfoMenu.AppendMenu(0,IDM_UM_SEARCH_USER,TEXT("Ѱ�����..."));
	//if ((bMySelf==false)&&(m_cbHideUserInfo==FALSE))
	//{
	//	UserInfoMenu.AppendMenu(0,IDM_UM_SET_FRIEND,TEXT("��Ϊ����"));
	//	UserInfoMenu.AppendMenu(0,IDM_UM_SET_DETEST,TEXT("��Ϊ���"));
	//	if (pUserData->cbCompanion==enCompanion_Friend) 
	//	{
	//		UserInfoMenu.CheckMenuItem(IDM_UM_SET_FRIEND,MF_BYCOMMAND|MF_CHECKED);
	//	}
	//	if (pUserData->cbCompanion==enCompanion_Detest) 
	//	{
	//		UserInfoMenu.CheckMenuItem(IDM_UM_SET_DETEST,MF_BYCOMMAND|MF_CHECKED);
	//	}
	//}

	////�˵��ָ�
	//UserInfoMenu.AppendMenu(0,MF_SEPARATOR);

//��Ϸ ID
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("�ʺţ�%s"),pUserData->szAccounts);
	UserInfoMenu.AppendMenu(0,0,szBuffer);
	//�������
	LPCTSTR pszMember[]={TEXT("��ͨ��Ա"),TEXT("�м���Ա"),TEXT("�߼���Ա")};
	if ((m_cbHideUserInfo==FALSE)&&(pUserData->cbMemberOrder>0)&&(pUserData->cbMemberOrder<CountArray(pszMember)))
	{
		BYTE cbMemberOrder=pUserData->cbMemberOrder;
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("�ǳƣ�%s [ %s ]"),pUserData->szName, pszMember[cbMemberOrder]);
	}
	else _sntprintf(szBuffer,CountArray(szBuffer),TEXT("�ǳƣ�%s"),pUserData->szName);
	UserInfoMenu.AppendMenu(0,0,szBuffer);

	//��Ϸ ID
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("�û� ID��%ld"),pUserData->dwUserID);
	UserInfoMenu.AppendMenu(0,0,szBuffer);

	//����ǩ��
	if (pUserData->szUnderWrite[0]!=0)
	{
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("����ǩ����%s"),pUserData->szUnderWrite);
		UserInfoMenu.AppendMenu(0,0,szBuffer);
	}

	// �Ա�
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("�Ա�%s"), (0 == pUserData->cbGender) ? _T("��") : _T("Ů"));
	UserInfoMenu.AppendMenu(0,0,szBuffer);

	// �ȼ�
	//_sntprintf(szBuffer,CountArray(szBuffer),TEXT("�ȼ���%ld �ȼ���ν��%s"), pUserData->dwUserRight, pUserData->chGradeName);
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("�ȼ���%ld �ȼ���ν��%s"), pUserData->dwGrade, pUserData->chGradeName);//Author<cxf>;DateTime<2012-03-15>
	UserInfoMenu.AppendMenu(0,0,szBuffer);

	//���λ��
	if (pUserData->wTableID!=INVALID_TABLE)
	{
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("���ţ�%d"), pUserData->wTableID+1);
		UserInfoMenu.AppendMenu(0,0,szBuffer);
	}

	// ���
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("��ң�%I64d"), pUserData->lGold);
	UserInfoMenu.AppendMenu(0,0,szBuffer);

	// ��ʯ
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("��ʯ��%I64d"), pUserData->lGem);
	UserInfoMenu.AppendMenu(0,0,szBuffer);

	//�˵��ָ�
	UserInfoMenu.AppendMenu(0,MF_SEPARATOR);

	////������ֵ
	//_sntprintf(szBuffer,CountArray(szBuffer),TEXT("����ֵ��%ld"),pUserData->lExperience);
	//UserInfoMenu.AppendMenu(0,0,szBuffer);

	// ����������
	double dFleeRate=0.0;
	LONG lPlayCount=pIUserItem->GetUserPlayCount();
	if (pUserData->lFleeCount>0)
	{
		dFleeRate=(double)(pUserData->lFleeCount*100)/(double)lPlayCount;
		if (dFleeRate<0.01) dFleeRate=0.0;
	}
	//��Ӯ��Ϣ
	//_sntprintf(szBuffer,CountArray(szBuffer),TEXT("�ܾ�����%ld  Ӯ�֣�%ld  ��֣�%ld  �;֣�%ld"),lPlayCount,pUserData->lWinCount,pUserData->lLostCount,pUserData->lDrawCount);
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("�ܾ�����%ld  �����ʣ�%5.2f%%"), lPlayCount, dFleeRate);
	UserInfoMenu.AppendMenu(0,0,szBuffer);

	//������Ϣ
	if (m_wGameGenre==GAME_GENRE_GOLD)
	{
		//_sntprintf(szBuffer,1024,TEXT("��ң�%I64d  �����ʣ�%5.2f%%"),pUserData->lScore,dFleeRate);
		_sntprintf(szBuffer,1024,TEXT("Ӯ�֣�%ld  ��֣�%ld  �;֣�%ld  �Ӿ֣�%ld"), 
			pUserData->lWinCount, pUserData->lLostCount, pUserData->lDrawCount, pUserData->lFleeCount);
		UserInfoMenu.AppendMenu(0,0,szBuffer);
	}
	else
	{
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("���֣�%I64d  �ܾ�����%ld  �����ʣ�%5.2f%%"),pUserData->lScore,lPlayCount,dFleeRate);
		UserInfoMenu.AppendMenu(0,0,szBuffer);
	}

	//������Ϣ
	if (pUserData->szGroupName[0]!=0)
	{
		_sntprintf(szBuffer,CountArray(szBuffer),TEXT("���ţ�%s"),pUserData->szGroupName);
		UserInfoMenu.AppendMenu(0,0,szBuffer);
	}

	//�û�����
	if ((pMeUserData->dwUserID!=m_dwMenuUserID)&&(pMeUserData->dwMasterRight!=0L))
	{
		//����˵�
		UserInfoMenu.AppendMenu(0,MF_SEPARATOR);
		UserInfoMenu.AppendMenu(0,IDM_UM_EXIT_TABLE,TEXT("�߳�����"));
		UserInfoMenu.AppendMenu(0,IDM_UM_CUT_USER,TEXT("������"));
		//UserInfoMenu.AppendMenu(0,IDM_UM_DISABLE_USER_IP,TEXT("�������IP..."));
		UserInfoMenu.AppendMenu(0,IDM_UM_LOOK_USER_IP,TEXT("�鿴�û� IP..."));
		UserInfoMenu.AppendMenu(0,IDM_UM_LIMIT_ACCOUN,TEXT("�����û��ʺ�..."));
		UserInfoMenu.AppendMenu(0,IDM_UM_SEND_WARNNING,TEXT("���;�����Ϣ..."));
		//UserInfoMenu.AppendMenu(0,IDM_UM_MANAGER_USER,TEXT("�û�Ȩ������..."));

		//��ֹ�˵�
		DWORD dwMasterRight=pMeUserData->dwMasterRight;
		if (CUserRight::CanSendWarning(dwMasterRight)==false) UserInfoMenu.EnableMenuItem(IDM_UM_SEND_WARNNING,MF_BYCOMMAND|MF_GRAYED);
		if (CUserRight::CanSeeUserIP(dwMasterRight)==false) UserInfoMenu.EnableMenuItem(IDM_UM_LOOK_USER_IP,MF_BYCOMMAND|MF_GRAYED);
		if (CUserRight::CanCutUser(dwMasterRight)==false) UserInfoMenu.EnableMenuItem(IDM_UM_CUT_USER,MF_BYCOMMAND|MF_GRAYED);
		if (CUserRight::CanForbidAccounts(dwMasterRight)==false) UserInfoMenu.EnableMenuItem(IDM_UM_LIMIT_ACCOUN,MF_BYCOMMAND|MF_GRAYED);
	}

	//�������
	if (pMeUserData->dwMasterRight!=0L)
	{
		//����˵�
		UserInfoMenu.AppendMenu(0,MF_SEPARATOR);
		UserInfoMenu.AppendMenu(0,IDM_UM_ISSUE_MESSAGE,TEXT("����ϵͳ��Ϣ..."));
		UserInfoMenu.AppendMenu(0,IDM_IM_SYSTEM_OPTION,TEXT("��Ϸ��������..."));

		//��ֹ�˵�
		DWORD dwMasterRight=pMeUserData->dwMasterRight;
		if (CUserRight::CanIssueMessage(dwMasterRight)==false) UserInfoMenu.EnableMenuItem(IDM_UM_ISSUE_MESSAGE,MF_BYCOMMAND|MF_GRAYED);
		if (CUserRight::CanServerOption(dwMasterRight)==false) UserInfoMenu.EnableMenuItem(IDM_IM_SYSTEM_OPTION,MF_BYCOMMAND|MF_GRAYED);
	}

	//��ʾ�˵�
	UserInfoMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,Point.x,Point.y,this);

	return true;
}

//��Դ���
HINSTANCE CRoomViewItem::GetResInstanceHandle()
{
	return g_GlobalUnits.m_PlatformResourceModule->GetResInstance();
}

//��ȡ��Դ
tagServerViewImage CRoomViewItem::GetServerViewRes()
{
	return g_GlobalUnits.m_ServerViewImage;
}

//������Դ
bool CRoomViewItem::UpdateSkinResource()
{
	//��ȡ��Դ
	HINSTANCE hInstance=GetResInstanceHandle();

	//�б�����
	m_EncircleList.ImageML.SetLoadInfo(GetServerViewRes().uUserListML,hInstance);
	m_EncircleList.ImageMR.SetLoadInfo(GetServerViewRes().uUserListMR,hInstance);
	m_EncircleList.ImageTL.SetLoadInfo(GetServerViewRes().uUserListTL,hInstance);
	m_EncircleList.ImageTM.SetLoadInfo(GetServerViewRes().uUserListTM,hInstance);
	m_EncircleList.ImageTR.SetLoadInfo(GetServerViewRes().uUserListTR,hInstance);
	m_EncircleList.ImageBL.SetLoadInfo(GetServerViewRes().uUserListBL,hInstance);
	m_EncircleList.ImageBM.SetLoadInfo(GetServerViewRes().uUserListBM,hInstance);
	m_EncircleList.ImageBR.SetLoadInfo(GetServerViewRes().uUserListBR,hInstance);

	//��������
	m_EncircleChat.ImageML.SetLoadInfo(GetServerViewRes().uChatML,hInstance);
	m_EncircleChat.ImageMR.SetLoadInfo(GetServerViewRes().uChatMR,hInstance);
	m_EncircleChat.ImageTL.SetLoadInfo(GetServerViewRes().uChatTL,hInstance);
	m_EncircleChat.ImageTM.SetLoadInfo(GetServerViewRes().uChatTM,hInstance);
	m_EncircleChat.ImageTR.SetLoadInfo(GetServerViewRes().uChatTR,hInstance);
	m_EncircleChat.ImageBL.SetLoadInfo(GetServerViewRes().uChatBL,hInstance);
	m_EncircleChat.ImageBM.SetLoadInfo(GetServerViewRes().uChatBM,hInstance);
	m_EncircleChat.ImageBR.SetLoadInfo(GetServerViewRes().uChatBR,hInstance);

	//��������
	m_EncircleTable.ImageML.SetLoadInfo(GetServerViewRes().uTableML,hInstance);
	m_EncircleTable.ImageMR.SetLoadInfo(GetServerViewRes().uTableMR,hInstance);
	m_EncircleTable.ImageTL.SetLoadInfo(GetServerViewRes().uTableTL,hInstance);
	m_EncircleTable.ImageTM.SetLoadInfo(GetServerViewRes().uTableTM,hInstance);
	m_EncircleTable.ImageTR.SetLoadInfo(GetServerViewRes().uTableTR,hInstance);
	m_EncircleTable.ImageBL.SetLoadInfo(GetServerViewRes().uTableBL,hInstance);
	m_EncircleTable.ImageBM.SetLoadInfo(GetServerViewRes().uTableBM,hInstance);
	m_EncircleTable.ImageBR.SetLoadInfo(GetServerViewRes().uTableBR,hInstance);

	//��ȡ��Ϣ
	CSkinAide::GetEncircleInfo(m_ImageInfoChat,m_EncircleChat);
	CSkinAide::GetEncircleInfo(m_ImageInfoList,m_EncircleList);
	CSkinAide::GetEncircleInfo(m_ImageInfoTable,m_EncircleTable);

	////��ֿؼ�
	//UINT uControlId = ((!m_bHideUserList)?GetServerViewRes().uBtHideUserList:GetServerViewRes().uBtShowUserList);
	////m_VorSplitter.ShowSplitterButton(uControlId,GetResInstanceHandle());

	////���ð�ť
	//m_btAutoSit.SetButtonImage(GetServerViewRes().uBtAutoJoin,hInstance,false);
	//m_btBank.SetButtonImage(GetServerViewRes().uBtBank,hInstance,false);
	//m_btFindUser.SetButtonImage(GetServerViewRes().uBtSearchUser,hInstance,false);
	//m_btQuitRoom.SetButtonImage(GetServerViewRes().uBtQuitRoom,hInstance,false);
	////m_btPhrase.SetButtonImage(GetServerViewRes().uBtCleanScreen,hInstance,false);
	//m_btExpression.SetButtonImage(GetServerViewRes().uBtExpression,hInstance,false);
	//m_btColorSet.SetButtonImage(GetServerViewRes().uBtColorSet,hInstance,false);
	//m_btCleanScreen.SetButtonImage(GetServerViewRes().uBtCleanScreen,hInstance,false);
	//m_btSendChat.SetButtonImage(GetServerViewRes().uBtSendChat,hInstance,false);
	//m_btBugle.SetButtonImage(GetServerViewRes().uBtBugle,hInstance,false);

	//���µ���
	//m_PropertyBar.UpdateSkinResource();

	return true;
}

bool CRoomViewItem::IsAdministrationHavePurview(int iGrade)
{
	const tagGlobalUserData & UserData=g_GlobalUnits.GetGolbalUserData();
	if (CUserRight::CanSendWarning(UserData.nMasterRight))
	{
		return true;
	}

	return false;
}

void CRoomViewItem::SetCtrolAddF()
{
	if (IsAdministrationHavePurview())
	{
		OnBnClickedFindUser();
	}
}

//���͵�¼��
bool CRoomViewItem::SendLogonPacket()
{
	//��ȡ��Ϣ
	BYTE cbBuffer[SOCKET_PACKET];
	tagGlobalUserData & GlobalUserData=g_GlobalUnits.GetGolbalUserData();

	//��¼���ݰ�
	CMD_GR_LogonByUserID * pLogonByUserID=(CMD_GR_LogonByUserID *)cbBuffer;
	pLogonByUserID->dwUserID=GlobalUserData.dwUserID;
	pLogonByUserID->dwPlazaVersion=g_GlobalUnits.GetPlazaVersion();
	pLogonByUserID->dwKindID = m_pListServer->GetListKind()->GetItemInfo()->wKindID;
	lstrcpyn(pLogonByUserID->szPassWord,GlobalUserData.szPassWord,sizeof(pLogonByUserID->szPassWord));

	//�������к�
	tagClientSerial ClientSerial;
	g_GlobalUnits.GetClientSerial(ClientSerial);

	//�������ݰ�
	CSendPacketHelper Packet(cbBuffer+sizeof(CMD_GR_LogonByUserID),sizeof(cbBuffer)-sizeof(CMD_GR_LogonByUserID));
	Packet.AddPacket(&ClientSerial,sizeof(ClientSerial),DTP_COMPUTER_ID);
	m_ClientSocket->SendData(MDM_GR_LOGON,SUB_GR_LOGON_USERID,cbBuffer,sizeof(CMD_GR_LogonByUserID)+Packet.GetDataSize());

	return true;
}

//������������
bool CRoomViewItem::SendOptionsPacket()
{
	return true;
}

//������������
bool CRoomViewItem::SendStandUpPacket()
{
	m_ClientSocket->SendData(MDM_GR_USER,SUB_GR_USER_STANDUP_REQ);

	return true;
}

//����ǿ������
bool CRoomViewItem::SendLeftGamePacket()
{
	m_ClientSocket->SendData(MDM_GR_USER,SUB_GR_USER_LEFT_GAME_REQ);

	return true;
}
//����ȡ���ų���Ϣ MDM_GP_USER  SUB_GR_USER_CANCEL
bool CRoomViewItem::SendCancelQueue()
{
	//m_ClientSocket->SendData(MDM_GP_USER,SUB_GR_USER_CANCEL);
	return true;
}

//���ͷ������
bool CRoomViewItem::SendUserRulePacket()
{
	//�������ݰ�
	CMD_GR_UserRule UserRule;
	UserRule.bLimitWin=m_pGameOption->m_bLimitWin;
	UserRule.bLimitFlee=m_pGameOption->m_bLimitFlee;
	UserRule.wWinRate=m_pGameOption->m_wWinRate;
	UserRule.wFleeRate=m_pGameOption->m_wFleeRate;
	UserRule.lMaxScore=m_pGameOption->m_lMaxScore;
	UserRule.lLessScore	=m_pGameOption->m_lLessScore;
	UserRule.bLimitScore=m_pGameOption->m_bLimitScore;
	UserRule.bPassword=m_pServerOption->m_bPassword;
	UserRule.bCheckSameIP=g_GlobalOption.m_bCheckSameIP;
	lstrcpyn(UserRule.szPassword,m_pServerOption->m_szPassword,CountArray(UserRule.szPassword));

	//�������ݰ�
	m_ClientSocket->SendData(MDM_GR_USER,SUB_GR_USER_RULE,&UserRule,sizeof(UserRule));

	return true;
}

//�����Թ�����
bool CRoomViewItem::SendLookonPacket(WORD wTableID, WORD wChairID, LPCTSTR pszTablePass)
{
	//�������ݰ�
	CMD_GR_UserSitReq UserUserSitReq;
	memset(&UserUserSitReq,0,sizeof(UserUserSitReq));
	UserUserSitReq.wTableID=wTableID;
	UserUserSitReq.wChairID=wChairID;
	lstrcpyn(UserUserSitReq.szTablePass,pszTablePass,sizeof(UserUserSitReq.szTablePass));
	UserUserSitReq.cbPassLen=CountStringBuffer(UserUserSitReq.szTablePass);

	//�������ݰ�
	WORD wSendSize=sizeof(UserUserSitReq)-sizeof(UserUserSitReq.szTablePass)+UserUserSitReq.cbPassLen;
	m_ClientSocket->SendData(MDM_GR_USER,SUB_GR_USER_LOOKON_REQ,&UserUserSitReq,wSendSize);

	return true;
}

//������������
bool CRoomViewItem::SendSitDownPacket(WORD wTableID, WORD wChairID, LPCTSTR pszTablePass)
{
	//�������ݰ�
	CMD_GR_UserSitReq UserSitReq;
	memset(&UserSitReq,0,sizeof(UserSitReq));
	UserSitReq.wTableID=wTableID;
	UserSitReq.wChairID=wChairID;
	lstrcpyn(UserSitReq.szTablePass,pszTablePass,sizeof(UserSitReq.szTablePass));
	UserSitReq.cbPassLen=CountStringBuffer(UserSitReq.szTablePass);

	//�������ݰ�
	WORD wSendSize=sizeof(UserSitReq)-sizeof(UserSitReq.szTablePass)+UserSitReq.cbPassLen;
	m_ClientSocket->SendData(MDM_GR_USER,SUB_GR_USER_SIT_REQ,&UserSitReq,wSendSize);

	return true;
}

//�Թ۶���
bool CRoomViewItem::PerformLookonAction(WORD wTableID, WORD wChairID)
{
	//Ч������
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);

	//����Ч��
	if (CanSitDownTable(wTableID,wChairID,true,true)==false) return false;

	//�������
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqChairID!=INVALID_CHAIR))
	{
		IUserItem * pIUserItem=m_TableFrame.GetUserInfo(m_wReqTableID,m_wReqChairID);
		if (pIUserItem==m_pMeUserItem)
		{
			m_TableFrame.SetUserInfo(m_wReqTableID,m_wReqChairID,NULL);
		}

	}

	//��������
	m_wReqTableID=wTableID;
	m_wReqChairID=wChairID;
	m_wFindTableID=INVALID_TABLE;
	m_TableFrame.EnsureVisibleTable(wTableID);
	SendLookonPacket(wTableID,wChairID,m_pServerOption->m_szEnterPassword);

	return true;
}

//���¶���
bool CRoomViewItem::PerformSitDownAction(WORD wTableID, WORD wChairID)
{
	//Ч������
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);

	//����Ч��
	if (CanSitDownTable(wTableID,wChairID,false,true)==false) return false;

	//�������
	if ((m_wReqTableID!=INVALID_TABLE)&&(m_wReqChairID!=INVALID_CHAIR))
	{
		IUserItem * pIUserItem=m_TableFrame.GetUserInfo(m_wReqTableID,m_wReqChairID);
		if (pIUserItem==m_pMeUserItem)
		{
			m_TableFrame.SetUserInfo(m_wReqTableID,m_wReqChairID,NULL);
		}
	}

	//��������
	m_wReqTableID=wTableID;
	m_wReqChairID=wChairID;
	m_wFindTableID=INVALID_TABLE;
	m_TableFrame.EnsureVisibleTable(wTableID);
	SendSitDownPacket(wTableID,wChairID,m_pServerOption->m_szEnterPassword);

	//Ԥ����ʾ
	if (m_TableFrame.GetChairCount()<=MAX_CHAIR_NORMAL)
	{
		IUserItem * pIUserItem=m_TableFrame.GetUserInfo(wTableID,wChairID);
		if(pIUserItem==NULL)
		{
			m_TableFrame.SetUserInfo(wTableID,wChairID,m_pMeUserItem);
		}
	}

	return true;
}

//������������
bool CRoomViewItem::SendChatPacket(DWORD dwTargetUserID, LPCTSTR pszChatMessage, COLORREF crFontColor)
{
	//��ȡ�û�
	ASSERT(m_pMeUserItem!=NULL);
	tagUserData * pUserData=m_pMeUserItem->GetUserData();

	//��������
	CMD_GR_UserChat UserChat;
	UserChat.crFontColor=crFontColor;
	UserChat.dwTargetUserID=dwTargetUserID;
	UserChat.dwSendUserID=pUserData->dwUserID;
	lstrcpyn(UserChat.szChatMessage,pszChatMessage,CountArray(UserChat.szChatMessage));
	UserChat.wChatLength=CountStringBuffer(UserChat.szChatMessage);

	//��������
	WORD wSendSize=sizeof(UserChat)-sizeof(UserChat.szChatMessage)+UserChat.wChatLength;
	m_ClientSocket->SendData(MDM_GR_USER,SUB_GR_USER_CHAT,&UserChat,wSendSize);

	return true;
}

//������Ϸ��Ϣ
bool CRoomViewItem::IPCSendGameInfo(CIPCSendCopyData * pSendCopyData)
{
	//Ч�����
	ASSERT(pSendCopyData!=NULL);
	ASSERT(m_pMeUserItem!=NULL);

	//�������
	tagUserData * pUserData=m_pMeUserItem->GetUserData();
	CListKind * pListKind=m_pListServer->GetListKind();
	tagGameKind * pGameKind=pListKind->GetItemInfo();
	tagGameServer * pGameServer=m_pListServer->GetItemInfo();

	//��������
	IPC_GF_ServerInfo ServerInfo;
	memset(&ServerInfo,0,sizeof(ServerInfo));
	ServerInfo.dwUserID=pUserData->dwUserID;
	ServerInfo.wTableID=pUserData->wTableID;
	ServerInfo.wChairID=pUserData->wChairID;
	ServerInfo.wKindID=pGameKind->wKindID;
	ServerInfo.wServerID=pGameServer->wServerID;
	ServerInfo.wGameGenre=m_wGameGenre;
	ServerInfo.dwVideoAddr=m_dwVideoAddr;
	ServerInfo.cbHideUserInfo=m_cbHideUserInfo;
	ServerInfo.wChairCount=m_TableFrame.GetChairCount();
	lstrcpyn(ServerInfo.szKindName,pGameKind->szKindName,CountArray(ServerInfo.szKindName));
	lstrcpyn(ServerInfo.szServerName,pGameServer->szServerName,CountArray(ServerInfo.szServerName));
	//cxf
	ServerInfo.wSortID=pGameServer->wSortID;//��Ϸ����
	ServerInfo.lCellScore=pGameServer->lCellScore;//��ͽ�����Ϸ������

	//��������
	pSendCopyData->SendData(IPC_MAIN_CONFIG,IPC_SUB_SERVER_INFO,&ServerInfo,sizeof(ServerInfo));

	return true;
}

//�����û�
bool CRoomViewItem::IPCSendTableUsers(CIPCSendCopyData * pSendCopyData)
{
	//Ч�����
	ASSERT(pSendCopyData!=NULL);

	//�����Լ���Ϣ
	ASSERT(m_pMeUserItem!=NULL);
	tagUserData * pMeUserData=m_pMeUserItem->GetUserData();
	ASSERT(pMeUserData->wTableID!=(WORD)INVALID_TABLE);
	if (pMeUserData->wTableID==(WORD)INVALID_TABLE) return false;
	SendTableUser(m_pMeUserItem,pSendCopyData);

	//���������û�
	IUserItem * pIUserItem=NULL;
	tagUserData * pUserData=NULL;

	//��Ϸ�û�
	WORD wEnumIndex=0;
	while (true)
	{
		pIUserItem=m_ClientUserManager.EnumUserItem(wEnumIndex++);
		if (pIUserItem==NULL) break;
		if (pIUserItem==m_pMeUserItem) continue;
		pUserData=pIUserItem->GetUserData();
		if ((pUserData->wTableID==pMeUserData->wTableID)&&(pUserData->cbUserStatus!=US_LOOKON))
			SendTableUser(pIUserItem,pSendCopyData);
	};

	//�Թ��û�
	wEnumIndex=0;
	while (true)
	{
		pIUserItem=m_ClientUserManager.EnumUserItem(wEnumIndex++);
		if (pIUserItem==NULL) break;
		if (pIUserItem==m_pMeUserItem) continue;
		pUserData=pIUserItem->GetUserData();
		if ((pUserData->wTableID==pMeUserData->wTableID)&&(pUserData->cbUserStatus==US_LOOKON))
			SendTableUser(pIUserItem,pSendCopyData);
	};

	return true;
}

//������Ϸ
int CRoomViewItem::StartGameClient()
{
	//�ж��Ƿ��Ѿ�����
	ASSERT(m_pShareMemory!=NULL);
	if ((m_GameProcessInfo.hProcess!=NULL)&&(WaitForSingleObject(m_GameProcessInfo.hProcess,0)==WAIT_TIMEOUT))
	{
		if (m_pShareMemory->hWndGameFrame!=NULL)
		{
			//���ش���
			ShowWindow(SW_HIDE);
			GetParent()->ShowWindow(SW_HIDE);


			//��ʾ����
			::ShowWindow(m_pShareMemory->hWndGameFrame,SW_SHOW);
			::SetForegroundWindow(m_pShareMemory->hWndGameFrame);

			//������Ϣ
			CIPCSendCopyData SendCopyData(m_hWndChannel,m_hWnd);
			IPCSendGameInfo(&SendCopyData);
			IPCSendTableUsers(&SendCopyData);
			SendCopyData.SendData(IPC_MAIN_CONCTROL,IPC_SUB_START_FINISH);

			((CPlazaDlg*)GetParent())->UpdateMusicSound(false);
			return SR_ALREADY_EXIST;
		}
		else
		{
			::TerminateProcess(m_GameProcessInfo.hProcess,0);
			//��ʾ����
			GetParent()->ShowWindow(SW_SHOW);
			ShowWindow(SW_SHOW);
		}
	}

	//��������
	m_hWndChannel=NULL;
	m_pShareMemory->hWndGameFrame=NULL;
	CloseHandle(m_GameProcessInfo.hThread);
	CloseHandle(m_GameProcessInfo.hProcess);
	memset(&m_GameProcessInfo,0,sizeof(m_GameProcessInfo));

	

	// del by HouGuoJiang 2011-11-25
	////���ش���
	if (IsAdministrationHavePurview())
	{
		ShowWindow(SW_HIDE);							// ������ʾ����
	}
	GetParent()->ShowWindow(SW_HIDE);		// ���ش���ѡ�񷿼�Ĵ���

	//����������
	CString strCommonLine;
	CListKind * pListKind=m_pListServer->GetListKind();
	tagGameKind * pGameKind=pListKind->GetItemInfo();

	// strCommonLine	{0x03e1ae90 "LandCrazy.exe /RoomToken:0x810235189125010132394129526 /ServerType:2 /HideUserInfo:0"}	ATL::CStringT<char,StrTraitMFC_DLL<char,ATL::ChTraitsCRT<char> > >
	strCommonLine.Format(TEXT("%s /RoomToken:%s /ServerType:%ld /HideUserInfo:%d"),pGameKind->szProcessName,m_szShareName,m_wGameGenre,m_cbHideUserInfo);

	//������Ϸ�ͻ���
	STARTUPINFO StartInfo;
	memset(&StartInfo,0,sizeof(StartInfo));
	StartInfo.cb=sizeof(StartInfo);
	StartInfo.wShowWindow=SW_SHOWMAXIMIZED;
	BOOL bSuccess=CreateProcess(NULL,strCommonLine.GetBuffer(),NULL,NULL,FALSE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,&StartInfo,&m_GameProcessInfo);
	strCommonLine.ReleaseBuffer();
	if (bSuccess==FALSE)
	{
		memset(&m_GameProcessInfo,0,sizeof(m_GameProcessInfo));
		return SR_CREATE_ERROR;
	}
	::memset(&m_tagGameFrameStatus, 0, sizeof(m_tagGameFrameStatus));

	((CPlazaDlg*)GetParent())->UpdateMusicSound(false);

	return SR_CREATE_SUCCESS;
}

// �̹߳�������
// ��Ե����⣺��ʾ��������̫���ˣ���Ϸ���ػ�δ��ɣ�������ͬͻȻ��ʧ�ˣ��ͻ���֪����
unsigned __stdcall  CRoomViewItem::thread_game_load_over(void* pParam) 	
{
	CRoomViewItem* pThis = (CRoomViewItem*)pParam;
	//DWORD  iWaitTime = 30000;	// ms ����ĵȴ�ʱ�䣬
	DWORD dwResult;

	dwResult = WaitForMultipleObjects(GAME_LOAD_OVER_COUNT,  pThis->m_hEventGameLoadOver,  FALSE,  INFINITE);
	switch(dwResult)
	{
	case WAIT_TIMEOUT:			// ��ʱ
	case WAIT_OBJECT_0 + 0:	// �ӵ���Ϸ�ͻ��˵ļ�����ϵ�֪ͨ
		{
			if (pThis && pThis->GetSafeHwnd())
			{
				pThis->ShowWindow(SW_HIDE);		// ������ʾ����
			}

			break;
		}
	case WAIT_OBJECT_0 + 1:	// �ӵ�����ʾ���֪ͨ��ֱ�ӽ����߳�
		{
			return 0;	
		}
	}

	return 0;
}

// �������߳�
bool CRoomViewItem::createThreads()
{
	if(m_hEventGameLoadOver[0] && m_hEventGameLoadOver[1])
	{
		m_hThread = (HANDLE)_beginthreadex( NULL, 0, &thread_game_load_over, (void*)this, 0, &m_dwThreadID);

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

//�ر���Ϸ
void CRoomViewItem::CloseGameClient()
{
	//�رռ�ⴰ�ڴ���
	m_tagGameFrameStatus.bIsOpen = false;
	KillTimer(IDI_GAME_FRAME_IS_EXIST);

	//��ʾ����
	GetParent()->SetForegroundWindow();
	GetParent()->ShowWindow(SW_SHOW);

	//������Ϣ
	if (m_GameProcessInfo.hProcess!=NULL)
	{
		DWORD dwResult=::WaitForSingleObject(m_GameProcessInfo.hProcess,1000);
		if (dwResult==WAIT_TIMEOUT) 
			::TerminateProcess(m_GameProcessInfo.hProcess,0);

		::memset(&m_tagGameFrameStatus, 0, sizeof(m_tagGameFrameStatus));
	}

	//��������
	if (NULL != m_hWndChannel)
	{
		m_hWndChannel=NULL;
	}
	if (NULL != m_pShareMemory->hWndGameFrame)
	{
		m_pShareMemory->hWndGameFrame=NULL;
	}

	CloseHandle(m_GameProcessInfo.hThread);
	CloseHandle(m_GameProcessInfo.hProcess);
	memset(&m_GameProcessInfo,0,sizeof(m_GameProcessInfo));
	//cxf 1108 m_AutoQueue.EnableWindow(TRUE);
	return;
}

//���±���
void CRoomViewItem::UpdateTitleText()
{
	//��ȡλ��
	//CRect rcVorSplitter;
	//m_VorSplitter.GetWindowRect(&rcVorSplitter);
	//ScreenToClient(&rcVorSplitter);

	//ˢ�½���
	//CRect rcInvalidate;
	//rcInvalidate.SetRect(0,0,rcVorSplitter.left-BUTTON_AREA_WIDTH,m_ImageInfoTable.nTBorder);
	//InvalidateRect(&rcInvalidate);

	return;
}

//�滭����
void CRoomViewItem::DrawTableFrame(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient,rcVorSplitter;
	GetClientRect(&rcClient);
	//m_VorSplitter.GetWindowRect(&rcVorSplitter);
	//ScreenToClient(&rcVorSplitter);

	//�滭���
	CRect rcTable;
	rcTable.right=rcClient.right;
	rcTable.left=0;
	rcTable.bottom=rcClient.Height();
	rcTable.top=0;
	CSkinAide::DrawEncircleFrame(pDC,rcTable,m_EncircleTable);

	//�������
	TCHAR szRoomTitle[128]=TEXT("");
	CListKind * pListKind=m_pListServer->GetListKind();
	_sntprintf(szRoomTitle,sizeof(szRoomTitle),TEXT("[ %s ] - [ %s ] ��%ld �ˣ�"),pListKind->GetItemInfo()->szKindName,
		m_pListServer->GetItemInfo()->szServerName,m_ClientUserManager.GetOnLineCount());

	//�������
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(64,64,64));
	pDC->SelectObject(CSkinResourceManager::GetDefaultFont());
	CRect rcTitle(10,5,rcClient.left-BUTTON_AREA_WIDTH-8,m_ImageInfoTable.nTBorder);
	pDC->DrawText(szRoomTitle,lstrlen(szRoomTitle),&rcTitle,DT_END_ELLIPSIS|DT_VCENTER|DT_SINGLELINE);

	return;
}

//����ͼ��
//void CRoomViewItem::DrawLeftViewFrame(CDC * pDC)
//{
//	//��ȡλ��
//	CRect rcClient,rcVorSplitter,rcHorSplitter;
//	GetClientRect(&rcClient);
//	m_VorSplitter.GetWindowRect(&rcVorSplitter);
//	m_HorSplitter.GetWindowRect(&rcHorSplitter);
//	ScreenToClient(&rcVorSplitter);
//	ScreenToClient(&rcHorSplitter);
//
//	//�滭�Ŀ�
//	CRect rcChat;
//	rcChat.left=rcVorSplitter.right;
//	rcChat.right=rcClient.Width();
//	rcChat.top=rcHorSplitter.bottom-rcHorSplitter.Height()/2;
//	rcChat.bottom=rcClient.Height();
//	CSkinAide::DrawEncircleFrame(pDC,rcChat,m_EncircleChat);
//
//	//�滭�б�
//	CRect rcList;
//	rcList.left=rcVorSplitter.right;
//	rcList.right=rcClient.Width();
//	rcList.top=0;
//	rcList.bottom=rcHorSplitter.top+rcHorSplitter.Height()/2;
//	CSkinAide::DrawEncircleFrame(pDC,rcList,m_EncircleList);
//
//	return;
//}

//��������
bool CRoomViewItem::FindGameTable(tagFindTable & FindInfo)
{
	//��������
	WORD wNullCount=0;
	ITableView * pTableView=NULL;
	WORD wChairUser=m_TableFrame.GetChairCount();
	WORD wMeTableID=m_pMeUserItem->GetUserData()->wTableID;

	//��������
	for (WORD i=0;i<m_TableFrame.GetTableCount();i++)
	{
		FindInfo.wResultTableID=(FindInfo.wBeginTableID+i)%m_TableFrame.GetTableCount();
		if (wMeTableID!=FindInfo.wResultTableID)
		{
			//��ȡ����
			pTableView=m_TableFrame.GetTableArrayPtr(FindInfo.wResultTableID);
			ASSERT(pTableView!=NULL);

			//�ж��Ƿ�ʼ��Ϸ
			if (pTableView->QueryPlayFlag()==true) continue;

			//��������
			bool bTablePass=pTableView->QueryPassFlag();
			if ((FindInfo.bFilterPass==true)&&(bTablePass==true)) continue;

			//Ѱ�ҿ�λ��
			wNullCount=pTableView->GetNullChairCount(FindInfo.wResultChairID);

			//Ч�����
			if (CanSitDownTable(FindInfo.wResultTableID,FindInfo.wResultChairID,false,false)==false) continue;

			//�ж���Ŀ
			if (wNullCount>0)
			{
				if ((FindInfo.bNotFull==true)&&(wNullCount<wChairUser))
				{
					return true;
				}
				if ((FindInfo.bOneNull==true)&&(wNullCount==1)) 
				{
					return true;
				}
				if ((FindInfo.bTwoNull==true)&&(wNullCount==2)) 
				{
					return true;
				}
				if ((FindInfo.bAllNull==true)&&(wNullCount==wChairUser)) 
				{
					return true;
				}
			}
		}
	}

	//��������
	FindInfo.wResultTableID=INVALID_TABLE;
	FindInfo.wResultChairID=INVALID_CHAIR;

	return false;
}

//�����ؼ�
void CRoomViewItem::RectifyControl(int nWidth, int nHeight)
{
	//״̬�ж�
	if (m_bInitDialog==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	//��������
	UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;
	const INT nXScreen=GetSystemMetrics(SM_CXSCREEN);

	HDWP hDwp=BeginDeferWindowPos(32);
	//��������
	//DeferWindowPos(hDwp,m_TableFrame,NULL,0,35,659,558,uFlags);

	uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	// 	DeferWindowPos(hDwp,m_btnReLogon,NULL,821,680,0,0,uFlags);
	// 	DeferWindowPos(hDwp,m_btnReturn,NULL,847,680,0,0,uFlags);
	// 	DeferWindowPos(hDwp,m_btnMinsize,NULL,872,680,0,0,uFlags);
	// 	DeferWindowPos(hDwp,m_btnSet,NULL,897,680,0,0,uFlags);
	// 	DeferWindowPos(hDwp,m_btnClose,NULL,921,680,0,0,uFlags);
	// 	DeferWindowPos(hDwp,m_btnFriend,NULL,292,29,0,0,uFlags);
	// 	DeferWindowPos(hDwp,m_btnTool,NULL,423,29,0,0,uFlags);
	// 	DeferWindowPos(hDwp,m_btnTask,NULL,554,29,0,0,uFlags);
	// 	DeferWindowPos(hDwp,m_btnDepository,NULL,684,29,0,0,uFlags);
	// 	DeferWindowPos(hDwp,m_btnShop,NULL,812,29,0,0,uFlags);
	// 	DeferWindowPos(hDwp,m_ChatMessage,NULL,19,511,263,161,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS);
	// 	DeferWindowPos(hDwp,m_editInput,NULL,22,677,169,19,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS);
	// 	DeferWindowPos(hDwp,m_btSendChat,NULL,194,676,0,0,uFlags);
	// 	DeferWindowPos(hDwp,m_btExpression,NULL,218,676,0,0,uFlags);
	// 	DeferWindowPos(hDwp,m_ChatObject,NULL,242,676,38,21,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS);
	DeferWindowPos(hDwp,m_btnPgDn,NULL,500,593,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnPgUp,NULL,300,593,0,0,uFlags);
	//cxf 1108 DeferWindowPos(hDwp,m_AutoQueue,NULL,229,76,0,0,uFlags);
	//DeferWindowPos(hDwp,m_btnDespokenTable,NULL,228,126,0,0,uFlags);
	EndDeferWindowPos(hDwp);

	m_rcQueue.left = nWidth/2-100;
	m_rcQueue.top = 0;
	m_rcQueue.right = m_rcQueue.left + 200;
	m_rcQueue.bottom = m_rcQueue.top + 48;

	/*cxf m_ptOrder.x = 125;
	m_ptOrder.y = 602;
	m_ptWinGemsToday.y = 602;
	m_ptWinGemsToday.x = 333;*/
	//�ػ�����
	Invalidate(FALSE);
	//UpdateWindow();
	return;
}

//��ʾ��Ϣ
int CRoomViewItem::ShowMessageBox(LPCTSTR pszMessage, UINT nType)
{
	//�ر���ʾ
	g_GlobalAttemper.DestroyStatusWnd(this);
	tagGameServer * pGameServer=m_pListServer->GetItemInfo();
	int nResult=ShowInformationEx(pszMessage,0,nType,pGameServer->szServerName);
	return nResult;
}

//�����û�
bool CRoomViewItem::SendTableUser(IUserItem * pIUserItem, CIPCSendCopyData * pSendCopyData)
{
	//Ч�����
	ASSERT(pIUserItem!=NULL);
	ASSERT(pSendCopyData!=NULL);

	//�������
	BYTE cbBuffer[IPC_PACKAGE];
	tagUserData * pUserData=pIUserItem->GetUserData();
	tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)cbBuffer;

	//��������
	memset(cbBuffer,0,sizeof(cbBuffer));
	pUserInfoHead->dwUserID=pUserData->dwUserID;
	pUserInfoHead->dwGameID=pUserData->dwGameID;
	pUserInfoHead->dwGroupID=pUserData->dwGroupID;
	pUserInfoHead->dwUserRight=pUserData->dwUserRight;
	pUserInfoHead->lLoveliness=pUserData->lLoveliness;
	pUserInfoHead->dwMasterRight=pUserData->dwMasterRight;
	pUserInfoHead->wFaceID=pUserData->wFaceID;
	pUserInfoHead->dwCustomFaceVer=pUserData->dwCustomFaceVer;
	pUserInfoHead->wTableID=pUserData->wTableID;
	pUserInfoHead->wChairID=pUserData->wChairID;
	pUserInfoHead->cbGender=pUserData->cbGender;
	pUserInfoHead->cbUserStatus=pUserData->cbUserStatus;
	pUserInfoHead->cbMemberOrder=pUserData->cbMemberOrder;
	pUserInfoHead->cbMasterOrder=pUserData->cbMasterOrder;
	/*pUserInfoHead->UserScoreInfo.lScore=pUserData->lScore;
	pUserInfoHead->UserScoreInfo.lGameGold=pUserData->lGameGold;*/
	pUserInfoHead->UserScoreInfo.lMoney=pUserData->lMoney;
	pUserInfoHead->UserScoreInfo.lWinCount=pUserData->lWinCount;
	pUserInfoHead->UserScoreInfo.lLostCount=pUserData->lLostCount;
	pUserInfoHead->UserScoreInfo.lDrawCount=pUserData->lDrawCount;
	pUserInfoHead->UserScoreInfo.lFleeCount=pUserData->lFleeCount;
	pUserInfoHead->UserScoreInfo.lExperience=pUserData->lExperience;

	pUserInfoHead->UserScoreInfo.dwGrade=pUserData->dwGrade;
	//pUserInfoHead->UserScoreInfo.dwOrder=pUserData->dwOrder;
	pUserInfoHead->UserScoreInfo.lGem=pUserData->lGem;
	pUserInfoHead->UserScoreInfo.lGold=pUserData->lGold;

	//�ȼ���ν
	::memcpy(pUserInfoHead->UserScoreInfo.chGradeName, ((CPlazaDlg*)GetParent())->m_pGradeInfo[pUserData->dwGrade-1].strName, sizeof(pUserInfoHead->UserScoreInfo.chGradeName));

	//�û���Ϸ���� Author<cxf>;DateTime<2012-03-05>
	pUserInfoHead->UserScoreInfo.lKindIDWinCount=pUserData->lKindIDWinCount;
	pUserInfoHead->UserScoreInfo.lKindIDLostCount=pUserData->lKindIDLostCount;
	pUserInfoHead->UserScoreInfo.lKindIDFleeCount=pUserData->lKindIDFleeCount;
	pUserInfoHead->UserScoreInfo.lKindIDDrawCount=pUserData->lKindIDDrawCount;

	//pUserInfoHead->UserScoreInfo.lHonor=pUserData->lHonor;
	//pUserInfoHead->UserScoreInfo.lCredit=pUserData->lCredit;


	//pUserInfoHead->lMoney = pUserData->lMoney;
	for ( WORD wPropID = 0; wPropID < PROPERTY_COUNT; ++wPropID )
	{
		pUserInfoHead->dwPropResidualTime[wPropID] = pUserData->dwPropResidualTime[wPropID];
	}

	//��������
	WORD wHeadSize=sizeof(tagUserInfoHead);
	CSendPacketHelper SendPacketHelper(cbBuffer+wHeadSize,sizeof(cbBuffer)-wHeadSize);
	SendPacketHelper.AddPacket(pUserData->szName,CountStringBuffer(pUserData->szName),DTP_USER_ACCOUNTS);
	SendPacketHelper.AddPacket(&pUserData->cbCompanion,sizeof(pUserData->cbCompanion),DTP_USER_COMPANION);
	SendPacketHelper.AddPacket(pUserData->szUnderWrite,CountStringBuffer(pUserData->szUnderWrite),DTP_UNDER_WRITE);
	SendPacketHelper.AddPacket(pUserData->szGroupName,CountStringBuffer(pUserData->szGroupName),DTP_USER_GROUP_NAME);

	//��������
	WORD wSendSize=wHeadSize+SendPacketHelper.GetDataSize();
	pSendCopyData->SendData(IPC_MAIN_USER,IPC_SUB_USER_COME,cbBuffer,wSendSize);


	const tagGlobalUserData & userdata = g_GlobalUnits.GetGolbalUserData();
	if (userdata.dwUserID == pUserData->dwUserID)
	{
		//�������
		IPC_UserPassWord userPassword;
		CopyMemory(userPassword.chUserPassWord, userdata.szPassWord, sizeof(userPassword.chUserPassWord));
		pSendCopyData->SendData(IPC_MAIN_USER,IPC_SUB_GETUSERPASSWORD,&userPassword,sizeof(userPassword));
	}
	return true;
}

//������Ϣ
bool CRoomViewItem::SendProcessData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//������Ϣ
	CIPCSendCopyData IPCSendCopyData(m_hWndChannel,m_hWnd);
	return IPCSendCopyData.SendData(wMainCmdID,wSubCmdID,pData,wDataSize);
}

//�ܷ�����
bool CRoomViewItem::CanSitDownTable(WORD wTableID, WORD wChairID, bool bLookon, bool bShowError)
{
	//Ч�����
	ASSERT(wTableID!=INVALID_TABLE);
	ASSERT(wChairID!=INVALID_CHAIR);
	if (wTableID>=m_TableFrame.GetTableCount())
	{
		return false;
	}
	if (wChairID>=m_TableFrame.GetChairCount()) 
	{
		return false;
	}
	if (m_ServiceStatus!=ServiceStatus_Serviceing) 
	{
		return false;
	}


	//�ж���Ӧ
	if ((m_wReqTableID!=INVALID_TABLE)||(m_wReqChairID!=INVALID_CHAIR))
	{
		if (bShowError==true)
		{
			LPCTSTR pszDescribe=TEXT("�ϴ�����û���յ���������Ӧ,���Ժ�");
			ShowMessageBox(pszDescribe,MB_ICONINFORMATION);
			return false;
		}
	}

	//�ж����
	ITableView * pITableView=m_TableFrame.GetTableArrayPtr(wTableID);
	IUserItem * pITableUserItem=pITableView->GetUserInfo(wChairID);
	if ((bLookon==true)&&(pITableUserItem==NULL))
	{
		return false;
	}
	if ((bLookon==false)&&(pITableUserItem!=NULL)) 
	{
		return false;
	}


	//�ж�״̬
	BYTE cbUserStatus=m_pMeUserItem->GetUserData()->cbUserStatus;
	if (cbUserStatus==US_PLAY)
	{
		if (bShowError==true)
		{
			LPCTSTR pszDescribe=TEXT("��������Ϸ�У���ʱ���ܻ�λ�ã����Ƚ�����Ϸ��");
			ShowMessageBox(pszDescribe,MB_ICONINFORMATION);
		}
		return false;
	}

	//�ж���Ϸ״̬
	bool bGameStart=pITableView->QueryPlayFlag();
	if ((bGameStart==false)&&(bLookon==true)&&(m_pMeUserItem->GetUserData()->dwMasterRight==0)) return false;

	//�ж�����
	if ((pITableView->QueryPassFlag()==true))
	{
		if (bShowError==true)
		{
			//��������
			CDlgEnterTablePass DlgEnterTablePass;
			ZeroMemory(m_pServerOption->m_szEnterPassword,sizeof(m_pServerOption->m_szEnterPassword));
			DlgEnterTablePass.SetPassword(m_pServerOption->m_szEnterPassword,CountArray(m_pServerOption->m_szEnterPassword));

			//��ʾ����
			DlgEnterTablePass.DoModal();
			if(m_pServerOption->m_szEnterPassword[0]==0)
			{
				return false;
			}
		}
		else 
		{
			return false;
		}

	}

	//�Թ��ж�
	if (bLookon==true)
	{
		return true;
	}


	//�����ж�
	tagUserData * pTableUserData=NULL;
	for (WORD i=0;i<m_TableFrame.GetTableCount();i++)
	{
		//��ȡ�û�
		pITableUserItem=pITableView->GetUserInfo(i);
		if ((pITableUserItem==NULL)||(pITableUserItem==m_pMeUserItem)) continue;
		pTableUserData=pITableUserItem->GetUserData();

		//������
		if (g_GlobalOption.m_bLimitDetest==true)
		{
			if (pTableUserData->cbCompanion==enCompanion_Detest)
			{
				if (bShowError==true)
				{
					TCHAR szDescribe[128]=TEXT("");
					_sntprintf(szDescribe,sizeof(szDescribe),TEXT("�������˲��벻�ܻ�ӭ�������Ϸ��������������ӭ����� [ %s ] ��"),pTableUserData->szName);
					ShowMessageBox(szDescribe,MB_ICONINFORMATION);
				}
				return false;
			}
		}

		//ʤ��Ч��
		if (m_pGameOption->m_bLimitWin)
		{
			WORD wWinRate=0;
			LONG lAllCount=pITableUserItem->GetUserPlayCount();
			if (lAllCount>0) wWinRate=(WORD)(pTableUserData->lWinCount*10000/lAllCount);
			if (wWinRate<m_pGameOption->m_wWinRate)
			{
				if (bShowError==true)
				{
					TCHAR szDescribe[128]=TEXT("");
					_sntprintf(szDescribe,sizeof(szDescribe),TEXT("[ %s ] ��ʤ��̫���ˣ����������ò�����"),pTableUserData->szName);
					ShowMessageBox(szDescribe,MB_ICONINFORMATION);
				}
				return false;
			}
		}

		//����Ч��
		if (m_pGameOption->m_bLimitFlee)
		{
			WORD wFillRate=0;
			LONG lAllCount=pITableUserItem->GetUserPlayCount();
			if (lAllCount>0) wFillRate=(WORD)(pTableUserData->lFleeCount*10000/lAllCount);
			if ((wFillRate>0)&&(wFillRate>m_pGameOption->m_wFleeRate))
			{
				if (bShowError==true)
				{
					TCHAR szDescribe[128]=TEXT("");
					_sntprintf(szDescribe,sizeof(szDescribe),TEXT("[ %s ] ��������̫���ˣ����������ò�����"),pTableUserData->szName);
					ShowMessageBox(szDescribe,MB_ICONINFORMATION);
				}
				return false;
			}
		}

		//����Ч��
		if (m_pGameOption->m_bLimitScore)
		{
			LONGLONG lTableUserScore=pTableUserData->lScore;
			if (lTableUserScore>m_pGameOption->m_lMaxScore)
			{
				if (bShowError==true)
				{
					TCHAR szDescribe[128]=TEXT("");
					_sntprintf(szDescribe,sizeof(szDescribe),TEXT("[ %s ] �Ļ���̫���ˣ����������ò�����"),pTableUserData->szName);
					ShowMessageBox(szDescribe,MB_ICONINFORMATION);
				}
				return false;
			}
			if (lTableUserScore<m_pGameOption->m_lLessScore)
			{
				if (bShowError==true)
				{
					TCHAR szDescribe[128]=TEXT("");
					_sntprintf(szDescribe,sizeof(szDescribe),TEXT("[ %s ] �Ļ���̫���ˣ����������ò�����"),pTableUserData->szName);
					ShowMessageBox(szDescribe,MB_ICONINFORMATION);
				}
				return false;
			}
		}
	}

	return true;
}

//����������Ϣ
bool __cdecl CRoomViewItem::OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode)
{
	//������
	if (nErrorCode!=0)
	{
		ShowMessageBox(TEXT("��Ϸ��������ʧ�ܣ�����ʱ���ܽ������Ϸ���䣡"),MB_ICONINFORMATION);
		g_GlobalAttemper.DestroyStatusWnd(this);
		//�رշ���
		//cxf 1108 GetParent()->PostMessage(WM_COMMAND,IDM_CALL_EXIT_GAME,0);
		CloseRoomViewItem();

		//		((CGameFrame*)AfxGetMainWnd())->CloseRoomViewItem(this);
		return true;
	}

	//���͵�¼��Ϣ
	SendLogonPacket();
	m_ServiceStatus=ServiceStatus_EfficacyUser;

	return true;
}

//�����ȡ��Ϣ
bool __cdecl CRoomViewItem::OnEventTCPSocketRead(WORD wSocketID, CMD_Command Command, VOID * pData, WORD wDataSize)
{
	if (!m_bIsDestroyStatus)
	{//���ٵȴ���
		g_GlobalAttemper.DestroyStatusWnd(this);
		m_bIsDestroyStatus =true;
	}

	bool bRet = false;
	switch (Command.wMainCmdID)
	{
	case MDM_GR_LOGON:			//��¼��Ϣ
		{
			bRet = OnSocketMainLogon(Command,pData,wDataSize);
			break;
		}
	case MDM_GR_USER:			//�û���Ϣ
		{
			bRet = OnSocketMainUser(Command,pData,wDataSize);
			break;
		}
	case MDM_GR_INFO:			//������Ϣ
		{
			bRet = OnSocketMainInfo(Command,pData,wDataSize);
			break;
		}
	case MDM_GR_STATUS:			//״̬��Ϣ
		{
			bRet = OnSocketMainStatus(Command,pData,wDataSize);
			break;
		}
	case MDM_GR_SYSTEM:			//ϵͳ��Ϣ
		{
			bRet = OnSocketMainSystem(Command,pData,wDataSize);
			break;
		}
	case MDM_GR_SERVER_INFO:	//������Ϣ
		{
			bRet = OnSocketMainServerInfo(Command,pData,wDataSize);
			break;
		}
	case MDM_GF_GAME:			//��Ϸ��Ϣ
	case MDM_GF_FRAME:			//�����Ϣ
	case MDM_GF_VIDEO:			//��Ƶ��Ϣ
	case MDM_GF_TASK:			//������Ϣ
	case MDM_GF_PRESENT:		//������Ϣ
		{
			bRet = OnSocketMainGameFrame(Command,pData,wDataSize);
			break;
		}
		// 	case MDM_GF_TASK:			//������Ϣ
		// 		{ 
		// 			return OnSocketTask(Command,pData,wDataSize);
		// 		}
	}
	if (!bRet)
	{
#ifdef ADMIN_PLAZA
#ifdef _DEBUG
		char chData[100]={0};
		sprintf(chData, "OnEventTCPSocketRead:%d", Command.wMainCmdID);
		AfxMessageBox(chData);
#endif
#endif
	}
	return true;
}

//����ر���Ϣ
bool __cdecl CRoomViewItem::OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason)
{
	//�жϹر�
	bool bCloseRoomView=(m_ServiceStatus!=ServiceStatus_Serviceing && cbShutReason != SHUT_REASON_NORMAL);

	//�رմ���
	g_GlobalAttemper.DestroyStatusWnd(this);
	m_ServiceStatus=ServiceStatus_NetShutDown;
	if (cbShutReason!=SHUT_REASON_NORMAL)
	{
		if ((cbShutReason==SHUT_REASON_REMOTE)||(cbShutReason==SHUT_REASON_TIME_OUT))
		{
			//m_MessageProxyHelper->InsertSystemString("�����������⣬���Ѿ���������Ͽ����ӣ�����������",0);
			ShowMessageBox("�����������⣬���Ѿ���������Ͽ����ӣ�����������",MB_ICONINFORMATION);
		}
		else
		{
			ShowMessageBox("�����������ݰ�����ʧ�ܣ������ж���",MB_ICONINFORMATION);
		}
		//�رշ���
		//GetParent()->SendMessage(WM_COMMAND,IDM_CALL_GAME_LIST,0);

		//SendProcessData(IPC_MAIN_CONCTROL,IPC_SUB_CLOSE_FRAME,NULL,0);
		//CloseRoomViewItem();
		SendRequestCloseRoomMsg(1);
	}
	else
	{
		//SendProcessData(IPC_MAIN_CONCTROL,IPC_SUB_CLOSE_FRAME,NULL,0);
		CloseGameClient();
	}

	//�رշ���
	//	if (bCloseRoomView==true) ((CGameFrame*)AfxGetMainWnd())->CloseRoomViewItem(this);

	return true;
}

//��¼��Ϣ
bool CRoomViewItem::OnSocketMainLogon(CMD_Command Command, void * pData, WORD wDataSize)
{
	ASSERT(Command.wMainCmdID==MDM_GR_LOGON);
	switch (Command.wSubCmdID)
	{
	case SUB_GR_LOGON_SUCCESS:		//��¼�ɹ�
		{
			//���ñ���
			m_ServiceStatus=ServiceStatus_RecvConfigInfo;

			return true;
		}
	case SUB_GR_LOGON_ERROR:		//��¼ʧ��
		{
			//Ч�����
			CMD_GR_LogonError * pLogonError=(CMD_GR_LogonError *)pData;
			ASSERT(wDataSize>=(sizeof(CMD_GR_LogonError)-sizeof(pLogonError->szErrorDescribe)));
			if (wDataSize<(sizeof(CMD_GR_LogonError)-sizeof(pLogonError->szErrorDescribe))) return false;

			//�ر�����
			//cxf 11-18 g_GlobalAttemper.DestroyStatusWnd(this);
			m_ClientSocket->CloseSocket();
			m_ServiceStatus=ServiceStatus_NetShutDown;
			//cxf 1108 m_AutoQueue.EnableWindow(FALSE);
			//�رշ���
			GetParent()->ShowWindow(SW_SHOW);
			GetParent()->PostMessage(WM_COMMAND,IDM_CALL_ROOM_LIST,0);
			CloseRoomViewItem();

			//��ʾ��Ϣ
			WORD wDescribeSize=wDataSize-(sizeof(CMD_GR_LogonError)-sizeof(pLogonError->szErrorDescribe));
			if (wDescribeSize>0)
			{
				pLogonError->szErrorDescribe[wDescribeSize-1]=0;

				tagGameServer * pGameServer=m_pListServer->GetItemInfo();
				int nResult=ShowInformationEx(pLogonError->szErrorDescribe,0,MB_ICONINFORMATION,pGameServer->szServerName);
			}

			return true;
		} 
	case SUB_GR_LOGON_FINISH:		//��¼���
		{
			//���ý���
			if (m_TableFrame.GetChairCount()<=MAX_CHAIR_NORMAL)
			{
				m_TableFrame.ShowUserInfo(true);
			}

			//cxf 11-18 g_GlobalAttemper.DestroyStatusWnd(this);
			//((CTableListDlg*)AfxGetMainWnd())->ActiveRoomViewItem(this);

			//���ñ���
			m_ServiceStatus=ServiceStatus_Serviceing;
			RectifyControl(960,720);

			//���͹���
			SendUserRulePacket();

			//cxf 1108 m_AutoQueue.EnableWindow(TRUE);

			//�����ж�
			ASSERT(m_pMeUserItem!=NULL);
			tagUserData * pUserData=m_pMeUserItem->GetUserData();
			//Author<cxf>;DateTime<2012-03-01>;Description<����>
			//m_bMasterRight=(pUserData->dwMasterRight>0)?true:false;//Author<cxf>;DateTime<2012-02-27>


			if (pUserData->wTableID!=INVALID_TABLE)
			{
				int iResult=StartGameClient();
			}

			if (!IsAdministrationHavePurview())
			{
				if (INVALID_TABLE == pUserData->wTableID)
				{//�Զ��Ŷ�...
					m_bQueueState=true;
					OnAutoQueue();
				}

				//��������
				SetTimer(IDI_LOADING_ANIMACTION,100,NULL);
			}

			return true;
		}
	case SUB_GR_LOGON_FULL:
		{
			//Ч�����
			ASSERT(Command.wMainCmdID==MDM_GR_LOGON);
			ASSERT(Command.wSubCmdID==SUB_GR_LOGON_FULL);
			//cxf 11-18 g_GlobalAttemper.DestroyStatusWnd(this);
			m_bLogonFull=true;
			return true;
		}
	}

	return true;
}

//�û���Ϣ
bool CRoomViewItem::OnSocketMainUser(CMD_Command Command, void * pData, WORD wDataSize)
{
	ASSERT(Command.wMainCmdID==MDM_GR_USER);
	switch (Command.wSubCmdID)
	{
	case SUB_GR_USER_COME:			//�û�����
		{
			return OnSocketSubUserCome(Command,pData,wDataSize);
		}
	case SUB_GR_USER_STATUS:		//�û�״̬
		{
			return OnSocketSubStatus(Command,pData,wDataSize);
		}
	case SUB_GR_USER_SCORE:			//�û�����
		{
			return OnSocketSubScore(Command,pData,wDataSize);
		}
	case SUB_GR_USER_RIGHT:			//�û�Ȩ��
		{
			return OnSocketSubRight(Command,pData,wDataSize);
		}
	case SUB_GR_MEMBER_ORDER:		//��Ա�ȼ�
		{
			return OnSocketSubMemberOrder(Command,pData,wDataSize);
		}
	case SUB_GR_SIT_FAILED:			//����ʧ��
		{
			return OnSocketSubSitFailed(Command,pData,wDataSize);
		}
	case SUB_GR_USER_CHAT:			//�û�����
		{
			return OnSocketSubChat(Command,pData,wDataSize);
		}
	case SUB_GR_USER_WISPER:		//�û�˽��
		{
			return OnSocketSubWisper(Command,pData,wDataSize);
		}
	case SUB_GR_USER_INVITE:		//�������
		{
			return OnSocketSubUserInvite(Command,pData,wDataSize);
		}
		//�Ŷӳɹ�      �������   
	case SUB_GR_AUTO_QUEUE_SUCC:
	case SUB_GR_AUTO_QUEUE:			//�Զ��Ŷ�
		{
			//Ч�����
			ASSERT(Command.wMainCmdID==MDM_GR_USER);
			//ASSERT(Command.wSubCmdID==SUB_GR_AUTO_QUEUE);

			//��Ϣ����
			CMD_GR_AutoQueue * pAutoQueue=(CMD_GR_AutoQueue *)pData;
			m_QueueNum=pAutoQueue->iAutoQueueNum; //�Ŷ�����
			//AfxMessageBox(str);

			//���½���
			InvalidateRect(m_rcQueue, FALSE);

			m_bQueueState = false;
			HINSTANCE hInstance=AfxGetInstanceHandle();
			//cxf 1108 m_AutoQueue.LoadRgnImage(hInstance,IDB_CANCEL_AUTO_QUEUE);
			//cxf 1108 m_ToolTipCtrl.AddTool(&m_AutoQueue,TEXT("ȡ���Ŷ�"));
			//cxf 1108 m_AutoQueue.EnableWindow(TRUE);
			//cxf 1108 m_AutoQueue.Invalidate(TRUE);
			//UpdateWindow();
			return true;
		}
	case SUB_GR_QUEUE_COUNT:	//�Ŷ�����
		{
			return OnSocketSubQueueCount(Command,pData,wDataSize);
		}
	case SUB_GR_AUTO_QUEUE_FAIL:	//�Ŷ�ʧ��
		{
			return OnSocketSubQueueFail(Command,pData,wDataSize);
		}
	case SUB_RECVACTIONINFO:		//���ܶ���
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(IPC_UserActionInfo));
			if (wDataSize<sizeof(IPC_UserActionInfo)) return false;

			if (NULL != pData)
			{
				SendProcessData(IPC_MAIN_USER, IPC_SUB_RECVACTIONINFO, pData, wDataSize);
			}
			return true;
		}
	case SUB_RECV_SOUND_MESS:		//��������
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(IPC_UserLanguageInfo));
			if (wDataSize<sizeof(IPC_UserLanguageInfo)) return false;

			if (NULL != pData)
			{
				SendProcessData(IPC_MAIN_USER, IPC_SUB_RECVGLINFO, pData, wDataSize);
			}
			return true;
		}
	case SUB_GR_NO_GOLD:		//��Ҳ��㣬�˻ط����б�
		{
			//ASSERT(wDataSize>=sizeof(CMD_GR_SendWarning));
			//if (wDataSize<sizeof(CMD_GR_SendWarning))
			//{
			//	return false;
			//}
			CMD_GR_SendWarning * warning=(CMD_GR_SendWarning *)pData;
			if (NULL != warning)
			{
				//�˳��Զ��Ŷ�
				m_bQueueState=false;
				OnAutoQueue();
				//cxf 1101ShowMessageBox(warning->szWarningMessage,MB_ICONINFORMATION);
				m_ConfirmGold.DoModal();
				//�˳��Ŷ�
				SendRequestCloseRoomMsg(1);
			}

			//������Ϸ����
			//GetParent()->ShowWindow(SW_SHOW);
			//GetParent()->SendMessage(WM_COMMAND,IDM_CALL_GAME_LIST,0);
			//CloseRoomViewItem();
			return true;
		}
	case SUB_GR_LOW_GRADE:		//�ȼ�̫�ߣ��˻ط����б�
		{
			CMD_GR_SendWarning * warning=(CMD_GR_SendWarning *)pData;
			if (NULL != warning)
			{
				//�˳��Զ��Ŷ�
				m_bQueueState=false;
				OnAutoQueue();
				//cxf 1101 ShowMessageBox(warning->szWarningMessage,MB_ICONINFORMATION);
				m_ConfirmGrade.DoModal();
				//�˳��Ŷ�
				SendRequestCloseRoomMsg(1);
			}

			return true;
		}
	case SUB_GR_AUTO_QUEUE_CANCEL_FALL:		//ȡ���Ŷ�ʧ��
		{
			return true;
		}
	case SUB_GR_AUTO_QUEUE_CANCEL_SUCC:		//ȡ���Ŷӳɹ�
		{
			m_bQueueState = true;
			HINSTANCE hInstance=AfxGetInstanceHandle();
			//cxf 1108 m_AutoQueue.LoadRgnImage(hInstance,IDB_AUTO_QUEUE);
			//cxf 1108 m_ToolTipCtrl.AddTool(&m_AutoQueue,TEXT("�Զ��Ŷ�"));
			//cxf 1108 m_AutoQueue.EnableWindow(TRUE);
			//cxf 1108 m_AutoQueue.Invalidate(TRUE);
			return true;
		}
	case SUB_GF_QUIT_GAME_SERVER_SUCC:		//���ص������б�
		{
			m_bRequestServerExit = false;
			KillTimer(IDI_GAME_FRAME_IS_EXIST);
			KillTimer(IDI_REQUEST_SERVER_EXIT_MSG);
			if (0 == m_btExitTag)
			{//ֱ�ӷ��ص���¼����
				//�رշ���
				GetParent()->PostMessage(WM_COMMAND,IDM_CALL_EXIT_GAME,0);
				CloseRoomViewItem();
			}
			else if (1 == m_btExitTag)
			{//�������б��ص������б�

				GetParent()->ShowWindow(SW_SHOW);
				GetParent()->PostMessage(WM_COMMAND,IDM_CALL_ROOM_LIST,0);
				CloseRoomViewItem();
			}
			else if (2 == m_btExitTag)
			{//���ص���Ϸ����

				GetParent()->ShowWindow(SW_SHOW);
				GetParent()->PostMessage(WM_COMMAND,IDM_CALL_GAME_LIST,0);
				CloseRoomViewItem();
			}
			else if (3 == m_btExitTag)
			{//�˳�������Ϸ
				GetParent()->PostMessage(WM_COMMAND,IDM_CALL_CANCEL_GAME,0);
			}
			return true;
		}
	case SUB_GR_GO_HALL:	//���ش���
		{
			KillTimer(IDI_GAME_FRAME_IS_EXIST);
			KillTimer(IDI_REQUEST_SERVER_EXIT_MSG);
			GetParent()->ShowWindow(SW_SHOW);
			GetParent()->PostMessage(WM_COMMAND,IDM_CALL_GAME_LIST,0);
			CloseRoomViewItem();
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
bool CRoomViewItem::OnSocketMainInfo(CMD_Command Command, void * pData, WORD wDataSize)
{
	ASSERT(Command.wMainCmdID==MDM_GR_INFO);
	switch (Command.wSubCmdID)
	{
	case SUB_GR_SERVER_INFO:	//������Ϣ
		{
			//Ч������
			ASSERT(wDataSize>=sizeof(CMD_GR_ServerInfo));
			if (wDataSize<sizeof(CMD_GR_ServerInfo))
			{
				return false;
			}


			//��Ϣ����
			CMD_GR_ServerInfo * pServerInfo=(CMD_GR_ServerInfo *)pData;

			//���ñ���
			m_wGameGenre=pServerInfo->wGameGenre;//��Ϸ����
			m_dwVideoAddr=pServerInfo->dwVideoAddr;
			m_cbHideUserInfo=pServerInfo->cbHideUserInfo;

			//��ȡ��Ϣ
			CListKind * pListKind=m_pListServer->GetListKind();
			tagGameKind * pGameKind=pListKind->GetItemInfo();
			IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IUnknownEx);
			// 			m_UserListView.m_wKindID=pServerInfo->wKindID;
			// 			m_UserListView.m_wGameGenre=pServerInfo->wGameGenre;

			//��������
			try
			{
				m_TableFrame.CreateTableFrame(this,100);
				m_TableFrame.InitTableFrame(pServerInfo->wTableCount,pServerInfo->wChairCount,(pServerInfo->cbHideUserInfo==TRUE),pGameKind,pIUnknownEx);
				CRect rect;
				GetClientRect(rect);
				m_TableFrame.OnSize(0, rect.Width(), rect.Height());
			}
			catch (...)
			{
				//�ر�����
				m_ClientSocket->CloseSocket();
				//cxf 11-18 g_GlobalAttemper.DestroyStatusWnd(this);

				//��ʾ��Ϣ
				int nResult=ShowMessageBox(TEXT("��Ϸ������Դ����ʧ�ܣ��������ذ�װ���ܻ������⣬�Ƿ��������أ�"),MB_ICONQUESTION|MB_YESNO);
				if (nResult==IDYES) 
				{
					CListKind * pListKind=m_pListServer->GetListKind();
					tagGameKind * pGameKind=pListKind->GetItemInfo();
					TCHAR szProcessName[MODULE_LEN];
					CopyMemory(szProcessName, pGameKind->szProcessName, sizeof(szProcessName));
					TCHAR * pCh = strrchr(szProcessName, '.');
					if (pCh) *pCh = '\0';
					g_GlobalAttemper.DownLoadClient(szProcessName,pGameKind->wKindID,true,1);
					//g_GlobalAttemper.DownLoadClient(pGameKind->szKindName,pGameKind->wKindID,true);

					GetParent()->ShowWindow(SW_SHOW);
					GetParent()->PostMessage(WM_COMMAND,IDM_CALL_GAME_LIST,0);
					CloseRoomViewItem();
				}
				else
				{
					OnBnReturn();
				}

				//�رշ���
				IRoomViewItem * pIRoomViewItem=QUERY_ME_INTERFACE(IRoomViewItem);
				//((CGameFrame*)AfxGetMainWnd())->CloseRoomViewItem(pIRoomViewItem);

				return false;
			}

			return true;
		}
	case SUB_GR_COLUMN_INFO:	//�б����
		{
			//��������
			CMD_GR_ColumnInfo * pColumnInfo=(CMD_GR_ColumnInfo *)pData;
			WORD wHeadSize=sizeof(CMD_GR_ColumnInfo)-sizeof(pColumnInfo->ColumnItem);

			//Ч�����
			ASSERT(wDataSize>=wHeadSize);
			ASSERT((wHeadSize+pColumnInfo->wColumnCount*sizeof(pColumnInfo->ColumnItem[0]))==wDataSize);
			if (wDataSize<wHeadSize) 
			{
				return false;
			}

			if ((wHeadSize+pColumnInfo->wColumnCount*sizeof(pColumnInfo->ColumnItem[0]))!=wDataSize) 
			{
				return false;
			}


			//�����б�
			CopyMemory(&m_ListColumnInfo,pColumnInfo,__min(wDataSize,sizeof(m_ListColumnInfo)));
			//m_UserListView.SetColumnDescribe(pColumnInfo->ColumnItem,pColumnInfo->wColumnCount);

			return true;
		}
	case SUB_GR_CONFIG_FINISH:	//�������
		{
			//��ԴĿ¼
			TCHAR szResDirectory[128]=TEXT("");
			WORD wStringIndex=0,wBufferCount=CountArray(szResDirectory);

			//����Ŀ¼
			const tagGameKind * pGameKind = GetKindInfo();
			ZeroMemory(szResDirectory,wBufferCount*sizeof(TCHAR));
			lstrcpyn(szResDirectory,(LPCTSTR)pGameKind->szProcessName,wBufferCount);
			while ((szResDirectory[wStringIndex]!=0)&&(szResDirectory[wStringIndex]!=TEXT('.'))) wStringIndex++;

			//�ַ���ֹ
			szResDirectory[wStringIndex]=0;

			//��Ϸ�ȼ�
			// 			ASSERT(m_UserListView.m_GameRankManager.GetInterface()!=NULL);
			// 			/*bool bSuccess=*/m_UserListView.m_GameRankManager->LoadGameLevel(szResDirectory,m_wGameGenre);

			//��ʾ����
			ShowWindow(SW_SHOW);
			//((CTableListDlg*)AfxGetMainWnd())->ActiveRoomViewItem(this);

			//���ñ���
			m_ServiceStatus=ServiceStatus_RecvRoomInfo;

			return true;
		}
	}

	return true;
}

//״̬��Ϣ
bool CRoomViewItem::OnSocketMainStatus(CMD_Command Command, void * pData, WORD wDataSize)
{
	ASSERT(Command.wMainCmdID==MDM_GR_STATUS);
	switch (Command.wSubCmdID)
	{
	case SUB_GR_TABLE_INFO:		//������Ϣ
		{
			//��������
			CMD_GR_TableInfo * pTableInfo=(CMD_GR_TableInfo *)pData;
			const WORD wHeadSize=sizeof(CMD_GR_TableInfo)-sizeof(pTableInfo->TableStatus);

			//Ч������
			ASSERT(wDataSize>=wHeadSize);
			ASSERT((wHeadSize+pTableInfo->wTableCount*sizeof(pTableInfo->TableStatus[0]))==wDataSize);
			if (wDataSize<wHeadSize)
			{
				return false;
			}

			if ((wHeadSize+pTableInfo->wTableCount*sizeof(pTableInfo->TableStatus[0]))!=wDataSize) 
			{
				return false;
			}


			//��Ϣ����
			for (WORD i=0;i<pTableInfo->wTableCount;i++)
			{
				m_TableFrame.SetPassFlag(i,pTableInfo->TableStatus[i].bTableLock?true:false);
				m_TableFrame.SetPlayFlag(i,pTableInfo->TableStatus[i].bPlayStatus?true:false);
			}

			return true;
		}
	case SUB_GR_TABLE_STATUS:	//����״̬
		{
			//Ч������
			ASSERT(wDataSize>=sizeof(CMD_GR_TableStatus));
			if (wDataSize<sizeof(CMD_GR_TableStatus))
			{
				return false;
			}


			//��Ϣ����
			CMD_GR_TableStatus * pTableStatus=(CMD_GR_TableStatus *)pData;
			ASSERT(pTableStatus->wTableID<m_TableFrame.GetTableCount());
			if (pTableStatus->wTableID<m_TableFrame.GetTableCount())
			{
				//��������
				IUserItem * pIUserItem=NULL;
				tagUserData * pUserData=NULL;
				BYTE bPlayStatus=m_TableFrame.QueryPlayFlag(pTableStatus->wTableID);

				//�����û�
				if (bPlayStatus!=pTableStatus->bPlayStatus)
				{
					BYTE cbUserStatus=pTableStatus->bPlayStatus?US_PLAY:US_SIT;
					for (WORD i=0;i<m_TableFrame.GetChairCount();i++)
					{
						pIUserItem=m_TableFrame.GetUserInfo(pTableStatus->wTableID,i);
						if (pIUserItem!=NULL)
						{
							pUserData=pIUserItem->GetUserData();
							if (pUserData->cbUserStatus!=US_OFFLINE) 
							{
								pUserData->cbUserStatus=cbUserStatus;
								OnUserItemUpdate(pIUserItem);
							}
						}
					}
				}

				//��������
				m_TableFrame.SetPassFlag(pTableStatus->wTableID,pTableStatus->bTableLock?true:false);
				m_TableFrame.SetPlayFlag(pTableStatus->wTableID,pTableStatus->bPlayStatus?true:false);

				//֪ͨ��Ϸ
				if (bPlayStatus!=pTableStatus->bPlayStatus)
				{
					tagUserData * pMeUserData=m_pMeUserItem->GetUserData();
					if (pMeUserData->wTableID==pTableStatus->wTableID)
					{
						WORD wSubCmdID=pTableStatus->bPlayStatus?IPC_SUB_GAME_START:IPC_SUB_GAME_FINISH;
						SendProcessData(IPC_MAIN_USER,wSubCmdID,NULL,0);
					}
				}
			}

			return true;
		}
	}

	return true;
}

//ϵͳ��Ϣ
bool CRoomViewItem::OnSocketMainSystem(CMD_Command Command, void * pData, WORD wDataSize)
{
	ASSERT(Command.wMainCmdID==MDM_GR_SYSTEM);
	switch (Command.wSubCmdID)
	{
	case SUB_GR_MESSAGE:		//ϵͳ��Ϣ
		{
			//Ч�����
			CMD_GR_Message * pMessage=(CMD_GR_Message *)pData;
			ASSERT(wDataSize>(sizeof(CMD_GR_Message)-sizeof(pMessage->szContent)));
			if (wDataSize<=(sizeof(CMD_GR_Message)-sizeof(pMessage->szContent)))
			{
				return false;
			}

			//��Ϣ����
			WORD wHeadSize=sizeof(CMD_GR_Message)-sizeof(pMessage->szContent);
			ASSERT(wDataSize==(wHeadSize+pMessage->wMessageLength*sizeof(TCHAR)));
			if (wDataSize!=(wHeadSize+pMessage->wMessageLength*sizeof(TCHAR))) 
			{
				return false;
			}

			pMessage->szContent[pMessage->wMessageLength-1]=0;
			if (pMessage->wMessageType&SMT_INFO) m_MessageProxyHelper->InsertSystemString(pMessage->szContent,MS_NORMAL);
			if (pMessage->wMessageType&SMT_EJECT) GetParent()->PostMessage(IDM_CALL_SHOW_CUSTOMER, (WPARAM)pData, (LPARAM)wDataSize);

			//�ر�����
			if (pMessage->wMessageType&SMT_INTERMIT_LINE) 
			{
				m_ClientSocket->CloseSocket();
				CloseGameClient();
				SendRequestCloseRoomMsg(1);				
			}
			else if (SMT_CLOSE_ROOM&pMessage->wMessageType)
			{
				CloseGameClient();
				SendRequestCloseRoomMsg(1);
			}

			return true;
		}
	case SUB_GR_LOGON_AFFICHE:			//ϵͳ����
		{
			char* chSysData = new char[wDataSize+1];
			if (NULL != chSysData)
			{
				::memcpy(chSysData, pData, wDataSize);
				chSysData[wDataSize] = '\0';

				/*cxf 11-26 if ((NULL!=m_RoomViewAffiche) && (::IsWindow(m_RoomViewAffiche->GetSafeHwnd())))
				{
				m_RoomViewAffiche->SetGameAfficheInfo(chSysData);
				}*/
				m_strSaveGameAffiche.clear();
				m_strSaveGameAffiche.append(chSysData);
				delete chSysData;

				//������Ϸ������Ϣ
				SendToGameFrameAfficheInfo();
			}
			chSysData = NULL;

			return true;
		}
	}

	return true;
}

//������Ϣ
bool CRoomViewItem::OnSocketMainServerInfo(CMD_Command Command, void * pData, WORD wDataSize)
{
	ASSERT(Command.wMainCmdID==MDM_GR_SERVER_INFO);
	switch (Command.wSubCmdID)
	{
	case SUB_GR_ONLINE_COUNT_INFO:		//������Ϣ
		{
			//Ч������
			ASSERT(wDataSize%sizeof(tagOnLineCountInfo)==0);
			if (wDataSize%sizeof(tagOnLineCountInfo)!=0) 
			{
				return false;
			}

			//��Ϣ����
			WORD wKindID=0;
			DWORD dwKindOnLineCount=0,dwAllOnLineCount=0L;
			WORD wInfoCount=wDataSize/sizeof(tagOnLineCountInfo);
			tagOnLineCountInfo * pOnLineCountInfo=(tagOnLineCountInfo *)pData;
			for (int i=0;i<wInfoCount;i++)
			{
				wKindID=(pOnLineCountInfo+i)->wKindID;
				dwKindOnLineCount=(pOnLineCountInfo+i)->dwOnLineCount;
				dwAllOnLineCount+=dwKindOnLineCount;
				g_GlobalUnits.m_ServerListManager.UpdateGameKindOnLine(wKindID,dwKindOnLineCount);
			}

			//��������
			g_GlobalUnits.m_ServerListManager.UpdateGameOnLineCount(dwAllOnLineCount);

			return true;
		}
	}

	return true;
}

//��Ϸ��Ϣ
bool CRoomViewItem::OnSocketMainGameFrame(CMD_Command Command, void * pData, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize<=SOCKET_PACKET);
	if (wDataSize>SOCKET_PACKET)
	{
		return false;
	}


	//��������
	IPC_SocketPackage SocketPackage;
	memset(&SocketPackage,0,sizeof(SocketPackage));
	SocketPackage.Command=Command;
	if (wDataSize>0)
	{
		ASSERT(pData!=NULL);
		CopyMemory(SocketPackage.cbBuffer,pData,wDataSize);
	}

	//���ݻ���
	if (m_hWndChannel==NULL)
	{
		//״̬�ж�
		ASSERT(m_pMeUserItem!=NULL);
		if (m_pMeUserItem==NULL)
		{
			return false;
		}


		//��ȡ�û�
		tagUserData * pUserData=m_pMeUserItem->GetUserData();

		//���ݻ���
		if ((pUserData->wTableID!=INVALID_TABLE)&&(pUserData->wChairID!=INVALID_CHAIR))
		{
			//���ñ���
			m_wPacketTableID=pUserData->wTableID;
			m_wPacketChairID=pUserData->wChairID;

			//��������
			BYTE cbBuffer[SOCKET_BUFFER];
			CopyMemory(cbBuffer,&Command,sizeof(Command));
			CopyMemory(&cbBuffer[sizeof(Command)],pData,wDataSize);

			//��������
			m_PacketDataStorage.InsertData(0,cbBuffer,sizeof(Command)+wDataSize);
		}
	}
	else
	{
		//��������
		WORD wSendSize=sizeof(SocketPackage.Command)+wDataSize;
		SendProcessData(IPC_MAIN_SOCKET,IPC_SUB_SOCKET_RECV,&SocketPackage,wSendSize);
	}

	/*

	//������Ŀ
	if ( Command.wSubCmdID == SUB_GF_RESIDUAL_PROPERTY )
	{
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GF_ResidualProperty));
	if (wDataSize!=sizeof(CMD_GF_ResidualProperty)) return false;

	//��������
	CMD_GF_ResidualProperty * ResidualProperty=(CMD_GF_ResidualProperty *)pData;

	//��ȡ���
	tagUserData *pMeUserData = m_pMeUserItem->GetUserData();

	//���õ���
	for ( WORD wPropID = 0; wPropID < PROPERTY_COUNT; ++wPropID )
	{
	pMeUserData->dwPropResidualTime[wPropID] = ResidualProperty->dwResidual[wPropID].dwResidualTime;
	}
	}
	//������Ϣ
	else if ( Command.wSubCmdID == SUB_GF_PROP_ATTRIBUTE )
	{
	//У�����
	ASSERT( wDataSize % sizeof(tagPropertyInfo) == 0 );
	if ( wDataSize % sizeof(tagPropertyInfo) != 0 ) return true;

	//����ת��
	int nInfoCount = wDataSize / sizeof(tagPropertyInfo);
	tagPropertyInfo *pPropertyInfo = (tagPropertyInfo*)pData;

	//��������
	for ( INT_PTR nIndex = 0; nIndex < nInfoCount; ++nIndex )
	m_PropertyAttributeArrary.Add(pPropertyInfo[nIndex]);

	/ *
	//��ȡ����
	CPropertyBar *pPropertyBar = CPropertyBar::GetInstance();
	ASSERT( pPropertyBar != NULL );
	if ( pPropertyBar == NULL ) return false;

	//��ʼ�ṹ
	for ( int nIndex = 0; nIndex < nInfoCount; ++nIndex )
	{
	pPropertyBar->SetPropertyInfo((pPropertyInfo+nIndex)->nPropertyID, *(pPropertyInfo+nIndex));
	}* /

	}
	//�ʻ�����
	else if ( Command.wSubCmdID == SUB_GF_FLOWER_ATTRIBUTE )
	{
	//У�����
	ASSERT( wDataSize % sizeof(tagFlowerInfo) == 0 );
	if ( wDataSize % sizeof(tagFlowerInfo) != 0 ) return true;

	//����ת��
	int nInfoCount = wDataSize / sizeof(tagFlowerInfo);
	tagFlowerInfo *pFlowerInfo= (tagFlowerInfo*)pData;

	//��������
	for ( INT_PTR nIndex = 0; nIndex < nInfoCount; ++nIndex )
	m_FlowerAttributeArrary.Add(pFlowerInfo[nIndex]);

	/ *
	//��ȡ����
	CPropertyBar *pPropertyBar = CPropertyBar::GetInstance();
	ASSERT( pPropertyBar != NULL );
	if ( pPropertyBar == NULL ) return false;

	//��ʼ�ṹ
	for ( int nIndex = 0; nIndex < nInfoCount; ++nIndex )
	{
	pPropertyBar->SetFlowerInfo((pFlowerInfo+nIndex)->nFlowerID, *(pFlowerInfo+nIndex));
	}* /


	}
	//������Ϣ
	else if ( Command.wSubCmdID == SUB_GF_PROP_BUGLE )
	{
	ASSERT( sizeof(CMD_GF_BugleProperty) == wDataSize );
	if ( sizeof(CMD_GF_BugleProperty) != wDataSize ) return false;

	//����ת��
	CMD_GF_BugleProperty *pBugleProperty = (CMD_GF_BugleProperty*)pData;

	//����ͼƬ
	CBitmap Bitmap;
	AfxSetResourceHandle(GetResInstanceHandle());
	if ( Bitmap.LoadBitmap(g_GlobalUnits.m_ChatExpViewImage.uChatBugle) )
	{
	m_MessageProxyHelper->InsertImage(&Bitmap);
	Bitmap.DeleteObject();
	}
	AfxSetResourceHandle(GetModuleHandle(NULL));

	//��������
	m_MessageProxyHelper->InsertUserChat(pBugleProperty->szUserName,pBugleProperty->szContext,pBugleProperty->crText,MS_NORMAL);

	//��������
	HINSTANCE hInstance = AfxGetInstanceHandle();
	HRSRC hResour=FindResource(hInstance,TEXT("BUGLE"),TEXT("WAVE"));
	if (hResour!=NULL)
	{
	HGLOBAL hGlobal=LoadResource(hInstance,hResour);
	if (hGlobal!=NULL) 
	{
	//��������
	LPCTSTR pszMemory=(LPCTSTR)LockResource(hGlobal);
	BOOL bResult = ::PlaySound(pszMemory,hInstance,SND_ASYNC|SND_MEMORY|SND_NODEFAULT);

	//������Դ
	UnlockResource(hGlobal);
	}
	FreeResource(hGlobal);
	}
	}
	//��������
	else if( Command.wSubCmdID == SUB_GF_FLOWER )
	{
	//Ч�����
	ASSERT( sizeof(CMD_GF_GiftNotify) == wDataSize );
	if ( sizeof(CMD_GF_GiftNotify) != wDataSize ) return false;

	//��������
	CMD_GF_GiftNotify * pGiftNotify=(CMD_GF_GiftNotify *)pData;

	//��������
	if(pGiftNotify->cbSendLocation==LOCATION_PLAZA_ROOM)
	{
	//��ȡ���
	IUserItem * pISendUserItem=m_ClientUserManager.SearchUserByUserID(pGiftNotify->dwSendUserID);
	IUserItem * pIRcvUserItem=m_ClientUserManager.SearchUserByUserID(pGiftNotify->dwRcvUserID);

	ASSERT( pISendUserItem != NULL && pIRcvUserItem != NULL );
	if ( pISendUserItem == NULL || pIRcvUserItem == NULL ) return true;

	tagUserData const *pSendUserData = pISendUserItem->GetUserData();
	tagUserData const *pRcvUserData = pIRcvUserItem->GetUserData();

	//����ж�
	ASSERT( pSendUserData != NULL && pRcvUserData != NULL );
	if ( pSendUserData == NULL || pRcvUserData == NULL ) return true;
	ASSERT( pGiftNotify->wGiftID < FLOWER_COUNT ) ;
	if ( pGiftNotify->wGiftID >= FLOWER_COUNT ) return true;

	//������Ϣ
	CString strGiftMsg;
	int nFlowerID = pGiftNotify->wGiftID;

	strGiftMsg.Format(TEXT("[%s]"),pSendUserData->szName);
	m_MessageProxyHelper->InsertShtickString(strGiftMsg,RGB(0,0,255), strGiftMsg.GetLength(), false);

	strGiftMsg = g_FlowerDescribe[nFlowerID].szAction;
	m_MessageProxyHelper->InsertShtickString(strGiftMsg,RGB(255,0,255), strGiftMsg.GetLength(), false);

	strGiftMsg.Format(TEXT("[%s]"),pRcvUserData->szName);
	m_MessageProxyHelper->InsertShtickString(strGiftMsg,RGB(0,0,255),strGiftMsg.GetLength(),false);

	strGiftMsg = g_FlowerDescribe[nFlowerID].szQuantifier1;
	m_MessageProxyHelper->InsertShtickString(strGiftMsg,RGB(255,0,255), strGiftMsg.GetLength(), false);

	//����ͼƬ
	CBitmap Bitmap;
	AfxSetResourceHandle(GetResInstanceHandle());
	if ( Bitmap.LoadBitmap(g_GlobalUnits.m_ChatExpViewImage.uChatGift[nFlowerID]) )
	{
	m_MessageProxyHelper->InsertImage(&Bitmap);
	Bitmap.DeleteObject();
	}
	AfxSetResourceHandle(GetModuleHandle(NULL));

	WORD wFlowerCount = pGiftNotify->wFlowerCount;
	strGiftMsg.Format(TEXT("����%d%s%s[ %s ]"), pGiftNotify->wFlowerCount, g_FlowerDescribe[nFlowerID].szQuantifier2,
	g_FlowerDescribe[nFlowerID].szResult, pRcvUserData->szName);
	m_MessageProxyHelper->InsertShtickString(strGiftMsg,RGB(255,0,255), strGiftMsg.GetLength(), false);	

	strGiftMsg.Format(g_FlowerDescribe[nFlowerID].szCharmResult, wFlowerCount * abs(CPropertyBar::m_FlowerInfo[nFlowerID].lRcvUserCharm));
	m_MessageProxyHelper->InsertShtickString(strGiftMsg,RGB(255,0,255), strGiftMsg.GetLength(), true);	
	}

	}
	*/

	return true;
}
/*

bool CRoomViewItem::OnSocketTask(CMD_Command Command, void * pBuffer, WORD wDataSize)
{
ASSERT(wDataSize<=SOCKET_PACKET);
if (wDataSize>SOCKET_PACKET)return false;

//��������
IPC_SocketPackage SocketPackage;
memset(&SocketPackage,0,sizeof(SocketPackage));
SocketPackage.Command=Command;
if (wDataSize>0)
{
ASSERT(pData!=NULL);
CopyMemory(SocketPackage.cbBuffer,pData,wDataSize);
}

switch (Command.wSubCmdID)
{
//�������
case SUB_GF_TASK_ACCEPT:
{
//Ч������
ASSERT(wDataSize%sizeof(CMD_GF_TASK_tagTask)==0);
if (wDataSize%sizeof(CMD_GF_TASK_tagTask)!=0)return false;
CMD_GF_TASK_tagTask * pTagTask = (CMD_GF_TASK_tagTask *)pBuffer;
CopyMemory(&m_tagTask, pTagTask, sizeof(m_tagTask));
//��ȡ�û�
tagUserData * pUserData=m_pMeUserItem->GetUserData();
//���ݻ���
if ((pUserData->wTableID!=INVALID_TABLE)&&(pUserData->wChairID!=INVALID_CHAIR))
{
//���ñ���
m_wPacketTableID=pUserData->wTableID;
m_wPacketChairID=pUserData->wChairID;
//��������
WORD wSendSize=sizeof(SocketPackage.Command)+wDataSize;
SendProcessData(IPC_MAIN_SOCKET,IPC_SUB_SOCKET_RECV,&SocketPackage,wSendSize);
}
return true;
}
//�������
case SUB_GF_TASK_SAVEJD:
{
ASSERT(wDataSize%sizeof(CMD_GF_TASK_CountNums)==0);
if (wDataSize%sizeof(CMD_GF_TASK_CountNums)!=0)	return false;
return true;
}
//�����������
case SUB_GF_TASK_ONCEEND:
{
ASSERT(wDataSize%sizeof(CMD_GF_TASK_tagTaskFinish)==0);
if (wDataSize%sizeof(CMD_GF_TASK_tagTaskFinish)!=0)	return false;


return true;
}
}

return true;
}*/

//�û�����
bool CRoomViewItem::OnSocketSubUserCome(CMD_Command Command, void * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(Command.wMainCmdID==MDM_GR_USER);
	ASSERT(Command.wSubCmdID==SUB_GR_USER_COME);
	ASSERT(wDataSize>=sizeof(tagUserInfoHead));
	if (wDataSize<sizeof(tagUserInfoHead))
	{
		return false;
	}


	//��ȡ������Ϣ
	tagUserData UserData;
	memset(&UserData,0,sizeof(UserData));
	tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)pData;

	//��ȡ��Ϣ
	UserData.dwUserID=pUserInfoHead->dwUserID;
	UserData.wTableID=pUserInfoHead->wTableID;
	UserData.wChairID=pUserInfoHead->wChairID;
	UserData.cbUserStatus=pUserInfoHead->cbUserStatus;
	UserData.dwUserRight=pUserInfoHead->dwUserRight;
	UserData.dwMasterRight=pUserInfoHead->dwMasterRight;
	//UserData.lMoney = pUserInfoHead->lMoney;

	//�����ж�
	if ((pUserInfoHead->dwUserID==g_GlobalUnits.GetGolbalUserData().dwUserID)&&(pUserInfoHead->cbMasterOrder>=2))
	{
		m_cbHideUserInfo=FALSE;
	}

	//��ȡ��Ϣ
	if ((m_cbHideUserInfo==FALSE)||(pUserInfoHead->dwUserID==g_GlobalUnits.GetGolbalUserData().dwUserID))
	{
		UserData.wFaceID=pUserInfoHead->wFaceID;
		UserData.dwCustomFaceVer=pUserInfoHead->dwCustomFaceVer;
		UserData.cbGender=pUserInfoHead->cbGender;
		UserData.cbMemberOrder=pUserInfoHead->cbMemberOrder;
		UserData.cbMasterOrder=pUserInfoHead->cbMasterOrder;
		UserData.dwGameID=pUserInfoHead->dwGameID;
		UserData.dwGroupID=pUserInfoHead->dwGroupID;
		UserData.lLoveliness=pUserInfoHead->lLoveliness;
		//UserData.lScore=pUserInfoHead->UserScoreInfo.lScore;
		//UserData.lGameGold=pUserInfoHead->UserScoreInfo.lGameGold;
		UserData.lMoney=pUserInfoHead->UserScoreInfo.lMoney;
		UserData.lWinCount=pUserInfoHead->UserScoreInfo.lWinCount;
		UserData.lLostCount=pUserInfoHead->UserScoreInfo.lLostCount;
		UserData.lDrawCount=pUserInfoHead->UserScoreInfo.lDrawCount;
		UserData.lFleeCount=pUserInfoHead->UserScoreInfo.lFleeCount;
		UserData.lExperience=pUserInfoHead->UserScoreInfo.lExperience;

		UserData.dwGrade = pUserInfoHead->UserScoreInfo.dwGrade;
		//UserData.dwOrder = pUserInfoHead->UserScoreInfo.dwOrder;

		UserData.lGem = pUserInfoHead->UserScoreInfo.lGem;   //�û��ı�ʯ
		UserData.lGold = pUserInfoHead->UserScoreInfo.lGold;//���
		//UserData.lHonor = pUserInfoHead->UserScoreInfo.lHonor;  //�û�������
		//UserData.lCredit = pUserInfoHead->UserScoreInfo.lCredit; //�û�������

		//�û���Ϸ���� Author<cxf>;DateTime<2012-03-05>
		UserData.lKindIDWinCount= pUserInfoHead->UserScoreInfo.lKindIDWinCount;
		UserData.lKindIDLostCount= pUserInfoHead->UserScoreInfo.lKindIDLostCount;
		UserData.lKindIDFleeCount= pUserInfoHead->UserScoreInfo.lKindIDFleeCount;
		UserData.lKindIDDrawCount= pUserInfoHead->UserScoreInfo.lKindIDDrawCount;

		//�ȼ���ν Author<cxf>;DateTime<2012-03-15>
		::memcpy(UserData.chGradeName, ((CPlazaDlg*)GetParent())->m_pGradeInfo[UserData.dwGrade-1].strName, sizeof(UserData.chGradeName));
		::memcpy(UserData.szAccounts, pUserInfoHead->szAccounts, sizeof(UserData.szAccounts));
		if (pUserInfoHead->dwUserID==g_GlobalUnits.GetGolbalUserData().dwUserID)
		{
			//����ȫ�ֱ����û���Ϣ	--osc update
			tagGlobalUserData& tagUserInfo = g_GlobalUnits.GetGolbalUserData();
			tagUserInfo.lGem = pUserInfoHead->UserScoreInfo.lGem;	//��ʯ
			tagUserInfo.lGold = pUserInfoHead->UserScoreInfo.lGold;	//���
			tagUserInfo.lMoney=pUserInfoHead->UserScoreInfo.lMoney;//�ر���
			tagUserInfo.dwExperience = pUserInfoHead->UserScoreInfo.lExperience;	//����
			tagUserInfo.dwGrade = pUserInfoHead->UserScoreInfo.dwGrade;	//�ȼ�
			//tagUserInfo.lHonor = pUserInfoHead->UserScoreInfo.lHonor;	//����
			tagUserInfo.lWinCount = pUserInfoHead->UserScoreInfo.lWinCount; //ʤ������
			tagUserInfo.lLostCount = pUserInfoHead->UserScoreInfo.lLostCount;	//ʧ������
			tagUserInfo.lDrawCount = pUserInfoHead->UserScoreInfo.lDrawCount;	//�;�����
			tagUserInfo.lFleeCount = pUserInfoHead->UserScoreInfo.lFleeCount;	//��������
			GetParent()->PostMessage(WM_COMMAND, IDB_CALL_UPDATE_USERIMAGE, 0);
		}
	}

	/*cxf if (pUserInfoHead->dwUserID==g_GlobalUnits.GetGolbalUserData().dwUserID)
	{
	m_dwWinGemsToday = pUserInfoHead->UserScoreInfo.dwWinGemsToday;
	}*/

	//��ȡ��չ��Ϣ
	void * pDataBuffer=NULL;
	tagDataDescribe DataDescribe;
	CRecvPacketHelper RecvPacket(pUserInfoHead+1,wDataSize-sizeof(tagUserInfoHead));
	while (true)
	{
		pDataBuffer=RecvPacket.GetData(DataDescribe);
		if (DataDescribe.wDataDescribe==DTP_NULL)
		{
			break;
		}
		switch (DataDescribe.wDataDescribe)
		{
		case DTP_USER_ACCOUNTS:		//�û��ǳ�
			{
				if ((m_cbHideUserInfo==FALSE)||(pUserInfoHead->dwUserID==g_GlobalUnits.GetGolbalUserData().dwUserID))
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserData.szName));
					if (DataDescribe.wDataSize<=sizeof(UserData.szName))
					{
						CopyMemory(&UserData.szName,pDataBuffer,DataDescribe.wDataSize);
						UserData.szName[CountArray(UserData.szName)-1]=0;
					}
				}
				else
				{
					lstrcpyn(UserData.szName,TEXT("��Ϸ�û�"),CountArray(UserData.szName));
				}
				break;
			}
		case DTP_UNDER_WRITE:		//����ǩ��
			{
				if ((m_cbHideUserInfo==FALSE)||(pUserInfoHead->dwUserID==g_GlobalUnits.GetGolbalUserData().dwUserID))
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserData.szUnderWrite));
					if (DataDescribe.wDataSize<=sizeof(UserData.szUnderWrite))
					{
						CopyMemory(&UserData.szUnderWrite,pDataBuffer,DataDescribe.wDataSize);
						UserData.szUnderWrite[CountArray(UserData.szUnderWrite)-1]=0;
					}
				}
				break;
			}
		case DTP_USER_GROUP_NAME:	//�û�����
			{
				if ((m_cbHideUserInfo==FALSE)||(pUserInfoHead->dwUserID==g_GlobalUnits.GetGolbalUserData().dwUserID))
				{
					ASSERT(pDataBuffer!=NULL);
					ASSERT(DataDescribe.wDataSize<=sizeof(UserData.szGroupName));
					if (DataDescribe.wDataSize<=sizeof(UserData.szGroupName))
					{
						CopyMemory(&UserData.szGroupName,pDataBuffer,DataDescribe.wDataSize);
						UserData.szGroupName[CountArray(UserData.szGroupName)-1]=0;
					}
				}
				break;
			}
		}
	}

	//�����û�
	IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(UserData.dwUserID);
	if (pIUserItem==NULL) 
	{
		const tagCompanionItem * pCompanionItem=NULL;
		pCompanionItem=g_GlobalUnits.m_CompanionManager->SearchCompanionItem(UserData.dwUserID);
		if (pCompanionItem!=NULL) 
		{
			UserData.cbCompanion=pCompanionItem->Companion;
		}
		pIUserItem=m_ClientUserManager.ActiveUserItem(UserData);
	}
	else
	{
		OnUserItemUpdate(pIUserItem);
	}


	//�����û���Ϣ
	ASSERT(pIUserItem!=NULL);
	if (pIUserItem!=NULL)
	{
		//�ж��Լ�
		if (m_pMeUserItem==NULL)
		{
			m_pMeUserItem=pIUserItem;
		}

		//���ý���
		BYTE cbUserStatus = UserData.cbUserStatus;
		if (  ( cbUserStatus >= US_SIT ) 
			&& ( cbUserStatus != US_LOOKON ) )
		{
			m_TableFrame.SetUserInfo(UserData.wTableID,UserData.wChairID,pIUserItem);
		}

		//��ʾ��Ϣ
		if ((m_cbHideUserInfo==FALSE)&&(m_ServiceStatus==ServiceStatus_Serviceing))
		{
			if ((UserData.cbCompanion==enCompanion_Friend)||(UserData.dwMasterRight!=0L))
			{
				TCHAR szMessage[256]=TEXT("");
				_sntprintf(szMessage,sizeof(szMessage),TEXT("[ %s ] ������"),UserData.szName);
				m_MessageProxyHelper->InsertSystemString(szMessage,0);
			}
			else if (g_GlobalOption.m_bShowInOutMessage==true)
			{
				TCHAR szMessage[256]=TEXT("");
				_sntprintf(szMessage,sizeof(szMessage),TEXT("[ %s ] ������"),UserData.szName);
				m_MessageProxyHelper->InsertGeneralString(szMessage,0,0,true);
			}
		}
	}

	//�Զ�ͷ��
	if(UserData.dwCustomFaceVer!=0)
	{
		//ͷ������
		CString strDirName = CString(g_GlobalUnits.GetWorkDirectory()) + TEXT("\\CustomFace");
		CString strFileName;
		strFileName.Format(TEXT("\\%ld_%d.bmp"), pUserInfoHead->dwUserID, pUserInfoHead->dwCustomFaceVer);

		//��ȡ�ļ�
		CImage FaceImage;
		HRESULT hResult = FaceImage.Load(strDirName + strFileName);
		if (SUCCEEDED(hResult))
		{
			FaceImage.Destroy();
		}
		//����ͷ��
		else
		{
			////Ͷ����Ϣ
			//CPlazaViewItem *pPlazaViewItem = ((CGameFrame *)AfxGetMainWnd())->GetPlazaViewItem();
			//pPlazaViewItem->PostMessage(WM_DOWN_LOAD_FACE, LPARAM(pUserInfoHead->dwCustomFaceVer), WPARAM(pUserInfoHead->dwUserID));
		}
	}

	//���±���
	UpdateTitleText();

	//��������
	DWORD dwOnLineCount=m_ClientUserManager.GetOnLineCount();
	g_GlobalUnits.m_ServerListManager.UpdateGameServerOnLine(m_pListServer,dwOnLineCount);

	return true;
}

//�û�״̬
bool CRoomViewItem::OnSocketSubStatus(CMD_Command Command, void * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(Command.wMainCmdID==MDM_GR_USER);
	ASSERT(Command.wSubCmdID==SUB_GR_USER_STATUS);
	ASSERT(wDataSize>=sizeof(CMD_GR_UserStatus));
	if (wDataSize<sizeof(CMD_GR_UserStatus))
	{
		return false;
	}

	//��������
	CMD_GR_UserStatus * pUserStatus=(CMD_GR_UserStatus *)pData;
	IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(pUserStatus->dwUserID);
	ASSERT(pIUserItem!=NULL);
	if (pIUserItem==NULL)
	{
		return true;
	}


	//�������
	tagUserData * pUserData=pIUserItem->GetUserData();
	tagUserData * pMeUserData=m_pMeUserItem->GetUserData();
	WORD wNowTableID=pUserStatus->wTableID,wLastTableID=pUserData->wTableID;
	WORD wNowChairID=pUserStatus->wChairID,wLastChairID=pUserData->wChairID;
	BYTE cbNowStatus=pUserStatus->cbUserStatus,cbLastStatus=pUserData->cbUserStatus;

	//����״̬
	if (pUserData==pMeUserData)
	{
		m_wReqTableID=INVALID_TABLE;
		m_wReqChairID=INVALID_CHAIR;
	}

	//�����״̬
	if ((wLastTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		ASSERT(wLastChairID!=INVALID_CHAIR);
		IUserItem * pITableUserItem=m_TableFrame.GetUserInfo(wLastTableID,wLastChairID);
		if (pITableUserItem==pIUserItem) m_TableFrame.SetUserInfo(wLastTableID,wLastChairID,NULL);
	}

	//�û��뿪
	if (cbNowStatus==US_NULL)
	{
		//֪ͨ��Ϸ
		if ((pMeUserData->wTableID!=INVALID_TABLE)&&(pMeUserData->wTableID==wLastTableID))
		{
			IPC_UserStatus UserStatus;
			memset(&UserStatus,0,sizeof(UserStatus));
			UserStatus.dwUserID=pUserData->dwUserID;
			UserStatus.cbUserStatus=pUserData->cbUserStatus;
			SendProcessData(IPC_MAIN_USER,IPC_SUB_USER_STATUS,&UserStatus,sizeof(UserStatus));
		}

		//ɾ���û�
		m_ClientUserManager.DeleteUserItem(pIUserItem);

		//���±���
		UpdateTitleText();

		//��������
		DWORD dwOnLineCount=m_ClientUserManager.GetOnLineCount();
		g_GlobalUnits.m_ServerListManager.UpdateGameServerOnLine(m_pListServer,dwOnLineCount);

		return true;
	}

	//����״̬
	tagUserStatus UserStatus;
	UserStatus.wTableID=wNowTableID;
	UserStatus.wChairID=wNowChairID;
	UserStatus.cbUserStatus=cbNowStatus;
	m_ClientUserManager.UpdateUserItemStatus(pIUserItem,&UserStatus);

	//������״̬
	if ((wNowTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
	{
		//��������
		if (cbNowStatus!=US_LOOKON)
		{
			ASSERT(wNowChairID!=INVALID_CHAIR);
			m_TableFrame.SetUserInfo(wNowTableID,wNowChairID,pIUserItem);
		}

		//�����û�
		if ((m_pMeUserItem!=pIUserItem)&&(pMeUserData->wTableID==wNowTableID))
		{
			CIPCSendCopyData SendCopyData(m_hWndChannel,m_hWnd);
			SendTableUser(pIUserItem,&SendCopyData);
		}
	}

	//���½���
	if ((wNowTableID!=INVALID_TABLE)&&(cbNowStatus!=US_LOOKON)
		&&(wNowTableID==wLastTableID)&&(wNowChairID==wLastChairID))
	{
		ASSERT(wNowChairID!=INVALID_CHAIR);
		m_TableFrame.UpdateTableView(wNowTableID,false);
	}

	//�жϷ���
	bool bNotifyGame=false;
	if (pIUserItem==m_pMeUserItem)
	{
		bNotifyGame=true;
	}
	else if ((pMeUserData->wTableID!=INVALID_TABLE)&&(pMeUserData->wTableID==wNowTableID)) 
	{
		bNotifyGame=true;
	}
	else if ((pMeUserData->wTableID!=INVALID_TABLE)&&(pMeUserData->wTableID==wLastTableID)) 
	{
		bNotifyGame=true;
	}


	//����״̬
	if (bNotifyGame==true)
	{
		IPC_UserStatus UserStatus;
		memset(&UserStatus,0,sizeof(UserStatus));
		UserStatus.dwUserID=pUserData->dwUserID;
		UserStatus.cbUserStatus=pUserData->cbUserStatus;
		SendProcessData(IPC_MAIN_USER,IPC_SUB_USER_STATUS,&UserStatus,sizeof(UserStatus));
	}

	//�л�λ���ж�
	if (cbLastStatus==US_FREE 
		&& cbNowStatus==US_PLAY 
		&& pMeUserData->cbUserStatus==US_LOOKON 
		&& wNowTableID==pMeUserData->wTableID 
		&& wNowChairID==pMeUserData->wChairID) 
	{
		IPC_UserStatus UserStatus;
		memset(&UserStatus,0,sizeof(UserStatus));
		UserStatus.dwUserID=pMeUserData->dwUserID;
		UserStatus.cbUserStatus=US_FREE;
		SendProcessData(IPC_MAIN_USER,IPC_SUB_USER_STATUS,&UserStatus,sizeof(UserStatus));
		StartGameClient();
	}

	//�ж��Լ�
	if (pIUserItem==m_pMeUserItem)
	{
		//���ñ���
		if ((wNowTableID==m_wReqTableID)&&(wNowChairID==m_wReqChairID))
		{
			m_wReqTableID=INVALID_TABLE;
			m_wReqChairID=INVALID_CHAIR;
		}

		//�ټ����ж�
		if (m_TableFrame.GetChairCount()>MAX_CHAIR_NORMAL && US_SIT==cbNowStatus)
		{
			m_wReqTableID=INVALID_TABLE;
			m_wReqChairID=INVALID_CHAIR;
		}

		//��������
		if ((m_wPacketTableID!=INVALID_TABLE)&&((m_wPacketTableID!=wNowTableID)||(m_wPacketChairID!=wNowChairID)))
		{
			m_wPacketTableID=INVALID_TABLE;
			m_wPacketChairID=INVALID_CHAIR;
			m_PacketDataStorage.RemoveData(false);
		}

		//������Ϸ
		if ((wNowTableID!=INVALID_TABLE)&&((wNowTableID!=wLastTableID)||(wNowChairID!=wLastChairID)))
		{
			int iResult=StartGameClient();
		}
	}

	return true;
}

//�û�����
bool CRoomViewItem::OnSocketSubScore(CMD_Command Command, void * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(Command.wMainCmdID==MDM_GR_USER);
	ASSERT(Command.wSubCmdID==SUB_GR_USER_SCORE);
	ASSERT(wDataSize>=sizeof(CMD_GR_UserScore));
	if (wDataSize<sizeof(CMD_GR_UserScore))
	{
		return false;
	}


	//��������
	CMD_GR_UserScore * pUserScore=(CMD_GR_UserScore *)pData;
	IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(pUserScore->dwUserID);
	ASSERT(pIUserItem!=NULL);
	if (pIUserItem==NULL) 
	{
		return true;
	}


	//�����ж�
	if ((m_cbHideUserInfo==FALSE)||(pIUserItem==m_pMeUserItem))
	{
		//���·���
		tagUserData * pUserData=pIUserItem->GetUserData();
		pUserData->lLoveliness = pUserScore->lLoveliness;
		pUserData->lGold = pUserScore->UserScore.lGold;
		pUserData->lMoney = pUserScore->UserScore.lMoney;
		pUserData->dwGrade = pUserScore->UserScore.dwGrade;
		//pUserData->dwOrder = pUserScore->UserScore.dwOrder;
		pUserData->lGem = pUserScore->UserScore.lGem;
		//pUserData->lHonor = pUserScore->UserScore.lHonor;
		//pUserData->lCredit = pUserScore->UserScore.lCredit;

		//�û���Ϸ���� Author<cxf>;DateTime<2012-03-05>
		pUserData->lKindIDWinCount=pUserScore->UserScore.lKindIDWinCount;
		pUserData->lKindIDLostCount=pUserScore->UserScore.lKindIDLostCount;
		pUserData->lKindIDFleeCount=pUserScore->UserScore.lKindIDFleeCount;
		pUserData->lKindIDDrawCount=pUserScore->UserScore.lKindIDDrawCount;

		if (pUserScore->dwUserID==g_GlobalUnits.GetGolbalUserData().dwUserID)
		{
			//m_dwWinGemsToday = pUserScore->UserScore.dwWinGemsToday;

			//����ȫ�ֱ����û���Ϣ	--osc update
			tagGlobalUserData& tagUserInfo = g_GlobalUnits.GetGolbalUserData();
			tagUserInfo.lGem = pUserScore->UserScore.lGem;	//��ʯ
			tagUserInfo.lGold = pUserScore->UserScore.lGold;	//���
			tagUserInfo.lMoney=pUserScore->UserScore.lMoney; //�ر���
			tagUserInfo.dwExperience = pUserScore->UserScore.lExperience;	//����
			tagUserInfo.dwGrade = pUserScore->UserScore.dwGrade;	//�ȼ�
			//tagUserInfo.lHonor = pUserScore->UserScore.lHonor;	//����
			tagUserInfo.lWinCount = pUserScore->UserScore.lWinCount; //ʤ������
			tagUserInfo.lLostCount = pUserScore->UserScore.lLostCount;	//ʧ������
			tagUserInfo.lDrawCount = pUserScore->UserScore.lDrawCount;	//�;�����
			tagUserInfo.lFleeCount = pUserScore->UserScore.lFleeCount;	//��������
			GetParent()->PostMessage(WM_COMMAND, IDB_CALL_UPDATE_USERIMAGE, 0);
		}

		m_ClientUserManager.UpdateUserItemScore(pIUserItem,&pUserScore->UserScore);

		//������Ϸ
		tagUserData * pMeUserData=m_pMeUserItem->GetUserData();
		if ((pMeUserData->wTableID!=INVALID_TABLE)&&(pMeUserData->wTableID==pUserData->wTableID))
		{
			IPC_UserScore UserScore;
			memset(&UserScore,0,sizeof(UserScore));
			UserScore.dwUserID=pUserScore->dwUserID;
			UserScore.UserScore=pUserScore->UserScore;
			UserScore.lLoveliness = pUserScore->lLoveliness;
			//�û���Ϸ���� Author<cxf>;DateTime<2012-03-05>
			UserScore.UserScore.lKindIDWinCount=pUserScore->UserScore.lKindIDWinCount;
			UserScore.UserScore.lKindIDLostCount=pUserScore->UserScore.lKindIDLostCount;
			UserScore.UserScore.lKindIDFleeCount=pUserScore->UserScore.lKindIDFleeCount;
			UserScore.UserScore.lKindIDDrawCount=pUserScore->UserScore.lKindIDDrawCount;

			//�ȼ���ν
			::memcpy(UserScore.UserScore.chGradeName,((CPlazaDlg*)GetParent())->m_pGradeInfo[pUserScore->UserScore.dwGrade-1].strName, sizeof(UserScore.UserScore.chGradeName));

			SendProcessData(IPC_MAIN_USER,IPC_SUB_USER_SCORE,&UserScore,sizeof(UserScore));
		}
	}

	return true;
}

//�û�Ȩ��
bool CRoomViewItem::OnSocketSubRight(CMD_Command Command, void * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(Command.wMainCmdID==MDM_GR_USER);
	ASSERT(Command.wSubCmdID==SUB_GR_USER_RIGHT);
	ASSERT(wDataSize==sizeof(CMD_GR_UserRight));
	if (wDataSize!=sizeof(CMD_GR_UserRight))
	{
		return false;
	}


	//��Ϣ����
	CMD_GR_UserRight * pUserRight=(CMD_GR_UserRight *)pData;

	//�û�Ȩ��
	IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(pUserRight->dwUserID);
	pIUserItem->GetUserData()->dwUserRight = pUserRight->dwUserRight;

	return true;
}

//��Ա�ȼ�
bool CRoomViewItem::OnSocketSubMemberOrder(CMD_Command Command, void * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(Command.wMainCmdID==MDM_GR_USER);
	ASSERT(Command.wSubCmdID==SUB_GR_MEMBER_ORDER);
	ASSERT(wDataSize==sizeof(CMD_GR_MemberOrder));
	if (wDataSize!=sizeof(CMD_GR_MemberOrder))
	{
		return false;
	}


	//��Ϣ����
	CMD_GR_MemberOrder * pMemberOrder=(CMD_GR_MemberOrder *)pData;

	//��Ա�ȼ�
	IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(pMemberOrder->dwUserID);
	tagUserData * pUserData=pIUserItem->GetUserData();
	pUserData->cbMemberOrder = pMemberOrder->cbMemberOrder;
	tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();
	GlobalUserInfo.cbMember = pMemberOrder->cbMemberOrder;
	GetParent()->PostMessage(WM_COMMAND, IDB_CALL_UPDATE_USERIMAGE, 0);

	//������Ϸ
	tagUserData * pMeUserData=m_pMeUserItem->GetUserData();
	if ((pMeUserData->wTableID!=INVALID_TABLE)&&(pMeUserData->wTableID==pUserData->wTableID))
	{
		IPC_MemberOrder MemberOrder;
		memset(&MemberOrder,0,sizeof(MemberOrder));
		MemberOrder.dwUserID=pUserData->dwUserID;
		MemberOrder.cbMember=pUserData->cbMemberOrder;
		SendProcessData(IPC_MAIN_USER,IPC_SUB_MEMBERORDER,&MemberOrder,sizeof(MemberOrder));
	}

	return true;
}

//����ʧ��
bool CRoomViewItem::OnSocketSubSitFailed(CMD_Command Command, void * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(Command.wMainCmdID==MDM_GR_USER);
	ASSERT(Command.wSubCmdID==SUB_GR_SIT_FAILED);

	//��Ϣ����
	CMD_GR_SitFailed * pSitFailed=(CMD_GR_SitFailed *)pData;
	IUserItem * pISendUserItem=m_TableFrame.GetUserInfo(m_wReqTableID,m_wReqChairID);
	if (pISendUserItem==m_pMeUserItem) 
	{
		m_TableFrame.SetUserInfo(m_wReqTableID,m_wReqChairID,NULL);
	}


	//���ñ���
	m_wReqTableID=INVALID_TABLE;
	m_wReqChairID=INVALID_CHAIR;

	//��ʾ��Ϣ
	ShowMessageBox(pSitFailed->szFailedDescribe,MB_ICONINFORMATION);

	//GetParent()->ShowWindow(SW_SHOW);
	//GetParent()->PostMessage(WM_COMMAND,IDM_CALL_GAME_LIST,0);
	//CloseRoomViewItem();

	return true;
}

//�û�����
bool CRoomViewItem::OnSocketSubChat(CMD_Command Command, void * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(Command.wMainCmdID==MDM_GR_USER);
	ASSERT(Command.wSubCmdID==SUB_GR_USER_CHAT);

	//Ч�����
	CMD_GR_UserChat * pUserChat=(CMD_GR_UserChat *)pData;
	ASSERT(wDataSize>=(sizeof(CMD_GR_UserChat)-sizeof(pUserChat->szChatMessage)));
	ASSERT(wDataSize==(sizeof(CMD_GR_UserChat)-sizeof(pUserChat->szChatMessage)+pUserChat->wChatLength));
	if (wDataSize<(sizeof(CMD_GR_UserChat)-sizeof(pUserChat->szChatMessage))) 
	{
		return false;
	}
	if (wDataSize!=(sizeof(CMD_GR_UserChat)-sizeof(pUserChat->szChatMessage)+pUserChat->wChatLength)) 
	{
		return false;
	}

	//Ѱ���û�
	IUserItem * pISendUserItem=m_ClientUserManager.SearchUserByUserID(pUserChat->dwSendUserID);
	if (pISendUserItem==NULL)
	{
		return true;
	}
	tagUserData * pSendUserData=pISendUserItem->GetUserData();

	//��Ϣ����
	if ((pISendUserItem!=m_pMeUserItem)&&(pSendUserData->cbCompanion==enCompanion_Detest)) 
	{
		return true;
	}


	//��ʾ��Ϣ
	if (pUserChat->dwTargetUserID!=0L)
	{
		IUserItem * pIRecvUserItem=m_ClientUserManager.SearchUserByUserID(pUserChat->dwTargetUserID);
		if (pIRecvUserItem==NULL) 
		{
			return true;
		}
		tagUserData * pRecvUserData=pIRecvUserItem->GetUserData();
		m_MessageProxyHelper->InsertUserChat(pSendUserData->szName,pRecvUserData->szName,pUserChat->szChatMessage,pUserChat->crFontColor,MS_NORMAL);
	}
	else
	{
		m_MessageProxyHelper->InsertUserChat(pSendUserData->szName,pUserChat->szChatMessage,pUserChat->crFontColor,MS_NORMAL);
	}

	return true;
}

//�û�˽��
bool CRoomViewItem::OnSocketSubWisper(CMD_Command Command, void * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(Command.wMainCmdID==MDM_GR_USER);
	ASSERT(Command.wSubCmdID==SUB_GR_USER_WISPER);

	//Ч�����
	CMD_GR_Wisper * pWisper=(CMD_GR_Wisper *)pData;
	ASSERT(wDataSize>=(sizeof(CMD_GR_Wisper)-sizeof(pWisper->szChatMessage)));
	ASSERT(wDataSize==(sizeof(CMD_GR_Wisper)-sizeof(pWisper->szChatMessage)+pWisper->wChatLength));
	if (wDataSize<(sizeof(CMD_GR_Wisper)-sizeof(pWisper->szChatMessage)))
	{
		return false;
	}
	if (wDataSize!=(sizeof(CMD_GR_Wisper)-sizeof(pWisper->szChatMessage)+pWisper->wChatLength)) 
	{
		return false;
	}


	//Ѱ���û�
	IUserItem * pISendUserItem=m_ClientUserManager.SearchUserByUserID(pWisper->dwSendUserID);
	IUserItem * pIRecvUserItem=m_ClientUserManager.SearchUserByUserID(pWisper->dwTargetUserID);
	if ((pISendUserItem==NULL)||(pIRecvUserItem==NULL))
	{
		return true;
	}

	tagUserData * pUserDataSend=pISendUserItem->GetUserData();
	tagUserData * pUserDataRecv=pIRecvUserItem->GetUserData();

	//��Ϣ����
	if ((pUserDataSend->cbMasterOrder==0) 
		&& (pISendUserItem!=m_pMeUserItem)
		&&(pUserDataSend->cbCompanion==enCompanion_Detest))
	{
		return true;
	}


	//��ʾ��Ϣ
	CShortMessage * pShortMessageWnd=NULL;
	if (pWisper->dwSendUserID==m_pMeUserItem->GetUserID())
	{
		//�Լ����͵���Ϣ
		pShortMessageWnd=ActiveShortWnd(pWisper->dwTargetUserID,pIRecvUserItem,true);
		if (pShortMessageWnd!=NULL) 
		{
			pShortMessageWnd->OnRecvMessage(pUserDataSend->szName,pWisper->szChatMessage,pWisper->crFontColor,true);
		}
	}
	else	//������Ϸ�߷�����Ϣ
	{
		pShortMessageWnd=ActiveShortWnd(pWisper->dwSendUserID,pISendUserItem,true);
		if (pShortMessageWnd!=NULL)
		{
			pShortMessageWnd->OnRecvMessage(pUserDataSend->szName,pWisper->szChatMessage,pWisper->crFontColor,false);
		}
	}

	return true;
}

//�û�����
bool CRoomViewItem::OnSocketSubUserInvite(CMD_Command Command, void * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(Command.wMainCmdID==MDM_GR_USER);
	ASSERT(Command.wSubCmdID==SUB_GR_USER_INVITE);

	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_UserInvite));
	if (wDataSize!=sizeof(CMD_GR_UserInvite)) 
	{
		return false;
	}


	//��Ϣ����
	CMD_GR_UserInvite * pUserInvite=(CMD_GR_UserInvite *)pData;
	ASSERT(pUserInvite->wTableID<m_TableFrame.GetTableCount());
	if (m_TableFrame.QueryPlayFlag(pUserInvite->wTableID)==true) 
	{
		return true;
	}
	//Ѱ���û�
	IUserItem * pIUserItem=m_ClientUserManager.SearchUserByUserID(pUserInvite->dwUserID);
	if (pIUserItem==NULL) 
	{
		return true;
	}

	tagUserData * pUserData=pIUserItem->GetUserData();
	if (pUserData->wTableID==INVALID_TABLE) 
	{
		return true;
	}


	//�û�����
	if (pUserData->cbCompanion==enCompanion_Detest)
	{
		return true;
	}
	if (g_GlobalOption.m_InviteMode==enInviteMode_None) 
	{
		return true;
	}
	if ((g_GlobalOption.m_InviteMode==enInviteMode_Friend)
		&&(pUserData->cbCompanion!=enCompanion_Friend))
	{
		return true;
	}


	//��ʾ��Ϣ
	TCHAR szMessage[256]=TEXT("");
	_sntprintf(szMessage,sizeof(szMessage),TEXT("[ %s ] ����������� [ %ld ] ��Ϸ��������Ϸ���Ƿ�ͬ�⣿"),pUserData->szName,pUserInvite->wTableID+1);
	if (ShowMessageBox(szMessage,MB_ICONINFORMATION|MB_YESNO)==IDYES)
	{
		WORD wChairID=INVALID_CHAIR;
		WORD wNullCount=m_TableFrame.GetNullChairCount(pUserInvite->wTableID,wChairID);
		if (wNullCount==0) 
		{
			ShowMessageBox(TEXT("����Ϸ���Ѿ�û�п�λ���ˣ�"),MB_ICONINFORMATION);
			return true;
		}
		PerformSitDownAction(pUserInvite->wTableID,wChairID);
	}

	return true;
}
//�����Ŷ�����
bool CRoomViewItem::OnSocketSubQueueCount(CMD_Command Command, void * pBuffer, WORD wDataSize)
{
	//Ч�����
	ASSERT(Command.wMainCmdID==MDM_GR_USER);
	ASSERT(Command.wSubCmdID==SUB_GR_QUEUE_COUNT);

	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GR_AutoQueue));
	if (wDataSize!=sizeof(CMD_GR_AutoQueue)) 
	{
		return false;
	}
	CMD_GR_AutoQueue * pUserInvite=(CMD_GR_AutoQueue *)pBuffer;
	m_QueueNum = pUserInvite->iAutoQueueNum;
	InvalidateRect(m_rcQueue, FALSE);
	return true;
}
//�Զ��Ŷ�ʧ�� SUB_GR_USER_QUEUE_FAIL:
bool CRoomViewItem::OnSocketSubQueueFail(CMD_Command Command, void * pBuffer, WORD wDataSize)
{
	//Ч�����
	ASSERT(Command.wMainCmdID==MDM_GR_USER);
	//ASSERT(Command.wSubCmdID==SUB_GR_USER_QUEUE_FAIL);

	//Ч�����
	//ASSERT(wDataSize==sizeof(CMD_GR_SendWarning));
	//if (wDataSize!=sizeof(CMD_GR_SendWarning)) 
	//{
	//	return false;
	//}

	CMD_GR_SendWarning * pUserInvite=(CMD_GR_SendWarning *)pBuffer;
	if (g_GlobalUnits.GetGolbalUserData().dwUserID == pUserInvite->dwTargetUserID)
	{
		CString str = pUserInvite->szWarningMessage;
		ShowMessageBox(str, MB_OK);

		GetParent()->ShowWindow(SW_SHOW);
		GetParent()->PostMessage(WM_COMMAND,IDM_CALL_GAME_LIST,0);
		CloseRoomViewItem();
	}
	return true;
}

//IPC �ں�����
bool CRoomViewItem::OnIPCKernel(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
	ASSERT(pHead->wMainCmdID==IPC_MIAN_IPC_KERNEL);
	switch (pHead->wSubCmdID)
	{
	case IPC_SUB_IPC_CLIENT_CONNECT:	//������Ϣ
		{
			//�ж�����
			if (::IsChild(m_pShareMemory->hWndGameFrame,hWndSend))
			{
				return false;
			}


			//���ñ���
			ASSERT(m_hWndChannel==NULL);
			m_hWndChannel=hWndSend;

			//������Ϣ
			CIPCSendCopyData SendCopyData(m_hWndChannel,m_hWnd);
			IPCSendGameInfo(&SendCopyData);
			IPCSendTableUsers(&SendCopyData);
			SendCopyData.SendData(IPC_MAIN_CONCTROL,IPC_SUB_START_FINISH);

			//��������
			if ((m_wPacketTableID!=INVALID_TABLE)
				&&(m_wPacketChairID!=INVALID_CHAIR))
			{
				//��������
				tagDataHead DataHead;
				ZeroMemory(&DataHead,sizeof(DataHead));

				//��������
				BYTE cbBuffer[SOCKET_BUFFER];
				IPC_SocketPackage SocketPackage;
				ZeroMemory(cbBuffer,sizeof(cbBuffer));
				ZeroMemory(&SocketPackage,sizeof(SocketPackage));

				//��ȡ����
				do
				{
					//��ȡ����
					tagBurthenInfo BurthenInfo;
					m_PacketDataStorage.GetBurthenInfo(BurthenInfo);

					//��ȡ����
					if (BurthenInfo.dwDataPacketCount==0L)
					{
						break;
					}
					if (m_PacketDataStorage.DistillData(DataHead,cbBuffer,sizeof(cbBuffer))==false)
					{
						break;
					}

					//��������
					CopyMemory(&SocketPackage.Command,cbBuffer,sizeof(CMD_Command));
					CopyMemory(SocketPackage.cbBuffer,&cbBuffer[sizeof(CMD_Command)],DataHead.wDataSize-sizeof(CMD_Command));

					//���ݴ���
					SendProcessData(IPC_MAIN_SOCKET,IPC_SUB_SOCKET_RECV,&SocketPackage,DataHead.wDataSize);

				} while (true);

				//���ñ���
				m_wPacketTableID=INVALID_TABLE;
				m_wPacketChairID=INVALID_CHAIR;
			}

			//��ʼ�����Ϸ����Ƿ����
			m_tagGameFrameStatus.bIsOpen = true;
			m_tagGameFrameStatus.byStatus = 1;
			SendCopyData.SendData(IPC_MAIN_CONCTROL,IPC_SUB_GAMEFRAME_EXIST);
			SetTimer(IDI_GAME_FRAME_IS_EXIST, TIMER_GAMEFRAME_EXIST, NULL);
			CPlazaDlg * plazaDlg = ((CPlazaDlg*)GetParent());
			plazaDlg->UpdateMusicSound(false);
			SendToGameFrameAfficheInfo();
			/*
			//���͵ȼ��б�
			WORD wGradeSize=plazaDlg->m_iGradeInfoDiv;
			CMD_GP_UserGradeInfo UserGradeInfo[128]={0};
			::memcpy(UserGradeInfo,plazaDlg->m_pGradeInfo,sizeof(CMD_GP_UserGradeInfo)*wGradeSize);
			SendCopyData.SendData(IPC_MAIN_USER,IPC_SUB_GAME_GRADE_LIST,UserGradeInfo,sizeof(CMD_GP_UserGradeInfo)*128);
			*/
			/*			//��������
			IPC_SocketPackage SocketPackage;
			ZeroMemory(&SocketPackage,sizeof(SocketPackage));
			WORD wSendSize=0;


			//������Ϣ
			SocketPackage.Command.wMainCmdID = MDM_GF_PRESENT;
			SocketPackage.Command.wSubCmdID = SUB_GF_PROP_ATTRIBUTE;

			//��������
			for (INT_PTR nIndex =0; nIndex < m_PropertyAttributeArrary.GetCount(); ++nIndex)
			{
			if ((wSendSize+sizeof(tagPropertyInfo))>SOCKET_PACKET)
			{
			SendProcessData(IPC_MAIN_SOCKET,IPC_SUB_SOCKET_RECV,&SocketPackage,wSendSize+sizeof(CMD_Command));
			wSendSize=0;
			}
			tagPropertyInfo &PropertyInfo = m_PropertyAttributeArrary[nIndex];
			CopyMemory(SocketPackage.cbBuffer+wSendSize,&PropertyInfo,sizeof(PropertyInfo));
			wSendSize+=sizeof(PropertyInfo);
			}

			if (wSendSize>0) 
			{
			SendProcessData(IPC_MAIN_SOCKET,IPC_SUB_SOCKET_RECV,&SocketPackage,wSendSize+sizeof(CMD_Command));
			}


			//������Ϣ
			SocketPackage.Command.wSubCmdID = SUB_GF_FLOWER_ATTRIBUTE;

			//�ʻ�����
			wSendSize=0L;
			for (INT_PTR nIndex =0; nIndex < m_FlowerAttributeArrary.GetCount(); ++nIndex)
			{
			if ((wSendSize+sizeof(tagFlowerInfo))>SOCKET_PACKET)
			{
			SendProcessData(IPC_MAIN_SOCKET,IPC_SUB_SOCKET_RECV,&SocketPackage,wSendSize+sizeof(CMD_Command));
			wSendSize=0;
			}
			tagFlowerInfo &FlowerInfo = m_FlowerAttributeArrary[nIndex];
			CopyMemory(SocketPackage.cbBuffer+wSendSize,&FlowerInfo,sizeof(FlowerInfo));
			wSendSize+=sizeof(FlowerInfo);
			}
			if (wSendSize>0)
			{
			SendProcessData(IPC_MAIN_SOCKET,IPC_SUB_SOCKET_RECV,&SocketPackage,wSendSize+sizeof(CMD_Command));
			}
			*/


			return true;
		}
	case IPC_SUB_IPC_CLIENT_CLOSE:		//�ر���Ϣ
		{
			//�ж�����
			if (hWndSend!=m_hWndChannel)
			{
				return false;
			}

			//������Ϣ
			tagUserData * pUserData=m_pMeUserItem->GetUserData();
			if (pUserData->wTableID!=WORD(INVALID_TABLE))
			{
				SendStandUpPacket();
			}
			else
			{
				//ȷ�����Լ��ر�����Ϸ
				if (pUserData->dwUserID == g_GlobalUnits.GetGolbalUserData().dwUserID)
				{
					SendCancelQueue();
				}
			}

			((CPlazaDlg*)GetParent())->UpdateMusicSound(true);

			//���ñ���
			m_hWndChannel=NULL;
			m_pShareMemory->hWndGameFrame=NULL;
			CloseHandle(m_GameProcessInfo.hThread);
			CloseHandle(m_GameProcessInfo.hProcess);
			memset(&m_GameProcessInfo,0,sizeof(m_GameProcessInfo));
			::memset(&m_tagGameFrameStatus, 0, sizeof(m_tagGameFrameStatus));
			HINSTANCE hInstance=AfxGetInstanceHandle();
			//cxf 1108 m_AutoQueue.LoadRgnImage(hInstance,IDB_AUTO_QUEUE);
			//cxf 1108 m_ToolTipCtrl.AddTool(&m_AutoQueue,TEXT("�Զ��Ŷ�"));
			//cxf 1108 m_AutoQueue.EnableWindow(TRUE);
			//cxf 1108 m_AutoQueue.Invalidate(TRUE);

			return true;
		}

	}
	return true;
}

//IPC ��������
bool CRoomViewItem::OnIPCSocket(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
	ASSERT(pHead->wMainCmdID==IPC_MAIN_SOCKET);
	switch (pHead->wSubCmdID)
	{
	case IPC_SUB_SOCKET_SEND:	//������Ϣ
		{
			//Ч������
			ASSERT(wDataSize>=sizeof(CMD_Command));
			if (wDataSize<sizeof(CMD_Command)) 
			{
				return true;
			}

			//��������
			IPC_SocketPackage * pSocketPackage=(IPC_SocketPackage *)pIPCBuffer;
			CMD_Command *pCommand= &pSocketPackage->Command;
			WORD wSendSize=wDataSize-sizeof(CMD_Command);


			if (wSendSize==0) 
			{
				m_ClientSocket->SendData(pCommand->wMainCmdID,
					pCommand->wSubCmdID);
			}
			else 
			{
				m_ClientSocket->SendData(pCommand->wMainCmdID,pCommand->wSubCmdID,pSocketPackage->cbBuffer,wSendSize);
			}
			return true;
		}
	}

	return false;
}

//�ŵ����ݴ���
bool __cdecl CRoomViewItem::OnChannelMessage(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend)
{
	switch (pHead->wMainCmdID)
	{
	case IPC_MIAN_IPC_KERNEL:		//�ں���Ϣ
		{
			return OnIPCKernel(pHead,pIPCBuffer,wDataSize,hWndSend);
		}
	case IPC_MAIN_SOCKET:			//�����¼�
		{
			return OnIPCSocket(pHead,pIPCBuffer,wDataSize,hWndSend);
		}
	case IPC_MAIN_CONCTROL:			//������Ϣ
		{
			/*if (pHead->wSubCmdID != IPC_SUB_GAMEFRAME_EXIST)
			{
			TCHAR aa[16]={""};
			_snprintf(aa,sizeof(aa),"%d---%d",pHead->wMainCmdID,pHead->wSubCmdID);
			AfxMessageBox(aa);
			}*/

			switch(pHead->wSubCmdID)
			{
			case IPC_SUB_GAMEFRAME_EXIST:	//��Ϸ��ܻ�����
				{
					m_tagGameFrameStatus.byStatus = 2;
					break;
				}
			case IPC_SUB_GAME_FRAME_CLOSED:	//��Ϸ��ܹر���Ϣ
				{
					KillTimer(IDI_GAME_FRAME_IS_EXIST);
					m_tagGameFrameStatus.bIsOpen = false;

					GetParent()->SetForegroundWindow();
					((CPlazaDlg*)GetParent())->UpdateMusicSound(true);

					if (ServiceStatus_NetShutDown == m_ServiceStatus)
					{
						GetParent()->ShowWindow(SW_SHOW);
						GetParent()->PostMessage(WM_COMMAND,IDM_CALL_ROOM_LIST,0);
						break;
					}

					GetParent()->PostMessage(WM_COMMAND, IDB_CALL_UPDATE_USERIMAGE, 0);
					SendRequestCloseRoomMsg(1);

					/*cxf 1022 KillTimer(IDI_GAME_FRAME_IS_EXIST);
					m_tagGameFrameStatus.bIsOpen = false;
					//��ʾ����
					m_bQueueState = true;
					HINSTANCE hInstance=AfxGetInstanceHandle();
					m_AutoQueue.LoadRgnImage(hInstance,IDB_AUTO_QUEUE);
					m_ToolTipCtrl.AddTool(&m_AutoQueue,TEXT("�Զ��Ŷ�"));
					m_AutoQueue.EnableWindow(TRUE);
					m_AutoQueue.Invalidate(TRUE);

					GetParent()->SetForegroundWindow();
					GetParent()->ShowWindow(SW_SHOW);
					ShowWindow(SW_SHOW);
					GetParent()->PostMessage(WM_COMMAND, IDB_CALL_UPDATE_USERIMAGE, 0);

					((CPlazaDlg*)GetParent())->UpdateMusicSound(true);
					*/
					break;
				}
			case IPC_SUB_JOIN_IN_GAME:	//������Ϸ
				{
					IPC_JoinInGame * pJoinInGame=(IPC_JoinInGame *)pIPCBuffer;
					if ((pJoinInGame->wTableID!=INVALID_TABLE)&&(pJoinInGame->wChairID!=INVALID_CHAIR))
					{
						PerformSitDownAction(pJoinInGame->wTableID,pJoinInGame->wChairID);
					}
					break;
				}
			case IPC_SUB_GO_PLAZA:		//���ش���(��Ϸ�б�)
				{
					KillTimer(IDI_GAME_FRAME_IS_EXIST);
					m_tagGameFrameStatus.bIsOpen = false;
					SendRequestCloseRoomMsg(2);

					((CPlazaDlg*)GetParent())->UpdateMusicSound(true);
					break;
				}
			case IPC_SUB_GO_ROOM:		//���ط����б�
				{
					KillTimer(IDI_GAME_FRAME_IS_EXIST);
					m_tagGameFrameStatus.bIsOpen = false;
					SendRequestCloseRoomMsg(1);

					((CPlazaDlg*)GetParent())->UpdateMusicSound(true);
					break;
				}
			case IPC_SUB_USER_UPDATE_DJINFO:	//���µ�����Ϣ
				{
					IPC_UserUpdateDJInfo* pDJInfo = (IPC_UserUpdateDJInfo*)pIPCBuffer;
					if (NULL != pDJInfo)
					{
						UpdateUserDJInfo(pDJInfo->iDJID, false);
					}
					break;
				}
			case IPC_SUB_SENDACTIONINFO:		//���Ͷ���
				{
					//Ч�����
					ASSERT(wDataSize>=sizeof(IPC_UserActionInfo));
					if (wDataSize<sizeof(IPC_UserActionInfo)) return false;

					IPC_UserActionInfo* pAction = (IPC_UserActionInfo*)pIPCBuffer;
					if (NULL != pAction)
					{//���Ͷ�����Ϣ
						m_ClientSocket->SendData(MDM_GR_USER,SUB_SENDACTIONINFO,pAction,sizeof(IPC_UserActionInfo));
					}
					break;
				}
			case IPC_SUB_SENDLANLISTINFO:		//������Ϣ
				{
					//AfxMessageBox("e");
					//Ч�����
					ASSERT(wDataSize>=sizeof(IPC_UserLanguageInfo));
					if (wDataSize<sizeof(IPC_UserLanguageInfo)) return false;

					IPC_UserLanguageInfo* pLInfo = (IPC_UserLanguageInfo*)pIPCBuffer;
					if (NULL != pLInfo)
					{//���Ͷ�����Ϣ
						m_ClientSocket->SendData(MDM_GR_USER,SUB_SEND_SOUND_MESS,pLInfo,sizeof(IPC_UserLanguageInfo));
					}
					break;
				}
			case IPC_SUB_CMD_CONNECTWEB:		//����վ
				{
					if (wDataSize < sizeof(IPC_ConnWeb)) return false;
					IPC_ConnWeb * pConnWeb = (IPC_ConnWeb * ) pIPCBuffer;
					if (NULL != pConnWeb)
					{
						tagGlobalUserData UserData=g_GlobalUnits.GetGolbalUserData();//�û���Ϣ
						CString strUrl("");
						
						switch (pConnWeb->byConWebTag)
						{
						case 1:	//�ٷ���վ
							{
								strUrl.Format(CInitData::s_cLoginHomePage, UserData.dwUserID, UserData.szKey);
								break;
							}
						case 2:	//�����̳�
							{
								strUrl.Format(CInitData::s_cLoginPropShopOpen, UserData.dwUserID, UserData.szKey);
								break;
							}
						case 3:	//�ͷ�����
							{
								strUrl.Format(CInitData::s_cClientCenter);
								break;
							}
						}
						
						ShellExecute(NULL,"open",strUrl,"",NULL,SW_SHOW);
					}
					break;
				}

			default:
				break;
			}
			return true;
		}
	}
	return false;
}

//��������رշ�����Ϣ
void CRoomViewItem::SendRequestCloseRoomMsg(BYTE btTag)
{
	//0 == m_btExitTag-->ֱ�ӷ��ص���¼����)
	//1 == m_btExitTag-->�������б��ص������б�
	//2 == m_btExitTag-->���ص���Ϸ����
	//3 == m_btExitTag-->�˳�������Ϸ

	//ɾ��������ʱ��
	KillTimer(IDI_LOADING_ANIMACTION);

	m_btExitTag = btTag;

	if ((ServiceStatus_NetShutDown == m_ServiceStatus) ||
		(m_ServiceStatus < ServiceStatus_RecvConfigInfo) || (m_bRequestServerExit))
	{//���SOCKET�Ǵ��������ж�״̬����û��״̬����ֱ��ִ��״̬��

		m_bRequestServerExit = false;
		KillTimer(IDI_REQUEST_SERVER_EXIT_MSG);
		switch(m_btExitTag)
		{
		case 0:	//ֱ�ӷ��ص���¼����
			{
				//�رշ���
				GetParent()->PostMessage(WM_COMMAND,IDM_CALL_EXIT_GAME,0);
				CloseRoomViewItem();
			}
			break;
		case 1:	//�������б��ص������б�
			{
				GetParent()->ShowWindow(SW_SHOW);
				GetParent()->PostMessage(WM_COMMAND,IDM_CALL_ROOM_LIST,0);
				CloseRoomViewItem();
			}
			break;
		case 2:	//���ص���Ϸ����
			{
				GetParent()->ShowWindow(SW_SHOW);
				GetParent()->PostMessage(WM_COMMAND,IDM_CALL_GAME_LIST,0);
				CloseRoomViewItem();
			}
			break;
		case 3:	//�˳�������Ϸ
			{
				GetParent()->PostMessage(WM_COMMAND,IDM_CALL_CANCEL_GAME,0);
			}
			break;
		default:
			break;
		}
	}
	else
	{
		//������Ϣ
		tagUserData * pUserData=m_pMeUserItem->GetUserData();
		if (pUserData->wTableID!=WORD(INVALID_TABLE))
		{
			if (pUserData->cbUserStatus==US_PLAY) 
			{
				SendLeftGamePacket();
			}
		}
		//�˳����͸���������
		m_ClientSocket->SendData(MDM_GR_USER,SUB_GF_QUIT_GAME_SERVER);

		//���ö�ʱ��
		m_bRequestServerExit = false;
		SetTimer(IDI_REQUEST_SERVER_EXIT_MSG, TIMER_REQUEST_SERVER_INTER, NULL);
	}

}

//����������
void __cdecl CRoomViewItem::OnLButtonHitTable(WORD wTableID, WORD wChairID, bool bMirror)
{
	//�ж�״̬
	if (m_ServiceStatus==ServiceStatus_NetShutDown)
	{
		if ((wTableID!=INVALID_TABLE)&&(wChairID!=INVALID_CHAIR))
		{
			LPCTSTR szMessage=TEXT("����Ϸ�������������Ѿ��жϣ��Ƿ��˳���ǰ��Ϸ���䣿");
			int iCode=ShowMessageBox(szMessage,MB_ICONINFORMATION|MB_YESNO);
			//if (iCode==IDYES) ((CGameFrame*)AfxGetMainWnd())->CloseRoomViewItem(this);
			GetParent()->ShowWindow(SW_SHOW);
			GetParent()->PostMessage(WM_COMMAND,IDM_CALL_GAME_LIST,0);
			CloseRoomViewItem();
		}
		return;
	}

	//��������
	if (m_ServiceStatus!=ServiceStatus_Serviceing) 
	{
		return;
	}
	if ((wTableID==m_wReqTableID)&&(wChairID==m_wReqChairID)) 
	{
		return;
	}
	if ((wTableID==INVALID_TABLE)||(wChairID==INVALID_CHAIR)) 
	{
		return;
	}
	//��Ϣ����
	IUserItem * pTableUserItem=m_TableFrame.GetUserInfo(wTableID,wChairID);
	if (pTableUserItem!=NULL)
	{
		//�Թۻ�������
		if (pTableUserItem==m_pMeUserItem)
		{
			if (m_pMeUserItem->GetUserData()->cbUserStatus>=US_PLAY) 
			{
				return;
			}
			if ((m_wReqTableID!=INVALID_TABLE)||(m_wReqChairID!=INVALID_CHAIR)) 
			{
				return;
			}
			m_wFindTableID=INVALID_TABLE;
			SendStandUpPacket();
		}
		else 
		{
			tagUserData * pUserData=m_pMeUserItem->GetUserData();
			if((pUserData->wTableID==wTableID)
				&& (pUserData->wChairID==wChairID)) 
			{
				return;
			}
			PerformLookonAction(wTableID,wChairID);
		}
	}
	else 
	{
		//���´���
		if (m_cbHideUserInfo==TRUE)
		{
			SendSitDownPacket(INVALID_TABLE,INVALID_CHAIR,NULL);
		}
		else
		{
			PerformSitDownAction(wTableID, wChairID);
		}
	}

	return;
}

//����Ҽ�����
void __cdecl CRoomViewItem::OnRButtonHitTable(WORD wTableID, 
											  WORD wChairID, 
											  bool bMirror)
{
	//��ȡ����
	ITableView * pITableView=m_TableFrame.GetTableArrayPtr(wTableID);
	if (pITableView==NULL) 
	{
		return;
	}

	//��ȡ�û�
	IUserItem * pIUserItem=pITableView->GetUserInfo(wChairID);
	if (pIUserItem==NULL) 
	{
		return;
	}

	//��ʾ�˵�
	CPoint Point;
	GetCursorPos(&Point);
	ShowUserInfoMenu(pIUserItem,Point);

	return;
}

//���˫��
void __cdecl CRoomViewItem::OnDoubleHitTable(WORD wTableID, bool bMirror)
{
}

//�û�����
void __cdecl CRoomViewItem::OnUserItemAcitve(IUserItem * pIUserItem)
{
	//m_UserListView.InsertUserItem(pIUserItem);
	return ;
}

//�û�����
void __cdecl CRoomViewItem::OnUserItemUpdate(IUserItem * pIUserItem)
{
	//m_UserListView.UpdateUserItem(pIUserItem);
	if (m_pFindUserDlg!=NULL) 
	{
		m_pFindUserDlg->UpdateUserItem(pIUserItem);
	}

	return ;
}

//�û�ɾ��
void __cdecl CRoomViewItem::OnUserItemDelete(IUserItem * pIUserItem)
{
	//��ȡ�û�
	ASSERT(pIUserItem!=NULL);
	DWORD dwUserID=pIUserItem->GetUserID();

	//�������
	if (m_dwChatUserID==dwUserID)
	{
		SetChatObject(NULL);
		TCHAR szMessage[256]=TEXT("");
		_sntprintf(szMessage,sizeof(szMessage),TEXT("[ %s ] �뿪�ˣ������������Ϊ������"),pIUserItem->GetUserName());
		m_MessageProxyHelper->InsertSystemString(szMessage,0);
	}

	//�������
	// 	int nChatObjectCount=m_ChatObject.GetCount();
	// 	for (int i=1;i<nChatObjectCount;i++)
	// 	{
	// 		if (m_ChatObject.GetItemData(i)==dwUserID)
	// 		{
	// 			if (m_ChatObject.GetCurSel()==i) 
	// 			{
	// 				m_dwChatUserID=0;
	// 				m_szChatName[0]=0;
	// 				m_ChatObject.SetCurSel(0);
	// 			}
	// 			m_ChatObject.DeleteString(i);
	// 			break;
	// 		}
	// 	}

	//������ͼ
	//m_UserListView.DeleteUserItem(pIUserItem);

	if (m_pFindUserDlg!=NULL)
	{
		m_pFindUserDlg->DeleteUserItem(pIUserItem);
	}
	return;
}

//������Ϣ����
CShortMessage * CRoomViewItem::ActiveShortWnd(long int dwUserID, IUserItem * pUserItem, bool bCreate)
{
	/*
	//��������
	INT_PTR iCloseIndex=-1,iHideIndex=-1;

	//Ѱ�����ϴ���
	for (INT_PTR i=0;i<m_ShortMessage.GetCount();i++)
	{
	CShortMessage * pShortMessageWnd=(CShortMessage *)m_ShortMessage.GetAt(i);
	ASSERT(pShortMessageWnd!=NULL);
	if (dwUserID==pShortMessageWnd->m_dwTargetUserID)
	{
	if (pShortMessageWnd->GetSafeHwnd()==NULL)
	{
	AfxSetResourceHandle(GetModuleHandle(NULL));
	pShortMessageWnd->Create(IDD_SHORT_MESSAGE,GetDesktopWindow());
	}
	pShortMessageWnd->ShowWindow(SW_SHOW);
	return pShortMessageWnd;
	}
	if (bCreate==true)
	{
	if ((iCloseIndex==-1)
	&&  (pShortMessageWnd->GetSafeHwnd()==NULL)) 
	{
	iCloseIndex=i;
	}
	if ((iHideIndex==-1)
	&& (pShortMessageWnd->GetSafeHwnd()!=NULL)
	&& (pShortMessageWnd->IsWindowVisible()==FALSE)) 
	{
	iHideIndex=i;
	}
	}
	}

	//ʹ�ùرյĴ���
	if ((bCreate==true)&&(iCloseIndex>=0))
	{
	CShortMessage *pShortMessageWnd = (CShortMessage *)m_ShortMessage.GetAt(iCloseIndex);
	ASSERT(pShortMessageWnd!=NULL);
	if (pShortMessageWnd->GetSafeHwnd()==NULL)
	{
	AfxSetResourceHandle(GetModuleHandle(NULL));
	pShortMessageWnd->Create(IDD_SHORT_MESSAGE,GetDesktopWindow());
	}
	pShortMessageWnd->SetTalkInfo(pUserItem,m_pListServer->GetItemInfo()->szServerName);
	pShortMessageWnd->ShowWindow(SW_SHOW);
	//�ö�����
	pShortMessageWnd->SetActiveWindow();
	pShortMessageWnd->BringWindowToTop();
	return pShortMessageWnd;
	}

	//ʹ�����ش���
	if ((bCreate==true)&&(iHideIndex>=0))
	{
	CShortMessage * pShortMessageWnd=(CShortMessage *)m_ShortMessage.GetAt(iHideIndex);
	ASSERT(pShortMessageWnd!=NULL);
	pShortMessageWnd->SetTalkInfo(pUserItem,m_pListServer->GetItemInfo()->szServerName);
	pShortMessageWnd->ShowWindow(SW_SHOW);
	//�ö�����
	pShortMessageWnd->SetActiveWindow();
	pShortMessageWnd->BringWindowToTop();
	return pShortMessageWnd;
	}

	//�����´���
	if ((bCreate==true)&&(m_ShortMessage.GetCount()<10))
	{
	try
	{
	CShortMessage * pShortMessageWnd=new CShortMessage(this);
	AfxSetResourceHandle(GetModuleHandle(NULL));
	pShortMessageWnd->SetTalkInfo(pUserItem,m_pListServer->GetItemInfo()->szServerName);
	pShortMessageWnd->Create(IDD_SHORT_MESSAGE,GetDesktopWindow());
	pShortMessageWnd->ShowWindow(SW_SHOW);
	//�ö�����
	pShortMessageWnd->SetActiveWindow();
	pShortMessageWnd->BringWindowToTop();

	m_ShortMessage.Add(pShortMessageWnd);

	return pShortMessageWnd;
	}
	catch (...) 
	{ 
	return NULL; 
	}
	}
	*/

	return NULL;
}

//�������
bool CRoomViewItem::SetChatObject(IUserItem * pIUserItem)
{
	if (pIUserItem!=NULL)
	{
		//���ñ���
		tagUserData * pUserData=pIUserItem->GetUserData();
		m_dwChatUserID=pUserData->dwUserID;
		lstrcpyn(m_szChatName,pUserData->szName,CountArray(m_szChatName));

		//Ѱ�����
		int nItemCount=m_ChatObject.GetCount();
		for (int i=0;i<nItemCount;i++)
		{
			if (m_ChatObject.GetItemData(i)==m_dwChatUserID)
			{
				m_ChatObject.SetCurSel(i);
				return true;
			}
		}

		//�������
		int iItem=m_ChatObject.AddString(m_szChatName);
		m_ChatObject.SetItemData(iItem,m_dwChatUserID);
		m_ChatObject.SetCurSel(iItem);
	}
	else
	{
		//���ñ���
		m_dwChatUserID=0L;
		m_szChatName[0]=0;

		//���ý���
		m_ChatObject.SetCurSel(0);
	}

	return true;
}


void CRoomViewItem::OnBnClickedReLogon()
{
	// 	tagGlobalUserData & GlobalUserData=g_GlobalUnits.GetGolbalUserData();
	// 	//ɾ����¼
	// 	g_GlobalUnits.DeleteUserCookie();
	// 	//���ñ���
	// 	memset(&GlobalUserData,0,sizeof(GlobalUserData));
	// 	//Ͷ����Ϣ
	// 	GetParent()->PostMessage(WM_COMMAND,IDM_LOGON_PLAZA,0);
	CloseRoomViewItem();
}

void CRoomViewItem::OnLButtonDown( UINT nFlags, CPoint point )
{	

	if (IsAdministrationHavePurview())
	{
		m_TableFrame.OnLButtonDown(nFlags, point);
	}

	if (m_rcCloseRect.PtInRect(point))
	{//���¹رհ�ť
		m_wCloseState=1;
		InvalidateRect(&m_rcCloseRect,FALSE);
		return;
	}
	GetParent()->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));

	CDialog::OnLButtonDown(nFlags, point );
}

//�����Ϣ
void CRoomViewItem::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (IsAdministrationHavePurview())
	{	
		m_TableFrame.OnLButtonDblClk(nFlags, point);
	}

	CDialog::OnLButtonDblClk(nFlags, point );
}
//�����Ϣ
void CRoomViewItem::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (IsAdministrationHavePurview())
	{
		m_TableFrame.OnLButtonUp(nFlags, point);
	}

	if (m_rcCloseRect.PtInRect(point))
	{
		//�رմ���
		OnTableClose();
		return;
	}

	CDialog::OnLButtonUp(nFlags, point );
}
//�����Ϣ
void CRoomViewItem::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (IsAdministrationHavePurview())
	{
		m_TableFrame.OnRButtonDown(nFlags, point);
	}
	CDialog::OnRButtonDown(nFlags, point );
}
//�����Ϣ
void CRoomViewItem::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (IsAdministrationHavePurview())
	{
		m_TableFrame.OnRButtonUp(nFlags, point);
	}
	CDialog::OnRButtonUp(nFlags, point );
}
//�����Ϣ
BOOL CRoomViewItem::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	if (IsAdministrationHavePurview())
	{
		if (m_TableFrame.OnSetCursor(pWnd, nHitTest, uMessage))
			return TRUE;
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, uMessage);
}
void CRoomViewItem::OnDestroy()
{
	CDialog::OnDestroy();
	
	m_ImgAnimAction.DestroyImage();
	m_ImgClose.DestroyImage();
	m_pngNumber.DestroyImage();
	m_pngText1.DestroyImage();
	m_pngText2.DestroyImage();
	m_pngImageHou.DestroyImage();
	m_pngImageCurSort.DestroyImage();
	m_pngImageDayWin.DestroyImage();
	m_pngGameICO.DestroyImage();
	m_ImageBack.DestroyImage();
	//m_ImageBackGM.DestroyImage();

	ClearMemDC();
}
//��վ��
void CRoomViewItem::ClearMemDC()
{
	if (m_memdc.m_hDC)
		m_memdc.DeleteDC();
	m_memdc.m_hDC = NULL;
}

//ȡ������
void CRoomViewItem::OnCommandCancelConnect()
{
	g_GlobalAttemper.DestroyStatusWnd(this);

	if(m_ServiceStatus < ServiceStatus_RecvConfigInfo)
	{
		//�˳����͸���������
		m_ClientSocket->SendData(MDM_GR_USER,SUB_GF_QUIT_GAME_SERVER);

		OnBnReturn();
	}
	else
	{
		GetParent()->ShowWindow(SW_SHOW);
		GetParent()->PostMessage(WM_COMMAND,IDM_CALL_ROOM_LIST,0);
		CloseRoomViewItem();
	}
	return;
}


//ʱ����Ϣ
void CRoomViewItem::OnTimer(UINT nIDEvent)
{
	CDialog::OnTimer(nIDEvent);
	switch(nIDEvent)
	{
	case IDI_GAME_FRAME_IS_EXIST:		//�����Ϸ����Ƿ����
		{
			if (m_tagGameFrameStatus.bIsOpen)
			{//�����Ѿ��򿪹���.
				if (::IsWindow(m_hWndChannel) && 2==m_tagGameFrameStatus.byStatus)
				{//����
					m_tagGameFrameStatus.byStatus = 1;
					CIPCSendCopyData SendCopyData(m_hWndChannel,m_hWnd);
					SendCopyData.SendData(IPC_MAIN_CONCTROL,IPC_SUB_GAMEFRAME_EXIST);
				}
				else
				{//������,�ر���Ϸ��ܣ����˻ص���Ϸ�б�.
					KillTimer(IDI_GAME_FRAME_IS_EXIST);
					m_bRequestServerExit = true;
					/*GetParent()->SetForegroundWindow();
					GetParent()->ShowWindow(SW_SHOW);
					SendRequestCloseRoomMsg(2);*/

					((CPlazaDlg*)GetParent())->UpdateMusicSound(true);
					m_ClientSocket->CloseSocket();
					m_ServiceStatus=ServiceStatus_NetShutDown;
					//�رշ���
					GetParent()->PostMessage(WM_COMMAND,IDM_CALL_ROOM_LIST,0);
					CloseRoomViewItem();
				}
			}
			else
			{
				m_bQueueState = true;
				//cxf 1108 HINSTANCE hInstance=AfxGetInstanceHandle();
				//cxf 1108 m_AutoQueue.LoadRgnImage(hInstance,IDB_AUTO_QUEUE);
				//cxf 1108 m_ToolTipCtrl.AddTool(&m_AutoQueue,TEXT("�Զ��Ŷ�"));
				//cxf 1108 m_AutoQueue.EnableWindow(TRUE);
				//cxf 1108 m_AutoQueue.Invalidate(TRUE);

				KillTimer(IDI_GAME_FRAME_IS_EXIST);
				GetParent()->SetForegroundWindow();
				GetParent()->ShowWindow(SW_SHOW);
				ShowWindow(SW_SHOW);

				((CPlazaDlg*)GetParent())->UpdateMusicSound(true);
			}
			break;
		}
	case IDI_REQUEST_SERVER_EXIT_MSG:		//�����������Ϸ�ͻ����˳�
		{
			KillTimer(IDI_REQUEST_SERVER_EXIT_MSG);
			m_bRequestServerExit = true;
			break;
		}
	case IDI_LOADING_ANIMACTION://Loading����
		{
			//����֡������Ϊ12֡��
			if (m_wAnimAction>=11)
			{
				m_wAnimAction=0;
			}
			else
			{
				m_wAnimAction++;
			}
			InvalidateRect(&m_rcAnimAction);
		}
	default:
		break;
	}
}


//���͹�����Ϣ
void CRoomViewItem::SendToGameFrameAfficheInfo()
{
	if (::IsWindow(m_hWndChannel) && m_tagGameFrameStatus.bIsOpen && (m_strSaveGameAffiche.size() > 0))
	{
		//������Ϣ
		CIPCSendCopyData IPCSendCopyData(m_hWndChannel,m_hWnd);
		IPCSendCopyData.SendData(IPC_MAIN_CONFIG,IPC_SUB_GAMEAFFICHE_INFO,(char*)m_strSaveGameAffiche.c_str(),m_strSaveGameAffiche.size());
	}
}
//����ƶ�
void CRoomViewItem::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_rcCloseRect.PtInRect(point))
	{//�ƶ��رհ�ť��
		if (2 != m_wCloseState)
		{
			m_wCloseState=2;
			InvalidateRect(&m_rcCloseRect,FALSE);
		}
	}
	else
	{//�ƿ��رհ�ť��
		if (0 != m_wCloseState)
		{
			m_wCloseState=0;
			InvalidateRect(&m_rcCloseRect,FALSE);
		}
	}

	CDialog::OnMouseMove(nFlags, point);
}

//���µ�����Ϣ
void CRoomViewItem::UpdateUserDJInfo(int iDJID, bool bIsRoom)
{	
	//��������
	//CMD_GF_User_Upate_Prop userdjinfo;
	//userdjinfo.cbSendLocation = LOCATION_PLAZA_ROOM;
	//userdjinfo.nPropertyID = iDJID;
	//m_ClientSocket->SendData(MDM_GF_PRESENT,SUB_GF_USER_UPDATE_PROP,&userdjinfo,sizeof(CMD_GF_User_Upate_Prop));
}

//��ǰ·��
void CRoomViewItem::GetCurrentDirectoryPath(CString &szPath)
{
	szPath="";
	TCHAR szPathTemp[MAX_PATH]={0};
	GetCurrentDirectory(MAX_PATH, szPathTemp);
	szPath.Append(szPathTemp);
}

//������Դ·��
void CRoomViewItem::GetImagePlazaPath(CString& strRetValue)
{
	CString strModuleFileName("");
	GetCurrentDirectoryPath(strModuleFileName);
	strModuleFileName.Append("\\data\\plaza\\");
	strRetValue = strModuleFileName;
}

void CRoomViewItem::OnTableClose()
{
	SendRequestCloseRoomMsg(1);
}
//////////////////////////////////////////////////////////////////////////

BOOL CRoomViewItem::DestroyWindow()
{
	// TODO: �ڴ����ר�ô����/����û���

	// ���͹ر���ʾ����¼�
	SetEvent(m_hClosePromtWndr);	
	WaitForSingleObject(m_hThread,  30000);

	CloseHandle(m_hClosePromtWndr);
	m_hClosePromtWndr = NULL;
	return __super::DestroyWindow();
}
