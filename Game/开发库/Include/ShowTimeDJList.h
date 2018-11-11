#pragma once
#include "Resource.h"
#include "GameClientDJInfo.h"
#include <vector>

//�ܵ�����
#define		GAME_DJ_TOTAL_NUMS			PROPERTY_COUNT

//������Ϣ
struct tag_dj_time_info
{
	int		idjID;		//����ID
	int		iICOIndex;	//ICO
	unsigned int uiTime;//ʣ��ʱ��(����Ϊ��λ)
};

using namespace std;

// CShowTimeDJList �Ի���

class CShowTimeDJList : public CDialog
{
	DECLARE_DYNAMIC(CShowTimeDJList)

public:
	CShowTimeDJList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShowTimeDJList();

// �Ի�������
	enum { IDD = IDD_SHOWTIMEDJLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	//��̬��ô��ڿ����
	CSize GetDlgWH(void);
	//���ô��ڴ�С
	void SetResetWndSize(void);

	CPngImage							m_ImageWndTop;			//��
	CPngImage							m_ImageWndCenter;		//��
	CPngImage							m_ImageWndDown;			//��
	CPngImage							m_ImageDJBackFrame;		//���߱�����
	CPngImage							m_ImageDJTimeNums;		//����ʱ������
	CPngImage							m_ImageNoMinute;		//����һ����ͼƬ
	
	bool								m_bIsDisableTime;		//����ʱ��
	CGameClientDJInfo					* m_pUserDJInfo;		//������Ϣ
public:
	virtual BOOL OnInitDialog();
//	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnPaint();
	//���Ʊ���
	void DrawWndBack(CDC *pDC);
	//���Ƶ���ʱ��
	void DrawDJTimeItem(CDC *pDC, int iIndex, int iX, int iY);

	//��ʾ����
	void SetShowWnd(bool bShow=true);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	
	//��ʼ��ʱ
	void SetDisableAndUpdateWnd(bool bIsDisable=true);
};
