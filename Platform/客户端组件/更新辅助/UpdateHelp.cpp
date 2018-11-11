// UpdateHelp.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "resource.h"
#include <tlhelp32.h>
#include "UpdateHelp.h"
#include <vector>

#define PROJECT_NAME							(_T("cbwUpdateHelp.exe"))	// ���������߳�

// ���ȸ����ļ��ṹ
typedef struct tagPriorityUpdate
{
	string filename;
	string tmpname;
	string path;

	tagPriorityUpdate()
	{
		filename.clear();
		tmpname.clear();
		path.clear();
	}

	tagPriorityUpdate(string filename, string tmpname, string path)
	{
		this->filename = filename;
		this->tmpname = tmpname;
		this->path = path;
	}
}PRIORITYUPDATE;

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	LOG(0, 0, "");
	LOG(0, 0, "");
	LOG(0, 0, "****************** ��ʼ %s �������� ******************", PROJECT_NAME);
	g_hInstance = hInstance;

	TCHAR tchCmd[CMD_LINE_COUNT][MAX_PATH]={0};
	// ��������³����Ӧ����������
	//"/modulefilename:GamePlaza.exe /account:a22222 /pw:dc483e80a7a0bd9ef71d8cf973673924 /updatename:cbwGameUpdate.exe"
	LOG(0, 0, "�����У�[%s]", lpCmdLine);
	GetCmdLinePara(tchCmd, lpCmdLine);

	for (BYTE i=0; i<CMD_LINE_COUNT; i++)
	{
		if (0 >= lstrlen(tchCmd[i]))
			return 0;
	}

	//DebugBreak();//Test

	TCHAR tchRunName[MAX_PATH]={0};
	_snprintf(tchRunName, sizeof(tchRunName), _T("%s"), tchCmd[3]+sizeof("/updatename:")-1);
	
	// �����̴��ڣ���ɱ����
	CheckProcess(tchRunName);

	TCHAR szKeyword[MAX_PATH]=TEXT("");
	TCHAR szServerPath[MAX_PATH]=TEXT("");
	TCHAR tchName[MAX_PATH]=TEXT("");
	TCHAR tchTmpName[MAX_PATH]=TEXT("");
	TCHAR tchPathName[MAX_PATH]=TEXT("");
	CString strAppDirectory = GetAppDirectory(TRUE);
	strAppDirectory.Append(_T("tmpupdate\\DownloadComplete.ini"));

	// ��ȡ���ȸ����ļ��б�
	std::vector<PRIORITYUPDATE> vPriorityUpdateFile;
	int iFlag = 1;
	do
	{
		_snprintf(szKeyword, sizeof(szKeyword), _T("DownloadFile%d"), iFlag++);
		GetPrivateProfileString(szKeyword, _T("filename"), "", tchName, MAX_PATH, strAppDirectory);
		GetPrivateProfileString(szKeyword, _T("tmpfilename"), "", tchTmpName, MAX_PATH, strAppDirectory);
		GetPrivateProfileString(szKeyword, _T("path"), "", tchPathName, MAX_PATH, strAppDirectory);

		if (0 >= lstrlen(tchName)  ||  0 >= lstrlen(tchTmpName)  ||  0 >= lstrlen(tchPathName))
		{
			break;
		}

		vPriorityUpdateFile.push_back(PRIORITYUPDATE(tchName, tchTmpName, tchPathName));
	}while (1);

	// ���������һֱ�����ԣ�ֱ����Ը���ٳ���Ϊֹ
	Sleep(1000);
	std::vector<PRIORITYUPDATE>::iterator vBeg = vPriorityUpdateFile.begin();
	for (; vBeg != vPriorityUpdateFile.end(); ++vBeg)
	{
		// ����Լ����ȸ��£�����������Ϊ�ڸ��½����Ѿ�������
		if (0 == (*vBeg).filename.compare(PROJECT_NAME))
		{
			continue;
		}

		while (1)
		{
			if (CopyFileUpdate((*vBeg).filename, (*vBeg).tmpname, (*vBeg).path))
			{
				LOG(0, 0, "�����ļ���%s", (*vBeg).path.c_str());
				break;
			}

			CString strBuffer;
			strBuffer.Format(_T("[%s]����ʧ�� �ļ����ڱ���һ���˻����ʹ�ã���ر���Ӧ����������һ��\r\n�����޷�����ʱ�����������ԡ�"), 
				(*vBeg).filename.c_str());
			int iReturn = MessageBox(NULL, strBuffer, TEXT("����UpdateHelp.exe��ʾ"), MB_RETRYCANCEL);
			if (IDRETRY == iReturn)
			{
				continue;
			}
			else
			{
				return 0; // ����ʧ�ܣ������ļ���ռ�ã����û�������ˡ�
			}
		}
	}

	//����������
	CString strCommonLine;
	strCommonLine.Format(TEXT("%s %s %s %s"), tchRunName, tchCmd[0], tchCmd[1], tchCmd[2]);
	//������Ϸ�ͻ���
	STARTUPINFO StartInfo;
	memset(&StartInfo,0,sizeof(StartInfo));
	StartInfo.cb=sizeof(StartInfo);
	StartInfo.wShowWindow=SW_SHOWMAXIMIZED;
	BOOL bSuccess = FALSE;
	PROCESS_INFORMATION ProcessInfo;
	memset(&ProcessInfo,0,sizeof(ProcessInfo));

	try
	{
		LOG(0, 0, "�������̣�%s", strCommonLine.GetBuffer());
		strCommonLine.ReleaseBuffer();
		bSuccess = CreateProcess(NULL,strCommonLine.GetBuffer(),NULL,NULL,FALSE,CREATE_DEFAULT_ERROR_MODE,NULL,NULL,
			&StartInfo,&ProcessInfo);
		DWORD dwError = GetLastError();
		CString strBuffer;
		strBuffer.Format("UpdateHelp: %d, ����ID %ld", bSuccess, dwError);
		OutputDebugString(strBuffer);
	}
	catch (...) 
	{ 
		DWORD dwError = GetLastError();
		DebugBreak();
	}

	strCommonLine.ReleaseBuffer();
	LOG(0, 0, "****************** ���� %s �������� ******************", PROJECT_NAME);
	return 0;
}