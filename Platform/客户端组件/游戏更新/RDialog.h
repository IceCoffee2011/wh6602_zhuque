//------------------------------------------------------------------------------
// �ļ����ƣ�RDialog.h
// �ļ��汾��v1.0
// �������ڣ�2006-05-05 16:36
// ��    �ߣ�Richard Ree
// �ļ��������Ի�Ի�����
// ʹ���Ի�Ի����������Զ���Ի���Ĳ����ǣ�
// 1�����Զ���Ի�����Ļ�����CDialog��ΪCRDialog
// 2����������Ĺ��캯�����Ի����ʼ������OnInitDialog�ȸ�������CDialog��ΪCRDialog
// 3�������������Ϣ����BEGIN_MESSAGE_MAP����Ӻ�RDIALOG_UI_MESSAGE_MAP
//------------------------------------------------------------------------------

#pragma once

#ifndef RDIALOG_H
#define RDIALOG_H

#include "RButton.h"

// ʵ���Ի�������Ϣӳ��꣬���˺���뵽�������BEGIN_MESSAGE_MAP��
#define RDIALOG_UI_MESSAGE_MAP\
	ON_WM_PAINT()\
	ON_WM_SIZE()\
	ON_WM_CTLCOLOR()\
	ON_WM_LBUTTONDOWN()

// CRDialog �Ի���

class CRDialog : public CDialog
{
	DECLARE_DYNAMIC(CRDialog)

public:
	// ����/��������
	CRDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	CRDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CRDialog();

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedMinimize();
	afx_msg void OnBnClickedMaximize();
	afx_msg void OnBnClickedClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

protected:
	// ��С����ť
	CRButton m_btnMinimize;
	// ��󻯰�ť
	CRButton m_btnMaximize;
	// �رհ�ť
	CRButton m_btnClose;
};

#endif // #ifndef RDIALOG_H
