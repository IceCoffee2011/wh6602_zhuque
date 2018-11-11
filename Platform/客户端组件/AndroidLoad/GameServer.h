#pragma once

#include "stdafx.h"
#include "afxcmn.h"
#include "IGamePlaza.h"
#include "GameUserInfo.h"

class CFlashEnterDlg;


//��Ϸ������
class CGameServer : public CWnd, public IViewItem, public ITCPSocketSink
{
	DECLARE_DYNAMIC(CGameServer)

public:
	CGameServer();
	virtual ~CGameServer();

protected:
	DECLARE_MESSAGE_MAP()

public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);
public:
	//��ȡ���
	virtual HWND __cdecl GetWindowHandle() { return GetSafeHwnd(); }
	//�����¼�
public:
	//�����¼�
	virtual bool __cdecl OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode);
	//�ر��¼�
	virtual bool __cdecl OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool __cdecl OnEventTCPSocketRead(WORD wSocketID, CMD_Command Command, VOID * pData, WORD wDataSize);

public:
	//��¼��Ϣ
	bool OnSocketMainLogon(WORD wSocketID, CMD_Command Command, void * pBuffer, WORD wDataSize);
	//�û���Ϣ
	bool OnSocketMainUser(WORD wSocketID, CMD_Command Command, void * pBuffer, WORD wDataSize);
	//������Ϣ
	bool OnSocketMainInfo(WORD wSocketID, CMD_Command Command, void * pBuffer, WORD wDataSize);
	//״̬��Ϣ
	bool OnSocketMainStatus(WORD wSocketID, CMD_Command Command, void * pBuffer, WORD wDataSize);
	//ϵͳ��Ϣ
	bool OnSocketMainSystem(WORD wSocketID, CMD_Command Command, void * pBuffer, WORD wDataSize);
	//������Ϣ
	bool OnSocketMainServerInfo(WORD wSocketID, CMD_Command Command, void * pBuffer, WORD wDataSize);
	//��Ϸ��Ϣ
	bool OnSocketMainGameFrame(WORD wSocketID, CMD_Command Command, void * pBuffer, WORD wDataSize);

public:
	//�û�����
	bool OnSocketSubUserCome(strUserGameInfo* pUserGameInfo,  void * pBuffer, WORD wDataSize);
	//�û�״̬
	bool OnSocketSubStatus(strUserGameInfo* pUserGameInfo,  void * pBuffer, WORD wDataSize);
	//�û�����
	bool OnSocketSubScore(strUserGameInfo* pUserGameInfo,  void * pBuffer, WORD wDataSize);

public:
	//����������ָ��
	void SetMainWnd(CFlashEnterDlg* pMainWnd);
	//������Ϸ������֮ǰ��ʼ������
	bool InitGameLocalData();
	//���ӵ���Ϸ������
	void OnConnectGameServer();
	//������Ӧֵ
	void GetLogonInfo(DWORD &dwLogonSucNums, DWORD &dwLogonFailNums, DWORD &dwTCPCoonectNums, DWORD &dwUserConnectNums);
	void GetQueueInfo(DWORD &dwQueueSucNums, DWORD &dwQueueFailNums);
	//�˳���Ϸ������
	bool OnEventExitServer();
	//��¼����Ϸ������
	void UserLogonToGameServer(strUserGameInfo* pUserInfo);
	//��������Ŷ�
	void SetAllUserQueue();
	//��ʼ���������IP�Ͷ˿ں�
	bool StartAllotServerIPAndPort();
	//��ұ�ʯ����,���ܵ�¼��Ϸ������
	bool UserCanLogonGame(LONG lRestrictScore);
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);

private:
	//�����ھ��
	CFlashEnterDlg*		m_pMainWnd;
	//�����û�socket��Ϣ
	CGameUserInfo*		m_pGameUserInfo;
	//��¼��Ϸ��Ϣ
	strLogonInfo		m_LogonInfo;
	//�Զ��Ŷӳɹ�����
	DWORD				m_dwQueueSuccNums;
	//�Ŷ�ʧ������
	DWORD				m_dwQueueFailNums;

};


