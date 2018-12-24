// RoomViewAffiche.cpp : ʵ���ļ�
//


#include "StdAfx.h"
#include "GameViewAffiche.h"

#include "MemDC.h"

//������Ϣ
#define	IDI_ROOM_AFFICHE_INFO			20000							//���乫����Ϣ��ʱ��
#define TIMER_ROOM_AFFICHE_INFO			30								//ʱ����


IMPLEMENT_DYNAMIC(CGameViewAffiche, CWnd)
CGameViewAffiche::CGameViewAffiche()
{
}

CGameViewAffiche::~CGameViewAffiche()
{
	if (!m_ImageBack.IsNull())
	{
		m_ImageBack.DestroyImage();
	}

	DeleteObject(m_fontDrawGameInfo);
}


BEGIN_MESSAGE_MAP(CGameViewAffiche, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CGameViewAffiche ��Ϣ�������


void CGameViewAffiche::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()

	CRect rect;
	GetClientRect(&rect);
	CMemDC dcMem(&dc, rect, true);	// �õ�ɫ��䱳��

	if (!m_ImageBack.IsNull())
	{
		m_ImageBack.DrawImage(&dcMem, 0, 0);
	}
	
	if (m_bIsDisableGameMsg)
	{
		//���Ʒ��乫����Ϣ
		dcMem.SetTextColor(RGB(255,139,83));
		dcMem.SetBkMode(TRANSPARENT);
		CFont * pOldFont=dcMem.SelectObject(&m_fontDrawGameInfo);
		if (m_iCountVecIndex < m_vecGameMsg.size())
		{
			dcMem.DrawText(m_vecGameMsg[m_iCountVecIndex].c_str(), m_rcCurMsgInfo, DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
		}		
		dcMem.SelectObject(pOldFont);
		pOldFont = NULL;
	}
}

//���ñ���ͼ
void CGameViewAffiche::SetBackImage(const char* pImage, bool bIsRgnWnd)
{
	if (NULL != pImage)
	{
		m_ImageBack.DestroyImage();
		m_ImageBack.LoadImage(TEXT(pImage));

		if (bIsRgnWnd && !m_ImageBack.IsNull())
		{
			SetImageRgn(m_hWnd, &m_ImageBack, SIRT_FULL_DOT_LESS);
		}
	}
}

BOOL CGameViewAffiche::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}

int CGameViewAffiche::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	//HINSTANCE hDllInstance=AfxGetInstanceHandle();
	//m_ImageBack.SetLoadInfo(IDB_ROOM_VIEW_AFFICHE, hDllInstance);
	
	//��Ϸ��������
	m_fontDrawGameInfo.CreateFont(18,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("����"));
	return 0;
}

void CGameViewAffiche::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	// TODO: �ڴ˴������Ϣ����������
}


//��������
void CGameViewAffiche::ParseChar(const char* pData, WORD wDataSize)
{
	if (NULL == pData || 0==wDataSize)
	{
		return;
	}
	m_vecGameMsg.clear();

	WORD iCountIndex = 0;
	char* pNewData = new char[wDataSize];
	if (NULL == pNewData)
	{
		return;
	}
	::memset(pNewData, 0, wDataSize);
	const char* pOldData = pData;
	while ((*pData != '\0') && (iCountIndex<wDataSize))
	{
		if ('|'== *pData)
		{
			if (('*' == *(pData+1))
				&& ('*' == *(pData+2))
				&& ('|' == *(pData+3)))
			{
				::memset(pNewData, 0, wDataSize);
				::memcpy(pNewData, pOldData, pData-pOldData);
				m_vecGameMsg.push_back(pNewData);
				pOldData = pData+4;
				pData = pData+3;
			}
		}
		pData++;
		++iCountIndex;
	}
	delete pNewData;
	pNewData = NULL;
}

//������Ϸ����
void CGameViewAffiche::SetGameAfficheInfo(char* pInfo)
{
	if (NULL == pInfo)
	{
		return;
	}
	
	CRect rect;
	GetClientRect(&rect);
	m_strSaveGameAffiche.clear();
	m_strSaveGameAffiche.append(pInfo);
	//��ʼ��������Ϣ
	m_vecGameMsg.clear();
	m_iCountVecIndex = 0;
	//��������
	ParseChar(pInfo, ::strlen(pInfo)+1);
	m_rcGameMsgInfo = rect;
	m_rcGameMsgInfo.top -= 6;
	m_rcGameMsgInfo.bottom -= 6;
	WORD wInfoCount = m_vecGameMsg.size();
	if (wInfoCount>0)
	{
		CClientDC dc(this);
		m_rcCurMsgInfo.left = m_rcGameMsgInfo.right;
		m_rcCurMsgInfo.top = m_rcGameMsgInfo.top;
		m_rcCurMsgInfo.right = m_rcCurMsgInfo.left+dc.GetTextExtent(m_vecGameMsg[0].c_str()).cx;
		m_rcCurMsgInfo.bottom = m_rcGameMsgInfo.bottom;
	}
	KillTimer(IDI_ROOM_AFFICHE_INFO);
	//���������
	if (wInfoCount > 0)
	{
		m_bIsDisableGameMsg = true;
		int iCurMsgW = m_rcCurMsgInfo.Width();
		int iGameMsgW = m_rcGameMsgInfo.Width();
		if ((wInfoCount > 1) || (iCurMsgW>iGameMsgW))
		{
			m_rcCurMsgInfo.left -= 400;
			m_rcCurMsgInfo.right -= 400;
			SetTimer(IDI_ROOM_AFFICHE_INFO, TIMER_ROOM_AFFICHE_INFO, NULL);
		}
		else
		{
			//��ʱ���������
			m_rcCurMsgInfo.left = (iGameMsgW-iCurMsgW)/2;
			m_rcCurMsgInfo.right = m_rcCurMsgInfo.left+iCurMsgW;
		}
	}
	InvalidateRect(&m_rcGameMsgInfo, FALSE);

}

void CGameViewAffiche::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CWnd::OnTimer(nIDEvent);


	switch(nIDEvent)
	{
	case IDI_ROOM_AFFICHE_INFO://���乫����Ϣ��ʱ��
		{
			if (m_bIsDisableGameMsg)
			{
				m_rcCurMsgInfo.left -= 2;
				m_rcCurMsgInfo.right -= 2;
				if (m_rcCurMsgInfo.right < m_rcGameMsgInfo.left)
				{
					++m_iCountVecIndex;
					if (m_iCountVecIndex>=m_vecGameMsg.size())
					{
						m_iCountVecIndex = 0;
					}

					if ((m_vecGameMsg.size()>0) && (m_iCountVecIndex<m_vecGameMsg.size()))
					{
						CClientDC dc(this);

						m_rcCurMsgInfo.left = m_rcGameMsgInfo.right;
						m_rcCurMsgInfo.top = m_rcGameMsgInfo.top;
						m_rcCurMsgInfo.right = m_rcCurMsgInfo.left+dc.GetTextExtent(m_vecGameMsg[m_iCountVecIndex].c_str()).cx;
						m_rcCurMsgInfo.bottom = m_rcGameMsgInfo.bottom;
					}
					else
					{
						KillTimer(IDI_ROOM_AFFICHE_INFO);
					}
				}
				Invalidate(FALSE);
			}
			break;
		}
	}
}


//
//BOOL CGameViewAffiche::SetImageRgn(CPngImage * pImage)
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