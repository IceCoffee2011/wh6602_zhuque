#include "Stdafx.h"
#include "Resource.h"
#include "GlobalOption.h"
#include "GameFrameControl.h"
#include ".\gameframecontrol.h"
#include "skinsb.h"
#include "MemDC.h"
//////////////////////////////////////////////////////////////////////////

//���ʱ��
#define IDI_BROWER_AD				2000								//��涨ʱ��
#define IDI_SWITCH_USER				2001								//�л���ʱ��
#define TIME_BROWER_AD				15000								//��涨ʱ��
#define TIME_SWITCH_USER			15000								//�л���ʱ��
#define IDI_MOVE_WND				2002								//�ƶ����ڶ�ʱ��
#define TIME_MOVE_WND				20									//�ƶ����ڶ�ʱ��

//�˵�����
#define	IDM_UM_COPY_NAME			WM_USER+100							//��������
#define	IDM_UM_SET_CHAT				WM_USER+101							//��������
#define	IDM_UM_CANECL_CHAT			WM_USER+102							//ȡ������
#define	IDM_UM_ALLOW_LOOKON			WM_USER+103							//�Թۿ���
#define	IDM_UM_ENJOIN_LOOKON		WM_USER+104							//�Թۿ���
#define	IDM_UM_KICK_TABLE_USER		WM_USER+105							//�����û�

#define WND_REMOTE_X	6					// ����Xƫ��
#define WND_REMOTE_Y	7					// ����Yƫ��

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameControl, CRgnDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_SOUND, OnBnClickedSound)
	ON_BN_CLICKED(IDC_LOOKON, OnBnClickedLookon)
	ON_BN_CLICKED(IDC_GAME_RULE, OnBnClickedRule)
	ON_BN_CLICKED(IDC_GAME_QUIT, OnBnClickedQuit)
	ON_BN_CLICKED(IDC_GAME_OPTION, OnBnClickedOption)
	ON_BN_CLICKED(IDC_SEND_CHAT, OnBnClickedSendChat)
	ON_BN_CLICKED(IDC_EXPRESSION, OnBnClickedExpression)
	ON_BN_CLICKED(IDC_COLOR_SET, OnBnClickedColorSet)
	ON_BN_CLICKED(IDC_CLEAN_SCREEN, OnBnClickedCleanScreen)
	ON_BN_CLICKED(IDC_BUGLE,	OnBnClickedBugle)
	ON_MESSAGE(WM_HIT_EXPMESSTION,OnHitExpression)
	ON_NOTIFY(NM_RCLICK, IDC_USER_LIST, OnNMRclickUserList)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_USER_LIST, OnNMclickUserList)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameFrameControl::CGameFrameControl(CGameFrameDlg * pGameFrameDlg) : CRgnDialog(IDD_FRAME_CONTROL), m_pGameFrameDlg(pGameFrameDlg)
{
	//���ñ���
	m_bShowAd=false;
	m_bInitDialog=false;
	m_dwLimitChatTimes=0;

	//�ӿ�ָ��
	m_pUserData=NULL;
	m_pIClientKernel=NULL;


	//��ʼ���ƶ���Ϣ
	m_MoveInfo.Init(0, 0);
	//Ĭ������
	m_bIsUp = true;
	//Ĭ�ϲ�����
	m_bDisableWndMove=false;
	m_iCountMoveIndex=0;				//�ƶ�֡
	m_iCountMoveTotal=0;				//�ƶ���֡��
	return;
}

//��������
CGameFrameControl::~CGameFrameControl()
{
	// ���ٿؼ�
}

//�ӿڲ�ѯ
void * CGameFrameControl::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
#if 0
	QUERYINTERFACE(IPurchaseInfo,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IPurchaseInfo,Guid,dwQueryVer);
#endif

	return NULL;
}

//��������
WORD CGameFrameControl::GetGameGenre()
{
	tagServerAttribute const *pServerAttribute = m_pIClientKernel->GetServerAttribute();
	return pServerAttribute->wGameGenre;
}

//ö�����
tagUserData * CGameFrameControl::EnumLookonUser(WORD wIndex)
{
	return m_pIClientKernel->EnumLookonUser(wIndex);
}

//�����û�
tagUserData * CGameFrameControl::SearchUserItem(DWORD dwUserID)
{
	return m_pIClientKernel->SearchUserItem(dwUserID);
}

//��ȡ���
const tagUserData * CGameFrameControl::GetUserInfo(WORD wChairID)
{
	return m_pIClientKernel->GetUserInfo(wChairID);
}

//��ȡ�Լ�
const tagUserData * CGameFrameControl::GetMeUserInfo()
{
	return m_pIClientKernel->GetMeUserInfo();
}

//������Ϣ
void CGameFrameControl::SendData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	m_pIClientKernel->SendSocketData(wMainCmdID, wSubCmdID, pData, wDataSize);

	return;
}

//�ؼ���
void CGameFrameControl::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_PHRASE, m_btPhrase);
	DDX_Control(pDX, IDC_COLOR_SET, m_btColorSet);
	DDX_Control(pDX, IDC_SEND_CHAT, m_btSendChat);
	DDX_Control(pDX, IDC_EXPRESSION, m_btExpression);
	DDX_Control(pDX, IDC_CLEAN_SCREEN, m_btCleanScreen);
	DDX_Control(pDX, IDC_BUGLE, m_btBugle);
	DDX_Control(pDX, IDC_CHAT_INPUT, m_ChatInput);
	DDX_Control(pDX, IDC_CHAT_MESSAGE, m_ChatMessage);
	DDX_Control(pDX, IDC_USER_LIST, m_UserListView);
	DDX_Control(pDX, IDC_SOUND, m_btSound);
	DDX_Control(pDX, IDC_LOOKON, m_btLookOn);
	DDX_Control(pDX, IDC_GAME_RULE, m_btGameRule);
	DDX_Control(pDX, IDC_GAME_QUIT, m_btGameQuit);
	DDX_Control(pDX, IDC_GAME_OPTION, m_btGameOption);
	DDX_Control(pDX, IDC_CHAT_OBJECT, m_ChatObject);
}

//��ʼ������
BOOL CGameFrameControl::OnInitDialog()
{
	__super::OnInitDialog();

	TCHAR szResourcePath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,szResourcePath);
	CString strPath(szResourcePath);
	strPath.Append("\\data\\gameframe\\plaza\\");

	//����
	m_ImageFrameBackAlpha.LoadImage(TEXT(strPath + "GameFrameControlBackground.png"));
	m_ImageFrameBack.SetLoadInfo(TEXT(strPath + "GameFrameControlBackground.png"));

	//������Ϣ
	m_UserInfoView.Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, 98, NULL);

	m_bmScroll.LoadBitmap(IDB_SCROLL_IMAGE);
	//���ÿؼ�
	//m_UserListView.InitUserListView();
	m_ChatMessage.SetBackgroundColor(FALSE,RGB(0,0,0));
	SkinSB_Init(m_ChatMessage.GetSafeHwnd(), m_bmScroll);

	//������ʾ�ؼ�
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_EXPRESSION),TEXT("����"));
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_COLOR_SET),TEXT("��ɫ"));
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_CLEAN_SCREEN),TEXT("����"));
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_SEND_CHAT),TEXT("����"));
	m_ToolTipCtrl.AddTool(GetDlgItem(IDC_BUGLE),TEXT("С����"));

	//�������
	m_ChatObject.AddString(TEXT("������"));
	m_ChatObject.SetCurSel(0);

	//���ñ���
	m_bInitDialog=true;

	//���ʱ��

	//Ĭ������
	m_bIsUp = true;
	//Ĭ�ϲ�����
	m_bDisableWndMove=false;


	SetWindowPos(&wndTop, 0, 0, m_ImageFrameBackAlpha.GetWidth(), m_ImageFrameBackAlpha.GetHeight()-17, SWP_NOMOVE);
	return TRUE;
}

//���ô������
void CGameFrameControl::SetWndPoint(CRect rcWnd)
{
	m_rcSaveWnd = rcWnd;

	m_iCountMoveIndex=0;				//�ƶ�֡
	m_iCountMoveTotal=10;				//�ƶ���֡��
	m_MoveInfo.Init(m_rcSaveWnd.top, m_rcSaveWnd.top+100);

}

//��Ϣ����
BOOL CGameFrameControl::PreTranslateMessage(MSG * pMsg)
{
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_RETURN))
	{
		//OnBnClickedSendChat();
		return TRUE;
	}
	//m_ToolTipCtrl.RelayEvent(pMsg);
	return __super::PreTranslateMessage(pMsg);
}

//�˵�����
BOOL CGameFrameControl::OnCommand(WPARAM wParam, LPARAM lParam)
{
	return TRUE;
	switch (LOWORD(wParam))
	{
	case IDM_UM_COPY_NAME:		//��������
		{
			//Ѱ�����
			if ((m_pUserData==NULL)||(m_pUserData->dwUserID==0)) return TRUE;

			//�򿪼��а�
			if (OpenClipboard()==FALSE) return TRUE;
			if (EmptyClipboard()==FALSE) 
			{
				CloseClipboard();
				return TRUE;
			}

			//��������
			HANDLE hData=GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT,NAME_LEN);
			if (hData==NULL) 
			{
				CloseClipboard();
				return TRUE;
			}
			LPTSTR szMemName=(LPTSTR)GlobalLock(hData);
			lstrcpy(szMemName,m_pUserData->szName);
			SetClipboardData(CF_TEXT,hData);
			GlobalUnlock(hData);
			CloseClipboard();

			//��ʾ����
			m_ChatInput.Paste();
			m_ChatInput.SetFocus();

			return TRUE;
		}
	case IDM_UM_SET_CHAT:		//��������
		{
			//Ѱ�����
			if ((m_pUserData==NULL)||(m_pUserData->dwUserID==0)) return TRUE;

			int nCount=m_ChatObject.GetCount();
			for (int i=1;i<nCount;i++)
			{
				if (m_ChatObject.GetItemData(i)==m_pUserData->dwUserID)
				{
					m_ChatObject.SetCurSel(i);
					break;
				}
			}
			m_ChatInput.SetFocus();

			return TRUE;
		}
	case IDM_UM_CANECL_CHAT:		//ȡ������
		{
			//Ѱ�����
			if ((m_pUserData==NULL)||(m_pUserData->dwUserID==0)) return TRUE;

			if (m_ChatObject.GetCurSel()!=LB_ERR)
			{
				DWORD dwUserID=(DWORD)m_ChatObject.GetItemData(m_ChatObject.GetCurSel());
				if (dwUserID==m_pUserData->dwUserID)
				{
					m_ChatObject.SetCurSel(0);
					break;
				}
			}
			m_ChatInput.SetFocus();

			return TRUE;
		}
	case IDM_UM_ALLOW_LOOKON:		//�����Թ�
	case IDM_UM_ENJOIN_LOOKON:		//��ֹ�Թ�
		{
			//Ѱ�����
			if ((m_pUserData==NULL)||(m_pUserData->dwUserID==0)) return TRUE;

			//���ÿ���
			bool bAllowLookon=(LOWORD(wParam)==IDM_UM_ALLOW_LOOKON);
			m_pIClientKernel->AllowUserLookon(m_pUserData->dwUserID,bAllowLookon);

			return TRUE;
		}
	case IDM_UM_KICK_TABLE_USER:	//�����û�
		{
			//Ѱ�����
			if ((m_pUserData==NULL)||(m_pUserData->dwUserID==0)) return TRUE;

			//������Ϣ
			CMD_GF_KickTableUser KickTableUser;
			KickTableUser.dwUserID=m_pUserData->dwUserID;
			m_pGameFrameDlg->m_ClientKernelHelper->SendSocketData(MDM_GF_FRAME,SUB_GF_KICK_TABLE_USER,&KickTableUser,sizeof(KickTableUser));

			return TRUE;
		}
	}

	return __super::OnCommand(wParam,lParam);
}

//�����ں�
void CGameFrameControl::SetClientKernel(IClientKernel * pIClientKernel)
{
	m_pIClientKernel=pIClientKernel;
	m_UserInfoView.SetClientKernel(m_pIClientKernel);

	return;
}

//���ýӿ�
bool CGameFrameControl::SetUserFaceRes(IUnknownEx * pIUnknownEx)
{
	return m_UserInfoView.SetUserFaceRes( pIUnknownEx);
}

//������Դ
bool CGameFrameControl::UpdateSkinResource()
{
	//��ȡ��Դ
	HINSTANCE hInstance=g_GlobalOption.m_PlatformResourceModule->GetResInstance();
	tagGameFrameImage &GameFrameImage=g_GlobalOption.m_GameFrameImage;

	//�б�����
	m_EncircleList.ImageML.SetLoadInfo(GameFrameImage.uUserListML,hInstance);
	m_EncircleList.ImageMR.SetLoadInfo(GameFrameImage.uUserListMR,hInstance);
	m_EncircleList.ImageTL.SetLoadInfo(GameFrameImage.uUserListTL,hInstance);
	m_EncircleList.ImageTM.SetLoadInfo(GameFrameImage.uUserListTM,hInstance);
	m_EncircleList.ImageTR.SetLoadInfo(GameFrameImage.uUserListTR,hInstance);
	m_EncircleList.ImageBL.SetLoadInfo(GameFrameImage.uUserListBL,hInstance);
	m_EncircleList.ImageBM.SetLoadInfo(GameFrameImage.uUserListBM,hInstance);
	m_EncircleList.ImageBR.SetLoadInfo(GameFrameImage.uUserListBR,hInstance);

	//��������
	m_EncircleChat.ImageML.SetLoadInfo(GameFrameImage.uChatML,hInstance);
	m_EncircleChat.ImageMR.SetLoadInfo(GameFrameImage.uChatMR,hInstance);
	m_EncircleChat.ImageTL.SetLoadInfo(GameFrameImage.uChatTL,hInstance);
	m_EncircleChat.ImageTM.SetLoadInfo(GameFrameImage.uChatTM,hInstance);
	m_EncircleChat.ImageTR.SetLoadInfo(GameFrameImage.uChatTR,hInstance);
	m_EncircleChat.ImageBL.SetLoadInfo(GameFrameImage.uChatBL,hInstance);
	m_EncircleChat.ImageBM.SetLoadInfo(GameFrameImage.uChatBM,hInstance);
	m_EncircleChat.ImageBR.SetLoadInfo(GameFrameImage.uChatBR,hInstance);

	//��ȡ��Ϣ
	CSkinAide::GetEncircleInfo(m_ImageInfoChat,m_EncircleChat);
	CSkinAide::GetEncircleInfo(m_ImageInfoList,m_EncircleList);

	//���ð�ť
	//m_btPhrase.SetButtonImage(IDB_BT_EXPESSION,hInstance,false);

	m_btColorSet.SetButtonImage(GameFrameImage.uBtColorSet,hInstance,false);
	m_btCleanScreen.SetButtonImage(GameFrameImage.uBtCleanScreen,hInstance,false);
	m_btBugle.SetButtonImage(GameFrameImage.uBtBugle,hInstance,false);
	//��Ϸ����
	m_btSound.SetButtonImage(GameFrameImage.uBtGameSound,hInstance,false);
	m_btLookOn.SetButtonImage(GameFrameImage.uBtGameLookOn,hInstance,false);
	m_btGameRule.SetButtonImage(GameFrameImage.uBtGameRule,hInstance,false);
	m_btGameOption.SetButtonImage(GameFrameImage.uBtGameOption,hInstance,false);

	HINSTANCE hDllInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
	m_btSendChat.LoadRgnImage(hDllInstance,IDB_BT_SENDCHAT, RGB(255, 0, 255));
	m_btExpression.LoadRgnImage(hDllInstance,IDB_BT_EXPRESSION, RGB(255, 0, 255));

	return true;
}

//���ÿ���
void CGameFrameControl::ResetGameFrameControl()
{
	//�������
	int nChatCount=m_ChatObject.GetCount();
	for (int i=1;i<nChatCount;i++) m_ChatObject.DeleteString(1);

	//�����б�
	m_UserListView.DeleteAllItems();

	return;
}

//�����û�
bool CGameFrameControl::InsertUserItem(tagUserData * pUserData)
{
	if ( m_pIClientKernel->GetMeUserInfo()->dwUserID == pUserData->dwUserID)
		m_UserInfoView.SetUserViewInfo(pUserData);

	return m_UserListView.InsertUserItem(pUserData);
}

//�����û�
bool CGameFrameControl::UpdateUserItem(tagUserData * pUserData)
{
	m_UserInfoView.UpdateUserInfo();

	return m_UserListView.UpdateUserItem(pUserData);
}

//ɾ���û�
bool CGameFrameControl::DeleteUserItem(tagUserData * pUserData)
{
	tagUserData const *pCurrentUserData = m_UserInfoView.GetCurrentUser();
	if ( pCurrentUserData != NULL && pUserData->dwUserID == pCurrentUserData->dwUserID )
		m_UserInfoView.SetUserViewInfo(m_pIClientKernel->GetMeUserInfo());

	return m_UserListView.DeleteUserItem(pUserData);
}

//�滭��Ϣ
void CGameFrameControl::OnPaint()
{
	CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);
	CMemDC dcMem(&dc, rect, false);	// �õ�ɫ��䱳��
	dcMem.SetBkMode(TRANSPARENT);
	
	if (m_ImageFrameBack.IsSetLoadInfo())
	{
		CImageHandle ImageHandle(&m_ImageFrameBack);
		m_ImageFrameBack.AlphaDrawImage(&dcMem, 0, 0, m_ImageFrameBack.GetWidth()-17, m_ImageFrameBack.GetHeight(), 
			WND_REMOTE_X, WND_REMOTE_Y, RGB(255,255,0));
	}

	dcMem.draw();
}

//���ð�ť
void CGameFrameControl::OnBnClickedOption()
{
	m_pGameFrameDlg->OnGameOptionSet();
	return;
}

//��Ϸ����
void CGameFrameControl::OnBnClickedRule()
{
	TCHAR szRuleUrl[128];
	_snprintf(szRuleUrl,sizeof(szRuleUrl),TEXT("http://qp.foxuc.net/GameRule.asp?KindID=%ld"),m_UserListView.m_wKindID);
	ShellExecute(NULL,TEXT("open"),szRuleUrl,NULL,NULL,SW_SHOWDEFAULT);

	return;
}

//��Ϸ�˳�
void CGameFrameControl::OnBnClickedQuit()
{
	//AfxGetMainWnd()->PostMessage(WM_CLOSE);
	return;
}

//��Ϸ����
void CGameFrameControl::OnBnClickedSound()
{
	if (m_pGameFrameDlg->IsEnableSound()==true)
	{
		m_pGameFrameDlg->EnableSound(false);
	}
	else
	{
		m_pGameFrameDlg->EnableSound(true);
	}

	return;
}

//��Ϸ�Թ�
void CGameFrameControl::OnBnClickedLookon()
{
	if (m_pGameFrameDlg->IsAllowUserLookon()==true)
	{
		m_pGameFrameDlg->AllowUserLookon(0L,false);
	}
	else
	{
		m_pGameFrameDlg->AllowUserLookon(0L,true);
	}

	return;
}

//�滭����
BOOL CGameFrameControl::OnEraseBkgnd(CDC * pDC)
{
	return TRUE;
}

//λ����Ϣ
void CGameFrameControl::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}

//������Ϣ
LRESULT CGameFrameControl::OnHitExpression(WPARAM wParam, LPARAM lParam)
{
	CExpressionItem * pExpressionItem=m_Expression.GetExpressItem((WORD)wParam);
	if (pExpressionItem!=NULL)
	{
		CString strBuffer;
		m_ChatInput.GetWindowText(strBuffer);
		strBuffer+=pExpressionItem->m_szTrancelate;
		m_ChatInput.SetWindowText(strBuffer);
		m_ChatInput.SetFocus();
		m_ChatInput.SetEditSel(strBuffer.GetLength(),strBuffer.GetLength());
	}

	return 0;
}

//���Ͱ�ť
void CGameFrameControl::OnBnClickedSendChat()
{
	//������Ϣ
	TCHAR szChatMessage[MAX_CHAT_LEN]=TEXT("");
	m_ChatInput.GetWindowText(szChatMessage,CountArray(szChatMessage));
	if (szChatMessage[0]!=0)
	{
		//��ʱ����
		DWORD dwCurTimes = GetTickCount();
		if(m_dwLimitChatTimes==0 || dwCurTimes-m_dwLimitChatTimes>=LIMIT_CHAT_TIMES)
		{
			m_dwLimitChatTimes = dwCurTimes;

			//OSC,������������.
			m_ChatInput.SetWindowText(TEXT(""));
			m_pIClientKernel->SendChatMessage(0,szChatMessage,g_GlobalOption.m_crMsgNormal);
		}
	}

	//���ý���
	m_ChatInput.SetFocus();

	return;
}

//���鰴ť
void CGameFrameControl::OnBnClickedExpression()
{
	//�������鴰��
	if (m_Expression.GetSafeHwnd()==NULL) m_Expression.CreateExpression(this);

	//�ƶ�����
	CRect rcButton;
	CSize ExpressSize;
	m_Expression.GetFixSize(ExpressSize);
	m_btExpression.GetWindowRect(&rcButton);
	m_Expression.MoveWindow(rcButton.right-ExpressSize.cx-220,rcButton.top-ExpressSize.cy,ExpressSize.cx,ExpressSize.cy);
	m_Expression.ShowWindow(SW_SHOW);
	m_Expression.SetFocus();

	return;
}

//��ɫ��ť
void CGameFrameControl::OnBnClickedColorSet()
{
	//������ɫ
	CColorDialog ColorDialog(g_GlobalOption.m_crChatTX,CC_FULLOPEN);
	if (ColorDialog.DoModal()==IDOK) g_GlobalOption.m_crChatTX=ColorDialog.GetColor();

	//���ý���
	m_ChatInput.SetFocus();

	return;
}

//������ť
void CGameFrameControl::OnBnClickedCleanScreen()
{
	m_ChatMessage.CleanScreen();
	return;
}

//���Ȱ�ť
void CGameFrameControl::OnBnClickedBugle()
{
	if(m_pIClientKernel==NULL)return;

	//���öԻ���
	//ShowBugleDlg(m_pIClientKernel,NULL,(tagUserData *)m_pIClientKernel->GetMeUserInfo());

	return ;
}

//����������Ϣ
void CGameFrameControl::InsertToGameFrameMsg(char* chData)
{
	if (NULL != chData)
	{//����Ϣ
		TCHAR szChatMessage[MAX_CHAT_LEN]=TEXT("");
		sprintf(szChatMessage, "%s", chData);
		m_pIClientKernel->SendChatMessage(0,szChatMessage,g_GlobalOption.m_crMsgNormal);
	}
}

//���ô�����ʾ
void CGameFrameControl::SetWndShow(bool bIsShow)
{
	return;
	//m_bIsUp = bIsShow;
	//m_iCountMoveIndex=0;				//�ƶ�֡
	//m_iCountMoveTotal=10;				//�ƶ���֡��
	//m_bDisableWndMove = true;
	//CRect rcTempMove;
	//if (true == m_bIsUp)
	//{
	//	//����
	//	m_MoveInfo.Init(m_rcSaveWnd.top+100, m_rcSaveWnd.top);
	//	rcTempMove.left = m_rcSaveWnd.left;
	//	rcTempMove.right = m_rcSaveWnd.right;
	//	rcTempMove.top = m_rcSaveWnd.top+100;
	//	rcTempMove.bottom = m_rcSaveWnd.top+m_rcSaveWnd.Height();
	//}
	//else
	//{
	//	//����
	//	m_MoveInfo.Init(m_rcSaveWnd.top, m_rcSaveWnd.top+100);
	//	rcTempMove = m_rcSaveWnd;
	//}

	////MoveWindow(&rcTempMove);
	//ShowWindow(SW_SHOW);

	////�����𶯴����ƶ���Ϣ
	//SetTimer(IDI_MOVE_WND, TIME_MOVE_WND, NULL);
}

//��ô���״̬
bool CGameFrameControl::GetWndIsUp(void)
{
	return m_bIsUp;
}


//����ͼ��
void CGameFrameControl::DrawLeftViewFrame(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	HDC hDC=pDC->m_hDC;
	int nXPos=0,nYPos=0;
	int nComminutePos=rcClient.Height()*4/8;

	//�滭�Ŀ�
	CRect rcChat;
	rcChat.left=0;
	rcChat.right=rcClient.Width();
	rcChat.top=nComminutePos;
	rcChat.bottom=rcClient.Height();
	CSkinAide::DrawEncircleFrame(pDC,rcChat,m_EncircleChat);

	//�滭�б�
	CRect rcList;
	rcList.left=0;
	rcList.right=rcClient.Width();
	rcList.top=0;
	rcList.bottom=nComminutePos;
	CSkinAide::DrawEncircleFrame(pDC,rcList,m_EncircleList);

	return;
}

//�����ؼ�
void CGameFrameControl::RectifyControl(int nWidth, int nHeight)
{
	//״̬�ж�
	if (m_bInitDialog==false) return;
	if ((nWidth==0)||(nHeight==0)) return;

	//��������
	const int nComminutePos=nHeight*4/8;
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS;

	//�ƶ��ؼ�
	HDWP hDwp=BeginDeferWindowPos(32);

	m_ChatMessage.SetWindowPos(&wndTop, WND_REMOTE_X+3, WND_REMOTE_Y-6, 181, 90, 0);
	//��������
	//m_PropertyBar.ShowWindow(SW_HIDE);
	//DeferWindowPos(hDwp, m_PropertyBar, NULL, m_ImageInfoList.nLBorder-4, nComminutePos-m_PropertyBar.GetHeight()+2, 
	//	nWidth-m_ImageInfoList.nLBorder-m_ImageInfoList.nRBorder+7, m_PropertyBar.GetHeight(), uFlags); 

	//�б�����
	m_UserListView.ShowWindow(SW_HIDE);

	//��������
	CRect rcButton;
	m_btBugle.GetWindowRect(&rcButton);
	CButton * pButtonArray[]={&m_btExpression,&m_btColorSet,&m_btCleanScreen,&m_btBugle};
	for (int i=0;i<CountArray(pButtonArray);i++)
	{
		//DeferWindowPos(hDwp,pButtonArray[i]->m_hWnd,NULL,m_ImageInfoList.nLBorder+(rcButton.Width()+5)*i,nHeight-53,rcButton.Width(),rcButton.Height(),uFlags);
		pButtonArray[i]->ShowWindow(SW_HIDE);
	}

	//����
	DeferWindowPos(hDwp,m_btExpression,NULL,226,116,248,136,uFlags);
	m_btExpression.ShowWindow(SW_HIDE);
	m_btSendChat.GetWindowRect(&rcButton);

	//����
	DeferWindowPos(hDwp,m_btSendChat,NULL,203,116,225,136,uFlags);
	DeferWindowPos(hDwp,m_ChatObject,NULL,8,33,70,200,uFlags);
	DeferWindowPos(hDwp,m_ChatInput,NULL,16,116,184,135,uFlags);
	m_ChatObject.ShowWindow(SW_HIDE);
	m_ChatInput.ShowWindow(SW_HIDE);
	m_btSendChat.ShowWindow(SW_HIDE);

	//���ܰ�ť
	m_btSound.GetWindowRect(&rcButton);
	int nBeginPos=12,nEndPos=nWidth-8;
	int nButtonSpace=(nEndPos-nBeginPos-4*rcButton.Width())/4,nYPosButton=nComminutePos+5;
	DeferWindowPos(hDwp,m_btSound,NULL,nBeginPos,nYPosButton,rcButton.Width(),rcButton.Height(),uFlags);
	DeferWindowPos(hDwp,m_btLookOn,NULL,nBeginPos+nButtonSpace+rcButton.Width(),nYPosButton,rcButton.Width(),rcButton.Height(),uFlags);
	DeferWindowPos(hDwp,m_btGameRule,NULL,nBeginPos+nButtonSpace*2+rcButton.Width()*2,nYPosButton,rcButton.Width(),rcButton.Height(),uFlags);
	DeferWindowPos(hDwp,m_btGameOption,NULL,nBeginPos+nButtonSpace*3+rcButton.Width()*3,nYPosButton,rcButton.Width(),rcButton.Height(),uFlags);
	DeferWindowPos(hDwp,m_btGameQuit,NULL,nBeginPos+nButtonSpace*4+rcButton.Width()*4,nYPosButton,rcButton.Width(),rcButton.Height(),uFlags);
	m_btGameQuit.ShowWindow(SW_HIDE);
	m_btSound.ShowWindow(SW_HIDE);
	m_btLookOn.ShowWindow(SW_HIDE);
	m_btGameRule.ShowWindow(SW_HIDE);
	m_btGameOption.ShowWindow(SW_HIDE);

	//�û�����
	DeferWindowPos(hDwp,m_UserInfoView,NULL,m_ImageInfoList.nLBorder,7,nWidth-m_ImageInfoList.nLBorder-m_ImageInfoList.nRBorder,145,uFlags);
	m_UserInfoView.ShowWindow(SW_HIDE);

	EndDeferWindowPos(hDwp);

	//�ػ�����
	Invalidate(FALSE);
	UpdateWindow();

	return;
}

//�Ҽ��б�
void CGameFrameControl::OnNMRclickUserList(NMHDR * pNMHDR, LRESULT * pResult)
{
	return;

	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	if (pListNotify->iItem!=-1)
	{
		//��ȡλ��
		CPoint MousePoint;
		GetCursorPos(&MousePoint);

		//��ȡ�û�
		TCHAR szBuffer[512]=TEXT("");
		m_pUserData=(tagUserData *)m_UserListView.GetItemData(pListNotify->iItem);
		if(m_pUserData==NULL) return;

		//Ԥ�ȼ���
		double dFleeRate=0.0;
		LONG lPlayCount=m_pUserData->lWinCount+m_pUserData->lLostCount+m_pUserData->lDrawCount+m_pUserData->lFleeCount;
		if (m_pUserData->lFleeCount>0)
		{
			dFleeRate=(double)(m_pUserData->lFleeCount*100)/(double)lPlayCount;
			if (dFleeRate<0.01) dFleeRate=0.0;
		}

		//����˵�
		CMenu UserInfoMenu;
		UserInfoMenu.CreatePopupMenu();

		//���ܲ˵�
		UserInfoMenu.AppendMenu(0,IDM_UM_COPY_NAME,TEXT("�����û���"));
		UserInfoMenu.AppendMenu(MF_SEPARATOR);

		//�û���ν
		LPCTSTR pszMemberOrder[]={TEXT("��ͨ��Ա"),TEXT("�м���Ա"),TEXT("�߼����")};
		if ((m_pUserData->cbMemberOrder>0)&&(m_pUserData->cbMemberOrder<CountArray(pszMemberOrder)))
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("�û�����%s [ %s ]"),m_pUserData->szName,pszMemberOrder[m_pUserData->cbMemberOrder]);
		}
		else _snprintf(szBuffer,sizeof(szBuffer),TEXT("�û�����%s"),m_pUserData->szName);
		UserInfoMenu.AppendMenu(0,0,szBuffer);

		//�û� ID
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("��Ϸ ID��%ld"),m_pUserData->dwGameID);
		UserInfoMenu.AppendMenu(0,0,szBuffer);

		//�û�����
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("����ֵ��%ld"),m_pUserData->lExperience);
		UserInfoMenu.AppendMenu(0,0,szBuffer);

		//�û�����
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("���֣�%I64d  �ܾ�����%ld  �����ʣ�%5.2f%%"),m_pUserData->lScore,lPlayCount,dFleeRate);
		UserInfoMenu.AppendMenu(0,0,szBuffer);

		//��ȡ�û�
		DWORD dwCurUserID=0L;
		const tagUserData * pMeUserData=m_pIClientKernel->GetMeUserInfo();
		if (m_ChatObject.GetCurSel()!=LB_ERR)
		{
			dwCurUserID=(DWORD)m_ChatObject.GetItemData(m_ChatObject.GetCurSel());
		}

		//�������
		if (pMeUserData->dwUserID!=m_pUserData->dwUserID) UserInfoMenu.AppendMenu(MF_SEPARATOR);
		if ((dwCurUserID!=m_pUserData->dwUserID)&&(dwCurUserID!=pMeUserData->dwUserID)&&(pMeUserData->dwUserID!=m_pUserData->dwUserID))
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("�� [ %s ] ����"),m_pUserData->szName);
			UserInfoMenu.AppendMenu(0,IDM_UM_SET_CHAT,szBuffer);
		}
		if (dwCurUserID==m_pUserData->dwUserID)
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("ȡ���� [ %s ] ������"),m_pUserData->szName);
			UserInfoMenu.AppendMenu(0,IDM_UM_CANECL_CHAT,szBuffer);
		}

		//�Թۿ���
		if ((pMeUserData!=m_pUserData)&&(pMeUserData->cbUserStatus!=US_LOOKON)&&(m_pUserData->wChairID==pMeUserData->wChairID))
		{
			//�ָ��˵�
			UserInfoMenu.AppendMenu(MF_SEPARATOR);

			//�����Թ�
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("���� [ %s ] �ۿ�����Ϸ"),m_pUserData->szName);
			UserInfoMenu.AppendMenu(0,IDM_UM_ALLOW_LOOKON,szBuffer);

			//��ֹ�Թ�
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("��ֹ [ %s ] �ۿ�����Ϸ"),m_pUserData->szName);
			UserInfoMenu.AppendMenu(0,IDM_UM_ENJOIN_LOOKON,szBuffer);
		}

		//��ȡ����
		tagServerAttribute const* pServerAttribute = m_pIClientKernel->GetServerAttribute();

		if ( pServerAttribute->wChairCount <= MAX_CHAIR_NORMAL || m_pIClientKernel->GetMeUserInfo()->cbMasterOrder > 0 )
		{
			//���˲˵�
			if (m_pIClientKernel->GetMeUserInfo()->dwUserID!=m_pUserData->dwUserID)
			{
				//���Ʋ˵�
				bool bEnable=false;
				if (bEnable==false) bEnable=(pMeUserData->cbMasterOrder>m_pUserData->cbMasterOrder);
				if (bEnable==false) bEnable=(pMeUserData->cbMemberOrder>m_pUserData->cbMemberOrder);
				if ( pMeUserData->cbMasterOrder<m_pUserData->cbMasterOrder ) bEnable = false;

				//����˵�
				_snprintf(szBuffer,sizeof(szBuffer),TEXT("�� [ %s ] �߳���Ϸ����"),m_pUserData->szName);
				UserInfoMenu.AppendMenu((bEnable==true)?MF_ENABLED:MF_GRAYED,IDM_UM_KICK_TABLE_USER,szBuffer);
			}
		}

		//��ʾ�˵�
		UserInfoMenu.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,MousePoint.x,MousePoint.y,this);
	}

	return;
}

//ѡ���б�
void CGameFrameControl::OnNMclickUserList(NMHDR *pNMHDR, LRESULT *pResult)
{
	return;

	NMITEMACTIVATE * pListNotify=(NMITEMACTIVATE *)pNMHDR;
	if (pListNotify->iItem!=-1)
	{
		//��ȡ�û�
		tagUserData *pUserData=(tagUserData *)m_UserListView.GetItemData(pListNotify->iItem);
		if(pUserData==NULL) return;

		//������Ϣ
		m_UserInfoView.SetUserViewInfo(pUserData);

		//ѡ���û�
		//m_PropertyBar.SetUserDataInfo(pUserData);

		//��ʱ�л�
		if(pUserData->dwUserID!=m_pIClientKernel->GetMeUserInfo()->dwUserID)SetTimer(IDI_SWITCH_USER,TIME_SWITCH_USER,NULL);
	}
	return;
}

//������������Ϣ
void CGameFrameControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	/*if (NULL != m_pGameFrameDlg)
	{
	m_pGameFrameDlg->SetShowUserActionWnd(false);
	}*/
	CDialog::OnLButtonDown(nFlags, point);
}


//ʱ����Ϣ
void CGameFrameControl::OnTimer(UINT nIDEvent)
{
	KillTimer(nIDEvent);
	return;
	switch(nIDEvent)
	{
	case IDI_MOVE_WND:			//�ƶ�������Ϣ
		{
			if (true == m_bDisableWndMove)
			{
				//����
				CRect rcTempMove;
				if (m_iCountMoveIndex < m_iCountMoveTotal-3)
				{
					float fX = m_MoveInfo.lerp(m_iCountMoveIndex, m_iCountMoveTotal);
					rcTempMove.left = m_rcSaveWnd.left;
					rcTempMove.right = m_rcSaveWnd.right;
					rcTempMove.top = fX;
					rcTempMove.bottom = rcTempMove.top+m_rcSaveWnd.Height();
					++m_iCountMoveIndex;
				}
				else
				{
					if (true == m_bIsUp)
					{
						//����
						rcTempMove = m_rcSaveWnd;
					}
					else
					{
						//����
						rcTempMove.left = m_rcSaveWnd.left;
						rcTempMove.right = m_rcSaveWnd.right;
						rcTempMove.top = m_rcSaveWnd.top+97;
						rcTempMove.bottom = rcTempMove.top+m_rcSaveWnd.Height();
					}

					KillTimer(IDI_MOVE_WND);
					m_iCountMoveIndex = 0;
				}				
			}

			break;
		}
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}
//////////////////////////////////////////////////////////////////////////

void CGameFrameControl::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//if (NULL != m_pGameFrameDlg)
	//{
	//m_pGameFrameDlg->SetShowUserActionWnd(false);
	//}
	__super::OnRButtonDown(nFlags, point);
}

BOOL CGameFrameControl::SetWindowPos(const CWnd* pWndInsertAfter, int x, int y,
									 int cx, int cy, UINT nFlags)
{
	m_ptWnd.SetPoint(x, y);
	return CRgnDialog::SetWindowPos(pWndInsertAfter, x+WND_REMOTE_X, y+WND_REMOTE_Y, cx-17, cy, nFlags);
}

void CGameFrameControl::DrawBackground(CDC *pDC, int nWidth, int nHeight, int iExcursionY/*=0*/)
{
	if (NULL == pDC  || NULL == GetSafeHwnd()  ||  FALSE == IsWindowVisible())
	{
		return;
	}

	if ( !m_ImageFrameBackAlpha.IsNull() )
	{
		m_ImageFrameBackAlpha.DrawImage(pDC, m_ptWnd.x, m_ptWnd.y-iExcursionY);
	}
}