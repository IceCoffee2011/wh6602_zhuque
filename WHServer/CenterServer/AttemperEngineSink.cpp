#include "StdAfx.h"
#include "DataBaseSink.h"
#include "AttemperEngineSink.h"

//////////////////////////////////////////////////////////////////////////

CAttemperEngineSink::CAttemperEngineSink()
{
	//���ñ���
	m_pInitParamter=NULL;
	m_pBindParameter=NULL;

	//�ӿڱ���
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;

	return;
}

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
	//�󶨲���
	m_pBindParameter=new tagBindParameter[m_pInitParamter->m_wMaxConnect];
	ZeroMemory(m_pBindParameter,sizeof(tagBindParameter)*m_pInitParamter->m_wMaxConnect);

	//�������
	if ((m_ServerListCenter.GetInterface()==NULL)&&(m_ServerListCenter.CreateInstance()==false))
	{
		CTraceService::TraceString(TEXT("���ķ������б��������ʧ��"),TraceLevel_Exception);
		return false;
	}

	//�����б�
	tagDataBaseInfo DataBaseInfo;
	DataBaseInfo.wDataBasePort=m_pInitParamter->m_wServerDataBasePort;
	DataBaseInfo.dwDataBaseAddr=inet_addr(m_pInitParamter->m_szServerDataBaseAddr);
	lstrcpyn(DataBaseInfo.szDataBaseUser,m_pInitParamter->m_szServerDataBaseUser,CountArray(DataBaseInfo.szDataBaseUser));
	lstrcpyn(DataBaseInfo.szDataBasePass,m_pInitParamter->m_szServerDataBasePass,CountArray(DataBaseInfo.szDataBasePass));
	lstrcpyn(DataBaseInfo.szDataBaseName,m_pInitParamter->m_szServerDataBaseName,CountArray(DataBaseInfo.szDataBaseName));
	if (m_ServerListCenter->LoadServerList(DataBaseInfo)==false) 
	{
		CTraceService::TraceString(TEXT("��Ϸ�б����ʧ��"),TraceLevel_Exception);
		return false;
	}

	return true;
}

//����ģ��ر�
bool __cdecl CAttemperEngineSink::OnAttemperEngineStop(IUnknownEx * pIUnknownEx)
{
	//���ñ���
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;

	//ɾ������
	SafeDeleteArray(m_pBindParameter);

	//�����б�
	if (m_ServerListCenter.GetInterface()!=NULL)
	{
		m_ServerListCenter->ResetServerList();
	}

	return true;
}

//Ӧ���¼�
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

//�ر��¼�
bool __cdecl CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientIP, DWORD dwActiveTime, DWORD dwSocketID)
{
	//�����Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	ZeroMemory((m_pBindParameter+wBindIndex),sizeof(tagBindParameter));

	return true;
}

//��ȡ�¼�
bool __cdecl CAttemperEngineSink::OnEventTCPNetworkRead(CMD_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (Command.wMainCmdID)
	{
	case MDM_CS_SERVER_LIST:		//�б���Ϣ
		{
			return OnSocketMainServerList(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_CS_SERVER_MANAGER:		//�������
		{
			return OnSocketMainServerManager(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_CS_CONTROL_COMMAND:	//�������ֱ��͸����������Ϸ����
		{
			return m_pITCPNetworkEngine->SendDataBatch(MDM_CS_CONTROL_COMMAND,Command.wSubCmdID,pData,wDataSize);
		}
	}

	return true;
}

//�б���Ϣ����
bool CAttemperEngineSink::OnSocketMainServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_CS_GET_SERVER_LIST:	//��ȡ�б�
		{
			//��������
			CMD_CS_ListInfo ListInfo;
			ZeroMemory(&ListInfo,sizeof(ListInfo));

			//���ñ���
			ListInfo.dwTypeCount=m_ServerListCenter->GetGameTypeCount();
			ListInfo.dwKindCount=m_ServerListCenter->GetGameKindCount();
			ListInfo.dwServerCount=m_ServerListCenter->GetGameServerCount();
			ListInfo.dwStationCount=m_ServerListCenter->GetGameStationCount();
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVER_LIST,SUB_CS_LIST_INFO,&ListInfo,sizeof(ListInfo));

			//������Ϣ
			POSITION Pos=NULL;
			WORD wSendSize=0L;
			BYTE cbBuffer[SOCKET_PACKET];
			for (DWORD i=0;i<ListInfo.dwTypeCount;i++)
			{
				if ((wSendSize+sizeof(tagGameType))>sizeof(cbBuffer))
				{
					m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVER_LIST,SUB_CS_LIST_TYPE,cbBuffer,wSendSize);
					wSendSize=0;
				}
				tagGameTypeItem * pGameTypeItem=m_ServerListCenter->EmunGameTypeItem(Pos);
				CopyMemory(cbBuffer+wSendSize,&pGameTypeItem->GameType,sizeof(tagGameType));
				wSendSize+=sizeof(tagGameType);
			}
			if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVER_LIST,SUB_CS_LIST_TYPE,cbBuffer,wSendSize);

			//������Ϣ
			Pos=NULL;
			wSendSize=0L;
			for (DWORD i=0;i<ListInfo.dwKindCount;i++)
			{
				if ((wSendSize+sizeof(tagGameKind))>sizeof(cbBuffer))
				{
					m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVER_LIST,SUB_CS_LIST_KIND,cbBuffer,wSendSize);
					wSendSize=0;
				}
				tagGameKindItem * pGameKindItem=m_ServerListCenter->EmunGameKindItem(Pos);
				CopyMemory(cbBuffer+wSendSize,&pGameKindItem->GameKind,sizeof(tagGameKind));
				wSendSize+=sizeof(tagGameKind);
			}
			if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVER_LIST,SUB_CS_LIST_KIND,cbBuffer,wSendSize);

			//վ����Ϣ
			Pos=NULL;
			wSendSize=0L;
			for (DWORD i=0;i<ListInfo.dwStationCount;i++)
			{
				if ((wSendSize+sizeof(tagGameStation))>sizeof(cbBuffer))
				{
					m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVER_LIST,SUB_CS_LIST_STATION,cbBuffer,wSendSize);
					wSendSize=0;
				}
				tagGameStationItem * pGameStationItem=m_ServerListCenter->EmunGameStationItem(Pos);
				CopyMemory(cbBuffer+wSendSize,&pGameStationItem->GameStation,sizeof(tagGameStation));
				wSendSize+=sizeof(tagGameStation);
			}
			if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVER_LIST,SUB_CS_LIST_STATION,cbBuffer,wSendSize);

			//������Ϣ
			Pos=NULL;
			wSendSize=0L;
			for (DWORD i=0;i<ListInfo.dwServerCount;i++)
			{
				if ((wSendSize+sizeof(tagGameServer))>sizeof(cbBuffer))
				{
					m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVER_LIST,SUB_CS_LIST_SERVER,cbBuffer,wSendSize);
					wSendSize=0;
				}
				tagGameServerItem * pGameServerItem=m_ServerListCenter->EmunGameServerItem(Pos);
				CopyMemory(cbBuffer+wSendSize,&pGameServerItem->GameServer,sizeof(tagGameServer));
				wSendSize+=sizeof(tagGameServer);
			}
			if (wSendSize>0) m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVER_LIST,SUB_CS_LIST_SERVER,cbBuffer,wSendSize);

			//�������
			m_pITCPNetworkEngine->SendData(dwSocketID,MDM_CS_SERVER_LIST,SUB_CS_LIST_FINISH);

			return true;	
		}
	}

	return false;
}

//������Ϣ����
bool CAttemperEngineSink::OnSocketMainServerManager(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
		case SUB_CS_REG_GAME_SERVER:		//����ע��
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_RegGameServer));
			if (wDataSize!=sizeof(CMD_CS_RegGameServer)) return false;

			//��Ϣ����
			CMD_CS_RegGameServer * pRegGameServer=(CMD_CS_RegGameServer *)pData;
			tagGameServer * pGameServer=&pRegGameServer->GameServer;
			tagGameServerItem * pGameServerItem=m_ServerListCenter->SearchGameServer(pGameServer->wKindID,pGameServer->wServerID);
			if (pGameServerItem!=NULL)
			{
				pGameServerItem->dwUpdateTime=(DWORD)time(NULL);
				CopyMemory(&pGameServerItem->GameServer,pGameServer,sizeof(tagGameServer));
			}
			else m_ServerListCenter->InsertGameServer(pGameServer);

			//����ӳ���ϵ
			m_mapServerID2SocketID[pGameServer->wServerID] = dwSocketID;

			return true;
		}
		case SUB_CS_UNREG_GAME_SERVER:		//ע������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_UnRegGameServer));
			if (wDataSize!=sizeof(CMD_CS_UnRegGameServer)) return false;

			//��Ϣ����
			CMD_CS_UnRegGameServer * pUnRegGameServer=(CMD_CS_UnRegGameServer *)pData;
			m_ServerListCenter->DeleteGameServer(pUnRegGameServer->wKindID,pUnRegGameServer->wServerID);

			//ɾ��ӳ���ϵ
			m_mapServerID2SocketID.erase(pUnRegGameServer->wServerID);

			return true;
		}
		case SUB_CS_SERVER_ONLINE_COUNT:	//��������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_ServerOnLineCount));
			if (wDataSize!=sizeof(CMD_CS_ServerOnLineCount)) return false;

			//��Ϣ����
			CMD_CS_ServerOnLineCount * pServerOnLineCount=(CMD_CS_ServerOnLineCount *)pData;
			WORD wKindID=pServerOnLineCount->wKindID;
			WORD wServerID=pServerOnLineCount->wServerID;
			m_ServerListCenter->UpdateServerOnLineCount(wKindID,wServerID,pServerOnLineCount->dwOnLineCount);

			return true;
		}
		case SUB_CS_CREATE_SELF_REQ:	//���󴴽��Խ���
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_CreateSelf));
			if (wDataSize!=sizeof(CMD_CS_CreateSelf)) return false;

			//��Ϣ����
			CMD_CS_CreateSelf * pReq=(CMD_CS_CreateSelf *)pData;

			//����nServerID�ҵ���Ӧ�ķ�������ת����Ϣ����
			if (pReq && m_mapServerID2SocketID.find(pReq->nServerID) != m_mapServerID2SocketID.end())
			{
				//ת������Ӧ����Ϸ����
				DWORD dwContextID = m_mapServerID2SocketID[pReq->nServerID];

				//�ж�����
				if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

				//ת����ָ������Ϸ������
				pReq->dwSocketIDLogin = dwSocketID;  //��ʶ���ĸ�LoginServer����
				m_pITCPNetworkEngine->SendData(dwContextID,MDM_CS_SERVER_MANAGER,SUB_CS_CREATE_SELF_REQ,pData,wDataSize);
			}

			return true;
		}
		case SUB_CS_QUERY_SELF_REQ:	//�����ѯ�Խ���
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_QuerySelf));
			if (wDataSize!=sizeof(CMD_CS_QuerySelf)) return false;

			//��Ϣ����
			CMD_CS_QuerySelf * pReq=(CMD_CS_QuerySelf *)pData;

			//����nServerID�ҵ���Ӧ�ķ�������ת����Ϣ����
			if (pReq && m_mapServerID2SocketID.find(pReq->nServerID) != m_mapServerID2SocketID.end())
			{
				//ת������Ӧ����Ϸ����
				DWORD dwContextID = m_mapServerID2SocketID[pReq->nServerID];

				//�ж�����
				if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

				//ת����ָ������Ϸ������
				pReq->dwSocketIDLogin = dwSocketID;  //��ʶ���ĸ�LoginServer����
				m_pITCPNetworkEngine->SendData(dwContextID,MDM_CS_SERVER_MANAGER,SUB_CS_QUERY_SELF_REQ,pData,wDataSize);
			}

			return true;
		}
		case SUB_CS_CHECK_PWD_SELF_REQ:	//�����Խ���У������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_CheckPwdSelf));
			if (wDataSize!=sizeof(CMD_CS_CheckPwdSelf)) return false;

			//��Ϣ����
			CMD_CS_CheckPwdSelf * pReq=(CMD_CS_CheckPwdSelf *)pData;

			//����nServerID�ҵ���Ӧ�ķ�������ת����Ϣ����
			if (pReq && m_mapServerID2SocketID.find(pReq->nServerID) != m_mapServerID2SocketID.end())
			{
				//ת������Ӧ����Ϸ����
				DWORD dwContextID = m_mapServerID2SocketID[pReq->nServerID];

				//�ж�����
				if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

				//ת����ָ������Ϸ������
				pReq->dwSocketIDLogin = dwSocketID;  //��ʶ���ĸ�LoginServer����
				m_pITCPNetworkEngine->SendData(dwContextID,MDM_CS_SERVER_MANAGER,SUB_CS_CHECK_PWD_SELF_REQ,pData,wDataSize);
			}

			return true;
		}
		case SUB_CS_CREATE_SELF_REP:	//�����Խ������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_CreateSelfRep));
			if (wDataSize!=sizeof(CMD_CS_CreateSelfRep)) return false;

			//��Ϣ����
			CMD_CS_CreateSelfRep * pReq=(CMD_CS_CreateSelfRep *)pData;

			//����socketid�ҵ���Ӧ�ķ�������ת����Ϣ����
			if (pReq)
			{
				//ת������Ӧ����Ϸ����
				DWORD dwContextID = pReq->dwSocketIDLogin;

				//�ж�����
				if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

				//ת����ָ���ĵ�¼������
				pReq->dwSocketIDLogin = dwSocketID;  //��ʶ���ĸ�LoginServer����
				m_pITCPNetworkEngine->SendData(dwContextID,MDM_CS_SERVER_MANAGER,SUB_CS_CREATE_SELF_REP,pData,wDataSize);
			}

			return true;
		}
		case SUB_CS_CHECK_PWD_SELF_REP:	//У��������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_CheckPwdSelfRep));
			if (wDataSize!=sizeof(CMD_CS_CheckPwdSelfRep)) return false;

			//��Ϣ����
			CMD_CS_CheckPwdSelfRep * pReq=(CMD_CS_CheckPwdSelfRep *)pData;

			//����socketid�ҵ���Ӧ�ķ�������ת����Ϣ����
			if (pReq)
			{
				//ת������Ӧ����Ϸ����
				DWORD dwContextID = pReq->dwSocketIDLogin;

				//�ж�����
				if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

				//ת����ָ���ĵ�¼������
				pReq->dwSocketIDLogin = dwSocketID;  //��ʶ���ĸ�LoginServer����
				m_pITCPNetworkEngine->SendData(dwContextID,MDM_CS_SERVER_MANAGER,SUB_CS_CHECK_PWD_SELF_REP,pData,wDataSize);
			}

			return true;
		}
		case SUB_CS_QUERY_SELF_REP:	//��ѯ�Խ������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_QuerySelfRep));
			if (wDataSize!=sizeof(CMD_CS_QuerySelfRep)) return false;

			//��Ϣ����
			CMD_CS_QuerySelfRep * pReq=(CMD_CS_QuerySelfRep *)pData;

			//����socketid�ҵ���Ӧ�ķ�������ת����Ϣ����
			if (pReq)
			{
				//ת������Ӧ����Ϸ����
				DWORD dwContextID = pReq->dwSocketIDLogin;

				//�ж�����
				if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

				//ת����ָ���ĵ�¼������
				pReq->dwSocketIDLogin = dwSocketID;  //��ʶ���ĸ�LoginServer����
				m_pITCPNetworkEngine->SendData(dwContextID,MDM_CS_SERVER_MANAGER,SUB_CS_QUERY_SELF_REP,pData,wDataSize);
			}

			return true;
		}
		case SUB_CS_SPEAKER_SEND_MSG_REQ:	//����������
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_SpeakerSendMsgReq));
			if (wDataSize!=sizeof(CMD_CS_SpeakerSendMsgReq)) return false;

			//����Ϣת�����������ӵ������ķ����������з���
			for (int i=0; i<m_pInitParamter->m_wMaxConnect; i++)
			{
				if (m_pBindParameter[i].dwSocketID != 0)  
				{
					//��������Ч��
					m_pITCPNetworkEngine->SendData(m_pBindParameter[i].dwSocketID,MDM_CS_SERVER_MANAGER,SUB_CS_SPEAKER_SEND_MSG_REQ,pData,wDataSize);
					
					/* TODO:�����в��ַ��䲻������Ϣ
					//��������Ч�ġ����ж��Ƿ�Ϊ����
					for (std::map<WORD, DWORD>::const_iterator cit = m_mapServerID2SocketID.begin();
						 cit != m_mapServerID2SocketID.end();
						 ++cit)
					{
						if (
						//�˷����Ƿ���������Ϣ
					}
					*/
				}
			}
				
			return true;
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
