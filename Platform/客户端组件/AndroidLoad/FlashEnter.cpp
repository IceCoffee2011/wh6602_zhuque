// FlashEnter.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "FlashEnter.h"
#include "GlobalUnits.h"
#include "FlashEnterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFlashEnterApp

BEGIN_MESSAGE_MAP(CFlashEnterApp, CWinApp)
END_MESSAGE_MAP()


// CFlashEnterApp ����

CFlashEnterApp::CFlashEnterApp()
{
	// TODO: �ڴ˴���ӹ�����룬
}


// Ψһ��һ�� CFlashEnterApp ����

CFlashEnterApp theApp;


// CFlashEnterApp ��ʼ��

BOOL CFlashEnterApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
	CWinApp::InitInstance();

	//�����ʼ��
	AfxOleInit();
	InitCommonControls();

	g_GlobalUnits.InitGlobalUnits();

	CFlashEnterDlg dlg;
	m_pMainWnd=&dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȷ�������ر�
		//�Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//�Ի���Ĵ���
	}
	return TRUE;
}

int CFlashEnterApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	return CWinApp::ExitInstance();
}
