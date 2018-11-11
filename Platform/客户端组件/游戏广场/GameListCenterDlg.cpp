// GameListCenterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GameListCenterDlg.h"
#include "GlobalUnits.h"
#include "afxwin.h"
#include ".\gamelistcenterdlg.h"
#include "PlazaDlg.h"

IMPLEMENT_DYNAMIC(CGameListCenterDlg, CDialog)
CGameListCenterDlg::CGameListCenterDlg(CWnd* pParent /*=NULL*/)
: CDialog(CGameListCenterDlg::IDD, pParent)
{
	m_memdc.m_hDC=NULL;
}

CGameListCenterDlg::~CGameListCenterDlg()
{
	m_memdc.DeleteDC();
}

void CGameListCenterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGameListCenterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CGameListCenterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strPath("");
	GetImagePlazaPath(strPath);
	m_ImageBK.LoadImage(TEXT(strPath+"BKGameList.png"));

	//����DC
	CPaintDC dc(this);
	m_memdc.DeleteDC();
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc,536,360);
	m_memdc.CreateCompatibleDC(&dc);
	m_memdc.SelectObject(bmp);
	InvalidateRect(NULL);
	bmp.DeleteObject();

	//������Ϸ�б�
	CreateGameUnit();
	return TRUE; 
}

//��ǰ·��
void CGameListCenterDlg::GetCurrentDirectoryPath(CString &szPath)
{
	szPath="";
	TCHAR szPathTemp[MAX_PATH]={0};
	GetCurrentDirectory(MAX_PATH, szPathTemp);
	szPath.Append(szPathTemp);
}

//������Դ·��
void CGameListCenterDlg::GetImagePlazaPath(CString& strRetValue)
{
	CString strModuleFileName("");
	GetCurrentDirectoryPath(strModuleFileName);
	strModuleFileName.Append("\\data\\plaza\\");
	strRetValue = strModuleFileName;
}

//�滭
void CGameListCenterDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (!m_ImageBK.IsNull())
	{
		m_ImageBK.DrawImage(&m_memdc,0,0);
	}

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &m_memdc, 0, 0, SRCCOPY);
}
//������Ϸ�б�
void CGameListCenterDlg::SetGameList()
{
	WORD wSortID[IGAME_COUNT]={0};
	WORD wKinds[IGAME_COUNT]={0};
	WORD wKindCount=0,wIndex=0;
	WORD wKindID=0;
	bool isAdd=true;
	CListServer * pListServer=NULL;
	//��ȡ������������ϷKindID
	do 
	{
		pListServer=g_GlobalUnits.m_ServerListManager.EnumServerItem(wIndex++);
		if (NULL == pListServer) break;
		wKindID=pListServer->m_pListKind->m_GameKind.wKindID;
		isAdd=true;
		for (int i=0;i<wKindCount;++i)
		{
			if (wKindID==wKinds[i])
			{
				isAdd=false;
				break;
			}
		}
		if (isAdd)
		{
			wSortID[wKindCount]=pListServer->m_pListKind->m_GameKind.wSortID;
			wKinds[wKindCount++]=wKindID;
		}
	} while (true);

	//���°���������
	for (int i=0; i<wKindCount-1; ++i)
	{
		for (int j=i+1; j<wKindCount; ++j)
		{
			if (wSortID[i] > wSortID[j])
			{
				WORD wTmp = wSortID[i];
				wSortID[i] = wSortID[j];
				wSortID[j] = wTmp;

				wTmp = wKinds[i];
				wKinds[i] = wKinds[j];
				wKinds[j] = wTmp;
			}
		}
	}

	TCHAR chTemp[16]=TEXT("");
	GameRoomRec stGameRoomRec;
	WORD x=2,y=0,w=178,h=125;	//����λ��

	for(WORD i=0;i<wKindCount;++i)
	{
		m_GameRoomDlg[i].SetMode(1);	//����Ϊ��Ϸ�б���

		CString strGameNameImg("");
		wKindID=wKinds[i];
		//������Ϸ
		((CPlazaDlg *) GetParent())->m_GameListHot.SetGameID(wKindID);
		//ǰ��
		_snprintf(chTemp,sizeof(chTemp),TEXT("%d"),wKindID);
		strGameNameImg="FTGame_";
		strGameNameImg.Append(chTemp);
		strGameNameImg.Append(".png");
		stGameRoomRec.strImgNameFT=strGameNameImg;
		//����
		_snprintf(chTemp,sizeof(chTemp),TEXT("%d"),i+1);
		strGameNameImg="BKGame_";
		strGameNameImg.Append(chTemp);
		strGameNameImg.Append(".png");
		stGameRoomRec.strImgNameBK=strGameNameImg;
		//ѡ���
		stGameRoomRec.strImgNameFocus=TEXT("ImageSelectGame.png");
		stGameRoomRec.wKindID=wKindID;//��ϷID
		stGameRoomRec.iDifference=-i;//��ť����λ��
		m_GameRoomDlg[i].SetResourceInfo(stGameRoomRec); //������Դ
		m_GameRoomDlg[i].SetWndPos(x, y, w, h);//���ô���λ��
		
		x += w;
		if ((i % 3)==2)
		{
			x=2;
			y += h;
		}
	}



	for (int j=0; j<wKindCount; ++j)
	{//��ʾ�����õĴ���
		m_GameRoomDlg[j].SetWndShow();
	}
}
void CGameListCenterDlg::SetWndHide()
{
	for (int j=0; j<IGAME_COUNT; ++j)
	{//��ʾ�����õĴ���
		m_GameRoomDlg[j].SetWndHide();
	}
}
//������Ϸ��Ԫ
void CGameListCenterDlg::CreateGameUnit()
{
	for (int i=0; i<IGAME_COUNT; ++i)
	{
		m_GameRoomDlg[i].Create(CGameRoomDlg::IDD, this);
	}
}
void CGameListCenterDlg::OnLButtonDown(UINT nFlags, CPoint point)
{

	CDialog::OnLButtonDown(nFlags, point);
}

BOOL CGameListCenterDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}
