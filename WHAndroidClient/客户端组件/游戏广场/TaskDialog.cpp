// TaskDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TaskDialog.h"
#include <afxtempl.h>
#include <afxctl.h>
#include <MsHtmdid.h>
#include "webpath.h"
#include "GlobalUnits.h"
#include ".\taskdialog.h"

// CTaskDialog �Ի���

IMPLEMENT_DYNAMIC(CTaskDialog, CDialog)
CTaskDialog::CTaskDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTaskDialog::IDD, pParent)
{
	m_exMain = NULL;
	EnableAutomation();//������,����Ȼ���GetIDispatch()ʱ��ʧ��.
}

CTaskDialog::~CTaskDialog()
{
}

void CTaskDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

// CTaskDialog ��Ϣ�������

BEGIN_MESSAGE_MAP(CTaskDialog, CDialog)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CTaskDialog, CDialog)
	DISP_FUNCTION_ID(CTaskDialog, "oncontextmenu", DISPID_HTMLDOCUMENTEVENTS2_ONCONTEXTMENU, onHtmlContextMenu, VT_BOOL, VTS_DISPATCH)
	DISP_FUNCTION_ID(CTaskDialog, "oncontextMouseDown", DISPID_HTMLDOCUMENTEVENTS2_ONMOUSEDOWN, onHtmlMouseDown, VT_BOOL, VTS_DISPATCH)
END_DISPATCH_MAP()

BEGIN_EVENTSINK_MAP(CTaskDialog, CDialog)
	ON_EVENT(CTaskDialog, 20, 259, OnDocumentCompleteExplorer1, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

// CTaskDialog ��Ϣ�������

BOOL CTaskDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE)^0x80000);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_exMain=new CExmain;
	m_exMain->Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,20,NULL);	 
	m_exMain->Navigate2(&CComVariant("about:blank"),NULL,NULL,NULL,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//����URL
void CTaskDialog::ResetDlgURL()
{
	if (NULL != m_exMain)
	{
		const tagGlobalUserData & userdata = g_GlobalUnits.GetGolbalUserData();
		CString strUrl;
		//strUrl.Format(WEB_MY_TASK, userdata.dwUserID, userdata.szPassWord);
		//m_exMain->Navigate2(&CComVariant(strUrl),NULL,NULL,NULL,NULL);
		CRect rect;
		GetClientRect(&rect);
		::SetWindowPos(m_exMain->GetSafeHwnd(), 0, 0, 0, rect.Width(), rect.Height(), SWP_SHOWWINDOW);
	}
}
BOOL CTaskDialog::onHtmlContextMenu(IHTMLEventObj *pEvtObj)
{ 
	//�ڳɹ��������¼�Դ��,ÿ���û��һ���������������,����Ը���pEvtObj���жϵ�ǰ���λ�õ�,Ȼ��������Լ������˵�,��IE�����˵�,����ʲô������...
	return S_OK;
}
BOOL CTaskDialog::onHtmlMouseDown(IHTMLEventObj *pEvtObj)
{
	//PostMessage (WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(0,0));
	return S_OK;
}
void CTaskDialog::OnDocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT FAR* URL) 
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

void CTaskDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
}

BOOL CTaskDialog::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CTaskDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

void CTaskDialog::OnLButtonDown(UINT nFlags, CPoint point)
{

	CDialog::OnLButtonDown(nFlags, point);
}
