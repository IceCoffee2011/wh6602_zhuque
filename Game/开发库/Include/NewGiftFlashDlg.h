#pragma once

#include "Resource.h"
#include "GlobalFrame.h"

// CNewGiftFlashDlg �Ի���

class CNewGiftFlashDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewGiftFlashDlg)

public:
	CNewGiftFlashDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CNewGiftFlashDlg();

// �Ի�������
	enum { IDD = IDD_NEWGIFT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CMD_GF_NEW_GIFT						m_tagNewGift;						//�������
	CPngImage							m_ImgBK;							// ����
	CPngImage							m_imgFireworks;						// �̻�
	CPngImage							m_imgUpgrade;						// ��ʯ
	int									m_iUpgradeCount;					// ��ʯ֡��
	int									m_iFrameCount;						// ֡��
	BOOL								m_bShowVal;							// ��ʾ��ֵ
	CImageNums							m_imgnumVal;						// ��ʾ��ֵ

public:
	//�������	Author<cxf>; DateTime<2012-01-16>
	void InitDataRes(CMD_GF_NEW_GIFT * pNewGift);
	//������ʾ��ֵ
	void SetNum(int iVal);

private:
	//��ȡ��ǰ·��
	void GetCurrentDirectoryPath(CString &szPath);
	//����·��
	void GetImagePlazaPath(CString& strRetValue);

public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
