// UserInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UserInfoDlg.h"
#include ".\userinfodlg.h"
#include <string>
#include "GameFrameControl.h"

#include "MemDC.h"

using namespace std;

//ID
const int IBBID			= 100;
const int ISOUNDID		= 101;


#define WND_REMOTE_X	17					// ����Xƫ��
#define WND_REMOTE_Y	50					// ����Yƫ��

// CUserInfoDlg �Ի���

IMPLEMENT_DYNAMIC(CUserInfoDlg, CDialog)
CUserInfoDlg::CUserInfoDlg(CWnd* pParent /*=NULL*/)
: CDialog(CUserInfoDlg::IDD, pParent), m_pUserData(NULL), m_pGameUserDaoJu(NULL), m_pGameFrameDlg(NULL), m_pLanguageListPtr(NULL)
{
	m_strGradeName.clear();
	m_ptWnd.SetPoint(0,0);
}

CUserInfoDlg::~CUserInfoDlg()
{
	m_pGameFrameDlg = NULL;
	m_pLanguageListPtr = NULL;
	m_pUserData = NULL;
	SafeDelete(m_pGameUserDaoJu);
}

void CUserInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUserInfoDlg, CDialog)
	ON_WM_PAINT()

	ON_BN_CLICKED(ISOUNDID, OnLanguageList)
	ON_BN_CLICKED(IBBID, OnOpenBB)

	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CUserInfoDlg ��Ϣ�������

void CUserInfoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

	CRect rect;
	GetClientRect(&rect);
	CMemDC dcMem(&dc, rect, true);	// �õ�ɫ��䱳��
	
	int iWidth = rect.Width();
	int iHeight = rect.Height();

	dcMem.SetBkMode(TRANSPARENT);

	if (!m_ImageBack.IsNull())
	{
		m_ImageBack.DrawImage(&dcMem, 0, 0, m_ImageBack.GetWidth()-WND_REMOTE_X, m_ImageBack.GetHeight()-WND_REMOTE_Y, 
			WND_REMOTE_X, WND_REMOTE_Y, m_ImageBack.GetWidth()-WND_REMOTE_X, m_ImageBack.GetHeight()-WND_REMOTE_Y);
	}

	//if (!m_GameInfoBack.IsNull())
	//	m_GameInfoBack.DrawImage(&dcMem, 429, 6);

	//��������
	CFont DrawFont;
	DrawFont.CreateFont(-16,0,0,0,800,0,0,0,134,3,2,1,2,TEXT("�ź�"));
	CFont * pOldFont=dcMem.SelectObject(&DrawFont);

	//������Ϣ����
	if (!m_InfoBack.IsNull())
	{
		m_InfoBack.DrawImage(&dcMem, 10-4-WND_REMOTE_X, 43+6-WND_REMOTE_Y, m_InfoBack.GetWidth()-50, m_InfoBack.GetHeight(), 0, 0);
		
		//���Ƶȼ�
		m_ClassNum.DrawImageNums(&dcMem);

		//�����ǳ�
		if (NULL != m_pUserData)
		{
			CRect rect(0, 6, 174, 28);
			dcMem.SetTextColor(RGB(188, 188, 188));
			//dcMem.FillSolidRect(&rect, RGB(255,0,0));//Test
			dcMem.DrawText(m_pUserData->szName, &rect, DT_CENTER);
		}

		//���Ƶȼ���ν
		if (!m_strGradeName.empty())
		{
			CRect rect(95, 30, 174, 47);
			dcMem.SetTextColor(RGB(85, 195, 96));
			//dcMem.FillSolidRect(&rect, RGB(255,0,0));//Test

			DrawFont.DeleteObject();
			DrawFont.CreateFont(-13,0,0,0,700,0,0,0,134,3,2,1,2,TEXT("�ź�"));
			dcMem.SelectObject(&DrawFont);
			dcMem.DrawText(m_strGradeName.c_str(), &rect, DT_CENTER);
		}
	}

	dcMem.SelectObject(pOldFont);
	pOldFont = NULL;
	DrawFont.DeleteObject();

	//��ұ���
	if (!m_GoldBack.IsNull())
	{
		m_GoldBack.DrawImage(&dcMem, 256-58-WND_REMOTE_X, 45+32-WND_REMOTE_Y);

		//���ƽ�Ҽ�
		m_GoldNum.DrawImageNums(&dcMem);
	}

	//��ʯ����
	if (!m_GemsBack.IsNull())
	{
		m_GemsBack.DrawImage(&dcMem, 256+65-WND_REMOTE_X, 74+3-WND_REMOTE_Y);
		//���Ʊ�ʯ
		m_GemsNum.DrawImageNums(&dcMem);
	}

	dcMem.draw();
}

//��������
void CUserInfoDlg::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
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

BOOL CUserInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pGameUserDaoJu = NULL;

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	TCHAR szResourcePath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,szResourcePath);
	CString strPath(szResourcePath);
	strPath.Append("\\data\\gameframe\\plaza\\");

	//����
	m_ImageBack.LoadImage(TEXT(strPath + "ButtonImage.png"));
	//����
	m_InfoBack.LoadImage(TEXT(strPath + "infoback.png"));
	//����
	m_GoldBack.LoadImage(TEXT(strPath + "gold.png"));
	//����
	m_GemsBack.LoadImage(TEXT(strPath + "Gems.png"));
	//����
	m_NumImage.LoadImage(TEXT(strPath + "GoldNumImage.png"));
	//����
	m_ClassNumImage.LoadImage(TEXT(strPath + "UserClassNumImage.png"));
	//���汳��
	//m_GameInfoBack.LoadImage(TEXT(strPath + "gamemsgaffiche.png"));

	//��ť
	//�����ؼ�
	CRect CreateRect(0,0,0,0);
	m_btSound.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CreateRect,this,ISOUNDID);
	m_btSound.SetButtonImage(TEXT(strPath + "LanguageSound.png"), false, 4);
	m_btSound.SetWindowPos(NULL, 645+15-WND_REMOTE_X, 50+10-WND_REMOTE_Y, 44, 41, SWP_SHOWWINDOW);
	m_btSound.EnableWindow(FALSE);

	m_btBB.Create(TEXT(""),WS_CHILD|WS_VISIBLE,CreateRect,this,IBBID);
	m_btBB.SetButtonImage(TEXT(strPath + "BBBT.png"), false, 4);
	m_btBB.SetWindowPos(NULL, 696+10-WND_REMOTE_X, 50+10-WND_REMOTE_Y, 44, 41, SWP_SHOWWINDOW);
	m_btBB.EnableWindow(FALSE);

	//��������ͼƬ
	std::string strNum = "";
	strNum.append(strPath.GetBuffer());
	strNum.append("UserClassNumImage.png");
	m_ClassNum.SetImageResource(strNum.c_str());
	m_ClassNum.SetShowRect(CRect(203-136-WND_REMOTE_X, 52+26-WND_REMOTE_Y, 236-136-WND_REMOTE_X, 65+26-WND_REMOTE_Y));
	m_ClassNum.SetNums(0);
	m_ClassNum.SetNumAlign(IA_LEFT);

	strNum = "";
	strNum.append(strPath.GetBuffer());
	strNum.append("GoldNumImage.png");
	m_GoldNum.SetImageResource(strNum.c_str());
	m_GoldNum.SetShowRect(CRect(241-WND_REMOTE_X, 83-WND_REMOTE_Y, 316-WND_REMOTE_X, 98-WND_REMOTE_Y));
	m_GoldNum.SetNums(0);
	m_GoldNum.SetNumAlign(IA_CENTER);

	m_GemsNum.SetImageResource(strNum.c_str());
	m_GemsNum.SetShowRect(CRect(364-WND_REMOTE_X, 83-WND_REMOTE_Y, 439-WND_REMOTE_X, 98-WND_REMOTE_Y));
	m_GemsNum.SetNums(0);
	m_GemsNum.SetNumAlign(IA_CENTER);


	//����������Ӵ���
	m_Affiche.Create(IDD_DLG_USER_AFFICE, this);
	m_Affiche.SetWindowPos(NULL, 432, 46, 200, 25, 0);
	std::string strMsgPath(strPath.GetBuffer());
	strMsgPath.append("gmsgafficheBack.png");
	m_Affiche.SetBackImage(strMsgPath.c_str(), true);
	m_Affiche.SetShowRect(CRect(0, 0, 200, 17));
	m_Affiche.SetTextColor(RGB(255, 0, 0));
	m_Affiche.SetGameAfficheInfo(""/*"�ر�����Ϸ---��Ϸ����ϵͳ�����Ч��---�ر�����Ϸ|**|"*/);
	m_Affiche.SetPlay(false);
	m_Affiche.ShowWindow(SW_SHOW);


	//������Ϸ�û�������
	m_pGameUserDaoJu=new CUserDaoJu(m_pGameFrameDlg);
	if (NULL == m_pGameUserDaoJu) 
	{
		throw TEXT("�û�����������ʧ��");
	}
	CRect rcGameView(0, 0, 0, 0);
	m_pGameUserDaoJu->Create(NULL,NULL,WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcGameView,this,11);
	//m_pGameUserDaoJu->SetShowDJInfo(m_pShowDJInfo);
	m_pGameUserDaoJu->SetWindowPos(NULL, 425+17-WND_REMOTE_X, 74+1-WND_REMOTE_Y, 214+17, 26, 0);
	m_pGameUserDaoJu->ShowWindow(SW_SHOW);
	//m_pUserDJInfo->SetUserDaoJuInfoPtr(m_pGameUserDaoJu);

	//������ʾ�ؼ�
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);
	m_ToolTipCtrl.AddTool(GetDlgItem(ISOUNDID),TEXT("����"));
	m_ToolTipCtrl.AddTool(GetDlgItem(IBBID),TEXT("����"));

	m_strGradeName.clear();

	//CImage BackImage;
	//BackImage.Load(TEXT(strPath + "ButtonImage.png"));
	//���ô���
	//SetImageRgn(m_hWnd, &m_ImageBack, SIRT_FULL_DOT_LESS);
	//BackImage.Destroy();

	SetWindowPos(&wndTop, 0, 0, m_ImageBack.GetWidth(), m_ImageBack.GetHeight(), SWP_NOMOVE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


//�����û���Ϣ
void CUserInfoDlg::SetUserData(tagUserData* pUserData)
{
	if (NULL != pUserData)
	{
		m_pUserData = pUserData;
		m_ClassNum.SetNums(m_pUserData->dwGrade);
		m_GoldNum.SetNums(m_pUserData->lGold);
		m_GemsNum.SetNums(m_pUserData->lGem);
		m_strGradeName.clear();
		m_strGradeName.append(pUserData->chGradeName);

		InvalidateRect(&CRect(0, 0, 420, 106), FALSE);
	}
}

//�����ͱ��������û���Ϣ��
void CUserInfoDlg::UpdateUserData(CMD_GF_NEW_GIFT* pNewData)
{
	if (NULL != pNewData && NULL != m_pUserData)
	{
		m_pUserData->dwGrade = pNewData->iGrade;							//�ȼ�
		m_pUserData->lGold = m_pUserData->lGold + pNewData->iGold;			//���
		m_pUserData->lGem = m_pUserData->lGem + pNewData->iGem;				//��ʯ

		m_ClassNum.SetNums(m_pUserData->dwGrade);
		m_GoldNum.SetNums(m_pUserData->lGold);
		m_GemsNum.SetNums(m_pUserData->lGem);
		InvalidateRect(&CRect(0, 0, 420, 106), FALSE);
	}
}

//���õ�����Ϣ
void CUserInfoDlg::SetShowDJInfo(CShowDJInfo* pShowDJInfoPtr)
{
	if (NULL != m_pGameUserDaoJu)
	{
		m_pGameUserDaoJu->SetShowDJInfo(pShowDJInfoPtr);
	}
}
//���������б�
void CUserInfoDlg::SetLLInfoPtr(CLanguageList* pLLPtr)
{
	if (NULL != pLLPtr)
	{
		m_pLanguageListPtr = pLLPtr;
	}
}
//��õ����б���Ϣ
CUserDaoJu* CUserInfoDlg::GetUserDaoJuListPtr()
{
	return m_pGameUserDaoJu;
}
//���ÿ��ָ��
void CUserInfoDlg::SetGameFrameDlgPtr(CGameFrameDlg* pFrameDlg)
{
	if (NULL != pFrameDlg)
	{
		m_pGameFrameDlg = pFrameDlg;
	}
}

BOOL CUserInfoDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	m_ToolTipCtrl.RelayEvent(pMsg);

	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_RETURN || VK_ESCAPE==pMsg->wParam))
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//����
void CUserInfoDlg::OnLanguageList()
{	
	if (m_pLanguageListPtr && m_pLanguageListPtr->GetSafeHwnd())
	{
		if (m_pLanguageListPtr->IsWindowVisible())
		{
			m_pLanguageListPtr->ShowWindow(SW_HIDE);
		}
		else
		{
			CRect rect;
			AfxGetMainWnd()->GetWindowRect(&rect);
			m_pLanguageListPtr->SetWindowPos(NULL, rect.left+566, rect.top+398, 180, 258, 0);
			m_pLanguageListPtr->ShowWindow(SW_SHOW);
			m_pLanguageListPtr->SetFocus();
		}
	}
}

//����
void CUserInfoDlg::OnOpenBB()
{
}
void CUserInfoDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (NULL != m_pLanguageListPtr)
	{
		m_pLanguageListPtr->ShowWindow(SW_HIDE);
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CUserInfoDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (NULL != m_pLanguageListPtr)
	{
		m_pLanguageListPtr->ShowWindow(SW_HIDE);
	}
	CDialog::OnRButtonDown(nFlags, point);
}

//���ť
void CUserInfoDlg::EnableBTWindow()
{
	m_btSound.EnableWindow(TRUE);
	m_btSound.ShowWindow(SW_SHOW);

	m_btBB.EnableWindow(TRUE);
	m_btBB.ShowWindow(SW_SHOW);
}

BOOL CUserInfoDlg::SetWindowPos(const CWnd* pWndInsertAfter, int x, int y,
								   int cx, int cy, UINT nFlags)
{
	m_ptWnd.SetPoint(x, y);
	return CDialog::SetWindowPos(pWndInsertAfter, x+WND_REMOTE_X, y+WND_REMOTE_Y, cx-WND_REMOTE_X, cy-WND_REMOTE_Y, nFlags);
}

void CUserInfoDlg::DrawBackground(CDC *pDC, int nWidth, int nHeight, int iExcursionY/*=0*/)
{
	if (NULL == pDC  || NULL == GetSafeHwnd()  ||  FALSE == IsWindowVisible())
	{
		return;
	}

	if (!m_ImageBack.IsNull())
	{
		m_ImageBack.DrawImage(pDC, m_ptWnd.x, m_ptWnd.y-iExcursionY);
	}

	//������Ϣ����
	if (!m_InfoBack.IsNull())
	{
		m_InfoBack.DrawImage(pDC, m_ptWnd.x+6, m_ptWnd.y-iExcursionY+49, m_InfoBack.GetWidth()-50, m_InfoBack.GetHeight(), 0, 0);
	}
}