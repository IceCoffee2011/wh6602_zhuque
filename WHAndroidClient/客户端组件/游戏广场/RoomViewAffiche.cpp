// RoomViewAffiche.cpp : ʵ���ļ�
//


#include "RoomViewAffiche.h"

// CRoomViewAffiche

//������Ϣ
#define	IDI_ROOM_AFFICHE_INFO			20000							//���乫����Ϣ��ʱ��
#define TIMER_ROOM_AFFICHE_INFO			30								//ʱ����


IMPLEMENT_DYNAMIC(CRoomViewAffiche, CWnd)
CRoomViewAffiche::CRoomViewAffiche()
{
	m_memdc.m_hDC = NULL;
}

CRoomViewAffiche::~CRoomViewAffiche()
{
	m_fontDrawGameInfo.DeleteObject();
	m_memdc.DeleteDC();
	m_memdc.m_hDC = NULL;
}


BEGIN_MESSAGE_MAP(CRoomViewAffiche, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CRoomViewAffiche ��Ϣ�������


void CRoomViewAffiche::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
	if (NULL != m_memdc.m_hDC)
	{
		CRect rect;
		GetClientRect(&rect);
		CImageHandle Handle(&m_ImageBack);
		m_ImageBack.AlphaDrawImage(&m_memdc, 0, 0, RGB(255, 0, 255));
		
		if (m_bIsDisableGameMsg)
		{//���Ʒ��乫����Ϣ
			m_memdc.SetTextColor(RGB(255,139,83));
			CFont * pOldFont=m_memdc.SelectObject(&m_fontDrawGameInfo);
			if (m_iCountVecIndex < m_vecGameMsg.size())
			{
				m_memdc.DrawText(m_vecGameMsg[m_iCountVecIndex].c_str(), m_rcCurMsgInfo, DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
			}		
			m_memdc.SelectObject(pOldFont);
			pOldFont = NULL;
		}
		
		dc.BitBlt(0, 0, rect.Width(), rect.Height(), &m_memdc, 0, 0, SRCCOPY);
	}
	
}

BOOL CRoomViewAffiche::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}

int CRoomViewAffiche::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	HINSTANCE hDllInstance=AfxGetInstanceHandle();
	CString strPath("");
	GetImagePlazaPath(strPath);
	m_ImageBack.SetLoadInfo(TEXT(strPath+"BKAffiche.png"),false);
	
	//��Ϸ��������
	m_fontDrawGameInfo.CreateFont(18,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("����ϸ��"));
	return 0;
}

void CRoomViewAffiche::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	
	CPaintDC dc(this);
	m_memdc.DeleteDC();
	CBitmap bmp;
	m_memdc.CreateCompatibleDC(&dc);
	bmp.CreateCompatibleBitmap(&dc, cx, cy);
	m_memdc.SelectObject(bmp);	
	m_memdc.SetBkMode(TRANSPARENT);
	UpdateWindow();
	bmp.DeleteObject();
	// TODO: �ڴ˴������Ϣ����������
}


//��������
void CRoomViewAffiche::ParseChar(const char* pData, WORD wDataSize)
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
void CRoomViewAffiche::SetGameAfficheInfo(char* pInfo)
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
		CFont * pOldFont=m_memdc.SelectObject(&m_fontDrawGameInfo);
		m_rcCurMsgInfo.left = m_rcGameMsgInfo.right;
		m_rcCurMsgInfo.top = m_rcGameMsgInfo.top;
		m_rcCurMsgInfo.right = m_rcCurMsgInfo.left+m_memdc.GetTextExtent(m_vecGameMsg[0].c_str()).cx;
		m_rcCurMsgInfo.bottom = m_rcGameMsgInfo.bottom;
		m_memdc.SelectObject(pOldFont);
		pOldFont = NULL;
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

void CRoomViewAffiche::OnTimer(UINT nIDEvent)
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
}
//��ǰ·��
void CRoomViewAffiche::GetCurrentDirectoryPath(CString &szPath)
{
	szPath="";
	TCHAR szPathTemp[MAX_PATH]={0};
	GetCurrentDirectory(MAX_PATH, szPathTemp);
	szPath.Append(szPathTemp);
}

//������Դ·��
void CRoomViewAffiche::GetImagePlazaPath(CString& strRetValue)
{
	CString strModuleFileName("");	GetCurrentDirectoryPath(strModuleFileName);
	strModuleFileName.Append("\\data\\plaza\\");
	strRetValue = strModuleFileName;
}
