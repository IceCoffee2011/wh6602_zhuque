#include "StdAfx.h"
#include "Afxinet.h"
#include "DataBaseSink.h"
#include "CGlobalData.h"


//////////////////////////////////////////////////////////////////////////

//���캯��
CDataBaseSink::CDataBaseSink()
{
	m_pRoomLog = NULL;		// ��־

	//���ñ���
	m_pGameUserDBInfo=NULL;
	m_pGameScoreDBInfo=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;
	m_pIDataBaseEngineEvent=NULL;

	return;
}

//��������
CDataBaseSink::~CDataBaseSink()
{
	CCGlobalData::UnAllocGradeInfoMemory();
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
	if ((m_AccountsDBModule.GetInterface()==NULL)&&(m_AccountsDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//��������
	if ((m_GameScoreDBModule.GetInterface()==NULL)&&(m_GameScoreDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	CString strDBName;

	try
	{
		//��������
		BYTE * pcbAddr=NULL;
		TCHAR szDataBaseAddr[16]=TEXT("");

		//�����û����ݿ�
		pcbAddr=(BYTE *)&m_pGameUserDBInfo->dwDataBaseAddr;
		_snprintf(szDataBaseAddr,sizeof(szDataBaseAddr),TEXT("%d.%d.%d.%d"),pcbAddr[0],pcbAddr[1],pcbAddr[2],pcbAddr[3]);
		m_AccountsDBModule->SetConnectionInfo(szDataBaseAddr,m_pGameUserDBInfo->wDataBasePort,m_pGameUserDBInfo->szDataBaseName,
			m_pGameUserDBInfo->szDataBaseUser,m_pGameUserDBInfo->szDataBasePass);

		strDBName = m_pGameUserDBInfo->szDataBaseName;

		//��������
		m_AccountsDBModule->OpenConnection();
		m_AccountsDBAide.SetDataBase(m_AccountsDBModule.GetInterface());

		//������Ϸ���ݿ�
		pcbAddr=(BYTE *)&m_pGameScoreDBInfo->dwDataBaseAddr;
		_snprintf(szDataBaseAddr,sizeof(szDataBaseAddr),TEXT("%d.%d.%d.%d"),pcbAddr[0],pcbAddr[1],pcbAddr[2],pcbAddr[3]);
		m_GameScoreDBModule->SetConnectionInfo(szDataBaseAddr,m_pGameScoreDBInfo->wDataBasePort,m_pGameScoreDBInfo->szDataBaseName,
			m_pGameScoreDBInfo->szDataBaseUser,m_pGameScoreDBInfo->szDataBasePass);

		strDBName = m_pGameScoreDBInfo->szDataBaseName;

		//��������
		m_GameScoreDBModule->OpenConnection();
		m_GameScoreDBAide.SetDataBase(m_GameScoreDBModule.GetInterface());

		if (m_pRoomLog)
			m_pRoomLog->AddMsg(0, 0, TRUE,"***** ���ݿ�ģ������");
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s , ����ԭ��%s, %s , ������ʧ�ܣ��û��������벻���ǲ��������ԭ��֮һ", 
				strErrorLine.GetBuffer(), pszDescribe, strDBName.GetBuffer());

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}

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
		if (m_GameScoreDBModule.GetInterface()) m_GameScoreDBModule->CloseConnection();
		
		if (m_pRoomLog)
			m_pRoomLog->AddMsg(0, 0, TRUE,"***** ���ݿ�ģ��ر�");
		
		return true;
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s , ����ԭ��%s", 
										strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}

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
	case DBR_GR_BANK_GET_GOLD:			//��ȡ���
		{
			return OnBankDrawoutGold(dwContextID,pData,wDataSize);
		}
	case DBR_GR_BANK_STORAGE_GOLD:		//�洢���
		{
			return OnBankStorageGold(dwContextID,pData,wDataSize);
		}
	case DBR_GR_LOAD_AFFICHE:  //����
		{
			return OnAffiche(dwContextID, pData, wDataSize);
		}
	case DBR_GR_LOAD_HAPPY_MESS: //ϲ��
		{
			return OnHappyMess(dwContextID, pData, wDataSize);
		}
	case DBR_GR_UNLOCK_USER: //�û�����
		{
			return OnUnLockUser(dwContextID, pData, wDataSize);
		}
	case DBR_GP_READ_GRADE_INFO:  //��ȡ�ȼ���Ϣ
		{
			return OnReadGradeInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GR_USER_FIND_GEMS_TASK_PERFORM_FINISH:       //�������
		{
			return OnFindGemsTaskPerformFinish(dwContextID,pData,wDataSize);
		}
	case DBR_GR_CONTINUE_CONNECT:	//�������ݿ�����һֱ����	Author:<cxf>
		{
			ContinueConnect();
			return true;
		}
	case DBR_GR_ONLINE_GIFT://��������<2012-02-07>
		{
			NewGiftUser(pData,wDataSize);
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

	//��½����
	try
	{
		//ִ�в�ѯ
		DBR_GR_LogonByUserID * pLogonByUserID=(DBR_GR_LogonByUserID *)pData;
		LONG lReturnValue=SPLogonByUserID(pLogonByUserID->dwUserID,pLogonByUserID->szPassWord,pLogonByUserID->dwClientIP,pLogonByUserID->szComputerID);

		//��¼ʧ��
		if (lReturnValue!=0L)
		{
			DBR_GR_LogonError LogonError;
			LogonError.lErrorCode=lReturnValue;
			m_GameScoreDBAide.GetValue_String(TEXT("ErrorDescribe"),LogonError.szErrorDescribe,CountArray(LogonError.szErrorDescribe));
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_LOGON_ERROR,dwContextID,&LogonError,sizeof(LogonError));
			m_GameScoreDBModule->CloseRecordset();
			return true;
		}

		//��������
		DBR_GR_LogonSuccess LogonSuccess;
		memset(&LogonSuccess,0,sizeof(LogonSuccess));

		//��ȡ�û���Ϣ
		LogonSuccess.dwUserID=m_GameScoreDBAide.GetValue_DWORD(TEXT("UserID"));
		LogonSuccess.dwCustomFaceVer=m_GameScoreDBAide.GetValue_DWORD(TEXT("CustomFaceVer"));
		LogonSuccess.dwGameID=m_GameScoreDBAide.GetValue_DWORD(TEXT("GameID"));
		LogonSuccess.wFaceID=m_GameScoreDBAide.GetValue_WORD(TEXT("FaceID"));
		LogonSuccess.dwGroupID=m_GameScoreDBAide.GetValue_DWORD(TEXT("GroupID"));
		LogonSuccess.lExperience=m_GameScoreDBAide.GetValue_LONG(TEXT("Experience"));
		LogonSuccess.dwUserRight=m_GameScoreDBAide.GetValue_DWORD(TEXT("UserRight"));
		LogonSuccess.lLoveliness=m_GameScoreDBAide.GetValue_DWORD(TEXT("Loveliness"));
		LogonSuccess.dwMasterRight=m_GameScoreDBAide.GetValue_DWORD(TEXT("MasterRight"));

		//�������
		LogonSuccess.lGold        = m_GameScoreDBAide.GetValue_LONGLONG("lGold");
		LogonSuccess.lGem         = m_GameScoreDBAide.GetValue_LONGLONG("lGem");
		LogonSuccess.dwGrade        = m_GameScoreDBAide.GetValue_DWORD("dwGrade");

		
		m_GameScoreDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));
		LogonSuccess.utIsAndroid     =  (USER_TYPE)m_GameScoreDBAide.GetValue_INT("IsAndroid");
		//��ӽ���
		m_GameScoreDBAide.GetValue_String(TEXT("Accounts"),LogonSuccess.szAccounts,CountArray(LogonSuccess.szAccounts));
		m_GameScoreDBAide.GetValue_String(TEXT("GroupName"),LogonSuccess.szGroupName,CountArray(LogonSuccess.szGroupName));
		m_GameScoreDBAide.GetValue_String(TEXT("UnderWrite"),LogonSuccess.szUnderWrite,CountArray(LogonSuccess.szUnderWrite));
		m_GameScoreDBAide.GetValue_String(TEXT("BankPassword"),LogonSuccess.szBankPassword,CountArray(LogonSuccess.szBankPassword));

		//�û�����
		LogonSuccess.cbGender=m_GameScoreDBAide.GetValue_BYTE(TEXT("Gender"));
		LogonSuccess.cbMemberOrder=m_GameScoreDBAide.GetValue_BYTE(TEXT("MemberOrder"));
		LogonSuccess.cbMasterOrder=m_GameScoreDBAide.GetValue_BYTE(TEXT("MasterOrder"));

		//��ȡ��Ϸ��Ϣ
		LogonSuccess.lMoney=m_GameScoreDBAide.GetValue_LONGLONG(TEXT("lMoney"));
		LogonSuccess.lWinCount=m_GameScoreDBAide.GetValue_LONG(TEXT("WinCount"));
		LogonSuccess.lLostCount=m_GameScoreDBAide.GetValue_LONG(TEXT("LostCount"));
		LogonSuccess.lDrawCount=m_GameScoreDBAide.GetValue_LONG(TEXT("DrawCount"));
		LogonSuccess.lFleeCount=m_GameScoreDBAide.GetValue_LONG(TEXT("FleeCount"));
		LogonSuccess.lOnlineTime=m_GameScoreDBAide.GetValue_LONG(TEXT("OnlineTime"));//Author<cxf>
		LogonSuccess.wOnlineGiftCount=m_GameScoreDBAide.GetValue_LONG(TEXT("OnlineGiftCount"));
		//������Ϣ
		CopyMemory(LogonSuccess.szPassWord,pLogonByUserID->szPassWord,sizeof(LogonSuccess.szPassWord));

		//Author<cxf>;DateTime<2012-03-02>;Description<��ȡӮȡ����>
		if (0!=SPDayWinLostCount(pLogonByUserID->dwUserID))
		{
			LogonSuccess.lKindIDWinCount=m_GameScoreDBAide.GetValue_LONG(TEXT("KindIDWinCount"));
			LogonSuccess.lKindIDLostCount=m_GameScoreDBAide.GetValue_LONG(TEXT("KindIDLostCount"));
			LogonSuccess.lKindIDDrawCount=m_GameScoreDBAide.GetValue_LONG(TEXT("KindIDDrawCount"));
			LogonSuccess.lKindIDFleeCount=m_GameScoreDBAide.GetValue_LONG(TEXT("KindIDFleeCount"));
		}
		//Ͷ�ݵ���֪ͨ
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_LOGON_SUCCESS,dwContextID,&LogonSuccess,sizeof(LogonSuccess));

		//�������
		LoadFindGamesTask( pLogonByUserID->dwUserID, pLogonByUserID->dwKindID);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe(); 
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}

		//����ʧ��
		DBR_GR_LogonError LogonError;
		LogonError.lErrorCode=-1;
		lstrcpyn(LogonError.szErrorDescribe,TEXT("��������ʧ�ܣ������Ժ����Ի�ѡ����һ��Ϸ��������"),CountArray(LogonError.szErrorDescribe));
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_LOGON_ERROR,dwContextID,&LogonError,sizeof(LogonError));
	}

	//�رռ�¼��)
	m_GameScoreDBModule->CloseRecordset();
	return true;
}

//д������
bool CDataBaseSink::OnRequestWriteUserScore(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GR_WriteUserScore));
		if (wDataSize!=sizeof(DBR_GR_WriteUserScore)) return false;

		//ִ�в�ѯ
		DBR_GR_WriteUserScore * pWriteUserScore=(DBR_GR_WriteUserScore *)pData;
		LONG lReturnValue=SPWriteUserScore(pWriteUserScore->dwUserID,   pWriteUserScore->dwPlayTimeCount, pWriteUserScore->lRevenue,
			                               pWriteUserScore->dwClientIP, pWriteUserScore->ScoreModifyInfo);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);
			
			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
	}

	return true;
}

//�뿪����
bool CDataBaseSink::OnRequestLeaveGameServer(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DBR_GR_LeaveGameServer));
		if (wDataSize!=sizeof(DBR_GR_LeaveGameServer)) return false;

		//ִ�в�ѯ
		DBR_GR_LeaveGameServer * pLeaveGameServer=(DBR_GR_LeaveGameServer *)pData;
		LONG lReturnValue=SPLeaveGameServer(pLeaveGameServer->dwUserID,pLeaveGameServer->dwPlayTimeCount,pLeaveGameServer->dwOnlineTimeCount,
			pLeaveGameServer->dwClientIP,pLeaveGameServer->lRevenue,pLeaveGameServer->lLoveliness);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
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

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
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

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
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
	}

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

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
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

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
	}
	return true;
}

//��ȡѰ������
void CDataBaseSink::LoadFindGamesTask( DWORD userID, DWORD kindID)
{
	LONG lReturnValue = SPGetLoadFindGemsTask(userID, kindID);
	
	try
	{
		if (lReturnValue > 0)
		{
			//��ȡ��¼
			DBR_FIND_GEMS_TASK  task;
			memset(&task,  0, sizeof(task));

			task.iTaskID      =   m_AccountsDBAide.GetValue_INT(TEXT("dwTaskID"));
			m_AccountsDBAide.GetValue_String(TEXT("Title"), task.strTitle, CountArray(task.strTitle));
			m_AccountsDBAide.GetValue_String(TEXT("Context"), task.strContext, CountArray(task.strContext));

			task.iLargessCount1 = m_AccountsDBAide.GetValue_INT(TEXT("LargessCount1"));
			task.iLargessCount2 = m_AccountsDBAide.GetValue_INT(TEXT("LargessCount2"));
			task.iLargessCount3 = m_AccountsDBAide.GetValue_INT(TEXT("LargessCount3"));
			task.iLargessCount4 = m_AccountsDBAide.GetValue_INT(TEXT("LargessCount4"));
			task.iTaskUserLogID = m_AccountsDBAide.GetValue_INT(TEXT("TaskUserLogID"));
			m_AccountsDBAide.GetValue_String(TEXT("TaskUserLogTable"), task.strUserLogTable, CountArray(task.strUserLogTable));
			task.dwUserID       = m_AccountsDBAide.GetValue_DWORD(TEXT("dwUserID"));
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_USER_FIND_GEMS_TASK_LOAD,
				0, 
				&task, 
				sizeof(DBR_FIND_GEMS_TASK));
		} 
	}
	catch (IDataBaseException * pIException)
	{
		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			//������Ϣ
			LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
			CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
	}
	
	// add by HouGuoJiang 2012-03-27
	//�رռ�¼����ֻҪ�õ��˼�¼�����ͱ���ر�
	m_AccountsDBModule->CloseRecordset();
}


//�õ�Ѱ������
LONG CDataBaseSink::SPGetLoadFindGemsTask(DWORD dwUserID, DWORD kindID)
 {
	try
	{
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"), dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@KindID"), kindID);
		return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_GetTask"),true);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
		return -1;
	}
}

bool CDataBaseSink::OnFindGemsTaskPerformFinish(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		DBR_TASK_Finish* pTask = (DBR_TASK_Finish*)pData;
		ASSERT(pTask->dwUserID  !=0L);
		ASSERT(pTask->iTaskID   !=0L);
		
		if (pTask->bFinish)
		{
			m_AccountsDBAide.ResetParameter();
			m_AccountsDBAide.AddParameter(TEXT("@UserID"),			 pTask->dwUserID);
			m_AccountsDBAide.AddParameter(TEXT("@TaskID"),			 pTask->iTaskID);
			m_AccountsDBAide.AddParameter(TEXT("@TaskUserID"),		 pTask->iTaskUserLogID);
			m_AccountsDBAide.AddParameter(TEXT("@SererTypeID"),      pTask->dwKindID);
			m_AccountsDBAide.AddParameter(TEXT("@TaskUserLogTable"), pTask->strUserLogTable);
			m_AccountsDBAide.AddParameter(TEXT("@SererID"),          pTask->dwServerID);
			m_AccountsDBAide.AddParameter(TEXT("@IpAddr"),           pTask->szClientIP);	
			m_AccountsDBAide.AddParameter(TEXT("@SortID"),           pTask->dwSortID);
			m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_SetTaskEnd"),true);
			
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_RE_FIND_GEMS_TASK_PERFORM_FINISH,
																								dwContextID,  pTask, sizeof(CMD_GF_TASK_tagTaskFinish));
		}

		LoadFindGamesTask(pTask->dwUserID, pTask->dwKindID);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
	}

	return true;
}


//I D �洢����
LONG CDataBaseSink::SPLogonByUserID(DWORD dwUserID, LPCTSTR pszPassword, DWORD dwClientIP, LPCTSTR pszComputerID)
{
	//Ч�����
	ASSERT(dwUserID!=0L);
	ASSERT(pszPassword!=NULL);
	try
	{
		//ת����ַ
		TCHAR szClientIP[16]=TEXT("");
		BYTE * pClientIP=(BYTE *)&dwClientIP;
		_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

		//ִ�д洢����
		m_GameScoreDBAide.ResetParameter();
		m_GameScoreDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
		m_GameScoreDBAide.AddParameter(TEXT("@strPassword"),pszPassword);
		m_GameScoreDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);
		m_GameScoreDBAide.AddParameter(TEXT("@strMachineSerial"),pszComputerID);
		m_GameScoreDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceAttrib->wKindID);
		m_GameScoreDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);

		return m_GameScoreDBAide.ExecuteProcess(TEXT("GSP_GR_EfficacyUserID"),true);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
	}

	return -1;
}

//д�ִ洢����
LONG CDataBaseSink::SPWriteUserScore(DWORD dwUserID, DWORD dwPlayTimeCount, LONGLONG lRevenue,
									 DWORD dwClientIP, tagUserScore & UserScore)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	try
	{
		//ת����ַ
		TCHAR szClientIP[16]=TEXT("");
		BYTE * pClientIP=(BYTE *)&dwClientIP;
		_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

		//ִ�д洢����
		m_GameScoreDBAide.ResetParameter();
		m_GameScoreDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
		m_GameScoreDBAide.AddParameter(TEXT("@lWinCount"),UserScore.lWinCount);
		m_GameScoreDBAide.AddParameter(TEXT("@lLostCount"),UserScore.lLostCount);
		m_GameScoreDBAide.AddParameter(TEXT("@lDrawCount"),UserScore.lDrawCount);
		m_GameScoreDBAide.AddParameter(TEXT("@lFleeCount"),UserScore.lFleeCount);
		m_GameScoreDBAide.AddParameter(TEXT("@lExperience"),UserScore.lExperience);
		m_GameScoreDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceAttrib->wKindID);
		m_GameScoreDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_GameScoreDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);
		
		//�������
		m_GameScoreDBAide.AddParameter(TEXT("@lGold"),    UserScore.lGold);
		m_GameScoreDBAide.AddParameter(TEXT("@lGem"),     UserScore.lGem);
		m_GameScoreDBAide.AddParameter(TEXT("@dwGrade"),  UserScore.dwGrade);

		m_GameScoreDBAide.AddParameter(TEXT("@lRevenue"),        lRevenue);
		m_GameScoreDBAide.AddParameter(TEXT("@dwPlayTimeCount"), dwPlayTimeCount);
		//��ӽ���
		return m_GameScoreDBAide.ExecuteProcess(TEXT("GSP_GR_WriteGameScore"),false);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
	}
	return -1;
}

//�뿪�洢����
LONG CDataBaseSink::SPLeaveGameServer(DWORD dwUserID, DWORD dwPlayTimeCount, DWORD dwOnLineTimeCount, DWORD dwClientIP, LONGLONG lRevenue, LONG lLoveliness)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	try
	{
		//ת����ַ
		TCHAR szClientIP[16]=TEXT("");
		BYTE * pClientIP=(BYTE *)&dwClientIP;
		_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

		//ִ�д洢����
		m_GameScoreDBAide.ResetParameter();
		m_GameScoreDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
		m_GameScoreDBAide.AddParameter(TEXT("@lLoveliness"),lLoveliness);
		m_GameScoreDBAide.AddParameter(TEXT("@lRevenue"),lRevenue);
		m_GameScoreDBAide.AddParameter(TEXT("@dwPlayTimeCount"),dwPlayTimeCount);
		m_GameScoreDBAide.AddParameter(TEXT("@dwOnLineTimeCount"),dwOnLineTimeCount);
		m_GameScoreDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceAttrib->wKindID);
		m_GameScoreDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_GameScoreDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);
		return m_GameScoreDBAide.ExecuteProcess(TEXT("GSP_GR_LeaveGameServer"),false);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
	}

	return -1;
}

//���Ŵ洢����
LONG CDataBaseSink::SPCongealAccounts(DWORD dwUserID, DWORD dwMasterUserID, DWORD dwClientIP)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	try
	{
		//ת����ַ
		TCHAR szClientIP[16]=TEXT("");
		BYTE * pClientIP=(BYTE *)&dwClientIP;
		_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

		//ִ�д洢����
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@dwMasterUserID"),dwMasterUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);
		return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_CongealAccounts"),false);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
	}
	return -1;
}

//Ȩ�޴洢����
LONG CDataBaseSink::SPSetUserGameRight(DWORD dwUserID, DWORD dwUserRight, DWORD dwMasterUserID, DWORD dwClientIP)
{
	//Ч�����
	ASSERT(dwUserID!=0L);
	
	try
	{
		//ת����ַ
		TCHAR szClientIP[16]=TEXT("");
		BYTE * pClientIP=(BYTE *)&dwClientIP;
		_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

		//ִ�д洢����
		m_GameScoreDBAide.ResetParameter();
		m_GameScoreDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
		m_GameScoreDBAide.AddParameter(TEXT("@dwUserRight"),dwUserRight);
		m_GameScoreDBAide.AddParameter(TEXT("@dwMasterUserID"),dwMasterUserID);
		m_GameScoreDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);

		return m_GameScoreDBAide.ExecuteProcess(TEXT("GSP_GR_SetUserRight"),false);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
	}
	return -1;
}

//Ȩ�޴洢����
LONG CDataBaseSink::SPSetUserAccountsRight(DWORD dwUserID, DWORD dwUserRight, DWORD dwMasterUserID, DWORD dwClientIP)
{
	//Ч�����
	ASSERT(dwUserID!=0L);
	try
	{
		//ת����ַ
		TCHAR szClientIP[16]=TEXT("");
		BYTE * pClientIP=(BYTE *)&dwClientIP;
		_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

		//ִ�д洢����
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@dwUserRight"),dwUserRight);
		m_AccountsDBAide.AddParameter(TEXT("@dwMasterUserID"),dwMasterUserID);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);

		return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GR_SetUserRight"),false);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
	}
	return -1;
}

//��¼����
LONG CDataBaseSink::SPRecordGiftGrant(DWORD dwSendUserID, DWORD dwRcvUserID, WORD wGiftID, DWORD dwClientIP, 
									  DWORD dwSendUserLoveliness, DWORD dwRcvUserLoveliness, LONG lValue, int nFlowerCount)
{
	ASSERT( dwSendUserID != 0 && dwRcvUserID != 0 );
	try
	{
		//ת����ַ
		TCHAR szClientIP[16]=TEXT("");
		BYTE * pClientIP=(BYTE *)&dwClientIP;
		_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

		//ִ�д洢����
		m_GameScoreDBAide.ResetParameter();
		m_GameScoreDBAide.AddParameter(TEXT("@dwSendUserID"),dwSendUserID);
		m_GameScoreDBAide.AddParameter(TEXT("@dwRcvUserID"),dwRcvUserID);
		m_GameScoreDBAide.AddParameter(TEXT("@dwSendUserLoveliness"),dwSendUserLoveliness);
		m_GameScoreDBAide.AddParameter(TEXT("@dwRcvUserLoveliness"),dwRcvUserLoveliness);
		m_GameScoreDBAide.AddParameter(TEXT("@dwFlowerCount"),(DWORD)nFlowerCount);
		m_GameScoreDBAide.AddParameter(TEXT("@strFlowerName"),g_FlowerDescribe[wGiftID].szName);
		m_GameScoreDBAide.AddParameter(TEXT("@lFlowerPay"),lValue);
		m_GameScoreDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceAttrib->wKindID);
		m_GameScoreDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_GameScoreDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);

		return m_GameScoreDBAide.ExecuteProcess(TEXT("GSP_GR_RecordFlowerGrant"),true);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
	}
	return -1;
}

//�������
LONG CDataBaseSink::SPBuyProperty(DWORD dwUserID, DWORD dwTargetUserID, int nPropertyID, DWORD dwCurCount, DWORD dwOnceCount, 
								  DWORD dwPachurCount, LONG lPrice, DWORD dwClientIP)
{
	ASSERT( dwUserID != 0 );

	try
	{
		//ת����ַ
		TCHAR szClientIP[16]=TEXT("");
		BYTE * pClientIP=(BYTE *)&dwClientIP;
		_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

		//ִ�д洢����
		m_GameScoreDBAide.ResetParameter();
		m_GameScoreDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
		m_GameScoreDBAide.AddParameter(TEXT("@dwRcvUserID"),dwTargetUserID);
		m_GameScoreDBAide.AddParameter(TEXT("@dwKindID"),(DWORD)m_pGameServiceAttrib->wKindID);
		m_GameScoreDBAide.AddParameter(TEXT("@wServerID"),m_pGameServiceOption->wServerID);
		m_GameScoreDBAide.AddParameter(TEXT("@dwCateID"),(DWORD)nPropertyID);
		m_GameScoreDBAide.AddParameter(TEXT("@dwCurCount"),dwCurCount);	
		m_GameScoreDBAide.AddParameter(TEXT("@dwOnceCount"),dwOnceCount);	
		m_GameScoreDBAide.AddParameter(TEXT("@dwPachurseCount"),dwPachurCount);
		m_GameScoreDBAide.AddParameter(TEXT("@bwSpendScore"),lPrice);	
		m_GameScoreDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);

		return m_GameScoreDBAide.ExecuteProcess(TEXT("GSP_GR_PurchaseProp"),true);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
	}
	return -1;
}

//д�����
LONG CDataBaseSink::SPWriteProperty(DWORD dwUserID, int  nPropertyID, DWORD lUseableTime, 
									DWORD dwLeftTime, bool bUse,   
									DWORD dwClientIP)
{
	ASSERT( dwUserID != 0 );

	try
	{
		//ת����ַ
		TCHAR szClientIP[16]=TEXT("");
		BYTE * pClientIP=(BYTE *)&dwClientIP;
		_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

		//ִ�д洢����
		m_GameScoreDBAide.ResetParameter();
		m_GameScoreDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);

		//�����ж�
		if ( nPropertyID == PROP_BUGLE )
			m_GameScoreDBAide.AddParameter(TEXT("@dwKindID"),0);
		else
			m_GameScoreDBAide.AddParameter(TEXT("@dwKindID"),0); //�������

		m_GameScoreDBAide.AddParameter(TEXT("@dwCateID"),nPropertyID);
		m_GameScoreDBAide.AddParameter(TEXT("@dwPropCount"),lUseableTime);
		m_GameScoreDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);
		m_GameScoreDBAide.AddParameter(TEXT("bUse"), (int)bUse);
		m_GameScoreDBAide.AddParameter(TEXT("dwLeftTime"), dwLeftTime);

		return m_GameScoreDBAide.ExecuteProcess(TEXT("GSP_GR_WriteUserProp"),false);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
	}
	return -1;
}

//��ȡ�洢����
LONG CDataBaseSink::SPBankDrawoutGold(DWORD dwUserID, LONGLONG lSwapScore, DWORD dwClientIP)
{
	ASSERT( dwUserID != 0 );
	try
	{
		//ת����ַ
		TCHAR szClientIP[16]=TEXT("");
		BYTE * pClientIP=(BYTE *)&dwClientIP;
		_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

		//ִ�д洢����
		m_GameScoreDBAide.ResetParameter();
		m_GameScoreDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
		m_GameScoreDBAide.AddParameter(TEXT("@lSwapScore"),lSwapScore);
		m_GameScoreDBAide.AddParameter(TEXT("@dwKindID"),m_pGameServiceAttrib->wKindID);
		m_GameScoreDBAide.AddParameter(TEXT("@dwServerID"),m_pGameServiceOption->wServerID);
		m_GameScoreDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);

		return m_GameScoreDBAide.ExecuteProcess(TEXT("GSP_GR_DrawoutGameGold"),false);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
	}
	return -1;
}

//���洢����
LONG CDataBaseSink::SPBankStorageGold(DWORD dwUserID, LONGLONG lSwapScore, DWORD dwClientIP)
{
	ASSERT( dwUserID != 0 );
	try
	{
		//ת����ַ
		TCHAR szClientIP[16]=TEXT("");
		BYTE * pClientIP=(BYTE *)&dwClientIP;
		_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

		//ִ�д洢����
		m_GameScoreDBAide.ResetParameter();
		m_GameScoreDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
		m_GameScoreDBAide.AddParameter(TEXT("@lSwapScore"),lSwapScore);
		m_GameScoreDBAide.AddParameter(TEXT("@dwKindID"),m_pGameServiceAttrib->wKindID);
		m_GameScoreDBAide.AddParameter(TEXT("@dwServerID"),m_pGameServiceOption->wServerID);
		m_GameScoreDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);

		return m_GameScoreDBAide.ExecuteProcess(TEXT("GSP_GR_SavingGameGold"),false);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
	}
	return -1;
}

//������
bool CDataBaseSink::OnAffiche(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_AFFICHE_SYNC, dwContextID, NULL, 0);
	    m_AccountsDBAide.ResetParameter();
       m_AccountsDBAide.AddParameter(TEXT("@state"),  3);
		long ret = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_GetPlacard"), true);
		if (ret > 0)
		{
			WORD dwCount = 1; 
			DBR_GR_affiche  affice;
			ZeroMemory(&affice, sizeof(DBR_GR_affiche));
			//��ȡ��¼
			m_AccountsDBModule->MoveToFirst();
			while ( m_AccountsDBModule->IsRecordsetEnd() == false )
			{
				affice.dwID = dwCount++;
				m_AccountsDBAide.GetValue_String(TEXT("Context"),affice.szInfo, CountArray(affice.szInfo));
				//Ͷ�ݵ���֪ͨ
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_AFFICHE_ONE, dwContextID, &affice, sizeof(DBR_GR_affiche));
				//�ƶ���¼
				m_AccountsDBModule->MoveToNext();
			}
		}
		else
		{
			#if _SHOW_MESSAGE_NOR
					CTraceService::TraceString("û�й���", TraceLevel_Normal);
			#endif	
		}
    }
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe(); 
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
	}

	//�رռ�¼��)
	m_AccountsDBModule->CloseRecordset();
	return true;
}

//����ϲ��
bool CDataBaseSink::OnHappyMess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_HAPPY_MESS_SYNC, dwContextID, NULL, 0);
		m_AccountsDBAide.ResetParameter();
	    m_AccountsDBAide.AddParameter(TEXT("@state"),  2);
		long ret = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_GetPlacard"), true);
		if (ret > 0)
		{
			WORD dwCount = 1; 
			DBR_GR_happyMess    affice;
			ZeroMemory(&affice, sizeof(DBR_GR_happyMess));
			//��ȡ��¼
			m_AccountsDBModule->MoveToFirst();
			while ( m_AccountsDBModule->IsRecordsetEnd() == false )
			{
				affice.dwID = dwCount++;
				m_AccountsDBAide.GetValue_String(TEXT("Context"),affice.szInfo, CountArray(affice.szInfo));
				//Ͷ�ݵ���֪ͨ
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_HAPPY_MESS_ONE, dwContextID, &affice, sizeof(DBR_GR_affiche));
				//�ƶ���¼
				m_AccountsDBModule->MoveToNext();
			}
		}
		else
		{
			#if _SHOW_MESSAGE_NOR
						CTraceService::TraceString("û��ϲ��", TraceLevel_Normal);
			#endif	
		}
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe(); 
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
	}
	
	//�رռ�¼��)
	m_AccountsDBModule->CloseRecordset();
	return true;
}

//��ȡ�ȼ���Ϣ
bool CDataBaseSink::OnReadGradeInfo( DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		LONG reValue = 0;
		m_AccountsDBAide.ResetParameter();
		reValue = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_GetGradeList"),true);
		
		if (reValue > 0)
		{
			CCGlobalData::AllocGradeInfoMemory(reValue);

			int   id = 0;
			TCHAR szName[24] = {0};
			int   exp = 0;
			//��ȡ��¼
			m_AccountsDBModule->MoveToFirst();
			while ( m_AccountsDBModule->IsRecordsetEnd() == false )
			{
				int id = m_AccountsDBAide.GetValue_INT("ID");
				m_AccountsDBAide.GetValue_String(TEXT("Name"), szName, CountArray(szName));
				exp = m_AccountsDBAide.GetValue_INT("Experience");
				CCGlobalData::SetGradeInfoData(id, szName, exp);
				m_AccountsDBModule->MoveToNext();
			}
		}
		else
		{
			CTraceService::TraceString("û�еȼ���Ϣ", TraceLevel_Normal);
		}
	}
	catch( IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
	}

	//�رռ�¼��)
	m_AccountsDBModule->CloseRecordset();
	return true;
}

//�û�����
bool CDataBaseSink::OnUnLockUser(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//Ч�����
		ASSERT(wDataSize==sizeof(DWORD));
		if (wDataSize!=sizeof(DWORD)) return false;

		//ִ�в�ѯ
		DWORD userID =*((DWORD*)pData);
		m_GameScoreDBAide.ResetParameter();
		m_GameScoreDBAide.AddParameter(TEXT("@UserID"), userID);
		long ret = m_GameScoreDBAide.ExecuteProcess(TEXT("GSP_GR_DeleteUserLocker"), true);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
	}

	return true;
}

//	Author: <cxf>
//	CrateDate: <2011-12-22>
//	Description: <�������ݿ����ӳ�һֱ����>
bool CDataBaseSink::ContinueConnect()
{
	try
	{
		//ִ�д洢����
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ContinueConnect"), true);
		return true;
	}
	catch(IDataBaseException * pIException)
	{
		//�������
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
	}

	return false;
}

// ������־ָ��
void CDataBaseSink::SetRoomLog(cnd_ILogMgr*	pRoomLog)
{
	m_pRoomLog = pRoomLog;;	// ������־
}

//��������<2012-02-07>
bool CDataBaseSink::NewGiftUser(VOID * pData,WORD wDataSize)
{
	try
	{
		//ִ�д洢����
		//Ч�����
		ASSERT(wDataSize==sizeof(CMD_GF_NEW_GIFT));
		if (wDataSize!=sizeof(CMD_GF_NEW_GIFT)) return false;

		//ִ�в�ѯ
		CMD_GF_NEW_GIFT * pNewGift=(CMD_GF_NEW_GIFT *)pData;
		//ת����ַ
		TCHAR szClientIP[16]=TEXT("");
		BYTE * pClientIP=(BYTE *)&pNewGift->dwClientIP;
		_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

		m_GameScoreDBAide.ResetParameter();
		m_GameScoreDBAide.AddParameter(TEXT("@UserID"),  pNewGift->wUserID);
		m_GameScoreDBAide.AddParameter(TEXT("@Type"),  pNewGift->wGiftType);
		m_GameScoreDBAide.AddParameter(TEXT("@Gold"),  pNewGift->iGold);
		m_GameScoreDBAide.AddParameter(TEXT("@gem"),  pNewGift->iGem);
		m_GameScoreDBAide.AddParameter(TEXT("@ServerID"),m_pGameServiceOption->wServerID);
		m_GameScoreDBAide.AddParameter(TEXT("@IpAddress"),  szClientIP);
		m_GameScoreDBAide.ExecuteProcess(TEXT("GSP_GR_NewGift"), true);
		return true;
	}
	catch(IDataBaseException * pIException)
	{
		//�������
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
	}
	return false;
}

//Author<cxf>;DateTime<2012-03-02>
//Description<��ȡĳ��Ϸ����Ӯ�����ܾ���>
LONG CDataBaseSink::SPDayWinLostCount(DWORD dwUserID)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	try
	{
		//ִ�д洢����
		m_GameScoreDBAide.ResetParameter();
		m_GameScoreDBAide.AddParameter(TEXT("@wKindID"),m_pGameServiceAttrib->wKindID);
		m_GameScoreDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);

		return m_GameScoreDBAide.ExecuteProcess(TEXT("GSP_GP_DayWinLoseCount"),true);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		// add by HouGuoJiang 2012-03-27
		if(m_pRoomLog)
		{
			CString strErrorLine;
			strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );

			CString strPrompt;
			strPrompt.Format("%s ,����ԭ��%s ", strErrorLine.GetBuffer(), pszDescribe);

			m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());
		}
	}

	return -1;
}
