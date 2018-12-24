#include "StdAfx.h"
#include "gameclientdjinfo.h"
#include "UserDaoJu.h"
#include "ShowTimeDJList.h"


//����ͼ��
#define				GAME_DJ_ICO_PROP_FSK					TEXT("IMAGE_DJ_ICO_FSK")			//������
#define				GAME_DJ_ICO_PROP_JBK_S					TEXT("IMAGE_DJ_ICO_JBK_S")			//������(С)
#define				GAME_DJ_ICO_PROP_MLSS					TEXT("IMAGE_DJ_ICO_MLSS")			//����˫�տ�
#define				GAME_DJ_ICO_PROP_JBK_M					TEXT("IMAGE_DJ_ICO_JBK_M")			//������(��)
#define				GAME_DJ_ICO_PROP_JBK_L					TEXT("IMAGE_DJ_ICO_JBK_L")			//������(��)
#define				GAME_DJ_ICO_PROP_HSZFK					TEXT("IMAGE_DJ_ICO_HSZFK")			//����ף����
#define				GAME_DJ_ICO_PROP_GOODLUCK				TEXT("IMAGE_DJ_ICO_GOODLUCK")			//����Ů��
#define				GAME_DJ_ICO_PROP_GAME_RACE				TEXT("IMAGE_DJ_ICO_GAME_RACE")			//�����ʸ�




CGameClientDJInfo* CGameClientDJInfo::m_pDJInfo = NULL;


CGameClientDJInfo* CGameClientDJInfo::GetDJInfoInstance(void)
{
	if (NULL == m_pDJInfo)
	{
		m_pDJInfo = new CGameClientDJInfo();
	}
	return m_pDJInfo;
}

CGameClientDJInfo::CGameClientDJInfo(void)
{
	m_iCountCurrentDJTotalNums = 0;
	::memset(m_GameDJPropertyList, 0, sizeof(m_GameDJPropertyList));

	//������Դ
	LoadDJICOImage();
	//������
	m_vecUserHaveDJList.clear();
	m_vecUserUseDJList.clear();

	//
	m_pUserDaoJuInfo = NULL;
	m_pUserCurUseTimeDJ = NULL;
}


//�����û�����ָ��
void CGameClientDJInfo::SetUserDaoJuInfoPtr(CUserDaoJu* pUserDaoJuInfoPtr)
{
	if (NULL != pUserDaoJuInfoPtr)
	{
		m_pUserDaoJuInfo = pUserDaoJuInfoPtr;
	}
}

//������ʱ����ָ��
void CGameClientDJInfo::SetUserTimeDJPtr(CShowTimeDJList* pUserTimeDJPtr)
{
	if (NULL != pUserTimeDJPtr)
	{
		m_pUserCurUseTimeDJ = pUserTimeDJPtr;
	}
}

CGameClientDJInfo::~CGameClientDJInfo(void)
{
	m_pUserDaoJuInfo = NULL;
	m_pUserCurUseTimeDJ = NULL;
	m_pDJInfo = NULL;
}



//��ӵ����б�
bool CGameClientDJInfo::AddGameDJInfo(const tagPropertyInfo *djInfo)
{
	if (NULL == djInfo)
	{
		return false;
	}
	
	if (m_iCountCurrentDJTotalNums < PROPERTY_COUNT)
	{
		m_GameDJPropertyList[m_iCountCurrentDJTotalNums].idjID = djInfo->nPropertyID;				//����ID
		m_GameDJPropertyList[m_iCountCurrentDJTotalNums].dwLastingTime = djInfo->dwLastingTime;		//���߳���ʱ��
		++m_iCountCurrentDJTotalNums;
		
		return true;
	}
	
	return false;
}


//�����ҵ���
void CGameClientDJInfo::AddUserDJList(const CMD_GF_ResidualProperty *pUserDJInfo)
{
	if (NULL == pUserDJInfo)
	{
		return;
	}

	/*
	use         nums
	false		0			:û�����ֵ���
	false		>0			:ӵ�����ֵ��ߣ���û��ʹ�á�
	true		0			:����ʹ�ã������Ѿ�û�����ֵ����ˡ�
	true		>0			:����ʹ�ã����һ�ӵ�����ֵ��ߡ�
	*/

	//�������
	m_vecUserHaveDJList.clear();	//ӵ�е���
	m_vecUserUseDJList.clear();		//ʹ�õ���
	tag_user_dj_info djinfo;
	for (int i=0; i<PROPERTY_COUNT; i++)
	{
		if (pUserDJInfo->dwResidual[i].dwID>=PROP_FSK && pUserDJInfo->dwResidual[i].dwID<=PROP_GOODLUCK)
		{//���˲��ֵ���
			if (!pUserDJInfo->dwResidual[i].bUse&&0==pUserDJInfo->dwResidual[i].dwResidualTime)
			{//���û�����ֵ���
				continue;
			}
			memset(&djinfo, 0, sizeof(tag_user_dj_info));
			djinfo.idjID = pUserDJInfo->dwResidual[i].dwID;	//����ID
			djinfo.bUse = pUserDJInfo->dwResidual[i].bUse;	//�Ƿ���ͬ���͵ĵ�������ʹ��
			djinfo.idjNums = pUserDJInfo->dwResidual[i].dwResidualTime;	//����ʣ����߸���
			djinfo.dwResidualTimeCount = pUserDJInfo->dwResidual[i].dwResidualTimeCount;	//���߻�ʣ�����ʱ����Ч
			if (pUserDJInfo->dwResidual[i].dwResidualTime>0)
			{//ӵ�е���
				//�����ҵ���
				m_vecUserHaveDJList.push_back(djinfo);
			}
			if (pUserDJInfo->dwResidual[i].bUse && pUserDJInfo->dwResidual[i].dwResidualTimeCount>0)
			{//����ʹ�õ���
				//�����ҵ���
				m_vecUserUseDJList.push_back(djinfo);
			}
		}
	}

	if (NULL != m_pUserDaoJuInfo)
	{//���µ����б�
		m_pUserDaoJuInfo->UpdateUserDJPage();
	}
	if (NULL != m_pUserCurUseTimeDJ)
	{//���������ʱ������Ϣ
		m_pUserCurUseTimeDJ->SetDisableAndUpdateWnd(true);
	}
}

//����ĳ�ֵ���������ʣ��ʱ��
void CGameClientDJInfo::SetDJHaveProperty(CMD_GF_UserProperty* pUserProperty)
{
	if (NULL == pUserProperty)
	{
		return;
	}

	if (pUserProperty->dwID>=PROP_FSK && pUserProperty->dwID<=PROP_GOODLUCK)
	{
		bool bIsHave = false;
		for (int i=0; i<m_vecUserHaveDJList.size(); i++)
		{
			if (m_vecUserHaveDJList[i].idjID == pUserProperty->dwID)
			{
				bIsHave = true;
				m_vecUserHaveDJList[i].bUse = pUserProperty->bUse;
				m_vecUserHaveDJList[i].idjNums = pUserProperty->dwResidualTime;
				m_vecUserHaveDJList[i].dwResidualTimeCount = pUserProperty->dwResidualTimeCount;
				if (m_vecUserHaveDJList[i].idjNums <= 0)
				{//û�����ֵ����ˡ�
					DelUserDJFromIndex(i);
				}
				break;
			}
		}
		tag_user_dj_info djinfo;
		memset(&djinfo, 0, sizeof(tag_user_dj_info));
		djinfo.idjID = pUserProperty->dwID;	//����ID
		djinfo.bUse = pUserProperty->bUse;	//�Ƿ���ͬ���͵ĵ�������ʹ��
		djinfo.idjNums = pUserProperty->dwResidualTime;	//����ʣ����߸���
		djinfo.dwResidualTimeCount = pUserProperty->dwResidualTimeCount;	//���߻�ʣ�����ʱ����Ч
		if (!bIsHave)
		{//���û�����ֵ��ߣ���������ֵ��ߡ�
			if (pUserProperty->dwResidualTime>0)
			{//ӵ�е���
				//�����ҵ���
				m_vecUserHaveDJList.push_back(djinfo);
			}
		}
		bool IsHave = GetUserUseDJNums(pUserProperty->dwID);
		if (!IsHave)
		{//û��ʹ�ù����ֵ���.
			if (pUserProperty->bUse && pUserProperty->dwResidualTimeCount>0)
			{//����ʹ�õ���
				//�����ҵ���
				m_vecUserUseDJList.push_back(djinfo);
			}
		}
		if (NULL != m_pUserDaoJuInfo)
		{//���µ����б�
			m_pUserDaoJuInfo->UpdateUserDJPage();
		}
		if (NULL != m_pUserCurUseTimeDJ)
		{//���������ʱ������Ϣ
			m_pUserCurUseTimeDJ->SetDisableAndUpdateWnd(true);
		}
	}
}

//���ָ�����߸���
bool CGameClientDJInfo::GetUserUseDJNums(int iDJID)
{
	if (iDJID>=PROP_FSK && iDJID<=PROP_GOODLUCK)
	{
		for (int i=0; i<m_vecUserUseDJList.size(); i++)
		{
			if (m_vecUserUseDJList[i].idjID == iDJID)
			{
				return true;
			}
		}
	}
	return false;
}

//����ʹ�õ�����
void CGameClientDJInfo::SetDJUseProperty(CMD_GF_UserProperty* pUserProperty)
{
	if (NULL == pUserProperty)
	{
		return;
	}

	bool bIsFind = false;
	if (pUserProperty->dwID>=PROP_FSK && pUserProperty->dwID<=PROP_GOODLUCK)
	{
		for (int i=0; i<m_vecUserUseDJList.size(); i++)
		{
			if (m_vecUserUseDJList[i].idjID == pUserProperty->dwID)
			{
				m_vecUserUseDJList[i].bUse = pUserProperty->bUse;
				m_vecUserUseDJList[i].idjNums = pUserProperty->dwResidualTime;
				m_vecUserUseDJList[i].dwResidualTimeCount = pUserProperty->dwResidualTimeCount;
				
				if (!pUserProperty->bUse || m_vecUserUseDJList[i].dwResidualTimeCount <= 0)
				{//�˵���ʱ���ѵ���ʧЧ�ˡ�
					DelUserUseDJ(pUserProperty->dwID);
				}
				bIsFind = true;
				break;
			}
		}
	}
	if (!bIsFind && pUserProperty->dwResidualTimeCount>0)
	{//û�д˵�����Ϣ������ӵ�����Ϣ.
		tag_user_dj_info info;
		::memset(&info, 0, sizeof(tag_user_dj_info));
		info.idjID = pUserProperty->dwID;
		info.bUse = pUserProperty->bUse;
		info.idjNums = pUserProperty->dwResidualTime;
		info.dwResidualTimeCount = pUserProperty->dwResidualTimeCount;
		m_vecUserUseDJList.push_back(info);
	}
	if (NULL != m_pUserCurUseTimeDJ)
	{//���������ʱ������Ϣ
		m_pUserCurUseTimeDJ->SetResetWndSize();
	}
}


//ɾ���������ʹ�õĵ���
bool CGameClientDJInfo::DelUserUseDJ(int iDJID)
{
	if (iDJID>=PROP_FSK && iDJID<=PROP_GOODLUCK)
	{
		//ɾ����ǰ����
		std::vector<tag_user_dj_info>::iterator itemiter = m_vecUserUseDJList.begin();
		for (int i=0; (i<m_vecUserUseDJList.size()) && (itemiter!=m_vecUserUseDJList.end()); itemiter++, i++)
		{
			if (i==iDJID)
			{
				m_vecUserUseDJList.erase(itemiter);
				return true;
			}
		}
	}
	return false;
}



//ɾ��ӵ�еĵ���
int CGameClientDJInfo::DelUserDJFromIndex(int iDJIndex)
{
	if (iDJIndex<0 || iDJIndex>=m_vecUserHaveDJList.size())
	{
		return 0;
	}

	bool bIsHave = false;
	//ɾ����ǰ����
	std::vector<tag_user_dj_info>::iterator itemiter = m_vecUserHaveDJList.begin();
	for (int i=0; (i<m_vecUserHaveDJList.size()) && (itemiter!=m_vecUserHaveDJList.end()); itemiter++, i++)
	{
		if (i==iDJIndex)
		{
			m_vecUserHaveDJList.erase(itemiter);
			bIsHave = true;
			break;
		}
	}
	if (false == bIsHave)
	{//û���ҵ�
		return 0;
	}

	return iDJIndex;
}



//ɾ����������ID�й�
bool CGameClientDJInfo::DelUserAllDJID(int iDJID)
{
	if (iDJID>=PROP_FSK && iDJID<=PROP_GOODLUCK)
	{
		int iSaveDJID = DelUserDJFromDJID(iDJID);
		bool bIsSuc = DelUserUseDJ(iDJID);

		if (iSaveDJID==iDJID)
		{
			if (NULL != m_pUserDaoJuInfo)
			{//���µ����б�
				m_pUserDaoJuInfo->UpdateUserDJPage();
			}
		}
		if (bIsSuc)
		{
			if (NULL != m_pUserCurUseTimeDJ)
			{//���������ʱ������Ϣ
				m_pUserCurUseTimeDJ->SetResetWndSize();
			}
		}
	}
	return false;
}


int CGameClientDJInfo::DelUserDJFromDJID(int iDJID)
{
	if (iDJID>=PROP_FSK && iDJID<=PROP_GOODLUCK)
	{
		for (int i=0; i<m_vecUserHaveDJList.size(); i++)
		{
			if (m_vecUserHaveDJList[i].idjID == iDJID)
			{
				if (i == DelUserDJFromIndex(i))
				{
					if (NULL != m_pUserDaoJuInfo)
					{
						m_pUserDaoJuInfo->UpdateUserDJInfo(i);
					}
					return iDJID;
				}
				else
				{
					return 0;
				}
			}
		}
	}
	return 0;
}


//����ͼƬ��Դ
void CGameClientDJInfo::LoadDJICOImage()
{
	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
	char chImageData[100]={0};

	//������
	m_ImageDJICO[PROP_FSK].LoadImage(hInstance, GAME_DJ_ICO_PROP_FSK);
	memset(chImageData, 0, sizeof(chImageData));
	sprintf(chImageData, TEXT("%s_L"), GAME_DJ_ICO_PROP_FSK);
	m_ImageDJICOLight[PROP_FSK].LoadImage(hInstance, chImageData);

	//������(С)
	m_ImageDJICO[PROP_JBK_S].LoadImage(hInstance, GAME_DJ_ICO_PROP_JBK_S);
	memset(chImageData, 0, sizeof(chImageData));
	sprintf(chImageData, TEXT("%s_L"), GAME_DJ_ICO_PROP_JBK_S);
	m_ImageDJICOLight[PROP_JBK_S].LoadImage(hInstance, chImageData);

	//����˫�տ�
	m_ImageDJICO[PROP_MLSS].LoadImage(hInstance, GAME_DJ_ICO_PROP_MLSS);
	memset(chImageData, 0, sizeof(chImageData));
	sprintf(chImageData, TEXT("%s_L"), GAME_DJ_ICO_PROP_MLSS);
	m_ImageDJICOLight[PROP_MLSS].LoadImage(hInstance, chImageData);

	//������(��)
	m_ImageDJICO[PROP_JBK_M].LoadImage(hInstance, GAME_DJ_ICO_PROP_JBK_M);
	memset(chImageData, 0, sizeof(chImageData));
	sprintf(chImageData, TEXT("%s_L"), GAME_DJ_ICO_PROP_JBK_M);
	m_ImageDJICOLight[PROP_JBK_M].LoadImage(hInstance, chImageData);

	//������(��)
	m_ImageDJICO[PROP_JBK_L].LoadImage(hInstance, GAME_DJ_ICO_PROP_JBK_L);
	memset(chImageData, 0, sizeof(chImageData));
	sprintf(chImageData, TEXT("%s_L"), GAME_DJ_ICO_PROP_JBK_L);
	m_ImageDJICOLight[PROP_JBK_L].LoadImage(hInstance, chImageData);

	//����ף����
	m_ImageDJICO[PROP_HSZFK].LoadImage(hInstance, GAME_DJ_ICO_PROP_HSZFK);
	memset(chImageData, 0, sizeof(chImageData));
	sprintf(chImageData, TEXT("%s_L"), GAME_DJ_ICO_PROP_HSZFK);
	m_ImageDJICOLight[PROP_HSZFK].LoadImage(hInstance, chImageData);

	//����Ů��
	m_ImageDJICO[PROP_GOODLUCK].LoadImage(hInstance, GAME_DJ_ICO_PROP_GOODLUCK);
	memset(chImageData, 0, sizeof(chImageData));
	sprintf(chImageData, TEXT("%s_L"), GAME_DJ_ICO_PROP_GOODLUCK);
	m_ImageDJICOLight[PROP_GOODLUCK].LoadImage(hInstance, chImageData);

	//�����ʸ�
	m_ImageDJICO[PROP_GAME_RACE].LoadImage(hInstance, GAME_DJ_ICO_PROP_GAME_RACE);
	memset(chImageData, 0, sizeof(chImageData));
	sprintf(chImageData, TEXT("%s_L"), GAME_DJ_ICO_PROP_GAME_RACE);
	m_ImageDJICOLight[PROP_GAME_RACE].LoadImage(hInstance, chImageData);
}


//���ݵ���ID���ͼƬ
CPngImage* CGameClientDJInfo::GetImageFromeDJID(int idjID)
{
	if (idjID<0 || idjID>=PROPERTY_COUNT)
	{
		return NULL;
	}
	return &m_ImageDJICO[idjID];
}
//��ü�����ͼƬ
CPngImage* CGameClientDJInfo::GetLightImageFromeDJID(int idjID)
{
	if (idjID<0 || idjID>=PROPERTY_COUNT)
	{
		return NULL;
	}
	return &m_ImageDJICOLight[idjID];
}

//��õ�����Ϣ
const GC_DJPropertyInfo* CGameClientDJInfo::GetGameDJInfo(int idjid)
{
	if (idjid<0 || idjid>=PROPERTY_COUNT)
	{
		return NULL;
	}
	return &m_GameDJPropertyList[idjid];
}


//������Ż�õ���ID
int CGameClientDJInfo::GetDJIDFromDJIndex(int iIndex)
{
	if (iIndex<0 || iIndex>=m_vecUserHaveDJList.size())
	{
		return -1;
	}
	
	return m_vecUserHaveDJList[iIndex].idjID;
}


//�������ʹ�õĲ�������ʱ���߸���
int CGameClientDJInfo::GetUserUseDJAndTimeDJNums()
{
	return m_vecUserUseDJList.size();
}

//������û��ʹ�õĵ���
int CGameClientDJInfo::GetUserNoUseDJNums()
{
	return m_vecUserHaveDJList.size();
}

//ʹ������ʱ���ߵ���ʱ.������������ʱ����.
bool CGameClientDJInfo::SetTimeDJAndClearDJ()
{
	bool bIsHaveZero = false;
	for (int i=0; i<m_vecUserUseDJList.size(); i++)
	{
		if (m_vecUserUseDJList[i].idjID>=PROP_FSK && m_vecUserUseDJList[i].idjID<=PROP_GOODLUCK)
		{
			{//�������ʹ�õĵ���
				--m_vecUserUseDJList[i].dwResidualTimeCount;
				if (m_vecUserUseDJList[i].dwResidualTimeCount <= 0)
				{
					bIsHaveZero = true;
				}
			}
		}
	}
	
	if (true == bIsHaveZero)
	{//���ô��ڴ�С
		std::vector<tag_user_dj_info>::iterator itemiter;
		for (itemiter=m_vecUserUseDJList.begin(); (m_vecUserUseDJList.size()>0) && (itemiter!=m_vecUserUseDJList.end()); itemiter++)
		{
			if (itemiter->dwResidualTimeCount<=0)
			{
				m_vecUserUseDJList.erase(itemiter);
				itemiter=m_vecUserUseDJList.begin();
			}
		}
	}

	return bIsHaveZero;
}