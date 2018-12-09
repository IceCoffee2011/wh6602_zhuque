#include "StdAfx.h"
#include "filterstring.h"

CFilterString::CFilterString(void)
{
	m_FilterList.clear();
}

CFilterString::~CFilterString(void)
{
}


//��ʼ������
void CFilterString::InitFilterData()
{
	return;//by hxh
	char buf[1024]={0};                //��ʱ�����ȡ�������ļ�����
	CString message="";
	ifstream infile;
	char chDirWork[1024]={0};
	//���ù���Ŀ¼
	GetModuleFileName(GetModuleHandle(GAME_FRAME_DLL_NAME),chDirWork,sizeof(chDirWork));
	CString strDirWorkPath="";
	strDirWorkPath.ReleaseBuffer();
	strDirWorkPath.Append(chDirWork);
	int nPos = 0; 
	nPos=strDirWorkPath.ReverseFind('\\'); 
	strDirWorkPath=strDirWorkPath.Left(nPos);
	if (strDirWorkPath.IsEmpty())
	{
		strDirWorkPath.ReleaseBuffer();
		strDirWorkPath.Append("KernelEngineString.dll");
	}
	else
	{
		strDirWorkPath.Append("\\KernelEngineString.dll");
	}
	message.ReleaseBuffer();
	infile.open(strDirWorkPath.GetBuffer(), ios::in|ios::binary|ios::_Nocreate|ios::beg, 0);
	if(infile.is_open())          //�ļ��򿪳ɹ�,˵������д�������
	{
		infile.seekg(ios::beg);
		while(infile.good() && !infile.eof())
		{
			memset(buf,0,1024);
			if (infile.getline(buf,1204))
			{
				message.Append(buf);
			}
			else break;
		}
		infile.close();
	}
	if (message.IsEmpty())
	{
		return;
	}
	m_FilterList.clear();
	message.TrimLeft();
	message.TrimRight();
	while(!message.IsEmpty())
	{
		int iIndex = message.Find(',');
		CString strInfo="";
		strInfo = message.Left(iIndex);
		int iSize = strInfo.GetLength();
		message.Delete(0, iSize+1);
		if (!strInfo.IsEmpty() && iSize>0)
		{
			strInfo.TrimLeft();
			strInfo.TrimRight();
			m_FilterList.push_back(strInfo);
		}
	}
}
//�����ַ�
void CFilterString::SetFilterString(CString& strData)
{
	if (strData.IsEmpty())
	{
		return;
	}

	DWORD dwSize = m_FilterList.size();
	for (DWORD i=0; i<dwSize; ++i)
	{
		strData.Replace(m_FilterList[i], "***");
	}
}