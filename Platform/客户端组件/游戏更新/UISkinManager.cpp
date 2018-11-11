//------------------------------------------------------------------------------
// �ļ����ƣ�UISkinManager.cpp
// �ļ��汾��v1.0
// �������ڣ�2006-02-14 16:59
// ��    �ߣ�Richard Ree
// �ļ�����������������ʵ���ļ�
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameUpdate.h"
#include "UISkinManager.h"

#pragma comment(lib, "msimg32.lib")
#pragma message("Automatically linking with msimg32.lib")

// ͸��ɫ
const COLORREF CUISkinManager::TRANSPARENT_COLOR = RGB(255, 0, 255);

// ��ǰ������
enum CUISkinManager::SkinStyle CUISkinManager::m_iCurrentStyle;

// ���ڱ��������

// �������߶�
UINT CUISkinManager::m_iTitleBarHeight;
// ���Ʊ����������ͼ���ȣ����أ�
UINT CUISkinManager::m_iTitleBarImageWidth;
// ������ͼ����߲��ֵĿ��
UINT CUISkinManager::m_iTitleBarImageLeftPartWidth;
// ������ͼ���м䲿�ֵĿ�ȣ��˲����ǿ�����ģ�
UINT CUISkinManager::m_iTitleBarImageMiddlePartWidth;
// ������ͼ���ұ߲��ֵĿ��
UINT CUISkinManager::m_iTitleBarImageRightPartWidth;
// ���Ʊ����������ͼ�񼯣�ÿ��ͼ����Ϊ m_iTitleBarImageWidth ���أ���������ʧȥ�����˳�����У��ܿ�Ȳ�С�� 2 * m_iTitleBarImageWidth ���أ�
CBitmap CUISkinManager::m_bmTitleBarImage;

// �Ի��򴰿����ұ߾�
UINT CUISkinManager::m_iLeftMargin;
// �Ի��򴰿����±߾�
UINT CUISkinManager::m_iTopMargin;

// ��ť���

// ��ť�߶ȣ����ư�ť�����ͼ��߶ȣ�
UINT CUISkinManager::m_iButtonHeight;
// ���ư�ť�����ͼ���ȣ����أ�
UINT CUISkinManager::m_iButtonImageWidth;
// ��ťͼ����߲��ֵĿ��
UINT CUISkinManager::m_iButtonImageLeftPartWidth;
// ��ťͼ���м䲿�ֵĿ�ȣ��˲����ǿ�����ģ�
UINT CUISkinManager::m_iButtonImageMiddlePartWidth;
// ��ťͼ���ұ߲��ֵĿ��
UINT CUISkinManager::m_iButtonImageRightPartWidth;
// ���ư�ť�����ͼ�񼯣�ÿ��ͼ����Ϊ m_iButtonImageWidth ���أ���������ȱʡ�������ͣ����갴�µ�˳�����У��ܿ�Ȳ�С�� 4 * m_iButtonImageWidth ���أ�
CBitmap CUISkinManager::m_bmButtonImage;
// ϵͳ���ư�ť�Ŀ�Ⱥ͸߶ȣ����أ�
UINT CUISkinManager::m_iSystemControlButtonWidth;
UINT CUISkinManager::m_iSystemControlButtonHeight;
// ����ϵͳ���ư�ť����С������󻯡��رյȰ�ť�������ͼ�񼯣�ÿ��ͼ����Ϊ m_iSystemControlButtonImageWidth ���أ�����С������󻯡��ָ����رա���ť����״̬��˳�����У��ܿ�Ȳ�С�� 8 * m_iSystemControlButtonImageWidth ���أ�
CBitmap CUISkinManager::m_bmSystemControlButtonImage;

// ���ƿؼ��Ļ���
HBRUSH CUISkinManager::m_hbr;
POINT CUISkinManager::m_roundSize;
bool CUISkinManager::m_bGenerate;

CUISkinManager::CUISkinManager()
{
}

CUISkinManager::~CUISkinManager()
{
}

BOOL CUISkinManager::Init(bool bGenerate)
{
	m_hbr = NULL;
	m_bGenerate = bGenerate;

	if (bGenerate)
		m_roundSize.x = m_roundSize.y = 5;
	else
		m_roundSize.x = m_roundSize.y = 0;
		

	if (!LoadImages())
	{
		return FALSE;
	}

	ChangeStyle(StyleSummer);

	return TRUE;
}

void CUISkinManager::Uninit()
{
	if (m_hbr != NULL)
	{
		DeleteObject(m_hbr);
	}

	m_bmTitleBarImage.DeleteObject();
	m_bmButtonImage.DeleteObject();
	m_bmSystemControlButtonImage.DeleteObject();
}

BOOL CUISkinManager::LoadImages()
{
	m_bmTitleBarImage.DeleteObject();
	m_bmButtonImage.DeleteObject();
	m_bmSystemControlButtonImage.DeleteObject();

	switch (m_iCurrentStyle)
	{
	case StyleSummer:
	default:
		m_bmTitleBarImage.LoadBitmap(IDB_TITLEBAR);
		m_bmButtonImage.LoadBitmap(IDB_BUTTON);
		m_bmSystemControlButtonImage.LoadBitmap(IDB_SYSTEM_CONTROL_BUTTON);
		break;
	}

	return TRUE;
}

HBRUSH CUISkinManager::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (pDC == NULL || pWnd == NULL || pWnd->m_hWnd == NULL)
	{
		return NULL;
	}

	if (nCtlColor == CTLCOLOR_EDIT
		|| nCtlColor == CTLCOLOR_MSGBOX
		|| nCtlColor == CTLCOLOR_LISTBOX
		|| nCtlColor == CTLCOLOR_SCROLLBAR)
	{
		return (HBRUSH)GetStockObject(WHITE_BRUSH);
	}

	pDC->SetBkMode(TRANSPARENT);

	return GetBrush();
}

// ���ش��ڱ���ɫ
COLORREF CUISkinManager::GetColor()
{
	switch (m_iCurrentStyle)
	{
	case StyleSpring:
		return RGB(240, 245, 255);
	case StyleSummer:
		return RGB(238, 247, 255);
	case StyleAutumn:
		return RGB(122, 33, 123);
	case StyleWinter:
//		return RGB(225, 233, 240); // ����ɫ
		return RGB(245, 245, 245); // ��ɫ
	case StyleXP:
		return RGB(240, 245, 255);
	case StyleNormal:
	case StyleCustomize:
	default:
		return ::GetSysColor(COLOR_WINDOW);
		break;
	}
}

HBRUSH CUISkinManager::GetBrush()
{
	return m_hbr;
}

// �л�������
void CUISkinManager::ChangeStyle(enum CUISkinManager::SkinStyle iNewStyle)
{
	m_iCurrentStyle = iNewStyle;

	switch (m_iCurrentStyle)
	{
	case StyleSummer:
	default:
		m_iTitleBarHeight = 24;
		m_iTitleBarImageWidth = 64;
		m_iTitleBarImageLeftPartWidth = 15;
		m_iTitleBarImageMiddlePartWidth = 2;
		m_iTitleBarImageRightPartWidth = 15;
		m_iLeftMargin = 8;
		m_iTopMargin = 8;
		m_iButtonHeight = 20;
		m_iButtonImageWidth = 128;
		m_iButtonImageLeftPartWidth = 15;
		m_iButtonImageMiddlePartWidth = 2;
		m_iButtonImageRightPartWidth = 15;
		m_iSystemControlButtonWidth = 16;
		m_iSystemControlButtonHeight = 14;
		break;
	}

	if (m_hbr != NULL)
	{
		DeleteObject(m_hbr);
	}

	m_hbr = ::CreateSolidBrush(GetColor());
}

// �Խ��䷽ʽ������
void CUISkinManager::GradientFillRect(CWnd* pWnd, CDC* pDC, CRect& rcFill, ULONG nMode, COLORREF crLeftTop, COLORREF crRightBottom)
{
	if (pWnd == NULL || pWnd->m_hWnd == NULL)
	{
		return;
	}

	COLOR16 r = (COLOR16)((crLeftTop & 0x000000FF) << 8);
	COLOR16 g = (COLOR16)(crLeftTop & 0x0000FF00);
	COLOR16 b = (COLOR16)((crLeftTop & 0x00FF0000) >> 8);

	TRIVERTEX vert[2] ;
	GRADIENT_RECT gRect;
	vert[0].x     = rcFill.left;
	vert[0].y     = rcFill.top;
	vert[0].Red   = r;
	vert[0].Green = g;
	vert[0].Blue  = b;
	vert[0].Alpha = 0x0000;

	r = (COLOR16) ((crRightBottom & 0x000000FF)<<8);
	g = (COLOR16) (crRightBottom & 0x0000FF00);
	b = (COLOR16) ((crRightBottom & 0x00FF0000)>>8);

	vert[1].x     = rcFill.right;
	vert[1].y     = rcFill.bottom;
	vert[1].Red   = r;
	vert[1].Green = g;
	vert[1].Blue  = b;
	vert[1].Alpha = 0x0000;

	gRect.UpperLeft  = 0;
	gRect.LowerRight = 1;

	GradientFill(pDC->m_hDC, vert, 2, &gRect, 1, nMode);
}

// ͸�����츴��
void CUISkinManager::TransparentBlt(HDC hdcDest, int nXDest, int nYDest, int nWidthDest, int nHeightDest, HDC hdcSrc, int nXSrc, int nYSrc, int nWidthSrc,int nHeightSrc, COLORREF crTransparent)
{
	HBITMAP hbm = CreateCompatibleBitmap(hdcDest, nWidthDest, nHeightDest);
	HBITMAP hbmMemory = CreateBitmap(nWidthDest, nHeightDest, 1, 1, NULL);	
	HDC hdc = CreateCompatibleDC(hdcDest);
	HDC hdcMemory = CreateCompatibleDC(hdcDest);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdc, hbm);
	HBITMAP hbmOldMemory = (HBITMAP)SelectObject(hdcMemory, hbmMemory);

	// step 1: hdcSrc--(copy)-->hdc
	if (nWidthDest == nWidthSrc && nHeightDest == nHeightSrc)
	{
		// ԭ������
		BitBlt(hdc, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXSrc, nYSrc, SRCCOPY);
	}
	else
	{
		// ���츴��
		StretchBlt(hdc, 0, 0, nWidthDest, nHeightDest, hdcSrc, nXSrc, nYSrc, nWidthSrc, nHeightSrc, SRCCOPY);
	}

	// step 2: hdc--(copy)-->hdcMemory
	SetBkColor(hdc, crTransparent);
	BitBlt(hdcMemory, 0, 0, nWidthDest, nHeightDest, hdc, 0, 0, SRCCOPY);

	// step 3: hdcMemory--(and)-->hdc
	SetBkColor(hdc, RGB(0, 0, 0));
	SetTextColor(hdc, RGB(255, 255, 255));
	BitBlt(hdc, 0, 0, nWidthDest, nHeightDest, hdcMemory, 0, 0, SRCAND);

	// step 4: hdcMemory--(and)-->hdcDest
	SetBkColor(hdcDest, RGB(255, 255, 255));
	SetTextColor(hdcDest, RGB(0, 0, 0));
	BitBlt(hdcDest, nXDest, nYDest, nWidthDest, nHeightDest, hdcMemory, 0, 0, SRCAND);

	// step 5: hdc--(paint)-->hdcDest
	BitBlt(hdcDest, nXDest, nYDest, nWidthDest, nHeightDest, hdc, 0, 0, SRCPAINT);

	// �ͷ���Դ
	SelectObject(hdc, hbmOld);
	DeleteDC(hdc);
	SelectObject(hdcMemory, hbmOldMemory);
	DeleteDC(hdcMemory);
	DeleteObject(hbm);
	DeleteObject(hbmMemory);
}

// ���ƴ���
void CUISkinManager::Paint(CWnd* pWnd, CDC* pDC, BOOL bFillClient, COLORREF crFillColor)
{
	if (pWnd == NULL || pWnd->m_hWnd == NULL)
	{
		return;
	}

	BOOL bGetDC = FALSE;

	if (!pDC)
	{
		bGetDC = TRUE;
		pDC = pWnd->GetDC();
	}

	switch (m_iCurrentStyle)
	{
	case StyleXP:
		Paint_XP(pWnd, pDC, bFillClient, crFillColor);
		break;
	default:
		Paint_Normal(pWnd, pDC, bFillClient, crFillColor);
		break;
	}

	if (bGetDC)
	{
		pWnd->ReleaseDC(pDC);
	}
}

void CUISkinManager::Paint_Normal(CWnd* pWnd, CDC* pDC, BOOL bFillClient, COLORREF crFillColor)
{
	if (pWnd == NULL || pWnd->m_hWnd == NULL)
	{
		return;
	}

	CRect rcWnd;
	pWnd->GetClientRect(&rcWnd);

	CDC dcMemory;
	dcMemory.CreateCompatibleDC(pDC);
	CBitmap bmpMemory;
	bmpMemory.CreateCompatibleBitmap(pDC, rcWnd.Width(), rcWnd.Height());
	CBitmap* pOldbmpMemory = (CBitmap*)dcMemory.SelectObject(&bmpMemory);

	// ��䴰�ڱ���ɫ
	dcMemory.FillSolidRect(&rcWnd, GetColor());

	int cx = ::GetSystemMetrics(SM_CXDLGFRAME);
	int cy = ::GetSystemMetrics(SM_CYDLGFRAME);

	CRect rc;
	CRgn rgn;
	pWnd->GetWindowRect(&rc);
	rc.right -= 2 * cx;
	rc.bottom -= 2 * cy;

	// �Ա���ͼ��������
	CRect rcTitleBar;
	rcTitleBar.SetRect(rc.left, rc.top, rc.right, rc.top + m_iTitleBarHeight);
	HDC hdcSrc = CreateCompatibleDC(dcMemory.GetSafeHdc());
	HGDIOBJ hOldGdiObj = SelectObject(hdcSrc, m_bmTitleBarImage);
	// ��
	TransparentBlt(dcMemory.GetSafeHdc(), 0, 0, m_iTitleBarImageLeftPartWidth, m_iTitleBarHeight
		, hdcSrc, 0, 0, m_iTitleBarImageLeftPartWidth, m_iTitleBarHeight, TRANSPARENT_COLOR);
	// �в�
	TransparentBlt(dcMemory.GetSafeHdc(), m_iTitleBarImageLeftPartWidth, 0
		, rcTitleBar.Width() - m_iTitleBarImageLeftPartWidth - m_iTitleBarImageRightPartWidth, m_iTitleBarHeight
		, hdcSrc, m_iTitleBarImageLeftPartWidth, 0
		, m_iTitleBarImageMiddlePartWidth, m_iTitleBarHeight, TRANSPARENT_COLOR);
	// �Ҳ�
	TransparentBlt(dcMemory.GetSafeHdc(), rcTitleBar.Width() - m_iTitleBarImageRightPartWidth, 0
		, m_iTitleBarImageRightPartWidth, m_iTitleBarHeight
		, hdcSrc, m_iTitleBarImageLeftPartWidth + m_iTitleBarImageMiddlePartWidth, 0
		, m_iTitleBarImageRightPartWidth, m_iTitleBarHeight, TRANSPARENT_COLOR);
	SelectObject(hdcSrc, hOldGdiObj);

	// ���ͻ�������ɫ
	pWnd->GetClientRect(&rc);
	rc.OffsetRect(-rc.left, -rc.top);
	rc.DeflateRect(m_iLeftMargin, m_iTitleBarHeight + 8, m_iLeftMargin, m_iTopMargin);
	dcMemory.FillSolidRect(rc.left, rc.top, rc.Width(), rc.Height(), RGB(238, 247, 255));

	// ���԰������ҡ��ױ���
	pWnd->GetClientRect(&rc);
	rgn.CreateRoundRectRgn(0, 0, rc.Width(), rc.Height(), 5, 5);
	CBrush br3(RGB(141, 193, 250));
	dcMemory.FrameRgn(&rgn, &br3, 1, 1); 
	br3.DeleteObject();
	rgn.DeleteObject();

	// �������������

	CFont font;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 18; // �ֺ�
	strncpy(lf.lfFaceName, "Microsoft Sans Serif", 32); // ����
	lf.lfWeight = 600; // ��ϸ
	VERIFY(font.CreateFontIndirect(&lf));

	CString strCaption;
	pWnd->GetWindowText(strCaption);
	CFont* pOldFont = dcMemory.SelectObject(&font);
	dcMemory.SetBkMode(TRANSPARENT);

	// �Իҡ���ɫ��������γ���ӰЧ��
	dcMemory.SetTextColor(RGB(160, 160, 160));
	dcMemory.TextOut(12, 4, strCaption);
	dcMemory.SetTextColor(RGB(10, 30, 100));
	dcMemory.TextOut(11, 3, strCaption);

	dcMemory.SelectObject(pOldFont);
	font.DeleteObject();

	// ���ڴ��豸��������λͼ������
	pDC->BitBlt(0, 0, rcWnd.Width(), rcWnd.Height(), &dcMemory, 0, 0, SRCCOPY);

	// �ͷ���Դ
	dcMemory.SelectObject(::GetStockObject(WHITE_PEN));
	dcMemory.SelectObject(pOldbmpMemory);
	bmpMemory.DeleteObject();
	dcMemory.DeleteDC();
	DeleteDC(hdcSrc);
}

void CUISkinManager::Paint_XP(CWnd* pWnd, CDC* pDC, BOOL bFillClient, COLORREF crFillColor)
{
}

// ���ƶԻ���߿򣬶Ի������ͨ����ʽ����ɫ���ߡ��·��а�ť������
void CUISkinManager::PaintDialogBorder(CWnd* pWnd, CDC* pDC)
{
	if (pWnd == NULL || pWnd->m_hWnd == NULL)
	{
		return;
	}

	BOOL bGetDC = FALSE;

	if (!pDC)
	{
		bGetDC = TRUE;
		pDC = pWnd->GetDC();
	}

	const int BOTTOM_COMMAND_BUTTON_BAR_HEIGHT = 45; // �Ի���ײ���ť�������߶ȣ����أ�
	CRect rcWnd;
	CRect rc;

	pWnd->GetClientRect(&rcWnd);
	rcWnd.DeflateRect(m_iLeftMargin, m_iTitleBarHeight + m_iTopMargin, m_iLeftMargin + 1, m_iTopMargin + 1);

	// ��ˮƽ�ָ���
	rc.SetRect(rcWnd.left, rcWnd.bottom - BOTTOM_COMMAND_BUTTON_BAR_HEIGHT, rcWnd.right, rcWnd.bottom - BOTTOM_COMMAND_BUTTON_BAR_HEIGHT + 2);
	pDC->Draw3dRect(rc, RGB(162, 185, 201), RGB(255, 255, 255));

	// ���߿�
	CBrush br(RGB(100, 155, 194));
	rc = rcWnd;
	pDC->FrameRect(&rc, &br);
	rc.DeflateRect(1, 1, 1, 1);
	pDC->FrameRect(&rc, &br);

	// ���·��İ�ť�����䱳��
	rc.DeflateRect(1, 1, 1, 1);
	rc.top = rc.bottom - BOTTOM_COMMAND_BUTTON_BAR_HEIGHT + 4;
	GradientFillRect(pWnd, pDC, rc, GRADIENT_FILL_RECT_V, RGB(170, 210, 245), RGB(255, 255, 255));

	if (bGetDC)
	{
		pWnd->ReleaseDC(pDC);
	}
}

// ���ô��ڿɼ�����
void CUISkinManager::SetRgn(CWnd* pWnd)
{
	switch (m_iCurrentStyle)
	{
	case StyleXP:
		SetRgn_XP(pWnd);
		break;
	default:
		SetRgn_Normal(pWnd);
		break;
	}
}

void CUISkinManager::SetRgn_Normal(CWnd* pWnd)
{
	if (NULL == pWnd || NULL == pWnd->m_hWnd)
	{
		return;
	}

	// ȡ�ô��ڱ߿��߿�ȣ��߿������ڲ����ػ淶Χ�������ص�
	int cx = ::GetSystemMetrics(SM_CXDLGFRAME);
	int cy = ::GetSystemMetrics(SM_CYDLGFRAME);

	CRect rc;
	CRgn rgn;
	pWnd->GetWindowRect(&rc);

	// ����Բ�Ǿ�����Ϊ���ڵĿɼ������ų��߿���
	rgn.CreateRoundRectRgn(cx, cy, rc.Width() - cx, rc.Height() - cy, m_roundSize.x, m_roundSize.y);

	// ���ô��ڿɼ�����
	pWnd->SetWindowRgn(rgn, TRUE);

	rgn.DeleteObject();
}

void CUISkinManager::SetRgn_XP(CWnd* pWnd)
{
	int cx = ::GetSystemMetrics(SM_CXDLGFRAME);
	int cy = ::GetSystemMetrics(SM_CYDLGFRAME);

	CRect rc;
	CRgn rgn;
	pWnd->GetWindowRect(&rc);

	// ����Բ�Ǿ�����Ϊ���ڵĿɼ�����
	rgn.CreateRoundRectRgn(cx, cy, rc.Width() - cx, rc.Height() - cy, 33, 33);

	// ���ô��ڿɼ�����
	pWnd->SetWindowRgn(rgn, TRUE);

	rgn.DeleteObject();
}
