// NewHandTask.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "resource.h"
#include "NewHandTask.h"
#include ".\newhandtask.h"
#include "GlobalUnits.h"
#include "initdata.h"

#define BT_DAILY_TASK				WM_USER+1000						// ÿ������
#define BT_CHALLENGE_TASK		WM_USER+1001						// ��ս����
#define BT_NEW_HAND_TASK		WM_USER+1002						// ��������
#define BT_CLOSE							WM_USER+1003						// �ر�
#define WEB_BROWER					WM_USER+1004						// WEB������

// CNewHandTask �Ի���

IMPLEMENT_DYNAMIC(CNewHandTask, CRgnDialog)
CNewHandTask::CNewHandTask(CWnd* pParent /*=NULL*/)
	: CRgnDialog(CNewHandTask::IDD, pParent)
{
}

CNewHandTask::~CNewHandTask()
{
}

void CNewHandTask::DoDataExchange(CDataExchange* pDX)
{
	CRgnDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNewHandTask, CRgnDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(BT_DAILY_TASK, OnbtnDailyTask)
	ON_BN_CLICKED(BT_CHALLENGE_TASK, OnbtnChallengeTask)
	ON_BN_CLICKED(BT_NEW_HAND_TASK, OnbtnNewHandTask)
	ON_BN_CLICKED(BT_CLOSE, OnbtnClose)
END_MESSAGE_MAP()


// CNewHandTask ��Ϣ�������

BOOL CNewHandTask::OnInitDialog()
{
	CRgnDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString strPath;
	GetImagePlazaPath(strPath);
	LoadRgnImage(strPath + TEXT("background.png"), RGB(255,0,255));

	CImageHandle ImageHandle(&m_ImageBack);
	SetWindowPos(NULL, 0, 0, m_ImageBack.GetWidth(), m_ImageBack.GetHeight(), SWP_NOMOVE|SWP_NOZORDER);

	m_btDailyTask.Create(NULL, WS_VISIBLE|WS_CHILD, CRect(0,0,0,0), this, BT_DAILY_TASK);
	m_btChallengeTask.Create(NULL, WS_VISIBLE|WS_CHILD, CRect(0,0,0,0), this, BT_CHALLENGE_TASK);
	m_btNewHandTask.Create(NULL, WS_VISIBLE|WS_CHILD, CRect(0,0,0,0), this, BT_NEW_HAND_TASK);
	m_btClose.Create(NULL, WS_VISIBLE|WS_CHILD, CRect(0,0,0,0), this, BT_CLOSE);
	
	m_btDailyTask.SetButtonImage(strPath + TEXT("dailytask.png"), FALSE);
	m_btChallengeTask.SetButtonImage(strPath + TEXT("challengetask.png"), FALSE);
	m_btNewHandTask.SetButtonImage(strPath + TEXT("newhandtask.png"), FALSE);
	m_btClose.SetButtonImage(strPath + TEXT("close.png"), FALSE);

	const UINT uFlags=SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	m_btNewHandTask.SetWindowPos(NULL, 18, 9, 0, 0, uFlags);
	m_btChallengeTask.SetWindowPos(NULL, 108, 9, 0, 0, uFlags);
	m_btDailyTask.SetWindowPos(NULL, 198, 9, 0, 0, uFlags);
	m_btClose.SetWindowPos(NULL, 513, 10, 0, 0, uFlags);
	m_btChallengeTask.EnableWindow(FALSE);
	m_btDailyTask.EnableWindow(FALSE);

	m_webBrower.Create(NULL, NULL, WS_CHILD, CRect(0,0,0,0), this, WEB_BROWER);
	m_webBrower.SetWindowPos(NULL, 3, 46, 544, 393, SWP_NOZORDER);
	OnbtnNewHandTask();

	CenterWindow(AfxGetMainWnd());
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CNewHandTask::GetImagePlazaPath(CString& strRetValue)
{
	CString strModuleFileName("");
	TCHAR szPathTemp[MAX_PATH]={0};
	GetCurrentDirectory(MAX_PATH, szPathTemp);
	strModuleFileName.Append(szPathTemp);
	strModuleFileName.Append("\\data\\plaza\\newhandtask\\");
	strRetValue = strModuleFileName;
}

void CNewHandTask::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������

	//�滭��ͼ
	CRect ClientRect;
	GetClientRect(&ClientRect);
	CImageHandle ImageHandle(&m_ImageBack);

	if (m_ImageBack.IsNull()==false)
	{
		m_ImageBack.BitBlt(dc,0,0);
	}
	// ��Ϊ��ͼ��Ϣ���� CRgnDialog::OnPaint()
}

void CNewHandTask::OnbtnDailyTask()
{
}

void CNewHandTask::OnbtnChallengeTask()
{
}

void CNewHandTask::OnbtnNewHandTask()
{
	tagGlobalUserData UserData=g_GlobalUnits.GetGolbalUserData();
	CString strUrl("");
	strUrl.Format(CInitData::s_cLoginTaskNew, UserData.dwUserID, UserData.szKey);

	m_webBrower.Navigate(strUrl,NULL,NULL,NULL,NULL);
	//m_webBrower.Navigate("www.baidu.com",NULL,NULL,NULL,NULL);
}

void CNewHandTask::OnbtnClose()
{
	CRgnDialog::OnOK();
}

BOOL CNewHandTask::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (1792 == pMsg->message)
	{
		m_webBrower.ShowWindow(SW_SHOW);
	}

	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			return TRUE;
		}
	}

	return CRgnDialog::PreTranslateMessage(pMsg);
}
