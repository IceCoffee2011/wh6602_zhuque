#pragma once
#include "resource.h"

// CNewComerGuide �Ի���

class CNewComerGuide : public CDialog
{
	DECLARE_DYNAMIC(CNewComerGuide)

public:
	CNewComerGuide(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNewComerGuide();

// �Ի�������
	enum { IDD = IDD_NEWCOMER_GUIDE };

private:
	//CWebBrowser					m_webNewComerGuide;						// ����ָ��(��ҳ)
	CPngImage					m_ImgNewComerGuide;						// ����
	CPngImage					m_ImgTag;								// ѡ���־
	CPngImage					m_ImgNewComerGift;						// �������ͼƬ
	CPngImage					m_ImgGameTask;							// ��Ϸ����
	CPngImage					m_ImgGameCurrency;						// ��Ϸ����
	CPngImage					m_ImgPropsShop;							// �̳�
	CPngImage					m_ImgPrizeExcharge;						// �һ�ʵ��
	CPngImage					m_ImgGameHelp;							// ����
	CSkinButton					m_btnClose;								// �ر�
	CSkinButton					m_btnNewComerGift;						// �������
	CSkinButton					m_btnGameTask;							// ��Ϸ����
	CSkinButton					m_btnGameCurrency;						// ��Ϸ����
	CSkinButton					m_btnPropsShop;							// �̳�
	CSkinButton					m_btnPrizeExcharge;						// �һ�ʵ��
	CSkinButton					m_btnGameHelp;							// ����

	CRect						m_rcTagPos;								// ��־λ��
	WORD						m_wTagPos;								// �����ť

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//��ʼ��
	virtual BOOL OnInitDialog();

private:
	//��ȡ��ǰ·��
	void GetCurrentDirectoryPath(CString &szPath);
	//��Դ·��
	void GetResPath(CString strClipName, CString& strPath);
	//ת��
	void ExplainSwerve(CDC &memdc);
	//Բ��
	BOOL SetImageRgn(CPngImage * img);
public:
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedNewcomerGift();
	afx_msg void OnBnClickedNewcomerGametask();
	afx_msg void OnBnClickedNewcomerGamecurrency();
	afx_msg void OnBnClickedNewcomerPropsshop();
	afx_msg void OnBnClickedNewcomerPrizeexcharge();
	afx_msg void OnBnClickedNewcomerGamehelp();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedCancel();
};
