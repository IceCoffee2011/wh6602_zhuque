//------------------------------------------------------------------------------
// �ļ����ƣ�RMessageBox.h
// �ļ��汾��v1.0
// �������ڣ�2006-05-06 04:25
// ��    �ߣ�Richard Ree
// �ļ��������Զ�����Ϣ����
//------------------------------------------------------------------------------

#pragma once

#ifndef RMESSAGEBOX_H
#define RMESSAGEBOX_H

#include "RDialog.h"
#include "RButton.h"


#define MB_CLOSERETRYCANCEL	0x00000007L		// ǿ�ƹرգ����ԣ�ȡ��
// CRMessageBox �Ի���

class CRMessageBox : public CRDialog
{
	DECLARE_DYNAMIC(CRMessageBox)

public:
	CRMessageBox(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRMessageBox();

// �Ի�������
	enum { IDD = IDD_MESSAGEBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedAbort();
	afx_msg void OnBnClickedIganore();
	afx_msg void OnBnClickedRetry();
	afx_msg void OnBnClickedContinue();
	afx_msg void OnBnClickedYes();
	afx_msg void OnBnClickedNo();
	afx_msg void OnBnClickedClose();
	void ResetMessageBox();

public:
	// ��Ϣ�����
	CString m_sTitle;
	// ��Ϣ����ʽ
	int m_iType;
	// �û����еİ�ť
	int m_iID;
	// ��ʾ��Ϣ
	CString m_sPromptMessage;
	// ��ѡ��1��ʾ��Ϣ�������ַ�����Ϊ��ʱ����ʾ��ѡ�򣬷������ظ�ѡ��
	CString m_sOptionPromptMessage1;
	// ��ѡ��1ѡ��״̬��ֵ
	BOOL m_bOption1;
	// ��ѡ��2��ʾ��Ϣ�������ַ�����Ϊ��ʱ����ʾ��ѡ�򣬷������ظ�ѡ��
	CString m_sOptionPromptMessage2;
	// ��ѡ��2ѡ��״̬��ֵ
	BOOL m_bOption2;

protected:
	CStatic m_stcIcon;
	CStatic m_stcPromptMessage;
	CButton m_chkOption1;
	CButton m_chkOption2;
	CRButton m_btnOK;
	CRButton m_btnCancel;
	CRButton m_btnAbort;
	CRButton m_btnIganore;
	CRButton m_btnRetry;
	CRButton m_btnContinue;
	CRButton m_btnYes;
	CRButton m_btnNo;
	CRButton m_btClose;
};

#endif // #ifndef RMESSAGEBOX_H
