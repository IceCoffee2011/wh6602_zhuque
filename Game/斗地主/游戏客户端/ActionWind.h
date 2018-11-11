#pragma once
#include "afxwin.h"
#include "BitmapSlider.h"
#include "Resource.h"
// CGameSoundSet �Ի���


class CGameFrameDlg;

class CGameSoundSet : public CImgDialogBase
{
	DECLARE_DYNAMIC(CGameSoundSet)

public:
	CGameSoundSet(LPCTSTR lpszFile, CWnd* pParent = NULL);	// standard constructor
	virtual ~CGameSoundSet();

//// �Ի�������
//	enum { IDD = IDD_DIALOG_GAMESET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:

	CGameFrameDlg*	m_pGameFrameDlg;

private:
	CPngImage		m_BackImage;


public:
	virtual BOOL OnInitDialog();
	virtual void OnDraw(Graphics & graphics);

public:
	BOOL SetImageRgn(CPngImage * pImage);
	afx_msg void OnPaint();
};
