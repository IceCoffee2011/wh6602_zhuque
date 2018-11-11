#ifndef		___CREATE_DEF_GAME_CLIENT_1005_DEF__
#define		___CREATE_DEF_GAME_CLIENT_1005_DEF__


#include "ddzandroid\ddzlogic.h"
#include "gameclientframe.h"
#include "CMD_Land.h"

//��Ϸ�ͻ���
class CGameClient : public CGameClientFrame
{
public:
	CGameClient(void);
	virtual ~CGameClient(void);


public:
	//��ʼ����
	virtual bool InitGameFrame();
	//���ÿ��
	virtual void ResetGameFrame();

	//ϵͳ�¼�
public:
	//ʱ����Ϣ
	virtual bool OnTimerMessage(UINT nElapse, UINT nTimerID);
	//������Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	virtual bool OnGameSceneMessage(BYTE cbGameStatus, const void * pBuffer, WORD wDataSize);

public:
	//�����˿�
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//�зֹ���
	bool OnSubGameUserLandScore(const void * pBuffer, WORD wDataSize);
	//�з����
	bool OnSubJiaoFenFin(const void * pBuffer, WORD wDataSize);
	//��Ҽӱ����
	bool OnSubUserDoubleResult(const void * pBuffer, WORD wDataSize);
	//��Ϸ��ʼ����
	bool OnSubGameStartOutCard(const void * pBuffer, WORD wDataSize);
	//��ҳ��ƽ��
	bool OnSubGameUserOutCardResult(const void * pBuffer, WORD wDataSize);
	//��ҷ�������
	bool OnSubGameUserPassCard(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);

public:
	//����֮ǰ��ʼ��
	void InitSendCard();
	//�Զ�����
	void GameUserAutoOutCard();
	//�Զ������һ����
	void GameAutoLastOneCard();
	//�ӱ��ж�
	void SetGameUserJiaBei(WORD wBankerID, WORD wCurrentID, const BYTE *pCircuit, const bool *pIsDouble);
	//ȡ�����ж�ʱ��
	void CancelAllTimer();

private:
	//��Ϸ�߼�
	CDDZLogic				m_GameLogic;

private:
	WORD					m_bBankerUserID;				//����
	WORD					m_bCurrentUserID;				//��ǰ���
	bool					m_bIsNewTurn;					//��һ��
	BYTE					m_bHandCardCount;				//�˿���Ŀ
	BYTE					m_bHandCardData[20];			//�����˿�
	BYTE					m_bJiaBeiTag;					//�ӱ���־
	BYTE					m_cbCallScorePhase;				//���ƽ׶�
	bool					m_bMeCanOutCard;				//�Ƿ����
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT nIDEvent);
};




#endif