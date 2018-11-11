#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "cnd_ILogMgr.h"
#include <vector>
using namespace std ;

//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
private:
	cnd_ILogMgr*	m_pRoomLog;	// ��־�����ָ��

public:
	// GameSerivce.dll ͨ����Ϸ����˵�ͨ�������Դ����κμ������ݻ��Աָ��
	virtual bool channel_to_game_service(int iMainCode, int iAssistantCode, void* pData, int iDataLen);

	//��Ϸ����
protected:
	WORD							m_wBombTime;						//ը������
	WORD							m_wBombNums;						//ը������
	BYTE							m_bLandScore;						//��������
	WORD							m_wFirstUser;						//�׽��û�
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ���
	BYTE							m_bScoreInfo[GAME_PLAYER];			//�з���Ϣ
	int                             m_bUserBeiShu[GAME_PLAYER];         //��ұ���
	bool                            m_IsMissile;                        //�Ƿ��ǻ��
	WORD                            m_OneBombNums[GAME_PLAYER];         //һ���˴����ը��
	WORD							m_wOperateUser;						//�����û�		Author<cxf>

	//������Ϣ
protected:
	WORD							m_wTurnWiner;						//ʤ�����
	BYTE							m_bTurnCardCount;					//������Ŀ
	BYTE							m_bTurnCardData[20];				//�����б�
	bool							m_bUserTrustee[GAME_PLAYER];		//����й�
	BYTE							m_bCircuit[GAME_PLAYER];			//�û����ƴ���
	int                             m_tagTask[GAME_PLAYER];             //�����ʶλ

	//������
protected:
	bool							m_bBrightCard;						//����ģʽ
	BYTE							m_bCallScorePhase;					//���ƽ׶�
	bool							m_bGiveUpBanker[GAME_PLAYER];		//������ʶ
	bool							m_bBrightStart[GAME_PLAYER];		//���ƿ�ʼ
	BYTE							m_cbBrightCardTime;					//���Ʊ���
	BYTE							m_cbDoubleCount;					//�ӱ�ͳ��
	BYTE							m_cbBrightCardCount;				//����ͳ��
	bool							m_bFinishDoubleUser[GAME_PLAYER];	//��ɼӱ�
	bool							m_bDoubleUser[GAME_PLAYER];			//�ӱ����
	bool							m_bAllowDoubleScore;				//����ӱ�
	bool							m_bUserBrightCard[GAME_PLAYER];		//�������
	BYTE							m_bGiveUpSnatch[GAME_PLAYER];		//��������ʶ<0:û�в���; 1:����; 2:������>

	//�˿���Ϣ
protected:
	BYTE							m_bBackCard[3];						//��Ϸ����
	BYTE							m_bCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							m_bOutCardCount[GAME_PLAYER];		//���ƴ���
	BYTE							m_bHandCardData[GAME_PLAYER][20];	//�����˿�
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	BYTE                            m_bSnatchCount;                     //����������

	
	//���������Ϣ,add by Ivan_han
protected:
	vector<WORD>                    m_vecChairIDOffLine;		            //���������ҵ����Ӻ�

	//�������
protected:
	ITableFrame						* m_pITableFrame;					//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;				//���ò���

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;						//��Ϸ����
	static const enStartMode		m_GameStartMode;					//��ʼģʽ

	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID __cdecl Release() { }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��ʼ��
	virtual bool __cdecl InitTableFrameSink(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual void __cdecl RepositTableFrameSink();

	//��Ϣ�ӿ�
public:
	//��ʼģʽ
	virtual enStartMode __cdecl GetGameStartMode();
	//��Ϸ״̬
	virtual bool __cdecl IsUserPlaying(WORD wChairID);

	//�����¼�
public:
	//�û�����
	virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem);// { return true; }
	//�û�����
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID, IServerUserItem * pIServerUserItem);// { return true; }
	//�û�����
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser) { return true; }
	//�û�����
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser) ;
	//�û�ͬ��
	virtual bool __cdecl OnActionUserReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool __cdecl OnEventGameStart();

	//��Ϸ�������(bLandWin=true��ʾ����Ӯ��,bLandWin=false��ʾũ��Ӯ��),add by Ivan_han
	bool __cdecl OnEventGameNormalEnd(WORD wChairID,bool bLandWin/*, IServerUserItem * pIServerUserItem, BYTE cbReason*/);
	//��Ϸ����
	virtual bool __cdecl OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool __cdecl SendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool __cdecl OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool __cdecl OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//��Ϸ�¼�
protected:
	//�з��¼�
	bool OnUserLandScore(WORD wChairID, BYTE bLandScore);		//Author<cxf>
	//�û�����
	bool OnUserOutCard(WORD wChairID, BYTE bCardData[], BYTE bCardCount);
	//�û�����
	bool OnUserPassCard(WORD wChairID);

	//add by Ivan_han
	//���������������ҳ��ƻ򲻳���е�����������
	bool OnAutoPassOrOutCard();

	//����й�
	bool OnUserTrustee(WORD wChairID, bool bTrustee);
	//�������¼�
	bool OnUserSnatchLand(WORD wChairID, BYTE bLandScore);		//Author<cxf>

	//����¼�
protected:
	//���ƿ�ʼ
	bool OnBrightStart( WORD wChairID, bool bBright );
	//�������
	bool  OnBrightCard( WORD wChairID, BYTE cbBrightCardTime);
	//��Ҽӱ�
	bool OnUserDoubleScore( WORD wChairID, bool bDoubleScore, WORD bDoubleType);
	//������Ϣ
	void SendGameMessage( WORD wChairID, CHAR szMsg[] );

    //���ù̶��ƻ�����ƻ����
	int SetFixOrRandCardOrGoodCard();
	//�����
	void SetRandCard();
	//����ֻ����һ����
	void SetGoodCard1();
	////����ֻ����������
	//void SetGoodCard2();

	//OSC add
	//�����ж�
protected:
	int OnUserTaskJudgement(WORD wChairID, WORD wBandID);
};

//////////////////////////////////////////////////////////////////////////

#endif
