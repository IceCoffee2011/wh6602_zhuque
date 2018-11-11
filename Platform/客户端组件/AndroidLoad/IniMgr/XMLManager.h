#ifndef CCXML_FILE_MANAGER_CREATE_1556_DEF_H_
#define CCXML_FILE_MANAGER_CREATE_1556_DEF_H_

#include "xmlfile.h"
#include <vector>

using namespace std;

//XML�ļ�����
class XML_SCRIPT_EXPORTS CXMLManager
{
public:
	~CXMLManager(void);

public:
	static CXMLManager* GetInstance();

public:
	//��XML�ļ�,������ļ�ָ��.
	CXmlFile* OpenXMLFile(const std::string strFileName);
	//����ָ��XML�ļ���������ļ�ָ��.
	CXmlFile* GetXMLFilePtr(const std::string strFileName);
	//����ָ��XML�ļ������ر�XML�ļ�.
	bool CloseXMLFile(const std::string strFileName);
	//�ر�����XML�ļ�.
	void CloseAllXMLFile();
	//��������XML�ļ�.
	void SaveAllXMLFile();

private:
	CXMLManager(void);

private:
	static CXMLManager* s_Instance;
	std::vector<std::pair<std::string, CXmlFile*> >	m_xmlFileList;
};


#endif