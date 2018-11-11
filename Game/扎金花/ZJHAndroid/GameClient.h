#ifndef		___CREATE_DEF_GAME_CLIENT_1005_DEF__
#define		___CREATE_DEF_GAME_CLIENT_1005_DEF__

#include "CMD_ZaJinHua.h"
#include "GameLogic.h"
#include "gameclientframe.h"
#include <vector>

using namespace std;

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
	//��Ϸ��ʼ
	bool OnSubGameStart(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubGiveUp(const void * pBuffer, WORD wDataSize);
	//�û���ע
	bool OnSubAddScore(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubLookCard(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubCompareCard(const void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSubOpenCard(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);

private:
	//����֮ǰ��ʼ��
	void InitGameData();
	//ȡ�����ж�ʱ��
	void CancelAllTimer();
	//��һ����ע
	void FirstXZ(bool bIsFirst=true);
	//��Ҳ���
	void MeOperator();
	//�Ƿ���Կ���
	bool IsOpenCard();
	//���Ʋ���
	void UserCompareCard();
	//���ƺ�Ĳ���
	void LookCardLaterOpera();
	//��ȡ������������
	int GetCardMaxPoint();
	//��ע
	void FollowCard();
	//�Զ��ж�
	void AutoJudge();

private:
	//��Ϸ�߼�
	CGameLogic						m_GameLogic;

private:
	//��ǰ�û�
	WORD							m_wCurrentUser;
	//ׯ���û�
	WORD							m_wBankerUser;
	//����Ӯ��
	WORD							m_wWinnerUser;				
	//��Ҹ���
	WORD							m_wPlayerNums;
	//�ⶥ��Ŀ
	LONGLONG						m_lMaxScore;
	//��Ԫ����
	LONGLONG						m_lMaxCellScore;
	//��Ԫ��ע
	LONGLONG						m_lCellScore;	
	//��ǰ����
	LONGLONG						m_lCurrentTimes;
	//������
	LONGLONG						m_lUserMaxScore;
	//��ע����
	WORD							m_wUserXZCount[GAME_PLAYER];
	//�����עֵ
	LONGLONG						m_lTableScore[GAME_PLAYER];
	//�Ƿ���
	bool							m_bIsLookCard[GAME_PLAYER];
	//�Ƿ���Ч���
	bool							m_cbPlayStatus[GAME_PLAYER];
	//�������
	BYTE							m_byUserCard[MAX_COUNT];
	//���Ƶ�����б�
	std::vector<int>				m_vecLookCardUserList;
	//���ƻ��������б�
	std::vector<int>				m_vecLostUserList;
	std::vector<int>				m_vecWinUserList;
	std::vector<int>				m_vecCancelUserList;
	//������ע����
	WORD							m_wContinuouFollowNums;
	//�����Ժ󲻲���
	bool							m_bIsLookCardLaterNoOperation;
	//��������
	bool							m_bIsFollowToOpenCard;
	//����Ƿ��ж���
	bool							m_bIsUserAction;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT nIDEvent);
};




#endif