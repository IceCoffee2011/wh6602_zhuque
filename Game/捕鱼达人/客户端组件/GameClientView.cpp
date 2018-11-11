#include "StdAfx.h"
#include "GameClient.h"
#include "GameClientView.h"
#include "GameClientDlg.h"
#include ".\gameclientview.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameView)
	ON_WM_CREATE()
END_MESSAGE_MAP()

//���캯��
CGameClientView::CGameClientView()
{
	m_bInitGameTag=false;

	return;
}

//��������
CGameClientView::~CGameClientView()
{
}

//���ý���
void CGameClientView::ResetGameView()
{
	return ;
}

//�����ؼ�
VOID CGameClientView::RectifyGameView(INT nWidth, INT nHeight)
{
		//������С
	if(m_GameClientViewDX.GetSafeHwnd()!=NULL)
	{
		m_GameClientViewDX.MoveWindow(0,0,nWidth,nHeight);
		m_GameClientViewDX.OnDeviceLost();
		if(!m_GameClientViewDX.UpdateDxWindowSize())
		{
			//AfxMessageBox("�豸����ʧ�ܣ����򼴽��˳���");
			AfxGetMainWnd()->SendMessage(WM_CLOSE);
			return; 
		}
		m_GameClientViewDX.InitScene(nWidth,nHeight);
	}

	//����dx����
	if(false==m_bInitGameTag && m_GameClientViewDX.GetSafeHwnd()==NULL)
	{
		m_GameClientViewDX.Create(AfxRegisterWndClass(0,NULL,NULL,NULL),NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,CRect(0,0,nWidth,nHeight),this,0);
		//m_GameClientViewDX.Create(IDD_DX_BASE,this);
		//SetWindowLong(m_GameClientViewDX.GetSafeHwnd(),GWL_STYLE,GetStyle()|WS_CLIPCHILDREN);
		if(!m_GameClientViewDX.InitDxWindow())
		{
			AfxMessageBox("�豸����ʧ�ܣ����򼴽��˳���");
			AfxGetMainWnd()->SendMessage(WM_CLOSE);
			return;  
		}
		else m_GameClientViewDX.InitScene(nWidth,nHeight);

		m_GameClientViewDX.MoveWindow(0,0,nWidth,nHeight);
		m_GameClientViewDX.ShowWindow(SW_SHOW);
		m_GameClientViewDX.RectifyGameView(nWidth,nHeight);
	}

	if(!m_bInitGameTag)m_bInitGameTag=true;

	return;
}

//�滭����
VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
	return;
}

//��������
INT CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//��������
	CRect rcCreate(0,0,0,0);
	HINSTANCE hResInstance=AfxGetInstanceHandle();

	//���ô���
	CGameClientDlg *pGameClientDlg=CONTAINING_RECORD(this,CGameClientDlg,m_GameClientView);
	if (NULL!=pGameClientDlg) m_GameClientViewDX.SetGameClientDlg(pGameClientDlg);

	return 0;
}



////��Ϣ��ȡ
//BOOL CGameClientView::PreTranslateMessage(MSG * pMsg)
//{
//	//�жϻس�
//	if(pMsg->message==WM_KEYDOWN)
//	{
//		BYTE bMesValue = (BYTE)(pMsg->wParam);
//		if(bMesValue==VK_SPACE || bMesValue==VK_DOWN || bMesValue==VK_RIGHT || bMesValue==VK_UP || bMesValue==VK_LEFT) 
//		{
//			return TRUE;
//		}
//	}
//	return __super::PreTranslateMessage(pMsg);
//}

//////////////////////////////////////////////////////////////////////////
