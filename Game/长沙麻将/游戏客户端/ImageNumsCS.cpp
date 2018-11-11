#include "StdAfx.h"
#include "ImageNumsCS.h"

CImageNumsCS::CImageNumsCS(void)
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
	
	//�������
	m_vecNumArray.clear();
}

CImageNumsCS::~CImageNumsCS(void)
{
	if (!m_ImageNums.IsNull())
	{
		m_ImageNums.DestroyImage();
	}
}


//��������
void CImageNumsCS::SetNums(long lNums)
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
void CImageNumsCS::SetSign(bool bIsSign)
{
	m_bIsSign = bIsSign;
}

//���÷ָ�����
void CImageNumsCS::SetSeparator(bool bIsSeparator)
{
	m_bIsSeparator = bIsSeparator;
}

//���ö�����
void CImageNumsCS::SetNumAlign(NumAlign align)
{
	m_enNumAlign = align;
}

//����������ʾ����
void CImageNumsCS::SetShowRect(CRect rect)
{
	m_rcNumsArea = rect;
}

//���÷ָ����Ŀ����
void CImageNumsCS::SetSeparatorWH(int iWidth, int iHeight)
{
	m_lSepatatorW	=	iWidth;//�ָ�����
	m_lSepatatorH	=	iHeight;//�ָ�����
}

//����ͼƬ
void CImageNumsCS::SetImageResource(char* pResource)
{
	if (NULL != pResource)
	{
		char chResource[50]={0};
		memset(chResource, 0, sizeof(chResource));
		sprintf(chResource, TEXT("%s"), pResource);
		
		//װ��ͼƬ��Դ
		HINSTANCE hInstance = AfxGetInstanceHandle();
		TCHAR szResourcePath[MAX_PATH]="";
		GetCurrentDirectory(MAX_PATH,szResourcePath);
		CString strPath(szResourcePath);
		strPath.Append("\\SparrowCS\\Picture\\GameEnd\\");
		strPath.Append(chResource);
		m_ImageNums.LoadImage((LPCTSTR)strPath);//����ͼƬ

		if (!m_ImageNums.IsNull())
		{//����Դ
			m_lNumWidth		=	(m_ImageNums.GetWidth()-m_lSepatatorW)/12;//���ֿ��
			m_lNumHeight	=	m_ImageNums.GetHeight()/4;//���ָ߶�
		}
	}
}

//��������
void CImageNumsCS::DrawImageNums(CDC* pDC)
{
	if (!m_ImageNums.IsNull())
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

			int nYPos=0;
			if (m_lNums>0)
			{
				nYPos=2*m_lNumHeight;
			}
			else if (m_lNums<0)
			{
				nYPos=m_lNumHeight;
			}
			else
			{
				nYPos=3*m_lNumHeight;
			}

			int iCount3Point = 0;
			for (int i=0; i<lBitNums; i++)
			{//��������
				if (m_vecNumArray[i]>=0 && m_vecNumArray[i]<=9)
				{
					if (true == m_bIsSign)
					{
						m_ImageNums.DrawImage(pDC, iX, m_rcNumsArea.top, m_lNumWidth, m_lNumHeight, m_vecNumArray[i]*m_lNumWidth, nYPos, m_lNumWidth, m_lNumHeight);
					}
					else
					{
						m_ImageNums.DrawImage(pDC, iX, m_rcNumsArea.top, m_lNumWidth, m_lNumHeight, m_vecNumArray[i]*m_lNumWidth, 0, m_lNumWidth, m_lNumHeight);
					}
					++iCount3Point;

					if ((true==m_bIsSeparator)&&(iCountSeparatorNums>0)&&(3==iCount3Point))
					{//���Ʒָ�����
						iX-=m_lSepatatorW;
						m_ImageNums.DrawImage(pDC, iX, m_rcNumsArea.top, m_lSepatatorW, m_lSepatatorH, 12*m_lNumWidth, nYPos, m_lSepatatorW, m_lSepatatorH);
						iCount3Point = 0;
						--iCountSeparatorNums;
					}
					iX-=m_lNumWidth;
				}
			}

			if (true == m_bIsSign)
			{//���Ʒ���
				if (0 != m_lNums)
				{
					int iDrawX = (m_lNums>0)?10:11;
					m_ImageNums.DrawImage(pDC, iX, m_rcNumsArea.top, m_lNumWidth, m_lNumHeight, iDrawX*m_lNumWidth, nYPos, m_lNumWidth, m_lNumHeight);
				}				
			}
		}
	}
}


