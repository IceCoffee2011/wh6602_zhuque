#pragma once
#include "Stdafx.h"
#include "PngImage.h"
#include <vector>


using namespace std;

// CTaskFinishFlash �Ի���

class SKIN_CONTROL_CLASS CGameFlash : public CDialog
{
	DECLARE_DYNAMIC(CGameFlash)

public:
	CGameFlash(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGameFlash();

// �Ի�������
	enum { IDD = 63020 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();

//private:
	void UpdateClockDisplay();

public:
	//init
	bool InitFlash(CString pImage, int iTotalFrame, CPoint ptClient, int iTransparent=100);
	//set pt
	void SetFlashPosition(CPoint ptClient);
	//time interval
	void SetTimeInterval(int iGFTimeInterval=100, int iStopTimeInterval=1000, int iAlphaTimeInterval=100);
	//���Ŷ���
	void Play(bool bIsPlay=true, int iPlayMode=0, int iLoopCount=-1);

private:
	CPngImage			m_pImageBack;

	BLENDFUNCTION m_Blend;
	int m_BakWidth , m_BakHeight ;
	HINSTANCE hFuncInst ;
	typedef BOOL (WINAPI *MYFUNC)(HWND,HDC,POINT*,SIZE*,HDC,POINT*,COLORREF,BLENDFUNCTION*,DWORD);          
	MYFUNC UpdateLayeredWindow;
	int	m_iTransparent;
	int	m_iSaveTransparent;
private:
	CPoint		m_pClientPos;
	//
	int			m_iTotalFrame;
	//֡����
	int			m_iCurFrame;
	//
	int			m_iPlayMode;
	//
	int			m_iLoopCount;
	int			m_iLoopNums;

	//time interval
	int			m_iGFTimeInterval;
	int			m_iStopTimeInterval;
	int			m_iAlphaTimeInterval;
};
