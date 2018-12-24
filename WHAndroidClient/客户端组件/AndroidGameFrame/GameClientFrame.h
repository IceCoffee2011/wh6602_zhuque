#pragma once
#include "stdafx.h"
#include "afxcmn.h"
#include "AndroidGameFrameEx.h"
#include "IGameClientFrame.h"

// CGameClient
//��Ϸ�ͻ��˿��
class ANDROID_GAME_FRAME_EX CGameClientFrame : public CWnd,  public IGameClientFrame
{
	DECLARE_DYNAMIC(CGameClientFrame)

public:
	CGameClientFrame();
	virtual ~CGameClientFrame();

protected:
	DECLARE_MESSAGE_MAP()

public:
	//��ʼ����
	virtual bool InitGameFrame()=NULL;
	//���ÿ��
	virtual void ResetGameFrame()=NULL;

public:
	//����SocketID
	virtual void __cdecl SetMeSocketID(WORD wSocketID);
	//�Ƿ񴰿�
	virtual bool __cdecl IsEnableWnd();
	//�رմ���
	virtual void __cdecl CloseWnd();
	//����Լ���Ϣ
	virtual const tagUserData* __cdecl GetMeData();

public:
	//��Ϸ�������
	virtual bool __cdecl OnSocketMainGameFrame(CMD_Command Command, void * pBuffer, WORD wDataSize);

	//ϵͳ�¼�
public:
	//ʱ����Ϣ
	virtual bool OnTimerMessage(UINT nElapse, UINT nTimerID)=NULL;
	//������Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)=NULL;
	//��Ϸ����
	virtual bool OnGameSceneMessage(BYTE cbGameStatus, const void * pBuffer, WORD wDataSize)=NULL;

public:
	//�¼�Socket
	virtual bool OnEventSocket(CMD_Command Command, void * pBuffer, WORD wDataSize);
	//�����Ϣ
	virtual bool OnFrameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);

	//�û��¼�
public:
	//�û�����
	virtual void __cdecl OnEventUserEnter(const tagUserData* pUserData);
	//�û��뿪
	virtual void __cdecl OnEventUserLeave();
	//�û�����
	virtual void __cdecl OnEventUserScore(const IPC_UserScore* pUserScore);
	//�û�״̬
	virtual void __cdecl OnEventUserStatus(WORD wTableID, WORD wChairID, BYTE cbUserStatus);

public:
	//�Լ�λ��
	WORD GetMeChairID();
	//���ͺ���
	bool SendData(WORD wSubCmdID);
	//���ͺ���
	bool SendData(WORD wSubCmdID, void * pData, WORD wDataSize);
	//���ͺ���
	bool SendData(WORD wMainCmdID, WORD wSubCmdID);
	//���ͺ���
	bool SendData(WORD wMainCmdID, WORD wSubCmdID, void * pData, WORD wDataSize);
	//����׼��
	bool SendUserReady(void * pBuffer, WORD wDataSize);
	//���ö�ʱ��
	bool SetGameTimer(UINT nTimerID, UINT nElapse);
	//ɾ����ʱ��
	bool KillGameTimer(UINT nTimerID);
	
	//��Ϸ״̬
public:
	//��Ϸ״̬
	BYTE GetGameStatus();
	//��Ϸ״̬
	void SetGameStatus(BYTE bGameStatus);

protected:
	//�Լ���Ϣ
	tagUserData			m_MeUserInfo;

private:
	//�����Լ���Socket��ʶ
	WORD				m_wSocketID;

	//ʱ����
	DWORD				m_wTimeID;
	DWORD				m_nElapseCount;

	//��Ϸ״̬
	BYTE				m_bGameStatus;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
};


