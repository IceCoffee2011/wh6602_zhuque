#ifndef ___GAME_CLIENT_DDZ_ANDROID_CREATE_16_51_DEF_H___
#define ___GAME_CLIENT_DDZ_ANDROID_CREATE_16_51_DEF_H___

#include "..\..\������\Include\AndroidGameFrameEx.h"
#include "..\..\������\Include\IGameClientFrame.h"

extern "C" ANDROID_GAME_FRAME_EX IGameClientFrame* NewGameClient(CWnd* pParent, DWORD dwWndID);
extern "C" ANDROID_GAME_FRAME_EX void FreeGameClient(IGameClientFrame* pClient);


#endif