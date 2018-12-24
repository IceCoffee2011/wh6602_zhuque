// UserAction.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UserAction.h"
#include ".\useraction.h"
#include "Resource.h"
#include "GameFrameDlg.h"

#include "MemDC.h"

#define			CREATE_BT_USER_ACTION_INDEX			2200
#define			BT_X								12
#define			BT_Y								10
#define			BT_WIDTH							62
#define			BT_HEIGHT							29
#define			BT_WIDTH_INTERVAL					2
#define			BT_HEIGHT_INTERVAL					2

//������������õĶ�ʱ��
#define			IDI_TIME_SEND_USER_ACTION			12350
//����Ƶ���ķ��Ͷ���
#define			IDI_TIME_SEND_TIME_INTERVAL			12351


#define			TIME_SEND_USER_ACTION_INTERVAL		8000
#define			TIME_SEND_TIME_INTERVAL_INTERVAL	3000

// CUserAction

IMPLEMENT_DYNAMIC(CUserAction, CWnd)
CUserAction::CUserAction()
{
	EnableAutomation();
	m_bHovering = false;
	m_btCurUserActionTag = -1;
	m_bIsSend = false;
	m_bIsAlreadySend = true;
}

CUserAction::~CUserAction()
{
}

void CUserAction::OnFinalRelease()
{
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease�����ཫ�Զ�
	// ɾ���ö����ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CUserAction, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_WM_SIZE()
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CUserAction, CWnd)
END_DISPATCH_MAP()

// ע��: ������� IID_IUserAction ֧��
//��֧������ VBA �����Ͱ�ȫ�󶨡��� IID ����ͬ���ӵ� .IDL �ļ��е�
//���Ƚӿڵ� GUID ƥ�䡣

// {4CD84A6A-40A4-458F-9085-FD461E1FCB90}
static const IID IID_IUserAction =
{ 0x4CD84A6A, 0x40A4, 0x458F, { 0x90, 0x85, 0xFD, 0x46, 0x1E, 0x1F, 0xCB, 0x90 } };

BEGIN_INTERFACE_MAP(CUserAction, CWnd)
	INTERFACE_PART(CUserAction, IID_IUserAction, Dispatch)
END_INTERFACE_MAP()


// CUserAction ��Ϣ�������


int CUserAction::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	// ���͸����չ��ʽ
	//SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE)^0x80000);
	//HINSTANCE hInst = LoadLibrary("User32.DLL");
	//if(hInst)
	//{
	//	typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD);
	//	MYFUNC fun = NULL;
	//	//ȡ��SetLayeredWindowAttributes����ָ��
	//	fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
	//	if(fun)
	//	{
	//		fun(this->GetSafeHwnd(),100,10,2);
	//	}
	//	FreeLibrary(hInst);
	//}

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
	m_ImageBack.SetLoadInfo(IDB_USER_ACTION_BACK_IMAGE, hInstance, false);

	int iCountLine = 0;
	CRect CreateRect(0,0,0,0);
	//������ť
	for (int i=0; i<20; i++)
	{
		m_btUserAction[i].Create(TEXT(""),WS_CHILD|WS_VISIBLE,CreateRect,this,CREATE_BT_USER_ACTION_INDEX+i);
		m_btUserAction[i].SetButtonImage(IDB_BT_USER_ACTION0+i, hInstance, false, 4);
		if ((i>0) && (0==i%5))
		{
			++iCountLine;
		}
		int iX = BT_X+(i%5)*(BT_WIDTH+BT_WIDTH_INTERVAL);
		int iY = BT_Y+iCountLine*(BT_HEIGHT+BT_HEIGHT_INTERVAL);
		m_btUserAction[i].SetWindowPos(NULL, iX, iY, BT_WIDTH, BT_HEIGHT, 0);
	}
	m_bIsSend = true;
	m_bIsAlreadySend = true;
	return 0;
}

void CUserAction::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()

	CRect rect;
	GetClientRect(&rect);
	CMemDC dcMem(&dc, rect, true);	// �õ�ɫ��䱳��

	DrawWindow(&dcMem);
}

void CUserAction::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent)
	{
	case IDI_TIME_SEND_USER_ACTION:
		{//ʱ�䵽�ˣ�ȡ�����͡�
			KillTimer(IDI_TIME_SEND_USER_ACTION);
			m_bIsSend = false;
			m_btCurUserActionTag = -1;
			m_bIsAlreadySend = true;
		}
		break;
	case IDI_TIME_SEND_TIME_INTERVAL:
		{
			KillTimer(IDI_TIME_SEND_TIME_INTERVAL);
			m_bIsAlreadySend = true;
		}
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

void CUserAction::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CWnd::OnLButtonDown(nFlags, point);
}

void CUserAction::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CWnd::OnLButtonUp(nFlags, point);
}

void CUserAction::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_bHovering==false)
	{
		//ע����Ϣ
		m_bHovering=true;
		Invalidate(FALSE);
		TRACKMOUSEEVENT TrackMouseEvent;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.hwndTrack=GetSafeHwnd();
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		_TrackMouseEvent(&TrackMouseEvent);
	}
	CWnd::OnMouseMove(nFlags, point);
}
//����뿪��Ϣ
LRESULT CUserAction::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	//ShowWindow(SW_HIDE);
	m_bHovering=false;
	return 0;
}

//��õ�ǰ�û�����Ķ�����ʶ
BYTE CUserAction::GetCurUserActionTag()
{
	return m_btCurUserActionTag;
}
//�Ƿ���Է���
bool CUserAction::GetCurSendStatus()
{
	return m_bIsSend;
}
//�����Ƿ���Է���
void CUserAction::SetCurSendStatus(bool bIsSend)
{
	m_bIsSend = bIsSend;
}
//�����Ƿ���Է���
void CUserAction::SetTimeIntervalSend(bool bIsSend)
{
	m_bIsAlreadySend = bIsSend;
}
//��ó�ʱ�Ƿ���Է���
bool CUserAction::GetTimeIntervalSendStatus()
{
	return m_bIsAlreadySend;
}
//��������
void CUserAction::SetResetData()
{
	m_btCurUserActionTag = -1;
	m_bIsSend = false;
	m_bIsAlreadySend = false;
	KillTimer(IDI_TIME_SEND_USER_ACTION);
	KillTimer(IDI_TIME_SEND_TIME_INTERVAL);
	SetTimer(IDI_TIME_SEND_TIME_INTERVAL, TIME_SEND_TIME_INTERVAL_INTERVAL, NULL);
}



void CUserAction::AddRectRgn(CDC* pDC,CRgn* rgn,CRect* rect,COLORREF col)
{
	for(int y=rect->top;y<rect->bottom;y++)
		for(int x=rect->left;x<rect->right;x++)
			if (pDC->GetPixel(x,y)==col)
			{
				CRgn rgnAdd;
				rgnAdd.CreateRectRgn (x, y, x+1, y+1);
				rgn->CombineRgn(rgn, &rgnAdd, RGN_DIFF);
				DeleteObject(rgnAdd);
			}
}
void CUserAction::UpdateRgn(CDC * pDC)
{
	CImageHandle ImageHandle(&m_ImageBack);
	CRect rcClient;
	GetClientRect(&rcClient);
	CRgn rgn;
	COLORREF col = RGB(255,0,255);  // mask color
	CRect rc(0,0,0,0);
	rgn.CreateRectRgn (0, 0, rcClient.Width(), rcClient.Height());

	//����
	rc.bottom =  m_ImageBack.GetHeight();
	rc.right = m_ImageBack.GetWidth();
	AddRectRgn(pDC,&rgn,&rc,col);
	SetWindowRgn (rgn,TRUE);
	//SetForegroundWindow();
}

bool CUserAction::DrawWindow(CDC * pDC)
{
	if (NULL != pDC && NULL != pDC->m_hDC)
	{
		CImageHandle ImageHandle(&m_ImageBack);
		pDC->SetBkMode(TRANSPARENT);
		pDC->FillSolidRect(0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),RGB(255,0,255));

		//�滭����ͼƬ
		CImageHandle HandleCenter(&m_ImageBack);
		m_ImageBack.AlphaDrawImage(pDC, 0, 0, RGB(255, 0, 255));
		return true;
	}
	return false;
}

void CUserAction::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

#if 0  // ɾ����һ�λ���������
	CClientDC dc(this);
	CRect rect;
	GetClientRect(&rect);
	CMemDC dcMem(&dc, rect, true);	// �õ�ɫ��䱳��

	if (DrawWindow(&dcMem))
	{
		UpdateRgn(&dcMem);
		CRect rect,rc,rc2;
		GetClientRect(&rect);
		CString sss;
		if( GetUpdateRect(&rc) )
		{
			rc2 = rc;
			if( rc.left < rc.right )
			{
				rc2.top = rect.top;
				rc2.bottom = rect.bottom;
				InvalidateRect(&rc2,0);
			}
			rc2 = rc;
			if( rc.top < rc.bottom )
			{
				rc2.left = rect.left;
				rc2.right = rect.right;
				InvalidateRect(&rc2,0);
			}
		}
		else
			InvalidateRect(0);
	}
#endif
}

BOOL CUserAction::OnCommand(WPARAM wParam, LPARAM lParam)
{
	CWnd::OnCommand(wParam, lParam);
	// TODO: �ڴ����ר�ô����/����û���
	WORD wIndex = LOWORD(wParam)-CREATE_BT_USER_ACTION_INDEX;
	if (wIndex>=0 && wIndex<=19)
	{
		m_bIsSend = true;
		m_btCurUserActionTag = wIndex;
		KillTimer(IDI_TIME_SEND_USER_ACTION);
		ShowWindow(SW_HIDE);

		CWnd* pMainWnd = AfxGetMainWnd();
		switch (m_btCurUserActionTag)
		{//����ж����.
		case 0:
		case 2:
		case 5:
		case 6:
		case 9:
		case 11:
		case 15:
		case 19:
			{
				if (NULL != pMainWnd)
				{
					((CGameFrameDlg*)pMainWnd)->InsertSystemString("��ѡ��Ҫ���͵Ķ���");
				}
				SetTimer(IDI_TIME_SEND_USER_ACTION, TIME_SEND_USER_ACTION_INTERVAL, NULL);
				return true;
			}
			break;
		default:
			break;
		}
		
		//û�ж���ģ�����ֱ�ӷ��͡�
		
		if (NULL != pMainWnd)
		{
			((CGameFrameDlg*)pMainWnd)->SendUserActionMsg(-1);
		}
	}
	return true;
}
