#pragma once

#include "resource.h"

class CConfirmGrade : public CDialog
{
	DECLARE_DYNAMIC(CConfirmGrade)

public:
	CConfirmGrade(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConfirmGrade();

// �Ի�������
	enum { IDD = IDD_CONFIRM_GRADE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();						// ��ʼ��
	afx_msg void OnPaint();								// �滭

private: 
	void OnBnClickedConfirm();							//ȷ��
	void GetCurrentDirectoryPath(CString &szPath);		//��ȡ��ǰ·��
	void GetImagePlazaPath(CString& strRetValue);		//����·��

	DECLARE_MESSAGE_MAP()

private:
	CRgnButton					m_btnConfirm;			//ȷ��
	CPngImage					m_ImgBK;				//����

};
