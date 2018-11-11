#include "StdAfx.h"
#include "Bonus.h"
#include "Mathaide.h"

//���캯��
CBonus::CBonus(void)
{
	m_dwDrawTime=GetTickCount();
	m_dwStartTime=GetTickCount();
}

//��������
CBonus::~CBonus(void)
{
}

//�滭��Ʒ
void CBonus::DrawBonus()
{
	//�Ϸ��ж�
	if (m_Sprite==NULL||m_pTexture==NULL) return ;
	if (0==m_BonusInfoFactory.GetActiveCount()) return;

	//�滭��Ʒ
	INT_PTR nEnumIndex=0;
	tagBonusInfo *pBonusInfo=NULL;
	do
	{
		//ö�ٽ�Ʒ
		pBonusInfo=m_BonusInfoFactory.EnumActiveObject(nEnumIndex);
		if (NULL==pBonusInfo) break;

		//״̬�ж�
		bool bMoveNext=true;

		//��ǰλ��
		CRect drawRect;

		//�滭����
		drawRect.left=0;
		drawRect.top=0;
		drawRect.right=drawRect.left+GetWidth(TEXTURE_INDEX_BONUS_BACK);
		drawRect.bottom=drawRect.top+GetHeight(TEXTURE_INDEX_BONUS_BACK);
		SetAlphaBlend(pBonusInfo->byBackAlpha,TEXTURE_INDEX_BONUS_BACK);
		DrawSprite(pBonusInfo->nBenchmarkPosX,pBonusInfo->nBenchmarkPosY,drawRect,TEXTURE_INDEX_BONUS_BACK);

		//��ʾ���
		if (255==pBonusInfo->byBackAlpha)
		{
			drawRect.left=0;
			drawRect.top=0;
			drawRect.right=drawRect.left+GetWidth(TEXTURE_INDEX_BONUS_LIGHT);
			drawRect.bottom=drawRect.top+GetHeight(TEXTURE_INDEX_BONUS_LIGHT);
			DrawSprite(pBonusInfo->nBenchmarkPosX+25,pBonusInfo->nBenchmarkPosY,drawRect,TEXTURE_INDEX_BONUS_LIGHT);

			drawRect.left=180;
			drawRect.top=134;
			drawRect.right=drawRect.left+GetWidth(TEXTURE_INDEX_BONUS_STAR);
			drawRect.bottom=drawRect.top+GetHeight(TEXTURE_INDEX_BONUS_STAR);
			int nExcursionX[6]={165,160,150,140,130};
			int nExcursionY[6]={120,120,115,110,100};
			BYTE byScalingIndex=pBonusInfo->byScalingIndex;
			FLOAT fScaling=1.4f-byScalingIndex*0.1f;
			DrawRotateAndZoom(pBonusInfo->nBenchmarkPosX+nExcursionX[byScalingIndex],pBonusInfo->nBenchmarkPosY+nExcursionY[byScalingIndex],drawRect,0.f,58,46,fScaling,fScaling,TEXTURE_INDEX_BONUS_STAR);

			int nWidth=180,nHeight=134;
			drawRect.left=(pBonusInfo->byStarIndex%2)*nWidth;
			drawRect.top=(pBonusInfo->byStarIndex/2)*nHeight;
			drawRect.right=drawRect.left+nWidth;
			drawRect.bottom=drawRect.top+nHeight;
			DrawSprite(pBonusInfo->nBenchmarkPosX+35,pBonusInfo->nBenchmarkPosY+10,drawRect,TEXTURE_INDEX_BONUS_STAR);
		}

		//��ʾ��Ŀ
		if (4==pBonusInfo->byScalingIndex)
		{
			drawRect.left=0;
			drawRect.top=0;
			drawRect.right=drawRect.left+GetWidth(TEXTURE_INDEX_BONUS_DETAIL);
			drawRect.bottom=drawRect.top+35;
			DrawSprite(pBonusInfo->nBenchmarkPosX+13,pBonusInfo->nBenchmarkPosY+130,drawRect,TEXTURE_INDEX_BONUS_DETAIL);

			if (pBonusInfo->lBonusCount>999)
                DrawNumber(pBonusInfo->nBenchmarkPosX+63,pBonusInfo->nBenchmarkPosY+135,pBonusInfo->lBonusCount,255);
			else if (pBonusInfo->lBonusCount>99)
				DrawNumber(pBonusInfo->nBenchmarkPosX+73,pBonusInfo->nBenchmarkPosY+135,pBonusInfo->lBonusCount,255);
			else
				DrawNumber(pBonusInfo->nBenchmarkPosX+80,pBonusInfo->nBenchmarkPosY+135,pBonusInfo->lBonusCount,255);
		}

		//����ж�
		if (0==pBonusInfo->byShowTimeCount) 
		{
			//������Դ
			m_BonusInfoFactory.FreeItem(pBonusInfo);

			//���ñ�ʶ
			bMoveNext=false;
		}

		//�ƶ�֡
		if(GetTickCount()>=(m_dwDrawTime+20))
		{
			//����ʱ��
			m_dwDrawTime=GetTickCount();

			//�޸�͸��
			if (pBonusInfo->byBackAlpha<255) pBonusInfo->byBackAlpha+=5;
			else if (pBonusInfo->byScalingIndex<4) pBonusInfo->byScalingIndex+=1;
			else if (pBonusInfo->byShowTimeCount>0) pBonusInfo->byShowTimeCount-=1;
		}

		if(GetTickCount()>=(m_dwStartTime+200))
		{
			//����ʱ��
			m_dwStartTime=GetTickCount();

			//�޸�����
			pBonusInfo->byStarIndex=(pBonusInfo->byStarIndex+1)%3;
		}		

		//�ƶ��ж�
		if (true==bMoveNext) ++nEnumIndex;

	} while (true);

}

//�滭����
void CBonus::DrawNumber(int nXPos, int nYPos, int nNumber, BYTE byAlphaBlend)
{
	if (nNumber>0)
	{
		//��ȡ����
		const INT nNumberHeight=GetHeight(TEXTURE_INDEX_BONUS_NUMBER);
		const INT nNumberWidth=GetWidth(TEXTURE_INDEX_BONUS_NUMBER);

		//������Ŀ
		LONG lNumberCount=0;
		WORD wNumberTemp=nNumber;
		do
		{
			lNumberCount++;
			wNumberTemp/=10;
		} while (wNumberTemp>0L);

		//λ�ö���
		int const nSpace=0;
		INT nYDrawPos=nYPos;
		INT nXDrawPos=nXPos;

		//��ǰλ��
		CRect drawRect;
		drawRect.left=0;
		drawRect.top=0;
		drawRect.right=drawRect.left+GetWidth();
		drawRect.bottom=drawRect.top+GetHeight();

		////�滭�˺�
		//SetAlphaBlend(byAlphaBlend,TEXTURE_INDEX_BONUS_NUMBER);
		//DrawSprite(nXDrawPos,nYDrawPos,drawRect,TEXTURE_INDEX_BONUS_NUMBER);

		//�滭����
		nXDrawPos+=lNumberCount*(nNumberWidth+nSpace);
		for (LONG i=0;i<lNumberCount;i++)
		{
			//�滭����
			WORD wCellNumber=nNumber%10;
			drawRect.left=(wCellNumber)*nNumberWidth;
			drawRect.top=0;
			drawRect.right=drawRect.left+GetWidth(TEXTURE_INDEX_BONUS_NUMBER);
			drawRect.bottom=drawRect.top+GetHeight(TEXTURE_INDEX_BONUS_NUMBER);
			DrawSprite(nXDrawPos,nYDrawPos,drawRect,TEXTURE_INDEX_BONUS_NUMBER);

			//���ñ���
			nNumber/=10;
			nXDrawPos-=(nNumberWidth+nSpace);
		}
	}
}

//���ӽ�Ʒ
void CBonus::AddBonus(LONG lBonusCount, INT nBenchmarkPosX, INT nBenchmarkPosY)
{
	//��ȡ����
	tagBonusInfo *pBonusInfo=m_BonusInfoFactory.ActiveItem();
	ASSERT(NULL!=pBonusInfo);
	if (NULL==pBonusInfo) return;

	//��ʼ����
	ZeroMemory(pBonusInfo,sizeof(pBonusInfo));
	pBonusInfo->lBonusCount=lBonusCount;
	pBonusInfo->byBackAlpha=0;
	pBonusInfo->byScalingIndex=0;
	pBonusInfo->byShowTimeCount=80;
	pBonusInfo->nBenchmarkPosX=nBenchmarkPosX;
	pBonusInfo->nBenchmarkPosY=nBenchmarkPosY;
}