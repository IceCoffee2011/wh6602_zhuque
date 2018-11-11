#include "StdAfx.h"
#include "readfixcard.h"

CReadFixCard::CReadFixCard(BYTE byAllCardNumber,BYTE byHeadSize)
:m_byAllCardNumber(byAllCardNumber),m_byHeadSize(byHeadSize)
{
	memset(m_byHeads, 0, sizeof(m_byHeads));

	fnSetHexStringMap();	// ����ʮ�������ַ��� Map �� 
}

CReadFixCard::CReadFixCard()
{
	memset(m_byHeads, 0, sizeof(m_byHeads));
	m_byAllCardNumber = 0;
	m_byHeadSize = 0;

	fnSetHexStringMap();	// ����ʮ�������ַ��� Map �� 
}

CReadFixCard::~CReadFixCard(void)
{
}

// ��ʼ��
void CReadFixCard::fnInit(BYTE byAllCardNumber,BYTE byHeadSize)
{
	m_byAllCardNumber = byAllCardNumber;
	m_byHeadSize = byHeadSize;
}

/* --------------------------------------------------------------------------
����˵������λʮ�����Ƶ��ִ�ת������
���������strTwoBit ����ʽΪ 0x0A �� 0A
����ֵ������ת�õ�����
--------------------------------------------------------------------------*/
int CReadFixCard::fnStringToByte(string strTwoBit)
{
	using namespace std ;
	
	BYTE byHeight = 0;
	BYTE byLow = 0;
	if(strTwoBit.size()==4)	// ��ʽΪ 0x0A
	{
		byHeight = hexStringMap[strTwoBit[2]];
		byLow = hexStringMap[strTwoBit[3]];
	}
	else if(strTwoBit.size()==2)	// ��ʽΪ 0A
	{
		byHeight = hexStringMap[strTwoBit[0]];
		byLow = hexStringMap[strTwoBit[1]];
	}
	else
	{
		return -1;
	}

	byHeight = byHeight<<4;

	return (int)(byHeight + byLow);
}

// ����ʮ�������ַ��� Map �� 
void CReadFixCard::fnSetHexStringMap()
{
	hexStringMap['0'] = 0;
	hexStringMap['1'] = 1;
	hexStringMap['2'] = 2;
	hexStringMap['3'] = 3;
	hexStringMap['4'] = 4;
	hexStringMap['5'] = 5;
	hexStringMap['6'] = 6;
	hexStringMap['7'] = 7;
	hexStringMap['8'] = 8;
	hexStringMap['9'] = 9;

	hexStringMap['A'] = 10;
	hexStringMap['B'] = 11;
	hexStringMap['C'] = 12;
	hexStringMap['D'] = 13;
	hexStringMap['E'] = 14;
	hexStringMap['F'] = 15;

	hexStringMap['a'] = 10;
	hexStringMap['b'] = 11;
	hexStringMap['c'] = 12;
	hexStringMap['d'] = 13;
	hexStringMap['e'] = 14;
	hexStringMap['f'] = 15;
}

//���ļ�����ֵд���˿�����
BOOL CReadFixCard::fnReadFile(const char* pFileName)
{
	using namespace std ;
	
	if (0==m_byAllCardNumber)
		return FALSE;

	CFile f;
	CFileException e;
	//char* pFileName = "RedA.txt";
	if( !f.Open( pFileName, CFile::modeRead , &e ) )
	{
		return false;
	}

	BOOL bRemark = false;	// ע��
	BYTE buffer[MAX_READ_BUFFER];
	UINT  uReadSize = 0;
	uReadSize = f.Read(buffer, MAX_READ_BUFFER);

	BYTE *p = buffer;
	string sTemp("this is");	
	sTemp.clear();
	for(UINT i=0; i<uReadSize; i++)
	{
		if(p[i]==F_RETURN_CAR || p[i+1]==F_EXCHANGE_LINE)
			bRemark = FALSE;

		if (bRemark)
			continue;

		if(p[i]=='/' && p[i+1]=='/')
		{
			bRemark = TRUE;
			continue;
		}
		else if(p[i]==F_SPACE || p[i]==F_TAB || p[i]==F_DOT)
		{
			if (!sTemp.empty())
				vec.push_back(sTemp);
			sTemp.clear();
		}
		else if(p[i]==F_RETURN_CAR || p[i+1]==F_EXCHANGE_LINE)
		{
			if (!sTemp.empty())
				vec.push_back(sTemp);
			sTemp.clear();
		}
		else
		{
			if(p[i]!=F_SPACE && p[i]!=F_RETURN_CAR && p[i]!=F_EXCHANGE_LINE && p[i]!=F_TAB && p[i]!=F_DOT)
				sTemp += p[i];
		}
	}

	if (!sTemp.empty())
		vec.push_back(sTemp);

	int maxSize = (int)vec.size();
	for(int i=0 ;i<maxSize; i++)
	{
		string sTemp = vec[i];
		int iTemp = atoi(sTemp.c_str());
	}

	p = NULL;
	f.Close();

	// ��ͷ
	for(int i=0; i<m_byHeadSize; i++)
	{
		string sTemp = vec[i];
		m_byHeads[i] = atoi(sTemp.c_str());
	}

	int iTemp = 0;

	return true;
}

/* --------------------------------------------------------------------------
����˵����������
������������
����ֵ�������Ƶ�����
--------------------------------------------------------------------------*/
int CReadFixCard::fnGetCards(BYTE* byCards)
{
	for(int i=0; i<m_byAllCardNumber; i++)
	{
		// ����
		BYTE byTemp = fnStringToByte(vec[i + m_byHeadSize + m_byHeads[0]*m_byAllCardNumber ]);
		byCards[i] = byTemp;
	}
	return m_byAllCardNumber;
}

/* --------------------------------------------------------------------------
����˵���������ļ���ͷ
����������ͷ
����ֵ������ͷ������
--------------------------------------------------------------------------*/
int CReadFixCard::fnGetHead(BYTE* byHeads)
{
	memcpy(byHeads, m_byHeads, sizeof(BYTE)*m_byHeadSize);
	return 0;
}