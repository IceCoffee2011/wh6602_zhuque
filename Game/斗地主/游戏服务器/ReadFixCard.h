#ifndef READ_FIXCARD_HEAD_FILE
#define READ_FIXCARD_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include <string>
#include <algorithm>
#include <map>
#include <vector>
using namespace std ;  // ��ǰһ��Ҫ��

#define MAX_READ_BUFFER		4096
#define F_TAB				0x09	// Tab 
#define F_DOT				0x2C	// ��Ӣ�Ķ���
#define F_SPACE				0x20	// �ո�
#define F_RETURN_CAR		0x0D	// �س�
#define F_EXCHANGE_LINE		0x0A	// ����

class CReadFixCard
{
public:
	CReadFixCard();
	CReadFixCard(BYTE byAllCardNumber,BYTE byHeadSize) ;
	~CReadFixCard(void);
	
	// ��ʼ��
	void fnInit(BYTE byAllCardNumber,BYTE byHeadSize);

	// ��λʮ�����Ƶ��ִ�ת������
	int fnStringToByte(string strTwoBit);

	// ����ʮ�������ַ��� Map �� 
	void fnSetHexStringMap();

	//���ļ�����ֵд���˿�����
	BOOL fnReadFile(const char* pFileName);
	
	// �����ļ���ͷ
	int fnGetHead(BYTE* byHeads);
	
	// ������
	int fnGetCards(BYTE* byCards);

	vector<string> vec;
	map<char,BYTE> hexStringMap;

public:
	BYTE m_byAllCardNumber;	// ���Ƶ�����
	BYTE m_byHeadSize;		// ͷ�ĳ���

	BYTE m_byHeads[10];
};

#endif
