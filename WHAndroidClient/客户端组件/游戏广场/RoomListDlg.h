#pragma once

#include "resource.h"
#include "exmain.h"
#include "WebPage.h"
#include "SystemOption.h"
#include "GameListManager.h"
// CRoomListDlg �Ի���

#define  MAX_ROOM_UNIT_COUNT			(6)
class CRoomUnit			//���䵥Ԫ
{
	
public:
	CWnd * m_pParent;
	CRect m_rect;
	bool  m_bEable;
	WORD m_wKindID;
	WORD m_Index;
	WORD m_wLight;				//�� 0 ��, 1, 2
	long m_lCellScore;
	long m_lLessScore;
	CPngImage m_pngGameIcon;	//��ϷIcon 

	static CPngImage m_pngBack;		//����ͼ
	static CPngImage m_pngBeilu;		//�����ʡ�
	static CPngImage m_pngLimit;		//���ޡ�
	static CPngImage m_pngMoney;		//����ʯ��
	static CPngImage m_pngBtn;					//���밴ť
	static CPngImage m_pngManIdentifier;	//������ʶ
	static CPngImage m_pngNumber_Max;		    //��������
	static CPngImage m_pngNumber_Min;		    //С������
	static CPngImage m_pngTitle;		//���� �磺������
	//CRgnButton m_btnOk;

public:
	CRoomUnit()
	{
		m_bEable = FALSE;
		memset(&m_rect, 0, sizeof(m_rect));
		m_wKindID=0;
		m_lCellScore = 0;
		m_lLessScore = 0;
	}
	~CRoomUnit(){}
public:
	static void Init();
	static void Uinit();
	void SetInfo(CWnd * pWnd, WORD index, const CRect & rect);
	void EableUnit(bool bEable);
	void SetRect(const CRect & rect) { CopyMemory(&m_rect, &rect, sizeof(m_rect));}
	void SetData(const HINSTANCE & hInstance,  WORD wKindID, WORD wLight, long lCellScore, long lLessScore);//��ʼ��
	void OnPaint(CDC * pDC);
	CSize DarwNumber(CDC * pDC, CPngImage & pngImg, LONG number, const CPoint & pt);
	bool IsFouse();
	int OnLButtonDown(const UINT & nFlags, const CPoint & pt);
	int OnMouseMove(const UINT & nFlags, const CPoint & point);
	WORD GetIndex() { return m_Index; }	
	bool isEableUnit() {return m_bEable;}
};
class CRoomListDlg : public CSkinDialogEx
{
	DECLARE_DYNAMIC(CRoomListDlg)

public:
	CRoomListDlg(/*CWnd* pParent = NULL*/);   // ��׼���캯��
	virtual ~CRoomListDlg();

// �Ի�������
	enum { IDD = IDD_ROOM_LIST };
protected:
	//�б�
	CExmain		*	m_exMain;
	IStream*		m_iStream;
	IWebBrowser2*	m_pBrowser;
	CWebPage		m_webPage;
	
public:
	WORD m_wKindID;
	int m_nRoomNum;
	CGameListManager * m_gameListManager;
	DWORD							m_dwLimitChatTimes;					//������ʱ
	CRoomUnit						m_Unit[MAX_ROOM_UNIT_COUNT];		//���6����Ϸ������
	CPngImage						m_pngGameTitle;
public:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();

public:
	//afx_msg void OnBnClickedExpression();
	afx_msg void OnBnClickedDepository();
	afx_msg void OnBnClickedFriend();
	afx_msg void OnBnClickedShop();
	afx_msg void OnBnClickedTask();
	afx_msg void OnBnClickedTool();
	virtual void OnPaint();
	//LRESULT OnHitExpression(WPARAM wParam, LPARAM lParam);
protected:
	virtual void OnOK();
public:
	void SetResetHtml();
	void ShowHtml(CString content);
	void EnterRoom();
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
