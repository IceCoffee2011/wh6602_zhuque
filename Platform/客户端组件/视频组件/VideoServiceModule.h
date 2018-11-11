#ifndef VIDEO_SERVICE_HEAD_FILE
#define VIDEO_SERVICE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////



#ifndef  GAME_LOAD_DLL
		#include "..\..\ģ���\Template.h"
		#include "..\..\�����ļ�\GlobalDef.h"
		#include "..\..\��Ϣ����\CMD_Video.h"
		#include "..\..\ģ���\Module.h"
		#include "..\..\�������\����ؼ�\SkinControls.h"
		#include "..\..\�ͻ������\��Ϸ���\GameFrame.h"
		#include "..\..\�ͻ������\�ʻ�����\FlowerEffectcontrol.h"
#else
		#include "Template.h"
		#include "GlobalDef.h"
		#include "CMD_Video.h"
		#include "Module.h"
		#include "SkinControls.h"
		#include "GameFrame.h"
		#include "FlowerEffectcontrol.h"
#endif


//////////////////////////////////////////////////////////////////////////
//�����궨��

//��������
#ifndef VIDEO_SERVICE_CLASS
	#ifdef  VIDEO_SERVICE_DLL
		#define VIDEO_SERVICE_CLASS _declspec(dllexport)
	#else
		#define VIDEO_SERVICE_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define VIDEO_SERVICE_DLL_NAME	TEXT("VideoService.dll")			//��� DLL ����
#else
	#define VIDEO_SERVICE_DLL_NAME	TEXT("VideoServiceD.dll")			//��� DLL ����
#endif

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//������Ϣ
struct tagVideoNetwordInfo
{
	WORD							wNatPort;							//����˿�
	WORD							wLocalPort;							//����˿�
	DWORD							dwNatAddr;							//�����ַ
	DWORD							dwLocalAddr;						//���ص�ַ
};

//////////////////////////////////////////////////////////////////////////

//������ͷ�ļ�

#include "VideoServiceControl.h"

//////////////////////////////////////////////////////////////////////////

#endif
