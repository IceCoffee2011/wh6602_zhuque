// PlazaGameWeb.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PlazaGameWeb.h"
#include ".\plazagameweb.h"
#include "webpath.h"
#include "GlobalUnits.h"
#include "InitData.h"

#define IDI_WEB_BROWSER_COMMON				1109291526					//��ҳ���

#define TIMER_WEB_LOADING_GAMEWEB			1111031633					//������ҳ
// CPlazaGameWeb �Ի���

IMPLEMENT_DYNAMIC(CPlazaGameWeb, CDialog)
CPlazaGameWeb::CPlazaGameWeb(CWnd* pParent /*=NULL*/)
: CDialog(CPlazaGameWeb::IDD, pParent)
{
}

CPlazaGameWeb::~CPlazaGameWeb()
{
}

void CPlazaGameWeb::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPlazaGameWeb, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CPlazaGameWeb ��Ϣ�������

BOOL CPlazaGameWeb::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_WebBrowser.Create(NULL,NULL,WS_VISIBLE | WS_CHILD,CRect(0,0,0,0),this,IDI_WEB_BROWSER_COMMON,NULL);
	m_WebBrowser.MoveWindow(0, 0, 536, 360);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPlazaGameWeb::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}

//��ǰ·��
void CPlazaGameWeb::GetCurrentDirectoryPath(CString &szPath)
{
	szPath="";
	TCHAR szPathTemp[MAX_PATH]={0};
	GetCurrentDirectory(MAX_PATH, szPathTemp);
	szPath.Append(szPathTemp);
}

//������Դ·��
void CPlazaGameWeb::GetImagePlazaPath(CString& strRetValue)
{
	CString strModuleFileName("");
	GetCurrentDirectoryPath(strModuleFileName);
	strModuleFileName.Append("\\data\\plaza\\");
	strRetValue = strModuleFileName;
}
//��Դ·��
void CPlazaGameWeb::GetResPath(CString strClipName, CString& strPath)
{
	CString strModuleFileName("");
	GetCurrentDirectoryPath(strModuleFileName);
	strModuleFileName.Append(strClipName);
	strPath = strModuleFileName;
}

//����������Ϸ��ҳ
bool CPlazaGameWeb::LoadPlazaGameWeb(UINT uiType)
{
	//���ر���ҳ��
	CString strUrlPath("");
	GetResPath("\\Url\\plaza\\",strUrlPath);
	m_WebBrowser.Navigate(TEXT(strUrlPath+"webBrowserCenter.html"),NULL,NULL,NULL,NULL);

	tagGlobalUserData UserData=g_GlobalUnits.GetGolbalUserData();
	CString strUrl("");
	switch (uiType)
	{
	case 1://�ʺų�ֵ
		strUrl.Format(CInitData::s_cLoginPayment, UserData.dwUserID, UserData.szKey);
		break;
	case 2://��Ʒ�һ�
		strUrl.Format(CInitData::s_cLoginExchange, UserData.dwUserID, UserData.szKey);
		break;
	case 3://�����̳�
		strUrl.Format(CInitData::s_cLoginPropShop, UserData.dwUserID, UserData.szKey);
		break;
	case 4://��������
		strUrl.Format(CInitData::s_cLoginTaskNew, UserData.dwUserID, UserData.szKey);
		break;
	case 5://����
		strUrl.Format(CInitData::s_cLoginBackpack, UserData.dwUserID, UserData.szKey);
		break;
	case 6://�û���Ϣ
		strUrl.Format(CInitData::s_cLoginUserInfo, UserData.dwUserID, UserData.szKey);
		break;
	case 7://����
		strUrl.Format(CInitData::s_cLoginBank, UserData.dwUserID, UserData.szKey);
		break;
	case 8://�ʼ�
		strUrl.Format(CInitData::s_cLoginSysMessage, UserData.dwUserID, UserData.szKey);
		break;
	case 9://������(��Ϸ����)
		strUrl.Format(CInitData::s_cGameHelpSingle, 10);
		break;
	case 10://��ɳ�齫(��Ϸ����)
		strUrl.Format(CInitData::s_cGameHelpSingle, 303);
		break;
	case 11://�����齫(��Ϸ����)
		strUrl.Format(CInitData::s_cGameHelpSingle, 310);
		break;
	default:
		return true;
	}
	m_WebBrowser.Navigate(strUrl,NULL,NULL,NULL,NULL);

	//m_WebBrowser.Navigate("file://C:\\test.html",NULL,NULL,NULL,NULL);
	return true;
}

BOOL CPlazaGameWeb::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

BOOL CPlazaGameWeb::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message==WM_KEYDOWN)
	{
		if (pMsg->wParam==VK_ESCAPE || pMsg->wParam==VK_RETURN)
		{
			return true;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
