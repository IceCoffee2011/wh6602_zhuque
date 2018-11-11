#include "StdAfx.h"
#include ".\cbusydeskstat.h"

CCBusyDeskStat::CCBusyDeskStat(void)
{
}

CCBusyDeskStat::~CCBusyDeskStat(void)
{
}

//��Ŀǰ��æ���ӵ�����д��
void CCBusyDeskStat::Insert(unsigned int num)
{
	if (m_busyDeskList.size() < MAX_SAMPLE)
	{
		m_busyDeskList.push_back(num);
	}else{
		m_busyDeskList.pop_front();
		m_busyDeskList.push_back(num);
	}
}
//�õ�ƽ������
unsigned int  CCBusyDeskStat::GetAverage()
{
	unsigned int count = 0;
	BusyDeskListIt it = m_busyDeskList.begin();
	BusyDeskListIt itE = m_busyDeskList.end();
	for (; it != itE; ++it)
	{
		count += (*it);
	}
	return (count / m_busyDeskList.size());
}