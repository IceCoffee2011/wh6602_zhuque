#pragma once

#include "resource.h"
// CGameRuleDlg �Ի���

class CGameRuleDlg : public CDialog
{
	DECLARE_DYNAMIC(CGameRuleDlg)

public:
	CGameRuleDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGameRuleDlg();

// �Ի�������
	enum { IDD = IDD_GAME_RULE };

private:
	CPngImage				m_ImgBK;					//����
	CRgnButton				m_btnClose;					//�ر�
	CString					m_sRuleUrl;					//����URL
	CWebBrowser				m_webBrowser;				//������ҳ

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

public:
	//����URL
	void SetRuleUrl(CString sUrl);

private:
	//��ȡ��ǰ·��
	void GetCurrentDirectoryPath(CString &szPath);
	//��Դ·��
	void GetResPath(CString strClipName, CString& strPath);
	//Բ��
	BOOL SetImageRgn(CPngImage * img);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
