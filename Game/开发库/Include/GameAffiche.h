#pragma once
#include "Resource.h"
#include <string>
#include <vector>
#include "ThreadGuard.h"

// CGameAffiche �Ի���

using namespace std;

class CGameAffiche : public CDialog
{
	DECLARE_DYNAMIC(CGameAffiche)

public:
	CGameAffiche(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGameAffiche();

// �Ի�������
	enum { IDD = IDD_DLG_GAMEAFFICHE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


public:
	//����Ϊ��չ����
	void SetWndExp(DWORD wAlpha=30);
	//������Ϸ����
	void SetGameAfficheInfo(const char* pInfo);
	//��������
	void ParseChar(const char* pData, WORD wDataSize);
	//���ñ���ͼ
	void SetBackImage(const char* pImage, bool bIsRgnWnd=false);
	//����Ƿ�Ϊ��չ����
	bool GetIsExp();
	//������ʾ����
	void SetShowRect(CRect rect);
	//����������ɫ
	void SetTextColor(COLORREF textcol=RGB(255, 255, 255));
	//����X�Ὺʼλ��
	void SetXAbs(DWORD dwXAbs);
	//���ò���
	void SetMoveLeep(DWORD dwLeep=2);
	//����ʱ��Ƶ��
	void SetTimerInterval(DWORD dwTimeInterval=30);
	//��ʼ����
	void SetPlay(bool bIsPlay=true, int iLoopNums=0);

private:
	//BOOL SetImageRgn(CPngImage * pImage);

private:
	CPngImage						m_ImageBack;						//����
	CFont							m_fontDrawGameInfo;					//��������
	CDC								m_memdc;							//���ھ��
	bool							m_bIsDisableGameMsg;				//�Ƿ񼤻���Ϸ����
	CRect							m_rcGameMsgInfo;					//�����������
	CRect							m_rcCurMsgInfo;						//��ǰ����
	std::vector<string>				m_vecGameMsg;						//��Ϸ����
	int								m_iCountVecIndex;					//��ǰ�������
	std::string						m_strSaveGameAffiche;				//������Ϸ����
	bool							m_bIsExp;							//�Ƿ���չ����
	COLORREF						m_bTextColor;						//������ɫ
	DWORD							m_wStartXAbs;						//X�Ὺʼƫ��
	DWORD							m_dwMoveLeep;						//�ƶ�����
	DWORD							m_dwTimeInterval;					//ʱ����
	int								m_iLoopNums;						//0:����ѭ��
	int								m_iCountLoop;


	//������
	CCriticalSpection	m_CriticalSpection;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
