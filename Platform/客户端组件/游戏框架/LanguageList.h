#pragma once
#include "Resource.h"
#include <vector>

//#include " E:/ZhuQue/Platform/�ͻ������/��Ϸ���/GameFrame.h"
#include "ClientKernel.h"
#include "GameFrameDlg.h"

using namespace std;
// CLanguageList �Ի���

//�б���Ϣ
typedef struct
{
	UINT	uiIndex;		//���
	char	chName[128];	//����
	char	chValue[512];	//��ֵ
	CRect	rcArea;			//��ʾ����
}LListInfo;

class CLanguageList : public CDialog
{
	DECLARE_DYNAMIC(CLanguageList)

public:
	CLanguageList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLanguageList();

// �Ի�������
	enum { IDD = IDD_DLG_GAME_LAUGUAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg void OnPaint();

public:
	//�������
	void GetLanguage(DWORD dwIndex, CString& strName, CString& strValue);
	//���ÿͻ����ں�
	void SetClientKernel(IClientKernel * pIClientKernel);

public:
	afx_msg void OnLeft();
	afx_msg void OnRight();
public:
	//��ʼ������
	void InitData();

private:
	//������λ��
	int CheckMousePos(const CPoint& point);
	//����������յ�
	void CountSEPoint();
	//���ô���
	//BOOL SetImageRgn(CPngImage * pImage);

private:
	//��ҳ��
	DWORD		m_dwTotalPageNums;
	//��ǰҳ��
	DWORD		m_dwCurPageIndex;
	//���
	DWORD		m_dwIndexStart;
	//�յ�
	DWORD		m_dwIndexEnd;
	//��굱ǰIndex
	int			m_dwMouseCurIndex;

private:
	//�б���Ϣ
	std::vector<LListInfo>	m_vecListInfo;
	//��ʾ����
	CRect			m_rcShowRect;
	//�ı���ʾ����
	CRect			m_rcPageShorRect;

private:
	//����
	CFont		m_TextFont;

	//����
	CPngImage	m_BackImage;
	//�ƶ�����
	CPngImage	m_MoveImage;
	//��
	CSkinButton	m_btLeft;
	//��
	CSkinButton	m_btRight;

private:
	IClientKernel*	m_pClientKernel;


private:
	bool		m_bIsCanSend;

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
};
