#include "StdAfx.h"
#include "TimerEngine.h"
#include "TraceService.h" //

//�궨��
#define NO_TIME_LEFT						DWORD(-1)				//����Ӧʱ��

//////////////////////////////////////////////////////////////////////////

//���캯��
CTimerThread::CTimerThread(void)
{
	m_pTimerEngine = NULL;
}

//��������
CTimerThread::~CTimerThread(void)
{
}

//���ú���
bool CTimerThread::InitThread(CTimerEngine * pTimerEngine)
{
	if (pTimerEngine == NULL) return false;

	//���ñ���
	m_pTimerEngine = pTimerEngine;

	return true;
}

//���к���
bool CTimerThread::OnEventThreadRun()
{
	ASSERT(m_pTimerEngine != NULL);
	Sleep(TIMER_SPACE);
	m_pTimerEngine->OnTimerThreadSink();
	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CTimerEngine::CTimerEngine(void)
{
	m_bService = false;
	m_dwTimePass=0L;
	m_dwTimeLeave=NO_TIME_LEFT;
}

//��������
CTimerEngine::~CTimerEngine(void)
{
	//ֹͣ����
	ConcludeService();

	//�����ڴ�
	tagTimerItem * pTimerItem = NULL;
	for (INT_PTR i = 0; i < m_TimerItemFree.GetCount(); i++)
	{
		pTimerItem = m_TimerItemFree[i];
		ASSERT(pTimerItem != NULL);
		SafeDelete(pTimerItem);
	}
	for (INT_PTR i = 0; i < m_TimerItemActive.GetCount(); i++)
	{
		pTimerItem = m_TimerItemActive[i];
		ASSERT(pTimerItem != NULL);
		SafeDelete(pTimerItem);
	}
	m_TimerItemFree.RemoveAll();
	m_TimerItemActive.RemoveAll();

	return;
}

//�ӿڲ�ѯ
void * __cdecl CTimerEngine::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITimerEngine, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITimerEngine, Guid, dwQueryVer);
	return NULL;
}

//���ö�ʱ��
bool __cdecl CTimerEngine::SetTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter)
{
	//������Դ
	CThreadLock lock(m_CriticalSection);//

	//Ч�����
	ASSERT(dwRepeat > 0L);
	if (dwRepeat == 0) return false;
	dwElapse=(dwElapse+TIMER_SPACE-1)/TIMER_SPACE*TIMER_SPACE;

	//���Ҷ�ʱ��
	bool bTimerExist = false;
	tagTimerItem * pTimerItem = NULL;
	for (INT_PTR i = 0; i < m_TimerItemActive.GetCount(); i++)
	{
		pTimerItem = m_TimerItemActive[i];
		ASSERT(pTimerItem != NULL);
		if (pTimerItem->wTimerID == dwTimerID)
		{
			bTimerExist = true;
			break;
		}
	}

	//������ʱ��
	if (bTimerExist == false)
	{
		INT_PTR nFreeCount = m_TimerItemFree.GetCount();
		if (nFreeCount > 0)
		{
			pTimerItem = m_TimerItemFree[nFreeCount-1];
			ASSERT(pTimerItem != NULL);
			m_TimerItemFree.RemoveAt(nFreeCount - 1);
		}
		else
		{
			try
			{
				pTimerItem = new tagTimerItem;
				ASSERT(pTimerItem != NULL);
				if (pTimerItem == NULL) return false;
			}
			catch (...)
			{
				return false;
			}
		}
	}

	//���ò���
	ASSERT(pTimerItem != NULL);
	pTimerItem->wTimerID = dwTimerID;
	pTimerItem->wBindParam = dwBindParameter;
	pTimerItem->dwElapse = dwElapse;
	pTimerItem->dwRepeatTimes = dwRepeat;
	pTimerItem->dwTimeLeave=dwElapse+m_dwTimePass;

	//���ʱ��
	m_dwTimeLeave=__min(m_dwTimeLeave,dwElapse);
	if (bTimerExist == false) m_TimerItemActive.Add(pTimerItem);

	return true;
}

//ɾ����ʱ��
bool __cdecl CTimerEngine::KillTimer(DWORD dwTimerID)
{
	//������Դ
	CThreadLock lock(m_CriticalSection);//

	//���Ҷ�ʱ��
	tagTimerItem * pTimerItem = NULL;
	for (INT_PTR i = 0; i < m_TimerItemActive.GetCount(); i++)
	{
		pTimerItem = m_TimerItemActive[i];
		ASSERT(pTimerItem != NULL);
		if (pTimerItem->wTimerID == dwTimerID)
		{
			m_TimerItemActive.RemoveAt(i);
			m_TimerItemFree.Add(pTimerItem);
			if (m_TimerItemActive.GetCount()==0) 
			{
				m_dwTimePass=0L;
				m_dwTimeLeave=NO_TIME_LEFT;
			}
			return true;;
		}
	}

	return false;
}

//ɾ����ʱ��
bool __cdecl CTimerEngine::KillAllTimer()
{
	//������Դ
	CThreadLock lock(m_CriticalSection);//

	//ɾ����ʱ��
	m_TimerItemFree.Append(m_TimerItemActive);
	m_TimerItemActive.RemoveAll();

	//���ñ���
	m_dwTimePass=0L;
	m_dwTimeLeave=NO_TIME_LEFT;

	return true;
}

//��ʼ����
bool __cdecl CTimerEngine::StartService()
{
	//Ч��״̬
	if (m_bService == true)
	{
		CTraceService::TraceString(TEXT("��ʱ�������ظ�������������������"), TraceLevel_Warning);
		return true;
	}

	//���ñ���
	m_dwTimePass=0L;
	m_dwTimeLeave=NO_TIME_LEFT;

	//���ñ���
	if (m_TimerThread.InitThread(this) == false)
	{
		CTraceService::TraceString(TEXT("��ʱ�������̷߳����ʼ��ʧ��"), TraceLevel_Exception);
		return false;
	}

	//��������
	if (m_TimerThread.StartThread() == false)
	{
		CTraceService::TraceString(TEXT("��ʱ�������̷߳�������ʧ��"), TraceLevel_Exception);
		return false;
	}

	SetThreadPriority(m_TimerThread.GetThreadHandle(), REALTIME_PRIORITY_CLASS);


	//���ñ���
	m_bService = true;

	return true;
}

//ֹͣ����
bool __cdecl CTimerEngine::ConcludeService()
{
	//���ñ���
	m_bService = false;

	//ֹͣ�߳�
	m_TimerThread.ConcludeThread(INFINITE);

	//���ñ���
	m_dwTimePass=0L;
	m_dwTimeLeave=NO_TIME_LEFT;
	m_TimerItemFree.Append(m_TimerItemActive);
	m_TimerItemActive.RemoveAll();

	//ֹͣ�������
	m_QueueServiceEvent.SetQueueServiceSink(NULL);

	return true;
}

//���ýӿ�
bool __cdecl CTimerEngine::SetTimerEngineEvent(IUnknownEx * pIUnknownEx)
{
	//Ч�����
	ASSERT(pIUnknownEx != NULL);
	ASSERT(m_bService == false);
	if (m_bService == true) return false;
	if (pIUnknownEx == NULL) return false;

	 return m_QueueServiceEvent.SetQueueServiceSink(pIUnknownEx);
}

//��ʱ��֪ͨ
void CTimerEngine::OnTimerThreadSink()
{
	//��������
	CThreadLock lock(m_CriticalSection);//

	//����ʱ��
	if (m_dwTimeLeave==NO_TIME_LEFT) 
	{
		ASSERT(m_TimerItemActive.GetCount()==0);
		return;
	}

	//����ʱ��
	ASSERT(m_dwTimeLeave>=TIMER_SPACE);
	m_dwTimeLeave-=TIMER_SPACE;
	m_dwTimePass+=TIMER_SPACE;

	//��ѯ��ʱ��
	if (m_dwTimeLeave==0)
	{
		bool bKillTimer=false;
		tagTimerItem * pTimerItem=NULL;
		DWORD dwTimeLeave=NO_TIME_LEFT;
		for (INT_PTR i=0;i<m_TimerItemActive.GetCount();)
		{
			//Ч�����
			pTimerItem=m_TimerItemActive[i];
			ASSERT(pTimerItem!=NULL);
			ASSERT(pTimerItem->dwTimeLeave>=m_dwTimePass);

			//��ʱ������
			bKillTimer=false;
			pTimerItem->dwTimeLeave-=m_dwTimePass;
			if (pTimerItem->dwTimeLeave==0L)
			{
				//����֪ͨ
				m_QueueServiceEvent.PostTimerEvent(pTimerItem->wTimerID, pTimerItem->wBindParam);

				//���ô���
				if (pTimerItem->dwRepeatTimes!=TIMES_INFINITY)
				{
					ASSERT(pTimerItem->dwRepeatTimes>0);
					if (pTimerItem->dwRepeatTimes==1L)
					{
						bKillTimer=true;
						m_TimerItemActive.RemoveAt(i);
						m_TimerItemFree.Add(pTimerItem);
					}
					else 
					{
						pTimerItem->dwRepeatTimes--;
					}
				}

				//����ʱ��
				if (bKillTimer==false) pTimerItem->dwTimeLeave=pTimerItem->dwElapse;
			}

			//��������
			if (bKillTimer==false) 
			{
				i++;
				dwTimeLeave=__min(dwTimeLeave,pTimerItem->dwTimeLeave);
				ASSERT(dwTimeLeave%TIMER_SPACE==0);
			}
		}

		//������Ӧ
		m_dwTimePass=0L;
		m_dwTimeLeave=dwTimeLeave;
	}

}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateTimerEngine(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CTimerEngine * pTimerEngine = NULL;
	try
	{
		pTimerEngine = new CTimerEngine();
		if (pTimerEngine == NULL) throw TEXT("����ʧ��");
		void * pObject = pTimerEngine->QueryInterface(Guid, dwInterfaceVer);
		if (pObject == NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pTimerEngine);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
