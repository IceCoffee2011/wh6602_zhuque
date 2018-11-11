#pragma once

#include "resource.h"
// CPlazaGameWeb �Ի���

class CPlazaGameWeb : public CDialog
{
	DECLARE_DYNAMIC(CPlazaGameWeb)

public:
	CPlazaGameWeb(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlazaGameWeb();

// �Ի�������
	enum { IDD = IDD_PLAZA_GAME_WEB };

private:

	CWebBrowser				m_WebBrowser;				//���Ƕ����ҳ

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
private:
	//��ȡ��ǰ·��
	void GetCurrentDirectoryPath(CString &szPath);
	//����·��
	void GetImagePlazaPath(CString& strRetValue);
	//��Դ·��
	void GetResPath(CString strClipName, CString& strPath);

public:
	//����������Ϸ��ҳ
	bool LoadPlazaGameWeb(UINT uiType);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
