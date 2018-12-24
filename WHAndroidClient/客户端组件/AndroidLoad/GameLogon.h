#pragma once

#include "stdafx.h"
#include "afxcmn.h"
#include "IGamePlaza.h"
#include "GameUserInfo.h"


class CFlashEnterDlg;

//��¼������
class CGameLogon : public CWnd, public IViewItem, public ITCPSocketSink
{
	DECLARE_DYNAMIC(CGameLogon)

public:
	CGameLogon();
	virtual ~CGameLogon();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

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
	bool OnSocketMainLogon(WORD wSocketID, CMD_Command Command, void * pData, WORD wDataSize);
	//�б���Ϣ
	bool OnSocketMainServerList(WORD wSocketID, CMD_Command Command, void * pBuffer, WORD wDataSize);

public:
	//����������ָ��
	void SetMainWnd(CFlashEnterDlg* pMainWnd);
	//���ӵ�¼������
	bool OnEventConnectServer();
	//�˳���¼������
	bool OnEventExitServer();
	//������Ӧֵ
	void GetLogonInfo(DWORD &dwLogonSucNums, DWORD &dwLogonFailNums, DWORD &dwTCPCoonectNums, DWORD &dwUserConnectNums);
	//��¼����¼������
	void UserLogonToLogonServer(strUserGameInfo* pUserInfo);

private:
	CFlashEnterDlg*		m_pMainWnd;
	CGameUserInfo*		m_pGameUserInfo;
	strLogonInfo		m_LogonInfo;
	
public:
	afx_msg void OnTimer(UINT nIDEvent);
};


