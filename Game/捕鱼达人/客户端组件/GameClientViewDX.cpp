#include "StdAfx.h"
#include "GameClientViewDX.h"
#include "Resource.h"
#include <mmsystem.h>
#include "Mathaide.h"
#include "Gameclientviewdx.h"
#include "GameClientDlg.h"
#include "GameHelpDlg.h"
#include "BulletHelpDlg.h"
#include ".\gameclientviewdx.h"

////////////////////////////////////////////////////////////////////////////////////
//ʱ�䶨��
#define	IDI_IDLE_TIME					4									//����ʱ��
#define	IDI_CHANGE_SCENE				5									//�л�����
#define	IDI_CHANGE_SCENE_EFFECT			6									//�л�����
#define	IDI_NO_BULLET_WARN				7									//�ӵ�����
#define	IDI_SHOOT_BULLET				8									//�����ӵ�

//ʱ�䶨��
#define	TIME_DX_CREATE_SOUNT			400									//��������
#define	TIME_DX_LOAD_IMAGE				200									//����ͼƬ
#define	TIME_CHANGE_SCENE_EFFECT		20									//����ͼƬ
#define	TIME_CHANGE_SCENE				5000								//�л�����
#define	TIME_SHOOT_BULLET				500									//�����ӵ�

#define	TIME_SHOOT_SPEED_F				400									//�����ٶ�
#define	TIME_SHOOT_SPEED_S				600									//�����ٶ�

//#define WM_FISH_CALC_TRACE				WM_USER+300							//�켣��Ϣ

#ifndef _DEBUG

#define	IDLE_TIME						120									//����ʱ��

#else

#define	IDLE_TIME						120									//����ʱ��

#endif

//��ť��ʶ
enum enViewButtonsID
{
	enViewButtonsID_Begin=200,

	IDC_SHOW_BULLET_SHOP,								//�����ӵ�
	IDC_QUIT_GAME,										//�˳���Ϸ
	IDC_ACCOUNTS_WND,									//�˺Ŵ���
	IDC_ACCOUNTS_WND_1,									//�˺Ŵ���
	IDC_ACCOUNTS_WND_2,									//�˺Ŵ���
	IDC_ACCOUNTS_WND_3,									//�˺Ŵ���
	IDC_ACCOUNTS_WND_4,									//�˺Ŵ���
	IDC_SHOW_GAME_HELP,									//��Ϸ����

	enViewButtonsID_End
};

////////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientViewDX, CDxDialog)

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()

	ON_MESSAGE(WM_FISH_DISAPPEAR, OnFishDisappear)
	ON_MESSAGE(WM_FISH_CALC_TRACE, OnFinishCalcTrace)
	ON_MESSAGE(WM_NET_OPEN, OnNetOpen)

	ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


//�����߳�
void ThreadCalcTrace(LPVOID pThreadData)
{
	//Ч�����
	ASSERT(pThreadData!=NULL);
	if(pThreadData==NULL) return;

	//����ת��
	CGameClientViewDX *pGameClientViewDX=(CGameClientViewDX *)pThreadData;

	//����켣
	while (true)
	{
		//�ȴ��ź�
		WaitForSingleObject(pGameClientViewDX->m_hEventFinish,INFINITE);

		//�˳��ж�
		if (true==pGameClientViewDX->m_bExist) break;

		//��ʼ����
		while (pGameClientViewDX->m_FishArrayBuffer.GetCount()>0)
		{
			//��ȡ����
			tagFishTraceInfo *pFishTraceInfo=&pGameClientViewDX->m_FishArrayBuffer[0];
			tagFishInfo *pFishInfo=pFishTraceInfo->pFishInfo;
			if (NULL==pFishInfo)
			{
				//	ASSERT(FALSE);
				TraceString(TEXT("if (NULL==pFishInfo)"));
				pGameClientViewDX->m_FishArrayBuffer.RemoveAt(0);
				continue;
			}
			if (true==pGameClientViewDX->m_bExist) break;

			//��ʼ����
			pFishInfo->wTracePtIndex=0;
			pFishInfo->byFrameIndex=0;
			pFishInfo->FishStatus=enFishStatus_Alive;
			pFishInfo->dwMoveTimes=GetTickCount();

			//����켣
			CMathAide::BuildLinear(pFishTraceInfo->nInitX, pFishTraceInfo->nInitY, pFishTraceInfo->nInitCount, pFishInfo->ptTrace, CountArray(pFishInfo->ptTrace), pFishInfo->wTracePtCount, pFishTraceInfo->byMoveSpace);

			//�Ϸ��ж�
			if (pFishInfo->wTracePtCount<2) 
			{
				continue;
			}

			//�׸��Ƕ�
			LONG lDistance=CMathAide::CalcDistance(pFishInfo->ptTrace[pFishInfo->wTracePtCount-1].x,pFishInfo->ptTrace[pFishInfo->wTracePtCount-1].y,pFishInfo->ptTrace[0].x,pFishInfo->ptTrace[0].y);
			if (lDistance<=0) continue;
			float fSinValue=(pFishInfo->ptTrace[pFishInfo->wTracePtCount-1].x-pFishInfo->ptTrace[0].x)/(lDistance*1.0f);
			pFishInfo->fAngle=acos(fSinValue);

			//�����Ƕ�
			if (pFishInfo->ptTrace[pFishInfo->wTracePtCount/2-1].y<pFishInfo->ptTrace[0].y) 
			{
				pFishInfo->fAngle=2*D3DX_PI-pFishInfo->fAngle;
			}
			pFishInfo->fAngleStep=0.f;

			//�˳��ж�
			if (true==pGameClientViewDX->m_bExist) break;
			if (true==pGameClientViewDX->m_bResetArray) break;
			if (pGameClientViewDX->m_FishArrayBuffer.GetCount()==0) break;			

			//�Ƴ�����
			pGameClientViewDX->m_FishArrayActive.Add(pFishInfo);
			pGameClientViewDX->m_FishArrayBuffer.RemoveAt(0);

			//���ñ���
			PostMessage(pGameClientViewDX->GetSafeHwnd(),WM_FISH_CALC_TRACE,0,0);
		}

		//�˳��ж�
		if (true==pGameClientViewDX->m_bExist) break;
		else ResetEvent(pGameClientViewDX->m_hEventFinish);
	}

	//�رվ��
	CloseHandle(pGameClientViewDX->m_hEventFinish);
	pGameClientViewDX->m_hEventFinish=NULL;

	return;
}

//////////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientViewDX::CGameClientViewDX(VOID)
{
	//�û���Ϣ
	ZeroMemory(m_lBulletCount,sizeof(m_lBulletCount));
	ZeroMemory(m_lCoinCount,sizeof(m_lCoinCount));
	ZeroMemory(m_lBulletCharge,sizeof(m_lBulletCharge));
	ZeroMemory(m_wCptrFishCount,sizeof(m_wCptrFishCount));
	m_bLookonMode=false;

	//��Ϸ����
	m_bReleaseKey=true;
	m_bWaitScoreInfo=false;
	m_bInitControlTag=false;
	m_dwDelayTimes=8;
	m_dwCurTimes=GetTickCount();
	m_wMeChairID=INVALID_CHAIR;
	m_dwLastShootTime=GetTickCount();
	m_wIdleTime=IDLE_TIME;
	m_CurSeaScene=enSeaSceneKind_No;
	m_bNoBulletWarn=false;
	m_dwNoBulletWarnTime=0L;
	for (int i=0; i<GAME_PLAYER; ++i) m_lCellScore[i]=1;
	m_dwLastCalcTime=GetTickCount();
	m_dwLastSwitchTime=GetTickCount();

	//�ʽ����
	m_lBonusCount=0L;

	//�ǳ�����
	m_nSceneMovePosX=0;
	m_NewSeaScene=enSeaSceneKind_1;
	m_bWaveLToR=true;
	m_bWaveEffect=false;
	m_bWaveComing=false;

	//�ں˱���
	m_hEventFinish=NULL;
	m_hCalcThread=NULL;
	m_bExist=false;
	m_bResetArray=false;
	m_bDeviceReset=false;

	//��Դ����
	m_pFontUserInfo=NULL;
	m_pFontGameInfo=NULL;

	//�������
	m_pGameClientDlg=NULL;

	//�̱߳���
	m_bLoadResFinish=false;

	///////////////////////////////////////////////
	/*m_CurSeaScene=enSeaSceneKind_3;
	m_wMeChairID=1;*/
	///////////////////////////////////////////////
}

//��������
CGameClientViewDX::~CGameClientViewDX(VOID)
{
	ReleaseScene();
}

//�ͷ���Դ
VOID CGameClientViewDX::ReleaseScene()
{
	//���ñ�ʶ
	m_bExist=true;
	if (NULL!=m_hEventFinish) SetEvent(m_hEventFinish);

	//�����߳�
	WaitForSingleObject(m_hCalcThread,2000);
	CloseHandle(m_hCalcThread);

	//�ͷ�ͼƬ
	m_ToolBarBk.Release();
	m_Cannon.Release();
	m_Bullet.Release();
	m_Coin.Release();
	m_Bonus.Release();
	m_UserInfo.Release();
	m_Number.Release();
	if (m_Wave.IsLoaded()==true) m_Wave.Release();
	m_NoBulletWarn.Release();
	m_WaveEffect.Release();
	m_GameTip.Release();
	for (enSeaSceneKind SeaSceneKind=enSeaSceneKind_1; SeaSceneKind<enSeaSceneKind_No; SeaSceneKind=enSeaSceneKind(SeaSceneKind+1))
	{
		if (m_SeaScene[SeaSceneKind].IsLoaded()==true) m_SeaScene[SeaSceneKind].Release();
	}

	//�ͷ���Դ
	SAFE_RELEASE(m_pFontUserInfo);
	SAFE_RELEASE(m_pFontGameInfo);

	//�ͷ���Ⱥ
	for (enFishKind FishKind=enFishKind_1; FishKind<enFishKind_No; FishKind=enFishKind(FishKind+1))
	{
		g_FishGroup[FishKind].Release();
		g_FishGroup[FishKind].ResetKenel();
	}

	return ;
}

//������Դ
VOID CGameClientViewDX::InitScene(INT nWidth,INT nHeight)
{
	//�Լ���λ
	m_Cannon.SetMeChairID(m_wMeChairID);

	//����λ��
	InitPosition();

	//��ʼ����
	m_FishArrayBuffer.RemoveAll();
	m_FishArrayActive.RemoveAll();

	//���ñ�ʶ
	m_bResetArray=false;

	//��������
	SAFE_RELEASE(m_pFontUserInfo);
	SAFE_RELEASE(m_pFontGameInfo);	
	D3DXCreateFont(g_pD3DDevice,14,0,0,1,0,ANSI_CHARSET,0,0,0,TEXT("������"),&m_pFontUserInfo);
	D3DXCreateFont(g_pD3DDevice,18,0,0,0,0,DEFAULT_CHARSET,0,0,0,TEXT("������"),&m_pFontGameInfo);

	//��ʼ��Ⱥ
	for (enFishKind FishKind=enFishKind_1; FishKind<enFishKind_No; FishKind=enFishKind(FishKind+1))
	{
		g_FishGroup[FishKind].SetGroupInfo(this->GetSafeHwnd(),FishKind);
	}

	//������Դ
	DWORD dwStart = GetTickCount();

	//������Ⱥ
	LoadFish();

	//����ͼƬ	
	if (!m_bInitControlTag) 
	{
		LoadDXImage();
		LoadSeaScene(m_CurSeaScene);
	}
	else 
	{
		OnResetDevice();
	}

	//��ʼ�ؼ�
	if (!m_bInitControlTag)
	{
		//���ñ���
		m_bInitControlTag = true;
	}

	//��Դλ��
	m_ToolBarBk.SetDrawPos(nWidth/2-m_ToolBarBk.GetImageWidth()/2,nHeight-m_ToolBarBk.GetImageHeight());
	m_Cannon.SetNotifyWnd(GetSafeHwnd());

	//�ӵ�����
	CRect rcBound;
	GetClientRect(rcBound);
	rcBound.DeflateRect(20,20,20,20);
	m_Bullet.SetNotifyWnd(GetSafeHwnd());
	m_Bullet.SetBound(rcBound);

	//�����߳�
	if (NULL==m_hCalcThread)
	{
		UINT uThreadID=0;
		m_hEventFinish=CreateEvent(NULL,FALSE,FALSE,NULL);
		m_hCalcThread=(HANDLE)::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadCalcTrace,LPVOID(this),0,0);		
	}

	return ;
}

//���ű�־
VOID CGameClientViewDX::EnablePlaySound(bool bEnable)
{
	return ;
}

//��������
VOID CGameClientViewDX::DrawTextString(LPD3DXFONT pFont, LPCWSTR pszString, D3DCOLOR crText, int nXPos, int nYPos)
{
	//�滭����
	RECT Rect;
	Rect.left = nXPos;	
	Rect.top = nYPos;
	Rect.right = 0;	Rect.bottom = 0;
	pFont->DrawTextW(NULL, pszString, (int)wcslen(pszString), &Rect,DT_SINGLELINE|DT_NOCLIP|DT_LEFT|DT_TOP, crText|0xFF000000);

	return;
}

//��Ϸ��Ϣ
void CGameClientViewDX::DrawGameInfo()
{
	//��ȡ��С
	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.top=160;

	//��������
	static WCHAR szBuffer[128]={0};
	static CRect ImageRect;

	//�˳���ʾ
	if (m_wIdleTime>0 && m_wIdleTime<=60)
	{
		//�����Ϣ
		_snwprintf(szBuffer,CountArray(szBuffer),L"����60��û���ڣ����򼴽��˳���(%d)",m_wIdleTime);
		m_pFontGameInfo->DrawTextW(NULL, szBuffer, (int)wcslen(szBuffer), &rcClient,DT_SINGLELINE|DT_NOCLIP|DT_CENTER|DT_TOP|DT_CALCRECT, RGB(0,255,255)|0xFF000000);

		//�������
		ImageRect.left=0;
		ImageRect.top=0;
		ImageRect.right=m_GameTip.GetImageWidth(0);
		ImageRect.bottom=m_GameTip.GetImageHeight(0);
		rcClient.left-=10;
		rcClient.right+=10;
		rcClient.bottom+=2*rcClient.Height();
		m_GameTip.DrawRotateAndZoom(rcClient.left+10,rcClient.top-rcClient.Height()/3,ImageRect,0.f,rcClient.Width()/float(ImageRect.Width()),rcClient.Height()/float(ImageRect.Height()),0,enXLeft,enYTop);

		//�����Ϣ
		m_pFontGameInfo->DrawTextW(NULL, szBuffer, (int)wcslen(szBuffer), &rcClient,DT_SINGLELINE|DT_NOCLIP|DT_CENTER|DT_TOP, RGB(0,255,255)|0xFF000000);
	}

	//�ǳ�֪ͨ
	if (true==m_bWaveComing)
	{

		//�����Ϣ
		_snwprintf(szBuffer,CountArray(szBuffer),L"��ˮ�������٣���Ⱥ�������֣���λ�������Ϻ��ӵ�����");
		m_pFontGameInfo->DrawTextW(NULL, szBuffer, (int)wcslen(szBuffer), &rcClient,DT_SINGLELINE|DT_NOCLIP|DT_CENTER|DT_TOP|DT_CALCRECT, RGB(0,255,255)|0xFF000000);

		//�������
		ImageRect.left=0;
		ImageRect.top=0;
		ImageRect.right=m_GameTip.GetImageWidth(0);
		ImageRect.bottom=m_GameTip.GetImageHeight(0);
		rcClient.left-=10;
		rcClient.right+=10;
		rcClient.bottom+=2*rcClient.Height();
		m_GameTip.DrawRotateAndZoom(rcClient.left+85,rcClient.top-rcClient.Height()/3,ImageRect,0.f,rcClient.Width()/float(ImageRect.Width()),rcClient.Height()/float(ImageRect.Height()),0,enXLeft,enYTop);

		//�����Ϣ
		m_pFontGameInfo->DrawTextW(NULL, szBuffer, (int)wcslen(szBuffer), &rcClient,DT_SINGLELINE|DT_NOCLIP|DT_CENTER|DT_TOP, RGB(0,255,255)|0xFF000000);
	}

	//��ȡ����
	if (true==m_bWaitScoreInfo)
	{
		//�����Ϣ
		_snwprintf(szBuffer,CountArray(szBuffer),L"���ڻ�ȡ��Ϣ�����Ժ󡭡�");
		m_pFontGameInfo->DrawTextW(NULL, szBuffer, (int)wcslen(szBuffer), &rcClient,DT_SINGLELINE|DT_NOCLIP|DT_CENTER|DT_TOP|DT_CALCRECT, RGB(0,255,255)|0xFF000000);

		//�������
		ImageRect.left=0;
		ImageRect.top=0;
		ImageRect.right=m_GameTip.GetImageWidth(0);
		ImageRect.bottom=m_GameTip.GetImageHeight(0);
		rcClient.left-=10;
		rcClient.right+=10;
		rcClient.bottom+=2*rcClient.Height();
		m_GameTip.DrawRotateAndZoom(rcClient.left-30,rcClient.top-rcClient.Height()/3,ImageRect,0.f,rcClient.Width()/float(ImageRect.Width()),rcClient.Height()/float(ImageRect.Height()),0,enXLeft,enYTop);

		//�����Ϣ
		m_pFontGameInfo->DrawTextW(NULL, szBuffer, (int)wcslen(szBuffer), &rcClient,DT_SINGLELINE|DT_NOCLIP|DT_CENTER|DT_TOP, RGB(0,255,255)|0xFF000000);
	}
}

//��������
//VOID CGameClientViewDX::UpdateControl()
//{
//	if(&g_UserControl==NULL) return;
//	if (INVALID_CHAIR==m_wMeChairID) return;
//
//	//�Թ��ж�
//	if (true==m_bLookonMode) return;
//
//	//�����ж�
//	CWnd *pActiveWnd=GetActiveWindow();
//	CWnd *pMainFrame=AfxGetMainWnd();
//	if (NULL==pActiveWnd) return;
//	if (NULL==pMainFrame) return;
//	if (pActiveWnd->GetSafeHwnd()!=pMainFrame->GetSafeHwnd()) return;
//
//	//��������
//	g_UserControl.UpdateInput();
//
//	//������Ϣ
//	WORD wKeyNumberIndex[]={DIK_1,DIK_2,DIK_3,DIK_4,DIK_5,DIK_SPACE,DIK_LEFT,DIK_RIGHT,DIK_UP,DIK_DOWN};
//	for(WORD i=0;i<CountArray(wKeyNumberIndex);i++)
//	{
//		int nIndex=wKeyNumberIndex[i];
//		if(nIndex<CountArray(g_UserControl.m_cbKeyState) && g_UserControl.m_cbKeyState[nIndex]&0x80)
//		{
//			//�����ж�
//			if (i<=4)
//			{
//				LONG nMinCount=min((i+1),GetCurBulletCount(m_wMeChairID));
//				m_Cannon.SetBulletCount(m_wMeChairID,nMinCount,true);
//				
//				//��������
//				PlayMp3Sound(TEXT("Effect\\SwitchSilo.mp3"),false);
//			}
//			else if (DIK_SPACE==wKeyNumberIndex[i] || DIK_UP==wKeyNumberIndex[i])
//			{
//				//�ո��ж�
//				if (DIK_SPACE==wKeyNumberIndex[i])
//				{
//					//�����ж�
//					CWnd *pFocusWnd=GetFocus();
//					if (NULL==pFocusWnd) return;
//					if (pFocusWnd->GetSafeHwnd()!=GetSafeHwnd()) return;
//				}
//
//				//�����ж�
//				UserShoot(m_wMeChairID);
//			}
//			else if (DIK_LEFT==wKeyNumberIndex[i])
//			{
//				//���ý���
//				SetFocus();
//
//				//���ýǶ�
//				float fNowAngle=m_Cannon.GetAngleCan(m_wMeChairID);
//				fNowAngle-=0.02f;
//				if (fNowAngle<ME_MIN_CAN_ANGLE) fNowAngle=ME_MIN_CAN_ANGLE;
//				m_Cannon.SetAngleCan(m_wMeChairID,fNowAngle);
//			}
//			else if (DIK_RIGHT==wKeyNumberIndex[i])
//			{
//				//���ý���
//				SetFocus();
//
//				//���ýǶ�
//				float fNowAngle=m_Cannon.GetAngleCan(m_wMeChairID);
//				fNowAngle+=0.02f;
//				if (fNowAngle>ME_MAX_CAN_ANGLE) fNowAngle=ME_MAX_CAN_ANGLE;
//				m_Cannon.SetAngleCan(m_wMeChairID,fNowAngle);
//			}
//			else if (DIK_DOWN==wKeyNumberIndex[i])
//			{
//				//�л��ڵ�
//				if (m_dwLastSwitchTime+300<=GetTickCount())
//				{
//					SwitchCannon();	
//					m_dwLastSwitchTime=GetTickCount();
//				}
//			}			
//		}
//	}
//
//	return;
//}

DWORD g_dwCurTimes=0;;

//�滭����
VOID CGameClientViewDX::DrawGameView2D()
{
	//�Ϸ��ж�
	if (false==m_bLoadResFinish) return;
	if (true==m_bDeviceReset) return;
	if (m_wMeChairID>=GAME_PLAYER) return;

	//ʱ���ж�
	if((m_dwCurTimes+m_dwDelayTimes)>= GetTickCount()) return;
	m_dwCurTimes=GetTickCount();

	//DWORD dwBeginTime= timeGetTime();

	//��ʼ�滭
	if (CDxSkin::m_Sprite!=NULL) CDxSkin::m_Sprite->Begin(D3DXSPRITE_ALPHABLEND|D3DXSPRITE_SORT_TEXTURE);

	//�滭����
	if (m_CurSeaScene<enSeaSceneKind_No) m_SeaScene[m_CurSeaScene].DrawScene();

	//�����ж�
	if (true==m_bWaveEffect)
	{
		//��ȡ��С
		CRect rcClient;
		GetClientRect(&rcClient);

		//ƫ��λ��
		int nExcursionX=(rcClient.Width()-m_SeaScene[m_NewSeaScene].GetImageWidth())/2;
		int nExcursionY=(rcClient.Height()-m_ToolBarBk.GetImageHeight()+24-m_SeaScene[m_NewSeaScene].GetImageHeight())/2;

		//�����ж�
		if (true==m_bWaveLToR)
		{
			//�滭����
			CRect rcDraw;
			if (true==m_SeaScene[m_NewSeaScene].IsReverse())
			{
				rcDraw.SetRect(m_SeaScene[m_NewSeaScene].GetImageWidth()-m_nSceneMovePosX,0,m_SeaScene[m_NewSeaScene].GetImageWidth(),m_SeaScene[m_NewSeaScene].GetImageHeight());
				m_SeaScene[m_NewSeaScene].DrawRotateAndZoom(nExcursionX+rcDraw.Width(),nExcursionY+rcDraw.Height(),rcDraw,D3DX_PI,1.f,1.f,0,enXLeft,enYTop);
			}
			else
			{
				rcDraw.SetRect(0,0,m_nSceneMovePosX,m_SeaScene[m_NewSeaScene].GetImageHeight());
				m_SeaScene[m_NewSeaScene].DrawSprite(nExcursionX,nExcursionY,rcDraw);
			}

			//�滭����
			rcDraw.SetRect(0,0,m_Wave.GetImageWidth(),m_Wave.GetImageHeight());
			m_Wave.DrawRotateAndZoom(m_nSceneMovePosX+nExcursionX+30,nExcursionY+m_Wave.GetImageHeight(),rcDraw,D3DX_PI,1.f,1.f,0,enXLeft,enYTop);
		}
		else
		{
			//�滭����
			CRect rcClient;
			GetClientRect(&rcClient);
			CRect rcDraw;
			if (true==m_SeaScene[m_NewSeaScene].IsReverse())
			{
				rcDraw.SetRect(0,0,m_nSceneMovePosX,m_SeaScene[m_NewSeaScene].GetImageHeight());
				m_SeaScene[m_NewSeaScene].DrawRotateAndZoom(rcClient.right-m_nSceneMovePosX-nExcursionX+rcDraw.Width(),nExcursionY+rcDraw.Height(),rcDraw,D3DX_PI,1.f,1.f,0,enXLeft,enYTop);
			}
			else
			{
				rcDraw.SetRect(rcClient.right-m_nSceneMovePosX,0,m_SeaScene[m_NewSeaScene].GetImageWidth(),m_SeaScene[m_NewSeaScene].GetImageHeight());
				m_SeaScene[m_NewSeaScene].DrawRotateAndZoom(rcClient.right-m_nSceneMovePosX-nExcursionX,nExcursionY,rcDraw,0.f,1.f,1.f,0,enXLeft,enYTop);
			}

			//�滭����
			rcDraw.SetRect(0,0,m_Wave.GetImageWidth(),m_Wave.GetImageHeight());
			m_Wave.DrawRotateAndZoom(rcClient.right-m_nSceneMovePosX-nExcursionX-30,nExcursionY,rcDraw,0.f,1.f,1.f,0,enXLeft,enYTop);
		}		
	}

	//�滭��Ⱥ
	for (enFishKind FishKind=enFishKind_1; FishKind<enFishKind_No; FishKind=enFishKind(FishKind+1))
	{
		g_FishGroup[FishKind].DrawFish();
	}

	//�����ӵ�
	m_Bullet.DrawBullet();

	//�滭����
	m_Cannon.DrawCannon();

	//�滭���
	m_Coin.DrawCoin();

	//�滭�ʽ�
	m_Bonus.DrawBonus();

	//ˮ��Ч��
	bool static bFlag=true;
	static DWORD dwTime=GetTickCount();
	static float fScaling=4.0f;
	if (dwTime+200<=GetTickCount())
	{
		dwTime=GetTickCount();

		if (false==bFlag)
		{
			if (fScaling<4.2f) fScaling+=0.01f;
			if (fScaling>=4.2f) bFlag=true;
		}
		else
		{
			if (fScaling>4.0f) fScaling-=0.01f;
			if (fScaling<=4.0f) bFlag=false;
		}	
	}

	BYTE byChannel=190;
	CRect drawRect;
	drawRect.left=0;
	drawRect.top=0;
	drawRect.right=m_WaveEffect.GetImageWidth(0);
	drawRect.bottom=m_WaveEffect.GetImageHeight(0);
	m_WaveEffect.DrawRotateAndZoom(205,205,drawRect,0.f,fScaling,fScaling,0,enXLeft,enYTop,RGB(byChannel,byChannel,byChannel));

	//�ȴ�����
	if (true==m_bWaitScoreInfo)
	{
		drawRect.left=0;
		drawRect.top=0;
		drawRect.right=m_WaveEffect.GetImageWidth(0);
		drawRect.bottom=m_WaveEffect.GetImageHeight(0);
		m_WaveEffect.DrawRotateAndZoom(205,205,drawRect,0.f,fScaling,fScaling,0,enXLeft,enYTop,RGB(byChannel,byChannel,byChannel));
	}	

	//������Ϣ
	DrawUserInfo();

	//������
	m_ToolBarBk.OnDrawImage();

	//�滭�ʽ�
	if (m_lBonusCount>0L)
	{
		//�����Ϣ
		static WCHAR szBuffer[128]={0};
		CRect rcClient(398,569,642,590);
		_snwprintf(szBuffer,CountArray(szBuffer),L"�ƽ����㣺%ld �ʽ�",m_lBonusCount);
		m_pFontGameInfo->DrawTextW(NULL, szBuffer, (int)wcslen(szBuffer), &rcClient,DT_SINGLELINE|DT_NOCLIP|DT_CENTER|DT_VCENTER, RGB(0,255,255)|0xFF000000);
	}

	//����ͼƬ
	if (true==m_bNoBulletWarn)
	{
		static bool bShow=true;
		if (true==bShow) m_NoBulletWarn.OnDrawImage();
		if (m_dwNoBulletWarnTime+800<GetTickCount())
		{
			bShow=!bShow;
			m_dwNoBulletWarnTime=GetTickCount();
		}
	}

	//��Ϸ��Ϣ
	DrawGameInfo();

	//�����滭
	if (CDxSkin::m_Sprite!=NULL) CDxSkin::m_Sprite->End();

	/*DWORD dwEndTime= timeGetTime();

	if ((dwEndTime-dwBeginTime)!=0L)
	{
	TCHAR szBuffer[256];
	m_pGameClientDlg->InsertSystemString(_itoa((dwEndTime-dwBeginTime),szBuffer,10));
	}*/

	return;
}

//�Ҽ���Ϣ
void CGameClientViewDX::OnRButtonDown(UINT nFlags, CPoint point)
{
	//�л��ڵ�
	SwitchCannon();	

	__super::OnRButtonDown(nFlags, point);
}

//������
VOID CGameClientViewDX::OnLButtonDown(UINT nFlags, CPoint Point)
{
	//���ø���
	__super::OnLButtonDown(nFlags, Point);

	//�Ϸ��ж�
	if (INVALID_CHAIR==m_wMeChairID) return;

	//�Թ��ж�
	if (true==m_bLookonMode) return;

	//���ý���
	SetFocus();

	//������
	if (true == _UserShoot(m_wMeChairID))
	{
		SetTimer(IDI_SHOOT_BULLET,TIME_SHOOT_BULLET,NULL);
	}
	else
	{
		char sz[100]={0};
		OutputDebugString("false == _UserShoot");
	}

	//���ò���
	SetCapture();

	return ;
}

//������
VOID CGameClientViewDX::OnLButtonUp(UINT nFlags, CPoint point)
{
	__super::OnLButtonUp(nFlags, point);

	//�رն�ʱ
	KillTimer(IDI_SHOOT_BULLET);

	//�ͷŲ���
	ReleaseCapture();

	return ;
}

//����ƶ�
VOID CGameClientViewDX::OnMouseMove(UINT nFlags, CPoint Point)
{
	__super::OnMouseMove(nFlags, Point);

	//�Ϸ��ж�
	if (INVALID_CHAIR==m_wMeChairID) return;

	//�Թ��ж�
	if (true==m_bLookonMode) return;

	//��ȡ���
	CPoint ptCusor;
	::GetCursorPos(&ptCusor);
	ScreenToClient(&ptCusor);

	//��׼λ��
	POINT ptBenchmarkPos={0};
	ptBenchmarkPos=m_Cannon.GetBenchmarkPos(m_wMeChairID);

	//����Ƕ�
	LONG lDistance=CMathAide::CalcDistance(ptCusor.x,ptCusor.y,ptBenchmarkPos.x,ptBenchmarkPos.y);
	if (lDistance<=0) return;
	float fSinValue=(ptCusor.x-ptBenchmarkPos.x)/(lDistance*1.0f);
	float fAngle=acos(fSinValue);

	//�����Ƕ�
	if (ptCusor.y<ptBenchmarkPos.y)
	{
		fAngle=2*D3DX_PI-fAngle;
	}
	fAngle+=D3DX_PI/2;

	//���ýǶ�
	m_Cannon.SetAngleCan(m_wMeChairID,fAngle);
	//for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	//{
	//	if (IsSameSide(wChairID,m_wMeChairID)) 
	//		m_Cannon.SetAngleCan(wChairID,fAngle);
	//	else
	//		m_Cannon.SetAngleCan(wChairID,SwitchAngle(fAngle));
	//}

	return ;
}

//��ʱ��Ϣ
void CGameClientViewDX::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case IDI_SHOOT_BULLET:				//�����ӵ�
		{
			if (false==_UserShoot(m_wMeChairID,false) && (m_dwLastShootTime+TIME_SHOOT_SPEED_F)>GetTickCount())
			{
				KillTimer(IDI_SHOOT_BULLET);
			}

			return;
		}
	case IDI_IDLE_TIME:					//����ʱ��
		{
			//�Ϸ��ж�
			if (INVALID_CHAIR==m_wMeChairID && m_wIdleTime<60)
			{
				KillTimer(IDI_IDLE_TIME);
				return;
			}

			//�ر��ж�
			if (m_wIdleTime>0) m_wIdleTime-=1;
			else AfxGetMainWnd()->PostMessage(WM_QUIT_GAME,0,0);

			break;
		}
	case IDI_CHANGE_SCENE_EFFECT:			//�ǳ�����
		{
			//�޸�λ��
			m_nSceneMovePosX+=10;

			//�����ж�
			CRect rcClient;
			GetClientRect(&rcClient);
			if (m_nSceneMovePosX>=rcClient.Width())
			{
				//�رն�ʱ
				KillTimer(IDI_CHANGE_SCENE_EFFECT);

				//�ͷ���Դ
				if (m_CurSeaScene<enSeaSceneKind_No) ReleaseSeaScene(m_CurSeaScene);
				m_Wave.Release();

				//���ñ���
				m_CurSeaScene=m_NewSeaScene;
				m_SeaScene[m_NewSeaScene].SetTride(false);
				m_NewSeaScene=enSeaSceneKind_No;	

				//���ñ�ʶ
				m_bWaveEffect=false;
				m_bWaveComing=false;
				CFishGroup::SetWaveComing(false);				
			}

			break;
		}
	case IDI_CHANGE_SCENE:				//�ǳ�׼��
		{
			//�ر�ʱ��
			KillTimer(IDI_CHANGE_SCENE);

			//������Դ
			LoadSeaScene(m_NewSeaScene);

			//���غ���
			TCHAR szFileName[MAX_PATH];
			_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\SeaScene\\WAVE.dds"),g_szWorkDir);
			m_Wave.SetLoadInfo(g_pD3DDevice,szFileName,0,D3DPOOL_MANAGED);

			//���ñ�ʶ
			m_bWaveEffect=true;

			//����ʱ��
			SetTimer(IDI_CHANGE_SCENE_EFFECT,TIME_CHANGE_SCENE_EFFECT,NULL);

			break;
		}
	case IDI_NO_BULLET_WARN:			//�ӵ�����
		{
			//�رն�ʱ
			KillTimer(IDI_NO_BULLET_WARN);

			//���ñ�ʶ
			m_bNoBulletWarn=false;

			break;
		}
	}
	return ;
}

//���س���
void CGameClientViewDX::LoadSeaScene(enSeaSceneKind SeaSceneKind)
{
	//��ȡ��С
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	TCHAR szFileName[MAX_PATH];
	BYTE byTextureIndex=0;
	BYTE wLestTimes=50, wRandTimes=40, wMultiple=4;
	BYTE wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
	D3DPOOL Pool=D3DPOOL_MANAGED;

	//����Ч��
	if (m_WaveEffect.IsLoaded()==false)
	{
		_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\SeaScene\\MASK.dds"),g_szWorkDir);
		m_WaveEffect.SetLoadInfo(g_pD3DDevice,szFileName,0,Pool);
	}

	if (enSeaSceneKind_1==SeaSceneKind)
	{
		//���ó���
		m_SeaScene[enSeaSceneKind_1].ResetScene();

		//���غ���
		_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\SeaScene\\SCENE_1.dds"),g_szWorkDir);
		m_SeaScene[enSeaSceneKind_1].SetLoadInfo(g_pD3DDevice,szFileName,0,Pool);
		m_SeaScene[enSeaSceneKind_1].SetReverse(true);

		_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\SeaScene\\BUBBLE.dds"),g_szWorkDir);
		m_SeaScene[enSeaSceneKind_1].SetLoadInfo(g_pD3DDevice,szFileName,MAX_TEXTURE_COUNT-1,Pool);

		//��������
		_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\SeaScene\\BIOLOGY_1.dds"),g_szWorkDir);
		m_SeaScene[enSeaSceneKind_1].SetLoadInfo(g_pD3DDevice,szFileName,1,Pool);

		//����λ��
		m_SeaScene[enSeaSceneKind_1].SetDrawPos(rcClient.Width()/2-m_SeaScene[enSeaSceneKind_1].GetImageWidth(byTextureIndex)/2+m_SeaScene[enSeaSceneKind_1].GetImageWidth(byTextureIndex),
			(rcClient.Height()-m_ToolBarBk.GetImageHeight()+24)/2-m_SeaScene[enSeaSceneKind_1].GetImageHeight(byTextureIndex)/2+m_SeaScene[enSeaSceneKind_1].GetImageHeight(byTextureIndex),byTextureIndex);

		byTextureIndex=1;
		m_SeaScene[enSeaSceneKind_1].SetImageSize(m_SeaScene[enSeaSceneKind_1].GetImageWidth(byTextureIndex)/5,m_SeaScene[enSeaSceneKind_1].GetImageHeight(byTextureIndex)/4,byTextureIndex);
		m_SeaScene[enSeaSceneKind_1].AddBiology(rcClient.Width()/2-70-126,rcClient.Height()/2+268-61,20,byTextureIndex,5,0.f,wSpaceTime);

		byTextureIndex=1;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_1].AddBiology(rcClient.Width()/2-135-162,rcClient.Height()/2-225-153,20,byTextureIndex,5,0.f,wSpaceTime);

		byTextureIndex=1;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_1].AddBiology(rcClient.Width()/2-280-143,rcClient.Height()/2-110-139,20,byTextureIndex,5,0.f,wSpaceTime);

		byTextureIndex=1;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_1].AddBiology(rcClient.Width()/2+60+171,rcClient.Height()/2+170+149,20,byTextureIndex,5,D3DX_PI,wSpaceTime);

		byTextureIndex=1;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_1].AddBiology(rcClient.Width()/2-412,rcClient.Height()/2+180,20,byTextureIndex,5,D3DX_PI,wSpaceTime);

		byTextureIndex=1;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_1].AddBiology(rcClient.Width()/2+10+175,rcClient.Height()/2-305+65,20,byTextureIndex,5,D3DX_PI,wSpaceTime);

		byTextureIndex=1;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_1].AddBiology(rcClient.Width()/2+252+180,rcClient.Height()/2+137,20,byTextureIndex,5,D3DX_PI/2,wSpaceTime,1.2f);

		byTextureIndex=1;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_1].AddBiology(rcClient.Width()/2+350,rcClient.Height()/2-50,20,byTextureIndex,5,D3DX_PI/3,wSpaceTime,0.8f);

		byTextureIndex=1;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_1].AddBiology(rcClient.Width()/2+470,rcClient.Height()/2,20,byTextureIndex,5,D3DX_PI/4,wSpaceTime,1.1f);

		byTextureIndex=1;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_1].AddBiology(rcClient.Width()/2+460,rcClient.Height()/2-100,20,byTextureIndex,5,D3DX_PI/5,wSpaceTime,0.7f);

		byTextureIndex=1;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_1].AddBiology(rcClient.Width()/2+440,rcClient.Height()/2-220,20,byTextureIndex,5,D3DX_PI/6,wSpaceTime,0.9f);
	}
	else if (enSeaSceneKind_2==SeaSceneKind)
	{
		//���ó���
		m_SeaScene[enSeaSceneKind_2].ResetScene();

		//���غ���
		_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\SeaScene\\SCENE_2.dds"),g_szWorkDir);
		m_SeaScene[enSeaSceneKind_2].SetLoadInfo(g_pD3DDevice,szFileName,0,Pool);

		_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\SeaScene\\BUBBLE.dds"),g_szWorkDir);
		m_SeaScene[enSeaSceneKind_2].SetLoadInfo(g_pD3DDevice,szFileName,MAX_TEXTURE_COUNT-1,Pool);

		//��������
		_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\SeaScene\\BIOLOGY_12.dds"),g_szWorkDir);
		m_SeaScene[enSeaSceneKind_2].SetLoadInfo(g_pD3DDevice,szFileName,1,Pool);
		_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\SeaScene\\BIOLOGY_14.dds"),g_szWorkDir);
		m_SeaScene[enSeaSceneKind_2].SetLoadInfo(g_pD3DDevice,szFileName,2,Pool);
		_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\SeaScene\\BIOLOGY_16.dds"),g_szWorkDir);
		m_SeaScene[enSeaSceneKind_2].SetLoadInfo(g_pD3DDevice,szFileName,3,Pool);

		//����λ��
		byTextureIndex=0;
		m_SeaScene[enSeaSceneKind_2].SetDrawPos(rcClient.Width()/2-m_SeaScene[enSeaSceneKind_2].GetImageWidth(byTextureIndex)/2,(rcClient.Height()-m_ToolBarBk.GetImageHeight()+24)/2-m_SeaScene[enSeaSceneKind_2].GetImageHeight(byTextureIndex)/2,byTextureIndex);

		byTextureIndex=1;
		wLestTimes=60, wRandTimes=40, wMultiple=4;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_2].SetImageSize(m_SeaScene[enSeaSceneKind_2].GetImageWidth(byTextureIndex)/8,m_SeaScene[enSeaSceneKind_2].GetImageHeight(byTextureIndex)/6,byTextureIndex);
		m_SeaScene[enSeaSceneKind_2].AddBiology(rcClient.Width()/2-355-109,rcClient.Height()/2+30-110,43,byTextureIndex,8,0.f,wSpaceTime);

		byTextureIndex=1;
		wLestTimes=60, wRandTimes=40, wMultiple=4;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_2].SetImageSize(m_SeaScene[enSeaSceneKind_2].GetImageWidth(byTextureIndex)/8,m_SeaScene[enSeaSceneKind_2].GetImageHeight(byTextureIndex)/6,byTextureIndex);
		m_SeaScene[enSeaSceneKind_2].AddBiology(rcClient.Width()/2+370+109,rcClient.Height()/2+190+110,43,byTextureIndex,8,D3DX_PI,wSpaceTime);

		byTextureIndex=1;
		wLestTimes=60, wRandTimes=40, wMultiple=4;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_2].SetImageSize(m_SeaScene[enSeaSceneKind_2].GetImageWidth(byTextureIndex)/8,m_SeaScene[enSeaSceneKind_2].GetImageHeight(byTextureIndex)/6,byTextureIndex);
		m_SeaScene[enSeaSceneKind_2].AddBiology(rcClient.Width()/2+470+80,rcClient.Height()/2-100,43,byTextureIndex,8,D3DX_PI/2,wSpaceTime,0.7f);

		byTextureIndex=1;
		wLestTimes=60, wRandTimes=40, wMultiple=4;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_2].SetImageSize(m_SeaScene[enSeaSceneKind_2].GetImageWidth(byTextureIndex)/8,m_SeaScene[enSeaSceneKind_2].GetImageHeight(byTextureIndex)/6,byTextureIndex);
		m_SeaScene[enSeaSceneKind_2].AddBiology(rcClient.Width()/2-360+30,rcClient.Height()/2+290-120,43,byTextureIndex,8,D3DX_PI/3,wSpaceTime,0.8f);

		byTextureIndex=2;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_2].SetImageSize(m_SeaScene[enSeaSceneKind_2].GetImageWidth(byTextureIndex)/7,m_SeaScene[enSeaSceneKind_2].GetImageHeight(byTextureIndex)/4,byTextureIndex);
		m_SeaScene[enSeaSceneKind_2].AddBiology(rcClient.Width()/2+210-245,rcClient.Height()/2-110,28,byTextureIndex,7,5*D3DX_PI/3,wSpaceTime,1.3f);

		byTextureIndex=2;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_2].SetImageSize(m_SeaScene[enSeaSceneKind_2].GetImageWidth(byTextureIndex)/7,m_SeaScene[enSeaSceneKind_2].GetImageHeight(byTextureIndex)/4,byTextureIndex);
		m_SeaScene[enSeaSceneKind_2].AddBiology(rcClient.Width()/2+320-160,rcClient.Height()/2+185-201,28,byTextureIndex,7,0.f,wSpaceTime);

		byTextureIndex=2;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_2].SetImageSize(m_SeaScene[enSeaSceneKind_2].GetImageWidth(byTextureIndex)/7,m_SeaScene[enSeaSceneKind_2].GetImageHeight(byTextureIndex)/4,byTextureIndex);
		m_SeaScene[enSeaSceneKind_2].AddBiology(rcClient.Width()/2+50,rcClient.Height()/2+230-153,28,byTextureIndex,7,D3DX_PI/2,wSpaceTime,1.2f);

		byTextureIndex=3;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_2].SetImageSize(m_SeaScene[enSeaSceneKind_2].GetImageWidth(byTextureIndex)/5,m_SeaScene[enSeaSceneKind_2].GetImageHeight(byTextureIndex)/3,byTextureIndex);
		m_SeaScene[enSeaSceneKind_2].AddBiology(rcClient.Width()/2-120-92,rcClient.Height()/2+265-91,15,byTextureIndex,5,0.f,wSpaceTime);

		byTextureIndex=5;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_2].SetImageSize(m_SeaScene[enSeaSceneKind_2].GetImageWidth(byTextureIndex)/5,m_SeaScene[enSeaSceneKind_2].GetImageHeight(byTextureIndex)/3,byTextureIndex);
		m_SeaScene[enSeaSceneKind_2].AddBiology(rcClient.Width()/2+120-92,rcClient.Height()/2+275-91,15,byTextureIndex,5,0.f,wSpaceTime);

		byTextureIndex=5;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_2].SetImageSize(m_SeaScene[enSeaSceneKind_2].GetImageWidth(byTextureIndex)/5,m_SeaScene[enSeaSceneKind_2].GetImageHeight(byTextureIndex)/3,byTextureIndex);
		m_SeaScene[enSeaSceneKind_2].AddBiology(rcClient.Width()/2+300-92,rcClient.Height()/2+40-91,15,byTextureIndex,5,0.f,wSpaceTime);

		byTextureIndex=5;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_2].SetImageSize(m_SeaScene[enSeaSceneKind_2].GetImageWidth(byTextureIndex)/5,m_SeaScene[enSeaSceneKind_2].GetImageHeight(byTextureIndex)/3,byTextureIndex);
		m_SeaScene[enSeaSceneKind_2].AddBiology(rcClient.Width()/2+455-int(92*0.8f),rcClient.Height()/2-80-int(91*0.8f),15,byTextureIndex,5,0.f,wSpaceTime,0.8f);

		byTextureIndex=5;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_2].SetImageSize(m_SeaScene[enSeaSceneKind_2].GetImageWidth(byTextureIndex)/5,m_SeaScene[enSeaSceneKind_2].GetImageHeight(byTextureIndex)/3,byTextureIndex);
		m_SeaScene[enSeaSceneKind_2].AddBiology(rcClient.Width()/2+495-int(92*0.7f),rcClient.Height()/2-140-int(91*0.7f),15,byTextureIndex,5,0.f,wSpaceTime,0.7f);
	}
	else if (enSeaSceneKind_3==SeaSceneKind)
	{
		//���ó���
		m_SeaScene[enSeaSceneKind_3].ResetScene();

		//���غ���
		_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\SeaScene\\SCENE_3.dds"),g_szWorkDir);
		m_SeaScene[enSeaSceneKind_3].SetLoadInfo(g_pD3DDevice,szFileName,0,Pool);
		m_SeaScene[enSeaSceneKind_3].SetReverse(true);

		_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\SeaScene\\BUBBLE.dds"),g_szWorkDir);
		m_SeaScene[enSeaSceneKind_3].SetLoadInfo(g_pD3DDevice,szFileName,MAX_TEXTURE_COUNT-1,Pool);

		//��������
		_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\SeaScene\\BIOLOGY_8.dds"),g_szWorkDir);
		m_SeaScene[enSeaSceneKind_3].SetLoadInfo(g_pD3DDevice,szFileName,1,Pool);
		_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\SeaScene\\BIOLOGY_10.dds"),g_szWorkDir);
		m_SeaScene[enSeaSceneKind_3].SetLoadInfo(g_pD3DDevice,szFileName,2,Pool);
		_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\SeaScene\\BIOLOGY_12.dds"),g_szWorkDir);
		m_SeaScene[enSeaSceneKind_3].SetLoadInfo(g_pD3DDevice,szFileName,3,Pool);

		//����λ��
		byTextureIndex=0;
		m_SeaScene[enSeaSceneKind_3].SetDrawPos(rcClient.Width()/2-m_SeaScene[enSeaSceneKind_3].GetImageWidth(byTextureIndex)/2+m_SeaScene[enSeaSceneKind_3].GetImageWidth(byTextureIndex),
			(rcClient.Height()-m_ToolBarBk.GetImageHeight()+24)/2-m_SeaScene[enSeaSceneKind_3].GetImageHeight(byTextureIndex)/2+m_SeaScene[enSeaSceneKind_3].GetImageHeight(byTextureIndex),byTextureIndex);

		byTextureIndex=5;
		wLestTimes=60, wRandTimes=40, wMultiple=4;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_3].SetImageSize(m_SeaScene[enSeaSceneKind_3].GetImageWidth(byTextureIndex)/8,m_SeaScene[enSeaSceneKind_3].GetImageHeight(byTextureIndex)/6,byTextureIndex);
		m_SeaScene[enSeaSceneKind_3].AddBiology(rcClient.Width()/2-80-109,rcClient.Height()/2+15-110,43,byTextureIndex,8,0.f,wSpaceTime);

		byTextureIndex=3;
		wLestTimes=60, wRandTimes=40, wMultiple=4;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_3].SetImageSize(m_SeaScene[enSeaSceneKind_3].GetImageWidth(byTextureIndex)/8,m_SeaScene[enSeaSceneKind_3].GetImageHeight(byTextureIndex)/6,byTextureIndex);
		m_SeaScene[enSeaSceneKind_3].AddBiology(rcClient.Width()/2+280+76,rcClient.Height()/2-100+77,43,byTextureIndex,8,D3DX_PI,wSpaceTime,0.7f);

		byTextureIndex=1;
		wLestTimes=60, wRandTimes=40, wMultiple=4;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_3].SetImageSize(m_SeaScene[enSeaSceneKind_3].GetImageWidth(byTextureIndex)/8,m_SeaScene[enSeaSceneKind_3].GetImageHeight(byTextureIndex)/4,byTextureIndex);
		m_SeaScene[enSeaSceneKind_3].AddBiology(rcClient.Width()/2-190+134,rcClient.Height()/2-200+126,32,byTextureIndex,8,D3DX_PI,wSpaceTime);

		byTextureIndex=1;
		wLestTimes=60, wRandTimes=40, wMultiple=4;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_3].SetImageSize(m_SeaScene[enSeaSceneKind_3].GetImageWidth(byTextureIndex)/8,m_SeaScene[enSeaSceneKind_3].GetImageHeight(byTextureIndex)/4,byTextureIndex);
		m_SeaScene[enSeaSceneKind_3].AddBiology(rcClient.Width()/2+250+120,rcClient.Height()/2-190+113,32,byTextureIndex,8,D3DX_PI,wSpaceTime,0.9f);

		byTextureIndex=1;
		wLestTimes=60, wRandTimes=40, wMultiple=4;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_3].SetImageSize(m_SeaScene[enSeaSceneKind_3].GetImageWidth(byTextureIndex)/8,m_SeaScene[enSeaSceneKind_3].GetImageHeight(byTextureIndex)/4,byTextureIndex);
		m_SeaScene[enSeaSceneKind_3].AddBiology(rcClient.Width()/2-110-175,rcClient.Height()/2+155-180,32,byTextureIndex,8,0.f,wSpaceTime,1.2f);

		byTextureIndex=2;
		wLestTimes=60, wRandTimes=40, wMultiple=4;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_3].SetImageSize(m_SeaScene[enSeaSceneKind_3].GetImageWidth(byTextureIndex)/7,m_SeaScene[enSeaSceneKind_3].GetImageHeight(byTextureIndex)/3,byTextureIndex);
		m_SeaScene[enSeaSceneKind_3].AddBiology(rcClient.Width()/2+120-225,rcClient.Height()/2+50-209,21,byTextureIndex,7,0.f,140);

		byTextureIndex=2;
		wLestTimes=60, wRandTimes=40, wMultiple=4;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_3].SetImageSize(m_SeaScene[enSeaSceneKind_3].GetImageWidth(byTextureIndex)/7,m_SeaScene[enSeaSceneKind_3].GetImageHeight(byTextureIndex)/3,byTextureIndex);
		m_SeaScene[enSeaSceneKind_3].AddBiology(rcClient.Width()/2-400-180,rcClient.Height()/2-190-167,21,byTextureIndex,7,0.f,160,0.8f);

		byTextureIndex=2;
		wLestTimes=60, wRandTimes=40, wMultiple=4;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_3].SetImageSize(m_SeaScene[enSeaSceneKind_3].GetImageWidth(byTextureIndex)/7,m_SeaScene[enSeaSceneKind_3].GetImageHeight(byTextureIndex)/3,byTextureIndex);
		m_SeaScene[enSeaSceneKind_3].AddBiology(rcClient.Width()/2+310-157,rcClient.Height()/2+220-146,21,byTextureIndex,7,0.f,180,0.7f);

		byTextureIndex=2;
		wLestTimes=60, wRandTimes=40, wMultiple=4;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_3].SetImageSize(m_SeaScene[enSeaSceneKind_3].GetImageWidth(byTextureIndex)/7,m_SeaScene[enSeaSceneKind_3].GetImageHeight(byTextureIndex)/3,byTextureIndex);
		m_SeaScene[enSeaSceneKind_3].AddBiology(rcClient.Width()/2+530,rcClient.Height()/2-175-160,21,byTextureIndex,7,D3DX_PI/2,180,0.6f);

		byTextureIndex=2;
		wLestTimes=60, wRandTimes=40, wMultiple=4;
		wSpaceTime=wLestTimes+rand()%wMultiple*wRandTimes;
		m_SeaScene[enSeaSceneKind_3].SetImageSize(m_SeaScene[enSeaSceneKind_3].GetImageWidth(byTextureIndex)/7,m_SeaScene[enSeaSceneKind_3].GetImageHeight(byTextureIndex)/3,byTextureIndex);
		m_SeaScene[enSeaSceneKind_3].AddBiology(rcClient.Width()/2-295+153,rcClient.Height()/2+80,21,byTextureIndex,7,D3DX_PI/3,200,0.7f);	
	}
}

//�ͷų���
void CGameClientViewDX::ReleaseSeaScene(enSeaSceneKind SeaSceneKind)
{
	if (enSeaSceneKind_1==SeaSceneKind)
	{
		//�ͷź���
		if (m_SeaScene[enSeaSceneKind_1].IsLoaded()==true) m_SeaScene[enSeaSceneKind_1].Release();
	}
	else if (enSeaSceneKind_2==SeaSceneKind)
	{
		//�ͷź���
		if (m_SeaScene[enSeaSceneKind_2].IsLoaded(0)==true) m_SeaScene[enSeaSceneKind_2].Release();
	}
	else if (enSeaSceneKind_3==SeaSceneKind)
	{
		//�ͷź���
		if (m_SeaScene[enSeaSceneKind_3].IsLoaded(0)==true) m_SeaScene[enSeaSceneKind_3].Release();
	}
}

//������Ⱥ
void CGameClientViewDX::LoadFish()
{
	//��������
	TCHAR szFileName[MAX_PATH];

	//��ȺͼƬ
	for (enFishKind FishKind=enFishKind_11; FishKind<enFishKind_No; FishKind=enFishKind(FishKind+1))
	{
		//����ͼƬ
		_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Fish\\FISH_%d.dds"),g_szWorkDir,FishKind+1);
		g_FishGroup[FishKind].SetLoadInfo(g_pD3DDevice,szFileName,TEXTURE_INDEX_FISH,D3DPOOL_DEFAULT);
		g_FishGroup[FishKind].SetImageSize(g_FishGroup[FishKind].GetImageWidth(TEXTURE_INDEX_FISH)/g_nColumnCount[FishKind],
			g_FishGroup[FishKind].GetImageHeight(TEXTURE_INDEX_FISH)/g_nRowCount[FishKind],TEXTURE_INDEX_FISH);
	}
}

//����ͼƬ
VOID CGameClientViewDX::LoadDXImage()
{
	//��������
	TCHAR szFileName[MAX_PATH];
	D3DPOOL Pool=D3DPOOL_MANAGED;

	//����ͼƬ
	if (m_ToolBarBk.IsLoaded(0)==false)
	{
		_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Background\\TOOL_BAR.dds"),g_szWorkDir);
		m_ToolBarBk.SetLoadInfo(g_pD3DDevice,szFileName,0,Pool);
	}

	//��ʾ����
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\SeaScene\\GAME_TIP.png"),g_szWorkDir);
	m_GameTip.SetLoadInfo(g_pD3DDevice,szFileName,0,Pool);	

	//����ͼƬ
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Tool\\Warn.png"),g_szWorkDir);
	m_NoBulletWarn.SetLoadInfo(g_pD3DDevice,szFileName,0,Pool);

	//��ȺͼƬ
	for (enFishKind FishKind=enFishKind_1; FishKind<enFishKind_11; FishKind=enFishKind(FishKind+1))
	{
		//����ͼƬ
		_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Fish\\FISH_%d.dds"),g_szWorkDir,FishKind+1);
		g_FishGroup[FishKind].SetLoadInfo(g_pD3DDevice,szFileName,TEXTURE_INDEX_FISH,Pool);
		g_FishGroup[FishKind].SetImageSize(g_FishGroup[FishKind].GetImageWidth(TEXTURE_INDEX_FISH)/g_nColumnCount[FishKind],
			g_FishGroup[FishKind].GetImageHeight(TEXTURE_INDEX_FISH)/g_nRowCount[FishKind],TEXTURE_INDEX_FISH);
	}

	//����ͼƬ
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Background\\BULLET_COUNT.dds"),g_szWorkDir);
	m_Number.SetLoadInfo(g_pD3DDevice,szFileName,TEXTURE_INDEX_BULLET_COUNT,Pool);
	m_Number.SetImageSize(m_Number.GetImageWidth(TEXTURE_INDEX_BULLET_COUNT)/11,m_Number.GetImageHeight(TEXTURE_INDEX_BULLET_COUNT),TEXTURE_INDEX_BULLET_COUNT);

	_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Background\\COIN_COUNT.dds"),g_szWorkDir);
	m_Number.SetLoadInfo(g_pD3DDevice,szFileName,TEXTURE_INDEX_COIN_COUNT,Pool);
	m_Number.SetImageSize(m_Number.GetImageWidth(TEXTURE_INDEX_COIN_COUNT)/10,m_Number.GetImageHeight(TEXTURE_INDEX_COIN_COUNT),TEXTURE_INDEX_COIN_COUNT);

	_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Background\\NONE.dds"),g_szWorkDir);
	m_Number.SetLoadInfo(g_pD3DDevice,szFileName,TEXTURE_INDEX_NONE,Pool);
	m_Number.SetImageSize(m_Number.GetImageWidth(TEXTURE_INDEX_NONE),m_Number.GetImageHeight(TEXTURE_INDEX_NONE),TEXTURE_INDEX_NONE);

	//����ͼƬ
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Fish\\FISH_12_2.dds"),g_szWorkDir);
	g_FishGroup[enFishKind_12].SetLoadInfo(g_pD3DDevice,szFileName,TEXTURE_INDEX_FISH+1,Pool);
	g_FishGroup[enFishKind_12].SetImageSize(g_FishGroup[enFishKind_12].GetImageWidth(TEXTURE_INDEX_FISH)/g_nColumnCount[enFishKind_12],
		g_FishGroup[enFishKind_12].GetImageHeight(TEXTURE_INDEX_FISH)/g_nRowCount[enFishKind_12],TEXTURE_INDEX_FISH+1);

	//���ش���
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Tool\\Cannon.dds"),g_szWorkDir);
	m_Cannon.SetLoadInfo(g_pD3DDevice,szFileName,TEXTURE_INDEX_CANNON,Pool);
	m_Cannon.SetImageSize(m_Cannon.GetImageWidth(TEXTURE_INDEX_CANNON)/5,m_Cannon.GetImageHeight(TEXTURE_INDEX_CANNON)/5,TEXTURE_INDEX_CANNON);

	_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Tool\\Cannon_Back.dds"),g_szWorkDir);
	m_Cannon.SetLoadInfo(g_pD3DDevice,szFileName,TEXTURE_INDEX_BACK,Pool);
	m_Cannon.SetImageSize(m_Cannon.GetImageWidth(TEXTURE_INDEX_BACK)/2,m_Cannon.GetImageHeight(TEXTURE_INDEX_BACK));

	_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Tool\\BulletCount.dds"),g_szWorkDir);
	m_Cannon.SetLoadInfo(g_pD3DDevice,szFileName,TEXTURE_INDEX_NUM,Pool);
	m_Cannon.SetImageSize(BULLET_NUMBER_WIDTH,m_Cannon.GetImageHeight(TEXTURE_INDEX_NUM)/2,TEXTURE_INDEX_NUM);

	//�����ӵ�
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Tool\\Bullet_1.png"),g_szWorkDir);
	m_Bullet.SetLoadInfo(g_pD3DDevice,szFileName,TEXTURE_INDEX_SHOOT_1,Pool);
	m_Bullet.SetImageSize(m_Bullet.GetImageWidth(TEXTURE_INDEX_SHOOT_1)/BULLET_FRAME_COUNT,m_Bullet.GetImageHeight(TEXTURE_INDEX_SHOOT_1),TEXTURE_INDEX_SHOOT_1);

	_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Tool\\Bullet_2.png"),g_szWorkDir);
	m_Bullet.SetLoadInfo(g_pD3DDevice,szFileName,TEXTURE_INDEX_SHOOT_2,Pool);
	m_Bullet.SetImageSize(m_Bullet.GetImageWidth(TEXTURE_INDEX_SHOOT_2)/BULLET_FRAME_COUNT,m_Bullet.GetImageHeight(TEXTURE_INDEX_SHOOT_2),TEXTURE_INDEX_SHOOT_2);

	_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Tool\\Bullet_3.png"),g_szWorkDir);
	m_Bullet.SetLoadInfo(g_pD3DDevice,szFileName,TEXTURE_INDEX_SHOOT_3,Pool);
	m_Bullet.SetImageSize(m_Bullet.GetImageWidth(TEXTURE_INDEX_SHOOT_3)/BULLET_FRAME_COUNT,m_Bullet.GetImageHeight(TEXTURE_INDEX_SHOOT_3),TEXTURE_INDEX_SHOOT_3);

	_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Tool\\Net.dds"),g_szWorkDir);
	m_Bullet.SetLoadInfo(g_pD3DDevice,szFileName,TEXTURE_INDEX_NET,Pool);
	m_Bullet.SetImageSize(m_Bullet.GetImageWidth(TEXTURE_INDEX_NET)/g_nNetColumnCount[enBulletKind_1],m_Bullet.GetImageHeight(TEXTURE_INDEX_NET)/g_nNetRowCount[enBulletKind_1],TEXTURE_INDEX_NET);

	//�ʽ�ͼƬ
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Score\\NUMBER.dds"),g_szWorkDir);
	m_Bonus.SetLoadInfo(g_pD3DDevice,szFileName,TEXTURE_INDEX_BONUS_NUMBER,Pool);
	m_Bonus.SetImageSize(m_Bonus.GetImageWidth(TEXTURE_INDEX_BONUS_NUMBER)/10,m_Bonus.GetImageHeight(TEXTURE_INDEX_BONUS_NUMBER),TEXTURE_INDEX_BONUS_NUMBER);

	_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Score\\BACK_LIGHT.dds"),g_szWorkDir);
	m_Bonus.SetLoadInfo(g_pD3DDevice,szFileName,TEXTURE_INDEX_BONUS_LIGHT,Pool);
	m_Bonus.SetImageSize(m_Bonus.GetImageWidth(TEXTURE_INDEX_BONUS_LIGHT),m_Bonus.GetImageHeight(TEXTURE_INDEX_BONUS_LIGHT),TEXTURE_INDEX_BONUS_LIGHT);

	_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Score\\BONUS_BACK.dds"),g_szWorkDir);
	m_Bonus.SetLoadInfo(g_pD3DDevice,szFileName,TEXTURE_INDEX_BONUS_BACK,Pool);
	m_Bonus.SetImageSize(m_Bonus.GetImageWidth(TEXTURE_INDEX_BONUS_BACK),m_Bonus.GetImageHeight(TEXTURE_INDEX_BONUS_BACK),TEXTURE_INDEX_BONUS_BACK);

	_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Score\\BONUS_FLASH.dds"),g_szWorkDir);
	m_Bonus.SetLoadInfo(g_pD3DDevice,szFileName,TEXTURE_INDEX_BONUS_STAR,Pool);
	m_Bonus.SetImageSize(m_Bonus.GetImageWidth(TEXTURE_INDEX_BONUS_STAR)/2,m_Bonus.GetImageHeight(TEXTURE_INDEX_BONUS_STAR)/2,TEXTURE_INDEX_BONUS_STAR);

	_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Score\\BONUS_DETAIL.dds"),g_szWorkDir);
	m_Bonus.SetLoadInfo(g_pD3DDevice,szFileName,TEXTURE_INDEX_BONUS_DETAIL,Pool);
	m_Bonus.SetImageSize(m_Bonus.GetImageWidth(TEXTURE_INDEX_BONUS_DETAIL),m_Bonus.GetImageHeight(TEXTURE_INDEX_BONUS_DETAIL),TEXTURE_INDEX_BONUS_DETAIL);

	//���ؽ��
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Fish\\VALUE_COUNT.dds"),g_szWorkDir);
	m_Coin.SetLoadInfo(g_pD3DDevice,szFileName,TEXTURE_INDEX_COIN_SHOW,Pool);
	m_Coin.SetImageSize(m_Coin.GetImageWidth(TEXTURE_INDEX_COIN_SHOW)/11,m_Coin.GetImageHeight(TEXTURE_INDEX_COIN_SHOW),TEXTURE_INDEX_COIN_SHOW);

	_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Fish\\COIN_1.dds"),g_szWorkDir);
	m_Coin.SetLoadInfo(g_pD3DDevice,szFileName,TEXTURE_INDEX_COIN_MOVE_1,Pool);
	m_Coin.SetImageSize(m_Coin.GetImageWidth(TEXTURE_INDEX_COIN_MOVE_1)/11,m_Coin.GetImageHeight(TEXTURE_INDEX_COIN_MOVE_1),TEXTURE_INDEX_COIN_MOVE_1);

	_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Fish\\COIN_2.dds"),g_szWorkDir);
	m_Coin.SetLoadInfo(g_pD3DDevice,szFileName,TEXTURE_INDEX_COIN_MOVE_2,Pool);
	m_Coin.SetImageSize(m_Coin.GetImageWidth(TEXTURE_INDEX_COIN_MOVE_2)/11,m_Coin.GetImageHeight(TEXTURE_INDEX_COIN_MOVE_2),TEXTURE_INDEX_COIN_MOVE_2);

	_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Fish\\COIN_3.dds"),g_szWorkDir);
	m_Coin.SetLoadInfo(g_pD3DDevice,szFileName,TEXTURE_INDEX_COIN_MOVE_3,Pool);
	m_Coin.SetImageSize(m_Coin.GetImageWidth(TEXTURE_INDEX_COIN_MOVE_3)/11,m_Coin.GetImageHeight(TEXTURE_INDEX_COIN_MOVE_3),TEXTURE_INDEX_COIN_MOVE_3);

	//�����Ϣ
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%sFish\\Background\\USER_INFO.dds"),g_szWorkDir);
	m_UserInfo.SetLoadInfo(g_pD3DDevice,szFileName,0,Pool);
	m_UserInfo.SetImageSize(m_UserInfo.GetImageWidth()/2,m_UserInfo.GetImageHeight());

	//���ñ�ʶ
	m_bLoadResFinish=true;

	return ;
}

//����ͼƬ
VOID CGameClientViewDX::OnResetDevice()
{
	//����ͼƬ
	if (m_ToolBarBk.IsLoaded()==true) m_ToolBarBk.OnResetDevice();
	if (m_Cannon.IsLoaded()==true) m_Cannon.OnResetDevice();
	if (m_Bullet.IsLoaded()==true) m_Bullet.OnResetDevice();
	if (m_Coin.IsLoaded()==true) m_Coin.OnResetDevice();
	if (m_UserInfo.IsLoaded()==true) m_UserInfo.OnResetDevice();
	if (m_Number.IsLoaded()==true) m_Number.OnResetDevice();
	if (m_Wave.IsLoaded()==true) m_Wave.OnResetDevice();
	if (m_NoBulletWarn.IsLoaded()==true) m_NoBulletWarn.OnResetDevice();
	if (m_WaveEffect.IsLoaded()==true) m_WaveEffect.OnResetDevice();
	if (m_GameTip.IsLoaded()==true) m_GameTip.OnResetDevice();
	if (m_Bonus.IsLoaded()==true) m_Bonus.OnResetDevice();

	for (enSeaSceneKind SeaSceneKind=enSeaSceneKind_1; SeaSceneKind<enSeaSceneKind_No; SeaSceneKind=enSeaSceneKind(SeaSceneKind+1))
	{	
		if (m_SeaScene[SeaSceneKind].IsLoaded()==true) m_SeaScene[SeaSceneKind].OnResetDevice();
	}

	//������Դ
	m_pFontUserInfo->OnResetDevice();
	m_pFontGameInfo->OnResetDevice();

	//������Ⱥ
	for (enFishKind FishKind=enFishKind_1; FishKind<enFishKind_No; FishKind=enFishKind(FishKind+1))
	{
		if (g_FishGroup[FishKind].IsLoaded()==true)
		{
			g_FishGroup[FishKind].OnResetDevice();
			g_FishGroup[FishKind].ResetKenel();
		}
	}

	return ;
}

//�ͷ���Դ
VOID CGameClientViewDX::OnDeviceLost()
{
	//����ͼƬ
	m_ToolBarBk.OnLostDevice();
	m_Cannon.OnLostDevice();
	m_Bullet.OnLostDevice();
	m_Coin.OnLostDevice();
	m_Bonus.OnLostDevice();
	m_UserInfo.OnLostDevice();
	m_Number.OnLostDevice();
	if (m_Wave.IsLoaded()==true) m_Wave.OnLostDevice();
	m_NoBulletWarn.OnLostDevice();

	//�ͷ���Դ
	m_pFontUserInfo->OnLostDevice();
	m_pFontGameInfo->OnLostDevice();

	////�ͷ���Ⱥ
	//for (enFishKind FishKind=enFishKind_1; FishKind<enFishKind_No; FishKind=enFishKind(FishKind+1))
	//{
	//	g_FishGroup[FishKind].OnLostDevice();
	//	g_FishGroup[FishKind].ResetKenel();
	//}

	//�ͷź���
	m_WaveEffect.OnLostDevice();
	for (enSeaSceneKind SeaSceneKind=enSeaSceneKind_1; SeaSceneKind<enSeaSceneKind_No; SeaSceneKind=enSeaSceneKind(SeaSceneKind+1))
	{
		if (m_SeaScene[SeaSceneKind].IsLoaded()==true) m_SeaScene[SeaSceneKind].OnLostDevice();
	}

	//�ͷ���Ⱥ
	for (enFishKind FishKind=enFishKind_11; FishKind<enFishKind_No; FishKind=enFishKind(FishKind+1))
	{
		g_FishGroup[FishKind].Release();
		g_FishGroup[FishKind].ResetKenel();
	}
	for (enFishKind FishKind=enFishKind_1; FishKind<enFishKind_11; FishKind=enFishKind(FishKind+1))
	{
		g_FishGroup[FishKind].OnLostDevice();
		g_FishGroup[FishKind].ResetKenel();
	}

	return ;
}

//��Ⱥ��ʧ
LRESULT CGameClientViewDX::OnFishDisappear(WPARAM wParam, LPARAM lParam)
{
	////��ȡ����
	//BYTE byShootUser=BYTE(LOWORD(wParam));
	//enFishKind FishKind=enFishKind(HIWORD(wParam));
	//int nPosX=LOWORD(lParam);
	//int nPosY=HIWORD(lParam);

	////�Ϸ��ж�
	//ASSERT(byShootUser<GAME_PLAYER);
	//ASSERT(FishKind<enFishKind_No);
	//if (!(byShootUser<GAME_PLAYER)) return 0;
	//if (!(FishKind<enFishKind_No)) return 0;

	////���ӽ��
	//if (FishKind<enFishKind_No)
	//{
	//	LONG lScore=g_nFishScore[FishKind];
	//	UserAddCoin(byShootUser, nPosX,nPosY, lScore);
	//}

	return 0;
}

//�켣��Ϣ
LRESULT CGameClientViewDX::OnFinishCalcTrace(WPARAM wParam, LPARAM lParam)
{
	//֪ͨ��Ⱥ
	while (m_FishArrayActive.GetCount()>0)
	{
		//���ñ�ʶ
		ASSERT(NULL!=m_FishArrayActive[0]);
		if (NULL!=m_FishArrayActive[0]) m_FishArrayActive[0]->bActive=true;

		//��Ⱥ��Ŀ
		WORD wFishCount=0;
		for (enFishKind FishKindIndex=enFishKind_1; FishKindIndex<enFishKind_No; FishKindIndex=enFishKind(FishKindIndex+1))
			wFishCount+=g_FishGroup[FishKindIndex].GetFishCount();

		//��Ŀ�ж�
		if (wFishCount>=70)
		{
			for (enFishKind FishKindIndex=enFishKind_1; FishKindIndex<enFishKind_No; FishKindIndex=enFishKind(FishKindIndex+1))
				g_FishGroup[FishKindIndex].FreeFish(m_FishArrayActive[0]);
		}

		//�Ƴ���Ϣ
		m_FishArrayActive.RemoveAt(0);
	}

	//����ʱ��
	m_dwLastCalcTime=GetTickCount();

	return 0;
}

//���´���
bool CGameClientViewDX::UpdateDxWindowSize()
{
	//���ñ�ʶ
	m_bResetArray=true;

	return __super::UpdateDxWindowSize();
}

//����λ��
void CGameClientViewDX::InitPosition()
{
	//��������
	int const static nCanPosX[2]={260,760};
	int const static nCanPosY[2]={37,527};
	int const static nNumPosX[2]={387,740};
	int const static nNumPosY[2]={47,560};

	//����λ��
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		//�Լ��ж�
		int nIndex=wChairID%2;
		if (true==IsSameSide(m_wMeChairID,wChairID))
		{
			//��������
			m_Cannon.SetBenchmarkPos(wChairID,nCanPosX[nIndex],nCanPosY[1]);

			//����λ��
			m_Cannon.SetNumberPos(wChairID,nNumPosX[nIndex],nNumPosY[1]);

			//���ýǶ�
			m_Cannon.SetAngleBk(wChairID,0.f);
		}
		else
		{
			//��������
			m_Cannon.SetBenchmarkPos(wChairID,nCanPosX[1-nIndex],nCanPosY[0]);

			//����λ��
			m_Cannon.SetNumberPos(wChairID,nNumPosX[1-nIndex],nNumPosY[0]);

			//���ýǶ�
			m_Cannon.SetAngleBk(wChairID,D3DX_PI);
		}
	}
}

//���ӹ켣
void CGameClientViewDX::AddTrace(int nInitX[], int nInitY[], int nInitCount, enFishKind FishKind, DWORD dwFishID, bool bRegular)
{
	//��������
	tagFishTraceInfo FishTraceInfo={0};
	FishTraceInfo.pFishInfo=g_FishGroup[FishKind].ActiveNewFish();

	//�Ϸ��ж�
	ASSERT(NULL!=FishTraceInfo.pFishInfo);
	if (NULL==FishTraceInfo.pFishInfo)
	{
		ASSERT(FALSE);
		TraceString(TEXT("if (NULL==FishTraceInfo.pFishInfo)"));
		return;
	}	

	//��Ⱥ��Ϣ
	FishTraceInfo.nInitCount=nInitCount;
	CopyMemory(FishTraceInfo.nInitX,nInitX,sizeof(FishTraceInfo.nInitX));
	CopyMemory(FishTraceInfo.nInitY,nInitY,sizeof(FishTraceInfo.nInitY));
	FishTraceInfo.pFishInfo->dwFishID=dwFishID;
	FishTraceInfo.pFishInfo->bySpaceTimes=bRegular?g_nMoveSpeed[enFishKind_1]:g_nMoveSpeed[FishKind];
	FishTraceInfo.byMoveSpace=(false==bRegular && FishKind>=enFishKind_11)?TRACE_POINT_SPACE_BIG:TRACE_POINT_SPACE_NORMAL;

	//����ת��
	if (0==m_wMeChairID || 1==m_wMeChairID)
	{
		for (int i=0; i<nInitCount; ++i)
		{
			FishTraceInfo.nInitX[i]=SwitchCoorX(FishTraceInfo.nInitX[i]);
			FishTraceInfo.nInitY[i]=SwitchCoorY(FishTraceInfo.nInitY[i]);
		}
	}

	//��������
	m_FishArrayBuffer.Add(FishTraceInfo);

	//�����ź�
	bool bRestart=false;
	if (NULL!=m_hEventFinish) 
	{
		SetEvent(m_hEventFinish);

		////ʱ���ж�
		//if ((m_dwLastCalcTime+15000)<GetTickCount())
		//{
		//	bRestart=true;
		//	CloseHandle(m_hEventFinish);
		//}
	}
	else
	{
		bRestart=true;
	}

	if (true==bRestart)
	{
		//�����߳�
		WaitForSingleObject(m_hCalcThread,2000);
		CloseHandle(m_hCalcThread);

		//�����߳�
		UINT uThreadID=0;
		m_hEventFinish=CreateEvent(NULL,FALSE,FALSE,NULL);
		m_hCalcThread=(HANDLE)::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadCalcTrace,LPVOID(this),0,0);		

#ifdef _DEBUG
		if (NULL!=m_pGameClientDlg) m_pGameClientDlg->InsertSystemString(TEXT("�����߳�"));
#endif
	}
}

//������λ
void CGameClientViewDX::SetMeChairID(WORD wMeChairID) 
{
	//�Լ���λ
	m_wMeChairID=wMeChairID;
	m_Cannon.SetMeChairID(m_wMeChairID);

	//�ӵ���Ŀ
	m_Cannon.SetBulletCount(m_wMeChairID,5,false);

	//����λ��
	InitPosition();
}






	////�����Ϣ
	//for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	//{
	//	if(m_lBulletCount[wChairID] > 0 || wChairID == m_wMeChairID)
	//	{
	//		//��ұ���
	//		GetDrawPos(wChairID,enDrawPos_MeGoldBack,nDrawPosX,nDrawPosY);
	//		int _w = m_CoinBack.GetWidth();
	//		int _h = m_CoinBack.GetHeight();
	//		rcDraw.SetRect(0,0,_w,_h);
	//		m_CoinBack.DrawSprite(nDrawPosX,nDrawPosY,rcDraw);

	//		//�����Ŀ(��̨�ұ�)
	//		GetDrawPos(wChairID,enDrawPos_MeGold,nDrawPosX,nDrawPosY);
	//		LONGLONG lScore= m_lCoinCount[wChairID] + m_lBulletCount[wChairID]*m_lCellScore[wChairID];
	//		//LONGLONG lScore=123456789;//������
	//		DrawNumber(nDrawPosX, nDrawPosY, lScore, TEXTURE_INDEX_COIN_COUNT);

	//		//д�ǳ�(��̨�ұ�)
	//		rcClient.SetRect(nDrawPosX+20,nDrawPosY-20,nDrawPosX + 120, nDrawPosY + 35);
	//		if(m_pGameClientDlg->GetUserData(wChairID) != NULL)
	//		{
	//			sprintf(m_szNiceName[wChairID], m_pGameClientDlg->GetUserData(wChairID)->szNickName);
	//			//m_pFontUserInfo->DrawTextA(NULL, "�ǳ�ABC", (int)strlen("�ǳ�ABC"), &rcClient,DT_SINGLELINE|DT_NOCLIP|DT_CENTER|DT_TOP, RGB(255,0,255)|0xFF000000);
	//			m_pFontUserInfo->DrawTextA(NULL, m_szNiceName[wChairID], (int)strlen(m_szNiceName[wChairID]), &rcClient,DT_SINGLELINE|DT_NOCLIP|DT_CENTER|DT_TOP, RGB(255,0,255)|0xFF000000);
	//		}

	//	}
	//}

//������Ϣ
void CGameClientViewDX::DrawUserInfo()
{
	//�Ϸ��ж�
	ASSERT(NULL!=m_pGameClientDlg);
	if (NULL==m_pGameClientDlg) return;
	if (m_wMeChairID>=GAME_PLAYER) return;

	//��ȡ��С
	CRect rcClient, rcDraw;
	GetClientRect(&rcClient);

	//�滭����
	int nDrawPosX,nDrawPosY;
	WCHAR szBuffer[256];

	//�����Ϣ
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		//��ȡ�û�
		tagUserData const *pUserData=m_pGameClientDlg->GetUserData(wChairID);
		if (NULL==pUserData) continue;

		/////////////////////////////////////////////////////////////////////////////////
		/*tagUserData UserData={0};
		tagUserData *pUserData=&UserData;
		lstrcpyn(UserData.szName,TEXT("KJDKFJDFKJDFKJF"),CountArray(UserData.szName));
		UserData.lScore=100;
		m_lBulletCount[wChairID][enBulletCountKind_800]=wChairID+1;
		m_Cannon.SetBulletCount(wChairID,5,false);*/
		/////////////////////////////////////////////////////////////////////////////////

		//�Լ��ж�
		if (m_wMeChairID==wChairID)
		{
			//��ұ���
			GetDrawPos(wChairID,enDrawPos_MeGoldBack,nDrawPosX,nDrawPosY);
			rcDraw.SetRect(0,0,100,24);
			m_UserInfo.DrawSprite(nDrawPosX,nDrawPosY,rcDraw);

			//�����Ŀ
			GetDrawPos(wChairID,enDrawPos_MeGold,nDrawPosX,nDrawPosY);
			//LONGLONG lScore=LONGLONG(pUserData->lScore);
			LONGLONG lScore=123456789;//������
			DrawTextString(m_pFontUserInfo,_ui64tow(lScore,szBuffer,10),RGB(0,255,255),nDrawPosX,nDrawPosY);

			////�����Ŀ(��̨�ұ�)
			//GetDrawPos(wChairID,enDrawPos_MeGold,nDrawPosX,nDrawPosY);
			//LONGLONG lScore= m_lCoinCount[wChairID] + m_lBulletCount[wChairID]*m_lCellScore[wChairID];
			////LONGLONG lScore=123456789;//������
			//DrawNumber(nDrawPosX, nDrawPosY, lScore, TEXTURE_INDEX_COIN_COUNT);

			//rcClient.SetRect(220, 665, 360, 680);
			//TCHAR _szNumber[32];
			//sprintf(_szNumber, "%d", m_lCoinCount[m_wMeChairID] + m_lBulletCount[m_wMeChairID]*m_lCellScore[m_wMeChairID]);
			//m_pFontUserInfo->DrawTextA(NULL, _szNumber, (int)strlen(_szNumber), &rcClient,DT_SINGLELINE|DT_NOCLIP|DT_CENTER|DT_TOP, RGB(255,0,255)|0xFF000000);
			////m_pFontUserInfo->DrawTextA(NULL, "54321", (int)strlen("54321"), &rcClient,DT_SINGLELINE|DT_NOCLIP|DT_CENTER|DT_TOP, RGB(255,0,255)|0xFF000000);

			//���½�
			CRect rcCellScore1;
			rcCellScore1.SetRect(0, 600, 75, 620);
			//ok:m_pFontGameInfo->DrawTextA(NULL,"�ǳ�DEF", (int)strlen("�ǳ�DEF"), &rcCellScore1,DT_SINGLELINE|DT_NOCLIP|DT_CENTER|DT_TOP, RGB(255,0,255)|0xFF000000);
			/*m_pFontUserInfo*/m_pFontGameInfo->DrawTextA(NULL, pUserData->szName/*m_szNiceName[m_wMeChairID]*/, (int)strlen(pUserData->szName/*m_szNiceName[m_wMeChairID]*/), &rcCellScore1,DT_SINGLELINE|DT_NOCLIP|DT_CENTER|DT_TOP, RGB(255,0,255)|0xFF000000);
			//ok:m_pFontGameInfo->DrawTextW(NULL,L"�ǳ�DEF", (int)wcslen(L"�ǳ�DEF"), &rcCellScore1,DT_SINGLELINE|DT_NOCLIP|DT_LEFT|DT_TOP, RGB(0,0,255)|0xFF000000);

			//�滭�ӵ�
			int nBulletCount=0;
			int nIndex=0;
			if (0!=m_lBulletCount[wChairID][enBulletCountKind_800])
			{
				rcDraw.SetRect(72,60,110,81);
				nBulletCount=m_lBulletCount[wChairID][enBulletCountKind_800];

				GetDrawPos(wChairID,enDrawPos_BulletBack,nDrawPosX,nDrawPosY,nIndex);
				m_UserInfo.DrawSprite(nDrawPosX,nDrawPosY,rcDraw);

				GetDrawPos(wChairID,enDrawPos_Bullet,nDrawPosX,nDrawPosY,nIndex);
				rcDraw.SetRect(0,0,57,20);
				DrawNumber(nDrawPosX,nDrawPosY,nBulletCount,TEXTURE_INDEX_BULLET_COUNT);

				++nIndex;
			}
			if (0!=m_lBulletCount[wChairID][enBulletCountKind_600])
			{
				rcDraw.SetRect(42,60,72,81);
				nBulletCount=m_lBulletCount[wChairID][enBulletCountKind_600];

				GetDrawPos(wChairID,enDrawPos_BulletBack,nDrawPosX,nDrawPosY,nIndex);
				m_UserInfo.DrawSprite(nDrawPosX+5,nDrawPosY,rcDraw);

				GetDrawPos(wChairID,enDrawPos_Bullet,nDrawPosX,nDrawPosY,nIndex);
				rcDraw.SetRect(0,0,57,20);
				DrawNumber(nDrawPosX,nDrawPosY,nBulletCount,TEXTURE_INDEX_BULLET_COUNT);

				++nIndex;
			}
			if (0!=m_lBulletCount[wChairID][enBulletCountKind_100])
			{
				rcDraw.SetRect(0,60,42,81);
				nBulletCount=m_lBulletCount[wChairID][enBulletCountKind_100];

				GetDrawPos(wChairID,enDrawPos_BulletBack,nDrawPosX,nDrawPosY,nIndex);
				m_UserInfo.DrawSprite(nDrawPosX,nDrawPosY,rcDraw);

				GetDrawPos(wChairID,enDrawPos_Bullet,nDrawPosX,nDrawPosY,nIndex);
				rcDraw.SetRect(0,0,57,20);
				DrawNumber(nDrawPosX,nDrawPosY,nBulletCount,TEXTURE_INDEX_BULLET_COUNT);

				++nIndex;
			}
			else
			{
				rcDraw.SetRect(0,60,42,81);
			}

			//û���ӵ�
			if (0==nBulletCount)
			{
				GetDrawPos(wChairID,enDrawPos_BulletBack,nDrawPosX,nDrawPosY,nIndex);
				m_UserInfo.DrawSprite(nDrawPosX,nDrawPosY,rcDraw);

				GetDrawPos(wChairID,enDrawPos_Bullet,nDrawPosX,nDrawPosY,nIndex);
				rcDraw.SetRect(0,0,57,20);
				m_Number.DrawSprite(nDrawPosX+15,nDrawPosY,rcDraw,TEXTURE_INDEX_NONE);
			}
		}
		else
		{
			//�ǳƱ���
			GetDrawPos(wChairID,enDrawPos_AccountBack,nDrawPosX,nDrawPosY);
			rcDraw.SetRect(0,26,146,60);
			m_UserInfo.DrawSprite(nDrawPosX,nDrawPosY,rcDraw);

			//����ǳ�
			GetDrawPos(wChairID,enDrawPos_Account,nDrawPosX,nDrawPosY);
			DrawTextString(m_pFontUserInfo,CA2W(pUserData->szName),RGB(0,255,255),nDrawPosX,nDrawPosY);

			//�滭�ӵ�
			int nBulletCount=0;
			if (0!=m_lBulletCount[wChairID][enBulletCountKind_800])
			{
				rcDraw.SetRect(72,60,110,81);
				nBulletCount=m_lBulletCount[wChairID][enBulletCountKind_800];
			}
			else if (0!=m_lBulletCount[wChairID][enBulletCountKind_600])
			{
				rcDraw.SetRect(42,60,72,81);
				nBulletCount=m_lBulletCount[wChairID][enBulletCountKind_600];
			}
			else if (0!=m_lBulletCount[wChairID][enBulletCountKind_100])
			{
				rcDraw.SetRect(0,60,42,81);
				nBulletCount=m_lBulletCount[wChairID][enBulletCountKind_100];
			}
			else
			{
				rcDraw.SetRect(0,60,42,81);
			}
			GetDrawPos(wChairID,enDrawPos_BulletBack,nDrawPosX,nDrawPosY);
			m_UserInfo.DrawSprite(nDrawPosX,nDrawPosY,rcDraw);

			GetDrawPos(wChairID,enDrawPos_Bullet,nDrawPosX,nDrawPosY);
			rcDraw.SetRect(0,0,57,20);
			if (nBulletCount>0) DrawNumber(nDrawPosX,nDrawPosY,nBulletCount,TEXTURE_INDEX_BULLET_COUNT);
			else m_Number.DrawSprite(nDrawPosX+15,nDrawPosY,rcDraw,TEXTURE_INDEX_NONE);
		}

		//�滭���
		rcDraw.SetRect(148,24,299,5020);
		GetDrawPos(wChairID,enDrawPos_CoinBack,nDrawPosX,nDrawPosY);
		m_UserInfo.DrawSprite(nDrawPosX,nDrawPosY,rcDraw);

		//��Ԫ����
		static WCHAR szBuffer[128]={0};
		_snwprintf(szBuffer,CountArray(szBuffer),L"(%ld��)",m_lCellScore[wChairID]);
		CRect rcCellScore;
		rcCellScore.left=0;	
		rcCellScore.top=nDrawPosY+5;
		rcCellScore.right=0;
		rcCellScore.bottom=0;
		m_pFontGameInfo->DrawTextW(NULL, szBuffer, (int)wcslen(szBuffer), &rcCellScore,DT_SINGLELINE|DT_NOCLIP|DT_LEFT|DT_TOP|DT_CALCRECT, RGB(0,0,255)|0xFF000000);
		rcCellScore.left=nDrawPosX-rcCellScore.Width();
		m_pFontGameInfo->DrawTextW(NULL, szBuffer, (int)wcslen(szBuffer), &rcCellScore,DT_SINGLELINE|DT_NOCLIP|DT_LEFT|DT_TOP, RGB(0,0,255)|0xFF000000);

		GetDrawPos(wChairID,enDrawPos_Coin,nDrawPosX,nDrawPosY);
		DrawNumber(nDrawPosX,nDrawPosY,987654/*m_lCoinCount[wChairID]*/,TEXTURE_INDEX_COIN_COUNT);
		//�����Ŀ
		nDrawPosX=100;
		nDrawPosY=600;
		DrawNumber(nDrawPosX,nDrawPosY,123456789/*m_lCoinCount[wChairID]*/,TEXTURE_INDEX_COIN_COUNT);
	}
}

//��ȡ�Ƕ�
float CGameClientViewDX::GetMeShootAngle()
{
	//��ȡ�Ƕ�
	float fAngle=m_Cannon.GetAngleCan(m_wMeChairID);

	//�Ƕ�ת��
	if (0==m_wMeChairID || 1==m_wMeChairID) fAngle=SwitchAngle(fAngle);

	return fAngle;
}

//��ҷ���
void CGameClientViewDX::UserShoot(WORD wChairID, float fAngle, enBulletCountKind BulletCountKind)
{
	//�����Ƕ�
	if (0==m_wMeChairID || 1==m_wMeChairID) fAngle=SwitchAngle(fAngle);

	//�����ڵ�
	DWORD dwCptrFishID[5]={0};
	m_Cannon.SetAngleCan(wChairID, fAngle);

	//���÷�Χ
	CRect rcBound;
	GetClientRect(rcBound);
	BYTE byBulletCount=m_Cannon.GetBulletCount(wChairID);
	if (byBulletCount>5) return;
	if (IsSameSide(m_wMeChairID,wChairID))
	{
		if (wChairID%2==0) rcBound.right-=(SHOOT_ROUND_SHRINK_X*(5-byBulletCount));
		else rcBound.left+=(SHOOT_ROUND_SHRINK_X*(5-byBulletCount));
		rcBound.top+=(SHOOT_ROUND_SHRINK_Y*(5-byBulletCount));
	}
	else
	{
		if (wChairID%2!=0) rcBound.right-=(SHOOT_ROUND_SHRINK_X*(5-byBulletCount));
		else rcBound.left+=(SHOOT_ROUND_SHRINK_X*(5-byBulletCount));
		rcBound.bottom-=(SHOOT_ROUND_SHRINK_Y*(5-byBulletCount));
	}
	m_Bullet.SetBound(rcBound);

	//�����ڵ�
	m_Bullet.Shoot(wChairID,false, dwCptrFishID, 5, m_Cannon.GetBenchmarkPos(wChairID).x,m_Cannon.GetBenchmarkPos(wChairID).y,
		m_Cannon.GetAngleCan(wChairID), BulletCountKind,byBulletCount);
	m_Cannon.Shoot(wChairID);

	//��������
	TCHAR *pSiloSound[]={TEXT("Effect\\Silo0.mp3"),TEXT("Effect\\Silo1.mp3"),TEXT("Effect\\Silo2.mp3"),TEXT("Effect\\Silo3.mp3"),TEXT("Effect\\Silo4.mp3")};
	PlayMp3Sound(pSiloSound[byBulletCount-1],false);
}

//����״̬
bool CGameClientViewDX::SetFishStatus(enFishKind FishKind, DWORD dwFishID, enFishStatus FishStatus)
{
	//�Ϸ��ж�
	ASSERT(FishKind<enFishKind_No);
	if (!(FishKind<enFishKind_No)) return false;

	//����״̬
	g_FishGroup[FishKind].SetStatus(dwFishID, FishStatus);

	return true;
}

//���ӽ��
void CGameClientViewDX::UserAddCoin(WORD wChairID, int nSourcePosX, int nSourcePosY, LONG lCoinCount)
{
	//����λ��
	int nRectifyPosX[]={30,-30,40,-40,60,-60};
	int nCoinKind[]={20,10,1};
	int nTextureIndex[]={TEXTURE_INDEX_COIN_MOVE_3,TEXTURE_INDEX_COIN_MOVE_2,TEXTURE_INDEX_COIN_MOVE_1};

	//���ӽ��
	int nCoinPosX,nCoinPosY;
	GetDrawPos(wChairID,enDrawPos_Coin,nCoinPosX,nCoinPosY);
	for (int nKIndex=0; nKIndex<CountArray(nCoinKind); ++nKIndex)
	{
		if (0==lCoinCount%nCoinKind[nKIndex])
		{
			int nCount=lCoinCount/nCoinKind[nKIndex];
			for (int nCTIndex=0; nCTIndex<nCount; ++nCTIndex)
			{
				m_Coin.AddCoin(nSourcePosX+nRectifyPosX[rand()%CountArray(nRectifyPosX)], nSourcePosY, nCoinPosX, nCoinPosY, nSourcePosX, nSourcePosY, lCoinCount, nTextureIndex[nKIndex]);
			}

			break;
		}
	}

	//��������
	PlayMp3Sound(TEXT("Effect\\CoinFly.mp3"),false);
}

//���Ӳʽ�
void CGameClientViewDX::AddBonus(WORD wChairID, LONG lBonusCount)
{
	int const static nBonusPosX[2]={140,630};
	int const static nBonusPosY[2]={37+70,527-220};
	int nIndex=wChairID%2;
	if (IsSameSide(m_wMeChairID,wChairID))
		m_Bonus.AddBonus(lBonusCount,nBonusPosX[nIndex],nBonusPosY[1]);
	else
		m_Bonus.AddBonus(lBonusCount,nBonusPosX[1-nIndex],nBonusPosY[0]);
}

//��Ⱥ����
enFishKind CGameClientViewDX::GetFishKind(DWORD dwFish)
{
	for (enFishKind FishKind=enFishKind_1; FishKind<enFishKind_No; FishKind=enFishKind(FishKind+1))
	{
		tagFishInfo *pFishInfo=g_FishGroup[FishKind].GetFish(dwFish);
		if (NULL!=pFishInfo) return FishKind;
	}

	return enFishKind_No;
}

//��������
bool CGameClientViewDX::InitDxWindow()
{
	//���ø���
	bool bResult=__super::InitDxWindow();

	//������ť
	m_btShowBulletShop.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(10,10,20,30),this,IDC_SHOW_BULLET_SHOP);
	m_btQuitGame.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(10,10,20,30),this,IDC_QUIT_GAME);
	m_btGameHelp.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(10,10,20,30),this,IDC_SHOW_GAME_HELP);

	//��ťͼƬ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btShowBulletShop.SetButtonImage(IDB_BT_SHOW_SHOP,hInstance,4,false);
	m_btQuitGame.SetButtonImage(IDB_BT_QUIT_GAME,hInstance,3,false);
	m_btGameHelp.SetButtonImage(IDB_BT_HELP,hInstance,4,false);

	//���ý��㣨���������Ϣû����Ӧ��
	SetFocus();

	//����ʱ��
	SetTimer(IDI_IDLE_TIME,1000,NULL);

	//�������
	srand((unsigned)time(NULL));

	//��������
	//km_TranslateWnd.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,CRect(10,10,20,30),this,IDC_WND);

	//SetTimer(IDI_CHANGE_SCENE_EFFECT,TIME_CHANGE_SCENE_EFFECT,NULL);
	//m_SeaScene[m_NewSeaScene].SetTride(true);

	//��������
	TCHAR *pszBKSound[]={TEXT("BGSound0.mp3"),TEXT("BGSound1.mp3"),TEXT("BGSound2.mp3")};
	int nIndex=rand()%CountArray(pszBKSound);
	PlayMp3SoundBG(pszBKSound[nIndex]);
	PlayMp3Sound(nIndex?TEXT("Effect\\ShellOpen1.mp3"):TEXT("Effect\\ShellOpen2.mp3"),false);

	return bResult;
}

//��С��Ϣ
void CGameClientViewDX::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyGameView(cx, cy);
}

//�����ؼ�
VOID CGameClientViewDX::RectifyGameView(INT nWidth, INT nHeight)
{
	//�Ϸ��ж�
	if (NULL==m_btShowBulletShop.GetSafeHwnd()) return;

	//��ʼ����
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	HDWP hDWP=BeginDeferWindowPos(10);

	//����λ��
	CRect rcButtonShop,rcButtonQuit;
	m_btShowBulletShop.GetWindowRect(&rcButtonShop);
	m_btQuitGame.GetWindowRect(&rcButtonQuit);
	DeferWindowPos(hDWP,m_btShowBulletShop,NULL,700,nHeight-rcButtonShop.Height(),0,0,uFlags);
	DeferWindowPos(hDWP,m_btQuitGame,NULL,700+rcButtonShop.Width()+5,nHeight-rcButtonShop.Height(),0,0,uFlags);
	DeferWindowPos(hDWP,m_btGameHelp,NULL,700+rcButtonShop.Width()+rcButtonQuit.Width()+10,nHeight-rcButtonShop.Height()+4,0,0,uFlags);

	//�����ƶ�
	EndDeferWindowPos(hDWP);
}

//�����
BOOL CGameClientViewDX::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);
	UINT nCommandID2=HIWORD(wParam);

	//��Ϣ����
	switch(nCommandID)
	{
	case IDC_SHOW_BULLET_SHOP:
		{
			////������Ϣ
			//if (NULL!=m_pGameClientDlg) m_pGameClientDlg->PostMessage(WM_SHOW_BULLET_SHOP,0,0);

			////���ý���
			//SetFocus();

			return TRUE;
		}
	case IDC_QUIT_GAME:
		{
			//�Ϸ��ж�
			if (m_wMeChairID>=GAME_PLAYER) return false;
			if (true==m_bLookonMode) return false;

			//������Ϣ
			SetWaitScoreInfo(true);

			//������Ϣ
			if (NULL!=m_pGameClientDlg) m_pGameClientDlg->SendData(SUB_C_GET_SCORE_INFO);

			return TRUE;
		}
	case IDC_SHOW_GAME_HELP:
		{
			//��Ϸ����
			CGameHelpDlg GameHelpDlg(this);
			GameHelpDlg.DoModal();

			return TRUE;
		}	
	//case IDC_SHOW_BALANCE://�����㡱��ť
	//	{
	//		//�Ϸ��ж�
	//		if (m_wMeChairID>=GAME_PLAYER) return false;
	//		if (true==m_bLookonMode) return false;

	//		//������Ϣ
	//		SetWaitScoreInfo(true);

	//		//������Ϣ
	//		if (NULL!=m_pGameClientDlg) m_pGameClientDlg->SendData(SUB_C_GET_SCORE_INFO);

	//		return TRUE;
	//	}
	//case IDC_SHOW_GAME_HELP://����ۡ���ť
	//	{
	//		//��Ϸ����������ļ�ֵ���Ի���
	//		CGameHelpDlg GameHelpDlg(this);
	//		GameHelpDlg.DoModal();

	//		return TRUE;
	//	}
	}
	return __super::OnCommand(wParam,lParam);
}

//�����ӵ�
void CGameClientViewDX::SetUserBulletCount(WORD wChairID, enBulletCountKind BulletCountKind, LONG lCount)
{
	//�Ϸ��ж�
	ASSERT(wChairID<GAME_PLAYER);
	ASSERT(BulletCountKind<enBulletCountKind_End);
	if (!(wChairID<GAME_PLAYER)) return;
	if (!(BulletCountKind<enBulletCountKind_End)) return;

	//���ñ���
	m_lBulletCount[wChairID][BulletCountKind]=lCount;

	//�����ж�
	if (m_wMeChairID==wChairID && true==m_bNoBulletWarn && lCount>40)
	{
		m_bNoBulletWarn=false;
	}
}

//���ý��
void CGameClientViewDX::SetUserCoinCount(WORD wChairID, int nCount)
{
	//�Ϸ��ж�
	ASSERT(wChairID<GAME_PLAYER);
	if (!(wChairID<GAME_PLAYER)) return;

	//���ñ���
	m_lCoinCount[wChairID]=nCount;
}

//�滭����
void CGameClientViewDX::DrawNumber(int nXPos, int nYPos, LONGLONG lNumber, int nTextIndex)
{
	//��ȡ����
	const INT nNumberHeight=m_Number.GetHeight(nTextIndex);
	const INT nNumberWidth=m_Number.GetWidth(nTextIndex);

	//������Ŀ
	LONG lNumberCount=0;
	LONGLONG lNumberTemp=lNumber;
	do
	{
		lNumberCount++;
		lNumberTemp/=10;
	} while (lNumberTemp>0L);

	//λ�ö���
	int const nSpace=0;
	INT nYDrawPos=nYPos;
	INT nXDrawPos=nXPos+lNumberCount*(nNumberWidth+nSpace);

	//��ǰλ��
	CRect drawRect;

	//�滭����
	for (LONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		WORD wCellNumber=WORD(lNumber%10);
		drawRect.left=wCellNumber*nNumberWidth;
		drawRect.top=0;
		drawRect.right=drawRect.left+m_Number.GetWidth(nTextIndex);
		drawRect.bottom=drawRect.top+m_Number.GetHeight(nTextIndex);
		m_Number.DrawSprite(nXDrawPos,nYDrawPos,drawRect,nTextIndex);

		//���ñ���
		lNumber/=10;
		nXDrawPos-=(nNumberWidth+nSpace);
	}
}

//��ȡλ��
void CGameClientViewDX::GetDrawPos(WORD wChairID,enDrawPos DrawPos, int &nPosX, int &nPosY, int nIndex)
{
	//���ñ���
	nPosX=0;
	nPosY=0;

	//�Ϸ��ж�
	ASSERT(wChairID<GAME_PLAYER);
	if (!(wChairID<GAME_PLAYER)) return;

	//��ȡ��С
	CRect rcClient;
	GetClientRect(&rcClient);

	//�Լ��ж�
	if (IsSameSide(m_wMeChairID,wChairID))
	{
		//����ж�
		if (wChairID%2==0)
		{
			//�Լ��ж�
			if (m_wMeChairID==wChairID)
			{
				if (enDrawPos_MeGoldBack==DrawPos)
				{
					nPosX=105;
					nPosY=rcClient.Height()-m_ToolBarBk.GetHeight()-12;
				}
				else if (enDrawPos_MeGold==DrawPos)
				{
					nPosX=105+35;
					nPosY=rcClient.Height()-m_ToolBarBk.GetHeight()-8;
				}
				else if (enDrawPos_BulletBack==DrawPos)
				{
					if (0==nIndex)
						nPosY=rcClient.Height()/2+195;
					else if (1==nIndex)
						nPosY=rcClient.Height()/2+173;
					else if (2==nIndex)
						nPosY=rcClient.Height()/2+151;
					nPosX=95;

				}
				else if (enDrawPos_Bullet==DrawPos)
				{
					if (0==nIndex)
						nPosY=rcClient.Height()/2+198;
					else if (1==nIndex)
						nPosY=rcClient.Height()/2+198-22;
					else if (2==nIndex)
						nPosY=rcClient.Height()/2+198-22-22;

					nPosX=116;					
				}
				else if (enDrawPos_CoinBack==DrawPos)
				{
					nPosX=95;
					nPosY=rcClient.Height()/2+218;
				}
				else if (enDrawPos_Coin==DrawPos)
				{
					nPosX=122;
					nPosY=rcClient.Height()/2+219;
				}				
			}
			else
			{
				if (enDrawPos_AccountBack==DrawPos)
				{
					nPosX=55;
					nPosY=rcClient.Height()-m_ToolBarBk.GetHeight()-21;
				}
				else if (enDrawPos_Account==DrawPos)
				{
					nPosX=102;
					nPosY=539;
				}
				else if (enDrawPos_BulletBack==DrawPos)
				{
					nPosX=51;
					nPosY=rcClient.Height()/2+188;
				}
				else if (enDrawPos_Bullet==DrawPos)
				{
					nPosX=77;
					nPosY=rcClient.Height()/2+191;
				}
				else if (enDrawPos_CoinBack==DrawPos)
				{
					nPosX=51;
					nPosY=rcClient.Height()/2+208;
				}
				else if (enDrawPos_Coin==DrawPos)
				{
					nPosX=77;
					nPosY=rcClient.Height()/2+208;
				}				
			}
		}
		else
		{
			//�Լ��ж�
			if (m_wMeChairID==wChairID)
			{
				if (enDrawPos_MeGoldBack==DrawPos)
				{
					nPosX=820+70;
					nPosY=rcClient.Height()-m_ToolBarBk.GetHeight()-12;
				}
				else if (enDrawPos_MeGold==DrawPos)
				{
					nPosX=854+70;
					nPosY=rcClient.Height()-m_ToolBarBk.GetHeight()-8;
				}
				else if (enDrawPos_BulletBack==DrawPos)
				{
					if (0==nIndex)
						nPosY=492;
					else if (1==nIndex)
						nPosY=492-22;
					else if (2==nIndex)
						nPosY=492-22-22;

					nPosX=805+70;
				}
				else if (enDrawPos_Bullet==DrawPos)
				{
					if (0==nIndex)
						nPosY=495;
					else if (1==nIndex)
						nPosY=495-22;
					else if (2==nIndex)
						nPosY=495-22-22;

					nPosX=829+70;					
				}
				else if (enDrawPos_CoinBack==DrawPos)
				{
					nPosX=805+70;
					nPosY=515;
				}
				else if (enDrawPos_Coin==DrawPos)
				{
					nPosX=833+70;
					nPosY=516;
				}				
			}
			else
			{
				if (enDrawPos_AccountBack==DrawPos)
				{
					nPosX=869;
					nPosY=rcClient.Height()-m_ToolBarBk.GetHeight()-21;
				}
				else if (enDrawPos_Account==DrawPos)
				{
					nPosX=915;
					nPosY=539;
				}
				else if (enDrawPos_BulletBack==DrawPos)
				{
					nPosX=865;
					nPosY=485;
				}
				else if (enDrawPos_Bullet==DrawPos)
				{
					nPosX=889;
					nPosY=488;
				}
				else if (enDrawPos_CoinBack==DrawPos)
				{
					nPosX=865;
					nPosY=505;
				}
				else if (enDrawPos_Coin==DrawPos)
				{
					nPosX=891;
					nPosY=507;
				}				
			}
		}
	}
	else
	{
		//����ж�
		if (wChairID%2!=0)
		{
			if (enDrawPos_AccountBack==DrawPos)
			{
				nPosX=55;
				nPosY=0;
			}
			else if (enDrawPos_Account==DrawPos)
			{
				nPosX=102;
				nPosY=8;
			}
			else if (enDrawPos_BulletBack==DrawPos)
			{
				nPosX=51;
				nPosY=60;
			}
			else if (enDrawPos_Bullet==DrawPos)
			{
				nPosX=75;
				nPosY=63;
			}
			else if (enDrawPos_CoinBack==DrawPos)
			{
				nPosX=51;
				nPosY=33;
			}
			else if (enDrawPos_Coin==DrawPos)
			{
				nPosX=77;
				nPosY=35;
			}
		}
		else
		{
			if (enDrawPos_AccountBack==DrawPos)
			{
				nPosX=869;
				nPosY=0;
			}
			else if (enDrawPos_Account==DrawPos)
			{
				nPosX=915;
				nPosY=8;
			}
			else if (enDrawPos_BulletBack==DrawPos)
			{
				nPosX=865;
				nPosY=60;
			}
			else if (enDrawPos_Bullet==DrawPos)
			{
				nPosX=889;
				nPosY=63;
			}
			else if (enDrawPos_CoinBack==DrawPos)
			{
				nPosX=865;
				nPosY=33;
			}
			else if (enDrawPos_Coin==DrawPos)
			{
				nPosX=891;
				nPosY=35;
			}
		}
	}
}

//�ӵ�����
enBulletCountKind CGameClientViewDX::GetCurUseBulletKind(WORD wChairID)
{
	//�Ϸ��ж�
	ASSERT(wChairID<GAME_PLAYER);
	if (!(wChairID<GAME_PLAYER)) return enBulletCountKind_100;

	if (m_lBulletCount[wChairID][enBulletCountKind_800]>0) return enBulletCountKind_800;
	else if (m_lBulletCount[wChairID][enBulletCountKind_600]>0) return enBulletCountKind_600;
	else if (m_lBulletCount[wChairID][enBulletCountKind_100]>0) return enBulletCountKind_100;

	return enBulletCountKind_100;
}

//������
bool CGameClientViewDX::_UserShoot(WORD wChairID, bool bReleaseKey)
{
	//�Թ��ж�
	if (true==m_bLookonMode) return false;

	//�����ж�
	CWnd *pActiveWnd=GetActiveWindow();
	CWnd *pMainFrame=AfxGetMainWnd();
	if (NULL==pActiveWnd) return false;
	if (NULL==pMainFrame) return false;
	if (pActiveWnd->GetSafeHwnd()!=pMainFrame->GetSafeHwnd()) return false;

	//ʱ���ж�
	bool bContinuous=(m_lBulletCount[wChairID][enBulletCountKind_600]+m_lBulletCount[wChairID][enBulletCountKind_800]>0)?true:false;
	if (true==bContinuous && (m_dwLastShootTime+TIME_SHOOT_SPEED_F)>GetTickCount()) return false;
	else if (false==bContinuous && ((m_dwLastShootTime+TIME_SHOOT_SPEED_S)>GetTickCount() || false==bReleaseKey)) return false;

	//����ʱ��
	m_dwLastShootTime=GetTickCount();

	//�Ϸ��ж�
	WORD wBulletCount=m_Cannon.GetBulletCount(wChairID);
	LONG lShootCount=0;
	enBulletCountKind BulletCountKind=enBulletCountKind_End;
	if (m_lBulletCount[wChairID][enBulletCountKind_800]>0) BulletCountKind=enBulletCountKind_800;
	else if (m_lBulletCount[wChairID][enBulletCountKind_600]>0) BulletCountKind=enBulletCountKind_600;
	else if (m_lBulletCount[wChairID][enBulletCountKind_100]>0) BulletCountKind=enBulletCountKind_100;

	//������Ŀ
	LONG lPreBulletCount=m_lBulletCount[wChairID][enBulletCountKind_800]+m_lBulletCount[wChairID][enBulletCountKind_600]+m_lBulletCount[wChairID][enBulletCountKind_100];

	//�Ϸ��ж�
	if (BulletCountKind!=enBulletCountKind_End)
	{
		if (m_lBulletCount[wChairID][BulletCountKind]>=wBulletCount)
		{
			lShootCount=wBulletCount;
			m_lBulletCount[wChairID][BulletCountKind]-=wBulletCount;			
		}
		else
		{
			lShootCount=m_lBulletCount[wChairID][BulletCountKind];
			m_lBulletCount[wChairID][BulletCountKind]=0;
		}

		//�л��ӵ�
		if (m_lBulletCount[wChairID][BulletCountKind]<wBulletCount && m_lBulletCount[wChairID][BulletCountKind]>0) m_Cannon.SetBulletCount(wChairID, m_lBulletCount[wChairID][BulletCountKind],false);
		else if (m_lBulletCount[wChairID][enBulletCountKind_600]>0) m_Cannon.SetBulletCount(wChairID, min(m_lBulletCount[wChairID][enBulletCountKind_600],wBulletCount),false);
		else if (m_lBulletCount[wChairID][enBulletCountKind_100]>0) m_Cannon.SetBulletCount(wChairID, min(m_lBulletCount[wChairID][enBulletCountKind_100],wBulletCount),false);

		//���÷�Χ
		CRect rcBound;
		GetClientRect(rcBound);
		BYTE byBulletCount=m_Cannon.GetBulletCount(wChairID);
		if (IsSameSide(m_wMeChairID,wChairID))
		{
			if (wChairID%2==0) rcBound.right-=(SHOOT_ROUND_SHRINK_X*(5-byBulletCount));
			else rcBound.left+=(SHOOT_ROUND_SHRINK_X*(5-byBulletCount));
			rcBound.top+=(SHOOT_ROUND_SHRINK_Y*(5-byBulletCount));
		}
		else
		{
			if (wChairID%2!=0) rcBound.right-=(SHOOT_ROUND_SHRINK_X*(5-byBulletCount));
			else rcBound.left+=(SHOOT_ROUND_SHRINK_X*(5-byBulletCount));
			rcBound.bottom-=(SHOOT_ROUND_SHRINK_Y*(5-byBulletCount));
		}
		m_Bullet.SetBound(rcBound);

		//�����ڵ�
		DWORD dwCptrFishID[5]={0};
		DWORD dwBulletID=m_Bullet.Shoot(wChairID,true, dwCptrFishID, 5, m_Cannon.GetBenchmarkPos(m_wMeChairID).x,m_Cannon.GetBenchmarkPos(m_wMeChairID).y,
			m_Cannon.GetAngleCan(m_wMeChairID), BulletCountKind,byBulletCount);
		m_Cannon.Shoot(m_wMeChairID);

		//�Ϸ��ж�
		ASSERT(0L!=dwBulletID);
		if (0L==dwBulletID) return false;

		//������Ϣ
		if (NULL!=m_pGameClientDlg) m_pGameClientDlg->PostMessage(WM_SHOOT,WPARAM(dwBulletID),MAKELPARAM(BulletCountKind,lShootCount));

		//��������
		TCHAR *pSiloSound[]={TEXT("Effect\\Silo0.mp3"),TEXT("Effect\\Silo1.mp3"),TEXT("Effect\\Silo2.mp3"),TEXT("Effect\\Silo3.mp3"),TEXT("Effect\\Silo4.mp3")};
		PlayMp3Sound(pSiloSound[wBulletCount-1],false);

		//�����ж�
		LONG lLeftBulletCount=m_lBulletCount[wChairID][enBulletCountKind_800]+m_lBulletCount[wChairID][enBulletCountKind_600]+m_lBulletCount[wChairID][enBulletCountKind_100];
		if (lPreBulletCount>=40 && lLeftBulletCount>0 && lLeftBulletCount<40) 
		{
			//���ñ�ʶ
			m_bNoBulletWarn=true;
			m_dwNoBulletWarnTime=GetTickCount();

			//����λ��
			int nDrawPosX,nDrawPosY;
			GetDrawPos(m_wMeChairID,enDrawPos_BulletBack,nDrawPosX,nDrawPosY);
			m_NoBulletWarn.SetDrawPos(nDrawPosX-70,nDrawPosY-50);

			//����ʱ��
			SetTimer(IDI_NO_BULLET_WARN,6000,NULL);
		}
		else if (lLeftBulletCount>40)
		{
			m_bNoBulletWarn=false;
		}

		//��������
		if (true==m_bNoBulletWarn) PlayMp3Sound(TEXT("Effect\\NoBullet.mp3"),false);

		//����ʱ��
		SetTimer(IDI_IDLE_TIME,1000,NULL);
		m_wIdleTime=IDLE_TIME;
	}
	else
	{
		//������Ϣ
		if (NULL!=m_pGameClientDlg) m_pGameClientDlg->PostMessage(WM_SHOW_COIN_SHOP/*WM_SHOW_BULLET_SHOP*/,0,0);
	}

	return true;
}

//�˳���ѯ
bool CGameClientViewDX::QuitGameInquire()
{
	//�Ϸ��ж�
	if (INVALID_CHAIR==m_wMeChairID) return false;
	if (true==m_bLookonMode) return false;

	//�ӵ���Ŀ
	for (enBulletCountKind BulletCountKind=enBulletCountKind_100; BulletCountKind<enBulletCountKind_End; BulletCountKind=enBulletCountKind(BulletCountKind+1))
	{
		if (m_lBulletCount[m_wMeChairID][BulletCountKind]>0) return true;
	}

	//�����Ŀ
	if (m_lCoinCount[m_wMeChairID]>0) return true;

	return false;
}

//��ʾ�ɼ�
bool CGameClientViewDX::ShowScoreView()
{
	//��������
	CFishScoreView ScoreView(this, m_lCellScore[m_wMeChairID]);

	//������Ϣ
	ScoreView.SetScoreInfo(m_lBulletCount[m_wMeChairID], m_wCptrFishCount, m_lCoinCount[m_wMeChairID], m_lBulletCharge);

	//��ʾ�Ի���
	if (IDOK==ScoreView.DoModal())
	{
		//���ý���
		SetFocus();

		//������Ϣ
		if (NULL!=m_pGameClientDlg) m_pGameClientDlg->SendData(SUB_C_CALC_SCORE);		
	}

	return true;
}

//�Թ�ģʽ
void CGameClientViewDX::SetLokonMode(bool bLookonMode)
{
	m_bLookonMode=bLookonMode;
	if (true==m_bLookonMode) KillTimer(IDI_IDLE_TIME);
}

//�л�����
void CGameClientViewDX::SetChangeScene(bool bWaveLToR, bool bChange, enSeaSceneKind SeaSceneKind)
{
	//�ı��ж�
	if (true==bChange)
	{
		//�޸�ʱ��
		/*WORD nEnumIndex=0;
		tagFishInfo *pFish=NULL;*/
		//for (enFishKind FishKind=enFishKind_1; FishKind<enFishKind_No; FishKind=enFishKind(FishKind+1))
		//{
		//	//nEnumIndex=0;
		//	//do
		//	//{
		//	//	//ö����Ⱥ
		//	//	pFish=g_FishGroup[FishKind].EnumFish(nEnumIndex++);
		//	//	if (NULL==pFish) break;

		//	//	//����ʱ��
		//	//	pFish->bySpaceTimes=0;

		//	//} while (true);
		//}

		//���ñ���
		m_NewSeaScene=SeaSceneKind;
		m_bWaveEffect=false;
		m_bWaveEffect=false;
		m_bWaveLToR=bWaveLToR;
		m_nSceneMovePosX=0;
		m_bWaveComing=true;		

		//����ʱ��
		SetTimer(IDI_CHANGE_SCENE,TIME_CHANGE_SCENE,NULL);

		//�ǳ�֪ͨ
		CFishGroup::SetWaveComing(true);

		//��������
		PlayMp3Sound(TEXT("Effect\\WaveEnter.mp3"),false);
	}
	else
	{
		//�л��ж�
		if (m_CurSeaScene!=SeaSceneKind)
		{
			//������Դ
			LoadSeaScene(SeaSceneKind);

			//�ͷ���Դ
			ReleaseSeaScene(m_CurSeaScene);

			//���ñ���
			m_CurSeaScene=SeaSceneKind;
		}	
	}
}

//�ӵ���Ŀ
LONG CGameClientViewDX::GetBulletCount(WORD wChairID, enBulletCountKind BulletCountKind)
{
	//�Ϸ��ж�
	ASSERT(wChairID<GAME_PLAYER);
	ASSERT(BulletCountKind<enBulletCountKind_End);
	if (!(wChairID<GAME_PLAYER)) return 0;
	if (!(BulletCountKind<enBulletCountKind_End)) return 0;

	return m_lBulletCount[wChairID][BulletCountKind];
}

//��ȡ���
CAudioPlayer *CGameClientViewDX::GetaudioPlayer(LPCTSTR pszAudioName)
{
	//��������
	CAudioPlayer *pAudioPlayer=NULL;
	int nIndex=0;

	//�����ж�
	/*for (int i=0; i<MAX_SYN_SOUND; ++i)
	{
	if (0==lstrcmpi(pszAudioName,m_AudioPlayerAsyn[i].GetAudioName()) && m_AudioPlayerAsyn[i].GetDeviceType()!=0) 
	{
	pAudioPlayer=&m_AudioPlayerAsyn[i];
	nIndex=i;
	break;
	}
	}*/

	//Ѱ�ҿ�λ
	if (NULL==pAudioPlayer)
	{
		for (int i=0; i<MAX_SYN_SOUND; ++i)
		{
			if (m_AudioPlayerAsyn[i].GetDeviceType()==0) 
			{
				pAudioPlayer=&m_AudioPlayerAsyn[i];
				nIndex=i;
				break;
			}
		}
	}
	else 
	{
		//�ر��ļ�
		pAudioPlayer->Stop();
		pAudioPlayer->RePlay();
		return NULL;
	}

	//�����ж�
	if (NULL!=pAudioPlayer && NULL==pAudioPlayer->GetSafeHwnd()) 
	{
		pAudioPlayer->Create(NULL,NULL,WS_CHILD,CRect(0,0,0,0),this,500+nIndex,NULL);
		//pAudioPlayer->SetGameFrameDlg(this);
	}

	return pAudioPlayer;
}

//��������
bool CGameClientViewDX::PlayMp3Sound(LPCTSTR pszSoundName, bool bSyn)
{
	//�����ж�
	if (m_pGameClientDlg->IsEnableSound()==false) return false;

	try
	{
		//����·��
		TCHAR szFullPath[MAX_PATH]={0};
		_sntprintf(szFullPath,CountArray(szFullPath),TEXT("%sFish\\Sound\\%s"),g_szWorkDir,pszSoundName);

		//�첽�ж�
		if (false==bSyn)
		{
			CAudioPlayer *pAudioPlayer=GetaudioPlayer(szFullPath);
			if (NULL!=pAudioPlayer) pAudioPlayer->Play(szFullPath);
		}
		else
		{
			m_AudioPlayerSyn.Play(szFullPath,false);
		}
	}
	catch (...)
	{
		return false;
	}

	return true;
}

//��������
bool CGameClientViewDX::PlayMp3Sound(LPCTSTR pszPath, LPCTSTR pszSoundName, bool bSyn)
{
	//�����ж�
	if (m_pGameClientDlg->IsEnableSound()==false) return false;

	try
	{
		//����·��
		TCHAR szFullPath[MAX_PATH]={0};
		_sntprintf(szFullPath,CountArray(szFullPath),TEXT("%s%s"),pszPath,pszSoundName);

		//�첽�ж�
		if (false==bSyn)
		{
			CAudioPlayer *pAudioPlayer=GetaudioPlayer(szFullPath);
			if (NULL!=pAudioPlayer) pAudioPlayer->Play(szFullPath);
		}
		else
		{
			m_AudioPlayerSyn.Play(szFullPath,false);
		}
	}
	catch (...)
	{
		return false;
	}

	return true;
}

//��������
bool CGameClientViewDX::PlayMp3SoundBG(LPCTSTR pszSoundName, bool bLoop)
{
	//�����ж�
	if (m_pGameClientDlg->IsEnableSound()==false) return false;
	//#ifdef _DEBUG
	//	InsertSystemString(TEXT("PlayMp3SoundBG0"));
	//#endif

	try
	{
		//����·��
		TCHAR szFullPath[MAX_PATH]={0};
		_sntprintf(szFullPath,CountArray(szFullPath),TEXT("%sFish\\Sound\\%s"),g_szWorkDir,pszSoundName);

		//��������
		m_AudioPlayerBG.Play(szFullPath,bLoop);
	}
	catch (...)
	{
		return false;
	}

	return true;
}

//��������
bool CGameClientViewDX::PlayMp3SoundBG(LPCTSTR pszPath, LPCTSTR pszSoundName, bool  bLoop)
{
	//�����ж�
	if (m_pGameClientDlg->IsEnableSound()==false) return false;

	//#ifdef _DEBUG
	//	InsertSystemString(TEXT("PlayMp3SoundBG"));
	//#endif

	try
	{
		//����·��
		TCHAR szFullPath[MAX_PATH]={0};
		_sntprintf(szFullPath,CountArray(szFullPath),TEXT("%s%s"),pszPath,pszSoundName);

		//��������
		m_AudioPlayerBG.Play(szFullPath,bLoop);
	}
	catch (...)
	{
		return false;
	}

	return true;
}

//ֹͣ����
bool CGameClientViewDX::StopMp3SoundBG()
{
	//#ifdef _DEBUG
	//	InsertSystemString(TEXT("StopMp3SoundBG"));
	//#endif

	try
	{
		m_AudioPlayerBG.Stop();
	}
	catch (...)
	{
		return false;
	}

	return true;
}
//��ͣ����
bool CGameClientViewDX::PauseMp3SoundBG()
{
	try
	{
		m_AudioPlayerBG.Pause();
	}
	catch (...)
	{
		return false;
	}

	return true;
}

//����
LRESULT CGameClientViewDX::OnNetOpen(WPARAM wParam, LPARAM lParam)
{
	//��������
	PlayMp3Sound(TEXT("Effect\\NetOpen.mp3"),false);

	return 0L;
}

//��ǰ��Ŀ
LONG CGameClientViewDX::GetCurBulletCount(WORD wChairID)
{
	//�Ϸ��ж�
	ASSERT(wChairID<GAME_PLAYER);
	if (!(wChairID<GAME_PLAYER)) return 1;

	if (m_lBulletCount[wChairID][enBulletCountKind_800]>0) return m_lBulletCount[wChairID][enBulletCountKind_800];
	else if (m_lBulletCount[wChairID][enBulletCountKind_600]>0) return m_lBulletCount[wChairID][enBulletCountKind_600];
	else if (m_lBulletCount[wChairID][enBulletCountKind_100]>0) return m_lBulletCount[wChairID][enBulletCountKind_100];

	return 1;
}

//�л��ڵ�
void CGameClientViewDX::SwitchCannon()
{
	//�Ϸ��ж�
	if (INVALID_CHAIR==m_wMeChairID) return;

	//�Թ��ж�
	if (true==m_bLookonMode) return;

	//��Ŀ�ж�
	int nBulletCount=m_Cannon.GetBulletCount(m_wMeChairID);
	LONG lCurBulletCount=GetCurBulletCount(m_wMeChairID);
	if (nBulletCount==lCurBulletCount && lCurBulletCount<5) 
		nBulletCount=1;
	else
		nBulletCount=(nBulletCount!=4) ? ((nBulletCount+1)%MAX_BULLET_COUNT):5;

	//�����ӵ�
	int nMinCount=min(nBulletCount,lCurBulletCount);
	m_Cannon.SetBulletCount(m_wMeChairID, nMinCount,true);

	//��������
	PlayMp3Sound(TEXT("Effect\\SwitchSilo.mp3"),false);
}
////////////////////////////////////////////////////////////////////////////////////

BOOL CGameClientViewDX::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	//���ù��
	SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));

	return __super::OnSetCursor(pWnd, nHitTest, message);
}

//��Ϣ����
BOOL CGameClientViewDX::PreTranslateMessage(MSG* pMsg)
{
	//��������
	if (WM_KEYDOWN==pMsg->message)
	{
		//�Ϸ��ж�
		bool bKeyDown=true;
		CWnd *pActiveWnd=GetActiveWindow();
		CWnd *pMainFrame=AfxGetMainWnd();
		if (INVALID_CHAIR==m_wMeChairID) bKeyDown=false;
		if (true==m_bLookonMode) bKeyDown=false;
		if (NULL==pActiveWnd) bKeyDown=false;
		if (NULL==pMainFrame) bKeyDown=false;
		if (pActiveWnd->GetSafeHwnd()!=pMainFrame->GetSafeHwnd()) bKeyDown=false;
		if (false==bKeyDown) return __super::PreTranslateMessage(pMsg);

		//�����ж�
		if (pMsg->wParam==49 || pMsg->wParam==50 || pMsg->wParam==51 || pMsg->wParam==52 || pMsg->wParam==53)
		{
			LONG nMinCount=min(INT(pMsg->wParam-49+1),GetCurBulletCount(m_wMeChairID));
			m_Cannon.SetBulletCount(m_wMeChairID,nMinCount,true);

			//��������
			PlayMp3Sound(TEXT("Effect\\SwitchSilo.mp3"),false);
		}
		else if (VK_SPACE==pMsg->wParam || VK_UP==pMsg->wParam)
		{
			//�ո��ж�
			if (VK_SPACE==pMsg->wParam)
			{
				//�����ж�
				CWnd *pFocusWnd=GetFocus();
				if (NULL==pFocusWnd) return TRUE;
				if (pFocusWnd->GetSafeHwnd()!=GetSafeHwnd()) return TRUE;
			}

			//�����ж�
			_UserShoot(m_wMeChairID, m_bReleaseKey);

			//���ñ�־
			m_bReleaseKey=false;
		}
		else if (VK_LEFT==pMsg->wParam)
		{
			//���ý���
			SetFocus();

			//���ýǶ�
			float fNowAngle=m_Cannon.GetAngleCan(m_wMeChairID);
			fNowAngle-=0.1f;
			if (fNowAngle<ME_MIN_CAN_ANGLE) fNowAngle=ME_MIN_CAN_ANGLE;
			m_Cannon.SetAngleCan(m_wMeChairID,fNowAngle);
		}
		else if (VK_RIGHT==pMsg->wParam)
		{
			//���ý���
			SetFocus();

			//���ýǶ�
			float fNowAngle=m_Cannon.GetAngleCan(m_wMeChairID);
			fNowAngle+=0.1f;
			if (fNowAngle>ME_MAX_CAN_ANGLE) fNowAngle=ME_MAX_CAN_ANGLE;
			m_Cannon.SetAngleCan(m_wMeChairID,fNowAngle);
		}
		else if (VK_DOWN==pMsg->wParam)
		{
			//�л��ڵ�
			if (m_dwLastSwitchTime+300<=GetTickCount())
			{
				SwitchCannon();	
				m_dwLastSwitchTime=GetTickCount();
			}
		}			

		return TRUE;
	}
	else if (WM_KEYUP==pMsg->message)
	{
		m_bReleaseKey=true;
	}

	return __super::PreTranslateMessage(pMsg);
}


BOOL CGameClientViewDX::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	//��������
	if(message==WM_KEYDOWN)
	{
		int a=0;
		//OnNcHitTest(wParam,lParam);
		//return TRUE;
	}
	////WM_NCHITTEST��Ϣ
	//if(message==WM_NCHITTEST)
	//{
	//	int a=0;
	//	//OnNcHitTest(wParam,lParam);
	//	//return TRUE;
	//}
	//////�켣��Ϣ
	////if(message==WM_FISH_CALC_TRACE)
	////{
	////	OnFinishCalcTrace(wParam,lParam);
	////	return TRUE;
	////}
	//////���� 
	////if(message==WM_NET_OPEN)
	////{
	////	OnNetOpen(wParam,lParam);
	////	return TRUE;
	////}
	////�˳���Ϸ,
	//if(message==WM_QUIT_GAME)
	//{
	//	OnQuitGame(wParam,lParam);
	//	return TRUE;
	//}
	////������Ⱥ
	//if(message==WM_HIT_FISH)
	//{
	//	OnHitFish(wParam,lParam);
	//	return TRUE;
	//}
	////�������(ʹ��������)
	//if(message==WM_HIT_Laser)
	//{
	//	OnHitLaser(wParam,lParam);
	//	return TRUE;
	//}
	return __super::OnWndMsg(message,wParam,lParam,pResult);
}



