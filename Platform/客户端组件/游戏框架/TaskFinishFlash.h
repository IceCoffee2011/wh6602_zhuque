#pragma once

#include "Resource.h"
#include <vector>
#include "ThreadGuard.h"

using namespace std;
// CTaskFinishFlash �Ի���

class CTaskFinishFlash : public CDialog
{
	DECLARE_DYNAMIC(CTaskFinishFlash)

public:
	CTaskFinishFlash(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTaskFinishFlash();

// �Ի�������
	enum { IDD = IDD_DLG_TASK_FINISH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();

	BOOL UpdateClockDisplay(int Transparent);

	//���Ŷ���
	void Play(bool bIsPlay, UINT uiNums);

private:
	CPngImage			m_pImageBack;
	CPngImage			m_pImageText;
	CPngImage			m_pImageNums;

	BLENDFUNCTION m_Blend;
	CDC m_hdcMemory;
	int m_BakWidth , m_BakHeight ;
	HINSTANCE hFuncInst ;
	typedef BOOL (WINAPI *MYFUNC)(HWND,HDC,POINT*,SIZE*,HDC,POINT*,COLORREF,BLENDFUNCTION*,DWORD);          
	MYFUNC UpdateLayeredWindow;
	int	m_iTransparent;

	//�����ı�
	void DrawText(CDC* pDC);
private:
	CPoint		m_pClientPos;

	//֡����
	int			m_iCurFrame;
	

	//����
	std::vector<int>	m_vecNumList;

	//�Ƿ���ʾ�ı�
	bool		m_bIsShowText;

	//������
	CCriticalSpection	m_CriticalSpection;
};
