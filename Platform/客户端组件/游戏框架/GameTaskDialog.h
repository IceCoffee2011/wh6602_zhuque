#pragma once

#include "Stdafx.h"
#include "GameFrame.h"
#include "Resource.h"

// CGameTaskDialog �Ի���

class CGameTaskDialog : public CDialog
{
	DECLARE_DYNAMIC(CGameTaskDialog)

public:
	CGameTaskDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGameTaskDialog();

// �Ի�������
	enum { IDD = IDD_GAME_TASK_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();


private:
	CImage					m_TaskImageBuffer;					//����λͼ
	CGDIPlus_Image			m_ImageTaskWndBack;					//���ڱ���
	BYTE					m_bCurrentTaskType;					//��ǰ��������.1:��������; 2:��ս����; 3:Ѱ������, 0:û����

	CSkinButton				m_BTTaskToDay;						//��������
	CSkinButton				m_BTTaskTJ;							//��ս����
	CSkinButton				m_BTTaskFind;						//Ѱ������

	CRect					m_rcInvalidateRect;					//ˢ������
	WORD					m_wSortID;							//�������
	CPoint					m_ptWnd;							//��������

	//����
	BYTE					m_byFlashTag;						//������ʶ
	int						m_iCountTimer;						//����ʱ��
	bool					m_bIsPlayFlash;						//�Ƿ񲥷Ŷ���
	CGDIPlus_Image			m_ImageFlash;

	//��������
private:
	CMD_GF_TASK_tagTask				m_tagTaskAcceptInfo;				//�ѽ��ܵ�����
	CMD_GF_TASK_tagTaskFinish		m_tagTaskFinishInfo;				//�����������



public:
	//�����������
	BYTE GetCurrentTaskType(void) { return m_bCurrentTaskType; }
	afx_msg void OnToDayTaskMsg();
	afx_msg void OnTJTaskMsg();
	afx_msg void OnFindTaskMsg();
	//�����ѽ��������
	void DrawTaskAcceptUI(CDC *pDC);
	//����������ɽ���
	void DrawTaskEndUI(CDC *pDC);
	// ���Ʊ���
	void DrawBackground(CDC *pDC, int nWidth, int nHeight,int iExcursionY=0);

	//������Ϣ
public:
	//�ѽ���������Ϣ
	void SetTaskAcceptInfo(const CMD_GF_TASK_tagTask* pTaskInfo);
	//�����������
	void SetTaskOnceFinish(const CMD_GF_TASK_tagTaskFinish* pTaskFinish);
	//���÷����ʶ
	void SetSortID(WORD wSortID);

	//��¼����Ľ���
	void SetTaskCount(DWORD wTaskID, DWORD iTaskCount, DWORD iTaskTotal);
	//�������
	void SetTaskEnd(DWORD wTaskID);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL SetWindowPos(const CWnd* pWndInsertAfter, int x, int y,
		int cx, int cy, UINT nFlags);
};
