#include "StdAfx.h"
#include "GameScoreView.h"
#include "ImageNums.h"

//////////////////////////////////////////////////////////////////////////

//BEGIN_MESSAGE_MAP(CScoreView, CWnd)
//	ON_WM_PAINT()
//	ON_WM_CREATE()
//	ON_WM_LBUTTONDOWN()
//	ON_WM_ERASEBKGND()
//	ON_WM_MOVE()
//END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameScoreView::CGameScoreView()
{
	//��������
	ZeroMemory(m_lTax,sizeof(m_lTax));
	ZeroMemory(m_lScore,sizeof(m_lScore));
	ZeroMemory(m_szUserName,sizeof(m_szUserName));

	//������Դ
	m_ImageWinLose.LoadImage(AfxGetInstanceHandle(),"WIN_LOSE");

	return;
}

//��������
CGameScoreView::~CGameScoreView()
{
}

//��ʼ����Ϣ
void CGameScoreView::InitScoreViewInfo()
{
	LoadBackImage(IDB_SCORE_VIEW);
}

//��ʼ����Ϣ��
void CGameScoreView::AlreadyInitInfo()
{
	SetScoreUI(1);
	//m_BTStartGame.SetWindowPos(NULL, 190, 290, 160, 46, 0);   //������Ϸ
	//m_BTGoPlaza.SetWindowPos(NULL, 190, 290, 160, 46, 0);     //���ش���
	//m_BTGoRoom.SetWindowPos(NULL, 190, 290, 160, 46, 0);     //���ط���

	m_BTGoPlaza.SetWindowPos(NULL, 24, 270, 126, 38, 0);
	m_BTStartGame.SetWindowPos(NULL, 129, 265, 160, 46, 0);
	m_BTGoRoom.SetWindowPos(NULL, 260, 270, 126, 38, 0);

}

////������Ϣ
//int CScoreView::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (__super::OnCreate(lpCreateStruct)==-1) return -1;
//
//	//���ñ���
//	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);
//
//	//�ƶ�����
//	CImageHandle ImageHandle(&m_ImageBack);
//	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);
//
//	if (ImageHandle.IsResourceValid())
//	{
//		//������ʱͼ
//		CSkinImage BufferImage;
//		int nImageWidth=m_ImageBack.GetWidth();
//		int nImageHeight=m_ImageBack.GetHeight();
//		BufferImage.Create(nImageWidth,nImageHeight,32);
//		ImageHandle->BitBlt(BufferImage.GetDC(),0,0,nImageWidth,nImageHeight,0,0);
//		BufferImage.ReleaseDC();
//
//		//��������
//		CRgn ImageRgn;
//		BufferImage.CreateImageRegion( ImageRgn, RGB( 255, 0, 255 ) );
//		if ( (( HRGN )ImageRgn) !=NULL)
//		{
//			SetWindowPos(NULL,0,0,nImageWidth,nImageHeight,SWP_NOMOVE);
//			SetWindowRgn(( HRGN )ImageRgn,TRUE);
// 
//			ImageRgn.DeleteObject();
//
//			return true;
//		}
//	}
//
//	return 0;
//}
//
////�����Ϣ
//void CScoreView::OnLButtonDown(UINT nFlags, CPoint Point)
//{
//	__super::OnLButtonDown(nFlags,Point);
//
//	//��Ϣģ��
//	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
//
//	return;
//}

////�ػ�����
//void CGameScoreView::OnPaint() 

//����
//void CGameScoreView::DrawScoreInfo(CDC* pDC)
void CGameScoreView::DrawScoreInfo(CDC* pDC)
{
  //#if 0
	//CPaintDC dc(this); 

	//���� DC
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(250,255,250));

	//��ʾ��Ϣ
	TCHAR szBuffer[128]=TEXT("");

	//��ʾ����
	//CImageHandle HandleWinLose(&m_ImageWinLose);
	INT nWinLoseWidth = m_ImageWinLose.GetWidth()/2;
	INT nWinLostHeight = m_ImageWinLose.GetHeight();
	WORD wCount = 0;

	CImageNums	ImageNums;
	ImageNums.SetSeparatorWH(17, 25);
	ImageNums.SetImageResource("IMAGE_IMAGENUMS");
	//ImageNums.SetNumAlign(IA_LEFT);
	ImageNums.SetSign(true);
	ImageNums.SetSeparator(false);

	for (WORD i=0;i<CountArray(m_szUserName);i++)
	{
		//sprintf(m_szUserName[i], "abcdefg");
		//m_lScore[i] = 1000000;

		if (m_szUserName[i][0]!=0)
		{
            //if ('0'==m_szUserName[i][0] || 0==m_lScore[i]) continue;
			//�������
			INT nScPos = 80;
			CRect rcName(-5,wCount*30+nScPos-5,140,(wCount+1)*29+nScPos+50);
			pDC->DrawText(m_szUserName[i],lstrlen(m_szUserName[i]),&rcName,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			//����ɼ�
			//CRect rcScore(95+75,wCount*38+nScPos+120,295,(wCount+1)*29+nScPos);
			//_snprintf(szBuffer,sizeof(szBuffer),TEXT("%d"),m_lScore[i]);
			//pDC->DrawText(szBuffer,lstrlen(szBuffer),&rcScore,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			CRect rcScore(140,wCount*30+100,300,wCount*30+200);
			ImageNums.SetShowRect(rcScore);
			ImageNums.SetNums(m_lScore[i]);
			ImageNums.DrawImageNums(pDC);


			//ʤ����־
			m_ImageWinLose.DrawImage(pDC,315-nWinLoseWidth/2,wCount*30+nScPos+33-nWinLostHeight/2,nWinLoseWidth,nWinLostHeight,
				(m_lScore[i]>0)?0:nWinLoseWidth,0);

			////���˰��
			//CRect rcTax(235+170,wCount*38+nScPos+120,335+90+80,(wCount+1)*29+nScPos);
			//_snprintf(szBuffer,sizeof(szBuffer),TEXT("%d"),m_lTax[i]);
			//pDC->DrawText(szBuffer,lstrlen(szBuffer),&rcTax,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			wCount++;
		}
		
	}

	return;
//#endif
}

//���û���
void CGameScoreView::SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONGLONG lScore)
{
	if (wChairID<CountArray(m_lScore))
	{
		m_lScore[wChairID]=lScore;
		lstrcpyn(m_szUserName[wChairID],pszUserName,CountArray(m_szUserName[wChairID]));
		Invalidate(FALSE);

		//��ʾ����
		Invalidate(NULL);
		ShowWindow(SW_SHOW);
	}
	return;
}

//����˰��
void CGameScoreView::SetGameTax(LONGLONG lTax,WORD wChairID)
{
	m_lTax[wChairID]=lTax;

	return;
}

//��������
void CGameScoreView::ResetScore()
{
	//��������
	ZeroMemory(m_lTax,sizeof(m_lTax));
	ZeroMemory(m_lScore,sizeof(m_lScore));
	ZeroMemory(m_szUserName,sizeof(m_szUserName));

	return;
}

//////////////////////////////////////////////////////////////////////////

//BOOL CScoreView::OnEraseBkgnd(CDC* pDC)
//{
//	//
//	//CRect rt;
//	//GetClientRect(&rt);
//	//CRgn WndRgn;
//	//WndRgn.CreateRoundRectRgn(rt.left,rt.top,rt.right+1,rt.bottom+1,10,10);
//	//SetWindowRgn(WndRgn,TRUE);
//	//WndRgn.DeleteObject();
//	return TRUE;
//	//return CWnd::OnEraseBkgnd(pDC);
//}

//void CScoreView::OnMove(int x, int y)
//{
//	CWnd::OnMove(x, y);
//
//	InvalidateRect(NULL,FALSE);
//}
