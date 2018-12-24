// GameFrameBorder.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GameFrameBorder.h"
#include ".\gameframeborder.h"
#include "MemDC.h"

// CGameFrameBorder �Ի���

IMPLEMENT_DYNAMIC(CGameFrameBorder, CDialog)
CGameFrameBorder::CGameFrameBorder(CWnd* pParent /*=NULL*/)
	: CDialog(CGameFrameBorder::IDD, pParent)
{
}

CGameFrameBorder::~CGameFrameBorder()
{
	m_ImageBack.DestroyImage();
}

void CGameFrameBorder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGameFrameBorder, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CGameFrameBorder ��Ϣ�������

BOOL CGameFrameBorder::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	TCHAR szResourcePath[MAX_PATH];	GetCurrentDirectory(MAX_PATH,szResourcePath);
	CString strPath(szResourcePath);
	strPath.Append("\\data\\gameframe\\plaza\\");

	//����
	m_ImageBack.LoadImage(TEXT(strPath + "GameFrameBorder.png"));

	SetImageRgn(m_hWnd, &m_ImageBack, SIRT_SOLID_DOT_LESS);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CGameFrameBorder::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

	CRect rect;
	GetClientRect(&rect);
	int iWidth = rect.Width();
	int iHeight = rect.Height();

	GetClientRect(&rect);
	CMemDC dcMem(&dc, rect, true);	// �õ�ɫ��䱳��

	dcMem.SetBkMode(TRANSPARENT);

	if (!m_ImageBack.IsNull())
	{
		m_ImageBack.DrawImage(&dcMem, 0, 0);
	}
}
