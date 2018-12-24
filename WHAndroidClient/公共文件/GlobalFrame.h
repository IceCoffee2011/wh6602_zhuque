#ifndef GLOBAL_FRAME_HEAD_FILE
#define GLOBAL_FRAME_HEAD_FILE

#pragma once

#include "GlobalDef.h"
#include "GlobalProperty.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//��Ϸ״̬
#define GS_FREE							0								//����״̬
#define GS_PLAYING						100								//��Ϸ״̬

//////////////////////////////////////////////////////////////////////////
//IPC �����¼�

#define IPC_MAIN_SOCKET					1								//������Ϣ

#define IPC_SUB_SOCKET_SEND				1								//���緢��
#define IPC_SUB_SOCKET_RECV				2								//�������

//IPC ������ṹ
struct IPC_SocketPackage
{
	CMD_Command							Command;						//������Ϣ
	BYTE								cbBuffer[SOCKET_PACKET];		//���ݻ���
};

//////////////////////////////////////////////////////////////////////////
//IPC ������Ϣ

#define IPC_MAIN_CONFIG					2								//������Ϣ

#define IPC_SUB_SERVER_INFO				1								//������Ϣ
#define IPC_SUB_COLUMN_INFO				2								//�б���Ϣ
#define IPC_SUB_GAMEAFFICHE_INFO		3								//������Ϣ

//��Ϸ��Ϣ
struct IPC_GF_ServerInfo
{
	DWORD								dwUserID;						//�û� I D
	WORD								wTableID;						//���Ӻ���
	WORD								wChairID;						//���Ӻ���
	WORD								wKindID;						//���ͱ�ʶ
	WORD								wServerID;						//�����ʶ
	WORD								wGameGenre;						//��Ϸ����
	WORD								wChairCount;					//������Ŀ
	BYTE								cbHideUserInfo;					//������Ϣ
	DWORD								dwVideoAddr;					//��Ƶ��ַ
	TCHAR								szKindName[KIND_LEN];			//��������
	TCHAR								szServerName[SERVER_LEN];		//��������
	WORD								wSortID;						//�������
	LONG								lCellScore;						//���뷿����ͽ��
};

//////////////////////////////////////////////////////////////////////////
//IPC �û���Ϣ

#define IPC_MAIN_USER					3								//�û���Ϣ

#define IPC_SUB_USER_COME				1								//�û���Ϣ
#define IPC_SUB_USER_STATUS				2								//�û�״̬
#define IPC_SUB_USER_SCORE				3								//�û�����
#define IPC_SUB_GAME_START				4								//��Ϸ��ʼ
#define IPC_SUB_GAME_FINISH				5								//��Ϸ����
#define IPC_SUB_UPDATE_FACE				6								//����ͷ��
#define IPC_SUB_MEMBERORDER				7								//����ͷ��
#define IPC_SUB_GETUSERPASSWORD			8								//��ȡ�û�md5����
#define IPC_SUB_RECVACTIONINFO			9								//�����û�������Ϣ
#define IPC_SUB_RECVGLINFO				11								//�����û�������Ϣ


//�û�״̬
struct IPC_UserStatus
{
	DWORD								dwUserID;						//�û� I D
	WORD								wNetDelay;						//������ʱ
	BYTE								cbUserStatus;					//�û�״̬
};

//�û�����
struct IPC_UserScore
{
	LONG								lLoveliness;					//�û�����
	DWORD								dwUserID;						//�û� I D
	tagUserScore						UserScore;						//�û�����
};

//��Ա�ȼ�
struct IPC_MemberOrder
{
	BYTE								cbMember;					//��Ա�ȼ�
	DWORD								dwUserID;						//�û� I D
};

//�û�����
struct IPC_UpdateFace
{
	DWORD								dwCustomFace;					//�û� I D
};

//�û�MD5����
struct IPC_UserPassWord
{
	char								chUserPassWord[PASS_LEN];		//�û�MD5����
};


//////////////////////////////////////////////////////////////////////////
//IPC ������Ϣ

#define IPC_MAIN_CONCTROL				4								//������Ϣ

#define IPC_SUB_START_FINISH			1								//�������
#define IPC_SUB_CLOSE_FRAME				2								//�رտ��
#define IPC_SUB_JOIN_IN_GAME			3								//������Ϸ
#define IPC_SUB_GO_PLAZA				4								//���ش���(��Ϸ�б�)
#define IPC_SUB_GO_ROOM					5								//���ط����б�
#define IPC_SUB_GAMEFRAME_EXIST			6								//��Ϸ����Ƿ����
#define IPC_SUB_GAME_FRAME_CLOSED		7								//��Ϸ��ܴ��ڹر���Ϣ
#define IPC_SUB_USER_UPDATE_DJINFO		8								//�û����µ�����Ϣ
#define IPC_SUB_SENDACTIONINFO			9								//�����û�������Ϣ
#define IPC_SUB_SENDLANLISTINFO			11								//����������Ϣ
#define IPC_SUB_CMD_CONNECTWEB			12								//connect web


//web
struct IPC_ConnWeb
{
	BYTE								byConWebTag;					//0:NO, 1:�ٷ���ҳ; 2:�����̳�: 3:�ͷ�����;
};


//������Ϸ
struct IPC_JoinInGame
{
	WORD								wTableID;						//���Ӻ���
	WORD								wChairID;						//���Ӻ���
};

//�û�������Ϣ
struct IPC_UserActionInfo 
{
	BYTE								dwSendUserID;					//������
	BYTE								dwRecvUserID;					//������
	BYTE								dwActionTag;					//������ʶ
};
//�û�������Ϣ
struct IPC_UserLanguageInfo
{
	BYTE								dwSendUserID;					//������
	DWORD								dwIndex;					//���
};

//�����û�����ID
struct IPC_UserUpdateDJInfo
{
	int									iDJID;							//����ID						
};
//////////////////////////////////////////////////////////////////////////
//����������

#define MDM_GF_GAME						100								//��Ϸ��Ϣ
#define MDM_GF_FRAME					101								//�����Ϣ
#define	MDM_GF_PRESENT					102								//������Ϣ
#define	MDM_GF_BANK						103								//������Ϣ
#define MDM_GF_TASK						104								//������Ϣ

#define SUB_GF_INFO						1								//��Ϸ��Ϣ
#define SUB_GF_USER_READY				2								//�û�ͬ��
#define SUB_GF_LOOKON_CONTROL			3								//�Թۿ���
#define SUB_GF_KICK_TABLE_USER			4								//�����û�

#define SUB_GF_OPTION					100								//��Ϸ����
#define SUB_GF_SCENE					101								//������Ϣ

#define SUB_GF_USER_CHAT				200								//�û�����

#define SUB_GF_MESSAGE					300								//ϵͳ��Ϣ

//#define SUB_GF_GIFT						400								//������Ϣ

#define SUB_GF_BANK_STORAGE				450								//���д洢
#define SUB_GF_BANK_GET					451								//������ȡ

//#define SUB_GF_FLOWER_ATTRIBUTE			500								//�ʻ�����
//#define SUB_GF_FLOWER					501								//�ʻ���Ϣ
//#define SUB_GF_EXCHANGE_CHARM			502								//�һ�����

//#define SUB_GF_PROPERTY					550							//������Ϣ
//#define SUB_GF_PROPERTY_RESULT			551						    //���߽��
//#define SUB_GF_RESIDUAL_PROPERTY		552								//ʣ�����
//#define SUB_GF_PROP_ATTRIBUTE			553								//��������
#define SUB_GF_PROP_BUGLE				554								//���ȵ���
//�������
//#define SUB_GF_ACTIVE_PROP              560							//���߼�����Ϣ
//#define SUB_GF_ACTIVE_SUCC              561							//���߼���ɹ�
//#define SUB_GF_ACTIVE_FAIL_NO_PROP      562							//û�е���     ����ʧ��
//#define SUB_GF_ACTIVE_FAIL_STILL_USE    563							//���߻���ʹ�� ����ʧ��
//#define SUB_GF_USER_UPDATE_PROP         564							//�û�������µ���

//#define SUB_GF_PROP_OVERDUE             570							//Ŀǰ�ĵ����Ѿ�������
//#define SUB_GF_PROP_UPDATE              571							//������Ϣ����


#define SUB_SENDACTIONINFO			    572								//�����û�������Ϣ
#define SUB_RECVACTIONINFO			    573							    //�����û�������Ϣ
#define SUB_SEND_SOUND_MESS         574                             //����Ԥ����������Ϣ
#define SUB_RECV_SOUND_MESS         575                             //����Ԥ����������Ϣ

#define SUB_GF_QUIT_GAME_SERVER         580								//�û������˳���Ϸ������     
#define SUB_GF_QUIT_GAME_SERVER_SUCC    581								//�����˳���Ϸ�������ɹ�
//��ӽ���


#define SUB_GF_FIND_GEMS_TASK_ACCEPT				600								//�������
//#define SUB_GF_TASK_SAVEJD						601								//�������
#define SUB_GF_FIND_GEMS_TASK_ONCEEND				602								//�����������
#define SUB_GF_NEW_GIFT								603								//�������	Author<cxf>; DateTime<2012-01-16>
#define SUB_GF_ONLINE_TIME							604								//��������-��ʱ��

//�汾��Ϣ
struct CMD_GF_Info
{
	BYTE								bAllowLookon;					//�Թ۱�־
};

//��Ϸ����
struct CMD_GF_Option
{
	BYTE								bGameStatus;					//��Ϸ״̬
	BYTE								bAllowLookon;					//�����Թ�
};

//�Թۿ���
struct CMD_GF_LookonControl
{
	DWORD								dwUserID;						//�û���ʶ
	BYTE								bAllowLookon;					//�����Թ�
};

//�����û�
struct CMD_GF_KickTableUser
{
	DWORD								dwUserID;						//�û� I D
};

//����ṹ
struct CMD_GF_UserChat
{
	WORD								wChatLength;					//��Ϣ����
	COLORREF							crFontColor;					//��Ϣ��ɫ
	DWORD								dwSendUserID;					//�����û�
	DWORD								dwTargetUserID;					//Ŀ���û�
	TCHAR								szChatMessage[MAX_CHAT_LEN];	//������Ϣ
};
//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define SMT_INFO						0x0001							//��Ϣ��Ϣ
#define SMT_EJECT						0x0002							//������Ϣ
#define SMT_GLOBAL						0x0004							//ȫ����Ϣ
#define SMT_GAMEDJINFO					0x0008							//������Ϣ
#define SMT_CLOSE_GAME					0x0010							//�ر���Ϸ
#define SMT_INTERMIT_LINE				0x0020							//�ж�����
#define SMT_CLOSE_ROOM					0x0040							//�رշ���
#define SMT_CUSTOMER_FRAME				0x0080							//�ͷ���

//��Ϣ���ݰ�
struct CMD_GF_Message
{
	WORD								wMessageType;					//��Ϣ����
	WORD								wMessageLength;					//��Ϣ����
	TCHAR								szContent[1024];				//��Ϣ����

	CMD_GF_Message()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

//////////////////////////////////////////////////////////////////////////

//�����ڴ涨��
struct tagShareMemory
{
	WORD								wDataSize;						//���ݴ�С
	HWND								hWndGameFrame;					//��ܾ��
	HWND								hWndGamePlaza;					//�㳡���
	HWND								hWndGameServer;					//������
};

//////////////////////////////////////////////////////////////////////////

//���ͳ���
#define	LOCATION_GAME_ROOM				1								//��Ϸ����
#define	LOCATION_PLAZA_ROOM				2								//��������

//////////////////////////////////////////////////////////////////////////

//���ͽṹ
struct CMD_GF_Gift
{
	BYTE								cbSendLocation;					//���ͳ���
	DWORD								dwSendUserID;					//������ID
	DWORD								dwRcvUserID;					//������ID
	WORD								wGiftID;						//����	ID
	WORD								wFlowerCount;					//�ʻ���Ŀ
};

//���߽ṹ
struct CMD_GF_Property
{
	BYTE								cbSendLocation;					//���ͳ���
	int									nPropertyID;					//����ID
	DWORD								dwPachurseCount;				//������Ŀ
	DWORD								dwOnceCount;					//������Ŀ
	DWORD								dwSourceUserID;					//�������
	DWORD								dwTargetUserID;					//Ŀ�����
	TCHAR								szRcvUserName[32];				//�û�����
};



//���Ƚṹ
struct CMD_GF_BugleProperty
{
	BYTE								cbSendLocation;					//���ͳ���
	TCHAR								szUserName[32];					//����ʺ�
	COLORREF							crText;							//������ɫ
	TCHAR								szContext[BUGLE_MAX_CHAR];		//��������
};

//�һ��ṹ
struct CMD_GF_ExchangeCharm
{
	BYTE								cbSendLocation;					//���ͳ���
	LONG								lLoveliness;					//������ֵ
	DWORD								lGoldValue;						//�����ֵ
};

//����֪ͨ
struct CMD_GF_GiftNotify
{
	BYTE								cbSendLocation;					//���ͳ���
	DWORD								dwSendUserID;					//������ID
	DWORD								dwRcvUserID;					//������ID
	WORD								wGiftID;						//����	ID
	WORD								wFlowerCount;					//�ʻ���Ŀ
};

//�洢���
struct CMD_GF_BankStorage
{
	LONGLONG 							lStorageValue;					//�洢���
	BYTE								cbGameAction;					//��Ϸ����
	TCHAR								szPassword[PASS_LEN];			//�û�����
};

//��ȡ���
struct CMD_GF_BankGet
{
	LONGLONG							lGetValue;						//��ȡ���
	BYTE								cbGameAction;					//��Ϸ����
	TCHAR								szPassword[PASS_LEN];			//�û�����
};

//�������
struct	CMD_GF_UserProperty     //�û��������� 
{
	DWORD								dwID;                          //����id
	DWORD							    dwResidualTime;                //���ж��ٸ�����
	DWORD								bUse;                          //�Ƿ��е�������ʹ��
	DWORD								dwResidualTimeCount;           //��ʣ�¶���ʱ�����ʧȥ���� ʱ��Ϊ��
};

struct CMD_GF_ResidualProperty  //�û���������
{
	CMD_GF_UserProperty					dwResidual[PROPERTY_COUNT];	    //ʣ��ʱ��
};


//����˵��
struct CMD_GF_TASK_tagTask
{
	DWORD								dwUserID;						//�û���id
	int									iTaskID;						//����id
	TCHAR								strTitle[24];					//����ı���
	TCHAR								strContext[64];					//���������
	int									iLargessCount1;                 //�����������ı�ʯ
	int									iLargessCount2;                 //�����������ı�ʯ
	int									iLargessCount3;                 //�м��������ı�ʯ
	int									iLargessCount4;                 //�߼��������ı�ʯ
	int									iTaskUserLogID;					//�����¼id
	TCHAR								strUserLogTable[24];			//����ı���
};

//��ɵ�����ṹ
struct CMD_GF_TASK_tagTaskFinish
{
	DWORD                               dwKindID;                       //��Ϸ����id;  m_pGameServiceOption->wKindID
	DWORD                               dwUserID;                       //�û�id      
	DWORD           					dwGems;       					//�����ı�ʯ
    int                                 iTaskID;                        //����id

	int									iTaskUserLogID;					//�����¼id
	TCHAR								strUserLogTable[24];			//����ı���	
	bool                                bFinish;                        //�Ƿ����
};

//�������	Author<cxf>; DateTime<2012-01-16>
struct CMD_GF_NEW_GIFT
{
	DWORD								wUserID;						//�û�ID
	WORD								wGiftType;						//��Ʒ����(1:���ź�;2��������;3:��������)
	int									iGold;							//���ͽ��
	int									iGem;							//���ͱ�ʯ
	int									iOnlineTime;					//����ʱ��
	int									iGrade;							//�ȼ� 
	int									iGiftTime;						//����ʱ��
	DWORD                               dwClientIP;						//�û�ip
	
	CMD_GF_NEW_GIFT()
	{
		wGiftType=0;
		iGold=0;
		iGem=0;
		iOnlineTime=0;
		iGrade=0;
		iGiftTime=0;
	}
};

// �������� - ��ʱ
struct CMD_GF_ONLINE
{
	DWORD								dwChairID;						// ����ID
	int									iToDayOnlineTime;				// ������Ϸʱ��
	BOOL								bStart;							// ��ʼ��ʱ

	CMD_GF_ONLINE()
	{
		ZeroMemory(this, sizeof(*this));
	}
};
//////////////////////////////////////////////////////////////////////////
#endif