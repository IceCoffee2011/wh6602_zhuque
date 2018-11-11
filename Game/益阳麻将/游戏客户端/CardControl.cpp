#include "StdAfx.h"
#include "Resource.h"
#include "CardControl.h"
#include "GameClientDlg.h"
#include "TraceCatch.h"
//////////////////////////////////////////////////////////////////////////
//�궨��

//��������
#define POS_SHOOT					5									//��������
#define POS_SPACE					8									//�ָ����
#define ITEM_COUNT					29									//������Ŀ
#define INVALID_ITEM				0xFFFF								//��Ч����

//�˿˴�С
#define CARD_WIDTH					48									//�˿˿��
#define CARD_HEIGHT					75									//�˿˸߶�

//////////////////////////////////////////////////////////////////////////

//���캯��
CCardListImage::CCardListImage()
{
	//λ�ñ���
	m_nItemWidth=0;
	m_nItemHeight=0;
	m_nViewWidth=0;
	m_nViewHeight=0;

	return;
}

//��������
CCardListImage::~CCardListImage()
{
}


//////////////////////////////////////////////////////////////////////////

//���캯��
CHeapCard::CHeapCard()
{
__ENTER_FUNCTION
	//���Ʊ���
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_East;

	//�˿˱���
	m_wFullCount=0;
	m_wMinusHeadCount=0;
	m_wMinusLastCount=0;

	//��������
	TCHAR szResourcePath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,szResourcePath);
	CString strPath(szResourcePath);
	strPath.Append("\\SparrowYY\\Picture\\Card\\");
	m_ImageHeapDoubleVL.LoadImage(TEXT(strPath+"CARD_HEAP_DOUBLE_VL.png"));
	m_ImageHeapDoubleVR.LoadImage(TEXT(strPath+"CARD_HEAP_DOUBLE_VR.png"));
	m_ImageHeapDoubleHT.LoadImage(TEXT(strPath+"CARD_HEAP_DOUBLE_HT.png"));
	m_ImageHeapDoubleHB.LoadImage(TEXT(strPath+"CARD_HEAP_DOUBLE_HB.png"));

	m_ImageHeapSingleVL.LoadImage(TEXT(strPath+"CARD_HEAP_SINGLE_VL.png"));
	m_ImageHeapSingleVR.LoadImage(TEXT(strPath+"CARD_HEAP_SINGLE_VR.png"));
	m_ImageHeapSingleHT.LoadImage(TEXT(strPath+"CARD_HEAP_SINGLE_HT.png"));
	m_ImageHeapSingleHB.LoadImage(TEXT(strPath+"CARD_HEAP_SINGLE_HB.png"));
__LEAVE_FUNCTION
	return;
}

//��������
CHeapCard::~CHeapCard()
{
}


//�滭�˿�
void CHeapCard::DrawCardControl(CDC * pDC)
{
__ENTER_FUNCTION
	switch (m_CardDirection)
	{
	case Direction_East:	//����
		{
			//�滭�˿�
			if ((m_wFullCount-m_wMinusHeadCount-m_wMinusLastCount)>0)
			{
				//��������
				int nXPos=0,nYPos=0;
				int wHeapIndex=m_wMinusHeadCount/2;
				int wDoubleHeap=(m_wMinusHeadCount+1)/2;
				int wDoubleLast=(m_wFullCount-m_wMinusLastCount)/2;
				int wFinallyIndex=(m_wFullCount-m_wMinusLastCount)/2;

				//ͷ���˿�
				if (m_wMinusHeadCount%2!=0)
				{
					int nWidth=m_ImageHeapSingleVR.GetWidth()/14;
					int nHeight=m_ImageHeapSingleVR.GetHeight();
					if (0==wHeapIndex)
					{
						nXPos=m_ControlPoint.x-4;
						nYPos=m_ControlPoint.y+2;
					}
					else if (1==wHeapIndex)
					{
						nXPos=m_ControlPoint.x+1;
						nYPos=m_ControlPoint.y+12;
					}
					else if (2==wHeapIndex)
					{
						nXPos=m_ControlPoint.x+6;
						nYPos=m_ControlPoint.y+22;
					}
					else if (3==wHeapIndex)
					{
						nXPos=m_ControlPoint.x+11;
						nYPos=m_ControlPoint.y+32;
					}
					else if (4==wHeapIndex)
					{
						nXPos=m_ControlPoint.x+17;
						nYPos=m_ControlPoint.y+42;
					}
					else if (5==wHeapIndex)
					{
						nXPos=m_ControlPoint.x+23;
						nYPos=m_ControlPoint.y+54;
					}
					else if (6==wHeapIndex)
					{
						nXPos=m_ControlPoint.x+28;
						nYPos=m_ControlPoint.y+66;
					}
					else if (7==wHeapIndex)
					{
						nXPos=m_ControlPoint.x+35;
						nYPos=m_ControlPoint.y+79;
					}
					else if (8==wHeapIndex)
					{
						nXPos=m_ControlPoint.x+43;
						nYPos=m_ControlPoint.y+93;
					}
					else if (9==wHeapIndex)
					{
						nXPos=m_ControlPoint.x+52;
						nYPos=m_ControlPoint.y+108;
					}
					else if (10==wHeapIndex)
					{
						nXPos=m_ControlPoint.x+59;
						nYPos=m_ControlPoint.y+125;
					}
					else if (11==wHeapIndex)
					{
						nXPos=m_ControlPoint.x+70;
						nYPos=m_ControlPoint.y+143;
					}
					else if (12==wHeapIndex)
					{
						nXPos=m_ControlPoint.x+80;
						nYPos=m_ControlPoint.y+163;
					}
					else if (13==wHeapIndex)
					{
						nXPos=m_ControlPoint.x+92;
						nYPos=m_ControlPoint.y+184;
					}
					m_ImageHeapSingleVR.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(13-wHeapIndex),0,nWidth,nHeight);
				}

				//�м��˿�
				for (int i=wDoubleHeap;i<wFinallyIndex;i++)
				{
					int nWidth=m_ImageHeapDoubleVR.GetWidth()/14;
					int nHeight=m_ImageHeapDoubleVR.GetHeight();
					if (0==i)
					{
						nXPos=m_ControlPoint.x-4;
						nYPos=m_ControlPoint.y+2;
					}
					else if (1==i)
					{
						nXPos=m_ControlPoint.x+1;
						nYPos=m_ControlPoint.y+12;
					}
					else if (2==i)
					{
						nXPos=m_ControlPoint.x+6;
						nYPos=m_ControlPoint.y+22;
					}
					else if (3==i)
					{
						nXPos=m_ControlPoint.x+11;
						nYPos=m_ControlPoint.y+32;
					}
					else if (4==i)
					{
						nXPos=m_ControlPoint.x+17;
						nYPos=m_ControlPoint.y+42;
					}
					else if (5==i)
					{
						nXPos=m_ControlPoint.x+23;
						nYPos=m_ControlPoint.y+54;
					}
					else if (6==i)
					{
						nXPos=m_ControlPoint.x+28;
						nYPos=m_ControlPoint.y+66;
					}
					else if (7==i)
					{
						nXPos=m_ControlPoint.x+35;
						nYPos=m_ControlPoint.y+79;
					}
					else if (8==i)
					{
						nXPos=m_ControlPoint.x+43;
						nYPos=m_ControlPoint.y+93;
					}
					else if (9==i)
					{
						nXPos=m_ControlPoint.x+52;
						nYPos=m_ControlPoint.y+108;
					}
					else if (10==i)
					{
						nXPos=m_ControlPoint.x+59;
						nYPos=m_ControlPoint.y+125;
					}
					else if (11==i)
					{
						nXPos=m_ControlPoint.x+70;
						nYPos=m_ControlPoint.y+143;
					}
					else if (12==i)
					{
						nXPos=m_ControlPoint.x+80;
						nYPos=m_ControlPoint.y+163;
					}
					else if (13==i)
					{
						nXPos=m_ControlPoint.x+92;
						nYPos=m_ControlPoint.y+184;
					}
					m_ImageHeapDoubleVR.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(13-i),0,nWidth,nHeight);
				}

				//β���˿�
				if (m_wMinusLastCount%2!=0)
				{
					nXPos=m_ControlPoint.x;
					nYPos=m_ControlPoint.y+wFinallyIndex*23+9;
					if (0==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x-4;
						nYPos=m_ControlPoint.y+2;
					}
					else if (1==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x+1;
						nYPos=m_ControlPoint.y+12;
					}
					else if (2==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x+6;
						nYPos=m_ControlPoint.y+22;
					}
					else if (3==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x+11;
						nYPos=m_ControlPoint.y+32;
					}
					else if (4==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x+17;
						nYPos=m_ControlPoint.y+42;
					}
					else if (5==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x+23;
						nYPos=m_ControlPoint.y+54;
					}
					else if (6==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x+28;
						nYPos=m_ControlPoint.y+66;
					}
					else if (7==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x+35;
						nYPos=m_ControlPoint.y+79;
					}
					else if (8==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x+43;
						nYPos=m_ControlPoint.y+93;
					}
					else if (9==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x+52;
						nYPos=m_ControlPoint.y+108;
					}
					else if (10==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x+59;
						nYPos=m_ControlPoint.y+125;
					}
					else if (11==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x+70;
						nYPos=m_ControlPoint.y+143;
					}
					else if (12==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x+80;
						nYPos=m_ControlPoint.y+163;
					}
					else if (13==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x+92;
						nYPos=m_ControlPoint.y+184;
					}
					int nWidth=m_ImageHeapSingleVR.GetWidth()/14;
					int nHeight=m_ImageHeapSingleVR.GetHeight();
					m_ImageHeapSingleVR.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(13-wFinallyIndex),0,nWidth,nHeight);
				}
			}

			break;
		}
	case Direction_South:	//����
		{
			//�滭�˿�
			if ((m_wFullCount-m_wMinusHeadCount-m_wMinusLastCount)>0)
			{
				//��������
				int nXPos=0,nYPos=0;
				int wHeapIndex=m_wMinusHeadCount/2;
				int wDoubleHeap=(m_wMinusHeadCount+1)/2;
				int wDoubleLast=(m_wFullCount-m_wMinusLastCount)/2;
				int wFinallyIndex=(m_wFullCount-m_wMinusLastCount)/2;


				//ͷ���˿�
				if (m_wMinusHeadCount%2!=0)
				{
					int nWidth=m_ImageHeapSingleHB.GetWidth()/14;
					int nHeight=m_ImageHeapSingleHB.GetHeight();
					if (wHeapIndex < 7)
					{
						nXPos=m_ControlPoint.x+35*(13-wHeapIndex)+4;
						nYPos=m_ControlPoint.y;
						m_ImageHeapSingleHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(13-wHeapIndex),0,nWidth,nHeight);
					}
				}

				//�м��˿�
				for (int i=wDoubleHeap;i<wFinallyIndex && i <7; ++i)
				{
					int nWidth=m_ImageHeapDoubleHB.GetWidth()/14;
					int nHeight=m_ImageHeapDoubleHB.GetHeight();
					nXPos=m_ControlPoint.x+35*(13-i)+4;
					nYPos=m_ControlPoint.y;

					m_ImageHeapDoubleHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(13-i),0,nWidth,nHeight);
				}

				//β���˿�
				if (m_wMinusLastCount%2!=0)
				{
					int nWidth=m_ImageHeapSingleHB.GetWidth()/14;
					int nHeight=m_ImageHeapSingleHB.GetHeight();
					if (wFinallyIndex > 6)
					{
						nXPos=m_ControlPoint.x+35*(13-wFinallyIndex);
						nYPos=m_ControlPoint.y;
						m_ImageHeapSingleHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(13-wFinallyIndex),0,nWidth,nHeight);
					} 
					else if (wFinallyIndex < 7)
					{
						nXPos=m_ControlPoint.x+35*(wFinallyIndex+1)+4;
						nYPos=m_ControlPoint.y;
						m_ImageHeapSingleHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(wFinallyIndex+1),0,nWidth,nHeight);
					}

				}

				//�м��˿�
				for (int i=wFinallyIndex;i>7 && i>wDoubleHeap; --i)
				{
					int nWidth=m_ImageHeapDoubleHB.GetWidth()/14;
					int nHeight=m_ImageHeapDoubleHB.GetHeight();
					nXPos=m_ControlPoint.x+35*(14-i)+3;
					nYPos=m_ControlPoint.y;
					m_ImageHeapDoubleHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(14-i),0,nWidth,nHeight);
				}

				//ͷ���˿�
				if (m_wMinusHeadCount%2!=0)
				{
					int nWidth=m_ImageHeapSingleHB.GetWidth()/14;
					int nHeight=m_ImageHeapSingleHB.GetHeight();
					if (wHeapIndex > 6 )
					{
						nXPos=m_ControlPoint.x+35*(13-wHeapIndex)+4;
						nYPos=m_ControlPoint.y;
						m_ImageHeapSingleHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(13-wHeapIndex),0,nWidth,nHeight);
					}
				}

			}

			break;
		}
	case Direction_West:	//����
		{
			//�滭�˿�
			if ((m_wFullCount-m_wMinusHeadCount-m_wMinusLastCount)>0)
			{
				//��������
				int nXPos=0,nYPos=0;
				int wHeapIndex=m_wMinusLastCount/2;
				int wDoubleHeap=(m_wMinusHeadCount+1)/2;
				int wDoubleLast=(m_wFullCount-m_wMinusLastCount)/2;
				int wFinallyIndex=(m_wFullCount-m_wMinusLastCount)/2;

				//β���˿�
				if (m_wMinusLastCount%2!=0)
				{
					if (0==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x+2;
						nYPos=m_ControlPoint.y-11;
					}
					else if (1==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x-3;
						nYPos=m_ControlPoint.y-1;
					}
					else if (2==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x-8;
						nYPos=m_ControlPoint.y+9;
					}
					else if (3==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x-13;
						nYPos=m_ControlPoint.y+19;
					}
					else if (4==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x-17;
						nYPos=m_ControlPoint.y+29;
					}
					else if (5==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x-23;
						nYPos=m_ControlPoint.y+41;
					}
					else if (6==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x-30;
						nYPos=m_ControlPoint.y+54;
					}
					else if (7==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x-37;
						nYPos=m_ControlPoint.y+67;
					}
					else if (8==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x-45;
						nYPos=m_ControlPoint.y+81;
					}
					else if (9==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x-52;
						nYPos=m_ControlPoint.y+96;
					}
					else if (10==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x-61;
						nYPos=m_ControlPoint.y+113;
					}
					else if (11==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x-70;
						nYPos=m_ControlPoint.y+131;
					}
					else if (12==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x-80;
						nYPos=m_ControlPoint.y+151;
					}
					else if (13==wFinallyIndex)
					{
						nXPos=m_ControlPoint.x-92;
						nYPos=m_ControlPoint.y+172;
					}
					int nWidth=m_ImageHeapSingleVL.GetWidth()/14;
					int nHeight=m_ImageHeapSingleVL.GetHeight();
					m_ImageHeapSingleVL.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*wFinallyIndex,0,nWidth,nHeight);
				}

				//�м��˿�
				for (int i=wFinallyIndex-1;i>=wDoubleHeap;i--)
				{
					int nWidth=m_ImageHeapDoubleVL.GetWidth()/14;
					int nHeight=m_ImageHeapDoubleVL.GetHeight();
					if (13==i)
					{
						nXPos=m_ControlPoint.x+2;
						nYPos=m_ControlPoint.y-11;
					}
					else if (12==i)
					{
						nXPos=m_ControlPoint.x-3;
						nYPos=m_ControlPoint.y-1;
					}
					else if (11==i)
					{
						nXPos=m_ControlPoint.x-8;
						nYPos=m_ControlPoint.y+9;
					}
					else if (10==i)
					{
						nXPos=m_ControlPoint.x-13;
						nYPos=m_ControlPoint.y+19;
					}
					else if (9==i)
					{
						nXPos=m_ControlPoint.x-17;
						nYPos=m_ControlPoint.y+29;
					}
					else if (8==i)
					{
						nXPos=m_ControlPoint.x-23;
						nYPos=m_ControlPoint.y+41;
					}
					else if (7==i)
					{
						nXPos=m_ControlPoint.x-30;
						nYPos=m_ControlPoint.y+54;
					}
					else if (6==i)
					{
						nXPos=m_ControlPoint.x-37;
						nYPos=m_ControlPoint.y+67;
					}
					else if (5==i)
					{
						nXPos=m_ControlPoint.x-45;
						nYPos=m_ControlPoint.y+81;
					}
					else if (4==i)
					{
						nXPos=m_ControlPoint.x-52;
						nYPos=m_ControlPoint.y+96;
					}
					else if (3==i)
					{
						nXPos=m_ControlPoint.x-61;
						nYPos=m_ControlPoint.y+113;
					}
					else if (2==i)
					{
						nXPos=m_ControlPoint.x-70;
						nYPos=m_ControlPoint.y+131;
					}
					else if (1==i)
					{
						nXPos=m_ControlPoint.x-80;
						nYPos=m_ControlPoint.y+151;
					}
					else if (0==i)
					{
						nXPos=m_ControlPoint.x-92;
						nYPos=m_ControlPoint.y+172;
					}
					m_ImageHeapDoubleVL.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*i,0,nWidth,nHeight);
				}

				//ͷ���˿�
				if (m_wMinusHeadCount%2!=0)
				{
					if (13==wDoubleHeap-1)
					{
						nXPos=m_ControlPoint.x+2;
						nYPos=m_ControlPoint.y-11;
					}
					else if (12==wDoubleHeap-1)
					{
						nXPos=m_ControlPoint.x-3;
						nYPos=m_ControlPoint.y-1;
					}
					else if (11==wDoubleHeap-1)
					{
						nXPos=m_ControlPoint.x-8;
						nYPos=m_ControlPoint.y+9;
					}
					else if (10==wDoubleHeap-1)
					{
						nXPos=m_ControlPoint.x-13;
						nYPos=m_ControlPoint.y+19;
					}
					else if (9==wDoubleHeap-1)
					{
						nXPos=m_ControlPoint.x-17;
						nYPos=m_ControlPoint.y+29;
					}
					else if (8==wDoubleHeap-1)
					{
						nXPos=m_ControlPoint.x-23;
						nYPos=m_ControlPoint.y+41;
					}
					else if (7==wDoubleHeap-1)
					{
						nXPos=m_ControlPoint.x-30;
						nYPos=m_ControlPoint.y+54;
					}
					else if (6==wDoubleHeap-1)
					{
						nXPos=m_ControlPoint.x-37;
						nYPos=m_ControlPoint.y+67;
					}
					else if (5==wDoubleHeap-1)
					{
						nXPos=m_ControlPoint.x-45;
						nYPos=m_ControlPoint.y+81;
					}
					else if (4==wDoubleHeap-1)
					{
						nXPos=m_ControlPoint.x-52;
						nYPos=m_ControlPoint.y+96;
					}
					else if (3==wDoubleHeap-1)
					{
						nXPos=m_ControlPoint.x-61;
						nYPos=m_ControlPoint.y+113;
					}
					else if (2==wDoubleHeap-1)
					{
						nXPos=m_ControlPoint.x-70;
						nYPos=m_ControlPoint.y+131;
					}
					else if (1==wDoubleHeap-1)
					{
						nXPos=m_ControlPoint.x-80;
						nYPos=m_ControlPoint.y+151;
					}
					else if (0==wDoubleHeap-1)
					{
						nXPos=m_ControlPoint.x-92;
						nYPos=m_ControlPoint.y+172;
					}
					int nWidth=m_ImageHeapSingleVL.GetWidth()/14;
					int nHeight=m_ImageHeapSingleVL.GetHeight();
					m_ImageHeapSingleVL.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(wDoubleHeap-1),0,nWidth,nHeight);
				}
			}

			break;
		}
	case Direction_North:	//����
		{
			//�滭�˿�
			if ((m_wFullCount-m_wMinusHeadCount-m_wMinusLastCount)>0)
			{
				//��������
				int nXPos=0,nYPos=0;
				int wHeapIndex=m_wMinusHeadCount/2;
				int wDoubleHeap=(m_wMinusHeadCount+1)/2;
				int wDoubleLast=(m_wFullCount-m_wMinusLastCount)/2;
				int wFinallyIndex=(m_wFullCount-m_wMinusLastCount)/2;

				//ͷ���˿�
				if (m_wMinusHeadCount%2!=0)
				{
					int nWidth=m_ImageHeapSingleHT.GetWidth()/14;
					int nHeight=m_ImageHeapSingleHT.GetHeight();
					if (wHeapIndex < 7)
					{
						nXPos=m_ControlPoint.x+24*wHeapIndex;
						nYPos=m_ControlPoint.y;
						m_ImageHeapSingleHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*wHeapIndex,0,nWidth,nHeight);
					}

				}

				//�м��˿�
				for (int i=wDoubleHeap;i<7 && i<wFinallyIndex; i++)
				{
					int nWidth=m_ImageHeapDoubleHT.GetWidth()/14;
					int nHeight=m_ImageHeapDoubleHT.GetHeight();
					nXPos=m_ControlPoint.x+24*i;
					nYPos=m_ControlPoint.y;
					m_ImageHeapDoubleHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*i,0,nWidth,nHeight);
				}

				//β���˿�
				if (m_wMinusLastCount%2!=0)
				{
					int nWidth=m_ImageHeapSingleHT.GetWidth()/14;
					int nHeight=m_ImageHeapSingleHT.GetHeight();
					if (wFinallyIndex < 7)
					{
						nXPos=m_ControlPoint.x+24*wFinallyIndex;
						nYPos=m_ControlPoint.y;
					}
					else if (wFinallyIndex > 6)
					{
						nXPos=m_ControlPoint.x+23*wFinallyIndex+7;
						nYPos=m_ControlPoint.y;
					}
					m_ImageHeapSingleHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*wFinallyIndex,0,nWidth,nHeight);
				}

				//�м��˿�
				for (int i=wFinallyIndex - 1;i > 6 && i >=wDoubleHeap ; --i)
				{
					int nWidth=m_ImageHeapDoubleHT.GetWidth()/14;
					int nHeight=m_ImageHeapDoubleHT.GetHeight();
					nXPos=m_ControlPoint.x+23*i+7;
					nYPos=m_ControlPoint.y;

					m_ImageHeapDoubleHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*i,0,nWidth,nHeight);
				}

				//ͷ���˿�
				if (m_wMinusHeadCount%2!=0)
				{
					int nWidth=m_ImageHeapSingleHT.GetWidth()/14;
					int nHeight=m_ImageHeapSingleHT.GetHeight();
					if (wHeapIndex > 6)
					{
						nXPos=m_ControlPoint.x+23*wHeapIndex+7;
						nYPos=m_ControlPoint.y;
						m_ImageHeapSingleHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*wHeapIndex,0,nWidth,nHeight);
					}
				}

			}

			break;
		}
	}
__LEAVE_FUNCTION
	return;
}

bool CHeapCard::SetCardData(WORD wMinusHeadCount, WORD wMinusLastCount, WORD wFullCount)
{
	//���ñ���
	m_wFullCount=wFullCount ;
	m_wMinusHeadCount=wMinusHeadCount;
	m_wMinusLastCount=wMinusLastCount;

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CWeaveCard::CWeaveCard()
{
__ENTER_FUNCTION
	//״̬����
	m_bDisplayItem=false;
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_South;
	m_cbDirectionCardPos = 1;

	//�˿�����
	m_wCardCount=0;
	ZeroMemory(&m_cbCardData,sizeof(m_cbCardData));

	//��������
	TCHAR szResourcePath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,szResourcePath);
	CString strPath(szResourcePath);
	strPath.Append("\\SparrowYY\\Picture\\Card\\");
	//����
	m_ImageUserWeaveCardVL.LoadImage(TEXT(strPath+"USER_TABLE_CARD_VL.png"));
	m_ImageUserWeaveCardVR.LoadImage(TEXT(strPath+"USER_TABLE_CARD_VR.png"));
	m_ImageUserWeaveCardHT.LoadImage(TEXT(strPath+"USER_TABLE_CARD_HT.png"));
	m_ImageUserWeaveCardHB.LoadImage(TEXT(strPath+"USER_TABLE_CARD_HB.png"));
	//������
	m_ImageWeaveCardNumVL.LoadImage(TEXT(strPath+"USER_TABCARDNUM_VL.png"));
	m_ImageWeaveCardNumVR.LoadImage(TEXT(strPath+"USER_TABCARDNUM_VR.png"));
	m_ImageWeaveCardNumHT.LoadImage(TEXT(strPath+"USER_TABCARDNUM_HT.png"));
	m_ImageWeaveCardNumHB.LoadImage(TEXT(strPath+"USER_TABCARDNUM_HB.png"));
	//����
	m_ImageWeaveCardBackVL.LoadImage(TEXT(strPath+"USER_WEAVE_CARD_BACKVL.png"));
	m_ImageWeaveCardBackVR.LoadImage(TEXT(strPath+"USER_WEAVE_CARD_BACKVR.png"));
	m_ImageWeaveCardBackHT.LoadImage(TEXT(strPath+"USER_WEAVE_CARD_BACKHT.png"));
	m_ImageWeaveCardBackHB.LoadImage(TEXT(strPath+"USER_WEAVE_CARD_BACKHB.png"));
__LEAVE_FUNCTION
	return;
}

//��������
CWeaveCard::~CWeaveCard()
{
}

//��ȡλ��
int CWeaveCard::GetImageIndex(BYTE cbCardData)
{
	//�����ж�
	if (cbCardData==0) 
		return 0;

	//����λ��
	BYTE cbValue=cbCardData&MASK_VALUE;
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return (cbColor>=0x03)?(cbValue+27):(cbColor*9+cbValue);
}


//�滭�˿�
void CWeaveCard::DrawCardControl(CDC * pDC)
{
__ENTER_FUNCTION
	//��ʾ�ж�
	if (m_wCardCount==0) 
		return;

	//��������
	int nXScreenPos=0,nYScreenPos=0;
	int nItemWidth=0,nItemHeight=0,nItemWidthEx=0,nItemHeightEx=0;

	//�滭�˿�
	switch (m_CardDirection)
	{
	case Direction_East:	//����
		{
			//��������
			nItemWidth=m_ImageUserWeaveCardVR.GetWidth()/14;
			nItemHeight=m_ImageUserWeaveCardVR.GetHeight();

			int nHeight =  m_ImageWeaveCardNumVR.GetHeight()/27;
			int nImageYPos=0;

			if (0 == m_iIndex)
			{
				//��һ��
				nXScreenPos = m_ControlPoint.x;
				nYScreenPos = m_ControlPoint.y +m_iIndex*23;
				nImageYPos=(GetImageIndex(m_cbCardData[0])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*2,0);
					m_ImageWeaveCardNumVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*2,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*2,0,nItemWidth,nItemHeight);
				}


				//�ڶ���
				nXScreenPos = m_ControlPoint.x+9;
				nYScreenPos = m_ControlPoint.y+(m_iIndex+1)*23-6;
				nImageYPos=(GetImageIndex(m_cbCardData[1])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth,0);
					m_ImageWeaveCardNumVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth,0,nItemWidth,nItemHeight);
				}


				//������
				nXScreenPos = m_ControlPoint.x+21;
				nYScreenPos = m_ControlPoint.y+(m_iIndex+2)*23-11;
				nImageYPos=(GetImageIndex(m_cbCardData[2])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,0,0);
					m_ImageWeaveCardNumVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,0,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,0,0,nItemWidth,nItemHeight);
				}


				if (m_wCardCount==4)
				{
					//������
					nXScreenPos = m_ControlPoint.x+10;
					nYScreenPos = m_ControlPoint.y+(m_iIndex+1)*23-16;
					nImageYPos=(GetImageIndex(m_cbCardData[3])-1)*nHeight;
					if (m_bDisplayItem)
					{
						m_ImageUserWeaveCardVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth,0);
						m_ImageWeaveCardNumVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth,nImageYPos);
					}
					else
					{
						m_ImageWeaveCardBackVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth,0,nItemWidth,nItemHeight);
					}

				}				
			}

			if (1 == m_iIndex)
			{
				//��һ��
				nXScreenPos = m_ControlPoint.x;
				nYScreenPos = m_ControlPoint.y +m_iIndex*23;
				nImageYPos=(GetImageIndex(m_cbCardData[0])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*5,0);
					m_ImageWeaveCardNumVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*5,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*5,0,nItemWidth,nItemHeight);
				}


				//�ڶ���
				nXScreenPos = m_ControlPoint.x+8;
				nYScreenPos = m_ControlPoint.y +(m_iIndex+1)*23-9;
				nImageYPos=(GetImageIndex(m_cbCardData[1])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*4,0);
					m_ImageWeaveCardNumVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*4,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*4,0,nItemWidth,nItemHeight);
				}


				//������
				nXScreenPos = m_ControlPoint.x+17;
				nYScreenPos = m_ControlPoint.y +(m_iIndex+2)*23-17;
				nImageYPos=(GetImageIndex(m_cbCardData[2])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*3,0);
					m_ImageWeaveCardNumVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*3,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*3,0,nItemWidth,nItemHeight);
				}


				if (m_wCardCount==4)
				{
					//������
					nXScreenPos = m_ControlPoint.x+9;
					nYScreenPos = m_ControlPoint.y +(m_iIndex+1)*23-21;
					nImageYPos=(GetImageIndex(m_cbCardData[3])-1)*nHeight;
					if (m_bDisplayItem)
					{
						m_ImageUserWeaveCardVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*4,0);
						m_ImageWeaveCardNumVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*4,nImageYPos);
					}
					else
					{
						m_ImageWeaveCardBackVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*4,0,nItemWidth,nItemHeight);
					}			
				}				
			}

			if (2 == m_iIndex)
			{
				//������
				nXScreenPos = m_ControlPoint.x+2;
				nYScreenPos = m_ControlPoint.y +m_iIndex*23;
				nImageYPos=(GetImageIndex(m_cbCardData[0])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*9,0);
					m_ImageWeaveCardNumVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*9,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*9,0,nItemWidth,nItemHeight);
				}


				//�ڶ���
				nXScreenPos = m_ControlPoint.x+7;
				nYScreenPos = m_ControlPoint.y +(m_iIndex+1)*23-13;
				nImageYPos=(GetImageIndex(m_cbCardData[1])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*8,0);
					m_ImageWeaveCardNumVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*8,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*8,0,nItemWidth,nItemHeight);
				}


				//��һ��
				nXScreenPos = m_ControlPoint.x+14;
				nYScreenPos = m_ControlPoint.y +(m_iIndex+2)*23-26;
				nImageYPos=(GetImageIndex(m_cbCardData[2])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*7,0);
					m_ImageWeaveCardNumVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*7,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*7,0,nItemWidth,nItemHeight);
				}


				if (m_wCardCount==4)
				{
					//������
					nXScreenPos = m_ControlPoint.x+9;
					nYScreenPos = m_ControlPoint.y +(m_iIndex+1)*23-23;
					nImageYPos=(GetImageIndex(m_cbCardData[3])-1)*nHeight;
					if (m_bDisplayItem)
					{
						m_ImageUserWeaveCardVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*8,0);
						m_ImageWeaveCardNumVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*8,nImageYPos);
					}
					else
					{
						m_ImageWeaveCardBackVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*8,0,nItemWidth,nItemHeight);
					}

				}				
			}

			if (3 == m_iIndex)
			{
				//������
				nXScreenPos = m_ControlPoint.x;
				nYScreenPos = m_ControlPoint.y +m_iIndex*23;
				nImageYPos=(GetImageIndex(m_cbCardData[0])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*12,0);
					m_ImageWeaveCardNumVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*12,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*12,0,nItemWidth,nItemHeight);
				}


				//�ڶ���
				nXScreenPos = m_ControlPoint.x+6;
				nYScreenPos = m_ControlPoint.y +(m_iIndex+1)*23-15;
				nImageYPos=(GetImageIndex(m_cbCardData[1])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*11,0);
					m_ImageWeaveCardNumVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*11,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*11,0,nItemWidth,nItemHeight);
				}


				//��һ��
				nXScreenPos = m_ControlPoint.x+11;
				nYScreenPos = m_ControlPoint.y +(m_iIndex+2)*23-30;
				nImageYPos=(GetImageIndex(m_cbCardData[2])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*10,0);
					m_ImageWeaveCardNumVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*10,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*10,0,nItemWidth,nItemHeight);
				}


				if (m_wCardCount==4)
				{
					//������
					nXScreenPos = m_ControlPoint.x+6;
					nYScreenPos = m_ControlPoint.y +(m_iIndex+1)*23-28;
					nImageYPos=(GetImageIndex(m_cbCardData[3])-1)*nHeight;
					if (m_bDisplayItem)
					{
						m_ImageUserWeaveCardVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*11,0);
						m_ImageWeaveCardNumVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*11,nImageYPos);	
					}
					else
					{
						m_ImageWeaveCardBackVR.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*11,0,nItemWidth,nItemHeight);
					}

				}				
			}

			break;
		}
	case Direction_South:	//����
		{
			//��������
			nItemWidth=m_ImageUserWeaveCardHB.GetWidth()/14;
			nItemHeight=m_ImageUserWeaveCardHB.GetHeight();

			//int nWidth = nItemWidth;
			int nHeight =  m_ImageWeaveCardNumHB.GetHeight()/27;
			int nImageYPos=0;

			if (0 == m_iIndex)
			{
				//��һ��
				nXScreenPos = m_ControlPoint.x +m_iIndex*38;
				nYScreenPos = m_ControlPoint.y;
				nImageYPos=(GetImageIndex(m_cbCardData[0])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,0,0);
					m_ImageWeaveCardNumHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,0,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,0,0,nItemWidth,nItemHeight);
				}


				//�ڶ���
				nXScreenPos = m_ControlPoint.x +(m_iIndex+1)*38;
				nYScreenPos = m_ControlPoint.y;
				nImageYPos=(GetImageIndex(m_cbCardData[1])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth,0);
					m_ImageWeaveCardNumHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth,0,nItemWidth,nItemHeight);
				}


				//������
				nXScreenPos = m_ControlPoint.x +(m_iIndex+2)*38;
				nYScreenPos = m_ControlPoint.y;
				nImageYPos=(GetImageIndex(m_cbCardData[2])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*2,0);
					m_ImageWeaveCardNumHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*2,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*2,0,nItemWidth,nItemHeight);
				}


				if (m_wCardCount==4)
				{
					//������
					nXScreenPos = m_ControlPoint.x +(m_iIndex+1)*38-3;
					nYScreenPos = m_ControlPoint.y-20+3;
					nImageYPos=(GetImageIndex(m_cbCardData[3])-1)*nHeight;
					if (m_bDisplayItem)
					{
						m_ImageUserWeaveCardHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth,0);
						m_ImageWeaveCardNumHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth,nImageYPos);
					}
					else
					{
						m_ImageWeaveCardBackHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth,0,nItemWidth,nItemHeight);
					}

				}				
			}

			if (1 == m_iIndex)
			{
				//��һ��
				nXScreenPos = m_ControlPoint.x +m_iIndex*38;
				nYScreenPos = m_ControlPoint.y;
				int nImageYPos=(GetImageIndex(m_cbCardData[0])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*3,0);
					m_ImageWeaveCardNumHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*3,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*3,0,nItemWidth,nItemHeight);
				}


				//�ڶ���
				nXScreenPos = m_ControlPoint.x +(m_iIndex+1)*38;
				nYScreenPos = m_ControlPoint.y;
				nImageYPos=(GetImageIndex(m_cbCardData[1])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*4,0);
					m_ImageWeaveCardNumHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*4,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*4,0,nItemWidth,nItemHeight);
				}


				//������
				nXScreenPos = m_ControlPoint.x +(m_iIndex+2)*38;
				nYScreenPos = m_ControlPoint.y;
				nImageYPos=(GetImageIndex(m_cbCardData[2])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*5,0);
					m_ImageWeaveCardNumHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*5,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*5,0,nItemWidth,nItemHeight);
				}


				if (m_wCardCount==4)
				{
					//������
					nXScreenPos = m_ControlPoint.x +(m_iIndex+1)*38-2;
					nYScreenPos = m_ControlPoint.y-20+3;
					nImageYPos=(GetImageIndex(m_cbCardData[3])-1)*nHeight;
					if (m_bDisplayItem)
					{
						m_ImageUserWeaveCardHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*4,0);
						m_ImageWeaveCardNumHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*4,nImageYPos);
					}
					else
					{
						m_ImageWeaveCardBackHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*4,0,nItemWidth,nItemHeight);
					}

				}				
			}

			if (2 == m_iIndex)
			{
				//������
				nXScreenPos = m_ControlPoint.x +(m_iIndex+2)*38-4;
				nYScreenPos = m_ControlPoint.y;
				nImageYPos=(GetImageIndex(m_cbCardData[0])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*9,0);
					m_ImageWeaveCardNumHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*9,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*9,0,nItemWidth,nItemHeight);
				}


				//�ڶ���
				nXScreenPos = m_ControlPoint.x +(m_iIndex+1)*38-1;
				nYScreenPos = m_ControlPoint.y;
				nImageYPos=(GetImageIndex(m_cbCardData[1])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*8,0);
					m_ImageWeaveCardNumHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*8,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*8,0,nItemWidth,nItemHeight);
				}


				//��һ��
				nXScreenPos = m_ControlPoint.x +m_iIndex*38;
				nYScreenPos = m_ControlPoint.y;
				nImageYPos=(GetImageIndex(m_cbCardData[2])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*7,0);
					m_ImageWeaveCardNumHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*7,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*7,0,nItemWidth,nItemHeight);
				}


				if (m_wCardCount==4)
				{
					//������
					nXScreenPos = m_ControlPoint.x +(m_iIndex+1)*38;
					nYScreenPos = m_ControlPoint.y-20+3;
					nImageYPos=(GetImageIndex(m_cbCardData[3])-1)*nHeight;
					if (m_bDisplayItem)
					{
						m_ImageUserWeaveCardHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*8,0);
						m_ImageWeaveCardNumHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*8,nImageYPos);
					}
					else
					{
						m_ImageWeaveCardBackHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*8,0,nItemWidth,nItemHeight);
					}

				}				
			}

			if (3 == m_iIndex)
			{
				//������
				nXScreenPos = m_ControlPoint.x +(m_iIndex+2)*38-2;
				nYScreenPos = m_ControlPoint.y;
				nImageYPos=(GetImageIndex(m_cbCardData[0])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*12,0);
					m_ImageWeaveCardNumHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*12,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*12,0,nItemWidth,nItemHeight);
				}


				//�ڶ���
				nXScreenPos = m_ControlPoint.x +(m_iIndex+1)*38-1;
				nYScreenPos = m_ControlPoint.y;
				nImageYPos=(GetImageIndex(m_cbCardData[1])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*11,0);
					m_ImageWeaveCardNumHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*11,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*11,0,nItemWidth,nItemHeight);
				}


				//��һ��
				nXScreenPos = m_ControlPoint.x +m_iIndex*38;
				nYScreenPos = m_ControlPoint.y;
				nImageYPos=(GetImageIndex(m_cbCardData[2])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*10,0);
					m_ImageWeaveCardNumHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*10,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*10,0,nItemWidth,nItemHeight);
				}


				if (m_wCardCount==4)
				{
					//������
					nXScreenPos = m_ControlPoint.x +(m_iIndex+1)*38+3;
					nYScreenPos = m_ControlPoint.y-20+3;
					nImageYPos=(GetImageIndex(m_cbCardData[3])-1)*nHeight;
					if (m_bDisplayItem)
					{
						m_ImageUserWeaveCardHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*11,0);
						m_ImageWeaveCardNumHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*11,nImageYPos);
					}
					else
					{
						m_ImageWeaveCardBackHB.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*11,0,nItemWidth,nItemHeight);
					}

				}				
			}

			break;
		}
	case Direction_West:	//����
		{
			//��������
			nItemWidth=m_ImageUserWeaveCardVL.GetWidth()/14;
			nItemHeight=m_ImageUserWeaveCardVL.GetHeight();

			int nHeight =  m_ImageWeaveCardNumVL.GetHeight()/27;
			int nImageYPos=0;

			if (0 == m_iIndex)
			{
				//��һ��
				nXScreenPos = m_ControlPoint.x;
				nYScreenPos = m_ControlPoint.y -(m_iIndex+2)*23;
				nImageYPos=(GetImageIndex(m_cbCardData[0])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*13,0);
					m_ImageWeaveCardNumVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*13,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*13,0,nItemWidth,nItemHeight);
				}


				//�ڶ���
				nXScreenPos = m_ControlPoint.x-6;
				nYScreenPos = m_ControlPoint.y -(m_iIndex+1)*23-14;
				nImageYPos=(GetImageIndex(m_cbCardData[1])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*12,0);
					m_ImageWeaveCardNumVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*12,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*12,0,nItemWidth,nItemHeight);
				}


				//������
				nXScreenPos = m_ControlPoint.x-12;
				nYScreenPos = m_ControlPoint.y -m_iIndex*23-28;
				nImageYPos=(GetImageIndex(m_cbCardData[2])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*11,0);
					m_ImageWeaveCardNumVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*11,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*11,0,nItemWidth,nItemHeight);
				}


				if (m_wCardCount==4)
				{
					//������
					nXScreenPos = m_ControlPoint.x-6;
					nYScreenPos = m_ControlPoint.y -(m_iIndex+1)*23-23;
					nImageYPos=(GetImageIndex(m_cbCardData[3])-1)*nHeight;
					if (m_bDisplayItem)
					{
						m_ImageUserWeaveCardVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*12,0);
						m_ImageWeaveCardNumVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*12,nImageYPos);
					}
					else
					{
						m_ImageWeaveCardBackVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*12,0,nItemWidth,nItemHeight);
					}

				}				
			}

			if (1 == m_iIndex)
			{
				//��һ��
				nXScreenPos = m_ControlPoint.x;
				nYScreenPos = m_ControlPoint.y -(m_iIndex+2)*23;
				nImageYPos=(GetImageIndex(m_cbCardData[0])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*10,0);
					m_ImageWeaveCardNumVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*10,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*10,0,nItemWidth,nItemHeight);
				}


				//�ڶ���
				nXScreenPos = m_ControlPoint.x-6;
				nYScreenPos = m_ControlPoint.y -(m_iIndex+1)*23-13;
				nImageYPos=(GetImageIndex(m_cbCardData[1])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*9,0);
					m_ImageWeaveCardNumVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*9,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*9,0,nItemWidth,nItemHeight);
				}


				//������
				nXScreenPos = m_ControlPoint.x-13;
				nYScreenPos = m_ControlPoint.y -m_iIndex*23-26;
				nImageYPos=(GetImageIndex(m_cbCardData[2])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*8,0);
					m_ImageWeaveCardNumVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*8,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*8,0,nItemWidth,nItemHeight);
				}

				if (m_wCardCount==4)
				{
					//������
					nXScreenPos = m_ControlPoint.x-8;
					nYScreenPos = m_ControlPoint.y -(m_iIndex+1)*23-24;
					nImageYPos=(GetImageIndex(m_cbCardData[3])-1)*nHeight;
					if (m_bDisplayItem)
					{
						m_ImageUserWeaveCardVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*9,0);
						m_ImageWeaveCardNumVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*9,nImageYPos);
					}
					else
					{
						m_ImageWeaveCardBackVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*9,0,nItemWidth,nItemHeight);
					}

				}				
			}

			if (2 == m_iIndex)
			{
				//������
				nXScreenPos = m_ControlPoint.x;
				nYScreenPos = m_ControlPoint.y -(m_iIndex+2)*23;
				nImageYPos=(GetImageIndex(m_cbCardData[0])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*6,0);
					m_ImageWeaveCardNumVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*6,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*6,0,nItemWidth,nItemHeight);
				}


				//�ڶ���
				nXScreenPos = m_ControlPoint.x-8;
				nYScreenPos = m_ControlPoint.y -(m_iIndex+1)*23-10;
				nImageYPos=(GetImageIndex(m_cbCardData[1])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*5,0);
					m_ImageWeaveCardNumVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*5,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*5,0,nItemWidth,nItemHeight);
				}


				//��һ��
				nXScreenPos = m_ControlPoint.x-17;
				nYScreenPos = m_ControlPoint.y -m_iIndex*23-19;
				nImageYPos=(GetImageIndex(m_cbCardData[2])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*4,0);
					m_ImageWeaveCardNumVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*4,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*4,0,nItemWidth,nItemHeight);
				}


				if (m_wCardCount==4)
				{
					//������
					nXScreenPos = m_ControlPoint.x-11;
					nYScreenPos = m_ControlPoint.y -(m_iIndex+1)*23-21;
					nImageYPos=(GetImageIndex(m_cbCardData[3])-1)*nHeight;
					if (m_bDisplayItem)
					{
						m_ImageUserWeaveCardVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*5,0);
						m_ImageWeaveCardNumVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*5,nImageYPos);
					}
					else
					{
						m_ImageWeaveCardBackVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*5,0,nItemWidth,nItemHeight);
					}

				}				
			}

			if (3 == m_iIndex)
			{
				//������
				nXScreenPos = m_ControlPoint.x;
				nYScreenPos = m_ControlPoint.y -(m_iIndex+2)*23;
				nImageYPos=(GetImageIndex(m_cbCardData[0])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*3,0);
					m_ImageWeaveCardNumVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*3,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*3,0,nItemWidth,nItemHeight);
				}


				//�ڶ���
				nXScreenPos = m_ControlPoint.x-9;
				nYScreenPos = m_ControlPoint.y -(m_iIndex+1)*23-8;
				nImageYPos=(GetImageIndex(m_cbCardData[1])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*2,0);
					m_ImageWeaveCardNumVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*2,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*2,0,nItemWidth,nItemHeight);
				}


				//��һ��
				nXScreenPos = m_ControlPoint.x-19;
				nYScreenPos = m_ControlPoint.y -m_iIndex*23-16;
				nImageYPos=(GetImageIndex(m_cbCardData[2])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*1,0);
					m_ImageWeaveCardNumVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*1,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*1,0,nItemWidth,nItemHeight);
				}


				if (m_wCardCount==4)
				{
					//������
					nXScreenPos = m_ControlPoint.x-11;
					nYScreenPos = m_ControlPoint.y -(m_iIndex+1)*23-22;
					nImageYPos=(GetImageIndex(m_cbCardData[3])-1)*nHeight;
					if (m_bDisplayItem)
					{
						m_ImageUserWeaveCardVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*2,0);
						m_ImageWeaveCardNumVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*2,nImageYPos);
					}
					else
					{
						m_ImageWeaveCardBackVL.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*2,0,nItemWidth,nItemHeight);
					}

				}				
			}

			break;
		}
	case Direction_North:	//����
		{
			//��������
			nItemWidth=m_ImageUserWeaveCardHT.GetWidth()/14;
			nItemHeight=m_ImageUserWeaveCardHT.GetHeight();

			int nHeight =  m_ImageWeaveCardNumHT.GetHeight()/27;
			int nImageYPos=0;

			if (0 == m_iIndex)
			{
				//��һ��
				nXScreenPos = m_ControlPoint.x -m_iIndex*23;
				nYScreenPos = m_ControlPoint.y;
				nImageYPos=(GetImageIndex(m_cbCardData[0])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*13,0);
					m_ImageWeaveCardNumHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*13,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*13,0,nItemWidth,nItemHeight);
				}


				//�ڶ���
				nXScreenPos = m_ControlPoint.x -(m_iIndex+1)*23;
				nYScreenPos = m_ControlPoint.y;
				nImageYPos=(GetImageIndex(m_cbCardData[1])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*12,0);
					m_ImageWeaveCardNumHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*12,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*12,0,nItemWidth,nItemHeight);
				}


				//������
				nXScreenPos = m_ControlPoint.x -(m_iIndex+2)*23;
				nYScreenPos = m_ControlPoint.y;
				nImageYPos=(GetImageIndex(m_cbCardData[2])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*11,0);
					m_ImageWeaveCardNumHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*11,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*11,0,nItemWidth,nItemHeight);
				}


				if (m_wCardCount==4)
				{
					//������
					nXScreenPos = m_ControlPoint.x -(m_iIndex+1)*23+1;
					nYScreenPos = m_ControlPoint.y-20+8;
					nImageYPos=(GetImageIndex(m_cbCardData[3])-1)*nHeight;
					if (m_bDisplayItem)
					{
						m_ImageUserWeaveCardHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*12,0);
						m_ImageWeaveCardNumHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*12,nImageYPos);
					}
					else
					{
						m_ImageWeaveCardBackHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*12,0,nItemWidth,nItemHeight);
					}

				}				
			}

			if (1 == m_iIndex)
			{
				//��һ��
				nXScreenPos = m_ControlPoint.x -m_iIndex*23;
				nYScreenPos = m_ControlPoint.y;
				nImageYPos=(GetImageIndex(m_cbCardData[0])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*10,0);
					m_ImageWeaveCardNumHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*10,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*10,0,nItemWidth,nItemHeight);
				}


				//�ڶ���
				nXScreenPos = m_ControlPoint.x -(m_iIndex+1)*23;
				nYScreenPos = m_ControlPoint.y;
				nImageYPos=(GetImageIndex(m_cbCardData[1])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*9,0);
					m_ImageWeaveCardNumHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*9,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*9,0,nItemWidth,nItemHeight);
				}


				//������
				nXScreenPos = m_ControlPoint.x -(m_iIndex+2)*23;
				nYScreenPos = m_ControlPoint.y;
				nImageYPos=(GetImageIndex(m_cbCardData[2])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*8,0);
					m_ImageWeaveCardNumHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*8,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*8,0,nItemWidth,nItemHeight);
				}


				if (m_wCardCount==4)
				{
					//������
					nXScreenPos = m_ControlPoint.x -(m_iIndex+1)*23+1;
					nYScreenPos = m_ControlPoint.y-20+9;
					nImageYPos=(GetImageIndex(m_cbCardData[3])-1)*nHeight;
					if (m_bDisplayItem)
					{
						m_ImageUserWeaveCardHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*9,0);
						m_ImageWeaveCardNumHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*9,nImageYPos);
					}
					else
					{
						m_ImageWeaveCardBackHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*9,0,nItemWidth,nItemHeight);
					}

				}				
			}

			if (2 == m_iIndex)
			{
				//������
				nXScreenPos = m_ControlPoint.x -(m_iIndex+2)*23;
				nYScreenPos = m_ControlPoint.y;
				nImageYPos=(GetImageIndex(m_cbCardData[0])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*4,0);
					m_ImageWeaveCardNumHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*4,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*4,0,nItemWidth,nItemHeight);
				}


				//�ڶ���
				nXScreenPos = m_ControlPoint.x -(m_iIndex+1)*23;
				nYScreenPos = m_ControlPoint.y;
				nImageYPos=(GetImageIndex(m_cbCardData[1])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*5,0);
					m_ImageWeaveCardNumHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*5,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*5,0,nItemWidth,nItemHeight);
				}


				//��һ��
				nXScreenPos = m_ControlPoint.x -m_iIndex*23;
				nYScreenPos = m_ControlPoint.y;
				nImageYPos=(GetImageIndex(m_cbCardData[2])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*6,0);
					m_ImageWeaveCardNumHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*6,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*6,0,nItemWidth,nItemHeight);
				}


				if (m_wCardCount==4)
				{
					//������
					nXScreenPos = m_ControlPoint.x -(m_iIndex+1)*23-1;
					nYScreenPos = m_ControlPoint.y-20+9;
					nImageYPos=(GetImageIndex(m_cbCardData[3])-1)*nHeight;
					if (m_bDisplayItem)
					{
						m_ImageUserWeaveCardHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*5,0);
						m_ImageWeaveCardNumHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*5,nImageYPos);
					}
					else
					{
						m_ImageWeaveCardBackHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*5,0,nItemWidth,nItemHeight);
					}

				}				
			}

			if (3 == m_iIndex)
			{
				//������
				nXScreenPos = m_ControlPoint.x -(m_iIndex+2)*23;
				nYScreenPos = m_ControlPoint.y;
				nImageYPos=(GetImageIndex(m_cbCardData[0])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*1,0);
					m_ImageWeaveCardNumHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*1,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*1,0,nItemWidth,nItemHeight);
				}


				//�ڶ���
				nXScreenPos = m_ControlPoint.x -(m_iIndex+1)*23;
				nYScreenPos = m_ControlPoint.y;
				nImageYPos=(GetImageIndex(m_cbCardData[1])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*2,0);
					m_ImageWeaveCardNumHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*2,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*2,0,nItemWidth,nItemHeight);
				}


				//��һ��
				nXScreenPos = m_ControlPoint.x -m_iIndex*23;
				nYScreenPos = m_ControlPoint.y;
				nImageYPos=(GetImageIndex(m_cbCardData[2])-1)*nHeight;
				if (m_bDisplayItem)
				{
					m_ImageUserWeaveCardHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*3,0);
					m_ImageWeaveCardNumHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*3,nImageYPos);
				}
				else
				{
					m_ImageWeaveCardBackHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*3,0,nItemWidth,nItemHeight);
				}


				if (m_wCardCount==4)
				{
					//������
					nXScreenPos = m_ControlPoint.x -(m_iIndex+1)*23-1;
					nYScreenPos = m_ControlPoint.y-20+9;
					nImageYPos=(GetImageIndex(m_cbCardData[3])-1)*nHeight;
					if (m_bDisplayItem)
					{
						m_ImageUserWeaveCardHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*2,0);
						m_ImageWeaveCardNumHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*2,nImageYPos);
					}
					else
					{
						m_ImageWeaveCardBackHT.DrawImage(pDC,nXScreenPos,nYScreenPos,nItemWidth,nItemHeight,nItemWidth*2,0,nItemWidth,nItemHeight);
					}

				}				
			}

			break;
		}
	}
__LEAVE_FUNCTION
	return;
}

//�����˿�
bool CWeaveCard::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	//Ч���С
	ASSERT(wCardCount<=CountArray(m_cbCardData));
	if (wCardCount>CountArray(m_cbCardData)) return false;

	//�����˿�
	m_wCardCount=wCardCount;
	CopyMemory(m_cbCardData,cbCardData,sizeof(BYTE)*wCardCount);

	return true;
}

//��ȡ�˿�
BYTE CWeaveCard::GetCardData(WORD wIndex)
{
	ASSERT(wIndex<CountArray(m_cbCardData));
	return ((m_bDisplayItem==true)||(wIndex==1))?m_cbCardData[wIndex]:0;
}
//////////////////////////////////////////////////////////////////////////////////////////////

//���캯��
CUserCard::CUserCard()
{
__ENTER_FUNCTION
	//�˿�����
	m_wCardCount=0;
	m_bCurrentCard=false;

	//���Ʊ���
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_East;

	//��������
	TCHAR szResourcePath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,szResourcePath);
	CString strPath(szResourcePath);
	strPath.Append("\\SparrowYY\\Picture\\Card\\");
	m_ImageUserHandVL.LoadImage(TEXT(strPath+"CARD_USER_HANDL.png"));
	m_ImageUserHandVR.LoadImage(TEXT(strPath+"CARD_USER_HANDR.png"));
	m_ImageUserHandHT.LoadImage(TEXT(strPath+"CARD_USER_HANDT.png"));
__LEAVE_FUNCTION
	return;
}

//��������
CUserCard::~CUserCard()
{
}

//�滭�˿�
void CUserCard::DrawCardControl(CDC * pDC)
{
__ENTER_FUNCTION
	switch (m_CardDirection)
	{

	case Direction_East:	//����
		{
			int nWidth=m_ImageUserHandVR.GetWidth()/14;
			int nHeight=m_ImageUserHandVR.GetHeight();

			//��ǰ�˿�
			if (m_bCurrentCard==true)
			{
				int nXPos=m_ControlPoint.x-2;
				int nYPos=m_ControlPoint.y+8;
				m_ImageUserHandVR.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*13,0);
			}

			//�����˿�
			if (m_wCardCount>0)
			{
				int nXPos=0,nYPos=0;
				for (WORD i=0;i<m_wCardCount;i++)
				{
					if (0==i)
					{
						nXPos=m_ControlPoint.x+5+5;
						nYPos=m_ControlPoint.y+27+10;
					}
					else if (1==i)
					{
						nXPos=m_ControlPoint.x+11 +5;
						nYPos=m_ControlPoint.y+35 +10;
					}
					else if (2==i)
					{
						nXPos=m_ControlPoint.x+16 + 2+5;
						nYPos=m_ControlPoint.y+46+10;
					}
					else if (3==i)
					{
						nXPos=m_ControlPoint.x+23 + 2+5;
						nYPos=m_ControlPoint.y+56+10;
					}
					else if (4==i)
					{
						nXPos=m_ControlPoint.x+30+3+5;
						nYPos=m_ControlPoint.y+68+10;
					}
					else if (5==i)
					{
						nXPos=m_ControlPoint.x+37+3+5;
						nYPos=m_ControlPoint.y+81+10;
					}
					else if (6==i)
					{
						nXPos=m_ControlPoint.x+45+4+5;
						nYPos=m_ControlPoint.y+94+10;
					}
					else if (7==i)
					{
						nXPos=m_ControlPoint.x+53+5+5;
						nYPos=m_ControlPoint.y+107+10;
					}
					else if (8==i)
					{
						nXPos=m_ControlPoint.x+61+6+5;
						nYPos=m_ControlPoint.y+120+10;
					}
					else if (9==i)
					{
						nXPos=m_ControlPoint.x+70+6+5;
						nYPos=m_ControlPoint.y+155-21+10;
					}
					else if (10==i)
					{
						nXPos=m_ControlPoint.x+80+6+5;
						nYPos=m_ControlPoint.y+169-20+10;
					}
					else if (11==i)
					{
						nXPos=m_ControlPoint.x+91+5+5;
						nYPos=m_ControlPoint.y+193-28+10;
					}
					if (12==i)
					{
						nXPos=m_ControlPoint.x+102+5+5;
						nYPos=m_ControlPoint.y+213-32+10;
					}
					m_ImageUserHandVR.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(12-i),0);
				}
			}




			break;
		}

	case Direction_West:	//����
		{
			int nWidth=m_ImageUserHandVL.GetWidth()/14;
			int nHeight=m_ImageUserHandVL.GetHeight();
			//�����˿�
			if (m_wCardCount>0)
			{
				int nXPos=0,nYPos=0;
				int num=13-m_wCardCount;
				for (WORD i=num;i<13;++i)
				{
					if (0==i)
					{
						nXPos=m_ControlPoint.x+80;
						nYPos=m_ControlPoint.y;
					}
					else if (1==i)
					{
						nXPos=m_ControlPoint.x-7+80+2;
						nYPos=m_ControlPoint.y+12-2;
					}
					else if (2==i)
					{
						nXPos=m_ControlPoint.x-14+80+3;
						nYPos=m_ControlPoint.y+24-4;
					}
					else if (3==i)
					{
						nXPos=m_ControlPoint.x-20+80+4;
						nYPos=m_ControlPoint.y+37-8+1;
					}
					else if (4==i)
					{
						nXPos=m_ControlPoint.x-27+80+4;
						nYPos=m_ControlPoint.y+50-12+1+1;
					}
					else if (5==i)
					{
						nXPos=m_ControlPoint.x-34+80+5;
						nYPos=m_ControlPoint.y+65-16+1+1;
					}
					else if (6==i)
					{
						nXPos=m_ControlPoint.x-43+80+7;
						nYPos=m_ControlPoint.y+80-20+3;
					}
					if (7==i)
					{
						nXPos=m_ControlPoint.x-51+80+7;
						nYPos=m_ControlPoint.y+97-24+3;
					}
					else if (8==i)
					{
						nXPos=m_ControlPoint.x-60+80+8;
						nYPos=m_ControlPoint.y+115-28+1+1;
					}
					else if (9==i)
					{
						nXPos=m_ControlPoint.x-70+90;
						nYPos=m_ControlPoint.y+133-33+1+1;
					}
					else if (10==i)
					{
						nXPos=m_ControlPoint.x-80+91;
						nYPos=m_ControlPoint.y+152-37+1+1;
					}
					else if (11==i)
					{
						nXPos=m_ControlPoint.x-90+91;
						nYPos=m_ControlPoint.y+172-42+1+1;
					}
					else if (12==i)
					{
						nXPos=m_ControlPoint.x-9;
						nYPos=m_ControlPoint.y+148;
					}
					m_ImageUserHandVL.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(13-i),0);
				}
			}

			//��ǰ�˿�
			if (m_bCurrentCard==true)
			{
				int nXPos=m_ControlPoint.x-25;
				int nYPos=m_ControlPoint.y+180;
				m_ImageUserHandVL.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,0,0);
			}
			break;
		}

	case Direction_North:	//����
		{
			int nWidth=m_ImageUserHandHT.GetWidth()/14;
			int nHeight=m_ImageUserHandHT.GetHeight();

			//�����˿�
			int count = m_wCardCount;
			if (count>0)
			{
				int nXPos=0,nYPos=0;
				if (count>6)
				{
					for (int i=count;i > 6;--i)
					{
						nYPos=m_ControlPoint.y;
						nXPos=m_ControlPoint.x+i*22+34;
						m_ImageUserHandHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*i,0);
					}
					count -= (count-6);
				}

				if (count <7)
				{
					for (int i=0;i < count;++i)
					{
						nYPos=m_ControlPoint.y;
						nXPos=m_ControlPoint.x+i*23+40+10;
						m_ImageUserHandHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*i,0);
					}
				}
			}

			//��ǰ�˿�
			if (m_bCurrentCard==true)
			{
				int nXPos=m_ControlPoint.x+10;
				int nYPos=m_ControlPoint.y;
				m_ImageUserHandHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth,0);
			}

			break;
		}

		return;
	}
__LEAVE_FUNCTION
}

//�����˿�
bool CUserCard::SetCurrentCard(bool bCurrentCard)
{
	//���ñ���
	m_bCurrentCard=bCurrentCard;

	return true;
}

//�����˿�
bool CUserCard::SetCardData(WORD wCardCount, bool bCurrentCard)
{
	//���ñ���
	m_wCardCount=wCardCount;
	m_bCurrentCard=bCurrentCard;

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CDiscardCard::CDiscardCard()
{
__ENTER_FUNCTION
	//�˿�����
	m_wCardCount=0;
	ZeroMemory(m_cbCardData,sizeof(m_cbCardData));

	//���Ʊ���
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_East;

	//��������
	TCHAR szResourcePath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,szResourcePath);
	CString strPath(szResourcePath);
	strPath.Append("\\SparrowYY\\Picture\\Card\\");
	m_ImageUserDisCardHB.LoadImage(TEXT(strPath+"USER_DIS_CARD_HB.png"));
	m_ImageUserDisCardHT.LoadImage(TEXT(strPath+"USER_DIS_CARD_HT.png"));
	m_ImageUserDisCardVL.LoadImage(TEXT(strPath+"USER_DIS_CARD_VL.png"));
	m_ImageUserDisCardVR.LoadImage(TEXT(strPath+"USER_DIS_CARD_VR.png"));
	m_ImageDisCardNumVL.LoadImage(TEXT(strPath+"USER_DISCARDNUM_VL.png"));
	m_ImageDisCardNumVR.LoadImage(TEXT(strPath+"USER_DISCARDNUM_VR.png"));
	m_ImageDisCardNumHT.LoadImage(TEXT(strPath+"USER_DISCARDNUM_HT.png"));
	m_ImageDisCardNumHB.LoadImage(TEXT(strPath+"USER_DISCARDNUM_HB.png"));

__LEAVE_FUNCTION
	return;
}

//��������
CDiscardCard::~CDiscardCard()
{
}

//��ȡλ��
int CDiscardCard::GetImageIndex(BYTE cbCardData)
{
	//�����ж�
	if (cbCardData==0) 
		return 0;

	//����λ��
	BYTE cbValue=cbCardData&MASK_VALUE;
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return (cbColor>=0x03)?(cbValue+27):(cbColor*9+cbValue);
}



//�滭�˿�
void CDiscardCard::DrawCardControl(CDC * pDC)
{
__ENTER_FUNCTION
	//�滭����
	switch (m_CardDirection)
	{
	case Direction_East:	//����
		{
			int nWidth=m_ImageUserDisCardVR.GetWidth()/18;
			int nHeight=m_ImageUserDisCardVR.GetHeight();
			int nItemWidth = nWidth;
			int nItemHeight =  m_ImageDisCardNumVR.GetHeight()/27;

			int nXPos=0,nYPos=0;
			//�滭�˿�
			int count1 = 0,count2=0,count3=0;
			if(m_wCardCount>12)
			{
				count1=6;
				count2=12;
				count3=m_wCardCount;
			}
			if (m_wCardCount > 6 && m_wCardCount<13)
			{
				count1 = 6;
				count2 = m_wCardCount;
			}
			else if (m_wCardCount < 7)
			{
				count1 = m_wCardCount ;
			}

			//��һ��
			if (count1<7)
			{
				for (int i=count1-1;i>=0;--i)
				{
					if (0 == i)
					{
						nXPos=m_ControlPoint.x-((17-i)/6)*29+42+5;
						nYPos=m_ControlPoint.y+((17-i)%6)*24-50;
					}
					else if (1 == i)
					{
						nXPos=m_ControlPoint.x-((17-i)/6)*29+33+5;
						nYPos=m_ControlPoint.y+((17-i)%6)*24-50-2+5;
					}
					else if (2 == i)
					{
						nXPos=m_ControlPoint.x-((17-i)/6)*29+24+5;
						nYPos=m_ControlPoint.y+((17-i)%6)*24-50-3+10;
					}
					else if (3 == i)
					{
						nXPos=m_ControlPoint.x-((17-i)/6)*29+16+5;
						nYPos=m_ControlPoint.y+((17-i)%6)*24-50-3+15;
					}
					else if (4 == i)
					{
						nXPos=m_ControlPoint.x-((17-i)/6)*29+8+6;
						nYPos=m_ControlPoint.y+((17-i)%6)*24-50-1+20;
					}
					else if (5 == i)
					{
						nXPos=m_ControlPoint.x-((17-i)/6)*29+7;
						nYPos=m_ControlPoint.y+((17-i)%6)*24-49+26;
					}
					//�滭����
					int nImageXPos=i*nItemWidth;
					int nImageYPos=(GetImageIndex(m_cbCardData[i])-1)*nItemHeight;
					m_ImageUserDisCardVR.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*i,0);
					m_ImageDisCardNumVR.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,nImageYPos);					
				}
			}
			//�ڶ���
			if(6<count2 && count2<13)
			{
				for (int i=count2-1;i>=6;--i)
				{
					if (6 == i)
					{
						nXPos=m_ControlPoint.x-((17-i)/6)*29+42+5-60+4;
						nYPos=m_ControlPoint.y+((17-i)%6)*24-50;
					}
					else if (7 == i)
					{
						nXPos=m_ControlPoint.x-((17-i)/6)*29+33+4-59+6;
						nYPos=m_ControlPoint.y+((17-i)%6)*24-50-2+5;
					}
					else if (8 == i)
					{
						nXPos=m_ControlPoint.x-((17-i)/6)*29+24+3-58+7;
						nYPos=m_ControlPoint.y+((17-i)%6)*24-50-3+10;
					}
					else if (9 == i)
					{
						nXPos=m_ControlPoint.x-((17-i)/6)*29+16+2-57+8;
						nYPos=m_ControlPoint.y+((17-i)%6)*24-50-3+15;
					}
					else if (10 == i)
					{
						nXPos=m_ControlPoint.x-((17-i)/6)*29+8+1-56+10;
						nYPos=m_ControlPoint.y+((17-i)%6)*24-50-1+20;
					}
					else if (11 == i)
					{
						nXPos=m_ControlPoint.x-((17-i)/6)*29-55+11;
						nYPos=m_ControlPoint.y+((17-i)%6)*24-49+26;
					}
					//�滭����
					int nImageXPos=i*nItemWidth;
					int nImageYPos=(GetImageIndex(m_cbCardData[i])-1)*nItemHeight;
					m_ImageUserDisCardVR.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*i,0);
					m_ImageDisCardNumVR.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,nImageYPos);					
				}
			}
			////������
			if (count3 > 12)
			{
				for (int i=count3-1;i>=12;--i)
				{
					if (12 == i)
					{
						nXPos=m_ControlPoint.x-((17-i)/6)*29+42+5-61-59+10;
						nYPos=m_ControlPoint.y+((17-i)%6)*24-50;
					}
					else if (13 == i)
					{
						nXPos=m_ControlPoint.x-((17-i)/6)*29+33+4-60-58+12;
						nYPos=m_ControlPoint.y+((17-i)%6)*24-50-2+5;
					}
					else if (14 == i)
					{
						nXPos=m_ControlPoint.x-((17-i)/6)*29+24+3-59-57+13;
						nYPos=m_ControlPoint.y+((17-i)%6)*24-50-3+10;
					}
					else if (15 == i)
					{
						nXPos=m_ControlPoint.x-((17-i)/6)*29+16+2-58-56+13;
						nYPos=m_ControlPoint.y+((17-i)%6)*24-50-3+15;
					}
					else if (16 == i)
					{
						nXPos=m_ControlPoint.x-((17-i)/6)*29+8+1-57-55+15;
						nYPos=m_ControlPoint.y+((17-i)%6)*24-50-1+20;
					}
					else if (17 == i)
					{
						nXPos=m_ControlPoint.x-((17-i)/6)*29-55-54+16;
						nYPos=m_ControlPoint.y+((17-i)%6)*24-49+26;
					}
					//�滭����
					int nImageXPos=i*nItemWidth;
					int nImageYPos=(GetImageIndex(m_cbCardData[i])-1)*nItemHeight;
					m_ImageUserDisCardVR.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*i,0);
					m_ImageDisCardNumVR.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,nImageYPos);					
				}
			}



			break;
		}
	case Direction_South:	//����
		{//�滭�˿�
			int nWidth=m_ImageUserDisCardHB.GetWidth()/20;
			int nHeight=m_ImageUserDisCardHB.GetHeight();

			int nItemWidth = nWidth;
			int nItemHeight =  m_ImageDisCardNumHB.GetHeight()/27;
			//�滭����
			int count = m_wCardCount;
			if (count > 15) //�ڶ����������
			{
				int num = count - 15;
				count -= num;
				for(int i = num; i>0; --i)
				{
					int nXPos=m_ControlPoint.x+(i%10)*28+90;
					int nYPos=m_ControlPoint.y-24;
					int nImageXPos=(count + i - 1)*nItemWidth;
					int nImageYPos=(GetImageIndex(m_cbCardData[i+14])-1)*nItemHeight;
					m_ImageUserDisCardHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,0);
					m_ImageDisCardNumHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,nImageYPos);

					//m_ImageUserDisCardHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(count + i - 1),0);
					//m_ImageDisCardNumHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(count + i - 1),0);
				}

			}

			if (count > 10)//�ڶ���ǰ������
			{
				int num = count %10;
				count -= num;
				for (int i=0; i<num; ++i)
				{
					int nXPos=m_ControlPoint.x+(i%10)*28-24;
					int nYPos=m_ControlPoint.y-24;
					int nImageXPos=(count + i)*nItemWidth;
					int nImageYPos=(GetImageIndex(m_cbCardData[i+10])-1)*nItemHeight;
					m_ImageUserDisCardHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,0);
					m_ImageDisCardNumHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,nImageYPos);

					//m_ImageUserDisCardHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(count + i),0);
					//m_ImageDisCardNumHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(count + i),0);
					//DrawCardItem(pDC,m_cbCardData[count + i],nXPos,nYPos);
				}
			}

			if (count > 5) //��һ���������
			{
				int num = count - 5;
				count -= num;
				for(int i = num; i>0; --i)
				{
					int nXPos=m_ControlPoint.x+(i%10)*29+90;
					int nYPos=m_ControlPoint.y+3;
					int nImageXPos=(count + i -1)*nItemWidth;
					int nImageYPos=(GetImageIndex(m_cbCardData[i+4])-1)*nItemHeight;
					m_ImageUserDisCardHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,0);
					m_ImageDisCardNumHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,nImageYPos);

					//m_ImageUserDisCardHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(count + i - 1),0);
					//m_ImageDisCardNumHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(count + i - 1),0);
					//DrawCardItem(pDC,m_cbCardData[count + i-1],nXPos,nYPos);
				}
			}

			if (count > 0)//��һ��ǰ������
			{
				int num = count %10;
				count -= num;
				for (int i=0; i<num; ++i)
				{
					int nXPos=m_ControlPoint.x+(i%10)*30-32;
					int nYPos=m_ControlPoint.y+3;
					int nImageXPos=(count + i)*nItemWidth;
					int nImageYPos=(GetImageIndex(m_cbCardData[i])-1)*nItemHeight;
					m_ImageUserDisCardHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,0);
					m_ImageDisCardNumHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,nImageYPos);

					//m_ImageUserDisCardHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(count + i),0);
					//m_ImageDisCardNumHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(count + i),0);
					//DrawCardItem(pDC,m_cbCardData[count + i],nXPos,nYPos);
				}
			}

			break;
		}
	case Direction_West:	//����
		{
			int nWidth=m_ImageUserDisCardVL.GetWidth()/18;
			int nHeight=m_ImageUserDisCardVL.GetHeight();

			int nItemWidth = nWidth;
			int nItemHeight = m_ImageDisCardNumVL.GetHeight()/27;

			//�滭�˿�
			for (WORD i=0;i<m_wCardCount;i++)
			{
				int nXPos=0,nYPos=0;
				if (0 == i)
				{
					nXPos=m_ControlPoint.x-((17-i)/6)*29-(i%6)*11;
					nYPos=m_ControlPoint.y-((17-i)%6)*24+(i%6)*11;
				}
				else if (1 == i)
				{
					nXPos=m_ControlPoint.x-((17-i)/6)*29-(i%6)*11+2-1+2;
					nYPos=m_ControlPoint.y-((17-i)%6)*24+(i%6)*11-8-6-5;
				}
				else if (2 == i)
				{
					nXPos=m_ControlPoint.x-((17-i)/6)*29-(i%6)*11+4-2+5;
					nYPos=m_ControlPoint.y-((17-i)%6)*24+(i%6)*11-15-12-10;
				}
				else if (3 == i)
				{
					nXPos=m_ControlPoint.x-((17-i)/6)*29-(i%6)*11+4-3+8;
					nYPos=m_ControlPoint.y-((17-i)%6)*24+(i%6)*11-21-15-18;
				}
				else if (4 == i)
				{
					nXPos=m_ControlPoint.x-((17-i)/6)*29-(i%6)*11+4-4+11;
					nYPos=m_ControlPoint.y-((17-i)%6)*24+(i%6)*11-26-19-25;
				}
				else if (5 == i)
				{
					nXPos=m_ControlPoint.x-((17-i)/6)*29-(i%6)*11+4-6+14;
					nYPos=m_ControlPoint.y-((17-i)%6)*24+(i%6)*11-30-20-33;
				}


				else if (6 == i)
				{
					nXPos=m_ControlPoint.x-((17-i)/6)*29-3-4;
					nYPos=m_ControlPoint.y-((17-i)%6)*24;
				}
				else if (7 == i)
				{
					nXPos=m_ControlPoint.x-((17-i)/6)*29-13;
					nYPos=m_ControlPoint.y-((17-i)%6)*24-3-6;
				}
				else if (8 == i)
				{
					nXPos=m_ControlPoint.x-((17-i)/6)*29-20;
					nYPos=m_ControlPoint.y-((17-i)%6)*24-5-11;
				}
				else if (9 == i)
				{
					nXPos=m_ControlPoint.x-((17-i)/6)*29-29;
					nYPos=m_ControlPoint.y-((17-i)%6)*24-3-19;
				}
				else if (10 == i)
				{
					nXPos=m_ControlPoint.x-((17-i)/6)*29-36;
					nYPos=m_ControlPoint.y-((17-i)%6)*24-26;
				}
				else if (11 == i)
				{
					nXPos=m_ControlPoint.x-((17-i)/6)*29-46;
					nYPos=m_ControlPoint.y-((17-i)%6)*24+5-33;
				}



				else if (12 == i)
				{
					nXPos=m_ControlPoint.x-((17-i)/6)*29-15;
					nYPos=m_ControlPoint.y-((17-i)%6)*24;
				}
				else if (13 == i)
				{
					nXPos=m_ControlPoint.x-((17-i)/6)*29-20;
					nYPos=m_ControlPoint.y-((17-i)%6)*24-9;
				}
				else if (14 == i)
				{
					nXPos=m_ControlPoint.x-((17-i)/6)*29-26;
					nYPos=m_ControlPoint.y-((17-i)%6)*24-16;
				}
				else if (15 == i)
				{
					nXPos=m_ControlPoint.x-((17-i)/6)*29-33;
					nYPos=m_ControlPoint.y-((17-i)%6)*24-22;
				}
				else if (16 == i)
				{
					nXPos=m_ControlPoint.x-((17-i)/6)*29-40;
					nYPos=m_ControlPoint.y-((17-i)%6)*24-25;
				}
				else if (17 == i)
				{
					nXPos=m_ControlPoint.x-((17-i)/6)*29-48;
					nYPos=m_ControlPoint.y-((17-i)%6)*24-28;
				}
				//�滭����
				int nImageXPos=i*nItemWidth;
				int nImageYPos=(GetImageIndex(m_cbCardData[i])-1)*nItemHeight;
				m_ImageUserDisCardVL.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*i,0);
				m_ImageDisCardNumVL.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,nImageYPos);
			}

			break;
		}
	case Direction_North:	//����
		{//�滭�˿�
			int nWidth=m_ImageUserDisCardHT.GetWidth()/20;
			int nHeight=m_ImageUserDisCardHT.GetHeight();

			int nItemWidth = nWidth;
			int nItemHeight =  m_ImageDisCardNumHT.GetHeight()/27;

			//�滭����
			int count = 0;
			if (m_wCardCount > 10)
			{
				count = 10;
			}else
			{
				count = m_wCardCount ;
			}

			if (count < 11 && count >5)//��һ��ǰ������
			{
				for (int i=count; i > 5; --i)
				{
					int nXPos=m_ControlPoint.x-i*23+205;
					int nYPos=m_ControlPoint.y;
					int nImageXPos=(10-i)*nItemWidth;
					int nImageYPos=(GetImageIndex(m_cbCardData[i-1])-1)*nItemHeight;
					m_ImageUserDisCardHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(10-i),0);
					m_ImageDisCardNumHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,nImageYPos);
				}
				count-=(count - 5);
			}

			if (count < 6) //��һ���������
			{
				for(int i = 0; i < count; ++i)
				{
					int nXPos=m_ControlPoint.x-i*23+180;
					int nYPos=m_ControlPoint.y;
					int nImageXPos=(9-i)*nItemWidth;
					int nImageYPos=(GetImageIndex(m_cbCardData[i])-1)*nItemHeight;
					m_ImageUserDisCardHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(9-i),0);
					m_ImageDisCardNumHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,nImageYPos);
				}
			}


			if (m_wCardCount>10)
			{
				count = m_wCardCount;

				if (count < 21 && count>15)//�ڶ���ǰ������
				{
					for (int i=count; i>15; --i)
					{
						int nXPos=m_ControlPoint.x-i*23+433;
						int nYPos=m_ControlPoint.y+23;
						int nImageXPos=(29-i)*nItemWidth;
						int nImageYPos=(GetImageIndex(m_cbCardData[i-1])-1)*nItemHeight;
						m_ImageUserDisCardHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(29-i),0);
						m_ImageDisCardNumHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,nImageYPos);
					}
					count-=(count-15);
				}

				if (count < 16) //�ڶ����������
				{
					for(int i = 10; i<count; ++i)
					{
						int nXPos=m_ControlPoint.x-i*24+421;
						int nYPos=m_ControlPoint.y+23;
						int nImageXPos=(29-i)*nItemWidth;
						int nImageYPos=(GetImageIndex(m_cbCardData[i])-1)*nItemHeight;
						m_ImageUserDisCardHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(29-i),0);
						m_ImageDisCardNumHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,nImageYPos);
					}
				}
			}


			/*
			//�滭�˿�
			for (WORD i=0;i<m_wCardCount;i++)
			{
			int nXPos=m_ControlPoint.x+((m_wCardCount-1-i)%8)*30+25;
			int nYPos=m_ControlPoint.y-((m_wCardCount-1-i)/8)*35+105;
			int nWidth=m_ImageUserDisCardHT.GetWidth()/20;
			int nHeight=m_ImageUserDisCardHT.GetHeight();
			//�滭����
			m_ImageUserDisCardHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(12-i),0);
			}
			*/
			break;
		}
	}
__LEAVE_FUNCTION
	return;
}

//�����˿�
bool CDiscardCard::AddCardItem(BYTE cbCardData)
{
	////�����˿�
	//if (m_wCardCount>=CountArray(m_cbCardData))
	//{
	//	m_wCardCount--;
	//	MoveMemory(m_cbCardData,m_cbCardData+1,CountArray(m_cbCardData)-1);
	//}

	//�����˿�
	m_cbCardData[m_wCardCount++]=cbCardData;

	return true;
}

//�����˿�
bool CDiscardCard::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	//Ч���С
	ASSERT(wCardCount<=CountArray(m_cbCardData));
	if (wCardCount>CountArray(m_cbCardData))
		return false;

	//�����˿�
	::memset(m_cbCardData, 0, sizeof(m_cbCardData));
	m_wCardCount=wCardCount;
	CopyMemory(m_cbCardData,cbCardData,sizeof(m_cbCardData[0])*wCardCount);

	return true;
}


//////////////////////////////////////////////////////////////////////////

//���캯��
CTableCard::CTableCard()
{
__ENTER_FUNCTION
	//�˿�����
	m_wCardCount=0;
	ZeroMemory(m_cbCardData,sizeof(m_cbCardData));

	//���Ʊ���
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_East;

	//ͼƬ��Դ
	TCHAR szResourcePath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,szResourcePath);
	CString strPath(szResourcePath);
	strPath.Append("\\SparrowYY\\Picture\\Card\\");
	m_ImageUserTableCardHB.LoadImage(TEXT(strPath+"USER_TABLE_CARD_HB.png"));
	m_ImageUserTableCardHT.LoadImage(TEXT(strPath+"USER_TABLE_CARD_HT.png"));
	m_ImageUserTableCardVL.LoadImage(TEXT(strPath+"USER_TABLE_CARD_VL.png"));
	m_ImageUserTableCardVR.LoadImage(TEXT(strPath+"USER_TABLE_CARD_VR.png"));

	m_ImageTabCardNumVL.LoadImage(TEXT(strPath+"USER_TABCARDNUM_VL.png"));
	m_ImageTabCardNumVR.LoadImage(TEXT(strPath+"USER_TABCARDNUM_VR.png"));
	m_ImageTabCardNumHT.LoadImage(TEXT(strPath+"USER_TABCARDNUM_HT.png"));
	m_ImageTabCardNumHB.LoadImage(TEXT(strPath+"USER_TABCARDNUM_HB.png"));

__LEAVE_FUNCTION
	return;
}

//��������
CTableCard::~CTableCard()
{
}



//��ȡλ��
int CTableCard::GetImageIndex(BYTE cbCardData)
{
	//�����ж�
	if (cbCardData==0) 
		return 0;

	//����λ��
	BYTE cbValue=cbCardData&MASK_VALUE;
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return (cbColor>=0x03)?(cbValue+27):(cbColor*9+cbValue);
}

//�滭�˿�
void CTableCard::DrawCardControl(CDC * pDC)
{
__ENTER_FUNCTION
	//�滭����
	switch (m_CardDirection)
	{
	case Direction_East:	//����
		{
			//�滭�˿�
			int nWidth=m_ImageUserTableCardVR.GetWidth()/14;
			int nHeight=m_ImageUserTableCardVR.GetHeight();
			int nItemWidth = nWidth;
			int nItemHeight =  m_ImageTabCardNumVR.GetHeight()/27;

			int num=0;
			if (2 == m_wCardCount%3)  //����
			{
				num=m_wCardCount-1;
			}
			else if(1 == m_wCardCount%3) //����
			{
				num=m_wCardCount;
			}
			for (WORD i=0;i<=num;i++)
			{
				int nXPos=0,nYPos=0;				
				if (i ==0 &&  2 == (m_wCardCount%3))
				{//����
					nXPos=m_ControlPoint.x-32-7*(13-i);
					nYPos=m_ControlPoint.y+i*18;
					int nImageXPos=13*nItemWidth;
					int nImageYPos=(GetImageIndex(m_cbCardData[m_wCardCount-1])-1)*nItemHeight;
					m_ImageUserTableCardVR.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,0);
					m_ImageTabCardNumVR.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,nImageYPos);
				}
				if(0!=i)
				{
					if (1 == i)
					{
						nXPos=m_ControlPoint.x-33-7*(13-i)-1;
						nYPos=m_ControlPoint.y+i*18-9;
					}
					else if (2 == i)
					{
						nXPos=m_ControlPoint.x-33-7*(13-i)-1;
						nYPos=m_ControlPoint.y+i*18-17;
					}
					else if (3 == i)
					{
						nXPos=m_ControlPoint.x-33-7*(13-i)-2;
						nYPos=m_ControlPoint.y+i*18-25;
					}
					else if (4 == i)
					{
						nXPos=m_ControlPoint.x-33-7*(13-i)-2;
						nYPos=m_ControlPoint.y+i*18-33;
					}
					else if (5 == i)
					{
						nXPos=m_ControlPoint.x-33-7*(13-i)-2;
						nYPos=m_ControlPoint.y+i*18-40;
					}
					else if (6 == i)
					{
						nXPos=m_ControlPoint.x-33-7*(13-i)-1;
						nYPos=m_ControlPoint.y+i*18-47;
					}
					else if (7 == i)
					{
						nXPos=m_ControlPoint.x-33-7*(13-i)-2;
						nYPos=m_ControlPoint.y+i*18-53;
					}
					else if (8 == i)
					{
						nXPos=m_ControlPoint.x-33-7*(13-i);
						nYPos=m_ControlPoint.y+i*18-58;
					}
					else if (9 == i)
					{
						nXPos=m_ControlPoint.x-33-7*(13-i)+2;
						nYPos=m_ControlPoint.y+i*18-62;
					}
					else if (10 == i)
					{
						nXPos=m_ControlPoint.x-33-7*(13-i)+5;
						nYPos=m_ControlPoint.y+i*18-65;
					}
					else if (11 == i)
					{
						nXPos=m_ControlPoint.x-33-7*(13-i)+9;
						nYPos=m_ControlPoint.y+i*18-67;
					}
					else if (12 == i)
					{
						nXPos=m_ControlPoint.x-33-7*(13-i)+11;
						nYPos=m_ControlPoint.y+i*18-69;
					}
					else if (13 == i)
					{
						nXPos=m_ControlPoint.x-33-7*(13-i)+17;
						nYPos=m_ControlPoint.y+i*18-69;
					}

					int nImageXPos=(13 - i)*nItemWidth;
					int nImageYPos=(GetImageIndex(m_cbCardData[num-i])-1)*nItemHeight;
					m_ImageUserTableCardVR.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,0);
					m_ImageTabCardNumVR.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,nImageYPos);
				}

			}

			break;
		}
	case Direction_South:	//����
		{//�滭�˿�
			int nWidth=m_ImageUserTableCardHB.GetWidth()/14;
			int nHeight=m_ImageUserTableCardHB.GetHeight();

			int nItemWidth = nWidth;
			int nItemHeight =  m_ImageTabCardNumHB.GetHeight()/27;
			//�滭����
			int count = m_wCardCount;
			if (2 == count%3)                  //����14
			{
				if (count > 7) //ǰ����
				{
					for(int i = count; i>7; --i)
					{
						int nXPos=m_ControlPoint.x-i*34+204;
						int nYPos=m_ControlPoint.y;
						int nImageXPos=(14 - i)*nItemWidth;
						int nImageYPos=(GetImageIndex(m_cbCardData[count-i])-1)*nItemHeight;
						m_ImageUserTableCardHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,0);
						m_ImageTabCardNumHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,nImageYPos);
					}
					count -= (count -7);
				}

				if (count > 0)//������
				{
					for (int i=0; i<count; ++i)
					{
						int nXPos=m_ControlPoint.x-i*34+170;
						int nYPos=m_ControlPoint.y;
						int nImageXPos=(13 - i)*nItemWidth;
						int nImageYPos=(GetImageIndex(m_cbCardData[m_wCardCount-i-1])-1)*nItemHeight;
						m_ImageUserTableCardHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,0);
						m_ImageTabCardNumHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,nImageYPos);
					}
				}
			}
			else  if (1 == count%3)             //û��13
			{
				if (count > 6) //ǰ����
				{
					for(int i = count; i>6; --i)
					{
						int nXPos=m_ControlPoint.x-(i+1)*34+204;
						int nYPos=m_ControlPoint.y;
						int nImageXPos=(13 - i)*nItemWidth;
						int nImageYPos=(GetImageIndex(m_cbCardData[count-i])-1)*nItemHeight;
						m_ImageUserTableCardHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,0);
						m_ImageTabCardNumHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,nImageYPos);
					}
					count -= (count -6);
				}

				if (count > 0)//������
				{
					for (int i=0; i<count; ++i)
					{
						int nXPos=m_ControlPoint.x-(i+1)*34+170;
						int nYPos=m_ControlPoint.y;
						int nImageXPos=(12 - i)*nItemWidth;
						int nImageYPos=(GetImageIndex(m_cbCardData[m_wCardCount-i-1])-1)*nItemHeight;
						m_ImageUserTableCardHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,0);
						m_ImageTabCardNumHB.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,nImageYPos);
					}
				}
			}

			break;
		}
	case Direction_West:	//����
		{
			//�滭�˿�
			int nWidth=m_ImageUserTableCardVL.GetWidth()/14;
			int nHeight=m_ImageUserTableCardVL.GetHeight();

			int nItemWidth = nWidth;
			int nItemHeight =  m_ImageTabCardNumVL.GetHeight()/27;

			int num=0;
			if (2 == m_wCardCount%3)  //����
			{
				num=m_wCardCount-1;
			}
			else if(1 == m_wCardCount%3) //����
			{
				num=m_wCardCount;
			}
			for (WORD i=13-num;i<=13;i++)
			{
				int nXPos=0,nYPos=0;
				if (13!=i)
				{
					if (0 == i)
					{
						nXPos=m_ControlPoint.x+9*(13-i);
						nYPos=m_ControlPoint.y+i*21-1;
					}
					else if (1 == i)
					{
						nXPos=m_ControlPoint.x+9*(13-i)+3;
						nYPos=m_ControlPoint.y+i*21-9-3;
					}
					else if (2 == i)
					{
						nXPos=m_ControlPoint.x+9*(13-i)+6;
						nYPos=m_ControlPoint.y+i*21-16-7;
					}
					else if (3 == i)
					{
						nXPos=m_ControlPoint.x+9*(13-i)+8;
						nYPos=m_ControlPoint.y+i*21-23-11;
					}
					else if (4 == i)
					{
						nXPos=m_ControlPoint.x+9*(13-i)+11;
						nYPos=m_ControlPoint.y+i*21-30-15;
					}
					else if (5 == i)
					{
						nXPos=m_ControlPoint.x+9*(13-i)+13;
						nYPos=m_ControlPoint.y+i*21-35-20;
					}
					else if (6 == i)
					{
						nXPos=m_ControlPoint.x+9*(13-i)+15;
						nYPos=m_ControlPoint.y+i*21-40-25;
					}
					else if (7 == i)
					{
						nXPos=m_ControlPoint.x+9*(13-i)+17;
						nYPos=m_ControlPoint.y+i*21-45-30;
					}
					else if (8 == i)
					{
						nXPos=m_ControlPoint.x+9*(13-i)+17;
						nYPos=m_ControlPoint.y+i*21-48-35;
					}
					else if (9 == i)
					{
						nXPos=m_ControlPoint.x+9*(13-i)+17;
						nYPos=m_ControlPoint.y+i*21-50-41;
					}
					else if (10 == i)
					{
						nXPos=m_ControlPoint.x+9*(13-i)+18;
						nYPos=m_ControlPoint.y+i*21-51-49;
					}
					else if (11 == i)
					{
						nXPos=m_ControlPoint.x+9*(13-i)+18;
						nYPos=m_ControlPoint.y+i*21-51-56;
					}
					else if (12 == i)
					{
						nXPos=m_ControlPoint.x+9*(13-i)+16;
						nYPos=m_ControlPoint.y+i*21-51-62;
					}

					int nImageXPos=(13 - i)*nItemWidth;
					int nImageYPos=(GetImageIndex(m_cbCardData[i+num-13])-1)*nItemHeight;
					m_ImageUserTableCardVL.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*(13 - i),0);
					m_ImageTabCardNumVL.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,nImageYPos);
				}


				if (i ==13 && 2 == (m_wCardCount%3))
				{
					nXPos=m_ControlPoint.x+9*(13-i)+15;
					nYPos=m_ControlPoint.y+i*21-49-68;
					int nImageXPos=0;
					int nImageYPos=(GetImageIndex(m_cbCardData[m_wCardCount-1])-1)*nItemHeight;
					m_ImageUserTableCardVL.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,0,0);
					m_ImageTabCardNumVL.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,nImageYPos);
				}

			}


			break;
		}
	case Direction_North:	//����
		{//�滭�˿�
			int nWidth=m_ImageUserTableCardHT.GetWidth()/14;
			int nHeight=m_ImageUserTableCardHT.GetHeight();

			int nItemWidth = nWidth;
			int nItemHeight =  m_ImageTabCardNumHT.GetHeight()/27;

			//�滭����
			int count = m_wCardCount;
			if (2 == count%3)                  //����14
			{
				if (count > 7) //������
				{
					for(int i = count-1; i>6; --i)
					{
						int nXPos=m_ControlPoint.x+(i%7)*23+30;
						int nYPos=m_ControlPoint.y;
						int nImageXPos=i*nItemWidth;
						int nImageYPos=(GetImageIndex(m_cbCardData[i])-1)*nItemHeight;
						m_ImageUserTableCardHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*i,0);
						m_ImageTabCardNumHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,nImageYPos);
					}
					count -= (count -7);
				}

				if (count > 0)//ǰ����
				{
					for (int i=0; i<count; ++i)
					{
						int nXPos=m_ControlPoint.x+(i%7)*23-130;
						int nYPos=m_ControlPoint.y;
						int nImageXPos=i*nItemWidth;
						int nImageYPos=(GetImageIndex(m_cbCardData[i])-1)*nItemHeight;
						m_ImageUserTableCardHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*i,0);
						m_ImageTabCardNumHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,nImageYPos);
					}
				}
			}
			else  if (1 == count%3)             //û��13
			{
				if (count > 7) //������
				{
					for(int i = count; i>6; --i)
					{
						int nXPos=m_ControlPoint.x+(i%7)*23+30;
						int nYPos=m_ControlPoint.y;
						int nImageXPos=i*nItemWidth;
						int nImageYPos=(GetImageIndex(m_cbCardData[i-1])-1)*nItemHeight;
						m_ImageUserTableCardHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*i,0);
						m_ImageTabCardNumHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,nImageYPos);
					}
					count -= (count -6);
				}

				if (count > 0)//ǰ����
				{
					for (int i=0; i<count; ++i)
					{
						int nXPos=m_ControlPoint.x+(i%6)*23-130+23;
						int nYPos=m_ControlPoint.y;
						int nImageXPos=i*nItemWidth;
						int nImageYPos=(GetImageIndex(m_cbCardData[i])-1)*nItemHeight;
						m_ImageUserTableCardHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nWidth*i,0);
						m_ImageTabCardNumHT.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nImageXPos,nImageYPos);
					}
				}
			}

			break;
		}
	}
__LEAVE_FUNCTION
	return;
}

//�����˿�
bool CTableCard::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	//Ч���С
	ASSERT(wCardCount<=CountArray(m_cbCardData));
	if (wCardCount>CountArray(m_cbCardData))
		return false;

	//�����˿�
	m_wCardCount=wCardCount;
	CopyMemory(m_cbCardData,cbCardData,sizeof(m_cbCardData[0])*wCardCount);

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CKingCard::CKingCard()
{
__ENTER_FUNCTION
	//�˿�����
	m_wCardCount=0;
	ZeroMemory(m_cbCardData,sizeof(m_cbCardData));

	//���Ʊ���
	m_ControlPoint.SetPoint(0,0);
	m_CardDirection=Direction_South;

	//ͼƬ��Դ
	TCHAR szResourcePath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,szResourcePath);
	CString strPath(szResourcePath);
	strPath.Append("\\SparrowYY\\Picture\\PNG\\");
	m_ImageUserKingCardBack.LoadImage(TEXT(strPath+"KINGCARDBACK.png"));
	m_ImageUserKingCardNum.LoadImage(TEXT(strPath+"KINGCARDNUM.png"));
__LEAVE_FUNCTION
	return;
}

//��������
CKingCard::~CKingCard()
{
}



//��ȡλ��
int CKingCard::GetImageIndex(BYTE cbCardData)
{
	//�����ж�
	if (cbCardData==0) 
		return 0;

	//����λ��
	BYTE cbValue=cbCardData&MASK_VALUE;
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return (cbColor>=0x03)?(cbValue+27):(cbColor*9+cbValue);
}

//�滭�˿�
void CKingCard::DrawCardControl(CDC * pDC)
{
__ENTER_FUNCTION
	{//�滭�˿�
		//�滭����
		int nXPos=m_ControlPoint.x+70;
		int nYPos=m_ControlPoint.y-30;
		int nWidth=m_ImageUserKingCardBack.GetWidth();
		int nHeight=m_ImageUserKingCardBack.GetHeight();
		int nItemWidth = m_ImageUserKingCardNum.GetWidth();
		int nItemHeight = m_ImageUserKingCardNum.GetHeight()/27;
		int nImageYPos=(GetImageIndex(m_cbCardData[0])-1)*nItemHeight;
		m_ImageUserKingCardBack.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,0,0);
		m_ImageUserKingCardNum.DrawImage(pDC,nXPos+100,nYPos,nItemWidth,nItemHeight,0,nImageYPos);
	}
__LEAVE_FUNCTION
	return;
}

//�����˿�
bool CKingCard::SetCardData(const BYTE cbCardData[], WORD wCardCount)
{
	//Ч���С
	ASSERT(wCardCount<=CountArray(m_cbCardData));
	if (wCardCount>CountArray(m_cbCardData))
		return false;

	//�����˿�
	m_wCardCount=wCardCount;
	CopyMemory(m_cbCardData,cbCardData,sizeof(m_cbCardData[0])*wCardCount);

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CCardControl::CCardControl()
{
__ENTER_FUNCTION
	//״̬����
	m_bPositively=false;
	m_bDisplayItem=false;

	//λ�ñ���
	m_XCollocateMode=enXCenter;
	m_YCollocateMode=enYCenter;
	m_BenchmarkPos.SetPoint(0,0);

	//�˿�����
	m_wCardCount=0;
	m_wHoverItem=INVALID_ITEM;
	ZeroMemory(&m_CurrentCard,sizeof(m_CurrentCard));
	ZeroMemory(&m_CardItemArray,sizeof(m_CardItemArray));

	//��������
	m_ControlPoint.SetPoint(0,0);
	m_ControlSize.cy=CARD_HEIGHT+POS_SHOOT;
	m_ControlSize.cx=(CountArray(m_CardItemArray)+1)*CARD_WIDTH+POS_SPACE;


	//��������
	TCHAR szResourcePath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,szResourcePath);
	CString strPath(szResourcePath);
	strPath.Append("\\SparrowYY\\Picture\\PNG\\");

	//�û��˿�
	//3DView
	//m_ImageMyselfCard.LoadImage(TEXT(strPath+"myselfcard.png"));

	m_ImageUserBottom.LoadImage(TEXT(strPath+"CARD_USER_BOTTOM.png"));
	m_ImageKingBottom.LoadImage(TEXT(strPath+"KING_USER_BOTTOM.png"));
	m_ImageOutCardNum.LoadImage(TEXT(strPath+"OUTCARDNUM.png"));

	m_iCardIndex = INVALID_ITEM;
	m_btCurCardData = 0;
	m_iCurCardX = 0;
__LEAVE_FUNCTION
	return;
}

//��������
CCardControl::~CCardControl()
{

}

//��׼λ��
void CCardControl::SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	m_BenchmarkPos.x=nXPos;
	m_BenchmarkPos.y=nYPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	//����λ��
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ m_ControlPoint.x=m_BenchmarkPos.x; break; }
	case enXCenter: { m_ControlPoint.x=m_BenchmarkPos.x-m_ControlSize.cx/2; break; }
	case enXRight:	{ m_ControlPoint.x=m_BenchmarkPos.x-m_ControlSize.cx; break; }
	}

	//����λ��
	switch (m_YCollocateMode)
	{
	case enYTop:	{ m_ControlPoint.y=m_BenchmarkPos.y; break; }
	case enYCenter: { m_ControlPoint.y=m_BenchmarkPos.y-m_ControlSize.cy/2; break; }
	case enYBottom: { m_ControlPoint.y=m_BenchmarkPos.y-m_ControlSize.cy; break; }
	}

	return;
}

//��׼λ��
void CCardControl::SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	m_BenchmarkPos=BenchmarkPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	//����λ��
	switch (m_XCollocateMode)
	{
	case enXLeft:	{ m_ControlPoint.x=m_BenchmarkPos.x; break; }
	case enXCenter: { m_ControlPoint.x=m_BenchmarkPos.x-m_ControlSize.cx/2; break; }
	case enXRight:	{ m_ControlPoint.x=m_BenchmarkPos.x-m_ControlSize.cx; break; }
	}

	//����λ��
	switch (m_YCollocateMode)
	{
	case enYTop:	{ m_ControlPoint.y=m_BenchmarkPos.y; break; }
	case enYCenter: { m_ControlPoint.y=m_BenchmarkPos.y-m_ControlSize.cy/2; break; }
	case enYBottom: { m_ControlPoint.y=m_BenchmarkPos.y-m_ControlSize.cy; break; }
	}

	return;
}

//��ȡ�˿�
BYTE CCardControl::GetHoverCard()
{
__ENTER_FUNCTION
	//��ȡ�˿�
	if (m_wHoverItem!=INVALID_ITEM)
	{
		if (m_wHoverItem==CountArray(m_CardItemArray))
			return m_CurrentCard.cbCardData;
		return m_CardItemArray[m_wHoverItem].cbCardData;
	}
__LEAVE_FUNCTION
	return 0;
}

//��ȡ�˿�
bool CCardControl::GetHoverCardItem(tagCardItem &CardItem)
{
__ENTER_FUNCTION
	//��ȡ�˿�
	if (m_wHoverItem!=INVALID_ITEM)
	{
		if (m_wHoverItem==CountArray(m_CardItemArray))
		{
			CardItem.bKingFlag = m_CurrentCard.bKingFlag;
			CardItem.bShoot = m_CurrentCard.bShoot;
			CardItem.cbCardData = m_CurrentCard.cbCardData;
			return true;
		}
		else
		{
			CardItem.bKingFlag = m_CardItemArray[m_wHoverItem].bKingFlag;
			CardItem.bShoot = m_CardItemArray[m_wHoverItem].bShoot;
			CardItem.cbCardData = m_CardItemArray[m_wHoverItem].cbCardData;
			return true;
		}
	}
__LEAVE_FUNCTION
	return false;
}


//�����˿�
bool CCardControl::SetCurrentCard(BYTE cbCardData)
{
	//���ñ���
	m_CurrentCard.bShoot=false;
	m_CurrentCard.cbCardData=cbCardData;

	return true;
}

//�����˿�
bool CCardControl::SetCurrentCard(tagCardItem CardItem)
{
	//���ñ���
	m_CurrentCard.bShoot=CardItem.bShoot;
	m_CurrentCard.cbCardData=CardItem.cbCardData;
	m_CurrentCard.bKingFlag = CardItem.bKingFlag;

	return true;
}

//�����˿�
bool CCardControl::SetCardData(const BYTE cbCardData[], WORD wCardCount, BYTE cbCurrentCard,BYTE cbKingCard)
{
__ENTER_FUNCTION
	//Ч���С
	ASSERT(wCardCount<=CountArray(m_CardItemArray));
	if (wCardCount>CountArray(m_CardItemArray)) return false;

	//��ǰ�˿�
	m_CurrentCard.bShoot=false;
	m_CurrentCard.cbCardData=cbCurrentCard;

	//if ( cbCurrentCard == cbKingCard)
	//{
	//  m_CurrentCard.bKingFlag=true;
	//}

	//�м����
	tagCardItem CardItemArray[14];
	ZeroMemory(&CardItemArray,sizeof(CardItemArray));

	//�����˿�
	m_wCardCount=wCardCount;
	WORD wKingCount=0;
	for (WORD i=0;i<m_wCardCount;i++)
	{
		m_CardItemArray[i].bShoot=false;
		m_CardItemArray[i].cbCardData=cbCardData[i];

		CardItemArray[i].bShoot=false;
		CardItemArray[i].cbCardData=cbCardData[i];

		//�ж��Ƿ�����
		bool bKingCard=false;
		if(cbCardData[i]==cbKingCard)
		{
			bKingCard=true;
		}

		//�������Ʊ�־
		m_CardItemArray[i].bKingFlag=bKingCard;
		CardItemArray[i].bKingFlag=bKingCard;

		if(bKingCard)wKingCount++;
	}

	//��������
	if(wKingCount>0)//���ƴ���
	{
		ZeroMemory(&m_CardItemArray,sizeof(m_CardItemArray));

		WORD wCount=0;

		for(WORD i=0;i<m_wCardCount;i++)
		{
			if(CardItemArray[i].bKingFlag==true)//���ƿ�ǰ
			{
				m_CardItemArray[wCount].bKingFlag=true;
				m_CardItemArray[wCount].bShoot=CardItemArray[i].bShoot;
				m_CardItemArray[wCount++].cbCardData = CardItemArray[i].cbCardData;
			}
		}

		if(wCount<m_wCardCount)//�����ƿ���
		{
			for(WORD i=0;i<m_wCardCount;i++)
			{
				if(CardItemArray[i].bKingFlag==false)
				{
					m_CardItemArray[wCount].bKingFlag=false;
					m_CardItemArray[wCount].bShoot=CardItemArray[i].bShoot;
					m_CardItemArray[wCount++].cbCardData = CardItemArray[i].cbCardData;
				}
			}

		}

	}
__LEAVE_FUNCTION
	return true;
}

//�����˿�
bool CCardControl::SetCardItem(const tagCardItem CardItemArray[], WORD wCardCount)
{
__ENTER_FUNCTION
	//Ч���С
	ASSERT(wCardCount<=CountArray(m_CardItemArray));
	if (wCardCount>CountArray(m_CardItemArray))
		return false;

	//�����˿�
	m_wCardCount=wCardCount;
	for (WORD i=0;i<m_wCardCount;i++)
	{
		m_CardItemArray[i].bShoot=CardItemArray[i].bShoot;
		m_CardItemArray[i].cbCardData=CardItemArray[i].cbCardData;
		m_CardItemArray[i].bKingFlag = CardItemArray[i].bKingFlag;
	}
__LEAVE_FUNCTION
	return true;
}


//��ȡλ��
int CCardControl::GetImageIndex(BYTE cbCardData)
{
	//�����ж�
	if (cbCardData==0) 
		return 0;

	//����λ��
	BYTE cbValue=cbCardData&MASK_VALUE;
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return (cbColor>=0x03)?(cbValue+27):(cbColor*9+cbValue);
}


//�滭�˿�
void CCardControl::DrawCardControl(CDC * pDC)
{
__ENTER_FUNCTION
	//�滭׼��
	int nXExcursion=m_ControlPoint.x+(CountArray(m_CardItemArray)-m_wCardCount)*CARD_WIDTH;

	//�滭�˿�
	for (WORD i=0;i<m_wCardCount;i++)
	{
		//����λ��
		int nXScreenPos=nXExcursion+CARD_WIDTH*i;
		int nYScreenPos=m_ControlPoint.y+(((m_CardItemArray[i].bShoot==false)&&(m_wHoverItem!=i))?POS_SHOOT:0);


		//�滭�˿�
		BYTE cbCardData=(m_bDisplayItem==true)?m_CardItemArray[i].cbCardData:0;
		BYTE byIndex= GetImageIndex(cbCardData);

		if(m_CardItemArray[i].bKingFlag)
		{
			m_ImageKingBottom.DrawImage(pDC,nXScreenPos,nYScreenPos,CARD_WIDTH,CARD_HEIGHT,byIndex*CARD_WIDTH,0,CARD_WIDTH,CARD_HEIGHT);
		}
		else
		{
			m_ImageUserBottom.DrawImage(pDC,nXScreenPos,nYScreenPos,CARD_WIDTH,CARD_HEIGHT,byIndex*CARD_WIDTH,0,CARD_WIDTH,CARD_HEIGHT);
		}


	}

	//��ǰ�˿�
	if (m_CurrentCard.cbCardData!=0)
	{
		//����λ��
		int nXScreenPos=m_ControlPoint.x+m_ControlSize.cx-CARD_WIDTH;
		int nYScreenPos=m_ControlPoint.y+(((m_CurrentCard.bShoot==false)&&(m_wHoverItem!=CountArray(m_CardItemArray)))?POS_SHOOT:0);

		//�滭�˿�
		BYTE cbCardData=(m_bDisplayItem==true)?m_CurrentCard.cbCardData:0;
		BYTE byIndex= GetImageIndex(cbCardData);
		if(m_CurrentCard.bKingFlag==true)
			m_ImageKingBottom.DrawImage(pDC,nXScreenPos+8.5,nYScreenPos,CARD_WIDTH,CARD_HEIGHT,byIndex*CARD_WIDTH,0,CARD_WIDTH,CARD_HEIGHT);
		else			
			m_ImageUserBottom.DrawImage(pDC,nXScreenPos+8.5,nYScreenPos,CARD_WIDTH,CARD_HEIGHT,byIndex*CARD_WIDTH,0,CARD_WIDTH,CARD_HEIGHT);

	}
__LEAVE_FUNCTION
	return;
}


//�����л�
WORD CCardControl::SwitchCardPoint(CPoint & MousePoint)
{
__ENTER_FUNCTION
	//��׼λ��
	int nXPos=MousePoint.x-m_ControlPoint.x;
	int nYPos=MousePoint.y-m_ControlPoint.y;

	//��Χ�ж�
	if ((nXPos<0)||(nXPos>m_ControlSize.cx)) 
		return INVALID_ITEM;
	if ((nYPos<POS_SHOOT)||(nYPos>m_ControlSize.cy)) 
		return INVALID_ITEM;

	//��������
	if (nXPos<CARD_WIDTH*CountArray(m_CardItemArray))
	{
		WORD wViewIndex=(WORD)(nXPos/CARD_WIDTH)+m_wCardCount;
		if (wViewIndex>=CountArray(m_CardItemArray))
			return wViewIndex-CountArray(m_CardItemArray);
		return INVALID_ITEM;
	}

	//��ǰ����
	if ((m_CurrentCard.cbCardData!=0)&&(nXPos>=(m_ControlSize.cx-CARD_WIDTH))) 
		return CountArray(m_CardItemArray);
__LEAVE_FUNCTION
	return INVALID_ITEM;
}

//�����Ϣ
bool CCardControl::OnEventSetCursor(CPoint Point, bool & bRePaint)
{

	//��ȡ����
	WORD wHoverItem=SwitchCardPoint(Point);
__ENTER_FUNCTION
	//��Ӧ�ж�
	if ((m_bPositively==false)&&(m_wHoverItem!=INVALID_ITEM))
	{
		bRePaint=true;
		m_wHoverItem=INVALID_ITEM;
	}

	//�����ж�
	if ((wHoverItem!=m_wHoverItem)&&(m_bPositively==true))
	{
		bRePaint=true;
		m_wHoverItem=wHoverItem;
		//m_pGameClientDlg->PlaySound("huashu",false);
		((CGameClientDlg*)(AfxGetMainWnd()))->PlaySound("huashu",false);
		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
	}
__LEAVE_FUNCTION
	return (wHoverItem!=INVALID_ITEM);
}




int CCardControl::CheckIndex(CPoint Point)
{
__ENTER_FUNCTION
	//��ȡ����
	WORD wHoverItem=SwitchCardPoint(Point);

	m_iCardIndex = wHoverItem;
	m_iCurCardX = m_ControlPoint.x+(CountArray(m_CardItemArray)-m_wCardCount)*CARD_WIDTH;
	if (wHoverItem>=0 && wHoverItem<CountArray(m_CardItemArray))
	{

		m_iCurCardX = m_iCurCardX+wHoverItem*CARD_WIDTH+CARD_WIDTH/2;
		m_btCurCardData = m_CardItemArray[wHoverItem].cbCardData;

		return wHoverItem;
	}
	else if (13==wHoverItem)
	{
		m_iCurCardX = m_ControlPoint.x+m_ControlSize.cx-CARD_WIDTH/2+8;
		m_btCurCardData = m_CurrentCard.cbCardData;
		return wHoverItem;
	}
__LEAVE_FUNCTION
	return -1;
}


void CCardControl::GetCurCardDataAndX(BYTE& btCardData, int& iX)
{
	btCardData = m_btCurCardData;
	iX = m_iCurCardX;
}
//set
void CCardControl::InitCurCardDataX()
{
	m_btCurCardData = 0;
	m_iCardIndex = INVALID_ITEM;
}
//////////////////////////////////////////////////////////////////////////

//��������
//CCardResource						g_CardResource;						//�˿���Դ

//////////////////////////////////////////////////////////////////////////
