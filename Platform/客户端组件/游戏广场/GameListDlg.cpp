// GameListDlg.cpp : ʵ���ļ�
//

#include "stdafx.h" 
#include "GameListDlg.h"
#include "GlobalUnits.h"
#include "StreamReader.h"   
#include "HtmlResource.h"
#include <afxctl.h>
#include ".\gamelistdlg.h"
#include "webpath.h"
#include "InitData.h"
// CGameListDlg �Ի���

IMPLEMENT_DYNAMIC(CGameListDlg, CDialog)
CGameListDlg::CGameListDlg(/*CWnd* pParent *//*=NULL*/):CDialog(IDD_GAME_LIST)
	/*: CDialog(CGameListDlg::IDD, pParent)*/
{

	EnableAutomation();//������,����Ȼ���GetIDispatch()ʱ��ʧ��.
}

CGameListDlg::~CGameListDlg()
{
	if (NULL != m_pExAd)
	{
		delete m_pExAd;
	}
	m_pExAd=NULL;
}

void CGameListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX,IDC_EXPLORER1,m_exMain);
	
	//DDX_Control(pDX,IDC_BT_HONOUR_ROLL,m_btnHonurRoll);
	DDX_Control(pDX,IDC_BT_SITE,m_btnSite);
	DDX_Control(pDX,IDC_BT_CHARGE,m_btnCharge);

	DDX_Control(pDX,IDC_BT_AM,m_btnAccountsManage); 
	DDX_Control(pDX,IDC_BT_CLINET,m_btnClinet); 

}


BEGIN_MESSAGE_MAP(CGameListDlg, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()	
	ON_WM_ERASEBKGND()


	//ON_BN_CLICKED(IDC_BT_HONOUR_ROLL,&CGameListDlg::OnBnClickedHonurRoll)
	ON_BN_CLICKED(IDC_BT_SITE,&CGameListDlg::OnBnClickedSite)
	ON_BN_CLICKED(IDC_BT_CHARGE,&CGameListDlg::OnBnClickedCharge)
	ON_BN_CLICKED(IDC_BT_CLINET,&CGameListDlg::OnBnClickedClinet)
	ON_BN_CLICKED(IDC_BT_AM,&CGameListDlg::OnBnClickedAccountsManage)

END_MESSAGE_MAP()
 
BEGIN_DISPATCH_MAP(CGameListDlg, CDialog)
	DISP_FUNCTION_ID(CGameListDlg, "oncontextmenu", DISPID_HTMLDOCUMENTEVENTS2_ONCONTEXTMENU, onHtmlContextMenu, VT_BOOL, VTS_DISPATCH)
END_DISPATCH_MAP()

BEGIN_EVENTSINK_MAP(CGameListDlg, CDialog)
	ON_EVENT(CGameListDlg, 20, 259, OnDocumentCompleteExplorer1, VTS_DISPATCH VTS_PVARIANT)
	ON_EVENT(CGameListDlg, 21, 259, OnDocumentCompleteExplorer2, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()


BOOL CGameListDlg::OnInitDialog()
{
	__super::OnInitDialog();	

	//SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE)^0x80000);

	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);

	m_exMain=new CExmain;
	m_exMain->Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(6,387,6+646,387+193),this,20,NULL);	 
	m_exMain->Navigate2(&CComVariant("about:blank"),NULL,NULL,NULL,NULL);

	HRESULT hr = ::CoMarshalInterThreadInterfaceInStream(IID_IWebBrowser2,m_exMain->GetControlUnknown(),&m_iStream);
	if ( FAILED(hr) )return 0;
	m_pBrowser = NULL;	 
	hr = ::CoGetInterfaceAndReleaseStream(m_iStream,IID_IWebBrowser2,(void**)&m_pBrowser); //�������ýӿ�ָ��
	CComPtr < IDispatch > spDispDoc;
	hr = m_pBrowser->get_Document( &spDispDoc );
	if (FALSE(hr))return 0; 

	CComQIPtr< IHTMLDocument2 > pIDoc = spDispDoc;
	if (pIDoc == NULL)return 0;	
	m_webPage.SetDocument(pIDoc);
 

	//��ȡhtml
	CString strHtml=m_gameListManager->GetGameListHtml(); 
	ShowHtml(strHtml);

	m_pExAd=new CExmain;
	m_pExAd->Create(NULL, NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,658,376),this,21,NULL);
	//m_pExAd->Navigate2(&CComVariant(WEB_HALL_AD),NULL,NULL,NULL,NULL);
	
	HINSTANCE hInstance=g_GlobalUnits.m_PlatformResourceModule->GetResInstance();	
	m_btnSite.LoadRgnImage(hInstance,g_GlobalUnits.m_PlatformFrameImage.uFrameNavigation[0], RGB(255, 0, 255), 4);
	m_btnCharge.LoadRgnImage(hInstance,g_GlobalUnits.m_PlatformFrameImage.uFrameNavigation[2], RGB(255, 0, 255), 4);
	m_btnAccountsManage.LoadRgnImage(hInstance,g_GlobalUnits.m_PlatformFrameImage.uFrameNavigation[5], RGB(255, 0, 255), 4);
	//m_btnHonurRoll.LoadRgnImage(hInstance,g_GlobalUnits.m_PlatformFrameImage.uFrameNavigation[6]);
	m_btnClinet.LoadRgnImage(hInstance,g_GlobalUnits.m_PlatformFrameImage.uFrameNavigation[8], RGB(255, 0, 255), 4);

	HINSTANCE hDllInstance = AfxGetInstanceHandle();
	m_ImageBack.SetLoadInfo(IDB_GAME_LIST_BACK, hDllInstance, false);

	m_ToolTipCtrl.AddTool(&m_btnSite,TEXT("�ٷ���վ"));
	m_ToolTipCtrl.AddTool(&m_btnCharge,TEXT("��ֵ����"));
	m_ToolTipCtrl.AddTool(&m_btnAccountsManage,TEXT("�ʺŹ���"));
	//m_ToolTipCtrl.AddTool(&m_btnHonurRoll,TEXT("�������"));
	m_ToolTipCtrl.AddTool(&m_btnClinet,TEXT("�ͻ�����"));

	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	DeferWindowPos(hDwp,m_btnSite,NULL,0,589,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnAccountsManage,NULL,167,589,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnCharge,NULL,329,589,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnClinet,NULL,491,589,0,0,uFlags);
	//m_btnAccountsManage.ShowWindow(SW_HIDE);
	//m_btnCharge.ShowWindow(SW_HIDE);
	//DeferWindowPos(hDwp,m_btnHonurRoll,NULL,803,31,0,0,uFlags);
	EndDeferWindowPos(hDwp);
 	return TRUE;
}

void CGameListDlg::OnLButtonDown( UINT nFlags, CPoint point )
{
	//CRect rectWindow;
	//GetWindowRect(&rectWindow);
	//if (point.x > 0 && point.x < rectWindow.right && point.y > 0 && point.y <rectWindow.bottom )
	//{		 
	//	GetParent()->PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x,point.y));
	//}
}

void CGameListDlg::OnPaint()
{
	CPaintDC dc(this);
	//CRect rect;
	//GetClientRect(&rect);
	//CDC pMemDC;
	//pMemDC.CreateCompatibleDC(&dc);
	//CBitmap hBitmap;
	//hBitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	//pMemDC.SelectObject(&hBitmap);
	//pMemDC.SetBkMode(TRANSPARENT);
	//pMemDC.FillSolidRect(0,0,rect.Width(), rect.Height(),RGB(0,0,0));
	////���Ʊ���
	//CImageHandle handle(&m_ImageBack);
	//m_ImageBack.AlphaDrawImage(&pMemDC, 0, 0, RGB(255, 0, 255));
	//dc.BitBlt(0, 0, rect.Width(), rect.Height(), &pMemDC, 0, 0, SRCCOPY);
	//DeleteObject(&hBitmap);
	//pMemDC.DeleteDC();
}
void CGameListDlg::OnOK()
{
	//AfxMessageBox(TEXT("CGameListDlg::OnOK"));
}

// ������� IDC_BT_HONOUR_ROLL
// void CGameListDlg::OnBnClickedHonurRoll()
// {
// 	ShellExecute(NULL,   "open",   "http://www.acetechcn.com ",     ""   ,NULL,   SW_SHOW);
// 
// }
//�ٷ���վ IDC_BT_SITE
void CGameListDlg::OnBnClickedSite()
{
	ShellExecute(NULL,   "open",   CInitData::s_cHomePage,     ""   ,NULL,   SW_SHOW);

}
//��ֵ���� IDC_BT_CHARGE
void CGameListDlg::OnBnClickedCharge()
{
	//ShellExecute(NULL,   "open",   WEB_DEPOSIT_LIST,     ""   ,NULL,   SW_SHOW);

}
//�ͻ����� IDC_BT_CLINET
void CGameListDlg::OnBnClickedClinet()
{
	ShellExecute(NULL,   "open",   CInitData::s_cContactService,     ""   ,NULL,   SW_SHOW);

}
//�ʺŹ��� IDC_BT_RE_ACCOUNTS_MANAGE
void CGameListDlg::OnBnClickedAccountsManage()
{
	ShellExecute(NULL,   "open",   CInitData::s_cAccountsManage,     ""   ,NULL,   SW_SHOW);

}	
//����
void CGameListDlg::SetHtml()
{
	//��ȡhtml
	CString strHtml=m_gameListManager->GetGameListHtml(); 
	ShowHtml(strHtml);
	m_exMain->ShowWindow(SW_SHOW);
}

void CGameListDlg::ShowHtml( CString content )
{
	CStreamReader coc;
	coc.SetData(content);

	HRESULT hr;

	CComPtr< IDispatch > spDoc(m_exMain->get_Document());
	ASSERT( spDoc );

	CComQIPtr< IPersistStreamInit, &IID_IPersistStreamInit > spPSI( spDoc );
	ASSERT(spPSI);


	hr = spPSI->InitNew();
	ASSERT( SUCCEEDED(hr) );

	hr = spPSI->Load( static_cast<IStream*>(&coc) );	
	ASSERT( SUCCEEDED(hr) );

}
 
BOOL CGameListDlg::onHtmlContextMenu(IHTMLEventObj *pEvtObj)
{ 
	//�ڳɹ��������¼�Դ��,ÿ���û��һ���������������,����Ը���pEvtObj���жϵ�ǰ���λ�õ�,Ȼ��������Լ������˵�,��IE�����˵�,����ʲô������...
	//MessageBox("onHtmlContextMenu");
	return S_OK;
}

void CGameListDlg::OnDocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT FAR* URL) 
{
	//����WebBrowser�ؼ���DocumentComplete�¼�,����ʼ��pDoc2ָ������ӵ��¼�Դ
	HRESULT hr=m_exMain->get_Document()->QueryInterface(IID_IHTMLDocument2,(void**)&pDoc2);
	BOOL Ret = AfxConnectionAdvise( 
									pDoc2,  //�����Ӷ���Ľӿ�ָ��
									DIID_HTMLDocumentEvents2, //���ӽӿ�ID
									GetIDispatch(FALSE), //����Ƕ��IDispatchʵ�����һ������ʵ��m_xDispatch���˳�ȥ
									FALSE,   //donod addref
									&m_dwCookie );  //cookie to break connection later...
	if(Ret){
//		AfxMessageBox("�ɹ��ҽ���");
	}
}
void CGameListDlg::OnDocumentCompleteExplorer2(LPDISPATCH pDisp, VARIANT FAR* URL)
{
	//����WebBrowser�ؼ���DocumentComplete�¼�,����ʼ��pDoc2ָ������ӵ��¼�Դ
	HRESULT hr=m_pExAd->get_Document()->QueryInterface(IID_IHTMLDocument2,(void**)&pDoc2);
	BOOL Ret = AfxConnectionAdvise( 
									pDoc2,  //�����Ӷ���Ľӿ�ָ��
									DIID_HTMLDocumentEvents2, //���ӽӿ�ID
									GetIDispatch(FALSE), //����Ƕ��IDispatchʵ�����һ������ʵ��m_xDispatch���˳�ȥ
									FALSE,   //donod addref
									&m_dwCookie );  //cookie to break connection later...
	if(Ret){
		//		AfxMessageBox("�ɹ��ҽ���");
	}
}
// CGameListDlg ��Ϣ�������

BOOL CGameListDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (m_ToolTipCtrl.m_hWnd!=NULL)
		m_ToolTipCtrl.RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CGameListDlg::OnEraseBkgnd(CDC * pDC)
{
	CRect rect;
	GetClientRect(&rect);
	CDC pMemDC;
	pMemDC.CreateCompatibleDC(pDC);
	CBitmap hBitmap;
	hBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	pMemDC.SelectObject(&hBitmap);
	pMemDC.SetBkMode(TRANSPARENT);
	pMemDC.FillSolidRect(0,0,rect.Width(), rect.Height(),RGB(0,0,0));
	//���Ʊ���
	CImageHandle handle(&m_ImageBack);
	m_ImageBack.AlphaDrawImage(&pMemDC, 0, 0, RGB(255, 0, 255));
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &pMemDC, 0, 0, SRCCOPY);
	DeleteObject(&hBitmap);
	pMemDC.DeleteDC();
	//pDC->FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(255, 0, 0));
	return TRUE;
}