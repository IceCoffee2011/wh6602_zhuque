#include "StdAfx.h"
#include "Resource.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include "DlgBulletShop.h"
#include "DlgCellScoreOption.h"
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_WM_NCHITTEST()

	//ON_MESSAGE(WM_SHOW_BULLET_SHOP,OnShowBulletShop)
	ON_MESSAGE(WM_SHOOT,OnShoot)
	//ON_MESSAGE(WM_CAPTURE_FISH,OnCaptureFish)
	ON_MESSAGE(WM_QUIT_GAME,OnQuitGame)
	ON_MESSAGE((WM_USER+888),OnEnableSound)
	ON_MESSAGE(WM_HIT_FISH,OnHitFish)

	//ON_MESSAGE(WM_USER+888,OnCapture)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg(): CGameFrameDlg(&m_GameClientView)
{
	//��ʼ����
	ZeroMemory(m_lBulletCharge,sizeof(m_lBulletCharge));
	m_BulletCountKindSelected=enBulletCountKind_600;
	m_lCellScore=0L;

	return;
}

//��������
CGameClientDlg::~CGameClientDlg()
{
}

//��ʼ����
bool  CGameClientDlg::InitGameFrame()
{
	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//�޸ķ��
	ModifyStyle(WS_MAXIMIZEBOX|WS_MINIMIZEBOX|WS_SIZEBOX,0);

	return true;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	return;
}
//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
	//�˿˿���

	return;
}

//��������
void  CGameClientDlg::ResetGameFrame()
{
	//�ں�����

	return ;
}

//ʱ����Ϣ
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	return false;
}

//������Ϣ
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pData, WORD wDataSize)
{
	//��Ϣ����
	switch (wSubCmdID)
	{
	case SUB_S_TRACE_POINT:				//�켣����
		{
			return OnSubFishTrace(pData, wDataSize);
		}
	case SUB_S_USER_SHOOT:				//�����ڵ�
		{
			return OnSubUserShoot(pData, wDataSize);
		}
	case SUB_S_CAPTURE_FISH:			//������Ⱥ
		{
			return OnSubCaptureFish(pData, wDataSize);
		}
	case SUB_S_BULLET_COUNT:			//�ӵ���Ŀ
		{
			return OnSubBulletCount(pData, wDataSize);
		}
	case SUB_S_COIN_COUNT:				//�����Ŀ
		{
			return OnSubCoinCount(pData, wDataSize);
		}
	case SUB_S_FISH_COUNT:				//��Ⱥ��Ŀ
		{
			return OnSubCptrFishCount(pData, wDataSize);
		}
	case SUB_S_CHANGE_SCENE:			//�л�����
		{
			return OnSubChangeScene(pData, wDataSize);
		}
	case SUB_S_SCORE_INFO:				//������Ϣ
		{
			return OnSubScoreInfo(pData, wDataSize);
		}
	case SUB_S_SET_CELL_SCORE:			//��Ԫ����
		{
			return OnSubSetCellScore(pData, wDataSize);
		}
	case SUB_S_BONUS_INFO:				//�ʽ���Ϣ
		{
			return OnSubBonusInfo(pData, wDataSize);
		}
	case SUB_S_GAME_MESSAGE:			//��Ϸ��Ϣ
		{
			return OnSubGameMessage(pData, wDataSize);
		}
	}
	return false;
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStatus, bool bLookonOther, const void * pData, WORD wDataSize)
{
	//��������
	switch (cbGameStatus)
	{
	case GS_FREE:
	case GS_PLAYING:
		{
			//�Ϸ��ж�
			ASSERT(wDataSize==sizeof(CMD_S_GameScene));
			if (!(wDataSize==sizeof(CMD_S_GameScene))) return false;

			//������λ
			m_GameClientView.m_GameClientViewDX.SetMeChairID(GetMeChairID());

			//����ģʽ
			m_GameClientView.m_GameClientViewDX.SetLokonMode(IsLookonMode());

			//����ת��
			CMD_S_GameScene *pGameScene=(CMD_S_GameScene *)pData;

			//�Ϸ��ж�
			ASSERT(pGameScene->CureSeaSceneKind<enSeaSceneKind_No);
			if (!(pGameScene->CureSeaSceneKind<enSeaSceneKind_No)) return false;

			//������Ϣ
			CopyMemory(m_lBulletCharge,pGameScene->lBulletCharge,sizeof(m_lBulletCharge));
			m_GameClientView.m_GameClientViewDX.SetBulletCharge(m_lBulletCharge);

			//���ó���
			m_GameClientView.m_GameClientViewDX.SetChangeScene(true, false, pGameScene->CureSeaSceneKind);

			//��Ԫ����
			for (int i=0; i<GAME_PLAYER; ++i) 
			{
				m_GameClientView.m_GameClientViewDX.SetCellScore(i,pGameScene->lUserCellScore[i]);
				if (i==GetMeChairID()) m_lCellScore=pGameScene->lUserCellScore[i];
			}

			////�����ӵ�
			//if (false==IsLookonMode())
			//{
			//	//���Ӻ���
			//	WORD wMeChairID=GetMeChairID();

			//	//��Ԫ����
			//	if (0==pGameScene->lUserCellScore[wMeChairID])
			//	{
			//		//�����Ի���
			//		CDlgCellScoreOption DlgCellScoreOption;
			//		DlgCellScoreOption.m_pCenterView=&m_GameClientView.m_GameClientViewDX;
			//		CopyMemory(DlgCellScoreOption.m_lCellScore,pGameScene->lCellScore,sizeof(DlgCellScoreOption.m_lCellScore));
			//		//DlgCellScoreOption.DoModal();

			//		//�Ϸ��ж�
			//		LONG lCellScore=DlgCellScoreOption.m_lCellScoreSelect;
			//		bool bReset=false;
			//		if (lCellScore!=pGameScene->lCellScore[0])
			//		{
			//			tagUserData const *pMeUserData=GetUserData(wMeChairID);
			//			if (NULL!=pMeUserData)
			//			{
			//				LONGLONG lUserScore=pMeUserData->lScore;
			//				if (lUserScore<(lCellScore*m_lBulletCharge[0]))
			//				{
			//					lCellScore=pGameScene->lCellScore[0];
			//					for (int i=0; i<CountArray(pGameScene->lCellScore); ++i)
			//					{
			//						if (lUserScore>=(pGameScene->lCellScore[i]*m_lBulletCharge[0]))
			//						{
			//							lCellScore=pGameScene->lCellScore[i];
			//							bReset=true;
			//						}
			//					}
			//				}
			//			}
			//		}
			//		if (true==bReset)
			//		{
			//			TCHAR szBuffer[256];
			//			_sntprintf(szBuffer,CountArray(szBuffer),TEXT("�Բ������Ľ�Ҳ�����ϵͳΪ�����������˱�������ǰ����Ϊ %ld"),lCellScore);
			//			MessageBox(szBuffer,TEXT("��ܰ��ʾ"));
			//		}

			//		//������Ϣ
			//		CMD_C_SetCellScore SetCellScore={0};
			//		SetCellScore.lCellScore=lCellScore;
			//		SendData(SUB_C_SET_CELL_SCORE,&SetCellScore,sizeof(SetCellScore));

			//		//������Ϣ
			//		m_lCellScore=DlgCellScoreOption.m_lCellScoreSelect;
			//		m_GameClientView.m_GameClientViewDX.SetCellScore(wMeChairID,DlgCellScoreOption.m_lCellScoreSelect);
			//	}

			//	//�����ӵ�
			//	ShowBulletShop();
			//}

			return true;
		}
	}
	return false;
}

//������Ϣ
UINT CGameClientDlg::OnNcHitTest(CPoint point)
{
	UINT uResult=CGameFrameDlg::OnNcHitTest(point);
	if (HTBOTTOM==uResult || HTRIGHT==uResult || HTLEFT==uResult || HTTOP==uResult) uResult=HTCLIENT;

	return uResult;
}

//��Ⱥ�켣
bool CGameClientDlg::OnSubFishTrace(const void * pData, WORD wDataSize)
{
	//�Ϸ��ж�
	ASSERT((wDataSize%sizeof(CMD_S_FishTrace))==0);
	if (!((wDataSize%sizeof(CMD_S_FishTrace))==0)) 
	{
		TraceString(TEXT("(!((wDataSize%sizeof(CMD_S_FishTrace))==0))"));
		return false;
	}

	//����ת��
	WORD wTraceCount=wDataSize/sizeof(CMD_S_FishTrace);
	CMD_S_FishTrace *pFishTrace=(CMD_S_FishTrace *)pData;

	//���ù켣
	for (int i=0; i<wTraceCount; ++i)
	{
		m_GameClientView.m_GameClientViewDX.AddTrace(pFishTrace->nInitX,pFishTrace->nInitY,pFishTrace->nInitCount,pFishTrace->FishKind,pFishTrace->dwFishID,pFishTrace->bRegular);
		pFishTrace++;
	}	

	return true;
}

//�����ڵ�
bool CGameClientDlg::OnSubUserShoot(const void * pData, WORD wDataSize)
{
	//�Ϸ��ж�
	ASSERT(wDataSize==sizeof(CMD_S_UserShoot));
	if (!(wDataSize==sizeof(CMD_S_UserShoot))) return false;

	//����ת��
	CMD_S_UserShoot *pUserShoot=(CMD_S_UserShoot *)pData;

	//�����ڵ�
	m_GameClientView.m_GameClientViewDX.UserShoot(pUserShoot->wChairID, pUserShoot->fAngle, pUserShoot->BulletCountKind);

	//�ӵ���Ŀ
	m_GameClientView.m_GameClientViewDX.SetShootBulletCount(pUserShoot->wChairID, pUserShoot->byShootCount);

	return true;
}

//������Ⱥ
bool CGameClientDlg::OnSubCaptureFish(const void * pData, WORD wDataSize)
{
	//�Ϸ��ж�
	ASSERT(wDataSize==sizeof(CMD_S_CaptureFish));
	if (!(wDataSize==sizeof(CMD_S_CaptureFish))) return false;

	//����ת��
	CMD_S_CaptureFish *pCaptureFish=(CMD_S_CaptureFish *)pData;

	//�Ϸ��ж�
	ASSERT(pCaptureFish->FishKind<enFishKind_No);
	if (!(pCaptureFish->FishKind<enFishKind_No)) 
	{
		TraceString(TEXT("(!(pCaptureFish->FishKind<enFishKind_No))"));
		return false;
	}

	//�����ڵ�
	m_GameClientView.m_GameClientViewDX.SetFishStatus(pCaptureFish->FishKind, pCaptureFish->dwFishID, enFishStatus_Die);

	//���ӽ��
	enFishKind FishKind=m_GameClientView.m_GameClientViewDX.GetFishKind(pCaptureFish->dwFishID);
	if (FishKind<enFishKind_No)
	{
		tagFishInfo *pFishInfo=g_FishGroup[FishKind].GetFish(pCaptureFish->dwFishID);
		LONG lScore=pCaptureFish->lFishScore;//g_nFishScore[FishKind];
		if (NULL!=pFishInfo &&pFishInfo->wTracePtIndex<pFishInfo->wTracePtCount)
		{
			m_GameClientView.m_GameClientViewDX.UserAddCoin(pCaptureFish->wChairID, pFishInfo->ptTrace[pFishInfo->wTracePtIndex].x,pFishInfo->ptTrace[pFishInfo->wTracePtIndex].y,lScore);
			if (enFishKind_12==FishKind) m_GameClientView.m_GameClientViewDX.AddBonus(pCaptureFish->wChairID,lScore);
		}
	}

	//��������
	if (pCaptureFish->FishKind>=enFishKind_8)
	{
		TCHAR *pszHitFishSound[]={TEXT("Effect\\HitFish01.mp3"),TEXT("Effect\\HitFish02.mp3"),TEXT("Effect\\HitFish03.mp3"),TEXT("Effect\\HitFish04.mp3"),TEXT("Effect\\HitFish05.mp3"),
			TEXT("Effect\\HitFish06.mp3"),TEXT("Effect\\HitFish07.mp3"),TEXT("Effect\\HitFish08.mp3"),TEXT("Effect\\HitFish09.mp3"),TEXT("Effect\\HitFish10.mp3"),TEXT("Effect\\HitFish11.mp3"),TEXT("Effect\\HitFish12.mp3")};
		m_GameClientView.m_GameClientViewDX.PlayMp3Sound(pszHitFishSound[pCaptureFish->FishKind],false);
	}

	//��������
	TCHAR *pszFishDyingSound[]={TEXT("Effect\\FishDying01.mp3"),TEXT("Effect\\FishDying02.mp3"),TEXT("Effect\\FishDying03.mp3"),TEXT("Effect\\FishDying04.mp3"),TEXT("Effect\\FishDying05.mp3"),
		TEXT("Effect\\FishDying06.mp3"),TEXT("Effect\\FishDying07.mp3"),TEXT("Effect\\FishDying08.mp3"),TEXT("Effect\\FishDying09.mp3"),TEXT("Effect\\FishDying10.mp3"),TEXT("Effect\\FishDying11.mp3")};
	m_GameClientView.m_GameClientViewDX.PlayMp3Sound(pszFishDyingSound[pCaptureFish->FishKind],false);

	return true;
}

//�ӵ���Ŀ
bool CGameClientDlg::OnSubBulletCount(const void * pData, WORD wDataSize)
{
	//�Ϸ��ж�
	ASSERT(wDataSize==sizeof(CMD_S_BulletCount));
	if (!(wDataSize==sizeof(CMD_S_BulletCount))) return false;

	//����ת��
	CMD_S_BulletCount *pBulletCount=(CMD_S_BulletCount *)pData;

	//�Ϸ��ж�
	ASSERT(pBulletCount->BulletCountKind<enBulletCountKind_End);
	if (!(pBulletCount->BulletCountKind<enBulletCountKind_End)) 
	{
		TraceString(TEXT("(!(pBulletCount->BulletCountKind<enBulletCountKind_End))"));
		return false;
	}

	//������Ŀ
	m_GameClientView.m_GameClientViewDX.SetUserBulletCount(pBulletCount->wChairID, pBulletCount->BulletCountKind, pBulletCount->lBulletCount);

	return true;
}

//�����Ŀ
bool CGameClientDlg::OnSubCoinCount(const void * pData, WORD wDataSize)
{
	//�Ϸ��ж�
	ASSERT(wDataSize==sizeof(CMD_S_CoinCount));
	if (!(wDataSize==sizeof(CMD_S_CoinCount))) return false;

	//����ת��
	CMD_S_CoinCount *pCoinCount=(CMD_S_CoinCount *)pData;

	//������Ŀ
	m_GameClientView.m_GameClientViewDX.SetUserCoinCount(pCoinCount->wChairID, pCoinCount->lCoinCount);

	return true;
}

//��Ⱥ��Ŀ
bool CGameClientDlg::OnSubCptrFishCount(const void * pData, WORD wDataSize)
{
	//�Ϸ��ж�
	ASSERT(wDataSize==sizeof(CMD_S_CptrFishCount));
	if (!(wDataSize==sizeof(CMD_S_CptrFishCount))) return false;

	//����ת��
	CMD_S_CptrFishCount *pCptrFishCount=(CMD_S_CptrFishCount *)pData;

	//�Ϸ��ж�
	ASSERT(pCptrFishCount->FishKind<enFishKind_No);
	if (!(pCptrFishCount->FishKind<enFishKind_No)) 
	{
		TraceString(TEXT("(!(pCptrFishCount->FishKind<enFishKind_No))"));
		return false;
	}

	//������Ŀ
	m_GameClientView.m_GameClientViewDX.SetCptrFishCount(pCptrFishCount->FishKind, pCptrFishCount->wFishCount);

	return true;
}

//������Ϣ
bool CGameClientDlg::OnSubScoreInfo(const void * pData, WORD wDataSize)
{
	//���ñ�־
	m_GameClientView.m_GameClientViewDX.SetWaitScoreInfo(false);

	//��ʾ�Ի���
	m_GameClientView.m_GameClientViewDX.ShowScoreView();

	return true;
}

//�ʽ���Ϣ
bool CGameClientDlg::OnSubBonusInfo(const void * pData, WORD wDataSize)
{
	//�Ϸ��ж�
	ASSERT(wDataSize==sizeof(CMD_S_BonusInfo));
	if (!(wDataSize==sizeof(CMD_S_BonusInfo))) return false;

	//����ת��
	CMD_S_BonusInfo *pBonusInfo=(CMD_S_BonusInfo *)pData;

	//������Ϣ
	m_GameClientView.m_GameClientViewDX.SetBonusInfo(pBonusInfo->lBonus);

	return true;
}

//��Ϸ��Ϣ
bool CGameClientDlg::OnSubGameMessage(const void * pData, WORD wDataSize)
{
	//Ч�����
	CMD_S_GameMessage * pMessage=(CMD_S_GameMessage *)pData;
	ASSERT(wDataSize>(sizeof(CMD_S_GameMessage)-sizeof(pMessage->szContent)));
	if (wDataSize<=(sizeof(CMD_S_GameMessage)-sizeof(pMessage->szContent))) return false;

	//��Ϣ����
	WORD wHeadSize=sizeof(CMD_S_GameMessage)-sizeof(pMessage->szContent);
	ASSERT(wDataSize==(wHeadSize+pMessage->wMessageLength*sizeof(TCHAR)));
	if (wDataSize!=(wHeadSize+pMessage->wMessageLength*sizeof(TCHAR))) return false;
	pMessage->szContent[pMessage->wMessageLength-1]=0;

	//�����Ϣ
	InsertGeneralString(TEXT("��ܰ��ʾ��"),RGB(255,0,0),false);
	InsertGeneralString(pMessage->szContent,RGB(255,0,255),true);

	return true;
}

//��Ԫ����
bool CGameClientDlg::OnSubSetCellScore(const void * pData, WORD wDataSize)
{
	//�Ϸ��ж�
	ASSERT(wDataSize==sizeof(CMD_S_SetCellScore));
	if (!(wDataSize==sizeof(CMD_S_SetCellScore))) return false;

	//����ת��
	CMD_S_SetCellScore *pSetCellScore=(CMD_S_SetCellScore *)pData;

	//������Ϣ
	if (GetMeChairID()==pSetCellScore->wChairID) m_lCellScore=pSetCellScore->lCellScore;
	m_GameClientView.m_GameClientViewDX.SetCellScore(pSetCellScore->wChairID,pSetCellScore->lCellScore);

	return true;
}

//�л�����
bool CGameClientDlg::OnSubChangeScene(const void * pData, WORD wDataSize)
{
	//�Ϸ��ж�
	ASSERT(wDataSize==sizeof(CMD_S_ChangeScene));
	if (!(wDataSize==sizeof(CMD_S_ChangeScene))) return false;

	//����ת��
	CMD_S_ChangeScene *pChangeScene=(CMD_S_ChangeScene *)pData;

	//�Ϸ��ж�
	ASSERT(pChangeScene->CureSeaSceneKind<enSeaSceneKind_No);
	if (!(pChangeScene->CureSeaSceneKind<enSeaSceneKind_No)) 
	{
		TraceString(TEXT("(!(pChangeScene->CureSeaSceneKind<enSeaSceneKind_No))"));
		return false;
	}

	//������Ŀ
	m_GameClientView.m_GameClientViewDX.SetChangeScene(pChangeScene->bLToR,pChangeScene->bChange, pChangeScene->CureSeaSceneKind);

	return true;
}

//������Ϣ
LRESULT CGameClientDlg::OnShoot(WPARAM wParam, LPARAM lParam)
{
	//�Թ��ж�
	if (true==IsLookonMode()) return 0L;

	/////////////////////////////////////////////////
#ifdef _DEBUG
	//��ȡ�ӿ�
	IClientKernel *pIClientKernel=(IClientKernel *)GetClientKernel(IID_IClientKernel,VER_IClientKernel);

	//���ñ�־
	//if (NULL!=pIClientKernel) pIClientKernel->SetExitFlags(false);
	const tagUserData *pUserData=pIClientKernel->GetMeUserInfo();

#endif
	/////////////////////////////////////////////////

	//������Ϣ
	CMD_C_UserShoot UserShoot;
	UserShoot.fAngle=m_GameClientView.m_GameClientViewDX.GetMeShootAngle();
	UserShoot.BulletCountKind=enBulletCountKind(LOWORD(lParam));
	UserShoot.byShootCount=BYTE(HIWORD(lParam));
	UserShoot.dwBulletID=DWORD(wParam);
	SendData(SUB_C_USER_SHOOT,&UserShoot,sizeof(UserShoot));

	return 0L;
}

//������Ϣ
LRESULT CGameClientDlg::OnHitFish(WPARAM wParam, LPARAM lParam)
{
	//�Ϸ��ж�
	ASSERT(0L!=wParam && 0L!=lParam);
	if (!(0L!=wParam && 0L!=lParam)) return 0L;

	//�Թ��ж�
	if (true==IsLookonMode()) return 0L;

	//��������
	CMD_C_HitFish HitFish={0};

	//������Ϣ
	HitFish.dwFishID=DWORD(wParam);
	HitFish.dwBulletID=DWORD(lParam);

	//������Ϣ
	SendData(SUB_C_HIT_FISH,&HitFish,sizeof(HitFish));

	return 0L;
}

//
////������Ϣ
//LRESULT CGameClientDlg::OnCaptureFish(WPARAM wParam, LPARAM lParam)
//{
//	//�Ϸ��ж�
//	ASSERT(0L!=wParam && 0L!=lParam);
//	if (!(0L!=wParam && 0L!=lParam)) return 0L;
//
//	//�Թ��ж�
//	if (true==IsLookonMode()) return 0L;
//
//	//���ӽ��
//	enFishKind FishKind=m_GameClientView.m_GameClientViewDX.GetFishKind(DWORD(wParam));
//	if (FishKind<enFishKind_No)
//	{
//		//��ȡ��Ⱥ
//		tagFishInfo *pFishInfo=g_FishGroup[FishKind].GetFish(DWORD(wParam));
//		ASSERT(NULL!=pFishInfo);
//		if (NULL==pFishInfo) return 0L;
//		ASSERT(pFishInfo->byBulletCount<=5);
//		if (!(pFishInfo->byBulletCount<=5)) return 0L;
//
//		//���ӽ��
//		if (pFishInfo->wTracePtIndex<pFishInfo->wTracePtCount)
//		{
//			LONG lScore=(enFishKind_12==FishKind)?(m_GameClientView.m_GameClientViewDX.GetBonusCount()):g_nFishScore[FishKind];
//			POINT ptPos=pFishInfo->ptTrace[pFishInfo->wTracePtIndex];
//			m_GameClientView.m_GameClientViewDX.UserAddCoin(GetMeChairID(), ptPos.x, ptPos.y, lScore);
//			if (enFishKind_12==FishKind) m_GameClientView.m_GameClientViewDX.AddBonus(GetMeChairID(),lScore);
//		}
//
//		//��������
//		CMD_C_CaptureFish CaptureFish={0};
//
//		//������Ϣ
//		CaptureFish.dwFishID=DWORD(wParam);
//		CaptureFish.dwBulletID=DWORD(lParam);
//		CaptureFish.byCptrProbability=m_byCptrProbability[pFishInfo->byBulletCount-1][FishKind];
//		CaptureFish.byBulletCount=pFishInfo->byBulletCount;
//
//		//������Ϣ
//		SendData(SUB_C_CAPTURE_FISH,&CaptureFish,sizeof(CaptureFish));
//
//		//��������
//		if (FishKind>=enFishKind_8)
//		{
//			TCHAR *pszHitFishSound[]={TEXT("Effect\\HitFish01.mp3"),TEXT("Effect\\HitFish02.mp3"),TEXT("Effect\\HitFish03.mp3"),TEXT("Effect\\HitFish04.mp3"),TEXT("Effect\\HitFish05.mp3"),
//				TEXT("Effect\\HitFish06.mp3"),TEXT("Effect\\HitFish07.mp3"),TEXT("Effect\\HitFish08.mp3"),TEXT("Effect\\HitFish09.mp3"),TEXT("Effect\\HitFish10.mp3"),TEXT("Effect\\HitFish11.mp3"),TEXT("Effect\\HitFish12.mp3")};
//			m_GameClientView.m_GameClientViewDX.PlayMp3Sound(pszHitFishSound[FishKind],false);
//		}
//
//		//��������
//		TCHAR *pszFishDyingSound[]={TEXT("Effect\\FishDying01.mp3"),TEXT("Effect\\FishDying02.mp3"),TEXT("Effect\\FishDying03.mp3"),TEXT("Effect\\FishDying04.mp3"),TEXT("Effect\\FishDying05.mp3"),
//				TEXT("Effect\\FishDying06.mp3"),TEXT("Effect\\FishDying07.mp3"),TEXT("Effect\\FishDying08.mp3"),TEXT("Effect\\FishDying09.mp3"),TEXT("Effect\\FishDying10.mp3"),TEXT("Effect\\FishDying11.mp3")};
//			m_GameClientView.m_GameClientViewDX.PlayMp3Sound(pszFishDyingSound[FishKind],false);
//	}
//
//	//TCHAR szMessage[64];
//	//_sntprintf(szMessage,CountArray(szMessage),TEXT("%ld"),CaptureFish.dwFishID);
//	//InsertShtickString(szMessage,RGB(255,0,0),true);
//
//	return 0L;
//}

////�����ӵ�
//LRESULT CGameClientDlg::OnShowBulletShop(WPARAM wParam, LPARAM lParam)
//{
//	//�����ӵ�
//	if (false==IsLookonMode())
//	{
//		//�����ӵ�
//		ShowBulletShop();		
//	}
//
//	return 0L;
//}

//��������
LRESULT CGameClientDlg::OnEnableSound(WPARAM wParam, LPARAM lParam)
{
	//��ֹ�ж�
	if (true==IsEnableSound())
	{
		//��������
		TCHAR *pszBKSound[]={TEXT("BGSound0.mp3"),TEXT("BGSound1.mp3"),TEXT("BGSound2.mp3")};
		int nIndex=rand()%CountArray(pszBKSound);
		m_GameClientView.m_GameClientViewDX.PlayMp3SoundBG(pszBKSound[nIndex]);
	}
	else
	{
		m_GameClientView.m_GameClientViewDX.StopMp3SoundBG();
	}

	return 0L;
}

//�˳���Ϸ
LRESULT CGameClientDlg::OnQuitGame(WPARAM wParam, LPARAM lParam)
{
	//��ȡ�ӿ�
	IClientKernel *pIClientKernel=(IClientKernel *)GetClientKernel(IID_IClientKernel,VER_IClientKernel);

	//���ñ�־
	//if (NULL!=pIClientKernel) pIClientKernel->SetExitFlags(false);//Author<cxf>;DateTime<2012-03-27>;Description<����>

	//���ø���
	__super::OnCancel();

	return 0L;
}

//�û�����
void __cdecl CGameClientDlg::OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	//�ӵ���Ŀ
	if (false==bLookonUser)
	{
		m_GameClientView.m_GameClientViewDX.SetShootBulletCount(wChairID, 5);
	}

	return ;
}

//�û��뿪
void __cdecl CGameClientDlg::OnEventUserLeave(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	//�ӵ���Ŀ
	if (false==bLookonUser)
	{
		m_GameClientView.m_GameClientViewDX.SetShootBulletCount(wChairID, 0);
		m_GameClientView.m_GameClientViewDX.SetUserCoinCount(wChairID, 0);

		//�ӵ���Ŀ
		m_GameClientView.m_GameClientViewDX.SetUserBulletCount(wChairID, enBulletCountKind_100, 0);
		m_GameClientView.m_GameClientViewDX.SetUserBulletCount(wChairID, enBulletCountKind_600, 0);
		m_GameClientView.m_GameClientViewDX.SetUserBulletCount(wChairID, enBulletCountKind_800, 0);
	}

	return ;
}

//�����ӵ�
void CGameClientDlg::ShowBulletShop()
{
	////�Թ��ж�
	//if (true==IsLookonMode()) return ;

	////�ظ��ж�
	//if (NULL!=FindWindow(NULL,TEXT("�����ӵ�"))) return ;

	//CDlgBulletShop DlgBulletShop(&m_GameClientView.m_GameClientViewDX,m_BulletCountKindSelected, m_lCellScore);
	//DlgBulletShop.SetBulletCharge(m_lBulletCharge);
	//if (IDOK==DlgBulletShop.DoModal())
	//{
	//	//��������
	//	CMD_C_BuyBullet BuyBullet;

	//	//��ȡ����
	//	BuyBullet.BulletCountKind=DlgBulletShop.GetBulletKind();

	//	//�Ϸ��ж�
	//	tagUserData const *pUserData=GetUserData(GetMeChairID());
	//	if (NULL==pUserData) return;
	//	if (pUserData->lScore<m_lBulletCharge[BuyBullet.BulletCountKind]*m_lCellScore)
	//	{
	//		//�����ļ���
	//		TCHAR szPath[MAX_PATH]=TEXT(""),szIniFileName[MAX_PATH]=TEXT("");
	//		GetCurrentDirectory(CountArray(szPath),szPath);	
	//		_sntprintf(szIniFileName,CountArray(szIniFileName),TEXT("%s\\fish\\Config.ini"),szPath);

	//		TCHAR szBuffer[1024];
	//		GetPrivateProfileString(TEXT("Config"),TEXT("URL"),TEXT("www.baidu.com"),szBuffer,CountArray(szBuffer),szIniFileName);
	//		if (IDYES==MessageBox(TEXT("���Ľ�Ҳ��㣬�Ƿ����ڹ���"),TEXT("��ܰ��ʾ"),MB_ICONQUESTION|MB_YESNO))
	//			ShellExecute(NULL,TEXT("open"),szBuffer,NULL,NULL,SW_SHOWDEFAULT);
	//	}
	//	else
	//	{
	//		//������Ϣ
	//		SendData(SUB_C_BUY_BULLET,&BuyBullet,sizeof(BuyBullet));

	//		//������Ŀ
	//		LONG lBulletCount=m_GameClientView.m_GameClientViewDX.GetBulletCount(GetMeChairID(),BuyBullet.BulletCountKind);
	//		if (enBulletCountKind_100==BuyBullet.BulletCountKind) lBulletCount+=100;
	//		else if (enBulletCountKind_600==BuyBullet.BulletCountKind) lBulletCount+=600;
	//		else if (enBulletCountKind_800==BuyBullet.BulletCountKind) lBulletCount+=800;
	//		m_GameClientView.m_GameClientViewDX.SetUserBulletCount(GetMeChairID(), BuyBullet.BulletCountKind, lBulletCount);

	//		//���ñ���
	//		m_BulletCountKindSelected=BuyBullet.BulletCountKind;
	//	}
	//}
	//�ظ��ж�
	if (NULL!=FindWindow(NULL,TEXT("��ֵ"))) return ;
	//�Ϸ��ж�
	tagUserData const *pUserData=GetUserData(GetMeChairID());
	if (NULL==pUserData) return;

	//�����ļ���
	TCHAR szPath[MAX_PATH]=TEXT(""),szIniFileName[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);	
	_sntprintf(szIniFileName,CountArray(szIniFileName),TEXT("%s\\Config.ini"),szPath);

	TCHAR szBuffer[1024];
	GetPrivateProfileString(TEXT("QianCheng"),TEXT("Recharge"),TEXT("http://game.baihuogu.com"),szBuffer,CountArray(szBuffer),szIniFileName);
	if (IDYES==MessageBox(TEXT("���Ľ�Ҳ��㣬�Ƿ����ڹ���"),TEXT("��ܰ��ʾ"),MB_ICONQUESTION|MB_YESNO))
	{
		ShellExecute(NULL,TEXT("open"),szBuffer,NULL,NULL,SW_SHOWDEFAULT);
		PostMessage(WM_QUIT_GAME,0,0);
	}
}

//�رպ���
void CGameClientDlg::OnCancel()
{
	PostMessage(WM_QUIT_GAME,0,0);
}

//���������Ϸ
void CGameClientDlg::OnEventContinueGame()
{
}

//BOOL CGameClientDlg::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
//{
//	//////WM_NCHITTEST��Ϣ
//	////if(message==WM_NCHITTEST)
//	////{
//	////	int a=0;
//	////	//OnNcHitTest(wParam,lParam);
//	////	//return TRUE;
//	////}
//	//�����ӵ�(����)
//	if(message==WM_SHOW_COIN_SHOP)
//	{
//		ShowBulletShop();
//		return TRUE;
//	}
//	////������Ϣ
//	//if(message==WM_SHOOT)
//	//{
//	//	OnShoot(wParam,lParam);
//	//	return TRUE;
//	//}
//	////�˳���Ϸ,
//	//if(message==WM_QUIT_GAME)
//	//{
//	//	OnQuitGame(wParam,lParam);
//	//	return TRUE;
//	//}
//	////������Ⱥ
//	//if(message==WM_HIT_FISH)
//	//{
//	//	OnHitFish(wParam,lParam);
//	//	return TRUE;
//	//}
//	////�������(ʹ��������)
//	//if(message==WM_HIT_Laser)
//	//{
//	//	OnHitLaser(wParam,lParam);
//	//	return TRUE;
//	//}
//	return FALSE;
//	//return __super::OnWndMsg(message,wParam,lParam,pResult);
//}
//////////////////////////////////////////////////////////////////////////
