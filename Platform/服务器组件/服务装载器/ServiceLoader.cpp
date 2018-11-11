#include "Stdafx.h"
#include "ServiceLoader.h"
#include "ServiceLoaderDlg.h"

//Ӧ�ó������
CServiceLoaderApp theApp;

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CServiceLoaderApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CServiceLoaderApp::CServiceLoaderApp() : CWinApp(TEXT("��Ϸ����"))
{
}

//��ʼ������
BOOL CServiceLoaderApp::InitInstance()
{
	__super::InitInstance();

	TCHAR buf[126] = {0};
	CMD5Encrypt::EncryptData("2jiw6lvj82011-11-15 11:43:13192.168.1.91", buf);

	//�ڲ���ʼ��
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();
	SetRegistryKey(szSystemRegKey);

	//���ù���Ŀ¼
	TCHAR szDirWork[MAX_PATH]=TEXT("");
	GetModuleFileName(AfxGetInstanceHandle(),szDirWork,sizeof(szDirWork));
	int nModuleLen=lstrlen(szDirWork);
	int nProcessLen=lstrlen(AfxGetApp()->m_pszExeName)+lstrlen(TEXT(".EXE"));
	szDirWork[nModuleLen-nProcessLen]=0;
	SetCurrentDirectory(szDirWork);

	//��������
	CServiceLoaderDlg ServiceLoaderDlg;
	ServiceLoaderDlg.DoModal();




	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
