#pragma once

#include "Resource.h"

// CGameFrameBorder �Ի���

class CGameFrameBorder : public CDialog
{
	DECLARE_DYNAMIC(CGameFrameBorder)

public:
	CGameFrameBorder(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGameFrameBorder();

// �Ի�������
	enum { IDD = IDD_DIALOG8 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CPngImage		m_ImageBack;		//����

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
