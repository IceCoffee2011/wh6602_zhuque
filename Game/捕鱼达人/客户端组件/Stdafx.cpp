// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// GameClient.pch ����Ԥ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

TCHAR g_szWorkDir[MAX_PATH];

//�����Ϣ
void TraceString(LPCTSTR pszMessage)
{
	try
	{
		//�����ļ���
		TCHAR szPath[MAX_PATH]=TEXT("");
		TCHAR szFileName[MAX_PATH];
		GetCurrentDirectory(CountArray(szPath),szPath);
		_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\ErrorLog.txt"),szPath);

		//���ļ�
		CFile FileRecord;
		if (FileRecord.Open(szFileName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
		{
			FileRecord.SeekToEnd();
			FileRecord.Write(pszMessage,lstrlen(pszMessage));
			FileRecord.Write(TEXT("\r\n"),lstrlen(TEXT("\r\n")));
			FileRecord.Close();
		}
	}
	catch(...){}
}

