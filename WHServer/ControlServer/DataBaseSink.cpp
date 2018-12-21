#include "StdAfx.h"
#include "Commonality.h"
#include "DataBaseSink.h"
#include "ControlServer.h"


//��������
extern CControlServerApp theApp;
extern void TraceMessage(LPCTSTR pszMessage);
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
	case DBR_GP_CHECK_GM_TASK:
		{
			return OnRequestCheckGMTask(wRequestID,dwContextID,pData,wDataSize);
		}
		break;
	case DBR_GP_SPEAKER_SEND:
		{
			return OnRequestSpeakerSend(wRequestID,dwContextID,pData,wDataSize);
		}
		break;
	case DBR_GP_CREATE_RANKING:
		{
			return OnRequestCreateRanking(wRequestID,dwContextID,pData,wDataSize);
		}
		break;
	case DBR_GP_WRITE_ONLINE:
		{
			return OnRequestWriteOnline(wRequestID,dwContextID,pData,wDataSize);
		}
		break;
	}

	return false;
}


//GMTask������
bool CDataBaseSink::OnRequestCheckGMTask(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//��������
	LONG lReturnValue=-1;
	try
	{
		//��鶨ʱ����
		lReturnValue=SPCheckGMTask();
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString("SPCheckGMTask exception",TraceLevel_Exception);
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}
	//����ж�
	if (lReturnValue!=0L)
	{
		CTraceService::TraceString("SPCheckGMTask fail",TraceLevel_Normal);
	}
	else
	{
		static int iCnt = 0;
		//ѹ����־����Ҫÿ�ζ���ʾ
		if (iCnt % 15 == 0)
		{
			CTraceService::TraceString("SPCheckGMTask OK",TraceLevel_Normal);
		}
		iCnt++;
	}

	try
	{
		//���ϵͳ����
		lReturnValue=SPCheckSpeakerTask();
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		CTraceService::TraceString("SPCheckSpeakerTask exception",TraceLevel_Exception);
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//����ж�
	if (lReturnValue!=0L)
	{
		CTraceService::TraceString("SPCheckSpeakerTask fail",TraceLevel_Normal);
	}
	else
	{
		//��ȡ����
		IDataBase * pIDataBase=(IDataBase *)m_AccountsDBAide.GetDataBase(IID_IDataBase,VER_IDataBase);

		int iCnt=0;
		//��ȡ��Ϣ
		while (pIDataBase->IsRecordsetEnd()==false)
		{
			DBR_GP_Speaker_Task_Result record;
			ZeroMemory(&record, sizeof(record));

			//��ȡ��Ϣ
			record.dwUserID=m_AccountsDBAide.GetValue_DWORD(TEXT("UserID"));
			m_AccountsDBAide.GetValue_String(TEXT("Params"),record.szMsg,CountArray(record.szMsg));
			record.nLen = std::string(record.szMsg).size();

			iCnt++;
			m_pIDataBaseEngineEvent->OnEventDataBaseResult(DBR_GP_CHECK_SPEAKER_TASK_RESULT,dwContextID,
							&record, sizeof(record));
			char buf[256];
			memset(buf, 0, 256);
			sprintf(buf, "msg:%s", record.szMsg);
			TraceMessage(buf);

			CTraceService::TraceString("Send Speaker OK",TraceLevel_Normal);

			//�ƶ���¼
			pIDataBase->MoveToNext();
		}

		//�رռ�¼
		pIDataBase->CloseRecordset();
	}

	return true;
}

//�������а�
bool CDataBaseSink::OnRequestCreateRanking(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	try
	{
		//��������
		LONG lReturnValue=-1;
		lReturnValue=SPCreateRanking();
		
		//����ж�
		if (lReturnValue!=0L)
		{
			CTraceService::TraceString("SPCreateRanking fail",TraceLevel_Normal);
		}
		else
		{
			CTraceService::TraceString("SPCreateRanking OK",TraceLevel_Normal);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�رռ�¼��
	m_AccountsDBModule->CloseRecordset();

	return true;
}

//д�������
bool CDataBaseSink::OnRequestWriteOnline(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GR_Online));
	if (wDataSize!=sizeof(DBR_GR_Online)) return false;

	//����ת��
	DBR_GR_Online *pOnline = (DBR_GR_Online*)pData;

	try
	{
		//��������
		LONG lReturnValue=-1;
		lReturnValue=SPWriteOnline(pOnline->dwTypeID, pOnline->dwValue, pOnline->szCollectDate);
		
		//����ж�
		if (lReturnValue!=0L)
		{
			CTraceService::TraceString("SPWriteOnline fail",TraceLevel_Normal);
		}
		else
		{
			CTraceService::TraceString("SPWriteOnline OK",TraceLevel_Normal);
		}
	}
	catch (IDataBaseException * pIException)
	{
		//�������
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	//�رռ�¼��
	m_AccountsDBModule->CloseRecordset();

	return true;
}

//д����
bool CDataBaseSink::OnRequestSpeakerSend(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//������֤
	ASSERT(wDataSize==sizeof(DBR_GP_SpeakerSend));
	if (wDataSize!=sizeof(DBR_GP_SpeakerSend)) return false;

	//����ת��
	DBR_GP_SpeakerSend *pReq = (DBR_GP_SpeakerSend*)pData;

	//ִ�д洢����
	LONG lReturnValue = -1;
	try
	{
		lReturnValue = SPSpeakerSend(pReq->dwUserID, pReq->iType, pReq->szDate);
	}
	catch (IDataBaseException * pIException)
	{
		//������Ϣ
		LPCTSTR pszDescribe=pIException->GetExceptionDescribe();
		CTraceService::TraceString(pszDescribe,TraceLevel_Exception);
	}

	return true;
}

//����������
LONG CDataBaseSink::SPCheckGMTask()
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();

	return m_AccountsDBAide.ExecuteProcess(TEXT("WHGameUserDB.dbo.GSP_GP_CheckGMTask"),true);
}

//ϵͳ����������
LONG CDataBaseSink::SPCheckSpeakerTask()
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();

	return m_AccountsDBAide.ExecuteProcess(TEXT("WHGameUserDB.dbo.GSP_GP_CheckSpeakerTask"),true);
}

//�������а�
LONG CDataBaseSink::SPCreateRanking()
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();

	return m_AccountsDBAide.ExecuteProcess(TEXT("WHTreasureDB.dbo.GSP_GR_WriteRanking"),true);
}

//д�������
LONG CDataBaseSink::SPWriteOnline(int type, int value, LPCTSTR collectDate)
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwTypeID"),type);
	m_AccountsDBAide.AddParameter(TEXT("@dwValue"),value);
	m_AccountsDBAide.AddParameter(TEXT("@DateCollect"),collectDate);

	return m_AccountsDBAide.ExecuteProcess(TEXT("WHServerInfoDB.dbo.GSP_GR_CommitOnline"),true);
}

//������
LONG CDataBaseSink::SPSpeakerSend(DWORD dwUserID, WORD wType, LPCTSTR pszContent)
{
	//ִ�д洢����
	m_AccountsDBAide.ResetParameter();
	m_AccountsDBAide.AddParameter(TEXT("@dwUserID"),dwUserID);
	m_AccountsDBAide.AddParameter(TEXT("@wType"),wType);
	m_AccountsDBAide.AddParameter(TEXT("@szMsg"),pszContent);

	return m_AccountsDBAide.ExecuteProcess(TEXT("WHServerInfoDB.dbo.GSP_GP_UserSpeakerSend"),true);
}

//////////////////////////////////////////////////////////////////////////
