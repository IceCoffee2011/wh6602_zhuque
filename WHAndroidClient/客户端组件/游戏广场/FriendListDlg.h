#pragma once

#include "StdAfx.h"
#include "Resource.h"
#include "Afxcmn.h"
#include "afxwin.h"
#include "UserListView.h"
// CFriendListDlg �Ի���



class CFriendListDlg : public CSkinDialog
{
	DECLARE_DYNAMIC(CFriendListDlg)

public:
	CFriendListDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFriendListDlg();

// �Ի�������
	enum { IDD = IDD_FRIEND_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ����
	CButton m_btnFriend;
	// İ����
	CButton m_btnStranger;
	// ������
	CButton m_btnBlackList;
	// ����
	CButton m_btnTeames;
	// �����б�
	CUserListView m_listFriend;
	// İ�����б�
	CUserListView m_listStranger;
	// �������б�
	CUserListView m_listBlackList;
	// �����б�
	CUserListView m_listTeames;
	// ����
	CEdit m_editInput;
	// ���
	CButton m_btnAdd;
	// ɾ��
	CButton m_btnDel;
	// ����������Ҽ���Ϊ����
	CButton m_btnAllowAddMy;
	// �������Զ����
	CButton m_btnAutoMatismAddMy;
	// �ر�
	CButton m_btnCancel;

	CUserListView * m_list;


	CSkinImage m_imgback;

	enum LIST_TYPE{
		LT_MY_FRIEND=0,
		LT_STRANGER,
		LT_BLACK_LIST,
		LT_TEAM
	};
	LIST_TYPE m_type;
protected:
	//�����б�ҳ��
	void UpdateListTab();
public:
	virtual BOOL OnInitDialog();
	//����
	afx_msg void OnBnClickedBtnFriend();
	//İ����
	afx_msg void OnBnClickedBtnStranger();
	//������
	afx_msg void OnBnClickedBtnTeames();
	//����
	afx_msg void OnBnClickedBtnBlacklist();
	afx_msg void OnPaint();
protected:
	virtual void OnCancel();
public:
	afx_msg void OnEnSetfocusName();
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnOK();
};
