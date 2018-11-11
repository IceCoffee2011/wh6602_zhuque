#include "stdafx.h"
#include "DlgBulletShop.h"
#include "resource.h"
#include "GameClientViewDX.h"

IMPLEMENT_DYNAMIC(CDlgBulletShop, CDialog)
BEGIN_MESSAGE_MAP(CDlgBulletShop, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

//���캯��
CDlgBulletShop::CDlgBulletShop(CGameClientViewDX * pGameClientViewDX, enBulletCountKind BulletCountKindSelected, LONG lCellScore):CDialog(IDD_BULLET_SHOP)
{
	//����ͼƬ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_PngBack.LoadImage(hInstance,TEXT("BULLET_SHOP_BACK"));
	m_PngBullet100.LoadImage(hInstance,TEXT("BULET100"));
	m_PngBullet600.LoadImage(hInstance,TEXT("BULET600"));
	m_PngBullet800.LoadImage(hInstance,TEXT("BULET800"));
	m_PngReturn.LoadImage(hInstance,TEXT("BT_RETURN"));
	m_PngBuy.LoadImage(hInstance,TEXT("BT_BUY_BULET"));
	m_PngHelp.LoadImage(hInstance,TEXT("BT_BULET_HELP"));

	//״̬����
	m_CurButtonsID=enButtonsID_Invalidate;
	m_CurBulletID=enButtonsID(enButtonsID_Bullet100+(BulletCountKindSelected-enBulletCountKind_100));
	m_bLeftButtonDown=false;
	m_lCellScore=lCellScore;
	
	//�������
	m_pGameClientViewDX=pGameClientViewDX;
}

//��������
CDlgBulletShop::~CDlgBulletShop()
{
}

//�ؼ���
void CDlgBulletShop::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
}

//��ʼ����
BOOL CDlgBulletShop::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("�����ӵ�"));

	//���ô�С
	MoveWindow(0,0,m_PngBack.GetWidth(),m_PngBack.GetHeight());

	//��������
	HRGN hRGN=CreateRoundRectRgn(0,0,m_PngBack.GetWidth(),m_PngBack.GetHeight(),10,10);
	if (NULL!=hRGN) SetWindowRgn(hRGN,TRUE);

	//����λ��
	m_rcBullet100.SetRect(21,27,135,173);
	m_rcBullet600.SetRect(m_rcBullet100.right,m_rcBullet100.top,m_rcBullet100.right+m_rcBullet100.Width(),m_rcBullet100.bottom);
	m_rcBullet800.SetRect(m_rcBullet600.right,m_rcBullet600.top,m_rcBullet600.right+m_rcBullet600.Width(),m_rcBullet600.bottom);
	m_rcReturn.SetRect(24,238,24+104,238+21);
	m_rcHelp.SetRect(m_rcReturn.right+10,m_rcReturn.top,m_rcReturn.right+m_rcReturn.Width()+10,m_rcReturn.bottom);
	m_rcBuy.SetRect(m_rcHelp.right+10,m_rcHelp.top,m_rcHelp.right+m_rcHelp.Width()+10,m_rcHelp.bottom);

	//͸������
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE, GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE) | 0x80000); 
	HINSTANCE hInst=LoadLibrary("User32.DLL"); 
	if(hInst) 
	{ 
		typedef BOOL     (WINAPI * MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC fun=NULL; 

		//ȡ��SetLayeredWindowAttributes����ָ�� 
		fun=(MYFUNC)GetProcAddress(hInst,"SetLayeredWindowAttributes"); 
		if(fun) fun(this->GetSafeHwnd(),RGB(0,1,3),230,2);
		FreeLibrary(hInst); 
	} 

	//���д���
	CenterWindow(m_pGameClientViewDX);

	return TRUE;
}

//�滭����
void CDlgBulletShop::OnPaint()
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

	//�滭ͼƬ
	if (enButtonsID_Bullet100==m_CurBulletID) 
		m_PngBullet100.DrawImage(&dcMemory,m_rcBullet100.left,m_rcBullet100.top,m_rcBullet100.Width(),m_rcBullet100.Height(),2*m_rcBullet100.Width(),0);
	else if (enButtonsID_Bullet100==m_CurButtonsID) 
		m_PngBullet100.DrawImage(&dcMemory,m_rcBullet100.left,m_rcBullet100.top,m_rcBullet100.Width(),m_rcBullet100.Height(),0*m_rcBullet100.Width(),0);
	else
		m_PngBullet100.DrawImage(&dcMemory,m_rcBullet100.left,m_rcBullet100.top,m_rcBullet100.Width(),m_rcBullet100.Height(),1*m_rcBullet100.Width(),0);

	if (enButtonsID_Bullet600==m_CurBulletID) 
		m_PngBullet600.DrawImage(&dcMemory,m_rcBullet600.left,m_rcBullet600.top,m_rcBullet600.Width(),m_rcBullet600.Height(),2*m_rcBullet600.Width(),0);
	else if (enButtonsID_Bullet600==m_CurButtonsID) 
		m_PngBullet600.DrawImage(&dcMemory,m_rcBullet600.left,m_rcBullet600.top,m_rcBullet600.Width(),m_rcBullet600.Height(),0*m_rcBullet600.Width(),0);
	else
		m_PngBullet600.DrawImage(&dcMemory,m_rcBullet600.left,m_rcBullet600.top,m_rcBullet600.Width(),m_rcBullet600.Height(),1*m_rcBullet600.Width(),0);

	if (enButtonsID_Bullet800==m_CurBulletID) 
		m_PngBullet800.DrawImage(&dcMemory,m_rcBullet800.left,m_rcBullet800.top,m_rcBullet800.Width(),m_rcBullet800.Height(),2*m_rcBullet800.Width(),0);
	else if (enButtonsID_Bullet800==m_CurButtonsID) 
		m_PngBullet800.DrawImage(&dcMemory,m_rcBullet800.left,m_rcBullet800.top,m_rcBullet800.Width(),m_rcBullet800.Height(),0*m_rcBullet800.Width(),0);
	else
		m_PngBullet800.DrawImage(&dcMemory,m_rcBullet800.left,m_rcBullet800.top,m_rcBullet800.Width(),m_rcBullet800.Height(),1*m_rcBullet800.Width(),0);

	if (enButtonsID_Return==m_CurButtonsID && true==m_bLeftButtonDown)
		m_PngReturn.DrawImage(&dcMemory,m_rcReturn.left,m_rcReturn.top,m_rcReturn.Width(),m_rcReturn.Height(),2*m_rcReturn.Width(),0);
	else if (enButtonsID_Return==m_CurButtonsID)
		m_PngReturn.DrawImage(&dcMemory,m_rcReturn.left,m_rcReturn.top,m_rcReturn.Width(),m_rcReturn.Height(),0*m_rcReturn.Width(),0);
	else
		m_PngReturn.DrawImage(&dcMemory,m_rcReturn.left,m_rcReturn.top,m_rcReturn.Width(),m_rcReturn.Height(),1*m_rcReturn.Width(),0);

	if (enButtonsID_Buy==m_CurButtonsID && true==m_bLeftButtonDown)
		m_PngBuy.DrawImage(&dcMemory,m_rcBuy.left,m_rcBuy.top,m_rcReturn.Width(),m_rcBuy.Height(),2*m_rcBuy.Width(),0);
	else if (enButtonsID_Buy==m_CurButtonsID)
		m_PngBuy.DrawImage(&dcMemory,m_rcBuy.left,m_rcBuy.top,m_rcBuy.Width(),m_rcBuy.Height(),0*m_rcBuy.Width(),0);
	else
		m_PngBuy.DrawImage(&dcMemory,m_rcBuy.left,m_rcBuy.top,m_rcBuy.Width(),m_rcBuy.Height(),1*m_rcBuy.Width(),0);

	if (enButtonsID_Help==m_CurButtonsID && true==m_bLeftButtonDown)
		m_PngHelp.DrawImage(&dcMemory,m_rcHelp.left,m_rcHelp.top,m_rcHelp.Width(),m_rcHelp.Height(),2*m_rcHelp.Width(),0);
	else if (enButtonsID_Help==m_CurButtonsID)
		m_PngHelp.DrawImage(&dcMemory,m_rcHelp.left,m_rcHelp.top,m_rcHelp.Width(),m_rcHelp.Height(),0*m_rcHelp.Width(),0);
	else
		m_PngHelp.DrawImage(&dcMemory,m_rcHelp.left,m_rcHelp.top,m_rcHelp.Width(),m_rcHelp.Height(),1*m_rcHelp.Width(),0);

	//��������
	CFont DrawFont;
	DrawFont.CreateFont(-12,0,0,0,FW_BOLD,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("����"));
	CFont * pOldFont=dcMemory.SelectObject(&DrawFont);
	dcMemory.SetTextColor(RGB(225,227,161));
	dcMemory.SetBkMode(TRANSPARENT);

	//�ӵ�����
	CString strCharge;
	CRect rcDraw;
	rcDraw.SetRect(m_rcBullet100.left,132,m_rcBullet100.right,m_rcBullet100.bottom);
	strCharge.Format(TEXT("��%ld���"),m_lBulletCharge[enBulletCountKind_100]*m_lCellScore);
	dcMemory.DrawText(strCharge,&rcDraw,DT_CENTER|DT_TOP);

	rcDraw.SetRect(m_rcBullet600.left,132,m_rcBullet600.right,m_rcBullet600.bottom);
	strCharge.Format(TEXT("��%ld���"),m_lBulletCharge[enBulletCountKind_600]*m_lCellScore);
	dcMemory.DrawText(strCharge,&rcDraw,DT_CENTER|DT_TOP);

	rcDraw.SetRect(m_rcBullet800.left,132,m_rcBullet800.right,m_rcBullet800.bottom);
	strCharge.Format(TEXT("��%ld���"),m_lBulletCharge[enBulletCountKind_800]*m_lCellScore);
	dcMemory.DrawText(strCharge,&rcDraw,DT_CENTER|DT_TOP);

	//�ӵ�Ч��
	WORD wMeChairID=m_pGameClientViewDX->GetMeChairID();
	CString strEffect;
	//������ɫ
	dcMemory.SetTextColor(RGB(255,0,0));
	if (INVALID_CHAIR!=wMeChairID)
	{
		//Ч���ж�
		if (m_pGameClientViewDX->GetBulletCount(wMeChairID,enBulletCountKind_800)>0)
			strEffect=TEXT("���٣�����Ч��");
		else if (m_pGameClientViewDX->GetBulletCount(wMeChairID,enBulletCountKind_600)>0)
			strEffect=TEXT("����Ч��");
		else
			strEffect=TEXT("��Ч��");
		dcMemory.TextOut(105,193,strEffect);
	}
	if (enButtonsID_Bullet800==m_CurBulletID) strEffect=TEXT("���٣�����Ч��");
	else if (enButtonsID_Bullet600==m_CurBulletID) strEffect=TEXT("����Ч��");
	else strEffect=TEXT("��Ч��");
	dcMemory.TextOut(105,210,strEffect);

	//�滭����
	CRect rcClip;
	dc.GetClipBox(&rcClip);
	dc.BitBlt(rcClip.left,rcClip.top,rcClip.Width(),rcClip.Height(),&dcMemory,rcClip.left,rcClip.top,SRCCOPY);

	//�����Դ
	dcMemory.SelectObject(pOldFont);
	DrawFont.DeleteObject();
	dcMemory.SelectObject(pOldBmp);
	bmpMemory.DeleteObject();
	dcMemory.DeleteDC();
}

//��������
enButtonsID CDlgBulletShop::GetButtonID(CPoint &ptPos)
{
	//�����ж�
	if (m_rcBullet100.PtInRect(ptPos)) return enButtonsID_Bullet100;
	else if (m_rcBullet600.PtInRect(ptPos)) return enButtonsID_Bullet600;
	else if (m_rcBullet800.PtInRect(ptPos)) return enButtonsID_Bullet800;
	else if (m_rcReturn.PtInRect(ptPos)) return enButtonsID_Return;
	else if (m_rcBuy.PtInRect(ptPos)) return enButtonsID_Buy;
	else if (m_rcHelp.PtInRect(ptPos)) return enButtonsID_Help;

	return enButtonsID_Invalidate;
}

//�ƶ���Ϣ
void CDlgBulletShop::OnMouseMove(UINT nFlags, CPoint point)
{
	//��ȡ��ʶ
	enButtonsID ButtonsID=GetButtonID(point);
	if (m_CurButtonsID!=ButtonsID)
	{
		m_CurButtonsID=ButtonsID;
		InvalidateRect(NULL,FALSE);
	}

	__super::OnMouseMove(nFlags, point);
}

//������Ϣ
void CDlgBulletShop::OnLButtonUp(UINT nFlags, CPoint point)
{
	//�������
	//ReleaseCapture();

	//��ȡ��ʶ
	if (true==m_bLeftButtonDown)
	{
		enButtonsID ButtonsID=GetButtonID(point);
		if (enButtonsID_Invalidate!=ButtonsID) PostMessage(WM_COMMAND,WPARAM(ButtonsID),0);
	}
	
	//���ñ�ʶ
	m_bLeftButtonDown=false;

	__super::OnLButtonUp(nFlags, point);
}

//��갴��
void CDlgBulletShop::OnLButtonDown(UINT nFlags, CPoint point)
{
	//��Ϣģ��
	bool bHitNClient=true;
	if (m_rcBullet100.PtInRect(point))  bHitNClient=false;
	if (m_rcBullet600.PtInRect(point))  bHitNClient=false;
	if (m_rcBullet800.PtInRect(point))  bHitNClient=false;
	if (m_rcReturn.PtInRect(point))  bHitNClient=false;
	if (m_rcBuy.PtInRect(point))  bHitNClient=false;
	if (m_rcHelp.PtInRect(point))  bHitNClient=false;
	if (true==bHitNClient) PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	//���ñ�ʶ
	m_bLeftButtonDown=true;

	//�������
	//SetCapture();

	//��ȡ��ʶ
	enButtonsID ButtonsID=GetButtonID(point);
	if (enButtonsID_Invalidate!=ButtonsID)
	{
		m_CurButtonsID=ButtonsID;
		InvalidateRect(NULL,FALSE);

		//��������
		m_pGameClientViewDX->PlayMp3Sound(TEXT("Effect\\SelectBullet.mp3"),false);
	}

	__super::OnLButtonDown(nFlags, point);
}

//������Ϣ
BOOL CDlgBulletShop::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	switch (wParam)
	{
	case enButtonsID_Bullet100:
	case enButtonsID_Bullet600:
	case enButtonsID_Bullet800:
		{
			m_CurButtonsID=enButtonsID(wParam);
			m_CurBulletID=enButtonsID(wParam);
			InvalidateRect(NULL,FALSE);

			return TRUE;
		}
	case enButtonsID_Return:
		{
			//�رմ���
			PostMessage(WM_CLOSE,0,0);

			return TRUE;
		}
	case enButtonsID_Buy:
		{
			//�رմ���
			__super::OnOK();

			return TRUE;
		}
	case enButtonsID_Help:
		{
			//�رմ���
			__super::OnCancel();

			//�����Ի���
			CBulletHelpDlg BulletHelpDlg(m_pGameClientViewDX);
			BulletHelpDlg.DoModal();

			return TRUE;
		}
	}

	return __super::OnCommand(wParam, lParam);
}

//�ӵ�����
enBulletCountKind CDlgBulletShop::GetBulletKind()
{
	if (enButtonsID_Bullet100==m_CurBulletID) return enBulletCountKind_100;
	else if (enButtonsID_Bullet600==m_CurBulletID) return enBulletCountKind_600;
	else if (enButtonsID_Bullet800==m_CurBulletID) return enBulletCountKind_800;

	return enBulletCountKind_End;
}

//���÷���
void CDlgBulletShop::SetBulletCharge(LONG lBulletCharge[enBulletCountKind_End])
{
	//������Ϣ
	CopyMemory(m_lBulletCharge,lBulletCharge,sizeof(m_lBulletCharge));
}