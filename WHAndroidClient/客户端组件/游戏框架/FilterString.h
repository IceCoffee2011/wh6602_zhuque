#pragma once
#include "StdAfx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "PublicDef.h"

using namespace std;

class CFilterString
{
public:
	CFilterString(void);
	~CFilterString(void);

public:
	//��ʼ������
	void InitFilterData();
	//�����ַ�
	void SetFilterString(CString& strData);

private:
	std::vector<CString> m_FilterList;
};
