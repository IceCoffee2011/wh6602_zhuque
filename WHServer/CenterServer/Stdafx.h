#pragma once

//////////////////////////////////////////////////////////////////////////
//MFC �ļ�

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WINVER
#define WINVER 0x0500
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif						

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0400
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

#define _AFX_ALL_WARNINGS

#include <AfxWin.h>
#include <AfxExt.h>
#include <AfxDisp.h>
#include <AfxDtctl.h>

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <AfxCmn.h>
#endif

//////////////////////////////////////////////////////////////////////////

//ȫ���ļ�
#include "..\..\Common\GlobalField.h"
#include "..\..\Common\GlobalService.h"

//���ͷ�ļ�
#include "..\ListService\ListService.h"
#include "..\KernelEngine\KernelEngineHead.h"

//��Ϣ����
#include "..\..\Common\CMD_Center.h"


#include "..\CommonServices\WinIni.h"
//////////////////////////////////////////////////////////////////////////
