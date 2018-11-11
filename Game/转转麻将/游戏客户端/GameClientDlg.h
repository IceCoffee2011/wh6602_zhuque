#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"
#include "GameScoreWnd.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//�û�����
protected:
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ�û�

	//�йܱ���
protected:
	bool							m_bStustee;             			//�йܱ�־
	WORD							m_wTimeOutCount;					//��ʱ����

	//״̬����
protected:
	//bool							m_bHearStatus;						//����״̬
	bool                            m_bZM;                              //�Ƿ����� 
	bool                            m_bFP;                              //�Ƿ����
	bool							m_bIsMeCanOutCard;					//�Ƿ��Լ����Գ���

	//������Ϣ
protected:
	BYTE							m_byUserAction[4];					//�û�����

	//������Ϣ
protected:
	BYTE							m_cbGender;					         //�û��Ա�
	CString                         m_InfoSound;                         //������Ϣ


	//��������
protected:
	WORD							m_wHeapHead;						//����ͷ��
	WORD							m_wHeapTail;						//����β��
	BYTE							m_cbHeapCardInfo[4][2];				//������Ϣ

	//�˿˱���
protected:
	BYTE							m_cbLeftCardCount;					//ʣ����Ŀ
	BYTE							m_cbCardIndex[MAX_INDEX];			//�����˿�
	//BYTE                            m_cbTempKing[4];                    //��ʱ����

	//����˿�
protected:
	BYTE							m_cbWeaveCount[GAME_PLAYER];		//�����Ŀ
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][4];	//����˿�

	//������Ϣ
protected:
	WORD							m_wOutCardUser;						//�����û�
	BYTE							m_cbOutCardData;					//�����˿�
	BYTE							m_cbDiscardCount[GAME_PLAYER];		//������Ŀ
	BYTE							m_cbDiscardCard[GAME_PLAYER][55];	//������¼

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
	CGameScoreWnd				    m_ScoreView;						//������ͼ
   
	tagGameScoreInfo                m_GameScoreInfo;                      //��������


	//
	CMD_S_GameStart					m_GameStartData;


	//�ؼ�����
public:
	//CGameFlash      				m_TingCardFlash;						//������ͼ
	

public:
	BYTE							m_byCanSelectCard[4];				//��ѡ���������
	BYTE							m_byCanSelectCount;					//��ѡ�������Ŀ
	BYTE							m_byCanSelectType;					//���ͣ��ܣ���

	//����
protected:
	CString							m_sDialect;						   //����
	
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

	//�û��¼�
public:
	//�û�����
	virtual void __cdecl OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
	//�û��뿪
	virtual void __cdecl OnEventUserLeave(tagUserData * pUserData, WORD wChairID, bool bLookonUser);

public:
	//���������Ϸ
	virtual void OnEventContinueGame();

	//����������
	void CheckCanHuPai(BYTE byCardData, int iX);

	//��Ϣ����
protected:
	bool OnSubStartSZFlash();
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//������Ϣ
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//������ʾ
	bool OnSubOperateNotify(const void * pBuffer, WORD wDataSize);
	//�������
	bool OnSubOperateResult(const void * pBuffer, WORD wDataSize);
	////���ײ���
	//bool OnSubOperateHaiDi(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//�û��й�
	bool OnSubTrustee(const void * pBuffer,WORD wDataSize);
	//����
	//bool OnSubTingCard(const void * pBuffer, WORD wDataSize);
	//���ƽ��
	//bool OnSubTingCardResult(const void * pBuffer, WORD wDataSize);

private:
	//������һ�ֵ�������
	void LastTurnOperateCard();

	//��������
public:
	//���ű�������
	void PlayMusic(CString szSoundRes);
	//��������
	void PlaySound(CString szSoundRes, bool bLoop);
	//��������
	void PlayCardSound(BYTE cbGender, BYTE cbCardData);
	//��������
	void PlayActionSound(BYTE cbGender, BYTE cbAction);
	//�����������
	void PlayGameSoundRand(BYTE cbGender, CString strSound, bool bSoundType);
	//��������
	/// [@param bSoundType in] ����������2��ѭ������2��ֻ��һ����0
	void PlayGameSoundLanguage(BYTE cbGender, CString strSound, BYTE bSoundType=1);//����(1:�Ա�;2:����;3���Ƿ���˫��)
	//�۳��˿�
	void DeductionTableCard(bool bHeadCard);
	//��ʾ����
	bool ShowOperateControl(BYTE cbUserAction,BYTE cbStartData);
	//������ж�ʱ��
	void ClearAllTimer();
	//��������
	void SendCardCommand(BYTE byOperatorCode, BYTE byOperatorCard);

	//��Ϣӳ��
protected:
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//����ɸ�Ӱ�ť
	LRESULT OnSZStart(WPARAM wParam, LPARAM lParam);
	//��ʼ��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnMoPai(WPARAM wParam, LPARAM lParam);
	//���Ʋ���
	LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
	//�˿˲���
	LRESULT OnCardOperate(WPARAM wParam, LPARAM lParam);
	////���ײ���
	//LRESULT OnOperateHaiDi(WPARAM wParam, LPARAM lParam);
	//�Ϲܿ���
	LRESULT OnStusteeControl(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnShowSZ(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnTingCard(WPARAM wParam, LPARAM lParam);
	//ɫ�Ӷ�����ť
	LRESULT OnSZFlashBT(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	//��õ�ǰ��Ϸ��Ҹ���
	int GetCurGameUserNums();
};

//////////////////////////////////////////////////////////////////////////
