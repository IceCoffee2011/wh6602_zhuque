//
// GdipButton.h : Version 1.0 - see article at CodeProject.com
//
// Author:  Darren Sessions
//          
//
// Description:
//     GdipButton is a CButton derived control that uses GDI+ 
//     to support alternate image formats
//
// History
//     Version 1.0 - 2008 June 10
//     - Initial public release
//
// License:
//     This software is released under the Code Project Open License (CPOL),
//     which may be found here:  http://www.codeproject.com/info/eula.aspx
//     You are free to use this software in any way you like, except that you 
//     may not sell this source code.
//
//     This software is provided "as is" with no expressed or implied warranty.
//     I accept no liability for any damage or loss of business that this 
//     software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SkinControls.h"

// GdipButton.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CGdipButton window
#include <gdiplus.h>
using namespace Gdiplus;  
#pragma comment(lib, "gdiplus.lib")


class SKIN_CONTROL_CLASS CGdipButton : public CButton
{
public:

	CGdipButton();
	virtual ~CGdipButton();

	BOOL LoadImage(const WCHAR * pwzFilePath);
	BOOL LoadStream(IStream * pStream);
	BOOL LoadImage(UINT uResourceID, HINSTANCE hResourceDLL, const LPCTSTR szResType=_TEXT("PNG"));

	void SetBkGnd(CDC* pDC);

	void SetToolTipText(CString spText, BOOL bActivate = TRUE);
	void SetToolTipText(UINT nId, BOOL bActivate = TRUE);
	void SetHorizontal(bool ImagesAreLaidOutHorizontally = FALSE);
	void DeleteToolTip();
	
	//����͸����
	void SetAlpha(float falpha=1.00000f);

	//��ð�ť�Ŀ��
	UINT GetWidth();
	//��ð�ť�ĸ߶�
	UINT GetHeight();

	//�Ƿ�Ϊ��
	bool IsNull();

	//���ð�ť
	void ResetBT();

protected:

	void PaintBk(CDC* pDC);
	void PaintBtn(CDC* pDC);

	BOOL	m_bIsHovering;
	BOOL	m_bIsTracking;

	DWORD   m_dwToken;

	int		m_nCurType;

	float	m_fAlpha;

	Gdiplus::Image* m_pStdImage;

	CString			m_tooltext;
	CToolTipCtrl*	m_pToolTip;
	
	void	InitToolTip();

	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//{{AFX_MSG(CGdipButton)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam) ;
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()


	//ͨ���������ļ�
	IStream * GetStreamFromRes(UINT uResourceID, HINSTANCE hResourceDLL, LPCTSTR szResType=_TEXT("PNG"));
	//���ô��ڴ�С
	void SetWndSize();
	//�����ʼ��Ϊ��
	void SetDCNull();
	//����
	void Update();

private:

	CDC		m_dcBk;			// button background
	
	CDC		m_dcStd;		// standard button
	CDC		m_dcStdP;		// standard button pressed
	CDC		m_dcStdH;		// standard button hot
	CDC		m_dcStdF;		// standard button get fosucs
	CDC		m_dcGS;			// grayscale button (does not have a hot or pressed state)

	CDC*	m_pCurBtn;		// current pointer to one of the above

	BOOL	m_bIsResetDC;

};
