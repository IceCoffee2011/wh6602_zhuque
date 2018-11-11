// UpdateThread.cpp : implementation file
//

#include "stdafx.h"
#include "GameUpdate.h"
#include "UpdateThread.h"
#include "GameUpdateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//const int BUFFER_SIZE = 512;
/////////////////////////////////////////////////////////////////////////////
// CUpdateThread

IMPLEMENT_DYNCREATE(CUpdateThread, CWinThread)

CUpdateThread::CUpdateThread()
{
	m_bSilenceMode = FALSE;
	m_bUserBreak = FALSE;
	m_hProgressWindow = NULL;
	m_setWantUpdateFile.clear();
	m_setDowndloadFile.clear();
	m_CompleteDownByte = 0;
}

CUpdateThread::~CUpdateThread()
{
}

BOOL CUpdateThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CUpdateThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	m_bUserBreak = TRUE;

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CUpdateThread, CWinThread)
	//{{AFX_MSG_MAP(CUpdateThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpdateThread message handlers

int CUpdateThread::Run() 
{
	if (GENERATE == m_StartOperator)
	{
		DoGenerateMD5Hash();
		return 1;
	}
	else if (GENERATE_RUN == m_StartOperator)
	{
		DoGenerateMD5Hash(TRUE);
	}
	else if (ONLY_RUN != m_StartOperator)
	{
		return 1;
	}

	CheckWantUpdateFile();
	if (DoUpdateEx())
	{
		if (m_hProgressWindow)
		{
			SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_FINISH_UPDATE, (LPARAM)1);
		}
		return 1;
	}
	else
	{
		if (m_hProgressWindow)
		{
			SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_FINISH_UPDATE, (LPARAM)0);
		}
		return 0;
	}

//	return CWinThread::Run();
}

// ִ������
BOOL CUpdateThread::DoUpdateEx()
{
	if (m_setWantUpdateFile.empty())
	{
		LOG(0, 0, "Error: ��Ҫ���µ��ļ��б�Ϊ�� ������Ӧ����:m_setWantUpdateFile,������:CUpdateThread::DoUpdateEx()");
		return FALSE;
	}

	char acBuffer[BUFFER_SIZE];
	DOWNLOAD_INFO_STRU DownloadInfo;

	for (map<string, FileHashTable>::iterator beg=m_setWantUpdateFile.begin(); beg!=m_setWantUpdateFile.end(); beg++)
	{
		DownloadInfo.iFileCount++;
		DownloadInfo.iFileSize += beg->second.size;
	}

	// ���ļ��������ļ���С�ܺ��ϱ�������
	if (!m_bSilenceMode && m_hProgressWindow != NULL)
		SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_DOWNLOAD_INFO, (LPARAM)&DownloadInfo);


	// �����ļ�
	UINT uFileCount=m_setDowndloadFile.size();
	char cFileCount[MAX_PATH]={0};
	memset(&DownloadInfo, 0, sizeof(DOWNLOAD_INFO_STRU));
	CString szPath(GetAppDirectory());
	TCHAR chBuffer[MAX_PATH]={0}, chKeyName[MAX_PATH]={0}, chFileSzie[MAX_PATH]={0};
	
	for (map<string, FileHashTable>::iterator beg=m_setWantUpdateFile.begin(); beg!=m_setWantUpdateFile.end(); beg++)
	{
		if (m_bUserBreak)
			return FALSE;

		//�����ļ��Ƿ��Ѿ����أ� ����������������
		map<string, FileHashTable>::iterator mLocal = m_setDowndloadFile.begin(); 
		for (; mLocal != m_setDowndloadFile.end(); mLocal++)
		{
			if (0 == beg->second.path.compare(mLocal->second.path))
			{
				break;
			}
		}

		// �ļ���������Ҫ������
		if (mLocal != m_setDowndloadFile.end())
		{
			//��ʱ��չ����Ȼ��֮ǰ����ʱȡ��
			beg->second.tmpFileName = mLocal->second.tmpFileName;
			continue;
		}


		_snprintf(cFileCount, sizeof(cFileCount), TEXT(".tmp%d"), ++uFileCount);
		beg->second.tmpFileName = (char *)cFileCount;

		if (!DownloadFile(beg->second.filename, beg->second.tmpFileName, beg->second.path))
		{
			// �ļ�����ʧ�ܣ�����ʧ��
			if (!m_bSilenceMode && m_hProgressWindow != NULL)
			{
				memset(acBuffer, 0, BUFFER_SIZE);
				_snprintf(acBuffer, sizeof(acBuffer), "%s", beg->first.c_str());
				SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_DOWNLOAD_FILE_FAIL, (LPARAM)acBuffer);
			}

			LOG(0, 0, "Error: ����ʧ�� %s, ������:CUpdateThread::DoUpdateEx()", beg->first.c_str());
			//// ĳ���ļ�����ʧ�ܣ���ʾ������������������˳�����
			//CString strBuffer;
			//strBuffer.Format("[%s]�ļ�����ʧ��!\r\n�����Գ��Ե����ȷ�������������棬���ߡ�ȡ������θ���", beg->first.c_str());
			//int iResult = RMessageBox(strBuffer, MB_OKCANCEL);
			//if (IDOK == iResult)
			//{
			//	TCHAR tchDowndPath[MAX_PATH];
			//	_snprintf(tchDowndPath, sizeof(tchDowndPath), _T("%s/download.php"), g_pCGameUpdateDlg->m_stTGS.szWeb);
			//	ShellExecute(NULL, "open", tchDowndPath, NULL, NULL, SW_SHOWNORMAL);
			//}

			//g_pCGameUpdateDlg->PostMessage(WM_CLOSE);
			return FALSE;
		}

		// �����������ϱ�������
		DownloadInfo.iFileCount++;
		DownloadInfo.iFileSize += beg->second.size;
		if (!m_bSilenceMode && m_hProgressWindow != NULL)
		{
			SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_DOWNLOADED_INFO, (LPARAM)&DownloadInfo);
		}

		//��¼������ɵ��ļ���Ϊ����׼��
		_snprintf(chBuffer, sizeof(chBuffer), TEXT("%s\\%s\\DownloadComplete.ini"), szPath, DOWNDLOAD_TMP_PATH);
		_snprintf(chKeyName, sizeof(chKeyName), TEXT("DownloadFile%d"), uFileCount);
		WritePrivateProfileString(chKeyName, TEXT("filename"), beg->second.filename.c_str(), chBuffer);
		WritePrivateProfileString(chKeyName, TEXT("tmpfilename"), beg->second.tmpFileName.c_str(), chBuffer);
		WritePrivateProfileString(chKeyName, TEXT("datname"), beg->second.DATName.c_str(), chBuffer);
		WritePrivateProfileString(chKeyName, TEXT("path"), beg->second.path.c_str(), chBuffer);
		WritePrivateProfileString(chKeyName, TEXT("hash"), beg->second.Hash.c_str(), chBuffer);
		_snprintf(chFileSzie, sizeof(chFileSzie), "%I64u", beg->second.size);
		WritePrivateProfileString(chKeyName, TEXT("size"), chFileSzie, chBuffer);
	}


	// ������Ϻ�У���ļ�
	for (map<string, FileHashTable>::iterator beg=m_setWantUpdateFile.begin(); beg!=m_setWantUpdateFile.end(); beg++)
	{
		if (m_bUserBreak)
			return FALSE;

		
		// У��ʧ�ܣ�Ĭ�������´�����
		int iCount=0;
		bool bVerifySuccess = false;
		do
		{
			if (VerifyFile(beg->second.filename, beg->second.tmpFileName, beg->second.Hash))
			{
				bVerifySuccess = true;
				break;
			}

			iCount++;
			DownloadFile(beg->second.filename, beg->second.tmpFileName, beg->second.path);
		}while (3 >= iCount);


		if (!bVerifySuccess && !VerifyFile(beg->second.filename, beg->second.tmpFileName, beg->second.Hash))
		{
			// �ļ�У�鲻ͨ��������ʧ��
			if (!m_bSilenceMode && m_hProgressWindow != NULL)
			{
				memset(acBuffer, 0, BUFFER_SIZE);
				_snprintf(acBuffer, sizeof(acBuffer), "%s", beg->first.c_str());
				SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_VERIFY_FILE_FAIL, (LPARAM)acBuffer);
			}

			//// ĳ���ļ�У��ʧ�ܣ���ʾ������������������˳�����
			//CString strBuffer;
			//strBuffer.Format("[%s]�ļ����غ�У��ʧ��!\r\n�����Գ��Ե����ȷ�������������棬���ߡ�ȡ������θ���", beg->first.c_str());
			//int iResult = RMessageBox(strBuffer, MB_OKCANCEL);
			//if (IDOK == iResult)
			//{
			//	TCHAR tchDowndPath[MAX_PATH];
			//	_snprintf(tchDowndPath, sizeof(tchDowndPath), _T("%s/download.php"), g_pCGameUpdateDlg->m_stTGS.szWeb);
			//	ShellExecute(NULL, "open", tchDowndPath, NULL, NULL, SW_SHOWNORMAL);
			//}

			//g_pCGameUpdateDlg->PostMessage(WM_CLOSE);
			return FALSE;
		}
	}

	
	//�������Ƿ��Ѿ��ر�
	CWnd *pWnd = NULL;
	while (true)
	{
		HWND hMain = ::FindWindow(TEXT("QPGamePlaza"), NULL);
		if (hMain != NULL)
		{
			int iResult = RMessageBox(_T("���ȹر���Ϸ�����������ԣ�"), 0x00000007L);
			if (IDCANCEL == iResult)
			{
				g_pCGameUpdateDlg->PostMessage(WM_CLOSE);
				return FALSE;
				
			}
			else if (IDYES == iResult)
			{
				PostMessage(hMain, WM_CLOSE, 0, 0);
				Sleep(100);
				continue;
			}
			else
			{
				continue;
			}
		}
		else
			break;
	}


	// ѭ�����������ļ�
	for (map<string, FileHashTable>::iterator beg=m_setWantUpdateFile.begin(); beg!=m_setWantUpdateFile.end(); beg++)
	{
		if (m_bUserBreak)
			return FALSE;

		if (!UpdateFile(beg->second.filename, beg->second.tmpFileName, beg->second.path))
		{
			CString strName;
			TCHAR szPath[MAX_PATH]=TEXT("");
			GetModuleFileName(AfxGetInstanceHandle(), szPath, sizeof(szPath));
			strName = GetFilename(szPath);

			// ��ռ�õ��ļ��Ǹ��³���ʱ���������³���ռ���޷������Ѿ��ڴ��������
			if (0 != strName.CompareNoCase(beg->second.filename.c_str()))
			{
				// �ļ����²��ɹ�������ʧ��
				if (!m_bSilenceMode && m_hProgressWindow != NULL)
				{
					memset(acBuffer, 0, BUFFER_SIZE);
					_snprintf(acBuffer, sizeof(acBuffer), "%s", beg->first.c_str());
					SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_UPDATE_FILE_FAIL, (LPARAM)acBuffer);
				}

				// ���������һֱ�����ԣ�ֱ����Ը���ٳ���Ϊֹ
				while (1)
				{
					CString strBuffer;
					strBuffer.Format(_T("[%s]����ʧ�� �ļ����ڱ���һ���˻����ʹ�ã���ر���Ӧ����������һ��\r\n�����޷�����ʱ�����������ԡ�"), beg->second.filename.c_str());
					int iReturn = RMessageBox(strBuffer, MB_RETRYCANCEL);
					if (IDRETRY == iReturn)
					{
						if (UpdateFile(beg->second.filename, beg->second.tmpFileName, beg->second.path))
							break;
					}
					else
					{
						g_pCGameUpdateDlg->PostMessage(WM_CLOSE);
						return FALSE;
					}
				}
			}
		}
	}


	CString strAppDirectory = GetAppDirectory();
	strAppDirectory.Append(DOWNDLOAD_TMP_PATH);
	strAppDirectory.Replace("\\\\", "\\");
	CString strIniFilePath = GetAppDirectory() + CONFIG_PATH + "\\" + UPDATE_CONFIG_FILENAME;
	strIniFilePath.Replace("\\\\", "\\");
	
	// ��������MD5 Hash �����ļ�
	if (FALSE == m_bUserBreak)
	{
		SetFileAttributes(strIniFilePath, FILE_ATTRIBUTE_NORMAL);
		DeleteFile(strIniFilePath);
		MoveFile(strAppDirectory+"\\"+UPDATE_CONFIG_TMP_FILENAME, strIniFilePath);
	}

	// ɾ��������ʹ�õĻ���Ŀ¼
	if (FALSE == m_bUserBreak)
	{
		CDirOperate::DeleteAllDirectory(strAppDirectory);
	}


	//������ɣ�������Ϸ����
	g_pCGameUpdateDlg->ShowWindow(SW_HIDE);
	g_pCGameUpdateDlg->TransferExternalProgram();
	g_pCGameUpdateDlg->PostMessage(WM_CLOSE);
	return TRUE;
}

BOOL CUpdateThread::DownFile(const char* sURL, \
					const char* sSaveFilename, \
					const char* sHeader, \
					HWND hProgressWindow)
{
	CInternet vInternet;
	if (CInternet::INTERNET_SUCCESS != vInternet.InternetGetURL(sURL, sSaveFilename, sHeader, hProgressWindow))
	{
		g_pCGameUpdateDlg->m_pCInetSession = new CInetSession(sURL, sSaveFilename);
		delete g_pCGameUpdateDlg->m_pCInetSession;
		g_pCGameUpdateDlg->m_pCInetSession = NULL;

		CFile file;
		if ( file.Open(sSaveFilename, CFile::modeRead))
		{
			file.Close();
			LOG(0, 0, "�ɹ����� \"%s\".", sSaveFilename);
			return TRUE;
		}

		LOG(0, 0, "Error: ����ʧ�� \"%s\".", sSaveFilename);
		return FALSE;
	}


	return TRUE;
}

BOOL CUpdateThread::DownloadFile(const string &strFile, const string &strTmpFile, const string &strPath) const
{
	TCHAR szResultPath[MAX_PATH]=TEXT("");
	CString strSavePath;
	_snprintf(szResultPath, sizeof(szResultPath), "%sconfig\\%s", GetAppDirectory(), UPDATE_CONFIG_FILENAME);
	strSavePath.Format("%s%s\\%s%s", GetAppDirectory(), DOWNDLOAD_TMP_PATH, strFile.c_str(), strTmpFile.c_str());

	GetPrivateProfileString(_T("UpdateAllFile"), _T("URL"), _T("http://192.168.1.5/update/hallfile/"), szResultPath, MAX_PATH, szResultPath);
	CString strURL(szResultPath);

	if ('/' != strURL.GetAt(strURL.GetLength()-1))
		strURL.Append("/");
	strURL.Append(strPath.c_str());

	strURL.Replace(_T("\\"), _T("/"));
	strURL.Replace(_T("\\\\"), _T("/"));
	strURL.Replace(_T("//"), _T("/"));
	strURL.Replace(_T(":/"), _T("://"));

	if (!m_bSilenceMode && m_hProgressWindow != NULL)
	{
		SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_DOWNLOADING_FILENAME, (LPARAM)strFile.c_str());
	}

	CInternet vInternet;
	int iResult = vInternet.InternetGetURL(strURL.GetBuffer(0), strSavePath.GetBuffer(0), NULL, m_hProgressWindow);
	strURL.ReleaseBuffer();
	strSavePath.ReleaseBuffer();

	if (iResult != CInternet::INTERNET_SUCCESS)
	{
		CGameUpdateDlg * pWnd = static_cast<CGameUpdateDlg * >(CGameUpdateDlg::FromHandle(m_hProgressWindow));
		pWnd->m_pCInetSession = new CInetSession(strURL.GetBuffer(0), strSavePath.GetBuffer(0));
		strURL.ReleaseBuffer();
		strSavePath.ReleaseBuffer();
		delete pWnd->m_pCInetSession;
		pWnd->m_pCInetSession = NULL;

		CFile file;
		if ( file.Open(strSavePath, CFile::modeRead))
		{
			file.Close();
			LOG(0, 0, "���سɹ� \"%s\".", strSavePath);
			return TRUE;
		}

		// ��¼�����ļ�ʧ����־
		char acBuffer[BUFFER_SIZE];
		LOG(0, 0, "Error: ����ʧ�� \"%s\".", strSavePath);
		memset(acBuffer, 0, BUFFER_SIZE);
		_snprintf(acBuffer, sizeof(acBuffer), "%s", strSavePath);
		SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_DOWNLOAD_FILE_FAIL, (LPARAM)acBuffer);
		return FALSE;
	}

	LOG(0, 0, "���سɹ� \"%s\" .", strSavePath);
	return TRUE;
}

// У���ļ�
BOOL CUpdateThread::VerifyFile(const string &strFile, const string &strTmpFile, const string &strHash) const
{
	TCHAR szSavePath[MAX_PATH]=TEXT("");
	CString strAppDirectory = GetAppDirectory();
	strAppDirectory.Append("\\");
	strAppDirectory.Append(DOWNDLOAD_TMP_PATH);
	strAppDirectory.Append("\\");
	strAppDirectory.Append(strFile.c_str());
	strAppDirectory.Append(strTmpFile.c_str());
	strAppDirectory.Replace("\\\\", "\\");
	
	// �����ļ���Hash���Խ��бȽ�
	unsigned char acMD5Digest[16];
	CalculateMD5(strAppDirectory.GetBuffer(0), acMD5Digest);
	strAppDirectory.ReleaseBuffer();

	if (0 == strHash.compare(MD5toString(acMD5Digest)))
	{
		LOG(0, 0, "Ч��ɹ� \"%s\" .", strAppDirectory.GetBuffer(0));
		return TRUE;
	}
	else
	{
		LOG(0, 0, "Error: ����ʧ�� ����ģ������\"%s\".", strAppDirectory.GetBuffer(0));
		strAppDirectory.ReleaseBuffer();
		char acBuffer[BUFFER_SIZE];
		memset(acBuffer, 0, BUFFER_SIZE);
		_snprintf(acBuffer, sizeof(acBuffer), "%s", strAppDirectory.GetBuffer(0));
		SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_DOWNLOAD_FILE_FAIL, (LPARAM)acBuffer);
		strAppDirectory.ReleaseBuffer();
	}

	return FALSE;
}

BOOL CUpdateThread::UpdateFile(const string &strFile, const string &strTmpFile, const string &strPath) const
{
	// �õ��ļ�·��
	string::size_type Index = strPath.rfind(strFile);

	CString strAppDirectory = GetAppDirectory();
	strAppDirectory.Append(strPath.substr(0, Index-1).c_str());
	strAppDirectory.Replace("\\\\", "\\");
		
	//�ж�Ŀ¼
	ResolvePath(strAppDirectory);
	CDirOperate::CreateAllDirectory(strAppDirectory);
	strAppDirectory.Append("\\");
	strAppDirectory.Append(strFile.c_str());
	strAppDirectory.Replace("\\\\", "\\");

	// ȡ�ñ�ģ���ļ�����Ҫ���Ƶ��ļ���
	char acBuffer[MAX_PATH] = {0};	
	GetModuleFileName(AfxGetInstanceHandle(), acBuffer, sizeof(acBuffer));
	string strModuleNameOne = GetFilename(acBuffer);
	string strModuleNameTwo = strModuleNameOne;
	strModuleNameTwo.insert(0, "\\");
	string strTmpPath(strPath);
	transform(strModuleNameOne.begin(), strModuleNameOne.end(), strModuleNameOne.begin(), towlower);
	transform(strModuleNameTwo.begin(), strModuleNameTwo.end(), strModuleNameTwo.begin(), towlower);
	transform(strTmpPath.begin(), strTmpPath.end(), strTmpPath.begin(), towlower);
	
	//�������ػ����е��ļ����ļ�Ŀ���ַ
	CString strExistingFile = GetAppDirectory()+DOWNDLOAD_TMP_PATH;
	strExistingFile.Append("\\");
	strExistingFile.Append(strFile.c_str());
	strExistingFile.Append(strTmpFile.c_str());	

	// �����ļ��뱾ģ����ͬ������ȡһ�������������г������к���ɾ������
	if (strPath==strModuleNameOne || strPath==strModuleNameTwo)
		strAppDirectory.Append(".new");

	SetFileAttributes(strAppDirectory, FILE_ATTRIBUTE_NORMAL);
	DeleteFile(strAppDirectory);
	return MoveFile(strExistingFile, strAppDirectory);
}

void CUpdateThread::CheckSingleFileUpdate(void)
{
	TCHAR szNamePath[MAX_PATH]=TEXT(""); //���������ļ�·��
	TCHAR szKeyword[MAX_PATH]=TEXT("");//�����ļ��ֶ�
	
	TCHAR szResultfile[MAX_PATH]=TEXT("");//�����ļ������
	TCHAR szResultPath[MAX_PATH]=TEXT("");//����·�����
	TCHAR szResultHash[MAX_PATH]=TEXT("");//����HASH���
	TCHAR szResultSize[MAX_PATH]=TEXT("");//�����ļ���С

	//��ȡ����HASH�б�
	CString szPath(GetAppDirectory());
	_snprintf(szNamePath, sizeof(szNamePath), "%s%s\\%s", szPath, CONFIG_PATH, UPDATE_CONFIG_FILENAME);
	CString strFile;
	map<string, FileHashTable> localFileList,ServerFileList; //���浥���ļ�����
	SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_MESSAGE, (LPARAM)"���ڶ�ȡ�����ļ��б�...");
	int iIndex=0;

	while (1)
	{
		_snprintf(szKeyword, sizeof(szKeyword), "_SignleFileUpdate__%d", iIndex++);
		GetPrivateProfileString(szKeyword, _T("name"), _T(""), szResultfile, MAX_PATH, szNamePath);
		GetPrivateProfileString(szKeyword, _T("size"), _T(""), szResultSize, MAX_PATH, szNamePath);
		GetPrivateProfileString(szKeyword, _T("path"), _T(""), szResultPath, MAX_PATH, szNamePath);
		GetPrivateProfileString(szKeyword, _T("hash"), _T(""), szResultHash, MAX_PATH, szNamePath);

		if (0==strcmp(szResultfile, "") || 0==strcmp(szResultPath, ""))
			break;

		FileHashTable filehashtable;
		filehashtable.filename = szResultfile;
		filehashtable.size = atol(szResultSize);
		filehashtable.path = szResultPath;
		filehashtable.Hash = szResultHash;
		string::size_type sfileNameIndex = filehashtable.filename.find(".");
		if (0<=sfileNameIndex && filehashtable.filename.size()>sfileNameIndex)
			filehashtable.DATName = filehashtable.filename.substr(sfileNameIndex, filehashtable.filename.size()-sfileNameIndex);
		localFileList.insert(make_pair(string(szResultPath), filehashtable));
	}


	//��ȡ�����ص��ļ�׼������
	int iFileCount = 0;
	FileHashTable filehashtable;
	TCHAR chBuffer[MAX_PATH], chKeyName[MAX_PATH], chVal[MAX_PATH];
	_snprintf(chBuffer, sizeof(chBuffer), TEXT("%s%s\\DownloadComplete.ini"), szPath, DOWNDLOAD_TMP_PATH);
	m_CompleteDownByte = 0;

	while (1)
	{
		_snprintf(chKeyName, sizeof(chKeyName), TEXT("DownloadFile%d"), ++iFileCount);
		GetPrivateProfileString(chKeyName, TEXT("filename"), TEXT(""), chVal, sizeof(chVal), chBuffer);
		filehashtable.filename = chVal;
		memset(chVal, 0, sizeof(chVal));
		GetPrivateProfileString(chKeyName, TEXT("tmpfilename"), TEXT(""), chVal, sizeof(chVal), chBuffer);
		filehashtable.tmpFileName = chVal;
		memset(chVal, 0, sizeof(chVal));
		GetPrivateProfileString(chKeyName, TEXT("datname"), TEXT(""), chVal, sizeof(chVal), chBuffer);
		filehashtable.DATName = chVal;
		memset(chVal, 0, sizeof(chVal));
		GetPrivateProfileString(chKeyName, TEXT("path"), TEXT(""), chVal, sizeof(chVal), chBuffer);
		filehashtable.path = chVal;
		memset(chVal, 0, sizeof(chVal));
		GetPrivateProfileString(chKeyName, TEXT("hash"), TEXT(""), chVal, sizeof(chVal), chBuffer);
		filehashtable.Hash = chVal;
		memset(chVal, 0, sizeof(chVal));
		GetPrivateProfileString(chKeyName, TEXT("size"), TEXT(""), chVal, sizeof(chVal), chBuffer);
		filehashtable.size = atol(chVal);

		if (0 < filehashtable.filename.size()  &&  0 < filehashtable.path.size())
		{
			m_CompleteDownByte += filehashtable.size;
			m_setDowndloadFile.insert(make_pair(filehashtable.path, filehashtable));
		}
		else
		{
			break;
		}
	}



	// ������ӦĿ¼
	CString cstrDirectory(GetAppDirectory());
	cstrDirectory.Append(DOWNDLOAD_TMP_PATH);
	//�����������������ļ��𻵣���ո��ļ���
	if ( m_setDowndloadFile.empty() )
	{
		CDirOperate::DeleteAllDirectory(cstrDirectory);
		CDirOperate::CreateAllDirectory(cstrDirectory);
	}


	//��ȡ������HASH�б�
	SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_MESSAGE, (LPARAM)"���ڻ�ȡ�������ļ��б�...");
	TCHAR szServerPath[MAX_PATH]=TEXT(""); //�����������ļ�����·��
	_snprintf(szNamePath, sizeof(szNamePath), "%s%s\\%s", szPath, CONFIG_PATH, UPDATE_CONFIG_FILENAME);
	_snprintf(szServerPath, sizeof(szServerPath), "%s%s",szPath,DOWNDLOAD_TMP_PATH);
	CDirOperate::CreateAllDirectory(szServerPath);//���������ļ�����Ŀ¼
	_snprintf(szServerPath, sizeof(szServerPath), "%s%s\\%s", szPath, DOWNDLOAD_TMP_PATH, UPDATE_CONFIG_TMP_FILENAME);
	GetPrivateProfileString(_T("UpdateAllFile"), _T("URL"), _T("http://192.168.1.5/update/hallfile/"), szNamePath, MAX_PATH, szNamePath);
	_snprintf(szNamePath, sizeof(szNamePath), "%s%s/%s", szNamePath, CONFIG_PATH, UPDATE_CONFIG_FILENAME);

	if (FALSE == DownFile(szNamePath, szServerPath, NULL, g_pCGameUpdateDlg->m_hWnd))
	{
		DWORD dwError = GetLastError();
		LOG(0, 0, "Error: ���ط����������ļ�ʧ�� �����:%ld �ļ�·��: %s", dwError, szNamePath);
		return;
	}

	//��ȡ�������ļ��б�
	iIndex=0;
	while (1)
	{
		_snprintf(szKeyword, sizeof(szKeyword), "_SignleFileUpdate__%d", iIndex++);
		GetPrivateProfileString(szKeyword, _T("name"), _T(""), szResultfile, MAX_PATH, szServerPath);
		GetPrivateProfileString(szKeyword, _T("size"), _T(""), szResultSize, MAX_PATH, szServerPath);
		GetPrivateProfileString(szKeyword, _T("path"), _T(""), szResultPath, MAX_PATH, szServerPath);
		GetPrivateProfileString(szKeyword, _T("hash"), _T(""), szResultHash, MAX_PATH, szServerPath);

		if (0==strcmp(szResultfile, "") || 0==strcmp(szResultPath, ""))
			break;

		FileHashTable filehashtable;
		filehashtable.filename = szResultfile;
		filehashtable.size = atol(szResultSize);
		filehashtable.path = szResultPath;
		filehashtable.Hash = szResultHash;
		string::size_type sfileNameIndex = filehashtable.filename.find(".");
		if (0<=sfileNameIndex && filehashtable.filename.size()>sfileNameIndex)
			filehashtable.DATName = filehashtable.filename.substr(sfileNameIndex, filehashtable.filename.size()-sfileNameIndex);
		ServerFileList.insert(make_pair(string(szResultPath), filehashtable));
	}

	//�Աȱ�����������ļ� HASH ȷ����Щ�ļ���Ҫ����
	SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_MESSAGE, (LPARAM)"���ڶԱȷ�������ͻ����ļ��б�...");
	for (map<string, FileHashTable>::iterator begServer=ServerFileList.begin(); begServer!=ServerFileList.end(); begServer++)
	{
		map<string, FileHashTable>::iterator begLocal=localFileList.begin();
		for (; begLocal!=localFileList.end(); begLocal++)
		{
			if (string::npos!=begServer->first.find(begLocal->first))
			{
				if (begLocal->second.Hash!=begServer->second.Hash)
				{
					m_setWantUpdateFile.insert(*begServer);
				}
				break;
			}
		}

		if (begLocal == localFileList.end())
			m_setWantUpdateFile.insert(*begServer);
	}
}

void CUpdateThread::CheckDirectoryUpdate(void)
{
	TCHAR szNamePath[MAX_PATH]=TEXT(""); //���������ļ�·��
	TCHAR szKeyword[MAX_PATH]=TEXT("");//�����ļ��ֶ�
	
	TCHAR szResultfile[MAX_PATH]=TEXT("");//�����ļ������
	TCHAR szResultPath[MAX_PATH]=TEXT("");//����·�����
	TCHAR szResultHash[MAX_PATH]=TEXT("");//����HASH���
	TCHAR szResultSize[MAX_PATH]=TEXT("");//�����ļ���С

	CString szPath(GetAppDirectory());
	_snprintf(szNamePath, sizeof(szNamePath), "%s%s\\%s", szPath, CONFIG_PATH, UPDATE_CONFIG_FILENAME);
	CString strFile;
	map<string, string> localDirectoryList,serverDirectoryList; //����Ŀ¼����

	SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_MESSAGE, (LPARAM)"���ڻ�ȡ������Ŀ¼�б�...");
	TCHAR szServerPath[MAX_PATH]=TEXT(""); //�����������ļ�����·��
	_snprintf(szNamePath, sizeof(szNamePath), "%s%s\\%s", szPath, CONFIG_PATH, UPDATE_CONFIG_FILENAME);
	_snprintf(szServerPath, sizeof(szServerPath), "%s%s\\%s", szPath, DOWNDLOAD_TMP_PATH, UPDATE_CONFIG_TMP_FILENAME);
	
	//��ȡ���ظ���Ŀ¼�б�
	int iIndex=0;
	SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_MESSAGE, (LPARAM)"���ڶ�ȡ���ظ���Ŀ¼�б�...");
	while (1)
	{
		_snprintf(szKeyword, sizeof(szKeyword), "_SignleDirectoryUpdate__%d", iIndex++);
		GetPrivateProfileString(szKeyword, _T("name"), _T(""), szResultfile, MAX_PATH, szNamePath);
		GetPrivateProfileString(szKeyword, _T("hash"), _T(""), szResultHash, MAX_PATH, szNamePath);
		if (0==strcmp(szResultHash, "") || 0==strcmp(szResultfile, ""))
			break;

		localDirectoryList.insert(make_pair(szResultfile, szResultHash));
	}

	//��ȡ����������Ŀ¼�б�
	iIndex=0;
	while (1)
	{
		_snprintf(szKeyword, sizeof(szKeyword), "_SignleDirectoryUpdate__%d", iIndex++);
		GetPrivateProfileString(szKeyword, _T("name"), _T(""), szResultfile, MAX_PATH, szServerPath);
		GetPrivateProfileString(szKeyword, _T("hash"), _T(""), szResultHash, MAX_PATH, szServerPath);
		if (0==strcmp(szResultHash, "") || 0==strcmp(szResultfile, ""))
			break;

		serverDirectoryList.insert(make_pair(szResultfile, szResultHash));
	}

	//�Աȱ����������Ŀ¼ HASH ȷ����Щ�ļ���Ҫ����
	map<string, string> mResultDirectory;
	SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_MESSAGE, (LPARAM)"���ڶԱȷ�������ͻ���Ŀ¼�б�...");
	for (map<string, string>::iterator begServer=serverDirectoryList.begin(); begServer!=serverDirectoryList.end(); begServer++)
	{
		map<string, string>::iterator begLocal=localDirectoryList.begin();
		for (; begLocal!=localDirectoryList.end(); begLocal++)
		{
			if (string::npos!=begServer->first.find(begLocal->first))
			{
				if (begLocal->second!=begServer->second)
				{
					mResultDirectory.insert(*begServer);
				}
				break;
			}
		}

		if (begLocal == localDirectoryList.end())
			mResultDirectory.insert(*begServer);
	}

	//���Ŀ¼Ϊ��λ�ĸ��µ�Ԫ�Ƿ��б䶯���б䶯���ټ�����µ��ļ�
	int iServerIndex=0;
	map<string, FileHashTable> mLocal, mServer;
	for (map<string, string>::iterator mbeg=mResultDirectory.begin(); mbeg!=mResultDirectory.end(); mbeg++)
	{
		iIndex=0;
		while (1)
		{
			_snprintf(szKeyword, sizeof(szKeyword), "%s%d", mbeg->first.c_str(), iIndex++);
			GetPrivateProfileString(szKeyword, _T("name"), _T(""), szResultfile, MAX_PATH, szNamePath);
			GetPrivateProfileString(szKeyword, _T("size"), _T(""), szResultSize, MAX_PATH, szNamePath);
			GetPrivateProfileString(szKeyword, _T("path"), _T(""), szResultPath, MAX_PATH, szNamePath);
			GetPrivateProfileString(szKeyword, _T("hash"), _T(""), szResultHash, MAX_PATH, szNamePath);

			if (0==strcmp(szResultfile, "") || 0==strcmp(szResultPath, ""))
				break;

			FileHashTable filehashtable;
			filehashtable.filename = szResultfile;
			filehashtable.size = atol(szResultSize);
			filehashtable.path = szResultPath;
			filehashtable.Hash = szResultHash;
			string::size_type sfileNameIndex = filehashtable.filename.find(".");
			if (0<=sfileNameIndex && filehashtable.filename.size()>sfileNameIndex)
				filehashtable.DATName = filehashtable.filename.substr(sfileNameIndex, filehashtable.filename.size()-sfileNameIndex);
			mLocal.insert(make_pair(string(szResultPath), filehashtable));
		}

		iServerIndex=0;
		while (1)
		{
			_snprintf(szKeyword, sizeof(szKeyword), "%s%d", mbeg->first.c_str(), iServerIndex++);
			GetPrivateProfileString(szKeyword, _T("name"), _T(""), szResultfile, MAX_PATH, szServerPath);
			GetPrivateProfileString(szKeyword, _T("size"), _T(""), szResultSize, MAX_PATH, szServerPath);
			GetPrivateProfileString(szKeyword, _T("path"), _T(""), szResultPath, MAX_PATH, szServerPath);
			GetPrivateProfileString(szKeyword, _T("hash"), _T(""), szResultHash, MAX_PATH, szServerPath);

			if (0==strcmp(szResultfile, "") || 0==strcmp(szResultPath, ""))
				break;

			FileHashTable filehashtable;
			filehashtable.filename = szResultfile;
			filehashtable.size = atol(szResultSize);
			filehashtable.path = szResultPath;
			filehashtable.Hash = szResultHash;
			string::size_type sfileNameIndex = filehashtable.filename.find(".");
			if (0<=sfileNameIndex && filehashtable.filename.size()>sfileNameIndex)
				filehashtable.DATName = filehashtable.filename.substr(sfileNameIndex, filehashtable.filename.size()-sfileNameIndex);
			mServer.insert(make_pair(string(szResultPath), filehashtable));
		}
	}


	//�Աȱ����������Ŀ¼�µ��ļ� HASH ȷ����Щ�ļ���Ҫ����
	SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_MESSAGE, (LPARAM)"���ڶԱȷ�������ͻ���Ŀ¼�µ��ļ��б�...");
	for (map<string, FileHashTable>::iterator begServer=mServer.begin(); begServer!=mServer.end(); begServer++)
	{
		map<string, FileHashTable>::iterator begLocal=mLocal.begin();
		for (; begLocal!=mLocal.end(); begLocal++)
		{
			if (string::npos!=begServer->first.find(begLocal->first))
			{
				if (begLocal->second.Hash!=begServer->second.Hash)
				{
					m_setWantUpdateFile.insert(*begServer);
				}
				break;
			}
		}

		if (begLocal == mLocal.end())
			m_setWantUpdateFile.insert(*begServer);
	}
}

void CUpdateThread::CheckWantUpdateFile(void)
{
	TCHAR szNamePath[MAX_PATH]=TEXT(""); //���������ļ�·��
	CString szPath(GetAppDirectory());
	_snprintf(szNamePath, sizeof(szNamePath), "%s%s\\%s", szPath, CONFIG_PATH, UPDATE_CONFIG_FILENAME);

	TCHAR szServerPath[MAX_PATH]=TEXT(""); //�����������ļ�����·��
	_snprintf(szServerPath, sizeof(szServerPath), "%s%s\\%s", szPath, DOWNDLOAD_TMP_PATH, UPDATE_CONFIG_TMP_FILENAME);

	CString cstrDirectory(GetAppDirectory());
	cstrDirectory.Append(DOWNDLOAD_TMP_PATH);

	// �����ļ����
	CheckSingleFileUpdate();
	// Ŀ¼��飨��Щָ����Ŀ¼��Ŀ¼��飬�ټ�����µ��ļ���
	CheckDirectoryUpdate();

	if (m_setWantUpdateFile.empty())
	{
		// ��������MD5 Hash �����ļ�
		_snprintf(szNamePath, sizeof(szNamePath), "%s%s\\%s", szPath, CONFIG_PATH, UPDATE_CONFIG_FILENAME);
		SetFileAttributes(szNamePath, FILE_ATTRIBUTE_NORMAL);
		DeleteFile(szNamePath);
		MoveFile(szServerPath, szNamePath);
		CDirOperate::DeleteAllDirectory(cstrDirectory);

		RMessageBox("���Ѿ������°汾��");
		g_pCGameUpdateDlg->ShowWindow(SW_HIDE);
		g_pCGameUpdateDlg->TransferExternalProgram();
		g_pCGameUpdateDlg->PostMessage(WM_CLOSE);
		return;
	}

	SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_MESSAGE, (LPARAM)"��ʼ����...");
}

BOOL CUpdateThread::DoGenerateMD5Hash(BOOL bShush)
{
//	//��ȡĿ¼
//	CString szPath(GetAppDirectory());
//	string sCurPath(szPath.GetBuffer(0));
//	szPath.ReleaseBuffer();
//	// �õ����е��ļ���
//	multimap<string, FileHashTable> AllFileHashStruct; //��������������ļ��������Ӧ��HASH��·��
//	//g_pCGameUpdateDlg->SearchCatalogueAllFile(AllFileHashStruct, sCurPath, sCurPath);
//	//���˵��ļ���
//	vector<string> leachFileName;
//
//	UINT uIndex=1;
//	TCHAR tchUpdateFile[MAX_PATH]=_T("");
//	TCHAR tchUpdatePath[MAX_PATH]=_T("");
//	TCHAR tchUpdateKey[MAX_PATH]=_T("");
//	_snprintf(tchUpdatePath, sizeof(tchUpdatePath), "%sconfig\\%s", szPath, UPDATE_SET_INI_FILE);
//	vector<string> updatefile;
//	TCHAR szServerPath[MAX_PATH]=TEXT(""); //�����������ļ�����·��
//	_snprintf(szServerPath, sizeof(szServerPath), "%s%s",szPath,DOWNDLOAD_TMP_PATH);
//
//	// �õ�����Ŀ¼���ļ���
//	CFile file;
//	TCHAR szHaseList[MAX_PATH]=TEXT(""); //���������ļ�·��
//	_snprintf(szHaseList, sizeof(szHaseList), "%s%s\\%s", szPath, CONFIG_PATH, UPDATE_CONFIG_FILENAME);
//	GetPrivateProfileString(_T("UpdateAllFile"), _T("GameHaseList"), _T(""), szHaseList, MAX_PATH, szHaseList);
//	
//	if (TRUE == bShush)
//	{
//		//update.ini�ļ���ȡʧ�ܣ���Ҫ�������ɣ���������Ҫupdateset.ini
//		//updateset.ini ��ǿ����ÿ�ζ���Ҫ�ӷ��������أ���Ϊ�����ܻ��б䶯
//		SetProgressPos(bShush, 0);
//		if ( (0==lstrcmp("", szHaseList)) || FALSE==file.Open(tchUpdatePath, CFile::modeRead) )
//		{
//			SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_MESSAGE, (LPARAM)"���ط�����������������ļ���");
//			TCHAR szNamePath[MAX_PATH]=TEXT(""); //���������ļ�·��
//			CDirOperate::CreateAllDirectory(szServerPath);//���������ļ�����Ŀ¼
//			_snprintf(szServerPath, sizeof(szServerPath), "%s%s\\%s", szPath, DOWNDLOAD_TMP_PATH, UPDATE_SET_INI_TMP_FILE);
//			_snprintf(szNamePath, sizeof(szNamePath), "http://%s/update/hallfile/config/updateset.dat", g_pCGameUpdateDlg->m_chURL);
//			OutputDebugString(szNamePath);
//
//			int iReturn;
//			DownFile(szNamePath, szServerPath, NULL, g_pCGameUpdateDlg->m_hWnd);
//			GetPrivateProfileString(_T("set"), _T("URL"), _T(""), szHaseList, MAX_PATH, szServerPath);
//
//			if (0 == lstrlen(szHaseList))
//			{
//				iReturn = RMessageBox(_T("��Ϸ�޸���Ҫ����������ļ�����ʧ�ܣ�"), MB_OK);
//				if (IDOK == iReturn)
//				{
//					g_pCGameUpdateDlg->PostMessage(WM_CLOSE);
//					return FALSE;
//				}
//				//else
//				//{
//				//	while (IDRETRY == iReturn)
//				//	{
//				//		// �����ٴ�����
//				//		DownFile(szNamePath, szServerPath, NULL, g_pCGameUpdateDlg->m_hWnd);
//				//		// ������ص��ļ��Ϸ���,ͨ����ȡ�ļ��е�UpdateURL�ֶ��ж��ļ��Ϸ���
//				//		GetPrivateProfileString(_T("set"), _T("UpdateURL"), _T(""), szHaseList, MAX_PATH, szServerPath);
//				//		if (0 != lstrlen(szHaseList))
//				//			break;
//
//				//		iReturn = RMessageBox(_T("��Ϸ�޸���Ҫ����������ļ�����ʧ�ܣ�"), MB_RETRYCANCEL);
//				//	}
//				//}
//			}
//		}
//		else
//			file.Close(); //���رգ������޷���ȡ
//	}
//
//
//	while (1)
//	{
//		_snprintf(tchUpdateKey, sizeof(tchUpdateKey), "directory%d", uIndex++);
//		// �޸�ʹ��tmpupdate\updatesetserver.ini�ļ�������ʹ��config\updateset.ini
//		GetPrivateProfileString(_T("enforce"), tchUpdateKey, _T(""), tchUpdateFile, MAX_PATH, (TRUE==bShush?szServerPath:tchUpdatePath));
//
//		if (0 == strcmp("", tchUpdateFile))
//			break;
//
//		string strFile(tchUpdateFile);
//		updatefile.push_back(strFile);
//	}
//
//	SetProgressPos(bShush, 10);
//	//����ָ����Ҫ���µ��ļ���Ŀ¼
//	unsigned char acMD5Digest[16];
//	int iCount = 1;
//	int iTotalCount = AllFileHashStruct.size();
//	CString strContent;
//	multimap<string, FileHashTable> tmpAllFileHashStruct;
//	for (multimap<string, FileHashTable>::iterator beg=AllFileHashStruct.begin(); beg!=AllFileHashStruct.end(); beg++)
//	{
//		vector<string>::iterator vbeg=updatefile.begin();
//		for (; vbeg!=updatefile.end(); vbeg++)
//		{
//			if (0==beg->second.path.find(*vbeg))
//			{
//				tmpAllFileHashStruct.insert(tmpAllFileHashStruct.begin(), *beg);
//				break;
//			}
//		}
//
//		if (vbeg==updatefile.end())
//		{
//			string strSubStr = beg->second.path.substr(beg->second.path.size()-4, 4);
//
//			if (beg->first.size()+1==beg->second.path.size())
//			{
//				if (0 == strSubStr.compare(".exe"))
//				{
//					SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_LEACH_FILE_NAME, (LPARAM)beg->second.path.c_str());
//					continue;				
//				}
//				tmpAllFileHashStruct.insert(tmpAllFileHashStruct.begin(), *beg);
//			}
//		}
//	}
//
//	SetProgressPos(bShush, 15);
//	// ������ӦMD5 HASHֵ
//	for (multimap<string, FileHashTable>::iterator beg=tmpAllFileHashStruct.begin(); beg!=tmpAllFileHashStruct.end(); beg++,iCount++)
//	{
//		string strTmpPath(szPath.GetBuffer(0));
//		szPath.ReleaseBuffer();
//		strTmpPath.append(beg->second.path);
//		CalculateMD5(strTmpPath.c_str(), acMD5Digest);
//		
//		CString strHash = MD5toString(acMD5Digest);
//		beg->second.Hash = strHash.GetBuffer();
//		strHash.ReleaseBuffer();
//		
//		strContent.Format("%s�����ļ�����: [%d/%d]", ((TRUE==bShush)?"�޸� ":""), iCount, iTotalCount); 
//		SendMessage(m_hProgressWindow, WM_USER, (WPARAM)((TRUE==bShush)?NOTIFY_MESSAGE:NOTIFY_GENERATE_MESSAGE), (LPARAM)strContent.GetBuffer(0));
//		SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_GENERATE_FILE_NAME, (LPARAM)strTmpPath.c_str());
//		strContent.ReleaseBuffer();
//	}
//
//
//	// ��ȡ��Ŀ¼Ϊ��λ���MD5ֵ����һ����µ�����·��
//	uIndex=1;
//	memset(tchUpdateFile, 0, sizeof(tchUpdateFile));
//	memset(tchUpdateKey, 0, sizeof(tchUpdateKey));
//	vector<string> vDirectoryUntix;
//	while (1)
//	{
//		_snprintf(tchUpdateKey, sizeof(tchUpdateKey), "unit%d", uIndex++);
//		GetPrivateProfileString(_T("directory"), tchUpdateKey, _T(""), tchUpdateFile, MAX_PATH, (TRUE==bShush?szServerPath:tchUpdatePath));
//		if (0 == strcmp("", tchUpdateFile))
//			break;
//
//		string strFile(tchUpdateFile);
//		vDirectoryUntix.push_back(strFile);
//	}
//
//
//	// ����MD5 Hash��ini�ļ���
//	int iIndex=0;
//	TCHAR szIniPath[MAX_PATH]=TEXT(""),szFileSize[MAX_PATH]=TEXT("");
//	_snprintf(szIniPath, sizeof(szIniPath), "%s%s", szPath, CONFIG_PATH);
//	CDirOperate::CreateAllDirectory(szIniPath);
//	_snprintf(szIniPath, sizeof(szIniPath), "%s%s\\%s", szPath, CONFIG_PATH, UPDATE_CONFIG_FILENAME);
//	SetFileAttributes(szIniPath, FILE_ATTRIBUTE_NORMAL);
//	::DeleteFile(szIniPath);
//	
//	string strLower;
//	CString strFile,strAllHash;
//	//֮�������� GameHaseList д����ֵ����Ϊ�Ҳ�ϣ��ͳ������GameHaseList �ŵ�ini �ļ��ĺ���
//	//��Ȼ��ʵ׼ȷ����������strAllHash�м���õ���д���
//	WritePrivateProfileString(_T("UpdateAllFile"), _T("GameHaseList"), _T(""), szIniPath);
//
//	// �������������ļ��Ĺ����ַ����Ѿ��ڵ������ʱ���ú�
//	// ���ﲻ��Ҫ�����ã��ظ�����Ҳ���������Ϊ���ù��˵ı༭������ʾ�Ѿ����˵ı༭����ͬһ��IDC_RICHEDIT21�ؼ���
//	if (TRUE == bShush)
//	{
//		;//g_pCGameUpdateDlg->SetLeachFileName(bShush);
//	}
//	//set< string > setTmpLeachName = g_pCGameUpdateDlg->GetLeachName();
//
//	iIndex=0;
//	iCount = 1;
//	iTotalCount = tmpAllFileHashStruct.size();
//	multimap<string, FileHashTable> mDirectoryUnit;
//	
//	for (multimap<string, FileHashTable>::iterator beg=tmpAllFileHashStruct.begin(); beg!=tmpAllFileHashStruct.end(); beg++,iCount++)
//	{
//		// ������Ŀ¼Ϊ������µİ��Ȳ�д�룬���������ӦĿ¼�ֶ��У�������_SignleFileUpdate__�ֶ�
//		vector<string>::iterator vbeg=vDirectoryUntix.begin();
//		for (; vbeg!=vDirectoryUntix.end(); vbeg++)
//		{
//			if (0 == beg->second.path.find(*vbeg))
//				break;
//		}
//		if (vDirectoryUntix.end() != vbeg)
//		{
//			beg->second.filename = beg->first;
//			mDirectoryUnit.insert(mDirectoryUnit.begin(), make_pair(*vbeg, beg->second));
//			continue;
//		}
//
//
//		// ���� �ļ��� ���ò���Ҫ���������������
//		strLower = beg->first.c_str();
//		transform(strLower.begin(), strLower.end(), strLower.begin(), towlower);
//		if (setTmpLeachName.end()!=setTmpLeachName.find(strLower))
//		{
//			SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_LEACH_FILE_NAME, (LPARAM)beg->second.path.c_str());
//			continue;
//		}
//
//		// ���� ��չ�� ���ò���Ҫ���������������
//		strLower = beg->second.DATName.c_str();
//		transform(strLower.begin(), strLower.end(), strLower.begin(), towlower);
//		if (setTmpLeachName.end()!=setTmpLeachName.find(strLower))
//		{
//			SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_LEACH_FILE_NAME, (LPARAM)beg->second.path.c_str());
//			continue;
//		}
//
//		strFile.Format("_SignleFileUpdate__%d", iIndex++);
//		_snprintf(szFileSize, sizeof(szFileSize), "%I64u", beg->second.size);
//		WritePrivateProfileString(strFile, _T("name"), beg->first.c_str(), szIniPath);
//		WritePrivateProfileString(strFile, _T("size"), szFileSize, szIniPath);
//		WritePrivateProfileString(strFile, _T("path"), beg->second.path.c_str(), szIniPath);
//		WritePrivateProfileString(strFile, _T("hash"), beg->second.Hash.c_str(), szIniPath);
//		strAllHash.Append(beg->second.Hash.c_str());
//
//		strContent.Format("%s�����ļ�����: [%d/%d]", ((TRUE==bShush)?"�޸� ":""), iCount, iTotalCount);
//		SendMessage(m_hProgressWindow, WM_USER, (WPARAM)((TRUE==bShush)?NOTIFY_MESSAGE:NOTIFY_GENERATE_MESSAGE), (LPARAM)strContent.GetBuffer(0));
//
//		strContent.ReleaseBuffer();
//		string strTmpPath(szPath.GetBuffer(0));
//		szPath.ReleaseBuffer();
//		strTmpPath.append(beg->second.path);
//		SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_GENERATE_FILE_NAME, (LPARAM)strTmpPath.c_str());
//	}
//
//
//	SetProgressPos(bShush, 45);
//	// ����Ŀ¼�µ��ļ�д��INI
//	iIndex=0;
//	iCount = 1;
//	map<string, string> mDirectory;
//	iTotalCount = mDirectoryUnit.size();
//	string sLastDirectory("");
//	for (multimap<string, FileHashTable>::iterator mbeg=mDirectoryUnit.begin(); mbeg!=mDirectoryUnit.end(); mbeg++,iCount++)
//	{
//		// ���� �ļ��� ���ò���Ҫ���������������
//		strLower = mbeg->second.filename.c_str();
//		transform(strLower.begin(), strLower.end(), strLower.begin(), towlower);
//		if (setTmpLeachName.end()!=setTmpLeachName.find(strLower))
//		{
//			SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_LEACH_FILE_NAME, (LPARAM)mbeg->second.path.c_str());
//			continue;
//		}
//
//		// ���� ��չ�� ���ò���Ҫ���������������
//		strLower = mbeg->second.DATName.c_str();
//		transform(strLower.begin(), strLower.end(), strLower.begin(), towlower);
//		if (setTmpLeachName.end()!=setTmpLeachName.find(strLower))
//		{
//			SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_LEACH_FILE_NAME, (LPARAM)mbeg->second.path.c_str());
//			continue;
//		}
//
//		pair<map<string, string>::iterator, bool> pairResult = mDirectory.insert(make_pair(mbeg->first, mbeg->second.Hash));
//		if ( !pairResult.second )
//		{
//			pairResult.first->second.append(mbeg->second.Hash);
//		}
//
//		if (0 != sLastDirectory.compare(mbeg->first))
//		{
//			iIndex = 0;
//		}
//
//		strFile.Format("%s%d", mbeg->first.c_str(), iIndex++);
//		strFile.Replace(" ", "");
//		strFile.Replace("\\\\", "_");
//		strFile.Replace("\\", "_");
//		strFile.Replace("/", "_");
//		_snprintf(szFileSize, sizeof(szFileSize), "%I64u", mbeg->second.size);
//		WritePrivateProfileString(strFile, _T("name"), mbeg->second.filename.c_str(), szIniPath);
//		WritePrivateProfileString(strFile, _T("size"), szFileSize, szIniPath);
//		WritePrivateProfileString(strFile, _T("path"), mbeg->second.path.c_str(), szIniPath);
//		WritePrivateProfileString(strFile, _T("hash"), mbeg->second.Hash.c_str(), szIniPath);
//		strAllHash.Append(mbeg->second.Hash.c_str());
//
//		strContent.Format("%sָ��Ŀ¼���ļ�����: [%d/%d]", ((TRUE==bShush)?"�޸� ":""), iCount, iTotalCount);
//		SendMessage(m_hProgressWindow, WM_USER, (WPARAM)((TRUE==bShush)?NOTIFY_MESSAGE:NOTIFY_GENERATE_MESSAGE), (LPARAM)strContent.GetBuffer(0));
//		strContent.ReleaseBuffer();
//
//		string strTmpPath(szPath.GetBuffer(0));
//		szPath.ReleaseBuffer();
//		strTmpPath.append(mbeg->second.path);
//		SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_GENERATE_FILE_NAME, (LPARAM)strTmpPath.c_str());
//		sLastDirectory = mbeg->first;
//	}
//
//
//	SetProgressPos(bShush, 90);
//	// ����Ŀ¼д��INI�ļ�
//	iIndex=0;
//	iCount = 1;
//	iTotalCount = mDirectory.size();
//	for (map<string, string>::iterator mbeg=mDirectory.begin(); mbeg!=mDirectory.end(); mbeg++,iCount++,iIndex++)
//	{
//		CalculateStringMD5(mbeg->second.c_str(), mbeg->second.size(), acMD5Digest);
//		CString strResult(MD5toString(acMD5Digest));
//		CString strDirectory(mbeg->first.c_str());
//		strDirectory.Replace(" ", "");
//		strDirectory.Replace("\\\\", "_");
//		strDirectory.Replace("\\", "_");
//		strDirectory.Replace("/", "_");
//
//		strFile.Format("_SignleDirectoryUpdate__%d", iIndex);
//		WritePrivateProfileString(strFile, _T("name"), strDirectory, szIniPath);
//		WritePrivateProfileString(strFile, _T("hash"), strResult, szIniPath);
//		strAllHash.Append(mbeg->second.c_str());
//
//		strContent.Format("%sָ��Ŀ¼��Ŀ¼����: [%d/%d]", ((TRUE==bShush)?"�޸� ":""), iCount, iTotalCount);
//		SendMessage(m_hProgressWindow, WM_USER, (WPARAM)((TRUE==bShush)?NOTIFY_MESSAGE:NOTIFY_GENERATE_MESSAGE), (LPARAM)strContent.GetBuffer(0));
//		strContent.ReleaseBuffer();
//
//		string strTmpPath(szPath.GetBuffer(0));
//		szPath.ReleaseBuffer();
//		strTmpPath.append(mbeg->first);
//		SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_GENERATE_FILE_NAME, (LPARAM)strTmpPath.c_str());		
//	}
//
//
//	SetProgressPos(bShush, 98);
//	//������ϷHASHֵ�����ж��Ƿ��������
//	CalculateStringMD5((LPCTSTR)strAllHash, strAllHash.GetLength(), acMD5Digest);
//	CString strResult(MD5toString(acMD5Digest));
//	WritePrivateProfileString(_T("UpdateAllFile"), _T("GameHaseList"), (LPCTSTR)strResult, szIniPath);
//	//���ø��µ�ַ
//	TCHAR szResultVal[MAX_PATH]=TEXT(""), szDefaultPath[MAX_PATH];
//	_snprintf(szDefaultPath, sizeof(szDefaultPath), _T("http://%s/update/hallfile/"), g_pCGameUpdateDlg->m_chURL);
//	GetPrivateProfileString(_T("set"), _T("URL"), szDefaultPath, szResultVal, MAX_PATH, (TRUE==bShush?szServerPath:tchUpdatePath));
//	WritePrivateProfileString(_T("UpdateAllFile"), _T("URL"), szResultVal, szIniPath);
//	_snprintf(szDefaultPath, sizeof(szDefaultPath), _T("http://%s/update.php?action=hall"), g_pCGameUpdateDlg->m_chURL);
//	//GetPrivateProfileString(_T("set"), _T("UpdateURL"), szDefaultPath, szResultVal, MAX_PATH, (TRUE==bShush?szServerPath:tchUpdatePath));
//	//WritePrivateProfileString(_T("UpdateAllFile"), _T("UpdateURL"), szResultVal, szIniPath);
//
//	   
//	if (FALSE == bShush)
//	{
//		g_pCGameUpdateDlg->m_btGenerateHash.EnableWindow();
//		g_pCGameUpdateDlg->m_btGenerateHash.SetWindowText(_T("���(&C)"));
//	}
//
//	_snprintf(szServerPath, sizeof(szServerPath), "%s%s",szPath,DOWNDLOAD_TMP_PATH);
//	CDirOperate::DeleteAllDirectory(szServerPath);
//	
//	szPath.Append("config\\");
//	CFile fileServerSet;
//	if (fileServerSet.Open(szPath+UPDATE_SET_INI_FILE, CFile::modeRead))
//	{
//		fileServerSet.Close();
//		CFile::Rename(szPath+UPDATE_SET_INI_FILE, szPath+TEXT("updateset .dat"));
//	}
//
//	SetProgressPos(bShush, 100);
//	SendMessage(m_hProgressWindow, WM_USER, (WPARAM)NOTIFY_GENERATE_MESSAGE, (LPARAM)"���ɳɹ�  ");
	return TRUE;
}

void CUpdateThread::SetProgressPos(BOOL bShush, int iPos)
{
	//if ( !bShush || NULL==g_pCGameUpdateDlg || FALSE==g_pCGameUpdateDlg->m_ProgressUpdate.IsWindowVisible())
		return;

	//g_pCGameUpdateDlg->m_ProgressUpdate.SetPos(iPos);
}
