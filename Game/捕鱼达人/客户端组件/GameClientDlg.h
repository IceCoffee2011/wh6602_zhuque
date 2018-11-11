#ifndef GAME_CLIENT_ENGINE_HEAD_FILE
#define GAME_CLIENT_ENGINE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ����
class CGameClientDlg : public CGameFrameDlg
{
	//��Ϸ����
protected:
	LONG							m_lBulletCharge[enBulletCountKind_End];	//�ӵ�����
	enBulletCountKind				m_BulletCountKindSelected;				//ѡ������
	LONG							m_lCellScore;							//��Ԫ����

	//�ؼ�����
public:
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ

	//��������
public:
	//���캯��
	CGameClientDlg();
	//��������
	virtual ~CGameClientDlg();

	//����̳�
private:
	//��ʼ����
	virtual bool InitGameFrame();
	//���ÿ��
	virtual void ResetGameFrame();
	//��Ϸ����
	virtual void OnGameOptionSet();
	//ʱ����Ϣ
	virtual bool OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID);
	//�Թ�״̬
	virtual void OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize);
	//������Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	virtual bool OnGameSceneMessage(BYTE cbGameStatus, bool bLookonOther, const void * pBuffer, WORD wDataSize);

	//�¼�����
private:
	//��Ⱥ�켣
	bool OnSubFishTrace(const void * pData, WORD wDataSize);
	//�����ڵ�
	bool OnSubUserShoot(const void * pData, WORD wDataSize);
	//������Ⱥ
	bool OnSubCaptureFish(const void * pData, WORD wDataSize);
	//�ӵ���Ŀ
	bool OnSubBulletCount(const void * pData, WORD wDataSize);
	//�����Ŀ
	bool OnSubCoinCount(const void * pData, WORD wDataSize);
	//��Ⱥ��Ŀ
	bool OnSubCptrFishCount(const void * pData, WORD wDataSize);
	//�л�����
	bool OnSubChangeScene(const void * pData, WORD wDataSize);
	//������Ϣ
	bool OnSubScoreInfo(const void * pData, WORD wDataSize);
	//��Ԫ����
	bool OnSubSetCellScore(const void * pData, WORD wDataSize);
	//�ʽ���Ϣ
	bool OnSubBonusInfo(const void * pData, WORD wDataSize);
	//��Ϸ��Ϣ
	bool OnSubGameMessage(const void * pData, WORD wDataSize);
		
	//��Ϣӳ��
protected:
	//������Ϣ
	afx_msg UINT OnNcHitTest(CPoint point);
	//������Ϣ
	LRESULT OnShoot(WPARAM wParam, LPARAM lParam);
	////������Ϣ
	//LRESULT OnCaptureFish(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnHitFish(WPARAM wParam, LPARAM lParam);
	////�����ӵ�
	//LRESULT OnShowBulletShop(WPARAM wParam, LPARAM lParam);
	//�˳���Ϸ
	LRESULT OnQuitGame(WPARAM wParam, LPARAM lParam);
	//��������
	LRESULT OnEnableSound(WPARAM wParam, LPARAM lParam);
	
	//LRESULT OnCapture(WPARAM wParam, LPARAM lParam);
	
	//�رպ���
	virtual void OnCancel();
	
	//�û��¼�
private:
	//�û�����
	virtual void __cdecl OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
	//�û��뿪
	virtual void __cdecl OnEventUserLeave(tagUserData * pUserData, WORD wChairID, bool bLookonUser);

public:
	//���������Ϸ
	virtual void OnEventContinueGame();//cxf

	////add by Ivan_han
	//BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
;
	//��������
private:
	//�����ӵ�
	void ShowBulletShop();

		
	DECLARE_MESSAGE_MAP()	
};

//////////////////////////////////////////////////////////////////////////

#endif