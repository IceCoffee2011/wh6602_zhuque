#ifndef		__DEF_INCLUDE_GAME_USER_INFO_INSTANCE_H___
#define		__DEF_INCLUDE_GAME_USER_INFO_INSTANCE_H___

#include "stdafx.h"
#include "AndroidUserInfoEx.h"
#include "IGameClientFrame.h"
#include "UserGameDataDef.h"
#include <map>

using namespace std;


typedef IGameClientFrame* NewGameClient(CWnd* pParent, DWORD dwWndID);
typedef  void FreeGameClient(IGameClientFrame* pClient);


 class ANDROID_USER_INFO_EX  CGameUserInfo
{

public:
	~CGameUserInfo(void);

public:
	//���Ψһʵ��
	static CGameUserInfo* GetInstance();

public:
	//����DLL
	bool LoadGameClientDll(const std::string& strDllName);
	//��ʼ����Ϣ
	WORD InitGameUserInfo();
	//�ͷų�ʼ����Ϣ
	bool FreeGameUserInfo();
	//��������ص�����
	bool SetIUnknownEx(BYTE byTag, IUnknownEx * pIUnknownEx);
	//��������˳���Ϸ
	void AllUserExitGame();
	//�ر�����socket
	void CloseAllSocket();
	//��ղ��˳�������
	void ClearGameUserInfo();

public:
	//��ʼ���û���������
	void InitGameUserShareData();
	//��÷�����Ϣ
	const Local_GR_ServerInfo* GetLocalRoomInfo();
	//���÷�����Ϣ
	void SetLocalRoomInfo(WORD wGameGenre, WORD wKindID, WORD wTableCount, WORD wChairCount);

public:
	//�����Ϣ����
	std::map<DWORD, strUserGameInfo*>		m_vecUserGameInfo;

protected:
	CGameUserInfo(void);
	
private:
	//��Ϸ�ͻ���DLLʵ��
	HINSTANCE	m_hGameDllInstance;
	//������Ϸ�ͻ���ָ��
	NewGameClient* m_ptrNewGameClient;
	//�ͷ���Ϸ�ͻ���ָ��
	FreeGameClient* m_ptrFreeGameClient;
	//ʵ��
	static CGameUserInfo*	m_Instance;
	//������Ϣ
	Local_GR_ServerInfo		m_GameServerInfo;

};


#endif