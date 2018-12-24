#pragma once

#include "resource.h"

// CDlgLogonLogo �Ի���

class CDlgLogonLogo : public CDialog
{
	DECLARE_DYNAMIC(CDlgLogonLogo)

private:
	BLENDFUNCTION			m_Blend;
	HDC						m_hdcMemory;
	int						m_BakWidth , m_BakHeight ;
	Gdiplus::Image			*m_pImageBack;
	HINSTANCE				hFuncInst ;
	typedef BOOL			(WINAPI *MYFUNC)(HWND,HDC,POINT*,SIZE*,HDC,POINT*,COLORREF,BLENDFUNCTION*,DWORD);          
	MYFUNC UpdateLayeredWindow;

public:
	CDlgLogonLogo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLogonLogo();

// �Ի�������
	enum { IDD = IDD_LOGONLOGO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnMove(int x, int y);

public:
	//��ͼlogoͼ
	BOOL DrawLogo(int Transparent=255);

	afx_msg void OnPaint();
};
