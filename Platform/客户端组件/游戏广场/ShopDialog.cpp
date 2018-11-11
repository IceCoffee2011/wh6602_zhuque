// ShopDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ShopDialog.h"
#include ".\shopdialog.h"
#include <afxtempl.h>
#include <afxctl.h>
#include <MsHtmdid.h>
#include "webpath.h"
#include "GlobalUnits.h"
// CShopDialog �Ի���

IMPLEMENT_DYNAMIC(CShopDialog, CDialog)
CShopDialog::CShopDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CShopDialog::IDD, pParent)
{
	m_exMain = NULL;
	EnableAutomation();//������,����Ȼ���GetIDispatch()ʱ��ʧ��.
}

CShopDialog::~CShopDialog()
{
	if (NULL != m_exMain)
	{
		delete m_exMain;
	}
	m_exMain=NULL;
}

void CShopDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShopDialog, CDialog)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CShopDialog, CDialog)
	DISP_FUNCTION_ID(CShopDialog, "oncontextmenu", DISPID_HTMLDOCUMENTEVENTS2_ONCONTEXTMENU, onHtmlContextMenu, VT_BOOL, VTS_DISPATCH)
	DISP_FUNCTION_ID(CShopDialog, "oncontextMouseDown", DISPID_HTMLDOCUMENTEVENTS2_ONMOUSEDOWN, onHtmlMouseDown, VT_BOOL, VTS_DISPATCH)	
END_DISPATCH_MAP()

BEGIN_EVENTSINK_MAP(CShopDialog, CDialog)
	ON_EVENT(CShopDialog, 20, 259, OnDocumentCompleteExplorer1, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

// CShopDialog ��Ϣ�������

BOOL CShopDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE)^0x80000);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	const tagGlobalUserData & userdata = g_GlobalUnits.GetGolbalUserData();
	CString strUrl;
	//strUrl.Format(WEB_MY_SHOP, userdata.dwUserID, userdata.szPassWord, 1);
	m_exMain=new CExmain;
	m_exMain->Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,20,NULL);	 
	m_exMain->Navigate2(&CComVariant(strUrl),NULL,NULL,NULL,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
BOOL CShopDialog::onHtmlContextMenu(IHTMLEventObj *pEvtObj)
{ 
	//�ڳɹ��������¼�Դ��,ÿ���û��һ���������������,����Ը���pEvtObj���жϵ�ǰ���λ�õ�,Ȼ��������Լ������˵�,��IE�����˵�,����ʲô������...
	return S_OK;
}
BOOL CShopDialog::onHtmlMouseDown(IHTMLEventObj *pEvtObj)
{
	//PostMessage (WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(0,0));
	return S_OK;
}
void CShopDialog::OnDocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT FAR* URL) 
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

void CShopDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: �ڴ˴������Ϣ����������
}

//��ʾһ������
void CShopDialog::SetWindowShow(bool bIsShow)
{
	if (bIsShow)
	{
		ShowWindow(SW_SHOW);
	}
	else
	{
		ShowWindow(SW_HIDE);
	}
}

//��������
void CShopDialog::SetResetURL()
{
	if (NULL != m_exMain)
	{
		const tagGlobalUserData & userdata = g_GlobalUnits.GetGolbalUserData();
		CString strUrl;
		//strUrl.Format(WEB_MY_SHOP, userdata.dwUserID, userdata.szPassWord, 1);
		m_exMain->Navigate2(&CComVariant(strUrl),NULL,NULL,NULL,NULL);
		CRect rect;
		GetClientRect(&rect);
		::SetWindowPos(m_exMain->GetSafeHwnd(), 0, 0, 0, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
	}
}

BOOL CShopDialog::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CShopDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

void CShopDialog::OnLButtonDown(UINT nFlags, CPoint point)
{

	CDialog::OnLButtonDown(nFlags, point);
}
