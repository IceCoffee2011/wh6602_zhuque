#include "StdAfx.h"
#include ".\ThreadGuard.h"

// �ؼ������
CCriticalSpection::CCriticalSpection(DWORD dwSpinCount)
{
	m_bInitState = false;
	m_dwSpinCount = dwSpinCount;

	for(int i=0; i<10; i++)
	{
		if(!m_bInitState)	// ���� DeleteCriticalSection ״̬
		{
			#if 1
				// �ڶ��̡߳�release ״̬�£����� 1200��/ms���ٶȿ����
				if (InitializeCriticalSectionAndSpinCount(&stCriticalSection, m_dwSpinCount))
				{
					m_bInitState = true;
					return;	// �ɹ��Ϳ��Է�����
				}
			#else
				// �ڶ��̡߳�release ״̬�£����� 650��/ms ���ٶ���
				InitializeCriticalSection(&stCriticalSection);
				m_bInitState = true;
				return;
			#endif
		}

		// ��ʼ���ؼ������_ѭ���� ʧ��! ���ڴ治�������»ᷢ�����ִ��� 
		Sleep(200);
	}
};

CCriticalSpection::~CCriticalSpection()
{
	if(m_bInitState)	// ���� InitializeCriticalSectionAndSpinCount ״̬
	{
		DeleteCriticalSection(&stCriticalSection);	// ����ؼ������

		m_bInitState = false;
	}
};

// ���عؼ������
CRITICAL_SECTION* CCriticalSpection::get_critical_section()
{
	return &stCriticalSection;
}

// ��Ч��
bool CCriticalSpection::is_validate()
{
	return m_bInitState;
}

///////////////////////////////////////////////////////////////////////////////////

// �̼߳��������
CThreadGuard::CThreadGuard(CCriticalSpection* pCriticalSection)
{
	m_bEnterState = false;

	m_pCriticalSection = pCriticalSection;

	if(m_pCriticalSection->is_validate())
	{
		enter();
	}
}

CThreadGuard::~CThreadGuard(void)
{
	leave();
}

// ����ؼ������
void CThreadGuard::enter()
{
	if(m_pCriticalSection)
	{
		::EnterCriticalSection(m_pCriticalSection->get_critical_section());

		m_bEnterState = true;
	}
}

// �뿪�ؼ������
void CThreadGuard::leave()
{
	if(m_bEnterState && m_pCriticalSection)
	{
		::LeaveCriticalSection(m_pCriticalSection->get_critical_section());

		m_bEnterState = false;
	}
}