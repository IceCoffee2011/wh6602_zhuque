// AndroidUserInfo.h : AndroidUserInfo DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CAndroidUserInfoApp
// �йش���ʵ�ֵ���Ϣ������� AndroidUserInfo.cpp
//

class CAndroidUserInfoApp : public CWinApp
{
public:
	CAndroidUserInfoApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
