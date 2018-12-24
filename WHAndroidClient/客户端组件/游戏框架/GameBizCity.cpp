// GameBizCity.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GameBizCity.h"
#include ".\gamebizcity.h"
#include <afxtempl.h>
#include <afxctl.h>
#include <MsHtmdid.h>
#include "GameFrameNetDefine.h"

// CGameBizCity �Ի���




IMPLEMENT_DYNAMIC(CGameBizCity, CDialog)
CGameBizCity::CGameBizCity(CWnd* pParent /*=NULL*/)
	: CDialog(CGameBizCity::IDD, pParent)
{
	m_exMain = NULL;
	EnableAutomation();
	::memset(m_chUserMD5PassWord, 0, sizeof(m_chUserMD5PassWord));
}

CGameBizCity::~CGameBizCity()
{
}

void CGameBizCity::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGameBizCity, CDialog)
END_MESSAGE_MAP()


// CGameBizCity ��Ϣ�������

BOOL CGameBizCity::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//���ؼ�
	m_exMain=new CGameFrameExmain;
	m_exMain->Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CRect(0,0,0,0),this,20,NULL);
	m_exMain->Navigate2(&CComVariant("about:blank"),NULL,NULL,NULL,NULL);
	::SetWindowPos(m_exMain->m_hWnd, NULL, 0, 0,960, 720, 0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//������ҳ����
void CGameBizCity::SetGameBizCity(int iUserID, char* chUserData)
{
	if ((NULL != chUserData) && (NULL != m_exMain))
	{
		char chNetData[256]={0};
		sprintf(chNetData, WEB_MY_SHOP, iUserID, chUserData);
		m_exMain->Navigate2(&CComVariant(chNetData),NULL,NULL,NULL,NULL);
	}
}



BEGIN_DISPATCH_MAP(CGameBizCity, CDialog)
	DISP_FUNCTION_ID(CGameBizCity, "oncontextmenu", DISPID_HTMLDOCUMENTEVENTS2_ONCONTEXTMENU, onHtmlContextMenu, VT_BOOL, VTS_DISPATCH)

	DISP_FUNCTION_ID(CGameBizCity, "oncontextMouseDown", DISPID_HTMLDOCUMENTEVENTS2_ONMOUSEDOWN, onHtmlMouseDown, VT_BOOL, VTS_DISPATCH)


END_DISPATCH_MAP()

BEGIN_EVENTSINK_MAP(CGameBizCity, CDialog)
	ON_EVENT(CGameBizCity, 20, 259, OnDocumentCompleteExplorer1, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()



BOOL CGameBizCity::onHtmlContextMenu(IHTMLEventObj *pEvtObj)
{ 
	//�ڳɹ��������¼�Դ��,ÿ���û��һ���������������,����Ը���pEvtObj���жϵ�ǰ���λ�õ�,Ȼ��������Լ������˵�,��IE�����˵�,����ʲô������...
	return S_OK;
}
BOOL CGameBizCity::onHtmlMouseDown(IHTMLEventObj *pEvtObj)
{
	PostMessage (WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(0,0));
	return S_OK;
}
void CGameBizCity::OnDocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT FAR* URL) 
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
		//AfxMessageBox("�ɹ��ҽ���");
	}
}
