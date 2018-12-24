#ifndef SCORE_VIEW_HEAD_FILE_FRAME_DEF__
#define SCORE_VIEW_HEAD_FILE_FRAME_DEF__

#pragma once

#include "GameFrame.h"

class CGameFrameDlg;

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class GAME_FRAME_CLASS CScoreView : public CDialog
{
	//��Դ����
protected:
	CImage							m_ImageBuffer;						//����λͼ
	CPngImage						m_ImagePngBack;						//
private:
	CGameFrameDlg					*m_pGameFrameDlg;					//��Ϸ���ָ��
	bool							m_bIsMoveWnd;						//�ɷ��ƶ�����

public:
	CSize							m_szWndSize;						//���ڴ�С
	//��������
public:
	//���캯��
	CScoreView();
	CScoreView(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	//��������
	virtual ~CScoreView();

	//������Ϸ���ָ��
	void SetGameFramePtr(CGameFrameDlg* pGameFrameDlg);
	//���ÿɷ��ƶ�����
	void SetDisableMoveWnd(bool bIsMove=true);

public:
	//���ر���ͼ
	bool	LoadBackImage(const char* pImagePath);
	//��ʼ����Ϣ
	virtual void InitScoreViewInfo() = 0;
	//����
	virtual void DrawScoreInfo(CDC* pDC) = 0;

public:
	//����UI�ָ�
	//BOOL SetImageRgn(CPngImage * pImage);

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);

public:
	//���ش���
	void OnGoPlaza();
	//������Ϸ
	void OnStartGame();
	//���ط���
	void OnGoRoom();
	//�˳���Ϸ
	void OnGoExit();
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

//////////////////////////////////////////////////////////////////////////

#endif
