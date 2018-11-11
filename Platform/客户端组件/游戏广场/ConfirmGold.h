#pragma once

#include "resource.h"

// CConfirmGold �Ի���

class CConfirmGold : public CDialog
{
	DECLARE_DYNAMIC(CConfirmGold)

public:
	CConfirmGold(CWnd* pParent = NULL);							// ��׼���캯��
	virtual ~CConfirmGold();

	// �Ի�������
	enum { IDD = IDD_CONFIRM_GOLD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);			// DDX/DDV ֧��
	virtual BOOL OnInitDialog();								// ��ʼ��

public:
	afx_msg void OnPaint();										// �滭
	afx_msg void OnBnClickedGetGold();							// ��ȡ��ʯ��ť
	afx_msg void OnBnClickedConfirm();							// ȷ����ť

public:
	void SetMode(WORD wMode);									//����ģ��

	DECLARE_MESSAGE_MAP()

private:
	//��ȡ��ǰ·��
	void GetCurrentDirectoryPath(CString &szPath);
	//����·��
	void GetImagePlazaPath(CString& strRetValue);

private:
	CRgnButton						m_btnGetGold;				// ��ȡ��Ұ�ť
	CRgnButton						m_btnConfirm;				// ȷ����ť
	CRgnButton						m_btnGetGoldAll;			// ��ȡ����
	CPngImage						m_ImgBK;					// ����
	WORD							m_wMode;					// ��ʽ

public:
	afx_msg void OnDestroy();

	afx_msg void OnBnClickedConfirmGoldGetall();
};
