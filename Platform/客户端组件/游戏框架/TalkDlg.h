#pragma once
#include "Resource.h"
#include "GameFrame.h"
#include "atlimage.h"

// CTalkDlg �Ի���


#ifndef IDD_TALK_DLG
#define IDD_TALK_DLG    8081
#endif

class CTalkDlg : public CDialog
{
	DECLARE_DYNAMIC(CTalkDlg)

public:
	CTalkDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTalkDlg();

	CGDIPlus_Image		m_TalkS;	//�������	
	CGDIPlus_Image		m_TalkX;	//�������	
	CImage				m_ImagePic;//����ͼ
	CGDIPlus_Image		m_ImageExpression;//����
	CString				m_strTextInfo;//����

	POINT				m_ptWndPoint;	//����λ��
	POINT				m_ptSavePoint;	//��ǰλ��

	BYTE				m_btDirectionTag;//�����־. 1:����, 2:����, 3:����, 4:����
	BYTE				m_btTalkSkin;	//��������Ƥ��ID

// �Ի�������
	enum { IDD = IDD_SAY_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL SetImageRgn(CImage * pImage);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void SetTalkDlgPos(int iwx, int iwy, int ix, int iy);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString InterceptString(CString strSource, BYTE &btHavePic);
	void SetTalkText(CString strTalk);
	void SetTalkDlgShow();
	afx_msg void OnTimer(UINT nIDEvent);
	void LoadTalkSkin(BYTE btSkinID=1);
	/// ɾ�����м�ʱ��
	void OnKillAllTimer();
};
