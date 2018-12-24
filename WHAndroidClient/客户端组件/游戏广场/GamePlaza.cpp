#include "Stdafx.h"
#include "GamePlaza.h"
//#include "GameFrame.h"
#include "GlobalUnits.h"
#include "PlazaDlg.h"
#include "CustSite.h"
#include ".\gameplaza.h"
#include "DlgLogon.h"


#define REGEDIT_PLAZA (TEXT("Software\\WHQPGame\\Plaza"))
#define REGEDIT_PLAZA_PATH (TEXT("Path"))
#define REGEDIT_OptionLogon (TEXT("Software\\WHQPGame\\GamePlaza\\OptionLogon"))
#define REGEDIT_LogonServer (TEXT("Software\\WHQPGame\\GamePlaza\\LogonServer"))


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGamePlazaApp, CWinApp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//Ӧ�ó������˵��
CGamePlazaApp theApp;

//���캯��
CGamePlazaApp::CGamePlazaApp()
{
}

//��ʼ������
BOOL CGamePlazaApp::InitInstance()
{
	__super::InitInstance();

	UpdateRegedit();

#ifndef _DEBUG
#ifndef	ADMIN_PLAZA				//���û���壬���ܶ࿪�汾
	//   ��Ӧ�ó����������ź���     
	HANDLE   hSem   =   ::CreateSemaphore(NULL,   1,   1,   MAIN_DLG_CLASSNAME);     
	//   �ź����Ѵ��ڣ�     
	//   �ź������ڣ����������һ��ʵ������     
	if   (GetLastError()   ==   ERROR_ALREADY_EXISTS)     
	{
		//   �ر��ź������ 
		::CloseHandle(hSem);
		//   Ѱ����ǰʵ����������     
		HWND   hWndPrevious   =   ::GetWindow(::GetDesktopWindow(),GW_CHILD);     
		while   (::IsWindow(hWndPrevious))     
		{     
			//   ��鴰���Ƿ���Ԥ��ı��?     
			//   �У���������Ѱ�ҵ�����     
			if   (::GetProp(hWndPrevious,   MAIN_DLG_CLASSNAME))     
			{     
				//   ����������С������ָ����С     
				if   (::IsIconic(hWndPrevious))     
					::ShowWindow(hWndPrevious,SW_RESTORE);     

				//   ����������     
				::SetForegroundWindow(hWndPrevious);     

				//   �������ĶԻ��򼤻�     
				::SetForegroundWindow(     
					::GetLastActivePopup(hWndPrevious));     

				//   �˳���ʵ��     
				return   0;     
			}     
			//   ����Ѱ����һ������     
			hWndPrevious   =   ::GetWindow(hWndPrevious,GW_HWNDNEXT);   
		}
	}
#endif
#endif
//
//#ifndef ADMIN_PLAZA
//#ifndef _DEBUG
//	bool bExistIGame=false;
//	CMutex Mutex(FALSE,MAIN_DLG_CLASSNAME,NULL);
//	if (Mutex.Lock(0)==FALSE) bExistIGame=true;
//	CWnd * pIGameWnd=CWnd::FindWindow(MAIN_DLG_CLASSNAME,NULL);
//	if (pIGameWnd!=NULL)
//	{
//		bExistIGame=true;
//		if (pIGameWnd->IsIconic()) pIGameWnd->ShowWindow(SW_RESTORE);
//		pIGameWnd->SetActiveWindow();
//		pIGameWnd->BringWindowToTop();
//		pIGameWnd->SetForegroundWindow();
//	}
//	if (bExistIGame==true) return FALSE;
//#endif
//#endif

	GdiplusStartupInput StartupInput;
	GdiplusStartup(&m_lGdiPlusToken,&StartupInput,NULL);

	//�����ʼ��
	AfxOleInit();
	AfxInitRichEdit();
	AfxInitRichEdit2();
	InitCommonControls();
	AfxEnableControlContainer();
	
	CCustomOccManager *pMgr = new CCustomOccManager;//br
	AfxEnableControlContainer( pMgr );

	//����ע���
	SetRegistryKey(szSystemRegKey);


	//��������
	g_GlobalOption.LoadOptionParameter();

	//ȫ����Ϣ
	if (g_GlobalUnits.InitGlobalUnits()==false)
	{
		ShowInformation(TEXT("��Ϸ�㳡��ʼ��ʧ�ܣ����򼴽��˳���"),0,MB_ICONSTOP);
		return FALSE;
	}

	//��������
	WNDCLASS WndClasss;
	ZeroMemory(&WndClasss,sizeof(WndClasss));

	//ע�ᴰ��
	WndClasss.style=CS_DBLCLKS;
	WndClasss.lpfnWndProc=DefWindowProc;
	WndClasss.lpszClassName=MAIN_DLG_CLASSNAME;
	WndClasss.hInstance=AfxGetInstanceHandle();
	WndClasss.hIcon=LoadIcon(MAKEINTRESOURCE(IDR_MAINFRAMEICO));
	WndClasss.hCursor=LoadStandardCursor(MAKEINTRESOURCE(IDC_ARROW));
	if (AfxRegisterClass(&WndClasss)==FALSE) AfxThrowResourceException();

	CStringArray arr;
	GetCmdLinePara(arr);//���������

	if (2 == arr.GetSize())
	{
		CString strAccount=arr.GetAt(0);
		strAccount.Replace("/account:", "");
		CString strPW=arr.GetAt(1);
		strPW.Replace("/pw:", "");
		arr.RemoveAll();
		arr.Add(strAccount);
		arr.Add(strPW);
		g_GlobalUnits.SaveApplicationCmdLine(arr);
	}

	
	//��������
	//CGameFrame * pMainFrame=new CGameFrame();
	//pMainFrame->Create(MAIN_DLG_CLASSNAME,TEXT(""),/*WS_MINIMIZEBOX|WS_CLIPCHILDREN|WS_CLIPSIBLINGS*/WS_OVERLAPPEDWINDOW,CRect(0,0,0,0));
	//m_pMainWnd=pMainFrame;

	//cxf
	//������Ϸ����
	m_pPlazaDlg = NULL;
	m_pPlazaDlg = new CPlazaDlg();
	m_external.m_palazaDlg=m_pPlazaDlg;
	if (NULL != m_pPlazaDlg)
	{
		m_pPlazaDlg->Create(IDD_PLAZADLG);
		m_pPlazaDlg->ShowWindow(SW_HIDE);
	}
	else
	{
		m_pPlazaDlg = NULL;
		return TRUE;
	}

	//������Ϸ��¼����
	m_pDlgLogon = NULL;
	m_pDlgLogon = new CDlgLogon();
	if (NULL != m_pDlgLogon)
	{
		m_pDlgLogon->Create(IDD_LOGON);
		m_pPlazaDlg->SetDlgLogonPtr(m_pDlgLogon);
	}
	else
	{
		m_pDlgLogon = NULL;
		return TRUE;
	}

	//���������� 
	m_pMainWnd = m_pPlazaDlg;
	
	//��ʾ����
	m_pPlazaDlg->ShowTab(0);

#ifndef	ADMIN_PLAZA				//���û���壬���ܶ࿪�汾
	::SetProp(m_pMainWnd->m_hWnd, MAIN_DLG_CLASSNAME, (HANDLE)1);
#endif

	/*cxf 2011-09-20
	CPlazaDlg dlg; 
	m_external.m_palazaDlg=&dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
		//  �Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//  �Ի���Ĵ���
	}
	*/

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//�˳�����
void CGamePlazaApp::ExitGame()
{
	if (NULL != m_pDlgLogon)
	{
		m_pDlgLogon->ExitLogon();
	}
	if (NULL != m_pPlazaDlg)
	{
		m_pPlazaDlg->OnBnCancelGame();
	}

	if (NULL != m_pDlgLogon)
	{
		delete m_pDlgLogon;
	}
	m_pDlgLogon = NULL;

	if (NULL != m_pPlazaDlg)
	{
		delete m_pPlazaDlg;
	}
	m_pPlazaDlg = NULL;
}

//�˳�����
void CGamePlazaApp::ExitGamePlaza()
{
	if (NULL != m_pDlgLogon)
	{
		m_pDlgLogon->ExitLogon();
	}
	if (NULL != m_pDlgLogon)
	{
		delete m_pDlgLogon;
	}
	m_pDlgLogon = NULL;

	if (NULL != m_pPlazaDlg)
	{
		delete m_pPlazaDlg;
	}
	m_pPlazaDlg = NULL;
}

int CGamePlazaApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	GdiplusShutdown(m_lGdiPlusToken);

	return CWinApp::ExitInstance();
}

void CGamePlazaApp::CallInnoIssMsgSelectGame(WPARAM wParam, LPARAM lParam)
{
	if (NULL != m_pPlazaDlg)
	{
		m_pPlazaDlg->OnCallInnoIssMsgSelectGame(wParam, lParam);
	}
}
void CGamePlazaApp::UpdateRegedit()
{
	DWORD dwType;
	TCHAR ch[MAX_PATH] = {TEXT("")};
	DWORD dwCount = sizeof(ch);
	GetModuleFileName(NULL, ch, sizeof(ch))  ;
	TCHAR * pCH = strrchr(ch, '\\');
	if (NULL != pCH)
	{
		*pCH = '\0';
	}
	SetCurrentDirectory(ch);

	HKEY hAppKey = NULL;
	HKEY hSoftKey = NULL;
	HKEY hPathKey = NULL;
	if (RegOpenKeyEx(HKEY_CURRENT_USER, REGEDIT_PLAZA, 0, KEY_WRITE|KEY_READ,&hSoftKey) != ERROR_SUCCESS)
	{
		if (RegCreateKey(HKEY_CURRENT_USER, REGEDIT_PLAZA, &hSoftKey) != ERROR_SUCCESS)
		{
			return;
		}
	}

// 	wchar_t * pCh = wcsrchr(ch, '\\');
// 	if (NULL != pCh) *pCh = '\0';
	
	if (NULL != hSoftKey)
	{
		if (RegSetValueEx(hSoftKey, REGEDIT_PLAZA_PATH, NULL, REG_SZ, (LPBYTE)ch, sizeof(ch)) != ERROR_SUCCESS)
		{
			AfxMessageBox(TEXT("дע���ʧ��~��"));
		}
	}
	if (hSoftKey != NULL)
		RegCloseKey(hSoftKey);
	if (hPathKey != NULL)
		RegCloseKey(hPathKey);
}

//�ַ����ֽ�
void CGamePlazaApp::GetCmdLinePara(CStringArray &paraArr)
{
	paraArr.RemoveAll();
	CString strLine=::AfxGetApp()->m_lpCmdLine;
	if(strLine.IsEmpty())
		return;

	int nLength=strLine.GetLength();
	char *buf=new char[nLength+1];
	lstrcpyn(buf, strLine, sizeof(char)*(nLength+1));
	char *p=buf;
	for(int i=0;i<128;i++)
	{
		if(buf[i]==0x20)//�ո�
		{
			buf[i]=0x00;
			paraArr.Add(p);
			i++;
			p=buf+i;
		}
		if(buf[i]==0x00)
		{
			paraArr.Add(p);
			break;
		}
	}

	SafeDeleteArray(buf);
}