// ThreadAnimation.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <WinUser.h>
#include "ThreadAnimation.h"
#include ".\threadanimation.h"

// CThreadAnimation

IMPLEMENT_DYNAMIC(CThreadAnimation, CWnd)
BEGIN_MESSAGE_MAP(CThreadAnimation, CWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

CThreadAnimation::CThreadAnimation()
{
	GdiplusStartupInput GdiplusStart;
	GdiplusStartup(&m_gdiplus_Token, &GdiplusStart, NULL );

	m_pImage = NULL;
	m_pMsgSonToParent = NULL;

	m_hThreadHandle = NULL;
	m_hThreadEvent = NULL;
	InterlockedExchange(&m_iRunState, STATE_STOP_NO_CLOSE_WND); 
}

CThreadAnimation::~CThreadAnimation()
{
	if (NULL != m_pImage)
	{
		SafeDelete(m_pImage);
		m_pImage = NULL;
	}

	GdiplusShutdown(m_gdiplus_Token);
}

BOOL CThreadAnimation::Create(CWnd* pParentWnd, UINT nID, const WCHAR* filename)
{
	ASSERT(NULL != filename);
	if (NULL == filename)
		return FALSE;

	OnLoad(filename);
	m_pParent = pParentWnd;
	m_strFileName = filename;

	m_Blend.BlendOp=AC_SRC_OVER;
	m_Blend.BlendFlags=0;
	m_Blend.AlphaFormat=AC_SRC_ALPHA ;
	m_Blend.SourceConstantAlpha=255;

	return CreateEx(NULL, "#32770", NULL, WS_POPUP, 
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, pParentWnd->GetSafeHwnd(), NULL);;
}

unsigned CThreadAnimation::OnDraw()
{
	CDC *pDC=GetDC();
	if (NULL == pDC)
		return 0;

	HDC hdcMemory=CreateCompatibleDC(pDC->GetSafeHdc());
	HBITMAP hBitMap=CreateCompatibleBitmap(pDC->GetSafeHdc(),m_iWidth/m_iFramesCount,m_iHeight);
	SelectObject(hdcMemory,hBitMap);
	Graphics graph(hdcMemory);

	Point points[] = { Point(0, 0), 
		Point(m_iWidth/m_iFramesCount, 0), 
		Point(0, m_iHeight),
	};

	graph.DrawImage(m_pImage, points, 3, m_iWidth/m_iFramesCount*m_uIndex, 0, m_iWidth/m_iFramesCount, m_iHeight,UnitPixel);
	HDC hdcScreen=::GetDC(GetSafeHwnd());
	DWORD dwExStyle=GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
	if((dwExStyle&0x80000)!=0x80000)
		SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, dwExStyle^0x80000);

	if (NULL == GetSafeHwnd())
		return 2;

	RECT rct;
	GetWindowRect(&rct);
	POINT ptWinPos={rct.left,rct.top};
	POINT ptSrc={0,0};
	SIZE sizeWindow={m_iWidth/m_iFramesCount, m_iHeight};
	BOOL bRet=FALSE;
	m_Blend.SourceConstantAlpha = 255;	

	// ��������
	if ((m_bStatFadeinout && 0==m_uIndex)  ||  (m_bEndFadeinout && m_uIndex+1==m_iFramesCount))
	{
		int iAlpha=255;
		if (0==m_uIndex && m_bStatFadeinout)
			iAlpha=0;
		int iTime=30;

		// ����һ֡����ʱ���Ӽ�Alpha�ж�
		bool bOneFramesAlpha = true;

		while (STATE_RUN==m_iRunState)
		{
			GetWindowRect(&rct);
			ptWinPos.x = rct.left;
			ptWinPos.y = rct.top;

			m_Blend.SourceConstantAlpha=iAlpha;				
			bRet= UpdateLayeredWindow(GetSafeHwnd(), hdcScreen, &ptWinPos, &sizeWindow, hdcMemory, &ptSrc, 0, &(m_Blend), 2);
			
			CString strBuffer;
			strBuffer.Format("CThreadAnimation::OnDraw  Alpah=%d", m_Blend.SourceConstantAlpha);
			OutputDebugString(strBuffer);
			Sleep(iTime);

			// �ı�Alphaֵ���ﵽ��������Ч��
			if (0==m_uIndex && m_bStatFadeinout && bOneFramesAlpha)
				iAlpha+=10;
			else
				iAlpha-=10;

			// ���������˳�
			if ((0==m_uIndex && 255<=iAlpha)  ||  (m_uIndex+1==m_iFramesCount && 0>=iAlpha))
			{
				// ����֡����
				if (1==m_iFramesCount)
				{
					// ���ײ��������ԣ���β���������ԣ�����޸�bOneFramesAlphaʹAlpha����
					bOneFramesAlpha = false;
					if ((255 <= iAlpha && !m_bEndFadeinout)  ||  0>=iAlpha)
						break;
					else
					{
						// ֻ��һ֡�����������ͣ����ʾʱ�䣬β���轥������Alphaֵ��ʼ��͸��
						Sleep(m_vFramesTime.at(m_uIndex));
						iAlpha = 255;
						continue;
					}
				}
				else
				{
					break;
				}
			}
		}
	}
	else
	{
		bRet= UpdateLayeredWindow(GetSafeHwnd(), hdcScreen, &ptWinPos, &sizeWindow, hdcMemory, &ptSrc, 0, &(m_Blend), 2);
		CString strBuffer;
		strBuffer.Format("CThreadAnimation::OnDraw  �ǽ������� Alpah=%d", m_Blend.SourceConstantAlpha);
		OutputDebugString(strBuffer);
	}

	m_uIndex++;
	graph.ReleaseHDC(hdcMemory);
	DeleteDC(hdcScreen);
	DeleteObject(hBitMap);
	DeleteDC(hdcMemory);
	hdcMemory=NULL;

	// modi by HouGuoJiang 2011-6-17
	ReleaseDC(pDC);	
	//pDC->DeleteDC();

	return 1;
}

int CThreadAnimation::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	m_uIndex = 0;
	m_dwThreadID = 0;
	m_hThreadHandle = NULL;
	m_hThreadEvent = NULL;

	m_bStatFadeinout = FALSE;
	m_bEndFadeinout = FALSE;
	m_iLoop = FALSE;
	m_iLoopStartIndex = 0;

	HINSTANCE hFuncInst = LoadLibrary("User32.DLL"); 
	BOOL bRet=FALSE;
	if(hFuncInst) 
	{
		UpdateLayeredWindow=(MYFUNC)GetProcAddress(hFuncInst, "UpdateLayeredWindow");
	}
	else
	{
		AfxMessageBox("User32.dll ERROR!");
		exit(1);
	}
	FreeLibrary(hFuncInst);

	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW/*|WS_EX_NOACTIVATE*/,0);
	return 0;
}

BOOL CThreadAnimation::OnEraseBkgnd(CDC *pDC)
{
	return TRUE;
}

unsigned __stdcall CThreadAnimation::ThreadWorkFuc(LPVOID lParam)
{
	int iFramesTime = 30;
	CThreadAnimation *pThreadAnimation = (CThreadAnimation *)lParam;
	if (NULL == pThreadAnimation  ||  NULL == pThreadAnimation->m_pParent)
		return 0;

	CRect rcParent;
	pThreadAnimation->m_pParent->GetWindowRect(&rcParent);
	pThreadAnimation->SetWindowPos(	&wndTop, 
		rcParent.left+pThreadAnimation->m_vFramesPos.at(pThreadAnimation->m_uIndex).x, 
		rcParent.top+pThreadAnimation->m_vFramesPos.at(pThreadAnimation->m_uIndex).y, 0, 0, 
		SWP_SHOWWINDOW|SWP_NOACTIVATE);
	pThreadAnimation->m_pParent->SetActiveWindow();

	while (STATE_RUN==pThreadAnimation->m_iRunState)
	{
		if (pThreadAnimation->m_uIndex >= pThreadAnimation->m_iFramesCount)
		{
			if (0 <= pThreadAnimation->m_iLoop)
			{
				if (1<pThreadAnimation->m_iLoop) // ѭ������
				{
					--pThreadAnimation->m_iLoop;
				}
				else if (1 == pThreadAnimation->m_iLoop) // ��ֹ��������ѭ��
				{
					pThreadAnimation->m_iLoop = -1;
				}

				pThreadAnimation->m_uIndex = pThreadAnimation->m_iLoopStartIndex;
				continue;
			}
			else
			{
				break; //����ǰ֡Ϊ���֡�� ����
			}
		}

		// ��ǰ֡ͣ��ʱ��
		iFramesTime = pThreadAnimation->m_vFramesTime.at(pThreadAnimation->m_uIndex);
		DWORD dwResult = WaitForSingleObject(pThreadAnimation->m_hThreadEvent, iFramesTime);

		if (WAIT_OBJECT_0 == dwResult)
		{
			TRACE("��Ӧ������Ϣ \n");
			break; //�������¼�)����
		}
		else if(WAIT_TIMEOUT == dwResult)
		{
			// ���ö������ڴ�С��λ��
			pThreadAnimation->OnDraw();
		}
	}

	// modi by HouGuoJiang 2011-6-17
	if(pThreadAnimation->m_uIndex == pThreadAnimation->m_iFramesCount)	// �Զ��������
	{
		if(STATE_RUN==pThreadAnimation->m_iRunState)	// STATE_RUN
		{
			InterlockedExchange(&pThreadAnimation->m_iRunState, STATE_STOP_NO_CLOSE_WND); // ֹͣ���������رմ���

			try
			{
				if (NULL != pThreadAnimation->GetSafeHwnd())
				{
					pThreadAnimation->ShowWindow(SW_HIDE);
				}
				pThreadAnimation->OnClear();

				//if(pThreadAnimation->m_pMsgSonToParent)
				//{
				//	pThreadAnimation->m_pMsgSonToParent->TranslateMsg(0, 0, 
				//		&(pThreadAnimation->m_callback), sizeof(tagCallBack));
				//}
			}
			catch (...)
			{
			}
		}
	}

	return 0;
}

HRESULT CThreadAnimation::OnLoad(const WCHAR* filename)
{
	if (NULL != m_pImage)
	{
		SafeDelete(m_pImage);
		m_pImage = NULL;
	}

	m_pImage = Image::FromFile(filename);
	if (NULL == m_pImage  ||  Ok != m_pImage->GetLastStatus())
	{
		CString strBuffer(filename);
		strBuffer.Insert(0, _T("��Դ·����"));
		strBuffer.Append(_T("\r\n��ȷ��������Դ���ڡ�\r\n\r\n�����ԡ����Գ��򣬡�ȡ������������"));
		int iResult = MessageBox(strBuffer, "��Ч��Դ", MB_ICONSTOP|MB_RETRYCANCEL);
		if (IDRETRY == iResult)
		{
			AfxDebugBreak();
		}
		return E_HANDLE;
	}

	m_iWidth = m_pImage->GetWidth();
	m_iHeight = m_pImage->GetHeight();
	return S_OK;
}

void CThreadAnimation::OnClear()
{
	if (NULL != m_hThreadEvent)
	{
		CloseHandle(m_hThreadEvent);
		m_hThreadEvent = NULL;
	}

	if (NULL != m_hThreadHandle)
	{
		CloseHandle(m_hThreadHandle);
		m_hThreadHandle = NULL;
	}

	m_uIndex = 0;
}

unsigned int CThreadAnimation::OnStart()
{
	if (NULL != m_hThreadEvent)
	{
		CloseHandle(m_hThreadEvent);
		m_hThreadEvent = NULL;
	}

	if (NULL != m_hThreadHandle)
	{
		CloseHandle(m_hThreadHandle);
		m_hThreadHandle = NULL;
	}

	m_hThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hThreadHandle = (HANDLE)::_beginthreadex(NULL, 0, &ThreadWorkFuc, (LPVOID)this, CREATE_SUSPENDED , &m_dwThreadID);
	if(m_hThreadHandle)
	{
		InterlockedExchange(&m_iRunState, STATE_RUN); // ����
		ResumeThread(m_hThreadHandle);
	}

	return m_dwThreadID;
}

void CThreadAnimation::Play(POINT iAllFramesPos, int iAllFramesTime/*=30*/, int iFramesCount/*=1*/, 
							BOOL bStartFadeinout/*=FALSE*/, BOOL bEndFadeinout/*=FALSE*/, int iLoop/*=-1*/, int iLoopStartIndex/*=0*/)
{
	ASSERT(0 < iFramesCount);
	if (0 >= iFramesCount)
		return;

	int *pAllFramesTime = new int[iFramesCount];
	POINT *pAllFramesPos = new POINT[iFramesCount];
	::memset(pAllFramesTime, 0, sizeof(int)*iFramesCount);
	::memset(pAllFramesPos, 0, sizeof(POINT)*iFramesCount);

	for (int i=0; i<iFramesCount; i++)
	{
		pAllFramesTime[i] = iAllFramesTime;
		pAllFramesPos[i].x = iAllFramesPos.x;
		pAllFramesPos[i].y = iAllFramesPos.y;
	}

	Play(pAllFramesPos, pAllFramesTime, iFramesCount, bStartFadeinout, bEndFadeinout, iLoop, iLoopStartIndex);

	SafeDeleteArray(pAllFramesTime);
	SafeDeleteArray(pAllFramesPos);
	pAllFramesTime = NULL;
	pAllFramesPos = NULL;
}

void CThreadAnimation::Play(POINT iFramesPos[]/*=NULL*/, int iFramesTime[]/*=NULL*/, int iFramesCount/*=1*/, 
							BOOL bStartFadeinout/*=FALSE*/, BOOL bEndFadeinout/*=FALSE*/, int iLoop/*=-1*/, int iLoopStartIndex/*=0*/)
{
	ASSERT(0 < iFramesCount);
	if (0 >= iFramesCount)
		return;
	if (STATE_RUN == m_iRunState)
		return;

	m_iFramesCount = iFramesCount;
	m_bStatFadeinout = bStartFadeinout;
	m_bEndFadeinout = bEndFadeinout;
	m_iLoop = iLoop;
	m_iLoopStartIndex = iLoopStartIndex;
	m_vFramesTime.clear();
	m_vFramesPos.clear();

	for (BYTE bIndex = 0; bIndex < iFramesCount; bIndex++)
	{
		m_vFramesTime.push_back(iFramesTime[bIndex]);
		m_vFramesPos.push_back(iFramesPos[bIndex]);
	}

	// �����߳̿�ʼ���ж���
	m_uIndex = 0;
	OnStart();
	return;
}

void CThreadAnimation::Stop(bool bMsgCallBack/*=false*/, int iRunState/*=STATE_STOP_NO_CLOSE_WND*/)
{
	// modi by HouGuoJiang 2011-6-17
	if(STATE_STOP_NO_CLOSE_WND==m_iRunState || STATE_STOP_CLOSE_WND==m_iRunState)
		return;

	InterlockedExchange(&m_iRunState, iRunState); 

	TRACE0("���ͽ����̵߳���Ϣ \n");

	SetEvent(m_hThreadEvent);
	DWORD dwResult = WaitForSingleObject(m_hThreadHandle, 5000);
	//DWORD dwResult = MsgWaitForMultipleObjects(1, &m_hThreadHandle, FALSE, 10000, QS_SENDMESSAGE);	// ��Ҫ������Σ�յķ�ʽ

	TRACE0("�߳̽�������ȫ��� \n");

	// modify by HouGuoJiang 2011-6-17
	if(m_pMsgSonToParent && bMsgCallBack)
	{
		m_pMsgSonToParent->TranslateMsg(0, 0, &m_callback, sizeof(tagCallBack));
	}

	try
	{
		if (NULL != GetSafeHwnd())
		{
			ShowWindow(SW_HIDE);
		}
		OnClear();
	}
	catch (...)
	{
	}
}

void CThreadAnimation::SetMsgSonToParent(CMsgSonToParent* pMsgSonToParent, tagCallBack &callback)
{
	::memcpy(&m_callback, &callback, sizeof(tagCallBack));
	m_pMsgSonToParent = pMsgSonToParent;
}

void CThreadAnimation::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint)
{
	if (m_vFramesPos.empty() || m_uIndex>=m_iFramesCount)
		return;

	__super::MoveWindow(x+m_vFramesPos.at(m_uIndex).x,  y+m_vFramesPos.at(m_uIndex).y,
		m_iWidth/m_iFramesCount, m_iHeight, bRepaint);
	return;
}

void CThreadAnimation::MoveWindow(LPCRECT lpRect, BOOL bRepaint)
{
	MoveWindow(lpRect->left, lpRect->top, lpRect->right-lpRect->left, lpRect->bottom-lpRect->top, bRepaint);
	return;
}

void CThreadAnimation::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	Stop(STATE_STOP_CLOSE_WND);
}

void CThreadAnimation::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	if (NULL != m_hWnd)
	{
		delete this;
	}
	CDialog::PostNcDestroy();
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// CThreadAnimationManager
CThreadAnimationManager * CThreadAnimationManager::m_staAnimationManager = NULL;

CThreadAnimationManager::CThreadAnimationManager()
{
	m_pParent = NULL;
}

CThreadAnimationManager::~CThreadAnimationManager()
{
	FreeAll();
}

void CThreadAnimationManager::Init(CWnd * pParent)
{
	GetInstance()->m_pParent = pParent;
}

//int CThreadAnimationManager::TranslateMsg(int iMainCode, int iAssCode, void* pData, int iDataLen)
//{
//	tagCallBack * pBuffer = (tagCallBack *)pData;
//	if (NULL != pBuffer && iDataLen==sizeof(tagCallBack))
//	{
//		CThreadAnimation * pBufferWnd = (CThreadAnimation *)pBuffer->wParam;
//		if (NULL != pBufferWnd)
//		{
//			for(PThreadAnimation::iterator vbeg=m_AllAnimation.begin(); vbeg<m_AllAnimation.end(); vbeg++)
//			{
//				CThreadAnimation * pVal = *vbeg;
//				if (NULL != pVal   &&   pVal==pBufferWnd)
//				{
//					try
//					{
//						delete pVal;
//					}
//					catch (...)
//					{
//						OutputDebugString("error:CThreadAnimationManager::TranslateMsg()");
//						//AfxMessageBox("ɾ���ڴ�ʧ��! --CThreadAnimationManager::TranslateMsg()");
//					}
//
//					m_AllAnimation.erase(vbeg);
//				}
//			}//end for
//		}
//	}
//
//	return 0;
//}

void CThreadAnimationManager::FreeAll()
{
	for(PThreadAnimation::iterator vbeg=GetInstance()->m_AllAnimation.begin(); 
		vbeg<GetInstance()->m_AllAnimation.end(); vbeg++)
	{
		CThreadAnimation * pVal = *vbeg;
		if (NULL != pVal)
		{
			if (NULL !=pVal->GetSafeHwnd())
			{
				pVal->Stop();
				pVal->DestroyWindow();
			}

			try
			{
				delete pVal;
			}
			catch (...)
			{
				OutputDebugString("error:CThreadAnimationManager::FreeAll()");
				//AfxMessageBox("ɾ���ڴ�ʧ��! --CThreadAnimationManager::FreeAll()");
			}
		}

		pVal = NULL;
		GetInstance()->m_AllAnimation.erase(vbeg);
	}

	GetInstance()->m_AllAnimation.clear();
}

bool CThreadAnimationManager::Play(const WCHAR* filename, POINT iAllFramesPos, int iAllFramesTime/*=30*/, int iFramesCount/*=1*/, 
								 BOOL bStartFadeinout/*=FALSE*/, BOOL bEndFadeinout/*=FALSE*/, int iLoop/*=-1*/, int iLoopStartIndex/*=0*/)
{
	if (NULL == GetInstance()->m_pParent)
	{
		return false;
	}

	// �������Ź������ٲ���
	CString strNameNew(filename);
	for(PThreadAnimation::iterator vbeg=GetInstance()->m_AllAnimation.begin(); 
		vbeg<GetInstance()->m_AllAnimation.end(); vbeg++)
	{
		CThreadAnimation * pVal = *vbeg;
		if (NULL != pVal  &&  NULL!=pVal->GetSafeHwnd() && FALSE==pVal->IsWindowVisible() &&
			0==strNameNew.CompareNoCase(pVal->m_strFileName))
		{
			pVal->Play(iAllFramesPos, iAllFramesTime, iFramesCount, bStartFadeinout, bEndFadeinout, iLoop, iLoopStartIndex);
			return true;
		}
	}

	CThreadAnimation * pVal = NULL;
	try
	{
		pVal = new CThreadAnimation;
	}
	catch (...)
	{
		OutputDebugString("error:CThreadAnimationManager::Play(const WCHAR* filename...)");
		//AfxMessageBox("�����ڴ�ʧ��!---CThreadAnimationManager::Play(const WCHAR* filename...)");
	}

	if (NULL != pVal)
	{
		pVal->Create(GetInstance()->m_pParent, GetInstance()->m_AllAnimation.size()+100, filename);
		//pVal->SetMsgSonToParent(this, tagCallBack(0, (WPARAM)pVal, 0));
		pVal->Play(iAllFramesPos, iAllFramesTime, iFramesCount, bStartFadeinout, bEndFadeinout, iLoop, iLoopStartIndex);
		GetInstance()->m_AllAnimation.push_back(pVal);
		return true;
	}

	return false;
}

bool CThreadAnimationManager::Play(const WCHAR* filename, POINT iFramesPos[], int iFramesTime[], int iFramesCount/*=1*/
								 , BOOL bStartFadeinout/*=FALSE*/, BOOL bEndFadeinout/*=FALSE*/, int iLoop/*=-1*/, int iLoopStartIndex/*=0*/)
{
	// ����֮ǰ���ŵĶ���
	if (NULL == GetInstance()->m_pParent)
	{
		return false;
	}

	CThreadAnimation * pVal = NULL;
	try
	{
		pVal = new CThreadAnimation;
	}
	catch (...)
	{
		OutputDebugString("error:CThreadAnimationManager::Play(const WCHAR* filename...[],[]...)");
		//AfxMessageBox("�����ڴ�ʧ��!---CThreadAnimationManager::Play(const WCHAR* filename...[],[]...)");
	}

	if (NULL != pVal)
	{
		pVal->Create(GetInstance()->m_pParent, GetInstance()->m_AllAnimation.size()+100, filename);
		//pVal->SetMsgSonToParent(this, tagCallBack(0, (WPARAM)pVal, 0));
		pVal->Play(iFramesPos, iFramesTime, iFramesCount, bStartFadeinout, bEndFadeinout, iLoop, iLoopStartIndex);
		GetInstance()->m_AllAnimation.push_back(pVal);
		return true;
	}

	return false;
}

CThreadAnimationManager * CThreadAnimationManager::GetInstance()
{
	if (NULL == m_staAnimationManager)
	{
		m_staAnimationManager = new CThreadAnimationManager;
	}

	ASSERT(NULL != m_staAnimationManager);
	return m_staAnimationManager;
}