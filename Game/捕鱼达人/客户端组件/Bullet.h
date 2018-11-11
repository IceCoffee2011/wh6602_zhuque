#ifndef BULLET_HEAD_FILE
#define BULLET_HEAD_FILE
#pragma once
#include "dxskin.h"
#include "Fish.h"

//�ӵ�״̬
enum enBulletStatus
{
	enBulletStatus_No,				//����״̬
	enBulletStatus_Shoot,			//���״̬
	enBulletStatus_Net,				//����״̬
};

//�ӵ�����
enum enBulletKind
{
	enBulletKind_1,					//�ӵ�����
	enBulletKind_2,					//�ӵ�����
	enBulletKind_3,					//�ӵ�����
	enBulletKind_4,					//�ӵ�����
	enBulletKind_5,					//�ӵ�����
	enBulletKind_No					//��������
};

//�ӵ�֡��
int const g_nShootColumnCount[enBulletKind_No]={1,1,1,1,1};
int const g_nShootRowCount[enBulletKind_No]={1,1,1,1,1};
int const g_nShootFrameCount[enBulletKind_No]={1,1,1,1,1};
int const g_nNetColumnCount[enBulletKind_No]={8,8,8,8,8};
int const g_nNetRowCount[enBulletKind_No]={2,2,2,2,2};
//int const g_nNetFrameCount[enBulletKind_No][5]={{6,7,8,9,16},{6,7,8,9,16},{6,7,8,9,16},{6,7,8,9,16},{6,7,8,9,16}};
int const g_nNetFrameCount[enBulletKind_No][5]={{16,16,16,16,16},{16,16,16,16,16},{16,16,16,16,16},{16,16,16,16,16},{16,16,16,16,16}};

int const g_nNetRadiusX[enBulletKind_No]={55,60,65,70,75};
int const g_nNetRadiusY[enBulletKind_No]={55,60,65,70,75};

#define MAX_CPTR_FISH_COUNT			15										//������Ŀ

#define TEXTURE_INDEX_SHOOT_1		0										//��������
#define TEXTURE_INDEX_SHOOT_2		1										//��������
#define TEXTURE_INDEX_SHOOT_3		2										//��������
#define TEXTURE_INDEX_NET			3										//��������

#define BULLET_SPEED_NORMAL			40										//�ӵ��ٶ�
#define BULLET_SPEED_ACCELERATE		30										//�ӵ��ٶ�

#define SHOOT_ROUND_SHRINK_X		90										//�ӵ��ٶ�
#define SHOOT_ROUND_SHRINK_Y		70										//�ӵ��ٶ�

#define BULLET_FRAME_COUNT			10										//�ӵ�֡��

//#define WM_NET_OPEN					WM_USER+310								//����

//�ӵ���Ϣ
struct tagBulletInfo
{
	//�ӵ���Ϣ
	enBulletStatus					BulletStatus;							//����״̬
	DWORD							dwCptrFishID[MAX_CPTR_FISH_COUNT];		//��������
	bool							bCptrMode;								//����ģʽ
	DWORD							dwBulletID;								//�ӵ���ʶ
	BYTE							byBulletCount;							//�ӵ���Ŀ
	BYTE							byShootUser;							//������
	BYTE							byTextureIndex;							//��������

	//�켣����
	//BYTE							byAddedFrame;							//����֡��
	BYTE							byFrameIndex;							//֡������
	DWORD							dwMoveTimes;							//�ƶ�ʱ��
	BYTE							bySpaceTimes;							//���ʱ��
	float							fAngle;									//�켣�Ƕ�
	POINT							ptTrace[500];							//�켣����
	WORD							wTracePtCount;							//�켣��Ŀ
	WORD							wTracePtIndex;							//�켣����
};

//�ӵ���
class CBullet :public CDxSkin
{
	//�ں˱���
private:
	HWND							m_hNotifyWnd;							//֪ͨ����
	enBulletKind					m_enBulletKind;							//�ӵ�����
	CFactoryTemplate<tagBulletInfo>	m_BulletFactory;						//�ӵ�����
	DWORD							m_dwBulletID;							//�ӵ���ʶ

	//λ�ñ���
private:
	CRect							m_rcBound;								//�����Χ

	//��̬����
private:

	//��������
public:
	//���캯��
	CBullet(void);
	//��������
	~CBullet(void);

	//�ӿں���
public:
	//���ô���
	void SetNotifyWnd(HWND hNotifyWnd) {m_hNotifyWnd=hNotifyWnd; }
	//�����ӵ�
	tagBulletInfo *ActiveNewBullet();
	//�����ӵ�
	DWORD Shoot(WORD wChairID, bool bCptrMode, DWORD dwCptrFishID[], WORD wCptrFishCount, int nSourceX, int nSourceXY, float fAngle, enBulletCountKind BulletCountKind, BYTE byBulletCount);
	//�滭�ӵ�
	void DrawBullet();
	//���÷�Χ
	void SetBound(CRect &rcBound) {m_rcBound=rcBound;}
};

#endif