//------------------------------------------------------------------------------
// �ļ����ƣ�RMessageBox.cpp
// �ļ��汾��v1.0
// �������ڣ�2006-05-06 04:25
// ��    �ߣ�Richard Ree
// �ļ��������Զ�����Ϣ����ʵ���ļ�
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameUpdate.h"
#include "RMessageBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CRMessageBox �Ի���

IMPLEMENT_DYNAMIC(CRMessageBox, CDialog)

CRMessageBox::CRMessageBox(CWnd* pParent /*=NULL*/)
	: CRDialog(CRMessageBox::IDD, pParent)
{
	m_iID = MB_OK;
	m_bOption1 = FALSE;
	m_bOption2 = FALSE;
}

CRMessageBox::~CRMessageBox()
{
}

void CRMessageBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_ICON, m_stcIcon);
	DDX_Control(pDX, IDC_STATIC_PROMPT, m_stcPromptMessage);
	DDX_Control(pDX, IDC_CHECK_OPTION1, m_chkOption1);
	DDX_Check(pDX, IDC_CHECK_OPTION1, m_bOption1);
	DDX_Control(pDX, IDC_CHECK_OPTION2, m_chkOption2);
	DDX_Check(pDX, IDC_CHECK_OPTION2, m_bOption2);
	DDX_Control(pDX, IDC_BUTTON_OK, m_btnOK);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_ABORT, m_btnAbort);
	DDX_Control(pDX, IDC_BUTTON_IGANORE, m_btnIganore);
	DDX_Control(pDX, IDC_BUTTON_RETRY, m_btnRetry);
	DDX_Control(pDX, IDC_BUTTON_CONTINUE, m_btnContinue);
	DDX_Control(pDX, IDC_BUTTON_YES, m_btnYes);
	DDX_Control(pDX, IDC_BUTTON_NO, m_btnNo);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_btClose);
}

BOOL CRMessageBox::OnInitDialog()
{
	CRDialog::OnInitDialog();

	// ���öԻ������
	SetWindowText(_T("��Ϣ��ʾ")/*m_sTitle*/);

	m_btnOK.SetWindowText(STRING(STR_BUTTON_OK, "ȷ��(&O)"));
	m_btnCancel.SetWindowText(STRING(STR_BUTTON_CANCEL, "ȡ��(&C)"));
	m_btnAbort.SetWindowText(STRING(STR_BUTTON_ABORT, "����(&A)"));
	m_btnIganore.SetWindowText(STRING(STR_BUTTON_IGANORE, "����(&I)"));
	m_btnRetry.SetWindowText(STRING(STR_BUTTON_RETRY, "����(&R)"));
	m_btnContinue.SetWindowText(STRING(STR_BUTTON_CONTINUE, "����(&C)"));
	m_btnYes.SetWindowText(STRING(STR_BUTTON_YES, "��(&Y)"));
	m_btnNo.SetWindowText(STRING(STR_BUTTON_NO, "��(&N)"));
	m_btClose.SetWindowText(_T("ǿ�ƹر�(&C)"));

	// �ж��Ƿ���Ҫ��ʾ��ѡ��
	if (!m_sOptionPromptMessage1.IsEmpty())
	{
		m_chkOption1.SetWindowText(m_sOptionPromptMessage1);
		m_chkOption1.ShowWindow(SW_SHOW);
	}
	else
	{
		m_chkOption1.ShowWindow(SW_HIDE);
	}
	if (!m_sOptionPromptMessage2.IsEmpty())
	{
		m_chkOption2.SetWindowText(m_sOptionPromptMessage2);
		m_chkOption2.ShowWindow(SW_SHOW);
	}
	else
	{
		m_chkOption2.ShowWindow(SW_HIDE);
	}

	// ͼ��ؼ�����Ϊռλ�����ã�����ʾ
	m_stcIcon.ShowWindow(SW_HIDE);

	UpdateData(FALSE);

	// ���öԻ����С����ťλ��
	ResetMessageBox();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CRMessageBox::OnOK()
{
	CWnd *pActiveControl = GetFocus();
	if (*pActiveControl == m_btnOK)
	{
		OnBnClickedOk();
	}
	else if (*pActiveControl == m_btnCancel)
	{
		OnBnClickedCancel();
	}
	else if (*pActiveControl == m_btnAbort)
	{
		OnBnClickedAbort();
	}
	else if (*pActiveControl == m_btnIganore)
	{
		OnBnClickedIganore();
	}
	else if (*pActiveControl == m_btnRetry)
	{
		OnBnClickedRetry();
	}
	else if (*pActiveControl == m_btnContinue)
	{
		OnBnClickedContinue();
	}
	else if (*pActiveControl == m_btnYes)
	{
		OnBnClickedYes();
	}
	else if (*pActiveControl == m_btnNo)
	{
		OnBnClickedNo();
	}
	else if (*pActiveControl == m_btClose)
	{
		OnBnClickedClose();
	}

//	CDialog::OnOK();
}

void CRMessageBox::OnCancel()
{
	// ������Ϣ����ʽ�԰���ESC�����رնԻ���ť�����Ľ����������
	switch (m_iType & 0xF)
	{
	case MB_OK:
		m_iID = IDOK;
		break;
	case MB_OKCANCEL:
		m_iID = IDCANCEL;
		break;
	case MB_YESNO:
		m_iID = IDNO;
		break;
	case MB_YESNOCANCEL:
		m_iID = IDCANCEL;
		break;
	case MB_RETRYCANCEL:
		m_iID = IDCANCEL;
		break;
//	case MB_CANCELTRYCONTINUE:
//		m_iID = IDCANCEL;
//		break;
	case MB_ABORTRETRYIGNORE:
		m_iID = IDABORT;
		break;
	case MB_CLOSERETRYCANCEL:
		m_iID = IDYES;
		break;
	default:
		m_iID = IDCANCEL;
		break;
	}
	CDialog::OnCancel();
}


BEGIN_MESSAGE_MAP(CRMessageBox, CDialog)
	RDIALOG_UI_MESSAGE_MAP
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_ABORT, OnBnClickedAbort)
	ON_BN_CLICKED(IDC_BUTTON_IGANORE, OnBnClickedIganore)
	ON_BN_CLICKED(IDC_BUTTON_RETRY, OnBnClickedRetry)
	ON_BN_CLICKED(IDC_BUTTON_CONTINUE, OnBnClickedContinue)
	ON_BN_CLICKED(IDC_BUTTON_YES, OnBnClickedYes)
	ON_BN_CLICKED(IDC_BUTTON_NO, OnBnClickedNo)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnBnClickedClose)
END_MESSAGE_MAP()

// CRMessageBox ��Ϣ�������

void CRMessageBox::OnPaint()
{
	if (!IsWindowVisible() || IsIconic())
	{
		CDialog::OnPaint();
		return;
	}

	// ���ƴ���
	CRDialog::OnPaint();

	// ��ʾͼ��
	HICON hIcon;
	CRect rcIcon;
	POINT point;

	m_stcIcon.GetWindowRect(&rcIcon);
	ScreenToClient(&rcIcon);
	point.x = rcIcon.left;
	point.y = rcIcon.top;

	// ������Ϣ�����ͼ��ز�ͬ��ͼ��
	switch (m_iType & 0xF0)
	{
	case MB_ICONHAND: // MB_ICONERROR/MB_ICONSTOP
		hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_HAND));
		break;
	case MB_ICONQUESTION:
		hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_QUESTION));
		break;
	case MB_ICONEXCLAMATION: // MB_ICONWARNING
		hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_WARNING));
		break;
	case MB_ICONINFORMATION: // MB_ICONASTERISK
		hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_INFORMATION));
		break;
	case MB_USERICON:
	default:
		hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_INFORMATION));
		break;
	}

	// ��ͼ��
	CDC *pDC = GetDC();
	pDC->DrawIcon(point, hIcon);
	ReleaseDC(pDC);
}

void CRMessageBox::OnBnClickedOk()
{
	m_iID = IDOK;

	// ���ش���
	CDialog::OnOK();
}

void CRMessageBox::OnBnClickedCancel()
{
	m_iID = IDCANCEL;

	// ���ش���
	CDialog::OnOK();
}

void CRMessageBox::OnBnClickedAbort()
{
	m_iID = IDABORT;

	// ���ش���
	CDialog::OnOK();
}

void CRMessageBox::OnBnClickedIganore()
{
	m_iID = IDIGNORE;

	// ���ش���
	CDialog::OnOK();
}

void CRMessageBox::OnBnClickedRetry()
{
	m_iID = IDRETRY;

	// ���ش���
	CDialog::OnOK();
}

void CRMessageBox::OnBnClickedContinue()
{
//	m_iID = IDCONTINUE;

	// ���ش���
	CDialog::OnOK();
}

void CRMessageBox::OnBnClickedYes()
{
	m_iID = IDYES;

	// ���ش���
	CDialog::OnOK();
}

void CRMessageBox::OnBnClickedNo()
{
	m_iID = IDNO;

	// ���ش���
	CDialog::OnOK();
}

void CRMessageBox::OnBnClickedClose()
{
	m_iID = IDYES;

	// ���ش���
	CDialog::OnOK();
}

// ������Ϣ����ʽ�����
void CRMessageBox::ResetMessageBox()
{
	// ������Ϣ�Ķ������öԻ��򴰿ڴ�С
	CRect rcWnd;
	CRect rcPromptMessage;
	CRect rcButton;
	CSize size;
	CDC* pDC;
	int iOffsetY; // ��ť�ؼ���Ҫ�ڴ�ֱ�����ϵ���λ����

	GetWindowRect(&rcWnd);

	// ������ʾ���ֵĶ��ټ��㴰�ڴ�С
	pDC = m_stcPromptMessage.GetDC();
	size = pDC->GetTextExtent(m_sPromptMessage);
	m_stcPromptMessage.ReleaseDC(pDC);
	if (size.cx < 200)
	{
		size.cx = 200;
	}
	else if (size.cx > 500)
	{
		size.cy = size.cx / 500 * size.cy;
		size.cx = 500;
	}
	if (size.cy < 32)
	{
		size.cy = 32;
	}
	rcWnd.right = rcWnd.left + size.cx + 100;
	rcWnd.bottom = rcWnd.top + size.cy + 155;
	// ���Ҫ��ʾ��ѡ��Ҫ���ϸ�ѡ��ĸ߶�
	if (!m_sOptionPromptMessage1.IsEmpty())
	{
		rcWnd.bottom += 18;
	}
	if (!m_sOptionPromptMessage2.IsEmpty())
	{
		rcWnd.bottom += 18;
	}

	// �������ƶ�����Ļ�м�
	CRect rcScreen;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcScreen, 0); // ȡ����Ļ��С
	rcWnd.OffsetRect(rcScreen.CenterPoint() - rcWnd.CenterPoint());

	MoveWindow(&rcWnd);

	// ������ʾ��Ϣ��ǩ�Ĵ�С
	m_stcPromptMessage.GetWindowRect(&rcPromptMessage);
	ScreenToClient(&rcPromptMessage);
	rcPromptMessage.right = rcPromptMessage.left + size.cx;
	rcPromptMessage.bottom = rcPromptMessage.top + size.cy + 5;
	m_stcPromptMessage.MoveWindow(&rcPromptMessage);

	// ���ø�ѡ��ؼ��Ĵ�С
	m_chkOption1.GetWindowRect(&rcPromptMessage);
	ScreenToClient(&rcPromptMessage);
	rcPromptMessage.right = rcPromptMessage.left + size.cx;
	m_chkOption1.MoveWindow(&rcPromptMessage);
	m_chkOption2.GetWindowRect(&rcPromptMessage);
	ScreenToClient(&rcPromptMessage);
	rcPromptMessage.right = rcPromptMessage.left + size.cx;
	m_chkOption2.MoveWindow(&rcPromptMessage);

	// ��ʾ��ʾ��Ϣ
	m_stcPromptMessage.SetWindowText(m_sPromptMessage);

	// ����������ʾ��ͬ�İ�ť�����������ƶ����Ի����м�
	m_btnOK.GetWindowRect(&rcButton);
	ScreenToClient(&rcButton);
	iOffsetY = rcWnd.Height() - rcButton.top - 48;
	switch (m_iType & 0xF)
	{
	case MB_OK:
		m_btnOK.ShowWindow(SW_SHOW);
		rcButton.OffsetRect((rcWnd.Width() - rcButton.Width()) / 2 - rcButton.left, iOffsetY);
		m_btnOK.MoveWindow(&rcButton);
		break;
	case MB_OKCANCEL:
		m_btnOK.ShowWindow(SW_SHOW);
		m_btnCancel.ShowWindow(SW_SHOW);
		rcButton.OffsetRect(rcWnd.Width() / 2 - rcButton.Width() - 2 - rcButton.left, iOffsetY);
		m_btnOK.MoveWindow(&rcButton);
		rcButton.OffsetRect(rcButton.Width() + 4, 0);
		m_btnCancel.MoveWindow(&rcButton);
		break;
	case MB_YESNO:
		m_btnYes.ShowWindow(SW_SHOW);
		m_btnNo.ShowWindow(SW_SHOW);
		rcButton.OffsetRect(rcWnd.Width() / 2 - rcButton.Width() - 2 - rcButton.left, iOffsetY);
		m_btnYes.MoveWindow(&rcButton);
		rcButton.OffsetRect(rcButton.Width() + 4, 0);
		m_btnNo.MoveWindow(&rcButton);
		break;
	case MB_YESNOCANCEL:
		m_btnYes.ShowWindow(SW_SHOW);
		m_btnNo.ShowWindow(SW_SHOW);
		m_btnCancel.ShowWindow(SW_SHOW);
		rcButton.OffsetRect((rcWnd.Width() - rcButton.Width()) / 2 - 2 - rcButton.Width() - rcButton.left, iOffsetY);
		m_btnYes.MoveWindow(&rcButton);
		rcButton.OffsetRect(rcButton.Width() + 4, 0);
		m_btnNo.MoveWindow(&rcButton);
		rcButton.OffsetRect(rcButton.Width() + 4, 0);
		m_btnCancel.MoveWindow(&rcButton);
		break;
	case MB_RETRYCANCEL:
		m_btnRetry.ShowWindow(SW_SHOW);
		m_btnCancel.ShowWindow(SW_SHOW);
		rcButton.OffsetRect(rcWnd.Width() / 2 - rcButton.Width() - 2 - rcButton.left, iOffsetY);
		m_btnRetry.MoveWindow(&rcButton);
		rcButton.OffsetRect(rcButton.Width() + 4, 0);
		m_btnCancel.MoveWindow(&rcButton);
		break;
//	case MB_CANCELTRYCONTINUE:
//		m_btnCancel.ShowWindow(SW_SHOW);
//		m_btnContinue.ShowWindow(SW_SHOW);
//		rcButton.OffsetRect(rcWnd.Width() / 2 - rcButton.Width() - 2 - rcButton.left, iOffsetY);
//		m_btnCancel.MoveWindow(&rcButton);
//		rcButton.OffsetRect(rcButton.Width() + 4, 0);
//		m_btnContinue.MoveWindow(&rcButton);
//		break;
	case MB_ABORTRETRYIGNORE:
		m_btnAbort.ShowWindow(SW_SHOW);
		m_btnRetry.ShowWindow(SW_SHOW);
		m_btnIganore.ShowWindow(SW_SHOW);
		rcButton.OffsetRect((rcWnd.Width() - rcButton.Width()) / 2 - 2 - rcButton.Width() - rcButton.left, iOffsetY);
		m_btnAbort.MoveWindow(&rcButton);
		rcButton.OffsetRect(rcButton.Width() + 4, 0);
		m_btnRetry.MoveWindow(&rcButton);
		rcButton.OffsetRect(rcButton.Width() + 4, 0);
		m_btnIganore.MoveWindow(&rcButton);
		break;
	case MB_CLOSERETRYCANCEL:
		m_btClose.ShowWindow(SW_SHOW);
		m_btnRetry.ShowWindow(SW_SHOW);
		m_btnCancel.ShowWindow(SW_SHOW);
		rcButton.OffsetRect((rcWnd.Width() - rcButton.Width()) / 2 - 2 - rcButton.Width() - rcButton.left, iOffsetY);
		m_btClose.MoveWindow(&rcButton);
		rcButton.OffsetRect(rcButton.Width() + 4, 0);
		m_btnRetry.MoveWindow(&rcButton);
		rcButton.OffsetRect(rcButton.Width() + 4, 0);
		m_btnCancel.MoveWindow(&rcButton);
	default:
		break;
	}

	// ͼ�����ʾ��OnPaint������ʵ��
}
