#pragma once

#include "GameFrame.h"
#include "GameFrameDlg.h"
#include "GameClientDJInfo.h"
#include <vector>


using namespace std;
// CUserDaoJu

#define		IDC_BT_USERDJ_LEFT				WM_USER+10000
#define		IDC_BT_USERDJ_RIGHT				WM_USER+10001
#define		IDC_BT_USERDJ_SET				WM_USER+10002
#define		IDC_BT_USERDJ_HELP				WM_USER+10003
#define		IDC_BT_USERDJ_CITY				WM_USER+10004
#define		IDC_BT_USERDJ_TASK				WM_USER+10005
#define		IDC_BT_USERDJ_ACTIVE			WM_USER+10006

class CShowDJInfo;

class CUserDaoJu : public CWnd
{
	DECLARE_DYNAMIC(CUserDaoJu)

public:
	CUserDaoJu(CGameFrameDlg * pGameFrameDlg);
	virtual ~CUserDaoJu();


	//��Ϣӳ��
protected:
	//�滭����
	VOID OnPaint();
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
	CGameFrameDlg				* m_pGameFrameDlg;				//�Ի���
	CGameClientDJInfo			* m_pUserDJInfo;				//������Ϣ
	CShowDJInfo					* m_pShowDJInfo;				//��ʾ������Ϣ
	bool						m_bIsTimer;

private:
	CToolTipCtrl	m_ToolTipCtrl;					//��ʾ�ؼ�
	CImage			m_DaoJuImageBuffer;					//����λͼ
	CPngImage		m_ImageWndBack;						//���ڱ���

	CSkinButton		m_BTLeft;							//��
	CSkinButton		m_BTRight;							//��

	bool			m_bHovering;

	//��ҳ�ж�
	int				m_iTotalPage;			//��ҳ��
	int				m_iCurrentPage;			//��ǰҳ
	//��ǰ����λ��
	int				m_iCurrentMousePoint;	//��ǰ���λ��

private:
	//��ť�¼�
	void OnBnLeftClickedEvent();
	void OnBnRightClickedEvent();

	//BOOL SetImageRgn(CPngImage * pImage);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT nIDEvent);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	//����뿪��Ϣ
	LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);


	//ɾ������
	bool DelUserUseDJFromeIndex(int iIndex);
	//���µ�����Ϣ
	void UpdateUserDJPage();
	//���õ���ҳ��
	void UpdateUserDJInfo(int iIndex);

	//������ʾ������Ϣָ��
	void SetShowDJInfo(CShowDJInfo* pShowDJInfoPtr);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


