#include "StdAfx.h"
#include "Mmsystem.h"
#include "GlobalUnits.h"


//////////////////////////////////////////////////////////////////////////

//״̬��Ϣ
struct tagAstatInfo
{
	ADAPTER_STATUS				AdapterStatus;						//����״̬
	NAME_BUFFER					NameBuff[16];						//���ֻ���
};

//ȫ�ֱ���
CGlobalUnits					g_GlobalUnits;						//��Ϣ���			

//////////////////////////////////////////////////////////////////////////

//���캯��
CGlobalUnits::CGlobalUnits(void)
{
	m_szDirWork[0]=0;
	m_dwPlazaVersion=VER_PLAZA_FRAME;
	m_bInitGameKindList = false;
	m_GameKindList.clear();
	return;
}

//��������
CGlobalUnits::~CGlobalUnits(void)
{
	m_GameKindList.clear();
}

//��ʼ������
bool CGlobalUnits::InitGlobalUnits()
{
	//�����������
	srand((UINT)time(NULL));

	//���ù���Ŀ¼
	GetModuleFileName(AfxGetInstanceHandle(),m_szDirWork,sizeof(m_szDirWork));
	int nModuleLen=lstrlen(m_szDirWork);
	int nProcessLen=lstrlen(AfxGetApp()->m_pszExeName)+lstrlen(TEXT(".EXE"));
	m_szDirWork[nModuleLen-nProcessLen]=0;
	SetCurrentDirectory(m_szDirWork);

	return true;
}

//��ȡ����
void CGlobalUnits::GetClientSerial(tagClientSerial & ClientSerial)
{
	//��ȡ�汾
	OSVERSIONINFO OSVerInfo;
	OSVerInfo.dwOSVersionInfoSize=sizeof(OSVerInfo);
	GetVersionEx(&OSVerInfo);
	ClientSerial.dwSystemVer=MAKELONG(OSVerInfo.dwMajorVersion,OSVerInfo.dwMinorVersion);

	//������ʶ
	ZeroMemory(ClientSerial.dwComputerID,sizeof(ClientSerial.dwComputerID));
	WORD wMacSize=GetMACAddress((BYTE *)ClientSerial.dwComputerID,sizeof(ClientSerial.dwComputerID));

	//Ӳ�̱�ʶ
	WORD wIndex=(wMacSize+sizeof(DWORD)-1)/sizeof(DWORD);
	LPCTSTR pszHardDisk[]={TEXT("C:\\"),TEXT("D:\\"),TEXT("E:\\")};
	for (WORD i=wIndex;i<CountArray(ClientSerial.dwComputerID);i++)
	{
		ASSERT(CountArray(pszHardDisk)>(i-wIndex));
		GetVolumeInformation(pszHardDisk[i-wIndex],NULL,0,&ClientSerial.dwComputerID[i],NULL,NULL,0,NULL);
	}

	return;
}
//������ַ
WORD CGlobalUnits::GetMACAddress(BYTE cbMacBuffer[], WORD wBufferSize)
{
	//��������
	WORD wMacSize=0;
	HINSTANCE hInstance=NULL;
	typedef BYTE __stdcall NetBiosProc(NCB * Ncb);

	try
	{
		//���� DLL
		hInstance=LoadLibrary(TEXT("NetApi32.dll"));
		if (hInstance==NULL) throw TEXT("���� NetApi32.dll ʧ��");

		//��ȡ����
		NetBiosProc * pNetBiosProc=(NetBiosProc *)GetProcAddress(hInstance,"Netbios");
		if (pNetBiosProc==NULL) throw TEXT("�޷��ҵ� NetBios ����");

		//��������
		NCB Ncb;
		LANA_ENUM LanaEnum;
		ZeroMemory(&Ncb,sizeof(Ncb));
		ZeroMemory(&LanaEnum,sizeof(LanaEnum));

		//ö������
		Ncb.ncb_command=NCBENUM;
		Ncb.ncb_length=sizeof(LanaEnum);
		Ncb.ncb_buffer=(BYTE *)&LanaEnum;
		if (pNetBiosProc(&Ncb)!=NRC_GOODRET) throw TEXT("ö������ʧ��");
		if (LanaEnum.length==0) throw TEXT("�Ҳ����κ�����");

		//��ȡ��ַ
		tagAstatInfo Adapter;
		for (BYTE i=0;i<LanaEnum.length;i++)
		{
			//��������
			ZeroMemory(&Ncb,sizeof(Ncb));
			Ncb.ncb_command=NCBRESET;
			Ncb.ncb_lana_num=LanaEnum.lana[i];
			if (pNetBiosProc(&Ncb)!=NRC_GOODRET) throw TEXT("��������ʧ��");

			//��ȡ״̬
			ZeroMemory(&Ncb,sizeof(Ncb));
			Ncb.ncb_command=NCBASTAT;
			Ncb.ncb_length=sizeof(Adapter);
			Ncb.ncb_buffer=(BYTE *)&Adapter;
			Ncb.ncb_lana_num=LanaEnum.lana[i];
			strcpy((char *)Ncb.ncb_callname,"*");
			if (pNetBiosProc(&Ncb)!=NRC_GOODRET) throw TEXT("��ȡ����״̬ʧ��");

			//��ȡ��ַ
			if (wMacSize<wBufferSize)
			{
				//������ַ
				WORD wCopySize=__min((wBufferSize-wMacSize),sizeof(BYTE)*6);
				CopyMemory(cbMacBuffer,Adapter.AdapterStatus.adapter_address,wCopySize);

				//����ж�
				wMacSize+=wCopySize;
				if (wMacSize==wBufferSize) break;
			}
		}

		//�ͷ���Դ
		if (hInstance!=NULL)
		{
			FreeLibrary(hInstance);
			hInstance=NULL;
		}

		return wMacSize;
	}
	catch (...) 
	{ 
		//�ͷ���Դ
		if (hInstance!=NULL)
		{
			FreeLibrary(hInstance);
			hInstance=NULL;
		}
	}

	return wMacSize;
}


//��õ�ǰ�ɽ���ķ�����Ϣ
bool CGlobalUnits::GetCurCanEnterRoomInfo(DWORD& dwServerAddr, DWORD& dwServerPort)
{
	dwServerAddr = 0;
	dwServerPort = 0;
	
	WORD dwRoomNums = m_GameKindList.size(); 
	for (int i=0; i<dwRoomNums; i++)
	{
		if (m_GameKindList[i].dwOnLineCount < m_GameKindList[i].dwMaxCount*DEF_NUM_VALUE)
		{
			dwServerAddr = m_GameKindList[i].dwGameServerIP;
			dwServerPort = m_GameKindList[i].dwGameServerport;
			return true;
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
