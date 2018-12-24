#pragma once
#ifndef __THREAD_ANIMATION_H_HEAD_
#define __THREAD_ANIMATION_H_HEAD_

#include "SkinControls.h"
#include <process.h>
#include <vector>
using namespace std;
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

class CThreadAnimationManager;

/// ��Ϣ��
class CMsgSonToParent
{
public:
	virtual int TranslateMsg(int iMainCode, int iAssCode, void* pData, int iDataLen) = 0;
};

/// �ص������ṹ
struct tagCallBack
{
	UINT			uID;			// ��ϢID
	WPARAM			wParam;			// ��Ϣ����
	LPARAM			lParam;			// ��Ϣ����

	// �ṹ�й��캯��������ı��������ٳ�ʼ��
	tagCallBack()
	{
		uID = 0;
		wParam = 0;
		lParam = 0;
	}

	tagCallBack(UINT uID, WPARAM wParam, LPARAM lParam)
	{
		this->uID = uID;
		this->wParam = wParam;
		this->lParam = lParam;
	}
};

/// �����߳���
class SKIN_CONTROL_CLASS CThreadAnimation : public CDialog
{
	friend class CThreadAnimationManager;
	DECLARE_DYNAMIC(CThreadAnimation)

public:
	CThreadAnimation();
	virtual ~CThreadAnimation();
	/// [@param filename in] ������Դ·��
	BOOL Create(CWnd* pParentWnd, UINT nID, const WCHAR* filename);

	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC *pDC);

public:
	enum	// modi by HouGuoJiang 2011-6-17
	{
		STATE_RUN=0,				// ������
		STATE_STOP_NO_CLOSE_WND=1,	// ֹͣ���������رմ���
		STATE_STOP_CLOSE_WND=2,		// ֹͣ�������رմ��ڣ����ڽ�������ʱ��
		
	};

protected:

	DECLARE_MESSAGE_MAP()

public:
	/// ���ûص�
	void SetMsgSonToParent(CMsgSonToParent* pMsgSonToParent, tagCallBack &callback);

	/// �̺߳���
	static unsigned __stdcall ThreadWorkFuc(LPVOID lParam);

	/// �滭�߳�
	//static unsigned __stdcall OnDraw(CThreadAnimation *p);
	unsigned OnDraw();	// ������ static , modi by HouGuoJiang 2011-6-17

	/// װ�붯����Դ
	HRESULT OnLoad(const WCHAR* filename);	
	/// ������Դ
	void OnClear();
	/// ��ʼ�����߳�
	unsigned int OnStart();

public:
	/// ���Ŷ���
	/// [@param iAllFramesPos in] ����֡λ��
	/// [@param iAllFramesTime in] ����֡ͣ��ʱ��
	/// [@param iFramesCount in] ֡����Ŀ
	/// [@param bStartFadeinout in] ��ʼ��������
	/// [@param bEndFadeinout in] ������������
	/// [@param iLoop in] ѭ�� 0 ��ʾ����ѭ����С��0��ѭ��������0ѭ���Ĵ���
	/// [@param iLoopStartIndex in] ѭ����ʼ��ͼƬ����
	void Play(POINT iAllFramesPos, int iAllFramesTime=30, int iFramesCount=1, 
		BOOL bStartFadeinout=FALSE, BOOL bEndFadeinout=FALSE, int iLoop=-1, int iLoopStartIndex=0);
	void Play(POINT iFramesPos[], int iFramesTime[], int iFramesCount=1
		, BOOL bStartFadeinout=FALSE, BOOL bEndFadeinout=FALSE, int iLoop=-1, int iLoopStartIndex=0);

	/// ֹͣ������ɾ�������Դ��
	/// [@param bMsgNoCallBack in] Ϊfalse ����������������Ϣ�ص�
	void Stop(bool bMsgCallBack=false, int iRunState=STATE_STOP_NO_CLOSE_WND);

	/// �ƶ�����λ��(����������ƶ�������û�и��������)
	void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint);
	void MoveWindow(LPCRECT lpRect, BOOL bRepaint);


private:
	CWnd					  * m_pParent;								///< ������ָ��
	CMsgSonToParent*			m_pMsgSonToParent;						///< �����ڵ�ר�ûص�ָ��
	UINT_PTR					m_uwaParam;								///< ��Ϣ����
	tagCallBack					m_callback;								///< �ص�����

	unsigned int				m_dwThreadID;							///< �߳�ID
	Image					  * m_pImage;								///< ��������
	BOOL						m_bStatFadeinout;						///< ��ʼ��������
	BOOL						m_bEndFadeinout;						///< ������������
	int							m_iLoop;								///< ѭ��
	int							m_iLoopStartIndex;					///< ѭ��ͼƬ����
	vector<int>					m_vFramesTime;							///< ֡ͣ��ʱ��
	vector<POINT>				m_vFramesPos;							///< ֡λ��
	int							m_iFramesCount;							///< ֡��Ŀ
	int							m_iWidth, m_iHeight;					///< ����
	int							m_uIndex;								///< ��������
	ULONG_PTR					m_gdiplus_Token;						///< GDI+��Դ	

	HANDLE						m_hThreadHandle;						///< �߳̾��
	HANDLE						m_hThreadEvent;							///< �����滭���¼�

	BLENDFUNCTION				m_Blend;
	typedef BOOL (WINAPI *MYFUNC)(HWND,HDC,POINT*,SIZE*,HDC,POINT*,COLORREF,BLENDFUNCTION*,DWORD);          
	MYFUNC						UpdateLayeredWindow;

	volatile long				m_iRunState;	// �ο� STATE_STOP_NO_CLOSE_WND
	CString						m_strFileName;
protected:
	virtual void PostNcDestroy();
};
typedef vector<CThreadAnimation *> PThreadAnimation;

// ��������
class SKIN_CONTROL_CLASS CThreadAnimationManager//: public CMsgSonToParent
{
public:
	~CThreadAnimationManager();
	/// ��ʼ��
	static void Init(CWnd * pParent);
	/// ��������ص�
	//virtual int TranslateMsg(int iMainCode, int iAssCode, void* pData, int iDataLen);
	/// ɾ�����ж�����Դ
	static void FreeAll();
	/// ���Ŷ���
	/// [filename in] ������Դ
	/// [iAllFramesPos in] ����֡λ��
	/// [iAllFramesTime in] ����֡ͣ��ʱ��
	/// [iFramesCount in] ֡����Ŀ
	/// [bStartFadeinout in] ��ʼ��������
	/// [bEndFadeinout in] ������������
	/// [iLoop in] ѭ�� 0 ��ʾ����ѭ����С��0��ѭ��������0ѭ���Ĵ���
	/// [iLoopStartIndex in] ѭ����ʼ��ͼƬ����
	static bool Play(const WCHAR* filename, POINT iAllFramesPos, int iAllFramesTime=30, int iFramesCount=1, 
		BOOL bStartFadeinout=FALSE, BOOL bEndFadeinout=FALSE, int iLoop=-1, int iLoopStartIndex=0);
	static bool Play(const WCHAR* filename, POINT iFramesPos[], int iFramesTime[], int iFramesCount=1
		, BOOL bStartFadeinout=FALSE, BOOL bEndFadeinout=FALSE, int iLoop=-1, int iLoopStartIndex=0);

private:
	PThreadAnimation							  m_AllAnimation;
	CWnd										* m_pParent;
	static CThreadAnimationManager				* m_staAnimationManager;

private:
	CThreadAnimationManager();
	/// ��ù�����ʵ��
	static CThreadAnimationManager * GetInstance();
};

#endif