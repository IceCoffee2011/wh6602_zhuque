// GameAffiche.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GameAffiche.h"
#include ".\gameaffiche.h"
#include "TraceCatch.h"

//������Ϣ
#define	IDI_ROOM_AFFICHE_INFO			20000							//���乫����Ϣ��ʱ��
#define TIMER_ROOM_AFFICHE_INFO			10								//ʱ����

// CGameAffiche �Ի���

IMPLEMENT_DYNAMIC(CGameAffiche, CDialog)
CGameAffiche::CGameAffiche(CWnd* pParent /*=NULL*/)
	: CDialog(CGameAffiche::IDD, pParent), m_bIsExp(false)
{
	m_memdc.m_hDC = NULL;
}

CGameAffiche::~CGameAffiche()
{
	__ENTER_FUNCTION

	CThreadGuard Guard(&m_CriticalSpection);

	if (!m_ImageBack.IsNull())
	{
		m_ImageBack.DestroyImage();
	}

	::DeleteObject(m_fontDrawGameInfo);
	//m_fontDrawGameInfo.DeleteObject();
	::DeleteDC(m_memdc.GetSafeHdc());
	//m_memdc.DeleteDC();
	m_memdc.m_hDC = NULL;

	__LEAVE_FUNCTION
}

void CGameAffiche::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGameAffiche, CDialog)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CGameAffiche ��Ϣ�������

int CGameAffiche::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	//��Ϸ��������
	m_fontDrawGameInfo.CreateFont(18,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("����"));
	m_bIsExp = false;
	m_iLoopNums = 0;
	m_iCountLoop = 0;
	m_vecGameMsg.clear();
	return 0;
}

void CGameAffiche::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

	__ENTER_FUNCTION

	CThreadGuard Guard(&m_CriticalSpection);

	int iNums = m_vecGameMsg.size();
	if (iNums > 0)
	{
		if (NULL != m_memdc.m_hDC)
		{
			CRect rect;
			GetClientRect(&rect);
			if (!m_ImageBack.IsNull())
			{
				m_ImageBack.DrawImage(&m_memdc, 0, 0);
			}

			m_memdc.SetTextColor(m_bTextColor);

			if (m_bIsDisableGameMsg)
			{//���Ʒ��乫����Ϣ
				CFont * pOldFont=m_memdc.SelectObject(&m_fontDrawGameInfo);
				if (m_iCountVecIndex < m_vecGameMsg.size())
				{
					m_memdc.DrawText(m_vecGameMsg[m_iCountVecIndex].c_str(), m_rcCurMsgInfo, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
				}		
				m_memdc.SelectObject(pOldFont);
				pOldFont = NULL;
			}

			dc.BitBlt(0, 0, rect.Width(), rect.Height(), &m_memdc, 0, 0, SRCCOPY);
		}
	}

	__LEAVE_FUNCTION
	
}

//
//BOOL CGameAffiche::SetImageRgn(CPngImage * pImage)
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

//������Ϸ����
void CGameAffiche::SetGameAfficheInfo(const char* pInfo)
{
	__ENTER_FUNCTION

	CThreadGuard Guard(&m_CriticalSpection);

	if (NULL != pInfo)
	{
		KillTimer(IDI_ROOM_AFFICHE_INFO);
		m_strSaveGameAffiche.clear();
		m_strSaveGameAffiche.append(pInfo);
		//��ʼ��������Ϣ
		m_vecGameMsg.clear();
		m_iCountVecIndex = 0;
		//��������
		ParseChar(pInfo, ::strlen(pInfo)+1);
		WORD wInfoCount = m_vecGameMsg.size();
		if (wInfoCount>0)
		{
			CFont * pOldFont=m_memdc.SelectObject(&m_fontDrawGameInfo);
			m_rcCurMsgInfo.left = m_rcGameMsgInfo.right;
			m_rcCurMsgInfo.top = m_rcGameMsgInfo.top;
			m_rcCurMsgInfo.right = m_rcCurMsgInfo.left+m_memdc.GetTextExtent(m_vecGameMsg[0].c_str()).cx;
			m_rcCurMsgInfo.bottom = m_rcGameMsgInfo.bottom;
			m_memdc.SelectObject(pOldFont);
			pOldFont = NULL;
		}
		//���������
		if (wInfoCount > 0)
		{
			int iCurMsgW = m_rcCurMsgInfo.Width();
			int iGameMsgW = m_rcGameMsgInfo.Width();
			if ((wInfoCount > 1) || (iCurMsgW>iGameMsgW))
			{
				m_rcCurMsgInfo.left -= m_wStartXAbs;
				m_rcCurMsgInfo.right -= m_wStartXAbs;
			}
			else
			{
				//��ʱ���������
				m_rcCurMsgInfo.left = (iGameMsgW-iCurMsgW)/2;
				m_rcCurMsgInfo.right = m_rcCurMsgInfo.left+iCurMsgW;
			}
		}
	}

	__LEAVE_FUNCTION
}
//��������
void CGameAffiche::ParseChar(const char* pData, WORD wDataSize)
{
	__ENTER_FUNCTION

	if (NULL != pData && wDataSize>0)
	{
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

	__LEAVE_FUNCTION
}

//������ʾ����
void CGameAffiche::SetShowRect(CRect rect)
{
	m_rcGameMsgInfo = rect;
}

//���ñ���ͼ
void CGameAffiche::SetBackImage(const char* pImage, bool bIsRgnWnd)
{
	__ENTER_FUNCTION

	CThreadGuard Guard(&m_CriticalSpection);

	if (NULL != pImage)
	{
		m_ImageBack.LoadImage(TEXT(pImage));

		if (!m_ImageBack.IsNull() && bIsRgnWnd)
		{
			SetImageRgn(m_hWnd, &m_ImageBack);
		}

		CRect rect;
		GetClientRect(&rect);
		CPaintDC dc(this);
		DeleteDC(m_memdc.GetSafeHdc());
		m_memdc.m_hDC = NULL;
		CBitmap bmp;
		m_memdc.CreateCompatibleDC(&dc);
		bmp.CreateCompatibleBitmap(&dc, m_ImageBack.GetWidth(), m_ImageBack.GetHeight());
		m_memdc.SelectObject(bmp);	
		m_memdc.SetBkMode(TRANSPARENT);
		UpdateWindow();
		DeleteObject(bmp);

		InvalidateRect(NULL, TRUE);
	}

	__LEAVE_FUNCTION
}

//����������ɫ
void CGameAffiche::SetTextColor(COLORREF textcol)
{
	m_bTextColor = textcol;
}

//����X�Ὺʼλ��
void CGameAffiche::SetXAbs(DWORD dwXAbs)
{
	m_wStartXAbs = dwXAbs;
}

//���ò���
void CGameAffiche::SetMoveLeep(DWORD dwLeep)
{
	m_dwMoveLeep = dwLeep;
}
//����ʱ��Ƶ��
void CGameAffiche::SetTimerInterval(DWORD dwTimeInterval)
{
	m_dwTimeInterval = dwTimeInterval;
}
//��ʼ����
void CGameAffiche::SetPlay(bool bIsPlay, int iLoopNums)
{
	__ENTER_FUNCTION

	WORD wInfoCount = m_vecGameMsg.size();
	//���������
	if (wInfoCount > 0)
	{
		m_iLoopNums = 0;
		m_iCountLoop = 0;
		if (!bIsPlay)
		{
			m_bIsDisableGameMsg = false;
			KillTimer(IDI_ROOM_AFFICHE_INFO);
		}
		else
		{
			m_iLoopNums = iLoopNums;
			m_bIsDisableGameMsg = true;
			int iCurMsgW = m_rcCurMsgInfo.Width();
			int iGameMsgW = m_rcGameMsgInfo.Width();
			if ((wInfoCount > 1) || (iCurMsgW>iGameMsgW))
			{
				m_rcCurMsgInfo.left -= m_wStartXAbs;
				m_rcCurMsgInfo.right -= m_wStartXAbs;
				SetTimer(IDI_ROOM_AFFICHE_INFO, m_dwTimeInterval, NULL);
			}
			else
			{
				//��ʱ���������
				m_rcCurMsgInfo.left = (iGameMsgW-iCurMsgW)/2;
				m_rcCurMsgInfo.right = m_rcCurMsgInfo.left+iCurMsgW;
			}
		}

		InvalidateRect(NULL, TRUE);
	}

	__LEAVE_FUNCTION
}

//����Ϊ��չ����
void CGameAffiche::SetWndExp(DWORD wAlpha)
{
	__ENTER_FUNCTION

	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE)^0x80000);
	// ��ӿ�
	HINSTANCE hInst = LoadLibrary("User32.DLL");
	if(hInst)
	{ 
		typedef BOOL (WINAPI *ShowLayer)(HWND,COLORREF,BYTE,DWORD); 
		ShowLayer fun = NULL;

		// ��ȡ����ָ��
		fun = (ShowLayer)GetProcAddress(hInst, "SetLayeredWindowAttributes");
		if (fun)
		{
			//// -- 128 ͸����  ���һ������ 1:͸��һ����ɫ, 2: ����͸��
			fun(this->GetSafeHwnd(), 0, wAlpha, 2);
			m_bIsExp = true;
		}
		FreeLibrary(hInst); 
	}

	__LEAVE_FUNCTION
}
//����Ƿ�Ϊ��չ����
bool CGameAffiche::GetIsExp()
{
	return m_bIsExp;
}

void CGameAffiche::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}

void CGameAffiche::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	__ENTER_FUNCTION

	CThreadGuard Guard(&m_CriticalSpection);

	switch(nIDEvent)
	{
	case IDI_ROOM_AFFICHE_INFO://���乫����Ϣ��ʱ��
		{
			if (m_bIsDisableGameMsg)
			{
				m_rcCurMsgInfo.left -= m_dwMoveLeep;
				m_rcCurMsgInfo.right -= m_dwMoveLeep;
				if (m_rcCurMsgInfo.left <= m_rcGameMsgInfo.left)
				{
					m_rcCurMsgInfo.left = m_rcGameMsgInfo.left;
				}
				if (m_rcCurMsgInfo.right < m_rcGameMsgInfo.left)
				{
					++m_iCountVecIndex;
					if (m_iCountVecIndex>=m_vecGameMsg.size())
					{
						if (m_iLoopNums>0)
						{
							++m_iCountLoop;
							if (m_iCountLoop>=m_iLoopNums)
							{
								KillTimer(IDI_ROOM_AFFICHE_INFO);
								ShowWindow(SW_HIDE);
							}
						}
						m_iCountVecIndex = 0;
					}
					if ((m_vecGameMsg.size()>0) && (m_iCountVecIndex<m_vecGameMsg.size()))
					{
						CFont * pOldFont=m_memdc.SelectObject(&m_fontDrawGameInfo);
						m_rcCurMsgInfo.left = m_rcGameMsgInfo.right;
						m_rcCurMsgInfo.top = m_rcGameMsgInfo.top;
						m_rcCurMsgInfo.right = m_rcCurMsgInfo.left+m_memdc.GetTextExtent(m_vecGameMsg[m_iCountVecIndex].c_str()).cx;
						m_rcCurMsgInfo.bottom = m_rcGameMsgInfo.bottom;
						m_memdc.SelectObject(pOldFont);
						pOldFont = NULL;
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

	__LEAVE_FUNCTION

	CDialog::OnTimer(nIDEvent);
}

BOOL CGameAffiche::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bIsExp = false;
	m_bTextColor = RGB(255, 255, 255);
	m_wStartXAbs = 0;
	m_dwMoveLeep = 1;
	m_dwTimeInterval = TIMER_ROOM_AFFICHE_INFO;
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CGameAffiche::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}
