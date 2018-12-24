// ShowGameMsg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ShowGameMsg.h"
#include ".\showgamemsg.h"
#include "GlobalUnits.h"
#include "webpath.h"

const int iDrawMsgX	= 28;
const int iDrawMsgY = 70;
const int iDrawMsgW = 363;
const int iDrawMsgH = 380;
const int iDrawContexW = 230;	//����һ�����ݵĿ��
const int iDrawMsgInterval	=	30;	//���и߶�
const int iDrawMulMsgInterval = 30;	//���л��и߶�
const int iDrawPZH	=	6;	//ͼƬ���ı��߶�
const int iTotalNums = 15;


// CShowGameMsg �Ի���

IMPLEMENT_DYNAMIC(CShowGameMsg, CDialog)
CShowGameMsg::CShowGameMsg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowGameMsg::IDD, pParent)
{
	m_memdc.m_hDC = NULL;
	m_vecMsgList.clear();
	m_font.CreateFont(12,0,0,700,200,0,0,0,134,3,2,1,2,TEXT("����"));

	HINSTANCE hInstance = AfxGetInstanceHandle();
	//m_BTLookOK.SetButtonImage(IDB_LOOK_GAME_MSG_OK, hInstance, false, 4);
	m_BTLookMore.SetButtonImage(IDB_LOOK_GAME_MSG_MORE, hInstance, false, 4);

	m_ImageBack.LoadImage(hInstance, TEXT("PNG_LOOK_GAME_MSG_BACK"));
	m_ImageICO[0].LoadImage(hInstance, TEXT("PNG_GAME_MSG_ICO_1"));
	m_ImageICO[1].LoadImage(hInstance, TEXT("PNG_GAME_MSG_ICO_2"));
	m_ImageICO[2].LoadImage(hInstance, TEXT("PNG_GAME_MSG_ICO_3"));
	m_ImageICO[3].LoadImage(hInstance, TEXT("PNG_GAME_MSG_ICO_4"));
	m_ImageICO[4].LoadImage(hInstance, TEXT("PNG_GAME_MSG_ICO_5"));
	m_ImageICO[5].LoadImage(hInstance, TEXT("PNG_GAME_MSG_ICO_6"));
}

CShowGameMsg::~CShowGameMsg()
{
	m_memdc.DeleteDC();
}

void CShowGameMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_EXIT, m_BTLookOK);
	DDX_Control(pDX, IDC_BT_MORE, m_BTLookMore);
}


BEGIN_MESSAGE_MAP(CShowGameMsg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BT_EXIT, OnBnClickedBtExit)
	ON_BN_CLICKED(IDC_BT_MORE, OnBnClickedBtMore)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CShowGameMsg ��Ϣ�������

BOOL CShowGameMsg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect rect;
	GetParent()->GetWindowRect(&rect);
	SetWindowPos(NULL, rect.left+395, rect.top+110, 416, 517, 0);
	m_BTLookOK.SetWindowPos(NULL, 170, 468, 68, 33, 0);
	m_BTLookMore.SetWindowPos(NULL, 315, 476, 75, 26, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CShowGameMsg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	if (NULL != m_memdc.m_hDC)
	{
		CRect rect;
		GetClientRect(&rect);
		m_memdc.SetBkMode(TRANSPARENT);
		m_memdc.FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(0, 0, 0));
		m_memdc.SetTextColor(RGB(0, 0, 0));

		if (!m_ImageBack.IsNull())
		{
			m_ImageBack.DrawImage(&m_memdc, 0, 0);
		}

		DWORD dwX=iDrawMsgX, dwY=iDrawMsgY;
		for (int i=0; i<m_vecMsgList.size()&&i<iTotalNums; i++)
		{
			//����ͼ��
			if (m_vecMsgList[i].byMsgType>=1 && m_vecMsgList[i].byMsgType<=6)
			{
				m_ImageICO[m_vecMsgList[i].byMsgType-1].DrawImage(&m_memdc, dwX, dwY);
			}
			//����ʱ��
			m_memdc.TextOut(dwX+24, dwY+iDrawPZH, m_vecMsgList[i].strTime.c_str());
			//����"|"�ָ���
			m_memdc.TextOut(dwX+120, dwY+iDrawPZH, "|");
			//������Ϣ����
			if (m_memdc.GetTextExtent(m_vecMsgList[i].strContex.c_str()).cx > iDrawContexW)
			{//����
				//��������
				CRect rect;
				rect.left = dwX+128;
				rect.top = dwY+iDrawPZH;
				rect.right = rect.left + iDrawContexW;
				rect.bottom = rect.top + iDrawMulMsgInterval;
				m_memdc.DrawText(m_vecMsgList[i].strContex.c_str(), &rect, DT_LEFT|DT_WORDBREAK);
			}
			else
			{
				//��������
				m_memdc.TextOut(dwX+128, dwY+iDrawPZH, m_vecMsgList[i].strContex.c_str());
			}
			dwY += iDrawMsgInterval;
			if (dwY>=iDrawMsgY+iDrawMsgH)
			{
				break;
			}
		}

		dc.BitBlt(0, 0, rect.Width(), rect.Height(), &m_memdc, 0, 0, SRCCOPY);
	}
}

//������Ϣ
bool CShowGameMsg::InsertGameMsg(const char* pMsgType, const char* pTimeMsg, const char* pContexMsg)
{
	if (NULL == pTimeMsg || NULL == pContexMsg || m_vecMsgList.size()>=20)
	{
		return false;
	}
	
	BYTE byMsgType = GetMsgType(pMsgType);
	if (byMsgType>=1 && byMsgType<=6)
	{
		GameMsg strMsgInfo;
		strMsgInfo.byMsgType = byMsgType;
		strMsgInfo.strTime.append(pTimeMsg);
		strMsgInfo.strContex.append(pContexMsg);
		m_vecMsgList.push_back(strMsgInfo);
		
		return true;
	}
	
	return false;
}

//������Ϣ����
BYTE CShowGameMsg::GetMsgType(const char* pMsgType)
{
	if (0 == ::strcmp(pMsgType, "��ֵ��Ϣ"))
	{
		return 1;
	}
	else if (0 == ::strcmp(pMsgType, "������Ϣ"))
	{
		return 2;
	}
	else if (0 == ::strcmp(pMsgType, "������Ϣ"))
	{
		return 3;
	}
	else if (0 == ::strcmp(pMsgType, "������Ϣ"))
	{
		return 4;
	}
	else if (0 == ::strcmp(pMsgType, "�ʺŰ�ȫ"))
	{
		return 5;
	}
	else if (0 == ::strcmp(pMsgType, "������Ϣ"))
	{
		return 6;
	}
	return -1;
}

//���������Ϣ
void CShowGameMsg::ClearAllMsg()
{
	m_vecMsgList.clear();
}

BOOL CShowGameMsg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CShowGameMsg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CPaintDC dc(this);
	m_memdc.DeleteDC();
	CBitmap bmp;
	m_memdc.CreateCompatibleDC(&dc);
	bmp.CreateCompatibleBitmap(&dc, cx, cy);
	m_memdc.SelectObject(bmp);
	m_memdc.SelectObject(m_font);
	InvalidateRect(NULL);
	bmp.DeleteObject();
	// TODO: �ڴ˴������Ϣ����������
}

void CShowGameMsg::OnBnClickedBtExit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CShowGameMsg::OnBnClickedBtMore()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/*cxf 1202 tagGlobalUserData & UserData=g_GlobalUnits.GetGolbalUserData();
	CString strUrl("");
	strUrl.Format(WEB_LOOK_MY_MSG, UserData.dwUserID, UserData.szPassWord);
	ShellExecute(NULL,   "open",   strUrl,     ""   ,NULL,   SW_SHOW);*/
}

void CShowGameMsg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	CDialog::OnLButtonDown(nFlags, point);
}
