#ifndef COIN_HEAD_FILE
#define COIN_HEAD_FILE
#pragma once

//////////////////////////////////////////////////////////////////////////////////

#include "Stdafx.h"
#include "DXSKin.h"

//�����Ϣ
struct tagCoinInfo
{
	//�����Ϣ
	LONG							lCoinCount;								//�����Ŀ
	BYTE							byTextureIndex;							//��������
	BYTE							byAlphaBlend;							//͸����

	//�켣����
	POINT							ptNumber;								//��������
	BYTE							byFrameIndex;							//֡������
	DWORD							dwMoveTimes;							//�ƶ�ʱ��
	BYTE							bySpaceTimes;							//���ʱ��
	POINT							ptTrace[500];							//�켣����
	WORD							wTracePtCount;							//�켣��Ŀ
	WORD							wTracePtIndex;							//�켣����
	float							fScaling;								//���Ŵ�С
};

#define TEXTURE_INDEX_COIN_SHOW			0										//��������
#define TEXTURE_INDEX_COIN_MOVE_1		1										//��������
#define TEXTURE_INDEX_COIN_MOVE_2		2										//��������
#define TEXTURE_INDEX_COIN_MOVE_3		3										//��������
#define TEXTURE_INDEX_COIN_BK			4										//��������

#define COIN_FRAME_COUNT				11										//֡��Ŀ

//������
class CCoin :public CDxSkin
{
	//��������
private:
	CFactoryTemplate<tagCoinInfo>	m_CoinInfoFactory;						//��ҹ���

	//��������
public:
	//���캯��
	CCoin(void);
	//��������
	~CCoin(void);

	//�滭����
public:
	//�滭���
	void DrawCoin();

	//�ӿں���
public:
	//���ӽ��
	void AddCoin(int nSourcePosX, int nSourcePosY, int nTargetPosX, int nTargetPosY, int nNumberX,int nNumberY,LONG lCoinCount, BYTE byTextureIndex);

	//�ڲ�����
private:
	//�滭����
	void DrawNumber(int nXPos, int nYPos, int nNumber, BYTE byAlphaBlend);
};

#endif