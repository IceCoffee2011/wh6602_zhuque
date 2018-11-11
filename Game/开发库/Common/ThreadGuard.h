#ifndef THREAD_GUARD_H
#define THREAD_GUARD_H


// �ؼ������
class __declspec(dllexport) CCriticalSpection
{
public:
	CCriticalSpection(DWORD dwSpinCount=4000);
	~CCriticalSpection();
	
	// ���عؼ������
	CRITICAL_SECTION*	get_critical_section();

	// ��Ч��
	bool is_validate();

private:
	CRITICAL_SECTION	stCriticalSection;
	DWORD				m_dwSpinCount;		// �����Ĵ���
	bool				m_bInitState;		// true ��ʾ m_pCriticalSection ���� InitializeCriticalSectionAndSpinCount ״̬��false ��ʾm_pCriticalSection ���� DeleteCriticalSection ״̬
};

// �̼߳��������
class __declspec(dllexport) CThreadGuard
{
public:
	CThreadGuard(CCriticalSpection* pCriticalSection);

	~CThreadGuard(void);

	// ����ؼ������
	void enter();

	// �뿪�ؼ������
	void leave();

private:

	// ��ʼ�������ٶ����������Զ����У��ŵ㣺��һ�����ڽ���ǰreturn�˻��쳣�ˣ�
	// g_SampleCS ��ͨ�������������ٵ��ؼ������
	CCriticalSpection*	m_pCriticalSection;

	bool				m_bEnterState;		// true ��ʾ m_pCriticalSection ���� EnterCriticalSection ״̬��false ��ʾ m_pCriticalSection ���� LeaveCriticalSection ״̬

	
};

#endif
