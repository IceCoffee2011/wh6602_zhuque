#ifndef __H__COMBOBOX
#define __H__COMBOBOX

#pragma once
#include "SkinControls.h"

/***************************************************/
/*2007-04-07			������			QQ:82358112*/
/***************************************************/

//==========================================================================
// ComboBoxEx(��������)
//@:ע��	���������OnPaint()���������DrawBorDer(CDC* pDC)����
class SKIN_CONTROL_CLASS ComboBoxEx : public CComboBox
{
	DECLARE_DYNAMIC(ComboBoxEx)

public:
	ComboBoxEx();
	virtual ~ComboBoxEx();	
	virtual void PreSubclassWindow();	

public:
	//���Ʊ߿��뱳��ɫ
	void DrawBorder(CDC* pDC);

	//����ʹ��Ĭ������
	void SetDefault();

	//���ñ�����ɫ
	void SetBKColor(COLORREF bkcolor);

	//���ñ༭�򱳾���ɫ
	void SetEditBKColor(COLORREF bkcolor);

	//���ñ༭���еİ�ťƫ��
	void SetEditBTX(int iabsX=0);
	void SetEditBTY(int iabxY=0);

	//����������ɫ
	void SetFontColor(COLORREF fontcolor);

	//����������ť����ͼ
	void SetArrowBKImage(PTCHAR Path);
	void SetArrowBKImage(class CSkinImage* pTmpImg);

protected:	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnPaint();	
	DECLARE_MESSAGE_MAP()

private:
	void DrawArrow();

private:
	class CSkinComBoboxRes*	pData;	
public:
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

#endif//__H__COMBOBOX