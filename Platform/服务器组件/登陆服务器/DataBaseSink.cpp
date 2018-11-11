#include "StdAfx.h"
#include "Commonality.h"
#include "DataBaseSink.h"
#include "LogonServer.h"
#include "TraceCatch.h"

//��������
extern CLogonServerApp theApp;
//////////////////////////////////////////////////////////////////////////

//�궨��

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
	__ENTER_FUNCTION

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

	__LEAVE_FUNCTION

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
	//case DBR_GP_LOGON_BY_USERID:		//I D ��¼
	//	{
	//		return OnRequestLogon(wRequestID,dwContextID,pData,wDataSize);
	//	}
	//case DBR_GP_REGISTER_ACCOUNTS:		//ע���ʺ�
	//	{
	//		return OnRequestLogon(wRequestID,dwContextID,pData,wDataSize);
	//	}
	case DBR_GP_GET_GIFT_GLOD:
		{
			return OnGiftGlod(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_UPDATE_USERINFO:
		{
			return UpdateUserInfo(wRequestID,dwContextID,pData,wDataSize);
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
	case DBR_GP_READ_GRADE_INFO:  //��ȡ�ȼ���Ϣ
		{
			return OnReadGradeInfo(wRequestID,dwContextID,pData,wDataSize);
		}
	case DBR_GP_CONTINUE_CONNECT:	//�������ݿ�����		Author:<cxf>
		{
			return ContinueConnect();
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
		DBR_GP_LogonByAccounts * pLogonByAccounts=(DBR_GP_LogonByAccounts *)pData;

		//��¼����
		switch (wRequestID)
		{
		case DBR_GP_LOGON_BY_ACCOUNTS:		//�ʺŵ�¼
			{
				//Ч�����
				ASSERT(wDataSize==sizeof(DBR_GP_LogonByAccounts));
				if (wDataSize!=sizeof(DBR_GP_LogonByAccounts)) return false;

				//ִ�в�ѯ
				lReturnValue=SPLogonByAccounts(pLogonByAccounts->szAccounts,pLogonByAccounts->szPassWord,
											   pLogonByAccounts->dwClientIP,pLogonByAccounts->szComputerID);
				pszPassword=pLogonByAccounts->szPassWord;

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
			LogonSuccess.cbGender=m_AccountsDBAide.GetValue_BYTE(TEXT("Gender"));
			LogonSuccess.cbMember=m_AccountsDBAide.GetValue_BYTE(TEXT("MemberOrder"));
			LogonSuccess.dwExperience=m_AccountsDBAide.GetValue_DWORD(TEXT("Experience"));
			LogonSuccess.dwCustomFaceVer=m_AccountsDBAide.GetValue_DWORD(TEXT("CustomFaceVer"));
			LogonSuccess.dwLockServerID = m_AccountsDBAide.GetValue_DWORD(TEXT("ServerID"));
			m_AccountsDBAide.GetValue_String(TEXT("Accounts"),LogonSuccess.szAccounts,CountArray(LogonSuccess.szAccounts));
			m_AccountsDBAide.GetValue_String(TEXT("UnderWrite"),LogonSuccess.szUnderWrite,CountArray(LogonSuccess.szUnderWrite));
			m_AccountsDBAide.GetValue_String(TEXT("ErrorDescribe"),LogonSuccess.szDescribeString,CountArray(LogonSuccess.szDescribeString));

            // �������
			m_AccountsDBAide.GetValue_String(TEXT("NickName"), LogonSuccess.szNickName, CountArray(LogonSuccess.szNickName));
			LogonSuccess.lMoney   = m_AccountsDBAide.GetValue_LONGLONG("lMoney");

			LogonSuccess.lGold       = m_AccountsDBAide.GetValue_LONGLONG("lGold");
			LogonSuccess.lGem      = m_AccountsDBAide.GetValue_LONGLONG("lGem");
			LogonSuccess.dwGrade     = m_AccountsDBAide.GetValue_DWORD("dwGrade");

			LogonSuccess.lWinCount   = m_AccountsDBAide.GetValue_LONG("WinCount");							//ʤ������
			LogonSuccess.lLostCount  = m_AccountsDBAide.GetValue_LONG("LostCount");							//ʧ������
			LogonSuccess.lDrawCount  = m_AccountsDBAide.GetValue_LONG("DrawCount");							//�;�����
			LogonSuccess.lFleeCount  = m_AccountsDBAide.GetValue_LONG("FleeCount");							//������Ŀ
            LogonSuccess.utIsAndroid = (USER_TYPE)m_AccountsDBAide.GetValue_INT("IsAndroid"); 
	 		m_AccountsDBAide.GetValue_String(TEXT("SsoMD5"),  LogonSuccess.szKey, CountArray(LogonSuccess.szKey));    
			LogonSuccess.lGiftNum    = m_AccountsDBAide.GetValue_INT("GiftCoumt");
			m_AccountsDBAide.GetValue_String(TEXT("HashID"), LogonSuccess.szHashID, CountArray(LogonSuccess.szHashID));
			//��ӽ���

			//��������
			CopyMemory(LogonSuccess.szPassword,pszPassword,sizeof(LogonSuccess.szPassword));
			LogonSuccess.nMasterRight  = m_AccountsDBAide.GetValue_LONG("MasterRight");							//�û�Ȩ��
			LogonSuccess.nMasterOrder  = m_AccountsDBAide.GetValue_LONG("MasterOrder");							//Ȩ�޵ȼ�
			
			CopyMemory(LogonSuccess.szComputerID, pLogonByAccounts->szComputerID, sizeof(LogonSuccess.szComputerID));

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
		lstrcpyn(LogonError.szErrorDescribe,TEXT("������������ʧ�ܣ������Ժ����Ի�ѡ����һ��������¼��"),CountArray(LogonError.szErrorDescribe));

		//Ͷ�ݽ��
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_LOGON_ERROR,dwContextID,&LogonError,sizeof(LogonError));
	}

	//�رռ�¼��
	m_AccountsDBModule->CloseRecordset();
	return true;
}

bool CDataBaseSink::OnGiftGlod(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof( DBR_GP_GiftGold));
	if (wDataSize!=sizeof( DBR_GP_GiftGold)) return false;
	LONG lRet = 0;
	try
	{
		DBR_GP_GiftGold   giftMoney;
		memcpy(&giftMoney, pData, wDataSize);

		//ת����ַ
		TCHAR szClientIP[16]=TEXT("");
		BYTE * pClientIP=(BYTE *)&giftMoney.dwClientIP;
		_snprintf(szClientIP,sizeof(szClientIP),TEXT("%d.%d.%d.%d"),pClientIP[0],pClientIP[1],pClientIP[2],pClientIP[3]);

		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),     giftMoney.dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@PassWord"),   giftMoney.szPassWord);
		m_AccountsDBAide.AddParameter(TEXT("@strClientIP"),szClientIP);
		lRet = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_GiftCurrency"),true);
		
		DBR_GP_GiftGoldResult  gr; 
		gr.lRet = lRet;
		gr.dwGold = m_AccountsDBAide.GetValue_DWORD("ChangeGlod");
		m_AccountsDBAide.GetValue_String(TEXT("Describe"), gr.szDescribe, CountArray(gr.szDescribe));//��ʾ
		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_GIFT_MONEY_RESULT, dwContextID,
													   &gr,					  sizeof(DBR_GP_GiftGoldResult));
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}
	return false;
}


//�����û���Ϣ
bool CDataBaseSink::UpdateUserInfo(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_UpdateUserInfo));
	if (wDataSize!=sizeof(DBR_GP_UpdateUserInfo)) return false;
	LONG lReturnValue=-1;
	try
	{
		DBR_GP_UpdateUserInfo   userInfo;
		memcpy(&userInfo, pData, wDataSize);
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.AddParameter(TEXT("@UserID"),   userInfo.dwUserID);
		m_AccountsDBAide.AddParameter(TEXT("@PassWord"), userInfo.szPassWord);
		lReturnValue = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ResetGetUserData"),true);
		if (lReturnValue)
		{
			//Ͷ�ݽ��
			DBR_GP_UserInfoSucc  userInfoSucc;
			userInfoSucc.cbGender   = m_AccountsDBAide.GetValue_BYTE("Gender");
			userInfoSucc.wFaceID    = m_AccountsDBAide.GetValue_WORD("FaceID");
			m_AccountsDBAide.GetValue_String(TEXT("NickName"),userInfoSucc.szNickName,CountArray(userInfoSucc.szNickName));
			userInfoSucc.lMoney     = m_AccountsDBAide.GetValue_LONGLONG("Money");
			userInfoSucc.lGem       = m_AccountsDBAide.GetValue_LONGLONG("Gems");
			userInfoSucc.lGold      = m_AccountsDBAide.GetValue_LONGLONG("Gold");
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_UPDATE_USER_SUCC, dwContextID,
				                                           &userInfoSucc,  sizeof(DBR_GP_UserInfoSucc));
			return true;	
		}else
		{
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_UPDATE_USER_FAIL, dwContextID,
														   0,    0);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}
	return false;
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
	__ENTER_FUNCTION

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

	__LEAVE_FUNCTION

	return true;
}

//�޸�����
bool CDataBaseSink::OnRequestModifyIndividual(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	__ENTER_FUNCTION


	//������֤
	ASSERT(wDataSize == sizeof(DBR_GP_ModifyIndividual));
	if ( wDataSize != sizeof(DBR_GP_ModifyIndividual)) return false;

	//����ת��
	DBR_GP_ModifyIndividual *pModifyIndividual = (DBR_GP_ModifyIndividual*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPModifyIndividual(pModifyIndividual->dwUserID, pModifyIndividual->szNickname, pModifyIndividual->nGender,
			pModifyIndividual->nAge, pModifyIndividual->szAddress, pModifyIndividual->szUnderWrite, pModifyIndividual->szPassword);
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
		lstrcpyn(ModifyIndividualResult.szDescribeMsg, TEXT("�����޸ĳɹ���"), sizeof(ModifyIndividualResult.szDescribeMsg));
		ModifyIndividualResult.bOperateSuccess = true;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_MODIFY_RESULT,dwContextID,
			&ModifyIndividualResult,sizeof(ModifyIndividualResult));
	}
	else
	{
		DBR_GP_ModifyIndividualResult ModifyIndividualResult;
		ZeroMemory(&ModifyIndividualResult, sizeof(ModifyIndividualResult));
		lstrcpyn(ModifyIndividualResult.szDescribeMsg, TEXT("�������ݿ�����쳣���޸�ʧ�ܣ�"), sizeof(ModifyIndividualResult.szDescribeMsg));
		ModifyIndividualResult.bOperateSuccess = false;

		m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_MODIFY_RESULT,dwContextID,
			&ModifyIndividualResult,sizeof(ModifyIndividualResult));
	}

	__LEAVE_FUNCTION

	return true;
}

//��ȡ�ȼ���Ϣ
bool CDataBaseSink::OnReadGradeInfo(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		LONG reValue = 0;
		m_AccountsDBAide.ResetParameter();
		reValue = m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_GetGradeList"),true);
		if (reValue > 0)
		{
			DBR_GR_gradeInfo  gradeInfo;
			ZeroMemory(&gradeInfo, sizeof(gradeInfo));
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_GRADE_INFO_NUM, 0, (void*)&reValue, sizeof(LONG));
			//��ȡ��¼
			m_AccountsDBModule->MoveToFirst();
			while ( m_AccountsDBModule->IsRecordsetEnd() == false )
			{
				gradeInfo.id = m_AccountsDBAide.GetValue_INT("ID");
				m_AccountsDBAide.GetValue_String(TEXT("Name"), gradeInfo.szName, CountArray(gradeInfo.szName));
				gradeInfo.iMaxExp = m_AccountsDBAide.GetValue_INT("Experience");
				//Ͷ�ݵ���֪ͨ
				m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GR_GRADE_INFO_ONE, 0, &gradeInfo, sizeof(DBR_GR_gradeInfo));
				//�ƶ���¼
				m_AccountsDBModule->MoveToNext();
			}

		}else
		{
			CTraceService::TraceString("û�еȼ���Ϣ", TraceLevel_Normal);
		}
	}
	catch( IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}
	//�رռ�¼��)
	m_AccountsDBModule->CloseRecordset();
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
	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_EfficacyAccounts"),true);
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
LONG CDataBaseSink::SPModifyIndividual(DWORD dwUserID, TCHAR szNickname[NAME_LEN], int nGender, int nAge, TCHAR szAddress[64], TCHAR szUnderWrite[32], TCHAR szPassword[32])
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@strLogonPass"),szPassword);
	m_AccountsDBAide.AddParameter(TEXT("@strNickName"),szNickname);
	m_AccountsDBAide.AddParameter(TEXT("@nGender"),nGender);
	m_AccountsDBAide.AddParameter(TEXT("@nAge"),nAge);
	m_AccountsDBAide.AddParameter(TEXT("@strAddress"),szAddress);
	m_AccountsDBAide.AddParameter(TEXT("@strUnderWrite"),szUnderWrite);

	return m_AccountsDBAide.ExecuteProcess(TEXT("GSP_CF_ModifyIndividual"),false);
}

//	Author: <cxf>
//	CrateDate: <2011-12-22>
//	Description: <�������ݿ����ӳ�һֱ����>
bool CDataBaseSink::ContinueConnect()
{
	try
	{//ִ�д洢����
		m_AccountsDBAide.ResetParameter();
		m_AccountsDBAide.ExecuteProcess(TEXT("GSP_GP_ContinueConnect"), true);
		return true;
	}
	catch(IDataBaseException * pIException)
	{
		//�������
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
