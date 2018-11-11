#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"
#include "GameScoreView.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameDlg
{
	//��Ϸ����
protected:
	WORD							m_wLandUser;						//�ڿ��û�
	WORD							m_wBombTime;						//ը������
	BYTE							m_bCardCount[3];					//�˿���Ŀ
	BYTE							m_bHandCardCount[3];				//�˿���Ŀ
	BYTE							m_bHandCardData[3][20];				//�����˿�
	bool							m_bAutoPlay ;						//�йܱ���

	//���Ʊ���
protected:
	BYTE							m_bTurnOutType;						//��������
	BYTE							m_bTurnCardCount;					//������Ŀ
	BYTE							m_bTurnCardData[20];				//�����б�

	//���ñ���
protected:
	bool							m_bDeasilOrder;						//����˳��
	DWORD							m_dwCardHSpace;						//�˿�����
	bool							m_bAllowLookon;						//�����Թ�

	//��������
protected:
	WORD							m_wMostUser;						//������
	WORD							m_wCurrentUser;						//��ǰ�û�
	WORD							m_wTimeOutCount;					//��ʱ����
	BYTE							m_cbSortType;						//��������
	bool                            m_bSystemTrustee;                   //ϵͳ�й�
	WORD                            m_wLastMostUser;                    //���ֳ������

	//��������
protected:
	BYTE							m_cbRemnantCardCount;				//ʣ����Ŀ
	BYTE							m_cbDispatchCardCount;				//�ɷ���Ŀ
	BYTE							m_cbDispatchCardData[20];			//�ɷ��˿�

	//������
protected:
	BYTE							m_cbCallScorePhase;					//���ƽ׶�

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
	CGameScoreView                  m_ScoreView;                        //������ͼ


	//����ͷ���
protected:
	bool                            m_IsSpring;                          //�Ƿ���
	bool                            m_IsFanSpring;                       //�Ƿ񷴴�
	int                             m_LargessCount[GAME_PLAYER];          //��ʯ����
	//�Ƿ��Ѿ�����
	bool                            m_bchuntian;
	bool                            m_bfanchun;
	bool                            m_bbaoshi;
	bool                            m_bjieshu;


	//����
protected:
	CString							m_sVernacular;						//����

private:
	tagOutCardResult				m_tagOldCard;
	tagOutCardResult				m_tagNewCard;
	//��������
public:
	//���캯��
	CGameClientDlg();
	//��������
	virtual ~CGameClientDlg();
	//��ȡ·��
    void GetPicPath(CString& strPath);

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

public:
	//�û�����
	virtual void __cdecl OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
	//�û��뿪
	virtual void __cdecl OnEventUserLeave(tagUserData * pUserData, WORD wChairID, bool bLookonUser);

public:
	//���������Ϸ
	virtual void OnEventContinueGame();

	//��Ϣ����
protected:
	//�����˿�
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//�û��е���
	bool OnSubLandScore(const void * pBuffer, WORD wDataSize);
	//�û�������
	bool onSubSnatchLand(const void *pBuffer,WORD wDataSize);
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//��������
	bool OnSubPassCard(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//����й�
	bool OnSubPlayerTrustee(const void * pBuffer, WORD wDataSize);

	//�����Ϣ
protected:
	//���ƿ�ʼ
	bool OnSubBrightStart(const void * pBuffer, WORD wDataSize);
	//�������
	bool OnSubBrightCard(const void * pBuffer, WORD wDataSize);
	//�ӱ���Ϣ
	bool OnSubDoubleScore(const void * pBuffer, WORD wDataSize);
	//��Ҽӱ�
	bool OnSubUserDouble(const void * pBuffer, WORD wDataSize);

	//��������
protected:
	//�����ж�
	bool VerdictOutCard();
	//�Զ�����
	bool AutomatismOutCard();
	//ֹͣ����
	bool DoncludeDispatchCard();
	//�ɷ��˿�
	bool DispatchUserCard(BYTE cbCardData[], BYTE cbCardCount);

	//����
protected:
	//��������
	void PlayGameSoundLanguage(BYTE cbGender, CString strSound,bool bSoundType);//����(1:�Ա�;2:����;3:��������)
   // void PlaySound(CString szSoundRes, bool bLoop);
	void PlayMusic(CString szSoundRes);
public:
	void PlaySound(CString szSoundRes, bool bLoop);
	//��Ϣӳ��
protected:
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//��ʼ��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnOutCard(WPARAM wParam, LPARAM lParam);
	//��������
	LRESULT OnPassCard(WPARAM wParam, LPARAM lParam);
	//�е�����Ϣ
	LRESULT OnCallLand(WPARAM wParam, LPARAM lParam);
	//��������Ϣ
	LRESULT OnSnatchLand(WPARAM wParem,LPARAM lParam);
	//������ʾ
	LRESULT OnAutoOutCard(WPARAM wParam, LPARAM lParam);
	//������˿�
	LRESULT OnLeftHitCard(WPARAM wParam, LPARAM lParam);
	//�Ҽ��˿�
	LRESULT OnRightHitCard(WPARAM wParam, LPARAM lParam);
	//�й���Ϣ
	LRESULT OnAutoPlay(WPARAM wParam, LPARAM lParam);
	//�����˿�
	LRESULT OnMessageSortCard(WPARAM wParam, LPARAM lParam);
	
	//�����Ϣ
protected:
	//���ƿ�ʼ
	LRESULT OnBrightStart(WPARAM wParam, LPARAM lParam);
	//�ӱ���Ϣ
	LRESULT OnDoubleScore(WPARAM wParam, LPARAM lParam);
		
	DECLARE_MESSAGE_MAP()

private:
	//�����PASS��ť,ֱ�ӳ��ơ�
	void NoHitPassOutCard();

	//ͳ������
	int GetCurGameUserNums();
};

//////////////////////////////////////////////////////////////////////////
