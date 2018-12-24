#pragma once

#include "stdafx.h"
#include "resource.h"
#include "afxwin.h"
#define KEY_NUM   50
// CKeyDialog �Ի���

class CKeyDialog : public CSkinDialogEx
{
	DECLARE_DYNAMIC(CKeyDialog)

public:
	CKeyDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CKeyDialog();

// �Ի�������
	enum { IDD = IDD_KEYDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


public:
	CRgnButton    m_KeyBtn[KEY_NUM];			//��
	BOOL       m_bShift;					//Shift 
	BOOL       m_bCapsLoc;					//Caps Lock
	int		   m_nRandKey[26];
	HWND		m_hParent;
	CSkinImage  m_backImage;
	//�������
	void	   RandKey(int key[], int count);
	//���°�ťƤ��
	void	   UpdateKeySkin();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickKey();

public:
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	CRgnButton m_btnClose;
};
