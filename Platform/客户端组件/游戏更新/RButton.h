//------------------------------------------------------------------------------
// �ļ����ƣ�RButton.h
// �ļ��汾��v1.0
// �������ڣ�2006-05-06 10:39
// ��    �ߣ�Richard Ree
// �ļ��������Ի水ť��
//------------------------------------------------------------------------------

#pragma once

#ifndef RBUTTON_H
#define RBUTTON_H

// CRButton

class CRButton : public CButton
{
public:
	enum enButtonType
	{
		ButtonTypeNormal = 0,
		ButtonTypeClose,
		ButtonTypeMaximize,
		ButtonTypeMinimize,
		ButtonTypeBottom,
		ButtonTypeLoadBitmap
	};

	DECLARE_DYNAMIC(CRButton)

public:
	CRButton(enButtonType m_iButtonType = ButtonTypeNormal);
	void SetLoadInfo(UINT uImage);
	void SetTextColorEx(COLORREF bg);
	virtual ~CRButton();

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
	virtual void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

protected:
	void DrawRoundBorder(CDC *pDC, CRect rcBorder);
	// ��λͼ��ʽ��水ť�ؼ���Ҫ��λͼ���Ϊ32���أ���15 + ��2 + ��15���м����ſؼ�������죩
	void DrawItemWithBitmap(HDC hdcDest, CRect& rc, HBITMAP hBitMap, UINT iBitMapHeight, UINT cx);
	// ��λͼ��ʽ���ϵͳ���ư�ť�ؼ���Ҫ��λͼ���Ϊ128��������
	void DrawSystemControlButtonWithBitmap(HDC hdcDest, CRect& rc, HBITMAP hBitMap, UINT cx);

protected:
	CBitmap m_bitBg;
	COLORREF m_rgbBg;
	enum enButtonType m_iButtonType;
	BOOL m_bTracking;
	BOOL m_bDefault;
	BOOL m_bFocus;
	BOOL m_bOver;
	BOOL m_bDown;
};

#endif // #ifndef RBUTTON_H
