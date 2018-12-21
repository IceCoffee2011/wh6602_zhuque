#include "Stdafx.h"
#include "ServiceLoader.h"
#include "ServiceLoaderDlg.h"

//////////////////////////////////////////////////////////////////////////

//ȫ��ϵͳ����
CInitParamter g_InitParamer;

BEGIN_MESSAGE_MAP(CSystemOptionDlg, CDialog)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CServiceLoaderDlg, CDialog)
	ON_BN_CLICKED(IDC_START, OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, OnBnClickedStop)
	ON_BN_CLICKED(IDC_CREATE, OnBnClickedCreate)
	ON_BN_CLICKED(IDC_OPEN, OnBnClickedOpen)
	ON_BN_CLICKED(IDC_OPTION, OnBnClickedOption)
	ON_BN_CLICKED(IDC_SYSTEM, OnBnClickedSystem)
//	ON_BN_CLICKED(IDC_SAVE_OPTION, OnBnClickedSaveOption)
	ON_WM_QUERYENDSESSION()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CSystemOptionDlg::CSystemOptionDlg() : CDialog(IDD_SYSTEM_OPTION)
{
}

//��������
CSystemOptionDlg::~CSystemOptionDlg()
{
}

//�ؼ����໯
void CSystemOptionDlg::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
}

//��ʼ������
BOOL CSystemOptionDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//��������
	((CEdit *)GetDlgItem(IDC_USER_DATABASE_PORT))->LimitText(5);
	((CEdit *)GetDlgItem(IDC_USER_DATABASE_USER))->LimitText(31);
	((CEdit *)GetDlgItem(IDC_USER_DATABASE_PASS))->LimitText(31);
	((CEdit *)GetDlgItem(IDC_USER_DATABASE_NAME))->LimitText(31);
	((CEdit *)GetDlgItem(IDC_SERVER_DATABASE_PORT))->LimitText(5);
	((CEdit *)GetDlgItem(IDC_SERVER_DATABASE_USER))->LimitText(31);
	((CEdit *)GetDlgItem(IDC_SERVER_DATABASE_PASS))->LimitText(31);
	((CEdit *)GetDlgItem(IDC_SERVER_DATABASE_NAME))->LimitText(31);

	//���ز���
//	CInitParamter InitParamter;
//	InitParamter.LoadInitParamter();

	//��¼���ݿ�
	SetDlgItemInt(IDC_USER_DATABASE_PORT,g_InitParamer.m_wGameUserDBPort,FALSE);
	SetDlgItemText(IDC_USER_DATABASE_USER,g_InitParamer.m_szGameUserDBUser);
	SetDlgItemText(IDC_USER_DATABASE_PASS,g_InitParamer.m_szGameUserDBPass);
	SetDlgItemText(IDC_USER_DATABASE_NAME,g_InitParamer.m_szGameUserDBName);

	//��Ϣ���ݿ�
	SetDlgItemInt(IDC_SERVER_DATABASE_PORT,g_InitParamer.m_wServerInfoDBPort,FALSE);
	SetDlgItemText(IDC_SERVER_DATABASE_USER,g_InitParamer.m_szServerInfoDBUser);
	SetDlgItemText(IDC_SERVER_DATABASE_PASS,g_InitParamer.m_szServerInfoDBPass);
	SetDlgItemText(IDC_SERVER_DATABASE_NAME,g_InitParamer.m_szServerInfoDBName);

	//��¼���ݿ��ַ
	DWORD dwDataBaseIP=inet_addr(g_InitParamer.m_szGameUserDBAddr);
	if (dwDataBaseIP==INADDR_NONE)
	{
		LPHOSTENT lpHost=gethostbyname(g_InitParamer.m_szGameUserDBAddr);
		if (lpHost!=NULL) dwDataBaseIP=((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}
	CIPAddressCtrl * pDataBaseIP=(CIPAddressCtrl *)GetDlgItem(IDC_USER_DATABASE_IP);
	pDataBaseIP->SetAddress(ntohl(dwDataBaseIP));

	//��Ϣ���ݿ��ַ
	dwDataBaseIP=inet_addr(g_InitParamer.m_szServerInfoDBAddr);
	if (dwDataBaseIP==INADDR_NONE)
	{
		LPHOSTENT lpHost=gethostbyname(g_InitParamer.m_szServerInfoDBAddr);
		if (lpHost!=NULL) dwDataBaseIP=((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}
	pDataBaseIP=(CIPAddressCtrl *)GetDlgItem(IDC_SERVER_DATABASE_IP);
	pDataBaseIP->SetAddress(ntohl(dwDataBaseIP));

	//���ķ�����
	dwDataBaseIP=inet_addr(g_InitParamer.m_szCenterServerAddr[0]);
	if (dwDataBaseIP==INADDR_NONE)
	{
		LPHOSTENT lpHost=gethostbyname(g_InitParamer.m_szCenterServerAddr[0]);
		if (lpHost!=NULL) dwDataBaseIP=((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}
	pDataBaseIP=(CIPAddressCtrl *)GetDlgItem(IDC_CENTER_SERVER_IP);
	pDataBaseIP->SetAddress(ntohl(dwDataBaseIP));

	return TRUE;
}

//ȷ������
void CSystemOptionDlg::OnOK()
{
	//��ȡ����
//	CInitParamter InitParamter;

	//��¼���ݿ� 
	g_InitParamer.m_wGameUserDBPort=GetDlgItemInt(IDC_USER_DATABASE_PORT);
	GetDlgItemText(IDC_USER_DATABASE_USER,g_InitParamer.m_szGameUserDBUser,sizeof(g_InitParamer.m_szGameUserDBUser));
	GetDlgItemText(IDC_USER_DATABASE_PASS,g_InitParamer.m_szGameUserDBPass,sizeof(g_InitParamer.m_szGameUserDBPass));
	GetDlgItemText(IDC_USER_DATABASE_NAME,g_InitParamer.m_szGameUserDBName,sizeof(g_InitParamer.m_szGameUserDBName));

	//��¼���ݿ��ַ
	DWORD dwDataBaseIP=INADDR_NONE;
	BYTE * pAddrByte=(BYTE *)&dwDataBaseIP;
	((CIPAddressCtrl *)GetDlgItem(IDC_USER_DATABASE_IP))->GetAddress(dwDataBaseIP);
	_snprintf(g_InitParamer.m_szGameUserDBAddr,sizeof(g_InitParamer.m_szGameUserDBAddr),TEXT("%d.%d.%d.%d"),
		pAddrByte[3],pAddrByte[2],pAddrByte[1],pAddrByte[0]);

	//��Ϣ���ݿ� 
	g_InitParamer.m_wServerInfoDBPort=GetDlgItemInt(IDC_SERVER_DATABASE_PORT);
	GetDlgItemText(IDC_SERVER_DATABASE_USER,g_InitParamer.m_szServerInfoDBUser,sizeof(g_InitParamer.m_szServerInfoDBUser));
	GetDlgItemText(IDC_SERVER_DATABASE_PASS,g_InitParamer.m_szServerInfoDBPass,sizeof(g_InitParamer.m_szServerInfoDBPass));
	GetDlgItemText(IDC_SERVER_DATABASE_NAME,g_InitParamer.m_szServerInfoDBName,sizeof(g_InitParamer.m_szServerInfoDBName));

	//��Ϣ���ݿ��ַ
	dwDataBaseIP=INADDR_NONE;
	pAddrByte=(BYTE *)&dwDataBaseIP;
	((CIPAddressCtrl *)GetDlgItem(IDC_SERVER_DATABASE_IP))->GetAddress(dwDataBaseIP);
	_snprintf(g_InitParamer.m_szServerInfoDBAddr,sizeof(g_InitParamer.m_szServerInfoDBAddr),TEXT("%d.%d.%d.%d"),
		pAddrByte[3],pAddrByte[2],pAddrByte[1],pAddrByte[0]);

	//���ķ�����
	dwDataBaseIP=INADDR_NONE;
	pAddrByte=(BYTE *)&dwDataBaseIP;
	((CIPAddressCtrl *)GetDlgItem(IDC_CENTER_SERVER_IP))->GetAddress(dwDataBaseIP);
	_snprintf(g_InitParamer.m_szCenterServerAddr[0],sizeof(g_InitParamer.m_szCenterServerAddr[0]),TEXT("%d.%d.%d.%d"),
		pAddrByte[3],pAddrByte[2],pAddrByte[1],pAddrByte[0]);

	//��������
	g_InitParamer.SaveInitParamter(false);

	__super::OnOK();
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CServiceLoaderDlg::CServiceLoaderDlg() : CDialog(IDD_SERVICELOADER_DIALOG)
{
}

//��������
CServiceLoaderDlg::~CServiceLoaderDlg()
{
}

//�ؼ���
void CServiceLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVICE_EVENT, m_RichEditCtrl);
}

//��ʼ������
BOOL CServiceLoaderDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//���ز���
	g_InitParamer.LoadInitParamter();

	//��������
	LPCTSTR pszCmdLine=AfxGetApp()->m_lpCmdLine;
	if (pszCmdLine[0]==0)
	{
	     pszCmdLine=".\\gameserver.ini";
	}
	if (pszCmdLine[0]!=0)
	{
		TCHAR szDescribe[128]=TEXT("");
		bool bSuccess=m_ServiceParameter.LoadParameter(pszCmdLine);
		if (bSuccess==true)
		{
			CFileFind finder;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ�
			BOOL ifFind = finder.FindFile(pszCmdLine);
			if( !ifFind )
			{
				//����¼�
				CTraceService::TraceString(TEXT("ini�ļ�������"),TraceLevel_Normal);	
			}

			//by leon ��ʼ��ϵͳ����
			g_InitParamer.InitParamter();

			CString szRetTmp;
			LOAD_STRING(pszCmdLine, "CenterServer", "IP", szRetTmp);
			lstrcpyn(g_InitParamer.m_szCenterServerAddr[0], szRetTmp, CountArray(g_InitParamer.m_szCenterServerAddr[0]));		
			g_InitParamer.m_wCenterPort[0] = LOAD_INT(pszCmdLine, "CenterServer", "Port");
			g_InitParamer.m_wCenterServerCount = 1;

			int iServerCount = LOAD_INT(pszCmdLine, "CenterServer", "Count");
			for (int i=1; i<iServerCount && i<MAX_CENTER_SERVER; i++)
			{
				char key[32];
				memset(key, 0, 32);
				sprintf(key, "CenterServer%d", i);

				szRetTmp = "";
				LOAD_STRING(pszCmdLine, key, "IP", szRetTmp);
				if (szRetTmp == "")break;   //�����������ֹ

				lstrcpyn(g_InitParamer.m_szCenterServerAddr[i], szRetTmp, CountArray(g_InitParamer.m_szCenterServerAddr[i]));		
				g_InitParamer.m_wCenterPort[i] = LOAD_INT(pszCmdLine, key, "Port");

				g_InitParamer.m_wCenterServerCount = i+1;
			}
			
			g_InitParamer.m_wGameUserDBPort = LOAD_INT(pszCmdLine, "GameUserDB", "Port");
			szRetTmp = "";
			LOAD_STRING(pszCmdLine, "GameUserDB", "IP", szRetTmp);
			lstrcpyn(g_InitParamer.m_szGameUserDBAddr, szRetTmp, CountArray(g_InitParamer.m_szGameUserDBAddr));
			szRetTmp = "";
			LOAD_STRING(pszCmdLine, "GameUserDB", "DBName", szRetTmp);
			lstrcpyn(g_InitParamer.m_szGameUserDBName, szRetTmp, CountArray(g_InitParamer.m_szGameUserDBName));
			szRetTmp = "";
			LOAD_STRING(pszCmdLine, "GameUserDB", "DBUser", szRetTmp);
			lstrcpyn(g_InitParamer.m_szGameUserDBUser, szRetTmp, CountArray(g_InitParamer.m_szGameUserDBUser));
			szRetTmp = "";
			LOAD_STRING(pszCmdLine, "GameUserDB", "DBPass", szRetTmp);
			CXOREncrypt::CrevasseData(szRetTmp, g_InitParamer.m_szGameUserDBPass,sizeof(g_InitParamer.m_szGameUserDBPass));
			
			g_InitParamer.m_wServerInfoDBPort = LOAD_INT(pszCmdLine, "ServerInfoDB", "Port");
			szRetTmp = "";
			LOAD_STRING(pszCmdLine, "ServerInfoDB", "IP", szRetTmp);
			lstrcpyn(g_InitParamer.m_szServerInfoDBAddr, szRetTmp, CountArray(g_InitParamer.m_szServerInfoDBAddr));
			szRetTmp = "";
			LOAD_STRING(pszCmdLine, "ServerInfoDB", "DBName", szRetTmp);
			lstrcpyn(g_InitParamer.m_szServerInfoDBName, szRetTmp, CountArray(g_InitParamer.m_szServerInfoDBName));
			szRetTmp = "";
			LOAD_STRING(pszCmdLine, "ServerInfoDB", "DBUser", szRetTmp);
			lstrcpyn(g_InitParamer.m_szServerInfoDBUser, szRetTmp, CountArray(g_InitParamer.m_szServerInfoDBUser));
			szRetTmp = "";
			LOAD_STRING(pszCmdLine, "ServerInfoDB", "DBPass", szRetTmp);
			CXOREncrypt::CrevasseData(szRetTmp, g_InitParamer.m_szServerInfoDBPass,sizeof(g_InitParamer.m_szServerInfoDBPass));

			//���ƽ���
			GetDlgItem(IDC_START)->EnableWindow(TRUE);
			GetDlgItem(IDC_OPTION)->EnableWindow(TRUE);
//			GetDlgItem(IDC_SAVE_OPTION)->EnableWindow(TRUE);

			//��ʾ��Ϣ
			_snprintf(szDescribe,sizeof(szDescribe),TEXT("��%s����Ϸ������سɹ�"),pszCmdLine);
			ShowErrorMessasge(szDescribe,TraceLevel_Normal);

			int iAutoStart = LOAD_INT(pszCmdLine, "Default", "AutoStart");
			if (iAutoStart > 0)
			{
				//�Զ���������
				this->OnBnClickedStart();
			}
		}
		else 
		{
			_snprintf(szDescribe,sizeof(szDescribe),TEXT("��%s����Ϸ�������ʧ��"),pszCmdLine);
			ShowErrorMessasge(szDescribe,TraceLevel_Exception);
		}
	}

	return TRUE;
}

//��Ϣ����
BOOL CServiceLoaderDlg::PreTranslateMessage(MSG * pMsg)
{
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE)) return TRUE;
	return __super::PreTranslateMessage(pMsg);
}

//ȷ����Ϣ
void CServiceLoaderDlg::OnOK()
{
	return;
}

//ȡ������
void CServiceLoaderDlg::OnCancel()
{
	//ֹͣ����
	if (m_GameService.GetInterface()!=NULL)
	{
		//ѯ���˳�
		bool bRuning=m_GameService->IsService();
		if (bRuning==true)
		{
			CString strMessage=TEXT("��Ϸ����������������У�ȷʵҪ�˳���");
			int iRetCode=AfxMessageBox(strMessage,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2);
			if (iRetCode!=IDYES) return;
		}

		//ֹͣ����
		if (bRuning==true) m_GameService->StopService();
	}

	__super::OnCancel();
}

//��������
void CServiceLoaderDlg::OnBnClickedStart()
{
	//Ч��״̬
	ASSERT(m_ServiceParameter.IsLoaded()==true);
	if (m_ServiceParameter.IsLoaded()==false) return;

	//���ز���
	g_InitParamer.LoadInitParamter();

	//��������
	tagGameServiceParameter GameServiceParameter;
	memset(&GameServiceParameter,0,sizeof(GameServiceParameter));
	const tagOptionParameter & OptionParameter=m_ServiceParameter.GetOptionParameter();

	//��ȡ����
/*	const DWORD dwDataBaseAddr=inet_addr(OptionParameter.szDataBaseAddr);
	const tagDBConnectInfo * pDBConnectInfo=GetDBConnectInfo(dwDataBaseAddr);
	if (pDBConnectInfo==NULL)
	{
		ShowErrorMessasge(TEXT("��Ϸ���ݿ���Ϣ�����ڣ���Ϸ�����������ʧ��"),TraceLevel_Exception);
		return;
	}
*/
	//�������
	GameServiceParameter.GameServiceOption=OptionParameter.GameServiceOption;
	
	GameServiceParameter.wCenterCount = g_InitParamer.m_wCenterServerCount;
	for (int i=0; i<MAX_CENTER_SERVER; i++)
	{
		GameServiceParameter.dwCenterAddr[i]=inet_addr(g_InitParamer.m_szCenterServerAddr[i]);
		GameServiceParameter.wCenterPort[i]=g_InitParamer.m_wCenterPort[i];
	}
	lstrcpyn(GameServiceParameter.szModule,OptionParameter.szModuleName,CountArray(GameServiceParameter.szModule));

	//�û����ݿ�
	GameServiceParameter.GameUserDBInfo.wDataBasePort=g_InitParamer.m_wGameUserDBPort;
	GameServiceParameter.GameUserDBInfo.dwDataBaseAddr=inet_addr(g_InitParamer.m_szGameUserDBAddr);
	lstrcpyn(GameServiceParameter.GameUserDBInfo.szDataBaseUser,g_InitParamer.m_szGameUserDBUser,CountArray(GameServiceParameter.GameUserDBInfo.szDataBaseUser));
	lstrcpyn(GameServiceParameter.GameUserDBInfo.szDataBasePass,g_InitParamer.m_szGameUserDBPass,CountArray(GameServiceParameter.GameUserDBInfo.szDataBasePass));
	lstrcpyn(GameServiceParameter.GameUserDBInfo.szDataBaseName,g_InitParamer.m_szGameUserDBName,CountArray(GameServiceParameter.GameUserDBInfo.szDataBaseName));

	//��ȡ����
	GameServiceParameter.wTreasureDBCount = OptionParameter.m_wTreasureDBCount;
	for (int i=0; i<OptionParameter.m_wTreasureDBCount; i++)
	{
		//const DWORD dwDataBaseAddr=inet_addr(OptionParameter.szDataBaseAddr[i]);
		const tagDBConnectInfo * pDBConnectInfo=GetDBConnectInfo(OptionParameter.szDataBaseAddr[i]);
		if (pDBConnectInfo==NULL)
		{
			ShowErrorMessasge(TEXT("��Ϸ���ݿ���Ϣ�����ڣ���Ϸ�����������ʧ��"),TraceLevel_Exception);
			return;
		}

		//��Ϸ���ݿ�
		GameServiceParameter.GameScoreDBInfo[i].dwDataBaseAddr=pDBConnectInfo->dwDataBaseAddr;
		GameServiceParameter.GameScoreDBInfo[i].wDataBasePort=pDBConnectInfo->wDataBasePort;
		lstrcpyn(GameServiceParameter.GameScoreDBInfo[i].szDataBaseUser,pDBConnectInfo->szDataBaseUser,CountArray(GameServiceParameter.GameScoreDBInfo[i].szDataBaseUser));
		lstrcpyn(GameServiceParameter.GameScoreDBInfo[i].szDataBasePass,pDBConnectInfo->szDataBasePass,CountArray(GameServiceParameter.GameScoreDBInfo[i].szDataBasePass));
		lstrcpyn(GameServiceParameter.GameScoreDBInfo[i].szDataBaseName,OptionParameter.szDataBaseName,CountArray(GameServiceParameter.GameScoreDBInfo[i].szDataBaseName));
	}
	//��������
	if ((m_GameService.GetInterface()==NULL)&&(m_GameService.CreateInstance()==false))
	{
		ShowErrorMessasge(TEXT("��Ϸ�����������ʧ��"),TraceLevel_Exception);
		return;
	}

	//��������
	TCHAR szDescribe[128]=TEXT("");
	if (m_GameService->StartService(&GameServiceParameter)==true)
	{
		//�������
		GetDlgItem(IDC_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_START)->EnableWindow(FALSE);
//		GetDlgItem(IDC_CREATE)->EnableWindow(FALSE);
//		GetDlgItem(IDC_OPEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_OPTION)->EnableWindow(FALSE);

		//��ʾ��Ϣ
		_snprintf(szDescribe,sizeof(szDescribe),TEXT("��%s - %ld�����������ɹ�"),GameServiceParameter.GameServiceOption.szGameRoomName,GameServiceParameter.GameServiceOption.wServerID);
		ShowErrorMessasge(szDescribe,TraceLevel_Normal);

		//���ñ���
		TCHAR szTitle[128]=TEXT("");
		_snprintf(szTitle,sizeof(szTitle),TEXT("��Ϸ���� [����] - %s - %s"),GameServiceParameter.GameServiceOption.szGameRoomName,GameServiceParameter.szModule);
		SetWindowText(szTitle);
	}
	else 
	{
		_snprintf(szDescribe,sizeof(szDescribe),TEXT("��%s����������ʧ��"),GameServiceParameter.GameServiceOption.szGameRoomName);
		ShowErrorMessasge(szDescribe,TraceLevel_Exception);
	}

	return;
}

//ֹͣ����
void CServiceLoaderDlg::OnBnClickedStop()
{
	if ((m_GameService.GetInterface()!=NULL)&&(m_GameService->StopService()==true))
	{
		SetWindowText(TEXT("��Ϸ���� - [ֹͣ]"));
		GetDlgItem(IDC_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
//		GetDlgItem(IDC_CREATE)->EnableWindow(TRUE);
//		GetDlgItem(IDC_OPEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_OPTION)->EnableWindow(TRUE);
		ShowErrorMessasge(TEXT("��Ϸ�������ֹͣ�ɹ�"),TraceLevel_Normal);
	}

	return;
}

//��������
void CServiceLoaderDlg::OnBnClickedCreate()
{
	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);

	//ѡ��ģ��
	LPCTSTR pszFilter=TEXT("��Ϸ������� ��*.DLL��|*.DLL||");
	CFileDialog DlgOpenParameter(TRUE,NULL,NULL,OFN_NOCHANGEDIR|OFN_HIDEREADONLY,pszFilter);
	DlgOpenParameter.m_pOFN->lpstrInitialDir=szPath;
	if (DlgOpenParameter.DoModal()!=IDOK) return;

	//��������
	bool bSuccess=m_ServiceParameter.CreateParameter(DlgOpenParameter.GetPathName());
	if (bSuccess==true)	ShowErrorMessasge(TEXT("��Ϸ���䴴���ɹ�"),TraceLevel_Normal);

	//���ƽ���
	GetDlgItem(IDC_START)->EnableWindow(bSuccess?TRUE:FALSE);
	GetDlgItem(IDC_OPTION)->EnableWindow(bSuccess?TRUE:FALSE);
//	GetDlgItem(IDC_SAVE_OPTION)->EnableWindow(bSuccess?TRUE:FALSE);

	return;
}

//�򿪷���
void CServiceLoaderDlg::OnBnClickedOpen()
{
	return;
}

//���÷���
void CServiceLoaderDlg::OnBnClickedOption()
{
	m_ServiceParameter.ConfigParameter();
	return;
}

//ϵͳ����
void CServiceLoaderDlg::OnBnClickedSystem()
{
	CSystemOptionDlg SystemOptionDlg;
	SystemOptionDlg.DoModal();

	return;
}

//��������
void CServiceLoaderDlg::OnBnClickedSaveOption()
{
	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);

	//ѡ���ļ�
	LPCTSTR pszFilter=TEXT("���������ļ� ��*.ROP��|*.ROP||");
	CFileDialog DlgSaveParameter(FALSE,TEXT("ROP"),NULL,OFN_NOCHANGEDIR|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,pszFilter);
	DlgSaveParameter.m_pOFN->lpstrInitialDir=szPath;
	if (DlgSaveParameter.DoModal()!=IDOK) return;

	//�����ļ�
	if (m_ServiceParameter.SaveParameter(DlgSaveParameter.GetPathName())==false)
	{
		ShowErrorMessasge(TEXT("���������ļ�����ʧ��"),TraceLevel_Exception);
		return;
	}

	return;
}

//��ȡ����
const tagDBConnectInfo * CServiceLoaderDlg::GetDBConnectInfo(const char* szDataBaseAddr)
{
	//Ѱ���ִ�
	tagDBConnectInfo * pDBConnectInfo=NULL;
	for (INT_PTR i=0;i<m_DBConnectInfoArray.GetCount();i++)
	{
		pDBConnectInfo=&m_DBConnectInfoArray[i];
		if (std::string(pDBConnectInfo->szDataBaseAddr)==szDataBaseAddr) return pDBConnectInfo;
	}

	//�������ݿ�
	CDataBaseAide ServerInfoAide;
	CDataBaseHelper ServerInfoDB;
	if (ServerInfoDB.CreateInstance()==false)
	{
		ShowErrorMessasge(TEXT("���ݿ��������ʧ�ܣ��޷���ȡ���ݿ���Ϣ"),TraceLevel_Exception);
		return NULL;
	}

	try
	{
		//�������ݿ�
		ServerInfoDB->SetConnectionInfo(g_InitParamer.m_szServerInfoDBAddr,g_InitParamer.m_wServerInfoDBPort,
			g_InitParamer.m_szServerInfoDBName,g_InitParamer.m_szServerInfoDBUser,g_InitParamer.m_szServerInfoDBPass);

		//������
		ServerInfoDB->OpenConnection();
		ServerInfoAide.SetDataBase(ServerInfoDB.GetInterface());

		//ת����ַ
//		TCHAR szDataBaseAdr[16]=TEXT("");
//		BYTE * pcbDataBaseAdr=(BYTE *)&dwDataBaseAddr;
//		_snprintf(szDataBaseAdr,sizeof(szDataBaseAdr),TEXT("%d.%d.%d.%d"),pcbDataBaseAdr[0],pcbDataBaseAdr[1],pcbDataBaseAdr[2],pcbDataBaseAdr[3]);

		//ִ�в�ѯ
		ServerInfoAide.ResetParameter();
		ServerInfoAide.AddParameter(TEXT("@strDataBaseAddr"),szDataBaseAddr);
		LONG lReturnCode=ServerInfoAide.ExecuteProcess(TEXT("GSP_LoadDataBaseInfo_V2"),true);

		//����ж�
		if (lReturnCode!=0L)
		{
			TCHAR szErrorDescribe[256]=TEXT("");
			ServerInfoAide.GetValue_String(TEXT("ErrorDescribe"),szErrorDescribe,CountArray(szErrorDescribe));
			ShowErrorMessasge(szErrorDescribe,TraceLevel_Exception);
			return NULL;
		}

		//��ȡ��Ϣ
		tagDBConnectInfo DBConnectInfo;
		memset(&DBConnectInfo,0,sizeof(DBConnectInfo));
		TCHAR szDBUser[512]=TEXT(""),szDBPass[512]=TEXT(""),szDBAddr[16]=TEXT("");
		ServerInfoAide.GetValue_String(TEXT("DBAddr"),szDBAddr,sizeof(szDBAddr));
		ServerInfoAide.GetValue_String(TEXT("DBUser"),szDBUser,sizeof(szDBUser));
		DBConnectInfo.wDataBasePort=ServerInfoAide.GetValue_WORD(TEXT("DBPort"));
		ServerInfoAide.GetValue_String(TEXT("DBPassword"),szDBPass,sizeof(szDBPass));
		CXOREncrypt::CrevasseData(szDBUser,DBConnectInfo.szDataBaseUser,sizeof(DBConnectInfo.szDataBaseUser));
		CXOREncrypt::CrevasseData(szDBPass,DBConnectInfo.szDataBasePass,sizeof(DBConnectInfo.szDataBasePass));

		//������Ϣ
		DBConnectInfo.dwDataBaseAddr=inet_addr(szDBAddr);
		lstrcpyn(DBConnectInfo.szDataBaseAddr,szDataBaseAddr,CountArray(DBConnectInfo.szDataBaseAddr));
		INT_PTR nIndex=m_DBConnectInfoArray.Add(DBConnectInfo);
		return &m_DBConnectInfoArray[nIndex];
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	return NULL;
}

//��ʾ����
void CServiceLoaderDlg::ShowErrorMessasge(LPCTSTR pszString, enTraceLevel TraceLevel)
{
	CTraceService::TraceString(pszString,TraceLevel);
}

//////////////////////////////////////////////////////////////////////////

BOOL CServiceLoaderDlg::OnQueryEndSession()
{
	//ֹͣ����
	if ((m_GameService.GetInterface()!=NULL)&&(m_GameService->IsService()==true))
	{
		CString strMessage=TEXT("��Ϸ����������������У�ϵͳҪ��ע���ػ�����ʧ��");
		ShowErrorMessasge(strMessage,TraceLevel_Warning);

		return FALSE;
	}


	return TRUE;
}
