#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "afxwin.h"

// CChat �Ի���

class CChat : public CDialog
{
	DECLARE_DYNAMIC(CChat)

public:
	CChat(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChat();


// �Ի�������
	enum { IDD = IDD_CHAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
public:
	CComboBox m_ChatInput;
public:
	//��ʼ��
	virtual BOOL OnInitDialog();
	//����λ��
	void SetPos(int cx,int cy);

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void OnOK();
};
