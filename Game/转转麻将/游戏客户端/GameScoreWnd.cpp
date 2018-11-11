#include "StdAfx.h"
#include "CardControl.h"
#include "GameScoreWnd.h"
#include "scoreview.h"
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
	strPath.Append("\\SparrowZZ\\Picture\\PNG\\");
	m_Direction.LoadImage(TEXT(strPath+"DIRECTION.png"));
	m_ImageLeft.LoadImage(TEXT(strPath+"LEFT.png"));
	m_ImageNature.LoadImage(TEXT(strPath + "Nature.png"));
	m_ImageFangPao.LoadImage(TEXT(strPath + "FangPao.png"));
	m_ImageOneMuscle.LoadImage(TEXT(strPath + "Muscle.png"));
	m_ImageNumDown.LoadImage(TEXT(strPath+"NUMBERDOWN.png"));

	m_NFanNum.SetImageResource(TEXT(strPath+"FanNum.png"));
	m_NFanNum.SetSeparatorWH(5, 17);
	m_NFanNum.SetNumAlign(IA_CENTER);
	m_NFanNum.SetSign(true);

	m_NScore.SetImageResource(TEXT(strPath+"ScoreNum.png"));
	m_NScore.SetSeparatorWH(5, 17);
	m_NScore.SetSign(true);
	m_NScore.SetNumAlign(IA_CENTER);
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
	m_btChangeTable.SetWindowPos(NULL, 138-33, 295+55, 75, 35, 0);//����
	m_btGoOn.SetWindowPos(NULL,265+74, 295+56, 75, 35, 0);//����
	m_btOverTime.SetWindowPos(NULL,195+132,9,72,28,0);//��ʱ
	m_btExit.SetWindowPos(NULL, 268+142, 9, 72, 28, 0);//�˳�


	m_btGoOn.ShowWindow(SW_SHOW);
	m_btExit.ShowWindow(SW_SHOW);
	m_btChangeTable.ShowWindow(SW_SHOW);
	m_btOverTime.ShowWindow(SW_SHOW);

	m_rcDrawTimer.left = 210+15;
	m_rcDrawTimer.top = 295+55;
	m_rcDrawTimer.right = m_rcDrawTimer.left + 52;
	m_rcDrawTimer.bottom = m_rcDrawTimer.top + 36;
}

//��ʼ����Ϣ��
void CGameScoreWnd::AlreadyInitInfo()
{
	//SetScoreUI(0);
	//m_BTGoPlaza.SetWindowPos(NULL, 24, 325, 126, 38, 0);
	//m_BTStartGame.SetWindowPos(NULL, 129, 320, 160, 46, 0);
	//m_BTGoRoom.SetWindowPos(NULL, 260, 325, 126, 38, 0);
}


//����
void CGameScoreWnd::DrawScoreInfo(CDC* pDC)
{
	//����DC
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(230,230,230));
	pDC->SelectObject(CSkinResourceManager::GetDefaultFont());

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	int nWidth = m_Direction.GetWidth();
	int nHeight = m_Direction.GetHeight()/12;
	int iPosx=0, iPosY=0;
	int iColvVD=59;

	CString strBuffer;
	CFont DrawFont;	
	CFont * pOldFont = NULL;
	COLORREF oldColor;
	DrawFont.CreateFont(-18,0,0,0,FW_BOLD,0,0,0,134,3,2,1,2,TEXT("΢���ź�"));
	pOldFont = pDC->SelectObject(&DrawFont);
	oldColor = pDC->SetTextColor(RGB(214,117,0));


	for (BYTE i=0,bIndex=0, bTmp=0; i<GAME_PLAYER; i++)
	{
		bIndex = bTmp;

		// ��λ
		if (m_MeChairId == i)
		{
			DrawFont.DeleteObject();
			DrawFont.CreateFont(-24,0,0,0,FW_BOLD,0,0,0,134,3,2,1,2,TEXT("΢���ź�"));
			pOldFont = pDC->SelectObject(&DrawFont);
			pDC->SetTextColor(RGB(214,117,0));

			bIndex = GAME_PLAYER - 1;

			strBuffer.Empty();
			strBuffer.Format(TEXT("%s"), m_GameScoreInfo.szUserName[i]);
			pDC->DrawText(strBuffer, &CRect(10,255,10+100,255+55), DT_SINGLELINE|DT_VCENTER|DT_LEFT);
			DrawFont.DeleteObject();

			DrawFont.CreateFont(-18,0,0,0,FW_BOLD,0,0,0,134,3,2,1,2,TEXT("΢���ź�"));
			pOldFont = pDC->SelectObject(&DrawFont);
			oldColor = pDC->SetTextColor(RGB(214,117,0));
		}
		else
		{
			WORD wIndex=((i+GAME_PLAYER)-m_BankerChairID)%GAME_PLAYER;
			m_Direction.DrawImage(pDC, 18, 94+bIndex*iColvVD, nWidth, nHeight, 0, 3*wIndex*nHeight, nWidth, nHeight);
		}

		// ��������
		if (m_IsUserLeft[i]) //����
		{
			m_ImageLeft.DrawImage(pDC, 100, 80+bIndex*(iColvVD-5));
		}
		else if (1 == m_GameScoreInfo.iChiHuKind[i]) // ����
		{
			m_ImageNature.DrawImage(pDC,  125, 80+bIndex*(iColvVD-2), m_ImageNature.GetWidth()/2, m_ImageNature.GetHeight(), 
				0, 0, m_ImageNature.GetWidth()/2, m_ImageNature.GetHeight());
		}
		else if (2 == m_GameScoreInfo.iChiHuKind[i])	// ����
		{
			m_ImageNature.DrawImage(pDC,  125, 80+bIndex*(iColvVD-2), m_ImageNature.GetWidth()/2, m_ImageNature.GetHeight(), 
				m_ImageNature.GetWidth()/2, 0, 	m_ImageNature.GetWidth()/2, m_ImageNature.GetHeight());
		}
		else if (3 == m_GameScoreInfo.iChiHuKind[i] ) //����
		{
			m_ImageFangPao.DrawImage(pDC, 105, 80+bIndex*iColvVD);
		}

		//���
		if (0 < m_GameScoreInfo.iMuscle[i])
		{
			m_ImageOneMuscle.DrawImage(pDC, 235, 95+bIndex*iColvVD, m_ImageOneMuscle.GetWidth(), m_ImageOneMuscle.GetHeight()/10, 
				0, m_ImageOneMuscle.GetHeight()/10 * (m_GameScoreInfo.iMuscle[i]-1), m_ImageOneMuscle.GetWidth(), m_ImageOneMuscle.GetHeight()/10);
		}

		// ����
		m_NFanNum.SetShowRect(CRect(340, 95+bIndex*iColvVD, 340+50, (95+25)+bIndex*iColvVD));
		m_NFanNum.SetNums(m_GameScoreInfo.cbTimes[i]);
		m_NFanNum.DrawImageNums(pDC);

		// ����
		m_NScore.SetShowRect(CRect(415, 95+bIndex*iColvVD, 415+70, (95+25)+bIndex*iColvVD));
		m_NScore.SetNums(m_GameScoreInfo.lScore[i]);
		m_NScore.DrawImageNums(pDC);

		// �׷�
		strBuffer.Empty();
		strBuffer.Format(TEXT("%I64d"), m_GameScoreInfo.iBasicScore);
		pDC->DrawText(strBuffer, &CRect(110,313,110+70,313+25), DT_VCENTER|DT_SINGLELINE|DT_CENTER);

		// ˰��
		strBuffer.Empty();
		strBuffer.Format(TEXT("%d"), m_GameScoreInfo.iCellScore);
		pDC->DrawText(strBuffer, &CRect(365,313,365+50,313+25), DT_VCENTER|DT_SINGLELINE|DT_CENTER);

		if (m_MeChairId != i)
		{
			bTmp++;
		}
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

	pDC->SetTextColor(oldColor);
	pDC->SelectObject(pOldFont);
	DrawFont.DeleteObject();

	return;
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
	strPath.Append("\\SparrowZZ\\Picture\\PNG\\");
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
	if (GetSafeHwnd())
	{
		InvalidateRect(&m_rcDrawTimer,FALSE);
	}
}


void CGameScoreWnd::GetViewID(WORD MyChairID, WORD BankerChairID)
{
	m_MeChairId = MyChairID;
	m_BankerChairID = BankerChairID;
	return;
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
	m_btOverTime.EnableWindow(TRUE);
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
		if (GetSafeHwnd())
		{
			InvalidateRect(&m_rcDrawTimer,FALSE);
		}
	}
}

void CGameScoreWnd::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (IDT_COUNTDOWN==nIDEvent)
	{
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

		if(GetSafeHwnd())
		{
			InvalidateRect(&m_rcDrawTimer,FALSE);
		}

	}

	CScoreView::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
