/*
*  �ļ����֣� CAutoQueue.h
*  �������ڣ� 2011.3.8
*  �����ˣ�   ����
*  ���ܣ�     ʵ�����Ŷ��㷨
*  ������
*/

#include "ThreadGuard.h"

#ifndef CCAUTOQUEUE_H_
#define CCAUTOQUEUE_H_
#include <list>
#include <set>
#include "GameServiceExport.h"
class CCAutoQueue
{
private:
	typedef std::list<IServerUserItem*>				 ItemQueue; 
	typedef std::list<IServerUserItem*>::iterator    ItemQueueIt;
	ItemQueue	m_listAutoQueue;						//�Ŷ��û�����

	CCriticalSpection	m_lockTwo;		// add by HouGuoJiang 2012-03-27
														// ������� m_listAutoQueue �� m_setAuto     

	typedef std::set<IServerUserItem*>				  ItemSet; 
	typedef std::set<IServerUserItem*>::iterator      ItemSetIt; 
	ItemSet     m_setAuto;                          //�Ŷ��û�������  

private:
	CCAutoQueue(const CCAutoQueue&);
	CCAutoQueue& operator = (const CCAutoQueue&);
public:
	CCAutoQueue(void);
	virtual ~CCAutoQueue(void);

public:
	//����
	void Clear(); 
	//ɾ��һ���û�
	void Erase(IServerUserItem* pUserItem);
	//�����û��Ƿ����
	bool IsExist(IServerUserItem* pUserItem);
	//�����β������һ���û�
    void PushBack(IServerUserItem* pUserItem);
	//�õ����г���
	size_t Size();
	//�Ӷ���ͷ���õ�һ���û�����
	IServerUserItem* GetFront();
	//ɾ��ͷ���
	void  PoPFront();
};



#endif