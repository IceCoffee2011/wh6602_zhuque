// SendMsgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SendMsgDlg.h"
#include ".\sendmsgdlg.h"
#include "GameFrameControl.h"
#include "GlobalOption.h"

//��ť������ɫ
#define		DEF_BT_COLOR		RGB(255, 0, 255)

//��Ϣ��Ӧ��ʱ��
#define		IDI_TIME_CHAT_MSG				1010
#define		TIME_CHAT_MSG_INTERVAL			1000


// CSendMsgDlg �Ի���

CSendMsgDlg::CSendMsgDlg(): CRgnDialog(IDD_INPUT_MSG_DLG)
{
	m_pGameFrameControl = NULL;
	m_dwLimitChatTimes = 0;
	m_bIsShowGameFrame = true;
	m_bIsShowWndTag = true;
	m_iCountMsgNums = 0;
	m_bIsStartTime=false;
}

CSendMsgDlg::~CSendMsgDlg()
{
}

void CSendMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UP_TAG, m_btWndTag);
	DDX_Control(pDX, IDC_INPUT_EDIT_MSG, m_ChatInput);
	DDX_Control(pDX, IDC_SEND_MSG, m_btSendChat);
	DDX_Control(pDX, IDC_EXP_BT, m_btExpression);
	DDX_Control(pDX, IDC_UP_TAG2, m_btWndTagLight);
}


BEGIN_MESSAGE_MAP(CSendMsgDlg, CRgnDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_HIT_EXPMESSTION,OnHitExpression)
//	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSendMsgDlg ��Ϣ�������

BOOL CSendMsgDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	if (pMsg->message==WM_KEYDOWN)
	{
		if (VK_RETURN == pMsg->wParam)
		{//�س���
			SetGameChatMsg();
			if (!m_bIsShowGameFrame)
			{
				m_iCountMsgNums = 0;
				SetGameChatShow();
			}
			
			return TRUE;
		}
		else if (VK_ESCAPE == pMsg->wParam)
		{
			return TRUE;
		}
	}
	return __super::PreTranslateMessage(pMsg);

	//return CRgnDialog::PreTranslateMessage(pMsg);
}

void CSendMsgDlg::OnPaint()
{
	CRgnDialog::OnPaint();

	if (!m_ImageBuffer.IsNull())
	{
		//��������
		CPaintDC dc(this);
		CDC * pDC=CDC::FromHandle(m_ImageBuffer.GetDC());

		int iWndWidth = m_ImageBuffer.GetWidth();
		int iWndHeight = m_ImageBuffer.GetHeight();

		pDC->SetBkMode(TRANSPARENT);
		pDC->FillSolidRect(0,0,iWndWidth,iWndHeight,RGB(0,0,0));

		//�滭����ͼƬ
		CImageHandle HandleCenter(&m_ImageWndBack);
		m_ImageWndBack.AlphaDrawImage(pDC, 0, 0, RGB(255, 0, 255));

		//�滭����
		m_ImageBuffer.BitBlt(dc,0,0,iWndWidth,iWndHeight,0,0);

		//������Դ
		m_ImageBuffer.ReleaseDC();
		ReleaseDC(pDC);
		pDC = NULL;
	}
	
}

//������ʾ
void CSendMsgDlg::SetGameChatShow()
{
	if (NULL != m_pGameFrameControl)
	{
		m_bIsStartTime = false;
		m_bIsShowGameFrame = !m_bIsShowGameFrame;
		m_pGameFrameControl->SetWndShow(m_bIsShowGameFrame);
		HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
		if (true == m_bIsShowGameFrame)
		{//����
			m_btWndTag.LoadRgnImage(hInstance, IDB_BT_DOWN, DEF_BT_COLOR, 4);
		}
		else
		{//����
			m_btWndTag.LoadRgnImage(hInstance, IDB_BT_UP, DEF_BT_COLOR, 4);
		}
		m_btWndTagLight.ShowWindow(SW_HIDE);
		m_btWndTag.ShowWindow(SW_SHOW);
		m_btWndTag.InvalidateRect(NULL);
	}
}

//������Ϣ
void CSendMsgDlg::SetGameChatMsg()
{
	m_iCountMsgNums = 0; 

	//������Ϣ
	CString strChatInfo="";
	m_ChatInput.GetWindowText(strChatInfo);
	if (!strChatInfo.IsEmpty())
	{
		////���ù���
		//g_GlobalOption.m_FilterString.SetFilterString(strChatInfo);
		//if (strChatInfo.IsEmpty()) return;

		//��ʱ����
		DWORD dwCurTimes = GetTickCount();
		if(m_dwLimitChatTimes==0 || dwCurTimes-m_dwLimitChatTimes>=LIMIT_CHAT_TIMES)
		{
			m_dwLimitChatTimes = dwCurTimes;

			//OSC,������������.
			m_ChatInput.SetWindowText(TEXT(""));

			if (NULL != m_pGameFrameControl)
			{
				m_pGameFrameControl->InsertToGameFrameMsg(strChatInfo.GetBuffer());
			}
		}
	}
	//���ý���
	m_ChatInput.SetFocus();
}

BOOL CSendMsgDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���

	switch (LOWORD(wParam))
	{
		case IDC_UP_TAG:		//��־��ť
		case IDC_UP_TAG2:
			{
				if (!m_bIsShowGameFrame && m_iCountMsgNums>0)
				{
					KillTimer(IDI_TIME_CHAT_MSG);
				}
				SetGameChatShow();
			}
			break;
		case IDC_SEND_MSG:		//���Ͱ�ť
			{
				//��ʾ���촰��
				if (!m_bIsShowGameFrame)
				{
					m_iCountMsgNums = 0;
					SetGameChatShow();
				}
				//������������
				SetGameChatMsg();
			}
			break;
		case IDC_EXP_BT:		//���鰴ť
			{
				//�������鴰��
				if (m_Expression.GetSafeHwnd()==NULL) m_Expression.CreateExpression(this);

				//�ƶ�����
				CRect rcButton;
				CSize ExpressSize;
				m_Expression.GetFixSize(ExpressSize);
				m_btExpression.GetWindowRect(&rcButton);
				m_Expression.MoveWindow(rcButton.right-ExpressSize.cx+2,rcButton.top-ExpressSize.cy,ExpressSize.cx,ExpressSize.cy);
				m_Expression.ShowWindow(SW_SHOW);
				m_Expression.SetFocus();
			}
			break;
		default:
			break;
	}

	return CRgnDialog::OnCommand(wParam, lParam);
}

//������Ϣ
LRESULT CSendMsgDlg::OnHitExpression(WPARAM wParam, LPARAM lParam)
{
	CExpressionItem * pExpressionItem=m_Expression.GetExpressItem((WORD)wParam);
	if (pExpressionItem!=NULL)
	{
		CString strBuffer;
		m_ChatInput.GetWindowText(strBuffer);
		strBuffer+=pExpressionItem->m_szTrancelate;
		m_ChatInput.SetWindowText(strBuffer);
		m_ChatInput.SetFocus();
		m_ChatInput.SetEditSel(strBuffer.GetLength(),strBuffer.GetLength());
	}

	return 0;
}


//���ô���ָ��
void CSendMsgDlg::SetGameFrameControlPtr(CGameFrameControl* pGameFrameControl)
{
	if (NULL != pGameFrameControl)
	{
		m_pGameFrameControl = pGameFrameControl;
	}
}

BOOL CSendMsgDlg::OnInitDialog()
{
	CRgnDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	//�������
	m_ChatInput.AddString(TEXT("�����ҵȵĻ���Ҳл��"));
	m_ChatInput.AddString(TEXT("��Һã��ܸ��˼�����λ��"));
	m_ChatInput.AddString(TEXT("��ô�ֶ����ˣ�����̫����..."));
	m_ChatInput.AddString(TEXT("��Ҫ�ߣ���ս��������"));
	m_ChatInput.AddString(TEXT("������������̫�����..."));
	m_ChatInput.AddString(TEXT("���ǽ������ѣ��ܸ�����������ϵ��ʽ��"));
	m_ChatInput.AddString(TEXT("����MM,����GG?"));
	m_ChatInput.AddString(TEXT("��㣬��������л�ˣ�"));
	m_ChatInput.AddString(TEXT("ʹ�죬ʹ�죬��ʮ��ţ�⣬����þƣ�"));
	m_ChatInput.AddString(TEXT("��Ҫ���ˣ���ʲô�ó���..."));
	m_ChatInput.AddString(TEXT("�ټ��ˣ��һ������ҵģ�BYE"));
	m_ChatInput.LimitText(MAX_CHAT_LEN-8);
	m_ChatInput.SetFocus();

	//������Դ
	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
	m_ImageWndBack.SetLoadInfo(IDB_IMAGE_DOWN_BACK, hInstance);
	m_btWndTag.LoadRgnImage(hInstance, IDB_BT_UP, DEF_BT_COLOR, 4);
	m_btSendChat.LoadRgnImage(hInstance, IDB_BT_SENDCHAT, DEF_BT_COLOR);
	m_btExpression.LoadRgnImage(hInstance, IDB_BT_EXPRESSION, DEF_BT_COLOR);
	m_btWndTagLight.LoadRgnImage(hInstance, IDB_BT_LIGHT_UP, DEF_BT_COLOR, 4);

	m_ImageBuffer.Destroy();
	CImageHandle HandleCenter(&m_ImageWndBack);
	m_ImageBuffer.Create(m_ImageWndBack.GetWidth(), m_ImageWndBack.GetHeight(), 24);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}



//����Ҽ�������Ϣ
void CSendMsgDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
}

//BOOL CSendMsgDlg::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//	return TRUE;
//	//return CRgnDialog::OnEraseBkgnd(pDC);
//}


//������ʾ��־
void CSendMsgDlg::SetGameFrameShowTag(bool bIsShow)
{
	m_bIsShowGameFrame = bIsShow;
	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
	if (true == m_bIsShowGameFrame)
	{//����
		m_btWndTag.LoadRgnImage(hInstance, IDB_BT_DOWN, DEF_BT_COLOR, 4);
	}
	else
	{//����
		m_btWndTag.LoadRgnImage(hInstance, IDB_BT_UP, DEF_BT_COLOR, 4);
	}
	m_btWndTagLight.ShowWindow(SW_HIDE);
	m_btWndTag.ShowWindow(SW_SHOW);
	m_btWndTag.InvalidateRect(NULL);
}


//��Ӧ��Ϣ
void CSendMsgDlg::SetMsgBeep()
{
	++m_iCountMsgNums;
	if (true == m_bIsShowGameFrame)
	{//��ʾ�Ͳ���Ӧ��.
		m_iCountMsgNums = 0;
	}
	else
	{//û����ʾ����
		if ((false==m_bIsStartTime)&&(1 == m_iCountMsgNums))
		{
			m_bIsStartTime = true;
			++m_iCountMsgNums;
			//������Ӧ��ʾ
			SetTimer(IDI_TIME_CHAT_MSG, TIME_CHAT_MSG_INTERVAL, NULL);
		}
	}
}

void CSendMsgDlg::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (IDI_TIME_CHAT_MSG == nIDEvent)
	{
		m_bIsStartTime=true;
		if (!m_bIsShowGameFrame && m_iCountMsgNums>0)
		{//��Ӧ��ť
			m_bIsShowWndTag = !m_bIsShowWndTag;
			HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
			if (true == m_bIsShowWndTag)
			{//����
				m_btWndTag.ShowWindow(SW_HIDE);
				m_btWndTagLight.ShowWindow(SW_SHOW);
			}
			else
			{
				m_btWndTagLight.ShowWindow(SW_HIDE);
				m_btWndTag.ShowWindow(SW_SHOW);
			}
		}
	}

	CRgnDialog::OnTimer(nIDEvent);
}
