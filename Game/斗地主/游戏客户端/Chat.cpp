// Chat.cpp : ʵ���ļ�
//

#include "stdafx.h"
//#include "��񶷵�����Ϸ.h"
#include "Chat.h"
#include ".\chat.h"


// CChat �Ի���

IMPLEMENT_DYNAMIC(CChat, CDialog)
CChat::CChat(CWnd* pParent /*=NULL*/)
	: CDialog(CChat::IDD, pParent)
{
}

CChat::~CChat()
{
}

void CChat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ChatInput);
}


BEGIN_MESSAGE_MAP(CChat, CDialog)
END_MESSAGE_MAP()


// CChat ��Ϣ�������

BOOL CChat::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ChatInput.LimitText(MAX_CHAT_LEN-1);

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

	return TRUE;
}

void CChat::SetPos(int cx,int cy)
{
	CRect rect;
	GetWindowRect(rect);
	MoveWindow(cx,cy,rect.Width(),rect.Height(),true);
}

BOOL CChat::PreTranslateMessage(MSG* pMsg)
{
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_RETURN))
	{
		//if(m_GameClientView.m_GameChat.Is)
		//m_GameClientView.m_GameChat.ShowWindow(SW_SHOW);
		//OnChat(1, 1);
		//ShowWindow(SW_SHOW);
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CChat::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialog::OnOK();
}
