#pragma once

#include "GameFrame.h"
#include <vector>

using namespace std;

// CUserAction

//�ܹ�������
#define		TOTAL_FLASH_NUMS					20
#define		MAX_FRAME_NUMS						30

//����
struct UAPlayFlashInfo 
{
	WORD						wTotalFrame;	//������֡
	bool						bIsPlay;		//�Ƿ񲥷�
	WORD						wTimeInterval;	//ʱ����

	UAPlayFlashInfo()
	{
		wTotalFrame = 0;
		bIsPlay = 0;
		wTimeInterval = 0;
	}
};


// CShowUserActionInfo

class CShowUserActionInfo : public CWnd
{
	DECLARE_DYNAMIC(CShowUserActionInfo)

public:
	CShowUserActionInfo();
	virtual ~CShowUserActionInfo();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

public:
	//���������ʼ��
	void InitFlashData();
	//��ʼ������
	void InitShowData();



public:
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);

	//���ö������ı�
	void SetPlayFlash(BYTE byFlashIndex, const char* pSendName, const char* pRecvName);
	void SetFlashText(BYTE byFlashIndex, const char* pSendName, const char* pRecvName);
	
public:
	//������ɫ
	void AddRectRgn(CDC* pDC,CRgn* rgn,CRect* rect,COLORREF col);
	void UpdateRgn(CDC * pDC);
	bool DrawWindow(CDC * pDC);
	
private:
	//������Ϣ
	char				m_chActionInfo[1024];
	CSkinImage			m_ImageBack;

	CRect				m_rcTextRect;
	CRect				m_rcImageRect;

	WORD				m_wCurPlayFlashIndex;	//��ǰ֡

	//���Ŷ�������
	UAPlayFlashInfo				m_PlayFlash[TOTAL_FLASH_NUMS];
	CPngImage					m_vecFlashImageArray[MAX_FRAME_NUMS];
	WORD						m_wCountFlashImageNums;	//

	//�ı�
	std::string					m_strTextList;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


