#ifndef __GAMELOGIC_H__
#define __GAMELOGIC_H__

#include <vector>
#include <list>
#include <map>
#include "DG4.h"
#include "../XY/CMD_DG4.h"
#include "TableFrameSink.h"

namespace DG4
{
//#define INVALID_CHAIR 0xFFFF

// ��Ϸ״̬
typedef enum tagGameState
{
	gsNull = 0,
	gsRoarPai,	// ����
	gsPlay,		// ����
}GameState;

class CGameLogic : public CTableFrameSink
{
	//////////////////////////////////////////////////////////////////////////
	// ����
public:
	CGameLogic(void);
	virtual ~CGameLogic(void);

	//////////////////////////////////////////////////////////////////////////
	// ��Ϸ�ӿ�
	//��ʱ���¼�
	virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam);

	//����ж���Ϸ����
	virtual bool __cdecl OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, unsigned char cbReason);

	virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, unsigned char cbGameStatus, bool bSendSecret);
	
	//�û�����
	virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem); 
	//�û�����
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem);

public:								
	// ��Ϸ��ʼ
	void OnStartGame();	
	void OnGameAction(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize);
	

	// �������״̬������,�˺�����������á�
	void Reset();
	void InitGame();
	long GetCPUTickCount( void );

public:
	/*һЩֽ����Ϸ���еķ���������*/
	enum ActionStep		// ��Ϸ����
	{//�޸Ĳ���
		AS_NULL = 0,
		AS_ROAR,		// Ӳ��
		AS_ENDROAR,		// ����Ӳ��
		AS_PLAY,		// ����
		AS_ENDPLAY,		// ���ƽ���
		AS_ENDGAME,		// ������Ϸ
		AS_COUNT
	};
	enum TGameType
	{
		GT_NULL,
		GT_NORMAL,		//��ͨģʽ��һ��Ϊ�����ѣ�����2vs2,���Ϊȡ����
		GT_ROAR,		//����ģʽ��һ��Ϊ1vs3,���Ϊ������
	};
	enum GameOverType
	{
		GOT_NULL = 0,
		GOT_ESCAPE,		//�������ܣ���Ϸ����
		GOT_NORMAL,		//�����Ľ�����Ϸ�������������ѵ�ģʽ�£��������꣬�ں���ģʽ�£�һ������
		GOT_ZHONGTU,    //��;������Ϸ��������һ������������������Ϸ
		GOT_DOUBLEKILL,	//���󡱣���Ϊ˫ɱ���������ѵ�ģʽ�£�һ��Ϊһ�κͶ�������
	};

	std::string m_helpUrl;				// ������ַ
	std::string m_purchaseUrl;			// ��ֵ��ַ
	std::string m_shopUrl;				// �̳���ַ
	std::string m_ADUrl;				// �����ַ

	GameState	m_GameState;			// ��Ϸ״̬
	int			m_tDownTime;			// Ȩ��ֹͣʱ��
	int			m_tPowerStartTime;		// Ȩ�޿�ʼʱ��
	ActionStep	m_theActionStep;		// ����ʱ
	ActionStep	m_nowActionStep;	

	//һЩ���е���Ϸ����

	unsigned char		m_whoplay;              // ��ǰ���	1--MAXPLAYER
	unsigned char		m_whoplayout;			// ��һ�����
	unsigned char		m_banker;				// ׯ��
	unsigned char		m_nextbanker;			// ��һ��ׯ��

	unsigned char		m_whoMakeCards;			//˭����			

	//�й�����
	unsigned char        m_TuoGuanPlayer[MAX_PLAYER];	  // ˭�й��ˣ�
	unsigned char		m_TrustCounts[MAX_PLAYER];		  // �йܴ���

	//��������
	unsigned char        m_whoBreak[MAX_PLAYER];			  // ˭�����ˣ�
	int			m_WireBreakCounts[MAX_PLAYER];	  // ���ߴ���

	//��������
	int			m_EscapePlayerNum;			      // ���ܵ������
	BOOL		m_isPlayerEscape[MAX_PLAYER];     // ˭�����ˣ�
	long		GameWinLostSR[MAX_PLAYER];	      // ���������Ӯ
	long		GameWinLostJF[MAX_PLAYER];	      // ���������Ӯ

	//������
	unsigned char		m_tempdbp[ALLCARD];				  // ������ ��������
	unsigned char		m_dbp[ALLCARD];					  // ������
	unsigned char		m_dbplay[MAX_PLAYER][MAXCARD];    // �ֵ�����

	unsigned char        m_allPaiOut[MAX_PLAYER][MAXCARD]; // ������
	unsigned char        m_lastPaiOut[MAX_PLAYER][MAXCARD];// ��һ�ֳ�����
	unsigned char		m_thePaiCount[MAX_PLAYER];		  // ����

	//���ӣ���������
	int			m_lastScore[MAX_PLAYER];		  // ��һ����Ӯ
	int			m_total[MAX_PLAYER];			  // ����Ӯ
	long		m_playerrich[MAX_PLAYER];		  // ��ҵĲƸ�

	//һЩ���еĹ���
	int			m_nAutoOutTime;					// �Զ�����ʱ�䣬�йܵ�ʱ�����ʱ��
	int			m_nTimeStart;					// ����֮�󣬰���ʼ�ĵȴ�ʱ��
	int			m_nPlayTime;					// ����ʱ��
	int			m_nSerPay;						// ��ˮ��ǧ�ֱ�
	int			m_nVipSerPay;					// vip��ˮ��ǧ�ֱ�

	int			m_nIsSysTrust;					// �Ƿ����й���Ϸ
	int			m_nWireBreakTimes;				// ������������Ĵ���
	int			m_nTrustCounts;					// �����йܵĴ���

	int			m_nConditionEnter;				// �Ƿ��н��뷿�������
	int			m_nEnter;						// ���뷿�������
	int			m_nConditionSit;				// �Ƿ�����С������
	int			m_nPlayerSit;					// ���µ�����

	time_t	    nowTime; 

	// ���ö���������ָ������
	void		SetActionStep(ActionStep as,unsigned long nSecond=0);  

	// ��������ָ������,
	void		OnActionStep(ActionStep as);
public://�Լ���ӵ�
	long		m_nBase;					// ��
	long		m_nNeedMinrich;				// ��Ҫ������ٵĲƸ�
	int			m_isSpecial;				// �Ƿ�Ϊ����
	int			m_theGift;					// �Ƿ������콱
	std::string	m_szGiftUrl;				// �콱��ַ
	std::string m_szGiftCall;				// �콱�绰
	std::string m_szDURL;					// �ײ����

	int			m_nRoarTime;				// ����ʱ��
	CDG4		m_dagun;

	bool		m_bMingJi;					// �Ƿ��Ѿ���ʾ������
	TGameType	m_GameType;					// ��Ϸ������
	int			m_score;					// ÿһ���Ƶķ�
	unsigned char			m_turn;						// ����
	unsigned char		m_RoarPai;					// �е���
	unsigned char		m_WhoRoar;					// ˭�����ˣ�
	unsigned char		m_parter;					// ׯ�ҵĶԼ�
	int			m_playerscore[MAX_PLAYER];	// ÿ�������ķ������п���û�����Ҳ�ӽ�ȥ��

	long		m_spay[MAX_PLAYER];			// �����

	unsigned char		m_whoPass[MAX_PLAYER];			// ˭������
	BOOL		m_playerWin[MAX_PLAYER];		// ˭�����ˣ�
	BOOL		m_StartMakePai;					// ���ƹ��ܿ���

	unsigned char		m_bTeamOut[MAX_PLAYER];			// �ҵĶԼ��Ƿ������ˣ�
	unsigned char        m_whoAllOutted[MAX_PLAYER];     // ˭�����ˣ�
	unsigned char			m_playerTurn[MAX_PLAYER];
	bool		m_whoReady[MAX_PLAYER];			// ˭�Ѿ�����˺��ƹ���
	int			m_who8Xi[MAX_PLAYER];			// ÿ���˵�8ϲ����
	int			m_who7Xi[MAX_PLAYER];			// ÿ���˵�7ϲ����
	int			m_whoHasKingBomb;				// ˭����ը
	int			m_lastOutType;					// �����Ƶ�����


	int			m_iFaOfTao;				//���ܵĳͷ�����
	int			m_iJiangOfTao;			//���ܵĲ�������


	int			m_iRandomTask;						//�������,�±�
	vector<int>	m_vecAllTaskID;	//���е�����ID
	map<int,int>	m_mapAllTask;	//��������: ID-������
	vector<int>	m_vecFinishedTask[MAX_PLAYER];		//��ɵ�����
	bool		m_bHaveOutCard;		//�Ƿ��Ѿ�������
	int			m_specialAward; 				// �������ͽ�������
public:

	unsigned char		GetTeamer(unsigned char who);			// who = 1-4
	int			GetTeamScore(unsigned char seat);		// �õ�����ȷ���ķ���

	void		StartRoar(unsigned char theSeat);		// ��ʼ����
	void		EndRoar(bool bRoar);			// ��������

	void GoNextPlayer();
	void StartPlay(unsigned char whoplay);		// ��ʼ��
	int AutoOutCard(unsigned char theSeat,const DG4::CMD_OutCard_CB& msgout);	//���ƶ���

	unsigned char		GetJiaoPai();														// �õ�����
	void		GameOver(unsigned char winner,GameOverType type);
	bool		JudgeEndGame(unsigned char who);
	void		OnRoarAction(unsigned char seat,unsigned char bRoar);									// ����Ӳ�ƶ���

	//�޸�����ĺ�����ʵ���Զ��й�
	int			AutoTuoGuan(unsigned char theSeat);

	// ����һ��
	bool EndTurn();
	void EndOut();
	void AddSpecailScore(long Score[MAX_PLAYER],int seat,int base);
	void GetFinalWinLoseScore( long score[MAX_PLAYER] );
	void SendPower(unsigned char whoplay,DWORD aPower,DWORD dwWaitTime = 0);	// ����Ȩ��
	void SendPlaySoundMsg(unsigned char seat,unsigned char type);						// ���Ͳ������ֵ���Ϣ

	//int MsgTuoGuan();												//�й�

	void SendTurnScore(int wChairID = INVALID_CHAIR);				// ����ÿ�ֵķ���
	void SendPlayerScore(int wChairID = INVALID_CHAIR);				// ����ÿ���˵ķ���
	void SendPaiCount(int wChairID = INVALID_CHAIR);				// ��������
	void SendPlayerTurn(int wChairID = INVALID_CHAIR);				// ������ҽ���˳��
	void SendPaiToTeamer(unsigned char seat1,unsigned char seat2);					// ��seat1���Ʒ��͸�seat2��ʾ����
	void sendGameStatus(WORD wChair ,bool isReLink);

	BOOL IsGamePlaying();


	//void stovs( const string& s, const string& sp, vector<string>& sa );
	void ParseTaskConfig(const string& strTaskID);	//�������õ�����
	void SendTaskID (bool bFirst, unsigned char nSeat = -1); //�����������
	void IsTaskFinished(const TCardType& re1, int iSeat, int iNumOfKing);  //�ж������Ƿ����
	void IsTaskFinishedOfLastHand(const TCardType& re1, int iSeat, int iNumOfKing);  //�ж����һ�ֵ������Ƿ����
	void SendFinishTask (int iSeat, int id, int cnt = 1, bool bToAll = true); //��������˵�����
	int GetTaskAward(int iSeat); //��ȡ������
	string GetTaskAwardDetails(int iSeat);	//��ȡ������ϸ
};

}; //end of namespace
#endif//__GAMELOGIC_H__

