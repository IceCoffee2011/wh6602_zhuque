#include "StdAfx.h"
#include ".\cautoqueue.h"

CCAutoQueue::CCAutoQueue(void)
{
}

CCAutoQueue::~CCAutoQueue(void)
{
}

void  CCAutoQueue::Clear()
{
	CThreadGuard guard(&m_lockTwo);


	IServerUserItem* pTmp = NULL;	// ������
	int iSize = m_listAutoQueue.size();

	#if 1
		// add by HouGuoJiang 2012-03-27
		while(m_listAutoQueue.size()>0)
		{
			ItemQueueIt qIt = m_listAutoQueue.begin();
			pTmp = *qIt;		// ������
			m_listAutoQueue.erase(qIt);
		}
	#else
		// del by HouGuoJiang 2012-03-27
		ItemQueueIt qEndIt = m_listAutoQueue.end();
		for (ItemQueueIt qIt = m_listAutoQueue.begin(); qIt != qEndIt; ++qIt)
		{
			m_listAutoQueue.erase(qIt);
		}
	#endif

	m_setAuto.clear();

}

void CCAutoQueue::Erase(IServerUserItem* pUserItem)
{
	CThreadGuard guard(&m_lockTwo);

	ItemSetIt sIt = m_setAuto.find(pUserItem);
	if (sIt != m_setAuto.end())
	{
		m_setAuto.erase(sIt);
	}

	ItemQueueIt qEndIt = m_listAutoQueue.end();
	for (ItemQueueIt qIt = m_listAutoQueue.begin(); qIt != qEndIt; ++qIt)
	{
		if ((*qIt) == pUserItem) //�ҵ����û�
		{
			m_listAutoQueue.erase(qIt);
			return;
		}
	}
}

bool CCAutoQueue::IsExist(IServerUserItem* pUserItem)
{
	CThreadGuard guard(&m_lockTwo);

	ItemSetIt sIt = m_setAuto.find(pUserItem);
	if (sIt != m_setAuto.end())
	{
		return true;
	}else
	{
		return false;
	}
}

//�����β������һ���û�
void CCAutoQueue::PushBack(IServerUserItem* pUserItem)
{
	CThreadGuard guard(&m_lockTwo);

	if (m_setAuto.find(pUserItem) == m_setAuto.end()) //���ڶ�������������,��ֹ�ظ������������
	{
		#if _SHOW_MESSAGE_NOR
			char info[BUFSIZ] = {0};
			sprintf(info, _T("CCAutoQueue::PushBack : %i"), pUserItem);
			CTraceService::TraceString(info,TraceLevel_Normal);
		#endif		

		m_listAutoQueue.push_back(pUserItem);
		m_setAuto.insert(pUserItem);
	}
	else
	{
		#if _SHOW_MESSAGE_NOR
			char info[BUFSIZ] = {0};
			sprintf(info, _T("CCAutoQueue::PushBack�û��Ѿ��Ŷ���"));
			CTraceService::TraceString(info,TraceLevel_Normal);
		#endif	
	}
}


//�õ����г���
size_t CCAutoQueue::Size()
{
	CThreadGuard guard(&m_lockTwo);
	return m_listAutoQueue.size();
}

//�Ӷ���ͷ���õ�һ���û����ҴӶ���ɾ��
IServerUserItem* CCAutoQueue::GetFront()
{
	CThreadGuard guard(&m_lockTwo);

	if (m_listAutoQueue.size() <= 0)
	{
		return NULL;
	}

	IServerUserItem* pItem = m_listAutoQueue.front();
	return pItem;
}

//ɾ��ͷ���
void  CCAutoQueue::PoPFront()
{
	CThreadGuard guard(&m_lockTwo);

	IServerUserItem* pItem = m_listAutoQueue.front();
	ItemSetIt it = m_setAuto.find(pItem);
	if (it != m_setAuto.end())
	{
		m_setAuto.erase(it);
	}
	m_listAutoQueue.pop_front();
}





