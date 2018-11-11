#pragma once

#include "resource.h"
#define			CMD_LINE_COUNT				4						//��������Ŀ
#define			PARAMTER_MAX_COUNT		MAX_PATH		//������ȡ���ֵ
HINSTANCE g_hInstance = NULL;

// ȡ�ó������е�Ŀ¼
// bEndWithBackSlash���Ƿ��Է�б�߽�β
CString GetAppDirectory(BOOL bEndWithBackSlash)
{
	char acBuffer[MAX_PATH];

	memset((void *)acBuffer, 0, MAX_PATH);
	GetModuleFileName(g_hInstance, acBuffer, sizeof(acBuffer));

	char *p = strrchr(acBuffer, '\\');
	if (p != NULL)
	{
		if (bEndWithBackSlash)
		{
			*(p + 1) = '\0';
		}
		else
		{
			*p = '\0';
		}
	}
	else
	{
		p = strrchr(acBuffer, ':');
		if (p != NULL)
		{
			if (bEndWithBackSlash)
			{
				*(p + 1) = '\\';
				*(p + 2) = '\0';
			}
			else
			{
				*p = '\\';
				*(p + 1) = '\0';
			}
		}
	}

	return CString(acBuffer);
}

// ����������
void GetCmdLinePara(TCHAR tchArray[CMD_LINE_COUNT][MAX_PATH], LPTSTR lpCmdLine)
{
	BYTE byArrayIndex = 0;
	CString strLine=lpCmdLine;
	if(strLine.IsEmpty())
		return;

	int nLength=strLine.GetLength();
	char *buf=new char[nLength+1];
	lstrcpyn(buf, strLine, sizeof(char)*(nLength+1));
	char *p=buf;

	for(int i=0;i<PARAMTER_MAX_COUNT;i++)
	{
		if (CMD_LINE_COUNT<=byArrayIndex)
			break;

		if(buf[i]==0x20)//�ո�
		{
			buf[i]=0x00;
			_snprintf(tchArray[byArrayIndex++], sizeof(tchArray[0]), TEXT("%s"), p);
			i++;
			p=buf+i;
		}
		if(buf[i]==0x00)
		{
			_snprintf(tchArray[byArrayIndex++], sizeof(tchArray[0]), TEXT("%s"), p);
			break;
		}
	}

	delete []buf;
}

// �滻·���е������ַ���Ϊʵ��·��
CString ResolvePath(const char* sPath)
{
	if (sPath == NULL)
	{
		DebugBreak();
		return CString("");
	}

	char acBuffer[MAX_PATH];
	CString sDestFilename = sPath;

	sDestFilename.Replace("<INSTDIR>", GetAppDirectory(TRUE).GetBuffer(0));

	memset(acBuffer, 0, MAX_PATH);
	GetWindowsDirectory(acBuffer, MAX_PATH);
	sDestFilename.Replace("<WINDIR>", acBuffer);

	memset(acBuffer, 0, MAX_PATH);
	GetSystemDirectory(acBuffer, MAX_PATH);
	sDestFilename.Replace("<SYSDIR>", acBuffer);

	memset(acBuffer, 0, MAX_PATH);
	SHGetSpecialFolderPath(NULL, acBuffer, 0x0014, FALSE); // CSIDL_FONTS (0x0014)
	sDestFilename.Replace("<FONTS>", acBuffer);

	memset(acBuffer, 0, MAX_PATH);
	SHGetSpecialFolderPath(NULL, acBuffer, 0x0026, FALSE); // CSIDL_PROGRAM_FILES (0x0026)
	sDestFilename.Replace("<PROGRAMFILES>", acBuffer);

	memset(acBuffer, 0, MAX_PATH);
	SHGetSpecialFolderPath(NULL, acBuffer, 0x002b, FALSE); // CSIDL_PROGRAM_FILES_COMMON (0x002b)
	sDestFilename.Replace("<COMMONFILES>", acBuffer);

	memset(acBuffer, 0, MAX_PATH);
	SHGetSpecialFolderPath(NULL, acBuffer, 0x0028, FALSE); // CSIDL_PROFILE (0x0028)
	sDestFilename.Replace("<PROFILE>", acBuffer);

	memset(acBuffer, 0, MAX_PATH);
	SHGetSpecialFolderPath(NULL, acBuffer, CSIDL_PERSONAL, FALSE);
	sDestFilename.Replace("<DOCUMENTS>", acBuffer);

	memset(acBuffer, 0, MAX_PATH);
	SHGetSpecialFolderPath(NULL, acBuffer, CSIDL_DESKTOPDIRECTORY, FALSE);
	sDestFilename.Replace("<DESKTOP>", acBuffer);

	memset(acBuffer, 0, MAX_PATH);
	SHGetSpecialFolderPath(NULL, acBuffer, CSIDL_STARTMENU, FALSE);
	sDestFilename.Replace("<STARTMENU>", acBuffer);

	memset(acBuffer, 0, MAX_PATH);
	SHGetSpecialFolderPath(NULL, acBuffer, CSIDL_PROGRAMS, FALSE);
	sDestFilename.Replace("<SMPROGRAMS>", acBuffer);

	memset(acBuffer, 0, MAX_PATH);
	SHGetSpecialFolderPath(NULL, acBuffer, CSIDL_STARTUP, FALSE);
	sDestFilename.Replace("<SMSTARTUP>", acBuffer);

	//	sDestFilename.Replace("<QUICKLAUNCH>", acBuffer);

	memset(acBuffer, 0, MAX_PATH);
	SHGetSpecialFolderPath(NULL, acBuffer, CSIDL_SENDTO, FALSE);
	sDestFilename.Replace("<SENDTO>", acBuffer);

	memset(acBuffer, 0, MAX_PATH);
	SHGetSpecialFolderPath(NULL, acBuffer, 0x0006, FALSE); // CSIDL_FAVORITES (0x0006)
	sDestFilename.Replace("<FAVORITES>", acBuffer);

	memset(acBuffer, 0, MAX_PATH);
	SHGetSpecialFolderPath(NULL, acBuffer, CSIDL_APPDATA, FALSE);
	sDestFilename.Replace("<APPDATA>", acBuffer);

	memset(acBuffer, 0, MAX_PATH);
	SHGetSpecialFolderPath(NULL, acBuffer, 0x001c, FALSE); // CSIDL_LOCAL_APPDATA (0x001c)
	sDestFilename.Replace("<LOCAL_APPDATA>", acBuffer);

	//	sDestFilename.Replace("<TEMP>", acBuffer);

	return sDestFilename;
}

//�ж�ָ����Ŀ¼�Ƿ����
BOOL IsDirExist(LPCTSTR lpszDir)
{
	BOOL bExist;
	int n;
	CString szTemp = lpszDir;
	CString szDir;
	n = szTemp.ReverseFind('\\');
	if(n<=0)
		return FALSE;
	szDir = szTemp.Left(n);

	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile(szDir,&wfd);
	if(hFind == INVALID_HANDLE_VALUE)
		bExist = FALSE;
	else
	{
		if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			bExist = TRUE;//Ŀ¼����
		else
			bExist = FALSE;//Ŀ¼������
		FindClose(hFind);
	}
	return bExist;
}

// ����·��Ŀ¼
void CreateAllDirectory(LPCTSTR lpszDir)
{
	int nPos = 0;
	CString strTemp = lpszDir;
	if(strTemp.Right(1)!="\\")
		strTemp += "\\";

	//��㴴��Ŀ¼
	while((nPos = strTemp.Find('\\',nPos+1)) != -1)
	{
		if(nPos != 2) //�ų���һ��'\\',(�磺C:\\)
		{
			if(!IsDirExist(strTemp.Left(nPos+1))) //�ж��Ƿ����
				::CreateDirectory(strTemp.Left(nPos+1),NULL);
		}
	}
}

// ���ļ�ȫ·��ȡ���ļ���
CString GetFilename(const char* sFilePath)
{
	if (sFilePath == NULL)
	{
		return CString("");
	}

	const char *pBackSlash = strrchr(sFilePath, '\\');
	if (pBackSlash == NULL)
	{
		return CString(sFilePath);
	}
	else
	{
		return CString(++pBackSlash);
	}
}

CString GetFilename(const CString &sFilePath)
{
	int iPos = sFilePath.ReverseFind('\\');
	if (iPos == -1)
	{
		return sFilePath;
	}
	else
	{
		return sFilePath.Mid(iPos + 1);
	}
}

// �����ļ�
BOOL CopyFileUpdate(const string &strFile, const string &strTmpFile, const string &strPath)
{
	// �õ��ļ�·��
	string::size_type Index = strPath.rfind(strFile);

	CString strAppDirectory = GetAppDirectory(TRUE);
	strAppDirectory.Append(strPath.substr(0, Index-1).c_str());
	strAppDirectory.Replace("\\\\", "\\");

	//�ж�Ŀ¼
	ResolvePath(strAppDirectory);
	CreateAllDirectory(strAppDirectory);
	strAppDirectory.Append("\\");
	strAppDirectory.Append(strFile.c_str());
	strAppDirectory.Replace("\\\\", "\\");

	// ȡ�ñ�ģ���ļ�����Ҫ���Ƶ��ļ���
	char acBuffer[MAX_PATH] = {0};	
	GetModuleFileName(g_hInstance, acBuffer, sizeof(acBuffer));
	string strModuleNameOne = GetFilename(acBuffer);
	string strModuleNameTwo = strModuleNameOne;
	strModuleNameTwo.insert(0, "\\");
	string strTmpPath(strPath);
	transform(strModuleNameOne.begin(), strModuleNameOne.end(), strModuleNameOne.begin(), towlower);
	transform(strModuleNameTwo.begin(), strModuleNameTwo.end(), strModuleNameTwo.begin(), towlower);
	transform(strTmpPath.begin(), strTmpPath.end(), strTmpPath.begin(), towlower);

	//�������ػ����е��ļ����ļ�Ŀ���ַ
	CString strExistingFile = GetAppDirectory(TRUE)+_T("tmpupdate");
	strExistingFile.Append("\\");
	strExistingFile.Append(strFile.c_str());
	strExistingFile.Append(strTmpFile.c_str());	

	// �����ļ��뱾ģ����ͬ������ȡһ�������������г������к���ɾ������
	if (strPath==strModuleNameOne || strPath==strModuleNameTwo)
	{
		DebugBreak();
	}

	ofstream filestream;
	filestream.open(strExistingFile, ios::in);
	if (filestream)
	{
		SetFileAttributes(strAppDirectory, FILE_ATTRIBUTE_NORMAL);
		DeleteFile(strAppDirectory);
		CopyFile(strExistingFile, strAppDirectory, FALSE);
		filestream.close();
	}
	
	return true;
}


// ��¼��־����
int g_iLogLevel;

// ��ʾ��־���ı���
//extern CRichEditCtrl *g_pLogViewer;

// ��־��ʽ[ʱ��][ģ��ID][��־����][��־����]
#define LOG_FORMAT "[%s][%d][%d][%s]\r\n"
// ��־��������С
#define LOG_BUFFER_SIZE 1024
// д��־
// iModuleID��ģ��ID
// iLevel����־����
// szFormat����־��ʽ
// ...����־����
void LOG(int iModuleID, int iLevel, const char* sFormat, ...)
{
	//#ifdef _DEBUG
	// ֻ��¼������ָ����־�������ϵ���־
	if (iLevel < g_iLogLevel)
	{
		return;
	}

	char *pBuffer1 = new char[LOG_BUFFER_SIZE];
	char *pBuffer2 = new char[LOG_BUFFER_SIZE];
	memset(pBuffer1, 0, LOG_BUFFER_SIZE);
	memset(pBuffer2, 0, LOG_BUFFER_SIZE);

	va_list args;
	va_start(args, sFormat);
	_vsnprintf(pBuffer1, LOG_BUFFER_SIZE, sFormat, args);
	va_end(args);

	CString sNow = CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
	if (0 < strlen(pBuffer1))
	{
		_snprintf(pBuffer2, LOG_BUFFER_SIZE, LOG_FORMAT, sNow.GetBuffer(0), iModuleID, iLevel, pBuffer1);
	}
	else
	{
		memset(pBuffer2, 0, LOG_BUFFER_SIZE);
		_snprintf(pBuffer2, LOG_BUFFER_SIZE, TEXT("\r\n"));
	}

	// д���ļ�
	ofstream appendFile;
	CreateDirectory(GetAppDirectory(TRUE) + "LOG\\GameUpdate.log", NULL);
	appendFile.open(GetAppDirectory(TRUE) + "LOG\\GameUpdate.log", ios::binary | ios::app | ios::out );	

	if(appendFile)
	{
		appendFile.write(pBuffer2, strlen(pBuffer2));
		appendFile.close();
	}


	// �ͷ���ʱ������ڴ�
	delete []pBuffer1;
	delete []pBuffer2;
	pBuffer1 = NULL;
	pBuffer2 = NULL;

	OutputDebugString("GameUpdateDlg.cpp,�뿪�ؼ������, LOG,Guard.leave();");//Test
	//#endif
}

void CheckProcess(LPCTSTR lpProcessName)
{
	HANDLE hProcessSnap = NULL; //��ʼ�� 
	PROCESSENTRY32 pe32 = {0}; 
	int count = 0; 
	pe32.dwSize = sizeof(PROCESSENTRY32); 
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);  //��þ�� 

	if (hProcessSnap == (HANDLE)-1)
		return ; 

	//�о����н��� 
	if (Process32First(hProcessSnap, &pe32)) 
	{  
		do  
		{
			//���������� GamePlace.exe �Ƚ� �� ���˳� û�м������� 
			if (0 == lstrcmp(lpProcessName, pe32.szExeFile)) 
			{
				count++;  
				// ��������Ӧ�Ĳ���

				// ������ǿɱ���̵Ĳ���
				bool bFlag = false;

				HANDLE hProcess = NULL; 
				DWORD dwExitCode = 0 ;
				hProcess = OpenProcess (PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID); // ���ݽ���ID��ý��̾��

				if(hProcess)
				{
					bFlag = GetExitCodeProcess(hProcess, &dwExitCode);
					if(bFlag)
					{
						bFlag = TerminateProcess(hProcess, dwExitCode);
					}
				}
			}
		} 
		while (Process32Next(hProcessSnap, &pe32));//ֱ���о���� 
	}

	CloseHandle(hProcessSnap);  //�رվ��
}