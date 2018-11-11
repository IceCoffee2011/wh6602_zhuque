#include "StdAfx.h"
#include "FishScoreView.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CFishScoreView, CDialog)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

#define IDC_EDIT_ID 1009
////��ť��ʶ
//enum enViewButtonsID
//{
//	enViewButtonsID_Begin=100,
//
//	IDC_QUIT_GAME,											//�˳���Ϸ
//	IDC_CALCSCORE,											//���㲢�˳�
//	IDC_CONTINUE_GAME,										//������Ϸ
//
//	enViewButtonsID_End
//};

//��ʱ��ʶ
enum enTimerID
{
	enTimerID_Begin=100,

	IDI_ALPHA_DEPTH,										//͸����ʱ

	enTimerID_End
};
//////////////////////////////////////////////////////////////////////////

//���캯��
CFishScoreView::CFishScoreView(CWnd	*pViewWnd, LONG lCellScore):CDialog(IDD_SCORE)
{
	//���ñ���
	m_hRGN=NULL;
	m_pViewWnd=pViewWnd;
	ZeroMemory(m_wCptrFishCount,sizeof(m_wCptrFishCount));
	ZeroMemory(m_byAlphaDepth,sizeof(m_byAlphaDepth));
	m_lBulletCount = 0L;
    m_lCoinCount=0L;	
	m_AlphaIndex=enFishKind_1;
	m_bShowNumber=false;
	m_lCellScore=lCellScore;

	//����ͼƬ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_PngBackUp.LoadImage(hInstance,TEXT("GAME_END_UP"));
	m_PngBackDown.LoadImage(hInstance,TEXT("GAME_END_DOWN"));
	m_PngNumber.LoadImage(hInstance,TEXT("GAME_END_NUMBER"));
	m_PngNumberScore.LoadImage(hInstance,TEXT("GAME_END_SCORE_NUMBER"));
	m_PngFish.LoadImage(hInstance,TEXT("GAME_END_FISH"));

	return;
}

//��������
CFishScoreView::~CFishScoreView()
{
}

//������Ϣ
int CFishScoreView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	return 0;
}


//�����Ϣ
void CFishScoreView::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//��Ϣģ��
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));	

	return;
}

//�ػ�����
void CFishScoreView::OnPaint() 
{
	//�滭DC
	CPaintDC dc(this);

	//�����ж�
	if (m_ImageFish.IsNull())
	{
		//����ͼƬ
		m_ImageFish.Create(m_PngFish.GetWidth(),m_PngFish.GetHeight(),24);

		//����ͼƬ
		HDC hDCBuffer=m_ImageFish.GetDC();
		CDC *pDCBuffer=CDC::FromHandle(hDCBuffer);
		pDCBuffer->FillSolidRect(0,0,m_PngFish.GetWidth(),m_PngFish.GetHeight(),RGB(12,34,60));
		m_PngFish.DrawImage(pDCBuffer,0,0);
		m_ImageFish.ReleaseDC();
	}

	//������Դ
	CDC dcMemory;
	CBitmap bmpMemory;

	//��ȡ��С
	CRect rcClient;
	GetClientRect(rcClient);

	//������Դ
	dcMemory.CreateCompatibleDC(&dc);
	bmpMemory.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
	CBitmap *pOldBmp=dcMemory.SelectObject(&bmpMemory);

	//�滭����
	dcMemory.FillSolidRect(rcClient,RGB(0,23,35));
	m_PngBackUp.DrawImage(&dcMemory,0,0);
	m_PngBackDown.DrawImage(&dcMemory,0,rcClient.Height()-m_PngBackDown.GetHeight());

    //��Ⱥ����
	CImageHandle ImageHandleFish(&m_ImageFish);
	int nDrawPosX=55,nDrawPosY=48;
	int nFishPosX=0,nFishPosY=0;
	int nFishWidth=m_ImageFish.GetWidth()/13,nFishHeight=m_ImageFish.GetHeight()/2;
	int nDrawSpaceX=(rcClient.Width()-110)/3;
	int nDrawSpaceY=152/4;

	//�滭��Ⱥ
	for (enFishKind FishKind=enFishKind_1; FishKind<enFishKind_No; FishKind=enFishKind(FishKind+1))
	{
		//��������
		int nFishIndex=FishKind;
		if (enFishKind_12==FishKind) nFishIndex+=1;

		//�����ж�
		if (m_wCptrFishCount[FishKind]>0 && m_byAlphaDepth[FishKind]>0)
		{
			//��ʾ��Ⱥ
			m_ImageFish.AlphaBlend(dcMemory.GetSafeHdc(),nDrawPosX+(FishKind%3)*nDrawSpaceX,nDrawPosY+(FishKind/3)*nDrawSpaceY,
				nFishWidth,nFishHeight,nFishIndex*nFishWidth,0,nFishWidth,nFishHeight,m_byAlphaDepth[FishKind]);

			//��ʾ����
			if (m_byAlphaDepth[FishKind]>=255) 
			{
				DrawNumber(&dcMemory,nDrawPosX+(FishKind%3)*nDrawSpaceX+50,nDrawPosY+(FishKind/3)*nDrawSpaceY+13,m_wCptrFishCount[FishKind],true);
			}
		}
		else
		{
			m_ImageFish.BitBlt(dcMemory.GetSafeHdc(),nDrawPosX+(FishKind%3)*nDrawSpaceX,nDrawPosY+(FishKind/3)*nDrawSpaceY,
				nFishWidth,nFishHeight,nFishIndex*nFishWidth,1*nFishHeight);
		}
	}

	//�����Ŀ
	if (true==m_bShowNumber)
	{
		long _lAllMoney = m_lCoinCount+m_lBulletCount*m_lCellScore;
		//�ܼƽ��
		DrawNumber(&dcMemory, 145, 228, _lAllMoney);
		//������� 1���� = 100���
		DrawNumber(&dcMemory, 305, 228, _lAllMoney / 100);
		//�Ͷ��ٸ����� �����Ǹ��������
		DrawNumber(&dcMemory, 145, 288, _lAllMoney / 100);
		//�͸�˭ �����Ǹ������

		//ʣ����ٸ�
		DrawNumber(&dcMemory,145,328, _lAllMoney % 100);
	}
	
	////��������
	//CFont DrawFont;
	//DrawFont.CreateFont(-12,0,0,0,FW_BOLD,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("����"));
	//CFont * pOldFont=dcMemory.SelectObject(&DrawFont);
	//dcMemory.SetTextColor(RGB(225,227,161));
	//dcMemory.SetBkMode(TRANSPARENT);

	//�滭����
	CRect rcClip;
	dc.GetClipBox(&rcClip);
	dc.BitBlt(rcClip.left,rcClip.top,rcClip.Width(),rcClip.Height(),&dcMemory,rcClip.left,rcClip.top,SRCCOPY);

	//�����Դ
	/*dcMemory.SelectObject(pOldFont);
	DrawFont.DeleteObject();*/
	dcMemory.SelectObject(pOldBmp);
	bmpMemory.DeleteObject();
	dcMemory.DeleteDC();

	return;
}

//��ʼ����
BOOL CFishScoreView::OnInitDialog()
{
	__super::OnInitDialog();
		//������ť
	m_btQuitGame.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(10,10,20,30),this,IDC_QUIT_GAME);
	m_btCalcScore.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(10,10,20,30),this,IDC_CALCSCORE);
	m_btContinueGame.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(10,10,20,30),this,IDC_CONTINUE_GAME);

	//�����
	m_font.CreateFont(32,16,0,0,0,FALSE,FALSE,0,0,0,0,0,0,_TEXT("����")); 
	m_etSendID.Create(WS_CHILD|WS_VISIBLE, CRect(0, 0, 150, 35), this, IDC_EDIT_ID);
	m_etSendID.SetFont(&m_font,TRUE);
	m_font.Detach();
	//m_etSendID.SetWindowText("");

	//��ťͼƬ
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btQuitGame.SetButtonImage(IDB_BT_QUIT_GAME_EX,hInstance,5,false);
	m_btCalcScore.SetButtonImage(IDB_BT_CALC_SCORE,hInstance,5,false);
	m_btContinueGame.SetButtonImage(IDB_BT_CONTINUE_GAME,hInstance,5,false);

	//���ô�С
	MoveWindow(0,0,486,427);

	//��������
	if (NULL!=m_hRGN) DeleteObject(m_hRGN);
	m_hRGN=CreateRoundRectRgn(0,0,486,427,25,25);
	if (NULL!=m_hRGN) SetWindowRgn(m_hRGN,TRUE);

	//���д���
	CenterWindow(m_pViewWnd);

	//���ö�ʱ
	SetTimer(IDI_ALPHA_DEPTH,10,NULL);

	return TRUE;
}

//�����
BOOL CFishScoreView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//��������
	UINT nCommandID=LOWORD(wParam);

	//��Ϣ����
	switch(nCommandID)
	{
	case IDC_QUIT_GAME:
		{
			AfxGetMainWnd()->PostMessage(WM_QUIT_GAME,0,0);

			__super::OnCancel();
			
			return TRUE;
		}
	case IDC_CONTINUE_GAME:
		{
			__super::OnCancel();

			return TRUE;
		}
	case IDC_CALCSCORE:	//���㲢�˳�
		{
			long _lAllMoney = m_lCoinCount+m_lBulletCount*m_lCellScore;	//��ʱ����

			char szTemp[30]={0};
			m_etSendID.GetWindowText(szTemp,sizeof(szTemp));	//�õ������ID

			m_Info.nBOSS = _lAllMoney / 100;	//��ʱ�ģ�ʵ��Ӧ�ô������ȡ����

			m_Info.ID	=atoi(szTemp);

			__super::OnOK();

			AfxGetMainWnd()->PostMessage(WM_QUIT_GAME,0,0);

			return TRUE;
		}		
	}
	return __super::OnCommand(wParam,lParam);
}

//��С��Ϣ
void CFishScoreView::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	//�Ϸ��ж�
	if (NULL==m_btQuitGame.GetSafeHwnd()) return;

	//��ʼ����
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	HDWP hDWP=BeginDeferWindowPos(10);

	//����λ��
	CRect rcButton;
	m_btCalcScore.GetWindowRect(&rcButton);
	int nSpace=(cx-70-rcButton.Width()*3)/2;
	DeferWindowPos(hDWP,m_btQuitGame,NULL,35,cy-45,0,0,uFlags);
	DeferWindowPos(hDWP,m_btCalcScore,NULL,35+1*(rcButton.Width()+nSpace),cy-45,0,0,uFlags);
	DeferWindowPos(hDWP,m_btContinueGame,NULL,35+2*(rcButton.Width()+nSpace),cy-45,0,0,uFlags);
	
	//
	DeferWindowPos(hDWP, m_etSendID, NULL, 290, 282, 0, 0, uFlags);
	
	//�����ƶ�
	EndDeferWindowPos(hDWP);
}

//��ʱ��Ϣ
void CFishScoreView::OnTimer(UINT nIDEvent)
{
	//����ʱ
	switch (nIDEvent)
	{
	case IDI_ALPHA_DEPTH:
		{
			//�޸�͸��
			if (m_AlphaIndex<enFishKind_No && m_byAlphaDepth[m_AlphaIndex]<255 && m_wCptrFishCount[m_AlphaIndex]>0) 
			{
				m_byAlphaDepth[m_AlphaIndex]+=51;
			}
			else if (m_AlphaIndex>=enFishKind_No-1)
			{
				m_bShowNumber=true;
				KillTimer(IDI_ALPHA_DEPTH);
			}
			else
			{
				while (true)
				{
					//��������
					m_AlphaIndex=enFishKind(m_AlphaIndex+1);

					//�ж��ж�
					if (m_AlphaIndex>=enFishKind_No)
					{
						m_bShowNumber=true;
						KillTimer(IDI_ALPHA_DEPTH);

						//���½���
						InvalidateRect(NULL,FALSE);
						return;
					}
					if (m_wCptrFishCount[m_AlphaIndex]>0) break;					
				}

				//�޸�͸��
				m_byAlphaDepth[m_AlphaIndex]+=51;
			}

			//���½���
			InvalidateRect(NULL,FALSE);

			return;
		}
	}
	__super::OnTimer(nIDEvent);
}

//�滭����
void CFishScoreView::DrawNumber(CDC *pDC, int nXPos, int nYPos, LONGLONG lNumber, bool bShowQuantifier)
{
	//��ȡ����
	const INT nNumberHeight=m_PngNumber.GetHeight();
	const INT nNumberWidth=m_PngNumber.GetWidth()/11;

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

	//�滭����
	for (LONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		WORD wCellNumber=WORD(lNumber%10);
		m_PngNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,wCellNumber*nNumberWidth,0);

		//���ñ���
		lNumber/=10;
		nXDrawPos-=(nNumberWidth+nSpace);
	}

	//�������
	if (true==bShowQuantifier)
	{
		nXDrawPos=nXPos+(lNumberCount+1)*(nNumberWidth+nSpace);
		m_PngNumber.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberWidth,10*nNumberWidth,0);
	}
}

//�滭����
void CFishScoreView::DrawNumber(CDC *pDC, int nXPos, int nYPos, LONGLONG lNumber)
{
	//��ȡ����
	const INT nNumberHeight=m_PngNumberScore.GetHeight();
	const INT nNumberWidth=m_PngNumberScore.GetWidth()/10;

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
	INT nXDrawPos=nXPos+lNumberCount*(nNumberWidth+nSpace)/2;

	//�滭����
	for (LONG i=0;i<lNumberCount;i++)
	{
		//�滭����
		WORD wCellNumber=WORD(lNumber%10);
		m_PngNumberScore.DrawImage(pDC,nXDrawPos,nYDrawPos,nNumberWidth,nNumberHeight,wCellNumber*nNumberWidth,0);

		//���ñ���
		lNumber/=10;
		nXDrawPos-=(nNumberWidth+nSpace);
	}
}

//������Ϣ
void CFishScoreView::SetScoreInfo(LONG lBulletCount,WORD wCptrFishCount[enFishKind_No],LONGLONG lCoinCount)//, long lLeaveScore)
{
	//������Ϣ
	m_lBulletCount = lBulletCount;											//�ӵ���Ŀ
	CopyMemory(m_wCptrFishCount,wCptrFishCount,sizeof(m_wCptrFishCount));	//�������
	m_lCoinCount=lCoinCount;												//ʣ��Ľ������
}

//weni23 ����֤
const CMD_C_BalanceInfo CFishScoreView::GetBalanceInfo()
{
	return m_Info;
}
//////////////////////////////////////////////////////////////////////////
