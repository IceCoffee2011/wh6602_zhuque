#ifndef BONUS_HEAD_FILE
#define BONUS_HEAD_FILE
#pragma once

//////////////////////////////////////////////////////////////////////////////////

#include "Stdafx.h"
#include "DXSKin.h"

//��Ʒ��Ϣ
struct tagBonusInfo
{
	//��Ʒ��Ϣ
	LONG							lBonusCount;							//�����Ŀ
	INT								nBenchmarkPosX;							//��׼λ��
	INT								nBenchmarkPosY;							//��׼λ��
	BYTE							byBackAlpha;							//����͸��
	BYTE							byScalingIndex;							//��ұ���
	BYTE							byStarIndex;							//��������
	BYTE							byShowTimeCount;						//��ʾʱ��
};

#define TEXTURE_INDEX_BONUS_BACK		0										//��������
#define TEXTURE_INDEX_BONUS_STAR		1										//��������
#define TEXTURE_INDEX_BONUS_DETAIL		2										//��������
#define TEXTURE_INDEX_BONUS_NUMBER		3										//��������
#define TEXTURE_INDEX_BONUS_LIGHT		4										//��������

//����Ʒ
class CBonus :public CDxSkin
{
	//��������
private:
	DWORD							m_dwDrawTime;
	DWORD							m_dwStartTime;
	CFactoryTemplate<tagBonusInfo>	m_BonusInfoFactory;						//��Ʒ����

	//��������
public:
	//���캯��
	CBonus(void);
	//��������
	~CBonus(void);

	//�滭����
public:
	//�滭���
	void DrawBonus();

	//�ӿں���
public:
	//���ӽ��
	void AddBonus(LONG lBonusCount, INT nBenchmarkPosX, INT nBenchmarkPosY);

	//�ڲ�����
private:
	//�滭����
	void DrawNumber(int nXPos, int nYPos, int nNumber, BYTE byAlphaBlend);
};

#endif