#include "StdAfx.h"
#include "Coin.h"
#include "Mathaide.h"

//���캯��
CCoin::CCoin(void)
{
}

//��������
CCoin::~CCoin(void)
{
}

//�滭���
void CCoin::DrawCoin()
{
	//�Ϸ��ж�
	if (m_Sprite==NULL||m_pTexture==NULL) return ;
	if (0==m_CoinInfoFactory.GetActiveCount()) return;

	//�滭���
	INT_PTR nEnumIndex=0;
	tagCoinInfo *pCoinInfo=NULL;
	do
	{
		//ö�ٽ��
		pCoinInfo=m_CoinInfoFactory.EnumActiveObject(nEnumIndex);
		if (NULL==pCoinInfo) break;

		//�滭����
		DrawNumber(pCoinInfo->ptNumber.x,pCoinInfo->ptNumber.y,pCoinInfo->lCoinCount,pCoinInfo->byAlphaBlend);

		//״̬�ж�
		bool bMoveNext=true;

		//��ǰλ��
		CRect drawRect;

		////�滭����
		//drawRect.left=0 * GetWidth(TEXTURE_INDEX_COIN_BK);
		//drawRect.top=2*GetHeight(TEXTURE_INDEX_COIN_BK);
		//drawRect.right=drawRect.left+GetWidth(TEXTURE_INDEX_COIN_BK);
		//drawRect.bottom=drawRect.top+GetHeight(TEXTURE_INDEX_COIN_BK);
		//DrawRotateAndZoom(100,100,drawRect,0.f,3,3,TEXTURE_INDEX_COIN_BK);

		//�滭���
		drawRect.left=pCoinInfo->byFrameIndex * GetWidth(pCoinInfo->byTextureIndex);
		drawRect.top=0;
		drawRect.right=drawRect.left+GetWidth(pCoinInfo->byTextureIndex);
		drawRect.bottom=drawRect.top+GetHeight(pCoinInfo->byTextureIndex);

		//�滭ͼƬ
		DrawRotateAndZoom(pCoinInfo->ptTrace[pCoinInfo->wTracePtIndex].x,pCoinInfo->ptTrace[pCoinInfo->wTracePtIndex].y,drawRect,0.f,pCoinInfo->fScaling,pCoinInfo->fScaling,pCoinInfo->byTextureIndex);

		//����ж�
		if (pCoinInfo->wTracePtIndex>=pCoinInfo->wTracePtCount-1) 
		{
			//������Դ
			m_CoinInfoFactory.FreeItem(pCoinInfo);

			//���ñ�ʶ
			bMoveNext=false;
		}

		//�ƶ�֡
		if(GetTickCount()>=(pCoinInfo->dwMoveTimes+pCoinInfo->bySpaceTimes))
		{
			//����ʱ��
			pCoinInfo->dwMoveTimes=GetTickCount();

			//�ƶ�֡
			pCoinInfo->byFrameIndex=(pCoinInfo->byFrameIndex+1)%COIN_FRAME_COUNT;

			//�ƶ�λ��
			pCoinInfo->wTracePtIndex+=2;
			if (pCoinInfo->wTracePtIndex>=pCoinInfo->wTracePtCount-1) pCoinInfo->wTracePtIndex=pCoinInfo->wTracePtCount-1;

			//�޸�͸��
			if (pCoinInfo->byAlphaBlend>=5) pCoinInfo->byAlphaBlend-=5;

			//�޸ı���
			if (pCoinInfo->fScaling<1.2) pCoinInfo->fScaling+=0.01f;
		}

		//�ƶ��ж�
		if (true==bMoveNext) ++nEnumIndex;

	} while (true);

}

//�滭����
void CCoin::DrawNumber(int nXPos, int nYPos, int nNumber, BYTE byAlphaBlend)
{
	if (nNumber>0)
	{
		//��ȡ����
		const INT nNumberHeight=GetHeight(TEXTURE_INDEX_COIN_SHOW);
		const INT nNumberWidth=GetWidth(TEXTURE_INDEX_COIN_SHOW);

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

		//�滭�˺�
		SetAlphaBlend(byAlphaBlend,TEXTURE_INDEX_COIN_SHOW);
		DrawSprite(nXDrawPos,nYDrawPos,drawRect,TEXTURE_INDEX_COIN_SHOW);

		//�滭����
		nXDrawPos+=lNumberCount*(nNumberWidth+nSpace);
		for (LONG i=0;i<lNumberCount;i++)
		{
			//�滭����
			WORD wCellNumber=nNumber%10;
			drawRect.left=(wCellNumber+1)*nNumberWidth;
			drawRect.top=0;
			drawRect.right=drawRect.left+GetWidth(TEXTURE_INDEX_COIN_SHOW);
			drawRect.bottom=drawRect.top+GetHeight(TEXTURE_INDEX_COIN_SHOW);
			DrawSprite(nXDrawPos,nYDrawPos,drawRect,TEXTURE_INDEX_COIN_SHOW);

			//���ñ���
			nNumber/=10;
			nXDrawPos-=(nNumberWidth+nSpace);
		}
	}
}

//���ӽ��
void CCoin::AddCoin(int nSourcePosX, int nSourcePosY, int nTargetPosX, int nTargetPosY, int nNumberX,int nNumberY,LONG lCoinCount, BYTE byTextureIndex)
{
	//��ȡ����
	tagCoinInfo *pCoinInfo=m_CoinInfoFactory.ActiveItem();
	ASSERT(NULL!=pCoinInfo);
	if (NULL==pCoinInfo) return;

	//��ʼ����
	ZeroMemory(pCoinInfo,sizeof(tagCoinInfo));

	//��������
	int nPosX[2]={nSourcePosX,nTargetPosX};
	int nPosY[2]={nSourcePosY,nTargetPosY};
	pCoinInfo->ptNumber.x=nNumberX-40;
	pCoinInfo->ptNumber.y=nNumberY-((nTargetPosY>nSourcePosY)?50:-50);

	//����켣
	CMathAide::BuildLinear(nPosX, nPosY, 2, pCoinInfo->ptTrace, CountArray(pCoinInfo->ptTrace), pCoinInfo->wTracePtCount, TRACE_POINT_SPACE_NORMAL);

	//�Ϸ��ж�
	if (pCoinInfo->wTracePtCount<2) return;

	//������Ϣ
	pCoinInfo->byTextureIndex=byTextureIndex;
	pCoinInfo->lCoinCount=lCoinCount;
	pCoinInfo->bySpaceTimes=20;
	pCoinInfo->dwMoveTimes=GetTickCount();
	pCoinInfo->byAlphaBlend=255;
	pCoinInfo->fScaling=1.f;
}