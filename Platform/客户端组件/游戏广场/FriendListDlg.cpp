// FriendListDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FriendListDlg.h"
#include ".\friendlistdlg.h"


// CFriendListDlg �Ի���

IMPLEMENT_DYNAMIC(CFriendListDlg, CSkinDialog)
CFriendListDlg::CFriendListDlg(CWnd* pParent /*=NULL*/)
	: CSkinDialog(CFriendListDlg::IDD, pParent)
{
	m_type=LT_MY_FRIEND;
	m_list=NULL;
}

CFriendListDlg::~CFriendListDlg()
{
}

void CFriendListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_FRIEND, m_btnFriend);
	DDX_Control(pDX, IDC_BTN_STRANGER, m_btnStranger);
	DDX_Control(pDX, IDC_BTN_BLACKLIST, m_btnBlackList);
	DDX_Control(pDX, IDC_BTN_TEAMES, m_btnTeames);
	DDX_Control(pDX, IDC_LIST_FRIEND, m_listFriend);
	DDX_Control(pDX, IDC_LIST_STRANGER, m_listStranger);
	DDX_Control(pDX, IDC_LIST_BLACKLIST, m_listBlackList);
	DDX_Control(pDX, IDC_LIST_TEAMES, m_listTeames);
	DDX_Control(pDX, IDC_NAME, m_editInput);
	DDX_Control(pDX, IDC_BTN_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_BTN_DEL, m_btnDel);
	DDX_Control(pDX, IDC_ALLOW_ADD_MY, m_btnAllowAddMy);
	DDX_Control(pDX, IDC_AUTOMATISM_ADD_MY, m_btnAutoMatismAddMy);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CFriendListDlg, CSkinDialog)
	ON_BN_CLICKED(IDC_BTN_FRIEND, OnBnClickedBtnFriend)
	ON_BN_CLICKED(IDC_BTN_STRANGER, OnBnClickedBtnStranger)
	ON_BN_CLICKED(IDC_BTN_TEAMES, OnBnClickedBtnTeames)
	ON_BN_CLICKED(IDC_BTN_BLACKLIST, OnBnClickedBtnBlacklist)
	ON_WM_PAINT()
	ON_EN_SETFOCUS(IDC_NAME, OnEnSetfocusName)
END_MESSAGE_MAP()


// CFriendListDlg ��Ϣ�������

BOOL CFriendListDlg::OnInitDialog()
{
	CSkinDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_imgback.SetLoadInfo(IDB_FRIEND_BACK, AfxGetInstanceHandle(), true);

	m_listFriend.InitUserListView();
	m_listStranger.InitUserListView();
	m_listBlackList.InitUserListView();
	m_listTeames.InitUserListView();

	HDWP hDwp=BeginDeferWindowPos(32);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	DeferWindowPos(hDwp,m_btnFriend,NULL,11,42,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnStranger,NULL,71,42,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnBlackList,NULL,131,42,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnTeames,NULL,191,42,0,0,uFlags);
	DeferWindowPos(hDwp,m_listFriend,NULL,11,70,254,305,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS);
	DeferWindowPos(hDwp,m_listStranger,NULL,11,70,254,305,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS);
	DeferWindowPos(hDwp,m_listBlackList,NULL,11,70,254,305,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS);
	DeferWindowPos(hDwp,m_listTeames,NULL,11,70,254,305,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS);
	DeferWindowPos(hDwp,m_editInput,NULL,14,380,129,17,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS);
	DeferWindowPos(hDwp,m_btnAdd,NULL,150,377,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnDel,NULL,209,377,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnAllowAddMy,NULL,15,401,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnAutoMatismAddMy,NULL,15,416,0,0,uFlags);
	DeferWindowPos(hDwp,m_btnCancel,NULL,241,409,0,0,uFlags);
	EndDeferWindowPos(hDwp);
	
	OnBnClickedBtnFriend();
	if (!m_imgback.IsNull())
	{
		SetWindowPos(0,0,0,m_imgback.GetWidth(),m_imgback.GetHeight(),SWP_NOMOVE);
	}else
	{
		SetWindowPos(0,0,0,275,437,SWP_NOMOVE);
	}

	CRect rect;
	GetWindowRect(rect);
	SetNCRect(rect.Height());
	CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CFriendListDlg::UpdateListTab()
{
	::ShowWindow(m_listFriend.GetSafeHwnd(), m_type==LT_MY_FRIEND?SW_SHOW:SW_HIDE);
	::ShowWindow(m_listStranger.GetSafeHwnd(), m_type==LT_STRANGER?SW_SHOW:SW_HIDE);
	::ShowWindow(m_listBlackList.GetSafeHwnd(), m_type==LT_BLACK_LIST?SW_SHOW:SW_HIDE);
	::ShowWindow(m_listTeames.GetSafeHwnd(), m_type==LT_TEAM?SW_SHOW:SW_HIDE);
}
void CFriendListDlg::OnBnClickedBtnFriend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_list = &m_listFriend;
	m_type=LT_MY_FRIEND;
	UpdateListTab();
}

void CFriendListDlg::OnBnClickedBtnStranger()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_list = &m_listStranger;
	m_type=LT_STRANGER;
	UpdateListTab();

}

void CFriendListDlg::OnBnClickedBtnTeames()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_list = &m_listTeames;
	m_type=LT_TEAM;
	UpdateListTab();
}

void CFriendListDlg::OnBnClickedBtnBlacklist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_list = &m_listBlackList;
	m_type=LT_BLACK_LIST;
	UpdateListTab();
}

void CFriendListDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CSkinDialog::OnPaint()
	if (!m_imgback.IsNull())
	{
		m_imgback.Draw(dc.m_hDC, 0, 0, m_imgback.GetWidth(), m_imgback.GetHeight(), 0, 0, m_imgback.GetWidth(), m_imgback.GetHeight() );
	}

}

void CFriendListDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CSkinDialog::OnCancel();
	ShowWindow(SW_HIDE);
}

void CFriendListDlg::OnEnSetfocusName()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

BOOL CFriendListDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CSkinDialog::OnCommand(wParam, lParam);
}

void CFriendListDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	//AfxMessageBox(TEXT("CFriendListDlg::OnOk"));
	CSkinDialog::OnOK();
}
