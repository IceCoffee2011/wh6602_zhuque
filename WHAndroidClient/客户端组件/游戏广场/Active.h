#pragma once


// CActive �Ի���

class CActive : public CDialog
{
	DECLARE_DYNAMIC(CActive)

public:
	CActive(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CActive();
	/// ���õ�ַ
	void SetURL(LPCTSTR lpURL);

// �Ի�������
	enum { IDD = IDD_ACTIVE };

private:
	CWebBrowser							m_webBrower;								///< �����
	TCHAR										m_tchPath[MAX_PATH];				///< ���ʵ�ַ

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
