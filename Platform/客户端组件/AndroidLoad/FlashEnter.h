// FlashEnter.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CFlashEnterApp:
// �йش����ʵ�֣������ FlashEnter.cpp
//

class CFlashEnterApp : public CWinApp
{
public:
	CFlashEnterApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CFlashEnterApp theApp;
