#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//�˿�����
BYTE const CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
};

//�˿�����
BYTE const m_cbCardDataArray[27]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,				    	//����
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,					    //ͬ��
};
//
//W: 0�� 1�� 2�� 3�� 4�� 5�� 6�� 7�� 8
//S: 9, 10, 11, 12, 13, 14, 15, 16, 17
//T: 18, 19, 20, 21, 22, 23, 24, 25, 26
//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
}

//��������
CGameLogic::~CGameLogic()
{
}

//�����˿�
void CGameLogic::RandCardData(BYTE cbCardData[], BYTE cbMaxCount)
{
	//����׼��
	BYTE cbCardDataTemp[CountArray(m_cbCardDataArray)];
	CopyMemory(cbCardDataTemp,m_cbCardDataArray,sizeof(m_cbCardDataArray));

	//����Ŀ¼
/*	TCHAR m_szWorkDirectory[MAX_PATH];
	GetModuleFileName(AfxGetInstanceHandle(),m_szWorkDirectory,sizeof(m_szWorkDirectory));
	int nModuleLen=lstrlen(m_szWorkDirectory);
	int nProcessLen=lstrlen(AfxGetApp()->m_pszExeName)+lstrlen(TEXT(".EXE"));
	m_szWorkDirectory[nModuleLen-nProcessLen]=0;
	SetCurrentDirectory(m_szWorkDirectory);

	//��ȡ�˿�
	TCHAR szFile[MAX_PATH]=TEXT(""),szItem[20];
	_sntprintf(szFile,CountArray(szFile),TEXT("%s\\CardInfo.INI"),m_szWorkDirectory);

	for (BYTE i=0;i<CountArray(m_cbCardDataArray);i++)
	{
		TCHAR szRead[64]=TEXT("");
		_sntprintf(szItem,CountArray(szItem),TEXT("Item%d"),i+1);
		GetPrivateProfileString(TEXT("CardData"),szItem,TEXT(""),szRead,sizeof(szRead),szFile);
		cbCardData[i]=atoi(szRead);
		if (cbCardData[i]==0) break;
	}
	if (i==CountArray(m_cbCardDataArray)) return;*/

	//�����˿�
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbMaxCount-cbRandCount);
		cbCardData[cbRandCount++]=cbCardDataTemp[cbPosition];
		cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
	} while (cbRandCount<cbMaxCount);

	return;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard)
{
	//Ч���˿�
	ASSERT(IsValidCard(cbRemoveCard));
	ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard)]>0);

	//ɾ���˿�
	BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard);
	if (cbCardIndex[cbRemoveIndex]>0)
	{
		cbCardIndex[cbRemoveIndex]--;
		return true;
	}

	//ʧ��Ч��
	ASSERT(FALSE);

	return false;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//ɾ���˿�
	for (BYTE i=0;i<cbRemoveCount;i++)
	{
		//Ч���˿�
		ASSERT(IsValidCard(cbRemoveCard[i]));
		ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard[i])]>0);

		//ɾ���˿�
		BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard[i]);
		if (cbCardIndex[cbRemoveIndex]==0)
		{
			//�������
			ASSERT(FALSE);

			//��ԭɾ��
			for (BYTE j=0;j<i;j++) 
			{
				ASSERT(IsValidCard(cbRemoveCard[j]));
				cbCardIndex[SwitchToCardIndex(cbRemoveCard[j])]++;
			}

			return false;
		}
		else 
		{
			//ɾ���˿�
			--cbCardIndex[cbRemoveIndex];
		}
	}

	return true;
}

//ɾ���˿�
bool CGameLogic::RemoveCard(BYTE cbCardData[], BYTE cbCardCount, BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//��������
	ASSERT(cbCardCount<=14);
	ASSERT(cbRemoveCount<=cbCardCount);

	//�������
	BYTE cbDeleteCount=0,cbTempCardData[14];
	if (cbCardCount>CountArray(cbTempCardData)) return false;
	CopyMemory(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

	//�����˿�
	for (BYTE i=0;i<cbRemoveCount;i++)
	{
		for (BYTE j=0;j<cbCardCount;j++)
		{
			if (cbRemoveCard[i]==cbTempCardData[j])
			{
				cbDeleteCount++;
				cbTempCardData[j]=0;
				break;
			}
		}
	}

	//�ɹ��ж�
	if (cbDeleteCount!=cbRemoveCount) 
	{
		ASSERT(FALSE);
		return false;
	}

	//�����˿�
	BYTE cbCardPos=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbTempCardData[i]!=0) cbCardData[cbCardPos++]=cbTempCardData[i];
	}

	return true;
}

//��Ч�ж�
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return (((cbValue>=1)&&(cbValue<=9)&&(cbColor<=2))||((cbValue>=1)&&(cbValue<=7)&&(cbColor==3)));
}

//�˿���Ŀ
BYTE CGameLogic::GetCardCount(BYTE cbCardIndex[MAX_INDEX])
{
	//��Ŀͳ��
	BYTE cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) cbCardCount+=cbCardIndex[i];

	return cbCardCount;
}

//��ȡ���
BYTE CGameLogic::GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4])
{
	//����˿�
	switch (cbWeaveKind)
	{
	case WIK_LEFT:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard+1;
			cbCardBuffer[1]=cbCenterCard+2;
			cbCardBuffer[2]=cbCenterCard;

			return 3;
		}
	case WIK_RIGHT:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard-2;
			cbCardBuffer[1]=cbCenterCard-1;
			cbCardBuffer[2]=cbCenterCard;

			return 3;
		}
	case WIK_CENTER:	//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard-1;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard+1;

			return 3;
		}
	case WIK_PENG:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;

			return 3;
		}
	case WIK_FILL:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;
			cbCardBuffer[3]=cbCenterCard;

			return 4;
		}
	case WIK_TING:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;
			cbCardBuffer[3]=cbCenterCard;

			return 4;
		}
	case WIK_GANG:		//���Ʋ���
		{
			//���ñ���
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;
			cbCardBuffer[3]=cbCenterCard;

			return 4;
		}
	default:
		{
			ASSERT(FALSE);
		}
	}

	return 0;
}

//�����ȼ�
BYTE CGameLogic::GetUserActionRank(BYTE cbUserAction)
{
	//���Ƶȼ�
	if (cbUserAction&WIK_CHI_HU) { return 4; }

	//���Ƶȼ�
	if (cbUserAction&(WIK_FILL|WIK_GANG|WIK_TING)) { return 3; }

	//���Ƶȼ�
	if (cbUserAction&WIK_PENG) { return 2; }

	//���Ƶȼ�
	if (cbUserAction&(WIK_RIGHT|WIK_CENTER|WIK_LEFT)) { return 1; }

	return 0;
}

//���Ƶȼ�
BYTE CGameLogic::GetChiHuActionRank(tagChiHuResult & ChiHuResult)
{
	//��������
	BYTE cbChiHuOrder=1;
	WORD wChiHuRight=ChiHuResult.wChiHuRight;
	WORD wChiHuKind=ChiHuResult.wChiHuKind;
	
	WORD byHuKind[3] = {CHK_PENG_PENG, CHK_JIANG_JIANG, CHK_QI_XIAO_DUI};
	for (int i=0; i<3; i++)
	{
		if (wChiHuKind&byHuKind[i])
		{
			cbChiHuOrder++;
		}
	}
	WORD byHuRight[7] = {CHR_DI, CHR_TIAN, CHR_HAI_DI, CHR_QIANG_GANG, CHR_QING_YI_SE, CHR_MEN_QIAN_QING, CHR_QUAN_QIU_REN};
	for (int k=0; k<7; k++)
	{
		if (wChiHuRight&byHuRight[k])
		{
			cbChiHuOrder++;
		}
	}

	return cbChiHuOrder;
}

////�Ժ��ж�(����С��)
//BYTE CGameLogic::EstimateChiHu(BYTE cbCardIndex[MAX_INDEX])
//{
//	//��������
//	BYTE cbActionMask=WIK_NULL;
//
//	//�������
//	if (IsSiXi(cbCardIndex)==true) cbActionMask|=WIK_CHI_HU;
//	else if (IsQueYiSe(cbCardIndex)==true) cbActionMask|=WIK_CHI_HU;
//	else if (IsBanBanHu(cbCardIndex)==true) cbActionMask|=WIK_CHI_HU;
//	else if (IsLiuLiuShun(cbCardIndex)==true) cbActionMask|=WIK_CHI_HU;
//
//	return cbActionMask;
//}

//�����ж�
BYTE CGameLogic::EstimateEatCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, BYTE cbKingCard[])
{
	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));

	//�����ж�
	if (cbCurrentCard>=0x31 || cbCurrentCard == cbKingCard[0]) return WIK_NULL;

	//��������
	BYTE cbExcursion[3]={0,1,2};
	BYTE cbItemKind[3]={WIK_LEFT,WIK_CENTER,WIK_RIGHT};

	//�����齫
	BYTE cbCardIndexTemp[MAX_INDEX]={0};
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(BYTE)*MAX_INDEX);

	cbCardIndexTemp[SwitchToCardIndex(cbKingCard[0])] = 0;

	//�����ж�
	BYTE cbEatKind=0,cbFirstIndex=0;
	BYTE cbCurrentIndex=SwitchToCardIndex(cbCurrentCard);
	for (BYTE i=0;i<CountArray(cbItemKind);i++)
	{
		BYTE cbValueIndex=cbCurrentIndex%9;
		if ((cbValueIndex>=cbExcursion[i])&&((cbValueIndex-cbExcursion[i])<=6))
		{
			//�����ж�
			cbFirstIndex=cbCurrentIndex-cbExcursion[i];			
			if ((cbCurrentIndex!=cbFirstIndex)&&(cbCardIndexTemp[cbFirstIndex]==0))
				continue;
			if ((cbCurrentIndex!=(cbFirstIndex+1))&&(cbCardIndexTemp[cbFirstIndex+1]==0))
				continue;
			if ((cbCurrentIndex!=(cbFirstIndex+2))&&(cbCardIndexTemp[cbFirstIndex+2]==0)) 
				continue;

		    //��������
			cbEatKind|=cbItemKind[i];
		}
	}

	return cbEatKind;
}

//�����ж�
BYTE CGameLogic::EstimatePengCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, BYTE cbKingCard[])
{
	//����Ч��
	ASSERT(IsValidCard(cbCurrentCard));

	//�����ж�
	if (cbCurrentCard == cbKingCard[0]) return WIK_NULL;

	//�����ж�
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]>=2)?WIK_PENG:WIK_NULL;
}


//�����ж�
BYTE CGameLogic::EstimateTingCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, BYTE IsGangData[MAX_INDEX],BYTE &GangCount,BYTE cbKingCard[])
{
	GangCount = 0;
	for (int i=0; i<MAX_INDEX; i++)
	{
		if (4 == cbCardIndex[i])
		{
			BYTE bySaveCardIndex[MAX_INDEX] = {0};
			memset(bySaveCardIndex, 0, sizeof(bySaveCardIndex));
			memcpy(bySaveCardIndex, cbCardIndex, sizeof(BYTE)*MAX_INDEX);

			BYTE byWeaveItemCount = cbItemCount;
			tagWeaveItem weaveItemTemp[5];
			memset(weaveItemTemp, 0, sizeof(tagWeaveItem)*5);
			if (byWeaveItemCount>0 && byWeaveItemCount<4)
			{
				memcpy(weaveItemTemp, WeaveItem, sizeof(tagWeaveItem)*byWeaveItemCount);
			}
			weaveItemTemp[byWeaveItemCount].cbCenterCard = SwitchToCardData(i);
			weaveItemTemp[byWeaveItemCount].cbWeaveKind = WIK_FILL;
			weaveItemTemp[byWeaveItemCount].wProvideUser = -1;
			weaveItemTemp[byWeaveItemCount].cbPublicCard = FALSE;
			++byWeaveItemCount;

			bySaveCardIndex[i] = 0;

			bool IsTingPai=IsCanTingCard(bySaveCardIndex,weaveItemTemp,byWeaveItemCount,cbKingCard);
			if (IsTingPai)
			{
				IsGangData[GangCount++] = SwitchToCardData(i);
			}
		}
	}

	for(BYTE i=0;i<cbItemCount;i++)
	{
		BYTE cbCardData=WeaveItem[i].cbCenterCard;
		if(0!=cbCardData && WeaveItem[i].cbWeaveKind==WIK_PENG)
		{
			for (int k=0; k<MAX_INDEX; k++)
			{
				if (cbCardIndex[k]>0)
				{
					BYTE byTempOutCard = SwitchToCardData(k);
					if (byTempOutCard == cbCardData)
					{
						BYTE bySaveCardIndex[MAX_INDEX] = {0};
						memset(bySaveCardIndex, 0, sizeof(bySaveCardIndex));
						memcpy(bySaveCardIndex, cbCardIndex, sizeof(BYTE)*MAX_INDEX);

						BYTE byWeaveItemCount = cbItemCount;
						tagWeaveItem weaveItemTemp[5];
						memset(weaveItemTemp, 0, sizeof(tagWeaveItem)*5);
						if (byWeaveItemCount>0 && byWeaveItemCount<4)
						{
							memcpy(weaveItemTemp, WeaveItem, sizeof(tagWeaveItem)*byWeaveItemCount);
						}
						weaveItemTemp[i].cbCenterCard = cbCardData;
						weaveItemTemp[i].cbWeaveKind = WIK_FILL;
						weaveItemTemp[i].wProvideUser = -1;
						weaveItemTemp[i].cbPublicCard = TRUE;

						bySaveCardIndex[k] = 0;

						bool IsTingPai=IsCanTingCard(bySaveCardIndex,weaveItemTemp,byWeaveItemCount,cbKingCard);
						if (IsTingPai)
						{
							IsGangData[GangCount++] = cbCardData;
						}
						return 0;
					}
				}
			}
		}
	}


	return 0;
}

//�����ж�
bool CGameLogic::EstimateTingCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, BYTE byOutCard,BYTE cbKingCard[])
{
	//����Ч��
	if (!IsValidCard(byOutCard))
	{
		return false;
	}

	for (int i=0; i<MAX_INDEX; i++)
	{
		BYTE byTempOutCard = SwitchToCardData(i);
		if ((0!=byTempOutCard) && (3==cbCardIndex[i]) && (byTempOutCard==byOutCard))
		{
			BYTE bySaveCardIndex[MAX_INDEX] = {0};
			memset(bySaveCardIndex, 0, sizeof(bySaveCardIndex));
			memcpy(bySaveCardIndex, cbCardIndex, sizeof(BYTE)*MAX_INDEX);

			BYTE byWeaveItemCount = cbItemCount;
			tagWeaveItem weaveItemTemp[5];
			memset(weaveItemTemp, 0, sizeof(tagWeaveItem)*5);
			if (byWeaveItemCount>0 && byWeaveItemCount<4)
			{
				memcpy(weaveItemTemp, WeaveItem, sizeof(tagWeaveItem)*byWeaveItemCount);
			}
			weaveItemTemp[byWeaveItemCount].cbCenterCard = byTempOutCard;
			weaveItemTemp[byWeaveItemCount].cbWeaveKind = WIK_FILL;
			weaveItemTemp[byWeaveItemCount].wProvideUser = -1;
			weaveItemTemp[byWeaveItemCount].cbPublicCard = TRUE;
			++byWeaveItemCount;

			bySaveCardIndex[i] = 0;

			bool IsTingPai=IsCanTingCard(bySaveCardIndex,weaveItemTemp,byWeaveItemCount,cbKingCard);
			return IsTingPai;
		}
	}

	return false;
}


//�����ж�
BYTE CGameLogic::EstimateGangCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, BYTE cbKingCard[])
{
	//����Ч��
	if (!IsValidCard(cbCurrentCard))
	{
		return WIK_NULL;
	}

	BYTE byType = WIK_NULL;
	//�����ж�
	if (cbCurrentCard == cbKingCard[0]) return WIK_NULL;

	BYTE byCardIndex = SwitchToCardIndex(cbCurrentCard);
	if (byCardIndex<MAX_INDEX)
	{
		//�����ж�
		if (3 == cbCardIndex[byCardIndex])
		{
			byType |= (WIK_GANG|WIK_FILL);
		}
	}
	return byType;
}

//���Ʒ���
BYTE CGameLogic::AnalyseGangCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult, BYTE cbKingCard[])
{
	//���ñ���
	BYTE cbActionMask=WIK_NULL;
	ZeroMemory(&GangCardResult,sizeof(GangCardResult));

	BYTE bySaveCardIndex[MAX_INDEX] = {0};
	memcpy(bySaveCardIndex, cbCardIndex, sizeof(BYTE)*MAX_INDEX);

	BYTE byKindCardIndex = SwitchToCardIndex(cbKingCard[0]);
	if (byKindCardIndex<MAX_INDEX)
	{//���Ʋ��ܸ�
		bySaveCardIndex[byKindCardIndex] = 0;
	}
	
	//���ϸ���
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (bySaveCardIndex[i]==4)
		{
			cbActionMask|=(WIK_GANG|WIK_FILL);
			GangCardResult.cbCardData[GangCardResult.cbCardCount]=(WIK_GANG|WIK_FILL);
			GangCardResult.cbCardData[GangCardResult.cbCardCount++]=SwitchToCardData(i);
		}
	}

	//��ϸ���
	for (BYTE i=0;i<cbWeaveCount;i++)
	{
		if (WeaveItem[i].cbWeaveKind==WIK_PENG)
		{
			if (bySaveCardIndex[SwitchToCardIndex(WeaveItem[i].cbCenterCard)]==1)
			{
				cbActionMask|=(WIK_GANG|WIK_FILL);
				GangCardResult.cbCardData[GangCardResult.cbCardCount]=(WIK_GANG|WIK_FILL);
				GangCardResult.cbCardData[GangCardResult.cbCardCount++]=WeaveItem[i].cbCenterCard;
			}
		}
	}

	return cbActionMask;
}


//�Ժ�����(������)
BYTE CGameLogic::AnalyseChiHuCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, WORD wChiHuRight, tagChiHuResult & ChiHuResult)
{
	//��������
	WORD wChiHuKind=CHK_NULL;
	static CAnalyseItemArray AnalyseItemArray;

	//���ñ���
	AnalyseItemArray.RemoveAll();
	ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));

	//�����˿�
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//�����˿�
	if (cbCurrentCard!=0) cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;

	//Ȩλ����
	if ((cbCurrentCard!=0)&&(cbWeaveCount==4)) wChiHuRight|=CHR_QUAN_QIU_REN;
	if (IsQingYiSe(cbCardIndexTemp,WeaveItem,cbWeaveCount)==true) wChiHuRight|=CHR_QING_YI_SE;
	if (IsMenQianQing(cbCardIndexTemp,WeaveItem,cbWeaveCount)==true) wChiHuRight|=CHR_MEN_QIAN_QING;

	//�����ж�
	if (IsQiXiaoDui(cbCardIndexTemp,WeaveItem,cbWeaveCount)==true) wChiHuKind|=CHK_QI_XIAO_DUI;
	if (IsJiangJiangHu(cbCardIndexTemp,WeaveItem,cbWeaveCount)==true) wChiHuKind|=CHK_JIANG_JIANG;

	//�����˿�
	AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray);

	//���Ʒ���
	if (AnalyseItemArray.GetCount()>0)
	{
		//��������
		bool bNeedSymbol=((wChiHuRight&0xFF00)==0);
		bool bMenQing=IsMenQianQing(cbCardIndexTemp,WeaveItem,cbWeaveCount);

		//���ͷ���
		for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
		{
			//��������
			bool bLianCard=false,bPengCard=false;
			tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[i];

			//��������
			BYTE cbEyeValue=pAnalyseItem->cbCardEye&MASK_VALUE;
			bool bSymbolEye=((cbEyeValue==2)||(cbEyeValue==5)||(cbEyeValue==8));

			//���ͷ���
			for (BYTE j=0;j<CountArray(pAnalyseItem->cbWeaveKind);j++)
			{
				BYTE cbWeaveKind=pAnalyseItem->cbWeaveKind[j];
				bPengCard=((cbWeaveKind&(WIK_GANG|WIK_FILL|WIK_PENG))!=0)?true:bPengCard;
				bLianCard=((cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT))!=0)?true:bLianCard;
			}

			//�����ж�
			ASSERT((bLianCard==true)||(bPengCard==true));

			//��������
			if ((bLianCard==false)&&(bPengCard==true)) 
			{
				wChiHuKind|=CHK_PENG_PENG;
			}
			if ((bLianCard==true)&&(bPengCard==true)&&((bSymbolEye==true)||((bNeedSymbol==false)&&(bMenQing==false)))) 
			{
				wChiHuKind|=CHK_JI_HU;
			}
			if ((bLianCard==true)&&(bPengCard==false)&&((bSymbolEye==true)||((bNeedSymbol==false)&&(bMenQing==false)))) 
			{
				wChiHuKind|=CHK_PING_HU;
			}
		}
	}

	//����ж�
	if (wChiHuKind!=CHK_NULL)
	{//������.

		
		//�Ѱ��ܵ�������Ϊ���¼�.
		BYTE bySaveWeaveCount = cbWeaveCount;
		for (BYTE i=0; i<bySaveWeaveCount; ++i)
		{
			if (((WeaveItem[i].cbWeaveKind&(WIK_GANG|WIK_FILL|WIK_TING))!=0) && (!WeaveItem[i].cbPublicCard))
			{//�ڸܡ���������²������Ʊ�־��Ϊ������.
				--cbWeaveCount;
			}
		}


		if ( cbWeaveCount!=0 && ( (wChiHuKind==CHK_JIANG_JIANG && wChiHuRight==CHK_NULL) || (wChiHuKind==CHK_JI_HU && wChiHuRight==CHK_NULL) || (wChiHuKind==CHK_PING_HU && wChiHuRight==CHK_NULL) ) )
		{//�¼���.����ǽ�������������ƽ�������ܺ��ơ�
			//���ý��
			return WIK_NULL;
		}
		else
		{
			if (cbWeaveCount!=0)
			{//�¼���,������Ǽ�����ƽ���������������ܺ��ơ�
				WORD byTempHuType = 0;
				WORD wHuCardType[3] = {CHK_JI_HU, CHK_PING_HU, CHK_PENG_PENG};
				for (int i=0; i<3; i++)
				{
					if ((wChiHuKind&wHuCardType[i]) > 0)
					{
						byTempHuType |= wHuCardType[i];
					}
				}
				wChiHuKind = byTempHuType;
				if (0 == wChiHuKind)
				{
					//���ý��
					return WIK_NULL;
				}
			}
			
			//���ý��
			ChiHuResult.wChiHuKind=wChiHuKind;
			ChiHuResult.wChiHuRight=wChiHuRight;

			return WIK_CHI_HU;
		}

	}

	return WIK_NULL;
}

//�Ժ������������ƣ�
BYTE CGameLogic::AnalyseChiHuLevity(BYTE cbCardIndex[MAX_INDEX],tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, WORD wChiHuRight, \
									tagChiHuResult &ChiHuResult,BYTE cbKingCard[],BYTE cbKingCount,BYTE cbReCardIndex[MAX_INDEX], bool bIsMe)
{
	//��������
	WORD wChiHuKind=CHK_NULL;
	static CAnalyseItemArray AnalyseItemArray;
	ZeroMemory(cbReCardIndex,sizeof(BYTE)*MAX_INDEX);

	//���ñ���
	AnalyseItemArray.RemoveAll();
	ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));

	//�����齫
	BYTE cbCardIndexTemp[MAX_INDEX]={0};
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(BYTE)*MAX_INDEX);
	CopyMemory(cbReCardIndex,cbCardIndex,sizeof(BYTE)*MAX_INDEX);
	////ת��������
	//for (int i=0;i<MAX_INDEX; i++)
	//{
	//	if (cbCardIndex[i] > 0)
	//	{
	//		cbCardIndexTemp[SwitchToCardIndex(cbCardIndex[i])]++;
	//		cbReCardIndex[SwitchToCardIndex(cbCardIndex[i])]++;
	//	}		
	//}

	////ͳ��������Ŀ
	BYTE bKingCount=0;
	//ͳ�Ƴ����������������
	bKingCount=GetKingCardCount(cbCardIndexTemp,NULL,0,cbKingCard,cbKingCount);
	if (!bIsMe)
	{
		if (cbKingCard[0] == cbCurrentCard)
		{
			bKingCount++;
		}
	}
	//û������
	if (bKingCount == 0)
	{
		BYTE byValue = AnalyseChiHuCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,0,ChiHuResult);
		wChiHuKind|=ChiHuResult.wChiHuKind;
        wChiHuRight|=ChiHuResult.wChiHuRight;
	}
	else
	{
		//�����齫
		if (cbCurrentCard!=0)
		{
			cbReCardIndex[SwitchToCardIndex(cbCurrentCard)]++;
		}

		//�жϺ���
		BYTE  cbCardDataTemp[27]=
		{
			0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,					//����
				0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,				//����
				0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,				//ͬ��
		};

		//һ������
		if((wChiHuKind==CHK_NULL)&&(bKingCount==1))
		{
			//�����˿�
			BYTE cbTempCard[MAX_INDEX]={0};
			CopyMemory(cbTempCard,cbReCardIndex,sizeof(cbTempCard));

			//ɾ������
			BYTE cbIndex=SwitchToCardIndex(cbKingCard[0]);
			if(cbIndex>=0 && cbIndex<MAX_INDEX && cbTempCard[cbIndex]>0)
			{
				cbTempCard[cbIndex]-=1;
			}

			tagWeaveItem *pWeaveItem=WeaveItem;
			//�м����
			//BYTE bHuKind=WIK_NULL;
			tagChiHuResult  ChiHuResult;
			WORD bMaxHuKind = 0;
			WORD bTempHuRight = 0;
			BYTE cbMaxReCardIndex[MAX_INDEX]={0};

			//�ܷ����
			for (BYTE i=0;i<27;i++)
			{
				ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));
				WORD bHuKind=WIK_NULL;
				WORD bChiHuKind = 0;
				WORD bChiHuRight = 0;
				bHuKind=AnalyseChiHuCard(cbTempCard,pWeaveItem,cbWeaveCount,cbCardDataTemp[i],0, ChiHuResult);
				if (bHuKind!=WIK_NULL)
				{
					bChiHuKind=ChiHuResult.wChiHuKind;
					bChiHuRight=ChiHuResult.wChiHuRight;
					//�����˿�
					cbTempCard[SwitchToCardIndex(cbCardDataTemp[i])]++;

					//�Ƿ�������
					if (bChiHuKind > bMaxHuKind)
					{
						bMaxHuKind = bChiHuKind;
						bTempHuRight = bChiHuRight;
						//ȡ���˿�
						CopyMemory(cbReCardIndex,cbTempCard,sizeof(cbTempCard));
					}

					//ɾ���˿�
					cbTempCard[SwitchToCardIndex(cbCardDataTemp[i])]--;
					//break;
				}
			}      
			wChiHuRight |= bTempHuRight;
			wChiHuKind = bMaxHuKind;

		}
		else if((wChiHuKind==CHK_NULL)&&(bKingCount==2))//��������
		{
			//�����˿�
			BYTE cbTempCard[MAX_INDEX]={0};
			CopyMemory(cbTempCard,cbReCardIndex,sizeof(cbTempCard));

			//ɾ������
			for(BYTE i=0;i<2;i++)
			{
				BYTE cbIndex=SwitchToCardIndex(cbKingCard[0]);
				if(cbIndex>=0 && cbIndex<MAX_INDEX && cbTempCard[cbIndex]>0)
				{
					cbTempCard[cbIndex]-=1;				
				}
			}

			tagWeaveItem *pWeaveItem=WeaveItem;

			//�м����
			WORD bHuKind=WIK_NULL;
			tagChiHuResult  ChiHuResult;
			WORD bMaxHuKind = 0;
			WORD bTempHuRight = 0;
			BYTE cbMaxReCardIndex[MAX_INDEX]={0};

			//��ȡ�˿�
			for (BYTE i=0;i<27;i++)
			{
				//�����˿�
				cbTempCard[SwitchToCardIndex(cbCardDataTemp[i])]++;
				ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));

				pWeaveItem=WeaveItem;
				WORD bChiHuKind = 0;
				WORD bChiHuRight = 0;

				//�ܷ����
				for(BYTE j=0;j<27;j++)
				{	
					ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));
					bHuKind=WIK_NULL;
					bHuKind=AnalyseChiHuCard(cbTempCard,pWeaveItem,cbWeaveCount,cbCardDataTemp[j],0, ChiHuResult);
					if(bHuKind!=WIK_NULL)
					{
						bChiHuKind=ChiHuResult.wChiHuKind;
						bChiHuRight=ChiHuResult.wChiHuRight;
						//�����˿�
						cbTempCard[SwitchToCardIndex(cbCardDataTemp[j])]++;

						//�Ƿ�������
						if (bChiHuKind > bMaxHuKind)
						{
							bMaxHuKind = bChiHuKind;
							bTempHuRight = bChiHuRight;
							//ȡ���˿�
							CopyMemory(cbReCardIndex,cbTempCard,sizeof(cbTempCard));
						}
						//ɾ���˿�
						cbTempCard[SwitchToCardIndex(cbCardDataTemp[j])]--;
					}
				}
				//ɾ���˿�
				cbTempCard[SwitchToCardIndex(cbCardDataTemp[i])]--;
			}

			wChiHuRight |= bTempHuRight;
			wChiHuKind = bMaxHuKind;

		}

		else if ((wChiHuKind==CHK_NULL)&&(bKingCount==3))//��������
		{
			//�����˿�
			BYTE cbTempCard[MAX_INDEX]={0};
			CopyMemory(cbTempCard,cbReCardIndex,sizeof(cbTempCard)); 

			//ɾ������
			for(BYTE i=0;i<3;i++)
			{
				BYTE cbIndex=SwitchToCardIndex(cbKingCard[0]);
				if(cbIndex>=0 && cbIndex<MAX_INDEX && cbTempCard[cbIndex]>0)
				{
					cbTempCard[cbIndex]-=1;				
				}
			}

			tagWeaveItem *pWeaveItem=WeaveItem;

			//�м����
			WORD bHuKind=WIK_NULL;
			tagChiHuResult  ChiHuResult;
			WORD bMaxHuKind = 0;
			WORD bTempHuRight = 0;
			BYTE cbMaxReCardIndex[MAX_INDEX]={0};

			//��ȡ�˿�
			for (BYTE i=0;i<27;i++)
			{
				//�����˿�
				cbTempCard[SwitchToCardIndex(cbCardDataTemp[i])]++;
				ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));	

				for(BYTE j=0;j<27;j++)
				{	
					//�����˿�
					cbTempCard[SwitchToCardIndex(cbCardDataTemp[j])]++;
					ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));
					pWeaveItem=WeaveItem;
					WORD bChiHuKind = 0;
					WORD bChiHuRight = 0;

					//�ܷ����
					bHuKind=WIK_NULL;
					for (BYTE m=0;m<27;m++)
					{
						ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));
						bHuKind=WIK_NULL;
						bHuKind=AnalyseChiHuCard(cbTempCard,pWeaveItem,cbWeaveCount,cbCardDataTemp[m],0,ChiHuResult);
						if(bHuKind!=WIK_NULL)
						{
							bChiHuKind=ChiHuResult.wChiHuKind;
							bChiHuRight=ChiHuResult.wChiHuRight;
							//�����˿�
							cbTempCard[SwitchToCardIndex(cbCardDataTemp[m])]++;

							//�Ƿ�������
							if (bChiHuKind > bMaxHuKind)
							{
								bMaxHuKind = bChiHuKind;
								bTempHuRight = bChiHuRight;
								//ȡ���˿�
								CopyMemory(cbReCardIndex,cbTempCard,sizeof(cbTempCard));
							}
							//ɾ���˿�
							cbTempCard[SwitchToCardIndex(cbCardDataTemp[m])]--;
						}
					}
					//ɾ���˿�
					cbTempCard[SwitchToCardIndex(cbCardDataTemp[j])]--;
				}
				//ɾ���˿�
				cbTempCard[SwitchToCardIndex(cbCardDataTemp[i])]--;
			}

			wChiHuRight |= bTempHuRight;
			wChiHuKind = bMaxHuKind;
		}

		else if ((wChiHuKind==CHK_NULL)&&(bKingCount==4))
		{
			//�����˿�
			BYTE cbTempCard[MAX_INDEX]={0};
			CopyMemory(cbTempCard,cbReCardIndex,sizeof(cbTempCard));

			//ɾ������
			for(BYTE i=0;i<4;i++)
			{
				BYTE cbIndex=SwitchToCardIndex(cbKingCard[0]);
				if(cbIndex>=0 && cbIndex<MAX_INDEX && cbTempCard[cbIndex]>0)
				{
					cbTempCard[cbIndex]-=1;				
				}
			}

			tagWeaveItem *pWeaveItem=WeaveItem;

			//�м����
			WORD bHuKind=WIK_NULL;
			tagChiHuResult  ChiHuResult;
			WORD bMaxHuKind = 0;
			WORD bTempHuRight = 0;
			BYTE cbMaxReCardIndex[MAX_INDEX]={0};

			//��ȡ�˿�
			for (BYTE i=0;i<27;i++)
			{
				//�����˿�
				cbTempCard[SwitchToCardIndex(cbCardDataTemp[i])]++;
				ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));	

				for(BYTE j=0;j<27;j++)
				{	
					//�����˿�
					cbTempCard[SwitchToCardIndex(cbCardDataTemp[j])]++;
					ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));

					for (BYTE m=0;m<27;m++)
					{
						//�����˿�
						cbTempCard[SwitchToCardIndex(cbCardDataTemp[m])]++;
						ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));
						pWeaveItem=WeaveItem;
						WORD bChiHuKind = 0;
						WORD bChiHuRight = 0;

						//�ܷ����
						bHuKind=WIK_NULL;
						for (BYTE n=0;n<27;n++)
						{
							ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));
							bHuKind=WIK_NULL;
							bHuKind=AnalyseChiHuCard(cbTempCard,pWeaveItem,cbWeaveCount,cbCardDataTemp[n],0, ChiHuResult);
							if(bHuKind!=WIK_NULL)
							{
								bChiHuKind=ChiHuResult.wChiHuKind;
								bChiHuRight=ChiHuResult.wChiHuRight;
								//�����˿�
								cbTempCard[SwitchToCardIndex(cbCardDataTemp[n])]++;

								//�Ƿ�������
								if (bChiHuKind > bMaxHuKind)
								{
									bMaxHuKind = bChiHuKind;
									bTempHuRight = bChiHuRight;
									//ȡ���˿�
									CopyMemory(cbReCardIndex,cbTempCard,sizeof(cbTempCard));
								}
								//ɾ���˿�
								cbTempCard[SwitchToCardIndex(cbCardDataTemp[n])]--;
							}
						}
						//ɾ���˿�
						cbTempCard[SwitchToCardIndex(cbCardDataTemp[m])]--;
					}
					//ɾ���˿�
					cbTempCard[SwitchToCardIndex(cbCardDataTemp[j])]--;
				}
				//ɾ���˿�
				cbTempCard[SwitchToCardIndex(cbCardDataTemp[i])]--;
			}

			wChiHuRight |= bTempHuRight;
			wChiHuKind = bMaxHuKind;
		}
	}

	//����ж�
	if (wChiHuKind!=CHK_NULL)
	{
		//���ý��
		ChiHuResult.wChiHuKind|=wChiHuKind;
		ChiHuResult.wChiHuRight|=wChiHuRight;
		ChiHuResult.m_bKingCount=bKingCount;

		return WIK_CHI_HU;
	}

	return WIK_NULL;

}
//�����ж�
bool CGameLogic::IsKingCard(BYTE cbCardData,BYTE bKingCard[],BYTE bKingCount)
{
	for(BYTE i=0;i<bKingCount;i++)
	{
		if(bKingCard[i]==0)
			continue;

		if(cbCardData==bKingCard[i])
			return true;

	}
	return false;
}

//������Ŀ
BYTE CGameLogic::GetKingCardCount(BYTE cbCardIndex[MAX_INDEX],tagWeaveItem WeaveItem[],BYTE cbItemCount,BYTE bKingCard[],BYTE bKingCount)
{
	BYTE bReturnKingCount=0;

	for(BYTE i=0;i<cbItemCount;i++)
	{
		BYTE cbCardData=WeaveItem[i].cbCenterCard;

		if(WeaveItem[i].cbWeaveKind==WIK_PENG)
		{

			if(IsKingCard(cbCardData,bKingCard,bKingCount))	//�����Ѿ�����	
			{
				bReturnKingCount=3;
				return bReturnKingCount;
			}
		}
		else if(WeaveItem[i].cbWeaveKind!=WIK_GANG)
		{
			BYTE bCardData[3]={0,0,0};//��������
			bool bEateType=false;	  //�����ж�
			switch(WeaveItem[i].cbWeaveKind)
			{
			case WIK_LEFT://���
				{
					bCardData[0]=cbCardData;
					bCardData[1]=cbCardData+0x01;
					bCardData[2]=cbCardData+0x02;
					bEateType=true;				
					break;
				}
			case WIK_CENTER://�г�
				{
					bCardData[0]=cbCardData-0x01;
					bCardData[1]=cbCardData;
					bCardData[2]=cbCardData+0x01;
					bEateType=true;
					break;
				}
			case WIK_RIGHT://�ҳ�
				{
					bCardData[0]=cbCardData-0x02;
					bCardData[1]=cbCardData-0x01;
					bCardData[2]=cbCardData;
					bEateType=true;
					break;
				}
			}

			if(bEateType)
			{
				for(BYTE j=0;j<3;j++)
				{
					//������������
					if(IsKingCard(bCardData[j],bKingCard,bKingCount))bReturnKingCount++;					
				}
			}

		}

		//if(bReturnKingCount>2)return bReturnKingCount;
	}

	for(BYTE i=0;i<MAX_INDEX;i++)
	{
		if(cbCardIndex[i]>0)
		{
			BYTE cbCard=SwitchToCardData(i);

			//������������
			if(IsKingCard(cbCard,bKingCard,bKingCount))
				bReturnKingCount+=cbCardIndex[i];

			//if(bReturnKingCount>2)return bReturnKingCount;
		}
	}

	return bReturnKingCount;
}

//�����˿�
bool CGameLogic::AnalyseCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, CAnalyseItemArray & AnalyseItemArray)
{
	//������Ŀ
	BYTE cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) cbCardCount+=cbCardIndex[i];

	//Ч����Ŀ
	ASSERT((cbCardCount>=2)&&(cbCardCount<=MAX_COUNT)&&((cbCardCount-2)%3==0));
	if ((cbCardCount<2)||(cbCardCount>MAX_COUNT)||((cbCardCount-2)%3!=0)) return false;

	//��������
	BYTE cbKindItemCount=0;
	tagKindItem KindItem[MAX_COUNT-2];
	ZeroMemory(KindItem,sizeof(KindItem));

	//�����ж�
	BYTE cbLessKindItem=(cbCardCount-2)/3;
	ASSERT((cbLessKindItem+cbWeaveCount)==4);

	//�����ж�
	if (cbLessKindItem==0)
	{
		//Ч�����
		ASSERT((cbCardCount==2)&&(cbWeaveCount==4));

		//�����ж�
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			if (cbCardIndex[i]==2)
			{
				//��������
				tagAnalyseItem AnalyseItem;
				ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

				//���ý��
				for (BYTE j=0;j<cbWeaveCount;j++)
				{
					AnalyseItem.cbWeaveKind[j]=WeaveItem[j].cbWeaveKind;
					AnalyseItem.cbCenterCard[j]=WeaveItem[j].cbCenterCard;
				}
				AnalyseItem.cbCardEye=SwitchToCardData(i);

				//������
				AnalyseItemArray.Add(AnalyseItem);

				return true;
			}
		}

		return false;
	}

	//��ַ���
	if (cbCardCount>=3)
	{
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			//ͬ���ж�
			if (cbCardIndex[i]>=3)
			{
				KindItem[cbKindItemCount].cbCenterCard=i;
				KindItem[cbKindItemCount].cbCardIndex[0]=i;
				KindItem[cbKindItemCount].cbCardIndex[1]=i;
				KindItem[cbKindItemCount].cbCardIndex[2]=i;
				KindItem[cbKindItemCount++].cbWeaveKind=WIK_PENG;
			}

			//�����ж�
			if ((i<(MAX_INDEX-2))&&(cbCardIndex[i]>0)&&((i%9)<7))
			{
				for (BYTE j=1;j<=cbCardIndex[i];j++)
				{
					if ((cbCardIndex[i+1]>=j)&&(cbCardIndex[i+2]>=j))
					{
						KindItem[cbKindItemCount].cbCenterCard=i;
						KindItem[cbKindItemCount].cbCardIndex[0]=i;
						KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
						KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
						KindItem[cbKindItemCount++].cbWeaveKind=WIK_LEFT;
					}
				}
			}
		}
	}

	//��Ϸ���
	if (cbKindItemCount>=cbLessKindItem)
	{
		//��������
		BYTE cbCardIndexTemp[MAX_INDEX];
		ZeroMemory(cbCardIndexTemp,sizeof(cbCardIndexTemp));

		//��������
		BYTE cbIndex[4]={0,1,2,3};
		tagKindItem * pKindItem[4];
		ZeroMemory(&pKindItem,sizeof(pKindItem));

		//��ʼ���
		do
		{
			//���ñ���
			CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
			for (BYTE i=0;i<cbLessKindItem;i++) pKindItem[i]=&KindItem[cbIndex[i]];

			//�����ж�
			bool bEnoughCard=true;
			for (BYTE i=0;i<cbLessKindItem*3;i++)
			{
				//�����ж�
				BYTE cbCardIndex=pKindItem[i/3]->cbCardIndex[i%3];
				if (cbCardIndexTemp[cbCardIndex]==0)
				{
					bEnoughCard=false;
					break;
				}
				else cbCardIndexTemp[cbCardIndex]--;
			}

			//�����ж�
			if (bEnoughCard==true)
			{
				//�����ж�
				BYTE cbCardEye=0;
				for (BYTE i=0;i<MAX_INDEX;i++)
				{
					if (cbCardIndexTemp[i]==2)
					{
						cbCardEye=SwitchToCardData(i);
						break;
					}
				}

				//�������
				if (cbCardEye!=0)
				{
					//��������
					tagAnalyseItem AnalyseItem;
					ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

					//�������
					for (BYTE i=0;i<cbWeaveCount;i++)
					{
						AnalyseItem.cbWeaveKind[i]=WeaveItem[i].cbWeaveKind;
						AnalyseItem.cbCenterCard[i]=WeaveItem[i].cbCenterCard;
					}

					//��������
					for (BYTE i=0;i<cbLessKindItem;i++) 
					{
						AnalyseItem.cbWeaveKind[i+cbWeaveCount]=pKindItem[i]->cbWeaveKind;
						AnalyseItem.cbCenterCard[i+cbWeaveCount]=pKindItem[i]->cbCenterCard;
					}

					//��������
					AnalyseItem.cbCardEye=cbCardEye;

					//������
					AnalyseItemArray.Add(AnalyseItem);
				}
			}

			//��������
			if (cbIndex[cbLessKindItem-1]==(cbKindItemCount-1))
			{
				for (BYTE i=cbLessKindItem-1;i>0;i--)
				{
					if ((cbIndex[i-1]+1)!=cbIndex[i])
					{
						BYTE cbNewIndex=cbIndex[i-1];
						for (BYTE j=(i-1);j<cbLessKindItem;j++) cbIndex[j]=cbNewIndex+j-i+2;
						break;
					}
				}
				if (i==0) break;
			}
			else cbIndex[cbLessKindItem-1]++;

		} while (true);

	}

	return (AnalyseItemArray.GetCount()>0);
}


//�˿�ת��
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex)
{
	ASSERT(cbCardIndex<34);
	return ((cbCardIndex/9)<<4)|(cbCardIndex%9+1);
}


//�˿�ת��
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT])
{
	//ת���˿�
	BYTE cbPosition=0;
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndex[i]!=0)
		{
			for (BYTE j=0;j<cbCardIndex[i];j++)
			{
				ASSERT(cbPosition<MAX_COUNT);
				cbCardData[cbPosition++]=SwitchToCardData(i);
			}
		}
	}

	return cbPosition;
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData)
{
	ASSERT(IsValidCard(cbCardData));
	return ((cbCardData&MASK_COLOR)>>4)*9+(cbCardData&MASK_VALUE)-1;
}

//�˿�ת��
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX])
{
	//���ñ���
	ZeroMemory(cbCardIndex,sizeof(BYTE)*MAX_INDEX);

	//ת���˿�
	for (BYTE i=0;i<cbCardCount;i++)
	{
		ASSERT(IsValidCard(cbCardData[i]));
		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}

	return cbCardCount;
}

//���ӣ���һɫ�ƣ���С���ƣ��������ƣ���������

//��һɫ��
bool CGameLogic::IsQingYiSe(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount)
{
	//�����ж�
	BYTE cbCardColor=0xFF;
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndex[i]!=0)
		{
			//��ɫ�ж�
			if (cbCardColor!=0xFF) return false;

			//���û�ɫ
			cbCardColor=(SwitchToCardData(i)&MASK_COLOR);

			//��������
			i=(i/9+1)*9;
		}
	}

	//����ж�
	for (BYTE i=0;i<cbItemCount;i++)
	{
		BYTE cbCenterCard=WeaveItem[i].cbCenterCard;
		if ((cbCenterCard&MASK_COLOR)!=cbCardColor) return false;
	}

	return true;
}

//��С����
bool CGameLogic::IsQiXiaoDui(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount)
{
	//����ж�
	if (cbWeaveCount!=0) return false;

	//�˿��ж�
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		BYTE cbCardCount=cbCardIndex[i];
		if ((cbCardCount!=0)&&(cbCardCount!=2)&&(cbCardCount!=4)) return false;
	}

	return true;
}

//��ǰ����
bool CGameLogic::IsMenQianQing(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount)
{
	//����ж�
	if (cbWeaveCount!=0)
	{
		return false;
	}

	  return true;

}

////��������
//bool CGameLogic::IsHaoHuaDui(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount)
//{
//	//��������
//	bool bFourCard=false;
//
//	//����ж�
//	for (BYTE i=0;i<cbItemCount;i++)
//	{
//		//�˲��ж�
//		if (WeaveItem[i].cbWeaveKind!=WIK_FILL) return false;
//		if (WeaveItem[i].cbWeaveKind!=WIK_GANG) return false;
//
//		//���ñ���
//		bFourCard=true;
//	}
//
//	//�˿��ж�
//	for (BYTE i=0;i<MAX_INDEX;i++)
//	{
//		//�����ж�
//		if (cbCardIndex[i]==4)
//		{
//			bFourCard=true;
//			continue;
//		}
//
//		//�����ж�
//		if ((cbCardIndex[i]!=0)&&(cbCardIndex[i]!=2)) return false;
//	}
//
//	//����ж�
//	if (bFourCard==false) return false;
//
//	return true;
//}

//��������
bool CGameLogic::IsJiangJiangHu(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount)
{
	//����ж�
	for (BYTE i=0;i<cbWeaveCount;i++)
	{
		//�����ж�
		BYTE cbWeaveKind=WeaveItem[i].cbWeaveKind;
		if ((cbWeaveKind!=WIK_PENG)&&(cbWeaveKind!=WIK_GANG)&&(cbWeaveKind!=WIK_FILL)) return false;

		//��ֵ�ж�
		BYTE cbCenterValue=(WeaveItem[i].cbCenterCard&MASK_VALUE);
		if ((cbCenterValue!=2)&&(cbCenterValue!=5)&&(cbCenterValue!=8)) return false;
	}

	//�˿��ж�
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if ((i%3!=1)&&(cbCardIndex[i]!=0)) return false;
	}

	return true;
}

//�ж��Ƿ��������
//���룺�����ƣ�����ƣ������
bool CGameLogic::IsCanTingCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbKingCard[])
{
	m_cbHuCardIndex.clear();
    //�����ж�
	BYTE chCanHu = WIK_NULL;
	BYTE byHuNums = 0;

	//�жϺ���
	BYTE  cbCardDataTemp[27]=
	{
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,					//����
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,			    	//����
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,			    	//ͬ��
	};

	for (int j=0;j<27;j++)
	{
		//�����齫
		BYTE cbCardIndexTemp[MAX_INDEX]={0};
		CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(BYTE)*MAX_INDEX);

		//���ܺ������Ƶ��ơ�
		bool bIsHave = false;
		for (int i=0; i<MAX_INDEX; i++)
		{
			if (4==cbCardIndexTemp[i])
			{
				bIsHave = false;
				BYTE byCard = SwitchToCardData(i);
				if (byCard == cbCardDataTemp[j])
				{
					bIsHave = true;
					break;
				}
			}
		}
		if (bIsHave)
		{
			continue;
		}

		tagChiHuResult ChiHuResult;
		BYTE cbReCardIndex[MAX_INDEX];
		BYTE cbCurrentCard=cbCardDataTemp[j];
		chCanHu  = AnalyseChiHuLevity(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,0,ChiHuResult,cbKingCard,1,cbReCardIndex);
		if (WIK_CHI_HU == chCanHu)
		{
			m_cbHuCardIndex.push_back(cbCurrentCard);
			++byHuNums;
			//break;
		}
	}

	//�����ж�
	if(/*WIK_CHI_HU == chCanHu*/byHuNums>0)
	{
		return true;
	}
	else
	{
		return false;
	}

}


int CGameLogic::GetHuPaiData(BYTE cbCardData[], BYTE cbCardCount)
{
	BYTE byHuNums = m_cbHuCardIndex.size();
	if (byHuNums>0)
	{
		for (int i=0; i<byHuNums&&i<8; ++i)
		{
			if (i < cbCardCount)
			{
				cbCardData[i] = m_cbHuCardIndex[i];
			}
		}
	}
	if (byHuNums>=8)
	{
		byHuNums=8;
	}
	return byHuNums;
}

//////////////////////////////////////////////////////////////////////////
