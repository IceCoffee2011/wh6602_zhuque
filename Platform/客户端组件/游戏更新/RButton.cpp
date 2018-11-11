//------------------------------------------------------------------------------
// �ļ����ƣ�RButton.h
// �ļ��汾��v1.0
// �������ڣ�2006-05-06 10:39
// ��    �ߣ�Richard Ree
// �ļ��������Ի水ť��ʵ���ļ�
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "GameUpdate.h"
#include "UISkinManager.h"
#include "RButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CRButton

IMPLEMENT_DYNAMIC(CRButton, CButton)

CRButton::CRButton(enButtonType iButtonType)
{
	m_iButtonType = iButtonType;
	m_bTracking = FALSE;
	m_bOver = FALSE;
	m_bDown = FALSE;
	m_bFocus = FALSE;
	m_bDefault = FALSE;
	SetTextColorEx(RGB(0,0,0));
}

CRButton::~CRButton()
{
}

void CRButton::SetLoadInfo(UINT uImage)
{
	m_iButtonType = ButtonTypeLoadBitmap;
	m_bitBg.LoadBitmap(uImage);
}

void CRButton::SetTextColorEx(COLORREF bg)
{
	m_rgbBg = bg;
}

void CRButton::PreSubclassWindow()
{
	CButton::PreSubclassWindow();

	UINT uStyle = GetButtonStyle();
	if (uStyle == BS_DEFPUSHBUTTON)
	{
		m_bDefault = TRUE;
	}

	ModifyStyle(0, BS_OWNERDRAW);
}

// �Ի�ؼ�
void CRButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CRect rcButton = lpDrawItemStruct->rcItem;
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC); 
	UINT iState = lpDrawItemStruct->itemState;

	// ��ȡ��ť��״̬
	m_bFocus = iState & ODS_FOCUS;
	m_bDown = iState & ODS_SELECTED;

	// ���ư�ť����
	if (CUISkinManager::m_iCurrentStyle == CUISkinManager::StyleSummer)
	{
		HBITMAP bitTmp;
		if (ButtonTypeLoadBitmap==m_iButtonType)
			bitTmp = (HBITMAP)m_bitBg;
		else
			bitTmp = CUISkinManager::m_bmButtonImage;

		if (m_iButtonType == ButtonTypeMinimize)
		{
			// ������С�����ڰ�ť
			DrawSystemControlButtonWithBitmap(pDC->GetSafeHdc(), rcButton
				, (HBITMAP)CUISkinManager::m_bmSystemControlButtonImage, m_bDown ? 16 : 0);
			return;
		}
		else if (m_iButtonType == ButtonTypeMaximize)
		{
			if (GetWindowLong(GetParent()->m_hWnd, GWL_STYLE) & WS_MAXIMIZE)
			{
				// ������󻯴��ڰ�ť
				DrawSystemControlButtonWithBitmap(pDC->GetSafeHdc(), rcButton
					, (HBITMAP)CUISkinManager::m_bmSystemControlButtonImage, m_bDown ? 48 : 32);
			}
			else
			{
				// ���ƻ�ԭ���ڴ�С��ť
				DrawSystemControlButtonWithBitmap(pDC->GetSafeHdc(), rcButton
					, (HBITMAP)CUISkinManager::m_bmSystemControlButtonImage, m_bDown ? 80 : 64);
			}
			return;
		}
		else if (m_iButtonType == ButtonTypeClose)
		{
			// ���ƹرմ��ڰ�ť
			DrawSystemControlButtonWithBitmap(pDC->GetSafeHdc(), rcButton
				, (HBITMAP)CUISkinManager::m_bmSystemControlButtonImage, m_bDown ? 112 : 96);
			return;
		}
		else if (m_bDefault)
		{
			DrawItemWithBitmap(pDC->GetSafeHdc(), rcButton, bitTmp, 20, 32);
		}
		else if (m_bOver)
		{
			DrawItemWithBitmap(pDC->GetSafeHdc(), rcButton, bitTmp, 20, 32 * 2);
		}
		else if (m_bDown)
		{
			DrawItemWithBitmap(pDC->GetSafeHdc(), rcButton, bitTmp, 20, 32 * 3);
		}
		else
		{
			DrawItemWithBitmap(pDC->GetSafeHdc(), rcButton, bitTmp, 20, 0);
		}
	}
	else // ȱʡ���
	{
		// �ڱ߿�
		COLORREF crTop, crBottom;
		CRect rcOver = rcButton;
		rcOver.DeflateRect(1,1);

		if (m_bDefault)
		{
			crTop = RGB(203, 231, 255);
			crBottom = RGB(105, 130, 238);
		}
		else if (m_bOver)
		{
			crTop = RGB(255, 240, 207);
			crBottom = RGB(229, 151, 0);
		}
		else if (m_bDown)
		{
			crTop = RGB(209, 204, 193);
			crBottom = RGB(218, 216, 207);
		}
		else
		{
			crTop = RGB(254, 254, 254);
			crBottom = RGB(234, 245, 255);
		}
		CUISkinManager::GradientFillRect(this, pDC, rcOver, GRADIENT_FILL_RECT_V, crTop, crBottom);

		// ��߿�
		CPen pen(PS_SOLID, 1, RGB(200, 210, 240)); // �߿���ɫ
		DrawRoundBorder(pDC, rcButton);

		// �ڲ����
		if (!m_bDown)
		{
			CRect rcIn = rcButton;
			if (m_bOver || m_bFocus)
			{
				rcIn.DeflateRect(3,3);
			}
			else
			{
				rcIn.DeflateRect(2,2);
			}
			crTop = RGB(252, 252, 251);
			crBottom = RGB(236, 235, 230);
			CUISkinManager::GradientFillRect(this, pDC, rcIn, GRADIENT_FILL_RECT_V, crTop, crBottom);
		}
	}

	rcButton.DeflateRect(CSize(GetSystemMetrics(SM_CXEDGE), GetSystemMetrics(SM_CYEDGE)));

	// ���ƽ����
	if (m_bFocus)
	{
		CRect rcFoucs = rcButton;
		pDC->DrawFocusRect(rcFoucs);
	}

	// ���ư�ť�����ı�

	CString strTitle;

	GetWindowText(strTitle);

	if (strlen(strTitle) > 0)
	{
		CFont* hFont = GetFont();
		CFont* hOldFont = pDC->SelectObject(hFont);
		// �����ı���ʾ�Ŀ��
		CSize szExtent = pDC->GetTextExtent(strTitle, lstrlen(strTitle));
		if (strstr(strTitle, "&") != 0)
		{
			// ��ȥ�� '&' �ַ�������ʱ�Ŀ��
			szExtent.cx -= pDC->GetTextExtent("&").cx;
		}

		CPoint pt(rcButton.CenterPoint().x - szExtent.cx / 2, rcButton.CenterPoint().y - szExtent.cy / 2);
		if (iState & ODS_SELECTED)
		{
			pt.Offset(1, 1);
		}
		int nMode = pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_rgbBg);

		if (iState & ODS_DISABLED)
		{
			pDC->DrawState(pt, szExtent, strTitle, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
		}
		else
		{
			pDC->DrawState(pt, szExtent, strTitle, DSS_NORMAL, TRUE, 0, (HBRUSH)NULL);
		}

		pDC->SelectObject(hOldFont);
		pDC->SetBkMode(nMode);
	}
}


BEGIN_MESSAGE_MAP(CRButton, CButton)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()


// CRButton ��Ϣ�������


void CRButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tms;
		tms.cbSize = sizeof(TRACKMOUSEEVENT);
		tms.dwFlags = TME_LEAVE|TME_HOVER;
		tms.dwHoverTime = 1;
		tms.hwndTrack = m_hWnd;
		m_bTracking = ::_TrackMouseEvent(&tms);
	}
	CButton::OnMouseMove(nFlags, point);
}

LRESULT CRButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	m_bOver = TRUE;
	InvalidateRect(NULL);
	return m_bTracking;
}

LRESULT CRButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bTracking = FALSE;
	m_bOver = FALSE;
	InvalidateRect(NULL);
	return m_bTracking;
}

// ��λͼ��ʽ��水ť�ؼ���Ҫ��λͼ���Ϊ32���أ���15 + ��2 + ��15���м����ſؼ�������죩
// HDC hdcDest��Ŀ���豸�������
// CRect& rc����ť��ռ�������������
// HBITMAP hBitMap��λͼ���
// UINT iBitMapHeight��λͼ�߶�
// UINT cx����λͼ���Ƶ����λ�ã���ť���ڲ�ͬ״̬���������н��㡢�����ͣ�����£�ʱ��Ӧ��ͬ�����λ��
void CRButton::DrawItemWithBitmap(HDC hdcDest, CRect& rc, HBITMAP hBitMap, UINT iBitMapHeight, UINT cx)
{
	if (hdcDest == NULL || hBitMap == NULL)
	{
		return;
	}

	HDC hdcSrc = CreateCompatibleDC(hdcDest);
	HGDIOBJ hOldGdiObj = SelectObject(hdcSrc, hBitMap);

	TransparentBlt(hdcDest, 0, 0, CUISkinManager::m_iButtonImageLeftPartWidth, rc.Height()
		, hdcSrc, cx, 0, CUISkinManager::m_iButtonImageLeftPartWidth, iBitMapHeight
		, CUISkinManager::TRANSPARENT_COLOR);
	TransparentBlt(hdcDest, CUISkinManager::m_iButtonImageLeftPartWidth, 0
		, rc.Width() - CUISkinManager::m_iButtonImageLeftPartWidth + CUISkinManager::m_iButtonImageRightPartWidth, rc.Height()
		, hdcSrc, cx + CUISkinManager::m_iButtonImageLeftPartWidth, 0
		, CUISkinManager::m_iButtonImageMiddlePartWidth, iBitMapHeight
		, CUISkinManager::TRANSPARENT_COLOR);
	TransparentBlt(hdcDest
		, rc.Width() - CUISkinManager::m_iButtonImageRightPartWidth, 0
		, CUISkinManager::m_iButtonImageRightPartWidth, rc.Height()
		, hdcSrc
		, cx + CUISkinManager::m_iButtonImageLeftPartWidth + CUISkinManager::m_iButtonImageMiddlePartWidth, 0
		, CUISkinManager::m_iButtonImageRightPartWidth, iBitMapHeight
		, CUISkinManager::TRANSPARENT_COLOR);

	SelectObject(hdcSrc, hOldGdiObj);
	DeleteDC(hdcSrc);
}

// ��λͼ��ʽ���ϵͳ���ư�ť�ؼ���Ҫ��λͼ���Ϊ128��������
// HDC hdcDest��Ŀ���豸�������
// CRect& rc����ť��ռ�������������
// HBITMAP hBitMap��λͼ���
// UINT cx����λͼ���Ƶ����λ��
void CRButton::DrawSystemControlButtonWithBitmap(HDC hdcDest, CRect& rc, HBITMAP hBitMap, UINT cx)
{
	if (hdcDest == NULL || hBitMap == NULL)
	{
		return;
	}

	HDC hdcSrc = CreateCompatibleDC(hdcDest);
	HGDIOBJ hOldGdiObj = SelectObject(hdcSrc, hBitMap);

	TransparentBlt(hdcDest, 0, 0, rc.Width(), rc.Height(), hdcSrc, cx, 0
		, CUISkinManager::m_iSystemControlButtonWidth
		, CUISkinManager::m_iSystemControlButtonHeight
		, CUISkinManager::TRANSPARENT_COLOR);

	SelectObject(hdcSrc, hOldGdiObj);
	DeleteDC(hdcSrc);
}

// ���߿�
void CRButton::DrawRoundBorder(CDC *pDC, CRect rcBorder)
{
	rcBorder.DeflateRect(0, 0, 1, 1);
	CPoint pt[] = {
		CPoint(rcBorder.left, rcBorder.top + 2),
		CPoint(rcBorder.left, rcBorder.bottom - 2),
		CPoint(rcBorder.left + 1, rcBorder.bottom - 1),
		CPoint(rcBorder.left + 2, rcBorder.bottom),
		CPoint(rcBorder.right - 2, rcBorder.bottom),
		CPoint(rcBorder.right - 1, rcBorder.bottom - 1),
		CPoint(rcBorder.right, rcBorder.bottom - 2),
		CPoint(rcBorder.right, rcBorder.top + 2),
		CPoint(rcBorder.right - 1, rcBorder.top + 1),
		CPoint(rcBorder.right - 2, rcBorder.top),
		CPoint(rcBorder.left + 2, rcBorder.top),
		CPoint(rcBorder.left + 1, rcBorder.top + 1),
		CPoint(rcBorder.left, rcBorder.top + 2)
	};

	BYTE bt[] = {
		PT_MOVETO,
		PT_LINETO,
		PT_LINETO,
		PT_LINETO,
		PT_LINETO,
		PT_LINETO,
		PT_LINETO,
		PT_LINETO,
		PT_LINETO,
		PT_LINETO,
		PT_LINETO,
		PT_LINETO,
		PT_LINETO
	};

	pDC->PolyDraw(pt, bt, 13);
}
