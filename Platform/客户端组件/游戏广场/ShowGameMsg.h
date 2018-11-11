#pragma once
#include "Stdafx.h"
#include "resource.h"
#include <string>
#include "afxwin.h"


using namespace std;

// CShowGameMsg �Ի���
struct GameMsg
{
	BYTE byMsgType;	//��Ϣ����
	std::string strTime;//ʱ��
	std::string strContex;//����
};

class CShowGameMsg : public CDialog
{
	DECLARE_DYNAMIC(CShowGameMsg)

public:
	CShowGameMsg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShowGameMsg();

// �Ի�������
	enum { IDD = IDD_SHOWGAMEMSG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	//������Ϣ
	bool InsertGameMsg(const char* pMsgType, const char* pTimeMsg, const char* pContexMsg);
	//���������Ϣ
	void ClearAllMsg();

private:
	CDC							m_memdc;
	std::vector<GameMsg>		m_vecMsgList;
	CFont						m_font;


	CPngImage					m_ImageBack;
	CPngImage					m_ImageICO[6];
	CSkinButton					m_BTLookMore;
	CSkinButton					m_BTLookOK;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtExit();
	afx_msg void OnBnClickedBtMore();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

private:
	//������Ϣ����
	BYTE GetMsgType(const char* pMsgType);
};
