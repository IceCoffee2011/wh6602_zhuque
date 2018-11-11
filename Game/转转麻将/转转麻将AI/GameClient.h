#ifndef		___CREATE_DEF_GAME_CLIENT_1005_DEF__
#define		___CREATE_DEF_GAME_CLIENT_1005_DEF__


#include "gameclientframe.h"
#include "CMD_Sparrow.h"
#include "GameLogic.h"
#include "..\..\������\Include\cnd_ILogMgr.h"

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
	bool					m_bHearStatus;								//����״̬
	bool					m_bIsMeCanOutCard;							//�Ƿ��Լ����Գ���

private:
	BYTE					m_byGangCard[4];							//����
	BYTE					m_byGangCardNums;							//����
	BYTE					m_byBuCard[4];								//����
	BYTE					m_byBuCardNums;								//����

	//�˿˱���
protected:
	BYTE					m_cbCardIndex[MAX_INDEX];					//�����˿�
	//BYTE                    m_TempKing;                                 //��ʱ����
	// add by Ivan_han 2011-12-28
	cnd_ILogMgr*	m_pRoomLog;	// ������־
public:
	// ��ʼ��һЩ����
	void InitSome(DWORD dwWndID);

	// �ͷ�һЩ����
	void UninitSome();

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
	//��ʼ��
	void InitData();
	//����֮ǰ��ʼ��
	void InitSendCard();
	//ȡ�����ж�ʱ��
	void CancelAllTimer();

	//��Ϣ����
protected:
	//�Ƿ�ʼ���Ӷ���
	bool OnSubStartSZFlash();
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	////���ײ���
	//bool OnSubOperateHaiDi(const void * pBuffer, WORD wDataSize);
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
	//����
	bool OnSubTingCard(const void * pBuffer, WORD wDataSize);
	//���ƽ��
	bool OnSubTingCardResult(const void * pBuffer, WORD wDataSize);

	//������
protected:
	//����
	void OnOutCard();
	//�û�������
	void OnUserOperateCard();
	//�˿˲���
	void OnCardOperate(BYTE byOperaType, BYTE byCardData);
	//�Ƿ����Ʋ�����
	bool bIsTingCardOpera();
	//�ж��Ƿ�������������
	bool bIsHaveCard(BYTE byCard, BYTE& byLastCard);

	//��������
protected:
	//��������
	void SearchOutCard(BYTE& byOutCard);

public:
	DECLARE_MESSAGE_MAP()
};
#endif