#ifndef			__INTERFACE_GAME_CLIENT_INCL_DEF_1141_CREATE__
#define			__INTERFACE_GAME_CLIENT_INCL_DEF_1141_CREATE__

#include "stdafx.h"


class IGameClientFrame
{
public:
	//�Ƿ񴰿�
	virtual bool __cdecl IsEnableWnd() = 0;
	//�رմ���
	virtual void __cdecl CloseWnd() = 0;
	//����SocketID
	virtual void __cdecl SetMeSocketID(WORD wSocketID) = 0;
	//��ȡ�Լ���Ϣ
	virtual const tagUserData* __cdecl GetMeData() = 0;
	//����Socket��Ϸ�������Ϣ
	virtual bool __cdecl OnSocketMainGameFrame(CMD_Command Command, void * pBuffer, WORD wDataSize) = 0;
	//�û�����
	virtual void __cdecl OnEventUserEnter(const tagUserData* pUserData) = 0;
	//�û��뿪
	virtual void __cdecl OnEventUserLeave() = 0;
	//�û�����
	virtual void __cdecl OnEventUserScore(const IPC_UserScore* pUserScore) = 0;
	//�û�״̬
	virtual void __cdecl OnEventUserStatus(WORD wTableID, WORD wChairID, BYTE cbUserStatus) = 0;
};

#endif