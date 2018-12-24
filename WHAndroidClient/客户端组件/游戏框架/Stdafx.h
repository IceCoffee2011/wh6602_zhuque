#pragma once

#ifndef GAME_FRAME_H
#define GAME_FRAME_H

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#endif
#define _WIN32_WINNT 0x0500	// ���һ��Ҫ���� #include <windows.h> ǰ�棬������벻��ȥ

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ���ض��� Windows 95 �� Windows NT 4 ����߰汾�Ĺ��ܡ�
#define WINVER 0x0501		// ���˸���Ϊ����� Windows 98 �� Windows 2000 ����߰汾�ĺ��ʵ�ֵ��
#endif

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows NT 4 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ���˸���Ϊ����� Windows 2000 ����߰汾�ĺ��ʵ�ֵ��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
#define _WIN32_WINDOWS 0x0501 // ���˸���Ϊ����� Windows Me ����߰汾�ĺ��ʵ�ֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ���ض��� IE 4.0 ����߰汾�Ĺ��ܡ�
#define _WIN32_IE 0x0501	// ���˸���Ϊ����� IE 5.0 ����߰汾�ĺ��ʵ�ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯����Ϊ��ʽ��

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE ��
#include <afxodlgs.h>       // MFC OLE �Ի�����
#include <afxdisp.h>        // MFC �Զ�����
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC ���ݿ���
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO ���ݿ���
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �����ؼ���֧��
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

////����ͷ�ļ�
//#include "..\..\�ͻ������\�ʻ�����\PropertyBar.h"
//#include "..\..\�ͻ������\�ʻ�����\PropertyModule.h"
//#include "..\..\�ͻ������\�ʻ�����\FlowerEffectcontrol.h"

#include "..\..\�������\����ؼ�\SkinControls.h"

//////////////////////////////////////////////////////////////////////////

enum SET_IMAGE_RGN_TYPE
{
	SIRT_TRANS_DOT_LESS = 0,	// ���һ��ͼ������ģ�͸�����ĵ�Ƚ���
	SIRT_SOLID_DOT_LESS = 1,	// ���һ��ͼ����ʵ�ģ���͸�����ĵ�Ƚ���
	SIRT_FULL_DOT_LESS = 2,		// �������ͼ //osc
	SIRT_DEBURRING_DOT_LESS = 3,// ����޼���ͼ			Author:	<cxf>
};

BOOL SetImageRgn(HWND hWnd, CPngImage * pImage, int iType=SIRT_FULL_DOT_LESS);

//struct CSONY
//{
//	CSONY()
//	{
//		iA = 0;
//		iB = 0;
//	}
//	int	iA; 
//	int    iB;
//};

//CSONY stSony;	// ֻ��Ϊ�˼���"��Ϸ�㳡"�Ƿ��ٴΰ����˱��ļ���

#endif