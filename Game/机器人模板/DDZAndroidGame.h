// DDZAndroidGame.h : DDZAndroidGame DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CDDZAndroidGameApp
// �йش���ʵ�ֵ���Ϣ������� DDZAndroidGame.cpp
//

class CDDZAndroidGameApp : public CWinApp
{
public:
	CDDZAndroidGameApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
