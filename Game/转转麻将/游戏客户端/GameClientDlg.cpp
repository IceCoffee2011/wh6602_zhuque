#include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientDlg.h"
#include "GameLogic.h"
#include "CrashRptFaceExp.h"
//////////////////////////////////////////////////////////////////////////

//��Ϸ��ʱ��
#define IDI_START_GAME				200									//��ʼ��ʱ��
#define IDI_OPERATE_CARD			202									//������ʱ��
#define IDI_OPERATE_HAI_DI			203									//���׶�ʱ��
#define IDI_USER_TING_CARD			204									//���ƶ�ʱ��
#define IDI_OTHER_USEROUTCARD		205									//��ʵ���ƶ�ʱ��
#define IDI_GAME_END                206                                 //������ʱ��
#define IDI_SHOW_SZ                 207                                 //���Ӷ�ʱ��
#define IDI_HUANGZHUANG             208                                 //��ׯ��ʱ��
#define IDI_SOUND                   209                                 //������ʱ��
#define IDI_SZSTART                 210                                 //����ɸ�Ӷ�ʱ��
#define IDI_SHOW_DIRECT             211                                 //��Ϊ����
#define	IDI_SELECT_OUT_CARD		212								//ѡ�����


//��Ϸ��ʱ��
#define TIME_START_GAME				15									//��ʼ��ʱ��
#define TIME_HEAR_STATUS			15									//���ƶ�ʱ��
#define TIME_OPERATE_CARD			10									//������ʱ��
#define TIME_OPERATE_HAI_DI			10									//������ʱ��
#define TIME_USERTINGCARD			5									//������ʱ��
#define TIME_GAME_END				3500								//��������ʾʱ��
#define TIME_SHOW_SZ				100								    //����ʱ��
#define TIME_SHOW_DIRECT            100                                 //��Ϊ����ʱ�� 
#define TIME_SELECT_OUT_CARD	  5										//ѡ��ʱ��


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_TING_CARD,OnTingCard)
	ON_MESSAGE(IDM_CARD_OPERATE,OnCardOperate)
	//ON_MESSAGE(IDM_OPERATE_HAI_DI,OnOperateHaiDi)
	ON_MESSAGE(IDM_TRUSTEE_CONTROL,OnStusteeControl)
	ON_MESSAGE(IDM_SHOW_SZ,OnShowSZ)
	ON_MESSAGE(IDM_MOPAI,OnMoPai)
	ON_MESSAGE(IDM_SZSTART,OnSZStart)
	ON_MESSAGE(IDM_SZ_BUTTON, OnSZFlashBT)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView, &m_ScoreView)
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//״̬����
	//m_bHearStatus=false;
	m_bIsMeCanOutCard = false;

	//�йܱ���
	m_bStustee=false;
	m_wTimeOutCount =0;

	//��������
	m_cbGender=0;
	m_InfoSound="";

	//��������
	m_wHeapHead=0;
	m_wHeapTail=0;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));	

	m_byCanSelectType = WIK_NULL;
	m_byCanSelectCount = 0;
	ZeroMemory(m_byCanSelectCard,sizeof(m_byCanSelectCard));

	//���Ա���
	m_sDialect="GY"; 

	return;
}

//��������
CGameClientDlg::~CGameClientDlg()
{
	//SafeDelete(m_FlashControl);
}

//��ʼ����
bool CGameClientDlg::InitGameFrame()
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//tagWeaveItem WeaveItem[4];
	//memset(WeaveItem, 0, sizeof(tagWeaveItem)*4);
	////WeaveItem[0].cbCenterCard = 0x21;
	////WeaveItem[0].cbPublicCard = FALSE;
	////WeaveItem[0].wProvideUser = -1;
	////WeaveItem[0].cbWeaveKind = WIK_GANG;

	////WeaveItem[1].cbCenterCard = 0x03;
	////WeaveItem[1].cbPublicCard = FALSE;
	////WeaveItem[1].wProvideUser = -1;
	////WeaveItem[1].cbWeaveKind = WIK_LEFT;

	////WeaveItem[2].cbCenterCard = 0x07;
	////WeaveItem[2].cbPublicCard = FALSE;
	////WeaveItem[2].wProvideUser = -1;
	////WeaveItem[2].cbWeaveKind = WIK_GANG;

	//tagChiHuResult result;
	//memset(&result, 0, sizeof(tagChiHuResult));
	//BYTE byCardData[13]={0x02,0x02,0x04,0x04,0x04,0x08,0x08,0x17,0x17,0x17,0x18,0x18,0x18};
	//BYTE m_cbCardIndex[MAX_INDEX]={0};
	//m_GameLogic.SwitchToCardIndex(byCardData,13,m_cbCardIndex);
	//BYTE byHuType = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex,WeaveItem,0,0x02,0,result);
	//BYTE bySaveHuType = byHuType;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//���ñ���
	//SetWindowText(TEXT("תת�齫  --  Ver��1.0.5.3"));

	//���Ը�ʽ
	//m_sDialect.Format("%s_",m_strLanguage.c_str()); 

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//������Դ
	//g_CardResource.LoadResource();
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	//m_GameClientView.m_btHaiDi.ShowWindow(SW_HIDE);
	//m_GameClientView.m_btNoHaiDi.ShowWindow(SW_HIDE);
	//m_GameClientView.m_btTingCard.ShowWindow(SW_HIDE);
	//m_GameClientView.m_btBuTing.ShowWindow(SW_HIDE);

	m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_HIDE);
	//m_ScoreView.ShowWindow(SW_SHOW);

	m_GameClientView.SetGameClientDlgPtr(this);
	//m_GameClientView.m_FlashControl.ShowWindow(SW_HIDE);
	//m_GameClientView.m_ControlWnd.ShowWindow(SW_SHOW);


	//m_ScoreView.ShowWindow(SW_SHOW);
	//m_ScoreView.SetCountDown(true);

	////��ʾ����
	//BYTE byMask = WIK_NULL;
	////byMask = WIK_CHI_HU;
	////byMask |= WIK_PENG;
	//byMask |= WIK_LEFT;


	//tagGangCardResult bucardresult;
	//BYTE cbCardData[4]={0x11, 0x11, 0x11,0x11 };
	//::memset(&bucardresult, 0, sizeof(tagGangCardResult));
	//memcpy(bucardresult.cbCardData,cbCardData,sizeof(cbCardData));
	//bucardresult.cbCardCount =0;
	//m_GameClientView.m_ControlWnd.SetControlInfo(0x12,byMask,bucardresult);
	//m_GameClientView.m_ControlWnd.AddGangCardData(NULL, 0);
	//m_GameClientView.m_ControlWnd.ShowWindow(SW_SHOW);


	//HWND hwnd=AfxGetMainWnd()->m_hWnd;
	//DWORD proID;//�������ID
	//GetWindowThreadProcessId(hwnd,&proID);//��ȡ����ID
	//char chData[128]={0};
	//sprintf(chData, "%d", proID);
	//InsertSystemString(chData);



	CString strPicPath="";
	TCHAR szResourcePath[MAX_PATH]={0};
	GetCurrentDirectory(MAX_PATH,szResourcePath);
	strPicPath.Append(szResourcePath);
	strPicPath.Append(TEXT("\\SparrowZZ\\Picture\\PNG\\Cartoon\\"));
	//m_FlashControl.Create(IDD_DLG_GAME_FLASH, this);
	//m_FlashControl.InitFlash(strPicPath + "ActChi.png", 12, CPoint(250, 200));


	m_GameClientView.InitViewFrame();

	//SetTaskWndShow(true);
	//������Ϸ��������
	PlayMusic("SparrowZZ.mp3");	

	SetChatShowTag(true,false);

	m_GameClientView.PlayDirectFlash(false);
	m_GameClientView.PlaySZFlashBT(false);
	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//״̬����
	//m_bHearStatus=false;
	m_bIsMeCanOutCard = false;

	//�йܱ���
	m_bStustee=false;
	m_wTimeOutCount =0;

	//��������
	m_cbGender=0;
	m_InfoSound="";

	//��������
	m_wHeapHead=0;
	m_wHeapTail=0;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	m_byCanSelectType = WIK_NULL;
	m_byCanSelectCount = 0;
	ZeroMemory(m_byCanSelectCard,sizeof(m_byCanSelectCard));
	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	//��������
	CGameOption GameOption;
	GameOption.m_bEnableSound=IsEnableSound();

	//��������
	if (GameOption.DoModal()==IDOK)
	{
		EnableSound(GameOption.m_bEnableSound);
	}

	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	//return true;

	if (IsLookonMode())
	{
		return true;
	}

	switch (nTimerID)
	{
	case IDI_SZSTART:     //����ɸ�Ӷ�ʱ��
		{
			if (nElapse==0)
			{
				WORD wMeChairID=GetMeChairID();
				if (wChairID == m_wBankerUser && wMeChairID==m_wBankerUser)
				{
					AfxGetMainWnd()->SendMessage(IDM_SZ_BUTTON, 0, 0);
				}
				return false;
			}

			return true;
		}
	case IDI_START_GAME:		//��ʼ��Ϸ
		{
			if (nElapse==0)
			{
				PostMessage(WM_CLOSE);
				return false;
			}
			else if (nElapse<=5) 
			{
				PlaySound("GAME_WARN",false);
			}

			return true;
		}
	case IDI_OPERATE_CARD:		//������ʱ��
		{
			//��ʱ�ж�
			if ((nElapse==0)||(m_bStustee==true))
			{
				//��ȡλ��
				WORD wMeChairID=GetMeChairID();

				//��������
				if (wChairID==wMeChairID && wMeChairID==m_wCurrentUser)
				{
					if((m_bStustee==false)/*&&(m_bHearStatus==false)*/)
					{
						m_wTimeOutCount++;
						if(m_wTimeOutCount==3)
						{
							InsertSystemString(TEXT("��������γ�ʱ���л�Ϊ��ϵͳ�йܡ�ģʽ."));
							OnStusteeControl(0,0);
						}
					}

					if( TRUE==m_GameClientView.m_ControlWnd.m_bthu.IsWindowVisible() 
						&& TRUE==m_GameClientView.m_ControlWnd.m_bthu.IsWindowEnabled())
					{
						OnCardOperate(WIK_CHI_HU,0);
					}
					else if (m_wCurrentUser==wMeChairID)
					{
						if (WIK_NULL != m_byUserAction[wMeChairID])
						{
							OnCardOperate(WIK_NULL,0);
						}
						else
						{
							BYTE cbCardData=m_GameClientView.m_HandCardControl.GetCurrentCard();
							OnOutCard(cbCardData,cbCardData);
						}
					}
					return true;
				}
			}

			//��������
			if ((nElapse<=3)&&(wChairID==GetMeChairID())) 
			{
				PlaySound("GAME_WARN",false);
			}

			return true;
		}
	case IDI_OTHER_USEROUTCARD:		//�������ƶ�ʱ��
		{
			//��ʱ�ж�
			if ((nElapse==0)||(m_bStustee==true))
			{
				//��ȡλ��
				WORD wMeChairID=GetMeChairID();

				//��������
				if (wChairID==wMeChairID && wMeChairID==m_wCurrentUser)
				{
					if((m_bStustee==false)/*&&(m_bHearStatus==false)*/)
					{
						m_wTimeOutCount++;
						if(m_wTimeOutCount==3)
						{
							InsertSystemString(TEXT("��������γ�ʱ���л�Ϊ��ϵͳ�йܡ�ģʽ."));
							OnStusteeControl(0,0);
						}
					}
					if (m_wCurrentUser==wMeChairID)
					{
						BYTE cbCardData=m_GameClientView.m_HandCardControl.GetCurrentCard();
						OnOutCard(cbCardData,cbCardData);
					}
					return true;
				}
			}

			//��������
			if ((nElapse<=3)&&(wChairID==GetMeChairID())) 
			{
				PlaySound("GAME_WARN",false);
			}

			return true;
		}
		//case IDI_OPERATE_HAI_DI:	//���׶�ʱ��
		//	{
		//		//��ʱ�ж�
		//		if (nElapse==0 || m_bStustee==true)
		//		{
		//			if (wChairID==m_wCurrentUser && wChairID==GetMeChairID())
		//			{
		//				OnOperateHaiDi(0,0);
		//			}
		//			return false;
		//		}

		//		//��������
		//		if ((nElapse<=3)&&(wChairID==GetMeChairID())) 
		//		{
		//			PlaySound("GAME_WARN",false);
		//		}

		//		return true;
		//	}
	case IDI_USER_TING_CARD:		//���ƶ�ʱ��
		{
			if (0==nElapse || m_bStustee==true)
			{
				if (GetMeChairID()==m_wCurrentUser)
				{
					//���Ͳ�����Ϣ
					CMD_UserTingOperate TingOperate;
					TingOperate.bTingCard = false;
					SendData(SUB_C_USERTINGCARD,&TingOperate,sizeof(TingOperate));
					//m_GameClientView.m_btTingCard.ShowWindow(SW_HIDE);
					//m_GameClientView.m_btBuTing.ShowWindow(SW_HIDE);
				}
			}
			//��������
			if ((nElapse<=3)&&(wChairID==GetMeChairID())) 
			{
				PlaySound("GAME_WARN",false);
			}
			return true;
		}
	case IDI_SELECT_OUT_CARD:	// ѡ�ƶ�ʱ��
		{
			if (0==nElapse || m_bStustee==true)
			{
				SendCardCommand(m_byCanSelectType, m_byCanSelectCard[0]);
			}

			//��������
			if ((nElapse<=3)&&(wChairID==GetMeChairID())) 
			{
				PlaySound("GAME_WARN",false);
			}

			return true;
		}
	}

	return false;
}

//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
}

//������Ϣ
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_C_SZSTART:			//����ɫ�Ӷ���
		{
			return OnSubStartSZFlash();
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_OUT_CARD:		//�û�����
		{
			return OnSubOutCard(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:		//������Ϣ
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_OPERATE_NOTIFY:	//������ʾ
		{
			return OnSubOperateNotify(pBuffer,wDataSize);
		}
	case SUB_S_OPERATE_RESULT:	//�������
		{
			return OnSubOperateResult(pBuffer,wDataSize);
		}
		//case SUB_S_OPERATE_HAI_DI:	//���ײ���
		//	{
		//		return OnSubOperateHaiDi(pBuffer,wDataSize);
		//	}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_TRUSTEE:			//�û��й�
		{
			return OnSubTrustee(pBuffer,wDataSize);
		}
		//case SUB_S_TING_CARD:      //���Ʊ��
		//	{
		//		return OnSubTingCard(pBuffer,wDataSize);			
		//	}
		//case SUB_S_USER_TINGCARD_RESULT:	//���ƵĽ��
		//	{
		//		return OnSubTingCardResult(pBuffer, wDataSize);
		//	}
	}

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	CString str;
	str.Format("Test:CGameClientDlg::OnGameSceneMessage -%d", cbGameStation);
	OutputDebugString(str);

	switch (cbGameStation)
	{
	case GS_MJ_FREE:	//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//��������
			m_wBankerUser=pStatusFree->wBankerUser;
			m_GameClientView.SetCellScore(pStatusFree->lCellScore);
			m_GameClientView.m_HandCardControl.SetDisplayItem(true);
			m_bIsMeCanOutCard = false;

			////���ý���
			//for (WORD i=0;i<GAME_PLAYER;i++)
			//{
			//	m_cbHeapCardInfo[i][0]=0;
			//	m_cbHeapCardInfo[i][1]=HEAP_FULL_COUNT-(((i==m_wBankerUser)||((i+2)%4==m_wBankerUser))?28:26);
			//	m_GameClientView.m_HeapCard[SwitchViewChairID(i)].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],HEAP_FULL_COUNT);
			//}

			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				//m_ScoreView.ShowWindow(SW_HIDE);
				SafeShowOrHideWnd(&m_ScoreView, SW_HIDE);
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.EnableWindow(TRUE);
				m_GameClientView.m_btStart.SetFocus();
				m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_HIDE);
				m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_HIDE);
				SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
			}
			//�й�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetTrustee(SwitchViewChairID(i),pStatusFree->bTrustee[i]);
			}

			// �йܰ�ť����
			if(IsLookonMode()==true)
			{
				m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_HIDE);
				m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_HIDE);
				m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
			}

			//���½���
			m_GameClientView.UpdateGameView(NULL);

			return true;
		}
	case GS_MJ_PLAY:	//��Ϸ״̬(�����ػ�)
		//case GS_MJ_HAI_DI:	//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay))
			{
				return false;
			}
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;
			if (NULL==pStatusPlay)
			{
				return false;
			}

			WORD byMeChairID = GetMeChairID();
			bool bIsMeIsLookonMode = IsLookonMode();

			//���ñ���
			m_bIsMeCanOutCard = false;
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_cbLeftCardCount=pStatusPlay->cbLeftCardCount;
			//m_bHearStatus=(pStatusPlay->cbHearStatus>0)?true:false;
			memset(m_byUserAction, WIK_NULL, sizeof(m_byUserAction));
			//m_cbTempKing[0] = pStatusPlay->byKindCard;

			//��ʷ����
			m_wOutCardUser=pStatusPlay->wOutCardUser;
			m_cbOutCardData=pStatusPlay->cbOutCardData;
			CopyMemory(m_cbDiscardCard,pStatusPlay->cbDiscardCard,sizeof(m_cbDiscardCard));
			CopyMemory(m_cbDiscardCount,pStatusPlay->cbDiscardCount,sizeof(m_cbDiscardCount));

			//�˿˱���
			CopyMemory(m_cbWeaveCount,pStatusPlay->cbWeaveCount,sizeof(m_cbWeaveCount));
			CopyMemory(m_WeaveItemArray,pStatusPlay->WeaveItemArray,sizeof(m_WeaveItemArray));
			m_GameLogic.SwitchToCardIndex(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,m_cbCardIndex);

			//��������
			WORD wViewChairID[GAME_PLAYER]={0,0,0,0};
			for (WORD i=0;i<GAME_PLAYER;i++) 
			{
				wViewChairID[i]=SwitchViewChairID(i);
			}

			//��������
			m_GameClientView.SetCellScore(pStatusPlay->lCellScore);
			m_GameClientView.SetBankerUser(wViewChairID[m_wBankerUser]);
			m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);


			////���ÿؼ�
			//if (IsLookonMode()==false)
			//{
			//	m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_SHOW);
			//	m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_HIDE);
			//}
			//
			//�й�,��������
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetTrustee(SwitchViewChairID(i),pStatusPlay->bTrustee[i]);
			}

			//����˿�
			BYTE cbWeaveCard[4]={0,0,0,0};
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				for (BYTE j=0;j<m_cbWeaveCount[i];j++)
				{
					BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
					BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;
					BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbCenterCard,cbWeaveCard);
					m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetCardData(cbWeaveCard,cbWeaveCardCount);
					if (cbWeaveKind&(WIK_GANG|WIK_FILL|WIK_TING))
					{
						if (m_WeaveItemArray[i][j].wProvideUser==i)
						{
							m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetDisplayItem(false);
						}
					}
				}
			}

			////�û��˿�
			//if (m_wCurrentUser==byMeChairID && cbGameStation!=GS_MJ_HAI_DI)
			//{
			//	BYTE cbCardCount=pStatusPlay->cbCardCount;
			//	tagCardItem CardItem;
			//	CardItem.cbCardData = pStatusPlay->cbCardData[0];
			//	CardItem.bShoot = false;
			//	CardItem.bKingFlag = false;//(pStatusPlay->cbCardData[0] == m_cbTempKing[0])?true:false;
			//	m_GameClientView.m_HandCardControl.SetCurrentCard(CardItem);
			//	m_GameClientView.m_HandCardControl.SetCardData(pStatusPlay->cbCardData,cbCardCount-1,CardItem.cbCardData/*,pStatusPlay->byKindCard*/);
			//}
			//else
			{
				m_GameClientView.m_HandCardControl.SetCardData(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,0/*,pStatusPlay->byKindCard*/); 
			}

			//�˿�����
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//�û��˿�
				if (i!=byMeChairID)
				{
					BYTE cbCardCount=13-m_cbWeaveCount[i]*3;
					WORD wUserCardIndex=(wViewChairID[i]<2)?wViewChairID[i]:2;
					m_GameClientView.m_UserCard[wUserCardIndex].SetCardData(cbCardCount,(m_wCurrentUser==i));
				}

				//�����˿�
				m_GameClientView.m_DiscardCard[wViewChairID[i]].SetCardData(m_cbDiscardCard[i],m_cbDiscardCount[i]);
			}

			m_GameClientView.m_HandCardControl.SetPositively(bIsMeIsLookonMode?false:true);
			m_GameClientView.m_HandCardControl.SetDisplayItem(bIsMeIsLookonMode?false:true);

			//�����˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_cbHeapCardInfo[i][0]=0;
				m_cbHeapCardInfo[i][1]=HEAP_FULL_COUNT-(((i==m_wBankerUser)||((i+2)%4==m_wBankerUser))?28:26);
			}

			//�ַ��˿�
			BYTE cbTakeCount=108-m_cbLeftCardCount;  //53��
			BYTE cbSiceFirst=HIBYTE(pStatusPlay->wSiceCount);
			BYTE cbSiceSecond=LOBYTE(pStatusPlay->wSiceCount);
			BYTE cbSiceCount=__min(cbSiceFirst,cbSiceSecond);
			WORD wTakeChairID=(m_wBankerUser-cbSiceFirst-cbSiceSecond+1+GAME_PLAYER*GAME_PLAYER)%GAME_PLAYER;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//������Ŀ
				BYTE cbValidCount=HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?cbSiceCount*2:0);
				BYTE cbRemoveCount=__min(cbValidCount,cbTakeCount);

				//��ȡ�˿�
				cbTakeCount-=cbRemoveCount;
				m_cbHeapCardInfo[wTakeChairID][(i==0)?1:0]+=cbRemoveCount;

				//if ((cbGameStation==GS_MJ_HAI_DI) && (0==i) && (cbRemoveCount>0 && cbRemoveCount<HEAP_FULL_COUNT))
				//{
				//	m_cbHeapCardInfo[wTakeChairID][0]=HEAP_FULL_COUNT-cbRemoveCount-1;
				//	m_cbHeapCardInfo[wTakeChairID][1]=cbRemoveCount;
				//}

				//����ж�
				if (cbTakeCount==0)
				{
					m_wHeapHead=wTakeChairID;
					m_wHeapTail=(m_wBankerUser-cbSiceFirst-cbSiceSecond+1+GAME_PLAYER*GAME_PLAYER/*7-cbSiceFirst*/)%GAME_PLAYER;
					break;
				}

				//�л�����
				wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
			}
			//��������
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.m_HeapCard[wViewChairID[i]].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],HEAP_FULL_COUNT);
			}

			//��ʷ�˿�
			if (m_wOutCardUser!=INVALID_CHAIR)
			{
				WORD wOutChairID=SwitchViewChairID(m_wOutCardUser);
				m_GameClientView.SetOutCardInfo(wOutChairID,m_cbOutCardData);
			}

			//�йܰ�ť
			m_bStustee = pStatusPlay->bTrustee[GetMeChairID()];
			if (m_bStustee)
			{
				m_GameClientView.m_btAutoPlayOff.ShowWindow(true);
			}
			else
			{
				m_GameClientView.m_btAutoPlayOn.ShowWindow(true);
			}
			//m_GameClientView.m_btStusteeControl.InvalidateRect(NULL);

			m_GameClientView.m_iCountDirectIndex=I_DIRECT_FLASH_TOTOL_NUMS;
			//��������
			if (cbGameStation==GS_MJ_PLAY && !bIsMeIsLookonMode)
			{
				if (pStatusPlay->bIsPlayDice)
				{
					KillGameTimer(IDI_SZSTART);
					SetGameTimer(m_wBankerUser,IDI_SZSTART,5);
					if (byMeChairID==m_wBankerUser)
					{
						m_GameClientView.PlaySZFlashBT();
					}
				}
				else
				{
					if (pStatusPlay->m_bTingCard)
					{
						//m_bHearStatus = false;
						//m_GameClientView.m_btTingCard.ShowWindow(SW_SHOW);	
						//m_GameClientView.m_btBuTing.ShowWindow(SW_SHOW);
						m_wCurrentUser = byMeChairID;
						//����
						SetGameTimer(m_wCurrentUser, IDI_USER_TING_CARD, TIME_USERTINGCARD);
					}
					else
					{
						BYTE byViewID = SwitchViewChairID(m_wOutCardUser);

						if (byMeChairID == m_wCurrentUser)
						{//�������
							if (pStatusPlay->cbActionMask!=WIK_NULL)
							{//����,�������
								m_byUserAction[byMeChairID] = pStatusPlay->cbActionMask;
								//��ȡ����
								BYTE cbActionMask=pStatusPlay->cbActionMask;
								BYTE cbActionCard=pStatusPlay->cbActionCard;

								//��������
								tagGangCardResult GangCardResult;
								ZeroMemory(&GangCardResult,sizeof(GangCardResult));
								//��������
								tagGangCardResult bucardresult;
								ZeroMemory(&bucardresult,sizeof(tagGangCardResult));
								//��ʾ����
								::memcpy(bucardresult.cbCardData, pStatusPlay->byBuCard, sizeof(BYTE)*pStatusPlay->byBuCardNums);
								bucardresult.cbCardCount = pStatusPlay->byBuCardNums;
								//m_GameClientView.m_ControlWnd.SetControlInfo(pStatusPlay->cbActionCard,cbActionMask,bucardresult);
								//m_GameClientView.m_ControlWnd.AddGangCardData(pStatusPlay->byGangCard, pStatusPlay->byGangCardNums);

								m_byCanSelectCount = pStatusPlay->byBuCardNums;
								memcpy(m_byCanSelectCard, pStatusPlay->byBuCard, sizeof(m_byCanSelectCard));
								m_GameClientView.m_ControlWnd.SetAction(pStatusPlay->cbActionCard, cbActionMask);

								//if (m_bHearStatus)
								//{
								//	if (pStatusPlay->cbActionMask&WIK_CHI_HU)
								//	{
								//		//�����
								//		OnCardOperate(WIK_CHI_HU,pStatusPlay->cbActionCard);
								//		return true;
								//	}
								//	//ȡ��,���ҳ���,�����ö�ʱ������.
								//	OnCardOperate(WIK_NULL,0);
								//	BYTE cbCardData=m_GameClientView.m_HandCardControl.GetCurrentCard();
								//	OnOutCard(cbCardData,cbCardData);
								//}
								//else
								{

									m_GameClientView.SetOutCardInfo(byViewID,m_cbOutCardData);

									//���ö�ʱ������
									m_bIsMeCanOutCard  = true;
									SetGameTimer(byMeChairID,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
								}
							}
							else
							{//����

								m_GameClientView.SetOutCardInfo(byViewID,m_cbOutCardData);

								//����
								m_byUserAction[byMeChairID] = WIK_NULL;
								m_wCurrentUser = byMeChairID;
								memset(m_byUserAction, 0, sizeof(m_byUserAction));
								SetGameTimer(byMeChairID,IDI_OTHER_USEROUTCARD,TIME_HEAR_STATUS-10);
								//������ʾ
								//m_GameClientView.SetStatusFlag(true,false);
							}
						}
						else
						{
							m_GameClientView.SetOutCardInfo(byViewID,m_cbOutCardData);

							if ((pStatusPlay->cbActionMask!=WIK_NULL) && (!pStatusPlay->bIsAlreadyOper))
							{//����
								//��ȡ����
								BYTE cbActionMask=pStatusPlay->cbActionMask;
								BYTE cbActionCard=pStatusPlay->cbActionCard;

								m_byUserAction[byMeChairID] = pStatusPlay->cbActionMask;
								//��������
								tagGangCardResult GangCardResult;
								ZeroMemory(&GangCardResult,sizeof(GangCardResult));
								//��������
								tagGangCardResult bucardresult;
								ZeroMemory(&bucardresult,sizeof(tagGangCardResult));
								//��ʾ����
								::memcpy(bucardresult.cbCardData, pStatusPlay->byBuCard, sizeof(BYTE)*pStatusPlay->byBuCardNums);
								bucardresult.cbCardCount = pStatusPlay->byBuCardNums;
								//m_GameClientView.m_ControlWnd.SetControlInfo(pStatusPlay->cbActionCard,cbActionMask,bucardresult);
								//m_GameClientView.m_ControlWnd.AddGangCardData(pStatusPlay->byGangCard, pStatusPlay->byGangCardNums);
								m_byCanSelectCount = pStatusPlay->byBuCardNums;
								memcpy(m_byCanSelectCard, pStatusPlay->byBuCard, sizeof(m_byCanSelectCard));
								m_GameClientView.m_ControlWnd.SetAction(pStatusPlay->cbActionCard, cbActionMask);

								//���ö�ʱ������
								m_wCurrentUser = byMeChairID;
								SetGameTimer(byMeChairID,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
							}
						}
					}
				}
			}
			////��������
			//else if (cbGameStation==GS_MJ_HAI_DI && !bIsMeIsLookonMode)
			//{
			//	//m_GameClientView.m_btHaiDi.ShowWindow(SW_HIDE);
			//	//m_GameClientView.m_btNoHaiDi.ShowWindow(SW_HIDE);
			//	//���ÿ���
			//	if (byMeChairID==m_wCurrentUser)
			//	{
			//		//m_GameClientView.m_btHaiDi.ShowWindow(SW_SHOW);
			//		//m_GameClientView.m_btNoHaiDi.ShowWindow(SW_SHOW);
			//	}
			//	//����ʱ��
			//	SetGameTimer(m_wCurrentUser,IDI_OPERATE_HAI_DI,TIME_OPERATE_HAI_DI);
			//}

			// �йܰ�ť����
			if(bIsMeIsLookonMode==true)
			{
				//m_GameClientView.m_btHaiDi.ShowWindow(SW_HIDE);
				//m_GameClientView.m_btNoHaiDi.ShowWindow(SW_HIDE);
				m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_HIDE);
				m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_HIDE);
			}
			m_GameClientView.m_btStart.ShowWindow(SW_HIDE);

			//���½���
			m_GameClientView.UpdateGameView(NULL);
			return true;
		}
	}

	return false;
}

//������ж�ʱ��
void CGameClientDlg::ClearAllTimer()
{
	KillGameTimer(IDI_START_GAME);
	KillGameTimer(IDI_OPERATE_CARD);
	KillGameTimer(IDI_OPERATE_HAI_DI);
	KillGameTimer(IDI_USER_TING_CARD);
	KillGameTimer(IDI_OTHER_USEROUTCARD);
	KillGameTimer(IDI_SZSTART);
	KillGameTimer(IDI_SELECT_OUT_CARD);
}

void CGameClientDlg::SendCardCommand(BYTE byOperatorCode, BYTE byOperatorCard)
{
	KillGameTimer(IDI_SELECT_OUT_CARD);

	//��������
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=byOperatorCode;
	OperateCard.cbOperateCard=byOperatorCard;
	if (byOperatorCode==9 && 0!=byOperatorCard)
	{
		OperateCard.cbOperateCode=WIK_GANG;
		OperateCard.cbOperateCard=byOperatorCard;
	}
	//else if (byOperatorCode==10 && 0!=byOperatorCard)
	//{
	//	OperateCard.cbOperateCode=WIK_TING;
	//	OperateCard.cbOperateCard=byOperatorCard;
	//}
	SendData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(CMD_C_OperateCard));

	memset(m_byUserAction, 0, sizeof(m_byUserAction));

	//m_GameClientView.SetStatusFlag(false,false);
	if (0==byOperatorCode)
	{
		if (m_bIsMeCanOutCard)
		{//�Լ������Գ���.
			m_byUserAction[GetMeChairID()] = WIK_NULL;
			//����
			m_wCurrentUser = GetMeChairID();
			memset(m_byUserAction, 0, sizeof(m_byUserAction));
			SetGameTimer(GetMeChairID(),IDI_OTHER_USEROUTCARD,TIME_HEAR_STATUS-10);
			//������ʾ
			//m_GameClientView.SetStatusFlag(true,false);
		}
	}

	m_byCanSelectType = WIK_NULL;
	m_byCanSelectCount = 0;
	ZeroMemory(m_byCanSelectCard,sizeof(m_byCanSelectCard));
	m_GameClientView.m_HandCardControl.SetSelectStatus(false);
	m_GameClientView.UpdateGameView(NULL);
}

//�Ƿ�ʼ���Ӷ���
bool CGameClientDlg::OnSubStartSZFlash()
{
	KillGameTimer(IDI_SZSTART);
	KillTimer(IDI_SHOW_SZ);
	SetTimer(IDI_SHOW_SZ,TIME_SHOW_SZ,NULL);
	m_GameClientView.UpdateGameView(NULL);
	return true;
}

//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	PlayWaitFlash(false);

	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart))
	{
		return false;
	}

	//��������
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	if (NULL == pGameStart)
	{
		return false;
	}

	//char sz[500]={0};
	//sprintf(sz,"\n��Ϸ��ʼ��ׯ��=%d,��ǰ���=%d(��ͼID=%d,�Լ�λ��=%d),�齫����=%d,%s,%d,%s,%d,%s,%d,%s,%d,%s,%d,%s,%d,%s,%d,%s,%d,%s,%d,%s,%d,%s,%d,%s,%d,%s,%d,%s\n",pGameStart->wBankerUser,pGameStart->wCurrentUser,SwitchViewChairID(pGameStart->wCurrentUser),GetMeChairID(),
	//	pGameStart->cbCardData[0],
	//	CGameLogic::GetMJCardName(pGameStart->cbCardData[0]),
	//	pGameStart->cbCardData[1],
	//	CGameLogic::GetMJCardName(pGameStart->cbCardData[1]),
	//	pGameStart->cbCardData[2],
	//	CGameLogic::GetMJCardName(pGameStart->cbCardData[2]),
	//	pGameStart->cbCardData[3],
	//	CGameLogic::GetMJCardName(pGameStart->cbCardData[3]),
	//	pGameStart->cbCardData[4],
	//	CGameLogic::GetMJCardName(pGameStart->cbCardData[4]),
	//	pGameStart->cbCardData[5],
	//	CGameLogic::GetMJCardName(pGameStart->cbCardData[5]),
	//	pGameStart->cbCardData[6],
	//	CGameLogic::GetMJCardName(pGameStart->cbCardData[6]),
	//	pGameStart->cbCardData[7],
	//	CGameLogic::GetMJCardName(pGameStart->cbCardData[7]),
	//	pGameStart->cbCardData[8],
	//	CGameLogic::GetMJCardName(pGameStart->cbCardData[8]),
	//	pGameStart->cbCardData[9],
	//	CGameLogic::GetMJCardName(pGameStart->cbCardData[9]),
	//	pGameStart->cbCardData[10],
	//	CGameLogic::GetMJCardName(pGameStart->cbCardData[10]),
	//	pGameStart->cbCardData[11],
	//	CGameLogic::GetMJCardName(pGameStart->cbCardData[11]),
	//	pGameStart->cbCardData[12],
	//	CGameLogic::GetMJCardName(pGameStart->cbCardData[12]),
	//	pGameStart->cbCardData[13],
	//	CGameLogic::GetMJCardName(pGameStart->cbCardData[13])
	//	);
	//TRACE(sz);
	//ASSERT(pGameStart->wBankerUser==pGameStart->wCurrentUser);
	//if(GetMeChairID()!=pGameStart->wCurrentUser)//�Լ�����ׯ��
	//	ASSERT(pGameStart->cbCardData[13]==0);
	//else//�Լ���ׯ��
	//	ASSERT(pGameStart->cbCardData[13]>0);

	m_GameClientView.PlaySZFlashBT(false);
	::memset(&m_GameStartData, 0, sizeof(CMD_S_GameStart));
	::memcpy(&m_GameStartData, pGameStart, sizeof(CMD_S_GameStart));

	//������ж�ʱ��
	ClearAllTimer();
	m_bIsMeCanOutCard = false;
	m_bStustee = false;

	//����״̬
	SetGameStatus(GS_MJ_PLAY);


	//��λ����
	KillTimer(IDI_SHOW_DIRECT);
	SetTimer(IDI_SHOW_DIRECT,TIME_SHOW_DIRECT,NULL);


	//���ñ���
	//m_bHearStatus=false;
	m_wBankerUser=pGameStart->wBankerUser;
	m_wCurrentUser=pGameStart->wCurrentUser;
	m_cbLeftCardCount=MAX_REPERTORY-GAME_PLAYER*(MAX_COUNT-1)-1;

	m_GameClientView.m_nDiceCount1 =HIBYTE( pGameStart->wSiceCount)-1;
	m_GameClientView.m_nDiceCount2 =LOBYTE( pGameStart->wSiceCount)-1;

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�����˿�
	BYTE byMeChairID = GetMeChairID();
	BYTE cbCardCount=(byMeChairID==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
	m_GameLogic.SwitchToCardIndex(pGameStart->cbCardData,cbCardCount,m_cbCardIndex);

	//���ý���
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_HandCardControl.SetPositively(bPlayerMode);
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));
	//m_ScoreView.ShowWindow(SW_HIDE);
	SafeShowOrHideWnd(&m_ScoreView, SW_HIDE);
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_HIDE);

	//m_GameClientView.m_nKingCardIndex=pGameStart->wMPIndex;


	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_cbHeapCardInfo[i][0]=0;
		m_cbHeapCardInfo[i][1]=HEAP_FULL_COUNT-(((i==m_wBankerUser)||((i+2)%4==m_wBankerUser))?28:26);
		m_GameClientView.m_HeapCard[SwitchViewChairID(i)].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],HEAP_FULL_COUNT);
	}


	//char cbData[128]={0};
	//sprintf(cbData,"ׯ��:%d ����:%d", m_wBankerUser, byMeChairID);
	//InsertSystemString(cbData);

	return true;
}

//������һ�ֵ�������
void CGameClientDlg::LastTurnOperateCard()
{
	//�����һ�ִ�����Ʒŵ������ƶ���,��������������ʾ��,���ٶ�����ʱ����


	//�����˿�
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		//�����˿� 
		WORD wOutViewChairID=SwitchViewChairID(m_wOutCardUser);
		m_GameClientView.m_DiscardCard[wOutViewChairID].AddCardItem(m_cbOutCardData);

		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;

		char cbData[128]={0};
		sprintf(cbData,"��һ�ֳ������:%d, ����:%x,%s",m_wOutCardUser,m_cbOutCardData,CGameLogic::GetMJCardName(m_cbOutCardData));
		TRACE(cbData);
	}

	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	m_GameClientView.UpdateGameView(NULL);

}

//�û�����
bool CGameClientDlg::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard)) return false;


	//������һ����ʾ����
	LastTurnOperateCard();


	//��Ϣ����
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);

	char sz[100]={0};
	sprintf(sz,"\n�û����ƣ�%d(��ͼID=%d,�Լ�λ��=%d),%d,%s\n",pOutCard->wOutCardUser,wOutViewChairID,wMeChairID,pOutCard->cbOutCardData,CGameLogic::GetMJCardName(pOutCard->cbOutCardData));
	TRACE(sz);

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	m_wOutCardUser=pOutCard->wOutCardUser;
	m_cbOutCardData=pOutCard->cbOutCardData;
	m_bIsMeCanOutCard = false;

	KillGameTimer(IDI_OPERATE_CARD); 
	KillGameTimer(IDI_OTHER_USEROUTCARD);
	KillGameTimer(IDI_OTHER_USEROUTCARD);

	//m_GameClientView.SetStatusFlag(false,false);

	memset(m_byUserAction, 0 , sizeof(m_byUserAction));


	////�����˿�
	//if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	//{
	//	//�����˿� 
	//	WORD wOutViewChairID=SwitchViewChairID(m_wOutCardUser);
	//	m_GameClientView.m_DiscardCard[wOutViewChairID].AddCardItem(m_cbOutCardData);
	//}

	//PlayCardSound(pOutCard->cbOutCardData);
	//���ƽ���
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(wOutViewChairID,pOutCard->cbOutCardData);

	//�����û�
	//if (IsLookonMode()==true)
	{
		//��������
		//�����˿�
		if (wOutViewChairID==2)
		{
			//ɾ���˿�
			BYTE cbCardData[MAX_COUNT]={0};
			m_GameLogic.RemoveCard(m_cbCardIndex,pOutCard->cbOutCardData);

			//�����˿�
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
			m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0/*,m_cbTempKing[0]*/);
		}
		else
		{
			WORD wUserIndex=(wOutViewChairID>2)?2:wOutViewChairID;
			m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(false);
		}
	} 

	//����
	const tagUserData *pUserData = GetUserData(pOutCard->wOutCardUser);//�û���Ϣ
	if (m_sDialect=="YY_")
	{
		int num=rand()%20;
		if (9==num)
		{
			{
				PlayGameSoundRand(pUserData->cbGender, TEXT("Rand"), true);
			}
		}
		else
		{
			PlayCardSound(pUserData->cbGender, pOutCard->cbOutCardData);
		}

	}
	else
	{
		PlayCardSound(pUserData->cbGender, pOutCard->cbOutCardData);
	}
	m_GameClientView.UpdateGameView(NULL);

	return true;
}

//������Ϣ
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//��������
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;
	if (NULL == pSendCard)
	{
		return false;
	}

	char sz[100]={0};
	sprintf(sz,"\n������Ϣ����ǰ���=%d(��ͼID=%d,�Լ�λ��=%d),�齫����=%d,%s,��ǰ����:%d\n",pSendCard->wCurrentUser,SwitchViewChairID(pSendCard->wCurrentUser),GetMeChairID(),pSendCard->cbCardData,CGameLogic::GetMJCardName(pSendCard->cbCardData),pSendCard->cbActionMask);
	TRACE(sz);

	//m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	SafeShowOrHideWnd(&m_GameClientView.m_ControlWnd, SW_HIDE);

	//������ж�ʱ��
	ClearAllTimer();
	m_bIsMeCanOutCard = false;

	//���ñ���
	WORD wMeChairID=GetMeChairID();
	m_wCurrentUser=pSendCard->wCurrentUser;

	////�����˿�
	//if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	//{
	//	//�����˿� 
	//	WORD wOutViewChairID=SwitchViewChairID(m_wOutCardUser);
	//	m_GameClientView.m_DiscardCard[wOutViewChairID].AddCardItem(m_cbOutCardData);
	//}

	////���ñ���
	//m_cbOutCardData=0;
	//m_wOutCardUser=INVALID_CHAIR;

	//���ƴ���
	if (pSendCard->cbCardData!=0)
	{
		//ȡ�ƽ���
		WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);
		if (wViewChairID!=2)
		{
			WORD wUserIndex=(wViewChairID>2)?2:wViewChairID;
			m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(true);
		}
		else
		{
			m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pSendCard->cbCardData)]++;

			tagCardItem CardItem;
			CardItem.cbCardData = pSendCard->cbCardData;
			CardItem.bShoot = false;
			CardItem.bKingFlag = false;//(pSendCard->cbCardData == m_cbTempKing[0])?true:false;
			m_GameClientView.m_HandCardControl.SetCurrentCard(CardItem);
			//m_GameClientView.m_HandCardControl.SetCurrentCard(pSendCard->cbCardData);
		}

		//�۳��˿�
		//by Ivan_han 20111229����������Ժ�����һ����ǽû����ʧ��BUG
		if ( pSendCard->bSupplyCard==true/* && m_byUserAction[pSendCard->wCurrentUser] != WIK_NULL 
										 && (m_byUserAction[pSendCard->wCurrentUser] & WIK_FILL 
										 ||m_byUserAction[pSendCard->wCurrentUser] & WIK_GANG) */)
		{
			DeductionTableCard(false);
		}
		else
		{
			DeductionTableCard(true);
		}	
	}

	::memset(m_byUserAction, 0, sizeof(m_byUserAction));
	//��ǰ�û�
	if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
	{
		m_byUserAction[wMeChairID] = pSendCard->cbActionMask;
		//��������
		if (pSendCard->cbActionMask!=WIK_NULL)
		{
			//��ȡ����
			BYTE cbActionCard=pSendCard->cbCardData;
			BYTE cbActionMask=pSendCard->cbActionMask;

			//��������
			tagGangCardResult GangCardResult;
			ZeroMemory(&GangCardResult,sizeof(GangCardResult));

			////�����ж�
			//if ((cbActionMask&(WIK_GANG|WIK_FILL))!=0)
			//{
			//	WORD wMeChairID=GetMeChairID();
			//	m_GameLogic.AnalyseGangCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangCardResult);
			//}
			//tagChiHuResult ChiHuResult;
			//m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wMeChairID],
			//m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],0,wChiHuRight,ChiHuResult);

			//��ʾ����
			tagGangCardResult bucardresult;
			::memset(&bucardresult, 0, sizeof(tagGangCardResult));
			::memcpy(bucardresult.cbCardData, pSendCard->byBuCard, sizeof(BYTE)*pSendCard->byBuCardNums);
			bucardresult.cbCardCount = pSendCard->byBuCardNums;
			//m_GameClientView.m_ControlWnd.SetControlInfo(pSendCard->cbCardData,cbActionMask,bucardresult);
			//m_GameClientView.m_ControlWnd.AddGangCardData(pSendCard->byGangCard, pSendCard->byGangCardNums);

			m_byCanSelectCount = pSendCard->byBuCardNums;
			memcpy(m_byCanSelectCard, pSendCard->byBuCard, sizeof(m_byCanSelectCard));
			m_GameClientView.m_ControlWnd.SetAction(pSendCard->cbCardData, cbActionMask);
			//m_GameClientView.m_ControlWnd.ShowWindow(SW_SHOW);
			SafeShowOrHideWnd(&m_GameClientView.m_ControlWnd, SW_SHOW);

			//���ý���
			//m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,cbActionMask,GangCardResult);
			//������
		}

		//if (m_bHearStatus)
		//{//m_GameClientView.m_ControlWnd.m_btChiHu.IsWindowVisible() && m_GameClientView.m_ControlWnd.m_btChiHu.IsWindowEnabled()
		//	if(pSendCard->cbActionMask&WIK_CHI_HU)
		//	{
		//		OnCardOperate(WIK_CHI_HU,0);
		//	}
		//	else
		//	{
		//		if (WIK_NULL != m_byUserAction[wMeChairID])
		//		{
		//			OnCardOperate(WIK_NULL,0);
		//		}
		//		else
		//		{
		//			BYTE cbCardData=m_GameClientView.m_HandCardControl.GetCurrentCard();
		//			OnOutCard(cbCardData,cbCardData);
		//		}
		//	}
		//	return true;
		//}

		if (pSendCard->cbActionMask!=WIK_NULL)
		{
			m_bIsMeCanOutCard = true;
			//������
			SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
		}
		else
		{
			//����
			SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_HEAR_STATUS);
		}

		////������ʾ
		//if (pSendCard->bSupplyCard)
		//{
		//	PlaySound("holdOneCard", false);
		//}
		//else
		//{
		//	//�û���Ϣ
		//	const tagUserData *pUserData = GetUserData(pSendCard->wCurrentUser);
		//	if (NULL != pUserData)
		//	{
		//		PlayGameSoundLanguage(pUserData->cbGender, TEXT("SupplyCard"), false);
		//	}
		//}
		PlaySound("holdOneCard", false);
		//m_GameClientView.SetStatusFlag(true,false);
		ActiveGameFrame();
	}
	else
	{
		SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_HEAR_STATUS);
		//������ʾ
		//m_GameClientView.SetStatusFlag(false,false);
	}

	//����
	//const tagUserData *pUserData = GetUserData(pSendCard->wCurrentUser);//�û���Ϣ

	//if (m_sDialect=="YY_")
	//{
	//	if (pSendCard->cbCardData == m_cbTempKing[0])
	//	{
	//		int num=rand()%2;
	//		if (pUserData->cbGender==1 && 0==num)
	//		{
	//			PlaySound("YY_King_W_1",false);
	//		}
	//		else if((pUserData->cbGender==0 && 1==num))
	//		{
	//			PlaySound("YY_King_M_1",false);
	//		}
	//	}
	//}

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	return true;
}

//������ʾ
bool CGameClientDlg::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) return false;

	//��������
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;
	char sz[200]={0};
	sprintf(sz,"\n������ʾ��%d(��ͼID=%d,�Լ�λ��=%d),��������=%d,�����齫=%d,%s\n",pOperateNotify->wResumeUser,SwitchViewChairID(pOperateNotify->wResumeUser),GetMeChairID(),pOperateNotify->cbActionMask,pOperateNotify->cbActionCard,CGameLogic::GetMJCardName(pOperateNotify->cbActionCard));
	TRACE(sz);

	m_bIsMeCanOutCard = false;
	KillGameTimer(IDI_OPERATE_CARD);
	WORD byChairID = GetMeChairID();
	m_byUserAction[byChairID] = WIK_NULL;
	//�û�����
	if ((IsLookonMode()==false)&&(pOperateNotify->cbActionMask!=WIK_NULL))
	{
		m_wCurrentUser = byChairID;
		m_byUserAction[byChairID] = pOperateNotify->cbActionMask;
		//��ȡ����
		BYTE cbActionMask=pOperateNotify->cbActionMask;
		BYTE cbActionCard=pOperateNotify->cbActionCard;

		//��������
		tagGangCardResult bucardresult;
		ZeroMemory(&bucardresult,sizeof(tagGangCardResult));

		//�����ж�
		//if ((cbActionMask&(WIK_GANG|WIK_FILL|WIK_TING))!=0)
		//{
		//	//�������
		//	if ((m_wCurrentUser==INVALID_CHAIR)&&(cbActionCard!=0))
		//	{
		//		bucardresult.cbCardCount=1;
		//		bucardresult.cbCardData[0]=cbActionCard;
		//	}
		//	//�Լ�����
		//	else if ((m_wCurrentUser==GetMeChairID())||(cbActionCard==0))
		//	{
		//		WORD wMeChairID=GetMeChairID();
		//		m_GameLogic.AnalyseGangCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],bucardresult);
		//	}
		//}

		//��ʾ����
		::memcpy(bucardresult.cbCardData, pOperateNotify->byBuCard, sizeof(BYTE)*pOperateNotify->byBuCardNums);
		bucardresult.cbCardCount = pOperateNotify->byBuCardNums;
		//m_GameClientView.m_ControlWnd.SetControlInfo(pOperateNotify->cbActionCard,cbActionMask,bucardresult);
		//m_GameClientView.m_ControlWnd.AddGangCardData(pOperateNotify->byGangCard, pOperateNotify->byGangCardNums);

		m_byCanSelectCount = pOperateNotify->byBuCardNums;
		memcpy(m_byCanSelectCard, pOperateNotify->byBuCard, sizeof(m_byCanSelectCard));
		m_GameClientView.m_ControlWnd.SetAction(pOperateNotify->cbActionCard, cbActionMask);


		//if ( m_bHearStatus)
		//{//������
		//	if( m_GameClientView.m_ControlWnd.m_btChiHu.IsWindowVisible() 
		//		&& m_GameClientView.m_ControlWnd.m_btChiHu.IsWindowEnabled())
		//	{
		//		OnCardOperate(WIK_CHI_HU,0);
		//	}
		//	else if (m_GameClientView.m_ControlWnd.m_btGiveUp.IsWindowVisible() 
		//				&& m_GameClientView.m_ControlWnd.m_btGiveUp.IsWindowEnabled())
		//	{
		//		OnCardOperate(WIK_NULL,0);
		//	}
		//}
		//else
		//{
		SetGameTimer(byChairID,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
		//}
		//���ý���
		ActiveGameFrame();
	}

	return true;
}

//�������
bool CGameClientDlg::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult)) return false;

	//��Ϣ����
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;
	char sz[300]={0};
	//bool							bBringMuscleCanHu;				//�����ܺ���
	//BYTE							cbOperateCode;						//��������
	//BYTE							cbOperateCard;						//�����˿�
	//BYTE								bMuscle;							// �ʽ, 0 ��Чֵ, 1 ���2 ���3 �Ž�
	//WORD							wOperateUser;						//�����û�
	//WORD							wProvideUser;						//��Ӧ�û�
	//LONGLONG					llMuscleGold[GAME_PLAYER]; //��Ҹ���ֵ
	static const char * szMuscle[4]={"��Чֵ","����","����","�Ž�"};
	sprintf(sz,"\n���������Ϣ��bBringMuscleCanHu=%d,�Լ�λ��=%d,�����û�=%d(�����û���ͼID=%d),��Ӧ�û�=%d(��Ӧ�û���ͼID=%d),�齫����=%d,%s,��������:%d,�ʽ=%d,%s\n",pOperateResult->bBringMuscleCanHu,GetMeChairID(),pOperateResult->wOperateUser,SwitchViewChairID(pOperateResult->wOperateUser),pOperateResult->wProvideUser,SwitchViewChairID(pOperateResult->wProvideUser),pOperateResult->cbOperateCard,CGameLogic::GetMJCardName(pOperateResult->cbOperateCard),pOperateResult->cbOperateCode,pOperateResult->bMuscle,szMuscle[pOperateResult->bMuscle]);
	TRACE(sz);

	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);

	//��������
	BYTE cbPublicCard=TRUE;
	WORD wOperateUser=pOperateResult->wOperateUser;
	BYTE cbOperateCard=pOperateResult->cbOperateCard;
	WORD wOperateViewID=SwitchViewChairID(wOperateUser);

	//���Ʊ���
	if(pOperateResult->cbOperateCode!=WIK_NULL)
	{
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}

	KillGameTimer(IDI_OTHER_USEROUTCARD);
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	//m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	SafeShowOrHideWnd(&m_GameClientView.m_ControlWnd, SW_HIDE);

	m_bIsMeCanOutCard = false;
	memset(m_byUserAction, 0, sizeof(m_byUserAction));
	//�������
	if ((pOperateResult->cbOperateCode&(WIK_GANG|WIK_FILL|WIK_TING))>0)
	{//�������,�����ǲ����ܡ���
		//Ϊ��ǰ�������
		m_wCurrentUser=INVALID_CHAIR;
		//���ñ���
		//if ((pOperateResult->cbOperateCode==WIK_TING)&&(wOperateUser==GetMeChairID())) 
		//{
		//	m_bHearStatus = true;
		//}

		//����˿�
		BYTE cbWeaveIndex=0xFF;
		for (BYTE i=0;i<m_cbWeaveCount[wOperateUser];i++)
		{
			BYTE cbWeaveKind=m_WeaveItemArray[wOperateUser][i].cbWeaveKind;
			BYTE cbCenterCard=m_WeaveItemArray[wOperateUser][i].cbCenterCard;
			if ((cbCenterCard==cbOperateCard)&&(cbWeaveKind==WIK_PENG))
			{
				cbWeaveIndex=i;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
				break;
			}
		}

		//����˿�
		if (cbWeaveIndex==0xFF)
		{
			//�����ж�
			cbPublicCard=(pOperateResult->wProvideUser==wOperateUser)?FALSE:TRUE;

			//�����˿�
			cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=cbPublicCard;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
		}

		//��Ͻ���
		BYTE cbWeaveCard[4]={0,0,0,0},cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbOperateCard,cbWeaveCard);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbWeaveCard,cbWeaveCardCount);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDisplayItem((cbPublicCard==1)?true:false);

		CString strPath="";
		m_GameClientView.GetPicPath(strPath);
		strPath.Append("Cartoon\\");

		// ���
		if ( !pOperateResult->bBringMuscleCanHu  &&  0!=pOperateResult->bMuscle)
		{
			LONGLONG llVal[GAME_PLAYER];
			ZeroMemory(llVal, sizeof(llVal));

			for (BYTE i=0; i<GAME_PLAYER; i++)
			{
				llVal[SwitchViewChairID(i)] = pOperateResult->llMuscleGold[i];
			}

			CString strGold;
			strGold.Format("client: CGameClientDlg::OnSubOperateResult  %I64d,%I64d,%I64d,%I64d", llVal[0], llVal[1], llVal[2], llVal[3]);
			OutputDebugString(strGold);

			//��Ľ�����ʾ�ڼ�ʱ����Ի���add by Ivan_han 20111227��
			const tagUserData *pUserData = GetUserData(GetMeChairID());//�û���Ϣ
			ASSERT(pUserData!=NULL);
			ASSERT(pOperateResult->bMuscle>=1 && pOperateResult->bMuscle<=3);
			TCHAR szBuffer[64];
			_snprintf(szBuffer,CountArray(szBuffer),TEXT("\n���ν����,�ɼ�ͳ��:"));
			InsertGeneralString(szBuffer,RGB(255,0,0),true);

			static const char *	szMuscle[4]={"��Чֵ","����","����","�Ž�"};
			static const char *	szDirect[4]={"ׯ��","�Ϸ�","����","����"};
			char sz[100]={0};
			if(pOperateResult->bMuscle==3)//"�Ž�"
			{

				WORD wIndex=((pOperateResult->wProvideUser+GAME_PLAYER)-m_wBankerUser)%GAME_PLAYER;
				strcpy(sz,szDirect[wIndex]);
			}
			else
			{
				if(pOperateResult->wOperateUser==GetMeChairID())//���Լ�"����","����"
					strcpy(sz,pUserData->szName);
				else
				{

					WORD wIndex=((pOperateResult->wOperateUser+GAME_PLAYER)-m_wBankerUser)%GAME_PLAYER;
					strcpy(sz,szDirect[wIndex]);
				}	
			}
			strcat(sz,szMuscle[pOperateResult->bMuscle]);
			//strcat(sz,"\n");
			InsertGeneralString(sz,RGB(255,255,0),true);
			TRACE(sz);

			for	(WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wIndex=((i+GAME_PLAYER)-m_wBankerUser)%GAME_PLAYER;
				WORD wViewID=SwitchViewChairID(i);
				ASSERT(wIndex>=0 &&	wIndex<4);
				ASSERT(wViewID>=0 && wViewID<4);
				if (2==wViewID)
				{
					_snprintf(szBuffer,CountArray(szBuffer),TEXT("%s��"),pUserData->szName);
				} 
				else
				{
					_snprintf(szBuffer,CountArray(szBuffer),TEXT("%s��"),szDirect[wIndex]);
				}
				InsertGeneralString(szBuffer,RGB(0,255,0),false);
				_snprintf(szBuffer,CountArray(szBuffer),TEXT("%+I64d"),llVal[wViewID]);
				InsertGeneralString(szBuffer,RGB(255,255,0),true);
			}
			//�����
			LONGLONG lltax=-(llVal[0]+llVal[1]+llVal[2]+llVal[3]);
			_snprintf(szBuffer,CountArray(szBuffer),TEXT("\n�����:%d"),lltax);
			InsertGeneralString(szBuffer,RGB(255,255,0),true);

			m_GameClientView.PlayMuscleFlash(wOperateViewID, 255, pOperateResult->bMuscle, llVal);
			PlaySound(TEXT("muscle"), false);
		}

		//�˿�����
		if (GetMeChairID()==wOperateUser)
		{
			m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pOperateResult->cbOperateCard)]=0;
		}

		//�����˿�
		if (GetMeChairID()==wOperateUser)
		{
			BYTE cbCardData[MAX_COUNT]={0};
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
			m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0/*,m_cbTempKing[0]*/);
		}
		else
		{
			WORD wUserIndex=(wOperateViewID>=3)?2:wOperateViewID;
			BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[wOperateUser]*3;
			m_GameClientView.m_UserCard[wUserIndex].SetCardData(cbCardCount-1,false);
		}
	}
	else if (pOperateResult->cbOperateCode!=WIK_NULL)
	{//������ң��ԡ��������
		//Ϊ��ǰ�������
		m_wCurrentUser=pOperateResult->wOperateUser;
		//�������
		BYTE cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;

		//��Ͻ���
		BYTE cbWeaveCard[4]={0,0,0,0},cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbOperateCard,cbWeaveCard);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbWeaveCard,cbWeaveCardCount);

		//�����˿�
		if (GetMeChairID()==wOperateUser)
		{
			//ɾ���˿�
			m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard,1);
			m_GameLogic.RemoveCard(m_cbCardIndex,cbWeaveCard,cbWeaveCardCount-1);


			BYTE cbCardData[MAX_COUNT]={0};
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
			tagCardItem CardItem;
			CardItem.cbCardData = cbCardData[cbCardCount-1];
			CardItem.bShoot = false;
			CardItem.bKingFlag = false;//(cbCardData[cbCardCount-1] == m_cbTempKing[0])?true:false;
			m_GameClientView.m_HandCardControl.SetCurrentCard(CardItem);
			m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount-1,cbCardData[cbCardCount-1]/*,m_cbTempKing[0]*/);
		}
		else
		{
			WORD wUserIndex=(wOperateViewID>=3)?2:wOperateViewID;
			BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[wOperateUser]*3;
			m_GameClientView.m_UserCard[wUserIndex].SetCardData(cbCardCount-1,true);
		}

		if (GetMeChairID()==m_wCurrentUser)
		{//���ǲ������,Ҫ���һ����
			//����ʱ��
			SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_HEAR_STATUS);
		}
	}

	//���ý���
	m_GameClientView.SetUserAction(wOperateViewID,pOperateResult->cbOperateCode);
	//m_GameClientView.SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),false);
	m_GameClientView.PlayOperationFlash(wOperateViewID,pOperateResult->cbOperateCode);
	//char cbData[128]={0};
	//sprintf(cbData,"%d.....%x",wOperateViewID,pOperateResult->cbOperateCode);
	//InsertSystemString(cbData);

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//��������
	const tagUserData *pUserData = GetUserData(pOperateResult->wOperateUser);//�û���Ϣ
	PlayActionSound(pUserData->cbGender, pOperateResult->cbOperateCode);

	return true;
}

////���ײ���
//bool CGameClientDlg::OnSubOperateHaiDi(const void * pBuffer, WORD wDataSize)
//{
//	//Ч������
//	ASSERT(wDataSize==sizeof(CMD_S_OperateHaiDi));
//	if (wDataSize!=sizeof(CMD_S_OperateHaiDi)) return false;
//
//	//��Ϣ����
//	CMD_S_OperateHaiDi * pOperateHaiDi=(CMD_S_OperateHaiDi *)pBuffer;
//
//	//����״̬
//	SetGameStatus(GS_MJ_HAI_DI);
//
//	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
//
//	//���ñ���
//	m_bIsMeCanOutCard = false;
//	m_wCurrentUser=INVALID_CHAIR;
//	//m_GameClientView.SetStatusFlag(false,false);
//	//m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
//	SafeShowOrHideWnd(&m_GameClientView.m_ControlWnd, SW_HIDE);
//
//	//�����˿�
//	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
//	{
//		//�����˿�
//		WORD wOutViewChairID=SwitchViewChairID(m_wOutCardUser);
//		m_GameClientView.m_DiscardCard[wOutViewChairID].AddCardItem(m_cbOutCardData);
//
//		//���ñ���
//		m_cbOutCardData=0;
//		m_wOutCardUser=INVALID_CHAIR;
//	}
//
//	//��������
//	if ((IsLookonMode()==false)&&(GetMeChairID()==pOperateHaiDi->wCurrentUser))
//	{
//		m_wCurrentUser = GetMeChairID();
//		//m_GameClientView.m_btHaiDi.ShowWindow(SW_SHOW);
//		//m_GameClientView.m_btNoHaiDi.ShowWindow(SW_SHOW);
//		ActiveGameFrame();
//	}
//
//	m_GameClientView.UpdateGameView(NULL);
//	//ʱ������
//	SetGameTimer(pOperateHaiDi->wCurrentUser,IDI_OPERATE_HAI_DI,TIME_OPERATE_HAI_DI);
//
//	return true;
//}



//��Ϸ����
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	if (NULL == pGameEnd )
	{
		return false;
	}

	//ɾ����ʱ��
	ClearAllTimer();

	m_GameClientView.PlaySZFlashBT(false);
	//����״̬
	SetGameStatus(GS_MJ_FREE);
	//m_GameClientView.SetStatusFlag(false,false);

	//���ÿؼ�
	m_bIsMeCanOutCard = false;
	//m_GameClientView.SetStatusFlag(false,false);
	//m_GameClientView.m_btHaiDi.ShowWindow(SW_HIDE);
	//m_GameClientView.m_btNoHaiDi.ShowWindow(SW_HIDE);
	//m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	SafeShowOrHideWnd(&m_GameClientView.m_ControlWnd, SW_HIDE);
	m_GameClientView.m_HandCardControl.SetPositively(false);

	//��ʼ��������Ϣ
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR, 0);	

	//��������
	//char chEndData[100]={0};
	//sprintf(chEndData, "pGameEnd->cbHaiDiCard=%d", pGameEnd->cbHaiDiCard);
	//InsertGeneralString(chEndData, RGB(0, 255, 0), true);

	WORD MyChairId=GetMeChairID();
	m_ScoreView.GetViewID(GetMeChairID(),m_wBankerUser);

	m_GameClientView.m_wChiHuType=pGameEnd->iChiHuType;  //  0:��ׯ ;  1:���� ; 2:����;  3:��������;  4:�����ڣ�5�����ף���ׯ
	m_GameScoreInfo.cbKingCard=pGameEnd->bKingCard;
	m_GameScoreInfo.wBankerUser=m_wBankerUser;
	m_GameScoreInfo.iCellScore=pGameEnd->iCellScore[MyChairId];
	m_GameScoreInfo.iBasicScore=m_GameClientView.GetCellScore();
	//m_GameScoreInfo.cbBirdCard=pGameEnd->cbBirdCard;
	m_GameScoreInfo.cbChiHuCard=pGameEnd->cbChiHuCard;
	m_GameScoreInfo.wProvideUser=pGameEnd->wProvideUser;
	//m_GameScoreInfo.wBirdHitUser=pGameEnd->wBirdHitUser;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		const tagUserData * pUserData=GetUserData(i);
		WORD byViewID = SwitchViewChairID(i);
		m_GameClientView.m_wChiHuKind[byViewID]=pGameEnd->wChiHuKind[i];	//��������
		m_GameClientView.m_wChiHuRight[byViewID]=pGameEnd->wChiHuRight[i];	//����Ȩλ

		m_GameScoreInfo.iMuscle[i]=pGameEnd->iMuscle[i];
		m_GameScoreInfo.cbTimes[i]=pGameEnd->cbTimes[i];
		m_GameScoreInfo.lScore[i]=pGameEnd->lGameScore[i];
		m_GameScoreInfo.wCardType[i]=pGameEnd->wChiHuKind[i];
		m_GameScoreInfo.cbKingCount[i]=pGameEnd->cbKingCount[i];
		m_GameScoreInfo.iChiHuKind[i]=pGameEnd->iChiHuKind[i];
		lstrcpyn(m_GameScoreInfo.szUserName[i],pUserData->szName,CountArray(m_GameScoreInfo.szUserName[i]));

		// �������
		if (5==m_GameClientView.m_wChiHuType  ||  3==m_GameClientView.m_wChiHuType  ||  4==m_GameClientView.m_wChiHuType)
		{
			m_GameClientView.m_HeapCard[i].SetCardData(0, 0, 0);
		}
	}


	//�ж��Ƿ�����ǿ��
	bool bIsLeft=false;
	for (int i=0;i<GAME_PLAYER;++i)
	{
		if (pGameEnd->IsUserLeft[i])
		{
			bIsLeft=true;
			break;
		}	
	}

	//�ɼ���ʾ�ڼ�ʱ����Ի���
	if (!pGameEnd->bIsHuangzhuang)
	{
		TCHAR szBuffer[64];
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("\n���ֽ���,�ɼ�ͳ��:"));
		InsertGeneralString(szBuffer,RGB(255,0,0),true);
		for (WORD i=0;i<GAME_PLAYER;i++)
		{//��������
			if (m_GameScoreInfo.szUserName[i]!=0)
			{
				WORD wIndex=((i+GAME_PLAYER)-m_GameScoreInfo.wBankerUser)%GAME_PLAYER;
				WORD wViewID=SwitchViewChairID(i);
				if (2==wViewID)
				{
					_snprintf(szBuffer,CountArray(szBuffer),TEXT("%s��"),m_GameScoreInfo.szUserName[i]);
				} 
				else
				{
					if (0==wIndex)
					{
						_snprintf(szBuffer,CountArray(szBuffer),TEXT("%s��"),"ׯ��");
					}
					else if (1==wIndex)
					{
						_snprintf(szBuffer,CountArray(szBuffer),TEXT("%s��"),"�Ϸ�");
					}
					else if (2==wIndex)
					{
						_snprintf(szBuffer,CountArray(szBuffer),TEXT("%s��"),"����");
					}
					else
					{
						_snprintf(szBuffer,CountArray(szBuffer),TEXT("%s��"),"����");
					}

				}
				InsertGeneralString(szBuffer,RGB(0,255,0),false);
				_snprintf(szBuffer,CountArray(szBuffer),TEXT("%+I64d"),pGameEnd->lGameScore[i]);
				InsertGeneralString(szBuffer,RGB(255,255,0),true);				
			}
			m_ScoreView.SetUserLeft(i,pGameEnd->IsUserLeft[i]);
		}

		//�����
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("\n�����:%d"),m_GameScoreInfo.iCellScore);
		InsertGeneralString(szBuffer,RGB(255,255,0),true);

		m_ScoreView.SetScoreInfo(m_GameScoreInfo);
		m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
		m_GameClientView.m_bIsHuangZhuang=false;
		m_GameClientView.SetHaiDiCard(pGameEnd->cbHaiDiCard);

		if (!bIsLeft)
		{
			//�ж��Ƿ�����������
			m_bZM=false;//����
			m_bFP=false;//����
			if (m_GameScoreInfo.wProvideUser != INVALID_CHAIR)
			{
				if(CHK_NULL != m_GameScoreInfo.wCardType[m_GameScoreInfo.wProvideUser])
				{
					m_bZM=true;
				}
				else 
				{
					m_bFP=true;
				}	
			}

			//////////////////////////////////////////////////////////////////////////////////
			//���ź��ƶ���
			//ͳ�ƺ�����Ҹ���
			BYTE bySaveHuPaiUser[GAME_PLAYER]={-1};
			int iChiHuNum=0;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if (pGameEnd->wChiHuKind[i]!=CHK_NULL) 
				{
					bySaveHuPaiUser[iChiHuNum++] = i;
				}
			}
			m_GameClientView.m_iChiHuNums=iChiHuNum;
			if (3==iChiHuNum)
			{//ͨ��
				//����
				m_GameClientView.PlayChiHuFlash(0);

				//����
				PlaySound("fangpao",false);
				for (BYTE i=0; i<GAME_PLAYER;i++)
				{
					// ���������Ҫ�������������һ�κ�����������ֻ��һ�ң����������ֻ���Լ���������
					if (i==GetMeChairID()  && i==m_GameScoreInfo.wProvideUser)
						continue;

					const tagUserData *pOtherUserData = GetUserData(bySaveHuPaiUser[0]);//�û���Ϣ
					if (NULL != pOtherUserData)
					{
						PlayGameSoundLanguage(pOtherUserData->cbGender,TEXT("chihu"),2);
						break;
					}
				}
			}
			else if(m_bZM)
			{//����
				const tagUserData *pUserData = GetUserData(m_GameScoreInfo.wProvideUser);//�û���Ϣ
				WORD wViewChairID=SwitchViewChairID(m_GameScoreInfo.wProvideUser);
				m_GameClientView.SetUserAction(wViewChairID,WIK_CHI_HU);
				m_byUserAction[m_GameScoreInfo.wProvideUser] = WIK_CHI_HU;

				//����
				m_GameClientView.PlayChiHuFlash(wViewChairID);

				//����
				PlayGameSoundLanguage(pUserData->cbGender,TEXT("zimo"));
			}
			else if (iChiHuNum > 0)
			{//����
				BYTE byMeChairID = GetMeChairID();
				BYTE byViewChairID = SwitchViewChairID(byMeChairID);
				const tagUserData *pUserData = GetUserData(byMeChairID);//�û���Ϣ
				
				//���ڷ��Ķ���
				m_GameClientView.PlayFangPaoFlash(SwitchViewChairID(m_GameScoreInfo.wProvideUser));
				//����
				PlaySound("fangpao",false);

				//���˵ĺ��ƶ���
				for (int i=0; i<iChiHuNum; ++i)
				{
					BYTE byOtherViewChairID = SwitchViewChairID(bySaveHuPaiUser[i]);
					m_GameClientView.SetUserAction(byOtherViewChairID,WIK_CHI_HU);
					m_byUserAction[bySaveHuPaiUser[i]] = WIK_CHI_HU;
					m_GameClientView.PlayChiHuFlash(byOtherViewChairID);
				}
				
				// ���ź�������
				for (BYTE i=0; i<GAME_PLAYER;i++)
				{
					if (i==byMeChairID)
						continue;

					const tagUserData *pOtherUserData = GetUserData(bySaveHuPaiUser[0]);//�û���Ϣ
					if (NULL != pOtherUserData  &&  CHK_NULL != pGameEnd->wChiHuKind[i])
					{
						PlayGameSoundLanguage(pUserData->cbGender, TEXT("chihu"), 2);
						break;
					}
				}
				
				//if(byMeChairID==m_GameScoreInfo.wProvideUser)
				//{//���˷��ڣ����˺��ˣ�
				//	for (int i=0; i<iChiHuNum; ++i)
				//	{//���˵ĺ��ƶ���
				//		BYTE byOtherViewChairID = SwitchViewChairID(bySaveHuPaiUser[i]);
				//		m_GameClientView.SetUserAction(byOtherViewChairID,WIK_CHI_HU);
				//		m_byUserAction[bySaveHuPaiUser[i]] = WIK_CHI_HU;
				//		m_GameClientView.PlayChiHuFlash(byOtherViewChairID);
				//	}

				//	//�Լ��Ķ���
				//	//MessageBox("���˺�");
				//	m_GameClientView.PlayFangPaoFlash(byViewChairID);

				//	//����
				//	PlaySound("fangpao",false);
				//	for (BYTE i=0; i<GAME_PLAYER;i++)
				//	{
				//		if (i==byMeChairID)
				//			continue;

				//		const tagUserData *pOtherUserData = GetUserData(bySaveHuPaiUser[0]);//�û���Ϣ
				//		if (NULL != pOtherUserData)
				//		{
				//			PlayGameSoundLanguage(pUserData->cbGender,TEXT("chihu"),2);
				//			break;
				//		}
				//	}
				//}
				//else
				//{
				//	if (pGameEnd->wChiHuKind[byMeChairID]!=CHK_NULL) 
				//	{//�Լ�������
				//		//MessageBox("�Լ���");

				//		for (int i=0; i<iChiHuNum; ++i)
				//		{//�Լ��ͱ��˵ĺ��ƶ���
				//			BYTE byOtherViewChairID = SwitchViewChairID(bySaveHuPaiUser[i]);
				//			m_GameClientView.SetUserAction(byOtherViewChairID,WIK_CHI_HU);
				//			m_byUserAction[bySaveHuPaiUser[i]] = WIK_CHI_HU;
				//			m_GameClientView.PlayChiHuFlash(byOtherViewChairID);
				//		}

				//		//���ڷ��Ķ���
				//		m_GameClientView.PlayFangPaoFlash(SwitchViewChairID(m_GameScoreInfo.wProvideUser));

				//		//����
				//		PlaySound("fangpao",false);
				//		PlayGameSoundLanguage(pUserData->cbGender,TEXT("chihu"), 2);
				//	}
				//	else
				//	{//�Լ�û�к���.
				//		for (int i=0; i<iChiHuNum; ++i)
				//		{
				//			//����
				//			BYTE byOtherViewChairID = SwitchViewChairID(bySaveHuPaiUser[i]);
				//			m_GameClientView.SetUserAction(byOtherViewChairID,WIK_CHI_HU);
				//			m_byUserAction[bySaveHuPaiUser[i]] = WIK_CHI_HU;
				//			m_GameClientView.PlayChiHuFlash(byOtherViewChairID);
				//		}

				//		//���ڷ��Ķ���
				//		m_GameClientView.PlayFangPaoFlash(SwitchViewChairID(m_GameScoreInfo.wProvideUser));

				//		//����
				//		const tagUserData *pOtherUserData = GetUserData(bySaveHuPaiUser[0]);//�û���Ϣ
				//		PlaySound("fangpao",false);
				//		PlayGameSoundLanguage(pOtherUserData->cbGender,TEXT("chihu"),2);
				//	}
				//}
			}
		}
	}
	else 
	{//���׻�ׯ
		DeductionTableCard(true);
		m_GameClientView.SetHaiDiCard(pGameEnd->cbHaiDiCard);
		m_GameClientView.m_bIsHuangZhuang=true;
	}


	//�����˿�
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0/*,NULL*/);


	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		m_GameClientView.m_TableCard[wViewChairID].SetCardData(pGameEnd->cbCardData[i],pGameEnd->cbCardCount[i]);

		if (1==pGameEnd->iChiHuKind[i]  ||  2==pGameEnd->iChiHuKind[i])
		{
			m_GameClientView.m_TableCard[wViewChairID].SetWinCardData(pGameEnd->cbChiHuCard);
		}
	}

	m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_HIDE);
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);

	//�����Թ۽���
	if (IsLookonMode()==true)
	{
		PlayWaitFlash(false);
		//m_ScoreView.ShowWindow(SW_SHOW);
		SafeShowOrHideWnd(&m_ScoreView, SW_SHOW);

		if (pGameEnd->cbChiHuCard == 0)
		{
			//m_ScoreView.ShowWindow(SW_HIDE);
			SafeShowOrHideWnd(&m_ScoreView, SW_HIDE);
		}
	}

	//���ý���
	if (IsLookonMode()==false)
	{
		const tagUserData *pUserData = GetUserData(GetMeChairID());//�û���Ϣ
		if (pGameEnd->bIsHuangzhuang)
		{//��ׯ
			//m_ScoreView.ShowWindow(SW_HIDE);
			SafeShowOrHideWnd(&m_ScoreView, SW_HIDE);
			SetTimer(IDI_HUANGZHUANG,1500,NULL);
			SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
			m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
			PlaySound("liuju", false);
			//PlayGameSoundLanguage(pUserData->cbGender,TEXT("liuju"),false);
			m_GameClientView.PlayChiHuFlash(SwitchViewChairID(pUserData->wChairID));
		}
		else
		{//��ʱ3.5����ʾ�����
			if (bIsLeft)
			{
				//MessageBox("����ʤ��");
				PlaySound("GAME_WIN",false);
				SetTimer(IDI_GAME_END,TIME_GAME_END-1500,NULL);
			}
			else
			{
				SetTimer(IDI_GAME_END,TIME_GAME_END,NULL);
			}
			m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
		}
	}

	//�����й�,���Ʊ�ʶ
	m_bStustee = false;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_bTrustee[i]=false;
		m_GameClientView.m_bTingFlag[i] = false;
	}

	//if(m_bStustee)
	//{
	//	OnStusteeControl(0,0);
	//}

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	return true;
}

////��������
//void CGameClientDlg::PlayCardSound(BYTE cbGender, BYTE cbCardData)
//{
//	//��������
//	TCHAR szSoundName[16]=TEXT("");
//	TCHAR szKind[]={TEXT('W'),TEXT('S'),TEXT('T'),TEXT('F')};
//
//	//��������
//	BYTE cbValue=(cbCardData&MASK_VALUE);
//	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
//	_snprintf(szSoundName,CountArray(szSoundName),TEXT("%c_%d"),szKind[cbColor],cbValue);
//
//	//��������
//	PlayGameSoundLanguage(cbGender,szSoundName);
//
//	return;
//}


//��������
void CGameClientDlg::PlayCardSound(BYTE cbGender, BYTE cbCardData)
{
	//��������
	TCHAR szSoundName[16]=TEXT("");

	//��������
	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	CString szSound="";
	if (0==cbColor)
	{
		szSound=TEXT("wan");
	}
	else if (1==cbColor)
	{
		szSound=TEXT("tiao");
	}
	else if (2==cbColor)
	{
		szSound=TEXT("tong");
	}
	else
	{
		szSound=TEXT("feng");
	}

	_snprintf(szSoundName,CountArray(szSoundName),TEXT("%d"),cbValue);

	//��������
	PlaySound("OUT_CARD",false);
	m_cbGender=cbGender;
	m_InfoSound=szSound+szSoundName;
	SetTimer(IDI_SOUND,200,NULL);	

	return;
}

//��������
void CGameClientDlg::PlayActionSound(BYTE cbGender, BYTE cbAction)
{
	CString strBuffer;
	strBuffer.Format("client:CGameClientDlg::PlayActionSound cbGender%d, cbAction%d",cbGender, cbAction);
	OutputDebugString(strBuffer);

	switch (cbAction)
	{
	case WIK_NULL:		//ȡ��
		{
			PlaySound("OUT_CARD",false);
			break;
		}
	case WIK_LEFT:     //����
	case WIK_CENTER:
	case WIK_RIGHT:	
		{
			//PlayGameSoundLanguage(cbGender,TEXT("chi"));
			break;
		}
	case WIK_PENG:		//����
		{
			PlayGameSoundLanguage(cbGender,TEXT("peng"), 3);
			break;
		}
	case WIK_FILL:	//����
	case WIK_GANG:	
		{
			PlayGameSoundLanguage(cbGender,TEXT("SupplyCard"));
			break;
		}
		//case WIK_TING:		//����״̬��������ʾ��
		//	{

		//		PlayGameSoundLanguage(cbGender,TEXT("gang"));
		//		break;
		//	}
	case WIK_CHI_HU:	//�Ժ�
		{
			break;
		}
	}

	return;
}

//�۳��˿�
void CGameClientDlg::DeductionTableCard(bool bHeadCard)
{
	//��˳��ɾ��
	if (bHeadCard==true)
	{
		//�л�����
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapHead][0]+m_cbHeapCardInfo[m_wHeapHead][1];
		if (cbHeapCount==HEAP_FULL_COUNT) 
			m_wHeapHead=(m_wHeapHead+1)%CountArray(m_cbHeapCardInfo);

		//�����˿�
		m_cbLeftCardCount--;
		m_cbHeapCardInfo[m_wHeapHead][0]++;

		//�����˿�
		WORD wHeapViewID=SwitchViewChairID(m_wHeapHead);
		WORD wMinusHeadCount=m_cbHeapCardInfo[m_wHeapHead][0];
		WORD wMinusLastCount=m_cbHeapCardInfo[m_wHeapHead][1];
		m_GameClientView.m_HeapCard[wHeapViewID].SetCardData(wMinusHeadCount,wMinusLastCount,HEAP_FULL_COUNT);
	}
	//�����һ��ɾ��
	else
	{
		//�л�����
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapTail][0]+m_cbHeapCardInfo[m_wHeapTail][1];
		if (cbHeapCount==HEAP_FULL_COUNT) m_wHeapTail=(m_wHeapTail+3)%CountArray(m_cbHeapCardInfo);

		//�����˿�
		m_cbLeftCardCount--;
		m_cbHeapCardInfo[m_wHeapTail][1]++;

		//�����˿�
		WORD wHeapViewID=SwitchViewChairID(m_wHeapTail);
		WORD wMinusHeadCount=m_cbHeapCardInfo[m_wHeapTail][0];
		WORD wMinusLastCount=m_cbHeapCardInfo[m_wHeapTail][1];
		m_GameClientView.m_HeapCard[wHeapViewID].SetCardData(wMinusHeadCount,wMinusLastCount,HEAP_FULL_COUNT);
	}

	return;
}

//��ʾ����
bool CGameClientDlg::ShowOperateControl(BYTE cbUserAction,BYTE cbStartData)
{
	if (cbUserAction==WIK_TING)
	{
		return true;
	}
	//��ʾ����
	tagGangCardResult bucardresult;
	::memset(&bucardresult, 0, sizeof(tagGangCardResult));
	::memcpy(bucardresult.cbCardData, m_GameStartData.byBuCard, sizeof(BYTE)*m_GameStartData.byBuCardNums);
	bucardresult.cbCardCount = m_GameStartData.byBuCardNums;
	//m_GameClientView.m_ControlWnd.SetControlInfo(cbStartData,cbUserAction,bucardresult);
	//m_GameClientView.m_ControlWnd.AddGangCardData(m_GameStartData.byGangCard, m_GameStartData.byGangCardNums);
	m_byCanSelectCount = m_GameStartData.byBuCardNums;
	memcpy(m_byCanSelectCard, m_GameStartData.byBuCard, sizeof(m_byCanSelectCard));
	m_GameClientView.m_ControlWnd.SetAction(cbStartData, cbUserAction);

	return true;
}


//ʱ����Ϣ
void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);

	//��λ����
	if (nIDEvent==IDI_SHOW_DIRECT)
	{
		//��������
		KillTimer(IDI_SHOW_DIRECT);
		m_GameClientView.PlayDirectFlash();
		PlaySound("EffectDirect", false);
		//PlaySound("GAME_START",false);
		return;
	}

	//���Ӷ�ʱ��
	else if (nIDEvent==IDI_SHOW_SZ)
	{
		//��������
		KillTimer(IDI_SHOW_SZ);
		m_GameClientView.SetDiceEffect(true);
		PlaySound("dice", false);
		return;
	}

	//������ʱ��
	else if (nIDEvent==IDI_SOUND)
	{
		KillTimer(IDI_SOUND);
		PlayGameSoundLanguage(m_cbGender,m_InfoSound);
		return;
	}

	//������ʱ��
	else if (nIDEvent==IDI_HUANGZHUANG)
	{
		KillTimer(IDI_HUANGZHUANG);
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		return;
	}

	//������ʱ��
	else if (nIDEvent==IDI_GAME_END)
	{
		KillGameTimer(IDI_START_GAME);
		KillTimer(IDI_GAME_END);
		PlayWaitFlash(false);
		//m_ScoreView.ShowWindow(SW_SHOW);
		SafeShowOrHideWnd(&m_ScoreView, SW_SHOW);
		m_ScoreView.SetCountDown(true);
		return;
	}
}

//��ʼ��ť
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//m_GameClientView.m_tDice=1;
	//m_GameClientView.SetDiceEffect(true);
	//m_GameClientView.SetStartEffect(true);

	//SetTimer(IDI_SHOW_SZ,TIME_SHOW_SZ,NULL);

	////������ʾ
	//BYTE cbCardData[MAX_INDEX]={0x12,0x18,0x27,0x27,0x27,0x03,0x04,0x05,0x11,0x12,0x13,0x16,0x17};
	//m_GameClientView.m_HandCardControl.SetCardData(cbCardData,13,0x29,0x01);
	//m_GameClientView.m_HandCardControl.SetDisplayItem(true);
	//m_GameClientView.m_HandCardControl.SetPositively(true);

	//BYTE byTempCardIndex[MAX_INDEX]={0};
	//for (int i=0;i<MAX_INDEX; i++)
	//{
	//	if (cbCardData[i] > 0)
	//	{
	//		byTempCardIndex[m_GameLogic.SwitchToCardIndex(cbCardData[i])]++;
	//	}		
	//}
	//::memcpy(m_cbCardIndex, byTempCardIndex, MAX_INDEX);

	//m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	//m_GameClientView.UpdateGameView(NULL);
	//return 0;


	//m_GameClientView.PlayOperationFlash(2);
	//return 0;

	//m_GameClientView.m_wChiHuType=1;
	//m_GameClientView.m_wChiHuKind[1]=0x0001;
	//   m_GameClientView.m_wChiHuRight[1]=0x0400;
	//m_GameClientView.PlayChiHuFlash(1);


	////��ʾ����
	//BYTE byMask = WIK_NULL;
	////byMask = WIK_CHI_HU;
	////byMask |= WIK_PENG;
	//byMask |= WIK_LEFT;


	//tagGangCardResult bucardresult;
	//BYTE cbCardData[4]={0x11, 0x11, 0x11,0x11 };
	//::memset(&bucardresult, 0, sizeof(tagGangCardResult));
	//memcpy(bucardresult.cbCardData,cbCardData,sizeof(cbCardData));
	//bucardresult.cbCardCount =0;
	//m_GameClientView.m_ControlWnd.SetControlInfo(0x12,byMask,bucardresult);
	//m_GameClientView.m_ControlWnd.AddGangCardData(NULL, 0);
	//m_GameClientView.m_ControlWnd.ShowWindow(SW_SHOW);

	//   m_GameClientView.m_wChiHuType=1;
	//m_GameClientView.PlayFangPaoFlash(0);
	//m_GameClientView.PlayChiHuFlash(1);
	//return 0;

	//��������
	CThreadAnimationManager::FreeAll();

	KillGameTimer(IDI_START_GAME);
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	//m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	SafeShowOrHideWnd(&m_GameClientView.m_ControlWnd, SW_HIDE);

	//���ý���
	//m_ScoreView.ShowWindow(SW_HIDE);
	SafeShowOrHideWnd(&m_ScoreView, SW_HIDE);
	m_GameClientView.SetHaiDiCard(0);
	//m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetBankerUser(INVALID_CHAIR);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);

	//��������
	memset(m_byUserAction, 0 , sizeof(m_byUserAction));

	//�˿�����
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0/*,NULL*/);

	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
		m_GameClientView.m_TableCard[i].SetWinCardData(0);
		m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][0].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][1].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][2].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][3].SetCardData(NULL,0);
		m_GameClientView.m_HeapCard[i].SetCardData(0,0,0);
	}

	////�����˿�
	//for (WORD i=0;i<GAME_PLAYER;i++)
	//{
	//	m_cbHeapCardInfo[i][0]=0;
	//	m_cbHeapCardInfo[i][1]=HEAP_FULL_COUNT-(((i==m_wBankerUser)||((i+2)%4==m_wBankerUser))?28:26);
	//	m_GameClientView.m_HeapCard[SwitchViewChairID(i)].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],HEAP_FULL_COUNT);
	//}

	//״̬����
	//m_bHearStatus=false;

	//��Ϸ����
	m_wCurrentUser=INVALID_CHAIR;

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�����˿�
	m_wHeapHead=0;
	m_wHeapTail=0;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	m_byCanSelectType = WIK_NULL;
	m_byCanSelectCount = 0;
	ZeroMemory(m_byCanSelectCard,sizeof(m_byCanSelectCard));

	//������Ϣ
	SendUserReady(NULL,0);

	int iCount = GetCurGameUserNums();
	PlayWaitFlash(false);
	if (GAME_PLAYER != iCount)
	{
		PlayWaitFlash(true);
	}

	m_GameClientView.UpdateGameView(NULL);
	return 0;
}
//�������ɸ�Ӷ�����
LRESULT CGameClientDlg::OnSZStart(WPARAM wParam, LPARAM lParam)
{
	m_GameClientView.PlaySZFlashBT(false);
	KillGameTimer(IDI_SZSTART);
	SetGameTimer(m_wBankerUser,IDI_SZSTART,5);
	BYTE byMeChairID = GetMeChairID();
	if (byMeChairID == m_wBankerUser)
	{
		m_GameClientView.PlaySZFlashBT(true);
	}

	if (m_wBankerUser == GetMeChairID())
	{
		const tagUserData *pUserData = GetUserData(m_wBankerUser);//�û���Ϣ
		PlayGameSoundLanguage(pUserData->cbGender,TEXT("Banker"));
	}

	//char cbData[128]={0};
	//sprintf(cbData,"CGameClientDlg::OnSZStart-->ׯ��:%d ����:%d", m_wBankerUser, byMeChairID);
	//InsertSystemString(cbData);

	m_GameClientView.UpdateGameView(NULL);
	return 0;
}

//���ƶ��������
LRESULT CGameClientDlg::OnMoPai(WPARAM wParam, LPARAM lParam)
{
	bool bPlayerMode=IsLookonMode();
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	SafeShowOrHideWnd(&m_ScoreView, SW_HIDE);
	memset(m_byUserAction, 0 , sizeof(m_byUserAction));

	// �йܰ�ť����
	if(bPlayerMode)
	{
		m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_HIDE);
		m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_HIDE);
		m_GameClientView.SetHaiDiCard(0);
		//m_GameClientView.SetStatusFlag(false,false);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);	
	}
	else
	{
		m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_SHOW);
		m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_HIDE);
	}

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_cbHeapCardInfo[i][0]=0;
		m_cbHeapCardInfo[i][1]=HEAP_FULL_COUNT-(((i==m_wBankerUser)||((i+2)%4==m_wBankerUser))?28:26);
	}

	//�ַ��˿�
	BYTE cbTakeCount=(MAX_COUNT-1)*GAME_PLAYER+1;    //53��
	BYTE cbSiceFirst=HIBYTE(m_GameStartData.wSiceCount);
	BYTE cbSiceSecond=LOBYTE(m_GameStartData.wSiceCount);
	BYTE cbSiceCount=__min(cbSiceFirst,cbSiceSecond);
	WORD wTakeChairID=(m_wBankerUser-cbSiceFirst-cbSiceSecond+1+GAME_PLAYER*GAME_PLAYER)%GAME_PLAYER;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//������Ŀ
		BYTE cbValidCount=HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?cbSiceCount*2:0);
		BYTE cbRemoveCount=__min(cbValidCount,cbTakeCount);

		//��ȡ�˿�
		cbTakeCount-=cbRemoveCount;
		m_cbHeapCardInfo[wTakeChairID][(i==0)?1:0]+=cbRemoveCount;

		//����ж�
		if (0==cbTakeCount)
		{
			m_wHeapHead=wTakeChairID;
			m_wHeapTail=(m_wBankerUser-cbSiceFirst-cbSiceSecond+1+GAME_PLAYER*GAME_PLAYER/*+7-cbSiceFirst*/)%GAME_PLAYER;
			break;
		}

		//�л�����
		wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
	}

	//�滭����
	//m_cbTempKing[0]=m_GameStartData.cbKingCard;
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0/*,NULL*/);

	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);

		m_GameClientView.m_TableCard[wViewChairID].SetCardData(NULL,0);
		m_GameClientView.m_DiscardCard[wViewChairID].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[wViewChairID][0].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[wViewChairID][1].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[wViewChairID][2].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[wViewChairID][3].SetCardData(NULL,0);

		//��Ͻ���
		m_GameClientView.m_WeaveCard[i][0].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][1].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][2].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][3].SetDisplayItem(true);
		m_GameClientView.m_HeapCard[wViewChairID].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],HEAP_FULL_COUNT);

		if (i == GetMeChairID())
		{
			BYTE cbBankerCard = 0;
			if (i == m_wBankerUser)
			{
				cbBankerCard=m_GameStartData.cbCardData[MAX_COUNT-1];
				tagCardItem CardItem;
				CardItem.cbCardData = cbBankerCard;
				CardItem.bShoot = false;
				CardItem.bKingFlag = false;//(cbBankerCard == m_cbTempKing[0])?true:false;
				m_GameClientView.m_HandCardControl.SetCurrentCard(CardItem);
			}
			m_GameClientView.m_HandCardControl.SetCardData(m_GameStartData.cbCardData,MAX_COUNT-1,cbBankerCard/*,m_cbTempKing[0]*/);
		}
		else
		{
			WORD wIndex=(wViewChairID>=3)?2:wViewChairID;
			m_GameClientView.m_UserCard[wIndex].SetCardData(MAX_COUNT-1,(i==m_wBankerUser));
		}

		////�û��˿�
		//if (wViewChairID!=2)
		//{
		//	WORD wIndex=(wViewChairID>=3)?2:wViewChairID;
		//	m_GameClientView.m_UserCard[wIndex].SetCardData(MAX_COUNT-1,(i==m_wBankerUser));
		//}
		//else
		//{
		//	BYTE cbBankerCard=(i==m_wBankerUser)?m_GameStartData.cbCardData[MAX_COUNT-1]:0;
		//	tagCardItem CardItem;
		//	CardItem.cbCardData = cbBankerCard;
		//	CardItem.bShoot = false;
		//	CardItem.bKingFlag = (cbBankerCard == m_cbTempKing[0])?true:false;
		//	m_GameClientView.m_HandCardControl.SetCurrentCard(CardItem);
		//	m_GameClientView.m_HandCardControl.SetCardData(m_GameStartData.cbCardData,MAX_COUNT-1,cbBankerCard,m_cbTempKing[0]);
		//}
	}

	////������ʾ
	//if ((bPlayerMode==false)&&(m_wCurrentUser!=INVALID_CHAIR))
	//{
	//	WORD wMeChairID=GetMeChairID();
	//	if (m_wCurrentUser==wMeChairID)
	//	{
	//		m_GameClientView.SetStatusFlag(true,false);
	//	}
	//}

	////��������
	//if ((bPlayerMode==false)&&(m_GameStartData.cbUserAction!=WIK_NULL))
	//{
	//	ShowOperateControl(m_GameStartData.cbUserAction);
	//	SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
	//}

	m_bIsMeCanOutCard = false;
	m_wCurrentUser = INVALID_CHAIR;
	m_byUserAction[GetMeChairID()] = WIK_NULL;
	WORD wMeChairID=GetMeChairID();
	if (!bPlayerMode && m_wBankerUser==wMeChairID)
	{
		m_wCurrentUser =  wMeChairID; 
		//m_GameClientView.SetStatusFlag(true,false);
		if (WIK_NULL != m_GameStartData.cbUserAction)
		{
			m_byUserAction[wMeChairID] = m_GameStartData.cbUserAction;
			ShowOperateControl(m_GameStartData.cbUserAction,m_GameStartData.cbCardData[MAX_COUNT-1]);
			m_bIsMeCanOutCard = true;
			SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
		}
		else
		{
			SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_HEAR_STATUS);
		}
	}
	else
	{
		SetGameTimer(m_wBankerUser,IDI_OPERATE_CARD,TIME_HEAR_STATUS);
	}

	//�й�����
	m_bStustee = false;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetTrustee(SwitchViewChairID(i),m_GameStartData.bTrustee[i]);
	}

	//������
	if (bPlayerMode==false) ActiveGameFrame();

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	return 0;
}

//���Ӷ���
LRESULT CGameClientDlg::OnShowSZ(WPARAM wParam, LPARAM lParam)
{
	m_GameClientView.m_nDiceCount1 = wParam;
	m_GameClientView.m_nDiceCount2 = lParam;

	//��������
	m_GameClientView.SetDiceEffect(true);

	return 0;
}


////���Ʋ���
//bool CGameClientDlg::OnSubTingCard(const void * pBuffer, WORD wDataSize)
//{
//	//Ч������
//	ASSERT(wDataSize==sizeof(CMD_S_GameTingCard));
//	if (wDataSize!=sizeof(CMD_S_GameTingCard)) return false;
//
//	KillGameTimer(IDI_USER_TING_CARD);
//	//��Ϣ����
//	CMD_S_GameTingCard * pGameTingCard=(CMD_S_GameTingCard *)pBuffer;
//	if (!IsLookonMode() && NULL!=pGameTingCard && pGameTingCard->wTingCardUser == GetMeChairID())
//	{
//		//m_bHearStatus = false;
//		//m_GameClientView.m_btTingCard.ShowWindow(SW_SHOW);	
//		//m_GameClientView.m_btBuTing.ShowWindow(SW_SHOW);
//		m_wCurrentUser = pGameTingCard->wTingCardUser;
//		//����
//		SetGameTimer(m_wCurrentUser, IDI_USER_TING_CARD, TIME_USERTINGCARD);
//	}
//
//	return true;
//
//}

//���ƽ��
//bool CGameClientDlg::OnSubTingCardResult(const void * pBuffer, WORD wDataSize)
//{
//	//Ч������
//	ASSERT(wDataSize==sizeof(CMD_S_GameTingCard));
//	if (wDataSize!=sizeof(CMD_S_GameTingCard)) return false;
//
//	//��Ϣ����
//	CMD_S_GameTingCard * pGameTingCard=(CMD_S_GameTingCard *)pBuffer;
//	if (NULL != pGameTingCard)
//	{
//		WORD byTingUserViewID  = SwitchViewChairID(pGameTingCard->wTingCardUser);
//		m_GameClientView.m_bTingFlag[byTingUserViewID] = true;
//		//m_GameClientView.PlayTingCardFlash(byTingUserViewID);
//		if (!IsLookonMode() && (pGameTingCard->wTingCardUser == GetMeChairID()))
//		{
//			//m_bHearStatus = true;
//			KillGameTimer(IDI_USER_TING_CARD);
//			//m_GameClientView.m_btTingCard.ShowWindow(SW_HIDE);	
//			//m_GameClientView.m_btBuTing.ShowWindow(SW_HIDE);
//		}
//	}
//	m_GameClientView.UpdateGameView(NULL);
//	return true;
//}

//���ư�ť
LRESULT CGameClientDlg::OnTingCard(WPARAM wParam, LPARAM lParam)
{
	KillGameTimer(IDI_USER_TING_CARD);
	//m_GameClientView.m_btTingCard.ShowWindow(SW_HIDE);
	//m_GameClientView.m_btBuTing.ShowWindow(SW_HIDE);
	BYTE byTingTag = (BYTE)wParam;
	CMD_UserTingOperate TingOperate;
	//����״̬
	if (1 == byTingTag)
	{//����
		TingOperate.bTingCard = true;
	}
	else
	{//����
		TingOperate.bTingCard = false;
	}
	SendData(SUB_C_USERTINGCARD,&TingOperate,sizeof(TingOperate));
	return 0;
}

//���Ʋ���
LRESULT CGameClientDlg::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	if (GS_MJ_PLAY != GetGameStatus())
	{
		return 0;
	}
	if (m_GameClientView.m_ControlWnd.IsWindowVisible())
	{
		return 0;
	}

	//�����ж�
	if ((IsLookonMode()==true)||(m_wCurrentUser!=GetMeChairID())) 
	{
		return 0;
	}

	CString strBuffer;
	strBuffer.Format("client:CGameClientDlg::OnOutCard ");
	OutputDebugString(strBuffer);

	//�����ж�
	//if ((m_bHearStatus==true)&&(m_GameClientView.m_HandCardControl.GetCurrentCard()!=(BYTE)wParam)) return 0;

	//�����ж�
	//if(m_cbTempKing == (BYTE)wParam) return 0;

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	BYTE cbOutCardData=(BYTE)wParam;
	//m_GameLogic.RemoveCard(m_cbCardIndex,cbOutCardData);

	//�����˿�
	//BYTE cbCardData[MAX_COUNT];
	//BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
	//m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0,m_cbTempKing[0]);

	//���ý���
	KillGameTimer(IDI_OPERATE_CARD);
	KillGameTimer(IDI_OTHER_USEROUTCARD);

	m_GameClientView.UpdateGameView(NULL);
	//m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	//m_GameClientView.SetOutCardInfo(2,cbOutCardData);
	//m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	SafeShowOrHideWnd(&m_GameClientView.m_ControlWnd, SW_HIDE);

	//��������
	memset(m_byUserAction, 0 , sizeof(m_byUserAction));

	//��������
	//PlayCardSound(cbOutCardData);


	//��������
	CMD_C_OutCard OutCard;
	OutCard.cbCardData=cbOutCardData;
	SendData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));
	return 0;

}

//�˿˲���
LRESULT CGameClientDlg::OnCardOperate(WPARAM wParam, LPARAM lParam)
{
	KillGameTimer(IDI_OPERATE_CARD);
	KillGameTimer(IDI_OTHER_USEROUTCARD);

	//��������
	BYTE cbOperateCode=(BYTE)(wParam);
	BYTE cbOperateCard=(BYTE)(LOWORD(lParam));

	//m_GameClientView.SetStatusFlag(false,true);
	//m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	SafeShowOrHideWnd(&m_GameClientView.m_ControlWnd, SW_HIDE);

	if (m_wCurrentUser!=GetMeChairID())
	{
		return 0;
	}

	CString strBuffer;
	strBuffer.Format("client:CGameClientDlg::OnCardOperate cbOperateCode %d, cbOperateCard %d", cbOperateCode,cbOperateCard);
	OutputDebugString(strBuffer);

	// ��������ѡ��ʱӦ���ṩ���ѡ��
	if (WIK_PENG==cbOperateCode  ||  WIK_FILL==cbOperateCode ||  WIK_GANG==cbOperateCode)
	{
		//BYTE bTmpHandCard[MAX_COUNT]={0};
		//CopyMemory(bTmpHandCard, m_cbCardIndex, sizeof(bTmpHandCard));
		m_byCanSelectType = cbOperateCode;

		//// ���ǳ�����ң����Լ�Ҳ���Ǹղ��������
		//if (m_wOutCardUser != GetMeChairID()  &&  INVALID_CHAIR!=m_wOutCardUser)
		//{
		//	bTmpHandCard[m_GameLogic.SwitchToCardIndex(cbOperateCard)]++;
		//}

		//for(BYTE i=0; i<m_cbWeaveCount[GetMeChairID()]; i++)
		//{
		//	if (WIK_PENG == m_WeaveItemArray[GetMeChairID()][i].cbWeaveKind)
		//	{
		//		// ����ɸ�ʱ���Ѿ���������ҲӦ�������
		//		BYTE bCardIndex = m_GameLogic.SwitchToCardIndex(m_WeaveItemArray[GetMeChairID()][i].cbCenterCard);
		//		bTmpHandCard[bCardIndex] +=3;
		//	}
		//}

		//m_GameLogic.AnalyseGangPengCountCard(bTmpHandCard, cbOperateCode, cbOperateCard, m_byCanSelectCard, m_byCanSelectCount);
		cbOperateCard = m_byCanSelectCard[0];

		CString strBuffer;
		strBuffer.Format("client: CGameClientDlg::OnCardOperate  cbOperateCode=%d, count=%d", cbOperateCode, m_byCanSelectCount);
		OutputDebugString(strBuffer);

		if (1 < m_byCanSelectCount)
		{
			m_GameClientView.m_HandCardControl.SetSelectStatus(true, m_byCanSelectCard, m_byCanSelectCount);
			m_GameClientView.UpdateGameView(NULL);
			SetGameTimer(GetMeChairID(), IDI_SELECT_OUT_CARD, TIME_SELECT_OUT_CARD);
			return 0;
		}
	}

	SendCardCommand(cbOperateCode, cbOperateCard);	
	return 0;
}

////���ײ���
//LRESULT CGameClientDlg::OnOperateHaiDi(WPARAM wParam, LPARAM lParam)
//{
//	//��������
//	KillGameTimer(TIME_OPERATE_HAI_DI);
//	//m_GameClientView.m_btHaiDi.ShowWindow(SW_HIDE);
//	//m_GameClientView.m_btNoHaiDi.ShowWindow(SW_HIDE);
//	//m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
//
//
//	//������Ϣ
//	CMD_C_OperateHaiDi OperateHaiDi;
//	OperateHaiDi.cbTakeHaiDi=(BYTE)wParam;
//	SendData(SUB_C_OPERATE_HAI_DI,&OperateHaiDi,sizeof(OperateHaiDi));
//
//	return 0;
//}

//�û��й�
bool CGameClientDlg::OnSubTrustee(const void * pBuffer,WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_Trustee));
	if (wDataSize!=sizeof(CMD_S_Trustee)) return false;

	//��Ϣ����
	CMD_S_Trustee * pTrustee=(CMD_S_Trustee *)pBuffer;
	m_GameClientView.SetTrustee(SwitchViewChairID(pTrustee->wChairID),pTrustee->bTrustee);
	if ((IsLookonMode()==true)||(pTrustee->wChairID!=GetMeChairID()))
	{
		const tagUserData * pUserData=GetUserData(pTrustee->wChairID);
		TCHAR szBuffer[256]={0};
		if(pTrustee->bTrustee==true)
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("���ѡ�����йܹ���."),pUserData->szName);
		else
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("���ȡ�����йܹ���."),pUserData->szName);
		InsertSystemString(szBuffer);
	}

	return true;
}

//�Ϲܿ���
LRESULT CGameClientDlg::OnStusteeControl(WPARAM wParam, LPARAM lParam)
{
	//���ñ���
	m_wTimeOutCount=0;

	//����״̬
	if (m_bStustee==true)
	{
		m_bStustee=false;
		m_GameClientView.m_btAutoPlayOn.ShowWindow(true);
		m_GameClientView.m_btAutoPlayOff.ShowWindow(false);
		InsertSystemString(_T("��ȡ�����йܹ���."));
		CMD_C_Trustee Trustee;
		Trustee.bTrustee = false;
		SendData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));

	}
	else
	{
		m_bStustee=true;
		m_GameClientView.m_btAutoPlayOff.ShowWindow(true);
		m_GameClientView.m_btAutoPlayOn.ShowWindow(false);
		InsertSystemString(_T("��ѡ�����йܹ���."));
		CMD_C_Trustee Trustee;
		Trustee.bTrustee = true;
		SendData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));
	}
	//m_GameClientView.m_btStusteeControl.UpdateWindow();

	return 0;
}


//���������Ϸ
void CGameClientDlg::OnEventContinueGame()
{
	OnStart(0, 0);
}

//������Ϸ����
void CGameClientDlg::PlayGameSoundLanguage(BYTE cbGender, CString strSound, BYTE bSoundType)
{
	CString strGameSound="";
	//�Ա𣨲�����Ů������
	if (1==cbGender)
	{//Ů
		strGameSound=TEXT(m_sDialect+"\\W\\"+strSound);
	}
	else
	{//��
		strGameSound=TEXT(m_sDialect+"\\M\\"+strSound);
	}

	if (1 < bSoundType)
	{//��������
		TCHAR cBuffer[10]={""};
		int iRand=rand()%bSoundType+1;
		::_snprintf(cBuffer,sizeof(cBuffer),"%d",iRand);
		strGameSound=strGameSound+TEXT(cBuffer);
	}

	CString strBuffer;
	strBuffer.Format("client:CGameClientDlg::PlayGameSoundLanguage path%s",strGameSound);
	OutputDebugString(strBuffer);
	PlaySound(strGameSound,false);
	//PlayGameSound(AfxGetInstanceHandle(),strGameSound);
}

//������Ϸ�������
void CGameClientDlg::PlayGameSoundRand(BYTE cbGender, CString strSound, bool bSoundType)
{
	CString strGameSound="";
	//�Ա𣨲�����Ů������
	if (1==cbGender)
	{//Ů
		strGameSound=m_sDialect+TEXT("\\W\\")+strSound;
	}
	else
	{//��
		strGameSound=m_sDialect+TEXT("\\M\\")+strSound;
	}

	if (bSoundType)
	{//��������
		TCHAR cBuffer[10]={""};
		int iRand=rand()%4+1;
		::_snprintf(cBuffer,sizeof(cBuffer),"%d",iRand);
		strGameSound=strGameSound+TEXT("_")+TEXT(cBuffer);
	}
	//PlayGameSound(AfxGetInstanceHandle(),strGameSound);
	PlaySound(strGameSound,false);
}

void CGameClientDlg::PlaySound(CString szSoundRes, bool bLoop)
{
	if (!szSoundRes.IsEmpty())
	{
		TCHAR szResourcePath[MAX_PATH]={0};
		GetCurrentDirectory(MAX_PATH,szResourcePath);
		CString strPath(szResourcePath);
		strPath.Append("\\SparrowZZ\\Sound\\");
		strPath.Append(szSoundRes);
		strPath.Append(".mp3");

		PlayDirSound(strPath, true, bLoop); //������Ϸ��Ч
	}
}

//���ű�������
void CGameClientDlg::PlayMusic(CString szSoundRes)
{
	if (!szSoundRes.IsEmpty())
	{
		TCHAR szResourcePath[MAX_PATH]={0};
		GetCurrentDirectory(MAX_PATH,szResourcePath);
		CString strPath(szResourcePath);
		strPath.Append("\\SparrowZZ\\Sound\\");
		strPath.Append(szSoundRes);

		PlayBGMusic(strPath, 0, true);
	}
}


void CGameClientDlg::CheckCanHuPai(BYTE byCardData, int iX)
{
	BYTE byChaiID = GetMeChairID();
	if (NULL==m_byUserAction[byChaiID] && m_wCurrentUser==byChaiID)
	{
		if (0 != byCardData)
		{
			//������ʾ
			BYTE cbHuCardData[MAX_INDEX]={0};
			BYTE byTempCardIndex[MAX_INDEX]={0};
			::memcpy(byTempCardIndex, m_cbCardIndex, MAX_INDEX);
			m_GameLogic.RemoveCard(byTempCardIndex,byCardData);
			//m_cbTempKing[0]=0x01;
			//m_GameLogic.IsCanTingCard(byTempCardIndex,0,0,m_cbTempKing);
			//m_GameLogic.IsCanTingCard(byTempCardIndex,m_WeaveItemArray[m_wCurrentUser],m_cbWeaveCount[m_wCurrentUser],m_cbTempKing);
			int iCardNums = m_GameLogic.GetHuPaiData(cbHuCardData,MAX_INDEX);

			//int iCardNums = 1;

			if (iCardNums>0)
			{
				m_GameClientView.SetCanHuPaiData(cbHuCardData, iCardNums, iX);
				//char cbData[128]={0};
				//sprintf(cbData,"%x,%x,%x,%x,%x,%d",cbHuCardData[0],cbHuCardData[1],cbHuCardData[2],cbHuCardData[3],cbHuCardData[4],iCardNums);
				//InsertSystemString(cbData);

				//char chDa[32]={0};
				//sprintf(chDa, "----%x", byCardData);
				//InsertSystemString(chDa);
			}
			else
			{
				m_GameClientView.SetCanHuPaiData(NULL, 0, 0);
			}
		}
	}
}


//ɫ�Ӷ�����ť
LRESULT CGameClientDlg::OnSZFlashBT(WPARAM wParam, LPARAM lParam)
{
	KillGameTimer(IDI_SZSTART);
	m_GameClientView.PlaySZFlashBT(false);
	SendData(SUB_C_SZSTART);
	return 0;
}
//////////////////////////////////////////////////////////////////////////

//��õ�ǰ��Ϸ��Ҹ���
int CGameClientDlg::GetCurGameUserNums()
{
	int iCount = 0;

	for (int i=0; i<GAME_PLAYER; ++i)
	{
		const tagUserData * pUserData=GetUserData(i);
		if (NULL != pUserData)
		{
			++iCount;
		}
	}

	return iCount;
}

//�û�����
void __cdecl CGameClientDlg::OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	CGameFrameDlg::OnEventUserEnter(pUserData, wChairID, bLookonUser);

	//int iCount = GetCurGameUserNums();
	//if (GAME_PLAYER == iCount)
	//{
	//	PlayWaitFlash(false);
	//}
}
//�û��뿪
void __cdecl CGameClientDlg::OnEventUserLeave(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	CGameFrameDlg::OnEventUserLeave(pUserData, wChairID, bLookonUser);

	int iCount = GetCurGameUserNums();
	PlayWaitFlash(false);
	if (GAME_PLAYER != iCount)
	{
		for (int i=0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
			m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[i][0].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[i][1].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[i][2].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[i][3].SetCardData(NULL,0);
			m_GameClientView.m_HeapCard[i].SetCardData(0,0,0);
		}		
		PlayWaitFlash(true);
	}
	m_GameClientView.UpdateGameView(NULL);
}