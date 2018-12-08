// FlashEnterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FlashEnter.h"
#include "FlashEnterDlg.h"
#include ".\flashenterdlg.h"
#include "GlobalUnits.h"
#include "GameUserInfo.h"
//#include "XMLManager.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFlashEnterDlg �Ի���

using namespace std;


CFlashEnterDlg::CFlashEnterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFlashEnterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pGameLogon = NULL;
	m_dwListCountItem = 0;
	//m_XmlFile = NULL;
}

void CFlashEnterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ACCOUNT, m_UserID);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_UserPassWord);
	DDX_Control(pDX, IDC_IPADDRESS, m_ServerIP);
	DDX_Control(pDX, IDC_EDIT_GAMEID, m_GameID);
	DDX_Control(pDX, IDC_EDIT_ROOMID, m_RoomID);
	DDX_Control(pDX, IDC_EDIT_USERNUMS, m_TotalUserNums);
	DDX_Control(pDX, IDC_EDIT_PORT, m_ServerPort);
	DDX_Control(pDX, IDC_EDIT_USER_INDEX, m_UserIndex);
	DDX_Control(pDX, IDC_LIST1, m_ErrorList);
	DDX_Control(pDX, IDC_BT_LOGON_CONNECT, m_ConnectLogonServer);
	DDX_Control(pDX, IDC_BT_LOGON_EXIT_SERVER, m_ResetUserData);
	DDX_Control(pDX, IDC_BT_GAME_CONNECT, m_ConnectGameServer);
	DDX_Control(pDX, IDC_BT_EXIT_GAME_SERVER, m_ExitGameServer);
	DDX_Control(pDX, IDC_BT_AUTO_USER_QUEUE, m_AutoQueue);
	DDX_Control(pDX, IDC_BT_ALL_USER_EXIT_GAME, m_AllExitGame);
	DDX_Control(pDX, IDC_EDIT1, m_GameDLL);
	DDX_Control(pDX, IDC_IPADDRESS_GAME_SERVER, m_GameServerIP);
	DDX_Control(pDX, IDC_EDIT_GAME_SERVER_PORT, m_GameServerPort);
}

BEGIN_MESSAGE_MAP(CFlashEnterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BT_LOGON_CONNECT, OnBnClickedBtLogonConnect)
	ON_BN_CLICKED(IDC_BT_LOGON_EXIT_SERVER, OnBnClickedBtLogonExitServer)
	ON_BN_CLICKED(IDC_BT_GAME_CONNECT, OnBnClickedBtGameConnect)
	ON_BN_CLICKED(IDC_BT_EXIT_GAME_SERVER, OnBnClickedBtExitGameServer)
	ON_BN_CLICKED(IDC_BT_AUTO_USER_QUEUE, OnBnClickedBtAutoUserQueue)
	ON_BN_CLICKED(IDC_BT_ALL_USER_EXIT_GAME, OnBnClickedBtAllUserExitGame)
END_MESSAGE_MAP()


// CFlashEnterDlg ��Ϣ�������

BOOL CFlashEnterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	
	//****************************************************************************************************************
	//��¼������
	m_pGameLogon = NULL;
	m_pGameLogon = new CGameLogon();
	if (NULL == m_pGameLogon)
	{
		AfxMessageBox(TEXT("��¼���������ڴ���ʧ�ܣ����򼴽��˳���"));
		return FALSE;
	}
	CRect rcGameView(0,0,0,0);
	m_pGameLogon->Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcGameView,this,1000);
	m_pGameLogon->SetWindowPos(NULL, 0, 0, 1, 1, 0);
	m_pGameLogon->ShowWindow(SW_SHOW);
	m_pGameLogon->SetMainWnd(this);
	//****************************************************************************************************************


	//****************************************************************************************************************
	//��Ϸ������
	m_pGameServer = NULL;
	m_pGameServer = new CGameServer();
	if (NULL == m_pGameServer)
	{
		AfxMessageBox(TEXT("��Ϸ���������ڴ���ʧ�ܣ����򼴽��˳���"));
		return FALSE;
	}
	m_pGameServer->Create(NULL,NULL,WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcGameView,this,1001);
	m_pGameServer->SetWindowPos(NULL, 2, 2, 3, 3, 0);
	m_pGameServer->ShowWindow(SW_SHOW);
	m_pGameServer->SetMainWnd(this);
	//****************************************************************************************************************
	

	//��������
	m_UserID.SetWindowText("test");
	m_UserPassWord.SetWindowText("a");
	m_ServerIP.SetAddress(192, 168, 1, 108);
	m_ServerPort.SetWindowText("10000");
	m_GameID.SetWindowText("10");
	m_RoomID.SetWindowText("1");
	m_TotalUserNums.SetWindowText("512");
	m_UserIndex.SetWindowText("0");
	m_GameDLL.SetWindowText("");
	m_GameDLL.SetLimitText(64);
	m_GameServerIP.SetAddress(192, 168, 1, 108);
	m_GameServerPort.SetWindowText("10000");

	//m_XmlFile = NULL;
	//m_XmlFile = CXMLManager::GetInstance()->OpenXMLFile("AndroidInI.xml");
	//if (NULL != m_XmlFile)
	if(xmlParse.ReadXMLFile("AndroidInI.xml")==TRUE)
	{//��ȡ��Ϣ
		std:string strValue = "";
		//m_XmlFile->getValue("UserID", strValue, "test");
		xmlParse.getValue("UserID", strValue, "test");
		m_UserID.SetWindowText(strValue.c_str());

		strValue = "";
		xmlParse.getValue("PassWord", strValue, "a");
		m_UserPassWord.SetWindowText(strValue.c_str());

		strValue = "";
		xmlParse.getValue("ServerIP", strValue, "192.168.1.108");
		m_ServerIP.SetWindowText(strValue.c_str());

		strValue = "";
		xmlParse.getValue("ServerPort", strValue, "10000");
		m_ServerPort.SetWindowText(strValue.c_str());

		strValue = "";
		xmlParse.getValue("GameID", strValue, "10");
		m_GameID.SetWindowText(strValue.c_str());

		strValue = "";
		xmlParse.getValue("RoomID", strValue, "1");
		m_RoomID.SetWindowText(strValue.c_str());

		strValue = "";
		xmlParse.getValue("UserIndex", strValue, "0");
		m_UserIndex.SetWindowText(strValue.c_str());

		strValue = "";
		xmlParse.getValue("UserNums", strValue, "0");
		m_TotalUserNums.SetWindowText(strValue.c_str());

		strValue = "";
		xmlParse.getValue("GameDll", strValue, "0");
		m_GameDLL.SetWindowText(strValue.c_str());

		strValue = "";
		xmlParse.getValue("GameServerIP", strValue, "192.168.1.108");
		m_GameServerIP.SetWindowText(strValue.c_str());

		strValue = "";
		xmlParse.getValue("GameServerPort", strValue, "10000");
		m_GameServerPort.SetWindowText(strValue.c_str());
	}

	m_ConnectLogonServer.EnableWindow(TRUE);
	m_ResetUserData.EnableWindow(FALSE);
	m_ConnectGameServer.EnableWindow(FALSE);
	m_ExitGameServer.EnableWindow(FALSE);
	m_AutoQueue.EnableWindow(FALSE);
	m_AllExitGame.EnableWindow(FALSE);

	DWORD dwStyle = m_ErrorList.GetExtendedStyle(); //��ȡ��ǰ��չ��ʽ
	dwStyle |= LVS_EX_FULLROWSELECT; //ѡ��ĳ��ʹ���и�����report���ʱ��
	dwStyle |= LVS_EX_GRIDLINES; //�����ߣ�report���ʱ��
	dwStyle |= LVS_SHOWSELALWAYS; //itemǰ����checkbox�ؼ�
	m_ErrorList.SetExtendedStyle(dwStyle); //������չ���

	m_ErrorList.InsertColumn(0,"Index",LVCFMT_LEFT,50); //������
	m_ErrorList.InsertColumn(1,"SocketID",LVCFMT_LEFT,100); //������
	m_ErrorList.InsertColumn(2,"ErrorID",LVCFMT_LEFT,100);
	m_ErrorList.InsertColumn(3,"ErrorInfo",LVCFMT_LEFT,430);
	m_ErrorList.InsertColumn(4,"time",LVCFMT_LEFT,100);

	m_dwListCountItem = 0;

	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFlashEnterDlg::OnPaint() 
{
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������
	
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0, 128, 0));

	if (NULL != m_pGameLogon)
	{
		DWORD dwLogonSucNums = 0;
		DWORD dwLogonFailNums = 0;
		DWORD dwTCPConNums = 0;
		DWORD dwUserConNums = 0;
		m_pGameLogon->GetLogonInfo(dwLogonSucNums, dwLogonFailNums, dwTCPConNums, dwUserConNums);
		char chData[20] = {0};
		//��¼�û�����
		sprintf(chData, "%d", g_GlobalUnits.m_GameOption.dwUserNums);
		dc.TextOut(200, 193, chData);
		//��¼�ɹ��û���
		::memset(chData, 0, sizeof(chData));
		sprintf(chData, "%d", dwLogonSucNums);
		dc.TextOut(200, 218, chData);
		//��¼ʧ���û���
		::memset(chData, 0, sizeof(chData));
		sprintf(chData, "%d", dwLogonFailNums);
		dc.TextOut(200, 242, chData);
		//TCP������
		::memset(chData, 0, sizeof(chData));
		sprintf(chData, "%d", dwTCPConNums);
		dc.TextOut(143, 269, chData);
		//User������
		::memset(chData, 0, sizeof(chData));
		sprintf(chData, "%d", dwUserConNums);
		dc.TextOut(285, 269, chData);
	}

	if (NULL != m_pGameServer)
	{
		DWORD dwLogonSucNums = 0;
		DWORD dwLogonFailNums = 0;
		DWORD dwTCPConNums = 0;
		DWORD dwUserConNums = 0;
		m_pGameServer->GetLogonInfo(dwLogonSucNums, dwLogonFailNums, dwTCPConNums, dwUserConNums);
		char chData[20] = {0};
		//��¼�û�����
		sprintf(chData, "%d", g_GlobalUnits.m_GameOption.dwUserNums);
		dc.TextOut(560, 193, chData);
		//��¼�ɹ��û���
		::memset(chData, 0, sizeof(chData));
		sprintf(chData, "%d", dwLogonSucNums);
		dc.TextOut(560, 218, chData);
		//��¼ʧ���û���
		::memset(chData, 0, sizeof(chData));
		sprintf(chData, "%d", dwLogonFailNums);
		dc.TextOut(560, 242, chData);
		//TCP������
		::memset(chData, 0, sizeof(chData));
		sprintf(chData, "%d", dwTCPConNums);
		dc.TextOut(492, 269, chData);
		//User������
		::memset(chData, 0, sizeof(chData));
		sprintf(chData, "%d", dwUserConNums);
		dc.TextOut(635, 269, chData);


		//�Ŷ���Ϣ
		dwLogonSucNums = 0;
		dwLogonFailNums = 0;
		m_pGameServer->GetQueueInfo(dwLogonSucNums, dwLogonFailNums);
		//�Ŷӳɹ��û���
		::memset(chData, 0, sizeof(chData));
		sprintf(chData, "%d", dwLogonSucNums);
		dc.TextOut(265, 330, chData);
		//�Ŷ�ʧ���û���
		::memset(chData, 0, sizeof(chData));
		sprintf(chData, "%d", dwLogonFailNums);
		dc.TextOut(510, 330, chData);
	}

	

}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CFlashEnterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CFlashEnterDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	ShowWindow(SW_HIDE);
	return 0;
}

//���µ�¼��Ϣ
void CFlashEnterDlg::UpdateLogonInfo()
{
	CRect rect;
	rect.left = 200;
	rect.top = 193;
	rect.right = rect.left + 100;
	rect.bottom = rect.top + 70;
	InvalidateRect(&rect);
}

void CFlashEnterDlg::UpdateGameInfo()
{
	CRect rect;
	rect.left = 560;
	rect.top = 193;
	rect.right = rect.left + 100;
	rect.bottom = rect.top + 70;
	InvalidateRect(&rect);
}

void CFlashEnterDlg::UpdateLogonConnInfo()
{
	CRect rect;
	rect.left = 143;
	rect.top = 269;
	rect.right = rect.left + 30;
	rect.bottom = rect.top + 20;
	InvalidateRect(&rect);
	rect.left = 285;
	rect.top = 269;
	rect.right = rect.left + 30;
	rect.bottom = rect.top + 20;
	InvalidateRect(&rect);
}

void CFlashEnterDlg::UpdateGameConnInfo()
{
	CRect rect;
	rect.left = 492;
	rect.top = 269;
	rect.right = rect.left + 30;
	rect.bottom = rect.top + 20;
	InvalidateRect(&rect);
	rect.left = 635;
	rect.top = 269;
	rect.right = rect.left + 30;
	rect.bottom = rect.top + 20;
	InvalidateRect(&rect);
}

void CFlashEnterDlg::UpdateQueueInfo()
{
	CRect rect;
	rect.left = 265;
	rect.top = 330;
	rect.right = rect.left + 30;
	rect.bottom = rect.top + 20;
	InvalidateRect(&rect);
	rect.left = 510;
	rect.top = 330;
	rect.right = rect.left + 30;
	rect.bottom = rect.top + 20;
	InvalidateRect(&rect);
}

void CFlashEnterDlg::OnBnClickedBtLogonConnect()
{
	m_dwListCountItem = 0;
	ClearAllListData();
	g_GlobalUnits.m_bInitGameKindList = false;
	g_GlobalUnits.m_GameKindList.clear();
	//���ӵ�¼������
	bool bIsSuc = SetGameOptionInfo();
	if (!bIsSuc)
	{
		return;
	}
	WORD wRet = CGameUserInfo::GetInstance()->InitGameUserInfo();
	if (0 != wRet)
	{
		char chData[64] = {0};
		sprintf(chData, "��ʼ����Ϸ��%d���û���Ϣʧ�ܣ�", wRet);
		MessageBox(chData);
		return;
	}

	//if (NULL != m_XmlFile)
	{//��ȡ��Ϣ
		CString strValue="";
		m_UserID.GetWindowText(strValue);
		//m_XmlFile->setValue("UserID", strValue.GetBuffer());
		xmlParse.setValue("UserID", strValue.GetBuffer());

		strValue="";
		m_UserPassWord.GetWindowText(strValue);
		xmlParse.setValue("PassWord", strValue.GetBuffer());

		strValue="";
		m_ServerIP.GetWindowText(strValue);
		xmlParse.setValue("ServerIP", strValue.GetBuffer());

		strValue="";
		m_ServerPort.GetWindowText(strValue);
		xmlParse.setValue("ServerPort", strValue.GetBuffer());

		strValue="";
		m_GameID.GetWindowText(strValue);
		xmlParse.setValue("GameID", strValue.GetBuffer());

		strValue="";
		m_RoomID.GetWindowText(strValue);
		xmlParse.setValue("RoomID", strValue.GetBuffer());

		strValue="";
		m_UserIndex.GetWindowText(strValue);
		xmlParse.setValue("UserIndex", strValue.GetBuffer());

		strValue="";
		m_TotalUserNums.GetWindowText(strValue);
		xmlParse.setValue("UserNums", strValue.GetBuffer());

		strValue="";
		m_GameDLL.GetWindowText(strValue);
		xmlParse.setValue("GameDll", strValue.GetBuffer());

		strValue="";
		m_GameServerIP.GetWindowText(strValue);
		xmlParse.setValue("GameServerIP", strValue.GetBuffer());

		strValue="";
		m_GameServerPort.GetWindowText(strValue);
		xmlParse.setValue("GameServerPort", strValue.GetBuffer());

		BOOL iRet=xmlParse.WriteXMLFile("AndroidInI.xml");
	}

	if (NULL != m_pGameLogon)
	{
		m_ConnectLogonServer.EnableWindow(FALSE);
		m_ResetUserData.EnableWindow(TRUE);
		m_ConnectGameServer.EnableWindow(TRUE);
		m_ExitGameServer.EnableWindow(FALSE);
		m_AutoQueue.EnableWindow(FALSE);
		m_AllExitGame.EnableWindow(FALSE);
		m_pGameLogon->OnEventConnectServer();
	}
}

void CFlashEnterDlg::OnBnClickedBtLogonExitServer()
{
	//��������
	if (NULL != m_pGameLogon)
	{
		m_ConnectLogonServer.EnableWindow(TRUE);
		m_ResetUserData.EnableWindow(FALSE);
		m_ConnectGameServer.EnableWindow(FALSE);
		m_ExitGameServer.EnableWindow(FALSE);
		m_AutoQueue.EnableWindow(FALSE);
		g_GlobalUnits.m_bInitGameKindList = false;
		g_GlobalUnits.m_GameKindList.clear();

		bool bRet = m_pGameLogon->OnEventExitServer();
		if (!bRet)
		{
			AfxMessageBox(TEXT("�˳���¼��������Ϣʧ�ܣ�"));
		}
	}
	ClearAllListData();
	SetGameOptionInfo();
}

//������Ϸ������Ϣ
bool CFlashEnterDlg::SetGameOptionInfo()
{
	//������Ϸ����
	CString strInfo = "";
	m_UserID.GetWindowText(strInfo);
	if (!strInfo.IsEmpty())
	{
		::memcpy(g_GlobalUnits.m_GameOption.chUserID, strInfo.GetBuffer(), strInfo.GetLength());
	}
	else
	{
		m_UserID.SetFocus();
		AfxMessageBox("UserID����Ϊ�գ�������UserID��");
		return false;
	}
	strInfo = "";
	m_UserPassWord.GetWindowText(strInfo);
	::memset(g_GlobalUnits.m_GameOption.chUserPassWord, 0, sizeof(g_GlobalUnits.m_GameOption.chUserPassWord));
	if (!strInfo.IsEmpty())
	{
		::memcpy(g_GlobalUnits.m_GameOption.chUserPassWord, strInfo.GetBuffer(), strInfo.GetLength());
	}
	else
	{
		m_UserPassWord.SetFocus();
		AfxMessageBox("���벻��Ϊ�գ����������룡");
		return false;
	}
	strInfo = "";
	m_ServerIP.GetWindowText(strInfo);
	if (!strInfo.IsEmpty())
	{
		::memcpy(g_GlobalUnits.m_GameOption.chServerIP, strInfo.GetBuffer(), strInfo.GetLength());
	}
	else
	{
		m_ServerIP.SetFocus();
		AfxMessageBox("��¼IP��ַ����Ϊ�գ��������¼IP��ַ��");
		return false;
	}

	//��Ϸ������IP
	strInfo ="";
	m_GameServerIP.GetWindowText(strInfo);
	if (!strInfo.IsEmpty())
	{
		::memcpy(g_GlobalUnits.m_GameOption.chGameServerIP, strInfo.GetBuffer(), strInfo.GetLength());
	}
	else
	{
		m_GameServerIP.SetFocus();
		AfxMessageBox("��Ϸ������IP��ַ����Ϊ�գ���������Ϸ������IP��ַ��");
		return false;
	}

	//��Ϸ�������˿�
	m_GameServerPort.GetWindowText(strInfo);
	if (!strInfo.IsEmpty())
	{
		g_GlobalUnits.m_GameOption.dwGameServerPort = ::atoi(strInfo.GetBuffer());
	}
	else
	{
		m_ServerPort.SetFocus();
		AfxMessageBox("��Ϸ�������˿ںŲ���Ϊ�գ���������Ϸ�������˿ںţ�");
		return false;
	}
	
	strInfo = "";
	m_GameID.GetWindowText(strInfo);
	if (!strInfo.IsEmpty())
	{
		g_GlobalUnits.m_GameOption.dwGameID = ::atoi(strInfo.GetBuffer());
	}
	else
	{
		m_GameID.SetFocus();
		AfxMessageBox("��ϷID����Ϊ�գ���������ϷID��");
		return false;
	}
	
	strInfo = "";
	m_UserIndex.GetWindowText(strInfo);
	if (!strInfo.IsEmpty())
	{
		g_GlobalUnits.m_GameOption.dwUserIndex = ::atoi(strInfo.GetBuffer());
	}
	else
	{
		m_UserIndex.SetFocus();
		AfxMessageBox("�û���¼��ʼ��Ų���Ϊ�գ��������û���¼��ʼ��ţ�");
		return false;
	}
	
	strInfo = "";
	m_RoomID.GetWindowText(strInfo);
	if (!strInfo.IsEmpty())
	{
		g_GlobalUnits.m_GameOption.dwRoomID = ::atoi(strInfo.GetBuffer());
	}
	else
	{
		m_RoomID.SetFocus();
		AfxMessageBox("����ID����Ϊ�գ������뷿��ID��");
		return false;
	}
	
	strInfo = "";
	m_TotalUserNums.GetWindowText(strInfo);
	if (!strInfo.IsEmpty())
	{
		g_GlobalUnits.m_GameOption.dwUserNums = ::atoi(strInfo.GetBuffer());
	}
	else
	{
		m_TotalUserNums.SetFocus();
		AfxMessageBox("�ܻ�����������Ϊ�գ����������������");
		return false;
	}
	
	strInfo = "";
	m_ServerPort.GetWindowText(strInfo);
	if (!strInfo.IsEmpty())
	{
		g_GlobalUnits.m_GameOption.dwServerPort = ::atoi(strInfo.GetBuffer());
	}
	else
	{
		m_ServerPort.SetFocus();
		AfxMessageBox("�˿ںŲ���Ϊ�գ�������˿ںţ�");
		return false;
	}
	
	strInfo = "";
	m_GameDLL.GetWindowText(strInfo);
	bool bIsLoadSuc = false;
	if (!strInfo.IsEmpty())
	{
		bIsLoadSuc = CGameUserInfo::GetInstance()->LoadGameClientDll(strInfo.GetBuffer());
	}
	else
	{
		m_GameDLL.SetFocus();
		AfxMessageBox("��ϷDLL����Ϊ�գ�������Ҫ���ص���ϷDLL��");
		return false;
	}
	if (!bIsLoadSuc)
	{
		AfxMessageBox("��Ϸ�ͻ���DLL���ز��ɹ���");
		return false;
	}
	return true;
}

void CFlashEnterDlg::OnBnClickedBtGameConnect()
{
	//������Ϸ������
	ClearAllListData();
	if (NULL != m_pGameServer)
	{
		m_ConnectLogonServer.EnableWindow(FALSE);
		m_ResetUserData.EnableWindow(FALSE);
		m_ConnectGameServer.EnableWindow(FALSE);
		m_ExitGameServer.EnableWindow(TRUE);
		m_AutoQueue.EnableWindow(TRUE);
		m_AllExitGame.EnableWindow(FALSE);

		m_pGameServer->OnConnectGameServer();
	}
}

void CFlashEnterDlg::OnBnClickedBtExitGameServer()
{
	//�˳���Ϸ������
	if (NULL != m_pGameServer)
	{
		m_ConnectLogonServer.EnableWindow(FALSE);
		m_ResetUserData.EnableWindow(TRUE);
		m_ConnectGameServer.EnableWindow(TRUE);
		m_ExitGameServer.EnableWindow(FALSE);
		m_AutoQueue.EnableWindow(FALSE);
		m_AllExitGame.EnableWindow(FALSE);

		m_pGameServer->OnEventExitServer();
	}
}

//�����б�����
void CFlashEnterDlg::InsertListData(WORD dwSocketID, int dwErrorID, char* pErrorData)
{
	++m_dwListCountItem;
	if (m_dwListCountItem>=1000)
	{
		ClearAllListData();
	}
	
	char chData[256]={0};

	::memset(chData, 0, sizeof(chData));
	sprintf(chData, "%d", m_dwListCountItem);
	int nRow = m_ErrorList.InsertItem(0, chData);        //������

	::memset(chData, 0, sizeof(chData));
	sprintf(chData, "%d", dwSocketID);
	m_ErrorList.SetItemText(nRow,1,chData);  //��������

	::memset(chData, 0, sizeof(chData));
	sprintf(chData, "%d", dwErrorID);
	m_ErrorList.SetItemText(nRow,2,chData);  //��������

	::memset(chData, 0, sizeof(chData));
	m_ErrorList.SetItemText(nRow,3,pErrorData);  //��������

	//ʱ��
	CTime time = CTime::GetCurrentTime(); ///����CTime����
	int m_nYear = time.GetYear(); ///��
	int m_nMonth = time.GetMonth(); ///��
	int m_nDay = time.GetDay(); ///��
	int m_nHour = time.GetHour(); ///Сʱ
	int m_nMinute = time.GetMinute(); ///����
	int m_nSecond = time.GetSecond(); ///��
	::memset(chData, 0, sizeof(chData));
	sprintf(chData, "%d/%d/%d-%d/%d/%d", m_nYear, m_nMonth, m_nDay, m_nHour, m_nMinute, m_nSecond);
	m_ErrorList.SetItemText(nRow, 4, chData);

	m_ErrorList.EnsureVisible(nRow, FALSE);

	m_ErrorList.SetRedraw(TRUE);
	m_ErrorList.Invalidate();
	m_ErrorList.UpdateWindow();
}

//�����������
void CFlashEnterDlg::ClearAllListData()
{
	m_dwListCountItem = 0;
	m_ErrorList.DeleteAllItems();
	m_ErrorList.SetRedraw(TRUE);
	m_ErrorList.Invalidate();
	m_ErrorList.UpdateWindow();
}

//�˳�����
void CFlashEnterDlg::OnCloseTool()
{
	OnCancel();
}

void CFlashEnterDlg::OnBnClickedBtAutoUserQueue()
{//�Զ��Ŷ�
	if (NULL != m_pGameServer)
	{
		m_pGameServer->SetAllUserQueue();

		m_AllExitGame.EnableWindow(TRUE);
	}
}

void CFlashEnterDlg::OnBnClickedBtAllUserExitGame()
{
	//��������˳���Ϸ
	CGameUserInfo::GetInstance()->AllUserExitGame();
}
