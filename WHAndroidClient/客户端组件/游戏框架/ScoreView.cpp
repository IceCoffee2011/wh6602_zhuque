#include "StdAfx.h"
#include "ScoreView.h"
#include "Resource.h"
#include ".\scoreview.h"
#include "GameFrameDlg.h"


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CScoreView, CDialog)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CScoreView::CScoreView() : CDialog()
{
	m_szWndSize.SetSize(0, 0);
	m_ImageBuffer.Destroy();
	m_ImageBuffer.Create(100, 100, 24);
	m_pGameFrameDlg = NULL;
	m_bIsMoveWnd = true;
}
CScoreView::CScoreView(UINT nIDTemplate, CWnd* pParentWnd ): CDialog(IDD_SCORE_VIEW_DLG,pParentWnd)
{
	m_szWndSize.SetSize(0, 0);
	m_ImageBuffer.Destroy();
	m_ImageBuffer.Create(100, 100, 24);
	m_pGameFrameDlg = NULL;
	m_bIsMoveWnd = true;
}

//��������
CScoreView::~CScoreView()
{
	m_ImageBuffer.Destroy();
	m_pGameFrameDlg = NULL;
}

//������Ϸ���ָ��
void CScoreView::SetGameFramePtr(CGameFrameDlg* pGameFrameDlg)
{
	if (NULL != pGameFrameDlg)
	{
		m_pGameFrameDlg = pGameFrameDlg;
	}
}

//���ÿɷ��ƶ�����
void CScoreView::SetDisableMoveWnd(bool bIsMove)
{
	m_bIsMoveWnd = bIsMove;
}

//������Ϣ
int CScoreView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	m_bIsMoveWnd = true;

	//��ʼ������
	InitScoreViewInfo();

	
	if (!m_ImagePngBack.IsNull())
	{
		m_ImageBuffer.Destroy();
		m_ImageBuffer.Create(m_ImagePngBack.GetWidth(), m_ImagePngBack.GetHeight(), 24);
		m_szWndSize.SetSize(m_ImagePngBack.GetWidth(), m_ImagePngBack.GetHeight());
		SetImageRgn(m_hWnd, &m_ImagePngBack, SIRT_DEBURRING_DOT_LESS);
		return 0;
	}
	return -1;
}

//���ر���ͼ
bool CScoreView::LoadBackImage(const char* pImagePath)
{
	if (NULL != pImagePath)
	{
		m_ImagePngBack.DestroyImage();
		//������Դ
		m_ImagePngBack.LoadImage(pImagePath);
		m_szWndSize.SetSize(m_ImagePngBack.GetWidth(), m_ImagePngBack.GetHeight());
		return true;
	}
	return false;
}

//�����Ϣ
void CScoreView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	if (true == m_bIsMoveWnd)
	{
		//��Ϣģ��
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}
	return;
}

//�ػ�����
void CScoreView::OnPaint() 
{
	if (!m_ImageBuffer.IsNull() && !m_ImagePngBack.IsNull())
	{
		//��������
		CPaintDC dc(this);

		CDC * pDC=CDC::FromHandle(m_ImageBuffer.GetDC());

		pDC->SetBkMode(TRANSPARENT);
		pDC->FillSolidRect(0,0,m_szWndSize.cx,m_szWndSize.cy,RGB(0,0,0));

		//�滭����ͼƬ
		m_ImagePngBack.DrawImage(pDC, 0, 0);

		DrawScoreInfo(pDC);

		//�滭����
		m_ImageBuffer.BitBlt(dc,0,0,m_szWndSize.cx,m_szWndSize.cy,0,0);
		m_ImageBuffer.ReleaseDC();

		CDC::DeleteTempMap();	// add by HouGuoJiang 2011-11-25
	}
}

//////////////////////////////////////////////////////////////////////////


void CScoreView::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CRect parrect;
	CWnd * pWnd= AfxGetMainWnd();
	if (NULL == pWnd)
	{
		return;
	}
	pWnd->GetWindowRect(&parrect);

	if (lpwndpos->y+m_szWndSize.cy > parrect.top+parrect.Height()-70)
	{//�±�
		lpwndpos->y = parrect.top+parrect.Height()-70-m_szWndSize.cy;
	}
	if (lpwndpos->x+m_szWndSize.cx > parrect.left+parrect.Width())
	{//�ұ�
		lpwndpos->x = parrect.left+parrect.Width()-m_szWndSize.cx;
	}
	if (lpwndpos->y < parrect.top+29)
	{//��
		lpwndpos->y = parrect.top+29;
	}
	if (lpwndpos->x < parrect.left)
	{//��
		lpwndpos->x = parrect.left;
	}
	CDialog::OnWindowPosChanging(lpwndpos);
	// TODO: �ڴ˴������Ϣ����������
}


//���ش���
void CScoreView::OnGoPlaza()
{
	if (NULL != m_pGameFrameDlg)
	{
		m_pGameFrameDlg->SetGameExitOperation(1);
	}
}

//������Ϸ
void CScoreView::OnStartGame()
{
	if (NULL != m_pGameFrameDlg)
	{
		m_pGameFrameDlg->SetGameExitOperation(2);
	}
}

//���ط���
void CScoreView::OnGoRoom()
{
	if (NULL != m_pGameFrameDlg)
	{
		m_pGameFrameDlg->SetGameExitOperation(3);
	}
}
//�˳���Ϸ
void CScoreView::OnGoExit()
{
	if (NULL != m_pGameFrameDlg)
	{
		m_pGameFrameDlg->SetGameExitOperation(4);
	}
}

BOOL CScoreView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CScoreView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnLButtonUp(nFlags, point);
}
//
//BOOL CScoreView::SetImageRgn(CPngImage * pImage)
//{
//	if (NULL==pImage) return FALSE;
//	if (pImage->IsNull()) return FALSE;
//	long wWidth = pImage->GetWidth();
//	long wHeight = pImage->GetHeight();
//
//	CRgn rgnImage, tempRgn;
//	rgnImage.CreateRectRgn(0, 0, 0, 0);
//
//	int leftX = 0;
//	int rightX = 0;
//	for (int i=wHeight-1; i>=0; --i)
//	{
//		for (int j = 0; j<wWidth; ++j)
//		{
//			leftX = j;
//			rightX = j;
//			for (int k=j; k<wWidth; ++k)  //��ס��ߵ�x
//			{
//				BYTE byValue = pImage->GetAlphaValue(k, i);
//				if (byValue != 0)
//				{
//					leftX = k;
//					break;
//				}
//			}
//			for (int k=leftX+1; k<wWidth; ++k)
//			{
//				BYTE byValue = pImage->GetAlphaValue(k, i);
//				if (byValue == 0 || (k == wWidth -1 && byValue!=0))
//				{
//					rightX = k;
//					break;
//				}
//			}
//
//			j = leftX > rightX ? leftX : rightX;
//
//			if (rightX > leftX)
//			{
//				tempRgn.CreateRectRgn(leftX, (wHeight - i-1) , rightX, (wHeight -i));
//				rgnImage.CombineRgn(&rgnImage, &tempRgn, RGN_OR);
//				tempRgn.DeleteObject();
//			}
//		}
//	}
//
//	if (NULL == rgnImage.m_hObject)
//		return FALSE;
//
//	SetWindowRgn((HRGN)rgnImage.m_hObject, TRUE);
//	rgnImage.DeleteObject();
//	return TRUE;
//}