#include "StdAfx.h"
#include "CardControl.h"
#include "GameScoreWnd.h"
#include "ImageNumsCS.h"
#include "D:\��Ŀ\������\Include\scoreview.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//��ť��ʶ 
#define IDC_GOON					101                     //������ť
#define IDC_EXIT                    102                     //�˳���ť
#define IDC_CHANGETABLE             103                     //������ť
#define IDC_OVERTIME                104                     //��ʱ
#define IDT_COUNTDOWN               201                     //����ʱ��ʶ
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameScoreWnd, CScoreView)
	ON_BN_CLICKED(IDC_GOON, OnGoOn)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_BN_CLICKED(IDC_CHANGETABLE, OnChangeTable)
	ON_BN_CLICKED(IDC_OVERTIME,OnOverTime)
	ON_WM_TIMER()
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////

//���캯��
CGameScoreWnd::CGameScoreWnd()
{
	//����ʱ
	m_IsTrue=false; 
	m_SecondCount=10;


	//���ñ���
	ZeroMemory(&m_GameScoreInfo,sizeof(m_GameScoreInfo));
	TCHAR szResourcePath[MAX_PATH]="";
	GetCurrentDirectory(MAX_PATH,szResourcePath);
	CString strPath(szResourcePath);
	strPath.Append("\\SparrowCS\\Picture\\GameEnd\\");
	m_Direction.LoadImage(TEXT(strPath+"DIRECTION.png"));
	m_ImageEndBirdCard.LoadImage(TEXT(strPath+"ENDBIRDCARD.png"));
	m_ImageWin.LoadImage(TEXT(strPath+"WIN.png"));
	m_ImageLost.LoadImage(TEXT(strPath+"LOST.png"));
	m_ImageChiHuDirection.LoadImage(TEXT(strPath+"CHIHUDIRECTION.png"));
	m_ImageNumDown.LoadImage(TEXT(strPath+"NUMBERDOWN.png"));

	ImageNums1.SetImageResource("NUMBER.png");
	ImageNums2.SetImageResource("NUMBER.png");
	return;
}

//��������
CGameScoreWnd::~CGameScoreWnd()
{
}

//���û���
void CGameScoreWnd::SetScoreInfo(tagGameScoreInfo GameScoreInfo)
{
	//���ñ���
	memcpy(&m_GameScoreInfo, &GameScoreInfo, sizeof(tagGameScoreInfo));

	//��ʾ����
	Invalidate(NULL);
	ShowWindow(SW_HIDE);

	return;
}

//��ʼ����Ϣ
void CGameScoreWnd::InitScoreViewInfo()
{
	//LoadBackImage(IDB_GAME_SCORE);
	CRect CreateRect(0,0,0,0);
	CString strPath = "";
	GetPicPath(strPath);
	LoadBackImage(TEXT(strPath+("GAME_END.png")));
	//������ť
	m_btGoOn.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_GOON);
	m_btExit.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_EXIT);
	m_btChangeTable.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_CHANGETABLE);
	m_btOverTime.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OVERTIME);
	//��ť����ͼƬ
	m_btGoOn.SetButtonImage(TEXT(strPath + "GOON.png"), false, 5);
	m_btExit.SetButtonImage(TEXT(strPath + "EXIT.png"), false, 5);
	m_btChangeTable.SetButtonImage(TEXT(strPath + "CHANGETABLE.png"), false, 5);
	m_btOverTime.SetButtonImage(TEXT(strPath+"OVERTIME.png"),false,5);

	//������ťλ��  
	m_btChangeTable.SetWindowPos(NULL, 138, 295, 75, 35, 0);//����
	m_btGoOn.SetWindowPos(NULL,265, 295, 75, 35, 0);//����
	m_btOverTime.SetWindowPos(NULL,195,9,72,28,0);//��ʱ
	m_btExit.SetWindowPos(NULL, 268, 9, 72, 28, 0);//�˳�
	

	m_btGoOn.ShowWindow(SW_SHOW);
	m_btExit.ShowWindow(SW_SHOW);
	m_btChangeTable.ShowWindow(SW_SHOW);
	m_btOverTime.ShowWindow(SW_SHOW);

	m_rcDrawTimer.left = 210;
	m_rcDrawTimer.top = 295;
	m_rcDrawTimer.right = m_rcDrawTimer.left + 52;
	m_rcDrawTimer.bottom = m_rcDrawTimer.top + 36;

}

//��ʼ����Ϣ��
void CGameScoreWnd::AlreadyInitInfo()
{
	/*SetScoreUI(1);
	m_BTGoPlaza.SetWindowPos(NULL, 24, 325, 126, 38, 0);
	m_BTStartGame.SetWindowPos(NULL, 129, 320, 160, 46, 0);
	m_BTGoRoom.SetWindowPos(NULL, 260, 325, 126, 38, 0);*/
}


//����
void CGameScoreWnd::DrawScoreInfo(CDC* pDC)
{
//#if 0
	//����DC
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(230,230,230));
	pDC->SelectObject(CSkinResourceManager::GetDefaultFont());

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	////��������
	//CDC BufferDC;
	//CBitmap ImageBuffer;
	//BufferDC.CreateCompatibleDC(&dc);
	//ImageBuffer.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
	//BufferDC.SelectObject(&ImageBuffer);

	//�滭����
	//CImageHandle BackImageHandle(&m_ImageBack);
	//m_ImageBack.BitBlt(BufferDC,0,0);

	//int iEndTagImageW = 0;
	//int iEndTagImageH = 0;
	//if (!m_ImageEndTag.IsNull())
	//{
	//	iEndTagImageW = m_ImageEndTag.GetWidth()/2;
	//	iEndTagImageH = m_ImageEndTag.GetHeight();
	//}

	//CImageNumsCS	ImageNums;
	//ImageNums.SetSeparatorWH(17, 25);
	//ImageNums.SetImageResource("NUMBER.png");
	////ImageNums.SetNumAlign(IA_LEFT);
	//ImageNums.SetSign(true);
	//ImageNums.SetSeparator(false);

	//CRect rcScore(100,i*29+105,300,i*29+200);
	//ImageNums.SetShowRect(rcScore);
	//ImageNums.SetNums(100);
	//ImageNums.DrawImageNums(&BufferDC);

	//���� DC
	//BufferDC.SetBkMode(TRANSPARENT);
	//BufferDC.SetTextColor(RGB(230,230,230));
	//BufferDC.SelectObject(CSkinResourceManager::GetDefaultFont());



	//�û��ɼ�
	CRect rcDraw;
	//TCHAR szBuffer[64];

	//m_GameScoreInfo.lScore[0]=1;
	//m_GameScoreInfo.lScore[1]=-47;
	//m_GameScoreInfo.lScore[2]=-45545;
	//m_GameScoreInfo.lScore[3]=15385;
	/*m_GameScoreInfo.lScore[0]=-1000;
	m_GameScoreInfo.lScore[1]=-1000;
	m_GameScoreInfo.lScore[2]=-1000;
	m_GameScoreInfo.lScore[3]=3000;
	m_GameScoreInfo.iChiHuKind[0]=0;
	m_GameScoreInfo.iChiHuKind[1]=0;
	m_GameScoreInfo.iChiHuKind[2]=0;
	m_GameScoreInfo.iChiHuKind[3]=1;*/
	bool bIsLeft = false;
	for (int j=0; j<GAME_PLAYER; ++j)
	{
		if (m_IsUserLeft[j])
		{
			bIsLeft = true;
			break;
		}
	}
	int iDrawDirectX = 69;

	for (int i=0;i<GAME_PLAYER;i++)
	{
		//m_GameScoreInfo.szUserName[i]="adadsdasds";
///////////////////////////////////////////////////////////////////////////////////////////
		//������Ҵ�ӡ
////////////////////////////////////////////////////////////////////////////////////////////
		//�û����� ��λ��Ϣ
		int nWidth = m_Direction.GetWidth();
		int nHeight = m_Direction.GetHeight()/20;
		LONGLONG  lShowScore=0;
		lShowScore=m_GameScoreInfo.lScore[i]+m_GameScoreInfo.iCellScore;//�������ʾ����

		WORD wIndex=((i+GAME_PLAYER)-m_GameScoreInfo.wBankerUser)%GAME_PLAYER;
		rcDraw.left=12;
		rcDraw.right=200;
		rcDraw.top=129+i*nHeight;
		rcDraw.bottom=rcDraw.top+nHeight;
		if (m_MeChairId == i)
		{
			pDC->DrawText(m_GameScoreInfo.szUserName[i],lstrlen(m_GameScoreInfo.szUserName[i]),&rcDraw,0);
			if(lShowScore>0 )
			{
				m_ImageWin.DrawImage(pDC,198,78);
				if (1==m_GameScoreInfo.iChiHuKind[i])
				{//����
					m_Direction.DrawImage(pDC,42,127+nHeight*i, nWidth-30, nHeight,30, 5*wIndex*nHeight);
				}
				else if (2==m_GameScoreInfo.iChiHuKind[i])
				{//����
					m_Direction.DrawImage(pDC,42,127+nHeight*i, nWidth-30, nHeight,30, (5*wIndex+1)*nHeight);
				}
				else
				{//����ʤ��
					m_Direction.DrawImage(pDC,42,127+nHeight*i, 146, nHeight,30, (5*wIndex+1)*nHeight,146, nHeight);
				}
				
			}
			else if (lShowScore<0)
			{
				m_ImageLost.DrawImage(pDC,198,78);
				if (3==m_GameScoreInfo.iChiHuKind[i])
				{//����
					m_Direction.DrawImage(pDC,42,127+nHeight*i, nWidth-30, nHeight,30, (5*wIndex+2)*nHeight);
				}
				else
				{//������
					m_Direction.DrawImage(pDC,42,127+nHeight*i, nWidth-30, nHeight,30, (5*wIndex+3)*nHeight);
				}
				
			}
			else
			{//ƽ��
				m_Direction.DrawImage(pDC,42,127+nHeight*i, nWidth-30, nHeight,30, (5*wIndex+4)*nHeight);
			}
			
		}
		else
		{
			if(lShowScore>0 )
			{
				if (1==m_GameScoreInfo.iChiHuKind[i])
				{//����
					m_Direction.DrawImage(pDC,12,127+nHeight*i, nWidth, nHeight,0, 5*wIndex*nHeight);
				}
				else if (2==m_GameScoreInfo.iChiHuKind[i])
				{//����
					m_Direction.DrawImage(pDC,12,127+nHeight*i, nWidth, nHeight,0, (5*wIndex+1)*nHeight);
				}
				else
				{//����ʤ��
					m_Direction.DrawImage(pDC,12,127+nHeight*i, 146, nHeight,0, (5*wIndex+1)*nHeight,146, nHeight);
				}
			}
			else if (lShowScore<0)
			{
				if (3==m_GameScoreInfo.iChiHuKind[i])
				{
					m_Direction.DrawImage(pDC,12,127+nHeight*i, nWidth, nHeight,0, (5*wIndex+2)*nHeight);
				}
				else
				{
					m_Direction.DrawImage(pDC,12,127+nHeight*i, nWidth, nHeight,0, (5*wIndex+3)*nHeight);
				}
			}
			else
			{
				m_Direction.DrawImage(pDC,12,127+nHeight*i, nWidth, nHeight,0, (5*wIndex+4)*nHeight);
			}

		}

		if (!bIsLeft)
		{
			if(lShowScore>0 )
			{//�滭�Ժ����
				m_ImageChiHuDirection.DrawImage(pDC,iDrawDirectX, 45,42,21,wIndex*42,0,42,21);
				iDrawDirectX += 50;
			}
		}
////////////////////////////////////////////////////////////////////////////////////////////
		//��Ԫ����
		ImageNums1.SetSeparatorWH(9, 12);
		ImageNums1.SetImageResource("NUMBER.png");
		ImageNums1.SetSeparator(false);
		CRect rcCellScore(67,82,200,200);
		ImageNums1.SetShowRect(rcCellScore);
		ImageNums1.SetSign(false);
		ImageNums1.SetNums(m_GameScoreInfo.iCellScore);
		ImageNums1.DrawImageNums(pDC);
////////////////////////////////////////////////////////////////////////////////////////////
		//�����
		CRect rcTax(67,205,200,200);
		ImageNums1.SetShowRect(rcTax);
		ImageNums1.SetNums(m_GameScoreInfo.iCellScore);
		ImageNums1.DrawImageNums(pDC);
////////////////////////////////////////////////////////////////////////////////////////////
        //����
		CRect rcScore(176,i*20+130,400,i*20+200);
		ImageNums2.SetSign(true);
		ImageNums2.SetImageResource("NUMBER.png");
		ImageNums2.SetShowRect(rcScore);
		ImageNums2.SetNums(lShowScore);
		ImageNums2.DrawImageNums(pDC);
////////////////////////////////////////////////////////////////////////////////////////////
		//����
		CRect rcTimes(280,i*20+130,400,i*20+200);
		ImageNums2.SetShowRect(rcTimes);
		ImageNums2.SetNums(m_GameScoreInfo.cbTimes[i]);
		if (m_GameScoreInfo.cbTimes[i]!=0)
		{
			ImageNums2.DrawImageNums(pDC);
		}		
////////////////////////////////////////////////////////////////////////////////////////////
		//�滭������
		if (0 != m_GameScoreInfo.cbBirdCard)
		{
			int nXPos=(GetImageIndex(m_GameScoreInfo.cbBirdCard)-1)*44;
			m_ImageEndBirdCard.DrawImage(pDC,50,256,44,46,nXPos,0);			
		}
		
		////�����־
		//if (m_GameScoreInfo.wBirdHitUser==i)
		//{
		//	m_ImageBirdTag.DrawImage(pDC,142,127+nHeight*i, 30, nHeight,0,0);
		//}
			



		////������Ŀ
		//if(m_GameScoreInfo.cbKingCount[i] != 0 && m_GameScoreInfo.lScore[i]>0)
		//{
		//  CRect rcKingCount(310,i*40+130,600,i*40+200);
		//  m_ImageKingCount.DrawImage(pDC, rcKingCount.left, rcKingCount.top, iKingCountImageW, iKingCountImageH, iKingCountImageW*(m_GameScoreInfo.cbKingCount[i]), 0, iKingCountImageW, iKingCountImageH);
		//}
////////////////////////////////////////////////////////////////////////////////////////////
		//�û�״̬
		//if (m_GameScoreInfo.wCardType[i]!=0)
		//{
		//	CRect rcStatus(175,i*41+120,300,i*41+200);
		//	m_ImageEndTag.DrawImage(pDC, rcStatus.left, rcStatus.top, iEndTagImageW, iEndTagImageH, 0, 0, iEndTagImageW, iEndTagImageH);
		//}
		//else if (m_GameScoreInfo.wProvideUser==i)
		//{
		//	CRect rcStatus(175,i*41+120,300,i*41+200);
		//	m_ImageEndTag.DrawImage(pDC, rcStatus.left, rcStatus.top, iEndTagImageW, iEndTagImageH, iEndTagImageW, 0, iEndTagImageW, iEndTagImageH);
		//}
////////////////////////////////////////////////////////////////////////////////////////////
		
		//ׯ����Ϣ
		//if (m_GameScoreInfo.wBankerUser==i)
		//{
		//	CRect rcStatus(138,i*42+120,300,i*42+200);
		//	m_Banker.DrawImage(pDC, rcStatus.left, rcStatus.top, m_Banker.GetWidth(),  m_Banker.GetHeight(), 0, 0,  m_Banker.GetWidth(),  m_Banker.GetHeight());
		//}
////////////////////////////////////////////////////////////////////////////////////////////

	}

	//�滭����ʱ
	if (m_IsTrue==true)
	{
		INT CountDownWith=m_ImageNumDown.GetWidth()/10;
		INT CountDownHeight=m_ImageNumDown.GetHeight();
		INT CountDown=0;
		INT iCountDown=0;
		iCountDown=m_SecondCount;
		if (iCountDown < 10)
		{
			CountDown = 1;
		}
		else
		{
			CountDown = 2;
		}
		if (CountDown>=1 && CountDown<=2)
		{
			INT xPos= m_rcDrawTimer.left;
			INT yPos=m_rcDrawTimer.top;
			if (1 == CountDown)
			{
				//��λ
				m_ImageNumDown.DrawImage(pDC, xPos+CountDownHeight/2,yPos,CountDownWith,CountDownHeight,iCountDown*CountDownWith,0);
			}
			else
			{
				//ʮλ
				m_ImageNumDown.DrawImage(pDC, xPos,yPos,CountDownWith,CountDownHeight,(iCountDown/10)*CountDownWith,0);
				//��λ
				m_ImageNumDown.DrawImage(pDC, xPos+CountDownWith,yPos,CountDownWith,CountDownHeight,(iCountDown%10)*CountDownWith,0);
			}
		}
	}

	//�滭����

	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pDC,0,0,SRCCOPY);

}


//��ȡλ��
int CGameScoreWnd::GetImageIndex(BYTE cbCardData)
{
	//�����ж�
	if (cbCardData==0) 
		return 0;

	//����λ��
	BYTE cbValue=cbCardData&MASK_VALUE;
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return (cbColor>=0x03)?(cbValue+27):(cbColor*9+cbValue);
}



void CGameScoreWnd::GetPicPath(CString& strPath)
{
	strPath = "";
	TCHAR szResourcePath[MAX_PATH]={0};
	GetCurrentDirectory(MAX_PATH,szResourcePath);
	strPath.Append(szResourcePath);
	strPath.Append("\\SparrowCS\\Picture\\GameEnd\\");
}

//��ť��Ϣ
void CGameScoreWnd::OnChangeTable()
{
	SetCountDown(false);
	OnStartGame();
}
void CGameScoreWnd::OnExit()
{
	SetCountDown(false);
	OnGoExit();
}
void CGameScoreWnd::OnGoOn()
{
	SetCountDown(false);
	OnStartGame();
}

void CGameScoreWnd::OnOverTime()
{
	m_SecondCount += 10;
	m_btOverTime.EnableWindow(FALSE);
	InvalidateRect(&m_rcDrawTimer,FALSE);
}


void CGameScoreWnd::GetViewID(WORD MyChairID, WORD BankerChairID)
{
	m_MeChairId = MyChairID;
	m_BankerChairID = BankerChairID;
}

//����ǿ��
void CGameScoreWnd::SetUserLeft(WORD wChairID,bool IsLeft)
{
	m_IsUserLeft[wChairID]=IsLeft;
	if(m_hWnd!=NULL) Invalidate(FALSE);
}
//����ʱ����
void CGameScoreWnd::SetCountDown(bool Istrue)
{
	if (Istrue)
	{
		m_IsTrue=true;
		m_SecondCount=10;
		SetTimer(IDT_COUNTDOWN,1000,NULL);
	}
	else
	{
		KillTimer(IDT_COUNTDOWN);
		m_IsTrue=false;
		m_SecondCount=10;
		InvalidateRect(&m_rcDrawTimer,FALSE);
	}
}

void CGameScoreWnd::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (IDT_COUNTDOWN==nIDEvent)
	{
		if (m_IsTrue==false)
		{
			KillTimer(IDT_COUNTDOWN);
			return;
		}
		if (m_SecondCount<=1)
		{
			KillTimer(IDT_COUNTDOWN);
			m_IsTrue=false;
			OnGoExit();
		}
		else
		{
			m_SecondCount--;
		}
		InvalidateRect(&m_rcDrawTimer,FALSE);
	}

	CScoreView::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
