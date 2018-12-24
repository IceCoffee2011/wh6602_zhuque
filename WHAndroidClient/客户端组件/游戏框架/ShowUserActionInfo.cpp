// ShowUserActionInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ShowUserActionInfo.h"
#include ".\showuseractioninfo.h"
#include "Resource.h"
#include "GameFrameDlg.h"

#include "MemDC.h"

// CShowUserActionInfo

#define		IDI_TIMER_SHOW_USER_ACTION_INFO				5000
#define		TIMER_SHOW_USER_ACTION_INFO_INTERVAL		4000

#define		IDI_TIMER_PLAY_FLASH						5001
#define		TIMER_PLAY_FLASH_INTERVAL					100


IMPLEMENT_DYNAMIC(CShowUserActionInfo, CWnd)
CShowUserActionInfo::CShowUserActionInfo()
{
	EnableAutomation();
	::memset(m_chActionInfo, 0, sizeof(m_chActionInfo));
	m_wCurPlayFlashIndex = 0;
	m_wCountFlashImageNums = 0;
}

CShowUserActionInfo::~CShowUserActionInfo()
{
}

void CShowUserActionInfo::OnFinalRelease()
{
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease�����ཫ�Զ�
	// ɾ���ö����ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CShowUserActionInfo, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CShowUserActionInfo, CWnd)
END_DISPATCH_MAP()

// ע��: ������� IID_IShowUserActionInfo ֧��
//��֧������ VBA �����Ͱ�ȫ�󶨡��� IID ����ͬ���ӵ� .IDL �ļ��е�
//���Ƚӿڵ� GUID ƥ�䡣

// {1049F0F4-BC96-486A-A1F4-9B7D86E4127D}
static const IID IID_IShowUserActionInfo =
{ 0x1049F0F4, 0xBC96, 0x486A, { 0xA1, 0xF4, 0x9B, 0x7D, 0x86, 0xE4, 0x12, 0x7D } };

BEGIN_INTERFACE_MAP(CShowUserActionInfo, CWnd)
	INTERFACE_PART(CShowUserActionInfo, IID_IShowUserActionInfo, Dispatch)
END_INTERFACE_MAP()


// CShowUserActionInfo ��Ϣ�������


void CShowUserActionInfo::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
	
	CRect rect;
	GetClientRect(&rect);
	CMemDC dcMem(&dc, rect, true);	// �õ�ɫ��䱳��

	if (DrawWindow(&dcMem))
	{
		//���ƶ���
		if (m_wCurPlayFlashIndex>=0 && m_wCurPlayFlashIndex<MAX_FRAME_NUMS)
		{
			//�ѻ��Ƶ�ͼƬ���ж���
			WORD wImageW = m_vecFlashImageArray[m_wCurPlayFlashIndex].GetWidth();
			WORD wImageH = m_vecFlashImageArray[m_wCurPlayFlashIndex].GetHeight();
			WORD wDrawX = m_rcImageRect.left + (m_rcImageRect.Width()-wImageW)/2;
			WORD wDrawY = m_rcImageRect.top + (m_rcImageRect.Height()-wImageH)/2;
			m_vecFlashImageArray[m_wCurPlayFlashIndex].DrawImage(&dcMem, wDrawX, wDrawY, wImageW, wImageH, 0, 0);
		}
		
		//�����ı�
		CFont DrawFont;
		DrawFont.CreateFont(-14,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("΢���ź�"));
		CFont * pOldFont=dcMem.SelectObject(&DrawFont);
		dcMem.DrawText(m_strTextList.c_str(), &m_rcTextRect, DT_LEFT|DT_TOP|DT_WORDBREAK);
		dcMem.SelectObject(pOldFont);
		DrawFont.DeleteObject();
		
		//GetClientRect(&rect);	// Ϊ�β�����ѭ�������棿 ��Ҫÿһ��ѭ������ֵ���ǲ����˷�CPU��Դ�أ�
	}
	
}

void CShowUserActionInfo::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	if (IDI_TIMER_PLAY_FLASH == nIDEvent)
	{
		++m_wCurPlayFlashIndex;
		if ((m_wCurPlayFlashIndex>=m_wCountFlashImageNums) || (m_wCurPlayFlashIndex>=MAX_FRAME_NUMS))
		{
			m_wCurPlayFlashIndex = 0;
		}
		InvalidateRect(&m_rcImageRect);
	}
	else if (IDI_TIMER_SHOW_USER_ACTION_INFO == nIDEvent)
	{
		KillTimer(IDI_TIMER_SHOW_USER_ACTION_INFO);
		ShowWindow(SW_HIDE);
	}

	CWnd::OnTimer(nIDEvent);
}

//��ʼ������
void CShowUserActionInfo::InitShowData()
{
	::memset(m_chActionInfo, 0, sizeof(m_chActionInfo));
	m_wCurPlayFlashIndex = -1;
	KillTimer(IDI_TIMER_PLAY_FLASH);
	InvalidateRect(NULL);
}

//���ö���
void CShowUserActionInfo::SetPlayFlash(BYTE byFlashIndex, const char* pSendName, const char* pRecvName)
{
	if (byFlashIndex>=0 && byFlashIndex<TOTAL_FLASH_NUMS)
	{
		if (m_PlayFlash[byFlashIndex].wTotalFrame>0)
		{
			ShowWindow(SW_SHOW);
			SetForegroundWindow();

			KillTimer(IDI_TIMER_PLAY_FLASH);
			KillTimer(IDI_TIMER_SHOW_USER_ACTION_INFO);
			
			//���ö���
			m_wCountFlashImageNums = 0;
			m_wCurPlayFlashIndex = 0;
			m_PlayFlash[byFlashIndex].bIsPlay = true;
			m_PlayFlash[byFlashIndex].wTimeInterval = TIMER_PLAY_FLASH_INTERVAL;
			HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
			CPngImage image;
			char chData[32]={0};
			for (int i=1; i<=m_PlayFlash[byFlashIndex].wTotalFrame; ++i)
			{
				m_vecFlashImageArray[m_wCountFlashImageNums].DestroyImage();
				::memset(chData, 0, sizeof(chData));
				::sprintf(chData, "IMAGE_UA_%d_%d_F", byFlashIndex, i);
				m_vecFlashImageArray[m_wCountFlashImageNums].LoadImage(hInstance, chData);
				++m_wCountFlashImageNums;
				if (m_wCountFlashImageNums>=MAX_FRAME_NUMS)
				{
					break;
				}
			}

			//�����ı�
			SetFlashText(byFlashIndex, pSendName, pRecvName);
			InvalidateRect(&m_rcImageRect);

			//������ʱ��
			SetTimer(IDI_TIMER_PLAY_FLASH, m_PlayFlash[byFlashIndex].wTimeInterval, NULL);

			//�Զ��رմ���
			SetTimer(IDI_TIMER_SHOW_USER_ACTION_INFO, TIMER_SHOW_USER_ACTION_INFO_INTERVAL, NULL);
		}
	}
}
//�����ı�
void CShowUserActionInfo::SetFlashText(BYTE byFlashIndex, const char* pSendName, const char* pRecvName)
{
	char chInfo[100] = {0};
	m_strTextList = "";
	switch(byFlashIndex)
	{
	case 0:	//��õ��--XX���ĵ��ߵ���XX��ߣ��ó���һ֦õ�塣
		{
			m_strTextList.append(pSendName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " ���ĵ��ߵ��� ");
			m_strTextList.append(chInfo);
			m_strTextList.append(pRecvName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " ���,�ó���һ֦õ��.");
			m_strTextList.append(chInfo);
		}
	break;
	case 1:	//������--XX�����Լ���������������������ͷ��		
		{
			m_strTextList.append(pSendName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " �����Լ���������������������ͷ.");
			m_strTextList.append(chInfo);
		}
		break;
	case 2:	//�ܱ�--XX�ݺݵı���XXһ�£���³���Դ����ᡣ	
		{
			m_strTextList.append(pSendName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " �ݺݵı��� ");
			m_strTextList.append(chInfo);
			m_strTextList.append(pRecvName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " һ��,��³���Դ�����.");
			m_strTextList.append(chInfo);
		}
		break;
	case 3:	//Ťƨ��--XX�����Լ���ƨ�ɣ��ڴ����ǰ���ơ�	
		{
			m_strTextList.append(pSendName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " �����Լ���ƨ��,�ڴ����ǰ����.");
			m_strTextList.append(chInfo);
		}
		break;
	case 4:	//��ͷ--XX���ε�������ͷ
		{
			m_strTextList.append(pSendName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " ���ε�������ͷ.");
			m_strTextList.append(chInfo);
		}
		break;
	case 5:	//����--XX��XX����һ������.			
		{
			m_strTextList.append(pSendName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " �� ");
			m_strTextList.append(chInfo);
			m_strTextList.append(pRecvName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " ����һ������.");
			m_strTextList.append(chInfo);
		}
		break;
	case 6:	//�׻�Ȧ--XX֣�ص���XX������һ�������Ļ�Ȧ	
		{
			m_strTextList.append(pSendName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " ֣�ص��� ");
			m_strTextList.append(chInfo);
			m_strTextList.append(pRecvName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " ������һ�������Ļ�Ȧ.");
			m_strTextList.append(chInfo);
		}
		break;
	case 7:	//����--XXί���Ŀ���	
		{
			m_strTextList.append(pSendName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " ί���Ŀ���.");
			m_strTextList.append(chInfo);
		}
		break;
	case 8:	//��һ��--XX�������˾���һ���ơ�	
		{
			m_strTextList.append(pSendName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " �������˾���һ����.");
			m_strTextList.append(chInfo);
		}
		break;
	case 9:	//��Ц--XXָ��XX�����Ц��һ�¡�
		{
			m_strTextList.append(pSendName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " ָ�� ");
			m_strTextList.append(chInfo);
			m_strTextList.append(pRecvName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " �����Ц��һ��.");
			m_strTextList.append(chInfo);
		}
		break;
	case 10:	//�κ�ë--XX���������˸�֫�ѣ�������֮��ֻ��������������һ��ë��
		{
			m_strTextList.append(pSendName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " ���������˸�֫��,ֻ������������һ��ë.");
			m_strTextList.append(chInfo);
		}
		break;
	case 11:	//����--XXָ��XX˵,������үЦһ��.
		{
			m_strTextList.append(pSendName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " ָ�� ");
			m_strTextList.append(chInfo);
			m_strTextList.append(pRecvName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " ˵,������үЦһ��.");
			m_strTextList.append(chInfo);
		}
		break;
	case 12:	//����--XX��СĴָ����˱ǿ���,���ž���а���Ц��.			
		{
			m_strTextList.append(pSendName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " ��СĴָ����˱ǿ���,���ž���а���Ц��.");
			m_strTextList.append(chInfo);
		}
		break;
	case 13:	//��Ц--XX������Ц.			
		{
			m_strTextList.append(pSendName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " ������Ц.");
			m_strTextList.append(chInfo);
		}
		break;
	case 14:	//�ڼ�--XX���Ŷ���,�������ͨ��.
		{
			m_strTextList.append(pSendName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " ���Ŷ���,�������ͨ��.");
			m_strTextList.append(chInfo);
		}
		break;
	case 15:	//�Ӽ���--XXʹ�����̵�������XX����һ����������			
		{
			m_strTextList.append(pSendName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " ʹ�����̵������� ");
			m_strTextList.append(chInfo);
			m_strTextList.append(pRecvName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " ����һ��������.");
			m_strTextList.append(chInfo);
		}
		break;
	case 16:	//��ƨ--�˵�һ�����죬֮������һ�ɻ�ѧ�����ζ����XXа���Ц�ˡ�
		{
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, "�˵�һ��,������һ�ɻ�ѧ����ζ��,");
			m_strTextList.append(chInfo);
			m_strTextList.append(pSendName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " а���Ц��.");
			m_strTextList.append(chInfo);
		}
		break;
	case 17:	//�ж�--XX�ж��Ŀ���			
		{
			m_strTextList.append(pSendName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " �ж��Ŀ���.");
			m_strTextList.append(chInfo);
		}
		break;
	case 18:	//�ӵ�--XXЪ˹����ĺ���,�ӵ���!			
		{
			m_strTextList.append(pSendName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " Ъ˹����ĺ���,�ӵ���!");
			m_strTextList.append(chInfo);
		}
		break;
	case 19:	//����--XX���΢Ц����XX�������֡�			
		{
			m_strTextList.append(pSendName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " ���΢Ц,�� ");
			m_strTextList.append(chInfo);
			m_strTextList.append(pRecvName);
			::memset(chInfo, 0, sizeof(chInfo));
			::sprintf(chInfo, " ��������.");
			m_strTextList.append(chInfo);
		}
		break;
	default:
		break;
	}
	CWnd* pMainWnd = AfxGetMainWnd();
	if (NULL != pMainWnd)
	{
		((CGameFrameDlg*)pMainWnd)->InsertSystemString(m_strTextList.c_str());
	}
	InvalidateRect(&m_rcTextRect);
}

//���������ʼ��
void CShowUserActionInfo::InitFlashData()
{
	m_wCurPlayFlashIndex = 0;
	for (int i=0; i<TOTAL_FLASH_NUMS; i++)
	{
		m_PlayFlash[i].bIsPlay = false;
		m_PlayFlash[i].wTotalFrame = 0;
		m_PlayFlash[i].wTimeInterval = TIMER_PLAY_FLASH_INTERVAL;
	}
	m_PlayFlash[0].wTotalFrame = 15;
	m_PlayFlash[1].wTotalFrame = 11;
	m_PlayFlash[2].wTotalFrame = 6;
	m_PlayFlash[3].wTotalFrame = 11;
	m_PlayFlash[4].wTotalFrame = 2;
	m_PlayFlash[5].wTotalFrame = 13;
	m_PlayFlash[6].wTotalFrame = 4;
	m_PlayFlash[7].wTotalFrame = 10;
	m_PlayFlash[8].wTotalFrame = 8;
	m_PlayFlash[9].wTotalFrame = 10;
	m_PlayFlash[10].wTotalFrame = 12;
	m_PlayFlash[11].wTotalFrame = 12;
	m_PlayFlash[12].wTotalFrame = 10;
	m_PlayFlash[13].wTotalFrame = 8;
	m_PlayFlash[14].wTotalFrame = 3;
	m_PlayFlash[15].wTotalFrame = 18;
	m_PlayFlash[16].wTotalFrame = 11;
	m_PlayFlash[17].wTotalFrame = 5;
	m_PlayFlash[18].wTotalFrame = 9;
	m_PlayFlash[19].wTotalFrame = 12;
}

int CShowUserActionInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	HINSTANCE hInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);
	m_ImageBack.SetLoadInfo(IDB_USER_ACTION_FLASH_BK, hInstance, false);

	//���������ʼ��
	InitFlashData();

	//�ı�����
	m_rcTextRect.left = 100;
	m_rcTextRect.top = 20;
	m_rcTextRect.right = m_rcTextRect.left + 130;
	m_rcTextRect.bottom = m_rcTextRect.top + 82;

	//��������
	m_rcImageRect.left = 0;
	m_rcImageRect.top = 8;
	m_rcImageRect.right = m_rcImageRect.left + 100;
	m_rcImageRect.bottom = m_rcImageRect.top + 82;
	return 0;
}




void CShowUserActionInfo::AddRectRgn(CDC* pDC,CRgn* rgn,CRect* rect,COLORREF col)
{
	for(int y=rect->top;y<rect->bottom;y++)
		for(int x=rect->left;x<rect->right;x++)
			if (pDC->GetPixel(x,y)==col)
			{
				CRgn rgnAdd;
				rgnAdd.CreateRectRgn (x, y, x+1, y+1);
				rgn->CombineRgn(rgn, &rgnAdd, RGN_DIFF);
				DeleteObject(rgnAdd);
			}
}
void CShowUserActionInfo::UpdateRgn(CDC * pDC)
{
	CImageHandle ImageHandle(&m_ImageBack);
	CRect rcClient;
	GetClientRect(&rcClient);
	CRgn rgn;
	COLORREF col = RGB(255,0,255);  // mask color
	CRect rc(0,0,0,0);
	rgn.CreateRectRgn (0, 0, rcClient.Width(), rcClient.Height());

	//����
	rc.bottom =  m_ImageBack.GetHeight();
	rc.right = m_ImageBack.GetWidth();
	AddRectRgn(pDC,&rgn,&rc,col);
	SetWindowRgn (rgn,TRUE);
	//SetForegroundWindow();
}

bool CShowUserActionInfo::DrawWindow(CDC * pDC)
{
	if (NULL != pDC && NULL != pDC->m_hDC)
	{
		CImageHandle ImageHandle(&m_ImageBack);
		pDC->SetBkMode(TRANSPARENT);
		pDC->FillSolidRect(0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),RGB(255,0,255));

		//�滭����ͼƬ
		CImageHandle HandleCenter(&m_ImageBack);
		m_ImageBack.AlphaDrawImage(pDC, 0, 0, RGB(255, 0, 255));
		return true;
	}
	return false;
}


void CShowUserActionInfo::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

#if 0  // ɾ����һ�λ���������
	CClientDC dc(this);
	CRect rect;
	GetClientRect(&rect);
	CMemDC dcMem(&dc, rect, true);	// �õ�ɫ��䱳��

	if (DrawWindow(&dc))
	{
		UpdateRgn(&dc);
		CRect rect,rc,rc2;
		GetClientRect(&rect);
		CString sss;
		if( GetUpdateRect(&rc) )
		{
			rc2 = rc;
			if( rc.left < rc.right )
			{
				rc2.top = rect.top;
				rc2.bottom = rect.bottom;
				InvalidateRect(&rc2,0);
			}
			rc2 = rc;
			if( rc.top < rc.bottom )
			{
				rc2.left = rect.left;
				rc2.right = rect.right;
				InvalidateRect(&rc2,0);
			}
		}
		else
			InvalidateRect(0);
	}
#endif

}
