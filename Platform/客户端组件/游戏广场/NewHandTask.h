#ifndef _NEW_HAND_TASK_H_2012_2_16_
#define  _NEW_HAND_TASK_H_2012_2_16_

#pragma once

// ��������

class CNewHandTask : public CRgnDialog
{
	DECLARE_DYNAMIC(CNewHandTask)

public:
	CNewHandTask(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNewHandTask();

// �Ի�������
	enum { IDD = IDD_NEWHANDTASK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void GetImagePlazaPath(CString& strRetValue);
	afx_msg void OnPaint();
	afx_msg void OnbtnDailyTask();
	afx_msg void OnbtnChallengeTask();
	afx_msg void OnbtnNewHandTask();
	afx_msg void OnbtnClose();

private:
	CSkinButton								m_btDailyTask;								///< ÿ������
	CSkinButton								m_btChallengeTask;						///< ��ս����
	CSkinButton								m_btNewHandTask;						///< ��������
	CSkinButton								m_btClose;									///< �ر�
	CWebBrowser							m_webBrower;								///< �����

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#endif