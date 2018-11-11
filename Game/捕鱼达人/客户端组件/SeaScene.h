#ifndef SEA_SCENE_HEAD_FILE
#define SEA_SCENE_HEAD_FILE

#pragma once
#include "stdafx.h"
#include "dxskin.h"

//������Ϣ
struct tagBiologyInfo
{
	int								nPosX;									//����λ��
	int								nPosY;									//����λ��
	bool							bAsc;									//�����ʶ
	float							fReAngle;								//��ת�Ƕ�
	float							fScaling;								//���ű���
	BYTE							byFrameIndex;							//��������
	BYTE							byFrameCount;							//����֡��
	BYTE							byTextureIndex;							//��������
	BYTE							byColumnCount;							//��������
	DWORD							dwMoveTimes;							//�ƶ�ʱ��
	BYTE							bySpaceTimes;							//���ʱ��
};

//������Ϣ
struct tagBubble
{
	int								nPosX;									//����λ��
	int								nPosY;									//����λ��
	int								nMoveSpaceX;							//����λ��
	int								nMoveSpaceY;							//����λ��
	float							fScaling;								//���ű���
	BYTE							byAlphaBlend;							//͸����
	DWORD							dwMoveTimes;							//�ƶ�ʱ��
};

//���׳���
class CSeaScene :public CDxSkin
{
	//�ں˱���
private:
	bool								m_bTride;							//�ǳ���ʶ
	bool								m_bReverse;							//��ת��ʶ
	enFishKind							m_enFishKind;						//��������
	CFactoryTemplate<tagBiologyInfo>	m_BiologyFactory;					//���﹤��
	tagBubble							m_Bubble[8];						//������Ϣ

	//��������
public:
	//���캯��
	CSeaScene();
	//��������
	virtual ~CSeaScene();

	//�ӿں���
public:
	//�ǳ���ʶ
	void SetTride(bool bTride) {m_bTride=bTride;}
	//�滭����
	void DrawScene();
	//���÷�ת
	void SetReverse(bool bReverse) {m_bReverse=bReverse;}
	//��ȡ��ת
	bool IsReverse() {return m_bReverse;}
	//��������
	bool AddBiology(int nPosX, int nPosY, BYTE byFrameCount, BYTE byTextureIndex, BYTE byColumnCount, float fReAngle=0.f, BYTE bySpaceTimes=50, float fScaling=1.f);
	//���ó���
	void ResetScene() {m_BiologyFactory.FreeAllItem();}

	//�ڲ�����
private:
	//��������
	void MakeBubble(tagBubble &Bubble);
};					

#endif
