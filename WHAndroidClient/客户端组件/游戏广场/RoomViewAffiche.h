#pragma once

#include "stdafx.h"
#include "resource.h"
// CRoomViewAffiche

//�����б���Ϸ������
class CRoomViewAffiche : public CWnd
{
	DECLARE_DYNAMIC(CRoomViewAffiche)

public:
	CRoomViewAffiche();
	virtual ~CRoomViewAffiche();

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

private:
	CSkinImage						m_ImageBack;						//����
	CFont							m_fontDrawGameInfo;					//��������
	CDC								m_memdc;							//���ھ��
	bool							m_bIsDisableGameMsg;				//�Ƿ񼤻���Ϸ����
	CRect							m_rcGameMsgInfo;					//�����������
	CRect							m_rcCurMsgInfo;						//��ǰ����
	std::vector<string>				m_vecGameMsg;						//��Ϸ����
	int								m_iCountVecIndex;					//��ǰ�������
	std::string						m_strSaveGameAffiche;				//������Ϸ����

private:
	//��ȡ��ǰ·��
	void GetCurrentDirectoryPath(CString &szPath);
	//����·��
	void GetImagePlazaPath(CString& strRetValue);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
};


