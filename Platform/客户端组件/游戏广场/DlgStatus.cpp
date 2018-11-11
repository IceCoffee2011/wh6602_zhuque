#include "Stdafx.h"
#include "DlgStatus.h"
#include "GlobalUnits.h"
#include ".\dlgstatus.h"

//��ʱ�� ID
#define IDT_STATUS_IMANGE					1					//״̬ͼ��
#define STATUS_IMAGE_COUNT					8					//״̬��Ŀ

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgStatus, CDialog)
	ON_BN_CLICKED(IDC_CLOSE, OnBnClickedClose)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgStatus::CDlgStatus() : CDialog(IDD_STATUS)
{
	m_wImagePos=0;
	m_pNotifyWnd=NULL;
	m_iAction=0;
	return;
}

//��������
CDlgStatus::~CDlgStatus()
{
}

//�ؼ���
void CDlgStatus::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CLOSE, m_btCancel);
}

//�ػ���Ϣ
void CDlgStatus::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

	/*CPaintDC dc(this);
	//��ȡλ��
	CRect rcClientRect;
	GetClientRect(&rcClientRect);

	CDC memdc;
	CBitmap   bmp;
	memdc.CreateCompatibleDC(&dc);
	bmp.CreateCompatibleBitmap(&dc, 300, 300);
	memdc.SelectObject(&bmp);

	if (!m_ImageBack.IsNull())
	{
	WORD wWidth=m_ImageBack.GetWidth()/12;
	m_ImageBack.DrawImage(&memdc, 0, 0, wWidth, m_ImageBack.GetHeight(), m_iAction*wWidth, 0);
	}
	dc.BitBlt(0, 0, rcClientRect.Width(), rcClientRect.Height(), &memdc, 0, 0, SRCCOPY);
	bmp.DeleteObject();
	memdc.DeleteDC();*/

	/*cxf
	//�滭����
	CImage ImageBuffer;
	ImageBuffer.Create(rcClientRect.Width(),rcClientRect.Height(),32);
	CDC * pDCBuffer=CDC::FromHandle(ImageBuffer.GetDC());
	pDCBuffer->FillSolidRect(0,0,rcClientRect.Width(),rcClientRect.Height(),CSkinDialog::m_SkinAttribute.m_crBackGround);
	if (!m_ImageBack.IsSetLoadInfo())
	{
	//��ȡ��Դ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_ImageBack.SetLoadInfo(IDB_DLG_STATUS_BACK, hInstance, false);
	}
	CImageHandle ImageHandle(&m_ImageBack);
	if (!m_ImageBack.IsNull() && m_ImageBack.IsSetLoadInfo())
	{
	m_ImageBack.AlphaDrawImage(pDCBuffer, 0, 0, RGB(255, 0, 255));
	}

	//�滭״̬
	CImage ImageStatus;
	ImageStatus.LoadFromResource(AfxGetInstanceHandle(),IDB_CONNECT_STATUS);
	CDC * pDCImage=CDC::FromHandle(ImageStatus.GetDC());
	AlphaDrawImage(pDCBuffer,10,10,32,32,pDCImage,m_wImagePos*32,0,32,32,RGB(255,0,255));

	//�滭��Ϣ
	CFont DrawFont;
	DrawFont.CreateFont(-12,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("����"));
	CFont * pOldFont=pDCBuffer->SelectObject(&DrawFont);
	pDCBuffer->SetTextColor(RGB(10,10,10));
	pDCBuffer->SetBkMode(TRANSPARENT);
	pDCBuffer->TextOut(60,20,m_strMessage);
	pDCBuffer->SelectObject(pOldFont);
	DrawFont.DeleteObject();

	//�滭����
	ImageBuffer.BitBlt(dc,0,0);
	//������Դ
	ImageStatus.ReleaseDC();
	ImageBuffer.ReleaseDC();
	ImageStatus.Destroy();
	ImageBuffer.Destroy();
	*/
	return;
}

//��ʱ����Ϣ
void CDlgStatus::OnTimer(UINT nIDEvent)
{
	if (nIDEvent==IDT_STATUS_IMANGE)
	{
		/*cxf
		CRect rcImageRect;
		rcImageRect.SetRect(10,10,42,42);
		m_wImagePos=(m_wImagePos+1)%STATUS_IMAGE_COUNT;
		InvalidateRect(&rcImageRect,FALSE);
		*/
		if (m_iAction>10)
		{
			m_iAction=0;
		}
		else
		{
			m_iAction++;
		}
		UpdateClockDisplay();

		return;
	}
	__super::OnTimer(nIDEvent);
}

//������Ϣ
bool CDlgStatus::ShowStatusMessage(LPCTSTR pszMessage, CWnd * pNotifyWnd)
{
	//������Ϣ
	m_pNotifyWnd=pNotifyWnd;
	m_strMessage=pszMessage;

	//��������
	if (m_hWnd==NULL)
	{
		//cxf Create(IDD_STATUS);
		DoModal();
	}

	//Invalidate(FALSE);
	//SetWindowPos(&wndTopMost, 0, 0, 326, 53, SWP_NOMOVE|SWP_NOZORDER|SWP_SHOWWINDOW);
	//cxf ::SetWindowPos(m_hWnd, NULL, 0, 0, 326, 53, SWP_NOMOVE | SWP_NOZORDER | SWP_SHOWWINDOW);
	//��ʾ����
	SetForegroundWindow();

	return true;
}

//���ٴ���
bool CDlgStatus::DestroyStatusWnd(CWnd * pNotifyWnd)
{
	KillTimer(IDT_STATUS_IMANGE);
	if ((m_pNotifyWnd==pNotifyWnd)||(pNotifyWnd==NULL))
	{
		//cxf
		CDialog::OnCancel();
		//cxf DestroyWindow();
		return true;
	}
	return false;
}

//ȡ����ť
void CDlgStatus::OnBnClickedClose()
{
	if (m_pNotifyWnd==NULL)
	{
		return;
	}
	if (!::IsWindow(m_pNotifyWnd->GetSafeHwnd())) 
	{
		return;
	}
	KillTimer(IDT_STATUS_IMANGE);
	::PostMessage(m_pNotifyWnd->GetSafeHwnd(), WM_COMMAND,IDM_CANCEL_CONNECT,0);
	return;
}

//////////////////////////////////////////////////////////////////////////

BOOL CDlgStatus::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	//cxf
	CString strPath("");
	GetResPath("\\data\\status\\",strPath);

	WCHAR * pWC = NULL;
	strPath.Append("BKStatus.png");
	pWC = strPath.AllocSysString();
	m_ImageBack.LoadFromFile(pWC);
	if (!m_ImageBack.IsNull())
	{
		m_BakWidth = m_ImageBack.GetWidth()/12;
		m_BakHeight = m_ImageBack.GetHeight();
	}
	if(0==m_BakWidth && m_BakWidth==m_BakHeight) return false;

	int width   =   GetSystemMetrics(SM_CXSCREEN); 
	int height   =   GetSystemMetrics(SM_CYSCREEN);
	::SetWindowPos(m_hWnd, NULL, (width-m_BakWidth)/2, (height-m_BakHeight)/2, m_BakWidth, m_BakHeight, SWP_NOZORDER | SWP_SHOWWINDOW);

	m_Blend.BlendOp=0;
	m_Blend.BlendFlags=0; 
	m_Blend.AlphaFormat=1; 
	m_Blend.SourceConstantAlpha=255;

	m_iAction=0;
	SetTimer(IDT_STATUS_IMANGE,100,NULL);

	//cxf 
	//���WS_EX_LAYERED(0x80000)��չ��� //����User32.DLL��̬���ӿ�
	/*ModifyStyleEx(0, 0x80000);
	HMODULE hModule = LoadLibrary("User32.DLL"); 
	if (hModule != NULL) 
	{ 
	typedef BOOL (WINAPI *FUNC)(HWND, COLORREF, BYTE, DWORD); 
	//���SetLayeredWindowAttributes����ָ�� 
	FUNC func = (FUNC)GetProcAddress(hModule, "SetLayeredWindowAttributes");
	if(func != NULL)
	{
	func(GetSafeHwnd(), 0, 128, 2); 
	}
	FreeLibrary(hModule); 
	}*/

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgStatus::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	/*if (m_memdc.m_hDC) m_memdc.DeleteDC();
	CWindowDC dc(this);
	CBitmap   bmp;
	m_memdc.CreateCompatibleDC(&dc);
	bmp.CreateCompatibleBitmap(&dc, cx, cy);
	CBitmap * pOldbmp = (CBitmap *)m_memdc.SelectObject(&bmp);
	if (pOldbmp) pOldbmp->DeleteObject();
	bmp.DeleteObject();*/
}

void CDlgStatus::OnDestroy()
{
	KillTimer(IDT_STATUS_IMANGE);
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	//	if (m_memdc.m_hDC) m_memdc.DeleteDC();
}

//��ǰ·��
void CDlgStatus::GetCurrentDirectoryPath(CString &szPath)
{
	szPath="";
	TCHAR szPathTemp[MAX_PATH]={0};
	GetCurrentDirectory(MAX_PATH, szPathTemp);
	szPath.Append(szPathTemp);
}

//������Դ·��
void CDlgStatus::GetResPath(CString strClipName, CString& strPath)
{
	CString strModuleFileName("");
	GetCurrentDirectoryPath(strModuleFileName);
	strModuleFileName.Append(strClipName);
	strPath = strModuleFileName;
}
BOOL CDlgStatus::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return true;
	//return CDialog::OnEraseBkgnd(pDC);
}

int CDlgStatus::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_hFuncInst = LoadLibrary("User32.DLL"); 
	BOOL bRet=FALSE;
	if(m_hFuncInst)
	{
		UpdateLayeredWindow=(MYFUNC)GetProcAddress(m_hFuncInst, "UpdateLayeredWindow");
	}
	else
	{
		AfxMessageBox("User32.dll ERROR!");
		exit(0);
	}

	return 0;
}

BOOL CDlgStatus::UpdateClockDisplay(int Transparent)
{
	CDC* pWndDC = GetDC();
	if (NULL==pWndDC)
	{
		pWndDC->DeleteDC();
		pWndDC = NULL;
		return false;
	}
	DeleteDC(m_hdcMemory);
	m_hdcMemory.CreateCompatibleDC(pWndDC);
	HBITMAP hBitMap=::CreateCompatibleBitmap(pWndDC->m_hDC,m_BakWidth,m_BakHeight);
	SelectObject(m_hdcMemory,hBitMap);

	if(Transparent<0||Transparent>100)	Transparent=100;

	m_Blend.SourceConstantAlpha=int(Transparent*2.55);//1~255
	HDC hdcScreen=::GetDC (m_hWnd);
	RECT rct;
	GetWindowRect(&rct);
	POINT ptWinPos={rct.left,rct.top};

	m_ImageBack.DrawImage(&m_hdcMemory, 0, 0, m_BakWidth,m_BakHeight, m_iAction*m_BakWidth, 0, m_BakWidth,m_BakHeight);

	SIZE sizeWindow={m_BakWidth,m_BakHeight};
	POINT ptSrc={0,0};
	DWORD dwExStyle=GetWindowLong(m_hWnd,GWL_EXSTYLE);
	if((dwExStyle&0x80000)!=0x80000)
		SetWindowLong(m_hWnd,GWL_EXSTYLE,dwExStyle^0x80000);

	BOOL bRet=FALSE;
	bRet= UpdateLayeredWindow( m_hWnd,hdcScreen,&ptWinPos,&sizeWindow,m_hdcMemory.m_hDC,&ptSrc,0,&m_Blend,2);
	::ReleaseDC(m_hWnd,hdcScreen);
	hdcScreen=NULL;
	pWndDC->DeleteDC();
	pWndDC = NULL;
	DeleteObject(hBitMap);
	DeleteDC(m_hdcMemory);
	return bRet;
}