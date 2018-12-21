#include "StdAfx.h"
#include "Commonality.h"
#include "DataBaseSink.h"
#include "LogonServer.h"


//��������
extern CLogonServerApp theApp;
//////////////////////////////////////////////////////////////////////////

//���캯��
CDataBaseSink::CDataBaseSink()
{
	//���ñ���
	m_pInitParamter=NULL;
	m_pIDataBaseEngineEvent=NULL;

	return;
}

//��������
CDataBaseSink::~CDataBaseSink()
{
}

//�ӿڲ�ѯ
VOID * __cdecl CDataBaseSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IDataBaseEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IDataBaseEngineSink,Guid,dwQueryVer);
	return NULL;
}

//ģ������
bool __cdecl CDataBaseSink::OnDataBaseEngineStart(IUnknownEx * pIUnknownEx)
{
	//��������
	if ((m_AccountsDBModule.GetInterface()==NULL)&&(m_AccountsDBModule.CreateInstance()==false))
	{
		ASSERT(FALSE);
		return false;
	}

	try
	{
		//�����û����ݿ�
		m_AccountsDBModule->SetConnectionInfo(m_pInitParamter->m_szUserDataBaseAddr,m_pInitParamter->m_wUserDataBasePort,
			m_pInitParamter->m_szUserDataBaseName,m_pInitParamter->m_szUserDataBaseUser,m_pInitParamter->m_szUserDataBasePass);
		m_AccountsDBModule->OpenConnection();

		//��������
		m_AccountsDBModule->OpenConnection();
		m_AccountsDBAide.SetDataBase(m_AccountsDBModule.GetInterface());

		return true;
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

//ģ��ر�
bool __cdecl CDataBaseSink::OnDataBaseEngineStop(IUnknownEx * pIUnknownEx)
{
	//���ö���
	m_AccountsDBAide.SetDataBase(NULL);

	//�ر�����
	if (m_AccountsDBModule.GetInterface()!=NULL)
	{
		m_AccountsDBModule->CloseConnection();
		m_AccountsDBModule.CloseInstance();
	}

	//�������
	m_pInitParamter=NULL;
	m_pIDataBaseEngineEvent=NULL;

	return true;
}

//���ݲ�������
bool __cdecl CDataBaseSink::OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBR_GP_LOGON_BY_ACCOUNTS:		//�ʺŵ�¼
		{
			return OnRequestLogon(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOGON_BY_USERID:		//I D ��¼
		{
			return OnRequestLogon(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_REGISTER_ACCOUNTS:		//ע���ʺ�
		{
			return OnRequestLogon(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_REGISTER_ACCOUNTS_WX:		//ע���ʺ�
		{
			return OnRequestLogon(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_WXUPDATE:		//������WX�ʺ�
		{
			return OnRequestLogon(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_SIGNIN:		//ǩ��
		{
			return OnRequestUserSignIn(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_GETAWARD:		//�콱
		{
			return OnRequestUserGetAward(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_EXCHANGE_INFO:		//�һ��ĸ�����Ϣ
		{
			return OnRequestUserExchangeInfo(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_EXCHANGE_PRODUCT:		//�һ���Ʒ
		{
			return OnRequestUserExchangeProduct(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_EXCHANGE_PRODUCT_V2:		//�һ���Ʒ
		{
			return OnRequestUserExchangeProduct_V2(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_EXCHANGE:		//�һ�
		{
			return OnRequestUserExchange(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_EXCHANGE_V2:		//�һ�
		{
			return OnRequestUserExchange_V2(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_EXCHANGE_RECORD:		//�һ���¼
		{
			return OnRequestUserExchangeRecord(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_UPLOAD_FACE:		//�ϴ�ͷ��
		{
			return OnRequestUploadFace(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_DOWNLOAD_FACE:			//����ͷ��
		{
			return OnRequestDownloadFace(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_DELETE_FACE:			//ɾ����Ϣ
		{
			return OnRequestCustomFaceDelete(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_INDIVIDUAL:		//�޸�����
		{
			return OnRequestModifyIndividual(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_UNDERWRITE:		//�޸ĸ���ǩ��
		{
			return OnRequestModifyUnderWrite(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_PHONE:		    //�޸��ֻ���
		{
			return OnRequestModifyPhone(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_CHECKID:				//У������
		{
			return OnRequestCheckID(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_CHECK_NICKNAME:			//У���ǳ�
		{
			return OnRequestCheckNickName(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_REGISTER:				//ע��
		{
			return OnRequestRegister(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_ID_UPDATE:				//�ʺ�����
		{
			return OnRequestIDUpdate(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_RESET_PWD:				//��������
		{
			return OnRequestResetPwd(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_INFO:				//�û���Ϣ
		{
			return OnRequestUserInfo(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_CAN_SPREAD:				//�û��ܷ����ƹ㽱��
		{
			return OnRequestUserCanSpread(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_GET_SPREAD:				//�û����ƹ㽱��
		{
			return OnRequestUserGetSpread(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_SPREADER_INFO:				//�ƹ�Ա��Ϣ
		{
			return OnRequestSpreaderInfo(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_GET_BBS_POP:				//����������Ϣ
		{
			return OnRequestGetBbsPop(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_GET_BBS_SCROLL:			//����������Ϣ
		{
			return OnRequestGetBbsScroll(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_GET_BBS_LIST:			//�б�����Ϣ
		{
			return OnRequestGetBbsList(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_GET_TASK_INFO:			//���������Ϣ
		{
			return OnRequestGetTaskInfo(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_GET_BANKING:			//������а�
		{
			return OnRequestGetRanking(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_GET_BANKING_V2:			//������а�
		{
			return OnRequestGetRanking_V2(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_MODIFY_RANKINGSTATUS:			//����Ƿ������а�
		{
			return OnRequestModifyRankingStatus(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_FEEDBACK:			//��ҷ������
		{
			return OnRequestFeedBack(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_GET_FEEDBACK:		//��ѯ��ҷ������
		{
			return OnRequestGetFeedBack(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_INFO_ID_CARD:		//��ѯ�û����֤�˺���Ϣ
		{
			return OnRequestUserInfoIDCard(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_AUTHENTICATION:		//�����ύ���֤��Ϣ(���������֤��)
		{
			return OnRequestUserAuthentication(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_MALL_PRODUCT:	//�����̳���Ʒ��Ϣ
		{
			return OnRequestMallProduct(wRequestID, dwContextID, pData, wDataSize);
		}
	case DBR_GP_USER_MALL_PRODUCT_IOS:	//�����̳���Ʒ��Ϣ
		{
			return OnRequestMallProduct(wRequestID, dwContextID, pData, wDataSize, true);
		}
	case DBR_GP_USER_MALL_PLACE_ORDER:	//�����̳Ƕ���
		{
			return OnRequestMallPlaceOrder(wRequestID, dwContextID, pData, wDataSize);
		}
	case DBR_GP_USER_BANK_INFO:	//��ѯ���������Ϣ
		{
			return OnRequestUserBankInfo(wRequestID, dwContextID, pData, wDataSize);
		}
	case DBR_GP_USER_BANK_CHARGE:	//��Ҳ�������
		{
			return OnRequestUserBankCharge(wRequestID, dwContextID, pData, wDataSize);
		}
	case DBR_GP_USER_CANCEL_ORDER:
		{
			return OnRequestUserCancelOrder(wRequestID, dwContextID, pData, wDataSize);
		}
	case DBR_GP_USER_MALL_BUY:
		{
			return OnRequestMallBuy(wRequestID, dwContextID, pData, wDataSize);
		}
	case DBR_GP_USER_MALL_UPDATE:
		{
			return OnRequestMallUpdateBuy(wRequestID, dwContextID, pData, wDataSize);
		}
	case DBR_GP_USER_ACTIVITY_LIST:  //��ѯ��б�
		{
			return OnRequestActivityList(wRequestID, dwContextID, pData, wDataSize);
		}
	case DBR_GP_USER_ACTIVITY:  //��ѯ�
		{
			return OnRequestActivity(wRequestID, dwContextID, pData, wDataSize);
		}
	case DBR_GP_USER_ACTIVITY_LUCKY:  //��齱
		{
			return OnRequestActivityLucky(wRequestID, dwContextID, pData, wDataSize);
		}
	case DBR_GP_USER_ACTIVITY_RECORD:		//�н���¼
		{
			return OnRequestUserActivityRecord(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_SPEAKER_SEND:		//������
		{
			return OnRequestUserSpeakerSend(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_USER_QUERY_CONFIG:		//��ѯ������
		{
			return OnRequestUserQueryConfig(wRequestID,dwContextID,pData,wDataSize);
		}
	}

	return false;
}

//��¼������
bool CDataBaseSink::OnRequestLogon(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//��������
		LONG lReturnValue=-1;
		LPCTSTR pszPassword=NULL;

		BOOL bIsRegisterAnony = FALSE;

		//��¼����
		switch (wRequestID)
		{
		case DBR_GP_LOGON_BY_ACCOUNTS:		//�ʺŵ�¼
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(DBR_GP_LogonByAccounts));
				if (wDataSize!=sizeof(DBR_GP_LogonByAccounts)) return false;

				//ִ�в�ѯ
				DBR_GP_LogonByAccounts * pLogonByAccounts=(DBR_GP_LogonByAccounts *)pData;
				lReturnValue=SPLogonByAccounts(pLogonByAccounts->szAccounts,pLogonByAccounts->szPassWord,pLogonByAccounts->dwClientIP,pLogonByAccounts->szComputerID);
				pszPassword=pLogonByAccounts->szPassWord;

				break;
			}
		case DBR_GP_LOGON_BY_USERID:		//I D ��¼
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(DBR_GP_LogonByUserID));
				if (wDataSize!=sizeof(DBR_GP_LogonByUserID)) return false;

				//ִ�в�ѯ
				DBR_GP_LogonByUserID * pLogonByUserID=(DBR_GP_LogonByUserID *)pData;
				lReturnValue=SPLogonByUserID(pLogonByUserID->dwUserID,pLogonByUserID->szPassWord,pLogonByUserID->dwClientIP,pLogonByUserID->szComputerID);
				pszPassword=pLogonByUserID->szPassWord;

				break;
			}
		case DBR_GP_REGISTER_ACCOUNTS:		//ע���ʺ�
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(DBR_GP_RegisterAccounts));
				if (wDataSize!=sizeof(DBR_GP_RegisterAccounts)) return false;

				//ִ�в�ѯ
				DBR_GP_RegisterAccounts * pRegisterAccounts=(DBR_GP_RegisterAccounts *)pData;
				lReturnValue=SPRegisterAccounts(pRegisterAccounts->szAccounts,pRegisterAccounts->szPassWord,pRegisterAccounts->szSpreader,
					pRegisterAccounts->wFaceID,pRegisterAccounts->cbGender,pRegisterAccounts->dwClientIP,pRegisterAccounts->szComputerID);
				pszPassword=pRegisterAccounts->szPassWord;

				if (std::string(pRegisterAccounts->szAccounts).size() == 0)
				{
					bIsRegisterAnony = TRUE;
				}

				break;
			}
		case DBR_GP_REGISTER_ACCOUNTS_WX:		//ע���ʺ�
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(DBR_GP_RegisterAccountsWX));
				if (wDataSize!=sizeof(DBR_GP_RegisterAccountsWX)) return false;

				//ִ�в�ѯ
				DBR_GP_RegisterAccountsWX * pRegisterAccounts=(DBR_GP_RegisterAccountsWX *)pData;
				lReturnValue=SPRegisterAccountsWX(pRegisterAccounts->szAccounts,pRegisterAccounts->szPassWord,pRegisterAccounts->szSpreader,
					pRegisterAccounts->szFaceUrl,pRegisterAccounts->cbGender,pRegisterAccounts->dwClientIP,pRegisterAccounts->szWXComputerID,pRegisterAccounts->szComputerID,
					pRegisterAccounts->szNickName, pRegisterAccounts->szOpenID, pRegisterAccounts->szUnionID);
				pszPassword=pRegisterAccounts->szPassWord;

				if (std::string(pRegisterAccounts->szAccounts).size() == 0)
				{
					bIsRegisterAnony = TRUE;
				}

				break;
			}
		case DBR_GP_WXUPDATE:		//������WX�ʺ�
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(DBR_GP_WXUpdate));
				if (wDataSize!=sizeof(DBR_GP_WXUpdate)) return false;

				//ִ�в�ѯ
				DBR_GP_WXUpdate * pR=(DBR_GP_WXUpdate *)pData;
				lReturnValue=SPWXUpdate(pR->dwUserID,pR->szFaceUrl,pR->cbGender,pR->dwClientIP,pR->szWXComputerID,pR->szComputerID,pR->szNickName, pR->szOpenID, pR->szUnionID);
				bIsRegisterAnony = TRUE;

				break;
			}
		default :	//Ĭ�ϴ���
			{
				ASSERT(FALSE); 
				return false;
			}
		}

		//����ж�
		if (lReturnValue==0L)
		{
			//��¼�ɹ�
			DBR_GP_LogonSuccess LogonSuccess;
			ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));

			//��ȡ����
			LogonSuccess.wFaceID=m_AccountsDBAide.GetValue_WORD(TEXT("FaceID"));
			LogonSuccess.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			LogonSuccess.dwGameID=m_AccountsDBAide.GetValue_DWORD(TEXT("GameID"));
			LogonSuccess.cbGender=m_AccountsDBAide.GetValue_INT(TEXT("Gender"));
			LogonSuccess.cbMember=m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));
			LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			try{ 
				//ֻ���˺ŵ�½ʱ������ֶ�
				LogonSuccess.lGoldScore=m_AccountsDBAide.GetValue_LONG(TEXT("GoldScore"));
				LogonSuccess.lWelfare=m_AccountsDBAide.GetValue_LONG(TEXT("Welfare"));
				LogonSuccess.wWelfareLost=m_AccountsDBAide.GetValue_WORD(TEXT("WelfareLost"));
			}
			catch(IDataBaseException * pIException){
				//�������--���Դ˴���
	//			LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
	//			CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
			}

			LogonSuccess.dwCustomFaceVer=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomFaceVer"));
			LogonSuccess.cbIsGuest=m_AccountsDBAide.GetValue_INT(TEXT("IsGuest"));
			LogonSuccess.cbHasGuest=m_AccountsDBAide.GetValue_INT(TEXT("HasGuest"));
			m_AccountsDBAide.GetValue_String(TEXT("Accounts"),LogonSuccess.szAccounts,CountArray(LogonSuccess.szAccounts));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),LogonSuccess.szNickName,CountArray(LogonSuccess.szNickName));
			m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"),LogonSuccess.szUnderWrite,CountArray(LogonSuccess.szUnderWrite));
			m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),LogonSuccess.szDescribeString,CountArray(LogonSuccess.szDescribeString));
			m_AccountsDBAide.GetValue_String(TEXT("LobbyRule"),LogonSuccess.szRule,CountArray(LogonSuccess.szRule));

			//��������
			if(pszPassword)CopyMemory(LogonSuccess.szPassword,pszPassword,CountArray(LogonSuccess.szPassword));		
			///�ο�
			if (bIsRegisterAnony)
			{
				try{ 
					//ֻ���ο��˺�ע��ʱ������ֶ�
					m_AccountsDBAide.GetValue_String(TEXT("GuestPassword"),LogonSuccess.szPassword,CountArray(LogonSuccess.szPassword));
				}
				catch(IDataBaseException * pIException){
					//�������
					LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
					CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
				}
			}

			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_LOGON_SUCCESS,dwContextID,&LogonSuccess,sizeof(LogonSuccess));
		}
		else
		{
			//��¼ʧ��
			DBR_GP_LogonError LogonError;
			ZeroMemory(&LogonError,sizeof(LogonError));

			//���ñ���
			LogonError.lErrorCode=lReturnValue;
			m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),LogonError.szErrorDescribe,CountArray(LogonError.szErrorDescribe));

			//Ͷ�ݽ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_LOGON_ERROR,dwContextID,&LogonError,sizeof(LogonError));
		}
	}
	catch (IDataBaseException * pIException)
	{
		//��¼ʧ��
		DBR_GP_LogonError LogonError;
		ZeroMemory(&LogonError,sizeof(LogonError));

		//�������
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		//����ʧ��
		LogonError.lErrorCode=-1;
		lstrcpyn(LogonError.szErrorDescribe,TEXT("�������ݿ�����쳣�������Ժ����Ի�ѡ����һ��������¼��"),CountArray(LogonError.szErrorDescribe));

		//Ͷ�ݽ��
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_LOGON_ERROR,dwContextID,&LogonError,sizeof(LogonError));
	}

	//�رռ�¼��
	m_AccountsDBModule->CloseRecordset();

	return true;
}

//ǩ��
bool CDataBaseSink::OnRequestUserSignIn(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserSignIn));
	if (wDataSize!=sizeof(DBR_GP_UserSignIn)) return false;

	//����ת��
	DBR_GP_UserSignIn *pSignIn = (DBR_GP_UserSignIn*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserSignIn(pSignIn->dwUserID);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_UserSignInResult SignInResult;
		ZeroMemory(&SignInResult, sizeof(SignInResult));
		lstrcpyn(SignInResult.szDescribeMsg, TEXT("ǩ���ɹ���"), sizeof(SignInResult.szDescribeMsg));
		SignInResult.bOperateSuccess = true;
		SignInResult.cbTodaySign = 1;
		SignInResult.wTimes = m_AccountsDBAide.GetValue_WORD(TEXT("Times"));
		SignInResult.dwGoldScore = m_AccountsDBAide.GetValue_DWORD(TEXT("GoldScore"));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_SIGNIN_RESULT,dwContextID,
			&SignInResult,sizeof(SignInResult));
	}
	else
	{
		DBR_GP_UserSignInResult SignInResult;
		ZeroMemory(&SignInResult, sizeof(SignInResult));
		lstrcpyn(SignInResult.szDescribeMsg, TEXT("�������ݿ�����쳣��ǩ��ʧ�ܣ�"), sizeof(SignInResult.szDescribeMsg));
		SignInResult.bOperateSuccess = false;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_SIGNIN_RESULT,dwContextID,
			&SignInResult,sizeof(SignInResult));
	}

	return true;
}

//�콱
bool CDataBaseSink::OnRequestUserGetAward(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserGetAward));
	if (wDataSize!=sizeof(DBR_GP_UserGetAward)) return false;

	//����ת��
	DBR_GP_UserGetAward *pAward = (DBR_GP_UserGetAward*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserGetAward(pAward->dwUserID, pAward->dwTaskID);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_UserGetAwardResult GetAwardResult;
		ZeroMemory(&GetAwardResult, sizeof(GetAwardResult));
		lstrcpyn(GetAwardResult.szDescribeMsg, TEXT("�콱�ɹ���"), sizeof(GetAwardResult.szDescribeMsg));
		GetAwardResult.cbStatus = m_AccountsDBAide.GetValue_WORD(TEXT("Status"));
		GetAwardResult.dwTaskID = m_AccountsDBAide.GetValue_WORD(TEXT("TaskID"));
		//GetAwardResult.dwScore = m_AccountsDBAide.GetValue_WORD(TEXT("Score"));
		GetAwardResult.dwScore = m_AccountsDBAide.GetValue_DWORD(TEXT("Score"));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GETAWARD_RESULT,dwContextID,
			&GetAwardResult,sizeof(GetAwardResult));
	}
	else
	{
		DBR_GP_UserGetAwardResult GetAwardResult;
		ZeroMemory(&GetAwardResult, sizeof(GetAwardResult));
		lstrcpyn(GetAwardResult.szDescribeMsg, TEXT("�������ݿ�����쳣���콱��ʧ�ܣ�"), sizeof(GetAwardResult.szDescribeMsg));
		GetAwardResult.cbStatus = lReturnValue;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GETAWARD_RESULT,dwContextID,
			&GetAwardResult,sizeof(GetAwardResult));
	}

	return true;
}

//�һ��ĸ�����Ϣ
bool CDataBaseSink::OnRequestUserExchangeInfo(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserExchangeInfo));
	if (wDataSize!=sizeof(DBR_GP_UserExchangeInfo)) return false;

	//����ת��
	DBR_GP_UserExchangeInfo *pExchangeInfo = (DBR_GP_UserExchangeInfo*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserExchangeInfo(pExchangeInfo->dwUserID, pExchangeInfo->szPhone);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_UserExchangeInfoResult ExchangeInfoResult;
		ZeroMemory(&ExchangeInfoResult, sizeof(ExchangeInfoResult));
		ExchangeInfoResult.bOperateSuccess = true;
		ExchangeInfoResult.dwGold = m_AccountsDBAide.GetValue_DWORD(TEXT("Gold"));
		m_AccountsDBAide.GetValue_String(TEXT("Phone"),ExchangeInfoResult.szPhone,CountArray(ExchangeInfoResult.szPhone));
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_EXCHANGE_INFO_RESULT,dwContextID,
			&ExchangeInfoResult,sizeof(ExchangeInfoResult));
	}
	else
	{
		DBR_GP_UserExchangeInfoResult ExchangeInfoResult;
		ZeroMemory(&ExchangeInfoResult, sizeof(ExchangeInfoResult));
		ExchangeInfoResult.bOperateSuccess = false;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_EXCHANGE_INFO_RESULT,dwContextID,
			&ExchangeInfoResult,sizeof(ExchangeInfoResult));
	}

	return true;
}

//�һ���Ʒ
bool CDataBaseSink::OnRequestUserExchangeProduct(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserExchangeProduct));
	if (wDataSize!=sizeof(DBR_GP_UserExchangeProduct)) return false;

	//����ת��
	DBR_GP_UserExchangeProduct *pExchangeProduct = (DBR_GP_UserExchangeProduct*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserExchangeProduct(pExchangeProduct->dwUserID);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_UserExchangeProductResult ExchangeProductResult;
		ZeroMemory(&ExchangeProductResult, sizeof(ExchangeProductResult));
		ExchangeProductResult.bOperateSuccess = true;

		//��ȡ����
		IDataBase * pIDataBase=(IDataBase *)m_AccountsDBAide.GetDataBase(IID_IDataBase,VER_IDataBase);

		//��ȡ��Ϣ
		while (pIDataBase->IsRecordsetEnd()==false)
		{
			DBR_GP_ExchangeProduct product;
			//��ȡ��Ϣ
			product.dwAwardID=m_AccountsDBAide.GetValue_DWORD(TEXT("AwardID"));
			product.dwPrice=m_AccountsDBAide.GetValue_DWORD(TEXT("Price"));
			product.dwLeft=m_AccountsDBAide.GetValue_DWORD(TEXT("Lefts"));
			m_AccountsDBAide.GetValue_String(TEXT("AwardImg"),product.szAwardImg,CountArray(product.szAwardImg));
			m_AccountsDBAide.GetValue_String(TEXT("AwardName"),product.szAwardname,CountArray(product.szAwardname));

			//�ƶ���¼
			pIDataBase->MoveToNext();

			//������Ϣ
			ExchangeProductResult.vecProducts.push_back(product);
		}

		//�رռ�¼
		pIDataBase->CloseRecordset();

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_EXCHANGE_PRODUCT_RESULT,dwContextID,
			&ExchangeProductResult,sizeof(ExchangeProductResult));
	}
	else
	{
		DBR_GP_UserExchangeProductResult ExchangeProductResult;
		ZeroMemory(&ExchangeProductResult, sizeof(ExchangeProductResult));
		ExchangeProductResult.bOperateSuccess = false;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_EXCHANGE_PRODUCT_RESULT,dwContextID,
			&ExchangeProductResult,sizeof(ExchangeProductResult));
	}

	return true;
}


//�һ���Ʒ
bool CDataBaseSink::OnRequestUserExchangeProduct_V2(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserExchangeProduct));
	if (wDataSize!=sizeof(DBR_GP_UserExchangeProduct)) return false;

	//����ת��
	DBR_GP_UserExchangeProduct *pExchangeProduct = (DBR_GP_UserExchangeProduct*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserExchangeProduct(pExchangeProduct->dwUserID);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_UserExchangeProductResult ExchangeProductResult;
		ZeroMemory(&ExchangeProductResult, sizeof(ExchangeProductResult));
		ExchangeProductResult.bOperateSuccess = true;

		//��ȡ����
		IDataBase * pIDataBase=(IDataBase *)m_AccountsDBAide.GetDataBase(IID_IDataBase,VER_IDataBase);

		//��ȡ��Ϣ
		while (pIDataBase->IsRecordsetEnd()==false)
		{
			DBR_GP_ExchangeProduct product;
			//��ȡ��Ϣ
			product.dwAwardID=m_AccountsDBAide.GetValue_DWORD(TEXT("AwardID"));
			product.dwPrice=m_AccountsDBAide.GetValue_DWORD(TEXT("Price"));
			product.dwLeft=m_AccountsDBAide.GetValue_DWORD(TEXT("Lefts"));
			product.dwType = m_AccountsDBAide.GetValue_INT(TEXT("Type"));
			m_AccountsDBAide.GetValue_String(TEXT("AwardImg"),product.szAwardImg,CountArray(product.szAwardImg));
			m_AccountsDBAide.GetValue_String(TEXT("AwardName"),product.szAwardname,CountArray(product.szAwardname));

			//�ƶ���¼
			pIDataBase->MoveToNext();

			//������Ϣ
			ExchangeProductResult.vecProducts.push_back(product);
		}

		//�رռ�¼
		pIDataBase->CloseRecordset();

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_EXCHANGE_PRODUCT_RESULT_V2,dwContextID,
			&ExchangeProductResult,sizeof(ExchangeProductResult));
	}
	else
	{
		DBR_GP_UserExchangeProductResult ExchangeProductResult;
		ZeroMemory(&ExchangeProductResult, sizeof(ExchangeProductResult));
		ExchangeProductResult.bOperateSuccess = false;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_EXCHANGE_PRODUCT_RESULT_V2,dwContextID,
			&ExchangeProductResult,sizeof(ExchangeProductResult));
	}

	return true;
}

//�һ�
bool CDataBaseSink::OnRequestUserExchange(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserExchange));
	if (wDataSize!=sizeof(DBR_GP_UserExchange)) return false;

	//����ת��
	DBR_GP_UserExchange *pExchange = (DBR_GP_UserExchange*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserExchange(pExchange->dwUserID, pExchange->dwAwardID);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_UserExchangeResult ExchangeResult;
		ZeroMemory(&ExchangeResult, sizeof(ExchangeResult));
		ExchangeResult.bOperateSuccess = true;
		ExchangeResult.dwGold = m_AccountsDBAide.GetValue_DWORD(TEXT("Gold"));
		ExchangeResult.dwAwardID = m_AccountsDBAide.GetValue_DWORD(TEXT("AwardID"));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_EXCHANGE_RESULT,dwContextID,
			&ExchangeResult,sizeof(ExchangeResult));
	}
	else
	{
		DBR_GP_UserExchangeResult ExchangeResult;
		ZeroMemory(&ExchangeResult, sizeof(ExchangeResult));
		ExchangeResult.bOperateSuccess = false;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_EXCHANGE_RESULT,dwContextID,
			&ExchangeResult,sizeof(ExchangeResult));
	}

	return true;
}

//�һ�
bool CDataBaseSink::OnRequestUserExchange_V2(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserExchange));
	if (wDataSize!=sizeof(DBR_GP_UserExchange)) return false;

	//����ת��
	DBR_GP_UserExchange *pExchange = (DBR_GP_UserExchange*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserExchange_V2(pExchange->dwUserID, pExchange->dwAwardID);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_UserExchangeResult ExchangeResult;
		ZeroMemory(&ExchangeResult, sizeof(ExchangeResult));
		ExchangeResult.bOperateSuccess = true;
		ExchangeResult.dwGold = m_AccountsDBAide.GetValue_DWORD(TEXT("Gold"));
		ExchangeResult.dwAwardID = m_AccountsDBAide.GetValue_DWORD(TEXT("AwardID"));
		ExchangeResult.dwScore = m_AccountsDBAide.GetValue_DWORD(TEXT("TotalScore"));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_EXCHANGE_RESULT_V2,dwContextID,
			&ExchangeResult,sizeof(ExchangeResult));
	}
	else
	{
		DBR_GP_UserExchangeResult ExchangeResult;
		ZeroMemory(&ExchangeResult, sizeof(ExchangeResult));
		ExchangeResult.bOperateSuccess = false;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_EXCHANGE_RESULT_V2,dwContextID,
			&ExchangeResult,sizeof(ExchangeResult));
	}

	return true;
}

//�һ���¼
bool CDataBaseSink::OnRequestUserExchangeRecord(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserExchangeRecord));
	if (wDataSize!=sizeof(DBR_GP_UserExchangeRecord)) return false;

	//����ת��
	DBR_GP_UserExchangeRecord *pExchangeRecord = (DBR_GP_UserExchangeRecord*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		if (pExchangeRecord->nPage < 1)pExchangeRecord->nPage = 1;
		lReturnValue = SPUserExchangeRecord(pExchangeRecord->dwUserID, pExchangeRecord->nPage - 1);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_UserExchangeRecordResult ExchangeRecordResult;
		ExchangeRecordResult.bOperateSuccess = true;

		//��ȡ����
		IDataBase * pIDataBase=(IDataBase *)m_AccountsDBAide.GetDataBase(IID_IDataBase,VER_IDataBase);

		//��ȡ��Ϣ
		while (pIDataBase->IsRecordsetEnd()==false)
		{
			DBR_GP_ExchangeRecord record;
			ZeroMemory(&record, sizeof(record));

			//��ȡ��Ϣ
			record.dwTotal=m_AccountsDBAide.GetValue_DWORD(TEXT("T"));
			record.dwID=m_AccountsDBAide.GetValue_DWORD(TEXT("ID"));
			record.dwAwardID=m_AccountsDBAide.GetValue_DWORD(TEXT("AwardID"));
	//		record.dwDate=m_AccountsDBAide.GetValue_DWORD(TEXT("ExchangeDate"));
			m_AccountsDBAide.GetValue_String(TEXT("D"),record.szDate,CountArray(record.szDate));
			m_AccountsDBAide.GetValue_String(TEXT("AwardName"),record.szAwardName,CountArray(record.szAwardName));
			m_AccountsDBAide.GetValue_String(TEXT("Status"),record.szStatus,CountArray(record.szStatus));

			//�ƶ���¼
			pIDataBase->MoveToNext();

			//������Ϣ
			ExchangeRecordResult.vecRecords.push_back(record);
		}

		//�رռ�¼
		pIDataBase->CloseRecordset();

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_EXCHANGE_RECORD_RESULT,dwContextID,
			&ExchangeRecordResult,sizeof(ExchangeRecordResult));
	}
	else
	{
		DBR_GP_UserExchangeRecordResult ExchangeRecordResult;
		ZeroMemory(&ExchangeRecordResult, sizeof(ExchangeRecordResult));
		ExchangeRecordResult.bOperateSuccess = false;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_EXCHANGE_RECORD_RESULT,dwContextID,
			&ExchangeRecordResult,sizeof(ExchangeRecordResult));
	}

	return true;
}


//�ϴ�ͷ��
bool CDataBaseSink::OnRequestUploadFace(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UploadCustomFace));
	if (wDataSize!=sizeof(DBR_GP_UploadCustomFace)) return false;

	//����ת��
	DBR_GP_UploadCustomFace *pUploadCustomFace = (DBR_GP_UploadCustomFace*)pData;

	//�ļ�����
	CFile fileCustomFace;
	TCHAR szFileName[128];
	_snprintf(szFileName, CountArray(szFileName), TEXT("%s\\UploadFile_%ld.zip"), theApp.m_szDirWork, pUploadCustomFace->dwUserID);

	try
	{
		if ( fileCustomFace.Open( szFileName, CFile::modeRead|CFile::typeBinary) )
		{
			BYTE cbCustomFaceData[1024*30];

			//��ȡ�ļ�
			UINT uReadSize = fileCustomFace.Read(cbCustomFaceData, CountArray(cbCustomFaceData));

			//�ر��ļ�
			fileCustomFace.Close();

			LONG lRet=-1;
			try
			{
				lRet = SPUploadFace(pUploadCustomFace->dwUserID, cbCustomFaceData, uReadSize);
			}
			catch (IDataBaseException * pIException)
			{
				//������Ϣ
				LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
				CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
			}

			//�ɹ��ж�
			if ( lRet != -1 )
			{
				DBR_GP_UploadFaceResult UploadFaceResult;
				ZeroMemory(&UploadFaceResult, sizeof(UploadFaceResult));
				lstrcpyn(UploadFaceResult.szDescribeMsg, TEXT("ͷ���ϴ��ɹ���"), sizeof(UploadFaceResult.szDescribeMsg));
				UploadFaceResult.dwFaceVer=lRet;
				UploadFaceResult.bOperateSuccess=true;

				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_UPLOAD_FACE_RESULT,dwContextID,
					&UploadFaceResult,sizeof(UploadFaceResult));
			}
			else
			{
				//����ʧ��
				DBR_GP_UploadFaceResult UploadFaceResult;
				ZeroMemory(&UploadFaceResult, sizeof(UploadFaceResult));
				lstrcpyn(UploadFaceResult.szDescribeMsg, TEXT("�������ݿ�����쳣���������ϴ���"), sizeof(UploadFaceResult.szDescribeMsg));
				UploadFaceResult.bOperateSuccess=false;

				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_UPLOAD_FACE_RESULT,dwContextID,
					&UploadFaceResult,sizeof(UploadFaceResult));
			}

			//ɾ���ļ�
			CFile::Remove(szFileName);
		}
	}catch(...){}	

	return true;
}

//����ͷ��
bool CDataBaseSink::OnRequestDownloadFace(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//У�����
	ASSERT(wDataSize == sizeof(DBR_GP_DownloadCustomFace));
	if ( wDataSize != sizeof(DBR_GP_DownloadCustomFace)) return false;

	//����ת��
	DBR_GP_DownloadCustomFace *pDownloadCustomFace = (DBR_GP_DownloadCustomFace*)pData;

	LONG lReturnValue=-1;
	long lSize=0;
	bool bRet = false;
	try
	{
		lReturnValue=SPDownloadFace(pDownloadCustomFace->dwUserID);

		//��ȡ����
		if ( lReturnValue==0 )
		{	
			DWORD dwImageSize=0;
			dwImageSize=m_AccountsDBAide.GetValue_DWORD(TEXT("ImgSize"));
			
			//��ȡ����
			IDataBase * pIDataBase=m_AccountsDBModule.GetInterface();
			_variant_t vtFld;
			pIDataBase->GetRecordsetValue(TEXT("CustomFaceImage"), vtFld);
			BYTE * pcbBuf=NULL;
			SafeArrayAccessData(vtFld.parray,(LPVOID *)&pcbBuf);

			//Ͷ������
			DBR_GP_DownloadFaceSuccess DownloadFaceSuccess;
			ZeroMemory(&DownloadFaceSuccess, sizeof(DownloadFaceSuccess));
			DownloadFaceSuccess.dwToltalSize = dwImageSize;
			DownloadFaceSuccess.dwUserID = pDownloadCustomFace->dwUserID;
			DWORD dwSendSize = 0;
			while( dwSendSize <dwImageSize )
			{
				//��������
				if ( 1024 < dwImageSize - dwSendSize ) DownloadFaceSuccess.dwCurrentSize = 1024;
				else DownloadFaceSuccess.dwCurrentSize = dwImageSize - dwSendSize ;
				CopyMemory(DownloadFaceSuccess.bFaceData, pcbBuf+dwSendSize, DownloadFaceSuccess.dwCurrentSize);

				//Ͷ����Ϣ
				WORD wPostSize = WORD(sizeof(DownloadFaceSuccess)-sizeof(DownloadFaceSuccess.bFaceData)+DownloadFaceSuccess.dwCurrentSize);
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_DOWNLOADFACE_SUCCESS, dwContextID, &DownloadFaceSuccess, wPostSize);

				//��������
				dwSendSize += DownloadFaceSuccess.dwCurrentSize;
			}

			//�ͷ���Դ
			SafeArrayUnaccessData(vtFld.parray);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}


	return true;
}

//ɾ����Ϣ
bool CDataBaseSink::OnRequestCustomFaceDelete(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize == sizeof(DBR_GP_CustomFaceDelete));
	if ( wDataSize != sizeof(DBR_GP_CustomFaceDelete)) return false;

	//����ת��
	DBR_GP_CustomFaceDelete *pCustomFaceDelete = (DBR_GP_CustomFaceDelete*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPDeleteCustomFace(pCustomFaceDelete->dwUserID);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_DeleteFaceResult DeleteFaceResult;
		ZeroMemory(&DeleteFaceResult, sizeof(DeleteFaceResult));
		lstrcpyn(DeleteFaceResult.szDescribeMsg, TEXT("ͷ��ɾ���ɹ���"), sizeof(DeleteFaceResult.szDescribeMsg));
		DeleteFaceResult.dwFaceVer=pCustomFaceDelete->dwFaceVer;
		DeleteFaceResult.bOperateSuccess = true;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_DELETE_FACE_RESULT,dwContextID,
			&DeleteFaceResult,sizeof(DeleteFaceResult));
	}
	else
	{
		DBR_GP_DeleteFaceResult DeleteFaceResult;
		ZeroMemory(&DeleteFaceResult, sizeof(DeleteFaceResult));
		lstrcpyn(DeleteFaceResult.szDescribeMsg, TEXT("�������ݿ�����쳣��ɾ��ʧ�ܣ�"), sizeof(DeleteFaceResult.szDescribeMsg));
		DeleteFaceResult.dwFaceVer=pCustomFaceDelete->dwFaceVer;
		DeleteFaceResult.bOperateSuccess = false;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_DELETE_FACE_RESULT,dwContextID,
			&DeleteFaceResult,sizeof(DeleteFaceResult));
	}

	return true;
}

//�޸�����
bool CDataBaseSink::OnRequestModifyIndividual(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize == sizeof(DBR_GP_ModifyIndividual));
	if ( wDataSize != sizeof(DBR_GP_ModifyIndividual)) return false;

	//����ת��
	DBR_GP_ModifyIndividual *pModifyIndividual = (DBR_GP_ModifyIndividual*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPModifyIndividual(pModifyIndividual->szAccount, pModifyIndividual->szNickname, pModifyIndividual->szPhone, pModifyIndividual->nGender);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_ModifyIndividualResult ModifyIndividualResult;
		ZeroMemory(&ModifyIndividualResult, sizeof(ModifyIndividualResult));
	//	lstrcpyn(ModifyIndividualResult.szDescribeMsg, TEXT("�����޸ĳɹ���"), sizeof(ModifyIndividualResult.szDescribeMsg));
		m_AccountsDBAide.GetValue_String(TEXT("Accounts"),ModifyIndividualResult.szAccount,CountArray(ModifyIndividualResult.szAccount));
		m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),ModifyIndividualResult.szDescribeMsg,CountArray(ModifyIndividualResult.szDescribeMsg));
		ModifyIndividualResult.iCode = 0;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_MODIFY_RESULT,dwContextID,
			&ModifyIndividualResult,sizeof(ModifyIndividualResult));
	}
	else
	{
		DBR_GP_ModifyIndividualResult ModifyIndividualResult;
		ZeroMemory(&ModifyIndividualResult, sizeof(ModifyIndividualResult));
	//	lstrcpyn(ModifyIndividualResult.szDescribeMsg, TEXT("�������ݿ�����쳣���޸�ʧ�ܣ�"), sizeof(ModifyIndividualResult.szDescribeMsg));
		m_AccountsDBAide.GetValue_String(TEXT("Accounts"),ModifyIndividualResult.szAccount,CountArray(ModifyIndividualResult.szAccount));
		m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),ModifyIndividualResult.szDescribeMsg,CountArray(ModifyIndividualResult.szDescribeMsg));
		ModifyIndividualResult.iCode = lReturnValue;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_MODIFY_RESULT,dwContextID,
			&ModifyIndividualResult,sizeof(ModifyIndividualResult));
	}
	return true;
}

//�޸ĸ���ǩ��
bool CDataBaseSink::OnRequestModifyUnderWrite(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize == sizeof(DBR_GP_ModifyUnderWrite));
	if ( wDataSize != sizeof(DBR_GP_ModifyUnderWrite)) return false;

	//����ת��
	DBR_GP_ModifyUnderWrite *pModifyUnderWrite = (DBR_GP_ModifyUnderWrite*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPModifyUnderWrite(pModifyUnderWrite->dwUserID, pModifyUnderWrite->szDescribe);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_ModifyUnderWriteResult ModifyUnderWriteResult;
		ZeroMemory(&ModifyUnderWriteResult, sizeof(ModifyUnderWriteResult));
	//	lstrcpyn(ModifyUnderWriteResult.szDescribeMsg, TEXT("����ǩ���޸ĳɹ���"), sizeof(ModifyUnderWriteResult.szDescribeMsg));
		m_AccountsDBAide.GetValue_String(TEXT("Accounts"),ModifyUnderWriteResult.szAccount,CountArray(ModifyUnderWriteResult.szAccount));
		m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),ModifyUnderWriteResult.szDescribeMsg,CountArray(ModifyUnderWriteResult.szDescribeMsg));
		ModifyUnderWriteResult.iCode = 0;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_MODIFY_UNDERWRITE_RESULT,dwContextID,
			&ModifyUnderWriteResult,sizeof(ModifyUnderWriteResult));
	}
	else
	{
		DBR_GP_ModifyUnderWriteResult ModifyUnderWriteResult;
		ZeroMemory(&ModifyUnderWriteResult, sizeof(ModifyUnderWriteResult));
	//	lstrcpyn(ModifyUnderWriteResult.szDescribeMsg, TEXT("�������ݿ�����쳣���޸�ʧ�ܣ�"), sizeof(ModifyUnderWriteResult.szDescribeMsg));
		//m_AccountsDBAide.GetValue_String(TEXT("Accounts"),ModifyUnderWriteResult.szAccount,CountArray(ModifyUnderWriteResult.szAccount));
		m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),ModifyUnderWriteResult.szDescribeMsg,CountArray(ModifyUnderWriteResult.szDescribeMsg));
		ModifyUnderWriteResult.iCode = lReturnValue;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_MODIFY_UNDERWRITE_RESULT,dwContextID,
			&ModifyUnderWriteResult,sizeof(ModifyUnderWriteResult));
	}
	return true;
}

//�޸��ֻ���
bool CDataBaseSink::OnRequestModifyPhone(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize == sizeof(DBR_GP_ModifyPhone));
	if ( wDataSize != sizeof(DBR_GP_ModifyPhone)) return false;

	//����ת��
	DBR_GP_ModifyPhone *pModifyPhone = (DBR_GP_ModifyPhone*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPModifyPhone(pModifyPhone->dwUserID, pModifyPhone->szPhone);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_ModifyPhoneResult ModifyPhoneResult;
		ZeroMemory(&ModifyPhoneResult, sizeof(ModifyPhoneResult));
	//	lstrcpyn(ModifyPhoneResult.szDescribeMsg, TEXT("�ֻ����޸ĳɹ���"), sizeof(ModifyPhoneResult.szDescribeMsg));
		m_AccountsDBAide.GetValue_String(TEXT("Accounts"),ModifyPhoneResult.szAccount,CountArray(ModifyPhoneResult.szAccount));
		m_AccountsDBAide.GetValue_String(TEXT("Phone"),ModifyPhoneResult.szPhone,CountArray(ModifyPhoneResult.szPhone));
		m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),ModifyPhoneResult.szDescribeMsg,CountArray(ModifyPhoneResult.szDescribeMsg));
		ModifyPhoneResult.iCode = 0;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_MODIFY_PHONE_RESULT,dwContextID,
			&ModifyPhoneResult,sizeof(ModifyPhoneResult));
	}
	else
	{
		DBR_GP_ModifyPhoneResult ModifyPhoneResult;
		ZeroMemory(&ModifyPhoneResult, sizeof(ModifyPhoneResult));
	//	lstrcpyn(ModifyPhoneResult.szDescribeMsg, TEXT("�������ݿ�����쳣���޸�ʧ�ܣ�"), sizeof(ModifyPhoneResult.szDescribeMsg));
		m_AccountsDBAide.GetValue_String(TEXT("Accounts"),ModifyPhoneResult.szAccount,CountArray(ModifyPhoneResult.szAccount));
		m_AccountsDBAide.GetValue_String(TEXT("Phone"),ModifyPhoneResult.szPhone,CountArray(ModifyPhoneResult.szPhone));
		m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),ModifyPhoneResult.szDescribeMsg,CountArray(ModifyPhoneResult.szDescribeMsg));
		ModifyPhoneResult.iCode = lReturnValue;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_MODIFY_PHONE_RESULT,dwContextID,
			&ModifyPhoneResult,sizeof(ModifyPhoneResult));
	}
	return true;
}

//У������
bool CDataBaseSink::OnRequestCheckID(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize == sizeof(DBR_GP_CheckID));
	if ( wDataSize != sizeof(DBR_GP_CheckID)) return false;

	//����ת��
	DBR_GP_CheckID *pReq = (DBR_GP_CheckID*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPCheckID(pReq->szAccount);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_CommonResult result;
		ZeroMemory(&result, sizeof(result));
		//lstrcpyn(result.szDescribeMsg, TEXT("У�����Ƴɹ���"), sizeof(result.szDescribeMsg));
		m_AccountsDBAide.GetValue_String(TEXT("Accounts"),result.szAccount,CountArray(result.szAccount));
		m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),result.szDescribeMsg,CountArray(result.szDescribeMsg));
		result.iCode = 0;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_CHECKID_RESULT,dwContextID,
			&result,sizeof(DBR_GP_CommonResult));
	}
	else
	{
		DBR_GP_CommonResult result;
		ZeroMemory(&result, sizeof(result));
		//lstrcpyn(result.szDescribeMsg, TEXT("�������ݿ�����쳣��У��ʧ�ܣ�"), sizeof(result.szDescribeMsg));
		
		m_AccountsDBAide.GetValue_String(TEXT("Accounts"),result.szAccount,CountArray(result.szAccount));
		m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),result.szDescribeMsg,CountArray(result.szDescribeMsg));
		result.iCode = lReturnValue;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_CHECKID_RESULT,dwContextID,
			&result,sizeof(DBR_GP_CommonResult));
	}
	return true;
}

//У���ǳ�	
bool CDataBaseSink::OnRequestCheckNickName(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize == sizeof(DBR_GP_CheckNickName));
	if ( wDataSize != sizeof(DBR_GP_CheckNickName)) return false;

	//����ת��
	DBR_GP_CheckNickName *pReq = (DBR_GP_CheckNickName*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPCheckNickName(pReq->szNickName);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_CommonResult result;
		ZeroMemory(&result, sizeof(result));
	//	lstrcpyn(result.szDescribeMsg, TEXT("У���ǳƳɹ���"), sizeof(result.szDescribeMsg));
		int sz = CountArray(result.szAccount);
		m_AccountsDBAide.GetValue_String(TEXT("Accounts"),result.szAccount,CountArray(result.szAccount));
		m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),result.szDescribeMsg,CountArray(result.szDescribeMsg));
		result.iCode = 0;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_CHECK_NICKNAME_RESULT,dwContextID,
			&result,sizeof(DBR_GP_CommonResult));
	}
	else
	{
		DBR_GP_CommonResult result;
		ZeroMemory(&result, sizeof(result));
	//	lstrcpyn(result.szDescribeMsg, TEXT("�������ݿ�����쳣��У��ʧ�ܣ�"), sizeof(result.szDescribeMsg));
		m_AccountsDBAide.GetValue_String(TEXT("Accounts"),result.szAccount,CountArray(result.szAccount));
		m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),result.szDescribeMsg,CountArray(result.szDescribeMsg));
		result.iCode = lReturnValue;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_CHECK_NICKNAME_RESULT,dwContextID,
			&result,sizeof(DBR_GP_CommonResult));
	}
	return true;
}

//ע��	
bool CDataBaseSink::OnRequestRegister(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize == sizeof(DBR_GP_Register));
	if ( wDataSize != sizeof(DBR_GP_Register)) return false;

	//����ת��
	DBR_GP_Register *pReq = (DBR_GP_Register*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPRegister(pReq->szAccount,pReq->szNickName,pReq->szPhone,pReq->nGender, pReq->szPassword, 
			pReq->dwClientIP,pReq->szComputerID, pReq->szChannel);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_CommonResult result;
		ZeroMemory(&result, sizeof(result));
//		lstrcpyn(result.szDescribeMsg, TEXT("ע��ɹ���"), sizeof(result.szDescribeMsg));
		m_AccountsDBAide.GetValue_String(TEXT("Accounts"),result.szAccount,CountArray(result.szAccount));
		m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),result.szDescribeMsg,CountArray(result.szDescribeMsg));
		result.iCode = 0;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_REGISTER_RESULT,dwContextID,
			&result,sizeof(DBR_GP_CommonResult));
	}
	else
	{
		DBR_GP_CommonResult result;
		ZeroMemory(&result, sizeof(result));
	//	lstrcpyn(result.szDescribeMsg, TEXT("�������ݿ�����쳣��ע��ʧ�ܣ�"), sizeof(result.szDescribeMsg));
		m_AccountsDBAide.GetValue_String(TEXT("Accounts"),result.szAccount,CountArray(result.szAccount));
		m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),result.szDescribeMsg,CountArray(result.szDescribeMsg));
		result.iCode = lReturnValue;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_REGISTER_RESULT,dwContextID,
			&result,sizeof(DBR_GP_CommonResult));
	}
	return true;
}

//�ʺ�����	
bool CDataBaseSink::OnRequestIDUpdate(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize == sizeof(DBR_GP_IDUpdate));
	if ( wDataSize != sizeof(DBR_GP_IDUpdate)) return false;

	//����ת��
	DBR_GP_IDUpdate *pReq = (DBR_GP_IDUpdate*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPIDUpdate(pReq->szAccountOld,pReq->szAccount, pReq->szNickName,pReq->nGender, pReq->szPassword);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_CommonResult result;
		ZeroMemory(&result, sizeof(result));
	//	lstrcpyn(result.szDescribeMsg, TEXT("�����ɹ���"), sizeof(result.szDescribeMsg));
		m_AccountsDBAide.GetValue_String(TEXT("Accounts"),result.szAccount,CountArray(result.szAccount));
		m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),result.szDescribeMsg,CountArray(result.szDescribeMsg));
		result.iCode = 0;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_ID_UPDATE_RESULT,dwContextID,
			&result,sizeof(DBR_GP_CommonResult));
	}
	else
	{
		DBR_GP_CommonResult result;
		ZeroMemory(&result, sizeof(result));
//		lstrcpyn(result.szDescribeMsg, TEXT("�������ݿ�����쳣������ʧ�ܣ�"), sizeof(result.szDescribeMsg));
		m_AccountsDBAide.GetValue_String(TEXT("Accounts"),result.szAccount,CountArray(result.szAccount));
		m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),result.szDescribeMsg,CountArray(result.szDescribeMsg));
		result.iCode = lReturnValue;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_ID_UPDATE_RESULT,dwContextID,
			&result,sizeof(DBR_GP_CommonResult));
	}
	return true;
}

//��������	
bool CDataBaseSink::OnRequestResetPwd(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize == sizeof(DBR_GP_ResetPwd));
	if ( wDataSize != sizeof(DBR_GP_ResetPwd)) return false;

	//����ת��
	DBR_GP_ResetPwd *pReq = (DBR_GP_ResetPwd*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPResetPwd(pReq->szAccount,pReq->szPassword);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_CommonResult result;
		ZeroMemory(&result, sizeof(result));
	//	lstrcpyn(result.szDescribeMsg, TEXT("��������ɹ���"), sizeof(result.szDescribeMsg));
		m_AccountsDBAide.GetValue_String(TEXT("Accounts"),result.szAccount,CountArray(result.szAccount));
		m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),result.szDescribeMsg,CountArray(result.szDescribeMsg));
		result.iCode = 0;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_RESET_PWD_RESULT,dwContextID,
			&result,sizeof(DBR_GP_CommonResult));
	}
	else
	{
		DBR_GP_CommonResult result;
		ZeroMemory(&result, sizeof(result));
//		lstrcpyn(result.szDescribeMsg, TEXT("�������ݿ�����쳣����������ʧ�ܣ�"), sizeof(result.szDescribeMsg));
		m_AccountsDBAide.GetValue_String(TEXT("Accounts"),result.szAccount,CountArray(result.szAccount));
		m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),result.szDescribeMsg,CountArray(result.szDescribeMsg));
		result.iCode = lReturnValue;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_RESET_PWD_RESULT,dwContextID,
			&result,sizeof(DBR_GP_CommonResult));
	}
	return true;
}

//�û���Ϣ	
bool CDataBaseSink::OnRequestUserInfo(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize == sizeof(DBR_GP_UserInfo));
	if ( wDataSize != sizeof(DBR_GP_UserInfo)) return false;

	//����ת��
	DBR_GP_UserInfo *pReq = (DBR_GP_UserInfo*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserInfo(pReq->szAccount);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_UserInfo_Result result;
		ZeroMemory(&result, sizeof(result));
		
	//	m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),result.szDescribeMsg,CountArray(result.szDescribeMsg));
		
		m_AccountsDBAide.GetValue_String(TEXT("Accounts"),result.szAccount,CountArray(result.szAccount));
		m_AccountsDBAide.GetValue_String(TEXT("NickName"),result.szNickName,CountArray(result.szNickName));
		m_AccountsDBAide.GetValue_String(TEXT("Phone"),result.szPhone,CountArray(result.szPhone));
		m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),result.szDescribeMsg,CountArray(result.szDescribeMsg));
		result.nGender = m_AccountsDBAide.GetValue_INT(TEXT("Gender"));

		result.iCode = lReturnValue;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_INFO_RESULT,dwContextID,
			&result,sizeof(DBR_GP_UserInfo_Result));
	}
	else
	{
		DBR_GP_UserInfo_Result result;
		ZeroMemory(&result, sizeof(result));
//		lstrcpyn(result.szDescribeMsg, TEXT("�������ݿ�����쳣����ѯʧ�ܣ�"), sizeof(result.szDescribeMsg));	
		m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),result.szDescribeMsg,CountArray(result.szDescribeMsg));
		result.iCode = lReturnValue;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_INFO_RESULT,dwContextID,
			&result,sizeof(DBR_GP_UserInfo_Result));
	}
	return true;
}

//�û��ܷ���ȡ�ƹ㽱��	
bool CDataBaseSink::OnRequestUserCanSpread(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize == sizeof(DBR_GP_UserCanSpread));
	if ( wDataSize != sizeof(DBR_GP_UserCanSpread)) return false;

	//����ת��
	DBR_GP_UserCanSpread *pReq = (DBR_GP_UserCanSpread*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserCanSpread(pReq->dwUserID, pReq->szComputerID);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_CommonResult result;
		ZeroMemory(&result, sizeof(result));
		
		m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),result.szDescribeMsg,CountArray(result.szDescribeMsg));

		result.iCode = lReturnValue;
		result.dwUserID = m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_CAN_SPREAD_RESULT,dwContextID,
			&result,sizeof(DBR_GP_CommonResult));
	}
	else
	{
		DBR_GP_CommonResult result;
		ZeroMemory(&result, sizeof(result));
		m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),result.szDescribeMsg,CountArray(result.szDescribeMsg));
		result.iCode = lReturnValue;
		result.dwUserID = m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_CAN_SPREAD_RESULT,dwContextID,
			&result,sizeof(DBR_GP_CommonResult));
	}
	return true;
}


//�û���ȡ�ƹ㽱��	
bool CDataBaseSink::OnRequestUserGetSpread(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize == sizeof(DBR_GP_UserGetSpread));
	if ( wDataSize != sizeof(DBR_GP_UserGetSpread)) return false;

	//����ת��
	DBR_GP_UserGetSpread *pReq = (DBR_GP_UserGetSpread*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserGetSpread(pReq->dwUserID, pReq->dwSpreaderID, pReq->szComputerID);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_UserGetSpreadResult result;
		ZeroMemory(&result, sizeof(result));
		
		m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),result.szDescribeMsg,CountArray(result.szDescribeMsg));

		result.iCode = lReturnValue;
		result.dwUserID = m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
		result.dwSpreaderID = m_AccountsDBAide.GetValue_DWORD(TEXT("Spreader"));
		result.dwAmount = m_AccountsDBAide.GetValue_DWORD(TEXT("Amount"));
		result.dwType = m_AccountsDBAide.GetValue_DWORD(TEXT("Type"));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_GET_SPREAD_RESULT,dwContextID,
			&result,sizeof(DBR_GP_UserGetSpreadResult));
	}
	else
	{
		DBR_GP_UserGetSpreadResult result;
		ZeroMemory(&result, sizeof(result));
		m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),result.szDescribeMsg,CountArray(result.szDescribeMsg));
		result.iCode = lReturnValue;
		result.dwUserID = m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_GET_SPREAD_RESULT,dwContextID,
			&result,sizeof(DBR_GP_UserGetSpreadResult));
	}
	return true;
}

//�ƹ�Ա��Ϣ	
bool CDataBaseSink::OnRequestSpreaderInfo(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize == sizeof(DBR_GP_UserSpreaderInfo));
	if ( wDataSize != sizeof(DBR_GP_UserSpreaderInfo)) return false;

	//����ת��
	DBR_GP_UserSpreaderInfo *pReq = (DBR_GP_UserSpreaderInfo*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserGetSpreadInfo(pReq->dwSpreaderID);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_UserSpreaderInfoResult result;
		ZeroMemory(&result, sizeof(result));

		//��ȡ����
		IDataBase * pIDataBase=(IDataBase *)m_AccountsDBAide.GetDataBase(IID_IDataBase,VER_IDataBase);

		//��ȡ��Ϣ
		while (pIDataBase->IsRecordsetEnd()==false)
		{
			result.dwSpreaderID = m_AccountsDBAide.GetValue_DWORD(TEXT("S")); //�ƹ�Ա
			result.dwAmount = m_AccountsDBAide.GetValue_DWORD(TEXT("C"));  //�ܽ�����
			result.dwUsers = m_AccountsDBAide.GetValue_DWORD(TEXT("T"));   //������
			
			if (result.dwNum < 10)  //���10��
			{
				m_AccountsDBAide.GetValue_String(TEXT("N"),result.vecNickNames[result.dwNum],CountArray(result.vecNickNames[result.dwNum]));
				result.iAward[result.dwNum] = m_AccountsDBAide.GetValue_DWORD(TEXT("M"));   //������
				result.dwNum++;  //����
			}
			else
			{
				break;
			}

			//�ƶ���¼
			pIDataBase->MoveToNext();
		}

		//�رռ�¼
		pIDataBase->CloseRecordset();

		result.iCode = lReturnValue;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_SPREADER_INFO_RESULT,dwContextID,
			&result,sizeof(DBR_GP_UserSpreaderInfoResult));
	}
	else
	{
		DBR_GP_UserSpreaderInfoResult result;
		ZeroMemory(&result, sizeof(result));
		m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),result.szDescribeMsg,CountArray(result.szDescribeMsg));
		result.iCode = lReturnValue;
		result.dwSpreaderID = m_AccountsDBAide.GetValue_DWORD(TEXT("S"));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_SPREADER_INFO_RESULT,dwContextID,
			&result,sizeof(DBR_GP_UserSpreaderInfoResult));
	}
	return true;
}

//����������Ϣ	
bool CDataBaseSink::OnRequestGetBbsPop(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize == sizeof(DBR_GP_UserGetBbsPop));
	if ( wDataSize != sizeof(DBR_GP_UserGetBbsPop)) return false;
	
	//����ת��
	DBR_GP_UserGetBbsPop *pReq = (DBR_GP_UserGetBbsPop*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserGetBbsPop(pReq->szChannel);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_UserGetBbsPopResult result;
		ZeroMemory(&result, sizeof(result));

		//��ȡ����
		IDataBase * pIDataBase=(IDataBase *)m_AccountsDBAide.GetDataBase(IID_IDataBase,VER_IDataBase);

		//��ȡ��Ϣ
		m_AccountsDBAide.GetValue_String(TEXT("Details"),result.szDetails,CountArray(result.szDetails));
		result.iAction = m_AccountsDBAide.GetValue_WORD(TEXT("Action")); 
		result.iScrollCount = m_AccountsDBAide.GetValue_DWORD(TEXT("ScrollCount"));  //������������
		result.iListCount = m_AccountsDBAide.GetValue_DWORD(TEXT("ListCount"));		 //�б�������
		
		//���3��
		if (result.iListCount > 3)result.iListCount=3;

		for (int i=0; i<result.iListCount; i++)
		{
			char buf[32];
			memset(buf, 0, 32);
			sprintf(buf, "List%d", i+1);
			result.dwListID[i] = m_AccountsDBAide.GetValue_DWORD(buf);		 //�б���ID
		}

		result.iCode = lReturnValue;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_GET_BBS_POP_RESULT,dwContextID,
			&result,sizeof(DBR_GP_UserGetBbsPopResult));
	}
	else
	{
		DBR_GP_UserGetBbsPopResult result;
		ZeroMemory(&result, sizeof(result));
		result.iCode = lReturnValue;
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_GET_BBS_POP_RESULT,dwContextID,
			&result,sizeof(DBR_GP_UserGetBbsPopResult));
	}
	return true;
}

//����������Ϣ	
bool CDataBaseSink::OnRequestGetBbsScroll(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize == sizeof(DBR_GP_UserGetBbsScroll));
	if ( wDataSize != sizeof(DBR_GP_UserGetBbsScroll)) return false;
	
	//����ת��
	DBR_GP_UserGetBbsPop *pReq = (DBR_GP_UserGetBbsPop*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserGetBbsScroll(pReq->szChannel);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_UserGetBbsScrollResult result;
		ZeroMemory(&result, sizeof(result));

		//��ȡ����
		IDataBase * pIDataBase=(IDataBase *)m_AccountsDBAide.GetDataBase(IID_IDataBase,VER_IDataBase);

		//��ȡ��Ϣ
		int iCnt = 0;
		while (pIDataBase->IsRecordsetEnd()==false)
		{
			m_AccountsDBAide.GetValue_String(TEXT("Title"),result.szDetails[iCnt],CountArray(result.szDetails[iCnt]));

			iCnt++;

			//�ƶ���¼
			pIDataBase->MoveToNext();
		}

		//�رռ�¼
		pIDataBase->CloseRecordset();

		result.iCount = iCnt;
		result.iCode = lReturnValue;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_GET_BBS_SCROLL_RESULT,dwContextID,
			&result,sizeof(DBR_GP_UserGetBbsScrollResult));
	}
	else
	{
		DBR_GP_UserGetBbsScrollResult result;
		ZeroMemory(&result, sizeof(result));
		result.iCode = lReturnValue;
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_GET_BBS_SCROLL_RESULT,dwContextID,
			&result,sizeof(DBR_GP_UserGetBbsScrollResult));
	}
	return true;
}

//�б�����Ϣ	
bool CDataBaseSink::OnRequestGetBbsList(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize == sizeof(DBR_GP_UserGetBbsList));
	if ( wDataSize != sizeof(DBR_GP_UserGetBbsList)) return false;
	
	//����ת��
	DBR_GP_UserGetBbsPop *pReq = (DBR_GP_UserGetBbsPop*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserGetBbsList(pReq->szChannel);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_UserGetBbsListResult result;
		ZeroMemory(&result, sizeof(result));

		//��ȡ����
		IDataBase * pIDataBase=(IDataBase *)m_AccountsDBAide.GetDataBase(IID_IDataBase,VER_IDataBase);

		//��ȡ��Ϣ
		int iCnt = 0;
		while (pIDataBase->IsRecordsetEnd()==false)
		{
			m_AccountsDBAide.GetValue_String(TEXT("Title"),result.szTitle[iCnt],CountArray(result.szTitle[iCnt]));
			m_AccountsDBAide.GetValue_String(TEXT("Details"),result.szDetails[iCnt],CountArray(result.szDetails[iCnt]));
			m_AccountsDBAide.GetValue_String(TEXT("D"),result.szDate[iCnt], CountArray(result.szDate[iCnt]));  //����
			
			result.iType[iCnt] = m_AccountsDBAide.GetValue_WORD(TEXT("Action")); //��������:��GM�����Э��
			result.iID[iCnt] = m_AccountsDBAide.GetValue_DWORD(TEXT("ID"));  
			iCnt++;

			//�ƶ���¼
			pIDataBase->MoveToNext();
		}

		//�رռ�¼
		pIDataBase->CloseRecordset();

		result.iCount = iCnt;
		result.iCode = lReturnValue;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_GET_BBS_LIST_RESULT,dwContextID,
			&result,sizeof(DBR_GP_UserGetBbsListResult));
	}
	else
	{
		DBR_GP_UserGetBbsListResult result;
		ZeroMemory(&result, sizeof(result));
		result.iCode = lReturnValue;
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_GET_BBS_LIST_RESULT,dwContextID,
			&result,sizeof(DBR_GP_UserGetBbsListResult));
	}
	return true;
}

//���������Ϣ	
bool CDataBaseSink::OnRequestGetTaskInfo(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize == sizeof(DBR_GP_UserGetTaskInfo));
	if ( wDataSize != sizeof(DBR_GP_UserGetTaskInfo)) return false;
	
	//����ת��
	DBR_GP_UserGetTaskInfo *pReq = (DBR_GP_UserGetTaskInfo*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserGetTaskInfo(pReq->dwUserID);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_UserGetTaskInfoResult result;
		ZeroMemory(&result, sizeof(result));

		//��ȡ����
		IDataBase * pIDataBase=(IDataBase *)m_AccountsDBAide.GetDataBase(IID_IDataBase,VER_IDataBase);

		//��ȡ��Ϣ
		result.dwUserID = pReq->dwUserID;
		result.iWinToday3 = m_AccountsDBAide.GetValue_DWORD(TEXT("wtoday3"));
		result.iLostToday3 = m_AccountsDBAide.GetValue_DWORD(TEXT("ltoday3"));
		result.iWinTotal3 = m_AccountsDBAide.GetValue_DWORD(TEXT("wtotal3"));
		result.iLostTotal3 = m_AccountsDBAide.GetValue_DWORD(TEXT("ltotal3"));
		result.iAwardToday3 = m_AccountsDBAide.GetValue_DWORD(TEXT("atoday3"));
		result.iAwardTotal3 = m_AccountsDBAide.GetValue_DWORD(TEXT("atotal3"));
		result.iWinToday4 = m_AccountsDBAide.GetValue_DWORD(TEXT("wtoday4"));
		result.iLostToday4 = m_AccountsDBAide.GetValue_DWORD(TEXT("ltoday4"));
		result.iWinTotal4 = m_AccountsDBAide.GetValue_DWORD(TEXT("wtotal4"));
		result.iLostTotal4 = m_AccountsDBAide.GetValue_DWORD(TEXT("ltotal4"));
		result.iAwardToday4 = m_AccountsDBAide.GetValue_DWORD(TEXT("atoday4"));
		result.iAwardTotal4 = m_AccountsDBAide.GetValue_DWORD(TEXT("atotal4"));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_GET_TASK_INFO_RESULT,dwContextID,
			&result,sizeof(DBR_GP_UserGetTaskInfoResult));
	}
	else
	{
		DBR_GP_UserGetTaskInfoResult result;
		ZeroMemory(&result, sizeof(result));
		result.iCode = lReturnValue;
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_GET_TASK_INFO_RESULT,dwContextID,
			&result,sizeof(DBR_GP_UserGetTaskInfoResult));
	}
	return true;
}

//������а�
bool CDataBaseSink::OnRequestGetRanking(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserGetRanking));
	if (wDataSize!=sizeof(DBR_GP_UserGetRanking)) return false;

	//����ת��
	DBR_GP_UserGetRanking *pReq = (DBR_GP_UserGetRanking*)pData;

		//ִ�д洢����
	LONG lReturnValue2 = -1;
	try
	{
		lReturnValue2 = SPUserRankingStatus(pReq->dwUserID);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue2 == 0 )
	{
		DBR_GP_UserGetRankingStatusResult RankingStatusResult;
		ZeroMemory(&RankingStatusResult, sizeof(RankingStatusResult));

		//��ȡ����
		IDataBase * pIDataBase=(IDataBase *)m_AccountsDBAide.GetDataBase(IID_IDataBase,VER_IDataBase);

		int iRankingStatus = -1;
		
		//��ȡ��Ϣ
		iRankingStatus = m_AccountsDBAide.GetValue_INT(TEXT("RS"));
			
		//�رռ�¼
		pIDataBase->CloseRecordset();

		RankingStatusResult.dwUserID = pReq->dwUserID;
		RankingStatusResult.isInRanking = iRankingStatus;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_GET_RANKINGSTATUS,dwContextID,
			&RankingStatusResult,sizeof(DBR_GP_UserGetRankingStatusResult));
	}
	else
	{
		DBR_GP_UserGetRankingStatusResult RankingStatusResult;
		ZeroMemory(&RankingStatusResult, sizeof(RankingStatusResult));
		RankingStatusResult.dwUserID = pReq->dwUserID;
		RankingStatusResult.isInRanking = -1;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_GET_RANKINGSTATUS,dwContextID,
			&RankingStatusResult,sizeof(RankingStatusResult));
	}

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		if (pReq->nPage < 1)pReq->nPage = 1;
		lReturnValue = SPUserRanking(pReq->dwUserID, pReq->nPage - 1);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_UserGetRankingResult result;
		ZeroMemory(&result, sizeof(result));

		

		//��ȡ����
		IDataBase * pIDataBase=(IDataBase *)m_AccountsDBAide.GetDataBase(IID_IDataBase,VER_IDataBase);

		//��ȡ��Ϣ
		int iTotal = 0;
		int iMine = 0;
		
		while (pIDataBase->IsRecordsetEnd()==false)
		{
			DBR_GP_UserRankingRecord record;
			ZeroMemory(&record, sizeof(record));

			//��ȡ��Ϣ
			iTotal = m_AccountsDBAide.GetValue_DWORD(TEXT("T"));
			iMine = m_AccountsDBAide.GetValue_INT(TEXT("Mine"));
			record.nIndex=m_AccountsDBAide.GetValue_DWORD(TEXT("ID"));
			record.lGoldScore=m_AccountsDBAide.GetValue_LONG(TEXT("Score"));
			record.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			record.cbGender=m_AccountsDBAide.GetValue_INT(TEXT("Gender"));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),record.szNickName,CountArray(record.szNickName));
			m_AccountsDBAide.GetValue_String(TEXT("D"),result.szDate,CountArray(result.szDate));

			//�ƶ���¼
			pIDataBase->MoveToNext();

			//������Ϣ
			result.vecRecords.push_back(record);
		}

		//�رռ�¼
		pIDataBase->CloseRecordset();

		result.dwUserID = pReq->dwUserID;
		result.dwTotal = iTotal;
		result.nMyIndex = iMine;
		
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_GET_RANKING_RESULT,dwContextID,
				&result,sizeof(DBR_GP_UserGetRankingResult));
	}
	else
	{
		DBR_GP_UserGetRankingResult result;
		ZeroMemory(&result, sizeof(result));
		result.iCode = lReturnValue;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_GET_RANKING_RESULT,dwContextID,
			&result,sizeof(result));
	}



	return true;
}

//������а�
bool CDataBaseSink::OnRequestGetRanking_V2(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserGetRanking));
	if (wDataSize!=sizeof(DBR_GP_UserGetRanking)) return false;

	//����ת��
	DBR_GP_UserGetRanking *pReq = (DBR_GP_UserGetRanking*)pData;

		//ִ�д洢����
	LONG lReturnValue2 = -1;
	try
	{
		lReturnValue2 = SPUserRankingStatus(pReq->dwUserID);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue2 == 0 )
	{
		DBR_GP_UserGetRankingStatusResult RankingStatusResult;
		ZeroMemory(&RankingStatusResult, sizeof(RankingStatusResult));

		//��ȡ����
		IDataBase * pIDataBase=(IDataBase *)m_AccountsDBAide.GetDataBase(IID_IDataBase,VER_IDataBase);

		int iRankingStatus = -1;

		//��ȡ��Ϣ
		iRankingStatus = m_AccountsDBAide.GetValue_INT(TEXT("RS"));

		//�رռ�¼
		pIDataBase->CloseRecordset();

		RankingStatusResult.dwUserID = pReq->dwUserID;
		RankingStatusResult.isInRanking = iRankingStatus;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_GET_RANKINGSTATUS,dwContextID,
			&RankingStatusResult,sizeof(DBR_GP_UserGetRankingStatusResult));
	}
	else
	{
		DBR_GP_UserGetRankingStatusResult RankingStatusResult;
		ZeroMemory(&RankingStatusResult, sizeof(RankingStatusResult));
		RankingStatusResult.dwUserID = pReq->dwUserID;
		RankingStatusResult.isInRanking = -1;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_GET_RANKINGSTATUS,dwContextID,
			&RankingStatusResult,sizeof(RankingStatusResult));
	}

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		if (pReq->nPage < 1)pReq->nPage = 1;
		lReturnValue = SPUserRanking_V2(pReq->dwUserID, pReq->nPage - 1);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_UserGetRankingResult_V2 result;
		ZeroMemory(&result, sizeof(result));



		//��ȡ����
		IDataBase * pIDataBase=(IDataBase *)m_AccountsDBAide.GetDataBase(IID_IDataBase,VER_IDataBase);

		//��ȡ��Ϣ
		int iTotal = 0;
		int iMine = 0;

		while (pIDataBase->IsRecordsetEnd()==false)
		{
			DBR_GP_UserRankingRecord_V2 record;
			ZeroMemory(&record, sizeof(record));

			//��ȡ��Ϣ
			iTotal = m_AccountsDBAide.GetValue_DWORD(TEXT("T"));
			iMine = m_AccountsDBAide.GetValue_INT(TEXT("Mine"));
			record.nIndex=m_AccountsDBAide.GetValue_DWORD(TEXT("ID"));
			record.lGoldScore=m_AccountsDBAide.GetValue_LONG(TEXT("Score"));
			record.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			record.cbGender=m_AccountsDBAide.GetValue_INT(TEXT("Gender"));
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),record.szNickName,CountArray(record.szNickName));
			m_AccountsDBAide.GetValue_String(TEXT("DescribeInfo"),record.szDescribe,CountArray(record.szDescribe));
			m_AccountsDBAide.GetValue_String(TEXT("D"),result.szDate,CountArray(result.szDate));

			//�ƶ���¼
			pIDataBase->MoveToNext();

			//������Ϣ
			result.vecRecords.push_back(record);
		}

		//�رռ�¼
		pIDataBase->CloseRecordset();

		result.dwUserID = pReq->dwUserID;
		result.dwTotal = iTotal;
		result.nMyIndex = iMine;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_GET_RANKING_RESULT_V2,dwContextID,
			&result,sizeof(DBR_GP_UserGetRankingResult_V2));
	}
	else
	{
		DBR_GP_UserGetRankingResult result;
		ZeroMemory(&result, sizeof(result));
		result.iCode = lReturnValue;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_GET_RANKING_RESULT_V2,dwContextID,
			&result,sizeof(result));
	}

	return true;
}

//�޸�����Ƿ������а�
bool CDataBaseSink::OnRequestModifyRankingStatus(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserModifyRankingStatus));
	if (wDataSize!=sizeof(DBR_GP_UserModifyRankingStatus)) return false;

	//����ת��
	DBR_GP_UserModifyRankingStatus *pReq = (DBR_GP_UserModifyRankingStatus*)pData;

	DBR_GP_UserModifyRankingStatusResult result;
	ZeroMemory(&result, sizeof(result));
	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserModifyRankingStatus(pReq->dwUserID, pReq->isInRanking);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	if(0 == lReturnValue)
	{
		result.iCode = pReq->isInRanking;
	}
	else
	{
		result.iCode = lReturnValue;
	}
	
	result.dwUserID = pReq->dwUserID;
	m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_GET_MODIFY_RANKINGSTATUS,dwContextID,
		&result,sizeof(result));

	return true;
}

//��ҷ������
bool CDataBaseSink::OnRequestFeedBack(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserFeedBack));
	if (wDataSize!=sizeof(DBR_GP_UserFeedBack)) return false;

	//����ת��
	DBR_GP_UserFeedBack *pReq = (DBR_GP_UserFeedBack*)pData;
	
	DBR_GP_UserFeedBackResult result;
	ZeroMemory(&result, sizeof(result));

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserFeedBack(pReq->dwUserID, pReq->szContent, pReq->szPicture);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);

		lstrcpyn(result.szDetails,TEXT("���ݿ�����쳣�������Ժ����ԣ�"),CountArray(result.szDetails));
	}
	result.iCode = lReturnValue;

	m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_FEEDBACK_RESULT,dwContextID,
		&result,sizeof(result));

	return true;
}

//��ѯ��ҷ��������
bool CDataBaseSink::OnRequestGetFeedBack(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserGetFeedBack));
	if (wDataSize!=sizeof(DBR_GP_UserGetFeedBack)) return false;

	//����ת��
	DBR_GP_UserGetFeedBack *pReq = (DBR_GP_UserGetFeedBack*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		if (pReq->nPage < 1)pReq->nPage = 1;
		lReturnValue = SPUserGetFeedBack(pReq->dwUserID, pReq->nPage - 1);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_UserGetFeedBackResult result;
		ZeroMemory(&result, sizeof(result));

		//��ȡ����
		IDataBase * pIDataBase=(IDataBase *)m_AccountsDBAide.GetDataBase(IID_IDataBase,VER_IDataBase);

		//��ȡ��Ϣ
		int iTotal = 0;
		while (pIDataBase->IsRecordsetEnd()==false)
		{
			DBR_GP_UserFeedBackRecord record;
			ZeroMemory(&record, sizeof(record));

			//��ȡ��Ϣ
			iTotal = m_AccountsDBAide.GetValue_DWORD(TEXT("T"));
			m_AccountsDBAide.GetValue_String(TEXT("Question"),record.szQuestion,CountArray(record.szQuestion));
			m_AccountsDBAide.GetValue_String(TEXT("Reply"),record.szAnswer,CountArray(record.szAnswer));
			m_AccountsDBAide.GetValue_String(TEXT("AttachPath"),record.szPicture,CountArray(record.szPicture));
			m_AccountsDBAide.GetValue_String(TEXT("D"),record.szDate,CountArray(record.szDate));

			if (std::strlen(record.szPicture)>0)record.cbHavePic = 1;

			//�ƶ���¼
			pIDataBase->MoveToNext();

			//������Ϣ
			result.vecRecords.push_back(record);
		}

		//�رռ�¼
		pIDataBase->CloseRecordset();

		result.dwUserID = pReq->dwUserID;
		result.dwTotal = iTotal;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_GET_FEEDBACK_RESULT,dwContextID,
			&result,sizeof(DBR_GP_UserGetFeedBackResult));
	}
	else
	{
		DBR_GP_UserGetFeedBackResult result;
		ZeroMemory(&result, sizeof(result));
		result.iCode = lReturnValue;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_GET_FEEDBACK_RESULT,dwContextID,
			&result,sizeof(result));
	}

	return true;
}

//��ѯ�û����֤�˺���Ϣ
bool CDataBaseSink::OnRequestUserInfoIDCard(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserInfoIDCard));
	if (wDataSize!=sizeof(DBR_GP_UserInfoIDCard)) return false;

	//����ת��
	DBR_GP_UserInfoIDCard *pReq = (DBR_GP_UserInfoIDCard*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserInfoIDCard(pReq->dwUserID);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_CommonResult result;
		ZeroMemory(&result, sizeof(result));

		//��ȡ����
		IDataBase * pIDataBase=(IDataBase *)m_AccountsDBAide.GetDataBase(IID_IDataBase,VER_IDataBase);
		
		//��ȡ��Ϣ
		result.dwUserID = pReq->dwUserID;
		result.iCode = lReturnValue;
		m_AccountsDBAide.GetValue_String(TEXT("IDCard"),result.szDescribeMsg,CountArray(result.szDescribeMsg));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_INFO_ID_CARD_RESULT,dwContextID,
			&result,sizeof(DBR_GP_CommonResult));
	}
	else
	{
		DBR_GP_CommonResult result;
		ZeroMemory(&result, sizeof(result));
		
		//��ȡ��Ϣ
		result.dwUserID = pReq->dwUserID;
		result.iCode = lReturnValue;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_INFO_ID_CARD_RESULT,dwContextID,
			&result,sizeof(result));
	}

	return true;
}

//�����ύ���֤��Ϣ
bool CDataBaseSink::OnRequestUserAuthentication(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserAuthentication));
	if (wDataSize!=sizeof(DBR_GP_UserAuthentication)) return false;

	//����ת��
	DBR_GP_UserAuthentication *pReq = (DBR_GP_UserAuthentication*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserAuthentication(pReq->dwUserID, pReq->szName, pReq->szIdCardNum);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//����ж�
	DBR_GP_CommonResult result;
	ZeroMemory(&result, sizeof(result));

	//��ȡ����
	IDataBase * pIDataBase=(IDataBase *)m_AccountsDBAide.GetDataBase(IID_IDataBase,VER_IDataBase);
		
	//��ȡ��Ϣ
	result.dwUserID = pReq->dwUserID;
	result.iCode = lReturnValue;

	m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_AUTHENTICATION_RESULT,dwContextID,
		&result,sizeof(DBR_GP_CommonResult));	

	return true;
}

//�����̳���Ʒ��Ϣ
bool CDataBaseSink::OnRequestMallProduct(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize, bool isIOS)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserMallProduct));
	if (wDataSize!=sizeof(DBR_GP_UserMallProduct)) return false;

	//����ת��
	DBR_GP_UserMallProduct *pMallProduct = (DBR_GP_UserMallProduct*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		if (isIOS == true)
			lReturnValue = SPUserMallProductIOS(pMallProduct->dwUserID);
		else
			lReturnValue = SPUserMallProduct(pMallProduct->dwUserID);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_UserMallProductResultFinish mallProductFinish;
		ZeroMemory(&mallProductFinish, sizeof(mallProductFinish));
		mallProductFinish.bOperateSuccess = true;

		//��ȡ����
		IDataBase * pIDataBase=(IDataBase *)m_AccountsDBAide.GetDataBase(IID_IDataBase,VER_IDataBase);

		//��ȡ��Ϣ
		while (pIDataBase->IsRecordsetEnd()==false)
		{
			DBR_GP_UserMallProductResult product;
			//��ȡ��Ϣ
			product.productID=m_AccountsDBAide.GetValue_DWORD(TEXT("productID"));
			product.productPrice=m_AccountsDBAide.GetValue_DWORD(TEXT("productPrice"));
			m_AccountsDBAide.GetValue_String(TEXT("productImg"),product.szProductImage,CountArray(product.szProductImage));
			m_AccountsDBAide.GetValue_String(TEXT("productName"),product.szProductName,CountArray(product.szProductName));

			product.version = pMallProduct->dwVersion;
			if (pMallProduct->dwVersion  == 1 || pMallProduct->dwVersion  == 2)
			{
				//������2���ֶ�
				product.cbHotFlag = m_AccountsDBAide.GetValue_INT(TEXT("HotFlag")); 
				m_AccountsDBAide.GetValue_String(TEXT("ProductNameAdd"),product.szProductNameAdd,CountArray(product.szProductNameAdd));
			}

			//�ƶ���¼
			pIDataBase->MoveToNext();

			//������Ϣ
			mallProductFinish.vecProducts.push_back(product);
		}

		//�رռ�¼
		pIDataBase->CloseRecordset();

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_MALL_PRODUCT_RESULT,dwContextID,
			&mallProductFinish,sizeof(mallProductFinish));
	}
	else
	{
		DBR_GP_UserMallProductResultFinish mallProductResult;
		ZeroMemory(&mallProductResult, sizeof(mallProductResult));
		mallProductResult.bOperateSuccess = false;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_MALL_PRODUCT_RESULT,dwContextID,
			&mallProductResult,sizeof(mallProductResult));
	}

	return true;
}

//�����̳Ƕ���
bool CDataBaseSink::OnRequestMallPlaceOrder(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserMallPlaceOrder));
	if (wDataSize!=sizeof(DBR_GP_UserMallPlaceOrder)) return false;

	//����ת��
	DBR_GP_UserMallPlaceOrder *pOrder = (DBR_GP_UserMallPlaceOrder*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserMallPlaceOrder(pOrder->dwUserID, pOrder->dwProductID,pOrder->szPayWay);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_UserMallPlaceOrderResult orderResult;
		ZeroMemory(&orderResult, sizeof(orderResult));
		orderResult.nResult = true;
		m_AccountsDBAide.GetValue_String(TEXT("dwOrderID"),orderResult.dwOrderID,CountArray(orderResult.dwOrderID));
		m_AccountsDBAide.GetValue_String(TEXT("productName"),orderResult.productName,CountArray(orderResult.productName));
		m_AccountsDBAide.GetValue_String(TEXT("token"),orderResult.token,CountArray(orderResult.token));
		orderResult.productPrice = m_AccountsDBAide.GetValue_DWORD(TEXT("productPrice"));
		m_AccountsDBAide.GetValue_String(TEXT("billingIndex"),orderResult.billingCode,CountArray(orderResult.billingCode));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_MALL_PLACE_ORDER_RESULT,dwContextID,
			&orderResult,sizeof(orderResult));
	}
	else
	{
		DBR_GP_UserMallPlaceOrderResult orderResult;
		ZeroMemory(&orderResult, sizeof(orderResult));
		orderResult.nResult = false;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_MALL_PLACE_ORDER_RESULT,dwContextID,
			&orderResult,sizeof(orderResult));
	}

	return true;
}

bool CDataBaseSink::OnRequestUserCancelOrder(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_Cancel_Order_Num));
	if (wDataSize!=sizeof(DBR_Cancel_Order_Num)) return false;

	//����ת��
	DBR_Cancel_Order_Num *pOrder = (DBR_Cancel_Order_Num*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserCancelOrder(pOrder->orderNum);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_Cancel_Order_Resut orderResult;
		ZeroMemory(&orderResult, sizeof(orderResult));

		orderResult.iCode = 0;
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_CANCEL_ORDER_NUM,dwContextID,
			&orderResult,sizeof(orderResult));
	}
	else
	{
		DBR_Cancel_Order_Resut orderResult;
		ZeroMemory(&orderResult, sizeof(orderResult));

		orderResult.iCode = -1;
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_CANCEL_ORDER_NUM,dwContextID,
			&orderResult,sizeof(orderResult));
	}

	return true;
}

//�����̳���Ʒ������
bool CDataBaseSink::OnRequestMallBuy(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserMallBuy));
	if (wDataSize!=sizeof(DBR_GP_UserMallBuy)) return false;

	//����ת��
	DBR_GP_UserMallBuy *pReq = (DBR_GP_UserMallBuy*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserMallBuy(pReq->dwOrderID);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	DBR_GP_UserMallBuyResult result;
	ZeroMemory(&result, sizeof(result));

	result.iCode = lReturnValue;
	result.dwUserID = m_AccountsDBAide.GetValue_UINT(TEXT("UserID"));
	result.productID = m_AccountsDBAide.GetValue_INT(TEXT("productID"));
	result.dwGoldScore = m_AccountsDBAide.GetValue_UINT(TEXT("dwGoldScore"));
	m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_MALL_BUY_RESULT, dwContextID,
		&result,sizeof(result));

	return true;
}

//�����̳ǲ�ѯ�����Ÿ��½��
bool CDataBaseSink::OnRequestMallUpdateBuy(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserMallUpdate));
	if (wDataSize!=sizeof(DBR_GP_UserMallUpdate)) return false;

	//����ת��
	DBR_GP_UserMallUpdate *pReq = (DBR_GP_UserMallUpdate*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserMallUpdateBuy(pReq->dwOrderID);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	DBR_GP_UserMallUpdateResult result;
	ZeroMemory(&result, sizeof(result));

	result.iCode = lReturnValue;
	result.dwUserID = m_AccountsDBAide.GetValue_UINT(TEXT("UserID"));
	result.productID = m_AccountsDBAide.GetValue_INT(TEXT("productID"));
	result.dwGoldScore = m_AccountsDBAide.GetValue_UINT(TEXT("dwGoldScore"));
	m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_MALL_UPDATE_RESULT, dwContextID,
		&result,sizeof(result));

	return true;
}

//�����ѯ�û�������Ϣ
bool CDataBaseSink::OnRequestUserBankInfo(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserBankInfo));
	if (wDataSize!=sizeof(DBR_GP_UserBankInfo)) return false;

	//����ת��
	DBR_GP_UserBankInfo *pReq = (DBR_GP_UserBankInfo*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserBankInfo(pReq->dwUserID);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_UserBankInfoResult result;
		ZeroMemory(&result, sizeof(result));
		result.nResult = lReturnValue;
		result.dwUserID = pReq->dwUserID;
		result.dwCurScore = m_AccountsDBAide.GetValue_DWORD(TEXT("curScore"));
		result.dwBankScore = m_AccountsDBAide.GetValue_DWORD(TEXT("bankScore"));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_BANK_INFO_RESULT,dwContextID,
			&result,sizeof(result));
	}
	else
	{
		DBR_GP_UserBankInfoResult result;
		ZeroMemory(&result, sizeof(result));
		result.nResult = lReturnValue;
		result.dwUserID = pReq->dwUserID;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_BANK_INFO_RESULT,dwContextID,
			&result,sizeof(result));
	}

	return true;
}

//�����������
bool CDataBaseSink::OnRequestUserBankCharge(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserBankCharge));
	if (wDataSize!=sizeof(DBR_GP_UserBankCharge)) return false;

	//����ת��
	DBR_GP_UserBankCharge *pReq = (DBR_GP_UserBankCharge*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserBankCharge(pReq->nOpCode, pReq->dwUserID, pReq->dwCurScore, pReq->dwBankScore);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		DBR_GP_UserBankChargeResult result;
		ZeroMemory(&result, sizeof(result));
		result.nResult = lReturnValue;
		result.dwUserID = pReq->dwUserID;
		result.dwCurScore = pReq->dwCurScore;
		result.dwBankScore = pReq->dwBankScore;

		//result.dwCurScore = m_AccountsDBAide.GetValue_DWORD(TEXT("curScore"));
		//result.dwBankScore = m_AccountsDBAide.GetValue_DWORD(TEXT("bankScore"));

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_BANK_CHARGE_RESULT,dwContextID,
			&result,sizeof(result));
	}
	else
	{
		DBR_GP_UserBankInfoResult result;
		ZeroMemory(&result, sizeof(result));
		result.nResult = lReturnValue;
		result.dwUserID = pReq->dwUserID;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_BANK_CHARGE_RESULT,dwContextID,
			&result,sizeof(result));
	}

	return true;
}

//��ѯ��б�
bool CDataBaseSink::OnRequestActivityList(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserActivityList();
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}
	
	DBR_GP_UserActivityListResult result;
	ZeroMemory(&result, sizeof(result));
	result.iCode = lReturnValue;

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		//��ȡ����
		IDataBase * pIDataBase=(IDataBase *)m_AccountsDBAide.GetDataBase(IID_IDataBase,VER_IDataBase);

		//��ȡ��Ϣ
		int iCnt = 0;
		while (pIDataBase->IsRecordsetEnd()==false)
		{
			m_AccountsDBAide.GetValue_String(TEXT("Title"),result.szTitle[iCnt],CountArray(result.szTitle[iCnt]));
			m_AccountsDBAide.GetValue_String(TEXT("Text"),result.szDetails[iCnt],CountArray(result.szDetails[iCnt]));
			m_AccountsDBAide.GetValue_String(TEXT("IconUrl"),result.szIconUrl[iCnt],CountArray(result.szIconUrl[iCnt]));
			m_AccountsDBAide.GetValue_String(TEXT("LinkUrl"),result.szLinkUrl[iCnt],CountArray(result.szLinkUrl[iCnt]));
			
			result.iType[iCnt] = m_AccountsDBAide.GetValue_WORD(TEXT("Type")); 
			result.iStatus[iCnt] = m_AccountsDBAide.GetValue_DWORD(TEXT("Status"));  
			//result.iStatus[iCnt] = m_AccountsDBAide.GetValue_DWORD(TEXT("Status")); 

			TCHAR szStatus[16]=TEXT("");
			m_AccountsDBAide.GetValue_String(TEXT("Status"),szStatus,sizeof(szStatus));
			result.iStatus[iCnt] = atoi(szStatus);
			result.iID[iCnt] = m_AccountsDBAide.GetValue_DWORD(TEXT("ID"));  

			iCnt++;

			//�ƶ���¼
			pIDataBase->MoveToNext();
		}

		//�رռ�¼
		pIDataBase->CloseRecordset();

		result.iCount = iCnt;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_ACTIVITY_LIST_RESULT,dwContextID,
			&result,sizeof(result));
	}
	else
	{
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_ACTIVITY_LIST_RESULT,dwContextID,
			&result,sizeof(result));
	}

	return true;
}

//��ѯ�
bool CDataBaseSink::OnRequestActivity(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserActivity));
	if (wDataSize!=sizeof(DBR_GP_UserActivity)) return false;

	//����ת��
	DBR_GP_UserActivity *pReq = (DBR_GP_UserActivity*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserActivity(pReq->dwUserID, pReq->dwActivityID);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}
	
	DBR_GP_UserActivityResult result;
	ZeroMemory(&result, sizeof(result));
	result.iCode = lReturnValue;

	result.dwActivityID =  pReq->dwActivityID;
	result.dwUserID =  pReq->dwUserID;

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		//��ȡ����
		IDataBase * pIDataBase=(IDataBase *)m_AccountsDBAide.GetDataBase(IID_IDataBase,VER_IDataBase);

		//��ȡ��Ϣ
		int iCnt = 0;
		while (pIDataBase->IsRecordsetEnd()==false)
		{
			//���5��
			if (iCnt == 5)break;

			//�ظ���Ϣ��ֻ��һ��
			if (iCnt == 0)
			{
				//���齱����
				DWORD dwMax = m_AccountsDBAide.GetValue_DWORD(TEXT("M"));
				//��ʹ�õĳ齱����
				DWORD dwUsed = m_AccountsDBAide.GetValue_DWORD(TEXT("N"));
				//������
				DWORD dwTotal = m_AccountsDBAide.GetValue_DWORD(TEXT("T"));
				//�����̳�һ��
				DWORD dwUnit = m_AccountsDBAide.GetValue_DWORD(TEXT("U"));
				char buf[128];
				memset(buf, 0, 128);
				sprintf(buf, "m:%d|n:%d|t:%d|u:%d", dwMax, dwUsed, dwTotal, dwUnit);
				memcpy(result.szParam, buf, CountArray(result.szParam));
		
				m_AccountsDBAide.GetValue_String(TEXT("ImageUrl"), result.szResourceUrl, CountArray(result.szResourceUrl));
			}

			//�������Ϣ
			TCHAR szUserName[32];
			TCHAR szPrizeName[32];

			m_AccountsDBAide.GetValue_String(TEXT("UserName"),szUserName,CountArray(szUserName));
			m_AccountsDBAide.GetValue_String(TEXT("PrizeName"),szPrizeName,CountArray(szPrizeName));

			if (strlen(szUserName) == 0||strlen(szPrizeName) == 0)
			{
				break;
			}
			sprintf(result.szText[iCnt], "��ϲ��� %s ����%s", szUserName, szPrizeName);

			iCnt++;

			//�ƶ���¼
			pIDataBase->MoveToNext();
		}

		//�رռ�¼
		pIDataBase->CloseRecordset();

		result.wCount = iCnt;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_ACTIVITY_RESULT,dwContextID,
			&result,sizeof(result));
	}
	else
	{
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_ACTIVITY_RESULT,dwContextID,
			&result,sizeof(result));
	}

	return true;
}
//�齱
bool CDataBaseSink::OnRequestActivityLucky(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserActivityLucky));
	if (wDataSize!=sizeof(DBR_GP_UserActivityLucky)) return false;

	//����ת��
	DBR_GP_UserActivityLucky *pReq = (DBR_GP_UserActivityLucky*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserActivityLuckyList(pReq->dwUserID, pReq->dwActivityID);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	DBR_GP_UserActivityLuckyResult result;
	ZeroMemory(&result, sizeof(DBR_GP_UserActivityLuckyResult));
	result.dwActivityID =  pReq->dwActivityID;
	result.dwUserID =  pReq->dwUserID;
	result.iCode = lReturnValue;
	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		//��ȡ�б���Ϣ
		IDataBase * pIDataBase=(IDataBase *)m_AccountsDBAide.GetDataBase(IID_IDataBase,VER_IDataBase);

		//��ȡ��Ϣ
		vector<unsigned int> vecPrizeID;
		vector<unsigned int> vecPossibility;
		while (pIDataBase->IsRecordsetEnd()==false)
		{

			DWORD stock = m_AccountsDBAide.GetValue_DWORD(TEXT("Stock"));
			DWORD maxCount = m_AccountsDBAide.GetValue_DWORD(TEXT("MaxCount"));
			DWORD todayCount = m_AccountsDBAide.GetValue_DWORD(TEXT("TodayCount"));

			if (stock > 0 && todayCount < maxCount)
			{
				vecPrizeID.push_back(m_AccountsDBAide.GetValue_DWORD(TEXT("PID")));
				vecPossibility.push_back(m_AccountsDBAide.GetValue_DWORD(TEXT("Possibility")));
			}

			//�ƶ���¼
			pIDataBase->MoveToNext();
		}

		//�رռ�¼
		pIDataBase->CloseRecordset();

		//ʧ��
		if (vecPrizeID.size() <= 0)
		{
			//Ŀǰû�н�Ʒ���Գ�
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_ACTIVITY_LUCKY_RESULT,dwContextID,
				&result,sizeof(result));
			return true;
		}

		int iTotal = 0;
		for (int i=0; i<vecPrizeID.size(); i++)
		{
			iTotal += vecPossibility[i];
		}
		int wIndex = rand()%iTotal;  //���������
		int iPrizeID = -1;
		iTotal = 0;
		for (int i=0; i<vecPrizeID.size(); i++)
		{
			if (wIndex < iTotal + vecPossibility[i])
			{
				//�ҵ���
				iPrizeID = i;
				break;
			}
			iTotal += vecPossibility[i];
		}

		//ʧ��
		if (iPrizeID == -1)
		{
			//�㷨�������⣬û�г���
			result.iCode = -3;
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_ACTIVITY_LUCKY_RESULT,dwContextID,
				&result,sizeof(result));
			return true;
		}

		//ȥ�콱
		//ִ�д洢����
		lReturnValue = -1;
		try
		{
			lReturnValue = SPUserActivityLucky(pReq->dwUserID, pReq->dwActivityID, vecPrizeID[iPrizeID]);
		}
		catch (IDataBaseException * pIException)
		{
			//������Ϣ
			LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
			CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
		}

		result.iCode = lReturnValue;
		//�ɹ��ж�
		if ( lReturnValue == 0 )
		{
			//��ȡ��Ϣ
			result.dwPrizeID = m_AccountsDBAide.GetValue_DWORD(TEXT("PID"));
			result.dwPrice = m_AccountsDBAide.GetValue_DWORD(TEXT("Price"));
		
			m_AccountsDBAide.GetValue_String(TEXT("PrizeUrl"), result.szImageUrl, CountArray(result.szImageUrl));
			m_AccountsDBAide.GetValue_String(TEXT("PrizeName"), result.szPrizeName, CountArray(result.szPrizeName));

			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_ACTIVITY_LUCKY_RESULT,dwContextID,
				&result,sizeof(result));
		}
		else
		{
			//ʧ��
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_ACTIVITY_LUCKY_RESULT,dwContextID,
				&result,sizeof(result));
			return true;
		}
	}
	else
	{
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_ACTIVITY_LUCKY_RESULT,dwContextID,
			&result,sizeof(result));
	}

	return true;
}
//�齱��¼
bool CDataBaseSink::OnRequestUserActivityRecord(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserActivityRecord));
	if (wDataSize!=sizeof(DBR_GP_UserActivityRecord)) return false;

	//����ת��
	DBR_GP_UserActivityRecord *pActivityRecord = (DBR_GP_UserActivityRecord*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		if (pActivityRecord->nPage < 1)pActivityRecord->nPage = 1;
		lReturnValue = SPUserActivityRecord(pActivityRecord->dwUserID, pActivityRecord->dwActivityID, pActivityRecord->nPage - 1);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}
	
	DBR_GP_UserActivityRecordResult ActivityRecordResult;
	ZeroMemory(&ActivityRecordResult, sizeof(ActivityRecordResult));
	ActivityRecordResult.dwActivityID = pActivityRecord->dwActivityID;
	ActivityRecordResult.dwUserID = pActivityRecord->dwUserID;

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		ActivityRecordResult.bOperateSuccess = true;

		//��ȡ����
		IDataBase * pIDataBase=(IDataBase *)m_AccountsDBAide.GetDataBase(IID_IDataBase,VER_IDataBase);

		//��ȡ��Ϣ
		while (pIDataBase->IsRecordsetEnd()==false)
		{
			DBR_GP_ActivityRecord record;
			ZeroMemory(&record, sizeof(record));

			//��ȡ��Ϣ
			record.dwTotal=m_AccountsDBAide.GetValue_DWORD(TEXT("T"));
			record.dwID=m_AccountsDBAide.GetValue_DWORD(TEXT("ID"));
			record.dwAwardID=m_AccountsDBAide.GetValue_DWORD(TEXT("PrizeID"));
	//		record.dwDate=m_AccountsDBAide.GetValue_DWORD(TEXT("ExchangeDate"));
			m_AccountsDBAide.GetValue_String(TEXT("D"),record.szDate,CountArray(record.szDate));
			m_AccountsDBAide.GetValue_String(TEXT("PrizeName"),record.szAwardName,CountArray(record.szAwardName));
			m_AccountsDBAide.GetValue_String(TEXT("Status"),record.szStatus,CountArray(record.szStatus));

			//�ƶ���¼
			pIDataBase->MoveToNext();

			//������Ϣ
			ActivityRecordResult.vecRecords.push_back(record);
		}

		//�رռ�¼
		pIDataBase->CloseRecordset();

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_ACTIVITY_RECORD_RESULT,dwContextID,
			&ActivityRecordResult,sizeof(ActivityRecordResult));
	}
	else
	{
		ActivityRecordResult.bOperateSuccess = false;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_ACTIVITY_RECORD_RESULT,dwContextID,
			&ActivityRecordResult,sizeof(ActivityRecordResult));
	}

	return true;
}

//��������
bool CDataBaseSink::OnRequestUserSpeakerSend(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserSpeakerSend));
	if (wDataSize!=sizeof(DBR_GP_UserSpeakerSend)) return false;

	//����ת��
	DBR_GP_UserSpeakerSend *pReq = (DBR_GP_UserSpeakerSend*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserSpeakerSend(pReq->dwUserID, pReq->iType, pReq->szDate);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}
	
	DBR_GP_UserSpeakerSendResult dbResult;
	ZeroMemory(&dbResult, sizeof(dbResult));
	dbResult.iType = pReq->iType;
	dbResult.dwUserID = pReq->dwUserID;
	dbResult.nLen = pReq->nLen;
	memcpy(dbResult.szDate, pReq->szDate, pReq->nLen);
	
	dbResult.iCode = lReturnValue;

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		//��ȡ��Ϣ
		dbResult.dwScore=m_AccountsDBAide.GetValue_DWORD(TEXT("Score"));
	}
	
	m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_SPEAKER_SEND_RESULT,dwContextID,
			&dbResult,sizeof(dbResult));

	return true;
}

//��ѯ������
bool CDataBaseSink::OnRequestUserQueryConfig(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UserQueryConfig));
	if (wDataSize!=sizeof(DBR_GP_UserQueryConfig)) return false;

	//����ת��
	DBR_GP_UserQueryConfig *pReq = (DBR_GP_UserQueryConfig*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPUserQueryConfig(pReq->dwUserID, pReq->dwVersion, pReq->dwConfigID, pReq->szChannel);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}
	
	DBR_GP_UserQueryConfigResult dbResult;
	ZeroMemory(&dbResult, sizeof(dbResult));
	dbResult.dwUserID = pReq->dwUserID;
	dbResult.dwVersion = pReq->dwVersion;
	
	dbResult.iCode = lReturnValue;

	//�ɹ��ж�
	if ( lReturnValue == 0 )
	{
		//��ȡ����
		IDataBase * pIDataBase=(IDataBase *)m_AccountsDBAide.GetDataBase(IID_IDataBase,VER_IDataBase);

		//��ȡ��Ϣ
		while (pIDataBase->IsRecordsetEnd()==false)
		{
			//ȡÿһ�е�ֵ
			CHAR key[64];
			CHAR value[300];
			memset(key, 0, sizeof(key));
			memset(value, 0, sizeof(value));
			memset(dbResult.szDate, 0, sizeof(dbResult.szDate));
			dbResult.dwLen = 0;
			
			m_AccountsDBAide.GetValue_String(TEXT("K"), key,CountArray(key));
			m_AccountsDBAide.GetValue_String(TEXT("V"), value,CountArray(value));

			sprintf(dbResult.szDate, "%s|%s", key, value);

			size_t iLenTmp = strlen(dbResult.szDate);
			dbResult.dwLen = iLenTmp>sizeof(dbResult.szDate)?sizeof(dbResult.szDate):iLenTmp;

			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_QUERY_CONFIG_RESULT,dwContextID,
							&dbResult,sizeof(dbResult));

			//�ƶ���¼
			pIDataBase->MoveToNext();
		}

		//�رռ�¼
		pIDataBase->CloseRecordset();

		//������Ϣ
		memset(dbResult.szDate, 0, sizeof(dbResult.szDate));
		dbResult.dwLen = 0;
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_QUERY_CONFIG_RESULT,dwContextID,
							&dbResult,sizeof(dbResult));
	}
	else
	{
		//������Ϣ
		memset(dbResult.szDate, 0, sizeof(dbResult.szDate));
		dbResult.dwLen = 0;
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_USER_QUERY_CONFIG_RESULT,dwContextID,
							&dbResult,sizeof(dbResult));
	}

	return true;
}

//�ʺŴ洢����
LONG CDataBaseSink::SPLogonByAccounts(LPCTSTR pszAccounts, LPCTSTR pszPassword, DWORD dwClientIP, LPCTSTR pszComputerID)
{
	//Ч�����
	ASSERT(pszAccounts!=NULL);
	ASSERT(pszPassword!=NULL);

	//ת����ַ
	TCHAR szClientIP[16]=TEXT("");
	BYTE * pClientIP=(BYTE *)&dwClientIP;
	_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pszAccounts);
	m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pszPassword);
	m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);
	m_AccountsDBAide.AddParameter(TEXT("@strMachineSerial"),pszComputerID);
	if (m_pInitParamter)
	{
		m_AccountsDBAide.AddParameter(TEXT("@strLoginServer"),m_pInitParamter->m_szMainPage);
	}

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyAccounts_v2"),true);
}

//���´洢����
LONG CDataBaseSink::SPRegisterAccounts(LPCTSTR pszAccounts, LPCTSTR pszPassword, LPCTSTR pszSpreader, WORD wFaceID, BYTE cbGender, DWORD dwClientIP, LPCTSTR pszComputerID)
{
	//Ч�����
	ASSERT(pszAccounts!=NULL);
	ASSERT(pszPassword!=NULL);

	//ת����ַ
	TCHAR szClientIP[16]=TEXT("");
	BYTE * pClientIP=(BYTE *)&dwClientIP;
	_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pszAccounts);
	m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pszPassword);
	m_AccountsDBAide.AddParameter(TEXT("@strSpreader"),pszSpreader);
	m_AccountsDBAide.AddParameter(TEXT("@nFaceID"),wFaceID);
	m_AccountsDBAide.AddParameter(TEXT("@cbGender"),cbGender);
	m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);
	m_AccountsDBAide.AddParameter(TEXT("@strMachineSerial"),pszComputerID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_RegisterAccounts"),true);
}

//ע��洢����WX
LONG CDataBaseSink::SPRegisterAccountsWX(LPCTSTR pszAccounts, LPCTSTR pszPassword, LPCTSTR pszSpreader, 
	LPCTSTR wFaceURL, BYTE cbGender, DWORD dwClientIP, LPCTSTR pszWXComputerID,LPCTSTR pszComputerID,
	LPCTSTR pszNickname, LPCTSTR pszOpenid, LPCTSTR pszUnionid)
{
	//ת����ַ
	TCHAR szClientIP[16]=TEXT("");
	BYTE * pClientIP=(BYTE *)&dwClientIP;
	_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),pszAccounts);
	m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pszPassword);
	m_AccountsDBAide.AddParameter(TEXT("@strSpreader"),pszSpreader);
	m_AccountsDBAide.AddParameter(TEXT("@strNickname"),pszNickname);
	m_AccountsDBAide.AddParameter(TEXT("@strOpenid"),pszOpenid);
	m_AccountsDBAide.AddParameter(TEXT("@strUnionid"),pszUnionid);
	m_AccountsDBAide.AddParameter(TEXT("@strFaceUrl"),wFaceURL);
	m_AccountsDBAide.AddParameter(TEXT("@cbGender"),cbGender);
	m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);
	m_AccountsDBAide.AddParameter(TEXT("@strWXMachineSerial"),pszWXComputerID);
	m_AccountsDBAide.AddParameter(TEXT("@strMachineSerial"),pszComputerID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_RegisterAccountsWX_v2"),true);
}

//������WX�˺Ŵ洢����
LONG CDataBaseSink::SPWXUpdate(DWORD dwUserID,LPCTSTR wFaceURL, BYTE cbGender, DWORD dwClientIP, LPCTSTR pszWXComputerID,LPCTSTR pszComputerID,LPCTSTR pszNickname, LPCTSTR pszOpenid, LPCTSTR pszUnionid)
{
	//ת����ַ
	TCHAR szClientIP[16]=TEXT("");
	BYTE * pClientIP=(BYTE *)&dwClientIP;
	_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@iUserId"),dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@strNickname"),pszNickname);
	m_AccountsDBAide.AddParameter(TEXT("@strOpenid"),pszOpenid);
	m_AccountsDBAide.AddParameter(TEXT("@strUnionid"),pszUnionid);
	m_AccountsDBAide.AddParameter(TEXT("@strFaceUrl"),wFaceURL);
	m_AccountsDBAide.AddParameter(TEXT("@cbGender"),cbGender);
	m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);
	m_AccountsDBAide.AddParameter(TEXT("@strWXMachineSerial"),pszWXComputerID);
	m_AccountsDBAide.AddParameter(TEXT("@strMachineSerial"),pszComputerID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_WXUpdate_v2"),true);
}

//I D �洢����
LONG CDataBaseSink::SPLogonByUserID(DWORD dwGameID, LPCTSTR pszPassword, DWORD dwClientIP, LPCTSTR pszComputerID)
{
	//Ч�����
	ASSERT(dwGameID!=0L);
	ASSERT(pszPassword!=NULL);

	//ת����ַ
	TCHAR szClientIP[16]=TEXT("");
	BYTE * pClientIP=(BYTE *)&dwClientIP;
	_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwGameID"),dwGameID);
	m_AccountsDBAide.AddParameter(TEXT("@strPassword"),pszPassword);
	m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);
	m_AccountsDBAide.AddParameter(TEXT("@strMachineSerial"),pszComputerID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyGameID"),true);
}

//ǩ���洢����
LONG CDataBaseSink::SPUserSignIn(DWORD dwUserID)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UserSignIn"),true);
}

//�콱�洢����
LONG CDataBaseSink::SPUserGetAward(DWORD dwUserID, DWORD dwTaskID)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@dwTaskID"),dwTaskID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UserGetAward"),true);
}

//�һ��ĸ�����Ϣ�洢����
LONG CDataBaseSink::SPUserExchangeInfo(DWORD dwUserID, LPCTSTR pszPhone)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@strPhone"),pszPhone);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UserExchangeInfo"),true);
}

//�һ���Ʒ
LONG CDataBaseSink::SPUserExchangeProduct(DWORD dwUserID)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UserExchangeProduct"),true);
}

//�һ�
LONG CDataBaseSink::SPUserExchange(DWORD dwUserID,DWORD dwAwardID)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@dwAwardID"),dwAwardID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UserExchange"),true);
}

//�һ���¼
LONG CDataBaseSink::SPUserExchangeRecord(DWORD dwUserID, int nPage)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@nPage"),nPage);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UserExchangeRecord"),true);
}

//�һ�
LONG CDataBaseSink::SPUserExchange_V2(DWORD dwUserID,DWORD dwAwardID)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@dwAwardID"),dwAwardID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UserExchange_V2"),true);
}

//�ϴ��洢����
LONG CDataBaseSink::SPUploadFace(DWORD dwUserID, BYTE *bFaceData, DWORD dwFaceDataSize)
{
	//Ч�����
	ASSERT(dwUserID!=0L);
	ASSERT(dwFaceDataSize!=0L);
	ASSERT(bFaceData!=NULL);

	//Ч�����
	ASSERT(dwUserID!=0L);
	ASSERT(dwFaceDataSize!=0L);
	ASSERT(bFaceData!=NULL);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);

	VARIANT varChunk;
	SAFEARRAY *psa=NULL;
	SAFEARRAYBOUND rgsabound[1];
	rgsabound[0].cElements = dwFaceDataSize;
    rgsabound[0].lLbound = 0;
    psa = SafeArrayCreate(VT_UI1,1,rgsabound);
    for (long index =0; index < long(dwFaceDataSize); index++ )
	{
		SafeArrayPutElement(psa,&index,&bFaceData[index]);
	}
    varChunk.vt = VT_ARRAY|VT_UI1;
    varChunk.parray = psa;

	IDataBase * pIDataBase=m_AccountsDBModule.GetInterface();
	pIDataBase->AddParameter(TEXT("@imgCustomFaceImage"),adLongVarBinary,adParamInput,dwFaceDataSize,_variant_t(varChunk));

	LONG lReturnValue=m_AccountsDBAide.ExecuteProcess(TEXT("GSP_CF_InsertCustomFace"),false);

	//�ͷ���Դ
	SafeArrayDestroy(psa);

	return lReturnValue;
}

//���ش洢����
LONG CDataBaseSink::SPDownloadFace(DWORD dwUserID)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);


	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_CF_LoadCustomFace"),true);
}

//ɾ���洢����
LONG CDataBaseSink::SPDeleteCustomFace(DWORD dwUserID)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_CF_DeleteCustomFace"),true);
}

//�޸Ĵ洢����
LONG CDataBaseSink::SPModifyIndividual(TCHAR szAccount[NAME_LEN],TCHAR szNickname[NAME_LEN], TCHAR szPhone[NAME_LEN],int nGender)
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),szAccount);
	m_AccountsDBAide.AddParameter(TEXT("@strNickName"),szNickname);
	m_AccountsDBAide.AddParameter(TEXT("@strPhone"),szPhone);
	m_AccountsDBAide.AddParameter(TEXT("@nGender"),nGender);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyIndividual"),true);
}

//�޸ĸ���ǩ���洢����
LONG CDataBaseSink::SPModifyUnderWrite(unsigned int dwUserID,TCHAR szDescribe[UNDER_WRITE_LEN])
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@UserId"),dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@strUnderWrite"),szDescribe);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyUnderWrite"),true);
}

//�޸��ֻ��Ŵ洢����
LONG CDataBaseSink::SPModifyPhone(unsigned int dwUserID,TCHAR szPhone[31])
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@UserId"),dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@strPhone"),szPhone);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ModifyPhone"),true);
}

//У�����ƴ洢����
LONG CDataBaseSink::SPCheckID(TCHAR szAccount[NAME_LEN])
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@strAccount"),szAccount);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_CheckID"),true);
}

//У���ǳƴ洢����
LONG CDataBaseSink::SPCheckNickName(TCHAR szNickName[NAME_LEN])
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@strNickName"),szNickName);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_CheckNickName"),true);
}

//ע��洢����
LONG CDataBaseSink::SPRegister(TCHAR szAccount[NAME_LEN],TCHAR szNickname[NAME_LEN],TCHAR szPhone[NAME_LEN], int nGender, 
							   TCHAR szPassword[PASS_LEN], DWORD dwClientIP, LPCTSTR pszComputerID, LPCTSTR pszChannel)
{
	
	//ת����ַ
	TCHAR szClientIP[16]=TEXT("");
	BYTE * pClientIP=(BYTE *)&dwClientIP;
	_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@szAccount"),szAccount);
	m_AccountsDBAide.AddParameter(TEXT("@strNickName"),szNickname);
	m_AccountsDBAide.AddParameter(TEXT("@strPhone"),szPhone);
	m_AccountsDBAide.AddParameter(TEXT("@szPassword"),szPassword);
	m_AccountsDBAide.AddParameter(TEXT("@cbGender"),nGender);
	m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);
	m_AccountsDBAide.AddParameter(TEXT("@strMachineSerial"),pszComputerID);
	m_AccountsDBAide.AddParameter(TEXT("@strChannel"),pszChannel);


	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_Register"),true);
}


//�ʺ������洢����
LONG CDataBaseSink::SPIDUpdate(TCHAR szAccount[NAME_LEN],TCHAR szAccountNew[NAME_LEN],TCHAR szNickname[NAME_LEN], int nGender, TCHAR szPassword[PASS_LEN])
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@strAccounts"),szAccount);
	m_AccountsDBAide.AddParameter(TEXT("@strAccountsNew"),szAccountNew);
	m_AccountsDBAide.AddParameter(TEXT("@strNickName"),szNickname);
	m_AccountsDBAide.AddParameter(TEXT("@strPassword"),szPassword);
	m_AccountsDBAide.AddParameter(TEXT("@cbGender"),nGender);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_IDUpdate"),true);
}

//��������洢����
LONG CDataBaseSink::SPResetPwd(TCHAR szAccount[NAME_LEN], TCHAR szPassword[PASS_LEN])
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@szAccount"),szAccount);
	m_AccountsDBAide.AddParameter(TEXT("@szPassword"),szPassword);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ResetPwd"),true);
}

//��ѯ��Ϣ�洢����
LONG CDataBaseSink::SPUserInfo(TCHAR szAccount[NAME_LEN])
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@szAccount"),szAccount);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UserInfo"),true);
}

//��ѯ�û��Ƿ������ȡ�ƹ㽱��
LONG CDataBaseSink::SPUserCanSpread(DWORD dwUser, LPCTSTR pszComputerID)
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUser);
	m_AccountsDBAide.AddParameter(TEXT("@strMachineSerial"),pszComputerID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UserCanGetSpreadAward"),true);
}

//��ѯ�û��Ƿ������ȡ�ƹ㽱��
LONG CDataBaseSink::SPUserGetSpread(DWORD dwUser, DWORD dwSpreader, LPCTSTR pszComputerID)
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUser);
	m_AccountsDBAide.AddParameter(TEXT("@dwSpreaderID"),dwSpreader);
	m_AccountsDBAide.AddParameter(TEXT("@strMachineSerial"),pszComputerID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UserGetSpreadAward"),true);
}

//��ѯ�ƹ�Ա��Ϣ
LONG CDataBaseSink::SPUserGetSpreadInfo(DWORD dwSpreader)
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwSpreaderID"),dwSpreader);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UserGetSpreadInfo"),true);
}

//����������Ϣ
LONG CDataBaseSink::SPUserGetBbsPop(LPCTSTR pszChannel)
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@szChannelName"),pszChannel);

	return m_AccountsDBAide.ExecuteProcess(TEXT("WHServerInfoDB.dbo.GSP_GR_GetBbsPop_V2"),true);
}

//����������Ϣ
LONG CDataBaseSink::SPUserGetBbsScroll(LPCTSTR pszChannel)
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@szChannelName"),pszChannel);

	return m_AccountsDBAide.ExecuteProcess(TEXT("WHServerInfoDB.dbo.GSP_GR_GetBbsScroll_V2"),true);
}

//�б�����Ϣ
LONG CDataBaseSink::SPUserGetBbsList(LPCTSTR pszChannel)
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@szChannelName"),pszChannel);

	return m_AccountsDBAide.ExecuteProcess(TEXT("WHServerInfoDB.dbo.GSP_GR_GetBbsList_V2"),true);
}

//���������Ϣ
LONG CDataBaseSink::SPUserGetTaskInfo(DWORD dwUser)
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUser);
	if (m_pInitParamter)
	{
		m_AccountsDBAide.AddParameter(TEXT("@strLoginServer"),m_pInitParamter->m_szMainPage);
	}

	return m_AccountsDBAide.ExecuteProcess(TEXT("WHTreasureDB.dbo.GSP_GR_GetTaskStat_v2"),true);
}

//���а��¼
LONG CDataBaseSink::SPUserRanking(DWORD dwUserID, int nPage)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@nPage"),nPage);

	return m_AccountsDBAide.ExecuteProcess(TEXT("WHTreasureDB.dbo.GSP_GR_GetRanking"),true);
}

//���а��¼
LONG CDataBaseSink::SPUserRanking_V2(DWORD dwUserID, int nPage)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@nPage"),nPage);

	return m_AccountsDBAide.ExecuteProcess(TEXT("WHTreasureDB.dbo.GSP_GR_GetRanking_V2"),true);
}

//������а��Ƿ����
LONG CDataBaseSink::SPUserRankingStatus(DWORD dwUserID)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("WHTreasureDB.dbo.GSP_GR_GetUserRankingStatus"),true);
}

//�޸��Ƿ������а�
LONG CDataBaseSink::SPUserModifyRankingStatus(DWORD dwUserID, int nRankingStatus)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@RankingStatus"),nRankingStatus);

	return m_AccountsDBAide.ExecuteProcess(TEXT("WHTreasureDB.dbo.GSP_GR_ModifyRankingStatus"),true);
}

//��������
LONG CDataBaseSink::SPUserFeedBack(DWORD dwUser,  LPCTSTR pszContent, LPCTSTR pszPicture)
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUser);
	m_AccountsDBAide.AddParameter(TEXT("@Question"),pszContent);
	m_AccountsDBAide.AddParameter(TEXT("@AttachPath"),pszPicture);

	return m_AccountsDBAide.ExecuteProcess(TEXT("WHServerInfoDB.dbo.GSP_GR_CommitKeFu"),true);
}

//��ѯ��ҷ������
LONG CDataBaseSink::SPUserGetFeedBack(DWORD dwUserID, int nPage)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@nPage"),nPage);

	return m_AccountsDBAide.ExecuteProcess(TEXT("WHServerInfoDB.dbo.GSP_GR_GetKeFu"),true);
}

//��ѯ�û����֤�˺���Ϣ
LONG CDataBaseSink::SPUserInfoIDCard(DWORD dwUserID)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_GetIDCard"),true);
}

//�����ύ���֤��Ϣ
LONG CDataBaseSink::SPUserAuthentication(DWORD dwUserID, LPCTSTR pszName, LPCTSTR pszIDCard)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@IDName"),pszName);
	m_AccountsDBAide.AddParameter(TEXT("@IDCard"),pszIDCard);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_CommitIDCard"),true);
}

//�̳���Ʒ��Ϣ
LONG CDataBaseSink::SPUserMallProduct(DWORD dwUserID)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UserMallProduct"),true);
}

//�̳���Ʒ��Ϣ(ios)
LONG CDataBaseSink::SPUserMallProductIOS(DWORD dwUserID)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UserMallProduct_IOS"),true);
}

//�¶���
LONG CDataBaseSink::SPUserMallPlaceOrder(DWORD dwUserID, DWORD dwProductID,TCHAR szPayWay[10])
{
	//Ч�����
	ASSERT(dwUserID!=0L);
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@dwProductID"),dwProductID);
	m_AccountsDBAide.AddParameter(TEXT("@RechargeWay"),szPayWay);
	
	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UserMallPlaceOrder"),true);
}

//��ѯ�û�������Ϣ
LONG CDataBaseSink::SPUserBankInfo(DWORD dwUserID)
{
	//Ч�����
	ASSERT(dwUserID!=0L);
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
	
	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UserBankInfo"),true);
}

//�û���������
LONG CDataBaseSink::SPUserBankCharge(int nOpCode, DWORD dwUserID, DWORD dwCurScore, DWORD dwBankScore)
{
	//Ч�����
	ASSERT(dwUserID!=0L);
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@wOpCode"),nOpCode);
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@dwCurScore"),dwCurScore);
	m_AccountsDBAide.AddParameter(TEXT("@dwBankScore"),dwBankScore);
	
	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UserBankCharge"),true);
}

LONG CDataBaseSink::SPUserCancelOrder(TCHAR szAccount[20])
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwOrderID"),szAccount);
	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UserCancelOrder"),true);
}
// �̳ǹ�����Ʒ���
LONG CDataBaseSink::SPUserMallBuy(TCHAR orderNum[20])
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwOrderNum"), orderNum);
	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UserMallBuyResult"),true);
}
// �̳Ǹ��¹�����
LONG CDataBaseSink::SPUserMallUpdateBuy(TCHAR orderNum[20])
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwOrderNum"), orderNum);
	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_UserMallUpdateResult"),true);
}

// ��ѯ��б�
LONG CDataBaseSink::SPUserActivityList()
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_GetActivityList"),true);
}

//��ѯ�
LONG CDataBaseSink::SPUserActivity(DWORD dwUserID, DWORD dwActivityID)
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();

	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@dwActivityID"), dwActivityID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_GetActivity"),true);
}

//�齱�б�
LONG CDataBaseSink::SPUserActivityLuckyList(DWORD dwUserID, DWORD dwActivityID)
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();

	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@dwActivityID"), dwActivityID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_GetActivityLuckyList"),true);
}

//�齱
LONG CDataBaseSink::SPUserActivityLucky(DWORD dwUserID, DWORD dwActivityID, DWORD dwPrizeID)
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();

	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"), dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@dwActivityID"), dwActivityID);
	m_AccountsDBAide.AddParameter(TEXT("@dwPrizeID"), dwPrizeID);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_GetActivityLucky"),true);
}

//�齱��¼
LONG CDataBaseSink::SPUserActivityRecord(DWORD dwUserID, DWORD dwActivityID, int nPage)
{
	//Ч�����
	ASSERT(dwUserID!=0L);

	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@dwActivityID"),dwActivityID);
	m_AccountsDBAide.AddParameter(TEXT("@nPage"),nPage);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_GetActivityRecord"),true);
}


//������
LONG CDataBaseSink::SPUserSpeakerSend(DWORD dwUserID, WORD wType, LPCTSTR pszContent)
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@wType"),wType);
	m_AccountsDBAide.AddParameter(TEXT("@szMsg"),pszContent);

	return m_AccountsDBAide.ExecuteProcess(TEXT("WHServerInfoDB.dbo.GSP_GP_UserSpeakerSend"),true);
}
//��ѯ������
LONG CDataBaseSink::SPUserQueryConfig(DWORD dwUserID, DWORD dwVersion, DWORD dwConfigID, LPCTSTR pszChannel)
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@dwVersion"),dwVersion);
	m_AccountsDBAide.AddParameter(TEXT("@dwConfigID"),dwConfigID);
	m_AccountsDBAide.AddParameter(TEXT("@szChannel"),pszChannel);

	return m_AccountsDBAide.ExecuteProcess(TEXT("WHServerInfoDB.dbo.GSP_UserQueryConfig"),true);
}

//////////////////////////////////////////////////////////////////////////
