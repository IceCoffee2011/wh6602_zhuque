#pragma once
#include "resource.h" 
#include "exmain.h"
#include "WebPage.h"
#include "GameListManager.h"
#include <MsHTML.h>//������IHTMLDocument2�Ƚӿ�
#include <mshtmdid.h>//������HTMLDocumentEvents2�ӿڵķ���DISPID

// CGameListDlg �Ի���

class CGameListDlg : public CDialog
{
public:
	//�б�
	CExmain		*				m_exMain;
	IStream*		m_iStream;
	IWebBrowser2*	m_pBrowser;
	CWebPage		m_webPage;
	CExmain		*	m_pExAd;					//���
	//CRgnButton		m_btnHonurRoll;				//������� IDC_BT_HONOUR_ROLL
	CRgnButton		m_btnSite;					//�ٷ���վ IDC_BT_SITE
	CRgnButton		m_btnCharge;				//��ֵ���� IDC_BT_CHARGE
	CRgnButton		m_btnClinet;			    //�ͻ����� IDC_BT_CLINET
	CRgnButton		m_btnAccountsManage;		//�ʺŹ��� IDC_BT_RE_ACCOUNTS_MANAGE

	CToolTipCtrl	m_ToolTipCtrl;					//��ʾ�ؼ�
	CSkinImage		m_ImageBack;
private:
	DECLARE_DYNAMIC(CGameListDlg)

public:
	CGameListDlg(/*CWnd* pParent = NULL*/);   // ��׼���캯��
	virtual ~CGameListDlg();

// �Ի�������
	
	enum { IDD = IDD_GAME_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual void OnPaint();
	virtual void OnCancel(){}
	virtual void OnOK();
	virtual void OnLButtonDown(UINT nFlags, CPoint point);

	//��ť�¼�
// 	//������� IDC_BT_HONOUR_ROLL
// 	afx_msg void OnBnClickedHonurRoll();
	//�ٷ���վ IDC_BT_SITE
	afx_msg void OnBnClickedSite();
	//��ֵ���� IDC_BT_CHARGE
	afx_msg void OnBnClickedCharge();
	//�ͻ����� IDC_BT_CLINET
	afx_msg void OnBnClickedClinet();
	//�ʺŹ��� IDC_BT_RE_ACCOUNTS_MANAGE
	afx_msg void OnBnClickedAccountsManage();		
	DECLARE_MESSAGE_MAP()

public:
	//����
	void SetHtml();
	void ShowHtml(CString content);

public:
		CGameListManager * m_gameListManager;



		
public:
	DECLARE_DISPATCH_MAP()//����dispatch map��
	DECLARE_EVENTSINK_MAP()
	BOOL onHtmlContextMenu(IHTMLEventObj *pEvtObj);
	void OnDocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT FAR* URL);
	void OnDocumentCompleteExplorer2(LPDISPATCH pDisp, VARIANT FAR* URL);
	//�¼�������,ԭ�Ϳ��Բο�MSDN�й���HTMLDocumentEvents2��˵��
	DWORD m_dwCookie;
	//���ڱ��һ�����ӵ�
	IHTMLDocument2* pDoc2;
	//��Ҫ�����¼���COM�����ָ��
 	
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
};
