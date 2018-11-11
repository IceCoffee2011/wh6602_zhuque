#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include <afxtempl.h>
#include "cnd_ILogMgr.h"
//////////////////////////////////////////////////////////////////////////

//�켣��Ϣ
struct tagServerFishTrace
{
	//�ؼ�����
	int								nInitX[5];								//�ؼ�����
	int								nInitY[5];								//�ؼ�����
	int								nInitCount;								//������Ŀ

	//��Ⱥ����
	enFishKind						FishKind;								//��Ⱥ����
	DWORD							dwFishID;								//�����ʶ

	//�켣����
	//POINT							ptTrace[500];							//�켣����
	//int							nTracePtCount;							//�켣��Ŀ
	DWORD							dwBuildTime;							//����ʱ��
};

//�ӵ���Ϣ
struct tagBulletInfo
{
	BYTE							byBulletCount;							//�ӵ���Ŀ
	DWORD							dwShootTime;							//����ʱ��
	BYTE							byCptrFishCount;						//������Ŀ
	enFishKind						FishKind;								//������
};


//��Ϸ����
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
private:
	cnd_ILogMgr*	m_pRoomLog;	// ��־�����ָ��

public:
	// GameSerivce.dll ͨ����Ϸ����˵�ͨ�������Դ����κμ������ݻ��Աָ��
	virtual bool channel_to_game_service(int iMainCode, int iAssistantCode, void* pData, int iDataLen);

	//���ñ���
private:
	LONG							m_lBulletCharge[enBulletCountKind_End];	//�ӵ�����
	BYTE							m_byCptrProbability[5][enFishKind_No];	//�������
	LONG							m_lUserCellScore[GAME_PLAYER];			//��Ԫ����
	LONG							m_lCellScore[CELL_SCORE_COUNT];			//��Ԫ����

	//��Ϸ����
protected:
	LONG							m_lBulletCount[GAME_PLAYER][enBulletCountKind_End];	//�ӵ���Ŀ
	LONG							m_lCoinCount[GAME_PLAYER];				//�����Ŀ
	LONG							m_lWinCoinCount[GAME_PLAYER];			//�����Ŀ
	LONG							m_lUserShootCount[GAME_PLAYER];			//�����Ŀ
	DWORD							m_dwFishID;								//��Ⱥ��ʶ
	WORD							m_wCptrFishCount[GAME_PLAYER][enFishKind_No];//��Ⱥ��Ŀ
	enSeaSceneKind					m_CureSeaSceneKind;						//��ǰ����
	bool							m_bWaveLToR;							//�ı䷽��
	DWORD							m_dwSharkAppearTime;					//����ʱ��
	DWORD							m_dwGoldSharkAppearTime;				//����ʱ��
	DWORD							m_dwPlayTime[GAME_PLAYER];				//��Ϸʱ��
	CMap<DWORD,DWORD,tagBulletInfo *,tagBulletInfo *>	m_ShootBullet[GAME_PLAYER];//�ӵ���Ϣ
	CFactoryTemplate<tagServerFishTrace>	m_ServerFishTrace;				//�켣����
	CFactoryTemplate<tagBulletInfo>			m_ShootBulletFactory;			//�ӵ�����

	//�ʽ����
protected:
	LONG							m_lBonusOrigin;							//��ʼ��Ŀ
	LONG							m_lBonusMultipleMax;					//�����
	LONG							m_lBonusMultipleUser[GAME_PLAYER];		//��ǰ����
	LONG							m_lBonusMultiple;						//��Ԫ��Ŀ
	LONG							m_lBulletLimit;							//�ӵ�����
	LONG							m_lBonusUnitTime;						//��Ԫʱ��
	LONG							m_lBonusAccumulate[GAME_PLAYER];		//��������
	LONG							m_lShootBulletCount[GAME_PLAYER];		//�����Ŀ

	//���Ʋ���
protected:
	bool							m_bControlScore;						//���ƻ���
	BYTE							m_byWindMulLimitBonus;					//�÷�����
	BYTE							m_byWindMulLimitMax;					//�÷�����
	BYTE							m_byWindMulLimitMin;					//�÷�����
	BYTE							m_byCellScoreRectify[CELL_SCORE_COUNT];	//��Ԫ����
	bool							m_bTraceGameInfo;						//�����Ϣ
	LONG							m_lTraceScoreLevel;						//�������

	//������Ⱥ
protected:
	BYTE							m_byRegFishColCount;					//��������
	int								m_nRegularSign;							//������
	BYTE							m_byRegularForm;						//������ʽ

	//ϵͳ��Ϣ
protected:
	TCHAR							m_szGameRoomName[SERVER_LEN];			//��������
	TCHAR							m_szIniFileName[MAX_PATH];				//�����ļ�
	BYTE							m_bySysIndex;							//��Ϣ����

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�

	//����ӿ�
protected:
	ITableFrame	*					m_pITableFrame;							//��ܽӿ�
	ITableFrameControl				* m_pITableFrameControl;				//��ܽӿ�
	const tagGameServiceOption		* m_pGameServiceOption;					//���ò���

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;							//��Ϸ����
	static const enStartMode		m_GameStartMode;						//��ʼģʽ

	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { if (IsValid()) delete this; return ; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��������
	virtual bool __cdecl InitTableFrameSink(IUnknownEx * pIUnknownEx);
	//��λ����
	virtual VOID __cdecl RepositTableFrameSink();

	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool __cdecl OnEventGameStart();
	//��Ϸ����
	virtual bool __cdecl OnEventGameEnd(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool __cdecl SendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//��Ϣ�ӿ�
public:
	//��ʼģʽ
	virtual enStartMode __cdecl GetGameStartMode();
	//��Ϸ״̬
	virtual bool __cdecl IsUserPlaying(WORD wChairID) { return true; }

	//�¼��ӿ�
public:
	//��ʱ���¼�
	virtual bool __cdecl OnTimerMessage(WORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ����
	virtual bool __cdecl OnGameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ����
	virtual bool __cdecl OnFrameMessage(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//�û��¼�
public:
	//�û�����
	virtual bool __cdecl OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem);
	//�û�����
	virtual bool __cdecl OnActionUserReConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool __cdecl OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool __cdecl OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool __cdecl OnActionUserReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }

	//��Ϸ�¼�
private:
	//�����ӵ�
	bool OnSubBuyBullet(IServerUserItem * pIServerUserItem, enBulletCountKind BulletCountKind);
	//�����ڵ�
	bool OnSubUserShoot(IServerUserItem * pIServerUserItem, DWORD dwBulletID, float fAngle, enBulletCountKind BulletCountKind, BYTE byShootCount);
	////��׽��Ⱥ
	//bool OnSubCaptureFish(IServerUserItem * pIServerUserItem, DWORD dwFishID, DWORD dwBulletID, BYTE byBulletCount, BYTE byCptrProbability);
	//��׽��Ⱥ
	bool OnSubHitFish(IServerUserItem * pIServerUserItem, DWORD dwFishID, DWORD dwBulletID);
	//�������
	bool OnSubCalcScore(IServerUserItem * pIServerUserItem);
	//��ȡ����
	bool OnSubGetScoreInfo(IServerUserItem * pIServerUserItem);
	//���õ�Ԫ
	bool OnSubSetCellScore(IServerUserItem * pIServerUserItem, LONG lCellScore);	
	
	//��������
private:
	//��Ⱥ��ʶ
	DWORD GetNewFishID();
	//�����켣
	bool BuildTracePointLinear(tagServerFishTrace *pServerFishTraceNew, tagServerFishTrace *pServerFishTraceLast);
	//�����켣
	bool BuildTracePointLinearBonus(tagServerFishTrace *pServerFishTraceNew);
	//������Ϣ
	void SendDataExcludeSourceUser(IServerUserItem * pIServerUserItemSource, WORD wSubCmdID, void * pData, WORD wDataSize);
	//�����Ϣ
	void TraceString(LPCTSTR pszMessage);
	//�����ӵ�
	void SendBulletCount(WORD wChairID, enBulletCountKind BulletCountKind, bool bBroadcast=true);
	//���ͽ��
	void SendCoinCount(WORD wChairID);
	//������Ⱥ
	void SendFishCount(WORD wChairID, enFishKind FishKind);
	//����Ⱥ��
	void BuildRegularFish(bool bLastCol);
	//�������
	void CalcScore(IServerUserItem * pIServerUserItem, WORD wChairID);
	//����λ��
	bool RectifyTracePosX(int &nPosX);
	//��ȡ����
	void ReadConfig();
	//������Ϣ
	bool SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage);
	//�����ж�
	tagServerFishTrace *GetFishTraceInfo(DWORD dwFishID);
	//������Ϣ
	void TraceGameInfo(IServerUserItem * pIServerUserItem, LONGLONG lScore, LONGLONG lLeftBullet, WORD wChairID);
};

//////////////////////////////////////////////////////////////////////////

#endif