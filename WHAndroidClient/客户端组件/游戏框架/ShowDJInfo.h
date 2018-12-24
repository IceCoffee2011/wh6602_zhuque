#pragma once
#include "Resource.h"
#include "MoveFun.h"

// CShowDJInfo �Ի���
class CGameClientDJInfo;

class CShowDJInfo : public CDialog
{
	DECLARE_DYNAMIC(CShowDJInfo)

public:
	CShowDJInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShowDJInfo();

// �Ի�������
	enum { IDD = IDD_SHOWDJINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


public:
	//��ʾ������Ϣ����
	void SetWndShow(bool bIsShow=true, int iIndex=0);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//��ʾ������Ϣ
	void SetShowDJDiscription(int idjindex);

public:
	//������ɫ
	void AddRectRgn(CDC* pDC,CRgn* rgn,CRect* rect,COLORREF col);
	void UpdateRgn(CDC * pDC);
	bool DrawWindow(CDC * pDC);

private:
	
	CSkinImage							m_ImageBack;
	
	MoveFun<float>						m_MoveInfo;						//�ƶ���Ϣ
	CPoint								m_rcSaveOldWnd;					//����ɴ���λ��
	CPoint								m_rcSaveNewWnd;					//�����´���λ��
	int									m_iCountMoveIndex;				//�ƶ�֡
	int									m_iCountMoveTotal;				//�ƶ���֡��
	int									m_iSaveMoveIndex;				//����ɵ��ƶ����

	int									m_iSaveCurDJID;					//���浱ǰ����ID


	CGameClientDJInfo					* m_pGameClientDJInfo;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
};
