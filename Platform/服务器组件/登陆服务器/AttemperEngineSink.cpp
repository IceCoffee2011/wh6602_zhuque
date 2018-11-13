#include "StdAfx.h"
#include "AttemperEngineSink.h"
#include "Zip.h"
#include "LogonServer.h"
#include "TraceCatch.h"

#define MANY_ENTER	0 // �࿪

//��������
extern CLogonServerApp theApp;
//////////////////////////////////////////////////////////////////////////

//�궨��
#define TIME_RECONNECT						15000L						//����ʱ��
#define TIME_UPDATE_LIST					30000L						//����ʱ��
#define TIME_DOWNLOAD_CUSTOM_FACE			200L						//����ͷ��
#define TIME_READ_GRADE_INFO                500L                        //��ȡ�ȼ���Ϣ
#define TIME_CONTINUE_CONNECT				(1000*600)					//�������ݿ�ʱ��						Author:<cxf>

//��ʱ�� ID
#define IDI_CONNECT_CENTER_SERVER			1							//���ӷ�����
#define IDI_UPDATE_SERVER_LIST				2							//�����б�
#define IDI_DOWNLOAD_CUSTOM_FACE			3							//����ͷ��

#define IDI_READ_GRADE_INFO                 4                           //��ȡ�ȼ���Ϣ
#define IDI_CONTINUE_CONNECT				5							//�������ݿⶨʱ��ID					Author:<cxf>

//////////////////////////////////////////////////////////////////////////

//���캯��
CAttemperEngineSink::CAttemperEngineSink()
{
	//���ñ���
	m_pInitParamter=NULL;
	m_pBindParameter=NULL;

	//���ñ���
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketCorrespond=NULL;

	m_pGradeInfo = 0;
	m_iGradeNum  = 0;
	return;
}

//��������
CAttemperEngineSink::~CAttemperEngineSink()
{
	delete m_pGradeInfo;
	m_pGradeInfo = 0;

	MMachine::iterator M_const_return = m_hmMachineCode.begin();
	for (; m_hmMachineCode.end() != M_const_return; M_const_return++)
	{
		MachineData * pData = M_const_return->second;
		delete pData;
	}
	m_hmMachineCode.clear();
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
	__ENTER_FUNCTION
	//�������
	m_ServerList.SetSocketEngine(m_pITCPNetworkEngine);

	//�󶨲���
	m_pBindParameter=new tagBindParameter[m_pInitParamter->m_wMaxConnect];
	ZeroMemory(m_pBindParameter,sizeof(tagBindParameter)*m_pInitParamter->m_wMaxConnect);

	//��������
	DWORD dwServerIP=inet_addr(m_pInitParamter->m_szCenterServerAddr);

#if 1
	// dwServerIP ֵ�Ƿ��ģ�����Ϊ�ι���Ҫ��һ����ֵ��
	m_pITCPSocketCorrespond->Connect(ntohl(dwServerIP),PORT_CENTER_SERVER);
#else
	m_pITCPSocketCorrespond->Connect(m_pInitParamter->m_szCenterServerAddr,PORT_CENTER_SERVER);
#endif

	//��ʼ����
	//m_DownloadFaceInfoMap.InitHashTable(503);
	//m_DownloadFaceInfoMap.RemoveAll();

	//���ö�ʱ��
	//m_pITimerEngine->SetTimer(IDI_DOWNLOAD_CUSTOM_FACE,TIME_DOWNLOAD_CUSTOM_FACE,TIMES_INFINITY,NULL);


	m_pITimerEngine->SetTimer(IDI_READ_GRADE_INFO, TIME_READ_GRADE_INFO, 1,NULL);

	m_pITimerEngine->SetTimer(IDI_CONTINUE_CONNECT, TIME_CONTINUE_CONNECT, TIMES_INFINITY, NULL);//Author:<cxf>

	__LEAVE_FUNCTION
	return true;
}

//����ģ��ر�
bool __cdecl CAttemperEngineSink::OnAttemperEngineStop(IUnknownEx * pIUnknownEx)
{
	//���ñ���
	m_pITimerEngine=NULL;
	m_pIDataBaseEngine=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pITCPSocketCorrespond=NULL;

	//ɾ������
	SafeDeleteArray(m_pBindParameter);
	m_hmMachineCode.clear();
	return true;
}

//��ʱ���¼�
bool __cdecl CAttemperEngineSink::OnEventTimer(DWORD dwTimerID, WPARAM wBindParam)
{
	__ENTER_FUNCTION
	switch (dwTimerID)
	{
	case IDI_CONNECT_CENTER_SERVER:		//�������ķ�����
		{
			//��������
			DWORD dwServerIP=inet_addr(m_pInitParamter->m_szCenterServerAddr);

			// add by HouGuoJiang 2011-11-25
			#if 1
				// dwServerIP ֵ�Ƿ��ģ�����Ϊ�ι���Ҫ��һ����ֵ��
				m_pITCPSocketCorrespond->Connect(ntohl(dwServerIP),PORT_CENTER_SERVER);
			#else
				m_pITCPSocketCorrespond->Connect(m_pInitParamter->m_szCenterServerAddr,PORT_CENTER_SERVER);
			#endif

			//������ʾ
			CTraceService::TraceString(TEXT("���ڳ��������������ķ�����...."),TraceLevel_Normal);
			return true;
		}
	case IDI_UPDATE_SERVER_LIST:		//���·������б�
		{
			m_pITCPSocketCorrespond->SendData(MDM_CS_SERVER_LIST,SUB_CS_GET_SERVER_LIST,NULL,0);

			return true;
		}

	case IDI_READ_GRADE_INFO: //��ȡ�ȼ���Ϣ
		{
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_READ_GRADE_INFO, 0, 0, 0);
			return true;
		}
	case IDI_CONTINUE_CONNECT://���������ݿ�(�������ݿ�����)
		{
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_CONTINUE_CONNECT, 0, 0, 0);			//Author:<cxf>
		}
	}
	__LEAVE_FUNCTION
	return false;
}

//���ݿ��¼�
bool __cdecl CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBR_GP_LOGON_ERROR:			//��¼ʧ��
		{
			return OnDBLogonError(dwContextID,pData,wDataSize);
		}
	case DBR_GP_LOGON_SUCCESS:			//��¼�ɹ�
		{
			return OnDBLogonSuccess(dwContextID,pData,wDataSize);
		}
	case DBR_GP_UPDATE_USER_SUCC:       //�����û���Ϣ�ɹ�
		{
			return OnUpdateUserInfoSucc(dwContextID,pData,wDataSize);
		}
	case DBR_GP_UPDATE_USER_FAIL: //����ʧ��
		{
			return OnUpdateUserInfoFail(dwContextID,pData,wDataSize);
		}
	case DBR_GP_DOWNLOADFACE_SUCCESS:	//���سɹ�
		{
			return OnDBDownloadFaceSuccess(dwContextID,pData,wDataSize);
		}
	case DBR_GP_UPLOAD_FACE_RESULT:		//�ϴ����
		{
			return OnDBRCustomFaceUploadResult(dwContextID,pData,wDataSize);
		}
	case DBR_GP_DELETE_FACE_RESULT:		//ɾ�����
		{
			return OnDBRCustomFaceDeleteResult(dwContextID,pData,wDataSize);
		}
	case DBR_GP_MODIFY_RESULT:			//�޸Ľ��
		{
			return OnDBRModifyIndividual(dwContextID,pData,wDataSize);
		}
	case DBR_GR_GRADE_INFO_NUM: //�ȼ�����Ŀ
		{
			return OnDBRGradeInfoNum(dwContextID, pData, wDataSize);
		}
	case DBR_GR_GRADE_INFO_ONE: //�õ��ȼ���Ŀ
		{
			return OnDBRGradeInfoOne(dwContextID, pData, wDataSize);
		}

	 case DBR_GP_GIFT_MONEY_RESULT: //ÿ��һ�ε������ȡ
		 {
			 return OnDBRGiftMoneyResult(dwContextID, pData, wDataSize);
		 }
	}

	return false;
}

//�û���¼�ɹ�
bool CAttemperEngineSink::OnDBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	__ENTER_FUNCTION
	/*char info[BUFSIZ] = {0};
	sprintf(info, _T("OnDBLogonSuccess"));
	CTraceService::TraceString(info, TraceLevel_Normal);*/

	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GP_LogonSuccess));
	if (wDataSize!=sizeof(DBR_GP_LogonSuccess)) return false;

	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParamter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	BYTE cbBuffer[SOCKET_PACKET] = {0};
	DBR_GP_LogonSuccess * pDBRLogonSuccess=(DBR_GP_LogonSuccess *)pData;
	CMD_GP_LogonSuccess * pCMDLogonSuccess=(CMD_GP_LogonSuccess *)cbBuffer;

#if MANY_ENTER	// ��ʱȥ���࿪�� // del by HouGuoJiang 2012-03-27
	if (ANDROID != pDBRLogonSuccess->utIsAndroid)
	{
		MMachine::iterator M_const_return = m_hmMachineCode.find(pDBRLogonSuccess->szComputerID);
		if (m_hmMachineCode.end() == M_const_return)
		{
			tagMachineCode * pMachinecode = new tagMachineCode;
			pMachinecode->SetVal(pDBRLogonSuccess->szAccounts);
			m_hmMachineCode.insert(std::pair<std::string, MachineData *>(pDBRLogonSuccess->szComputerID, pMachinecode));
		}
		else
		{
			// ͬһ̨����3Сʱ�ڣ���ֹ��¼������ͬ���˻�
			if (M_const_return->second->compare(pDBRLogonSuccess->szAccounts, 3600*3))
			{
				DBR_GP_LogonError LogonError;
				ZeroMemory(&LogonError,sizeof(LogonError));
				LogonError.lErrorCode=-1; 
				lstrcpyn(LogonError.szErrorDescribe,TEXT("��Ǹ��һ̨����ֻ�����¼һ���ͻ��ˡ�"),CountArray(LogonError.szErrorDescribe));
				OnDBLogonError(dwContextID, &LogonError, sizeof(LogonError));
				return true;
			}
		}
	}
#endif


	//��������
	pCMDLogonSuccess->wFaceID=pDBRLogonSuccess->wFaceID;
	pCMDLogonSuccess->cbGender=pDBRLogonSuccess->cbGender;
	pCMDLogonSuccess->cbMember=pDBRLogonSuccess->cbMember;
	pCMDLogonSuccess->dwUserID=pDBRLogonSuccess->dwUserID;
	pCMDLogonSuccess->dwGameID=pDBRLogonSuccess->dwGameID;
	pCMDLogonSuccess->dwExperience=pDBRLogonSuccess->dwExperience;
	pCMDLogonSuccess->dwCustomFaceVer=pDBRLogonSuccess->dwCustomFaceVer;
	pCMDLogonSuccess->dwLockServerID =pDBRLogonSuccess->dwLockServerID;

	//�������
	lstrcpyn(pCMDLogonSuccess->szNickName, pDBRLogonSuccess->szNickName, CountArray(pCMDLogonSuccess->szNickName));	
	lstrcpyn(pCMDLogonSuccess->szKey, pDBRLogonSuccess->szKey, CountArray(pCMDLogonSuccess->szKey));		
	pCMDLogonSuccess->lMoney       = pDBRLogonSuccess->lMoney ; 											
	pCMDLogonSuccess->lGold        = pDBRLogonSuccess->lGold;                              
	pCMDLogonSuccess->lGem         = pDBRLogonSuccess->lGem;                             
	pCMDLogonSuccess->dwGrade      = pDBRLogonSuccess->dwGrade; 

	pCMDLogonSuccess->lWinCount    = pDBRLogonSuccess->lWinCount;							//ʤ������
	pCMDLogonSuccess->lLostCount   = pDBRLogonSuccess->lLostCount;							//ʧ������
	pCMDLogonSuccess->lDrawCount   = pDBRLogonSuccess->lDrawCount;							//�;�����
	pCMDLogonSuccess->lFleeCount   = pDBRLogonSuccess->lFleeCount;							//������Ŀ
	pCMDLogonSuccess->utIsAndroid  = pDBRLogonSuccess->utIsAndroid;							//�Ƿ��ǻ�����
	pCMDLogonSuccess->lGiftNum     = pDBRLogonSuccess->lGiftNum; 
	pCMDLogonSuccess->nMasterRight = pDBRLogonSuccess->nMasterRight;						//Ȩ�޹���
	pCMDLogonSuccess->nMasterOrder = pDBRLogonSuccess->nMasterOrder;						//Ȩ�޵ȼ�
	//��ӽ���

	// add by HouGuoJiang 2011-11-25
	_snprintf(pCMDLogonSuccess->szHashID,  sizeof(pCMDLogonSuccess->szHashID), "%s", pDBRLogonSuccess->szHashID);

	//��������
	CSendPacketHelper SendPacket(cbBuffer+sizeof(CMD_GP_LogonSuccess),sizeof(cbBuffer)-sizeof(CMD_GP_LogonSuccess));
	SendPacket.AddPacket(pDBRLogonSuccess->szPassword,CountStringBuffer(pDBRLogonSuccess->szPassword),DTP_USER_PASS);
	SendPacket.AddPacket(pDBRLogonSuccess->szAccounts,CountStringBuffer(pDBRLogonSuccess->szAccounts),DTP_USER_ACCOUNTS);
	SendPacket.AddPacket(pDBRLogonSuccess->szUnderWrite,CountStringBuffer(pDBRLogonSuccess->szUnderWrite),DTP_UNDER_WRITE);

	//������Ϣ
	if (pDBRLogonSuccess->szGroupName[0]!=0)
	{
		SendPacket.AddPacket(pDBRLogonSuccess->szGroupName,CountStringBuffer(pDBRLogonSuccess->szGroupName),DTP_USER_GROUP_NAME);
	}

	//վ����ҳ
	if (m_pInitParamter->m_szMainPage[0]!=0)
	{
		SendPacket.AddPacket(m_pInitParamter->m_szMainPage,CountStringBuffer(m_pInitParamter->m_szMainPage),DTP_STATION_PAGE);
	}

	//���͵�¼���
	WORD wSendSize=sizeof(CMD_GP_LogonSuccess)+SendPacket.GetDataSize();
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_SUCCESS,cbBuffer,wSendSize);

	/*sprintf(info, _T("OnDBLogonSuccess SUB_GP_LOGON_SUCCESS"));
	CTraceService::TraceString(info, TraceLevel_Normal);*/

	//�б�����
	CMD_GP_ListConfig ListConfig;
	ListConfig.bShowOnLineCount=TRUE;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_SERVER_LIST,SUB_GP_LIST_CONFIG,&ListConfig,sizeof(ListConfig));

	/*sprintf(info, _T("OnDBLogonSuccess SUB_GP_LIST_CONFIG"));
	CTraceService::TraceString(info, TraceLevel_Normal);*/
	//������Ϸ�б�
	m_ServerList.SendGameTypeList(dwContextID);
	m_ServerList.SendGameKindList(dwContextID);
	m_ServerList.SendGameStationList(dwContextID);
	m_ServerList.SendGameServerList(dwContextID);

	m_pITCPNetworkEngine->SendData(dwContextID,  MDM_GP_LOGON, SUB_GP_LOGON_USER_GRADE_INFO, //���͵ȼ���Ϣ
		m_pGradeInfo, sizeof(CMD_GP_UserGradeInfo) * m_iGradeNum);

	/*sprintf(info, _T("OnDBLogonSuccess SUB_GP_LOGON_USER_GRADE_INFO"));
	CTraceService::TraceString(info, TraceLevel_Normal);*/
	//���͵�¼���
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_FINISH);

	/*sprintf(info, _T("OnDBLogonSuccess SUB_GP_LOGON_FINISH"));
	CTraceService::TraceString(info, TraceLevel_Normal);*/
	//�ر�����
	if ( pDBRLogonSuccess->dwCustomFaceVer == 0 )
		m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
	
	__LEAVE_FUNCTION
	return true;
}


bool CAttemperEngineSink::OnUpdateUserInfoSucc(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	__ENTER_FUNCTION
	//Ч�����
	ASSERT(wDataSize==sizeof(CMD_GP_UserInfoSucc));
	if (wDataSize!=sizeof(CMD_GP_UserInfoSucc)) return false;

	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER, SUB_GP_UPDATE_USERINFO_SUCC, //���͵ȼ���Ϣ
		pData, sizeof(CMD_GP_UserInfoSucc));
	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	__LEAVE_FUNCTION
	return true;
}

//�û�����ʧ��
bool CAttemperEngineSink::OnUpdateUserInfoFail(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	__ENTER_FUNCTION

	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	__LEAVE_FUNCTION
	return true;
}



//�û���¼ʧ��
bool CAttemperEngineSink::OnDBLogonError(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	__ENTER_FUNCTION


	//Ч�����
	ASSERT(wDataSize==sizeof(DBR_GP_LogonError));
	if (wDataSize!=sizeof(DBR_GP_LogonError)) return false;

	//�ж�����
	ASSERT(LOWORD(dwContextID)<m_pInitParamter->m_wMaxConnect);
	if ((m_pBindParameter+LOWORD(dwContextID))->dwSocketID!=dwContextID) return true;

	//��������
	CMD_GP_LogonError LogonError;
	DBR_GP_LogonError * pLogonError=(DBR_GP_LogonError *)pData;
	pLogonError->szErrorDescribe[CountArray(pLogonError->szErrorDescribe)-1]=0;

	//��������
	LogonError.lErrorCode=pLogonError->lErrorCode;
	lstrcpyn(LogonError.szErrorDescribe,pLogonError->szErrorDescribe,CountArray(LogonError.szErrorDescribe));
	WORD wDescribeSize=CountStringBuffer(LogonError.szErrorDescribe);

	//��������
	WORD wSendSize=sizeof(LogonError)-sizeof(LogonError.szErrorDescribe)+wDescribeSize;
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GP_LOGON,SUB_GP_LOGON_ERROR,&LogonError,wSendSize);

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	__LEAVE_FUNCTION
	return true;
}

//���سɹ�
bool CAttemperEngineSink::OnDBDownloadFaceSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	__ENTER_FUNCTION
	//����ת��
	DBR_GP_DownloadFaceSuccess *pDownloadFaceSuccess = (DBR_GP_DownloadFaceSuccess*)pData;

	//������֤
	WORD wPostSize = WORD(sizeof(DBR_GP_DownloadFaceSuccess)-sizeof(pDownloadFaceSuccess->bFaceData)+pDownloadFaceSuccess->dwCurrentSize);
	ASSERT(wDataSize == wPostSize);
	if ( wPostSize != wDataSize ) return false;

	//��������
	CMD_GP_DownloadFaceSuccess DownloadFaceSuccess;
	DownloadFaceSuccess.dwUserID = pDownloadFaceSuccess->dwUserID;
	DownloadFaceSuccess.dwToltalSize = pDownloadFaceSuccess->dwToltalSize;
	DownloadFaceSuccess.dwCurrentSize = pDownloadFaceSuccess->dwCurrentSize;
	CopyMemory(DownloadFaceSuccess.bFaceData, pDownloadFaceSuccess->bFaceData, pDownloadFaceSuccess->dwCurrentSize);
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER, SUB_GP_USER_DOWNLOAD_FACE, &DownloadFaceSuccess, wPostSize);

	__LEAVE_FUNCTION
	return true;
}

//�ɹ���Ϣ
bool CAttemperEngineSink::OnDBRCustomFaceUploadResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	__ENTER_FUNCTION

	ASSERT(wDataSize == sizeof(DBR_GP_UploadFaceResult));
	if ( wDataSize != sizeof(DBR_GP_UploadFaceResult)) return false;

	//����ת��
	DBR_GP_UploadFaceResult *pUploadFaceResult = (DBR_GP_UploadFaceResult*)pData;
	pUploadFaceResult->szDescribeMsg[CountArray(pUploadFaceResult->szDescribeMsg)-1] = 0;

	//��������
	CMD_GP_UploadFaceResult UploadFaceResult;
	ZeroMemory(&UploadFaceResult, sizeof(UploadFaceResult));
	lstrcpy(UploadFaceResult.szDescribeMsg, pUploadFaceResult->szDescribeMsg);
	UploadFaceResult.dwFaceVer = pUploadFaceResult->dwFaceVer;
	UploadFaceResult.bOperateSuccess=pUploadFaceResult->bOperateSuccess;

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER, SUB_GP_UPLOAD_FACE_RESULT, &UploadFaceResult, sizeof(UploadFaceResult));

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	__LEAVE_FUNCTION
	return true;
}

//ɾ�����
bool CAttemperEngineSink::OnDBRCustomFaceDeleteResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	__ENTER_FUNCTION

	//������֤
	ASSERT(wDataSize == sizeof(DBR_GP_DeleteFaceResult));
	if (wDataSize != sizeof(DBR_GP_DeleteFaceResult)) return false;

	//����ת��
	DBR_GP_DeleteFaceResult *pDeleteFaceResult = (DBR_GP_DeleteFaceResult*)pData;
	pDeleteFaceResult->szDescribeMsg[CountArray(pDeleteFaceResult->szDescribeMsg)-1] = 0;

	//��������
	CMD_GP_DeleteFaceResult DeleteFaceResult;
	ZeroMemory(&DeleteFaceResult, sizeof(DeleteFaceResult));
	lstrcpy(DeleteFaceResult.szDescribeMsg, pDeleteFaceResult->szDescribeMsg);
	DeleteFaceResult.dwFaceVer=pDeleteFaceResult->dwFaceVer;
	DeleteFaceResult.bOperateSuccess = pDeleteFaceResult->bOperateSuccess;

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER, SUB_GP_DELETE_FACE_RESULT, &DeleteFaceResult, sizeof(DeleteFaceResult));

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	__LEAVE_FUNCTION

	return true;
}

//�޸Ľ��
bool CAttemperEngineSink::OnDBRModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	__ENTER_FUNCTION
	//������֤
	ASSERT(wDataSize == sizeof(DBR_GP_ModifyIndividualResult));
	if (wDataSize != sizeof(DBR_GP_ModifyIndividualResult)) return false;

	//����ת��
	DBR_GP_ModifyIndividualResult *pModifyIndividualResul = (DBR_GP_ModifyIndividualResult*)pData;
	pModifyIndividualResul->szDescribeMsg[CountArray(pModifyIndividualResul->szDescribeMsg)-1] = 0;

	CMD_GP_ModifyIndividualResult ModifyIndividualResult;
	ZeroMemory(&ModifyIndividualResult, sizeof(ModifyIndividualResult));
	ModifyIndividualResult.bOperateSuccess = pModifyIndividualResul->bOperateSuccess;
	lstrcpyn(ModifyIndividualResult.szDescribeMsg, pModifyIndividualResul->szDescribeMsg, sizeof(ModifyIndividualResult.szDescribeMsg));

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER, SUB_GP_MODIFY_INDIVIDUAL_RESULT, &ModifyIndividualResult, sizeof(ModifyIndividualResult));

	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	__LEAVE_FUNCTION

	return true;
}

bool CAttemperEngineSink::OnDBRGradeInfoNum(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	__ENTER_FUNCTION

	if (wDataSize != sizeof(LONG))
	{
		return false;
	}	

	LONG num = *((LONG*)pData);

	m_pGradeInfo = new CMD_GP_UserGradeInfo[num];
	memset(m_pGradeInfo, 0, sizeof(CMD_GP_UserGradeInfo) * num);
	m_iGradeNum  = num;

	__LEAVE_FUNCTION

	return true;
}

bool CAttemperEngineSink::OnDBRGradeInfoOne(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	__ENTER_FUNCTION

	if (wDataSize != sizeof(DBR_GR_gradeInfo))
	{
		return false;
	}

	DBR_GR_gradeInfo* pGradeInfo = (DBR_GR_gradeInfo*)pData;
	if (pGradeInfo->id <= m_iGradeNum )
	{
		int index = pGradeInfo->id  - 1;
		m_pGradeInfo[index].iMaxExp = pGradeInfo->iMaxExp;
		memcpy(m_pGradeInfo[index].strName,  pGradeInfo->szName, sizeof(m_pGradeInfo[index].strName));
		m_pGradeInfo[index].strName[sizeof(m_pGradeInfo[index].strName) - 1] = 0;
	}

	__LEAVE_FUNCTION
	return true;
}

//���������ȡ�Ľ��
bool CAttemperEngineSink::OnDBRGiftMoneyResult(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	__ENTER_FUNCTION

	if (wDataSize != sizeof(DBR_GP_GiftGoldResult))
	{
		return false;
	}

	//��������
	m_pITCPNetworkEngine->SendData(dwContextID, MDM_GP_USER, 
		SUB_GP_GET_GIFT_GOLD_RESULT , pData, wDataSize);
	//�ر�����
	m_pITCPNetworkEngine->ShutDownSocket(dwContextID);

	__LEAVE_FUNCTION

	return true;
}

//�����¼�
bool __cdecl CAttemperEngineSink::OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)
{
	__ENTER_FUNCTION

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

	__LEAVE_FUNCTION

	return true;
}

//�ر��¼�
bool __cdecl CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	__ENTER_FUNCTION

	//��������
	m_pITimerEngine->KillTimer(IDI_UPDATE_SERVER_LIST);
	m_pITimerEngine->SetTimer(IDI_CONNECT_CENTER_SERVER,TIME_RECONNECT,1,NULL);

	//������ʾ
	CTraceService::TraceString(TEXT("���ķ����������жϣ��Ժ󽫻��ٴγ���...."),TraceLevel_Warning);

	__LEAVE_FUNCTION

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
	}

	return true;
}

//����Ӧ���¼�
bool __cdecl CAttemperEngineSink::OnEventTCPNetworkBind(DWORD dwClientIP, DWORD dwSocketID)
{
	__ENTER_FUNCTION

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

	__LEAVE_FUNCTION

	return true;
}

//�����ȡ�¼�
bool __cdecl CAttemperEngineSink::OnEventTCPNetworkRead(CMD_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	bool ret = false;
	switch (Command.wMainCmdID)
	{
	case MDM_GP_LOGON:				//��¼��Ϣ
		{
			ret = OnSocketMainLogon(Command.wSubCmdID,pData,wDataSize,dwSocketID);
			break;
		} 
	case MDM_GP_USER:				//�û���Ϣ
		{
			ret = OnSocketMainUser(Command.wSubCmdID,pData,wDataSize,dwSocketID);
			break;
		}
	}

	if (!ret)
	{
		char info[BUFSIZ] = {0};
		sprintf(info, _T("OnEventTCPNetworkRead %i %i"), Command.wMainCmdID, Command.wSubCmdID);
		CTraceService::TraceString(info, TraceLevel_Normal);
	}
	return ret;
}

//����ر��¼�
bool __cdecl CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientIP, DWORD dwActiveTime, DWORD dwSocketID)
{
	//�����Ϣ
	WORD wBindIndex=LOWORD(dwSocketID);
	ZeroMemory((m_pBindParameter+wBindIndex),sizeof(tagBindParameter));

	/*char info[BUFSIZ] = {0};
	sprintf(info, _T("OnEventTCPNetworkShut"));
	CTraceService::TraceString(info, TraceLevel_Normal);*/
	return true;
}

//��¼��Ϣ����
bool CAttemperEngineSink::OnSocketMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	__ENTER_FUNCTION

	switch (wSubCmdID)
	{
	case SUB_GP_LOGON_ACCOUNTS:		//�ʺŵ�¼
		{
			//Ч�����
			/*char info[BUFSIZ] = {0};
			sprintf(info, _T("OnSocketMainLogon"));
			CTraceService::TraceString(info, TraceLevel_Normal);*/

			ASSERT(wDataSize>=sizeof(CMD_GP_LogonByAccounts));
			if (wDataSize<sizeof(CMD_GP_LogonByAccounts)) return true;

			//������Ϣ
			CMD_GP_LogonByAccounts * pLogonByAccounts=(CMD_GP_LogonByAccounts *)pData;
			pLogonByAccounts->szAccounts[CountArray(pLogonByAccounts->szAccounts)-1]=0;
			pLogonByAccounts->szPassWord[CountArray(pLogonByAccounts->szPassWord)-1]=0;

			/*sprintf(info, _T("OnSocketMainLogon %s"), pLogonByAccounts->szAccounts);
			CTraceService::TraceString(info, TraceLevel_Normal);*/

			//������Ϣ
			ASSERT(LOWORD(dwSocketID)<m_pInitParamter->m_wMaxConnect);
			DWORD dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientIP;

			////Ч��汾
			//if (pLogonByAccounts->dwPlazaVersion!=VER_PLAZA_FRAME)
			//{
			//	//��ȡ�汾
			//	WORD wLow=LOWORD(pLogonByAccounts->dwPlazaVersion);
			//	WORD wHigh=HIWORD(pLogonByAccounts->dwPlazaVersion);

			//	//��������
			//	CMD_GP_Version Version;
			//	Version.bNewVersion=TRUE;
			//	Version.bAllowConnect=(wHigh==VER_PLAZA_HIGH)?TRUE:FALSE;

			//	//��������
			//	m_pITCPNetworkEngine->SendData(dwSocketID,MDM_GP_SYSTEM,SUB_GP_VERSION,&Version,sizeof(Version));

			//	//�жϹر�
			//	//if (Version.bAllowConnect==FALSE)
			//	//{
			//	//	return true;
			//	//}
			//	sprintf(info, _T("OnSocketMainLogon  SUB_GP_VERSION"));
			//	CTraceService::TraceString(info, TraceLevel_Normal);
			//	return true;
			//}

			//��������
			DBR_GP_LogonByAccounts LogonByAccounts;
			ZeroMemory(&LogonByAccounts,sizeof(LogonByAccounts));

			//���ñ���
			LogonByAccounts.dwClientIP=dwClientAddr;
			lstrcpyn(LogonByAccounts.szAccounts,pLogonByAccounts->szAccounts,sizeof(LogonByAccounts.szAccounts));
			lstrcpyn(LogonByAccounts.szPassWord,pLogonByAccounts->szPassWord,sizeof(LogonByAccounts.szPassWord));

			//��������
			VOID * pData=NULL;
			tagDataDescribe DataDescribe;
			CRecvPacketHelper RecvPacket(pLogonByAccounts+1,wDataSize-sizeof(CMD_GP_LogonByAccounts));

			//��չ��Ϣ
			while (true)
			{
				//��ȡ����
				pData=RecvPacket.GetData(DataDescribe);
				if (DataDescribe.wDataDescribe==DTP_NULL) break;

				//���ݷ���
				switch (DataDescribe.wDataDescribe)
				{
				case DTP_COMPUTER_ID:		//������ʶ
					{
						ASSERT(pData!=NULL);
						ASSERT(DataDescribe.wDataSize==sizeof(tagClientSerial));
						if (DataDescribe.wDataSize==sizeof(tagClientSerial))
						{
							BYTE * pcbByte=(BYTE *)pData;
							for (INT i=0;i<sizeof(tagClientSerial)/sizeof(BYTE);i++)
							{
								ASSERT(CountArray(LogonByAccounts.szComputerID)>((i*2)+1));
								_stprintf(&LogonByAccounts.szComputerID[i*2],TEXT("%02X"),*(pcbByte+i));
							}
						}

						break;
					}
				}
			}

			/*sprintf(info, _T("OnSocketMainLogon  DBR_GP_LOGON_BY_ACCOUNTS"));
			CTraceService::TraceString(info, TraceLevel_Normal);*/

			//Ͷ�����ݿ�
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_LOGON_BY_ACCOUNTS,dwSocketID,&LogonByAccounts,sizeof(LogonByAccounts));

			return true;
		}
	}

	__LEAVE_FUNCTION

	return true;
}

//�û���Ϣ����
bool CAttemperEngineSink::OnSocketMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	__ENTER_FUNCTION

	switch (wSubCmdID)
	{
	case SUB_GP_USER_UPLOAD_FACE:		//�ϴ�ͷ��
		{
			//����ת��
			CMD_GP_UploadCustomFace *pUploadCustomFace = (CMD_GP_UploadCustomFace*)pData;

			//������֤
			ASSERT(wDataSize == sizeof(CMD_GP_UploadCustomFace)-sizeof(pUploadCustomFace->bFaceData)+pUploadCustomFace->wCurrentSize);
			if (wDataSize != sizeof(CMD_GP_UploadCustomFace)-sizeof(pUploadCustomFace->bFaceData)+pUploadCustomFace->wCurrentSize) return false;

			//д���ļ�
			try
			{
				//�ļ�����
				CFile fileCustomFace;
				TCHAR szFileName[128];
				_snprintf(szFileName, CountArray(szFileName), TEXT("%s\\UploadFile_%ld.zip"), theApp.m_szDirWork, pUploadCustomFace->dwUserID);
				BOOL bSuccess=FALSE;
				if ( pUploadCustomFace->bFirstPacket == true )
					bSuccess=fileCustomFace.Open( szFileName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
				else
					bSuccess=fileCustomFace.Open( szFileName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary|CFile::modeNoTruncate);
				if ( bSuccess )
				{
					//�ƶ�ָ��
					fileCustomFace.SeekToEnd();

					//д���ļ�
					fileCustomFace.Write(pUploadCustomFace->bFaceData, pUploadCustomFace->wCurrentSize);

					//�ر��ļ�
					fileCustomFace.Close();

					//����ж�
					if ( pUploadCustomFace->bLastPacket == true )
					{
						//Ͷ������
						DBR_GP_UploadCustomFace UploadCustomFace;
						ZeroMemory(&UploadCustomFace, sizeof(UploadCustomFace));
						UploadCustomFace.dwUserID = pUploadCustomFace->dwUserID;
						m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_USER_UPLOAD_FACE, dwSocketID, &UploadCustomFace, sizeof(DBR_GP_UploadCustomFace));
					}
				}
			}
			catch(...){}

			return true;
		}
	case SUB_GP_USER_DOWNLOAD_FACE:		//����ͷ��
		{
			//������֤
			ASSERT(sizeof(CMD_GP_DownloadFace)==wDataSize);
			if ( sizeof(CMD_GP_DownloadFace) != wDataSize) return false;

			CMD_GP_DownloadFace *pDownloadFace = (CMD_GP_DownloadFace*)pData;

			ASSERT( pDownloadFace->dwUserID != 0 );

			//Ͷ������
			DBR_GP_DownloadCustomFace DownloadCustomFace;
			ZeroMemory(&DownloadCustomFace, sizeof(DownloadCustomFace));
			DownloadCustomFace.dwUserID = pDownloadFace->dwUserID ;
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_DOWNLOAD_FACE, dwSocketID, &DownloadCustomFace, sizeof(DownloadCustomFace));

			return true;
		}
	case SUB_GP_CUSTOM_FACE_DELETE:		//ɾ��ͷ��
		{
			//������֤
			ASSERT(sizeof(CMD_GP_CustomFaceDelete) == wDataSize);
			if ( sizeof(CMD_GP_CustomFaceDelete) != wDataSize) return false;

			//����ת��
			CMD_GP_CustomFaceDelete *pCustomFaceDelete = (CMD_GP_CustomFaceDelete *)pData;

			DBR_GP_CustomFaceDelete CustomFaceDelete;
			ZeroMemory(&CustomFaceDelete, sizeof(CustomFaceDelete));
			CustomFaceDelete.dwUserID = pCustomFaceDelete->dwUserID;
			CustomFaceDelete.dwFaceVer = pCustomFaceDelete->dwFaceVer;

			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_DELETE_FACE, dwSocketID, &CustomFaceDelete, sizeof(CustomFaceDelete));

			return true;
		}
	case SUB_GP_MODIFY_INDIVIDUAL:		//�޸�����
		{
			//������֤
			ASSERT(sizeof(CMD_GP_ModifyIndividual) == wDataSize);
			if ( sizeof(CMD_GP_ModifyIndividual) != wDataSize) return false;

			//����ת��
			CMD_GP_ModifyIndividual *pModifyIndividual = (CMD_GP_ModifyIndividual *)pData;
			pModifyIndividual->szAddress[sizeof(pModifyIndividual->szAddress)-1] = 0;
			pModifyIndividual->szNickname[sizeof(pModifyIndividual->szNickname)-1] = 0;
			pModifyIndividual->szPassword[sizeof(pModifyIndividual->szPassword)-1] = 0;
			pModifyIndividual->szUnderWrite[sizeof(pModifyIndividual->szUnderWrite)-1] = 0;

			DBR_GP_ModifyIndividual ModifyIndividual;

			ZeroMemory(&ModifyIndividual, sizeof(ModifyIndividual));

			ModifyIndividual.dwUserID = pModifyIndividual->dwUserID;
			ModifyIndividual.nAge = pModifyIndividual->nAge;
			ModifyIndividual.nGender = pModifyIndividual->nGender;
			lstrcpyn(ModifyIndividual.szAddress, pModifyIndividual->szAddress, sizeof(ModifyIndividual.szAddress));
			lstrcpyn(ModifyIndividual.szNickname, pModifyIndividual->szNickname, sizeof(ModifyIndividual.szNickname));
			lstrcpyn(ModifyIndividual.szPassword, pModifyIndividual->szPassword, sizeof(ModifyIndividual.szPassword));
			lstrcpyn(ModifyIndividual.szUnderWrite, pModifyIndividual->szUnderWrite, sizeof(ModifyIndividual.szUnderWrite));

			//Ͷ������
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_MODIFY_INDIVIDUAL, dwSocketID, &ModifyIndividual, sizeof(ModifyIndividual));

			return true;
		}
	case SUB_GP_UPDATE_USERINFO :       //��ѯ�û���Ϣ
		{
			//Ч�����
			if (wDataSize != sizeof(CMD_GP_UpdateUserInfo))
				return false;

			//��������
			DBR_GP_UpdateUserInfo updateUserInfo;
			memcpy(&updateUserInfo, pData, sizeof(updateUserInfo));

			//Ͷ�����ݿ�
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_UPDATE_USERINFO,dwSocketID,&updateUserInfo, sizeof(updateUserInfo));
			return true;
		}
	case SUB_GP_GET_GIFT_GOLD:     //ÿ��һ�ε������ȡ
		{
			//Ч�����
			if (wDataSize != sizeof(CMD_GP_GiftGold))
				return false;

			//��������
			DBR_GP_GiftGold   giftMoney;
			memcpy(&giftMoney, pData, sizeof(giftMoney));
			//������Ϣ
			ASSERT(LOWORD(dwSocketID)<m_pInitParamter->m_wMaxConnect);
			DWORD dwClientAddr=(m_pBindParameter+LOWORD(dwSocketID))->dwClientIP;
			giftMoney.dwClientIP=dwClientAddr;
			//Ͷ�����ݿ�
			m_pIDataBaseEngine->PostDataBaseRequest(DBR_GP_GET_GIFT_GLOD,dwSocketID,&giftMoney, sizeof(giftMoney));
			return true;
		}
	case SUB_GP_CHECK_LINE:		// �����·
		{
			//Ч�����
			if (wDataSize != sizeof(CMD_GP_CheckLine))
				return false;

			CMD_GP_CheckLine * pCheckLineData = (CMD_GP_CheckLine *)pData;

			//��������
			CMD_GP_CheckLine checkline;
			memcpy(&checkline, pCheckLineData, sizeof(checkline));
			//������Ϣ
			ASSERT(LOWORD(dwSocketID)<m_pInitParamter->m_wMaxConnect);
			checkline.dwClientIP = (m_pBindParameter+LOWORD(dwSocketID))->dwClientIP;
			//��������
			m_pITCPNetworkEngine->SendData(dwSocketID, MDM_GP_USER, SUB_GP_CHECK_LINE, &checkline, sizeof(checkline));
			//�ر�����
			m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);
			return true;
		}
	case SUB_GP_DELETE_MACHINE_CODE:	//ɾ��������
		{
#if !defined(_DEBUG)
			//Ч�����
			if (wDataSize != sizeof(CMD_GP_DeleteMachineCode))
				return false;

			CMD_GP_DeleteMachineCode * pDeleteMachineCode = (CMD_GP_DeleteMachineCode *)pData;
			BYTE * pcbByte=(BYTE *)pDeleteMachineCode;
			TCHAR	szComputerID[COMPUTER_ID_LEN];		//��������

			for (INT i=0;i<sizeof(tagClientSerial)/sizeof(BYTE);i++)
			{
				ASSERT(CountArray(szComputerID)>((i*2)+1));
				_stprintf(&szComputerID[i*2],TEXT("%02X"),*(pcbByte+i));
			}

			MMachine::iterator M_const_return = m_hmMachineCode.find(szComputerID);
			if (m_hmMachineCode.end() != M_const_return)
			{
				if (M_const_return->second->compare(pDeleteMachineCode->szAccounts))
				{
					m_hmMachineCode.erase(M_const_return);
				}
			}
#endif
			//��������
			m_pITCPNetworkEngine->SendData(dwSocketID, MDM_GP_USER, SUB_GP_DELETE_MACHINE_CODE);
			//�ر�����
			m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);

			return true;
		}
	default:
		char info[BUFSIZ] = {0};
		{
			sprintf(info, _T("OnSocketMainUser, ���õ���Ϣ"));
			CTraceService::TraceString(info, TraceLevel_Normal);
			ASSERT(TRUE);
			return true;
		}
	}

	__LEAVE_FUNCTION

	return true;
}



//�б���Ϣ����
bool CAttemperEngineSink::OnCenterMainServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	__ENTER_FUNCTION

	switch (wSubCmdID)
	{
	case SUB_CS_LIST_INFO:		//�б���Ϣ
		{
			//Ч�����
			ASSERT(wDataSize==sizeof(CMD_CS_ListInfo));
			if (wDataSize!=sizeof(CMD_CS_ListInfo))
				return false;

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

			return true;
		}
	}

	__LEAVE_FUNCTION

	return true;
}

//////////////////////////////////////////////////////////////////////////
