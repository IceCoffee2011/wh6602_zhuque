#ifndef CLIENT_KERNEL_SINK_HEAD_FILE
#define CLIENT_KERNEL_SINK_HEAD_FILE

#pragma once
#include "Stdafx.h"
#define REMOVE_VIDEO  0 // 0Ϊ�Ƴ���Ƶ��1�����д���0��ֵ��Ϊ������Ƶ

class CGameFrameDlg;

//////////////////////////////////////////////////////////////////////////

//�ں˹�����
class CClientKernelSink : public IClientKernelSink
{
	//��������
protected:
	CGameFrameDlg				& m_GameFrameDlg;						//��Ϸ���

	//��������
public:
	//���캯��
	CClientKernelSink(CGameFrameDlg & GameFrameDlg);
	//��������
	virtual ~CClientKernelSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//�����ӿ�
private:
	//��ȡ����
	virtual HWND __cdecl GetFrameWnd();
	//����״̬
	virtual void __cdecl ResetGameFrame();
	//�رմ���
	virtual void __cdecl CloseGameFrame(bool bForceClose);

	//�ں��¼�
private:
	//ʱ����Ϣ
	virtual bool __cdecl OnEventTimer(WORD wChairID, UINT nElapse, UINT nTimerID);
	//������Ϣ
	virtual bool __cdecl OnEventSocket(const CMD_Command & Command, const void * pBuffer, WORD wDataSize);
	//������Ϣ
	virtual bool __cdecl OnEventProcess(const IPC_Head & Head, const void * pBuffer, WORD wDataSize, HWND hWndSend);

	//��Ϸ�¼�
private:
	//��Ϸ״̬
	virtual bool __cdecl OnEventGameScene(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize);
	//������Ϣ
	virtual bool __cdecl OnUserChatMessage(tagUserData * pSendUserData, tagUserData * pRecvUserData, LPCTSTR pszChatMessage, COLORREF crColor);
	//ʱ�����
	virtual void __cdecl OnEventTimerKilled(WORD wChairID, UINT nTimerID);
	//�Թ�״̬
	virtual void __cdecl OnEventLookonChanged(bool bLookonUser, void * pBuffer, WORD wDataSize);

	//�û��¼�
public:
	//�û�����
	virtual void __cdecl OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
	//�û��뿪
	virtual void __cdecl OnEventUserLeave(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
	//�û�����
	virtual void __cdecl OnEventUserScore(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
	//�û�״̬
	virtual void __cdecl OnEventUserStatus(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
	//��Ա�ȼ�
	virtual void __cdecl OnEventUserMemberOrder(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
	
	//�����¼�
public:
	//��ʼ����
	virtual void __cdecl OnEventInitProperty(tagPropertyInfo *pPropertyInfo, int nInfoCount);
	//��ʼ�ʻ�
	virtual void __cdecl OnEventInitFlower(tagFlowerInfo *pFlowerInfo, int nInfoCount);
	//�ʻ���Ϣ
	virtual void __cdecl OnEventFlower(const tagUserData * pSendUserData, const tagUserData * pRecvUserData, UINT uFlowerID, UINT uFlowerEffectID);
	//���ʣ�����
	virtual void __cdecl OnEventUserHaveDJInfo(CMD_GF_ResidualProperty *pUserDJInfo);
	//���߲�����Ϣ
	virtual void __cdecl OnEventUserOperationDJInfo(BYTE byTag, void * pBuffer);
	//����������Ϣ
	virtual void __cdecl OnEventGameDJInfo(const char* pInfo);
	//�������	Author<cxf>; DateTime<2012-01-16>
	virtual void __cdecl OnEventNewGift(CMD_GF_NEW_GIFT * pNewGift);
	//��������-��ʱ
	virtual void __cdecl OnEventOnlineKeepTime(CMD_GF_ONLINE * pOnline);

	//������Ϣ��Ӧ
public:
	//����Ϣ
	virtual void __cdecl OnEventHaveChatMessage();
	//������Ϣ
	virtual void __cdecl OnEventGameAfficheInfo(const char* pDataInfo, WORD wDataSize);
	//�û�md5����
	virtual void __cdecl OnEventUserMD5PWMessage(void *pBuffer);
	//�û�������Ϣ
	virtual void __cdecl OnEventUserActionInfo(BYTE btSender, BYTE btRecver, BYTE btTag);
	//�û�������Ϣ
	virtual void __cdecl OnEventUserLanguageInfo(BYTE bySender, DWORD dwIndex);

	//������Ϣ
public:
	//����������
	virtual void __cdecl OnEventTaskAccept(const CMD_GF_TASK_tagTask* pTaskAccept);
	//�����������
	virtual void __cdecl OnEventTaskOnceFinish(const CMD_GF_TASK_tagTaskFinish* pTaskFinish);

};

//////////////////////////////////////////////////////////////////////////

#endif