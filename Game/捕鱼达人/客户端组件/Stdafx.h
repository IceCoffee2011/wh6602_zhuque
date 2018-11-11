#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ��ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ�� Windows 95 �� Windows NT 4 ����߰汾���ض����ܡ�
#define WINVER 0x0500		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_WINNT		// ����ʹ�� Windows NT 4 ����߰汾���ض����ܡ�
#define _WIN32_WINNT 0x0400		//Ϊ Windows98 �� Windows 2000 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ�� Windows 98 ����߰汾���ض����ܡ�
#define _WIN32_WINDOWS 0x0410 //Ϊ Windows Me �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ�� IE 4.0 ����߰汾���ض����ܡ�
#define _WIN32_IE 0x0400	//Ϊ IE 5.0 �����°汾�ı�Ϊ�ʵ���ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ��������������ȫ���Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ���ĺͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <afxdisp.h>        // MFC �Զ�����

#include <afxdtctl.h>		// Internet Explorer 4 �����ؼ��� MFC ֧��
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows �����ؼ��� MFC ֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

//////////////////////////////////////////////////////////////////////////

#include "..\��Ϣ����\CMD_Game.h"
#include "..\..\..\������\Include\GameFrame.h"

#pragma comment (lib,"Winmm.lib")

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif


//ȫ�ֱ���
extern TCHAR							g_szWorkDir[MAX_PATH];

////////////////////////////////////////////////////////////////////////////
//��Ϣ����
enum enMessage
{
	enMessage_Begin=WM_USER+100,

	//������Ϣ
	WM_KEY_NUMBER,
	WM_KEY_SWITCH,
	WM_KEY_DELETE,

	//��Ⱥ��Ϣ
	WM_FISH_DISAPPEAR,

	//�ӵ���Ϣ
	WM_BULLET_DISAPPEAR,

	enMessage_End
};

//add by Ivan_han
#define WM_SHOW_COIN_SHOP		WM_USER+101								//����С��� weni23
#define WM_SHOOT				WM_USER+100								//�����ڵ�
#define WM_QUIT_GAME		WM_USER+301								//�˳���Ϸ
#define WM_CAPTURE_FISH				WM_USER+200								//������Ⱥ
#define WM_HIT_FISH					WM_USER+201								//������Ⱥ
#define WM_HIT_Laser				WM_USER+202								//�������

#define WM_FISH_CALC_TRACE				WM_USER+300							//�켣��Ϣ
#define WM_NET_OPEN					WM_USER+310								//����

//�����Ϣ
void TraceString(LPCTSTR pszMessage);
////////////////////////////////////////////////////////////////////////////
