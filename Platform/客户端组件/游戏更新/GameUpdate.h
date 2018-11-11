// GameUpdate.h : main header file for the GameUpdate application
//

#if !defined(AFX_GameUpdate_H__187D3C3A_DD05_4283_9FCA_1FAF58B23B29__INCLUDED_)
#define AFX_GameUpdate_H__187D3C3A_DD05_4283_9FCA_1FAF58B23B29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "MultiLanguage.h"

// �����в�����
#define PARA_KEY_APP_NAME "/AppName="
#define PARA_KEY_CURRENTVERSION "/CurrentVersion="
#define PARA_KEY_CHECKURL "/CheckURL="
#define PARA_KEY_NOTIFYWINDOW "/NotifyWindow="
#define PARA_KEY_NOTIFYWINDOWTITLE "/NotifyWindowTitle="
#define PARA_KEY_NOTIFYFINISH "/NotifyFinish="
#define PARA_KEY_SILENCE "/Silence="


// ���������ļ��е�������
#define SECTION_UPDATE "UPDATE"
#define SECTION_COMMON "COMMON"
#define SECTION_LANGUAGE "LANGUAGE"


/////////////////////////////////////////////////////////////////////////////
// CGameUpdateApp:
// See GameUpdate.cpp for the implementation of this class
//

class CGameUpdateApp : public CWinApp
{
public:
	CGameUpdateApp();
	void InitStringTable(enum enLANGUAGE Language = LANGUAGE_BOTTOM);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameUpdateApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGameUpdateApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	//BOOL ParseCommandLine();			// ����������
	BOOL IsAppRunning();
	//BOOL CheckUpdate();					// �������վ�������������ļ�������Ƿ����°汾���������
	void GetCmdLinePara(CStringArray &paraArr);

private:
	CString m_sAppName;					// Ӧ�ó����������Դ���������
	//CString m_sVersion;					// Ӧ�ó���ĵ�ǰ�汾
	//CString m_sURL;						// �������������ļ���URL
	//DWORD m_iNotifyWindow;				// ���������з�����Ϣ��Ŀ�괰�ھ��
	//CString m_sNotifyWindowTitle;		// ���������з�����Ϣ��Ŀ�괰�ڱ���
	//DWORD m_iNotifyFinish;				// ������Ϸ��͵���Ϣ����
	BOOL m_bSilenceMode;				// ��Ĭ��ʽִ������������ʾ����������棬ֻ��������Ϻ������û�
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GameUpdate_H__187D3C3A_DD05_4283_9FCA_1FAF58B23B29__INCLUDED_)
