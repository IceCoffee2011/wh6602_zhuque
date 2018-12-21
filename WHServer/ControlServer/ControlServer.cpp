#include "Stdafx.h"
#include "ControlServer.h"
#include "ControlServerDlg.h"

//////////////////////////////////////////////////////////////////////////

//Ӧ�ó������
CControlServerApp theApp;

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CControlServerApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CControlServerApp::CControlServerApp()
{
}

//��ʼ������
BOOL CControlServerApp::InitInstance()
{
	__super::InitInstance();

	//��ʼ�����
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();

	//����ע���
	SetRegistryKey(szSystemRegKey);

	//��ȡĿ¼
	GetModuleFileName(AfxGetInstanceHandle(),m_szDirWork,sizeof(m_szDirWork));
	int nModuleLen=lstrlen(m_szDirWork);
	int nProcessLen=lstrlen(m_pszExeName)+lstrlen(TEXT(".EXE"));
	m_szDirWork[nModuleLen-nProcessLen]=0;

	//����Ŀ¼
	lstrcat(m_szDirWork, TEXT("CustomFace"));
	CreateDirectory(m_szDirWork, NULL) ;

	//��ʾ������
	CControlServerDlg ServerDlg;
	m_pMainWnd=&ServerDlg;
	ServerDlg.DoModal();

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
