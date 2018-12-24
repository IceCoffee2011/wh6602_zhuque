#pragma once

#include "Stdafx.h"
#include "Resource.h"

// CSendMsgDlg �Ի���

class CGameFrameControl;

class CSendMsgDlg : public CRgnDialog
{
public:
	CSendMsgDlg();   // ��׼���캯��
	virtual ~CSendMsgDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

private:
	CRgnButton				m_btWndTag;						//��־��ť
	CComboBox				m_ChatInput;					//��������
	CRgnButton				m_btSendChat;					//���Ͱ�ť
	CRgnButton				m_btExpression;					//���鰴ť
	CExpression				m_Expression;					//���鴰��
	CRgnButton				m_btWndTagLight;				//��־��ť����

	CGameFrameControl*		m_pGameFrameControl;			//��ܿؼ�ָ��
	DWORD					m_dwLimitChatTimes;				//������ʱ
	bool					m_bIsShowGameFrame;				//��ʾ��־
	CImage					m_ImageBuffer;					//����λͼ
	int						m_iCountMsgNums;				//������Ϣ����
	bool					m_bIsShowWndTag;				//��Ӧ��־
	bool					m_bIsStartTime;					//�Ƿ�������ʱ��


private:
	CSkinImage				m_ImageWndBack;					//���ڱ���

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//������Ϣ
	LRESULT OnHitExpression(WPARAM wParam, LPARAM lParam);

	//����Ҽ�������Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);


	//������Ϣ
	void SetGameChatMsg();

	//������ʾ
	void SetGameChatShow();

public:
	//��Ӧ��Ϣ
	void SetMsgBeep();

public:
	virtual BOOL OnInitDialog();

	//���ô���ָ��
	void SetGameFrameControlPtr(CGameFrameControl* pGameFrameControl);

	//������ʾ��־
	void SetGameFrameShowTag(bool bIsShow=true);

	
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
};
