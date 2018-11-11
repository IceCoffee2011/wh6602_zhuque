//#include "StdAfx.h"
#pragma once
#include "SkinControls.h"
#ifndef __H__CEDIT
#define __H__CEDIT


/***************************************************/
/*2007-04-07			������			QQ:82358112*/
/***************************************************/

//==========================================================================
// CEditEx���༭�ؼ���
//@:ע��:�����Res��Դ��ʹ���˱��ؼ�������������ķ�������WS_BORDER����.
//@:ע��:����Ƕ�̬�����Ŀؼ��������Ƿ������WS_BORDER���Կؼ������Զ������������
class SKIN_CONTROL_CLASS CEditEx : public CEdit
{	
	DECLARE_DYNAMIC(CEditEx)

public:
	CEditEx();
	virtual ~CEditEx();
	virtual void PreSubclassWindow();
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);

	//����ʹ��Ĭ������
	void SetDefault();

	//���ñ�����ɫ
	void SetBKColor(COLORREF bkcolor);

	//����������ɫ
	void SetFontColor(COLORREF fontcolor);

protected:
	afx_msg void OnNcPaint();	
	afx_msg void OnPaint();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()	

protected:
	virtual void DrawBorder();
	
private:
	class CSkinEditRes*	pData;
};


#endif// __H__CEDIT