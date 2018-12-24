//Create Author: osc
//Create Date:	2011-4-14
//Description:	������Ϣ�б�,�������˵�����Ϣ

#ifndef		__GAME_CLIENT_FRAME_DJ_INFO_DEF___
#define		__GAME_CLIENT_FRAME_DJ_INFO_DEF___

#include "StdAfx.h"
#include "GameFrame.h"
#include <vector>

using namespace std;

//������Ϣ�ṹ
struct	GC_DJPropertyInfo
{
	int				idjID;				//����ID
	DWORD			dwLastingTime;		//���߳���ʱ��	
};

//��ҵ�ǰ�����б�
struct	tag_user_dj_info
{
	int		idjID;		//����ID
	int		idjNums;	//���߸���
	bool	bUse;       //�Ƿ��е�������ʹ��
	DWORD	dwResidualTimeCount;  //��ʣ�¶���ʱ�����ʧȥ���� ʱ��Ϊ��
};

class CUserDaoJu;
class CShowTimeDJList;

//���������Ϣ
class CGameClientDJInfo
{
public:
	//��õ�ǰΨһʵ��
	static CGameClientDJInfo* GetDJInfoInstance(void);
	//�����û�����ָ��
	void SetUserDaoJuInfoPtr(CUserDaoJu* pUserDaoJuInfoPtr);
	//������ʱ����ָ��
	void SetUserTimeDJPtr(CShowTimeDJList* pUserTimeDJPtr);

	//��ӵ�����Ϣ
	bool AddGameDJInfo(const tagPropertyInfo *djInfo);
	//��õ�����Ϣ
	const GC_DJPropertyInfo* GetGameDJInfo(int idjid);
	//���ݵ���ID���ͼƬ
	CPngImage* GetImageFromeDJID(int idjID);
	CPngImage* GetLightImageFromeDJID(int idjID);

	//�����ҵ���
	void AddUserDJList(const CMD_GF_ResidualProperty *pUserDJInfo);
	//ɾ������
	int DelUserDJFromIndex(int iDJIndex);
	int DelUserDJFromDJID(int iDJID);
	//������Ż�õ���ID
	int GetDJIDFromDJIndex(int iIndex);

	//�������ʹ�õĲ�������ʱ���߸���
	int GetUserUseDJAndTimeDJNums();
	//������û��ʹ�õĵ���
	int GetUserNoUseDJNums();
	//ʹ������ʱ���ߵ���ʱ.������������ʱ����.
	bool SetTimeDJAndClearDJ();
	//���ָ�����߸���
	bool GetUserUseDJNums(int iDJID);

	//����ĳ�ֵ�����������ʣ��ʱ��
	void SetDJHaveProperty(CMD_GF_UserProperty* pUserProperty);
	//����ʹ�õ�����
	void SetDJUseProperty(CMD_GF_UserProperty* pUserProperty);
	
	//ɾ���������ʹ�õĵ���
	bool DelUserUseDJ(int iDJID);
	//ɾ����������ID�й�
	bool DelUserAllDJID(int iDJID);
	
public:
	//��ҵ����б�
	std::vector<tag_user_dj_info>	m_vecUserHaveDJList;	//ӵ�е���
	std::vector<tag_user_dj_info>	m_vecUserUseDJList;		//ʹ�õ���

protected:
	CGameClientDJInfo(void);
public:
	virtual ~CGameClientDJInfo(void);
	
private:
	static CGameClientDJInfo*	m_pDJInfo;
	CUserDaoJu*					m_pUserDaoJuInfo;//��ҵ�����Ϣ
	CShowTimeDJList*			m_pUserCurUseTimeDJ;	//��ҵ�ǰ����ʹ�õ���ʱ����
	
private:
	CPngImage						m_ImageDJICO[PROPERTY_COUNT];				//����ͼ��
	CPngImage						m_ImageDJICOLight[PROPERTY_COUNT];			//���߼���ͼ��
	GC_DJPropertyInfo				m_GameDJPropertyList[PROPERTY_COUNT];		//�����б�
	DWORD							m_iCountCurrentDJTotalNums;					//��¼��ǰ�ܵ�����

private:
	//������Դ
	void	LoadDJICOImage();

	
};



#endif