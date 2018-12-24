#include "StdAfx.h"
#include "Resource.h"
#include "GlobalOption.h"
#include "OnlineKeepTime.h"
#include "GameFrameView.h"
#include ".\gameframeview.h"
#include "GameFrameDlg.h"
#include "TalkDlg.h"

//////////////////////////////////////////////////////////////////////////

//��̬����
const int			CGameFrameView::m_nXFace=300;						//ͷ��߶�
const int			CGameFrameView::m_nYFace=200;						//ͷ����

const int			CGameFrameView::m_nXTimer=110;						//��ʱ����
const int			CGameFrameView::m_nYTimer=110;						//��ʱ����

const int			CGameFrameView::m_nXBorder=0;						//��ʱ����
const int			CGameFrameView::m_nYBorder=0;						//��ʱ����

const int			CGameFrameView::m_nXTimerNumber=20;					//��ʱ�����ֿ�
const int			CGameFrameView::m_nYTimerNumber=23;					//��ʱ�����ָ�

//ʱ�䶨��
#define IDI_FLOWER_START			3000								//�ʻ���ʱ��
#define IDI_TIMER_FLASH				3091								//��ʱ������
#define IDI_GAME_WAIT_FLASH			3100								//��Ϸ�ȴ�����

//ʱ����
#define TIMER_FLASH_DEF				100
#define TIMER_WAIT_FLASH_INTERVAL	200


//�û��������
#define				GAME_CLIENT_USER_IMAGE_NUM_DEF			5
#define				GAME_IMAGE_WIDTH						225
#define				GAME_IMAGE_HEIGHT						290

//////////////////////////////////////////////////////////////////////////
//�߿���Դ
//static CSkinImage						m_ImageFrameLB;						//�߿�ͼƬ
//static CSkinImage						m_ImageFrameLM;						//�߿�ͼƬ
//static CSkinImage						m_ImageFrameLT;						//�߿�ͼƬ
//static CSkinImage						m_ImageFrameBL;						//�߿�ͼƬ
//static CSkinImage						m_ImageFrameBM;						//�߿�ͼƬ
//static CSkinImage						m_ImageFrameBR;						//�߿�ͼƬ
//static CSkinImage						m_ImageFrameTM;						//�߿�ͼƬ
//static CSkinImage						m_ImageFrameMM;						//�߿�ͼƬ

static CPngImage						m_ImageUserFace[GAME_CLIENT_USER_IMAGE_NUM_DEF][2]; //�û�����, 0:��,1:Ů
static CPngImage						m_ImageReady;						//׼��

//��ʱ��������֡��
const int	ITIMER_FLASH_TOTAL_FRAMES	=	50;
//�����������ʼ��
const int	ITIMER_FRAMES_START			=	5;
//����λ����λ
const int	ITIMER_FLASH_PT				=	1;
//����ƫ��
const int	ITIMER_NUMS_X				=	-4;
const int	ITIMER_NUMS_Y				=	3;



//�ȴ�����
const int	I_GAME_WAIT_TOTAL_FRAMES	=	5;
const int	I_GAME_WAIT_FLASH_WIDTH		=	333;
const int	I_GAME_WAIT_FLASH_HEIGHT	=	136;

///////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CGameFrameView, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameFrameView::CGameFrameView(bool bDoubleBuf ,UINT uColorCount, CThreadDraw * pThreadDraw) 
	: m_bDoubleBuf(bDoubleBuf), m_uColorCount(uColorCount), m_pThreadDraw(pThreadDraw)
{
	//���ñ���
	m_bReDraw=true;
	m_pReserve=NULL;
	m_nAnimeStep=30;
	m_pIUserFaceRes=NULL;
	memset(&m_wTimer,0,sizeof(m_wTimer));
	memset(&m_ptName,0,sizeof(m_ptName));
	memset(&m_ptFace,0,sizeof(m_ptFace));
	memset(&m_ptTimer,0,sizeof(m_ptTimer));
	memset(&m_ptReady,0,sizeof(m_ptReady));
	memset(&m_pUserItem,0,sizeof(m_pUserItem));
	memset(&m_ptSayPos, 0, sizeof(m_ptSayPos));
	m_ptOnlineTime.x = 0;
	m_ptOnlineTime.y = 0;

#if REMOVE_FLOWER	// del by HouGuoJiang 2011-11-25
	ZeroMemory(m_FlowerInfo, sizeof(m_FlowerInfo));
	m_nFlowerCount=0;
#endif

	memset(m_bIsDrawTimer, 0, sizeof(m_bIsDrawTimer));
	memset(m_iCurTimeFrames, 0, sizeof(m_iCurTimeFrames));

	//Ĭ�ϼ���
	m_bIsDisableTimerFlash = true;
	m_wGameUserNums = 0;
	m_bIsStartTimer = false;

	
	////������Դ
	//HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
	//m_ImageFrameLB.SetLoadInfo(IDB_FRAME_LB,hInstance,false);
	//m_ImageFrameLM.SetLoadInfo(IDB_FRAME_LM,hInstance,false);
	//m_ImageFrameLT.SetLoadInfo(IDB_FRAME_LT,hInstance,false);
	//m_ImageFrameBL.SetLoadInfo(IDB_FRAME_BL,hInstance,false);
	//m_ImageFrameBM.SetLoadInfo(IDB_FRAME_BM,hInstance,false);
	//m_ImageFrameBR.SetLoadInfo(IDB_FRAME_BR,hInstance,false);
	//m_ImageFrameTM.SetLoadInfo(IDB_FRAME_TM,hInstance,false);
	//m_ImageFrameMM.SetLoadInfo(IDB_FRAME_MM,hInstance,false);

	//for (int i=0; i<GAME_CLIENT_USER_IMAGE_NUM_DEF; i++)
	//{
	//	m_ImageUserFace[i].SetLoadInfo(IDB_BT_USERIMAGE_0+i,hInstance,false);
	//}

	m_nKindIDWinCount=0;
	m_nKindIDLostCount=0;
	m_nKindIDDrawCount=0;
	m_nKindIDFleeCount=0;

	for (int i=0; i<10; i++)
	{
		m_pTalkDlg[i] = NULL;
	}
	
	m_pOnlineKeepTime = NULL;
	return;
}

//��������
CGameFrameView::~CGameFrameView()
{
	m_ImageReady.DestroyImage();

#if REMOVE_FLOWER	// del by HouGuoJiang 2011-11-25
	//�ͷſؼ�
	for(WORD i=0;i<CountArray(m_FlowerEffectControl);i++)
		if ( m_FlowerEffectControl[i].m_hWnd != NULL )
			m_FlowerEffectControl[i].DestroyWindow();
#endif

	for (int i=0; i<10; i++)
	{
		if (NULL != m_pTalkDlg[i])
		{
			delete m_pTalkDlg[i];
			m_pTalkDlg[i] = NULL;
		}
	}

	if (NULL != m_pOnlineKeepTime)
	{
		SafeDelete(m_pOnlineKeepTime);
	}
}

//�ӿڲ�ѯ
void * __cdecl CGameFrameView::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameFrameView,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameFrameView,Guid,dwQueryVer);
	return NULL;
}

//���½���
void CGameFrameView::UpdateGameView(const CRect * pRect)
{
	if (m_bDoubleBuf==true) 
		m_bReDraw=true;

	if(this->GetSafeHwnd())	// add by HouGuoJiang 2011-11-25
	{
		if (pRect!=NULL) 
			InvalidateRect(pRect,FALSE);
		else 
			Invalidate(FALSE);
	}

	return;
}

//��ȡ����
bool CGameFrameView::GetGameViewImage(CImage & ImageBuffer, int nXPos, int nYPos, int nWidth, int nHeight)
{
	ASSERT(ImageBuffer.IsNull()==false);
	ASSERT(m_ImageBuffer.IsNull()==false);
	if (m_ImageBuffer.IsNull()==false)
	{
		m_ImageBuffer.BitBlt(ImageBuffer.GetDC(),nXPos,nYPos,nWidth,nHeight,nXPos,nYPos);
		ImageBuffer.ReleaseDC();
		return true;
	}
	return false;
}

//����λ��
void CGameFrameView::SetFlowerControlInfo(WORD wChairID,int nXPos, int nYPos, int nWidth, int nHeight, UINT nAnimeStep)
{
#if REMOVE_FLOWER	// del by HouGuoJiang 2011-11-25
	//�Ϸ��ж�
	ASSERT(wChairID<CountArray(m_FlowerEffectControl));
	if (wChairID>=CountArray(m_FlowerEffectControl)) return;
#endif

	//���ñ���
	m_nAnimeStep = nAnimeStep;

#if REMOVE_FLOWER	// del by HouGuoJiang 2011-11-25
	//�ƶ�����
	if ( m_FlowerEffectControl[wChairID].m_hWnd != NULL )
	{
		//����ת��
		CRect rcScreen(nXPos, nYPos, nXPos+nWidth, nYPos+nHeight);
		ClientToScreen(&rcScreen);

		//�ƶ�����
		m_FlowerEffectControl[wChairID].MoveWindow(rcScreen);
	}
#endif

	return ;
}

//�滭�ʻ�
void CGameFrameView::ShowFlower(WORD wSourceChairID, WORD wTargetChairID, UINT uFlowerID, UINT uFlowerEffectID, bool bBombEffect)
{
#if REMOVE_FLOWER	// del by HouGuoJiang 2011-11-25
	//�Ϸ��ж�
	ASSERT(wTargetChairID<CountArray(m_FlowerEffectControl) && wSourceChairID<CountArray(m_FlowerEffectControl));
	if (wTargetChairID>=CountArray(m_FlowerEffectControl) || wSourceChairID>=CountArray(m_FlowerEffectControl)) return;
	if(m_FlowerEffectControl[wTargetChairID].m_hWnd == NULL || m_FlowerEffectControl[wSourceChairID].m_hWnd == NULL) return ;
	if (m_nFlowerCount>0) return;

	for ( int n = 0; n < MAX_FLOWER_COUNT; ++n )
	{
		tagFlowerEffectInfo & FlowerInfo = m_FlowerInfo[n];

		if ( FlowerInfo.wMoveCount == 0 ) 
		{
			//��ȡλ��
			CRect rcVideo,rcTargetVideo;
			m_FlowerEffectControl[wSourceChairID].GetClientRect(&rcVideo);
			m_FlowerEffectControl[wTargetChairID].GetClientRect(&rcTargetVideo);
			m_FlowerEffectControl[wSourceChairID].MapWindowPoints(this, &rcVideo);
			m_FlowerEffectControl[wTargetChairID].MapWindowPoints(this, &rcTargetVideo);

			//λ����Ϣ
			FlowerInfo.ptFrom.x = rcVideo.left + rcVideo.Width()/2;
			FlowerInfo.ptFrom.y = rcVideo.top  + rcVideo.Height()/2;
			FlowerInfo.ptDest.x = rcTargetVideo.left + rcTargetVideo.Width()/2;
			FlowerInfo.ptDest.y = rcTargetVideo.top  + rcTargetVideo.Height()/2;

			//�ж�λ��
			if(FlowerInfo.ptFrom.x==FlowerInfo.ptDest.x && FlowerInfo.ptFrom.y==FlowerInfo.ptDest.y)
			{
				//��ʾ����
				m_FlowerEffectControl[wTargetChairID].StartFlowerEffect(uFlowerEffectID);
				m_FlowerEffectControl[wTargetChairID].BringWindowToTop();

				//�������
				ZeroMemory(&FlowerInfo, sizeof(FlowerInfo));
			}
			else
			{
				//��ʼλ��
				FlowerInfo.ptCurPos = FlowerInfo.ptFrom ;

				//��������
				INT nXCount=abs(FlowerInfo.ptDest.x-FlowerInfo.ptFrom.x)/m_nAnimeStep;
				INT nYCount=abs(FlowerInfo.ptDest.y-FlowerInfo.ptFrom.y)/m_nAnimeStep;
				FlowerInfo.wMoveCount=__max(1,__max(nXCount,nYCount));
				FlowerInfo.wMoveIndex = 0;

				//�û���Ϣ
				FlowerInfo.wSourceChairID = wSourceChairID;
				FlowerInfo.wTargetChairID = wTargetChairID;
				FlowerInfo.uFlowerID = uFlowerID;
				FlowerInfo.uFlowerEffectID = uFlowerEffectID;
				FlowerInfo.bBombEffect = bBombEffect;

				//��������
				m_nFlowerCount++;

				//�״��ж�
				if ( m_nFlowerCount == 1 ) SetTimer(IDI_FLOWER_START, 100, NULL);
			}
			break;
		}
	}
#endif

	return ;
}

void CGameFrameView::InitHeadPortrait(CWnd *pParent, UINT uCount)
{
	//m_HeadPortrait.Init(pParent, uCount);
}

void CGameFrameView::SetHeadPortraitPos(CPoint pt[])
{
	//m_HeadPortrait.SetPos(pt);
}

void CGameFrameView::PlayHeadPortraitAction(WORD wID, UINT uIndex, UINT uFrameTime/*=150*/, UINT uIntervalTime/*=0*/, UINT uResultTime/*=0*/, UINT uDefaultAction/*=0*/)
{
	const tagUserData * pUserData = GetUserInfo(wID);
	if (NULL != pUserData)
	{
		//m_HeadPortrait.PlayAction(wID, (1==pUserData->cbGender)?TRUE:FALSE, uIndex, uFrameTime, uIntervalTime, uResultTime, uDefaultAction);
	}
}

void CGameFrameView::HideHeadPortrait(UINT uID, BOOL bAllHide/*=FALSE*/)
{
	//m_HeadPortrait.Hide(uID, bAllHide);
}

void CGameFrameView::OnlineKeepTime(int iTime, BOOL bStart/*=TRUE*/)
{
	if (FALSE == bStart)
	{
		if (NULL!=m_pOnlineKeepTime && NULL!=m_pOnlineKeepTime->GetSafeHwnd() && 
			TRUE==m_pOnlineKeepTime->IsWindowVisible())
		{
			static_cast<COnlineKeepTime *>(m_pOnlineKeepTime)->PlayStatus(FALSE);
		}

		TRACE("CGameFrameView::OnlineKeepTime ��ͣ���������ʱ ��ǰʱ��%d��", iTime);
		return;
	}

	TRACE("CGameFrameView::OnlineKeepTime ��ʾ���������ʱ %d��", iTime);
	if (NULL == m_pOnlineKeepTime)
	{
		m_pOnlineKeepTime = new COnlineKeepTime;
		if (NULL == m_pOnlineKeepTime)
		{
			ASSERT(0);
			return;
		}

		m_pOnlineKeepTime->Create(NULL, NULL, WS_VISIBLE|WS_CHILD, CRect(0,0,0,0), this, 2000);
	}

	//RectifyKeepTimePos(730,0);//Author<cxf>
	static_cast<COnlineKeepTime *>(m_pOnlineKeepTime)->SetWindowPos(NULL, 
		m_ptOnlineTime.x, m_ptOnlineTime.y, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	static_cast<COnlineKeepTime *>(m_pOnlineKeepTime)->ShowWindow(SW_SHOW);
	static_cast<COnlineKeepTime *>(m_pOnlineKeepTime)->SetKeepTime(iTime);
}

void CGameFrameView::RectifyKeepTimePos(int x, int y)
{
	m_ptOnlineTime.x = x;
	m_ptOnlineTime.y = y;
}

void CGameFrameView::DrawFrameView(CDC * pDC, int nWidth, int nHeight)
{
	if (NULL != g_GameFrameDlg  &&  TRUE==g_GameFrameDlg->IsWindowVisible())
	{
		g_GameFrameDlg->DrawFrameView(pDC, nWidth, nHeight);
	}
}

//��ȡʱ��
WORD CGameFrameView::GetUserTimer(WORD wChairID)
{
	ASSERT(wChairID<MAX_CHAIR);
	if (wChairID>=MAX_CHAIR) return 0;
	return m_wTimer[wChairID];
}

//����ʱ��
bool CGameFrameView::SetUserTimer(WORD wChairID, WORD wTimer)
{
	ASSERT(wChairID<MAX_CHAIR);
	if (wChairID>=MAX_CHAIR) return false;
	m_wTimer[wChairID]=wTimer;
	m_bIsDrawTimer[wChairID] = wTimer?true:false;

	if (!m_bIsStartTimer && wTimer<=ITIMER_FRAMES_START)
	{
		m_iCurTimeFrames[wChairID] = 1;
		KillTimer(IDI_TIMER_FLASH);
		SetTimer(IDI_TIMER_FLASH, TIMER_FLASH_DEF, NULL);
	}
	else if (0==wTimer)
	{
		m_bIsStartTimer = false;
		KillTimer(IDI_TIMER_FLASH);
	}

	m_ptTimerCurPT[wChairID] = m_ptTimer[wChairID];
	CRect rect;
	rect.left = m_ptTimerCurPT[wChairID].x-m_nXTimer/2;
	rect.top = m_ptTimerCurPT[wChairID].y-m_nYTimer/2;
	rect.right = rect.left + m_nXTimer;
	rect.bottom = rect.top + m_nYTimer;
	UpdateGameView(&rect);
	return true;
}


//��ȡ���
const tagUserData * CGameFrameView::GetUserInfo(WORD wChairID)
{
	ASSERT(wChairID<MAX_CHAIR);
	if (wChairID>=MAX_CHAIR) return NULL;
	return m_pUserItem[wChairID];
}

//�滭����
void CGameFrameView::DrawViewImage(CDC * pDC, CSkinImage & SkinImage, enImageMode ImageMode)
{
	//��ȡλ��
	RECT rcClient;
	GetClientRect(&rcClient);

	//�滭λͼ
	switch (ImageMode)
	{
	case enMode_ElongGate:	//����ģʽ
		{
			CImageHandle ImageHandle(&SkinImage);
			int nImageWidth=ImageHandle->GetWidth();
			int nImageHeight=ImageHandle->GetHeight();
			ImageHandle->StretchBlt(pDC->m_hDC,0,0,rcClient.right,rcClient.bottom,0,0,nImageWidth,nImageHeight);
			break;
		}
	case enMode_Centent:	//����ģʽ
		{
			CImageHandle ImageHandle(&SkinImage);
			int nXPos=(rcClient.right-ImageHandle->GetWidth())/2;
			int nYPos=(rcClient.bottom-ImageHandle->GetHeight())/2;
			ImageHandle->BitBlt(pDC->m_hDC,nXPos,nYPos);
			break;
		}
	case enMode_Spread:		//ƽ��ģʽ
		{
			CImageHandle ImageHandle(&SkinImage);
			int nImageWidth=ImageHandle->GetWidth();
			int nImageHeight=ImageHandle->GetHeight();
			for (int nXPos=0;nXPos<rcClient.right;nXPos+=nImageWidth)
			{
				for (int nYPos=0;nYPos<rcClient.bottom;nYPos+=nImageHeight)
				{
					ImageHandle->BitBlt(pDC->m_hDC,nXPos,nYPos);
				}
			}
			break;
		}
	}
	
	return;
}

//�滭ͷ��
void CGameFrameView::DrawUserFace(CDC * pDC,const tagUserData * pUserData, int nXPos, int nYPos, bool bOffLine)
{
	//if (NULL == pDC || NULL==pUserData) return;
	//if (pUserData->wFaceID>=GAME_CLIENT_USER_IMAGE_NUM_DEF) return;
	//ASSERT(m_pIUserFaceRes!=NULL && pUserData!=NULL);
	//m_pIUserFaceRes->DrawNormalFace(pDC,nXPos,nYPos,pUserData->wFaceID,pUserData->dwUserID,pUserData->dwCustomFaceVer);

	//if (pUserData->wFaceID>=0 && pUserData->wFaceID<GAME_CLIENT_USER_IMAGE_NUM_DEF)
	//{
	//	CImageHandle ImageHandle(&m_ImageUserFace[pUserData->wFaceID]);
	//	if (!m_ImageUserFace[pUserData->wFaceID].IsNull())
	//	{
	//		m_ImageUserFace[pUserData->wFaceID].AlphaDrawImage(pDC, nXPos, nYPos, RGB(255, 0, 255));
	//	}
	//}
	
	return;
}

void CGameFrameView::GetUserImageInfo(BYTE byViewID, int &iPosX, int &iPosY)
{
	iPosX = 0;
	iPosY = 0;
	if (3==m_wGameUserNums)
	{//������
		if (0==byViewID)
		{
			iPosX = 0;
		}
		else if (2==byViewID)
		{
			iPosX = GAME_IMAGE_WIDTH;
		}
		else
		{
			iPosX = 2*GAME_IMAGE_WIDTH;
		}
	}
	else if (4==m_wGameUserNums)
	{//�ĸ���
		if (1==byViewID)
		{
			iPosX = GAME_IMAGE_WIDTH;
		}
		else if (3==byViewID)
		{
			iPosX = 0;
		}
		else
		{
			iPosX = 2*GAME_IMAGE_WIDTH;
		}
	}
	else
	{//������
		if (0==byViewID)
		{
			iPosX = 0;
		}
		else if (1==byViewID)
		{
			iPosX = GAME_IMAGE_WIDTH;
		}
	}
}

//�滭ͷ��
void CGameFrameView::DrawCustomFace(CDC * pDC, BYTE byViewID, const tagUserData * pUserData, int nXPos, int nYPos)
{
	if (NULL == pDC || NULL==pUserData) return;
	int iFaceID = pUserData->wFaceID;
	if (iFaceID>=0 && iFaceID<GAME_CLIENT_USER_IMAGE_NUM_DEF)
	{
		int iPosX=0, iPosY=0;
		GetUserImageInfo(byViewID, iPosX, iPosY);
		CPngImage* pUserImage=NULL;
		if (0 == pUserData->cbGender)
		{//��
			pUserImage = &(m_ImageUserFace[iFaceID][0]);
		}
		else
		{//Ů
			pUserImage = &(m_ImageUserFace[iFaceID][1]);
		}
		if (NULL!=pUserImage)
		{
			pUserImage->DrawImage(pDC, nXPos, nYPos, GAME_IMAGE_WIDTH, GAME_IMAGE_HEIGHT, iPosX, iPosY, GAME_IMAGE_WIDTH, GAME_IMAGE_HEIGHT);
		}
	}
}

//�滭׼��
void CGameFrameView::DrawUserReady(CDC * pDC, int nXPos, int nYPos)
{
	if (!m_ImageReady.IsNull())
	{
		//�滭׼��
		CSize SizeImage(m_ImageReady.GetWidth(),m_ImageReady.GetHeight());
		m_ImageReady.DrawImage(pDC,nXPos-SizeImage.cx/2,nYPos-SizeImage.cy/2);
	}
	return;
}

//���ƶ��߱�־
void CGameFrameView::DrawBreakLineTag(CDC * pDC, int nXPos, int nYPos)
{
	if (!m_ImageBreakLine.IsNull())
	{
		m_ImageBreakLine.DrawImage(pDC, nXPos, nYPos);
	}
}

//�滭ʱ��
void CGameFrameView::DrawUserTimer(CDC * pDC, BYTE byViewID)
{
	if (byViewID>=MAX_CHAIR || 0==m_wTimer[byViewID] || m_wTimer[byViewID]>100)
	{
		return;
	}
	WORD wTime = m_wTimer[byViewID];
	//�����������
	POINT ptBack, ptNumber;
	//����������ʾ
	m_ptTimerCurPT[byViewID] = m_ptTimer[byViewID];
	ptBack.x = m_ptTimerCurPT[byViewID].x-m_nXTimer/2;
	ptBack.y = m_ptTimerCurPT[byViewID].y-m_nYTimer/2;

	//���Ʊ���λ��
	if (m_bIsDisableTimerFlash && wTime<=ITIMER_FRAMES_START)
	{//�����ˣ�����ʱ���Ѿ����ڵ���ITIMER_FRAMES_START,������ʼ����.
		m_ImageTimerBack.DrawImage(pDC,ptBack.x,ptBack.y, m_nXTimer, m_nYTimer, m_iCurTimeFrames[byViewID]*m_nXTimer, 0, m_nXTimer, m_nYTimer);
	}
	else if (wTime>ITIMER_FRAMES_START)
	{//��������
		//������Ŀ
		WORD lNumberCount=1;
		if (wTime>=10)
		{//��λ
			lNumberCount = 2;
		}

		//����λ��
		ptNumber.x = m_ptTimerCurPT[byViewID].x-(lNumberCount*m_nXTimerNumber)/2-14;
		ptNumber.y = m_ptTimerCurPT[byViewID].y-m_nYTimerNumber/2-4;

		//�滭����
		m_ImageTimerBack.DrawImage(pDC,ptBack.x,ptBack.y, m_nXTimer, m_nYTimer, 0, 0, m_nXTimer, m_nYTimer);
		//��������
		if (lNumberCount>1)
		{//���ƶ�λ
			WORD wCellNumber=wTime/10;
			//����ʮλ
			m_ImageTimerNumber.DrawImage(pDC,ptNumber.x+2,ptNumber.y,m_nXTimerNumber,m_nYTimerNumber,wCellNumber*m_nXTimerNumber,0,m_nXTimerNumber,m_nYTimerNumber);
			//���Ƹ�λ
			wCellNumber=wTime%10;
			m_ImageTimerNumber.DrawImage(pDC,ptNumber.x+m_nXTimerNumber-2,ptNumber.y,m_nXTimerNumber,m_nYTimerNumber,wCellNumber*m_nXTimerNumber,0,m_nXTimerNumber,m_nYTimerNumber);
		}
		else
		{//����һλ
			m_ImageTimerNumber.DrawImage(pDC,ptNumber.x,ptNumber.y,m_nXTimerNumber,m_nYTimerNumber,wTime*m_nXTimerNumber,0,m_nXTimerNumber,m_nYTimerNumber);
		}
	}
}

//͸���滭
void CGameFrameView::AlphaDrawImage(CDC * pDesDC, int nXDes, int nYDes, int nDesWidth, int nDesHeight, CDC * pScrDC, int nXScr, int nYScr, COLORREF crTransColor)
{
	::AlphaDrawImage(pDesDC,nXDes,nYDes,nDesWidth,nDesHeight,pScrDC,nXScr,nYScr,nDesWidth,nDesHeight,crTransColor);
	return;
}

//�滭͸��ͼ
void CGameFrameView::AlphaDrawImage(CDC * pDesDC, int nXDes, int nYDes, int nDesWidth, int nDesHeight, HBITMAP hBitBmp, int nXScr, int nYScr, COLORREF crTransColor)
{
	//���� DC
	CDC DCBuffer;
	DCBuffer.CreateCompatibleDC(NULL);
	HGDIOBJ hOldGdiObj=DCBuffer.SelectObject(hBitBmp);

	//�滭λͼ
	::AlphaDrawImage(pDesDC,nXDes,nYDes,nDesWidth,nDesHeight,&DCBuffer,nXScr,nYScr,nDesWidth,nDesHeight,crTransColor);

	//������Դ
	DCBuffer.SelectObject(hOldGdiObj);
	DeleteDC(DCBuffer.GetSafeHdc());
	DCBuffer.m_hDC = NULL;

	return;
}

//���ý���
void CGameFrameView::ResetData()
{
	//���ñ���
	memset(m_wTimer,0,sizeof(m_wTimer));
	memset(m_pUserItem,0,sizeof(m_pUserItem));

	//���ý���
	ResetGameView();
	UpdateGameView(NULL);

	return;
}

//���ýӿ�
bool CGameFrameView::SetUserFaceRes(IUnknownEx * pIUnknownEx)
{
	ASSERT(pIUnknownEx!=NULL);
	m_pIUserFaceRes=(IUserFaceRes *)pIUnknownEx->QueryInterface(IID_IUserFaceRes,VER_IUserFaceRes);
	return (m_pIUserFaceRes!=NULL);
}

//�����û�
bool CGameFrameView::SetUserInfo(WORD wChairID, tagUserData * pUserItem)
{
	ASSERT(wChairID<MAX_CHAIR);
	if (wChairID>=MAX_CHAIR) return false;
	m_pUserItem[wChairID]=pUserItem;
	UpdateGameView(NULL);
	return true;
}

//������Ϣ
int CGameFrameView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//��ʼ��
	//SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);
	//if (m_pThreadDraw!=NULL) m_pThreadDraw->BeginThread();
	
	//�����ؼ�
	//for(WORD i=0;i<CountArray(m_FlowerEffectControl);i++)
	//{
	//	SetFlowerControlInfo(i,0,0);
	//	m_FlowerEffectControl[i].CreateControl(this);
	//}

	std::string strCurPath = "";
	TCHAR szResourcePath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,szResourcePath);
	strCurPath.append(szResourcePath);
	CString strPath(strCurPath.c_str());
	strPath.Append("\\data\\gameframe\\plaza\\");
	//������Դ
	m_ImageTimerBack.LoadImage(TEXT(strPath + "TimerBack.png"));
	m_ImageTimerNumber.LoadImage(TEXT(strPath + "TimerNumber.png"));
	m_ImageBreakLine.LoadImage(TEXT(strPath + "ImageBreakLine.png"));
	m_ImageReady.LoadImage(TEXT(strPath + "USER_READY.PNG"));
	//m_ImageWaitFlash.LoadImage(TEXT(strPath + "GameWaitFlash.PNG"));

	//Author<cxf>
	m_ImageWinLostBk.LoadImage(TEXT(strPath + "UserWinLostNumber.png"));

	//װ������ͼƬ
	CString szPath("");
	szPath.Append(TEXT(strPath + "TitleTextNum.png"));
	char* chData = szPath.GetBuffer();
	m_ImageNumber.SetImageResource(chData,10);


	m_rcWinNumberArea.top=0;
	m_rcWinNumberArea.left=0;
	m_rcWinNumberArea.bottom=0;
	m_rcWinNumberArea.right=0;
	if (!m_ImageWinLostBk.IsNull())
	{
		m_rcWinNumberArea.bottom=m_ImageWinLostBk.GetHeight();
		m_rcWinNumberArea.right=m_ImageWinLostBk.GetWidth();
	}

	//��������
	CString strImage(strCurPath.c_str());
	strImage.Append("\\data\\image\\");
	CString strUserImage="";
	for (int i=0; i<GAME_CLIENT_USER_IMAGE_NUM_DEF; ++i)
	{
		if (0==i)
		{
			char chUserImage[32]={0};
			sprintf(chUserImage, "Image_G_%d_m.png", i);
			strUserImage.Append(strImage);
			strUserImage.Append(chUserImage);
			m_ImageUserFace[i][0].LoadImage(TEXT(strUserImage));
			
			strUserImage = "";
			::memset(chUserImage, 0, sizeof(chUserImage));
			sprintf(chUserImage, "Image_G_%d_w.png", i);
			strUserImage.Append(strImage);
			strUserImage.Append(chUserImage);
			m_ImageUserFace[i][1].LoadImage(TEXT(strUserImage));
		}
		else
		{
			strUserImage = "";
			char chUserImage[32]={0};
			sprintf(chUserImage, "Image_G_%d.png", i);
			strUserImage.Append(strImage);
			strUserImage.Append(chUserImage);
			m_ImageUserFace[i][0].LoadImage(TEXT(strUserImage));
			m_ImageUserFace[i][1].LoadImage(TEXT(strUserImage));
		}
	}
	
	return 0;
}

//�滭����
void CGameFrameView::OnPaint()
{
	CPaintDC dc(this);

	//�ж�ģʽ
	//if (m_pThreadDraw!=NULL) return;

	//��ȡλ��
	CRect ClientRect;
	GetClientRect(&ClientRect);
	int iWidth = ClientRect.Width();
	int iHeight = ClientRect.Height();

	//�ػ�������
	if ((m_bDoubleBuf==true)&&(m_ImageBuffer.IsNull()==false))
	{
		if (m_bReDraw==true)
		{
			CDC * pDC=CDC::FromHandle(m_ImageBuffer.GetDC());

			//���»���
			m_bReDraw=false;
			pDC->SetBkMode(TRANSPARENT);
			pDC->FillSolidRect(0,0,iWidth,iHeight,RGB(0,0,0));

			//�滭����
			DrawGameView(pDC,iWidth,iHeight);
			DrawFrameView(pDC, iWidth, iHeight);

			////���ŵȴ�����
			//if (m_bIsPlayWaitFlash)
			//{
			//	if (!m_ImageWaitFlash.IsNull())
			//	{
			//		m_ImageWaitFlash.DrawImage(pDC, m_rcWaitDrawFlash.left, m_rcWaitDrawFlash.top, I_GAME_WAIT_FLASH_WIDTH, I_GAME_WAIT_FLASH_HEIGHT, \
			//			m_iCountWaitFrame*I_GAME_WAIT_FLASH_WIDTH, 0, I_GAME_WAIT_FLASH_WIDTH,I_GAME_WAIT_FLASH_HEIGHT);
			//	}
			//}

			//������Դ
			m_ImageBuffer.ReleaseDC();

			CDC::DeleteTempMap();	// add by HouGuoJiang 2011-11-25
		}

		//�滭����
		CRect rcClip;
		dc.GetClipBox(&rcClip);
		m_ImageBuffer.BitBlt(dc,rcClip.left,rcClip.top,rcClip.Width(),rcClip.Height(),rcClip.left,rcClip.top);
	}
	else 
	{
		//�滭����
		dc.SetBkMode(TRANSPARENT);
		dc.FillSolidRect(0,0,iWidth,iHeight,RGB(0,0,0));
		DrawGameView(&dc,iWidth,iHeight);
		DrawFrameView(&dc, iWidth, iHeight);
	}

	DrawFrameView(&dc, iWidth, iHeight);
	return;
}

//λ�ñ仯
void CGameFrameView::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	
	//Ч��״̬
	if ((cx==0)||(cy==0)) return;
	if (GetSystemMetrics(SM_CXSCREEN)<cx) return;

	//���Ļ���ͼ
	if ((m_bDoubleBuf==true)&&(nType!=SIZE_MINIMIZED))
	{
		if ((cx>m_ImageSize.cx)||(cy>m_ImageSize.cy))
		{
			m_bReDraw=true;
			m_ImageBuffer.Destroy();
			m_ImageSize.cx=__max(cx,m_ImageSize.cx);
			m_ImageSize.cy=__max(cy,m_ImageSize.cy);
			m_ImageBuffer.Create(m_ImageSize.cx,m_ImageSize.cy,m_uColorCount);

			CFont DrawFont;
			DrawFont.CreateFont(-12,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("����"));
			CDC * pDC=CDC::FromHandle(m_ImageBuffer.GetDC());
			pDC->SelectObject(&DrawFont);

			//������Դ
			m_ImageBuffer.ReleaseDC();

			DrawFont.DeleteObject();
			CDC::DeleteTempMap();	// add by HouGuoJiang 2011-11-25
		}
	}

	//������ͼ
	RectifyGameView(cx,cy);

	//���û滭����
	if (m_pThreadDraw!=NULL)
	{
		m_pThreadDraw->SetClientRange(cx,cy);
		if (nType==SIZE_MINIMIZED) m_pThreadDraw->SuspendDrawThread();
		else m_pThreadDraw->ResumeDrawThread();
	}
	else UpdateGameView(NULL);

	return;
}

//�����Ϣ
void CGameFrameView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	
	WORD iChairNums = 0;
	CWnd* pMainWnd = GetParent();
	if (NULL != pMainWnd)
	{
		((CGameFrameDlg*)pMainWnd)->ViewHitOper();
		((CGameFrameDlg*)pMainWnd)->SetShowUserActionWnd(false);
		iChairNums = ((CGameFrameDlg*)pMainWnd)->GetGameDeskChairNums();
	}
	
	if (iChairNums <= 100 )
	{
		CRect rcFace;
		for (BYTE i=0; i<iChairNums; ++i)
		{
			rcFace.left = m_ptFace[i].x;
			rcFace.top = m_ptFace[i].y;
			rcFace.right = rcFace.left + CGameFrameView::m_nYFace;
			rcFace.bottom = rcFace.top + CGameFrameView::m_nXFace;
			if (rcFace.PtInRect(point))
			{
				const tagUserData * pUserData = GetUserInfo(i);
				if (NULL!=pUserData && NULL!=pMainWnd)
				{
					((CGameFrameDlg*)pMainWnd)->SendUserActionMsg(pUserData->wChairID);
				}
				break;
			}
		}
	}

	__super::OnLButtonDown(nFlags, point);
}

//������Ϣ
void CGameFrameView::OnDestroy()
{
	__super::OnDestroy();

	//�����߳�
	if (m_pThreadDraw!=NULL) m_pThreadDraw->EndThread();

	return;
}

//ʱ����Ϣ
void CGameFrameView::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);

	if (IDI_TIMER_FLASH == nIDEvent)
	{
		if (m_bIsDisableTimerFlash)
		{//����ʱ�䶯����
			m_bIsStartTimer = true;
			bool bIsIng = true;
			for (int i=0; i<m_wGameUserNums; ++i)
			{
				if (m_bIsDrawTimer[i] && m_wTimer[i]>0 && m_wTimer[i]<=ITIMER_FRAMES_START)
				{//���Ի����ˣ�����ʱ��Ҳ����ITIMER_FRAMES_START���ˡ�
	
					++m_iCurTimeFrames[i];
					
					if (m_iCurTimeFrames[i]>ITIMER_FLASH_TOTAL_FRAMES)
					{
						m_iCurTimeFrames[i] = ITIMER_FLASH_TOTAL_FRAMES;
						m_bIsDrawTimer[i] = false;
						m_bIsStartTimer = false;
						KillTimer(IDI_TIMER_FLASH);
					}
					CRect rect;
					rect.left = m_ptTimerCurPT[i].x-m_nXTimer/2;
					rect.top = m_ptTimerCurPT[i].y-m_nYTimer/2;
					rect.right = rect.left + m_nXTimer;
					rect.bottom = rect.top + m_nYTimer;
					UpdateGameView(&rect);
					bIsIng = false;
				}
			}
			if (bIsIng)
			{
				m_bIsStartTimer = false;
				KillTimer(IDI_TIMER_FLASH);
			}
		}
	}
	//else if (IDI_GAME_WAIT_FLASH == nIDEvent)
	//{
	//	if (m_bIsPlayWaitFlash)
	//	{
	//		++m_iCountWaitFrame;
	//		if (m_iCountWaitFrame >= I_GAME_WAIT_TOTAL_FRAMES-1)
	//		{
	//			m_iCountWaitFrame = 0;
	//		}
	//		UpdateGameView(&m_rcWaitDrawFlash);
	//	}
	//}


#if 0
	if ( nIDEvent == IDI_FLOWER_START )
	{
		//�ƶ��ж�
		for ( int nIndex = 0; nIndex < MAX_FLOWER_COUNT; ++nIndex )
		{
			//�Ϸ��ж�
			if(m_FlowerInfo[nIndex].wMoveCount==0)continue;

			//�ƶ�����
			if(m_FlowerInfo[nIndex].wMoveIndex<m_FlowerInfo[nIndex].wMoveCount)
			{
				m_FlowerInfo[nIndex].wMoveIndex++;
				WORD wMoveIndex = m_FlowerInfo[nIndex].wMoveIndex;
				WORD wMoveCount = m_FlowerInfo[nIndex].wMoveCount;
				m_FlowerInfo[nIndex].ptCurPos.x =m_FlowerInfo[nIndex].ptFrom.x + (m_FlowerInfo[nIndex].ptDest.x-m_FlowerInfo[nIndex].ptFrom.x)*wMoveIndex/wMoveCount;
				m_FlowerInfo[nIndex].ptCurPos.y =m_FlowerInfo[nIndex].ptFrom.y + (m_FlowerInfo[nIndex].ptDest.y-m_FlowerInfo[nIndex].ptFrom.y)*wMoveIndex/wMoveCount;
			}
			//�����ж�
			else
			{
				//��ʾ����
				m_FlowerEffectControl[m_FlowerInfo[nIndex].wTargetChairID].StartFlowerEffect(m_FlowerInfo[nIndex].uFlowerEffectID);
				m_FlowerEffectControl[m_FlowerInfo[nIndex].wTargetChairID].BringWindowToTop();

				//���ٴ���
				m_nFlowerCount--;

				//�������
				ZeroMemory(&m_FlowerInfo[nIndex], sizeof(m_FlowerInfo[nIndex]));
			}
		}

		//�����ж�
		if ( m_nFlowerCount == 0 ) KillTimer(nIDEvent);

		//���½���
		UpdateGameView(NULL);

		return;
	}
#endif
}

//////////////////////////////////////////////////////////////////////////

void CGameFrameView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CWnd* pMainWnd = AfxGetMainWnd();
	if (NULL != pMainWnd)
	{
		((CGameFrameDlg*)pMainWnd)->ViewHitOper();
		((CGameFrameDlg*)pMainWnd)->SetShowUserActionWnd(false);
	}
	__super::OnRButtonDown(nFlags, point);
}



//��ʾ���Կ�
void CGameFrameView::ShowUserSayInfo(BYTE btViewUserID, CString strTalkText)
{
	ASSERT(btViewUserID>=0 && btViewUserID<m_wGameUserNums);
	if (btViewUserID>=m_wGameUserNums) return;
	if (NULL != m_pTalkDlg[btViewUserID])
	{
		delete m_pTalkDlg[btViewUserID];
	}
	m_pTalkDlg[btViewUserID] = NULL;
	m_pTalkDlg[btViewUserID] = new CTalkDlg;
	if (NULL==m_pTalkDlg[btViewUserID]) return;
	CRect rect;
	AfxGetMainWnd()->GetWindowRect(&rect);
	m_pTalkDlg[btViewUserID]->Create(IDD_SAY_DLG, this);
	m_pTalkDlg[btViewUserID]->LoadTalkSkin(1);
	m_pTalkDlg[btViewUserID]->SetTalkDlgPos(rect.left, rect.top, rect.left+m_ptSayPos[btViewUserID].x, rect.top+m_ptSayPos[btViewUserID].y+30);
	m_pTalkDlg[btViewUserID]->SetTalkText(strTalkText);
	m_pTalkDlg[btViewUserID]->SetTalkDlgShow();
}

void CGameFrameView::HideUserSayInfo()
{
	for (int i=0; i<10; i++)
	{
		if (NULL != m_pTalkDlg[i])
		{
			if (m_pTalkDlg[i]->GetSafeHwnd() && m_pTalkDlg[i]->IsWindowVisible())
			{
				m_pTalkDlg[i]->OnKillAllTimer();
				m_pTalkDlg[i]->ShowWindow(SW_HIDE);
			}
		}
	}
}

//������Ϸ����û���
void CGameFrameView::SetGameUserNums(WORD dwNums)
{
	m_wGameUserNums = dwNums;
}

//������Ϸ�ȴ�����
void CGameFrameView::PlayGameWaitFlash(bool bIsPlay)
{
	return;

	CRect ClientRect;
	GetClientRect(&ClientRect);
	int iWidth = ClientRect.Width();
	int iHeight = ClientRect.Height();

	m_rcWaitDrawFlash.left = (iWidth-I_GAME_WAIT_FLASH_WIDTH)/2;
	m_rcWaitDrawFlash.top = (iHeight-I_GAME_WAIT_FLASH_HEIGHT)/2-50;
	m_rcWaitDrawFlash.right = m_rcWaitDrawFlash.left + I_GAME_WAIT_FLASH_WIDTH;
	m_rcWaitDrawFlash.bottom= m_rcWaitDrawFlash.top + I_GAME_WAIT_FLASH_HEIGHT;

	m_iCountWaitFrame = 0;
	m_bIsPlayWaitFlash = bIsPlay;
	KillTimer(IDI_GAME_WAIT_FLASH);
	if (bIsPlay)
	{
		//SetTimer(IDI_GAME_WAIT_FLASH, TIMER_WAIT_FLASH_INTERVAL, NULL);
	}

	UpdateGameView(&m_rcWaitDrawFlash);
}
//ˢ�¾��� Author<cxf>;DateTime<2012-03-05>
void CGameFrameView::ResetWinLost(tagUserData* pUserData)
{
	if (NULL != pUserData)
	{
		//�滭
		m_nKindIDWinCount=pUserData->lKindIDWinCount;
		m_nKindIDLostCount=pUserData->lKindIDLostCount;
		m_nKindIDDrawCount=pUserData->lKindIDDrawCount;
		m_nKindIDFleeCount=pUserData->lKindIDFleeCount;
		InvalidateRect(&m_rcWinNumberArea);
	}
}
//�滭Ӯȡ����ͼƬ;Author<cxf>
void CGameFrameView::DrawWinLostNumberBk(CDC * pDC)
{
	//Ӯȡ����ͼ
	if (!m_ImageWinLostBk.IsNull())
	{
		m_ImageWinLostBk.DrawImage(pDC,m_rcWinNumberArea.left, m_rcWinNumberArea.top);
	}
	//Ӯȡ����
	m_ImageNumber.SetNums(m_nKindIDWinCount);
	m_ImageNumber.SetShowRect(CRect((m_rcWinNumberArea.left+m_rcWinNumberArea.right),m_rcWinNumberArea.top+10,
		(m_rcWinNumberArea.left+m_rcWinNumberArea.right)+60,30));
	m_ImageNumber.DrawImageNums(pDC);

	m_ImageNumber.SetNums(m_nKindIDWinCount+m_nKindIDLostCount+m_nKindIDDrawCount+m_nKindIDFleeCount);
	m_ImageNumber.SetShowRect(CRect((m_rcWinNumberArea.left+m_rcWinNumberArea.right),m_rcWinNumberArea.top+30,
		(m_rcWinNumberArea.left+m_rcWinNumberArea.right)+60,30));
	m_ImageNumber.DrawImageNums(pDC);
}
//���û滭Ӯȡ����λ��;Author<cxf>
void CGameFrameView::SetWinLostDrawPoint(CPoint cpPoint)
{
	m_rcWinNumberArea.left=cpPoint.x;
	m_rcWinNumberArea.top=cpPoint.y;
}