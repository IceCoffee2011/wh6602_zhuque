// UserDaoJu.cpp : ʵ���ļ�
//

#include "StdAfx.h"
#include "resource.h"
#include "GlobalOption.h"
#include "UserInfoView.h"
#include "GameFrameControl.h"
#include "UserDaoJu.h"
#include ".\userdaoju.h"
#include "GameClientDJInfo.h"
#include "ShowDJInfo.h"
#include "GameFrameNetDefine.h"


#define		IDI_TIME_TASK_BT_DEF		1023
#define		ID_TIME_TASK_INTERVAL		1000

//���Ƶ��ߵ���Ϣ
#define		DJ_ICO_BACK_WIDTH			24
#define		DJ_ICO_BACK_HEIGHT			24

#define		DJ_ICO_WIDTH				20
#define		DJ_ICO_HEIGHT				20

#define		DRAW_DJ_X					14
#define		DRAW_DJ_Y					1

#define		DRAW_DJ_X_ABS				2
#define		DRAW_DJ_Y_ABS				2

#define		DRAW_DJ_INTERVAL			3

#define		DRAW_DJ_NUM_X				1
#define		DRAW_DJ_NUM_Y				8

#define		DJ_SHOW_PAGE_NUMS			7


// CUserDaoJu

IMPLEMENT_DYNAMIC(CUserDaoJu, CWnd)
CUserDaoJu::CUserDaoJu(CGameFrameDlg * pGameFrameDlg):m_pGameFrameDlg(pGameFrameDlg)
{
	m_bIsTimer = false;
	m_bHovering=false;
	m_iTotalPage = 0;
	m_iCurrentPage = 0;
	m_pShowDJInfo = NULL;
}

CUserDaoJu::~CUserDaoJu()
{
}


BEGIN_MESSAGE_MAP(CUserDaoJu, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BT_USERDJ_LEFT, OnBnLeftClickedEvent)
	ON_BN_CLICKED(IDC_BT_USERDJ_RIGHT, OnBnRightClickedEvent)
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)

	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
//	ON_WM_NCMOUSEMOVE()
ON_WM_LBUTTONDOWN()
ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()



//�滭����
VOID CUserDaoJu::OnPaint()
{
	if(m_ImageWndBack.IsNull())
	{
		return;
	}
	if (m_DaoJuImageBuffer.IsNull())
	{
		return;
	}

	//��������
	CPaintDC dc(this);
	CDC * pDC=CDC::FromHandle(m_DaoJuImageBuffer.GetDC());

	int iWndWidth = m_ImageWndBack.GetWidth();
	int iWndHeight = m_ImageWndBack.GetHeight();

	//��������
	CFont DrawFont;
	DrawFont.CreateFont(-13,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("����"));
	CFont * pOldFont=pDC->SelectObject(&DrawFont);

	pDC->SetBkMode(TRANSPARENT);
	pDC->FillSolidRect(0,0,iWndWidth,iWndHeight,RGB(0,0,0));

	//�滭����ͼƬ
	m_ImageWndBack.DrawImage(pDC, 0, 0);

	int iCountIndex = 0;
	if ((m_iTotalPage > 0) && (NULL != m_pUserDJInfo))
	{
		//�е���
		//���Ƶ�����Ϣ
		for (int i=m_iCurrentPage*DJ_SHOW_PAGE_NUMS,k=0; i<m_pUserDJInfo->m_vecUserHaveDJList.size()&&k<DJ_SHOW_PAGE_NUMS;i++,k++)
		{
			{
				//���Ƶ���
				//���Ƶ���ͼƬ
				CPngImage* pDJImage = m_pUserDJInfo->GetImageFromeDJID(m_pUserDJInfo->m_vecUserHaveDJList[i].idjID);
				if (NULL != pDJImage && !pDJImage->IsNull())
				{
					pDJImage->DrawImage(pDC, DRAW_DJ_X+DRAW_DJ_X_ABS+iCountIndex*(DJ_ICO_BACK_WIDTH+DRAW_DJ_INTERVAL), DRAW_DJ_Y+DRAW_DJ_Y_ABS);

					//���������Ӧ
					if (m_iCurrentMousePoint == k)
					{
						pDJImage = NULL;
						pDJImage = m_pUserDJInfo->GetLightImageFromeDJID(m_pUserDJInfo->m_vecUserHaveDJList[i].idjID);
						if (NULL != pDJImage && !pDJImage->IsNull())
						{
							pDJImage->DrawImage(pDC, DRAW_DJ_X+DRAW_DJ_X_ABS+iCountIndex*(DJ_ICO_BACK_WIDTH+DRAW_DJ_INTERVAL), DRAW_DJ_Y+DRAW_DJ_Y_ABS);
						}
					}
				}
				if (m_pUserDJInfo->m_vecUserHaveDJList[i].idjNums>0)
				{
					//��������
					pDC->SetTextColor(RGB(255, 255, 255));
					char chNumData[10]={0};
					sprintf(chNumData, "%d", m_pUserDJInfo->m_vecUserHaveDJList[i].idjNums);

					RECT textrect;
					textrect.left = DRAW_DJ_X+DRAW_DJ_X_ABS+iCountIndex*(DJ_ICO_BACK_WIDTH+DRAW_DJ_INTERVAL)+DRAW_DJ_NUM_X;
					textrect.top = DRAW_DJ_Y+DRAW_DJ_Y_ABS+DRAW_DJ_NUM_Y;
					textrect.right = textrect.left + 8;
					textrect.bottom = textrect.top + 20;
					DrawTextString(pDC, chNumData, RGB(255, 255, 255), RGB(0, 0, 0), textrect.left, textrect.top);
				}
				++iCountIndex;
			}
		}
	}
	
	//�滭����
	m_DaoJuImageBuffer.BitBlt(dc,0,0,iWndWidth,iWndHeight,0,0);

	//������Դ
	pDC->SelectObject(pOldFont);

	m_DaoJuImageBuffer.ReleaseDC();
	DrawFont.DeleteObject();

	ReleaseDC(pDC);			// add by HouGuoJiang 2011-11-25
	CDC::DeleteTempMap();	// add by HouGuoJiang 2011-11-25
}


//������Ϣ
INT CUserDaoJu::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	TCHAR szResourcePath[MAX_PATH];	GetCurrentDirectory(MAX_PATH,szResourcePath);
	CString strPath(szResourcePath);
	strPath.Append("\\data\\gameframe\\plaza\\");


	//������Դ
	m_DaoJuImageBuffer.Destroy();
	m_DaoJuImageBuffer.Create(214, 26, 24);

	m_ImageWndBack.LoadImage(TEXT(strPath + "DaoJuBack.png"));

	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);

	//�����ؼ�
	CRect CreateRect(0,0,0,0);

	//������ť
	m_BTLeft.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CreateRect,this,IDC_BT_USERDJ_LEFT);
	m_BTRight.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CreateRect,this,IDC_BT_USERDJ_RIGHT);

	//���ð�ť
	m_BTLeft.SetButtonImage(TEXT(strPath + "UserDJL.png"), false, 4);
	m_BTRight.SetButtonImage(TEXT(strPath + "UserDJR.png"), false, 4);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	m_BTLeft.SetWindowPos(NULL, 1, 0, 12, 26, 0);//��
	m_BTRight.SetWindowPos(NULL, 201, 0, 12, 26, 0);//��

	if(GetDlgItem(IDC_BT_USERDJ_ACTIVE))
		m_ToolTipCtrl.AddTool(GetDlgItem(IDC_BT_USERDJ_ACTIVE),TEXT("����"));
	
	m_BTLeft.ShowWindow(SW_SHOW);
	m_BTRight.ShowWindow(SW_SHOW);

	//Ĭ��û�е���
	m_iTotalPage = 0;
	m_iCurrentPage = 0;
	m_BTLeft.EnableWindow(FALSE);
	m_BTRight.EnableWindow(FALSE);

	//��õ�����Ϣ
	m_pUserDJInfo = CGameClientDJInfo::GetDJInfoInstance();

	SetImageRgn(m_hWnd, &m_ImageWndBack, SIRT_FULL_DOT_LESS);
	return 0;
}

//
//BOOL CUserDaoJu::SetImageRgn(CPngImage * pImage)
//{
//	if (NULL==pImage) return FALSE;
//	if (pImage->IsNull()) return FALSE;
//	long wWidth = pImage->GetWidth();
//	long wHeight = pImage->GetHeight();
//
//	CRgn rgnImage, tempRgn;
//	rgnImage.CreateRectRgn(0, 0, 0, 0);
//
//	int leftX = 0;
//	int rightX = 0;
//	for (int i=wHeight-1; i>=0; --i)
//	{
//		for (int j = 0; j<wWidth; ++j)
//		{
//			leftX = j;
//			rightX = j;
//			for (int k=j; k<wWidth; ++k)  //��ס��ߵ�x
//			{
//				BYTE byValue = pImage->GetAlphaValue(k, i);
//				if (byValue != 0)
//				{
//					leftX = k;
//					break;
//				}
//			}
//			for (int k=leftX+1; k<wWidth; ++k)
//			{
//				BYTE byValue = pImage->GetAlphaValue(k, i);
//				if (byValue == 0 || (k == wWidth -1 && byValue!=0))
//				{
//					rightX = k;
//					break;
//				}
//			}
//
//			j = leftX > rightX ? leftX : rightX;
//
//			if (rightX > leftX)
//			{
//				tempRgn.CreateRectRgn(leftX, (wHeight - i-1) , rightX, (wHeight -i));
//				rgnImage.CombineRgn(&rgnImage, &tempRgn, RGN_OR);
//				tempRgn.DeleteObject();
//			}
//		}
//	}
//
//	if (NULL == rgnImage.m_hObject)
//		return FALSE;
//
//	SetWindowRgn((HRGN)rgnImage.m_hObject, TRUE);
//	rgnImage.DeleteObject();
//	return TRUE;
//}


//��ť�¼�
void CUserDaoJu::OnBnLeftClickedEvent()
{//����
	if (m_iCurrentPage>0)
	{
		--m_iCurrentPage;
		CRect DrawRect(DRAW_DJ_X, DRAW_DJ_Y, DRAW_DJ_X+DJ_SHOW_PAGE_NUMS*(DJ_ICO_BACK_WIDTH+DRAW_DJ_INTERVAL), DRAW_DJ_Y+DJ_ICO_BACK_HEIGHT);
		InvalidateRect(DrawRect);
		if (m_iCurrentPage <= 0)
		{
			m_BTLeft.EnableWindow(FALSE);
		}
		if (m_iTotalPage > 0)
		{
			m_BTRight.EnableWindow(TRUE);
		}
	}
}

void CUserDaoJu::OnBnRightClickedEvent()
{//����
	if (m_iCurrentPage<m_iTotalPage-1)
	{
		++m_iCurrentPage;
		CRect DrawRect(DRAW_DJ_X, DRAW_DJ_Y, DRAW_DJ_X+DJ_SHOW_PAGE_NUMS*(DJ_ICO_BACK_WIDTH+DRAW_DJ_INTERVAL), DRAW_DJ_Y+DJ_ICO_BACK_HEIGHT);
		InvalidateRect(DrawRect);

		if (m_iCurrentPage==m_iTotalPage-1)
		{
			m_BTRight.EnableWindow(FALSE);
		}
		if (m_iTotalPage > 0)
		{
			m_BTLeft.EnableWindow(TRUE);
		}
	}
}

//������ʾ������Ϣָ��
void CUserDaoJu::SetShowDJInfo(CShowDJInfo* pShowDJInfoPtr)
{
	if (NULL != pShowDJInfoPtr)
	{
		m_pShowDJInfo = pShowDJInfoPtr;
	}
}

// CUserDaoJu ��Ϣ�������}
void CUserDaoJu::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CWnd::OnTimer(nIDEvent);
}


//��������
void CUserDaoJu::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
	//��������
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//�滭�߿�
	pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//�滭����
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	return;
}

void CUserDaoJu::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	m_iCurrentMousePoint = -1;
	CRect DrawRect(DRAW_DJ_X, DRAW_DJ_Y, DRAW_DJ_X+DJ_SHOW_PAGE_NUMS*(DJ_ICO_BACK_WIDTH+DRAW_DJ_INTERVAL), DRAW_DJ_Y+DJ_ICO_BACK_HEIGHT);
	if (TRUE == DrawRect.PtInRect(point))
	{//������֮��
		m_iCurrentMousePoint = (point.x-DRAW_DJ_X)/(DRAW_DJ_INTERVAL+DJ_ICO_BACK_WIDTH);
		if (m_iCurrentMousePoint>=0 && m_iCurrentMousePoint<DJ_SHOW_PAGE_NUMS)
		{
			if (NULL == m_pUserDJInfo) return;
			if (m_iCurrentPage*DJ_SHOW_PAGE_NUMS+m_iCurrentMousePoint < m_pUserDJInfo->m_vecUserHaveDJList.size())
			{
				if (NULL != m_pShowDJInfo)
				{
					m_pShowDJInfo->SetShowDJDiscription(m_iCurrentPage*DJ_SHOW_PAGE_NUMS+m_iCurrentMousePoint);
					m_pShowDJInfo->SetWndShow(true, m_iCurrentMousePoint);
				}
			}
			else
			{
				m_iCurrentMousePoint - 1;
				if (NULL != m_pShowDJInfo)
				{
					m_pShowDJInfo->SetWndShow(false);
				}
			}
			InvalidateRect(&DrawRect);
		}
	}
	else
	{
		if (NULL != m_pShowDJInfo)
		{
			m_pShowDJInfo->SetWndShow(false);
		}
	}
	
	if (m_bHovering==false)
	{
		//ע����Ϣ
		m_bHovering=true;
		Invalidate(FALSE);
		TRACKMOUSEEVENT TrackMouseEvent;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.hwndTrack=GetSafeHwnd();
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		_TrackMouseEvent(&TrackMouseEvent);
	}
	
	CWnd::OnMouseMove(nFlags, point);
}


void CUserDaoJu::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (NULL != m_pGameFrameDlg)
	{
		if (m_pGameFrameDlg->IsLookonMode())
		{
			return;
		}
	}
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//CRect DrawRect(DRAW_DJ_X, DRAW_DJ_Y, DRAW_DJ_X+DJ_SHOW_PAGE_NUMS*(DJ_ICO_BACK_WIDTH+DRAW_DJ_INTERVAL), DRAW_DJ_Y+DJ_ICO_BACK_HEIGHT);
	//if (TRUE == DrawRect.PtInRect(point))
	//{//������֮��
	//	if (m_iCurrentMousePoint>=0 && m_iCurrentMousePoint<DJ_SHOW_PAGE_NUMS)
	//	{//ʹ�ô˵���
	//		if (NULL == m_pUserDJInfo) return;
	//		if (m_iCurrentPage*DJ_SHOW_PAGE_NUMS+m_iCurrentMousePoint < m_pUserDJInfo->m_vecUserHaveDJList.size())
	//		{/*
	//			char chIndexInfo[100]={0};
	//			sprintf(chIndexInfo, "��ȷ��Ҫʹ�õ���:%d(Index:%d)��", m_iCurrentPage*DJ_SHOW_PAGE_NUMS+m_iCurrentMousePoint, m_iCurrentMousePoint);
	//			MessageBox(chIndexInfo);*/
	//			if (NULL != m_pGameFrameDlg)
	//			{
	//				int iSaveDJID = m_pUserDJInfo->GetDJIDFromDJIndex(m_iCurrentPage*DJ_SHOW_PAGE_NUMS+m_iCurrentMousePoint);
	//				if (iSaveDJID>=0)
	//				{
	//					CMD_GF_Active_Prop prop;
	//					::memset(&prop, 0, sizeof(CMD_GF_Active_Prop));
	//					prop.nPropertyID = iSaveDJID;
	//					prop.cbSendLocation = LOCATION_GAME_ROOM;
	//					m_pGameFrameDlg->m_ClientKernelHelper->SendSocketData(MDM_GF_PRESENT,SUB_GF_ACTIVE_PROP,&prop,sizeof(CMD_GF_Active_Prop));
	//					char chInfo[255]={0};
	//					sprintf(chInfo, "%d�����ѳ���ʹ�ã�ʹ����Ϣ�ѷ�������������", iSaveDJID);
	//					m_pGameFrameDlg->InsertGeneralString(chInfo, RGB(0, 255, 255), true);
	//				}
	//				
	//			}
	//			//DelUserUseDJFromeIndex(m_iCurrentPage*DJ_SHOW_PAGE_NUMS+m_iCurrentMousePoint);
	//		}
	//		else
	//		{
	//			m_iCurrentMousePoint - 1;
	//		}
	//	}
	//}
	CWnd::OnLButtonUp(nFlags, point);
}

//����뿪��Ϣ
LRESULT CUserDaoJu::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	if (NULL != m_pGameFrameDlg)
	{
		m_pGameFrameDlg->SetShowTimeDJListWnd(false);
		if (NULL != m_pShowDJInfo)
		{
			m_pShowDJInfo->SetWndShow(false);
		}
		m_bHovering=false;
		InvalidateRect(NULL);
	}
	return 0;
}


//ɾ������
bool CUserDaoJu::DelUserUseDJFromeIndex(int iIndex)
{
	if (NULL == m_pUserDJInfo || iIndex >= m_pUserDJInfo->m_vecUserHaveDJList.size())
	{
		return false;
	}

	int bRet = m_pUserDJInfo->DelUserDJFromIndex(iIndex);
	ASSERT(bRet==iIndex);
	if (iIndex != bRet)
	{//ɾ�����ɹ�
		return false;
	}
	else
	{
		UpdateUserDJInfo(bRet);
	}
	return true;
}


//���õ���ҳ��
void CUserDaoJu::UpdateUserDJInfo(int iIndex)
{
	if (NULL == m_pUserDJInfo)
	{
		return;
	}
	int iTotalDJNums = m_pUserDJInfo->m_vecUserHaveDJList.size();
	if (iTotalDJNums>0)
	{//���е���
		int iCountPage = iTotalDJNums/DJ_SHOW_PAGE_NUMS;//ɾ�������ҳ��
		if (0 != iTotalDJNums%DJ_SHOW_PAGE_NUMS)
		{
			++iCountPage;
		}
		if (iCountPage != m_iTotalPage)
		{//�б仯
			m_iTotalPage = iCountPage;
			if (iIndex<DJ_SHOW_PAGE_NUMS)
			{//�ڵ�һҳ
				m_iCurrentPage = 0;
				//��߰�ť������
				m_BTLeft.EnableWindow(FALSE);
			}
			else
			{//��ǰ��һҳ
				--iIndex;
				m_iCurrentPage = iIndex/DJ_SHOW_PAGE_NUMS;//��ǰҳ
			}
			if (0 == iTotalDJNums%DJ_SHOW_PAGE_NUMS)
			{
				//�ұ߰�ť������
				m_BTRight.EnableWindow(FALSE);
			}
			if (iTotalDJNums<=DJ_SHOW_PAGE_NUMS)
			{
				//��ť������
				m_BTLeft.EnableWindow(FALSE);
				m_BTRight.EnableWindow(FALSE);
			}
		}
	}
	else
	{
		//��ť������
		m_BTLeft.EnableWindow(FALSE);
		m_BTRight.EnableWindow(FALSE);
	}
	InvalidateRect(NULL);
}


//���µ�����Ϣ
void CUserDaoJu::UpdateUserDJPage()
{
	//Ĭ��û�е���
	m_iTotalPage = 0;
	m_iCurrentPage = 0;

	if (NULL != m_pUserDJInfo)
	{
		int iTotalDJNums = m_pUserDJInfo->m_vecUserHaveDJList.size();
		if (iTotalDJNums > 0)
		{
			int iCountPage = iTotalDJNums/DJ_SHOW_PAGE_NUMS;//ҳ��
			if (0 != iTotalDJNums%DJ_SHOW_PAGE_NUMS)
			{
				++iCountPage;
			}
			m_iTotalPage = iCountPage;//��ҳ��
		}
	}
	m_BTLeft.EnableWindow(FALSE);
	if (m_iTotalPage>1)
	{
		m_BTRight.EnableWindow(TRUE);
	}
	else
	{
		m_BTRight.EnableWindow(FALSE);
	}
	//���½���
	InvalidateRect(NULL);
}

void CUserDaoJu::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (NULL != m_pGameFrameDlg)
	{
		m_pGameFrameDlg->SetShowUserActionWnd(false);
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CUserDaoJu::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (NULL != m_pGameFrameDlg)
	{
		m_pGameFrameDlg->SetShowUserActionWnd(false);
	}
	CWnd::OnRButtonDown(nFlags, point);
}

BOOL CUserDaoJu::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	m_ToolTipCtrl.RelayEvent(pMsg);
	return CWnd::PreTranslateMessage(pMsg);
}
