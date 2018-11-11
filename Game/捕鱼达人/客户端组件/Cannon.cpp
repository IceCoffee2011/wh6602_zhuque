#include "StdAfx.h"
#include "cannon.h"

//���캯��
CCannon::CCannon(void)
{
	//�ں˱���
	m_hNotifyWnd=NULL;

	//λ�ñ���
	m_fScalingSpace=0.f;
	m_bCountScaling=false;
	m_wMeChairID=0;
	m_fCountScaling=1.f;
	m_dwScalingTimes=0L;
	ZeroMemory(m_ptBenchmarkPos,sizeof(m_ptBenchmarkPos));
	ZeroMemory(m_ptNumberPos,sizeof(m_ptNumberPos));
	for (int i=0; i<CountArray(m_fAngleCan); ++i) 
	{
		m_fAngleCan[i]=D3DX_PI/2;
		m_fAngleBk[i]=0.f;
		m_wBulletCount[i]=0;
		m_enCannonStatus[i]=enCannonStatus_Normal;
		m_dwChangeTimes[i]=0;
	}

	//״̬����
	ZeroMemory(m_nFrameIndex,sizeof(m_nFrameIndex));
	ZeroMemory(m_nFrameIndexBack,sizeof(m_nFrameIndexBack));
}

//��������
CCannon::~CCannon(void)
{
}

//�滭����
void CCannon::DrawCannon()
{
	//�Ϸ��ж�
	if (m_Sprite==NULL||m_pTexture==NULL) return ;	

	//�滭λ��
	CRect drawRect;

	//�滭����
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{	
		//��Ŀ�ж�
		if (0==m_wBulletCount[wChairID]) continue;

		//����λ��
		drawRect.left=3 * GetWidth(TEXTURE_INDEX_NUM);
		drawRect.top=0;
		drawRect.right=GetImageWidth();
		drawRect.bottom=GetImageHeight();

		//���ű���
		float fCountScaling=1.f;
		if (wChairID==m_wMeChairID) fCountScaling=m_fCountScaling;

		//����ƫ��
		int nExcursionX=int((fCountScaling-1.f)*(drawRect.Width()/2+45));
		int nExcursionY=int((fCountScaling-1.f)*(drawRect.Height()/2+10));

		//�滭����
		DrawRotateAndZoom(m_ptNumberPos[wChairID].x+nExcursionX,m_ptNumberPos[wChairID].y+nExcursionY,drawRect,0.f,fCountScaling,fCountScaling,TEXTURE_INDEX_NUM);

		//�滭��Ŀ
		drawRect.left=(m_wBulletCount[wChairID]-1)%3 * GetWidth(TEXTURE_INDEX_NUM);
		drawRect.top=(m_wBulletCount[wChairID]-1)/3 * GetHeight(TEXTURE_INDEX_NUM);
		drawRect.right=drawRect.left+GetWidth(TEXTURE_INDEX_NUM);
		drawRect.bottom=drawRect.top+GetHeight(TEXTURE_INDEX_NUM);
		DrawRotateAndZoom(m_ptNumberPos[wChairID].x-53,m_ptNumberPos[wChairID].y-20,drawRect,0.f,fCountScaling,fCountScaling,TEXTURE_INDEX_NUM);

		//�滭λ��
		drawRect.left=m_nFrameIndexBack[wChairID] * GetWidth(TEXTURE_INDEX_BACK);
		drawRect.top=0;
		drawRect.right=drawRect.left+GetWidth(TEXTURE_INDEX_BACK);
		drawRect.bottom=drawRect.top+GetHeight(TEXTURE_INDEX_BACK);

		//�滭����
		DrawRotateAndZoom(m_ptBenchmarkPos[wChairID].x,m_ptBenchmarkPos[wChairID].y,drawRect,m_fAngleBk[wChairID],1.f,1.f,TEXTURE_INDEX_BACK);

		//�滭����
		drawRect.left=m_nFrameIndex[wChairID] * GetWidth(TEXTURE_INDEX_CANNON);
		drawRect.top=(m_wBulletCount[wChairID]-1) * GetHeight(TEXTURE_INDEX_CANNON);
		drawRect.right=drawRect.left+GetWidth(TEXTURE_INDEX_CANNON);
		drawRect.bottom=drawRect.top+GetHeight(TEXTURE_INDEX_CANNON);

		//�滭ͼƬ
		DrawRotateAndZoom(m_ptBenchmarkPos[wChairID].x,m_ptBenchmarkPos[wChairID].y-4,drawRect,m_fAngleCan[wChairID],(drawRect.right-drawRect.left)/2,41,1.f,1.f,TEXTURE_INDEX_CANNON);

		//�������
		int const static nSpaceTime=20;
		if (GetTickCount()>=(m_dwChangeTimes[wChairID]+nSpaceTime) && enCannonStatus_Shoot==m_enCannonStatus[wChairID])
		{
			//�����ж�
			if (m_nFrameIndex[wChairID]>=CANNON_FRAME_COUNT-1)
			{
				m_enCannonStatus[wChairID]=enCannonStatus_Normal;
				m_nFrameIndex[wChairID]=0;
				m_nFrameIndexBack[wChairID]=0;
			}
			else
			{
				++m_nFrameIndex[wChairID];
			}

			//����ʱ��
			m_dwChangeTimes[wChairID]=GetTickCount()+nSpaceTime;
		}

		//�޸ı���
		if (true==m_bCountScaling && GetTickCount()>=(m_dwScalingTimes+nSpaceTime))
		{
			//����ʱ��
			m_dwScalingTimes=GetTickCount()+50;

			//�޸ı���
			m_fCountScaling+=m_fScalingSpace;

			//�޸ķ���
			if (m_fCountScaling>=1.6f) m_fScalingSpace=-0.1f;

			//�����ж�
			if (m_fCountScaling<=1.0f) m_bCountScaling=false;
		}
	}
}

//����λ��
void CCannon::SetBenchmarkPos(WORD wChairID, int nPosX, int nPosY)
{
	//�Ϸ��ж�
	ASSERT(wChairID<GAME_PLAYER);
	if (!(wChairID<GAME_PLAYER)) return;

	//��������
	m_ptBenchmarkPos[wChairID].x=nPosX;
	m_ptBenchmarkPos[wChairID].y=nPosY;
}

//���ýǶ�
void CCannon::SetAngleCan(WORD wChairID, float fAngle)
{
	//�Ϸ��ж�
	ASSERT(wChairID<GAME_PLAYER);
	if (!(wChairID<GAME_PLAYER)) return;

	//���ñ���
	m_fAngleCan[wChairID]=fAngle;

	//���Ʒ�Χ
	if (IsSameSide(m_wMeChairID,wChairID))
	{
		if (m_fAngleCan[wChairID]>=D3DX_PI/2 && m_fAngleCan[wChairID]<=D3DX_PI) m_fAngleCan[wChairID]=ME_MAX_CAN_ANGLE;
		if (m_fAngleCan[wChairID]>=D3DX_PI && m_fAngleCan[wChairID]<3*D3DX_PI/2) m_fAngleCan[wChairID]=ME_MIN_CAN_ANGLE;
	}
	else
	{
		if (m_fAngleCan[wChairID]>=4*D3DX_PI/2 && m_fAngleCan[wChairID]<=5*D3DX_PI/2) m_fAngleCan[wChairID]=UP_MIN_CAN_ANGLE;
		if (m_fAngleCan[wChairID]>3*D3DX_PI/2 && m_fAngleCan[wChairID]<4*D3DX_PI/2) m_fAngleCan[wChairID]=UP_MAX_CAN_ANGLE;
	}
}

//�����ڵ�
void CCannon::Shoot(WORD wChairID)
{
	//�Ϸ��ж�
	ASSERT(wChairID<GAME_PLAYER);
	if (!(wChairID<GAME_PLAYER)) return;

	//����״̬
	if (enCannonStatus_Normal==m_enCannonStatus[wChairID])
	{
		m_enCannonStatus[wChairID]=enCannonStatus_Shoot;
		m_nFrameIndex[wChairID]=0;
	}

	//��������
	m_nFrameIndexBack[wChairID]=1;

	//����ʱ��
	m_dwChangeTimes[wChairID]=GetTickCount();
}

//����λ��
void CCannon::SetNumberPos(WORD wChairID, int nPosX, int nPosY)
{
	//�Ϸ��ж�
	ASSERT(wChairID<GAME_PLAYER);
	if (!(wChairID<GAME_PLAYER)) return;

	//���ñ���
	m_ptNumberPos[wChairID].x=nPosX;
	m_ptNumberPos[wChairID].y=nPosY;
}

//�ӵ���Ŀ
void CCannon::SetBulletCount(WORD wChairID, int nBulletCount, bool bScale) 
{
	//���ñ���
	m_wBulletCount[wChairID]=nBulletCount;
	m_dwScalingTimes=GetTickCount();

	if (true==bScale)
	{
		m_bCountScaling=true;
		m_fScalingSpace=0.1f;
		m_fCountScaling=1.0f;
	}
}