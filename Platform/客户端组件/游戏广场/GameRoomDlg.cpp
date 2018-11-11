// GameRoomDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GameRoomDlg.h"
#include ".\gameroomdlg.h"
#include "webpath.h"
#include "PlazaDlg.h"
#include "InitData.h"

// CGameRoomDlg �Ի���



//��Ϸ��ť�����
const int	I_G_GAMEBT_W	=	45;
const int	I_G_GAMEBT_H	=	45;
const int	I_G_RULEBT_W	=	61;
const int	I_G_RULEBT_H	=	23;

//����
const int	I_R_GAMEBT_W	=	45;
const int	I_R_GAMEBT_H	=	45;
const int	I_R_RULEBT_W	=	74;
const int	I_R_RULEBT_H	=	23;



IMPLEMENT_DYNAMIC(CGameRoomDlg, CDialog)
CGameRoomDlg::CGameRoomDlg(CWnd* pParent /*=NULL*/)
: CDialog(CGameRoomDlg::IDD, pParent)
{
	m_byGameBTState = 0;
	m_byRuleBTState = 0;

	m_bHovering = false;
	m_bIsDrawFocus = false;
}

CGameRoomDlg::~CGameRoomDlg()
{
	m_memdc.DeleteDC();
	m_ImgBK.DestroyImage();
	m_ImgFT.DestroyImage();
	m_ImgFocus.DestroyImage();
	m_ImgGameRoomFont.DestroyImage();
	m_ImgRoomName.DestroyImage();
	m_ImgLight.DestroyImage();
	m_ImageBTRule.DestroyImage();
	m_ImageBTGame.DestroyImage();
}

BEGIN_MESSAGE_MAP(CGameRoomDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_NCMOUSEMOVE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover) 
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave) 
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CGameRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CGameRoomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//����ͼƬ
	TCHAR tcTemp[16]={""};
	CString strPath("");
	GetImagePlazaPath(strPath);

	m_ImgLight.LoadImage(TEXT(strPath+"ImageLight.png"));//����״̬
	m_ImgRoomName.LoadImage(TEXT(strPath+"ImageRoomName.png"));//��������

	strPath="";
	GetImageDataPath(strPath);
	//����ͼƬ
	m_ImageNumsHuang.SetImageResource(TEXT(strPath+"Number_Huang.png"));
	m_ImageNumsLv.SetImageResource(TEXT(strPath+"Number_Lv.png"));

	//��ʼDC
	CPaintDC dc(this);
	CBitmap	bmp;
	m_memdc.DeleteDC();
	bmp.CreateCompatibleBitmap(&dc,m_rcClientRect.Width(),m_rcClientRect.Height());
	m_memdc.CreateCompatibleDC(&dc);
	m_memdc.SelectObject(bmp);
	InvalidateRect(NULL);
	bmp.DeleteObject();
	return TRUE;  // return TRUE unless you set the focus to a control
}

//�滭
void CGameRoomDlg::OnPaint()
{
	if (NULL != m_memdc.m_hDC)
	{
		CPaintDC dc(this); // device context for painting
		if (!m_ImgBK.IsNull())
		{//����
			m_ImgBK.DrawImage(&m_memdc,0,0);
		}

		if (m_bIsDrawFocus)
		{//���ƽ���
			if (!m_ImgFocus.IsNull())
			{
				m_ImgFocus.DrawImage(&m_memdc, 0, 0);
			}
		}

		if (1==m_wMode) 
		{//��Ϸ�б�
			if (!m_ImgFT.IsNull())
			{//ǰ��
				if (ACTIVE_ID == m_wKindID)
				{
					m_ImgFT.DrawImage(&m_memdc, 30, 32, m_ImgFT.GetWidth(), m_ImgFT.GetHeight(), 0, 0);
				}
				else
				{
					m_ImgFT.DrawImage(&m_memdc, 36, 34);
				}			
			}

			if (!m_ImageBTGame.IsNull())
			{//��Ϸ��ť
				m_ImageBTGame.DrawImage(&m_memdc, m_rcGameBT.left, m_rcGameBT.top, I_G_GAMEBT_W, I_G_GAMEBT_H, 
					m_byGameBTState*I_G_GAMEBT_W, 0, I_G_GAMEBT_W, I_G_GAMEBT_H);
			}

			if (!m_ImageBTRule.IsNull())
			{//��Ϸ����
				m_ImageBTRule.DrawImage(&m_memdc, m_rcRuleBT.left, m_rcRuleBT.top, I_G_RULEBT_W, I_G_RULEBT_H, 
					m_byRuleBTState*I_G_RULEBT_W, 0, I_G_RULEBT_W, I_G_RULEBT_H);
			}
		}
		else if(2==m_wMode)
		{//�����б�
			if (ACTIVE_ID != m_wKindID)
			{
				WORD wSort = m_wSortID-1;
				if (!m_ImgFT.IsNull())
				{//ǰ��
					long lWidth=m_ImgFT.GetWidth()/4;
					m_ImgFT.DrawImage(&m_memdc,118,11,lWidth,m_ImgFT.GetHeight(),(wSort>4?0:wSort)*lWidth,0);
				}

				if (!m_ImgRoomName.IsNull())
				{//��������
					long lWidth=m_ImgRoomName.GetWidth()/7;
					m_ImgRoomName.DrawImage(&m_memdc,127,14,lWidth,m_ImgRoomName.GetHeight(),wSort*lWidth,0);
				}
				if (!m_ImgLight.IsNull())
				{//����״̬
					long lWidth=m_ImgLight.GetWidth()/3;
					m_ImgLight.DrawImage(&m_memdc,92,125,lWidth,m_ImgLight.GetHeight(),m_wLight*lWidth,0);
				}
				if (!m_ImgGameRoomFont.IsNull())
				{//��Ϸ�����������
					m_ImgGameRoomFont.DrawImage(&m_memdc,18,35);
				}

				CRect rcRect;
				rcRect.left = 55+(m_wKindID != 10 ? 12 : 0);
				rcRect.top = 38;
				rcRect.right = rcRect.left + 33;
				rcRect.bottom = rcRect.top + 17;

				//��Ԫ����
				m_ImageNumsHuang.SetNumAlign(IA_CENTER);
				m_ImageNumsHuang.SetNums(m_lCellScore);
				m_ImageNumsHuang.SetShowRect(rcRect);
				m_ImageNumsHuang.DrawImageNums(&m_memdc);
				//�׷�
				rcRect.left = 80;
				rcRect.top = 58;
				rcRect.right = rcRect.left + 100;
				rcRect.bottom = rcRect.top + 17;

				m_ImageNumsHuang.SetNumAlign(IA_LEFT);
				m_ImageNumsLv.SetNums(m_lCellScore);
				m_ImageNumsLv.SetShowRect(rcRect);
				m_ImageNumsLv.DrawImageNums(&m_memdc);
				//�ⶥ
				rcRect.top = 78;
				rcRect.bottom = rcRect.top + 17;

				m_ImageNumsLv.SetNums(m_lRestrictScore);
				m_ImageNumsLv.SetShowRect(rcRect);
				m_ImageNumsLv.DrawImageNums(&m_memdc);
				//�������
				rcRect.top = 96;
				rcRect.bottom = rcRect.top + 17;

				m_ImageNumsLv.SetNums(m_lEnterMoney);
				m_ImageNumsLv.SetShowRect(rcRect);
				m_ImageNumsLv.DrawImageNums(&m_memdc);


				if (!m_ImageBTRule.IsNull())
				{//��Ϸ����
					m_ImageBTRule.DrawImage(&m_memdc, m_rcRuleBT.left, m_rcRuleBT.top, I_R_RULEBT_W, I_R_RULEBT_H, 
						m_byRuleBTState*I_R_RULEBT_W, 0, I_R_RULEBT_W, I_R_RULEBT_H);
				}
			}

			if (!m_ImageBTGame.IsNull())
			{//��Ϸ��ť
				m_ImageBTGame.DrawImage(&m_memdc, m_rcGameBT.left, m_rcGameBT.top, I_R_GAMEBT_W, I_R_GAMEBT_H, 
					m_byGameBTState*I_R_GAMEBT_W, 0, I_R_GAMEBT_W, I_R_GAMEBT_H);
			}
		}

		dc.BitBlt(0,0,m_rcClientRect.Width(),m_rcClientRect.Height(),&m_memdc,0,0,SRCCOPY);
	}
}
//���ô���λ��
void CGameRoomDlg::SetWndPos(WORD x, WORD y, WORD w, WORD h)
{
	SetWindowPos(NULL, x, y, w, h, 0);
	m_rcClientRect.left=x;
	m_rcClientRect.top=y;
	m_rcClientRect.right=m_rcClientRect.left+w;
	m_rcClientRect.bottom=m_rcClientRect.top+h;

	//��ʼDC
	CDC* pDC = GetDC();
	if (NULL != pDC)
	{
		CBitmap	bmp;
		m_memdc.DeleteDC();
		m_memdc.m_hDC = NULL;
		bmp.CreateCompatibleBitmap(pDC,m_rcClientRect.Width(),m_rcClientRect.Height());
		m_memdc.CreateCompatibleDC(pDC);
		m_memdc.SelectObject(bmp);
		bmp.DeleteObject();
	}
	ReleaseDC(pDC);
	pDC = NULL;

	InvalidateRect(NULL);
}
//���ô���ģ��
void CGameRoomDlg::SetMode(WORD wMode)
{
	m_wMode=wMode;
}
//������Դ
void CGameRoomDlg::SetResourceInfo(GameRoomRec stGameRoomRec)
{
	CString strPath("");
	GetImagePlazaPath(strPath);//��ǰ������Դ·��
	m_ImgBK.DestroyImage();
	m_ImgBK.LoadImage(TEXT(strPath+stGameRoomRec.strImgNameBK));//����ͼƬ
	m_ImgFocus.DestroyImage();
	m_ImgFocus.LoadImage(TEXT(strPath+stGameRoomRec.strImgNameFocus));//��ȡ���ͼƬ
	m_ImgFT.DestroyImage();
	m_ImgFT.LoadImage(TEXT(strPath+stGameRoomRec.strImgNameFT));//ǰ��
	m_wKindID = stGameRoomRec.wKindID;//��ϷID
	m_iDifference = stGameRoomRec.iDifference;//��ť����λ�ò���
	CString strEnterGamePath(""),strGameRule("");

	//״̬��ʼ��ΪĬ��״̬
	m_byGameBTState = 0;
	m_byRuleBTState = 0;

	if (1==m_wMode)
	{//��Ϸ����
		strEnterGamePath="BTEnterGame.png";
		strGameRule="BTGameRule.png";

		//���밴ť����
		m_rcGameBT.left = 126/*+m_iDifference*/;
		m_rcGameBT.top = 70;
		m_rcGameBT.right = m_rcGameBT.left + I_G_GAMEBT_W;
		m_rcGameBT.bottom = m_rcGameBT.top + I_G_GAMEBT_H;

		//����ť����
		m_rcRuleBT.left = 34/*+m_iDifference*/;
		m_rcRuleBT.top = 87;
		m_rcRuleBT.right = m_rcRuleBT.left + I_G_RULEBT_W;
		m_rcRuleBT.bottom = m_rcRuleBT.top + I_G_RULEBT_H;
	}
	else if(2==m_wMode)
	{//��Ϸ����
		strEnterGamePath="BTStartGame.png";
		strGameRule="BTRoomRule.png";

		m_wLight=stGameRoomRec.wLight; //��������״̬
		m_wSortID=stGameRoomRec.wSortID;//��Ϸ������
		m_lCellScore=stGameRoomRec.lCellScore;//��Ϸ��Ԫ����
		m_lRestrictScore=stGameRoomRec.lRestrictScore;//����������
		m_lEnterMoney = stGameRoomRec.lEnterMoney;//��ͽ�����
		//���밴ť����
		m_rcGameBT.left = 127+m_iDifference;
		m_rcGameBT.top = 104;
		m_rcGameBT.right = m_rcGameBT.left + I_R_GAMEBT_W;
		m_rcGameBT.bottom = m_rcGameBT.top + I_R_GAMEBT_H;

		//����ť����
		m_rcRuleBT.left = 11;
		m_rcRuleBT.top = 125;
		m_rcRuleBT.right = m_rcRuleBT.left + I_R_RULEBT_W;
		m_rcRuleBT.bottom = m_rcRuleBT.top + I_R_RULEBT_H;
	}

	m_ImageBTGame.DestroyImage();
	m_ImageBTGame.LoadImage(TEXT(strPath+strEnterGamePath));
	m_ImageBTRule.DestroyImage();
	m_ImageBTRule.LoadImage(TEXT(strPath+strGameRule));

	if (ACTIVE_ID == stGameRoomRec.wKindID)
	{
		m_ImageBTRule.DestroyImage();
		m_rcRuleBT.SetRectEmpty();
	}

	//��Ϸ��������
	CString strGameRoomtFont("");
	TCHAR tcTemp[16]={""};
	_snprintf(tcTemp,sizeof(tcTemp),"%d",m_wKindID);
	strGameRoomtFont.Append("ImageGameRoomFont_");
	strGameRoomtFont.Append(tcTemp);
	strGameRoomtFont.Append(".png");
	m_ImgGameRoomFont.DestroyImage();
	m_ImgGameRoomFont.LoadImage(TEXT(strPath+strGameRoomtFont));

	InvalidateRect(NULL, FALSE);
}
//��ʾ����
void CGameRoomDlg::SetWndShow()
{
	this->ShowWindow(SW_SHOW);
}
//���ش���
void CGameRoomDlg::SetWndHide()
{
	this->ShowWindow(SW_HIDE);
}
//��ǰ·��
void CGameRoomDlg::GetCurrentDirectoryPath(CString &szPath)
{
	szPath="";
	TCHAR szPathTemp[MAX_PATH]={0};
	GetCurrentDirectory(MAX_PATH, szPathTemp);
	szPath.Append(szPathTemp);
}
//������Դ·��
void CGameRoomDlg::GetImagePlazaPath(CString& strRetValue)
{
	CString strModuleFileName("");
	GetCurrentDirectoryPath(strModuleFileName);
	strModuleFileName.Append("\\data\\plaza\\");
	strRetValue = strModuleFileName;
}
//��Դ·��
void CGameRoomDlg::GetImageDataPath(CString& strRetValue)
{
	CString strModuleFileName("");
	GetCurrentDirectoryPath(strModuleFileName);
	strModuleFileName.Append("\\data\\");
	strRetValue = strModuleFileName;
}

void CGameRoomDlg::OnBnClickedGameRule()
{
	//CPlazaDlg * plazaDlg=(CPlazaDlg *)GetParent()->GetParent();
	CString strUrl("");
	if (10==m_wKindID)
	{
		//plazaDlg->LoadPlazaGameWeb(9, 7);
		strUrl.Format(CInitData::s_cGameHelpSingle, 10);
	}
	else if (303==m_wKindID)
	{
		//plazaDlg->LoadPlazaGameWeb(10, 8);
		strUrl.Format(CInitData::s_cGameHelpSingle, 303);
	}
	else if (310==m_wKindID)
	{
		//plazaDlg->LoadPlazaGameWeb(11, 9);
		strUrl.Format(CInitData::s_cGameHelpSingle, 310);
	}
	else if (SISEQIU_ID==m_wKindID)
	{
		//plazaDlg->LoadPlazaGameWeb(11, 9);
		strUrl.Format(CInitData::s_cGameHelpSingle, SISEQIU_ID);
	}
	m_GameRuleDlg.SetRuleUrl(strUrl);
	m_GameRuleDlg.DoModal();
	//ShellExecute(NULL,"open",strUrl,"",NULL,SW_SHOW);
}



BOOL CGameRoomDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CGameRoomDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CGameRoomDlg::OnBnClickedEnterGame()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CPlazaDlg * PlazaDlg = (CPlazaDlg *) GetParent()->GetParent();
	if (1==m_wMode)
	{//��Ϸ�б�
		PlazaDlg->m_RoomListCenterDlg.m_wKindID=m_wKindID;
		PlazaDlg->PostMessage(WM_COMMAND,IDM_CALL_ROOM_LIST,0);
		if (10==m_wKindID)
		{
			PlazaDlg->SwitchTitle(7);//�л�����
		}
		else if (303==m_wKindID)
		{
			PlazaDlg->SwitchTitle(8);//�л�����
		}
		else if (310==m_wKindID)
		{
			PlazaDlg->SwitchTitle(9);//�л�����
		}
		else if (SISEQIU_ID==m_wKindID)
		{
			PlazaDlg->SwitchTitle(14);
		}
		else if (ACTIVE_ID == m_wKindID)
		{
			PlazaDlg->SwitchTitle(15);
		}
	}
	else
	{//�����б�
		PlazaDlg->m_RoomListCenterDlg.m_wSortID=m_wSortID;
		PlazaDlg->PostMessage(WM_COMMAND,IDM_CALL_TABLE_LIST,0);
	}
}
void CGameRoomDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (!m_bIsDrawFocus)
	{
		m_bIsDrawFocus = true;
		InvalidateRect(NULL, FALSE);
	}

	if (m_rcGameBT.PtInRect(point))
	{
		if (2 != m_byGameBTState)
		{
			m_byGameBTState = 2;
			//��������
			InvalidateRect(&m_rcGameBT, FALSE);
		}
	}
	else
	{
		if (0 != m_byGameBTState)
		{
			m_byGameBTState = 0;
			//��������
			InvalidateRect(&m_rcGameBT, FALSE);
		}
	}

	if (m_rcRuleBT.PtInRect(point))
	{
		if (2 != m_byRuleBTState)
		{
			m_byRuleBTState = 2;
			//��������
			InvalidateRect(&m_rcRuleBT, FALSE);
		}
	}
	else
	{
		if (0 != m_byRuleBTState)
		{
			m_byRuleBTState = 0;
			//��������
			InvalidateRect(&m_rcRuleBT, FALSE);
		}
	}

	if (!m_bHovering)
	{
		m_bHovering = true;
		TRACKMOUSEEVENT TrackMouseEvent;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.hwndTrack=GetSafeHwnd();
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		_TrackMouseEvent(&TrackMouseEvent);

		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);
		TrackMouseEvent.dwFlags=TME_HOVER;
		TrackMouseEvent.hwndTrack=GetSafeHwnd();
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		_TrackMouseEvent(&TrackMouseEvent);

	}

	CDialog::OnMouseMove(nFlags, point);
}

void CGameRoomDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	//������Ϸ��ť
	if (m_rcGameBT.PtInRect(point))
	{
		if (1 != m_byGameBTState)
		{
			m_byGameBTState = 1;
			//��������
			InvalidateRect(&m_rcGameBT, FALSE);
		}
	}
	//��Ϸ����ť
	if (m_rcRuleBT.PtInRect(point))
	{
		if (1 != m_byRuleBTState)
		{
			m_byRuleBTState = 1;
			//��������
			InvalidateRect(&m_rcRuleBT, FALSE);
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CGameRoomDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	//������Ϸ��ť
	/*if (m_rcGameBT.PtInRect(point))
	{
	OnBnClickedEnterGame();
	return ;
	}*/
	//��Ϸ����ť
	if (m_rcRuleBT.PtInRect(point))
	{
		OnBnClickedGameRule();
		return ;
	}
	//������Ϸ
	OnBnClickedEnterGame();

	CDialog::OnLButtonUp(nFlags, point);
}

LRESULT CGameRoomDlg::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	if (!m_bIsDrawFocus)
	{
		m_bIsDrawFocus = true;
		InvalidateRect(NULL, FALSE);
	}
	return 0;
}
LRESULT CGameRoomDlg::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bHovering = false;
	m_byGameBTState = 0;
	m_byRuleBTState = 0;
	m_bIsDrawFocus = false;
	InvalidateRect(NULL, FALSE);

	return 0;
}

void CGameRoomDlg::OnDestroy()
{
	CDialog::OnDestroy();

	m_memdc.DeleteDC();
	m_ImgBK.DestroyImage();
	m_ImgFT.DestroyImage();
	m_ImgFocus.DestroyImage();
	m_ImgGameRoomFont.DestroyImage();
	m_ImgRoomName.DestroyImage();
	m_ImgLight.DestroyImage();
	m_ImageBTRule.DestroyImage();
	m_ImageBTGame.DestroyImage();
}
