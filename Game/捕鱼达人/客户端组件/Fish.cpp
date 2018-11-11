#include "StdAfx.h"
#include "Bullet.h"
#include "Fish.h"
#include "Mathaide.h"
//////////////////////////////////////////////////////////////////////////////////////
//��Ⱥ����
CFishGroup						g_FishGroup[enFishKind_No];

//��̬����
bool CFishGroup::m_bWaveComing=false;
//////////////////////////////////////////////////////////////////////////////////////

//���캯��
CFishGroup::CFishGroup()
{
	//�ں˱���
	m_hNotifyWnd=NULL;

	//��Ⱥ��Ϣ
	m_enFishKind=enFishKind_No;
}

//��������
CFishGroup::~CFishGroup()
{
	//�ͷ��ڴ�
	m_FishFactory.FreeExtra();
}

//D X �滭
void CFishGroup::DrawFish()
{
////////////////////////////////////////////////////
//#ifdef _DEBUG
//	//��ǰλ��
//	CRect drawRect2;
//	drawRect2.left=0;
//	drawRect2.top=0;
//	drawRect2.right=GetWidth();
//	drawRect2.bottom=GetHeight();
//	static DWORD dwMoveTimes=GetTickCount();
//	static float fAngle=0.f;
//	//if (GetTickCount()>=dwMoveTimes+10)
//	//{
//	//	dwMoveTimes=GetTickCount();
//	//	fAngle+=0.01f;
//	//}
//
//	//DrawRotateAndZoom(300,250,drawRect2,fAngle,drawRect2.Width()/2+g_nDrawCenterExcsX[m_enFishKind],drawRect2.Height()/2+g_nDrawCenterExcsY[m_enFishKind]);
//
//	drawRect2.left=0;
//	drawRect2.top=0;
//	drawRect2.right=GetWidth(9);
//	drawRect2.bottom=GetHeight(9);
//	DrawRotateAndZoom(33,33,drawRect2,0.f,1.f,1.f,9,enXRight,enYBottom);
//#endif
//	/////////////////////////////////////////////////////
	//�Ϸ��ж�
	if (m_Sprite==NULL||m_pTexture==NULL) return ;
	if (enFishKind_No==m_enFishKind) return;
	if (0==m_FishFactory.GetActiveCount()) return;

	//�滭��Ⱥ
	INT_PTR nEnumIndex=0;
	tagFishInfo *pFish=NULL;
	do
	{
		//ö����Ⱥ
		pFish=m_FishFactory.EnumActiveObject(nEnumIndex);
		if (NULL==pFish) break;

		//�����ж�
		if (false==pFish->bActive)
		{
			++nEnumIndex;

			continue;
		}

		//��ǰλ��
		CRect drawRect;
		drawRect.left=(pFish->byFrameIndex % g_nColumnCount[m_enFishKind]) * GetWidth();
		drawRect.top=(pFish->byFrameIndex / g_nColumnCount[m_enFishKind]) * GetHeight();
		drawRect.right=drawRect.left+GetWidth();
		drawRect.bottom=drawRect.top+GetHeight();

		//�滭ͼƬ
		if (enFishStatus_Die==pFish->FishStatus && enFishKind_12==m_enFishKind)
            DrawRotateAndZoom(pFish->ptTrace[pFish->wTracePtIndex].x,pFish->ptTrace[pFish->wTracePtIndex].y,drawRect,pFish->fAngle,1.f,1.f,1);
		else
			DrawRotateAndZoom(pFish->ptTrace[pFish->wTracePtIndex].x,pFish->ptTrace[pFish->wTracePtIndex].y,drawRect,pFish->fAngle);

		//����ж�
		bool bMoveNext=true;
		if (pFish->wTracePtIndex>=pFish->wTracePtCount-1) 
		{
			//����״̬
			pFish->FishStatus=enFishStatus_No;

			//������Դ
			m_FishFactory.FreeItem(pFish);

			//���ñ�ʶ
			bMoveNext=false;
		}

		//�ƶ�֡
		if(GetTickCount()>=pFish->dwMoveTimes)
		{
			//�ƶ�֡��
			int nFrameCount=g_nAliveFrameCount[m_enFishKind];
			if (enFishStatus_Die==pFish->FishStatus) nFrameCount=g_nTotalFrameCount[m_enFishKind];

			//�ظ�����
			BYTE byRepeatCount=3;
			if (g_nTotalFrameCount[m_enFishKind]-g_nAliveFrameCount[m_enFishKind]>=20) byRepeatCount=1;
			else if (g_nTotalFrameCount[m_enFishKind]-g_nAliveFrameCount[m_enFishKind]<=8) byRepeatCount=4;

			//�����ж�
			if (enFishStatus_Die!=pFish->FishStatus) 
			{
				//��������
				++pFish->wTracePtIndex;

				//�ı�Ƕ�
				if (pFish->fAngle>=pFish->fAngleStep) pFish->fAngle-=pFish->fAngleStep;
			}
			else if (pFish->byFrameIndex==(nFrameCount-1) && enFishStatus_Die==pFish->FishStatus && pFish->byDieCount<byRepeatCount)
			{
				pFish->byFrameIndex=g_nAliveFrameCount[m_enFishKind];
				++pFish->byDieCount;
			}
			else if (pFish->byFrameIndex==(nFrameCount-1) && pFish->byDieCount>=byRepeatCount)
			{
				////֪ͨ��Ϣ
				//if (pFish->wTracePtIndex<pFish->wTracePtCount)
				//	PostMessage(m_hNotifyWnd, WM_FISH_DISAPPEAR, MAKEWPARAM(pFish->byShootUser,m_enFishKind),MAKELPARAM(pFish->ptTrace[pFish->wTracePtIndex].x,pFish->ptTrace[pFish->wTracePtIndex].y));

				//����״̬
				pFish->FishStatus=enFishStatus_No;

				//������Դ
				m_FishFactory.FreeItem(pFish);

				//���ñ�ʶ
				bMoveNext=false;
			}

			//����ʱ��
			pFish->dwMoveTimes=m_bWaveComing?(GetTickCount()):(GetTickCount()+pFish->bySpaceTimes);

			//�ƶ�֡
			pFish->byFrameIndex=(pFish->byFrameIndex+1)%nFrameCount;			
		}

		//�ƶ��ж�
		if (true==bMoveNext) ++nEnumIndex;

	} while (true);

	return ;
}

//��ʼ����
bool CFishGroup::InitTrace(int nInitX[], int nInitY[], int nInitCount, DWORD dwFishID)
{
	//��ȡ����
	tagFishInfo *pFishInfo=m_FishFactory.ActiveItem();
	if (NULL==pFishInfo) return false;

	//��ʼ����
	pFishInfo->wTracePtIndex=0;
	pFishInfo->byFrameIndex=0;
	pFishInfo->FishStatus=enFishStatus_Alive;
	pFishInfo->dwFishID=dwFishID;
	pFishInfo->dwMoveTimes=GetTickCount();

	//���ñ���
	pFishInfo->bySpaceTimes=g_nMoveSpeed[m_enFishKind];

	//����켣
	CMathAide::BuildBezier(nInitX, nInitY, nInitCount, pFishInfo->ptTrace, CountArray(pFishInfo->ptTrace), pFishInfo->wTracePtCount, TRACE_POINT_SPACE_NORMAL);

	//�Ϸ��ж�
	if (pFishInfo->wTracePtCount<2) 
	{
		//�ͷ���Դ
		m_FishFactory.FreeItem(pFishInfo);

		return false;
	}

	//�׸��Ƕ�
	LONG lDistance=CMathAide::CalcDistance(pFishInfo->ptTrace[1].x,pFishInfo->ptTrace[1].y,pFishInfo->ptTrace[0].x,pFishInfo->ptTrace[0].y);
	float fSinValue=(pFishInfo->ptTrace[1].x-pFishInfo->ptTrace[0].x)/(lDistance*1.0f);
	pFishInfo->fAngle=acos(fSinValue);

	//�м�Ƕ�
	lDistance=CMathAide::CalcDistance(pFishInfo->ptTrace[pFishInfo->wTracePtCount/2].x,pFishInfo->ptTrace[pFishInfo->wTracePtCount/2].y,pFishInfo->ptTrace[pFishInfo->wTracePtCount/2-1].x,pFishInfo->ptTrace[pFishInfo->wTracePtCount/2-1].y);
	fSinValue=(pFishInfo->ptTrace[pFishInfo->wTracePtCount/2].x-pFishInfo->ptTrace[pFishInfo->wTracePtCount/2-1].x)/(lDistance*1.0f);
	float fAngleMiddle=acos(fSinValue);

	//����Ƕ�
	pFishInfo->fAngleStep=(pFishInfo->fAngle-fAngleMiddle)/(pFishInfo->wTracePtCount/2);

	//�����Ƕ�
	if (pFishInfo->ptTrace[pFishInfo->wTracePtCount/2-1].y<pFishInfo->ptTrace[0].y) 
	{
		pFishInfo->fAngle=2*D3DX_PI-pFishInfo->fAngle;
		fAngleMiddle=2*D3DX_PI-fAngleMiddle;
		pFishInfo->fAngleStep=(pFishInfo->fAngle-fAngleMiddle)/(pFishInfo->wTracePtCount/2);
	}

	//�����Ƕ�(Ϊ���ڵ����м�Ƕȵ�ʱ����Ҫ�����ж��л��Ƕ�)
	pFishInfo->fAngle+=(2*D3DX_PI);

	return true;
}


//��ʼ����
bool CFishGroup::InitTraceLinear(int nInitX[], int nInitY[], int nInitCount, DWORD dwFishID)
{
	//��ȡ����
	tagFishInfo *pFishInfo=m_FishFactory.ActiveItem();
	if (NULL==pFishInfo) return false;

	//��ʼ����
	pFishInfo->wTracePtIndex=0;
	pFishInfo->byFrameIndex=0;
	pFishInfo->FishStatus=enFishStatus_Alive;
	pFishInfo->dwFishID=dwFishID;
	pFishInfo->dwMoveTimes=GetTickCount();

	//���ñ���
	pFishInfo->bySpaceTimes=g_nMoveSpeed[m_enFishKind];

	///////////////////////////////////////////////////////
	////Ѱ������
	//tagFishInfo *pFishInfoTmp=m_FishFactory.EnumActiveObject(0);
	//if (NULL!=pFishInfoTmp && m_FishFactory.GetActiveCount()>1) 
	//{
	//	CopyMemory(pFishInfo->ptTrace,pFishInfoTmp->ptTrace,sizeof(pFishInfoTmp->ptTrace));
	//	pFishInfo->wTracePtCount=pFishInfoTmp->wTracePtCount;
	//}
	//else
	//////////////////////////////////////////////////////

	//����켣
	CMathAide::BuildBezier(nInitX, nInitY, nInitCount, pFishInfo->ptTrace, CountArray(pFishInfo->ptTrace), pFishInfo->wTracePtCount, TRACE_POINT_SPACE_NORMAL);

	//�Ϸ��ж�
	if (pFishInfo->wTracePtCount<2) 
	{
		//�ͷ���Դ
		m_FishFactory.FreeItem(pFishInfo);

		return false;
	}

	//�׸��Ƕ�
	LONG lDistance=CMathAide::CalcDistance(pFishInfo->ptTrace[pFishInfo->wTracePtCount-1].x,pFishInfo->ptTrace[pFishInfo->wTracePtCount-1].y,pFishInfo->ptTrace[0].x,pFishInfo->ptTrace[0].y);
	float fSinValue=(pFishInfo->ptTrace[pFishInfo->wTracePtCount-1].x-pFishInfo->ptTrace[0].x)/(lDistance*1.0f);
	pFishInfo->fAngle=acos(fSinValue);

	//�����Ƕ�
	if (pFishInfo->ptTrace[pFishInfo->wTracePtCount/2-1].y<pFishInfo->ptTrace[0].y) 
	{
		pFishInfo->fAngle=2*D3DX_PI-pFishInfo->fAngle;		
	}
	pFishInfo->fAngleStep=0.f;

	return true;
}

//����״̬
bool CFishGroup::SetStatus(DWORD dwFishID, enFishStatus FishStatus)
{
	//��ȡ����
	tagFishInfo *pFishInfo=GetFish(dwFishID);
	if (NULL==pFishInfo) return false;

	//���ñ���
	pFishInfo->FishStatus=FishStatus;
	pFishInfo->bySpaceTimes=20;

	//��������
	if (enFishKind_12!=m_enFishKind)
        pFishInfo->byFrameIndex=g_nAliveFrameCount[m_enFishKind];
	else
		pFishInfo->byFrameIndex=0;

	return true;
}

//������Ⱥ
void CFishGroup::ResetKenel()
{
	//�����ڴ�
	m_FishFactory.FreeAllItem();
}

//��ȡ����
tagFishInfo *CFishGroup::GetFish(DWORD dwFishID)
{
	//Ѱ������
	INT_PTR nEnumIndex=0;
	tagFishInfo *pFishInfo=NULL;
	do
	{
		//ö����Ⱥ
		pFishInfo=m_FishFactory.EnumActiveObject(nEnumIndex++);
		if (NULL==pFishInfo) break;
		if (pFishInfo->dwFishID==dwFishID) return pFishInfo;

	} while (true);

	return NULL;
}

//��������
tagFishInfo *CFishGroup::ActiveNewFish()
{
	tagFishInfo *pFishInfo=m_FishFactory.ActiveItem();
	ZeroMemory(pFishInfo,sizeof(*pFishInfo));
	pFishInfo->bActive=false;

	return pFishInfo;
}

//�����ж�
bool CFishGroup::ShootTest(WORD wChairID, int nPosX, int nPosY, enBulletKind BulletKind, bool bCptrMode, DWORD dwBulletID, BYTE byBulletCount, bool bNetStatus)
{
	//�Ϸ��ж�
	ASSERT(byBulletCount<=5);
	if (!(byBulletCount<=5)) return false;

	//Ѱ������
	INT_PTR nEnumIndex=0;
	tagFishInfo *pFishInfo=NULL;
	CRect rcBound;
	POINT ptShoot;
	ptShoot.x=nPosX;
	ptShoot.y=nPosY;
	bool bShoot=false;
	CRect rcAnd;
	do
	{
		//ö����Ⱥ
		pFishInfo=m_FishFactory.EnumActiveObject(nEnumIndex++);

		//�Ϸ��ж�
		if (NULL==pFishInfo) break;
		if (enFishStatus_Alive!=pFishInfo->FishStatus && true==bCptrMode) continue;
		if (pFishInfo->wTracePtIndex>=pFishInfo->wTracePtCount) continue;

		//���÷�Χ
		int const nBound=g_nShootRoundH[m_enFishKind]/2;
		int nMoveSpace=m_enFishKind>=enFishKind_11?TRACE_POINT_SPACE_BIG:TRACE_POINT_SPACE_NORMAL;
		int nStepCount=g_nShootRoundW[m_enFishKind]/2/nMoveSpace;
		int nBeginIndex=max(0,(int(pFishInfo->wTracePtIndex)-nStepCount));
		int nEndIndex=min(pFishInfo->wTracePtCount,(pFishInfo->wTracePtIndex+nStepCount));

		//�����ж�
		int nCenterX=0;
		int nCenterY=0;
		for (int nIndex=nBeginIndex; nIndex<nEndIndex; ++nIndex)
		{
			nCenterX=pFishInfo->ptTrace[nIndex].x;
			nCenterY=pFishInfo->ptTrace[nIndex].y;
			rcBound.SetRect(nCenterX-nBound,nCenterY-nBound,nCenterX+nBound,nCenterY+nBound);
			if (rcBound.PtInRect(ptShoot)) 
			{
				//���ñ�ʶ
				bShoot=true;

				//�ж��ж�
				break;
			}
		}

		//�ж��ж�
		if (true==bShoot) break;

	} while (true);

	if (true==bCptrMode && (true==bShoot || true==bNetStatus))
	{
		//���ñ���
		nEnumIndex=0;
		CRect rcShoot(nPosX-g_nNetRadiusX[BulletKind],nPosY-g_nNetRadiusY[BulletKind],nPosX+g_nNetRadiusX[BulletKind],nPosY+g_nNetRadiusY[BulletKind]);

		do
		{
			//ö����Ⱥ
			pFishInfo=m_FishFactory.EnumActiveObject(nEnumIndex++);

			//�Ϸ��ж�
			if (NULL==pFishInfo) break;
			if (enFishStatus_Alive!=pFishInfo->FishStatus) continue;
			if (pFishInfo->wTracePtIndex>=pFishInfo->wTracePtCount) continue;

			//�����ж�
			ptShoot.x=pFishInfo->ptTrace[pFishInfo->wTracePtIndex].x;
			ptShoot.y=pFishInfo->ptTrace[pFishInfo->wTracePtIndex].y;
			if (rcShoot.PtInRect(ptShoot)) 
			{
				////���ʲ���
				//BYTE byRandPrecent=rand()%200+1;
				//if (byRandPrecent<=m_byCptrProbability[byBulletCount-1][m_enFishKind])
				//{
				//	//����״̬
				//	SetStatus(pFishInfo->dwFishID,enFishStatus_Die);

				//	//�ӵ���Ŀ
				//	pFishInfo->byBulletCount=byBulletCount;

				//	//������
				//	pFishInfo->byShootUser=BYTE(wChairID);

				//	//������Ϣ
				//	AfxGetMainWnd()->PostMessage(WM_CAPTURE_FISH,pFishInfo->dwFishID,dwBulletID);
				//}

				//������Ϣ
				AfxGetMainWnd()->PostMessage(WM_HIT_FISH,pFishInfo->dwFishID,dwBulletID);
			}

		} while (true);
	}

	return bShoot;
}

//ö������
tagFishInfo *CFishGroup::EnumFish(WORD wIndex)
{
	return m_FishFactory.EnumActiveObject(wIndex);
}

//�ͷ���Ⱥ
void CFishGroup::FreeFish(tagFishInfo *pFishInfo)
{
	if (NULL!=GetFish(pFishInfo->dwFishID)) m_FishFactory.FreeItem(pFishInfo);
}

//��ȡ��Ŀ
WORD CFishGroup::GetFishCount()
{
	//ͳ����Ŀ
	WORD wFishActiveCount=0;
	INT_PTR nEnumIndex=0;
	tagFishInfo *pFishInfo=NULL;
	do
	{ 
		//ö����Ⱥ
		pFishInfo=m_FishFactory.EnumActiveObject(nEnumIndex++);
		if (NULL==pFishInfo) break;
		if (false==pFishInfo->bActive) continue;

		//������Ŀ
		++wFishActiveCount;

	} while (true);

	return wFishActiveCount;
}

//�����ж�
bool CFishGroup::PtInFish(tagFishInfo *pFishInfo, int nCurIndex, int nPosShootX, int nPosShootY)
{
	////�Ϸ��ж�
	//ASSERT(NULL!=pFishInfo && nCurIndex<pFishInfo->wTracePtCount);
	//if (!(NULL!=pFishInfo && nCurIndex<pFishInfo->wTracePtCount)) return false;

	////�����ж�
	//WORD wLen

	return true;
}