#pragma once
#include "resource.h"
#include "afxcmn.h"
#include "exmain.h"
// CShopDialog �Ի���

class CShopDialog : public CDialog
{
	CExmain	* m_exMain;
private:
	DECLARE_DYNAMIC(CShopDialog)

public:
	CShopDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShopDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_SHOP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	DECLARE_DISPATCH_MAP()//����dispatch map��
	DECLARE_EVENTSINK_MAP()
	BOOL onHtmlContextMenu(IHTMLEventObj *pEvtObj);
	BOOL onHtmlMouseDown(IHTMLEventObj *pEvtObj);
	void OnDocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT FAR* URL);
	//�¼�������,ԭ�Ϳ��Բο�MSDN�й���HTMLDocumentEvents2��˵��
	DWORD m_dwCookie;
	//���ڱ��һ�����ӵ�
	IHTMLDocument2* pDoc2;
	//��Ҫ�����¼���COM�����ָ��
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	//��ʾһ������
	void SetWindowShow(bool bIsShow=true);
	//��������
	void SetResetURL();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
