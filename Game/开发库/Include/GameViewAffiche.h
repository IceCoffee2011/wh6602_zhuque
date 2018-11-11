#pragma once

#include "stdafx.h"
#include "resource.h"
#include <string>
#include <vector>

using namespace std;

//��Ϸ������
class CGameViewAffiche : public CWnd
{
	DECLARE_DYNAMIC(CGameViewAffiche)

public:
	CGameViewAffiche();
	virtual ~CGameViewAffiche();

public:
	//������Ϸ����
	void SetGameAfficheInfo(char* pInfo);
	//��������
	void ParseChar(const char* pData, WORD wDataSize);


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
	//���ñ���ͼ
	void SetBackImage(const char* pImage, bool bIsRgnWnd=false);


private:
	//BOOL SetImageRgn(CPngImage * pImage);

private:
	CPngImage						m_ImageBack;						//����
	CFont							m_fontDrawGameInfo;					//��������
	bool							m_bIsDisableGameMsg;				//�Ƿ񼤻���Ϸ����
	CRect							m_rcGameMsgInfo;					//�����������
	CRect							m_rcCurMsgInfo;						//��ǰ����
	std::vector<string>				m_vecGameMsg;						//��Ϸ����
	int								m_iCountVecIndex;					//��ǰ�������
	std::string						m_strSaveGameAffiche;				//������Ϸ����

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
};


