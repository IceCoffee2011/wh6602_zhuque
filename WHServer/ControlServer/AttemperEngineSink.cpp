#include "StdAfx.h"
#include "AttemperEngineSink.h"
#include "Zip.h"
#include "ControlServer.h"


#include "biostream.h"

//��������
extern CControlServerApp theApp;
//////////////////////////////////////////////////////////////////////////

//�궨��
#define TIME_RECONNECT						15000L						//����ʱ��
#define TIME_UPDATE_LIST					30000L						//����ʱ��
#define TIME_DOWNLOAD_CUSTOM_FACE			200L						//����ͷ��
#define TIME_CHECK_GM_TASK					120000L						//���GM����

//��ʱ�� ID
#define IDI_CONNECT_CENTER_SERVER			1							//���ӷ�����
#define IDI_UPDATE_SERVER_LIST				2							//�����б�
#define IDI_DOWNLOAD_CUSTOM_FACE			3							//����ͷ��
#define IDI_CHECK_GM_TASK					4							//���GM����

//////////////////////////////////////////////////////////////////////////

#define SEND_USER_DATA(msg)		do{char cbBuffer[SOCKET_PACKET];\
	bostream bos;\
	bos.attach(cbBuffer, SOCKET_PACKET);\
	bos<<msg;\
	if(m_pITCPNetworkEngine)m_pITCPNetworkEngine->SendData(dwContextID, msg.M_ID, msg.S_ID, cbBuffer, bos.length());}while(0)


//�����Ϣ
void TraceMessage(LPCTSTR pszMessage)
{
	if(0 == lstrlen(pszMessage))
	{
		return ;
	}
	
	SYSTEMTIME sys; 
	GetLocalTime( &sys ); 
	char buf[128];
	memset(buf, 0, 128);
	sprintf(buf, "%4d/%02d/%02d %02d:%02d:%02d.%03d ",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds);

	//���������־
	char filename[128];
	memset(filename, 0, 128);
	sprintf(filename, "TraceData_Control_%4d%02d%02d.txt",sys.wYear,sys.wMonth,sys.wDay);

	CFile File;
	if ((File.Open(filename,CFile::modeWrite)==FALSE)&&
		(File.Open(filename,CFile::modeWrite|CFile::modeCreate)==FALSE))
	{
		ASSERT(FALSE);
		return;
	}

	File.SeekToEnd();
	File.Write(buf,lstrlen(buf));
	File.Write(pszMessage,lstrlen(pszMessage));
	File.Write(TEXT("\r\n"),lstrlen(TEXT("\r\n")));

	File.Flush();
	File.Close();

	return;
}

//���캯��
CAttemperEngineSink::CAttemperEngineSink()
{
	//���ñ���
	m_pInitParamter=NULL;
	m_pBindParameter=NULL;

	//���ñ���
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
//	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketCorrespond=NULL;

	return;
}

//��������
CAttemperEngineSink::~CAttemperEngineSink()
{
}

//�ӿڲ�ѯ
void * __cdecl CAttemperEngineSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAttemperEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink,Guid,dwQueryVer);
	return NULL;
}

//����ģ������
bool __cdecl CAttemperEngineSink::OnAttemperEngineStart(IUnknownEx * pIUnknownEx)
{
	//�������
//	m_ServerList.SetSocketEngine(m_pITCPNetworkEngine);

	//�󶨲���
	m_pBindParameter=new tagBindParameter[m_pInitParamter->m_wMaxConnect];
	ZeroMemory(m_pBindParameter,sizeof(tagBindParameter)*m_pInitParamter->m_wMaxConnect);

	//��������
	DWORD dwServerIP=inet_addr(m_pInitParamter->m_szCenterServerAddr);
	m_pITCPSocketCorrespond->Connect(htonl(dwServerIP),m_pInitParamter->m_wCenterServerPort);

	//��ʼ����
	//m_DownloadFaceInfoMap.InitHashTable(503);
	//m_DownloadFaceInfoMap.RemoveAll();

	//���ö�ʱ��
	//m_pITimerEngine->SetTimer(IDI_DOWNLOAD_CUSTOM_FACE,TIME_DOWNLOAD_CUSTOM_FACE,TIMES_INFINITY,NULL);

	return true;
}

//����ģ��ر�
bool __cdecl CAttemperEngineSink::OnAttemperEngineStop(IUnknownEx * pIUnknownEx)
{
	//���ñ���
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
//	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketCorrespond=NULL;

	//ɾ������
	SafeDeleteArray(m_pBindParameter);

	return true;
}

//��ʱ���¼�
bool __cdecl CAttemperEngineSink::OnEventTimer(DWORD dwTimerID, WPARAM wBindParam)
{
	switch (dwTimerID)
	{
	case IDI_CONNECT_CENTER_SERVER:		//�������ķ�����
		{
			//��������
			DWORD dwServerIP=inet_addr(m_pInitParamter->m_szCenterServerAddr);
			m_pITCPSocketCorrespond->Connect(htonl(dwServerIP),m_pInitParamter->m_wCenterServerPort);

			//������ʾ
			CTraceService::TraceString(TEXT("���ڳ��������������ķ�����...."),TraceLevel_Normal);

			return true;
		}
	case IDI_UPDATE_SERVER_LIST:		//���·������б�
		{
			m_pITCPSocketCorrespond->SendData(MDM_CS_SERVER_LIST,SUB_CS_GET_SERVER_LIST,NULL,0);

			return true;
		}
	case IDI_CHECK_GM_TASK:		//���GM����
		{
			//Ͷ�����ݿ�
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CHECK_GM_TASK,0,NULL,0);

			//���а�
			SYSTEMTIME sys; 
			GetLocalTime( &sys ); 

			static SYSTEMTIME lastTime;  //��һ�����ɵ�ʱ��
			
			//ÿ��3��
			if (sys.wHour == 3 
				&& lastTime.wDay != sys.wDay )
			{
				//Ͷ�����ݿ�
				DBR_GR_Online online;
				online.dwTypeID = 16;   //�����������
				online.dwValue = m_iMaxOnLineToday;
				lstrcpy(online.szCollectDate, m_strCollectDateToday.c_str());
				m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_WRITE_ONLINE,0,&online,sizeof(online));

				//���˹������������
				if (m_mapMaxToday.find(998) != m_mapMaxToday.end() && m_mapMaxToday[998].first > 0)
				{
					online.dwTypeID = 17;   //�����������
					online.dwValue = m_mapMaxToday[998].first;
					lstrcpy(online.szCollectDate, m_mapMaxToday[998].second.c_str());
					m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_WRITE_ONLINE,0,&online,sizeof(online));
				}
				//���˹������������
				if (m_mapMaxToday.find(997) != m_mapMaxToday.end() && m_mapMaxToday[997].first > 0)
				{
					online.dwTypeID = 18;   //�����������
					online.dwValue = m_mapMaxToday[997].first;
					lstrcpy(online.szCollectDate, m_mapMaxToday[997].second.c_str());
					m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_WRITE_ONLINE,0,&online,sizeof(online));
				}
				//������ʮK�����������
				if (m_mapMaxToday.find(995) != m_mapMaxToday.end() && m_mapMaxToday[995].first > 0)
				{
					online.dwTypeID = 22;   //�����������
					online.dwValue = m_mapMaxToday[995].first;
					lstrcpy(online.szCollectDate, m_mapMaxToday[995].second.c_str());
					m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_WRITE_ONLINE,0,&online,sizeof(online));
				}
				//��ʯ��ʮK�����������
				if (m_mapMaxToday.find(996) != m_mapMaxToday.end() && m_mapMaxToday[996].first > 0)
				{
					online.dwTypeID = 25;   //�����������
					online.dwValue = m_mapMaxToday[996].first;
					lstrcpy(online.szCollectDate, m_mapMaxToday[996].second.c_str());
					m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_WRITE_ONLINE,0,&online,sizeof(online));
				}
				//Ǳ��ǧ�ֽ����������
				if (m_mapMaxToday.find(990) != m_mapMaxToday.end() && m_mapMaxToday[990].first > 0)
				{
					online.dwTypeID = 28;   //�����������
					online.dwValue = m_mapMaxToday[990].first;
					lstrcpy(online.szCollectDate, m_mapMaxToday[990].second.c_str());
					m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_WRITE_ONLINE,0,&online,sizeof(online));
				}
				//��ڽ����������
				if (m_mapMaxToday.find(999) != m_mapMaxToday.end() && m_mapMaxToday[999].first > 0)
				{
					online.dwTypeID = 31;   //�����������
					online.dwValue = m_mapMaxToday[999].first;
					lstrcpy(online.szCollectDate, m_mapMaxToday[999].second.c_str());
					m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_WRITE_ONLINE,0,&online,sizeof(online));
				}
				//�������������������
				if (m_mapMaxToday.find(988) != m_mapMaxToday.end() && m_mapMaxToday[988].first > 0)
				{
					online.dwTypeID = 34;   //�����������
					online.dwValue = m_mapMaxToday[988].first;
					lstrcpy(online.szCollectDate, m_mapMaxToday[988].second.c_str());
					m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_WRITE_ONLINE,0,&online,sizeof(online));
				}
				//���������������
				if (m_mapMaxToday.find(987) != m_mapMaxToday.end() && m_mapMaxToday[987].first > 0)
				{
					online.dwTypeID = 37;   //�����������
					online.dwValue = m_mapMaxToday[987].first;
					lstrcpy(online.szCollectDate, m_mapMaxToday[987].second.c_str());
					m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_WRITE_ONLINE,0,&online,sizeof(online));
				}
				//ͨɽ�����������
				if (m_mapMaxToday.find(986) != m_mapMaxToday.end() && m_mapMaxToday[986].first > 0)
				{
					online.dwTypeID = 40;   //�����������
					online.dwValue = m_mapMaxToday[986].first;
					lstrcpy(online.szCollectDate, m_mapMaxToday[986].second.c_str());
					m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_WRITE_ONLINE,0,&online,sizeof(online));
				}
				//���ҽ����������
				if (m_mapMaxToday.find(989) != m_mapMaxToday.end() && m_mapMaxToday[989].first > 0)
				{
					online.dwTypeID = 43;   //�����������
					online.dwValue = m_mapMaxToday[989].first;
					lstrcpy(online.szCollectDate, m_mapMaxToday[989].second.c_str());
					m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_WRITE_ONLINE,0,&online,sizeof(online));
				}
				//ţţ�����������
				if (m_mapMaxToday.find(994) != m_mapMaxToday.end() && m_mapMaxToday[994].first > 0)
				{
					online.dwTypeID = 19;   //�����������
					online.dwValue = m_mapMaxToday[994].first;
					lstrcpy(online.szCollectDate, m_mapMaxToday[994].second.c_str());
					m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_WRITE_ONLINE,0,&online,sizeof(online));
				}
				//���Ž����������
				if (m_mapMaxToday.find(961) != m_mapMaxToday.end() && m_mapMaxToday[961].first > 0)
				{
					online.dwTypeID = 60;   //�����������
					online.dwValue = m_mapMaxToday[961].first;
					lstrcpy(online.szCollectDate, m_mapMaxToday[961].second.c_str());
					m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_WRITE_ONLINE,0,&online,sizeof(online));
				}

				//���������������
				m_mapMaxToday.clear();
				m_iMaxOnLineToday = 0;
				m_strCollectDateToday.assign("");

				//Ͷ�����ݿ�
				m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CREATE_RANKING,0,NULL,0);
				lastTime = sys;
			}

			return true;
		}
	}

	return false;
}

//���ݿ��¼�
bool __cdecl CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
		case DBR_GP_CHECK_SPEAKER_TASK_RESULT:			//ϵͳ����
		{
			return OnDBCheckSpeakerTaskResult(dwContextID,pData,wDataSize);
		}
	}

	return false;
}


//�����¼�
bool __cdecl CAttemperEngineSink::OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)
{
	//�����ж�
	if (nErrorCode!=0)
	{
		//����ʱ��
		m_pITimerEngine->SetTimer(IDI_CONNECT_CENTER_SERVER,TIME_RECONNECT,1,NULL);

		//������ʾ
		CTraceService::TraceString(TEXT("���ķ���������ʧ�ܣ��Ժ󽫻��ٴγ���...."),TraceLevel_Warning);

		return true;
	}
	else
	{
		//������ʾ
		CTraceService::TraceString(TEXT("���ķ��������ӳɹ�"),TraceLevel_Normal);
	}

	//��ȡ�б�
	m_pITCPSocketCorrespond->SendData(MDM_CS_SERVER_LIST,SUB_CS_GET_SERVER_LIST,NULL,0);

	//���ö�ʱ��
	m_pITimerEngine->SetTimer(IDI_UPDATE_SERVER_LIST,TIME_UPDATE_LIST,TIMES_INFINITY,NULL);
	m_pITimerEngine->SetTimer(IDI_CHECK_GM_TASK,TIME_CHECK_GM_TASK,TIMES_INFINITY,NULL);

	return true;
}

//�ر��¼�
bool __cdecl CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	//��������
	m_pITimerEngine->KillTimer(IDI_UPDATE_SERVER_LIST);
	m_pITimerEngine->SetTimer(IDI_CONNECT_CENTER_SERVER,TIME_RECONNECT,1,NULL);

	//������ʾ
	CTraceService::TraceString(TEXT("���ķ����������жϣ��Ժ󽫻��ٴγ���...."),TraceLevel_Warning);

	return true;
}

//��ȡ�¼�
bool __cdecl CAttemperEngineSink::OnEventTCPSocketRead(WORD wServiceID, CMD_Command Command, VOID * pData, WORD wDataSize)
{
	switch (Command.wMainCmdID)
	{
	case MDM_CS_SERVER_LIST:	//�б���Ϣ
		{
			return OnCenterMainServerList(Command.wSubCmdID,pData,wDataSize);
		}
	case MDM_CS_CONTROL_COMMAND:  //�������� 
		{
			return OnCenterMainControlCommand(Command,pData,wDataSize);
		}
	}

	return true;
}

//����Ӧ���¼�
bool __cdecl CAttemperEngineSink::OnEventTCPNetworkBind(DWORD dwClientIP, DWORD dwSocketID)
{
	//��ȡ����
	ASSERT(LOWORD(dwSocketID)<m_pInitParamter->m_wMaxConnect);
	if (LOWORD(dwSocketID)>=m_pInitParamter->m_wMaxConnect) return false;

	//��������
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=(m_pBindParameter+wBindIndex);

	//���ñ���
	pBindParameter->dwSocketID=dwSocketID;
	pBindParameter->dwClientIP=dwClientIP;
	pBindParameter->dwActiveTime=(DWORD)time(NULL);

	return true;
}



//�����ȡ�¼�
bool __cdecl CAttemperEngineSink::OnEventTCPNetworkRead(CMD_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (Command.wMainCmdID)
	{
	case MDM_GP_USER:				//�û���Ϣ
		{
			return OnSocketMainUser(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//����ر��¼�
bool __cdecl CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientIP, DWORD dwActiveTime, DWORD dwSocketID)
{
	//�����Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	ZeroMemory((m_pBindParameter+wBindIndex),sizeof(tagBindParameter));

	return true;
}

//�û���Ϣ����
bool CAttemperEngineSink::OnSocketMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	default:
		{
			ASSERT(FALSE);
			return false;
		}
	}

	return true;
}



//�б���Ϣ����
bool CAttemperEngineSink::OnCenterMainServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_LIST_INFO:		//�б���Ϣ
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_ListInfo));
			if (wDataSize!=sizeof(CMD_CS_ListInfo)) return false;

			//��Ϣ����
			m_ServerList.ResetServerListBuffer();

			return true;
		}
	case SUB_CS_LIST_TYPE:		//������Ϣ
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(tagGameType));
			ASSERT(wDataSize%sizeof(tagGameType)==0);

			//��Ϣ����
			DWORD dwCount=wDataSize/sizeof(tagGameType);
			tagGameType * pGameType=(tagGameType *)pData;
			m_ServerList.AppendGameTypeBuffer(pGameType,dwCount);

			return true;
		}
	case SUB_CS_LIST_KIND:		//������Ϣ
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(tagGameKind));
			ASSERT(wDataSize%sizeof(tagGameKind)==0);

			//��Ϣ����
			DWORD dwCount=wDataSize/sizeof(tagGameKind);
			tagGameKind * pGameKind=(tagGameKind *)pData;
			m_ServerList.AppendGameKindBuffer(pGameKind,dwCount);

			return true;
		}
	case SUB_CS_LIST_STATION:	//վ����Ϣ
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(tagGameStation));
			ASSERT(wDataSize%sizeof(tagGameStation)==0);

			//��Ϣ����
			DWORD dwCount=wDataSize/sizeof(tagGameStation);
			tagGameStation * pGameStation=(tagGameStation *)pData;
			m_ServerList.AppendGameStationBuffer(pGameStation,dwCount);

			return true;
		}
	case SUB_CS_LIST_SERVER:	//������Ϣ
		{
			//Ч�����
			ASSERT(wDataSize>=sizeof(tagGameServer));
			ASSERT(wDataSize%sizeof(tagGameServer)==0);

			//��Ϣ����
			DWORD dwCount=wDataSize/sizeof(tagGameServer);
			tagGameServer * pGameServer=(tagGameServer *)pData;
			m_ServerList.AppendGameServerBuffer(pGameServer,dwCount);

			return true;
		}
	case SUB_CS_LIST_FINISH:	//�������
		{
			//��Ϣ����
			m_ServerList.ActiveServerListBuffer();

			//д��־,��Ϊ�����и���
			static unsigned int iTotalMax = 0;
			unsigned int iTotal = 0;
			std::map<DWORD, DWORD> mapKindTotal;  //��kindid�������
			DWORD dwServerCount=(DWORD)m_ServerList.m_ServerListInfoActive.m_GameServerPtrArrayActive.GetCount();
			for (DWORD i=0;i<dwServerCount;i++)
			{
				tagGameServer * pGameServer=m_ServerList.m_ServerListInfoActive.m_GameServerPtrArrayActive[i];

				mapKindTotal[pGameServer->wKindID] += pGameServer->dwOnLineCount;  //���

				bool bNewMax = false;
				if (m_mapMax.find(pGameServer->wServerID) != m_mapMax.end()
					&& m_mapMax[pGameServer->wServerID] >= pGameServer->dwOnLineCount)
				{
					//nothing
				}
				else
				{
					bNewMax = true;
					m_mapMax[pGameServer->wServerID] = pGameServer->dwOnLineCount;
				}

				iTotal += pGameServer->dwOnLineCount;

				char buf[128];
				memset(buf, 0, 128);
				sprintf(buf, "�����=%u ����=%u ��߷�=%u", pGameServer->wServerID, pGameServer->dwOnLineCount, 
						m_mapMax[pGameServer->wServerID]);
				if (bNewMax)
				{
					CTraceService::TraceString(buf,TraceLevel_Normal);
				}
				TraceMessage(buf);
			}
			
			if (iTotal > iTotalMax)iTotalMax = iTotal;

			char buf2[128];
			memset(buf2, 0, 128);
			sprintf(buf2, "������=%u ��߷�=%u", iTotal, iTotalMax);
			TraceMessage(buf2);

			if (iTotalMax==iTotal && iTotalMax!=0)
			{
				CTraceService::TraceString(buf2,TraceLevel_Normal);
			}

			////ͳ�ƽ����������:�����ߣ�����Ϸ����
			//��ǰʱ��
			SYSTEMTIME sys; 
			GetLocalTime( &sys ); 
			char szNow[32];
			memset(szNow, 0, 32);
			sprintf(szNow, "%04d-%02d-%02d %02d:%02d:%02d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);

			if (iTotal > m_iMaxOnLineToday)
			{
				m_iMaxOnLineToday = iTotal;
				m_strCollectDateToday = szNow;
			}
			char buf[128];
			memset(buf, 0, 128);
			sprintf(buf, "�������������,����=%u ʱ��=%s", 
						m_iMaxOnLineToday,
						m_strCollectDateToday.c_str());
			TraceMessage(buf);

			//������Ϸ�Ƿ��г�����
			for (std::map<DWORD, DWORD>::const_iterator cit = mapKindTotal.begin();
				cit != mapKindTotal.end(); 
				++cit)
			{
				if (cit->second > m_mapMaxToday[cit->first].first)
				{
					m_mapMaxToday[cit->first].first = cit->second;
					m_mapMaxToday[cit->first].second = szNow;
				}
			}
			//д��־
			for (std::map<DWORD, std::pair<DWORD, std::string>>::const_iterator cit = m_mapMaxToday.begin();
				cit != m_mapMaxToday.end(); 
				++cit)
			{
				char buf[128];
				memset(buf, 0, 128);
				sprintf(buf, "�������Ϸ�������,kindid=%d ����=%u ʱ��=%s", 
							cit->first,
							m_mapMaxToday[cit->first].first, 
							m_mapMaxToday[cit->first].second.c_str());
				
				TraceMessage(buf);
			}

			return true;
		}
	}

	return true;
}

//�б���Ϣ����
bool CAttemperEngineSink::OnCenterMainControlCommand(CMD_Command Command, VOID * pData, WORD wDataSize)
{
	switch (Command.wSubCmdID)
	{
	case SUB_CS_CONTROL_COMMAND_REQ:		//��������
		{
			//ֱ�Ӻ��Ե�
			return true;
		}
	case SUB_CS_CONTROL_COMMAND_REP:		//��Ӧ����
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_ControlCommandRep));
			if (wDataSize!=sizeof(CMD_CS_ControlCommandRep)) return false;

			//��Ϣ����
			CMD_CS_ControlCommandRep* rep = (CMD_CS_ControlCommandRep*)pData;
			if (rep == NULL)
			{
				CTraceService::TraceString("�����ֻظ���Ϣ�岻��ȷ SUB_CS_CONTROL_COMMAND_REP", TraceLevel_Warning);
				return false;
			}
				
			if (rep->wCode == 0)
			{
				//�ɹ� 
				char buf[128];
				memset(buf, 0, 128);
				if (rep->wCommandID == 1)  //���� 
				{
					sprintf(buf, "�ɹ��������%u, �û�ID=%u �ɹ�������·����߳���� ", rep->dwServerID, rep->dwUserID);
				}
				else if (rep->wCommandID == 2)  //����ά�� 
				{
					sprintf(buf, "�ɹ��������%u ����ά���ڡ�",rep->dwServerID); 
				}
				CTraceService::TraceString(buf, TraceLevel_Normal);
			}
			else 
			{
				//ʧ��
				char buf[128];
				memset(buf, 0, 128);
				sprintf(buf, "�����ʧ�ܡ������%u �û�ID=%u ������%d",rep->dwServerID,rep->dwUserID,rep->wCommandID); 
				CTraceService::TraceString(buf, TraceLevel_Warning);
				memset(buf, 0, 128);
				sprintf(buf, "������%d ����%s",rep->wCode, rep->szResult); 
				CTraceService::TraceString(buf, TraceLevel_Warning);
			}

			return true;
		}
		return true;
	}

	return true;
}


//������
bool CAttemperEngineSink::OnDBCheckSpeakerTaskResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GP_Speaker_Task_Result));
	if (wDataSize!=sizeof(DBR_GP_Speaker_Task_Result)) return false;
	
	DBR_GP_Speaker_Task_Result * pDBResult=(DBR_GP_Speaker_Task_Result *)pData;

	//����Ϣת�����ķ������������ķ������㲥��Ϣ
	CMD_CS_SpeakerSendMsgReq csReq;
	memset(&csReq, 0, sizeof(csReq));
	csReq.nType = 2;   //ϵͳ����
	csReq.nLen = pDBResult->nLen;
	memcpy(csReq.szMsg, pDBResult->szMsg, pDBResult->nLen);

	//ת��
	if (m_pITCPSocketCorrespond)m_pITCPSocketCorrespond->SendData(MDM_CS_SERVER_MANAGER, SUB_CS_SPEAKER_SEND_MSG_REQ, &csReq, sizeof(csReq));

	//Ͷ�����ݿ���������д��־
	DBR_GP_SpeakerSend dbReq;
	ZeroMemory(&dbReq, sizeof(dbReq));
	dbReq.iType = 2;   //ϵͳ����
	dbReq.dwUserID = 0;   //ϵͳ�û�
	dbReq.nLen = csReq.nLen;
	memcpy(dbReq.szDate, csReq.szMsg, dbReq.nLen);
	m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_SPEAKER_SEND, dwContextID, &dbReq, sizeof(dbReq));

	return true;
}

//////////////////////////////////////////////////////////////////////////
