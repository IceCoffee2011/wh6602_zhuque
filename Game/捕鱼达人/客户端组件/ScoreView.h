#ifndef SCORE_VIEW_HEAD_FILE
#define SCORE_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "SkinButtonEx.h"

//#define WM_QUIT_GAME		WM_USER+301								//�˳���Ϸ

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CFishScoreView : public CDialog
{
	//��������
protected:
	HRGN								m_hRGN;								//������
	LONG								m_lBulletCount[enBulletCountKind_End];//�ӵ���Ŀ
	LONG								m_lBulletCharge[enBulletCountKind_End];	//�ӵ�����
	WORD								m_wCptrFishCount[enFishKind_No];	//��Ⱥ��Ŀ
	BYTE								m_byAlphaDepth[enFishKind_No];		//͸��ϵ��
	LONGLONG							m_lCoinCount;						//�����Ŀ
	enFishKind							m_AlphaIndex;						//͸������
	bool								m_bShowNumber;						//��ʾ��ʶ
	LONG								m_lCellScore;						//��Ԫ����

	//��Դ����
protected:
	CPngImage							m_PngBackUp;						//����ͼ��
	CPngImage							m_PngBackDown;						//����ͼ��
	CPngImage							m_PngNumber;						//����ͼ��
	CPngImage							m_PngNumberScore;					//����ͼ��
	CPngImage							m_PngFish;							//��ȺͼƬ
	CSkinImage							m_ImageFish;						//��ȺͼƬ

	//�������
protected:
	CWnd								* m_pViewWnd;						//��ͼ����

	//��ť����
	CSkinButtonEx						m_btQuitGame;						//�˳���Ϸ
	CSkinButtonEx						m_btCalcScore;						//�������
	CSkinButtonEx						m_btContinueGame;					//������Ϸ

	//��������
public:
	//���캯��
	CFishScoreView(CWnd	*pViewWnd, LONG lCellScore);
	//��������
	virtual ~CFishScoreView();

	//���غ���
public:
	//��ʼ����
	virtual BOOL OnInitDialog();
	//������Ϣ
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//��Ϣ�ӿ�
public:
	//������Ϣ
	void SetScoreInfo(LONG lBulletCount[enBulletCountKind_End],WORD wCptrFishCount[enFishKind_No],LONGLONG lCoinCount, LONG lBulletCharge[enBulletCountKind_End]);

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//��С��Ϣ
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//��ʱ��Ϣ
	afx_msg void OnTimer(UINT nIDEvent);

	//��������
private:
	//�滭����
	void DrawNumber(CDC *pDC, int nXPos, int nYPos, LONGLONG lNumber, bool bShowQuantifier);
	//�滭����
	void DrawNumber(CDC *pDC, int nXPos, int nYPos, LONGLONG lNumber);

	DECLARE_MESSAGE_MAP()	
};

//////////////////////////////////////////////////////////////////////////

#endif