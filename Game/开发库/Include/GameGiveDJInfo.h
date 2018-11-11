#pragma once
#include "resource.h"

#include "PublicDef.h"

// CGameGiveDJInfo �Ի���

class CGameGiveDJInfo : public CDialog
{
	DECLARE_DYNAMIC(CGameGiveDJInfo)

public:
	CGameGiveDJInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGameGiveDJInfo();

// �Ի�������
	enum { IDD = IDD_DLG_DJ_GIVE_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	//������ɫ
	void AddRectRgn(CDC* pDC,CRgn* rgn,CRect* rect,COLORREF col);
	void UpdateRgn(CDC * pDC);
	bool DrawWindow(CDC * pDC);


	//������ֵ
	void SetGiveDJInfo(const char* chGiveInfo);

private:
	CSkinImage							m_ImageBack;
	char								m_chGiveInfo[1024];

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
};
