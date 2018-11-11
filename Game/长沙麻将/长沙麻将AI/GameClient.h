#ifndef		___CREATE_DEF_GAME_CLIENT_1005_DEF__
#define		___CREATE_DEF_GAME_CLIENT_1005_DEF__

#include "..\..\������\Include\gameclientframe.h"
#include "CMD_Sparrow.h"
#include "GameLogic.h"

//���ƽ��
struct tagOutCardResult
{
	BYTE							cbOperateCode;						//���ƴ���
	BYTE							cbOperateCard;						//����ֵ
};

//���Ʒ������
struct tagTingCardResult
{
	BYTE bAbandonCount;													//�ɶ�������
	BYTE bAbandonCard[MAX_COUNT];										//�ɶ�����
	BYTE bTingCardCount[MAX_COUNT];										//������Ŀ
	BYTE bTingCard[MAX_COUNT][MAX_COUNT-1];								//����
	BYTE bRemainCount[MAX_COUNT];										//�����Ƶ�ʣ����
};

//��Ϸ�ͻ���
class CGameClient : public CGameClientFrame
{
public:
	CGameClient(void);
	virtual ~CGameClient(void);

private:
	//��Ϸ�߼�
	CGameLogic				m_GameLogic;

private:
	WORD					m_wCurrentUser;								//��ǰ���
	WORD					m_wBankerUser;								//ׯ��
	BYTE					m_cbActionMask;								//������ʶ
	BYTE					m_cbActionCard;								//������

	//������Ϣ
protected:
	WORD					m_wOutCardUser;								//�����û�
	BYTE					m_cbOutCardData;							//�����˿�
	BYTE					m_cbDiscardCount[GAME_PLAYER];				//������Ŀ
	BYTE					m_cbDiscardCard[GAME_PLAYER][60];			//������¼

	//�˿˱���
protected:
	BYTE					m_cbCardIndex[GAME_PLAYER][MAX_INDEX];		//�����˿�
	BYTE					m_cbLeftCardCount;							//ʣ����Ŀ

	//����˿�
protected:
	BYTE					m_cbWeaveCount[GAME_PLAYER];				//�����Ŀ
	tagWeaveItem			m_WeaveItemArray[GAME_PLAYER][MAX_WEAVE];	//����˿�

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
	//����֮ǰ��ʼ��
	void InitSendCard();
	//ȡ�����ж�ʱ��
	void CancelAllTimer();

	//��Ϣ����
protected:
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//���ײ���
	bool OnSubOperateHaiDi(const void * pBuffer, WORD wDataSize);
	//��Ϸ��ʼ����
	bool OnSubGameStartOutCard(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubOutCard(const void * pBuffer, WORD wDataSize);
	//������ʾ
	bool OnSubOperateNotify(const void * pBuffer, WORD wDataSize);
	//������Ϣ
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//�������
	bool OnSubOperateResult(const void * pBuffer, WORD wDataSize);

	//������
protected:
	//����
	void OnOutCard( BYTE cbOutCard );
	//������
	void OnOperateCard( BYTE cbOperateCode,BYTE cbOperateCard );

	//��������
protected:
	//��������
	bool SearchOutCard( tagOutCardResult &OutCardResult );

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT nIDEvent);
};
#endif