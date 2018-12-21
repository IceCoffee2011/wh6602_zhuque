#include "StdAfx.h"
#include "Afxinet.h"
#include "DataBaseSink.h"

//////////////////////////////////////////////////////////////////////////
extern void TraceMessageScore(LPCTSTR pszMessage);

//���캯��
CDataBaseSink::CDataBaseSink()
{
	//���ñ���
	m_pGameUserDBInfo=NULL;
	m_pGameScoreDBInfoVec=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;
	m_pIDataBaseEngineEvent=NULL;

	return;
}

//��������
CDataBaseSink::~CDataBaseSink()
{
}

//�ӿڲ�ѯ
void * __cdecl CDataBaseSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseEngineSink,Guid,dwQueryVer);
	return NULL;
}

//����ģ������
bool __cdecl CDataBaseSink::OnDataBaseEngineStart(IUnknownEx * pIUnknownEx)
{
	//��������
/*	if ((m_AccountsDBModule.GetInterface()==NULL)&&(m_AccountsDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}
*/
	//��������
	if ((m_GameScoreDBModule.GetInterface()==NULL)&&(m_GameScoreDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}
	for (int i=0; i<m_pGameScoreDBModuleVec.size(); i++)
	{
		if (m_pGameScoreDBModuleVec[i])delete m_pGameScoreDBModuleVec[i];
	}
	m_pGameScoreDBModuleVec.clear();
	m_GameScoreDBAideVec.clear();
	for (int i=0; i<m_pGameScoreDBInfoVec->size(); i++)
	{
		m_pGameScoreDBModuleVec.push_back(m_GameScoreDBModule.GetNewInterface());
		m_GameScoreDBAideVec.push_back(CDataBaseAide());
		
		//��������
		BYTE * pcbAddr=NULL;
		TCHAR szDataBaseAddr[16]=TEXT("");
		//������Ϸ���ݿ�
		pcbAddr=(BYTE *)&(*m_pGameScoreDBInfoVec)[i].dwDataBaseAddr;
		_snprintf(szDataBaseAddr,sizeof(szDataBaseAddr),TEXT("%d.%d.%d.%d"),pcbAddr[0],pcbAddr[1],pcbAddr[2],pcbAddr[3]);
		m_pGameScoreDBModuleVec[i]->SetConnectionInfo(szDataBaseAddr,
											  (*m_pGameScoreDBInfoVec)[i].wDataBasePort,
											  (*m_pGameScoreDBInfoVec)[i].szDataBaseName,
											  (*m_pGameScoreDBInfoVec)[i].szDataBaseUser,
											  (*m_pGameScoreDBInfoVec)[i].szDataBasePass);

		//��������
		m_pGameScoreDBModuleVec[i]->OpenConnection();
		m_GameScoreDBAideVec[i].SetDataBase(m_pGameScoreDBModuleVec[i]);
	}

	try
	{
		//��������
		BYTE * pcbAddr=NULL;
		TCHAR szDataBaseAddr[16]=TEXT("");

		//�����û����ݿ�
/*		pcbAddr=(BYTE *)&m_pGameUserDBInfo->dwDataBaseAddr;
		_snprintf(szDataBaseAddr,sizeof(szDataBaseAddr),TEXT("%d.%d.%d.%d"),pcbAddr[0],pcbAddr[1],pcbAddr[2],pcbAddr[3]);
		m_AccountsDBModule->SetConnectionInfo(szDataBaseAddr,m_pGameUserDBInfo->wDataBasePort,m_pGameUserDBInfo->szDataBaseName,
			m_pGameUserDBInfo->szDataBaseUser,m_pGameUserDBInfo->szDataBasePass);

		//��������
		m_AccountsDBModule->OpenConnection();
		m_AccountsDBAide.SetDataBase(m_AccountsDBModule.GetInterface());
*/
		/*
		//������Ϸ���ݿ�
		pcbAddr=(BYTE *)&m_pGameScoreDBInfo->dwDataBaseAddr;
		_snprintf(szDataBaseAddr,sizeof(szDataBaseAddr),TEXT("%d.%d.%d.%d"),pcbAddr[0],pcbAddr[1],pcbAddr[2],pcbAddr[3]);
		m_GameScoreDBModule->SetConnectionInfo(szDataBaseAddr,m_pGameScoreDBInfo->wDataBasePort,m_pGameScoreDBInfo->szDataBaseName,
			m_pGameScoreDBInfo->szDataBaseUser,m_pGameScoreDBInfo->szDataBasePass);

		//��������
		m_GameScoreDBModule->OpenConnection();
		m_GameScoreDBAide.SetDataBase(m_GameScoreDBModule.GetInterface());
		*/
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		return false;
	}

	return true;
}

//����ģ��ر�
bool __cdecl CDataBaseSink::OnDataBaseEngineStop(IUnknownEx * pIUnknownEx)
{
	try
	{
		//�ر�����
		if (m_AccountsDBModule.GetInterface()) m_AccountsDBModule->CloseConnection();
//		if (m_GameScoreDBModule.GetInterface()) m_GameScoreDBModule->CloseConnection();
		for (int i=0; i<m_pGameScoreDBModuleVec.size(); i++)
		{
			if (m_pGameScoreDBModuleVec[i]) m_pGameScoreDBModuleVec[i]->CloseConnection();
		}

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		return false;
	}

	return false;
}

//���ݲ�������
bool __cdecl CDataBaseSink::OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBR_GR_LOGON_BY_USERID:		//I D ��¼
		{
			return OnRequestLogon(dwContextID,pData,wDataSize);
		}
	case DBR_GR_WRITE_GAME_SCORE:		//д�ֲ���
		{
			return OnRequestWriteUserScore(dwContextID,pData,wDataSize);
		}
	case DBR_GR_GAME_SCORE_RECORD:		//��Ϸ���ּ�¼
		{
			return OnRequestGameScoreRecord(dwContextID,pData,wDataSize);
		}
	case DBR_GR_LEAVE_GAME_SERVER:		//�뿪����
		{
			return OnRequestLeaveGameServer(dwContextID,pData,wDataSize);
		}
	case DBR_GR_LIMIT_ACCOUNTS:			//�����ʺ�
		{
			return OnRequestLimitAccounts(dwContextID,pData,wDataSize);
		}
	case DBR_GR_SET_USER_RIGHT:			//����Ȩ��
		{
			return OnRequestSetUserRight(dwContextID,pData,wDataSize);
		}
	case DBR_GR_LOAD_ANDROID:			//�����û�
		{
			return OnRequestLoadAndroid(dwContextID,pData,wDataSize);
		}
	case DBR_GR_SEND_GIFT:				//�����ʻ�
		{
			return OnRequestSendGift(dwContextID,pData,wDataSize);
		}
	case DBR_GR_BUY_PROPERTY:			//������Ϣ
		{
			return OnRequestProperty(dwContextID,pData,wDataSize);
		}
	case DBR_GR_WRITE_PROPERTY:			//д�����
		{
			return OnWriteProperty(dwContextID,pData,wDataSize);
		}
	case DBR_GR_LOAD_PROPERTY:			//���ص���
		{
			return OnLoadProperty(dwContextID,pData,wDataSize);
		}
	case DBR_GR_EXCHANGE_CHARM:			//�����һ�
		{
			return OnExchangeCharm(dwContextID,pData,wDataSize);
		}
	case DBR_GR_BANK_GET_GOLD:			//��ȡ���
		{
			return OnBankDrawoutGold(dwContextID,pData,wDataSize);
		}
	case DBR_GR_BANK_STORAGE_GOLD:		//�洢���
		{
			return OnBankStorageGold(dwContextID,pData,wDataSize);
		}
	case DBR_GR_START_FINISH:			//�����������
		{
			return OnRoomStartFinish(dwContextID,pData,wDataSize);
		}
	}

	return false;
}

//��¼������
bool CDataBaseSink::OnRequestLogon(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_LogonByUserID));
	if (wDataSize!=sizeof(DBR_GR_LogonByUserID)) return false;

	//����������ݿ�����
	for (size_t i=0; i<m_pGameScoreDBModuleVec.size(); i++)
	{
		//��½����
		try
		{
			//ִ�в�ѯ
			DBR_GR_LogonByUserID * pLogonByUserID=(DBR_GR_LogonByUserID *)pData;
			//LONG lReturnValue=SPLogonByUserID(pLogonByUserID->dwUserID,pLogonByUserID->szPassWord,pLogonByUserID->dwClientIP,pLogonByUserID->szComputerID);
			//ת����ַ
			TCHAR szClientIP[16]=TEXT("");
			BYTE * pClientIP=(BYTE *)&pLogonByUserID->dwClientIP;
			_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

			//ִ�д洢����
			m_GameScoreDBAideVec[i].ResetParameter();
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@dwUserID"),pLogonByUserID->dwUserID);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@strPassword"),pLogonByUserID->szPassWord);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@strClientIP"),szClientIP);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@strMachineSerial"),pLogonByUserID->szComputerID);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@wKindID"),m_pGameServiceAttrib->wKindID);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);

			LONG lReturnValue = m_GameScoreDBAideVec[i].ExecuteProcess(TEXT("GSP_GR_EfficacyUserID"),true);

			//��¼ʧ��
			if (lReturnValue!=0L)
			{
				if (i == m_pGameScoreDBModuleVec.size() - 1)
				{
					//���һ��������ʧ��
					DBR_GR_LogonError LogonError;
					LogonError.lErrorCode=lReturnValue;
					m_GameScoreDBAideVec[i].GetValue_String(TEXT("ErrorDescribe"),LogonError.szErrorDescribe,CountArray(LogonError.szErrorDescribe));
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_LOGON_ERROR,dwContextID,&LogonError,sizeof(LogonError));
					m_pGameScoreDBModuleVec[i]->CloseRecordset();
					return true;
				}
				else
				{
					//������һ��
					continue;
				}
			}

			//��������
			DBR_GR_LogonSuccess LogonSuccess;
			memset(&LogonSuccess,0,sizeof(LogonSuccess));

			//��ȡ�û���Ϣ
			LogonSuccess.dwUserID=m_GameScoreDBAideVec[i].GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwCustomFaceVer=m_GameScoreDBAideVec[i].GetValue_DWORD(TEXT("CustomFaceVer"));
			LogonSuccess.dwGameID=m_GameScoreDBAideVec[i].GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.wFaceID=m_GameScoreDBAideVec[i].GetValue_WORD(TEXT("FaceID"));
			LogonSuccess.dwGroupID=m_GameScoreDBAideVec[i].GetValue_DWORD(TEXT("GroupID"));
			LogonSuccess.lExperience=m_GameScoreDBAideVec[i].GetValue_LONG(TEXT("Experience"));
			LogonSuccess.dwUserRight=m_GameScoreDBAideVec[i].GetValue_DWORD(TEXT("UserRight"));
			LogonSuccess.lLoveliness=m_GameScoreDBAideVec[i].GetValue_DWORD(TEXT("Loveliness"));
			LogonSuccess.dwMasterRight=m_GameScoreDBAideVec[i].GetValue_DWORD(TEXT("MasterRight"));
			m_GameScoreDBAideVec[i].GetValue_String(TEXT("Accounts"),LogonSuccess.szAccounts,CountArray(LogonSuccess.szAccounts));
			m_GameScoreDBAideVec[i].GetValue_String(TEXT("GroupName"),LogonSuccess.szGroupName,CountArray(LogonSuccess.szGroupName));
			m_GameScoreDBAideVec[i].GetValue_String(TEXT("UnderWrite"),LogonSuccess.szUnderWrite,CountArray(LogonSuccess.szUnderWrite));
			m_GameScoreDBAideVec[i].GetValue_String(TEXT("FaceUrl"),LogonSuccess.szFaceUrl,CountArray(LogonSuccess.szFaceUrl));

			//�û�����
			LogonSuccess.cbGender=m_GameScoreDBAideVec[i].GetValue_INT(TEXT("Gender"));
			LogonSuccess.cbMemberOrder=m_GameScoreDBAideVec[i].GetValue_BYTE(TEXT("MemberOrder"));
			LogonSuccess.cbMasterOrder=m_GameScoreDBAideVec[i].GetValue_BYTE(TEXT("MasterOrder"));

			//��ȡ��Ϸ��Ϣ
			LogonSuccess.lScore=m_GameScoreDBAideVec[i].GetValue_LONG(TEXT("Score"));
			LogonSuccess.lInsureScore=m_GameScoreDBAideVec[i].GetValue_LONG(TEXT("InsureScore"));
			LogonSuccess.lGameGold=m_GameScoreDBAideVec[i].GetValue_LONG(TEXT("GameGold"));
			LogonSuccess.lWinCount=m_GameScoreDBAideVec[i].GetValue_LONG(TEXT("WinCount"));
			LogonSuccess.lLostCount=m_GameScoreDBAideVec[i].GetValue_LONG(TEXT("LostCount"));
			LogonSuccess.lDrawCount=m_GameScoreDBAideVec[i].GetValue_LONG(TEXT("DrawCount"));
			LogonSuccess.lFleeCount=m_GameScoreDBAideVec[i].GetValue_LONG(TEXT("FleeCount"));

			//������Ϣ
			CopyMemory(LogonSuccess.szPassWord,pLogonByUserID->szPassWord,sizeof(LogonSuccess.szPassWord));

			//Ͷ�ݵ���֪ͨ
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_LOGON_SUCCESS,dwContextID,&LogonSuccess,sizeof(LogonSuccess));

			//������Ŀ
			int nPropertyCount = 0;
			nPropertyCount=m_GameScoreDBAideVec[i].GetValue_INT(TEXT("PropCount"));

			//���ص���
			if ( 0 < nPropertyCount )
			{
				lReturnValue = SPLoadUserProperty(pLogonByUserID->dwUserID);

				if ( lReturnValue == 0 )
				{
					DBR_GR_UserProperty UserProperty;
					ZeroMemory(&UserProperty, sizeof(UserProperty));

					UserProperty.dwUserID = LogonSuccess.dwUserID;

					//��ȡ��¼
					m_pGameScoreDBModuleVec[i]->MoveToFirst();
					while ( m_pGameScoreDBModuleVec[i]->IsRecordsetEnd() == false )
					{
						UserProperty.nPropertyID=m_GameScoreDBAideVec[i].GetValue_INT(TEXT("CateID"));
						UserProperty.dwCurCount=m_GameScoreDBAideVec[i].GetValue_DWORD(TEXT("PropCount"));

						//Ͷ�ݵ���֪ͨ
						m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_USER_PROPERTY,dwContextID, &UserProperty, sizeof(UserProperty));

						//�ƶ���¼
						m_pGameScoreDBModuleVec[i]->MoveToNext();
					}

					//Ͷ�ݵ���֪ͨ
					m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_LOAD_PROP_FINISHI,dwContextID, 0, 0);
				}
			}
			
			//�رռ�¼��
			m_pGameScoreDBModuleVec[i]->CloseRecordset();
			return true;  ///ֻҪ��һ������ɹ��ˣ��ͷ���
		}
		catch (IDataBaseException * pIException)
		{
			//���һ��������ʧ��
			if (i == m_pGameScoreDBModuleVec.size() - 1)
			{
				//������Ϣ
				LPCTSTR pszDescribe=pIException->GetExceptionDescribe(); 
				CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

				//����ʧ��
				DBR_GR_LogonError LogonError;
				LogonError.lErrorCode=-1;
				lstrcpyn(LogonError.szErrorDescribe,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��Ϸ��������"),sizeof(LogonError.szErrorDescribe));
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_LOGON_ERROR,dwContextID,&LogonError,sizeof(LogonError));
			}
		}

		//�رռ�¼��
		m_pGameScoreDBModuleVec[i]->CloseRecordset();
	}

	return true;
}

//д������
bool CDataBaseSink::OnRequestWriteUserScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_WriteUserScore));
	if (wDataSize!=sizeof(DBR_GR_WriteUserScore)) return false;

	//ִ�в�ѯ
	DBR_GR_WriteUserScore * pWriteUserScore=(DBR_GR_WriteUserScore *)pData;

	//����������ݿ�����
	for (size_t i=0; i<m_pGameScoreDBModuleVec.size(); i++)
	{
		try
		{
		//	LONG lReturnValue=SPWriteUserScore(pWriteUserScore->dwUserID,pWriteUserScore->dwPlayTimeCount,pWriteUserScore->dwOnlineTimeCount,
		//		pWriteUserScore->dwClientIP,pWriteUserScore->lRevenue,pWriteUserScore->ScoreModifyInfo);
			//ת����ַ
			TCHAR szClientIP[16]=TEXT("");
			BYTE * pClientIP=(BYTE *)&pWriteUserScore->dwClientIP;
			_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

			//ִ�д洢����
			m_GameScoreDBAideVec[i].ResetParameter();
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@dwUserID"),pWriteUserScore->dwUserID);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@lScore"),pWriteUserScore->ScoreModifyInfo.lScore);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@lRevenue"),pWriteUserScore->lRevenue);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@lWinCount"),pWriteUserScore->ScoreModifyInfo.lWinCount);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@lLostCount"),pWriteUserScore->ScoreModifyInfo.lLostCount);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@lDrawCount"),pWriteUserScore->ScoreModifyInfo.lDrawCount);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@lFleeCount"),pWriteUserScore->ScoreModifyInfo.lFleeCount);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@lExperience"),pWriteUserScore->ScoreModifyInfo.lExperience);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@dwPlayTimeCount"),pWriteUserScore->dwPlayTimeCount);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@dwOnLineTimeCount"),pWriteUserScore->dwOnlineTimeCount);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@wKindID"),m_pGameServiceAttrib->wKindID);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@strClientIP"),szClientIP);

			LONG lReturnValue= m_GameScoreDBAideVec[i].ExecuteProcess(TEXT("GSP_GR_WriteGameScore"),false);
			if (lReturnValue == 0)
			{
				return true;
			}
		}
		catch (IDataBaseException * pIException)
		{
			//������Ϣ
			LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
			CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
		}

		if (i == m_pGameScoreDBModuleVec.size()-1)
		{
			//���һ����û�д���ɹ�
			char buf[64];
			memset(buf, 0, 64);
			sprintf(buf, "write score fail. id=%ld score=%d", pWriteUserScore->dwUserID, pWriteUserScore->ScoreModifyInfo.lScore);
			CTraceService::TraceString(buf,TraceLevel_Exception);
		}
	}

	return true;
}

//д���ּ�¼����
bool CDataBaseSink::OnRequestGameScoreRecord(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_WriteGameResultDetails));
	if (wDataSize!=sizeof(DBR_GR_WriteGameResultDetails)) return false;

	//ִ�в�ѯ
	DBR_GR_WriteGameResultDetails * pGameScoreRecord=(DBR_GR_WriteGameResultDetails *)pData;

	//����������ݿ�����
	for (size_t i=0; i<m_pGameScoreDBModuleVec.size(); i++)
	{
		try
		{
			//ת����ַ
			TCHAR szClientIP[16]=TEXT("");
			BYTE * pClientIP=(BYTE *)&(pGameScoreRecord->dwClientIP);
			_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

			//ִ�д洢����
			m_GameScoreDBAideVec[i].ResetParameter();
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@dwUserID"),pGameScoreRecord->dwUserID);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@lScore"),pGameScoreRecord->lScore);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@lTotalScore"),pGameScoreRecord->lTotalScore);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@lRevenue"),pGameScoreRecord->lRevenue);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@lResult"),pGameScoreRecord->lResult);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@lAward"),pGameScoreRecord->lAward);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@lTime"),pGameScoreRecord->lTime);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@wKindID"),m_pGameServiceAttrib->wKindID);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@wTableID"),pGameScoreRecord->lTableID);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@strClientIP"),szClientIP);

			LONG lReturnValue = m_GameScoreDBAideVec[i].ExecuteProcess(TEXT("GSP_GR_WriteGameRecord_V2"),false);
			if (lReturnValue == 0)
			{
				return true;
			}
		}
		catch (IDataBaseException * pIException)
		{
			//������Ϣ
			LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
			CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
		}

		if (i == m_pGameScoreDBModuleVec.size()-1)
		{
			//���һ����û�д���ɹ�
			char buf[64];
			memset(buf, 0, 64);
			sprintf(buf, "GSP_GR_WriteGameRecord_V2 record score fail. id=%ld score=%d", pGameScoreRecord->dwUserID, pGameScoreRecord->lScore);
			CTraceService::TraceString(buf,TraceLevel_Exception);

			//д��ʧ�ܣ���¼���ļ��У����鼰�ֹ�����
			TraceMessageScore(buf);
		}
	}

	return true;
}

//�뿪����
bool CDataBaseSink::OnRequestLeaveGameServer(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_LeaveGameServer));
	if (wDataSize!=sizeof(DBR_GR_LeaveGameServer)) return false;

	//ִ�в�ѯ
	DBR_GR_LeaveGameServer * pLeaveGameServer=(DBR_GR_LeaveGameServer *)pData;

	//����������ݿ�����
	for (size_t i=0; i<m_pGameScoreDBModuleVec.size(); i++)
	{
		try
		{
	//		LONG lReturnValue=SPLeaveGameServer(pLeaveGameServer->dwUserID,pLeaveGameServer->dwPlayTimeCount,pLeaveGameServer->dwOnlineTimeCount,
	//			pLeaveGameServer->dwClientIP,pLeaveGameServer->lRevenue,pLeaveGameServer->lLoveliness,pLeaveGameServer->ScoreModifyInfo);
			//ת����ַ
			TCHAR szClientIP[16]=TEXT("");
			BYTE * pClientIP=(BYTE *)&pLeaveGameServer->dwClientIP;
			_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

			//ִ�д洢����
			m_GameScoreDBAideVec[i].ResetParameter();
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@dwUserID"),pLeaveGameServer->dwUserID);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@lScore"),pLeaveGameServer->ScoreModifyInfo.lScore);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@lGameGold"),pLeaveGameServer->ScoreModifyInfo.lGameGold);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@lInsureScore"),pLeaveGameServer->ScoreModifyInfo.lInsureScore);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@lLoveliness"),pLeaveGameServer->lLoveliness);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@lRevenue"),pLeaveGameServer->lRevenue);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@lWinCount"),pLeaveGameServer->ScoreModifyInfo.lWinCount);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@lLostCount"),pLeaveGameServer->ScoreModifyInfo.lLostCount);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@lDrawCount"),pLeaveGameServer->ScoreModifyInfo.lDrawCount);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@lFleeCount"),pLeaveGameServer->ScoreModifyInfo.lFleeCount);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@lExperience"),pLeaveGameServer->ScoreModifyInfo.lExperience);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@dwPlayTimeCount"),pLeaveGameServer->dwPlayTimeCount);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@dwOnLineTimeCount"),pLeaveGameServer->dwOnlineTimeCount);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@wKindID"),m_pGameServiceAttrib->wKindID);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@strClientIP"),szClientIP);

			LONG lReturnValue = m_GameScoreDBAideVec[i].ExecuteProcess(TEXT("GSP_GR_LeaveGameServer"),false);
			if (lReturnValue == 0)
			{
				return true;
			}
		}
		catch (IDataBaseException * pIException)
		{
			//������Ϣ
			LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
			CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
		}

		if (i == m_pGameScoreDBModuleVec.size()-1)
		{
			//���һ����û�д���ɹ�
			char buf[64];
			memset(buf, 0, 64);
			sprintf(buf, "GSP_GR_LeaveGameServer record score fail. id=%ld score=%d", pLeaveGameServer->dwUserID, pLeaveGameServer->ScoreModifyInfo.lScore);
			CTraceService::TraceString(buf,TraceLevel_Exception);
		}
	}

	return true;
}

//�����ʻ�
bool CDataBaseSink::OnRequestLimitAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GR_LimitAccounts));
		if (wDataSize!=sizeof(DBR_GR_LimitAccounts)) return false;

		//ִ�в�ѯ
		DBR_GR_LimitAccounts * pLimitAccounts=(DBR_GR_LimitAccounts *)pData;
		LONG lReturnValue=SPCongealAccounts(pLimitAccounts->dwUserID,pLimitAccounts->dwMasterUserID,pLimitAccounts->dwMasterClientIP);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	return true;
}

//����Ȩ��
bool CDataBaseSink::OnRequestSetUserRight(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_SetUserRight));
	if (wDataSize!=sizeof(DBR_GR_SetUserRight)) return false;

	//ִ�в�ѯ
	DBR_GR_SetUserRight * pSetUserRight=(DBR_GR_SetUserRight *)pData;
	LONG lGameFailed = TRUE,lAccountsFailed = TRUE;

	try
	{
		//��ϷȨ��
		if (pSetUserRight->cbGame==TRUE) 
		{
			lGameFailed = SPSetUserGameRight(pSetUserRight->dwUserID,pSetUserRight->dwUserRight,pSetUserRight->dwMasterUserID,pSetUserRight->dwMasterClientIP);
		}

		//�ʺ�Ȩ��
		if (pSetUserRight->cbAccounts==TRUE) 
		{
			lAccountsFailed = SPSetUserAccountsRight(pSetUserRight->dwUserID,pSetUserRight->dwUserRight,pSetUserRight->dwMasterUserID,pSetUserRight->dwMasterClientIP);
		}

		//������Ϣ
		DBR_GR_UserRightResult UserRightResult;
		ZeroMemory(&UserRightResult,sizeof(UserRightResult));

		UserRightResult.dwUserRight = pSetUserRight->dwUserRight;
		UserRightResult.dwUserID = pSetUserRight->dwUserID;
		UserRightResult.dwMasterUserID = pSetUserRight->dwMasterUserID;
		UserRightResult.bGameSuccess=((lGameFailed==FALSE)?true:false);
		UserRightResult.bAccountsSuccess=((lAccountsFailed==FALSE)?true:false);

		//������Ϣ
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_USER_RIGHT_RESULT,dwContextID,&UserRightResult,sizeof( UserRightResult ));
	
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		//������Ϣ
		DBR_GR_UserRightResult UserRightResult;
		ZeroMemory(&UserRightResult,sizeof(UserRightResult));

		UserRightResult.dwUserRight = pSetUserRight->dwUserRight;
		UserRightResult.dwUserID = pSetUserRight->dwUserID;
		UserRightResult.dwMasterUserID = pSetUserRight->dwMasterUserID;
		UserRightResult.bGameSuccess=((lGameFailed==FALSE)?true:false);
		UserRightResult.bAccountsSuccess=((lAccountsFailed==FALSE)?true:false);

		//������Ϣ
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_USER_RIGHT_RESULT,dwContextID,&UserRightResult,sizeof( UserRightResult ));
	}

	return true;
}

//�����û�
bool CDataBaseSink::OnRequestLoadAndroid(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	return true;
/*
	try
	{
		//��������
		DBO_GR_AndroidUser AndroidUser;
		ZeroMemory(&AndroidUser,sizeof(AndroidUser));

		//�û��ʻ�
		m_GameScoreDBAide.ResetParameter();
		m_GameScoreDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceAttrib->wKindID);
		m_GameScoreDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);

		//ִ�в�ѯ
		AndroidUser.lResultCode=m_GameScoreDBAide.ExecuteProcess(TEXT("GSP_GR_LoadAndroidUser"),true);

		//��ȡ��Ϣ
		for (WORD i=0;i<CountArray(AndroidUser.dwAndroidID);i++)
		{
			//�����ж�
			if (m_GameScoreDBModule->IsRecordsetEnd()==true) break;

			//��ȡ����
			DWORD dwAndroidID=0L;
			dwAndroidID=m_GameScoreDBAide.GetValue_DWORD(TEXT("UserID"));
			AndroidUser.dwAndroidID[AndroidUser.wAndroidCount++]=dwAndroidID;

			//�ƶ���¼
			m_GameScoreDBModule->MoveToNext();
		}

		//������Ϣ
		WORD wHeadSize=sizeof(AndroidUser)-sizeof(AndroidUser.dwAndroidID);
		WORD wPacketSize=wHeadSize+AndroidUser.wAndroidCount*sizeof(AndroidUser.dwAndroidID[0]);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_ANDROID_USER,dwContextID,&AndroidUser,wPacketSize);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		//��������
		DBO_GR_AndroidUser AndroidUser;
		ZeroMemory(&AndroidUser,sizeof(AndroidUser));

		//�������
		AndroidUser.lResultCode=-1;
		AndroidUser.wAndroidCount=0L;

		//������Ϣ
		WORD wPacketSize=sizeof(AndroidUser)-sizeof(AndroidUser.dwAndroidID);
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_ANDROID_USER,dwContextID,&AndroidUser,wPacketSize);
	}

	return false;
*/
}

//�����ʻ�
bool CDataBaseSink::OnRequestSendGift(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	return true;

	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_SendGift));
	if (wDataSize!=sizeof(DBR_GR_SendGift)) return false;

	//����ת��
	DBR_GR_SendGift * pSendGift=(DBR_GR_SendGift *)pData;

	try
	{
		//��¼����
		LONG lRet = SPRecordGiftGrant(pSendGift->dwSendUserID, pSendGift->dwRcvUserID, pSendGift->wGiftID, pSendGift->dwClientIP,
			pSendGift->lSendUserCharm, pSendGift->lRcvUserCharm, pSendGift->lFlowerPrice, pSendGift->wFlowerCount);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	return true;
}

//������Ϣ
bool CDataBaseSink::OnRequestProperty(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//����ת��
	DBR_GR_Property * pProperty=(DBR_GR_Property *)pData;

	try
	{
		//��¼����
		SPBuyProperty(pProperty->dwUserID, pProperty->dwTargetUserID, pProperty->nPropertyID, pProperty->dwCurCount, 
			pProperty->dwOnceCount, pProperty->dwPachurseCount, pProperty->lPrice, pProperty->dwClientIP);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	return true;
}

//д�����
bool CDataBaseSink::OnWriteProperty(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_WriteProperty));
	if (wDataSize!=sizeof(DBR_GR_WriteProperty)) return false;

	//����ת��
	DBR_GR_WriteProperty * pWriteProperty=(DBR_GR_WriteProperty *)pData;

	try
	{
		//д�����
		SPWriteProperty(pWriteProperty->dwUserID, pWriteProperty->nPropertyID, pWriteProperty->lUseableTime, pWriteProperty->dwClientIP);

		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	return true;
}

//���ص���
bool CDataBaseSink::OnLoadProperty(DWORD dwContextID, VOID * pData, WORD wDataSize)
{	
	return true;
/*
	try
	{
		LONG lReturn = -1;

		//���ص���
		lReturn = SPLoadPropertyAttribute();
		if ( lReturn > 0 )
		{
			DBR_GR_PropertyAttribute PropertyAttribute;
			ZeroMemory(&PropertyAttribute, sizeof(PropertyAttribute));

			//��ȡ��¼
			m_GameScoreDBModule->MoveToFirst();
			while ( m_GameScoreDBModule->IsRecordsetEnd() == false )
			{
				PropertyAttribute.wPropertyID=m_GameScoreDBAide.GetValue_WORD(TEXT("CateID"));
				PropertyAttribute.dwPropCount1=m_GameScoreDBAide.GetValue_DWORD(TEXT("PropCount1"));
				PropertyAttribute.dwPropCount2=m_GameScoreDBAide.GetValue_DWORD(TEXT("PropCount2"));
				PropertyAttribute.dwPropCount3=m_GameScoreDBAide.GetValue_DWORD(TEXT("PropCount3"));
				PropertyAttribute.dwPropCount4=m_GameScoreDBAide.GetValue_DWORD(TEXT("PropCount4"));
				PropertyAttribute.dwPropCount5=m_GameScoreDBAide.GetValue_DWORD(TEXT("PropCount5"));
				PropertyAttribute.dwPropCount6=m_GameScoreDBAide.GetValue_DWORD(TEXT("PropCount6"));
				PropertyAttribute.dwPropCount7=m_GameScoreDBAide.GetValue_DWORD(TEXT("PropCount7"));
				PropertyAttribute.dwPropCount8=m_GameScoreDBAide.GetValue_DWORD(TEXT("PropCount8"));
				PropertyAttribute.dwPropCount9=m_GameScoreDBAide.GetValue_DWORD(TEXT("PropCount9"));
				PropertyAttribute.dwPropCount10=m_GameScoreDBAide.GetValue_DWORD(TEXT("PropCount10"));

				PropertyAttribute.lPrice1=m_GameScoreDBAide.GetValue_LONG(TEXT("Price1"));
				PropertyAttribute.lPrice2=m_GameScoreDBAide.GetValue_LONG(TEXT("Price2"));
				PropertyAttribute.lPrice3=m_GameScoreDBAide.GetValue_LONG(TEXT("Price3"));
				PropertyAttribute.lPrice4=m_GameScoreDBAide.GetValue_LONG(TEXT("Price4"));
				PropertyAttribute.lPrice5=m_GameScoreDBAide.GetValue_LONG(TEXT("Price5"));
				PropertyAttribute.lPrice6=m_GameScoreDBAide.GetValue_LONG(TEXT("Price6"));
				PropertyAttribute.lPrice7=m_GameScoreDBAide.GetValue_LONG(TEXT("Price7"));
				PropertyAttribute.lPrice8=m_GameScoreDBAide.GetValue_LONG(TEXT("Price8"));
				PropertyAttribute.lPrice9=m_GameScoreDBAide.GetValue_LONG(TEXT("Price9"));
				PropertyAttribute.lPrice10=m_GameScoreDBAide.GetValue_LONG(TEXT("Price10"));

				PropertyAttribute.cbDiscount=m_GameScoreDBAide.GetValue_BYTE(TEXT("Discount"));

				//Ͷ�ݵ���֪ͨ
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_PROPERTY_ATTRIBUTE,dwContextID,&PropertyAttribute, sizeof(PropertyAttribute));

				//�ƶ���¼
				m_GameScoreDBModule->MoveToNext();
			}
		}

		//�����ʻ�
		lReturn = SPLoadFlowerAttribute();
		if ( lReturn > 0 )
		{
			//��������
			DBR_GR_FlowerAttribute FlowerAttribute;
			ZeroMemory(&FlowerAttribute, sizeof(FlowerAttribute));

			//��ȡ��¼
			m_GameScoreDBModule->MoveToFirst();
			while ( m_GameScoreDBModule->IsRecordsetEnd() == false )
			{
				FlowerAttribute.nFlowerID=m_GameScoreDBAide.GetValue_INT(TEXT("ID"));
				FlowerAttribute.lSendUserCharm=m_GameScoreDBAide.GetValue_LONG(TEXT("SendUserCharm"));
				FlowerAttribute.lRcvUserCharm=m_GameScoreDBAide.GetValue_LONG(TEXT("RcvUserCharm"));
				FlowerAttribute.lPrice=m_GameScoreDBAide.GetValue_LONG(TEXT("Price"));
				FlowerAttribute.cbDiscount=m_GameScoreDBAide.GetValue_BYTE(TEXT("Discount"));

				//Ͷ�ݵ���֪ͨ
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_FLOWER_ATTRIBUTE,dwContextID,&FlowerAttribute, sizeof(FlowerAttribute));

				//�ƶ���¼
				m_GameScoreDBModule->MoveToNext();
			}
		}

	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	return true;
*/
}

//�����һ�
bool CDataBaseSink::OnExchangeCharm(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_ExchangeLoveliness));
	if (wDataSize!=sizeof(DBR_GR_ExchangeLoveliness)) return false;

	//����ת��
	DBR_GR_ExchangeLoveliness * pExchangeLoveliness=(DBR_GR_ExchangeLoveliness *)pData;

	LONG lReturnValue = -1;
	try
	{
		//�����һ�
		lReturnValue = SPExchangeCharm(pExchangeLoveliness->dwUserID, pExchangeLoveliness->lLoveliness, 
			pExchangeLoveliness->lGoldValue, pExchangeLoveliness->dwClientIP);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	////�������
	//DBR_GR_ExchangeCharmResult ExchangeCharmResult;
	//ZeroMemory(&ExchangeCharmResult, sizeof(ExchangeCharmResult));
	//ExchangeCharmResult.cbResultCode = BYTE(lReturnValue);
	//ExchangeCharmResult.dwUserID = pExchangeLoveliness->dwUserID;
	//ExchangeCharmResult.lLoveliness = pExchangeLoveliness->lLoveliness;
	//ExchangeCharmResult.lGoldValue = pExchangeLoveliness->lGoldValue;

	////Ͷ�ݵ���֪ͨ
	//m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_EXCHANGE_RESULT,dwContextID, &ExchangeCharmResult, sizeof(ExchangeCharmResult));

	return true;
}

//��ȡ���
bool CDataBaseSink::OnBankDrawoutGold(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_BankDrawoutGold));
	if (wDataSize!=sizeof(DBR_GR_BankDrawoutGold)) return false;

	//����ת��
	DBR_GR_BankDrawoutGold * pBankGet=(DBR_GR_BankDrawoutGold *)pData;

	LONG lReturnValue = -1;
	try
	{
		//��ȡ���
		lReturnValue = SPBankDrawoutGold(pBankGet->dwUserID,pBankGet->DrawoutGoldCount,pBankGet->dwClientIP);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	return true;
}

//�洢���
bool CDataBaseSink::OnBankStorageGold(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GR_BankStorage));
	if (wDataSize!=sizeof(DBR_GR_BankStorage)) return false;

	//����ת��
	DBR_GR_BankStorage * pBankStorage=(DBR_GR_BankStorage *)pData;

	LONG lReturnValue = -1;
	try
	{
		//��ȡ���
		lReturnValue = SPBankStorageGold(pBankStorage->dwUserID,pBankStorage->lStorageCount,pBankStorage->dwClientIP);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	return true;
}


//�����������
bool CDataBaseSink::OnRoomStartFinish(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//Ч�����
	ASSERT(wDataSize==sizeof(tagGameServiceOption));
	if (wDataSize!=sizeof(tagGameServiceOption)) return false;

	//����ת��
	tagGameServiceOption * pParam=(tagGameServiceOption *)pData;

	//����������ݿ�����
	for (size_t i=0; i<m_pGameScoreDBModuleVec.size(); i++)
	{
		try
		{
			//ִ�д洢����
			m_GameScoreDBAideVec[i].ResetParameter();
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@wKindID"),m_pGameServiceAttrib->wKindID);
			m_GameScoreDBAideVec[i].AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);

			LONG lReturnValue= m_GameScoreDBAideVec[i].ExecuteProcess(TEXT("GSP_GR_RoomStartFinish"),false);
		}
		catch (IDataBaseException * pIException)
		{
			//������Ϣ
			LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
			CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
		}
	}

	return true;
}


//I D �洢����
LONG CDataBaseSink::SPLogonByUserID(DWORD dwUserID, LPCTSTR pszPassword, DWORD dwClientIP, LPCTSTR pszComputerID)
{
	return 0;
}

//д�ִ洢����
LONG CDataBaseSink::SPWriteUserScore(DWORD dwUserID, DWORD dwPlayTimeCount, DWORD dwOnLineTimeCount, DWORD dwClientIP, LONG lRevenue, tagUserScore & UserScore)
{
	return 0;
}

//д��Ϸ������־�洢����
LONG CDataBaseSink::SPGameScoreRecord(DBR_GR_WriteGameResultDetails * pGameScoreRecord)
{
	return 0;
}

//�뿪�洢����
LONG CDataBaseSink::SPLeaveGameServer(DWORD dwUserID, DWORD dwPlayTimeCount, DWORD dwOnLineTimeCount, DWORD dwClientIP, LONG lRevenue, LONG lLoveliness, tagUserScore & UserScore)
{
	return 0;
}

//���Ŵ洢����
LONG CDataBaseSink::SPCongealAccounts(DWORD dwUserID, DWORD dwMasterUserID, DWORD dwClientIP)
{
	return 0;
}

//Ȩ�޴洢����
LONG CDataBaseSink::SPSetUserGameRight(DWORD dwUserID, DWORD dwUserRight, DWORD dwMasterUserID, DWORD dwClientIP)
{
	return 0;
}

//Ȩ�޴洢����
LONG CDataBaseSink::SPSetUserAccountsRight(DWORD dwUserID, DWORD dwUserRight, DWORD dwMasterUserID, DWORD dwClientIP)
{
	return 0;
}

//��¼����
LONG CDataBaseSink::SPRecordGiftGrant(DWORD dwSendUserID, DWORD dwRcvUserID, WORD wGiftID, DWORD dwClientIP, 
									  DWORD dwSendUserLoveliness, DWORD dwRcvUserLoveliness, LONG lValue, int nFlowerCount)
{
	return 0;
}

//�������
LONG CDataBaseSink::SPBuyProperty(DWORD dwUserID, DWORD dwTargetUserID, int nPropertyID, DWORD dwCurCount, DWORD dwOnceCount, 
								  DWORD dwPachurCount, LONG lPrice, DWORD dwClientIP)
{
	return 0;
}

//д�����
LONG CDataBaseSink::SPWriteProperty(DWORD dwUserID, int nPropertyID, DWORD lUseableTime, DWORD dwClientIP)
{
	return 0;
}

//���ص���
LONG CDataBaseSink::SPLoadPropertyAttribute()
{
	return 0;
}

//�����ʻ�
LONG CDataBaseSink::SPLoadFlowerAttribute()
{
	return 0;
}

//���ص���
LONG CDataBaseSink::SPLoadUserProperty(DWORD dwUserID)
{
	return 0;
}

//�����һ�
LONG CDataBaseSink::SPExchangeCharm(DWORD dwUserID, LONG lLoveliness, LONG lGoldValue, DWORD dwClientIP)
{
	return 0;
}

//��ȡ�洢����
LONG CDataBaseSink::SPBankDrawoutGold(DWORD dwUserID, DWORD lSwapScore, DWORD dwClientIP)
{
	return 0;
}

//���洢����
LONG CDataBaseSink::SPBankStorageGold(DWORD dwUserID, DWORD lSwapScore, DWORD dwClientIP)
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////
