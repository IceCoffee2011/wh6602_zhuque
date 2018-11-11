#ifndef SCORE_VIEW_HEAD_FILE_DEF_
#define SCORE_VIEW_HEAD_FILE_DEF_

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CGameScoreView : public CScoreView
{
	//��������
protected:
	LONGLONG						m_lGameTax[3];						  //��Ϸ˰��
	LONGLONG						m_lGameScore[GAME_PLAYER];			  //��Ϸ�÷�
	TCHAR							m_szUserName[GAME_PLAYER][NAME_LEN];  //�û�����
	LONGLONG                        m_RoomScore[GAME_PLAYER];             //�������
	int                             m_UserBeiShu[GAME_PLAYER];            //��Ϸ����
	WORD                            m_MeId;                               //�ҵ����Ӻ�
	WORD                            m_LandChair;                          //�������Ӻ�
	BYTE                            m_ActionCount;                        //����֡��
	bool                            m_IsShow;                             //�Ƿ񲥷�
	int                             m_EncourageGem[GAME_PLAYER];          //������ʯ
	bool                            m_IsUserLeft[GAME_PLAYER];            //����Ƿ�ǿ��
	CRect							m_rcDrawTimer;

	//��Դ����
protected:
	CPngImage						m_ImageWin;						      //ʤ����ʶͼƬ
	CPngImage                       m_ImageLose;                          //ʤ����ʶͼƬ 
	CPngImage                       m_BalanceBei;                         //����ͼƬ
	CPngImage                       m_ImageRoomScore;                     //�������ͼƬ 
	CPngImage                       m_ImageServers;                       //�����ͼƬ
	CPngImage                       m_ImageLand;                          //����
	CPngImage                       m_ImageFarmer;                        //ũ��
	CPngImage                       m_ImageLandWin;                       //����ӮͼƬ
	CPngImage                       m_ImageLandLose;                      //������ͼƬ
	CPngImage                       m_ImageFarmerWin;                     //ũ��ӮͼƬ
	CPngImage                       m_ImageFarmerLose;                    //ũ����ͼƬ
	CPngImage                       m_ImageUserLeft;                      //���ǿ��ͼƬ
	CPngImage                       m_ImageCountDown;                     //����ʱͼƬ
	//��ť
public:
	CSkinButton                     m_btGoOn;                             //������ť
	CSkinButton                     m_btChangeTable;                      //����
	CSkinButton                     m_btExit;                             //�˳�
	CSkinButton                     m_btOverTime;                         //��ʱ

	//��������
public:
	//���캯��
	CGameScoreView(void);
	//��������
	virtual ~CGameScoreView(void);

	//���ܺ���
public:
	//���û���
	void ResetScore();
	//����˰��
	void SetGameTax(WORD wChairID,LONGLONG lGameTax);
	//���û���
	//void SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONGLONG lScore);
	void CGameScoreView::SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONGLONG lScore,int UserBeiShu,LONGLONG RoomScore);
	void CGameScoreView::GetMeID(WORD MeChair,WORD landchair);
	void CGameScoreView::GetWinLose(bool Istrue);
	void CGameScoreView::SetCountDown(bool Istrue);
	//���ñ�ʯ
	void CGameScoreView::SetGem(WORD wChairID ,int iGem);
	//����ǿ��
	void CGameScoreView::SetUserLeft(WORD wchairID,bool IsLeft);
public:
	//��ʼ����Ϣ
	virtual void InitScoreViewInfo();
	//��ʼ����Ϣ��
	virtual void AlreadyInitInfo();
	//����
	virtual void DrawScoreInfo(CDC* pDC);
	//��ȡ·��
	void GetPicPath(CString& strPath);
protected:
	//������ť
	afx_msg void OnGoOn();
	//�˳���ť
	afx_msg void OnExit();
	//������ť
	afx_msg void OnChangeTable();
	//��ʱ��ť
	afx_msg void OnOverTime();
	DECLARE_MESSAGE_MAP();
protected:
	//�Ƿ���ʾ����ʱ
	bool   m_IsTrue; 
	//����ʱ�ӳ�����
	BYTE   m_SecondCount;


public:
	afx_msg void OnTimer(UINT nIDEvent);
	//ˢ������
	void SafeInvalidate(CRect& rect, BOOL bErase);
};


//////////////////////////////////////////////////////////////////////////

#endif
