#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include ".\gameclient.h"

//////////////////////////////////////////////////////////////////////////

//�������
CGameClientApp theApp;

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientApp::CGameClientApp()
{
	//AfxMessageBox("ok");
	//#if _DEBUG
	//	DebugBreak();	// ������
	//#endif
}

//��������
CGameClientApp::~CGameClientApp()
{
}

//////////////////////////////////////////////////////////////////////////

BOOL CGameClientApp::InitInstance()
{
	//���ù���Ŀ¼
	ZeroMemory(g_szWorkDir,sizeof(g_szWorkDir));
	GetModuleFileName(AfxGetInstanceHandle(),g_szWorkDir,sizeof(g_szWorkDir));
	int nModuleLen=lstrlen(g_szWorkDir);
	int nProcessLen=lstrlen(AfxGetApp()->m_pszExeName)+lstrlen(TEXT(".EXE"));
	g_szWorkDir[nModuleLen-nProcessLen]=0;
	SetCurrentDirectory(g_szWorkDir);
	if ('\\'!=g_szWorkDir[lstrlen(g_szWorkDir)-1]) g_szWorkDir[lstrlen(g_szWorkDir)-1]='\\';

	return CGameFrameApp::InitInstance();
}
