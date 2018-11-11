#ifndef ___IMAGE_NUMS_H_CS_DEF___
#define ___IMAGE_NUMS_H_CS_DEF___

#include "SkinControls.h"
#include <vector>
//Author: OSC
//Time: 2011-3-23
//ͼƬ���ֹ���.
//1:������������
//2:���Ʒ���.
//3:���Ʒָ���.
//4:���������Ҷ��뷽ʽ

using namespace std;

//enum NumAlign
//{
//	IA_LEFT	=	0,
//	IA_CENTER,
//	IA_RIGHT
//};


//����ͼƬ��
class CImageNumsCS
{
public:
	CImageNumsCS(void);
	virtual ~CImageNumsCS(void);

public:

	//��������
	void	SetNums(long lNums);
	long	GetNums(void)	{	return m_lNums;	}
	int		GetBitNums(void)	{	return (int)m_vecNumArray.size();	}

	//�������޷���
	void	SetSign(bool bIsSign=false);
	bool	GetSign(void)	{	return m_bIsSign;}

	//���÷ָ�����
	void	SetSeparator(bool bIsSeparator=false);
	bool	GetSeparator(void){	return	m_bIsSeparator;	}

	//���ö�����
	void	SetNumAlign(NumAlign align=IA_LEFT);
	NumAlign	GetNumAlign(void)	{	return m_enNumAlign;	}

	//���÷ָ����Ŀ����
	void	SetSeparatorWH(int iWidth, int iHeight);

	//����ͼƬ
	void	SetImageResource(char* pResource);

	//����������ʾ����
	void	SetShowRect(CRect rect);

	//��������
	void	DrawImageNums(CDC* pDC);

	
private:
	long		m_lNums;		//����
	bool		m_bIsSign;		//���޷���
	bool		m_bIsSeparator;	//���޷ָ���
	NumAlign	m_enNumAlign;	//���뷽ʽ
	CRect		m_rcNumsArea;	//������������
	CPngImage	m_ImageNums;	//����ͼƬ
	long		m_lNumWidth;	//���ֿ��
	long		m_lNumHeight;	//���ָ߶�
	long		m_lSepatatorW;	//�ָ�����
	long		m_lSepatatorH;	//�ָ�����


private:
	std::vector<int>	m_vecNumArray;	//��������
};



#endif
