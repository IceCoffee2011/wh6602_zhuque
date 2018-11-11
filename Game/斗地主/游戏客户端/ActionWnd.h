#pragma once
#include "Resource.h"

// ActionWnd �Ի���

class ActionWnd : public CDialog
{
	DECLARE_DYNAMIC(ActionWnd)

public:
	ActionWnd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ActionWnd();

// �Ի�������
	enum { IDD = IDD_GAME_FLASH };


public:
	void SetActionImage(CString strImage,bool bIsMove,int TotallFrames, bool bIsAutoHide=true);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	BOOL ImageFromIDResource(UINT nID,LPCTSTR sTR,Image * &pImg);
	BOOL UpdateClockDisplay(int Transparent=255);

	BLENDFUNCTION m_Blend;
	//CDC m_hdcMemory;		// add by HouGuoJiang 2011-11-25
											// ������̵߳����� UpdateClockDisplay ������m_hdcMemory ���������������m_hdcMemory=NULL�ı���
	int m_BakWidth , m_BakHeight ;
	HINSTANCE hFuncInst ;
	typedef BOOL (WINAPI *MYFUNC)(HWND,HDC,POINT*,SIZE*,HDC,POINT*,COLORREF,BLENDFUNCTION*,DWORD);          
	MYFUNC UpdateLayeredWindow;

	int		m_iTransparent;
	bool	m_bIsUpTag;
	DWORD   m_dwToken;
	CString m_strImage;

	//
	int		m_iCountCur;
	int		m_iTotalFrames;

	CPngImage	m_ImageBack;

	bool	m_bIsAutoHide;

	// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT nIDEvent);


};
