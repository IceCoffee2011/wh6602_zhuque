#ifndef D3D_ENGINE_HEAD_HEAD_FILE
#define D3D_ENGINE_HEAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����ļ�

//MFC �ļ�
#include <Afxmt.h>
#include <AtlBase.h>
#include <AfxTempl.h>
#include <MMSystem.h>

//D3D �ļ�
#include <D3D9.h>
#include <D3Dx9.h>
#include <DSound.h>

//ƽ̨�ļ�
#include "GameFrame.h"

//////////////////////////////////////////////////////////////////////////
//��������

//��������
#ifndef D3D_ENGINE_CLASS
	#ifdef  D3D_ENGINE_DLL
		#define D3D_ENGINE_CLASS _declspec(dllexport)
	#else
		#define D3D_ENGINE_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define D3D_ENGINE_DLL_NAME		TEXT("D3DEngine.dll")				//�������
#else
	#define D3D_ENGINE_DLL_NAME		TEXT("D3DEngineD.dll")				//�������
#endif

//////////////////////////////////////////////////////////////////////////
//�����ļ�

#ifndef D3D_ENGINE_DLL
	#include "D3DFont.h"
	#include "D3DSound.h"
	#include "D3DEngine.h"
	#include "D3DDirect.h"
	#include "D3DDevice.h"
	#include "D3DSurface.h"
	#include "D3DTexture.h"
	#include "LapseCount.h"
	#include "VirtualEngine.h"
	#include "VirtualWindow.h"
	#include "VirtualButton.h"
#endif

//////////////////////////////////////////////////////////////////////////

#endif