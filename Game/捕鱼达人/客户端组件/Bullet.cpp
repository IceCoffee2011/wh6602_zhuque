#include "StdAfx.h"
#include "Bullet.h"
#include "Mathaide.h"

//���캯��
CBullet::CBullet(void)
{
	//���ñ���
	m_hNotifyWnd=NULL;
	m_enBulletKind=enBulletKind_1;
	m_BulletFactory.FreeAllItem();
	m_dwBulletID=0L;
}

//��������
CBullet::~CBullet(void)
{
}

//�����ӵ�
tagBulletInfo *CBullet::ActiveNewBullet()
{
	//��ȡ����
	tagBulletInfo *pBulletInfo=m_BulletFactory.ActiveItem();
	ZeroMemory(pBulletInfo,sizeof(tagBulletInfo));

	return pBulletInfo;
}

//�����ӵ�
DWORD CBullet::Shoot(WORD wChairID, bool bCptrMode, DWORD dwCptrFishID[], WORD wCptrFishCount, int nSourceX, int nSourceXY, float fAngle, enBulletCountKind BulletCountKind, BYTE byBulletCount)
{
	//��Ŀ�ж�
	if (false==bCptrMode && m_BulletFactory.GetActiveCount()>=50) return 0L;

	//�Ϸ��ж�
	ASSERT(byBulletCount<=5);
	if (!(byBulletCount<=5)) return 0L;

	//�����ӵ�
	tagBulletInfo *pBulletInfo=ActiveNewBullet();
	ASSERT(NULL!=pBulletInfo);
	if (NULL==pBulletInfo) return 0L;

	//��������
	int nPosX[2], nPosY[2];
	nPosX[0]=nSourceX;
	nPosY[0]=nSourceXY;

	//���������
	int const nMaxLen=1200;
	float fTmpValue=0.f;
	fTmpValue=nPosX[0]+sin(fAngle)*nMaxLen;
	nPosX[1]=LONG(fTmpValue);
	fTmpValue=nPosY[0]-cos(fAngle)*nMaxLen;
	nPosY[1]=LONG(fTmpValue);

	//����ֱ��
	bool bAccelerate=enBulletCountKind_800==BulletCountKind?true:false;
	CMathAide::BuildLinear(nPosX, nPosY, 2, pBulletInfo->ptTrace, CountArray(pBulletInfo->ptTrace), pBulletInfo->wTracePtCount, bAccelerate?8:7, m_rcBound);

	//�Ϸ��ж�
	int const static nExcursion=6;
	if (pBulletInfo->wTracePtCount<(nExcursion+2)) 
	{
		//�ͷ���Դ
		m_BulletFactory.FreeItem(pBulletInfo);

		return 0L;
	}

	//�޳���ʼ�㵽�ڿڵĵ�
	pBulletInfo->wTracePtCount-=nExcursion;
	CopyMemory(pBulletInfo->ptTrace,pBulletInfo->ptTrace+nExcursion,pBulletInfo->wTracePtCount*sizeof(pBulletInfo->ptTrace[0]));

	//�׸��Ƕ�
	LONG lDistance=CMathAide::CalcDistance(pBulletInfo->ptTrace[pBulletInfo->wTracePtCount-1].x,pBulletInfo->ptTrace[pBulletInfo->wTracePtCount-1].y,pBulletInfo->ptTrace[0].x,pBulletInfo->ptTrace[0].y);
	float fSinValue=(pBulletInfo->ptTrace[pBulletInfo->wTracePtCount-1].x-pBulletInfo->ptTrace[0].x)/(lDistance*1.0f);
	pBulletInfo->fAngle=acos(fSinValue);

	//�����Ƕ�
	if (nPosY[1]<=nPosY[0]) 
	{
		pBulletInfo->fAngle=2*D3DX_PI-pBulletInfo->fAngle;
	}

	//���ñ���
	pBulletInfo->bCptrMode=bCptrMode;
	pBulletInfo->BulletStatus=enBulletStatus_Shoot;
	CopyMemory(pBulletInfo->dwCptrFishID,dwCptrFishID,min(wCptrFishCount,CountArray(pBulletInfo->dwCptrFishID))*sizeof(DWORD));
	pBulletInfo->dwMoveTimes=GetTickCount();
	pBulletInfo->bySpaceTimes=enBulletCountKind_800==BulletCountKind?BULLET_SPEED_ACCELERATE:BULLET_SPEED_NORMAL;
	pBulletInfo->byFrameIndex=0;
	pBulletInfo->wTracePtIndex=0;
	pBulletInfo->dwBulletID=bCptrMode?(++m_dwBulletID):0L;
	pBulletInfo->byBulletCount=byBulletCount;
	//pBulletInfo->byAddedFrame=(byBulletCount==5)?0:5;
	pBulletInfo->byShootUser=BYTE(wChairID);
	pBulletInfo->byTextureIndex=BulletCountKind-enBulletCountKind_100;

	//������ʶ
	if (true==bCptrMode && 0L==pBulletInfo->dwBulletID)
	{
		m_dwBulletID=1;
		pBulletInfo->dwBulletID=1;
	}

	return bCptrMode?m_dwBulletID:0L;
}

//�滭�ӵ�
void CBullet::DrawBullet()
{
	//�Ϸ��ж�
	if (m_Sprite==NULL||m_pTexture==NULL) return ;
	if (enBulletKind_No==m_enBulletKind) return;
	if (0==m_BulletFactory.GetActiveCount()) return;

	//�滭�ӵ�
	INT_PTR nEnumIndex=0;
	tagBulletInfo *pBulletInfo=NULL;
	do
	{
		//ö���ӵ�
		pBulletInfo=m_BulletFactory.EnumActiveObject(nEnumIndex);
		if (NULL==pBulletInfo) break;

		//״̬�ж�
		if (pBulletInfo->BulletStatus==enBulletStatus_No)
		{
			m_BulletFactory.FreeItem(pBulletInfo);
			++nEnumIndex;
			continue;
		}

		//״̬�ж�
		bool bShowNet=false;
		if (enBulletStatus_Shoot==pBulletInfo->BulletStatus)
		{
			//��ǰλ��
			CRect drawRect;
			drawRect.left=pBulletInfo->byFrameIndex * GetWidth(pBulletInfo->byTextureIndex);
			drawRect.top=0;
			drawRect.right=drawRect.left+GetWidth(pBulletInfo->byTextureIndex);
			drawRect.bottom=drawRect.top+GetHeight(pBulletInfo->byTextureIndex);

			//�滭ͼƬ
			DrawRotateAndZoom(pBulletInfo->ptTrace[pBulletInfo->wTracePtIndex].x,pBulletInfo->ptTrace[pBulletInfo->wTracePtIndex].y,drawRect,pBulletInfo->fAngle,
				1.f,1.f,pBulletInfo->byTextureIndex);

			//�л���ʶ
			bool bSwitchStatus=false;

			//�����ж�
			if (pBulletInfo->wTracePtIndex<(pBulletInfo->wTracePtCount-1))
			{
				for (enFishKind FishKind=enFishKind_1; FishKind<enFishKind_No; FishKind=enFishKind(FishKind+1))
				{
					if (true==g_FishGroup[FishKind].ShootTest(pBulletInfo->byShootUser, pBulletInfo->ptTrace[pBulletInfo->wTracePtIndex].x,pBulletInfo->ptTrace[pBulletInfo->wTracePtIndex].y, m_enBulletKind, pBulletInfo->bCptrMode, pBulletInfo->dwBulletID, pBulletInfo->byBulletCount, bSwitchStatus))
					{
						bSwitchStatus=true;
						break;
					}					
				}
				if (true==bSwitchStatus && true==pBulletInfo->bCptrMode)
				{
					for (enFishKind FishKind=enFishKind_1; FishKind<enFishKind_No; FishKind=enFishKind(FishKind+1))
						g_FishGroup[FishKind].ShootTest(pBulletInfo->byShootUser, pBulletInfo->ptTrace[pBulletInfo->wTracePtIndex].x,pBulletInfo->ptTrace[pBulletInfo->wTracePtIndex].y, m_enBulletKind, pBulletInfo->bCptrMode, pBulletInfo->dwBulletID, pBulletInfo->byBulletCount, bSwitchStatus);
				}
			}
			else if (pBulletInfo->wTracePtIndex>=(pBulletInfo->wTracePtCount-1))
			{
				bSwitchStatus=true;
			}

			if (true==bSwitchStatus)
			{
				//����״̬
				pBulletInfo->BulletStatus=enBulletStatus_Net;

				//���ñ���
				pBulletInfo->byFrameIndex=0;
				pBulletInfo->bySpaceTimes=30;

				//Ͷ����Ϣ
				::PostMessage(m_hNotifyWnd,WM_NET_OPEN,0,0);

				//���ñ���
				bShowNet=true;
			}

			//�ƶ�֡
			if(false==bShowNet && GetTickCount()>=pBulletInfo->dwMoveTimes)
			{
				//�ƶ�֡��
				int nSpaceFrame=pBulletInfo->wTracePtCount/BULLET_FRAME_COUNT;
				if (nSpaceFrame>0) pBulletInfo->byFrameIndex=pBulletInfo->wTracePtIndex/nSpaceFrame;
				if (pBulletInfo->byFrameIndex>=BULLET_FRAME_COUNT) pBulletInfo->byFrameIndex=BULLET_FRAME_COUNT-1;

				//����ʱ��
				pBulletInfo->dwMoveTimes=GetTickCount()+pBulletInfo->bySpaceTimes;

				//�ƶ�λ��
				++pBulletInfo->wTracePtIndex;
			}
		}
		else
		{
			//���ñ���
			bShowNet=true;
		}
		
		if (true==bShowNet)
		{
			//��ǰλ��
			CRect drawRect;
			drawRect.left=(pBulletInfo->byFrameIndex % g_nNetColumnCount[m_enBulletKind]) * GetWidth(TEXTURE_INDEX_NET);
			drawRect.top=(pBulletInfo->byFrameIndex / g_nNetColumnCount[m_enBulletKind]) * GetWidth(TEXTURE_INDEX_NET);
			drawRect.right=drawRect.left+GetWidth(TEXTURE_INDEX_NET);
			drawRect.bottom=drawRect.top+GetHeight(TEXTURE_INDEX_NET);

			//�滭ͼƬ
			static const float fScaling[]={0.8f,0.9f,1.0f,1.1f,1.2f};
			static const D3DCOLOR clrBullet[]={D3DCOLOR_XRGB(255, 255, 0),D3DCOLOR_XRGB(0, 255, 0),D3DCOLOR_XRGB(0, 0, 255),D3DCOLOR_XRGB(255, 0, 255),D3DCOLOR_XRGB(255, 0, 0)};
			DrawRotateAndZoom(pBulletInfo->ptTrace[pBulletInfo->wTracePtIndex].x,pBulletInfo->ptTrace[pBulletInfo->wTracePtIndex].y,drawRect,pBulletInfo->fAngle+D3DX_PI/2,
				fScaling[pBulletInfo->byBulletCount-1],fScaling[pBulletInfo->byBulletCount-1],TEXTURE_INDEX_NET,enXCenter, enYCenter,clrBullet[pBulletInfo->byBulletCount-1]);

			//�����ж�
			//if (pBulletInfo->byFrameIndex>=(g_nNetFrameCount[m_enBulletKind][pBulletInfo->byBulletCount-1]-1) && 0==pBulletInfo->byAddedFrame)
			if (pBulletInfo->byFrameIndex>=(g_nNetFrameCount[m_enBulletKind][pBulletInfo->byBulletCount-1]-1))
			{
				//����״̬
				pBulletInfo->BulletStatus=enBulletStatus_No;

				//�ͷ���Դ
				m_BulletFactory.FreeItem(pBulletInfo);

				continue;
			}

			//�ƶ�֡
			if(GetTickCount()>=pBulletInfo->dwMoveTimes)
			{
				//�ƶ�֡��
				if (pBulletInfo->byFrameIndex<(g_nNetFrameCount[m_enBulletKind][pBulletInfo->byBulletCount-1]-1))
					pBulletInfo->byFrameIndex=(pBulletInfo->byFrameIndex+1)%g_nNetFrameCount[m_enBulletKind][pBulletInfo->byBulletCount-1];
				//else if (pBulletInfo->byAddedFrame>0)
					//pBulletInfo->byAddedFrame-=1;

				//����ʱ��
				pBulletInfo->dwMoveTimes=GetTickCount()+pBulletInfo->bySpaceTimes;
			}
		}

		//�����ж�
		++nEnumIndex;

	} while (true);
}