// NewGiftFlashDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NewGiftFlashDlg.h"
#include ".\newgiftflashdlg.h"


#define ID_TIME_FIREWORKS			100		// �̻���ʱ��
#define	ID_TIME_PAUSE				101		// �̻����
#define	ID_TIME_CLOSE				102		// �رմ���

#define ID_UPGRADE_ACTION			103		//�������ͱ�ʯ����ID

#define TIME_UPGRADE_ACTION			100		//�������ͱ�ʯ�������ʱ��

#define	ANIMATION_FRAME_COUNT		10		// �̻�֡��Ŀ
#define ANIMATION_UPGRADE_COUNT		8		// ���ͱ�ʯ֡��

// CNewGiftFlashDlg �Ի���

IMPLEMENT_DYNAMIC(CNewGiftFlashDlg, CDialog)
CNewGiftFlashDlg::CNewGiftFlashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewGiftFlashDlg::IDD, pParent)
{
	ZeroMemory(&m_tagNewGift,sizeof(m_tagNewGift));			//�������
	m_iFrameCount = 0;
	m_iUpgradeCount=0;
	m_bShowVal = FALSE;
}

CNewGiftFlashDlg::~CNewGiftFlashDlg()
{
	m_ImgBK.DestroyImage();
	m_imgFireworks.DestroyImage();
	m_imgUpgrade.DestroyImage();
	m_bShowVal = FALSE;
}

void CNewGiftFlashDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if (!m_ImgBK.IsNull())
	{
		m_ImgBK.DestroyImage();
	}

	if (!m_imgFireworks.IsNull())
	{
		m_imgFireworks.DestroyImage();
	}

	if (!m_imgUpgrade.IsNull())
	{
		m_imgUpgrade.DestroyImage();
	}
	m_bShowVal = FALSE;
}


void CNewGiftFlashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNewGiftFlashDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CNewGiftFlashDlg ��Ϣ�������

BOOL CNewGiftFlashDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ZeroMemory(&m_tagNewGift, sizeof(m_tagNewGift));
	m_iUpgradeCount=0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CNewGiftFlashDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CDC memdc;
	CBitmap bmp;
	CRect rect;
	GetClientRect(&rect);
	bmp.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	memdc.CreateCompatibleDC(&dc);
	memdc.SelectObject(bmp);

	if (!m_ImgBK.IsNull())
	{
		m_ImgBK.DrawImage(&memdc,0,0);
	}

	if (!m_imgFireworks.IsNull() && ANIMATION_FRAME_COUNT>m_iFrameCount && 0<=m_iFrameCount)
	{
		m_imgFireworks.DrawImage(&memdc, 0, 0, 
			m_imgFireworks.GetWidth()/ANIMATION_FRAME_COUNT, m_imgFireworks.GetHeight(), 
			m_iFrameCount*m_imgFireworks.GetWidth()/ANIMATION_FRAME_COUNT, 0, 
			m_imgFireworks.GetWidth()/ANIMATION_FRAME_COUNT, m_imgFireworks.GetHeight());
	}

	if (!m_imgUpgrade.IsNull())
	{
		m_imgUpgrade.DrawImage(&memdc, 0, 0, 
			m_imgUpgrade.GetWidth()/ANIMATION_UPGRADE_COUNT, m_imgUpgrade.GetHeight(), 
			m_iUpgradeCount*m_imgUpgrade.GetWidth()/ANIMATION_UPGRADE_COUNT, 0, 
			m_imgUpgrade.GetWidth()/ANIMATION_UPGRADE_COUNT, m_imgUpgrade.GetHeight());
	}

	if (m_bShowVal)
	{
		m_imgnumVal.DrawImageNums(&memdc);
	}

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memdc, 0, 0, SRCCOPY);
	bmp.DeleteObject();
	memdc.DeleteDC();
}

//�������	Author<cxf>; DateTime<2012-01-16>
void CNewGiftFlashDlg::InitDataRes(CMD_GF_NEW_GIFT * pNewGift)
{
	ZeroMemory(&m_tagNewGift,sizeof(m_tagNewGift));			//�������
	m_iFrameCount = 0;
	m_iUpgradeCount=0;
	m_bShowVal = FALSE;

	if (NULL != pNewGift)
	{
		CPoint ptBK;
		//��ǰ·��
		CString strPath("");
		GetImagePlazaPath(strPath);
		memcpy(&m_tagNewGift, pNewGift, sizeof(CMD_GF_NEW_GIFT));
		switch (m_tagNewGift.wGiftType)
		{
		case 1:
			{//���ź�
				m_ImgBK.LoadImage(TEXT(strPath+"BKFirstGift.png"));
				break;
			}
		case 2:
			{//�������� 
				m_ImgBK.LoadImage(TEXT(strPath+"BKOnline.png"));
				break;
			}
		case 3:
			{//�����ͱ�ʯ
				m_ImgBK.LoadImage(TEXT(strPath+"BKUpgrade.png"));
				m_imgUpgrade.LoadImage(TEXT(strPath+"UpgradeAction.png"));
				SetTimer(ID_UPGRADE_ACTION, TIME_UPGRADE_ACTION, NULL);
				break;
			}
		}
		ptBK.x=m_ImgBK.GetWidth();
		ptBK.y=m_ImgBK.GetHeight();

		m_imgFireworks.LoadImage(strPath+"FireworksAction.png");

		SetWindowPos(&wndTop, 0, 0, ptBK.x, ptBK.y, SWP_NOMOVE);
				
		CRgn wndRgn;
		wndRgn.CreateRoundRectRgn(0, 0, ptBK.x, ptBK.y, 20, 20);
		SetWindowRgn((HRGN)wndRgn.m_hObject, FALSE);
		wndRgn.DeleteObject();
		CenterWindow(GetParent());
		ShowWindow(SW_SHOW);
		m_iFrameCount = 0;

		SetTimer(ID_TIME_FIREWORKS, 100, NULL);
		SetTimer(ID_TIME_CLOSE, 4000, NULL);
	}
}

void CNewGiftFlashDlg::SetNum(int iVal)
{
	m_bShowVal = TRUE;

	//��ǰ·��
	CString strPath("");
	GetImagePlazaPath(strPath);
	m_imgnumVal.SetImageResource(TEXT(strPath+"OnlineHaveGift.png"), 10);
	if (3==m_tagNewGift.wGiftType)
	{//�����ͱ�ʯ
		m_imgnumVal.SetShowRect(CRect(130, 167, 130+150, 167+40));
	}
	else
	{
		m_imgnumVal.SetShowRect(CRect(125, 70, 125+150, 70+40));
	}
	
	m_imgnumVal.SetNums((long)iVal);	
	m_imgnumVal.SetNumAlign(IA_CENTER);
}

//��ǰ·��
void CNewGiftFlashDlg::GetCurrentDirectoryPath(CString &szPath)
{
	szPath="";
	TCHAR szPathTemp[MAX_PATH]={0};
	GetCurrentDirectory(MAX_PATH, szPathTemp);
	szPath.Append(szPathTemp);
}

//��Դ·��
void CNewGiftFlashDlg::GetImagePlazaPath(CString& strRetValue)
{
	CString strModuleFileName("");
	GetCurrentDirectoryPath(strModuleFileName);
	strModuleFileName.Append("\\data\\gameframe\\plaza\\");
	strRetValue = strModuleFileName;
}

BOOL CNewGiftFlashDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return FALSE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CNewGiftFlashDlg::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (ID_TIME_FIREWORKS == nIDEvent)
	{
		int iActionCount=(int)ANIMATION_FRAME_COUNT;
		if (iActionCount-1 <= m_iFrameCount++)
		{
			KillTimer(ID_TIME_FIREWORKS);
			m_iFrameCount = -1;
			srand((unsigned int)time(NULL));
			SetTimer(ID_TIME_PAUSE, rand()%3*1000, NULL);
		}

		InvalidateRect(NULL);
		return;
	}
	else if (ID_TIME_PAUSE == nIDEvent)
	{
		KillTimer(ID_TIME_PAUSE);
		m_iFrameCount = 0;
		SetTimer(ID_TIME_FIREWORKS, 100, NULL);
		InvalidateRect(NULL);
		return;
	}
	else if (ID_TIME_CLOSE == nIDEvent)
	{
		KillTimer(ID_TIME_FIREWORKS);
		KillTimer(ID_TIME_PAUSE);
		KillTimer(ID_TIME_CLOSE);

		PostMessage(WM_CLOSE);
		return;
	}
	else if (ID_UPGRADE_ACTION == nIDEvent)
	{
		int iActionCount=(int)ANIMATION_UPGRADE_COUNT;
		if (iActionCount-1 <= m_iUpgradeCount++)
		{
			KillTimer(ID_UPGRADE_ACTION);
			m_iUpgradeCount=iActionCount-1;
		}
		InvalidateRect(NULL);
		return;
	}

	CDialog::OnTimer(nIDEvent);
}

void CNewGiftFlashDlg::OnClose()
{
	if (!m_ImgBK.IsNull())
	{
		m_ImgBK.DestroyImage();
	}

	if (!m_imgFireworks.IsNull())
	{
		m_imgFireworks.DestroyImage();
	}

	if (!m_imgUpgrade.IsNull())
	{
		m_imgUpgrade.DestroyImage();
	}
	m_bShowVal = FALSE;

	CDialog::OnClose();
}

void CNewGiftFlashDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (1!=m_tagNewGift.wGiftType) return; //���ź�

	ShellExecute(NULL,   "open",   "http://www.cbwgame.com/shop.php",     ""   ,NULL,   SW_SHOW);

	CDialog::OnLButtonDown(nFlags, point);
}
