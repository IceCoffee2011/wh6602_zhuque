#include "StdAfx.h"
#include "SkinControls.h"
#include "GameScoreView.h"
#include ".\scoreview.h"
#include ".\gamescoreview.h"

#include "TraceCatch.h"

#include <math.h>
//////////////////////////////////////////////////////////////////////////
//��ť��ʶ 
#define IDC_GOON					101                     //������ť
#define IDC_EXIT                    102                     //�˳���ť
#define IDC_CHANGETABLE             103                     //������ť
#define IDC_OVERTIME                104                     //��ʱ

#define IDT_COUNTDOWN               201                     //����ʱ��ʶ
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameScoreView, CScoreView)
   ON_BN_CLICKED(IDC_GOON, OnGoOn)
   ON_BN_CLICKED(IDC_EXIT, OnExit)
   ON_BN_CLICKED(IDC_CHANGETABLE, OnChangeTable)
   ON_BN_CLICKED(IDC_OVERTIME,OnOverTime)
   ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
//���캯��
CGameScoreView::CGameScoreView(void)
{
	__ENTER_FUNCTION 

	//��������
	memset(m_lGameTax,0,sizeof(m_lGameTax));
	memset(m_lGameScore,0,sizeof(m_lGameScore));
	memset(m_szUserName,0,sizeof(m_szUserName));
    memset(m_RoomScore,0,sizeof(m_RoomScore)); 
	memset(m_UserBeiShu,0,sizeof(m_UserBeiShu));
	//����֡��
	m_ActionCount=0;
	m_IsShow=false;
	//����ʱ
	m_IsTrue=false; 
	m_SecondCount=10;

	//������ʯ
	memset(m_EncourageGem,0,sizeof(m_EncourageGem));
	//�û�ǿ��
	for(int i=0;i<GAME_PLAYER;++i)
	{ 
		m_IsUserLeft[i]=false;
	}
	CString strPath = "";
	GetPicPath(strPath);
	//������Դ
	 m_ImageWin.LoadImage(TEXT(strPath+"WIN.png"));
	 m_ImageLose.LoadImage(TEXT(strPath+"LOSE.png"));
	 m_BalanceBei.LoadImage(TEXT(strPath+"BALANCEBEI.png"));
	 m_ImageRoomScore.LoadImage (TEXT(strPath+"BALANCEBEI.png"));
	 m_ImageServers.LoadImage(TEXT(strPath+("SERVERS.png")));
	 m_ImageLand.LoadImage(TEXT(strPath+("LAND.png")));
	 m_ImageFarmer.LoadImage(TEXT(strPath+("FARMER.png")));
	 m_ImageLandWin.LoadImage(TEXT(strPath+("LANDWIN.png")));
	 m_ImageLandLose.LoadImage(TEXT(strPath+("LANDLOSE.png")));
	 m_ImageFarmerLose.LoadImage(TEXT(strPath+("FARMERLOSE.png")));
	 m_ImageFarmerWin.LoadImage(TEXT(strPath+("FARMERWIN.png")));
	 m_ImageUserLeft.LoadImage(TEXT(strPath+("USERLEFT.png")));
	 m_ImageCountDown.LoadImage(TEXT(strPath+("COUNTDOWN.png")));

	 __LEAVE_FUNCTION
}

//��������
CGameScoreView::~CGameScoreView(void)
{
}

//��ʼ����Ϣ
void CGameScoreView::InitScoreViewInfo()
{
	__ENTER_FUNCTION 

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
	m_btGoOn.SetButtonImage(TEXT(strPath + "GO_ON.png"), false, 4);
	m_btExit.SetButtonImage(TEXT(strPath + "EXIT.png"), false, 4);
	m_btChangeTable.SetButtonImage(TEXT(strPath + "CHANGE_TABLE.png"), false, 4);
	m_btOverTime.SetButtonImage(TEXT(strPath+"OVERTIME.png"),false,4);
	//������ťλ��   
	m_btGoOn.SetWindowPos(NULL,90, 277, 76, 35, 0);//����
	m_btChangeTable.SetWindowPos(NULL, 240, 277, 76, 35, 0);//����
	m_btExit.SetWindowPos(NULL, 230, 17, 71, 36, 0);//�˳�
	m_btOverTime.SetWindowPos(NULL,316,17,71,36,0);//��ʱ

	m_btGoOn.ShowWindow(SW_SHOW);
	m_btExit.ShowWindow(SW_SHOW);
	m_btChangeTable.ShowWindow(SW_SHOW);
	m_btOverTime.ShowWindow(SW_SHOW);
	m_btOverTime.EnableWindow(TRUE);

	m_rcDrawTimer.left = 180;
	m_rcDrawTimer.top = 275;
	m_rcDrawTimer.right = m_rcDrawTimer.left + 52;
	m_rcDrawTimer.bottom = m_rcDrawTimer.top + 36;
	__LEAVE_FUNCTION
}

//��ʼ����Ϣ��
void CGameScoreView::AlreadyInitInfo()
{
	__ENTER_FUNCTION 
	__LEAVE_FUNCTION
}

//����
void CGameScoreView::DrawScoreInfo(CDC* pDC)
{
	__ENTER_FUNCTION 

	//���� DC
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(250,250,250));
	pDC->SelectObject(CSkinResourceManager::GetDefaultFont());
	//�滭��־
	//CImageHandle ImageHandleWinLose( &m_ImageWinLose );
	//���û���ͼƬ

	/*m_ImageLandWin.DrawImage(pDC,0,0,143,147,143*(m_ActionCount%10),0);*/
	/*SetTimer(1,200,NULL);*/

	//CString strPath = "";
	//GetPicPath(strPath);
	//std::string str="";
	//str.append(strPath.GetBuffer());
	//str.append("ImageNums.png");

	CImageNums	ImageNums;
	ImageNums.SetImageResource("LandCrazy\\picture\\ImageNums.png");
	ImageNums.SetNumAlign(IA_CENTER);
	ImageNums.SetSign(true);
	ImageNums.SetSeparator(false);


	//��ʾ����
	CRect rcDraw;
	TCHAR szBuffer[64]=TEXT("");
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		/*sprintf(m_szUserName[i], "osc_8");
		m_lGameScore[ i ] = 10000;*/

		//�û�����
		CRect rcScore;
		rcDraw.left=185;
		rcDraw.right=242;
		rcDraw.top=86;
		rcDraw.bottom=rcDraw.top+118+20;
        if (m_MeId==i)
        {
			pDC->DrawText(m_szUserName[i],lstrlen(m_szUserName[i]),&rcDraw,DT_LEFT|DT_END_ELLIPSIS);
			CRect rcScore(50,80,500,264);
			ImageNums.SetShowRect(rcScore);
			if ( 0 < m_lGameScore[ i ] )
			{//Ӯ
				m_ImageWin.DrawImage(pDC,325,69,m_ImageWin.GetWidth(),m_ImageWin.GetHeight(),0,0);
			}
			else 
			{//��
				m_ImageLose.DrawImage(pDC,325,69,m_ImageLose.GetWidth(),m_ImageLose.GetHeight(),0,0);

			}	
        }
		else if (((m_MeId+1)%GAME_PLAYER)==i)
		{
			if (m_LandChair==i)
			{//����
				m_ImageLand.DrawImage(pDC,180,185,m_ImageLand.GetWidth(),m_ImageLand.GetHeight(),0,0);
			}
			else
			{//ũ��
				m_ImageFarmer.DrawImage(pDC,180,185,m_ImageFarmer.GetWidth(),m_ImageFarmer.GetHeight(),0,0);
			}
			CRect rcScore(50,192,500,376);
			ImageNums.SetShowRect(rcScore);
			if ( 0 < m_lGameScore[ i ] )
			{//Ӯ
				m_ImageWin.DrawImage(pDC,325,184,m_ImageWin.GetWidth(),m_ImageWin.GetHeight(),0,0);
			}
			else 
			{//��
				if (m_IsUserLeft[i])
				{//ǿ��
					m_ImageUserLeft.DrawImage(pDC,295,160,m_ImageUserLeft.GetWidth(),m_ImageUserLeft.GetHeight(),0,0);
				}
				else
				{
					m_ImageLose.DrawImage(pDC,325,184,m_ImageLose.GetWidth(),m_ImageLose.GetHeight(),0,0);

				}			
			}
		}
		else
		{
			if (m_LandChair==i)
			{//����
				m_ImageLand.DrawImage(pDC,180,129,m_ImageLand.GetWidth(),m_ImageLand.GetHeight(),0,0);
			}
			else
			{//ũ��
				m_ImageFarmer.DrawImage(pDC,180,129,m_ImageFarmer.GetWidth(),m_ImageFarmer.GetHeight(),0,0);
			}
			CRect rcScore(50,136,500,320);
			ImageNums.SetShowRect(rcScore);
			if ( 0 < m_lGameScore[ i ] )
			{//Ӯ
				m_ImageWin.DrawImage(pDC,325,129,m_ImageWin.GetWidth(),m_ImageWin.GetHeight(),0,0);
			}
			else 
			{//��
				if (m_IsUserLeft[i])
				{//ǿ��
					m_ImageUserLeft.DrawImage(pDC,295,110,m_ImageUserLeft.GetWidth(),m_ImageUserLeft.GetHeight(),0,0);
				}
				else
				{
					m_ImageLose.DrawImage(pDC,325,129,m_ImageLose.GetWidth(),m_ImageLose.GetHeight(),0,0);
				}
			}
		}
			
		//����ɼ� 
		//CRect rcScore(125,i*56+80,500,i*56+146+118);
		//ImageNums.SetShowRect(rcScore);
		ImageNums.SetNums(m_lGameScore[i]);
		ImageNums.DrawImageNums(pDC);

		//�滭��ұ���
		if(m_MeId==i)
		{
			INT GrandWith=m_BalanceBei.GetWidth()/12;
			INT GrandHeighe=m_BalanceBei.GetHeight();
			int GrandCount=0;
			int Grand=0;
			Grand=m_UserBeiShu[i];
			int Fan=Grand;
			while(1<=Grand)
			{
				GrandCount++;
				Grand/=10;
			}
			if (1<=GrandCount)
			{
				INT xPos= 286;
				INT yPos=228;
				while (0<GrandCount)
				{
					xPos+=GrandWith;
					--GrandCount;
					INT n_Mutiple=(INT)pow(10,GrandCount);
					INT DrawGrand=(INT)(Fan/n_Mutiple);
					m_BalanceBei.DrawImage(pDC,xPos,yPos,GrandWith,GrandHeighe,DrawGrand*GrandWith,0);
					Fan-=(n_Mutiple*DrawGrand);
				}
			}
		}
		//�滭��ʯ
		if(m_MeId==i)
		{
			INT GrandWith=m_ImageServers.GetWidth()/12;
			INT GrandHeighe=m_ImageServers.GetHeight();
			INT GrandCount=0;
			INT Grand=0;
			Grand= m_EncourageGem[i];
			INT Fan=Grand;
			while(1<=Grand)
			{
				GrandCount++;
				Grand/=10;
			}
			if (1<=GrandCount)
			{
				INT xPos= 145;
				INT yPos=250;
				while (0<GrandCount)
				{
					xPos+=GrandWith;
					--GrandCount;
					INT n_Mutiple=(INT)pow(10,GrandCount);
					INT DrawGrand=(INT)(Fan/n_Mutiple);
					m_ImageServers.DrawImage(pDC,xPos,yPos,GrandWith,GrandHeighe,DrawGrand*GrandWith,0);
					Fan-=(n_Mutiple*DrawGrand);
				}
			}
		}
		//�滭�������
		if(m_MeId==i)
		{
			INT GrandWith=m_BalanceBei.GetWidth()/12;
			INT GrandHeighe=m_BalanceBei.GetHeight();
			INT GrandCount=0;
			INT Grand=0;
			Grand= m_RoomScore[i];
			INT Fan=Grand;
			while(1<=Grand)
			{
				GrandCount++;
				Grand/=10;
			}
			if (1<=GrandCount)
			{
				INT xPos= 145;
				INT yPos=227;
				while (0<GrandCount)
				{
					xPos+=GrandWith;
					--GrandCount;
					INT n_Mutiple=(INT)pow(10,GrandCount);
					INT DrawGrand=(INT)(Fan/n_Mutiple);
					m_BalanceBei.DrawImage(pDC,xPos,yPos,GrandWith,GrandHeighe,DrawGrand*GrandWith,0);
					Fan-=(n_Mutiple*DrawGrand);
				}
			}
		}
		//�滭˰��
		/*for (WORD j=0;j<GAME_PLAYER;j++)
		{*/
			/*LONGLONG ScoreRoom=0;
			if (m_lGameScore[j]>0)
			{
				ScoreRoom=m_lGameTax;
			}
			else
			{
				ScoreRoom=0;
			}*/
			if(m_MeId==i)
			{
				LONGLONG ScoreRoom=0;
				ScoreRoom=m_lGameTax[i];
				INT GrandWith=m_ImageServers.GetWidth()/12;
				INT GrandHeighe=m_ImageServers.GetHeight();
				INT GrandCount=0;
				INT Grand=0;
				Grand= ScoreRoom;
				INT Fan=Grand;
				while(1<=Grand)
				{
					GrandCount++;
					Grand/=10;
				}
				if (1<=GrandCount)
				{
					INT xPos= 308;
					INT yPos=252;
					while (0<GrandCount)
					{
						xPos+=GrandWith;
						--GrandCount;
						INT n_Mutiple=(INT)pow(10,GrandCount);
						INT DrawGrand=(INT)(Fan/n_Mutiple);
						m_ImageServers.DrawImage(pDC,xPos,yPos,GrandWith,GrandHeighe,DrawGrand*GrandWith,0);
						Fan-=(n_Mutiple*DrawGrand);
					}
				}
			}

		/*}*/
		//ʤ��ʧ�ܶ���
		if(m_MeId==i)
		{
			if (m_IsShow==true)
			{
				INT  ImageWidth=m_ImageLandWin.GetWidth()/10;
				INT  ImageHeight=m_ImageLandWin.GetHeight();
				/*m_ImageLandWin.DrawImage(pDC,30,70,ImageWidth,ImageHeight,ImageWidth*(m_ActionCount%10),0);*/
				if (m_lGameScore[i]>0)
				{
					if (m_LandChair==i)
					{
						m_ImageLandWin.DrawImage(pDC,30,70,ImageWidth,ImageHeight,ImageWidth*(m_ActionCount%10),0);
					}
					else
					{
						m_ImageFarmerWin.DrawImage(pDC,30,70,ImageWidth,ImageHeight,ImageWidth*(m_ActionCount%10),0);
					}
				}
				else if(m_lGameScore[i]<0)
				{
					if (m_LandChair==i)
					{
						m_ImageLandLose.DrawImage(pDC,30,70,ImageWidth,ImageHeight,ImageWidth*(m_ActionCount%10),0);
					}
					else
					{
						m_ImageFarmerLose.DrawImage(pDC,30,70,ImageWidth,ImageHeight,ImageWidth*(m_ActionCount%10),0);
					}
				}
			}
		}
	}
	//�滭����ʱ
	if (m_IsTrue==true)
	{
		INT CountDownWith=m_ImageCountDown.GetWidth()/10;
		INT CountDownHeight=m_ImageCountDown.GetHeight();
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
				m_ImageCountDown.DrawImage(pDC, xPos,yPos,CountDownWith,CountDownHeight,iCountDown*CountDownWith,0);
			}
			else
			{
				//ʮλ
				m_ImageCountDown.DrawImage(pDC, xPos,yPos,CountDownWith,CountDownHeight,(iCountDown/10)*CountDownWith,0);
				//��λ
				m_ImageCountDown.DrawImage(pDC, xPos+CountDownWith,yPos,CountDownWith,CountDownHeight,(iCountDown%10)*CountDownWith,0);
			}
		}
	}
	__LEAVE_FUNCTION
}

//���û���
void CGameScoreView::ResetScore()
{
	__ENTER_FUNCTION 

	//��������
	memset(m_lGameTax,0,sizeof(m_lGameTax));
	memset(m_lGameScore,0,sizeof(m_lGameScore));
	memset(m_szUserName,0,sizeof(m_szUserName));

	//�滭��ͼ
	if (m_hWnd!=NULL) Invalidate(NULL);

	__LEAVE_FUNCTION
}

//����˰��
void CGameScoreView::SetGameTax(WORD wChairID,LONGLONG lGameTax)
{
	__ENTER_FUNCTION 

	//���ñ���
		m_lGameTax[wChairID]=lGameTax;
		if (m_hWnd!=NULL) Invalidate(FALSE);
	__LEAVE_FUNCTION
}
//���ñ�ʯ
void CGameScoreView::SetGem(WORD wChairID, int iGem)
{
	__ENTER_FUNCTION 

	m_EncourageGem[wChairID]=iGem;
	if(m_hWnd!=NULL) Invalidate(FALSE);
	__LEAVE_FUNCTION
}
//����ǿ��
void CGameScoreView::SetUserLeft(WORD wchairID,bool IsLeft)
{
	__ENTER_FUNCTION 

	m_IsUserLeft[wchairID]=IsLeft;
    if(m_hWnd!=NULL) Invalidate(FALSE);
	__LEAVE_FUNCTION
}
//
void CGameScoreView::GetMeID(WORD MeChair,WORD landchair)
{
	__ENTER_FUNCTION 

	m_MeId=MeChair;
	m_LandChair=landchair;
	__LEAVE_FUNCTION
}

//���û���
void CGameScoreView::SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONGLONG lScore,int UserBeiShu,LONGLONG RoomScore)
{
	__ENTER_FUNCTION 

	//���ñ���
	if (wChairID<GAME_PLAYER)
	{
		m_lGameScore[wChairID]=lScore;
		lstrcpyn(m_szUserName[wChairID],pszUserName,CountArray(m_szUserName[wChairID]));
	    m_UserBeiShu[wChairID]=UserBeiShu;
	    m_RoomScore[wChairID]=RoomScore;
		if (m_hWnd!=NULL) Invalidate(FALSE);
	}

	__LEAVE_FUNCTION
}
void CGameScoreView::GetPicPath(CString& strPath)
{
	__ENTER_FUNCTION 

	strPath = "";
	TCHAR szResourcePath[MAX_PATH]={0};
	GetCurrentDirectory(MAX_PATH,szResourcePath);
	strPath.Append(szResourcePath);
	strPath.Append("\\LandCrazy\\picture\\");
	__LEAVE_FUNCTION
}

//��ť��Ϣ
void CGameScoreView::OnChangeTable()
{
	__ENTER_FUNCTION 

	SetCountDown(false);
	OnStartGame();
	__LEAVE_FUNCTION
}
void CGameScoreView::OnExit()
{
	__ENTER_FUNCTION 

	OnGoExit();
	__LEAVE_FUNCTION
}
void CGameScoreView::OnGoOn()
{
	__ENTER_FUNCTION 

	SetCountDown(false);
	OnStartGame();
	__LEAVE_FUNCTION
}
void CGameScoreView::OnOverTime()
{
	__ENTER_FUNCTION 

	m_SecondCount += 10;
	m_btOverTime.EnableWindow(FALSE);
	//InvalidateRect(&m_rcDrawTimer,FALSE);
	SafeInvalidate(m_rcDrawTimer,FALSE);
	__LEAVE_FUNCTION
}

//��Ӯ����
void CGameScoreView::GetWinLose(bool Istrue)
{
	__ENTER_FUNCTION 

    if(Istrue)
	{
       m_IsShow=true;
	   m_ActionCount=0;
	   SetTimer(1,200,NULL);
	}
	else
	{	
		if (m_IsShow==true)
		{
			//ɾ��ʱ��
			KillTimer(1);

			//���ñ���
			m_IsShow=false;
			m_ActionCount=0;
		}
		//InvalidateRect(&CRect(30,70,173,217),FALSE);
        SafeInvalidate(CRect(30,70,173,217),FALSE);
	}
	__LEAVE_FUNCTION
}
//����ʱ����
void CGameScoreView::SetCountDown(bool Istrue)
{
	__ENTER_FUNCTION 

	if (Istrue)
	{
		m_btGoOn.ShowWindow(SW_SHOW);
		m_btExit.ShowWindow(SW_SHOW);
		m_btChangeTable.ShowWindow(SW_SHOW);
		m_btOverTime.ShowWindow(SW_SHOW);
		m_btOverTime.EnableWindow(TRUE);
		m_IsTrue=true;
		m_SecondCount=10;
		SetTimer(IDT_COUNTDOWN,1000,NULL);
	}
	else
	{
		if (m_IsTrue=true)
		{
			KillTimer(IDT_COUNTDOWN);
			m_IsTrue=false;
			m_SecondCount=10;
		}
		//InvalidateRect(&m_rcDrawTimer,FALSE);
        SafeInvalidate(m_rcDrawTimer,FALSE); 
	}
	__LEAVE_FUNCTION
}
void CGameScoreView::OnTimer(UINT nIDEvent)
{
	__ENTER_FUNCTION 

	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (1==nIDEvent)
	{
		if(m_IsShow==false)
		{
			KillTimer(1);
			return;
		}
		if((m_ActionCount+1)>=10) 
		{
			//KillTimer(1);
			//m_IsShow=false;
			m_ActionCount=0;
		}
		else
		{
			m_ActionCount++;
		}
	    //InvalidateRect(&CRect(30,70,173,217),FALSE);
		SafeInvalidate(CRect(30,70,173,217),FALSE); 
	}
	else if (IDT_COUNTDOWN==nIDEvent)
	{
		if (m_IsTrue==false)
		{
			KillTimer(IDT_COUNTDOWN);
			return;
		}
		if ((m_SecondCount)<=0)
	    {
			KillTimer(IDT_COUNTDOWN);
			m_IsTrue=false;
			OnGoExit();
		}
		else
		{
			m_SecondCount--;
		}
	SafeInvalidate(m_rcDrawTimer,FALSE);
	//InvalidateRect(&m_rcDrawTimer,FALSE);
			
	}
	CScoreView::OnTimer(nIDEvent);
	__LEAVE_FUNCTION
}

void CGameScoreView::SafeInvalidate(CRect& rect, BOOL bErase)
{
	__ENTER_FUNCTION 
	
	if (GetSafeHwnd())
	{
		InvalidateRect(rect,bErase);
	}
	__LEAVE_FUNCTION
}