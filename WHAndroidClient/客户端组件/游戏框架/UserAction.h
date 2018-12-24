#pragma once

#include "GameFrame.h"

class CUserAction : public CWnd
{
	DECLARE_DYNAMIC(CUserAction)

public:
	CUserAction();
	virtual ~CUserAction();

	virtual void OnFinalRelease();

public:
	//��õ�ǰ�û�����Ķ�����ʶ
	BYTE GetCurUserActionTag();
	//�Ƿ���Է���
	bool GetCurSendStatus();
	//�����Ƿ���Է���
	void SetCurSendStatus(bool bIsSend=true);
	//�����Ƿ���Է���
	void SetTimeIntervalSend(bool bIsSend=false);
	//��ó�ʱ�Ƿ���Է���
	bool GetTimeIntervalSendStatus();
	//��������
	void SetResetData();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//����뿪��Ϣ
	LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);

public:
	//������ɫ
	void AddRectRgn(CDC* pDC,CRgn* rgn,CRect* rect,COLORREF col);
	void UpdateRgn(CDC * pDC);
	bool DrawWindow(CDC * pDC);

private:
	bool			m_bIsSend;
	bool			m_bIsAlreadySend;
	bool			m_bHovering;
	BYTE			m_btCurUserActionTag;
	CSkinImage		m_ImageBack;

	CSkinButton		m_btUserAction[20];
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};


