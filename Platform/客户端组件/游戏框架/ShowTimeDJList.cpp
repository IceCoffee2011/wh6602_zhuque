// ShowTimeDJList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ShowTimeDJList.h"
#include ".\showtimedjlist.h"

#include "MemDC.h"

#define			DJ_TIME_CENTER_INTERVAL			34
#define			DJ_DRAW_TIME_X					10
#define			DJ_DRAW_TIME_Y					35
#define			DJ_DRAW_TIME_INTERVAL			33

#define			IDI_DJ_COUNT_TIME_ID			1023
#define			ID_DJ_TIME_INTERVAL				1000


// CShowTimeDJList �Ի���

IMPLEMENT_DYNAMIC(CShowTimeDJList, CDialog)
CShowTimeDJList::CShowTimeDJList(CWnd* pParent /*=NULL*/)
	: CDialog(CShowTimeDJList::IDD, pParent)
{
	m_pUserDJInfo = NULL;
	m_bIsDisableTime = false;
}

CShowTimeDJList::~CShowTimeDJList()
{
	m_pUserDJInfo = NULL;
}

void CShowTimeDJList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CShowTimeDJList, CDialog)
//	ON_WM_NCMOUSEMOVE()
ON_WM_PAINT()
ON_WM_ERASEBKGND()
ON_WM_TIMER()
END_MESSAGE_MAP()


//��̬��ô��ڿ����
CSize CShowTimeDJList::GetDlgWH(void)
{
	CSize wndsize;
	wndsize.SetSize(0, 0);
	if (NULL!=m_pUserDJInfo)
	{
		int iNums = m_pUserDJInfo->GetUserUseDJAndTimeDJNums();
		if (iNums>0)
		{
			wndsize.cx = m_ImageWndTop.GetWidth();
			wndsize.cy = m_ImageWndTop.GetHeight() + m_ImageWndDown.GetHeight() + (iNums-1)*DJ_TIME_CENTER_INTERVAL;
		}
	}
	return wndsize;
}

//���ô��ڴ�С
void CShowTimeDJList::SetResetWndSize(void)
{
	if (NULL==m_pUserDJInfo)
	{
		return;
	}
	int iNums = m_pUserDJInfo->GetUserUseDJAndTimeDJNums();
	if (iNums > 0)
	{
		CSize wndsize;
		wndsize.cx = m_ImageWndTop.GetWidth();
		wndsize.cy = m_ImageWndTop.GetHeight() + m_ImageWndDown.GetHeight() + (iNums-1)*DJ_TIME_CENTER_INTERVAL;
		
		CWnd* pParamWnd = GetParent();
		if (NULL!=pParamWnd)
		{
			CRect rect;
			pParamWnd->GetClientRect(&rect);
			SetWindowPos(NULL, 380, rect.bottom-40-wndsize.cy, wndsize.cx, wndsize.cy, 0);
			InvalidateRect(NULL);
		}
	}
	else
	{
		ShowWindow(SW_HIDE);
	}
}

// CShowTimeDJList ��Ϣ�������

BOOL CShowTimeDJList::OnInitDialog()
{
	CDialog::OnInitDialog();

	////����WS_EX_LAYERED��չ����
	//SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,
	//	GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	//HINSTANCE hInst = LoadLibrary("User32.DLL");
	//if(hInst)
	//{
	//	typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD);
	//	MYFUNC fun = NULL;
	//	//ȡ��SetLayeredWindowAttributes����ָ��
	//	fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
	//	if(fun)fun(this->GetSafeHwnd(),200,128,2);
	//	FreeLibrary(hInst);
	//}

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_pUserDJInfo = CGameClientDJInfo::GetDJInfoInstance();

	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
	
	//��
	m_ImageWndTop.LoadImage(hInstance, TEXT("IMAGE_DJTIME_BACK_UP"));
	//��
	m_ImageWndCenter.LoadImage(hInstance, TEXT("IMAGE_DJTIME_BACK_CENTER"));
	//��
	m_ImageWndDown.LoadImage(hInstance, TEXT("IMAGE_DJTIME_BACK_DOWN"));
	//���߱�����
	m_ImageDJBackFrame.LoadImage(hInstance, TEXT("IMAGE_DJ_TIME_DJBACK"));
	//����ʱ������
	m_ImageDJTimeNums.LoadImage(hInstance, TEXT("IMAGE_DJ_TIME_NUMS"));
	//����һ����
	m_ImageNoMinute.LoadImage(hInstance, TEXT("IMAGE_DJ_NO_MINUTE"));
	m_bIsDisableTime = false;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//��ʼ��ʱ
void CShowTimeDJList::SetDisableAndUpdateWnd(bool bIsDisable)
{
	KillTimer(IDI_DJ_COUNT_TIME_ID);
	if (true == bIsDisable)
	{
		SetResetWndSize();
		//������ʱ��
		SetTimer(IDI_DJ_COUNT_TIME_ID, ID_DJ_TIME_INTERVAL, NULL);
	}
	m_bIsDisableTime = bIsDisable;
	InvalidateRect(NULL);
}

void CShowTimeDJList::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);
	CMemDC dcMem(&dc, rect, true);	// �õ�ɫ��䱳��

	dcMem.SetBkMode(TRANSPARENT);
	dcMem.FillSolidRect(0,0,rect.Width(), rect.Height(),RGB(0,0,0));
	
	//���Ʊ���
	DrawWndBack(&dcMem);

	//���Ƶ�����Ϣ
	if (NULL!=m_pUserDJInfo)
	{
		for (int i=0; i<m_pUserDJInfo->m_vecUserUseDJList.size(); i++)
		{
			DrawDJTimeItem(&dcMem, i, DJ_DRAW_TIME_X, DJ_DRAW_TIME_Y+i*DJ_DRAW_TIME_INTERVAL);
		}
	}
}


//��ʾ����
void CShowTimeDJList::SetShowWnd(bool bShow)
{
	if (false == bShow)
	{
		ShowWindow(SW_HIDE);
	}
	else
	{
		if (NULL!=m_pUserDJInfo&&m_pUserDJInfo->m_vecUserUseDJList.size() > 0)
		{
			SetResetWndSize();
			ShowWindow(SW_SHOW);
		}
	}
}
BOOL CShowTimeDJList::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CShowTimeDJList::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (IDI_DJ_COUNT_TIME_ID == nIDEvent)
	{
		if (NULL!=m_pUserDJInfo)
		{
			//ʹ������ʱ���ߵ���ʱ.������������ʱ����.
			bool bIsChanged = m_pUserDJInfo->SetTimeDJAndClearDJ();
			if (true == bIsChanged)
			{
				SetResetWndSize();
			}
			InvalidateRect(NULL);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

//���Ʊ���
void CShowTimeDJList::DrawWndBack(CDC *pDC)
{
	if (NULL == pDC)
	{
		return;
	}

	CRect wndrect;
	GetClientRect(&wndrect);

	int iCountHeight = 0;
	int iImageTopHeight = 0;
	if (!m_ImageWndTop.IsNull())
	{//��
		m_ImageWndTop.DrawImage(pDC, 0, 0);
		iImageTopHeight = m_ImageWndTop.GetHeight();
		iCountHeight += iImageTopHeight;
	}
	if (!m_ImageWndDown.IsNull())
	{//��
		int iDownHeight = m_ImageWndDown.GetHeight();
		m_ImageWndDown.DrawImage(pDC, 0, wndrect.bottom-iDownHeight);
		iCountHeight += iDownHeight;
	}
	if (!m_ImageWndCenter.IsNull())
	{//��
		int iDrawCenterHeight = wndrect.Height()-iCountHeight;//�м�߶�
		int iImageCenterHeight = m_ImageWndCenter.GetHeight();
		int iImageCenterWidth = m_ImageWndCenter.GetWidth();
		iCountHeight = 0;
		while (iDrawCenterHeight>0)
		{//��
			if (iImageCenterHeight > iDrawCenterHeight)
			{
				iCountHeight = iDrawCenterHeight;
			}
			else
			{
				iCountHeight = iImageCenterHeight;
			}
			m_ImageWndCenter.DrawImage(pDC, 0, iImageTopHeight, iImageCenterWidth, iCountHeight, 0, 0, iImageCenterWidth, iCountHeight);
			iDrawCenterHeight -= iCountHeight;
			iImageTopHeight += iCountHeight;
		}
	}
}

//���Ƶ���ʱ��
void CShowTimeDJList::DrawDJTimeItem(CDC *pDC, int iIndex, int iX, int iY)
{
	if (NULL==pDC || NULL==m_pUserDJInfo || iIndex>=m_pUserDJInfo->m_vecUserUseDJList.size())
	{
		return;
	}

	//���Ƶ��߱�����
	if (!m_ImageDJBackFrame.IsNull())
	{
		m_ImageDJBackFrame.DrawImage(pDC, iX, iY);
	}
	CPngImage* pImage = m_pUserDJInfo->GetImageFromeDJID(m_pUserDJInfo->m_vecUserUseDJList[iIndex].idjID);
	if (NULL!=pImage&&!pImage->IsNull())
	{
		pImage->DrawImage(pDC, iX+4, iY+3);
	}

	if (!m_ImageDJTimeNums.IsNull())
	{//ʱ��ͼƬ
		DWORD iTime = m_pUserDJInfo->m_vecUserUseDJList[iIndex].dwResidualTimeCount;
		//����ʱ��
		if (iTime>0)
		{
			int iTimeX = iX+34;
			int iTimeY = iY+4;
			int iTimeWidth = m_ImageDJTimeNums.GetWidth()/11;
			int iTimeHeight = m_ImageDJTimeNums.GetHeight();
			//����ʱ
			int iHour = iTime/3600;
			int iMinute = (iTime%3600)/60;
			if (iHour>0 || iMinute>0)
			{//��ʱ����ʾ
				if (iHour<10)
				{
					m_ImageDJTimeNums.DrawImage(pDC, iTimeX, iTimeY, iTimeWidth, iTimeHeight, 0, 0, iTimeWidth, iTimeHeight);
					m_ImageDJTimeNums.DrawImage(pDC, iTimeX+iTimeWidth, iTimeY, iTimeWidth, iTimeHeight, iHour*iTimeWidth, 0, iTimeWidth, iTimeHeight);
				}
				else
				{
					m_ImageDJTimeNums.DrawImage(pDC, iTimeX, iTimeY, iTimeWidth, iTimeHeight, (iHour/10)*iTimeWidth, 0, iTimeWidth, iTimeHeight);
					m_ImageDJTimeNums.DrawImage(pDC, iTimeX+iTimeWidth, iTimeY, iTimeWidth, iTimeHeight, (iHour%10)*iTimeWidth, 0, iTimeWidth, iTimeHeight);
				}
				iTimeX += 2*iTimeWidth;

				//���Ʒָ���
				m_ImageDJTimeNums.DrawImage(pDC, iTimeX, iTimeY, iTimeWidth, iTimeHeight, 10*iTimeWidth, 0, iTimeWidth, iTimeHeight);
				iTimeX += iTimeWidth;

				//���Ʒ�
				if (iMinute<10)
				{
					m_ImageDJTimeNums.DrawImage(pDC, iTimeX, iTimeY, iTimeWidth, iTimeHeight, 0, 0, iTimeWidth, iTimeHeight);
					m_ImageDJTimeNums.DrawImage(pDC, iTimeX+iTimeWidth, iTimeY, iTimeWidth, iTimeHeight, iMinute*iTimeWidth, 0, iTimeWidth, iTimeHeight);
				}
				else
				{
					m_ImageDJTimeNums.DrawImage(pDC, iTimeX, iTimeY, iTimeWidth, iTimeHeight, (iMinute/10)*iTimeWidth, 0, iTimeWidth, iTimeHeight);
					m_ImageDJTimeNums.DrawImage(pDC, iTimeX+iTimeWidth, iTimeY, iTimeWidth, iTimeHeight, (iMinute%10)*iTimeWidth, 0, iTimeWidth, iTimeHeight);
				}
			}
			else
			{//����һ����
				m_ImageNoMinute.DrawImage(pDC, iTimeX, iTimeY+5);
			}
		}
	}
	
}