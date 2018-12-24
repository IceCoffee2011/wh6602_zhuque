#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CSetDialog �Ի���

class CSetDialog : public CSkinDialogEx
{
public:
	CRgnButton m_btnOk;
	//CRgnButton m_btnCancel;
	//CRgnButton m_btnResume;
	//CRgnButton m_btnClose;
	CComboBox  m_combo;
	//CSliderCtrl m_slider1;
	//CSliderCtrl m_slider2;
	//CSliderCtrl m_slider3;
	//CSliderCtrl m_slider4;
	CBrush		m_ctrBrush;
private:
	DECLARE_DYNAMIC(CSetDialog)

public:
	CSetDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
// 	afx_msg void OnBnClickedRadio2();
// 	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnPaint();


public:
	void Init();
	afx_msg void OnBnClickedBtnResume();
protected:
	virtual void OnOK();
public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedCheckBackMusic();
	afx_msg void OnBnClickedCheckGameMusic();
	afx_msg void OnBnClickedCheckManMusic();
	afx_msg void OnBnClickedCheckCartoonMusic();
	afx_msg void OnDestroy();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
