#include "Stdafx.h"
#include "AtlBase.h"
#include "ControlServerDlg.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSystemOptionDlg, CDialog)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CControlServerDlg, CDialog)
	ON_BN_CLICKED(IDC_START_SERVICE, OnBnClickedStartService)
	ON_BN_CLICKED(IDC_STOP_SERVICE, OnBnClickedStopService)
	ON_BN_CLICKED(IDC_INIT_SERVICE, OnBnClickedInitService)
	ON_BN_CLICKED(IDC_SEND_COMMAND, OnBnClickedSendCommand)
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
//	((CEdit *)GetDlgItem(IDC_LISTEN_PORT))->LimitText(5);
//	((CEdit *)GetDlgItem(IDC_MAX_CONNECT))->LimitText(3);
//	((CEdit *)GetDlgItem(IDC_MAIN_PAGE))->LimitText(31);
	((CEdit *)GetDlgItem(IDC_USER_DATABASE_PORT))->LimitText(5);
	((CEdit *)GetDlgItem(IDC_USER_DATABASE_USER))->LimitText(31);
	((CEdit *)GetDlgItem(IDC_USER_DATABASE_PASS))->LimitText(31);
	((CEdit *)GetDlgItem(IDC_USER_DATABASE_NAME))->LimitText(31);

	//���ز���
//	CInitParamter InitParamter;
//	InitParamter.LoadInitParamter();

	//���ÿؼ�
//	SetDlgItemInt(IDC_LISTEN_PORT,InitParamter.m_wListenPort,FALSE);
//	SetDlgItemInt(IDC_MAX_CONNECT,InitParamter.m_wMaxConnect,FALSE);
	
	SetDlgItemInt(IDC_CENTER_PORT,g_InitParamter.m_wCenterServerPort,FALSE);

	//��¼���ݿ�
	SetDlgItemInt(IDC_USER_DATABASE_PORT,g_InitParamter.m_wUserDataBasePort,FALSE);
	SetDlgItemText(IDC_USER_DATABASE_USER,g_InitParamter.m_szUserDataBaseUser);
	SetDlgItemText(IDC_USER_DATABASE_PASS,g_InitParamter.m_szUserDataBasePass);
	SetDlgItemText(IDC_USER_DATABASE_NAME,g_InitParamter.m_szUserDataBaseName);

	//��¼���ݿ��ַ
	DWORD dwDataBaseIP=inet_addr(g_InitParamter.m_szUserDataBaseAddr);
	if (dwDataBaseIP==INADDR_NONE)
	{
		LPHOSTENT lpHost=gethostbyname(g_InitParamter.m_szUserDataBaseAddr);
		if (lpHost!=NULL) dwDataBaseIP=((LPIN_ADDR)lpHost->h_addr)->s_addr;
	}
	CIPAddressCtrl * pDataBaseIP=(CIPAddressCtrl *)GetDlgItem(IDC_USER_DATABASE_IP);
	pDataBaseIP->SetAddress(ntohl(dwDataBaseIP));

	//��վ��ַ
//	if (InitParamter.m_szMainPage[0]==0) SetDlgItemText(IDC_MAIN_PAGE,szStationPage);
//	else SetDlgItemText(IDC_MAIN_PAGE,InitParamter.m_szMainPage);

	//���ķ�����
	dwDataBaseIP=inet_addr(g_InitParamter.m_szCenterServerAddr);
	if (dwDataBaseIP==INADDR_NONE)
	{
		LPHOSTENT lpHost=gethostbyname(g_InitParamter.m_szCenterServerAddr);
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
	CInitParamter InitParamter;
//	InitParamter.m_wListenPort=GetDlgItemInt(IDC_LISTEN_PORT);
//	InitParamter.m_wMaxConnect=GetDlgItemInt(IDC_MAX_CONNECT);

	//��¼���ݿ� 
	InitParamter.m_wUserDataBasePort=GetDlgItemInt(IDC_USER_DATABASE_PORT);
	GetDlgItemText(IDC_USER_DATABASE_USER,InitParamter.m_szUserDataBaseUser,sizeof(InitParamter.m_szUserDataBaseUser));
	GetDlgItemText(IDC_USER_DATABASE_PASS,InitParamter.m_szUserDataBasePass,sizeof(InitParamter.m_szUserDataBasePass));
	GetDlgItemText(IDC_USER_DATABASE_NAME,InitParamter.m_szUserDataBaseName,sizeof(InitParamter.m_szUserDataBaseName));

	//��¼���ݿ��ַ
	DWORD dwDataBaseIP=INADDR_NONE;
	BYTE * pAddrByte=(BYTE *)&dwDataBaseIP;
	((CIPAddressCtrl *)GetDlgItem(IDC_USER_DATABASE_IP))->GetAddress(dwDataBaseIP);
	_snprintf(InitParamter.m_szUserDataBaseAddr,sizeof(InitParamter.m_szUserDataBaseAddr),TEXT("%d.%d.%d.%d"),
		pAddrByte[3],pAddrByte[2],pAddrByte[1],pAddrByte[0]);

	//��վ��ַ
//	GetDlgItemText(IDC_MAIN_PAGE,InitParamter.m_szMainPage,sizeof(InitParamter.m_szMainPage));
//	if (lstrcmp(InitParamter.m_szMainPage,szStationPage)==0) InitParamter.m_szMainPage[0]=0;

	//���ķ�����
	dwDataBaseIP=INADDR_NONE;
	pAddrByte=(BYTE *)&dwDataBaseIP;
	((CIPAddressCtrl *)GetDlgItem(IDC_CENTER_SERVER_IP))->GetAddress(dwDataBaseIP);
	_snprintf(InitParamter.m_szCenterServerAddr,sizeof(InitParamter.m_szCenterServerAddr),TEXT("%d.%d.%d.%d"),
		pAddrByte[0],pAddrByte[1],pAddrByte[2],pAddrByte[3]);

	//��������
	InitParamter.SaveInitParamter(false);

	__super::OnOK();
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CControlServerDlg::CControlServerDlg() : CDialog(IDD_CONTROLSERVER_DIALOG)
{
}

//��������
CControlServerDlg::~CControlServerDlg()
{
}

//�ؼ����໯
void CControlServerDlg::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVICE_EVENT, m_RichEditTrace);
}

//��ʼ������
BOOL CControlServerDlg::OnInitDialog()
{
	__super::OnInitDialog();

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//��������
	LPCTSTR pszCmdLine=AfxGetApp()->m_lpCmdLine;
	if (pszCmdLine[0]==0)
	{
	     pszCmdLine=".\\controlserver.ini";
	}
	if (pszCmdLine[0]!=0)
	{
		CFileFind finder;   //�����Ƿ����ini�ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ�
		BOOL ifFind = finder.FindFile(pszCmdLine);
		if( !ifFind )
		{
			//����¼�
			CTraceService::TraceString(TEXT("ini�ļ�������"),TraceLevel_Normal);	
		}

		g_InitParamter.m_wUserDataBasePort = LOAD_INT(pszCmdLine, "ControlServer", "DBPort");

		CString szRetTmp;
		LOAD_STRING(pszCmdLine, "ControlServer", "DBIP", szRetTmp);
		lstrcpyn(g_InitParamter.m_szUserDataBaseAddr, szRetTmp, CountArray(g_InitParamter.m_szUserDataBaseAddr));

		szRetTmp = "";
		LOAD_STRING(pszCmdLine, "ControlServer", "DBName", szRetTmp);
		lstrcpyn(g_InitParamter.m_szUserDataBaseName, szRetTmp, CountArray(g_InitParamter.m_szUserDataBaseName));

		szRetTmp = "";
		LOAD_STRING(pszCmdLine, "ControlServer", "DBUser", szRetTmp);
		lstrcpyn(g_InitParamter.m_szUserDataBaseUser, szRetTmp, CountArray(g_InitParamter.m_szUserDataBaseUser));

		szRetTmp = "";
		LOAD_STRING(pszCmdLine, "ControlServer", "DBPass", szRetTmp);
		CXOREncrypt::CrevasseData(szRetTmp, g_InitParamter.m_szUserDataBasePass,sizeof(g_InitParamter.m_szUserDataBasePass));	
		
		szRetTmp = "";
		LOAD_STRING(pszCmdLine, "CenterServer", "IP", szRetTmp);
		lstrcpyn(g_InitParamter.m_szCenterServerAddr, szRetTmp, CountArray(g_InitParamter.m_szCenterServerAddr));	

		g_InitParamter.m_wCenterServerPort = LOAD_INT(pszCmdLine, "CenterServer", "Port");
	
		int iAutoStart = LOAD_INT(pszCmdLine, "Default", "AutoStart");
		if (iAutoStart > 0)
		{
			//�Զ���������
			this->OnBnClickedStartService();
		}
	}

	return TRUE;
}

//ȷ����Ϣ
void CControlServerDlg::OnOK()
{
	return;
}

//ȡ������
void CControlServerDlg::OnCancel()
{
	//��ȡ״̬
	bool bRuning=m_ControlService.IsService();

	//ѯ���˳�
	if (bRuning==true)
	{
		CString strMessage=TEXT("���Ʒ��������������У�ȷʵҪ�˳���");
		int iRetCode=AfxMessageBox(strMessage,MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2);
		if (iRetCode!=IDYES) return;
	}

	//ֹͣ����
	if (bRuning==true)
	{
		m_ControlService.StopService();
	}

	__super::OnCancel();
}

//��Ϣ����
BOOL CControlServerDlg::PreTranslateMessage(MSG * pMsg)
{
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE)) return TRUE;
	return __super::PreTranslateMessage(pMsg);
}

//��������
void CControlServerDlg::OnBnClickedStartService()
{
	//��������
	if (m_ControlService.StartService()==true)
	{
		//���ñ���
		TCHAR szTitle[128]=TEXT("");
		_snprintf(szTitle,sizeof(szTitle),TEXT("���Ʒ��� - %d"), g_InitParamter.m_wUserDataBasePort);
		SetWindowText(szTitle);

		//���ý���
		GetDlgItem(IDC_STOP_SERVICE)->EnableWindow(TRUE);
		GetDlgItem(IDC_START_SERVICE)->EnableWindow(FALSE);

		//����¼�
		CTraceService::TraceString(TEXT("���Ʒ��������ɹ�"),TraceLevel_Normal);
	}
	else
	{
		//����¼�
		CTraceService::TraceString(TEXT("���Ʒ�������ʧ��"),TraceLevel_Exception);
	}

	return;
}

//ֹͣ����
void CControlServerDlg::OnBnClickedStopService()
{
	//ֹͣ����
	if (m_ControlService.StopService()==true)
	{
		//���ý���
		GetDlgItem(IDC_STOP_SERVICE)->EnableWindow(FALSE);
		GetDlgItem(IDC_START_SERVICE)->EnableWindow(TRUE);

		//����¼�
		CTraceService::TraceString(TEXT("���Ʒ���ֹͣ�ɹ�"),TraceLevel_Normal);
	}
	else
	{
		//����¼�
		CTraceService::TraceString(TEXT("���Ʒ���ֹͣʧ��"),TraceLevel_Exception);
	}

	return;
}

//ϵͳ����
void CControlServerDlg::OnBnClickedInitService()
{
	CSystemOptionDlg SystemOptionDlg;
	SystemOptionDlg.DoModal();

	return;
}

//����������
void CControlServerDlg::OnBnClickedSendCommand()
{
	char command[32];
	memset(command, 0, 32);
	GetDlgItemText(IDC_CONTROL_COMMAND_EDIT2,command, 32);

	CMD_CS_ControlCommandReq req;
	memset(&req, 0, sizeof(req));
	req.wCommandID = atoi(command);
	GetDlgItemText(IDC_CONTROL_COMMAND_PARAM_EDIT, req.szParam, 128);

	//Ŀǰֻ֧��2��������
	if (req.wCommandID <= 0 || req.wCommandID > 2)
	{
		CTraceService::TraceString(TEXT("��Ч��������"),TraceLevel_Warning);
		return;
	}

	//�·�����
	if (req.wCommandID == 1)  //����
	{
		req.dwUserID = atol(req.szParam);
		if (req.dwUserID <= 0)
		{
			CTraceService::TraceString(TEXT("�����������"),TraceLevel_Warning);
			return;
		}
	}
	else if (req.wCommandID == 2)  //����ά��
	{
		req.dwServerID = atol(req.szParam);
		if (req.dwServerID <= 0)
		{
			CTraceService::TraceString(TEXT("�����������"),TraceLevel_Warning);
			return;
		}
	}

	if (m_ControlService.SendCommand(&req, sizeof(req)))
	{
		char buf[256];
		memset(buf, 0, 256);
		sprintf(buf, "�·��ɹ�.����%d ����%s", req.wCommandID, req.szParam); 
		CTraceService::TraceString(buf,TraceLevel_Normal);
	}
	else
	{
		CTraceService::TraceString(TEXT("�����·�ʧ��"),TraceLevel_Warning);
	}
	
	return;
}

//////////////////////////////////////////////////////////////////////////

