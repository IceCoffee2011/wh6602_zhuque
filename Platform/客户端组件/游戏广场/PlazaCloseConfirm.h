#pragma once

#include "resource.h"
#include "afxwin.h"

// CPlazaCloseConfirm �Ի���

class CPlazaCloseConfirm : public CDialog
{
	DECLARE_DYNAMIC(CPlazaCloseConfirm)

public:
	CPlazaCloseConfirm(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlazaCloseConfirm();

// �Ի�������
	enum { IDD = IDD_PLAZA_CLOSE_CONFIRM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CDC							m_memdc;				

private:
	CPngImage					m_imgBK;					// ����
	CPngImage					m_imgFT;					// ǰ��(����)
	BYTE						m_byCloseType;				// 0:�˳���Ϸ;1:���µ�¼
	CRgnButton					m_btnOver;					// �رմ���
	CRgnButton					m_btnResetLogon;			// ���µ�¼
	CRgnButton					m_btnCancel;				// ȡ��
	CRgnButton					m_btnClose;					// �ر�

public:
	
	afx_msg void OnBnClickedPlazaCloseConfirmOver();

public:
	BYTE GetCloseType();

private:
	//��ȡ��ǰ·��
	void GetCurrentDirectoryPath(CString &szPath);
	//����·��
	void GetImagePlazaPath(CString& strRetValue);

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedPlazaCloseConfirmResetLogon();
	afx_msg void OnBnClickedPlazaCloseConfirmCancel();
	afx_msg void OnBnClickedPlazaCloseConfirmClose();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
};
