// AndroidGameFrame.h : AndroidGameFrame DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CAndroidGameFrameApp
// �йش���ʵ�ֵ���Ϣ������� AndroidGameFrame.cpp
//

class CAndroidGameFrameApp : public CWinApp
{
public:
	CAndroidGameFrameApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
