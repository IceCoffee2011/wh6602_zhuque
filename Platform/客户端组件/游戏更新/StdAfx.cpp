// stdafx.cpp : source file that includes just the standard includes
//	GameUpdate.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "GameUpdate.h"
#include "RMessageBox.h"

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
CCriticalSpection criticalLog; 
void LOG(int iModuleID, int iLevel, const char* sFormat, ...)
{
	if (FALSE == g_IsWriteLog)
		return;

	OutputDebugString("GameUpdateDlg.cpp,����ؼ������, LOG,Guard(&criticalLog);");//Test
	CThreadGuard Guard(&criticalLog);
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

	// ������ı���
//	g_pLogViewer->SetSel(-1, 0);
//	g_pLogViewer->ReplaceSel(pBuffer2 + CString("\n"));

	// д���ļ�
	CFile file;
	CreateDirectory(GetAppDirectory() + "LOG", NULL);
	file.Open(GetAppDirectory() + "LOG\\GameUpdate.log"
		, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
	file.SeekToEnd();
	file.Write(pBuffer2, (UINT)strlen(pBuffer2));
	file.Close();

	// �ͷ���ʱ������ڴ�
	delete []pBuffer1;
	delete []pBuffer2;
	pBuffer1 = NULL;
	pBuffer2 = NULL;

	OutputDebugString("GameUpdateDlg.cpp,�뿪�ؼ������, LOG,Guard.leave();");//Test
//#endif
}

// ȡ�ó������е�Ŀ¼
// bEndWithBackSlash���Ƿ��Է�б�߽�β
CString GetAppDirectory(BOOL bEndWithBackSlash)
{
	char acBuffer[MAX_PATH];

	memset((void *)acBuffer, 0, MAX_PATH);
	GetModuleFileName(AfxGetInstanceHandle(), acBuffer, sizeof(acBuffer));

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

// ȡ���ļ���չ��������'.'�ַ���
CString GetFileExtendName(const char* sFilename)
{
	if (sFilename == NULL)
	{
		return CString("");
	}

	const char *pExtendName = strrchr(sFilename, '.');
	if (pExtendName == NULL)
	{
		return CString("");
	}
	else
	{
		return CString(++pExtendName);
	}
}

// ȡ���ļ���չ��������'.'�ַ���
CString GetFileExtendName(const CString& sFilename)
{
	int iPos = sFilename.ReverseFind('.');
	if (iPos == -1)
	{
		return CString("");
	}
	else
	{
		return sFilename.Mid(iPos + 1);
	}
}

// ȡ���ļ���С
size_t GetFileSize(const char* sFilename)
{
	CFileFind FileFinder;

	if (sFilename == NULL)
	{
		return 0;
	}

	if (FileFinder.FindFile(sFilename))
	{
		FileFinder.FindNextFile();
		return (size_t)FileFinder.GetLength();
	}
	else
	{
		return 0;
	}
}

// ȡ���ļ���С
size_t GetFileSize(const CString& sFilename)
{
	CFileFind FileFinder;

	if (FileFinder.FindFile(sFilename))
	{
		FileFinder.FindNextFile();
		return (size_t)FileFinder.GetLength();
	}
	else
	{
		return 0;
	}
}

// ��ʾ��Ϣ��
int RMessageBox(CString& sText, UINT nType, UINT nIDHelp)
{
	return RMessageBox(sText.GetBuffer(0), nType, nIDHelp);
}

// ��ʾ��Ϣ��
int RMessageBox(LPCTSTR lpszText, UINT nType, UINT nIDHelp)
{
	ASSERT(lpszText);

	CRMessageBox mb;

	mb.m_sTitle = AfxGetApp()->m_pszAppName;
	mb.m_iType = nType;
	mb.m_sPromptMessage = lpszText;
	mb.DoModal();
	return mb.m_iID;
}

// �滻·���е������ַ���Ϊʵ��·��
CString ResolvePath(const char* sPath)
{
	if (sPath == NULL)
	{
		ASSERT(FALSE);
		return CString("");
	}

	char acBuffer[MAX_PATH];
	CString sDestFilename = sPath;

	sDestFilename.Replace("<INSTDIR>", GetAppDirectory().GetBuffer(0));

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

string& replace_all(string& str, const string& old_value, const string& new_value)
{
	while (true)
	{
		string::size_type pos(0);
		if( string::npos != (pos=str.find(old_value)) )
			str.replace(pos,old_value.length(),new_value);
		else
			break;
	}

	return str;
}