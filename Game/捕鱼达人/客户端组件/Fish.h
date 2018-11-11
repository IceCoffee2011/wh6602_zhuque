//////////////////////////////////////////////////////////////////////////////////

#ifndef FISH_HEAD_FILE
#define FISH_HEAD_FILE
#pragma once

//////////////////////////////////////////////////////////////////////////////////

#include "Stdafx.h"
#include "DXSKin.h"

//��Ⱥ״̬
enum enFishStatus
{
	enFishStatus_No,				//����״̬
	enFishStatus_Alive,				//��Ծ״̬
	enFishStatus_Die,				//����״̬
};

#define TEXTURE_INDEX_FISH			0										//��������

//�ƶ��ٶ�
int const g_nMoveSpeed[enFishKind_No]={40,40,40,35,35,32,32,50,33,32,60,60};
int const g_nDieSpeed[enFishKind_No]={10,10,10,10,6,10,10,10,10,10,10,10};

//ͼƬ��Ϣ
int const g_nColumnCount[enFishKind_No]=	{8,8,8,8,8,8,8,6,8,8,4,4};
int const g_nRowCount[enFishKind_No]=		{3,5,4,14,6,3,5,5,5,5,5,3};
int const g_nAliveFrameCount[enFishKind_No]={14,24,20,96,17,15,21,18,15,13,12,12};
int const g_nTotalFrameCount[enFishKind_No]={22,34,32,106,45,22,33,30,37,36,20,12};
int const g_nShootRoundW[enFishKind_No]={40,56,68,56,68,88,72,80,166,180,354,368};
int const g_nShootRoundH[enFishKind_No]={10,18,24,49,30,20,26,44,60,50,60,60};

//int const g_nDrawCenterExcsX[enFishKind_No]={0,5,5,0,5,0,5,0,0,0,10};
//int const g_nDrawCenterExcsY[enFishKind_No]={0,0,5,0,0,0,0,0,0,0,-5};

//������Ϣ
struct tagFishInfo
{
	//״̬��Ϣ
	bool							bActive;								//�����ж�

	//��Ⱥ��Ϣ
	enFishStatus					FishStatus;								//����״̬
	DWORD							dwFishID;								//�����ʶ
	BYTE							byCptrPro;								//�������

	//�켣����
	BYTE							byFrameIndex;							//֡������
	DWORD							dwMoveTimes;							//�ƶ�ʱ��
	BYTE							bySpaceTimes;							//���ʱ��
	float							fAngle;									//�켣�Ƕ�
	float							fAngleStep;								//�켣����
	POINT							ptTrace[500];							//�켣����
	WORD							wTracePtCount;							//�켣��Ŀ
	WORD							wTracePtIndex;							//�켣����
	BYTE							byDieCount;								//������Ŀ
	BYTE							byShootUser;							//������

	//������Ϣ
	BYTE							byBulletCount;							//�ӵ���Ŀ	
};

//#define WM_CAPTURE_FISH				WM_USER+200								//������Ⱥ
//#define WM_HIT_FISH					WM_USER+201								//������Ⱥ

////////////////////////////////////////////////////////////////////////////////
//��������
enum enBulletKind;

//dxͼ����
class CFishGroup :public CDxSkin
{
	//�ں˱���
private:
	HWND							m_hNotifyWnd;							//֪ͨ����
	enFishKind						m_enFishKind;							//��Ⱥ����
	CFactoryTemplate<tagFishInfo>	m_FishFactory;							//��Ⱥ����

	//��̬����
private:
	static bool						m_bWaveComing;							//�ǳ�����

	//��������
public:
	//���캯��
	CFishGroup();
	//��������
	virtual ~CFishGroup();

	//��̬����
public:
	//�ǳ�����
	static void SetWaveComing(bool bComing) {m_bWaveComing=bComing;}

	//���ܺ���
public:
	//D X �滭
	void DrawFish();
	//�����ж�
	bool ShootTest(WORD wChairID, int nPosX, int nPosY, enBulletKind BulletKind, bool bCptrMode, DWORD dwBulletID, BYTE byBulletCount, bool bNetStatus);
	//�����ж�
	bool PtInFish(tagFishInfo *pFishInfo, int nCurIndex, int nPosShootX, int nPosShootY);
	//����״̬
	bool SetStatus(DWORD dwFishID, enFishStatus FishStatus);
	//��ȡ����
	tagFishInfo *GetFish(DWORD dwFishID);
	//��������
	tagFishInfo *ActiveNewFish();
	//ö������
	tagFishInfo *EnumFish(WORD wIndex);
	//��ȡ��Ŀ
	WORD GetFishCount();
	//�ͷ���Ⱥ
	void FreeFish(tagFishInfo *pFishInfo);
		
	//���ú���
public:
	//��ʼ����
	bool InitTrace(int nInitX[], int nInitY[], int nInitCount, DWORD dwFishID);
	//��ʼ����
	bool InitTraceLinear(int nInitX[], int nInitY[], int nInitCount, DWORD dwFishID);
	//��Ⱥ��Ϣ
	void SetGroupInfo(HWND hNotifyWnd, enFishKind FishKind) {m_hNotifyWnd=hNotifyWnd; m_enFishKind=FishKind;}
	//������Ⱥ
	void ResetKenel();	
};

//��Ⱥ����
extern CFishGroup						g_FishGroup[enFishKind_No];

////////////////////////////////////////////////////////////////////////////////
#endif