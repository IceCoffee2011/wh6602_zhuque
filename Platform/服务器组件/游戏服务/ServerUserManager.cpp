#include "StdAfx.h"
#include "ServerUserManager.h"
#include "CGlobalData.h"

//���캯��
CServerUserItem::CServerUserItem(void)
{
	m_pRoomLog = NULL;

	InitServerUserItem();
}

//��������
CServerUserItem::~CServerUserItem(void)  
{
}

//�ӿڲ�ѯ
void * __cdecl CServerUserItem::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IServerUserItem,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IServerUserItem,Guid,dwQueryVer);
	return NULL;
}

// ������־ָ��
void __cdecl CServerUserItem::SetRoomLog(cnd_ILogMgr*	pRoomLog)
{
	m_pRoomLog = pRoomLog;;	// ������־

	//if(!m_pRoomLog)		// �˶β�����
	//{
	//	CString strPromptNull;	
	//	strPromptNull.Format("���⣺m_pRoomLog=NULL ����������%s  �����У�%d ",  __FUNCTION__ ,  __LINE__  );	
	//	AfxMessageBox(strPromptNull);
	//}
}

//�û�ʤ��
WORD __cdecl CServerUserItem::GetUserWinRate()
{
	__ENTER_FUNCTION_LOG_CATCH
	//Ч�����
	ASSERT(m_bAcitve==true);

	//����ʤ��
	tagUserScore * pUserScore=&m_ServerUserData.UserScoreInfo;
	LONG lPlayCount=pUserScore->lWinCount+pUserScore->lLostCount+pUserScore->lDrawCount+pUserScore->lFleeCount;
	if (lPlayCount!=0L) return (WORD)(pUserScore->lWinCount*10000L/lPlayCount);

	__LEAVE_FUNCTION_LOG_CATCH

	return 0;
}

//�û�����
WORD __cdecl CServerUserItem::GetUserLostRate()
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч�����
	ASSERT(m_bAcitve==true);

	//��������
	tagUserScore * pUserScore=&m_ServerUserData.UserScoreInfo;
	LONG lPlayCount=pUserScore->lWinCount+pUserScore->lLostCount+pUserScore->lDrawCount+pUserScore->lFleeCount;
	if (lPlayCount!=0L) return (WORD)(pUserScore->lLostCount*10000L/lPlayCount);

	__LEAVE_FUNCTION_LOG_CATCH

	return 0;
}

//�û�����
WORD __cdecl CServerUserItem::GetUserDrawRate()
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч�����
	ASSERT(m_bAcitve==true);

	//�������
	tagUserScore * pUserScore=&m_ServerUserData.UserScoreInfo;
	LONG lPlayCount=pUserScore->lWinCount+pUserScore->lLostCount+pUserScore->lDrawCount+pUserScore->lFleeCount;
	if (lPlayCount!=0L) return (WORD)(pUserScore->lDrawCount*10000L/lPlayCount);

	__LEAVE_FUNCTION_LOG_CATCH

	return 0;
}

//�û�����
WORD __cdecl CServerUserItem::GetUserFleeRate()
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч�����
	ASSERT(m_bAcitve==true);

	//��������
	tagUserScore * pUserScore=&m_ServerUserData.UserScoreInfo;
	LONG lPlayCount=pUserScore->lWinCount+pUserScore->lLostCount+pUserScore->lDrawCount+pUserScore->lFleeCount;
	if (lPlayCount!=0L) return (WORD)(pUserScore->lFleeCount*10000L/lPlayCount);

	__LEAVE_FUNCTION_LOG_CATCH

	return 0;
}

BOOL __cdecl CServerUserItem::GetOnlineTimeGolSign(BYTE byIndex)
{
	__ENTER_FUNCTION_LOG_CATCH
	ASSERT(0<=byIndex && 4>byIndex);
	if (0>byIndex || 4<=byIndex)
		return FALSE;

	return m_bIsPresentOnline[byIndex];
	__LEAVE_FUNCTION_LOG_CATCH
}

void __cdecl CServerUserItem::SetOnlineTimeGoldSign(BYTE byIndex, BOOL bVal/*=TRUE*/)
{
	__ENTER_FUNCTION_LOG_CATCH
	ASSERT(0<=byIndex && 4>byIndex);
	if (0>byIndex || 4<=byIndex)
		return;

	m_bIsPresentOnline[byIndex] = bVal;
	__LEAVE_FUNCTION_LOG_CATCH
}

BOOL __cdecl CServerUserItem::GetUpgradeGemSign(BYTE byIndex)
{
	__ENTER_FUNCTION_LOG_CATCH
	ASSERT(0<=byIndex && 7>byIndex);
	if (0>byIndex || 7<=byIndex)
		return FALSE;

	return m_bIsUpgradePresent[byIndex];
	__LEAVE_FUNCTION_LOG_CATCH
}

void __cdecl CServerUserItem::SetUpgradeGemSign(BYTE byIndex, BOOL bVal/*=TRUE*/)
{
	__ENTER_FUNCTION_LOG_CATCH
	ASSERT(0<=byIndex && 7>byIndex);
	if (0>byIndex || 7<=byIndex)
		return;

	m_bIsUpgradePresent[byIndex] = bVal;
	__LEAVE_FUNCTION_LOG_CATCH
}

//���ù���
bool __cdecl CServerUserItem::SetUserRule(tagUserRule & UserRule)
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч�����
	ASSERT(m_bAcitve==true);
	if (m_bAcitve==false) return false;

	//���ù���
	m_UserRule=UserRule;

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//��Ϸ����
bool __cdecl CServerUserItem::SetServerType(WORD wServerType)
{
	m_wServerType = wServerType;

	return true;
}

//�޸Ļ���
bool __cdecl CServerUserItem::WriteScore(tagScoreInfo & ScoreInfo, DWORD dwPlayTimeCount)
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч�����
	ASSERT(m_bAcitve==true);
	if (m_bAcitve==false) return false;

	//�޸�����
	switch (ScoreInfo.ScoreKind)
	{
	case enScoreKind_Win: 
		{
			m_ScoreModify.lWinCount++;
			m_ServerUserData.UserScoreInfo.lWinCount++;
			ScoreInfo.lXp += 2;
			m_ServerUserData.UserScoreInfo.lKindIDWinCount++;//Author<cxf>;DateTime<2012-03-05>
			break;
		}
	case enScoreKind_Lost: 
		{
			m_ScoreModify.lLostCount++;
			m_ServerUserData.UserScoreInfo.lLostCount++;
			m_ServerUserData.UserScoreInfo.lKindIDLostCount++;//Author<cxf>;DateTime<2012-03-05>
			break;
		}
	case enScoreKind_Draw:
		{
			m_ScoreModify.lDrawCount++;
			m_ServerUserData.UserScoreInfo.lDrawCount++;
			m_ServerUserData.UserScoreInfo.lKindIDDrawCount++;//Author<cxf>;DateTime<2012-03-05>
			break;
		}
	case enScoreKind_Flee:
		{
			m_ScoreModify.lFleeCount++;
			m_ServerUserData.UserScoreInfo.lFleeCount++;
			m_ServerUserData.UserScoreInfo.lKindIDFleeCount++;//Author<cxf>;DateTime<2012-03-05>
			break;
		}
	}

	//�������
	if (m_ServerUserData.utUserType == PERSON)
	{
		m_ScoreModify.lExperience += (LONG)ScoreInfo.lXp;
		m_ServerUserData.UserScoreInfo.lExperience +=  (LONG)ScoreInfo.lXp;
	}

	//��ӽ���

	//�޸Ļ���
	m_lRoundRevenue = ScoreInfo.lRevenue;
	m_lRevenue      +=ScoreInfo.lRevenue;
	if(m_wServerType==GAME_GENRE_GOLD)
	{
		if ( m_ServerUserData.UserScoreInfo.lGold+ScoreInfo.lScore <=0) //��Ҳ���Ϊ����
		{
			m_ScoreModify.lGold += (0 - m_ServerUserData.UserScoreInfo.lGold);
			m_ServerUserData.UserScoreInfo.lGold = 0;

		}else
		{
			m_ScoreModify.lGold += ScoreInfo.lScore;
			m_ServerUserData.UserScoreInfo.lGold+=ScoreInfo.lScore;
		}
	}

	//�û�����
	if ((ScoreInfo.ScoreKind!=enScoreKind_Service)&&(ScoreInfo.ScoreKind!=enScoreKind_Present))
	{
		//��Ϸʱ��
		m_dwPlayTimeCount +=dwPlayTimeCount;
		m_dwRoundPlayTime  = dwPlayTimeCount;
	}

//	RefreshProp();

	//����ʹ��
	if (m_ServerUserData.UserScoreInfo.lExperience > 999999 || m_ServerUserData.UserScoreInfo.lExperience < 0)
	{
		char info[BUFSIZ] = {0};
		sprintf(info, "д�����ʱ�򣬼�ⷢ�־����쳣, id��%i, �ȼ�:%i, ���飺%i", 
			    m_ServerUserData.dwUserID,
			    m_ServerUserData.UserScoreInfo.dwGrade,
			    m_ServerUserData.UserScoreInfo.lExperience);
		CTraceService::TraceString(info,TraceLevel_Normal);
	}

	// ���¼���ȼ�
	CaluClass();
	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//���¼���ȼ�
void CServerUserItem::CaluClass()
{
	if (m_ServerUserData.utUserType == PERSON)
	{
		m_ServerUserData.UserScoreInfo.dwGrade;
		int grade = 0;
		grade = CCGlobalData::CaluGrade(m_ServerUserData.UserScoreInfo.lExperience);

		m_ScoreModify.dwGrade = grade - m_ServerUserData.UserScoreInfo.dwGrade;
		m_ServerUserData.UserScoreInfo.dwGrade = grade;
	}
}

//ˢ�µ���
void CServerUserItem::RefreshProp()
{
	for ( int nIdex = 0; nIdex < PROPERTY_COUNT; ++nIdex )
	{
		//��Ա��
		if ( PROP_MEMBER_1 <= nIdex && nIdex <= PROP_MEMBER_4 ) continue;

		if ( 0 < m_ServerUserData.PropertyInfo[nIdex].dwPropCount || m_ServerUserData.PropertyInfo[nIdex].bUse)
		{
			DWORD nowTime = (DWORD)time(NULL);
			DWORD pastTime = nowTime - m_ServerUserData.PropertyInfo[nIdex].dwActiveTime;
			if (pastTime >= m_ServerUserData.PropertyInfo[nIdex].dwLeaveTime)
			{
				m_ServerUserData.PropertyInfo[nIdex].bUse = false;
				m_ServerUserData.PropertyInfo[nIdex].dwActiveTime = 0;
				m_ServerUserData.PropertyInfo[nIdex].dwLeaveTime  = 0;
			}else
			{
				m_ServerUserData.PropertyInfo[nIdex].dwLeaveTime  -= pastTime;
				m_ServerUserData.PropertyInfo[nIdex].dwActiveTime  = nowTime;
			}
		}
	}
}



//����״̬
bool __cdecl CServerUserItem::SetUserStatus(BYTE cbUserStatus, WORD wTableID, WORD wChairID)
{
	//Ч��״̬
	ASSERT(m_bAcitve==true);
	if (m_bAcitve==false) return false;

	//���ñ���
	m_ServerUserData.wTableID=wTableID;
	m_ServerUserData.wChairID=wChairID;
	m_ServerUserData.cbUserStatus=cbUserStatus;

	return true;
}

//�޸��ʿ�
bool __cdecl CServerUserItem::ModifyBankStorageGold(LONGLONG lStorageCount)
{
	m_ServerUserData.UserScoreInfo.lMoney += lStorageCount;
	m_ScoreModify.lMoney += lStorageCount;

	//�Ϸ��ж�
	ASSERT(0<=m_ServerUserData.UserScoreInfo.lMoney);

	return true;	
}

//��ʼ��
void CServerUserItem::InitServerUserItem()
{
	m_bAcitve=false;
	m_bAndroid=false;
	m_lRevenue=0L;
	m_lRoundRevenue = 0L;
	m_wServerType=0;
	m_dwClientIP=0L;
	m_szPassword[0]=0;
	m_wUserIndex=0xFFFF;
	m_dwPlayTimeCount=0L;
	m_dwRoundPlayTime=0L; 
	m_dwLogonTime=(LONG)time(NULL);
	memset(&m_UserRule,0,sizeof(m_UserRule));
	memset(&m_ScoreBorn,0,sizeof(m_ScoreBorn));
	memset(&m_ScoreModify,0,sizeof(m_ScoreModify));
	memset(&m_ServerUserData,0,sizeof(m_ServerUserData));
	memset(m_bIsPresentOnline,0,sizeof(m_bIsPresentOnline));
	memset(m_bIsUpgradePresent,0,sizeof(m_bIsUpgradePresent));
	return;
}

//������Ϣ
bool __cdecl CServerUserItem::GetUserScoreInfo(tagUserScore & ScoreModifyInfo)
{
	//Ч��״̬
	ASSERT(m_bAcitve==true);
	if (m_bAcitve==false) return false;

	//���ñ���
	ScoreModifyInfo=m_ScoreModify;
	return true;;
}



//������ʱ��д�뱦ʯ
bool __cdecl CServerUserItem::WriteUserGem( DWORD lGem)
{
	m_ScoreModify.lGem                  += lGem;
	m_ServerUserData.UserScoreInfo.lGem += lGem;
	return true; 
}

bool __cdecl CServerUserItem::WriteUserGold(LONGLONG llGold, LONGLONG llRevenue/*=0l*/)
{
	if(m_wServerType==GAME_GENRE_GOLD)
	{
		if ( m_ServerUserData.UserScoreInfo.lGold+llGold <=0) //��Ҳ���Ϊ����
		{
			m_ScoreModify.lGold += (0 - m_ServerUserData.UserScoreInfo.lGold);
			m_ServerUserData.UserScoreInfo.lGold = 0;

		}else
		{
			m_ScoreModify.lGold += llGold;
			m_ServerUserData.UserScoreInfo.lGold+=llGold;
		}

		return true;
	}

	return false;
}

void __cdecl CServerUserItem::ClearScorModify()
{
	memset(&m_ScoreModify,0,sizeof(m_ScoreModify));
	m_lRoundRevenue = 0;
	m_dwRoundPlayTime = 0;
}


//��ӽ���

//////////////////////////////////////////////////////////////////////////

//���캯��
CServerUserManager::CServerUserManager(void)
{
	m_pRoomLog = NULL;
}

//��������
CServerUserManager::~CServerUserManager(void)
{
	//�ͷ��û�
	CServerUserItem * pServerUserItem=NULL;
	for (INT_PTR i=0;i<m_UserItemStore.GetCount();i++)
	{
		pServerUserItem=m_UserItemStore[i];
		ASSERT(pServerUserItem!=NULL);
		SafeDelete(pServerUserItem);
	}
	for (INT_PTR i=0;i<m_OnLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OnLineUserItem[i];
		ASSERT(pServerUserItem!=NULL);
		SafeDelete(pServerUserItem);
	}
	for (INT_PTR i=0;i<m_OffLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OffLineUserItem[i];
		ASSERT(pServerUserItem!=NULL);
		SafeDelete(pServerUserItem);
	}
	m_UserItemStore.RemoveAll();
	m_OnLineUserItem.RemoveAll();
	m_OffLineUserItem.RemoveAll();

	return;
}

//�ӿڲ�ѯ
void * __cdecl CServerUserManager::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IServerUserManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IServerUserManager,Guid,dwQueryVer);
	return NULL;
}

// ������־ָ��
void __cdecl CServerUserManager::SetRoomLog(cnd_ILogMgr*	pRoomLog)
{
	m_pRoomLog = pRoomLog;	// ������־

	if(!m_pRoomLog)		// �˶β�����
	{
		CString strPromptNull;	
		strPromptNull.Format("���⣺m_pRoomLog=NULL ����������%s  �����У�%d ",  __FUNCTION__ ,  __LINE__  );	
		//AfxMessageBox(strPromptNull);
	}
}

//�����û�
bool __cdecl CServerUserManager::ResetUserManager()
{
	//�ָ�����
	m_UserItemStore.Append(m_OnLineUserItem);
	m_UserItemStore.Append(m_OffLineUserItem);
	m_OnLineUserItem.RemoveAll();
	m_OffLineUserItem.RemoveAll();
	
	return true;
}

//�л��û�
bool __cdecl CServerUserManager::SwitchOnLineUserItem(IServerUserItem * pIServerUserItem, DWORD dwClientIP, WORD wUserIndex)
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//Ѱ���û�
	CServerUserItem * pServerUserItem=NULL;
	for (INT_PTR i=0;i<m_OnLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OnLineUserItem[i];
		ASSERT(pServerUserItem!=NULL);
		if (pServerUserItem==pIServerUserItem)
		{
			//�����û�
			pServerUserItem->m_dwClientIP=dwClientIP;
			pServerUserItem->m_wUserIndex=wUserIndex;
			pServerUserItem->m_bAndroid=(wUserIndex>=INDEX_ANDROID);

			return true;
		}
	}
	ASSERT(FALSE);

	__LEAVE_FUNCTION_LOG_CATCH

	return true;
}

//�����û�
bool __cdecl CServerUserManager::ActiveOffLineUserItem(IServerUserItem * pIServerUserItem, DWORD dwClientIP, WORD wUserIndex)
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	ASSERT(pIServerUserItem->GetUserStatus()==US_OFFLINE);
	if (pIServerUserItem==NULL) return false;

	//Ѱ���û�
	CServerUserItem * pServerUserItem=NULL;
	for (INT_PTR i=0;i<m_OffLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OffLineUserItem[i];
		ASSERT(pServerUserItem!=NULL);
		if (pServerUserItem==pIServerUserItem)
		{
			//�����û�
			pServerUserItem->m_dwClientIP=dwClientIP;
			pServerUserItem->m_wUserIndex=wUserIndex;
			pServerUserItem->m_bAndroid=(wUserIndex>=INDEX_ANDROID);

			//ע������
			m_OffLineUserItem.RemoveAt(i);
			m_OnLineUserItem.Add(pServerUserItem);

			return true;
		}
	}
	ASSERT(FALSE);

	__LEAVE_FUNCTION_LOG_CATCH

	return false;
}

//�����û�
IServerUserItem * __cdecl CServerUserManager::ActiveUserItem(tagServerUserData & ServerUserData, DWORD dwClientIP, WORD wUserIndex, TCHAR szPassword[PASS_LEN],WORD wServerType)
{
	//��ȡ���ж���
	CServerUserItem * pServerUserItem=NULL;
	if (m_UserItemStore.GetCount()>0)
	{
		INT_PTR nItemPostion=m_UserItemStore.GetCount()-1;
		pServerUserItem=m_UserItemStore[nItemPostion];
		ASSERT(pServerUserItem!=NULL);
		m_UserItemStore.RemoveAt(nItemPostion);
	}

	//�����¶���
	if (pServerUserItem==NULL)
	{
		try
		{
			pServerUserItem = new CServerUserItem;
			if (pServerUserItem==NULL) 
				return NULL;
		}
		catch (...) 
		{ 
			return NULL; 
		}
	}

	pServerUserItem->SetRoomLog(m_pRoomLog);

	//�����û�
	ASSERT(pServerUserItem!=NULL);
	m_OnLineUserItem.Add(pServerUserItem);
	pServerUserItem->InitServerUserItem();

	//�����û�
	pServerUserItem->m_bAcitve=true;
	pServerUserItem->m_wUserIndex=wUserIndex;
	pServerUserItem->m_bAndroid=(wUserIndex>=INDEX_ANDROID);

	//�����û�
	pServerUserItem->m_dwClientIP=dwClientIP;
	pServerUserItem->m_dwLogonTime=(LONG)time(NULL);
	pServerUserItem->m_ScoreBorn=ServerUserData.UserScoreInfo;
	pServerUserItem->SetServerType(wServerType);
	CopyMemory(&pServerUserItem->m_ServerUserData,&ServerUserData,sizeof(ServerUserData));
	lstrcpyn(pServerUserItem->m_szPassword,szPassword,CountArray(pServerUserItem->m_szPassword));

	return pServerUserItem;
}

//ö���û�
IServerUserItem * __cdecl CServerUserManager::EnumOnLineUser(WORD wEnumIndex)
{
	if (wEnumIndex>=m_OnLineUserItem.GetCount()) return NULL;
	return m_OnLineUserItem[wEnumIndex];
}

//ö���û�
IServerUserItem * __cdecl CServerUserManager::EnumOffLineUser(WORD wEnumIndex)
{
	if (wEnumIndex>=m_OffLineUserItem.GetCount()) return NULL;
	return m_OffLineUserItem[wEnumIndex];
}

//ɾ���û�
bool __cdecl CServerUserManager::DeleteUserItem(IServerUserItem * pIServerUserItem)
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	//tagServerUserData * pUserData=pIServerUserItem->GetUserData();

	//Ѱ���û�
	if (pIServerUserItem->GetUserIndex()!=0xFFFF)
	{
		for (INT_PTR i=0;i<m_OnLineUserItem.GetCount();i++)
		{
			//��ȡ�û�
			CServerUserItem * pOnLineUserItem=m_OnLineUserItem[i];

			//�û��ж�
			if (pOnLineUserItem==pIServerUserItem)
			{
				pOnLineUserItem->m_bAcitve=false;
				m_OnLineUserItem.RemoveAt(i);
				m_UserItemStore.Add(pOnLineUserItem);
				return true;
			}
		}
	}
	else
	{
		for (INT_PTR i=0;i<m_OffLineUserItem.GetCount();i++)
		{
			//��ȡ�û�
			CServerUserItem * pOffLineUserItem=m_OffLineUserItem[i];

			//�û��ж�
			if (pOffLineUserItem==pIServerUserItem)
			{
				pOffLineUserItem->m_bAcitve=false;
				m_OffLineUserItem.RemoveAt(i);
				m_UserItemStore.Add(pOffLineUserItem);
				return true;
			}
		}
	}

	ASSERT(FALSE);

	__LEAVE_FUNCTION_LOG_CATCH

	return false;
}

//ע�����
bool __cdecl CServerUserManager::RegOffLineUserItem(IServerUserItem * pIServerUserItem)
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	CServerUserItem * pServerUserItem=NULL;
	tagServerUserData * pUserData=pIServerUserItem->GetUserData();

	//Ѱ���û�
	ASSERT(pUserData->cbUserStatus!=US_OFFLINE);
	for (INT_PTR i=0;i<m_OnLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OnLineUserItem[i];
		ASSERT(pServerUserItem!=NULL);
		if (pServerUserItem==pIServerUserItem)
		{
			pServerUserItem->m_wUserIndex=0xFFFF;
			m_OnLineUserItem.RemoveAt(i);
			m_OffLineUserItem.Add(pServerUserItem);
			return true;
		}
	}

	ASSERT(FALSE);

	__LEAVE_FUNCTION_LOG_CATCH

	return false;
}

//ɾ������
bool __cdecl CServerUserManager::UnRegOffLineUserItem(IServerUserItem * pIServerUserItem)
{
	__ENTER_FUNCTION_LOG_CATCH

	//Ч�����
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	CServerUserItem * pServerUserItem=NULL;
	//tagServerUserData * pUserData=pIServerUserItem->GetUserData();

	//Ѱ���û�
	for (INT_PTR i=0;i<m_OffLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OffLineUserItem[i];
		ASSERT(pServerUserItem!=NULL);
		if (pServerUserItem==pIServerUserItem)
		{
			m_OffLineUserItem.RemoveAt(i);
			m_OnLineUserItem.Add(pServerUserItem);
			return true;
		}
	}

	__LEAVE_FUNCTION_LOG_CATCH


	return false;
}

//�����û�
IServerUserItem * __cdecl CServerUserManager::SearchOnLineUser(DWORD dwUserID)
{
	__ENTER_FUNCTION_LOG_CATCH


	CServerUserItem * pServerUserItem=NULL;
	for (INT_PTR i=0;i<m_OnLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OnLineUserItem[i];
		ASSERT(pServerUserItem!=NULL);
		if (pServerUserItem->m_ServerUserData.dwUserID==dwUserID) return pServerUserItem;
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return NULL;
}

//�û��Ƿ�����
bool  CServerUserManager::IsUserInServer(IServerUserItem* pServerUserItem)
{
	__ENTER_FUNCTION_LOG_CATCH


	for (INT_PTR i=0;i<m_OnLineUserItem.GetCount();i++)
	{
		ASSERT(pServerUserItem!=NULL);
		if (m_OnLineUserItem[i]==pServerUserItem) 
			return  true;
	}

	for (INT_PTR i=0;i<m_OffLineUserItem.GetCount();i++)
	{
		ASSERT(pServerUserItem!=NULL);
		if (m_OffLineUserItem[i]==pServerUserItem) 
			return  true;
	}

	__LEAVE_FUNCTION_LOG_CATCH


	return false;
}

//�����û�
IServerUserItem * __cdecl CServerUserManager::SearchOffLineUser(DWORD dwUserID)
{

	__ENTER_FUNCTION_LOG_CATCH

	CServerUserItem * pServerUserItem=NULL;
	for (INT_PTR i=0;i<m_OffLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OffLineUserItem[i];
		ASSERT(pServerUserItem!=NULL);
		if (pServerUserItem->m_ServerUserData.dwUserID==dwUserID) return pServerUserItem;
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return NULL;
}

//�����û�
IServerUserItem * __cdecl CServerUserManager::SearchOnLineUser(LPCTSTR pszAccounts)
{
	__ENTER_FUNCTION_LOG_CATCH

	CString strAccounts=pszAccounts,strAccountsTemp;
	strAccounts.MakeLower();
	CServerUserItem * pServerUserItem=NULL;
	for (INT_PTR i=0;i<m_OnLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OnLineUserItem[i];
		ASSERT(pServerUserItem!=NULL);
		strAccountsTemp=pServerUserItem->m_ServerUserData.szAccounts;
		strAccountsTemp.MakeLower();
		if (strAccounts==strAccountsTemp) return pServerUserItem;
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return NULL;
}

//�����û�
IServerUserItem * __cdecl CServerUserManager::SearchOffLineUser(LPCTSTR pszAccounts)
{
	__ENTER_FUNCTION_LOG_CATCH

	CString strAccounts=pszAccounts,strAccountsTemp;
	strAccounts.MakeLower();
	CServerUserItem * pServerUserItem=NULL;
	for (INT_PTR i=0;i<m_OffLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OffLineUserItem[i];
		ASSERT(pServerUserItem!=NULL);
		strAccountsTemp=pServerUserItem->m_ServerUserData.szAccounts;
		strAccountsTemp.MakeLower();
		if (strAccounts==strAccountsTemp) return pServerUserItem;
	}

	__LEAVE_FUNCTION_LOG_CATCH

	return NULL;
}

//////////////////////////////////////////////////////////////////////////