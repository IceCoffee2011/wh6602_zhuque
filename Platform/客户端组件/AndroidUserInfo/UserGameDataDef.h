#ifndef		___DEF_USER_GAME_DATA_INCL__1342_CREATE_DEF__
#define		___DEF_USER_GAME_DATA_INCL__1342_CREATE_DEF__

#include <vector>
using namespace std;

//�ܹ����Ӵ���
const int I_CONNECT_NUMS = 5;

//������Ϣ
struct strLogonInfo 
{
	DWORD		dwLogonSucNums;			//��¼�ɹ��û���
	DWORD		dwLogonFailNums;		//��¼ʧ���û���
	DWORD		dwTCPConnectNums;		//TCP������
	DWORD		dwUserConnectNums;		//User������
};


//-------------------------------------------------------
//TCP����״̬
#define		TAG_LINK_FAIL		0		//ʧ��
#define		TAG_LINK_SUC		1		//�ɹ�
#define		TAG_LINKING			2		//��������...
//-------------------------------------------------------


//--------------------------------------------------------------------------------
//�������״̬
#define		USER_LOGON_SERVER_FAIL				0		//ʧ��
#define		USER_LOGON_SERVER_SUC				1		//�ɹ�
#define		USER_LOGON_SERVER_LINKING			2		//��������...
#define		USER_AUTO_QUEUE_SERVER_LINKING		3		//�����Ŷ�...
//--------------------------------------------------------------------------------

//��ֵ
#define		DEF_NUM_VALUE						0.9


//��Ϸ������Ϣ
struct GameOption
{
	char	chUserID[32];
	char	chUserPassWord[32];			//����
	char	chUserAddPassWord[32];		//���ܺ�����
	DWORD	dwGameID;
	DWORD	dwRoomID;
	DWORD	dwUserNums;
	char	chServerIP[32];
	DWORD	dwServerPort;
	DWORD	dwSockStartID;
	DWORD	dwUserIndex;
	char	chGameServerIP[32];
	DWORD	dwGameServerPort;

	GameOption()
	{
		::memset(chUserID, 0, sizeof(chUserID));
		::memset(chUserPassWord, 0, sizeof(chUserPassWord));
		::memset(chUserAddPassWord, 0, sizeof(chUserAddPassWord));
		::memset(chServerIP, 0, sizeof(chServerIP));
		dwGameID = 0;
		dwRoomID = 0;
		dwUserNums = 0;
		dwServerPort = 0;
		dwUserIndex = 0;
		dwSockStartID = 2000;
		::memset(chGameServerIP, 0, sizeof(chGameServerIP));
		dwGameServerPort=0;
	}
};



class IGameClientFrame;

//��Ϸ�û���Ϣ
struct strUserGameInfo 
{
	char						chName[32];		//username
	DWORD						dwUserID;		//userid
	DWORD						dwKindID;		//game kindid
	DWORD						dwSocketID;		//socketid	,SOCKET��ʶ
	DWORD						dwGameServerIP;		//gameserverip
	DWORD						dwGameServerport;	//gameserverport
	DWORD						dwLogonLinkState;	//��¼����������״̬
	DWORD						dwGameLinkState;	//��Ϸ����������״̬
	DWORD						dwUserLogonState;	//��ҵ�¼״̬
	DWORD						dwUserGameState;	//��ҵ�¼��Ϸ״̬
	LONGLONG					llUserGems;			//��ұ�ʯ
	DWORD						dwCountConnect;		//���Ӵ���
	CTCPSocketHelper			TcpSocketHelper;	//sockethelper
	IGameClientFrame*			pIGameClient;		//��Ϸ�ͻ���

	strUserGameInfo()
	{
		::memset(chName, 0, sizeof(chName));
		dwKindID = 0;
		dwUserID = 0;
		dwSocketID = 0;
		dwGameServerIP = 0;
		dwGameServerport = 0;
		llUserGems = 0;
		dwCountConnect = 0;
		dwLogonLinkState = TAG_LINK_FAIL;
		dwLogonLinkState = TAG_LINK_FAIL;
		dwUserLogonState = USER_LOGON_SERVER_FAIL;
		dwUserGameState = USER_LOGON_SERVER_FAIL;
	}
};



//��Ϸ������Ϣ
struct Local_GR_ServerInfo
{
	//��������
	WORD							wKindID;							//���� I D
	WORD							wTableCount;						//������Ŀ
	WORD							wChairCount;						//������Ŀ
	//��չ����
	WORD							wGameGenre;							//��Ϸ����
	bool							bIsInit;							//�Ƿ��ʼ��
};


//��Ϸ�����б�
struct GameServerList
{
	WORD						wSortID;			//�����ʶ
	WORD						wKindID;			//��Ϸ����
	DWORD						dwGameServerIP;		//gameserverip
	DWORD						dwGameServerport;	//gameserverport
	DWORD						dwOnLineCount;		//onLineCount
	DWORD						dwMaxCount;			//MaxCount
	LONG                        lRestrictScore;		//���ƻ���
};


#endif