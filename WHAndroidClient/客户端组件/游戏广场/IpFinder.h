
#ifndef IP_FINDER_H
#define IP_FINDER_H

#include <string>
#include "atlstr.h"

// ��Ҫ���� ��QQ IP���ݿ� 20120320 ����桿����װ����ȡ qqwry.dat �ļ����������� qqwry__��ɾ.dat


/*		��֪��IP�����ڲ��Դ���
����
	222.240.205.10
	222.240.205.11
	222.240.205.12
	222.240.205.13
	222.240.205.14
	222.240.205.150
	222.240.205.246
��ͨ����ͨ
	58.20.54.237
	58.20.54.238
	58.20.54.239
	58.20.54.240
	58.20.54.241
��ͨ
	59.191.45.0
	59.191.47.255
*/

enum
{
	IP_TYPE_Unknow = 0,				// δ֪
	IP_TYPE_China_Telecom = 1,		// �й�����
	IP_TYPE_China_Unicom = 2,		// �й���ͨ����ͨ����ͨ�Ѿ��ϲ��ˣ���һ����
	IP_TYPE_China_Railcom = 3,		// �й���ͨ
};

const int INDEX_LENGTH = 7;        // һ����������4�ֽڵ���ʼIP��3�ֽڵ�IP��¼ƫ�ƣ���7�ֽ�
const int IP_LENGTH = 4;
const int OFFSET_LENGTH = 3;

enum 
{
	REDIRECT_MODE_1 = 0x01,    // �ض���ģʽ1 ƫ�������޵�����
	REDIRECT_MODE_2 = 0x02,    // �ض���ģʽ2 ƫ�������е�����
};

class CIpFinder
{
public:
	CIpFinder();
	CIpFinder(const char* pszFileName);
	~CIpFinder();

	// ��ȡip��������������
	void GetAddressByIp(unsigned long ipValue, std::string& strCountry, std::string& strLocation) const;
	void GetAddressByIp(const char* pszIp, std::string& strCountry, std::string& strLocation) const;
	void GetAddressByOffset(unsigned long ulOffset, std::string& strCountry, std::string& strLocation) const;

	unsigned long GetString(std::string& str, unsigned long indexStart) const;
	unsigned long GetValue3(unsigned long indexStart) const;
	unsigned long GetValue4(unsigned long indexStart) const;

	// ת��
	unsigned long IpString2IpValue(const char *pszIp) const;
	void IpValue2IpString(unsigned long ipValue, char* pszIpAddress, int nMaxCount) const;
	bool IsRightIpString(const char* pszIp) const;

	// �������
	unsigned long OutputData(const char* pszFileName, unsigned long ulIndexStart = 0, unsigned long ulIndexEnd = 0) const;
	unsigned long OutputDataByIp(const char* pszFileName, unsigned long ipValueStart, unsigned long ipValueEnd) const;
	unsigned long OutputDataByIp(const char* pszFileName, const char* pszStartIp, const char* pszEndIp) const;

	unsigned long SearchIp(unsigned long ipValue, unsigned long indexStart = 0, unsigned long indexEnd = 0) const;
	unsigned long SearchIp(const char* pszIp, unsigned long indexStart = 0, unsigned long indexEnd = 0) const;

	bool Open(const char* pszFileName);

	// ���IP���ͣ�����õ��š���ͨ����ͨ
	int GetIpType(const char* szInfo);

private:

	FILE *m_fpIpDataFile;				// IP���ݿ��ļ�
	unsigned long m_indexStart;    // ��ʼ����ƫ��
	unsigned long m_indexEnd;      // ��������ƫ��
};

#endif