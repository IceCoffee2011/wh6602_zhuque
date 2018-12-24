#include "Stdafx.h"
#include "Resource.h"
#include "TableFrame.h"
#include "GlobalUnits.h"
#include ".\tableframe.h"

//////////////////////////////////////////////////////////////////////////

//��ʾ��ʽ
#define	NT_RIGHT						0x01								//�Ҷ���
#define	NT_CENTER						0x02								//�ж���
#define	NT_LEFT							0x04								//�����
#define	NT_TOP							0x10								//�϶���
#define	NT_VCENTER						0x20								//�ж���
#define	NT_BOTTOM						0x40								//�¶���

//////////////////////////////////////////////////////////////////////////

#define RESOURCE_VER					1									//��Դ�汾


const CPoint g_ptTableView[MAX_TABLE_VIEW_COUT] = {
													CPoint(19, 44), CPoint(233, 44),CPoint(452, 44), CPoint(671, 44),
													CPoint(19, 225), CPoint(233, 225),CPoint(452, 225), CPoint(671, 225),
													CPoint(19, 405), CPoint(233, 405),CPoint(452, 405), CPoint(671, 405)};
//��Դ���ýṹ
struct tagTableResourceInfo
{
	COLORREF							crName;								//������ɫ
	COLORREF							crNote;								//��ע��ɫ
	COLORREF							crTableID;							//������ɫ
	POINT								ptLock;								//��λ��
	RECT								rcNote;								//��עλ��
	RECT								rcTableID;							//����λ��
	RECT								rcName[MAX_CHAIR];					//����λ��
	RECT								rcChair[MAX_CHAIR];					//����λ��
	POINT								ptReady[MAX_CHAIR];					//׼��λ��
	INT									nDrawStyle[MAX_CHAIR];				//�����ʽ
};

//��Դ���ýṹ
struct tagTableResourceInfo_Normal
{
	COLORREF							crName;								//������ɫ
	COLORREF							crNote;								//��ע��ɫ
	COLORREF							crTableID;							//������ɫ
	POINT								ptLock;								//��λ��
	RECT								rcNote;								//��עλ��
	RECT								rcTableID;							//����λ��
	RECT								rcName[MAX_CHAIR_NORMAL];			//����λ��
	RECT								rcChair[MAX_CHAIR_NORMAL];			//����λ��
	POINT								ptReady[MAX_CHAIR_NORMAL];			//׼��λ��
	INT									nDrawStyle[MAX_CHAIR_NORMAL];		//�����ʽ
};

//�����ļ��ṹ
struct tagTableConfigFile
{
	WORD								wFileSize;							//�ļ���С
	WORD								wStructVer;							//�ṹ�汾
	tagTableResourceInfo				TableResourceInfo;					//���ò���
};

//�����ļ��ṹ
struct tagTableConfigFile_Normal
{
	WORD								wFileSize;							//�ļ���С
	WORD								wStructVer;							//�ṹ�汾
	tagTableResourceInfo_Normal			TableResourceInfo;					//���ò���
};

//////////////////////////////////////////////////////////////////////////

//���캯��
CTableResource::CTableResource()
{
	//��־����
	m_bDZShowHand=false;

	//���ñ���
	m_wWidth=0;
	m_wHeight=0;
	m_crName=RGB(0,0,0);
	m_crNote=RGB(0,0,0);
	m_crTableID=RGB(0,0,0);
	m_crBackGround=RGB(0,0,0);
	memset(m_nDrawStyle,0,sizeof(m_nDrawStyle));
	return;
}

//��������
CTableResource::~CTableResource()
{
	DestroyResource();
	return;
}

//������Դ
bool CTableResource::LoadFromFiles(bool bHideInfo,LPCTSTR pszGameName, WORD wChairCount, WORD wKindID)
{
	try
	{
		//������Դ
		CFile ResourceFile;
		TCHAR szResourcePath[MAX_PATH];
		_sntprintf(szResourcePath,sizeof(szResourcePath),TEXT("%s%s\\TableFrame.DAT"),g_GlobalUnits.GetWorkDirectory(),pszGameName);
		if (ResourceFile.Open(szResourcePath,CFile::modeRead,NULL)==FALSE) throw 0;

		//��ȡ����
		tagTableConfigFile TableConfig;
		ULONGLONG uLong=ResourceFile.GetLength();
		switch (uLong)
		{
		case sizeof(tagTableConfigFile):
			{
				UINT uReadCount=ResourceFile.Read(&TableConfig,sizeof(TableConfig));
				if (uReadCount!=sizeof(TableConfig)) throw 0;
				if (TableConfig.wFileSize!=sizeof(tagTableConfigFile)) throw 0;
				break;
			}
		case sizeof(tagTableConfigFile_Normal):
			{
				tagTableConfigFile_Normal TableConfig_Normal;
				UINT uReadCount=ResourceFile.Read(&TableConfig_Normal,sizeof(TableConfig_Normal));
				if (uReadCount!=sizeof(TableConfig_Normal)) throw 0;
				if (TableConfig_Normal.wFileSize!=sizeof(TableConfig_Normal)) throw 0;
				if (TableConfig_Normal.wStructVer!=1) throw 0;

				TableConfig.TableResourceInfo.crName=TableConfig_Normal.TableResourceInfo.crName;
				TableConfig.TableResourceInfo.crNote=TableConfig_Normal.TableResourceInfo.crNote;
				TableConfig.TableResourceInfo.crTableID=TableConfig_Normal.TableResourceInfo.crTableID;
				TableConfig.TableResourceInfo.ptLock=TableConfig_Normal.TableResourceInfo.ptLock;
				TableConfig.TableResourceInfo.rcNote=TableConfig_Normal.TableResourceInfo.rcNote;
				TableConfig.TableResourceInfo.rcTableID=TableConfig_Normal.TableResourceInfo.rcTableID;
				for (int i=0;i<8;i++)
				{
					TableConfig.TableResourceInfo.rcName[i]=TableConfig_Normal.TableResourceInfo.rcName[i];
					TableConfig.TableResourceInfo.rcChair[i]=TableConfig_Normal.TableResourceInfo.rcChair[i];
					TableConfig.TableResourceInfo.ptReady[i]=TableConfig_Normal.TableResourceInfo.ptReady[i];
					TableConfig.TableResourceInfo.nDrawStyle[i]=TableConfig_Normal.TableResourceInfo.nDrawStyle[i];
				}
				break;
			}
		default: throw 0;
		}

		//�����˿�
		if (lstrcmp(pszGameName,TEXT("DZShowHand"))==0)
		{
			m_bDZShowHand=true;
			_sntprintf(szResourcePath,sizeof(szResourcePath),TEXT("%s%s\\Player.BMP"),g_GlobalUnits.GetWorkDirectory(),pszGameName);
			m_ImagePeople.Load(szResourcePath);
			if (m_ImagePeople.IsNull()) throw 0;
		}

		//������λͼ
		_sntprintf(szResourcePath,sizeof(szResourcePath),TEXT("%s%s\\Locker.BMP"),g_GlobalUnits.GetWorkDirectory(),pszGameName);
		m_ImageLock.Load(szResourcePath);
		if (m_ImageLock.IsNull()) 
		{
			m_ImageLock.LoadFromResource(AfxGetInstanceHandle(),IDB_TABLE_LOCK);
			if (m_ImageLock.IsNull()) throw 0;
		}

		//����׼����־
		_sntprintf(szResourcePath,sizeof(szResourcePath),TEXT("%s%s\\UserReady.BMP"),g_GlobalUnits.GetWorkDirectory(),pszGameName);
		m_ImageReady.Load(szResourcePath);
		if (m_ImageReady.IsNull()) 
		{
			m_ImageReady.LoadFromResource(AfxGetInstanceHandle(),IDB_TABLE_READY);
			if (m_ImageReady.IsNull()) throw 0;
			throw 0;
		}

		//���ؿ�ʼ��־
// 		_sntprintf(szResourcePath,sizeof(szResourcePath),TEXT("%s%s\\TablePlaying.BMP"),g_GlobalUnits.GetWorkDirectory(),pszGameName);
// 		m_ImagePlay.Load(szResourcePath);
// 		if (m_ImagePlay.IsNull()) throw 0;

		//��������λͼ
		_sntprintf(szResourcePath,sizeof(szResourcePath),TEXT("%s%s\\TableNormal.png"),g_GlobalUnits.GetWorkDirectory(),pszGameName);
		m_ImageTable.LoadImage(szResourcePath);
		if (m_ImageTable.IsNull()) throw 0;

		//��ȡ����
		m_wWidth=m_ImageTable.GetWidth()/2;
		m_wHeight=m_ImageTable.GetHeight();
		//m_crBackGround=m_ImageTable.GetPixel(0,0);

		//���ò���
		tagTableResourceInfo * pResourceInfo=&TableConfig.TableResourceInfo;
		m_ptLock=pResourceInfo->ptLock;
		m_rcNote=pResourceInfo->rcNote;
		m_crName=pResourceInfo->crName;
		m_crNote=pResourceInfo->crNote;
		m_crTableID=pResourceInfo->crTableID;
		m_rcTableID=pResourceInfo->rcTableID;
		for (WORD i=0;i<MAX_CHAIR;i++)
		{
			m_ptReady[i]=pResourceInfo->ptReady[i];
			m_rcName[i]=pResourceInfo->rcName[i];
			m_rcChair[i]=pResourceInfo->rcChair[i];
			m_nDrawStyle[i]=pResourceInfo->nDrawStyle[i];
			m_rcChair[i].bottom = m_rcChair[i].top + 40;
			m_rcChair[i].right = m_rcChair[i].left + 40;
			//�޸�����
			if (wChairCount>MAX_CHAIR_NORMAL)
			{
				m_rcChair[0].left=322;
				m_rcChair[0].right=456;
				m_rcChair[0].top=399;
				m_rcChair[0].bottom=443;
			}
		}

		return true;
	}
	catch (...) { DestroyResource(); }

	return false;
}

//ж����Դ
void CTableResource::DestroyResource()
{
	//���ñ���
	m_bDZShowHand=false;

	//�ͷ�λͼ
	m_ImageLock.Destroy();
	m_ImagePlay.Destroy();
	m_ImageReady.Destroy();
	m_ImageTable.DestroyImage();
	m_ImagePeople.Destroy();

	//���ñ���
	m_wWidth=0;
	m_wHeight=0;
	m_crName=RGB(0,0,0);
	m_crNote=RGB(0,0,0);
	m_crTableID=RGB(0,0,0);
	m_crBackGround=RGB(0,0,0);
	memset(m_nDrawStyle,0,sizeof(m_nDrawStyle));

	return;
}

//////////////////////////////////////////////////////////////////////////

// BEGIN_MESSAGE_MAP(CTableFrame, CWnd)
// 	ON_WM_PAINT()
// 	ON_WM_SIZE()
// 	ON_WM_RBUTTONDOWN()
// 	ON_WM_RBUTTONUP()
// 	ON_WM_LBUTTONDOWN()
// 	ON_WM_LBUTTONUP()
// 	ON_WM_LBUTTONDBLCLK()
// 	ON_WM_SETCURSOR()
// 	ON_WM_DESTROY()
// END_MESSAGE_MAP()

//���캯��
CTableView::CTableView()
{
	m_bMouseDown=false;
	m_bFocusFrame=false;
	m_pITableFrameView=NULL;
	m_wHoverChairID=INVALID_CHAIR;
	memset(&m_TableState,0,sizeof(m_TableState));
	m_iPage=0;
}

//��������
CTableView::~CTableView()
{
}

//�ӿڲ�ѯ
void * __cdecl CTableView::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableView,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableView,Guid,dwQueryVer);
	return NULL;
}

//��ʼ������
void __cdecl CTableView::InitTableView(WORD wTableID, WORD wChairCount, ITableFrameView * pITableFrameView)
{
	m_TableState.wTableID=wTableID;
	m_TableState.wChairCount=wChairCount;
	m_TableState.wWatchCount = 0;
	//m_TableState.wChairCount=1;
	m_pITableFrameView=pITableFrameView;
	return;
}

//�����û���Ϣ
bool __cdecl CTableView::SetUserInfo(WORD wChairID, IUserItem * pIUserItem)
{
	if (wChairID>=CountArray(m_TableState.pIUserItem)) return false;
	m_TableState.pIUserItem[wChairID]=pIUserItem;
	m_pITableFrameView->UpdateTableView(m_TableState.wTableID,false);
	return true;
}

//������갴��
void __cdecl CTableView::SetMouseDown(bool bMouseDown)
{
	if (m_bMouseDown!=bMouseDown)
	{
		m_bMouseDown=bMouseDown; 
		m_pITableFrameView->UpdateTableView(m_TableState.wTableID,true);
	}
	return;
}

//���ý�����
void __cdecl CTableView::SetFocusFrame(bool bFocusFrame)
{
	if (m_bFocusFrame!=bFocusFrame)
	{
		m_bFocusFrame=bFocusFrame;
		m_pITableFrameView->UpdateTableView(m_TableState.wTableID,false);
	}
	return;
}

//��������λ��
void __cdecl CTableView::SetHoverChairID(WORD wHoverChairID)
{
	if (m_wHoverChairID!=wHoverChairID)
	{
		m_wHoverChairID=wHoverChairID;
		m_pITableFrameView->UpdateTableView(m_TableState.wTableID,true);
	}
	return;
}

//������Ϸ��־ 
void __cdecl CTableView::SetPlayFlag(bool bPlaying)
{
	if (m_TableState.bPlaying!=bPlaying)
	{
		m_TableState.bPlaying=bPlaying; 
		m_pITableFrameView->UpdateTableView(m_TableState.wTableID,true);
	}
	return;
}

//���������־
void __cdecl CTableView::SetPassFlag(bool bPass)
{
	if (m_TableState.bPass!=bPass)
	{
		m_TableState.bPass=bPass; 
		m_pITableFrameView->UpdateTableView(m_TableState.wTableID,false);
	}
	return;
}

//���ñ�ע
void __cdecl CTableView::SetTableNote(LPCTSTR pszNote)
{
	lstrcpyn(m_TableState.szTableNote,pszNote,CountArray(m_TableState.szTableNote));
	m_pITableFrameView->UpdateTableView(m_TableState.wTableID,true);
	return;
}

//��ȡ�û���Ϣ
IUserItem * __cdecl CTableView::GetUserInfo(WORD wChairID)
{
	if (wChairID>=CountArray(m_TableState.pIUserItem)) return NULL;
	return m_TableState.pIUserItem[wChairID];
}

//��ȡ��������
WORD __cdecl CTableView::GetNullChairCount(WORD & wNullChairID)
{
	WORD wNullCount=0;
	wNullChairID=INVALID_CHAIR;
	for (WORD i=0;i<m_TableState.wChairCount;i++)
	{
		if (m_TableState.pIUserItem[i]==NULL)
		{
			++wNullCount;
			wNullChairID=i;
		}
	}
	return wNullCount;
}

//�滭����
void __cdecl CTableView::DrawTable(CDC * pDC, bool bHideInfo, CTableResource * pTableResource, bool bShowUser)
{
	//�滭��ͼ
/*
	if ((m_TableState.bPlaying)||((bHideInfo==true)&&(m_TableState.wTableID>0)))
	{
		pTableResource->m_ImagePlay.BitBlt(pDC->m_hDC,0,0);
	}
	else 
	{
		pTableResource->m_ImageTable.BitBlt(pDC->m_hDC,0,0);
	}
*/

	if ((m_TableState.bPlaying)||((bHideInfo==true)&&(m_TableState.wTableID>0)))
	{
		pTableResource->m_ImageTable.DrawImage(pDC,
												0,
												0,
												pTableResource->m_ImageTable.GetWidth()/2,
												pTableResource->m_ImageTable.GetHeight(),
												pTableResource->m_ImageTable.GetWidth()/2, 
												0);		
	}
	else 
	{
		pTableResource->m_ImageTable.DrawImage(pDC,
											   0,
											   0,
											   pTableResource->m_ImageTable.GetWidth()/2,
											   pTableResource->m_ImageTable.GetHeight(),
											   0, 
											   0);
	}


	//�滭��Ϣ
	COLORREF crBackGround=pTableResource->m_crBackGround;
	IUserFaceRes * pIUserFaceRes=g_GlobalUnits.GetUserFaceRes();
	for (WORD i=0;i<m_TableState.wChairCount;i++)
	{
		////�滭�û�
		//if ((bHideInfo==true)&&(bShowUser==true))
		//{
		//	//��������
		//	CRect rcDrawRect=pTableResource->m_rcName[i];
		//	LPCTSTR pszString=(m_TableState.wTableID==0)?TEXT("�������"):TEXT("����λ��");

		//	//λ�ñ���
		//	int nYPos=0;
		//	int nDrawStyte=pTableResource->m_nDrawStyle[i];
		//	int nDrawFormat=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS;
		//	int nNameLength=lstrlen(pszString);
		//	int nNameHeight=pDC->DrawText(pszString,nNameLength,&rcDrawRect,nDrawFormat|DT_CALCRECT);

		//	//����λ��
		//	if (nDrawStyte&NT_LEFT) 
		//	{
		//		nDrawFormat|=DT_LEFT;
		//	}
		//	else if (nDrawStyte&NT_CENTER) 
		//	{
		//		nDrawFormat|=DT_CENTER;
		//	}
		//	else 
		//	{
		//		nDrawFormat|=DT_RIGHT;
		//	}

		//	if (nDrawStyte&NT_VCENTER) 
		//	{
		//		nYPos=(pTableResource->m_rcName[i].Height()-nNameHeight)/2;
		//	}
		//	else if (nDrawStyte&NT_BOTTOM) 
		//	{
		//		nYPos=(pTableResource->m_rcName[i].Height()-nNameHeight);
		//	}
		//	if (nYPos<0) 
		//	{
		//		nYPos=0;
		//	}

		//	rcDrawRect.left=pTableResource->m_rcName[i].left;
		//	rcDrawRect.right=pTableResource->m_rcName[i].right;
		//	rcDrawRect.top=pTableResource->m_rcName[i].top+nYPos;
		//	rcDrawRect.bottom=pTableResource->m_rcName[i].bottom;

		//	//�û���Ϣ
		//	if (m_TableState.wTableID==0)
		//	{
		//		//�û�����
		//		pDC->SetTextColor(pTableResource->m_crName);
		//		pDC->DrawText(pszString,nNameLength,&rcDrawRect,nDrawFormat);
		//	}
		//	else
		//	{
		//		//�û�ͷ��
		//		pDC->FillSolidRect(&pTableResource->m_rcChair[i],crBackGround);
		//		pIUserFaceRes->DrawNormalFace(pDC,pTableResource->m_rcChair[i].left+1,
		//			pTableResource->m_rcChair[i].top+1,((m_TableState.wTableID-1)*m_TableState.wChairCount+i)%pIUserFaceRes->GetFaceCount(),0,0);

		//		//�û�����
		//		pDC->SetTextColor(pTableResource->m_crName);
		//		pDC->DrawText(pszString,nNameLength,&rcDrawRect,nDrawFormat);
		//	}
		//}

		//�滭�û�
		if ((bHideInfo==false)&&(bShowUser==true)&&(m_TableState.pIUserItem[i]!=NULL))
		{
			//��������
			CRect rcDrawRect=pTableResource->m_rcName[i];
			tagUserData * pUserData=m_TableState.pIUserItem[i]->GetUserData();

			//�Զ���ͷ��
			tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();

			////�û�ͷ��
			//if (pTableResource->m_bDZShowHand==true)
			//{
			//	//��������
			//	BYTE cbGender=m_TableState.pIUserItem[i]->GetUserData()->cbGender;
			//	CSize SizePeople(pTableResource->m_ImagePeople.GetWidth()/8,pTableResource->m_ImagePeople.GetHeight()/2);

			//	//����λ��
			//	INT nXImagePos=SizePeople.cx*i;
			//	INT nYImagePos=(cbGender==0)?0:SizePeople.cy;
			//	pTableResource->m_ImagePeople.AlphaDrawImage(pDC,pTableResource->m_rcChair[i].left+1,pTableResource->m_rcChair[i].top+1,
			//		SizePeople.cx,SizePeople.cy,nXImagePos,nYImagePos,RGB(255,0,255));
			//}
			//else
			//{
			//	pDC->FillSolidRect(&pTableResource->m_rcChair[i],crBackGround);
			//	pIUserFaceRes->DrawNormalFace(pDC,pTableResource->m_rcChair[i].left+1,
			//		pTableResource->m_rcChair[i].top+1,pUserData->wFaceID,pUserData->dwUserID,pUserData->dwCustomFaceVer);
			//}

			//ͬ���־
			if ((m_TableState.bPlaying==false)&&(pUserData->cbUserStatus==US_READY))
			{
				pTableResource->m_ImageReady.AlphaDrawImage(pDC,pTableResource->m_ptReady[i].x,
					pTableResource->m_ptReady[i].y,RGB(255,0,255));
			}

			//λ�ñ���
			int nYPos=0;
			int nDrawStyte=pTableResource->m_nDrawStyle[i];
			int nDrawFormat=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS;
			int nNameLength=lstrlen(pUserData->szName);
			int nNameHeight=pDC->DrawText(pUserData->szName,nNameLength,&rcDrawRect,nDrawFormat|DT_CALCRECT);

			//����λ��
			if (nDrawStyte&NT_LEFT)
			{
				nDrawFormat|=DT_LEFT;
			}
			else if (nDrawStyte&NT_CENTER)
			{
				nDrawFormat|=DT_CENTER;
			}
			else 
			{
				nDrawFormat|=DT_RIGHT;
			}

			if (nDrawStyte&NT_VCENTER)
			{
				nYPos=(pTableResource->m_rcName[i].Height()-nNameHeight)/2;
			}
			else if (nDrawStyte&NT_BOTTOM)
			{
				nYPos=(pTableResource->m_rcName[i].Height()-nNameHeight);
			}
			if (nYPos<0)
			{
				nYPos=0;
			}
			rcDrawRect.left=pTableResource->m_rcName[i].left;
			rcDrawRect.right=pTableResource->m_rcName[i].right;
			rcDrawRect.top=pTableResource->m_rcName[i].top+nYPos;
			rcDrawRect.bottom=pTableResource->m_rcName[i].bottom;

			//�û�����
			pDC->SetTextColor(pTableResource->m_crName);
			pDC->DrawText(pUserData->szName,nNameLength,&rcDrawRect,nDrawFormat);
		}


		//�滭�߿�
		if (m_wHoverChairID==i)
		{
			CRect rcChair(&pTableResource->m_rcChair[i]);
			if (m_bMouseDown) 
			{
				pDC->Draw3dRect(&rcChair,RGB(125,125,125),RGB(255,255,255));
			}
			else
			{
				pDC->Draw3dRect(&rcChair,RGB(255,255,255),RGB(125,125,125));
			}
		}
	}

	//�滭��
	if (m_TableState.bPass==true)
	{
		int nXLock=pTableResource->m_ptLock.y-pTableResource->m_ImageLock.GetWidth()/2;
		int nYLock=pTableResource->m_ptLock.y-pTableResource->m_ImageLock.GetHeight()/2;
		pTableResource->m_ImageLock.AlphaDrawImage(pDC,nXLock,nYLock,RGB(255,0,255));
	}


	//�滭����
	if (m_TableState.szTableNote[0]==0)
	{
		TCHAR szBuffer[32];
		pDC->SetTextColor(pTableResource->m_crTableID);
		_sntprintf(szBuffer,sizeof(szBuffer),TEXT("- %d -"),m_TableState.wTableID+1+m_iPage*9);
		pDC->DrawText(szBuffer,lstrlen(szBuffer),&pTableResource->m_rcTableID,DT_VCENTER|DT_CENTER|DT_NOCLIP|DT_SINGLELINE);
	}
	else
	{
		pDC->SetTextColor(pTableResource->m_crTableID);
		pDC->DrawText(m_TableState.szTableNote,lstrlen(m_TableState.szTableNote),&pTableResource->m_rcTableID,DT_VCENTER|DT_CENTER|DT_NOCLIP|DT_SINGLELINE);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////

//��̬����
//int					CTableFrame::m_iScrollPos=10;					//��������
//int					CTableFrame::m_iWhellTimes=2;					//���ֱ���

//���캯��
CTableFrame::CTableFrame()
{
	//״̬����
	m_bLeftMouseDown=false;
	m_bRightMouseDown=false;
	m_wDownChairID=INVALID_CHAIR;
	m_wDownTableID=INVALID_TABLE;
	m_wFrameTableID=INVALID_TABLE;

	//������Ϣ
	m_iCountRow=0;
	m_iXExcursion=0;
	//m_iCurrentPos=0;
	//m_iMaxScrollPos=0;	
	m_iCountAllLine=0;
	m_iCountVisibleLine=0;
	m_iPage=0;
	m_iPageNum=0;

	//���ñ���
	m_wTableCount=0;
	m_wChairCount=0;
	m_bShowUser=false;
	m_bHideInfo=false;

	//����ָ��
	m_pTableViewArray=NULL;
	m_pITableFrameSink=NULL;
	m_pParentWnd = NULL;

	for (WORD i=0; i<MAX_TABLE_VIEW_COUT; i++)
		m_pCurTable[i]=NULL;

	return;
}

//��������
CTableFrame::~CTableFrame()
{
	m_pParentWnd = NULL;
	DestroyTableFrame();
	return;
}

//�ӿڲ�ѯ
void * __cdecl CTableFrame::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameView,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameView,Guid,dwQueryVer);
	return NULL;
}

//��������
bool __cdecl CTableFrame::CreateTableFrame(CWnd * pParentWnd, UINT uWndID)
{
	m_pParentWnd = pParentWnd;
	//CRect rcFrameRect(0,0,0,0);
	//DWORD dwStyle=WS_CHILD|WS_VISIBLE/*|WS_VSCROLL*/|WS_TABSTOP|WS_CLIPSIBLINGS|WS_CLIPCHILDREN;
	//Create(NULL,NULL,dwStyle,rcFrameRect,pParentWnd,uWndID);
	//SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);
	return true;
}

//���ú���
bool __cdecl CTableFrame::InitTableFrame(WORD wTableCount, WORD wChairCount, bool bHideInfo, const tagGameKind * pGameKind, IUnknownEx * pIUnknownEx)
{
	//Ч�����
	ASSERT(pGameKind!=NULL);
	ASSERT(wChairCount<=MAX_CHAIR);

	//��������
	bool bLoadResource=false;

	//��ȡ��Դ
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	GetGameResDirectory(pGameKind,szDirectory,CountArray(szDirectory));

	//�ļ�����
	if (m_TableResource.LoadFromFiles(bHideInfo,szDirectory,wChairCount,pGameKind->wKindID)==false)
	{
		throw TEXT("��Ϸ������Դ����ʧ�ܣ��볢�����°�װ��Ϸ������⣡");
	}

	m_DrawTextFont.CreateFont(-12,0,0,0,400,0,0,0,134,3,2,1,2,TEXT("����"));

	//������Ϸ����
	m_bHideInfo=bHideInfo;
	m_wTableCount=wTableCount;
	m_wChairCount=wChairCount;
	//m_wChairCount=1;
	SetPage(0);
	m_pTableViewArray=new CTableView[m_wTableCount];
	if (m_pTableViewArray==NULL) 
	{
		throw TEXT("����ϵͳ��Դ���㣬�޷�������Ϸ���ӣ�");
	}

	m_iMaxPage = m_wTableCount/MAX_TABLE_VIEW_COUT+1;
	for (WORD i=0;i<m_wTableCount;i++) 
	{
		((CTableView*)(m_pTableViewArray+i))->InitTableView(i,wChairCount,this);
	}
	UpdatePage();
	//���ýӿ�
	m_pITableFrameSink=(ITableFrameSink *)pIUnknownEx->QueryInterface(IID_ITableFrameSink,VER_ITableFrameSink);
	ASSERT(m_pITableFrameSink!=NULL);
	if (m_pITableFrameSink==NULL)
	{
		throw TEXT("�ӿڲ�ѯʧ�ܣ��޷�������Ϸ���ӣ�");
	}


	return true;
}

//���ٺ���
void __cdecl CTableFrame::DestroyTableFrame()
{
	//���ٿؼ�
// 	if (m_hWnd!=NULL)
// 	{
// 		//DestroyWindow();
// 	}
	m_DrawTextFont.DeleteObject();
	SafeDeleteArray(m_pTableViewArray);
	m_TableResource.DestroyResource();

	//��������
	m_iCountRow=0;
	m_iXExcursion=0;
	//m_iCurrentPos=0;
	//m_iMaxScrollPos=0;	
	m_iCountAllLine=0;
	m_iCountVisibleLine=0;
	m_iPage=0;
	//m_iPageNum=0;

	//״̬����
	m_bLeftMouseDown=false;
	m_bRightMouseDown=false;
	m_wDownChairID=INVALID_CHAIR;
	m_wDownTableID=INVALID_TABLE;
	m_wFrameTableID=INVALID_TABLE;

	//���ñ���
	m_wTableCount=0;
	m_wChairCount=0;
	m_bShowUser=false;

	//����ָ��
	m_pTableViewArray=NULL;
	m_pITableFrameSink=NULL;

	return;
}

//��������
void __cdecl CTableFrame::UpdateTableView(WORD wTableID, bool bMustUpdate)
{
	if ((m_pParentWnd!=NULL)&&((m_bShowUser==true)||(bMustUpdate==true)))
	{
		for (WORD i=0; i<MAX_TABLE_VIEW_COUT; i++)
		{
			if (m_pCurTable[i])
			{
				if ( wTableID == m_pCurTable[i]->GetTableAttrib()->wTableID )
				{
					CRect rcTableView(0, 0, m_TableResource.m_wWidth, m_TableResource.m_wHeight);
					rcTableView.MoveToXY(g_ptTableView[i].x, g_ptTableView[i].y);
					m_pParentWnd->InvalidateRect(&rcTableView,FALSE);
				}
			}
		}
	}
	/* 2011-4-1 yy �滻
	if ((m_pParentWnd!=NULL)&&((m_bShowUser==true)||(bMustUpdate==true)))
		{
			ITableView * pITableView=GetTableArrayPtr(wTableID);
			if ((pITableView!=NULL)&&(m_iCountRow!=0))
			{
				RECT rcTableView;
				rcTableView.left=(wTableID%m_iCountRow)*m_TableResource.m_wWidth+m_iXExcursion;
				rcTableView.top=(wTableID/m_iCountRow)*m_TableResource.m_wHeight/ *-m_iCurrentPos* /;
				rcTableView.right=rcTableView.left+m_TableResource.m_wWidth;
				rcTableView.bottom=rcTableView.top+m_TableResource.m_wHeight;
				//GetParent()->InvalidateRect(&rcTableView,FALSE);
				m_pParentWnd->InvalidateRect(&rcTableView,FALSE);
			}
		}*/
	
	return;
}

//��ȡ��Ϸ��ָ��
ITableView * __cdecl CTableFrame::GetTableArrayPtr(WORD wTableID)
{
	if ((m_pTableViewArray==NULL)||(wTableID>=m_wTableCount))
	{
		return NULL;
	}
	return (m_pTableViewArray+wTableID);
}

//������ʾ
bool __cdecl CTableFrame::ShowUserInfo(bool bShowUser)
{
	m_bShowUser=bShowUser;
	//if (m_hWnd!=NULL) Invalidate(FALSE);
	if (m_pParentWnd!=NULL) 
	{
		m_pParentWnd->Invalidate(FALSE);
	}

	return true;
}

//������ͼ����
bool __cdecl CTableFrame::EnsureVisibleTable(WORD wTableID)
{
	////Ч�����
	//ASSERT(wTableID<m_wTableCount);
	//if (wTableID>=m_wTableCount) return false;

	////����λ��
	//int iLastPos=m_iCurrentPos;
	//int iXPos=(wTableID%m_iCountRow)*m_TableResource.m_wWidth+m_iXExcursion;
	//int iYPos=(wTableID/m_iCountRow)*m_TableResource.m_wHeight-m_iCurrentPos;

	////����λ��
	//CRect rcTable,rcClient;
	//GetClientRect(&rcClient);
	//rcTable.SetRect(iXPos,iYPos,iXPos+m_TableResource.m_wWidth,iYPos+m_TableResource.m_wHeight);
	//if ((rcTable.bottom>rcClient.bottom)||(rcTable.top<0)) m_iCurrentPos+=rcTable.bottom-rcClient.bottom;
	//m_iCurrentPos=__max(0,__min(m_iCurrentPos,m_iMaxScrollPos-m_iCountVisibleLine*m_TableResource.m_wHeight));

	//������ͼ
	//if (iLastPos!=m_iCurrentPos)
	//{
	//	SetScrollPos(SB_VERT,m_iCurrentPos);
	//	ScrollWindow(0,iLastPos-m_iCurrentPos,NULL,NULL);
	//}

	return true;
}

//��������
bool __cdecl CTableFrame::FlashTable(WORD wTableID)
{
	//��ȡ����
	ITableView * pITableView=GetTableArrayPtr(wTableID);
	if (pITableView==NULL) return false;

	// ת��ָ��ҳ
	OpenDesignationPage(wTableID);

	//����λ��
	CRect rcTable;
	//int iTableRow=wTableID%m_iCountRow;
	//int iTableLine=wTableID/m_iCountRow;
	//rcTable.left=iTableRow*m_TableResource.m_wWidth+m_iXExcursion;
	//rcTable.top=iTableLine*m_TableResource.m_wHeight/*-m_iCurrentPos*/;
	//rcTable.right=rcTable.left+m_TableResource.m_wWidth;
	//rcTable.bottom=rcTable.top+m_TableResource.m_wHeight;

	rcTable.left=g_ptTableView[wTableID%MAX_TABLE_VIEW_COUT].x;
	rcTable.top=g_ptTableView[wTableID%MAX_TABLE_VIEW_COUT].y;
	rcTable.right=rcTable.left + m_TableResource.m_wWidth+5;
	rcTable.bottom=rcTable.top + m_TableResource.m_wHeight+5;
	
	//�������
	const int nFlashTimes=18,nStepWidth=4;
	const int nExpandPos=nFlashTimes*nStepWidth;
	rcTable.InflateRect(nExpandPos,nExpandPos);

	//ѭ���滭
	//UpdateWindow();
	CDC * pDC=m_pParentWnd->GetDC();
	CPen LinePen(PS_SOLID,0,GetSysColor(COLOR_BTNHILIGHT));
	CPen * pOldPen=pDC->SelectObject(&LinePen);
	pDC->SetROP2(R2_XORPEN);
	for (int nTimes=nFlashTimes;nTimes>=0;nTimes--)
	{
		pDC->MoveTo(rcTable.left,rcTable.top);
		pDC->LineTo(rcTable.right,rcTable.top);
		pDC->LineTo(rcTable.right,rcTable.bottom);
		pDC->LineTo(rcTable.left,rcTable.bottom);
		pDC->LineTo(rcTable.left,rcTable.top);
		Sleep((nFlashTimes-nTimes)*2);
		pDC->MoveTo(rcTable.left,rcTable.top);
		pDC->LineTo(rcTable.right,rcTable.top);
		pDC->LineTo(rcTable.right,rcTable.bottom);
		pDC->LineTo(rcTable.left,rcTable.bottom);
		pDC->LineTo(rcTable.left,rcTable.top);
		rcTable.DeflateRect(nStepWidth,nStepWidth);
	}
	pDC->SelectObject(pOldPen);
	m_pParentWnd->ReleaseDC(pDC);

	return true;
}

//��������
bool __cdecl CTableFrame::FlashChair(WORD wTableID, WORD wChairID)
{
	//��ȡ����
	ITableView * pITableView=GetTableArrayPtr(wTableID);
	if (pITableView==NULL) return false;

	// ת��ָ��ҳ
	OpenDesignationPage(wTableID);

	//��ȡλ��
	CRect rcChair=m_TableResource.m_rcChair[wChairID];

	////��ȡ��ͼλ��
	//int iTableRow=wTableID%m_iCountRow;
	//int iTableLine=wTableID/m_iCountRow;
	//int iXPos=iTableRow*m_TableResource.m_wWidth/*+m_iXExcursion*/;
	//int iYPos=iTableLine*m_TableResource.m_wHeight/*-m_iCurrentPos*/;
	//rcChair.OffsetRect(iXPos,iYPos);
	rcChair.OffsetRect(g_ptTableView[wTableID%MAX_TABLE_VIEW_COUT].x, g_ptTableView[wTableID%MAX_TABLE_VIEW_COUT].y);

	//�������
	const int nFlashTimes=18,nStepWidth=4;
	const int nExpandPos=nFlashTimes*nStepWidth;
	rcChair.InflateRect(nExpandPos,nExpandPos);

	//ѭ���滭
	//m_pParentWnd->UpdateWindow();
	CDC * pDC=m_pParentWnd->GetDC();
	CPen LinePen(PS_SOLID,0,GetSysColor(COLOR_BTNHILIGHT));
	CPen * pOldPen=pDC->SelectObject(&LinePen);
	pDC->SetROP2(R2_XORPEN);
	for (int nTimes=nFlashTimes;nTimes>=0;nTimes--)
	{
		pDC->MoveTo(rcChair.left,rcChair.top);
		pDC->LineTo(rcChair.right,rcChair.top);
		pDC->LineTo(rcChair.right,rcChair.bottom);
		pDC->LineTo(rcChair.left,rcChair.bottom);
		pDC->LineTo(rcChair.left,rcChair.top);
		Sleep((nFlashTimes-nTimes)*2);
		pDC->MoveTo(rcChair.left,rcChair.top);
		pDC->LineTo(rcChair.right,rcChair.top);
		pDC->LineTo(rcChair.right,rcChair.bottom);
		pDC->LineTo(rcChair.left,rcChair.bottom);
		pDC->LineTo(rcChair.left,rcChair.top);
		rcChair.DeflateRect(nStepWidth,nStepWidth);
	}

	pDC->SelectObject(pOldPen);
	m_pParentWnd->ReleaseDC(pDC);

	return true;
}

//�����û���Ϣ
bool __cdecl CTableFrame::SetUserInfo(WORD wTableID, WORD wChairID, IUserItem * pIUserItem)
{
	ITableView * pITableView=GetTableArrayPtr(wTableID);
	ASSERT(pITableView!=NULL);
	if (pITableView==NULL) return false;
	return pITableView->SetUserInfo(wChairID,pIUserItem);
}

//������Ϸ��־ 
void __cdecl CTableFrame::SetPlayFlag(WORD wTableID, bool bPlaying)
{
	ITableView * pITableView=GetTableArrayPtr(wTableID);
	ASSERT(pITableView!=NULL);
	if (pITableView!=NULL) pITableView->SetPlayFlag(bPlaying);
	return;
}

//���������־
void __cdecl CTableFrame::SetPassFlag(WORD wTableID, bool bPass)
{
	ITableView * pITableView=GetTableArrayPtr(wTableID);
	ASSERT(pITableView!=NULL);
	if (pITableView!=NULL) pITableView->SetPassFlag(bPass);
	return;
}

//��ȡ�û���Ϣ
IUserItem * __cdecl CTableFrame::GetUserInfo(WORD wTableID, WORD wChairID)
{
	ITableView * pITableView=GetTableArrayPtr(wTableID);
	ASSERT(pITableView!=NULL);
	if (pITableView==NULL) return NULL;
	return pITableView->GetUserInfo(wChairID);
}

//��ȡ��������
WORD __cdecl CTableFrame::GetNullChairCount(WORD wTableID, WORD & wNullChairID)
{
	ITableView * pITableView=GetTableArrayPtr(wTableID);
	ASSERT(pITableView!=NULL);
	if (pITableView==NULL) return 0;
	return pITableView->GetNullChairCount(wNullChairID);
}

//��ѯ��Ϸ��־
bool __cdecl CTableFrame::QueryPlayFlag(WORD wTableID)
{
	ITableView * pITableView=GetTableArrayPtr(wTableID);
	ASSERT(pITableView!=NULL);
	if (pITableView==NULL) return false;
	return pITableView->QueryPlayFlag();
}

//��ѯ�����־
bool __cdecl CTableFrame::QueryPassFlag(WORD wTableID)
{
	ITableView * pITableView=GetTableArrayPtr(wTableID);
	ASSERT(pITableView!=NULL);
	if (pITableView==NULL) return false;
	return pITableView->QueryPassFlag();
}

//��ȡ��������
const tagTableState * CTableFrame::GetTableAttrib(WORD wTableID)
{
	ITableView * pITableView=GetTableArrayPtr(wTableID);
	ASSERT(pITableView!=NULL);
	if (pITableView==NULL) return NULL;
	return pITableView->GetTableAttrib();
}

//�ػ���Ϣ
void CTableFrame::OnPaint(CDC * pDc)
{
	if (m_pParentWnd==NULL || NULL == pDc)
		return;
	//���� DC
	//CPaintDC dc(this);

	//��ȡ����
	CRect rcClient,ClipRect;
	pDc->GetClipBox(&ClipRect);
	m_pParentWnd->GetClientRect(&rcClient);

	//�滭��Ϸ��
	if (m_pTableViewArray!=NULL)
	{
		//�������
		int iTableWidth=m_TableResource.m_wWidth+5;
		int iTableHeight=m_TableResource.m_wHeight+5;

		//��������ͼ
		CDC BufferDC;
		CBitmap BufferBmp;
		BufferDC.CreateCompatibleDC(pDc);
		BufferDC.SetBkMode(TRANSPARENT);
		BufferDC.SelectObject(&m_DrawTextFont);
		BufferBmp.CreateCompatibleBitmap(pDc,iTableWidth,iTableHeight);
		BufferDC.SelectObject(&BufferBmp);
	
		//�滭����
/*
		RECT rcTableView;
		ITableView * pITableView=NULL;
		int iBeginRow=(ClipRect.left-m_iXExcursion)/iTableWidth;
		int iBeginLine=(/ *m_iCurrentPos+* /ClipRect.top)/iTableHeight;
		int iEndDrawRow=3;//(ClipRect.right-m_iXExcursion+iTableWidth-1)/iTableWidth;
		int iEndDrawLine=3;//(m_iCurrentPos+ClipRect.bottom+iTableHeight-1)/iTableHeight;
		for (int iLine=iBeginLine; iLine<iEndDrawLine; iLine++)
		{
			for (int iRow=iBeginRow; iRow<iEndDrawRow; iRow++)
			{
				if(iLine == 0 && iRow == 1)
				{
					continue;
				}
				int iTableIndex = iLine * m_iCountRow + iRow;
				rcTableView.left = iRow * iTableWidth + m_iXExcursion;
				rcTableView.top = iLine * iTableHeight/ *-m_iCurrentPos* /;
				rcTableView.right = rcTableView.left + iTableWidth;
				rcTableView.bottom = rcTableView.top + iTableHeight;
				if ( ( iRow < m_iCountRow ) && ( iTableIndex < m_wTableCount ) )
				{
					pITableView = GetTableArrayPtr( iLine * m_iCountRow + iRow + m_iPage * 9 );
					ASSERT(pITableView!=NULL);
					if(pITableView == NULL)
					{
						return;
					}
					pITableView->SetPage(m_iPageNum);
					BufferDC.BitBlt(0, 0, iTableWidth-5,iTableHeight-5, pDc, rcTableView.left,rcTableView.top, SRCCOPY);
					pITableView->DrawTable(&BufferDC,m_bHideInfo,&m_TableResource,m_bShowUser);
					pDc->BitBlt(rcTableView.left,rcTableView.top,iTableWidth-5,iTableHeight-5,&BufferDC,0,0,SRCCOPY);
				}
				//else dc.FillSolidRect(&rcTableView,m_TableResource.m_crBackGround);
			}
		}
*/		//2011-4-1 yy �滻
		for (WORD i=0; i<MAX_TABLE_VIEW_COUT; i++)
		{
			if (m_pCurTable[i])
			{
				BufferDC.BitBlt(0, 0, iTableWidth,iTableHeight, pDc, g_ptTableView[i].x,g_ptTableView[i].y, SRCCOPY);
				m_pCurTable[i]->DrawTable(&BufferDC,m_bHideInfo,&m_TableResource,m_bShowUser);
				pDc->BitBlt(g_ptTableView[i].x,g_ptTableView[i].y,iTableWidth,iTableHeight,&BufferDC,0,0,SRCCOPY);
			}
		}
		//�滭��϶
		//if (m_iXExcursion>ClipRect.left)
			//dc.FillSolidRect(ClipRect.left,ClipRect.top,m_iXExcursion-ClipRect.left,ClipRect.bottom,m_TableResource.m_crBackGround);

		//������Դ
		BufferDC.DeleteDC();
		BufferBmp.DeleteObject();
	}
	else
	{
		pDc->FillSolidRect(&ClipRect,m_TableResource.m_crBackGround);
	}


	return;
}

//λ����Ϣ
void CTableFrame::OnSize(UINT nType, int cx, int cy)
{
	//__super::OnSize(nType, cx, cy);

	//�������
	int iTableWidth=m_TableResource.m_wWidth;
	int iTableHeight=m_TableResource.m_wHeight;

	//����λ��
	if (m_wTableCount!=0)
	{
		int iViewWidth=__max(iTableWidth,cx);
		int iViewHeight=__max(iTableHeight,cy);
		m_iCountRow=3;//iViewWidth/iTableWidth;
		m_iCountVisibleLine=3;//iViewHeight/iTableHeight;
		m_iCountAllLine=3;//(m_wTableCount+m_iCountRow-1)/m_iCountRow;
		m_iXExcursion=(iViewWidth-m_iCountRow*iTableWidth)/2;
	}	
	//���ù�������
	//m_iMaxScrollPos=m_iCountAllLine*iTableHeight;
	//m_iCurrentPos=__min(m_iCurrentPos,m_iMaxScrollPos-m_iCountVisibleLine*iTableHeight);
	//if (m_iCurrentPos<0) m_iCurrentPos=0;
	
	//���ù�����
	//SCROLLINFO ScrollInfo;
	//ScrollInfo.cbSize=sizeof(SCROLLINFO);
	//ScrollInfo.fMask=SIF_RANGE|SIF_PAGE|SIF_POS;
	//ScrollInfo.nMin=0;
	//ScrollInfo.nMax=m_iMaxScrollPos;
	//ScrollInfo.nPage=cy;
	//ScrollInfo.nPos=m_iCurrentPos;
	//SetScrollInfo(SB_VERT,&ScrollInfo,TRUE);

	//return;
}

//��Ϣ����
// BOOL CTableFrame::PreTranslateMessage(MSG * pMsg)
// {
// 	/*if (pMsg->message==WM_KEYDOWN)
// 	{
// 		switch (pMsg->wParam)
// 		{
// 		case VK_UP:
// 			{
// 				SendMessage(WM_VSCROLL,MAKELONG(SB_LINEUP,m_iCurrentPos),NULL);
// 				return TRUE;
// 			}
// 		case VK_DOWN:
// 			{
// 				SendMessage(WM_VSCROLL,MAKELONG(SB_LINEDOWN,m_iCurrentPos),NULL);
// 				return TRUE;
// 			}
// 		case VK_PRIOR:
// 			{
// 				SendMessage(WM_VSCROLL,MAKELONG(SB_PAGEUP,m_iCurrentPos),NULL);
// 				return TRUE;
// 			}
// 		case VK_NEXT:
// 		case VK_SPACE:
// 			{
// 				SendMessage(WM_VSCROLL,MAKELONG(SB_PAGEDOWN,m_iCurrentPos),NULL);
// 				return TRUE;
// 			}
// 		case VK_HOME:
// 			{
// 				SendMessage(WM_VSCROLL,MAKELONG(SB_TOP,m_iCurrentPos),NULL);
// 				return TRUE;
// 			}
// 		case VK_END:
// 			{
// 				SendMessage(WM_VSCROLL,MAKELONG(SB_BOTTOM,m_iCurrentPos),NULL);
// 				return TRUE;
// 			}
// 		}
// 	}*/
// 	return __super::PreTranslateMessage(pMsg);
// }

//������Ϣ
//void CTableFrame::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar)
//{
//	//��ȡ����
//	RECT rcClientRect;
//	int iLastPos=m_iCurrentPos;
//	GetClientRect(&rcClientRect);
//
//	//�ƶ�����
//	switch (nSBCode)
//	{
//	case SB_TOP:
//		{
//			m_iCurrentPos=0;
//			break;
//		}
//	case SB_BOTTOM:
//		{
//			m_iCurrentPos=m_iMaxScrollPos-m_iCountVisibleLine*m_TableResource.m_wHeight;
//			break;
//		}
//	case SB_LINEUP:
//		{
//			m_iCurrentPos-=m_iScrollPos;
//			break;
//		}
//	case SB_PAGEUP:
//		{
//			m_iCurrentPos-=rcClientRect.bottom;
//			break;
//		}
//	case SB_LINEDOWN:
//		{
//			m_iCurrentPos+=m_iScrollPos;
//			break;
//		}
//	case SB_PAGEDOWN:
//		{
//			m_iCurrentPos+=rcClientRect.bottom;
//			break;
//		}
//	case SB_THUMBTRACK:
//		{
//			m_iCurrentPos=nPos;
//			break;
//		}
//	}
//	
//	//����λ��
//	SetFocus();
//	m_iCurrentPos=__min(m_iCurrentPos,m_iMaxScrollPos-rcClientRect.bottom);
//	if (m_iCurrentPos<0) m_iCurrentPos=0;
//	if (iLastPos!=m_iCurrentPos)
//	{
//		SetScrollPos(SB_VERT,m_iCurrentPos);
//		ScrollWindow(0,iLastPos-m_iCurrentPos,NULL,NULL);
//	}
//
//	return;
//}

//�����Ϣ
void CTableFrame::OnLButtonDblClk(UINT nFlags, CPoint Point)
{
	if (INVALID_CHAIR!=m_wDownTableID)
	{
		m_pParentWnd->SetFocus();
		m_pITableFrameSink->OnDoubleHitTable(m_wDownTableID,false);
	}
	
	return;
}

//�����Ϣ
void CTableFrame::OnLButtonDown(UINT nFlags, CPoint Point)
{
/*
	m_pParentWnd->SetFocus();
	if ((m_bRightMouseDown==false)&&(m_bLeftMouseDown==false))
	{
		::SetCapture(m_pParentWnd->GetSafeHwnd());
		m_bLeftMouseDown=true;
		m_wDownTableID=TableHitTest(Point);
		if (m_wDownTableID!=INVALID_TABLE)
		{
			ITableView * pITableView=GetTableArrayPtr(m_wDownTableID);
			ASSERT(pITableView!=NULL);
			CPoint TablePoint;
			TablePoint.x=(Point.x-m_iXExcursion)%m_TableResource.m_wWidth;
			TablePoint.y=(Point.y)%m_TableResource.m_wHeight;
			m_wDownChairID=ChairHitTest(TablePoint);
			if (m_wDownChairID!=INVALID_CHAIR) 
			{
				pITableView->SetMouseDown(m_bLeftMouseDown);
				pITableView->SetHoverChairID(m_wDownChairID);
			}
		}
	}*/ //2011-4-1 yy �滻
	if ((m_bRightMouseDown==false)&&(m_bLeftMouseDown==false))
	{
		::SetCapture(m_pParentWnd->GetSafeHwnd());
		m_bLeftMouseDown=true;
		m_wDownTableID=TableHitTest(Point);
		WORD i = GetViewTableID(m_wDownTableID);
		if (m_wDownTableID!=INVALID_TABLE && i>=0&&i<MAX_TABLE_VIEW_COUT)
		{
			ITableView * pITableView=GetTableArrayPtr(m_wDownTableID);
			ASSERT(pITableView!=NULL);
			CPoint TablePoint;
			TablePoint.x=Point.x-g_ptTableView[i].x;
			TablePoint.y=Point.y-g_ptTableView[i].y;
			m_wDownChairID=ChairHitTest(TablePoint);
			if (m_wDownChairID!=INVALID_CHAIR) 
			{
				pITableView->SetMouseDown(m_bLeftMouseDown);
				pITableView->SetHoverChairID(m_wDownChairID);
			}
		}
	}
	return;
}

//�����Ϣ
void CTableFrame::OnLButtonUp(UINT nFlags, CPoint Point)
{
	/*
if ((m_bLeftMouseDown==true)&&(m_bRightMouseDown==false))
	{
		//���԰���
		::ReleaseCapture();
		m_bLeftMouseDown=false;
		WORD wTableID=TableHitTest(Point);
		if(wTableID == 1)return;
		ITableView * pITableView=GetTableArrayPtr(m_wDownTableID);
		if (pITableView!=NULL) 
		{
			pITableView->SetMouseDown(false);
			pITableView->SetHoverChairID(INVALID_CHAIR);
		}
		//��ȡλ��
		WORD wChairID=INVALID_CHAIR;
		if (wTableID==m_wDownTableID)
		{
			ITableView * pITableView=GetTableArrayPtr(wTableID);
			if (pITableView!=NULL)
			{
				CPoint TablePoint;
				TablePoint.x=(Point.x-m_iXExcursion)%m_TableResource.m_wWidth;
				TablePoint.y=(Point.y)%m_TableResource.m_wHeight;
				wChairID=ChairHitTest(TablePoint);
			}
		}

		//������Ϣ
		if ((wTableID==m_wDownTableID)&&(wChairID==m_wDownChairID))
		{
			m_wDownTableID=INVALID_TABLE;
			m_wDownChairID=INVALID_CHAIR;
			m_pITableFrameSink->OnLButtonHitTable(wTableID,wChairID,false);
		}
		else
		{
			m_wDownTableID=INVALID_TABLE;
			m_wDownChairID=INVALID_CHAIR;
		}
	}
*/ //2011-4-1 yy �滻
	if ((m_bLeftMouseDown==true)&&(m_bRightMouseDown==false))
	{
		//���԰���
		::ReleaseCapture();
		m_bLeftMouseDown=false;
		WORD wTableID=TableHitTest(Point);
		ITableView * pITableView=GetTableArrayPtr(m_wDownTableID);
		if (pITableView!=NULL) 
		{
			pITableView->SetMouseDown(false);
			pITableView->SetHoverChairID(INVALID_CHAIR);
		}
		//��ȡλ��
		WORD wChairID=INVALID_CHAIR;
		if (wTableID==m_wDownTableID)
		{
			WORD i = GetViewTableID(wTableID);
			ITableView * pITableView=GetTableArrayPtr(wTableID);
			if (pITableView!=NULL&& i>=0&&i<MAX_TABLE_VIEW_COUT)
			{
				CPoint TablePoint;
				TablePoint.x=Point.x-g_ptTableView[i].x;
				TablePoint.y=Point.y-g_ptTableView[i].y;
				wChairID=ChairHitTest(TablePoint);
			}
		}
		//������Ϣ
		if ((INVALID_CHAIR!=wTableID)&&(wTableID==m_wDownTableID)&&(wChairID==m_wDownChairID))
		{
			m_wDownTableID=INVALID_TABLE;
			m_wDownChairID=INVALID_CHAIR;
			m_pITableFrameSink->OnLButtonHitTable(wTableID,wChairID,false);
		}
		else
		{
			m_wDownTableID=INVALID_TABLE;
			m_wDownChairID=INVALID_CHAIR;
		}
	}

	return;
}

//�����Ϣ
void CTableFrame::OnRButtonDown(UINT nFlags, CPoint Point)
{
	//m_pParentWnd->SetFocus();
	if ((m_bRightMouseDown==false)&&(m_bLeftMouseDown==false))
	{
		SetCapture(m_pParentWnd->GetSafeHwnd());
		m_bRightMouseDown=true;
		m_wDownTableID=TableHitTest(Point);
		if (m_wDownTableID!=INVALID_TABLE)
		{
			ITableView * pITableView=GetTableArrayPtr(m_wDownTableID);
			ASSERT(pITableView!=NULL);
			CPoint TablePoint;
			//TablePoint.x=(Point.x-m_iXExcursion)%m_TableResource.m_wWidth;
			//TablePoint.y=(Point.y)%m_TableResource.m_wHeight;
			TablePoint.x=Point.x-g_ptTableView[m_wDownTableID%MAX_TABLE_VIEW_COUT].x;
			TablePoint.y=Point.y-g_ptTableView[m_wDownTableID%MAX_TABLE_VIEW_COUT].y;
			m_wDownChairID=ChairHitTest(TablePoint);
			if (m_wDownChairID!=INVALID_CHAIR) 
			{
				pITableView->SetMouseDown(m_bRightMouseDown);
				pITableView->SetHoverChairID(m_wDownChairID);
			}
		}
	}
	return;
}

//�����Ϣ
void CTableFrame::OnRButtonUp(UINT nFlags, CPoint Point)
{
	if (m_bRightMouseDown==true)	
	{
		//���԰���
		ReleaseCapture();
		m_bRightMouseDown=false;
		WORD wTableID=TableHitTest(Point);
		ITableView * pITableView=GetTableArrayPtr(m_wDownTableID);
		if (pITableView!=NULL) 
		{
			pITableView->SetMouseDown(false);
			pITableView->SetHoverChairID(INVALID_CHAIR);
		}

		//��ȡλ��
		WORD wChairID=INVALID_CHAIR;
		if (wTableID==m_wDownTableID)
		{
			ITableView * pITableView=GetTableArrayPtr(wTableID);
			if (pITableView!=NULL)
			{
				CPoint TablePoint;
				//TablePoint.x=(Point.x-m_iXExcursion)%m_TableResource.m_wWidth;
				//TablePoint.y=(Point.y)%m_TableResource.m_wHeight;
				TablePoint.x=Point.x-g_ptTableView[wTableID%MAX_TABLE_VIEW_COUT].x;
				TablePoint.y=Point.y-g_ptTableView[wTableID%MAX_TABLE_VIEW_COUT].y;
				wChairID=ChairHitTest(TablePoint);
			}
		}

		//������Ϣ
		if ((wTableID==m_wDownTableID)&&(wChairID==m_wDownChairID))
		{
			m_wDownTableID=INVALID_TABLE;
			m_wDownChairID=INVALID_CHAIR;
			m_pITableFrameSink->OnRButtonHitTable(wTableID,wChairID,false);
		}
		else
		{
			m_wDownTableID=INVALID_TABLE;
			m_wDownChairID=INVALID_CHAIR;
		}
	}

	return;
}

//�����Ϣ
BOOL CTableFrame::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message)
{
	if (NULL == m_pParentWnd)
		return FALSE;

//	if ((m_bRightMouseDown==false)&&(m_bLeftMouseDown==false))
//	{
//		//��������
//		int iTableWidth=m_TableResource.m_wWidth;
//		int iTableHeight=m_TableResource.m_wHeight;
//
//		//���԰���
//		CPoint MousePoint;
//		GetCursorPos(&MousePoint);
//		m_pParentWnd->ScreenToClient(&MousePoint);
//		WORD wNewChairID=INVALID_CHAIR;
//		WORD wNewTableID=TableHitTest(MousePoint);
//		//if(wNewTableID == 1)
//		//{
//		//	return true;
//		//}
//
//		if (wNewTableID<m_wTableCount)
//		{
//			CPoint TablePoint;
//			TablePoint.x=(MousePoint.x-m_iXExcursion)%iTableWidth;
//			TablePoint.y=(MousePoint.y)%iTableHeight;
//			wNewChairID=ChairHitTest(TablePoint);
//		}
//
//		//���½���
//		if (((wNewTableID!=m_wDownTableID)||(wNewChairID!=m_wDownChairID))
//			&&((wNewTableID!=INVALID_CHAIR)||(m_wDownChairID!=INVALID_CHAIR)))
//		{
//			if (m_wDownTableID!=INVALID_TABLE)
//			{
//				GetTableArrayPtr(m_wDownTableID)->SetHoverChairID(INVALID_CHAIR);
//			}
//			if (wNewTableID!=INVALID_TABLE) 
//			{
//				GetTableArrayPtr(wNewTableID)->SetHoverChairID(wNewChairID);
//			}
//		}
//
//		//��������
//		m_wDownTableID=wNewTableID;
//		m_wDownChairID=wNewChairID;
//		if ((m_wDownTableID!=INVALID_TABLE)&&(m_wDownChairID!=INVALID_CHAIR))
//		{
//			SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
//			return TRUE;
//		}
//	}

	if ((m_bRightMouseDown==false)&&(m_bLeftMouseDown==false))
	{
		//��������
		int iTableWidth=m_TableResource.m_wWidth;
		int iTableHeight=m_TableResource.m_wHeight;

		//���԰���
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		m_pParentWnd->ScreenToClient(&MousePoint);
		WORD wNewChairID=INVALID_CHAIR;
		WORD wNewTableID=TableHitTest(MousePoint);
		if (wNewTableID != INVALID_TABLE)
		{
			int i = GetViewTableID(wNewTableID);
			if (i>=0 && i<MAX_TABLE_VIEW_COUT)
			{
				CPoint TablePoint(g_ptTableView[i]);
				TablePoint.x=MousePoint.x-g_ptTableView[i].x;
				TablePoint.y=MousePoint.y-g_ptTableView[i].y;
				wNewChairID=ChairHitTest(TablePoint);
			}
		}
		//���½���
		if (((wNewTableID!=m_wDownTableID)||(wNewChairID!=m_wDownChairID))
			&&((wNewTableID!=INVALID_CHAIR)||(m_wDownChairID!=INVALID_CHAIR)))
		{
			if (m_wDownTableID!=INVALID_TABLE)
			{
				GetTableArrayPtr(m_wDownTableID)->SetHoverChairID(INVALID_CHAIR);
			}
			if (wNewTableID!=INVALID_TABLE) 
			{
				GetTableArrayPtr(wNewTableID)->SetHoverChairID(wNewChairID);
			}
		}
		//��������
		m_wDownTableID=wNewTableID;
		m_wDownChairID=wNewChairID;
		if ((m_wDownTableID!=INVALID_TABLE)&&(m_wDownChairID!=INVALID_CHAIR))
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
			return TRUE;
		}
	}
	return FALSE;
}

//���Ӳ���
WORD CTableFrame::TableHitTest(POINT MousePoint)
{

// 	int iTableWidth=m_TableResource.m_wWidth;
// 	int iTableHeight=m_TableResource.m_wHeight;
// 	if ((iTableWidth!=0)&&(iTableHeight!=0))
// 	{
// 		int iTableRow=(MousePoint.x-m_iXExcursion)/iTableWidth;
// 		int iTableLine=MousePoint.y/iTableHeight;
// 		if ((iTableRow>=0)&&(iTableRow<m_iCountRow)&&(iTableLine<3))
// 		{
// 			WORD wTableID=iTableLine*m_iCountRow+iTableRow;
// 			if (wTableID<m_wTableCount) 
// 			{
// 				if ((wTableID>0)&&(m_bHideInfo==true)&&(wTableID!=1)) return INVALID_TABLE;
// 				return wTableID;
// 			}
// 		}
// 	}
	int iTableWidth=m_TableResource.m_wWidth;
	int iTableHeight=m_TableResource.m_wHeight;
	CRect rect(0, 0, iTableWidth, iTableHeight);
	if ((iTableWidth!=0)&&(iTableHeight!=0))
	{
		for (WORD i=0; i<MAX_TABLE_VIEW_COUT; i++)
		{
			rect.MoveToXY(g_ptTableView[i]);
			if (rect.PtInRect(MousePoint))
			{
				if (m_pCurTable[i])
				{
					const tagTableState* pTableState = m_pCurTable[i]->GetTableAttrib();
					if (NULL != pTableState)
					{
						return pTableState->wTableID;
					}
				}
			}
		}
	}
	return INVALID_TABLE;
}

//���Ӳ���
WORD CTableFrame::ChairHitTest(POINT MousePoint)
{
	//CString str;
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_TableResource.m_rcChair[i].PtInRect(MousePoint)) 
		{
// 			str.Format(TEXT("ChiairHitText point:%d, %d, ChiarID:[%d]\n"),MousePoint.x, MousePoint.y, i);
// 			TRACE(str);
			return i;
		}
	}
// 	str.Format(TEXT("ChiairHitText point:%d, %d, ChiarID:[%d]\n"),MousePoint.x, MousePoint.y, INVALID_CHAIR);
// 	TRACE(str);
	return INVALID_CHAIR;
}

//��ԴĿ¼
void CTableFrame::GetGameResDirectory(const tagGameKind * pGameKind, TCHAR szResDirectory[], WORD wBufferCount)
{
	//��������
	WORD wStringIndex=0;
	//����Ŀ¼
	ZeroMemory(szResDirectory,wBufferCount*sizeof(TCHAR));
	lstrcpyn(szResDirectory,(LPCTSTR)pGameKind->szProcessName,wBufferCount);
	while ((szResDirectory[wStringIndex]!=0)&&(szResDirectory[wStringIndex]!=TEXT('.'))) wStringIndex++;
	//�ַ���ֹ
	szResDirectory[wStringIndex]=0;
	return;
}

//��һҳ
void CTableFrame::OnBnUp()
{
	int iPage=GetPage();
	//��ҳ�ж�
	if(GetPageNum() == 0 && iPage == 0)return;
	if(iPage > 0)
	{
		iPage--;
		SetPage(iPage);
		UpdatePage();
		if (m_pParentWnd)
			m_pParentWnd->Invalidate(FALSE);
	}
}
//��һҳ
void CTableFrame::OnBnDwon()
{
	int iPage=GetPage();
	if( GetAllLine() / MAX_TABLE_VIEW_COUT > iPage)
	{
		iPage++;
		SetPage(iPage);
		UpdatePage();
		if (m_pParentWnd)
			m_pParentWnd->Invalidate(FALSE);
	}
}
//����ҳ��
void CTableFrame::UpdatePage()
{
	WORD i=0;
	
	for (WORD k=0; k<MAX_TABLE_VIEW_COUT; k++)
	{
		m_pCurTable[k]=0;
	}

	for (; i<(m_iPage*MAX_TABLE_VIEW_COUT) && i<m_wTableCount; i++);
	for (WORD j=0; j<MAX_TABLE_VIEW_COUT && i<m_wTableCount; j++, i++)
	{
		m_pCurTable[j]=((CTableView*)(m_pTableViewArray+i));
	}
}

int CTableFrame::OpenDesignationPage(int iTableID)
{
	int iPage = iTableID/MAX_TABLE_VIEW_COUT;
	if (0>iPage  ||  m_iMaxPage<iPage  ||  iPage==m_iPage)
		return 0;

	SetPage(iPage);
	UpdatePage();
	
	if (m_pParentWnd)
	{
		m_pParentWnd->Invalidate(FALSE);
	}
}

//ͨ�����Ӻŵõ���ͼλ��
WORD CTableFrame::GetViewTableID(WORD wTableId)
{
	for (WORD i=0; i<MAX_TABLE_VIEW_COUT; i++)
	{
		if (m_pCurTable[i] && wTableId == m_pCurTable[i]->GetTableAttrib()->wTableID)
		{
			return i;
		}
	}
	return INVALID_TABLE;
}
//////////////////////////////////////////////////////////////////////////

// void CTableFrame::OnDestroy()
// {
// 	__super::OnDestroy();
// 
// 	// TODO: �ڴ˴������Ϣ����������
// }
