#ifndef CANNON_HEAD_FILE
#define CANNON_HEAD_FILE
#pragma once
#include "stdafx.h"
#include "dxskin.h"

#define TEXTURE_INDEX_BACK			0										//��������
#define TEXTURE_INDEX_CANNON		1										//��������
#define TEXTURE_INDEX_NUM			2										//��������

#define CANNON_FRAME_COUNT			5										//��̨֡��

#define MAX_BULLET_COUNT			5										//�ӵ���Ŀ

#define BULLET_NUMBER_WIDTH			20										//��Ŀ���
#define BULLET_NUMBER_HEIGHT		21										//��Ŀ�߶�

//��̨״̬
enum enCannonStatus
{
	enCannonStatus_No,				//����״̬
	enCannonStatus_Normal,			//����״̬
	enCannonStatus_Shoot			//����״̬
};

//�����ڵ�
class CCannon :public CDxSkin
{
	//�ں˱���
private:
	HWND							m_hNotifyWnd;							//֪ͨ����

	//λ�ñ���
private:
	POINT							m_ptBenchmarkPos[GAME_PLAYER];			//��׼λ��
	POINT							m_ptNumberPos[GAME_PLAYER];				//����λ��
	float							m_fAngleCan[GAME_PLAYER];				//���ڽǶ�
	float							m_fAngleBk[GAME_PLAYER];				//�����Ƕ�
	WORD							m_wMeChairID;							//�Լ���λ
	float							m_fCountScaling;						//��Ŀ����

	//״̬����
private:
	DWORD							m_dwChangeTimes[GAME_PLAYER];			//�ı�ʱ��
	DWORD							m_dwScalingTimes;						//�ı�ʱ��
	bool							m_bCountScaling;						//���ű�ʶ
	float							m_fScalingSpace;						//��������
	int								m_wBulletCount[GAME_PLAYER];			//�ӵ���Ŀ
	int								m_nFrameIndex[GAME_PLAYER];				//�滭����
	int								m_nFrameIndexBack[GAME_PLAYER];			//�滭����
	enCannonStatus					m_enCannonStatus[GAME_PLAYER];			//��̨״̬

	//��������
public:
	//���캯��
	CCannon(void);
	//��������
	~CCannon(void);

	//�ӿں���
public:
	//�滭����
	void DrawCannon();
	//���ô���
	void SetNotifyWnd(HWND hNotifyWnd) {m_hNotifyWnd=hNotifyWnd; }
	//�����ڵ�
	void Shoot(WORD wChairID);
	//�ӵ���Ŀ
	void SetBulletCount(WORD wChairID, int nBulletCount, bool bScale);
	//�ӵ���Ŀ
	int GetBulletCount(WORD wChairID) { return m_wBulletCount[wChairID];}
	//������λ
	void SetMeChairID(WORD wChairID) {m_wMeChairID=wChairID;}

	//λ�ú���
public:
	//����λ��
	void SetNumberPos(WORD wChairID, int nPosX, int nPosY);
	//����λ��
	void SetBenchmarkPos(WORD wChairID, int nPosX, int nPosY);
	//��ȡλ��
	POINT &GetBenchmarkPos(WORD wChairID) {return m_ptBenchmarkPos[wChairID];}
	//���ýǶ�
	void SetAngleCan(WORD wChairID, float fAngle);
	//��ȡ�Ƕ�
	float GetAngleCan(WORD wChairID) {return m_fAngleCan[wChairID];}
	//���ýǶ�
	void SetAngleBk(WORD wChairID, float fAngle) {m_fAngleBk[wChairID]=fAngle;}
	//��ȡ�Ƕ�
	float GetAngleBk(WORD wChairID) {return m_fAngleBk[wChairID];}
};
#endif