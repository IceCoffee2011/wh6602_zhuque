#ifndef SERVER_USER_MANAGER_HEAD_FILE
#define SERVER_USER_MANAGER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameServiceExport.h"
#include "LogCatch.h"

//�û���Ϣ��
class CServerUserItem : public IServerUserItem
{
private:
	// add by HouGuoJiang 2012-03-27
	cnd_ILogMgr*	m_pRoomLog;	// ������־

	//��Ԫ����
	friend class CServerUserManager;
	friend class CAttemperEngineSink;

	//������Ϣ
protected:
	bool								m_bAcitve;						//�����־
	bool								m_bAndroid;						//�����û�
	LONGLONG							m_lRevenue;						//��Ϸ˰��
	LONGLONG                            m_lRoundRevenue;                //ÿһ��˰��
	WORD								m_wServerType;					//��Ϸ����
	tagUserRule							m_UserRule;						//�û�����
	tagUserScore						m_ScoreBorn;					//ԭ������
	tagUserScore						m_ScoreModify;					//�������
	tagServerUserData					m_ServerUserData;				//�û���Ϣ
   	//������Ϣ
protected:
	WORD								m_wUserIndex;					//�û�����
	DWORD								m_dwClientIP;					//���ӵ�ַ
	DWORD								m_dwLogonTime;					//��¼ʱ��
	DWORD								m_dwPlayTimeCount;				//��Ϸʱ��
	DWORD                               m_dwRoundPlayTime;              //ÿһ�ֵ���Ϸʱ��
	TCHAR								m_szPassword[PASS_LEN];			//�û�����
	BOOL								m_bIsPresentOnline[4];			//�Ƿ���������{15,30,60,120}
	BOOL								m_bIsUpgradePresent[7];			//�Ƿ���������{2,3,4,5,6,7,8}

	//��������
public:
	//���캯��
	CServerUserItem(void);
	//��������
	virtual ~CServerUserItem(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//��Ϣ�ӿ�
public:

	// ������־ָ��
	void __cdecl SetRoomLog(cnd_ILogMgr*	pRoomLog);

	//�Ƿ񼤻�
	virtual bool __cdecl IsActive() { return m_bAcitve; }
	//�û�����
	virtual WORD __cdecl GetUserIndex() { return m_wUserIndex; }
	//��ȡ��ַ
	virtual DWORD __cdecl GetClientIP() { return m_dwClientIP; }
	//��ȡ����
	virtual LPCTSTR __cdecl GetPassword() { return m_szPassword; }
	//��ȡ����
	virtual const tagUserRule * __cdecl GetUserRule() { return &m_UserRule; }
	//��ȡ����
	virtual const tagUserScore * __cdecl GetUserScore() { return &m_ServerUserData.UserScoreInfo; }
	//��ȡ��Ϣ
	virtual tagServerUserData * __cdecl GetUserData() { return &m_ServerUserData; };

	//���Խӿ�
public:
	//�û�����
	virtual LPCTSTR __cdecl GetAccounts() { return m_ServerUserData.szAccounts; }
	//�û� I D
	virtual DWORD __cdecl GetUserID() { return m_ServerUserData.dwUserID; }
	//���Ӻ���
	virtual WORD __cdecl GetTableID() { return m_ServerUserData.wTableID; }
	//���Ӻ���
	virtual WORD __cdecl GetChairID() { return m_ServerUserData.wChairID; }
	//�û�״̬
	virtual BYTE __cdecl GetUserStatus() { return m_ServerUserData.cbUserStatus; }
	//�û�ʤ��
	virtual WORD __cdecl GetUserWinRate();
	//�û�����
	virtual WORD __cdecl GetUserLostRate();
	//�û�����
	virtual WORD __cdecl GetUserDrawRate();
	//�û�����
	virtual WORD __cdecl GetUserFleeRate();

	//ͳ�ƽӿ�
public:
	//��ȡ˰��
	virtual LONGLONG __cdecl GetUserRevenue() { return m_lRevenue; }
	//��ȡÿһ�ֵ�˰��
	virtual LONGLONG __cdecl GetRoundRevenue(){ return m_lRoundRevenue;}
	//��Ϸʱ��
	virtual DWORD __cdecl GetPlayTimeCount() { return m_dwPlayTimeCount; }
	//ÿ����Ϸʱ��
	virtual DWORD __cdecl GetRoundPlayTime() { return m_dwRoundPlayTime;}

	//����ʱ��
	virtual DWORD __cdecl GetOnlineTimeCount() { return (DWORD)time(NULL)-m_dwLogonTime; }
	
	// �������������
	virtual BOOL __cdecl GetOnlineTimeGolSign(BYTE byIndex);
	// ��������������
	virtual void __cdecl SetOnlineTimeGoldSign(BYTE byIndex, BOOL bVal=TRUE);
	// ��������ͱ����
	virtual BOOL __cdecl GetUpgradeGemSign(BYTE byIndex);
	// ���������ͱ����
	virtual void __cdecl SetUpgradeGemSign(BYTE byIndex, BOOL bVal=TRUE);

	//���ܽӿ�
public:
	//���ù���
	virtual bool __cdecl SetUserRule(tagUserRule & UserRule);
	//��Ϸ����
	virtual bool __cdecl SetServerType(WORD wServerType);
	//������Ϣ
	virtual bool __cdecl GetUserScoreInfo(tagUserScore & ScoreModifyInfo);
	//�޸Ļ���
    virtual bool __cdecl WriteScore(tagScoreInfo & ScoreInfo, DWORD dwPlayTimeCount);
	//����״̬
	virtual bool __cdecl SetUserStatus(BYTE cbUserStatus, WORD wTableID, WORD wChairID);
	//�޸Ľ��
	virtual bool __cdecl ModifyBankStorageGold(LONGLONG lStorageCount);
	//������Ϣ
	virtual tagUserScore* __cdecl GetUserScoreModifyInfo(){return &m_ScoreModify;}

	//�����û�
public:
	//����״̬
	virtual bool __cdecl IsAndroidUser() { return m_bAndroid; }
	//���ÿ���
	virtual VOID __cdecl SetAndroidUser(bool bAndroidUser) { m_bAndroid=bAndroidUser; }

	//���ܺ���
public:
	//��ʼ��
	void InitServerUserItem();
	//����modify����
	virtual void __cdecl ClearScorModify();

	//�����������
	//������ʱ��д�뱦ʯ
	bool __cdecl WriteUserGem( DWORD lGem);
	// ������ʱ��д����
	virtual bool __cdecl WriteUserGold(LONGLONG llGold, LONGLONG llRevenue=0l);


	//���¼���ȼ�
	void CaluClass();
private:
	//ˢ�µ���
	void RefreshProp();
	
};

//////////////////////////////////////////////////////////////////////////

//��˵��
typedef CArrayTemplate<CServerUserItem *> CServerUserItemArray;

//�������û�����
class CServerUserManager : public IServerUserManager
{
	// add by HouGuoJiang 2012-03-27
	cnd_ILogMgr*	m_pRoomLog;	// ������־

	//��������
protected:
	CServerUserItemArray				m_UserItemStore;				//�洢�û�
	CServerUserItemArray				m_OnLineUserItem;				//�����û�
	CServerUserItemArray				m_OffLineUserItem;				//�����û�

	//��������
public:
	//���캯��
	CServerUserManager(void);
	//��������
	virtual ~CServerUserManager(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:

	// ������־ָ��
	void __cdecl SetRoomLog(cnd_ILogMgr*	pRoomLog);

	//�����û�
	virtual bool __cdecl ResetUserManager();
	//�л��û�
	virtual bool __cdecl SwitchOnLineUserItem(IServerUserItem * pIServerUserItem, DWORD dwClientIP, WORD wUserIndex);
	//�����û�
	virtual bool __cdecl ActiveOffLineUserItem(IServerUserItem * pIServerUserItem, DWORD dwClientIP, WORD wUserIndex);
	//�����û�
	virtual IServerUserItem * __cdecl ActiveUserItem(tagServerUserData & ServerUserData, DWORD dwClientIP, WORD wUserIndex, TCHAR szPassword[PASS_LEN],WORD wServerType);
	//ɾ���û�
	virtual bool __cdecl DeleteUserItem(IServerUserItem * pIServerUserItem);
	//ע�����
	virtual bool __cdecl RegOffLineUserItem(IServerUserItem * pIServerUserItem);
	//ɾ������
	virtual bool __cdecl UnRegOffLineUserItem(IServerUserItem * pIServerUserItem);

	//���ҽӿ�
public:
	//ö���û�
	virtual IServerUserItem * __cdecl EnumOnLineUser(WORD wEnumIndex);
	//ö���û�
	virtual IServerUserItem * __cdecl EnumOffLineUser(WORD wEnumIndex);
	//�����û�
	virtual IServerUserItem * __cdecl SearchOnLineUser(DWORD dwUserID);
	//�����û�
	virtual IServerUserItem * __cdecl SearchOffLineUser(DWORD dwUserID);
	//�����û�
	virtual IServerUserItem * __cdecl SearchOnLineUser(LPCTSTR pszAccounts);
	//�����û�
	virtual IServerUserItem * __cdecl SearchOffLineUser(LPCTSTR pszAccounts);

	//�û��Ƿ�����  �������
	bool   IsUserInServer(IServerUserItem* pServerUserItem);
    

	//��Ϣ�ӿ�
public:
	//��������
	virtual DWORD __cdecl GetOnLineCount() { return (DWORD)m_OnLineUserItem.GetCount(); }
	//��������
	virtual DWORD __cdecl GetOffLineCount() { return (DWORD)m_OffLineUserItem.GetCount(); }
};

//////////////////////////////////////////////////////////////////////////

#endif