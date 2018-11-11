//------------------------------------------------------------------------------
// �ļ����ƣ�RDialog.cpp
// �ļ��汾��v1.0
// �������ڣ�2006-05-05 16:36
// ��    �ߣ�Richard Ree
// �ļ��������Ի�Ի�����ʵ���ļ�
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameUpdate.h"
#include "UISkinManager.h"
#include "RDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CRDialog �Ի���

IMPLEMENT_DYNAMIC(CRDialog, CDialog)

CRDialog::CRDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd /*=NULL*/)
	: CDialog(lpszTemplateName, pParentWnd)
{
}

CRDialog::CRDialog(UINT nIDTemplate, CWnd* pParentWnd /*=NULL*/)
	: CDialog(nIDTemplate, pParentWnd)
	, m_btnMinimize(CRButton::ButtonTypeMinimize)
	, m_btnMaximize(CRButton::ButtonTypeMaximize)
	, m_btnClose(CRButton::ButtonTypeClose)
{
}

CRDialog::~CRDialog()
{
}

BOOL CRDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ������С�����Զ������رհ�ť
	CRect rcWnd;
	CRect rcButton;

	GetClientRect(&rcWnd);
	rcButton.SetRect(0, 0, CUISkinManager::m_iSystemControlButtonWidth, CUISkinManager::m_iSystemControlButtonHeight);
	rcButton.OffsetRect(rcWnd.Width() - CUISkinManager::m_iLeftMargin - rcButton.Width(), 6);

	m_btnMinimize.Create("", WS_CHILD, rcButton, this, IDC_BUTTON_MINIMIZE);
	m_btnMaximize.Create("", WS_CHILD, rcButton, this, IDC_BUTTON_MAXIMIZE);
	m_btnClose.Create("", WS_CHILD, rcButton, this, IDC_BUTTON_CLOSE);

	m_btnClose.MoveWindow(rcButton);
	rcButton.OffsetRect(-rcButton.Width() - 2, 0);
	m_btnMaximize.MoveWindow(rcButton);
	rcButton.OffsetRect(-rcButton.Width() - 2, 0);
	m_btnMinimize.MoveWindow(rcButton);

	// ��ʾ/���ذ�ť
	DWORD iWindowStyle;

	iWindowStyle = GetStyle();
	if (iWindowStyle & WS_MINIMIZEBOX)
	{
		m_btnMinimize.ShowWindow(SW_SHOW);
	}
	if (iWindowStyle & WS_MAXIMIZEBOX)
	{
		m_btnMaximize.ShowWindow(SW_SHOW);
	}
//	if (iWindowStyle & WS_CLOSE)
//	{
		m_btnClose.ShowWindow(SW_SHOW);
//	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

// ����������Ϣ
BOOL CRDialog::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (nID == IDC_BUTTON_MINIMIZE)
	{
		OnBnClickedMinimize();
	}
	else if (nID == IDC_BUTTON_MAXIMIZE)
	{
		OnBnClickedMaximize();
	}
	else if (nID == IDC_BUTTON_CLOSE)
	{
		OnBnClickedClose();
	}

	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BEGIN_MESSAGE_MAP(CRDialog, CDialog)
//	RDIALOG_UI_MESSAGE_MAP
	ON_BN_CLICKED(IDC_BUTTON_MINIMIZE, OnBnClickedMinimize)
	ON_BN_CLICKED(IDC_BUTTON_MAXIMIZE, OnBnClickedMaximize)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnBnClickedClose)
END_MESSAGE_MAP()


// CRDialog ��Ϣ�������

void CRDialog::OnPaint()
{
	if (!IsWindowVisible() || IsIconic())
	{
		CDialog::OnPaint();
		return;
	}

	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

	CUISkinManager::Paint(this, &dc);

	// �Ի洰��
	CUISkinManager::PaintDialogBorder(this, &dc);

	// �ػ水ť
//	m_btnMinimize.Invalidate();
//	m_btnMaximize.Invalidate();
//	m_btnClose.Invalidate();

	return;
}

void CRDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// �ƶ���ť
	if (m_btnClose.m_hWnd != NULL)
	{
		CRect rcButton;
		m_btnClose.GetWindowRect(&rcButton);
		rcButton.OffsetRect(cx - CUISkinManager::m_iLeftMargin - rcButton.Width() - rcButton.left, 6 - rcButton.top);
		m_btnClose.MoveWindow(rcButton);
		rcButton.OffsetRect(-rcButton.Width() - 2, 0);
		m_btnMaximize.MoveWindow(rcButton);
		rcButton.OffsetRect(-rcButton.Width() - 2, 0);
		m_btnMinimize.MoveWindow(rcButton);
	}

	CUISkinManager::SetRgn(this);
}

HBRUSH CRDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
//	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
//	return hbr;
	return CUISkinManager::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CRDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// ��������ڱ�������������Ϣģ��ʵ���϶����ڵĹ���
	if (point.y <= (LONG)CUISkinManager::m_iTitleBarHeight)
	{
		SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, 0);
	}

	CDialog::OnLButtonDown(nFlags, point);
}

// �����С����ť
void CRDialog::OnBnClickedMinimize()
{
	// ������С��������Ϣ
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

// �����󻯰�ť
void CRDialog::OnBnClickedMaximize()
{
	if (GetWindowLong(m_hWnd, GWL_STYLE) & WS_MAXIMIZE)
	{
		// �����ǰ���ڴ������״̬�����ͻָ����ڴ�С��Ϣ
		SendMessage(WM_SYSCOMMAND, SC_RESTORE);
	}
	else
	{
		// ������󻯴�����Ϣ
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
	}
}

// ����رհ�ť
void CRDialog::OnBnClickedClose()
{
	// ���͹رմ�����Ϣ
	SendMessage(WM_SYSCOMMAND, SC_CLOSE);
}
