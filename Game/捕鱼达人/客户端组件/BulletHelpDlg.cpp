#include "StdAfx.h"
#include "BulletHelpDlg.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CBulletHelpDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CBulletHelpDlg::CBulletHelpDlg(CWnd	*pViewWnd):CDialog(IDD_BULLET_HELP),m_pViewWnd(pViewWnd)
{
	//����ͼƬ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_PngBack.LoadImage(hInstance,TEXT("BULLET_HELP"));

	return;
}

//��������
CBulletHelpDlg::~CBulletHelpDlg()
{
}

//�ؼ���
void CBulletHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_CLOSE, m_btClose);
	__super::DoDataExchange(pDX);
}

//�ػ�����
void CBulletHelpDlg::OnPaint() 
{
	//�滭DC
	CPaintDC dc(this);

	//������Դ
	CDC dcMemory;
	CBitmap bmpMemory;

	//��ȡ��С
	CRect rcClient;
	GetClientRect(rcClient);

	//������Դ
	dcMemory.CreateCompatibleDC(&dc);
	bmpMemory.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
	CBitmap *pOldBmp=dcMemory.SelectObject(&bmpMemory);

	//�滭����
	m_PngBack.DrawImage(&dcMemory,0,0);

	////��������
	//CFont DrawFont;
	//DrawFont.CreateFont(-12,0,0,0,FW_BOLD,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("����"));
	//CFont * pOldFont=dcMemory.SelectObject(&DrawFont);
	//dcMemory.SetTextColor(RGB(225,227,161));
	//dcMemory.SetBkMode(TRANSPARENT);

	//�滭����
	CRect rcClip;
	dc.GetClipBox(&rcClip);
	dc.BitBlt(rcClip.left,rcClip.top,rcClip.Width(),rcClip.Height(),&dcMemory,rcClip.left,rcClip.top,SRCCOPY);

	//�����Դ
	/*dcMemory.SelectObject(pOldFont);
	DrawFont.DeleteObject();*/
	dcMemory.SelectObject(pOldBmp);
	bmpMemory.DeleteObject();
	dcMemory.DeleteDC();

	return;
}

//��ʼ����
BOOL CBulletHelpDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//���ô�С
	MoveWindow(0,0,m_PngBack.GetWidth(),m_PngBack.GetHeight());

	//��������
	HRGN hRGN=CreateRoundRectRgn(0,0,m_PngBack.GetWidth(),m_PngBack.GetHeight(),10,10);
	if (NULL!=hRGN) SetWindowRgn(hRGN,TRUE);

	//��ťͼƬ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btClose.SetButtonImage(IDB_BT_ISEE,hInstance,3,false);

	//����λ��
	CRect rcClient,rcButton;
	GetClientRect(rcClient);
	m_btClose.GetWindowRect(rcButton);
	m_btClose.MoveWindow((rcClient.Width()-rcButton.Width())/2,rcClient.Height()-rcButton.Height()-5,rcButton.Width(),rcButton.Height());

	//͸������
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE, GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE) | 0x80000); 
	HINSTANCE hInst=LoadLibrary("User32.DLL"); 
	if(hInst) 
	{ 
		typedef BOOL     (WINAPI * MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC fun=NULL; 

		//ȡ��SetLayeredWindowAttributes����ָ�� 
		fun=(MYFUNC)GetProcAddress(hInst,"SetLayeredWindowAttributes"); 
		if(fun) fun(this->GetSafeHwnd(),RGB(39,75,113),240,2);
		FreeLibrary(hInst); 
	}

	//���д���
	CenterWindow(m_pViewWnd);
	
	return TRUE;
}

//�����
BOOL CBulletHelpDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	//��Ϣ����
	switch(nCommandID)
	{
	case IDC_CLOSE:
		{
			__super::OnOK();

			//������Ϣ
			//AfxGetMainWnd()->PostMessage(WM_SHOW_BULLET_SHOP,0,0);

			return TRUE;
		}
	}
	return __super::OnCommand(wParam,lParam);
}


//�����Ϣ
void CBulletHelpDlg::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//��Ϣģ��
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));	

	return;
}
//////////////////////////////////////////////////////////////////////////


