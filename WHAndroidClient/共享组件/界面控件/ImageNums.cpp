#include "StdAfx.h"
#include "imagenums.h"

CImageNums::CImageNums(void)
{
	m_lNums			=	0;		//����
	m_bIsSign		=	false;	//���޷���
	m_bIsSeparator	=	false;	//���޷ָ���
	m_enNumAlign	=	IA_LEFT;//���뷽ʽ
	m_rcNumsArea.SetRect(0, 0, 100, 100);	//������������

	//����Դ
	m_lSepatatorW	=	0;//�ָ�����17
	m_lSepatatorH	=	0;//�ָ�����25
	m_lNumWidth		=	0;//���ֿ��
	m_lNumHeight	=	0;//���ָ߶�
	m_iFrameCount	=	12;
	
	//�������
	m_vecNumArray.clear();
}

CImageNums::~CImageNums(void)
{
	if (!m_ImageNums.IsNull())
	{
		m_ImageNums.DestroyImage();
	}
}


//��������
void CImageNums::SetNums(long lNums)
{
	m_lNums = lNums;
	lNums = abs(lNums);
	m_vecNumArray.clear();
	do 
	{
		m_vecNumArray.push_back(lNums%10);
		lNums = lNums/10;
	} while (0!=lNums);
}

//�������޷���
void CImageNums::SetSign(bool bIsSign)
{
	m_bIsSign = bIsSign;
}

//���÷ָ�����
void CImageNums::SetSeparator(bool bIsSeparator)
{
	m_bIsSeparator = bIsSeparator;
}

//���ö�����
void CImageNums::SetNumAlign(NumAlign align)
{
	m_enNumAlign = align;
}

//����������ʾ����
void CImageNums::SetShowRect(CRect rect)
{
	m_rcNumsArea = rect;
}

//���÷ָ����Ŀ����
void CImageNums::SetSeparatorWH(int iWidth, int iHeight)
{
	m_lSepatatorW	=	iWidth;//�ָ�����
	m_lSepatatorH	=	iHeight;//�ָ�����


	if (!m_ImageNums.IsNull())
	{//����Դ
		m_lNumWidth		=	(m_ImageNums.GetWidth()-m_lSepatatorW)/m_iFrameCount;//���ֿ��
		m_lNumHeight	=	m_ImageNums.GetHeight();//���ָ߶�
	}
}

//����ͼƬ
void CImageNums::SetImageResource(const char* pResource, int iFrameCount/*=12*/)
{
	if (NULL != pResource)
	{
		char chResource[256]={0};
		memset(chResource, 0, sizeof(chResource));
		sprintf(chResource, TEXT("%s"), pResource);
		
		//װ��ͼƬ��Դ
		m_ImageNums.LoadImage(TEXT(chResource));//����ͼƬ

		if (!m_ImageNums.IsNull())
		{//����Դ
			m_iFrameCount = iFrameCount;
			m_lNumWidth		=	(m_ImageNums.GetWidth()-m_lSepatatorW)/iFrameCount;//���ֿ��
			m_lNumHeight	=	m_ImageNums.GetHeight();//���ָ߶�
		}
	}
}

//��������
void CImageNums::DrawImageNums(CDC* pDC)
{
	if (NULL!=pDC && !m_ImageNums.IsNull())
	{
		long lBitNums = m_vecNumArray.size();
		if (lBitNums>0)
		{
			//�ܳ���
			long lTotalImageNumsWidth = 0;
			//�����ֿ��
			lTotalImageNumsWidth = lBitNums*m_lNumWidth;
			
			if (true == m_bIsSign)
			{//�з���
				lTotalImageNumsWidth += m_lNumWidth;
			}
			
			int iCountSeparatorNums = lBitNums/3;
			if (true == m_bIsSeparator)
			{//�зָ�����,���һ����ж�һ��Ҫ���ٷָ�����
				if (0==lBitNums%3)
				{//�����3�ı���,����һ���ָ�����
					iCountSeparatorNums -= 1;
				}
				lTotalImageNumsWidth += iCountSeparatorNums*m_lSepatatorW;
			}
			int iX = 0;
			if (IA_LEFT == m_enNumAlign)
			{//�����
				iX = m_rcNumsArea.left+lTotalImageNumsWidth;
			}
			else if (IA_CENTER == m_enNumAlign)
			{//���ж���
				iX = m_rcNumsArea.left+(m_rcNumsArea.Width()-lTotalImageNumsWidth)/2+lTotalImageNumsWidth;//�������
			}
			else if (IA_RIGHT == m_enNumAlign)
			{//�Ҷ���
				iX = m_rcNumsArea.right;
			}
			//��ȥһ�����ֵĿ��Ϊĩ��
			iX-=m_lNumWidth;

			int iCount3Point = 0;
			for (int i=0; i<lBitNums; i++)
			{//��������
				if (m_vecNumArray[i]>=0 && m_vecNumArray[i]<=9)
				{
					m_ImageNums.DrawImage(pDC, iX, m_rcNumsArea.top, m_lNumWidth, m_lNumHeight, m_vecNumArray[i]*m_lNumWidth, 0, m_lNumWidth, m_lNumHeight);
					++iCount3Point;

					if ((true==m_bIsSeparator)&&(iCountSeparatorNums>0)&&(3==iCount3Point))
					{//���Ʒָ�����
						iX-=m_lSepatatorW;
						m_ImageNums.DrawImage(pDC, iX, m_rcNumsArea.top, m_lSepatatorW, m_lSepatatorH, m_iFrameCount*m_lNumWidth, 0, m_lSepatatorW, m_lSepatatorH);
						iCount3Point = 0;
						--iCountSeparatorNums;
					}
					iX-=m_lNumWidth;
				}
			}

			if (true == m_bIsSign)
			{//���Ʒ���
				int iDrawX = (m_lNums>=0)?10:11;
				m_ImageNums.DrawImage(pDC, iX, m_rcNumsArea.top, m_lNumWidth, m_lNumHeight, iDrawX*m_lNumWidth, 0, m_lNumWidth, m_lNumHeight);
			}
		}
	}
}


