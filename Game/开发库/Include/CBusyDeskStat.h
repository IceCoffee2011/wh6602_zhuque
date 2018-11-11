#pragma once
#ifndef  CCBUSYDESKSTAT_H_
#define  CCBUSYDESKSTAT_H_
#include <list>
class CCBusyDeskStat
{
	enum { MAX_SAMPLE=1 };
	typedef std::list<unsigned int>                           BusyDeskList;
	typedef std::list<unsigned int>::iterator                 BusyDeskListIt;
	BusyDeskList  m_busyDeskList;                      //��æ���ӵ���������
public:
	//��Ŀǰ��æ���ӵ�����д��
	void Insert(unsigned int num);
	//�õ�ƽ������
    unsigned int  GetAverage(); 


public:
	CCBusyDeskStat(void);
	~CCBusyDeskStat(void);

};
#endif
