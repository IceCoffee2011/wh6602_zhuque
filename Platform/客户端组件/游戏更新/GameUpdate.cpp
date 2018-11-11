// GameUpdate.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "GameUpdate.h"
#include "UISkinManager.h"
#include "RMessageBox.h"
#include "GameUpdateDlg.h"
#include "Generate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ȫ���ַ������������飨�����ʼ����InitStringTable()�����н��У�
struct StringStru g_String[STRING_BOTTOM];

// ȫ�����Դ��루���������޸���ֵ������ᵼ�³����쳣����
enum enLANGUAGE g_LanguageID;

/////////////////////////////////////////////////////////////////////////////
// CGameUpdateApp

BEGIN_MESSAGE_MAP(CGameUpdateApp, CWinApp)
	//{{AFX_MSG_MAP(CGameUpdateApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameUpdateApp construction

CGameUpdateApp::CGameUpdateApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bSilenceMode = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGameUpdateApp object

CGameUpdateApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGameUpdateApp initialization

BOOL CGameUpdateApp::InitInstance()
{
	// ��ʼ��ȫ���ַ�������������
	InitStringTable();

	// �������Ƿ��Ѿ������У�������ֱ���˳�
	if (IsAppRunning())
	{
		return FALSE;
	}
	
	AfxInitRichEdit2();
	AfxEnableControlContainer();	

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// ��ʼ���Ի����
	bool bGenerate=false;
	
	//��ȡĿ¼
	CString szPath(GetAppDirectory());
	szPath.Append("config\\");
	szPath.Append(UPDATE_SET_INI_FILE);
	CFile file;

	//AfxDebugBreak();//Test

	CStringArray arr;
	GetCmdLinePara(arr);//���������	

	if(3 != arr.GetSize())
	{
		bGenerate = true;
	}

	//bGenerate = FALSE;//Test

	// �����ʼ��
	CUISkinManager::Init(bGenerate);

	if (bGenerate)
	{
		if (file.Open(szPath, CFile::modeRead))
		{
			file.Close();		
		}
		else
		{
			return FALSE;
		}

		CGenerate GenerateDlg;
		m_pMainWnd = &GenerateDlg;
		GenerateDlg.DoModal();
		return TRUE;
	}

	// ��ʾ������
	CGameUpdateDlg dlg;
	m_pMainWnd = &dlg;
	dlg.m_sAppName = m_sAppName;
	dlg.m_bSilenceMode = m_bSilenceMode;
	dlg.m_bGenerateIniFile = bGenerate;
	
	if (3 == arr.GetSize())
	{
		string strTmp;
		// �õ��ⲿ������
		strTmp = arr.GetAt(0);
		string::size_type sIndex = strTmp.find(TEXT("/modulefilename:"));
		if (0 != sIndex)
			return FALSE;
		dlg.m_strExternalProgramFileName = strTmp.erase(0, strlen(TEXT("/modulefilename:")));

		// �õ���½��
		strTmp = arr.GetAt(1);
		sIndex = strTmp.find("/account:");
		if (0 != sIndex)
			return FALSE;
		dlg.m_strAccount = strTmp.erase(0, strlen("/account:"));

		// �õ�����
		strTmp = arr.GetAt(2);
		sIndex = strTmp.find("/pw:");
		if (0 != sIndex)
			return FALSE;
		dlg.m_strPW = strTmp.erase(0, strlen("/pw:"));
	}

	//if (m_bSilenceMode)
	//{
	//	// ��Ĭ��ʽִ������
	//	m_pMainWnd = NULL;
	//	dlg.SetConfigFile(GetAppDirectory() + UPDATE_CONFIG_FILENAME);
	//	dlg.CheckWantUpdateFile();
	//}
	//else
	{
		int nResponse = dlg.DoModal();
	}

	// �����Ի����
	CUISkinManager::Uninit();

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

// ����������
// ����ֵΪ TRUE ��ʾҪ�������к���ĳ���Ϊ FALSE ��ʾ��ֹ��������
//BOOL CGameUpdateApp::ParseCommandLine()
//{
//	CString sParameter;
//
//	// ��������в���
//
//	for (int i = 1; i < __argc; i++)
//	{
//		sParameter = __argv[i];
//		if (sParameter.Left(strlen(PARA_KEY_APP_NAME)).CompareNoCase(PARA_KEY_APP_NAME) == 0)
//		{
//			m_sAppName = sParameter.Mid(strlen(PARA_KEY_APP_NAME));
//		}
//		else if (sParameter.Left(strlen(PARA_KEY_CURRENTVERSION)).CompareNoCase(PARA_KEY_CURRENTVERSION) == 0)
//		{
//			m_sVersion = sParameter.Mid(strlen(PARA_KEY_CURRENTVERSION));
//		}
//		else if (sParameter.Left(strlen(PARA_KEY_CHECKURL)).CompareNoCase(PARA_KEY_CHECKURL) == 0)
//		{
//			m_sURL = sParameter.Mid(strlen(PARA_KEY_CHECKURL));
//		}
//		else if (sParameter.Left(strlen(PARA_KEY_NOTIFYWINDOW)).CompareNoCase(PARA_KEY_NOTIFYWINDOW) == 0)
//		{
//			m_iNotifyWindow = (DWORD)atoi(sParameter.Mid(strlen(PARA_KEY_NOTIFYWINDOW)));
//		}
//		else if (sParameter.Left(strlen(PARA_KEY_NOTIFYWINDOWTITLE)).CompareNoCase(PARA_KEY_NOTIFYWINDOWTITLE) == 0)
//		{
//			m_sNotifyWindowTitle = sParameter.Mid(strlen(PARA_KEY_NOTIFYWINDOWTITLE));
//		}
//		else if (sParameter.Left(strlen(PARA_KEY_NOTIFYFINISH)).CompareNoCase(PARA_KEY_NOTIFYFINISH) == 0)
//		{
//			m_iNotifyFinish = (DWORD)atoi(sParameter.Mid(strlen(PARA_KEY_NOTIFYFINISH)));
//		}
//		else if (sParameter.Left(strlen(PARA_KEY_SILENCE)).CompareNoCase(PARA_KEY_SILENCE) == 0)
//		{
//			m_bSilenceMode = (sParameter.Mid(strlen(PARA_KEY_SILENCE)) == "1");
//		}
//		else
//		{
//			LOG(0, 0, "Invalid parameter : %s", sParameter.GetBuffer(0));
//			return FALSE;
//		}
//	}
//
//	m_sAppName.TrimLeft();
//	m_sAppName.TrimRight();
//	if (m_sAppName.IsEmpty())
//	{
//		LOG(0, 0, "Invalid parameters.");
//		return FALSE;
//	}
//
//	m_sVersion.TrimLeft();
//	m_sVersion.TrimRight();
//	if (m_sVersion.IsEmpty())
//	{
//		LOG(0, 0, "Invalid parameters.");
//		return FALSE;
//	}
//
//	// ������������ļ����ж��Ƿ����°汾���������
//	if (CheckUpdate())
//	{
//		if (!m_bSilenceMode)
//		{
//			CRMessageBox MsgBox;
//			MsgBox.m_sTitle = m_sAppName + " " + STRING(STR_AUTO_UPDATE, "Auto Update");
//			MsgBox.m_sPromptMessage = STRING(STR_PROMPT_NEWER_VERSION_AVAILABLE, "��ǰ�и��°汾��������ã��Ƿ�����������");
//			MsgBox.m_bOption1 = FALSE;
//			MsgBox.m_sOptionPromptMessage1 = STRING(STR_OPTION_UPGRADE_IN_BACKGROUND , "�������г��򣬴Ӻ�ִ̨��������");
//			MsgBox.m_iType = MB_YESNO + MB_ICONQUESTION;
//			if (IDOK == MsgBox.DoModal())
//			{
//				m_bSilenceMode = MsgBox.m_bOption1;
//
//				if (MsgBox.m_iID == IDYES)
//				{
//					return TRUE;
//				}
//				else
//				{
//					return FALSE;
//				}
//			}
//			else
//			{
//				return FALSE;
//			}
//		}
//	}
//
//	return TRUE;
//}

// �������վ�������������ļ�������Ƿ����°汾���������
// ����ֵΪ TRUE ��ʾ���°汾���������
//BOOL CGameUpdateApp::CheckUpdate()
//{
//	CString sConfigFilename = GetAppDirectory() + UPDATE_CONFIG_FILENAME;
//
//	// ��ָ����URL�������������ļ�
//	if (!m_sURL.IsEmpty())
//	{
//		if (Internet::InternetGetURL(m_sURL.GetBuffer(0), sConfigFilename.GetBuffer(0))
//			!= Internet::INTERNET_SUCCESS)
//		{
//			LOG(0, 0, "Fail to download file %s", m_sURL.GetBuffer(0));
//			return FALSE;
//		}
//	}
//
//	// �����������ļ�ȡ����������汾��
//	CString sKey = "Version";
//	const int BUFFER_SIZE = 512;
//	char acBuffer[BUFFER_SIZE] = {0};
//	GetPrivateProfileString(SECTION_UPDATE, sKey.GetBuffer(0), ""
//		, acBuffer, BUFFER_SIZE, sConfigFilename.GetBuffer(0));
//	CString sVersion = (char*)acBuffer;
//
//	// �뵱ǰ����汾�űȽ���ȷ���Ƿ���Ҫ����
//	if (sVersion > m_sVersion)
//	{
//		return TRUE;
//	}
//	else
//	{
//		return FALSE;
//	}
//}

// ��ʼ��ȫ���ַ���������
void CGameUpdateApp::InitStringTable(enum enLANGUAGE Language)
{
	if (Language < LANGUAGE_BOTTOM)
	{
		g_LanguageID = Language;
	}
	else
	{
		// ���ݲ���ϵͳ���Դ���ȷ�������������Դ���
		switch (GetSystemDefaultLangID()) 
		{
		case 0x0804: // Chinese (PRC)
		case 0x1004: // Chinese (Singapore)
			g_LanguageID = LANGUAGE_GB;
			break;
		case 0x0404: // Chinese (Taiwan)
		case 0x0c04: // Chinese (Hong Kong SAR, PRC)
		case 0x1404: // Chinese (Macao)
			g_LanguageID = LANGUAGE_BIG5;
			break;
		default:
			g_LanguageID = LANGUAGE_ENGLISH;
			break;
		}
	}

// for test
//g_LanguageID=LANGUAGE_ENGLISH;

	// ��ʼ��ȫ���ַ���������

	g_String[STR_NULL].Set("", "", "");
	g_String[STR_AUTO_UPDATE].Set("�Զ�����", "�۰ʤɯ�", "Auto Update");
	g_String[STR_APP_ALREADY_RUNNING].Set("���������Ѿ��������У�", "�ɯŵ{�Ǥw�g�b�B�椤�I", "Auto update program is already running."); //  �˳�Ӧ�ó�����ʾ�����ĵ�\n�˳�

	g_String[STR_PROMPT_NEWER_VERSION_AVAILABLE].Set("��ǰ�и��°汾��������ã��Ƿ�����������", "��e����s�������n��i�ΡA�O�_�ߧY�ɯšH", "There is a newer version available. Do you want to update right now?");
	g_String[STR_OPTION_UPGRADE_IN_BACKGROUND].Set("��ִ̨������", "��x����ɯ�", "Run updating in background mode");
	g_String[STR_PROMPT_UPGRADE_READY].Set("����׼��������", "�ɯŷǳƴN���C", "Update is ready. Please press \"Start update\" button to update.");
	g_String[STR_PROMPT_FAIL_TO_OPEN_UPDATE_CONFIG_FILE].Set("�����������ļ�ʧ�ܣ��޷�ִ��������", "���}�ɯŰt�m�ɮץ��ѡA�L�k����ɯšI", "Fail to open update config file. Update is canceled.");
	g_String[STR_PROMPT_DOWNLOADING_FILE].Set("���������ļ� %s", "���b�U���ɮ� %s", "Downloading file %s");
	g_String[STR_TOTAL_UPGRADE_PROGRESS].Set("�����ܽ��� [%d \/ %d]  �ٶ�: %.2fKB\/S", "�ɯ��`�i�� %d / %d", "Total progress %d / %d");
	g_String[STR_PROMPT_FAIL_IN_DOWNLOADING_FILES].Set("�����ļ� %s ʧ�ܣ�", "�U���ɮ� %s ���ѡI", "Fail in downloading file %s!");
	g_String[STR_PROMPT_FAIL_IN_VERIFYING_FILES].Set("У���ļ� %s ʧ�ܣ�", "�����ɮ� %s ���ѡI", "Fail in verifying file %s!");
	g_String[STR_PROMPT_FAIL_IN_UPDATING_FILES].Set("�����ļ� %s ʧ�ܣ�", "��s�ɮ� %s ���ѡI", "Fail in updating file %s!");
	g_String[STR_PROMPT_FAIL_IN_UPDATING].Set("����ʧ�ܣ�", "�ɯť��ѡI", "Fail in updating!");
	g_String[STR_PROMPT_UPGRADE_FINISHED].Set("������ϣ�", "�ɯŧ����I", "Update finished!");

	g_String[STR_BUTTON_START_UPGRADE].Set("��ʼ����", "�}�l�ɯ�", "Start update");
	g_String[STR_BUTTON_CANCEL_UPGRADE].Set("ȡ   ��", "�����ɯ�", "Cancel");

	g_String[STR_BUTTON_OK].Set("ȷ��(&O)", "�T�w(&O)", "&OK");
	g_String[STR_BUTTON_CANCEL].Set("ȡ��(&C)", "����(&C)", "&Cancel");
	g_String[STR_BUTTON_ABORT].Set("����(&A)", "���X(&A)", "&Abort");
	g_String[STR_BUTTON_IGANORE].Set("����(&I)", "����(&I)", "&Ignore");
	g_String[STR_BUTTON_RETRY].Set("����(&R)", "����(&R)", "&Retry");
	g_String[STR_BUTTON_CONTINUE].Set("����(&C)", "�~��(&C)", "&Continue");
	g_String[STR_BUTTON_YES].Set("��(&Y)", "�O(&Y)", "&Yes");
	g_String[STR_BUTTON_NO].Set("��(&N)", "�_(&N)", "&No");
	g_String[STR_BUTTON_CLOSE].Set("�ر�", "����", "Close");
	g_String[STR_BUTTON_APPLY].Set("Ӧ��(&A)", "�M��(&A)", "&Apply");

	g_String[STR_OTHER].Set("����", "��L", "Other");

	g_String[STR_ERROR].Set("����", "���~", "Error");
	g_String[STR_ERROR_MESSAGE].Set("����%s", "���~�G%s", "Error: %s");
}

BOOL CGameUpdateApp::IsAppRunning()
{
	// ��������������ֹͬʱ�����������ʵ��
	CString sMutex = m_sAppName + "GameUpdateMutex";
	if (::OpenMutex(MUTEX_ALL_ACCESS, FALSE, sMutex.GetBuffer(0)))
	{
		return TRUE;
	}
	else
	{
		::CreateMutex(0, FALSE, sMutex.GetBuffer(0));
		sMutex.ReleaseBuffer();
		return FALSE;
	}
}

//�ַ����ֽ�
void CGameUpdateApp::GetCmdLinePara(CStringArray &paraArr)
{
	paraArr.RemoveAll();
	//--"/modulefilename:GamePlaza.exe /account:a11111 /pw:dc483e80a7a0bd9ef71d8cf973673924"
	CString strLine=::AfxGetApp()->m_lpCmdLine;
	if(strLine.IsEmpty())
		return;

	int nLength=strLine.GetLength();
	char *buf=new char[nLength+1];
	lstrcpyn(buf, strLine, sizeof(char)*(nLength+1));
	char *p=buf;
	for(int i=0;i<128;i++)
	{
		if(buf[i]==0x20)//�ո�
		{
			buf[i]=0x00;
			paraArr.Add(p);
			i++;
			p=buf+i;
		}
		if(buf[i]==0x00)
		{
			paraArr.Add(p);
			break;
		}
	}

	delete []buf;
}