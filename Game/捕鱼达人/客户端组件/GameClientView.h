#ifndef GAME_CLIENT_HEAD_FILE
#define GAME_CLIENT_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "ScoreView.h"
#include "ScoreView.h"
#include "GameClientViewDx.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//////////////////////////////////////////////////////////////////////////


//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��Ϸ����
public:
	bool							m_bInitGameTag;						//��ʼ��־

	//�ؼ����
public:
	CGameClientViewDX				m_GameClientViewDX;					//d3d����

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();


	//���ƽӿ�
public:
	//���ý���
	virtual void ResetGameView();

	//�̳к���
protected:
	//�����ؼ�
	virtual void RectifyGameView(INT nWidth, INT nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, INT nWidth, INT nHeight);

	//��Ϣ����
public:
	//��������
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	////��Ϣ��ȡ
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	DECLARE_MESSAGE_MAP()		
};

//////////////////////////////////////////////////////////////////////////

#endif