#ifndef ANDROID_USER_ITEM_SINK_HEAD_FILE
#define ANDROID_USER_ITEM_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////

//��������
class CAndroidUserItemSink : public IAndroidUserItemSink
{
	//�û�����
protected:
	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ�û�
	BYTE							m_cbActionMask;						//������ʶ
	BYTE							m_cbActionCard;						//������

	//�˿˱���
protected:
	BYTE							m_cbLeftCardCount;						//ʣ����Ŀ
	BYTE							m_cbCardIndex[GAME_PLAYER][MAX_INDEX];	//�����˿�
	BYTE							m_cbRepertoryCard[MAX_REPERTORY];		//����˿�

	//����˿�
protected:
	BYTE							m_cbWeaveCount[GAME_PLAYER];		//�����Ŀ
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][MAX_WEAVE];	//����˿�

	//������Ϣ
protected:
	WORD							m_wOutCardUser;						//�����û�
	BYTE							m_cbOutCardData;					//�����˿�
	BYTE							m_cbDiscardCount[GAME_PLAYER];		//������Ŀ
	BYTE							m_cbDiscardCard[GAME_PLAYER][60];	//������¼

	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	IAndroidUserItem *				m_pIAndroidUserItem;				//�û��ӿ�

	//��������
public:
	//���캯��
	CAndroidUserItemSink();
	//��������
	virtual ~CAndroidUserItemSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���ƽӿ�
public:
	//��ʼ�ӿ�
	virtual bool __cdecl InitUserItemSink(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	virtual bool __cdecl RepositUserItemSink();

	//��Ϸ�¼�
public:
	//ʱ����Ϣ
	virtual bool __cdecl OnEventTimer(UINT nTimerID);
	//��Ϸ��Ϣ
	virtual bool __cdecl OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	//��Ϸ��Ϣ
	virtual bool __cdecl OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize);
	//������Ϣ
	virtual bool __cdecl OnEventGameScene(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize);

	//�û��¼�
public:
	//�û�����
	virtual void __cdecl OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û��뿪
	virtual void __cdecl OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û�����
	virtual void __cdecl OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û�״̬
	virtual void __cdecl OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);
	//�û���λ
	virtual void __cdecl OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser);

	//��Ϣ����
protected:
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
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize);
	//���ײ���
	bool OnSubOperateHaiDi(const void * pBuffer, WORD wDataSize);

	//������
protected:
	//����
	void OnOutCard( BYTE cbOutCard );
	//������
	void OnOperateCard( BYTE cbOperateCode,BYTE cbOperateCard );

	//��������
protected:
	//���ط�������
	bool SearchTingCard( tagTingCardResult &TingCardResult );
	//��������
	bool SearchOutCard( tagOutCardResult &OutCardResult );
};

//////////////////////////////////////////////////////////////////////////

#endif