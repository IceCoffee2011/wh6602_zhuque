#include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientDlg.h"
#include "GameLogic.h"
//////////////////////////////////////////////////////////////////////////

//��Ϸ��ʱ��
#define IDI_START_GAME				200									//��ʼ��ʱ��
#define IDI_OPERATE_CARD			202									//������ʱ��
#define IDI_OPERATE_HAI_DI			203									//���׶�ʱ��
#define IDI_OTHER_USEROUTCARD		205									//��ʵ���ƶ�ʱ��
#define IDI_GAME_END                206                                 //������ʱ��
#define IDI_SHOW_SZ                 207                                 //���Ӷ�ʱ��
#define IDI_HUANGZHUANG             208                                 //��ׯ��ʱ��
#define IDI_BIRD                    209                                 //����ʱ��
#define IDI_SOUND                   210                                 //������ʱ��
//��Ϸ��ʱ��
#define TIME_START_GAME				15									//��ʼ��ʱ��
#define TIME_HEAR_STATUS			15									//���ƶ�ʱ��
#define TIME_OPERATE_CARD			10									//������ʱ��
#define TIME_OPERATE_HAI_DI			10									//������ʱ��
#define TIME_USERTINGCARD			5									//������ʱ��
#define TIME_GAME_END				5000								//��������ʾʱ��
#define TIME_SHOW_SZ				800								    //����ʱ��


//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_CARD_OPERATE,OnCardOperate)
	ON_MESSAGE(IDM_OPERATE_HAI_DI,OnOperateHaiDi)
	ON_MESSAGE(IDM_TRUSTEE_CONTROL,OnStusteeControl)
	ON_MESSAGE(IDM_SHOW_SZ,OnShowSZ)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView, &m_ScoreView)
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_bBirdUserID=4;

	//״̬����
	m_bHearStatus=false;
	m_bIsMeCanOutCard = false;

	//�йܱ���
	m_bStustee=false;
	m_wTimeOutCount =0;

	//��������
	m_cbGender=0;
	m_InfoSound="";

	//��������
	m_wHeapHand=0;
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

	//���Ա���
	m_sDialect="GY"; 

	return;
}

//��������
CGameClientDlg::~CGameClientDlg()
{
}

//��ʼ����
bool CGameClientDlg::InitGameFrame()
{
	//���ñ���
	//SetWindowText(TEXT("��ɳ�齫  --  Ver��1.0.5.3"));

	//���Ը�ʽ
	//m_sDialect.Format("%s_",m_strLanguage.c_str()); 

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//������Դ
	m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
	m_GameClientView.m_btHaiDi.ShowWindow(SW_HIDE);
	m_GameClientView.m_btNoHaiDi.ShowWindow(SW_HIDE);

	m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_SHOW);
	m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_HIDE);
	//m_ScoreView.ShowWindow(SW_SHOW);

	m_GameClientView.SetGameClientDlgPtr(this);
	//m_GameClientView.m_FlashControl.ShowWindow(SW_HIDE);
	//m_GameClientView.m_ControlWnd.ShowWindow(SW_SHOW);


	//m_ScoreView.ShowWindow(SW_SHOW);
	//m_ScoreView.SetCountDown(true);


	//
	////��ʾ����
	//BYTE byMask = WIK_NULL;
	//byMask = WIK_CHI_HU;
	//byMask |= WIK_PENG;
	//byMask |= WIK_LEFT;

	//tagGangCardResult bucardresult;
	//BYTE cbCardData[4]={0x11, 0x11, 0x11,0x11 };
	//::memset(&bucardresult, 0, sizeof(tagGangCardResult));
	//memcpy(bucardresult.cbCardData,cbCardData,sizeof(cbCardData));
	//bucardresult.cbCardCount =1;
	//m_GameClientView.m_ControlWnd.SetControlInfo(0x12,byMask,bucardresult);
	//m_GameClientView.m_ControlWnd.AddGangCardData(NULL, 0);
	//m_GameClientView.m_ControlWnd.ShowWindow(SW_SHOW);


	//HWND hwnd=AfxGetMainWnd()->m_hWnd;
	//DWORD proID;//�������ID
	//GetWindowThreadProcessId(hwnd,&proID);//��ȡ����ID
	//char chData[128]={0};
	//sprintf(chData, "%d", proID);
	//InsertSystemString(chData);

	/*tagGangCardResult bucardresult;
	::memset(&bucardresult, 0, sizeof(tagGangCardResult));
	::memcpy(bucardresult.cbCardData, pSendCard->byBuCard, sizeof(BYTE)*pSendCard->byBuCardNums);
	bucardresult.cbCardCount = 5;
	m_GameClientView.m_ControlWnd.SetControlInfo(NULL,cbActionMask,bucardresult);
	m_GameClientView.m_ControlWnd.AddGangCardData(pSendCard->byGangCard, pSendCard->byGangCardNums);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_SHOW);*/


	m_GameClientView.InitViewFrame();

	//SetTaskWndShow(true);
	//������Ϸ��������
	PlayMusic("SparrowCS.mp3");	

	SetChatShowTag(true,false);
	return true;
}

//���ÿ��
void CGameClientDlg::ResetGameFrame()
{
	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_bBirdUserID=4;

	//״̬����
	m_bHearStatus=false;
	m_bIsMeCanOutCard = false;

	//�йܱ���
	m_bStustee=false;
	m_wTimeOutCount =0;

	//��������
	m_cbGender=0;
	m_InfoSound="";


	//��������
	m_wHeapHand=0;
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
	case IDI_START_GAME:		//��ʼ��Ϸ
		{
			if (nElapse==0)
			{
				PostMessage(WM_CLOSE);
			//	m_ScoreView.ShowWindow(false);
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
				if (wChairID==wMeChairID)
				{
					if(m_bStustee==false)
					{
						m_wTimeOutCount++;
						if(m_wTimeOutCount==3)
						{
							InsertSystemString(TEXT("��������γ�ʱ���л�Ϊ��ϵͳ�йܡ�ģʽ."));
							OnStusteeControl(0,0);
							m_GameClientView.UpdateGameView(NULL);
						}
					}

					if( m_GameClientView.m_ControlWnd.m_btChiHu.IsWindowVisible() 
						&& m_GameClientView.m_ControlWnd.m_btChiHu.IsWindowEnabled())
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
					else
					{
					  OnCardOperate(WIK_NULL,0);
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
					if(m_bStustee==false)
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
	case IDI_OPERATE_HAI_DI:	//���׶�ʱ��
		{
			//��ʱ�ж�
			if (nElapse==0 || m_bStustee==true)
			{
				if (wChairID==m_wCurrentUser && wChairID==GetMeChairID())
				{
                   OnOperateHaiDi(0,0);
				}
				return false;
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
	case SUB_S_OPERATE_HAI_DI:	//���ײ���
		{
			return OnSubOperateHaiDi(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_TRUSTEE:			//�û��й�
		{
			return OnSubTrustee(pBuffer,wDataSize);
		}
	}

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
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
				m_ScoreView.ShowWindow(SW_HIDE);
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
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
	case GS_MJ_PLAY:	//��Ϸ״̬
	case GS_MJ_HAI_DI:	//����״̬
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
			m_bHearStatus=(pStatusPlay->cbHearStatus>0)?true:false;
			memset(m_byUserAction, WIK_NULL, sizeof(m_byUserAction));
		

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



			//�й�����
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

			//�û��˿�
			if (m_wCurrentUser==byMeChairID && cbGameStation!=GS_MJ_HAI_DI)
			{
				BYTE cbCardCount=pStatusPlay->cbCardCount;
				m_GameClientView.m_HandCardControl.SetCardData(pStatusPlay->cbCardData,cbCardCount-1,pStatusPlay->cbCardData[0]);
			}
			else
			{
				m_GameClientView.m_HandCardControl.SetCardData(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,0); 
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
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.m_DiscardCard[wViewChairID].SetCardData(m_cbDiscardCard[i],m_cbDiscardCount[i]);
			}

			//��������
			if (bIsMeIsLookonMode==false) 
			{
				m_GameClientView.m_HandCardControl.SetPositively(true);
				m_GameClientView.m_HandCardControl.SetDisplayItem(true);
			}

			//�����˿�
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_cbHeapCardInfo[i][0]=0;
				m_cbHeapCardInfo[i][1]=HEAP_FULL_COUNT-(((i==m_wBankerUser)||((i+2)%4==m_wBankerUser))?28:26);
			}

			//�ַ��˿�
			BYTE cbTakeCount=108-m_cbLeftCardCount;   
			BYTE cbSiceFirst=HIBYTE(pStatusPlay->wSiceCount);   
			BYTE cbSiceSecond=LOBYTE(pStatusPlay->wSiceCount);
			WORD wTakeChairID=(m_wBankerUser+7-cbSiceFirst)%GAME_PLAYER;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//������Ŀ
				BYTE cbValidCount=HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?(cbSiceSecond-1)*2:0);
				BYTE cbRemoveCount=__min(cbValidCount,cbTakeCount);

				//��ȡ�˿�
				cbTakeCount-=cbRemoveCount;
				m_cbHeapCardInfo[wTakeChairID][(i==0)?1:0]+=cbRemoveCount;
 
                //ȷ�������ػ����һ���Ƶ�λ��
				//if ((cbGameStation==GS_MJ_HAI_DI) && (0==i) && (cbRemoveCount>0 && cbRemoveCount<HEAP_FULL_COUNT))
				//{
				//	m_cbHeapCardInfo[wTakeChairID][0]=HEAP_FULL_COUNT-cbRemoveCount-1;
				//	m_cbHeapCardInfo[wTakeChairID][1]=cbRemoveCount;
				//}

				//����ж�
				if (cbTakeCount==0)
				{
					m_wHeapHand=wTakeChairID;
					m_wHeapTail=(m_wBankerUser+7-cbSiceFirst)%GAME_PLAYER;
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



			//��������
			if (cbGameStation==GS_MJ_PLAY && !bIsMeIsLookonMode)
			{
			     if (byMeChairID == m_wCurrentUser)
				 {
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
							m_GameClientView.m_ControlWnd.SetControlInfo(pStatusPlay->cbActionCard,cbActionMask,bucardresult);
							m_GameClientView.m_ControlWnd.AddGangCardData(pStatusPlay->byGangCard, pStatusPlay->byGangCardNums);

							if (m_bHearStatus)
							{
								if (pStatusPlay->cbActionMask&WIK_CHI_HU)
								{
									//�����
									OnCardOperate(WIK_CHI_HU,pStatusPlay->cbActionCard);
									return true;
								}
								//ȡ��,���ҳ���,�����ö�ʱ������.
								OnCardOperate(WIK_NULL,0);
								BYTE cbCardData=m_GameClientView.m_HandCardControl.GetCurrentCard();
								OnOutCard(cbCardData,cbCardData);
							}
							else
							{
								//���ö�ʱ������
								m_bIsMeCanOutCard  = true;
								SetGameTimer(byMeChairID,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
							}
						}
						else
						{//����
							m_byUserAction[byMeChairID] = WIK_NULL;
							m_wCurrentUser = byMeChairID;
							memset(m_byUserAction, 0, sizeof(m_byUserAction));
							SetGameTimer(byMeChairID,IDI_OTHER_USEROUTCARD,TIME_HEAR_STATUS-10);
							//������ʾ
							m_GameClientView.SetStatusFlag(true,false);
						}
					}
				 }
				 else
				 {//����
						if ((pStatusPlay->cbActionMask!=WIK_NULL) /*&& (!pStatusPlay->bIsAlreadyOper)*/)
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
							m_GameClientView.m_ControlWnd.SetControlInfo(pStatusPlay->cbActionCard,cbActionMask,bucardresult);
							m_GameClientView.m_ControlWnd.AddGangCardData(pStatusPlay->byGangCard, pStatusPlay->byGangCardNums);

							//���ö�ʱ������
							m_wCurrentUser = byMeChairID;
							SetGameTimer(byMeChairID,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
						}
				}
			}


			//��������
			else if (cbGameStation==GS_MJ_HAI_DI && !bIsMeIsLookonMode)
			{
				m_GameClientView.m_btHaiDi.ShowWindow(SW_HIDE);
				m_GameClientView.m_btNoHaiDi.ShowWindow(SW_HIDE);
				//���ÿ���
				if (byMeChairID==m_wCurrentUser)
				{
					m_GameClientView.m_btHaiDi.ShowWindow(SW_SHOW);
					m_GameClientView.m_btNoHaiDi.ShowWindow(SW_SHOW);
				}

				//����ʱ��
				SetGameTimer(m_wCurrentUser,IDI_OPERATE_HAI_DI,TIME_OPERATE_HAI_DI);
			}

			// �йܰ�ť����
			if(bIsMeIsLookonMode==true)
			{
				m_GameClientView.m_btHaiDi.ShowWindow(SW_HIDE);
				m_GameClientView.m_btNoHaiDi.ShowWindow(SW_HIDE);
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
	KillGameTimer(IDI_OTHER_USEROUTCARD);
}

//��Ϸ��ʼ
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
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


	::memset(&m_GameStartData, 0, sizeof(CMD_S_GameStart));
	::memcpy(&m_GameStartData, pGameStart, sizeof(CMD_S_GameStart));
	
	//������ж�ʱ��
	ClearAllTimer();
	m_bIsMeCanOutCard = false;
	//����״̬
	SetGameStatus(GS_MJ_PLAY);
	//m_GameClientView.SetDiceEffect(true);
	SetTimer(IDI_SHOW_SZ,TIME_SHOW_SZ,NULL);

	//���ñ���
	m_bHearStatus=false;
	m_wBankerUser=pGameStart->wBankerUser;
	m_wCurrentUser=pGameStart->wCurrentUser;
	m_cbLeftCardCount=MAX_REPERTORY-GAME_PLAYER*(MAX_COUNT-1)-1;

	m_GameClientView.m_nDiceCount1 =HIBYTE( pGameStart->wSiceCount-1);
	m_GameClientView.m_nDiceCount2 =LOBYTE( pGameStart->wSiceCount-1);

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�����˿�
	BYTE cbCardCount=(GetMeChairID()==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
	m_GameLogic.SwitchToCardIndex(pGameStart->cbCardData,cbCardCount,m_cbCardIndex);

	//���ý���
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_HandCardControl.SetPositively(bPlayerMode);
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));
	m_ScoreView.ShowWindow(SW_HIDE);
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);

	return true;

}

//�û�����
bool CGameClientDlg::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard)) return false;

	//��Ϣ����
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	m_wOutCardUser=pOutCard->wOutCardUser;
	m_cbOutCardData=pOutCard->cbOutCardData;
	m_bIsMeCanOutCard = false;

	KillGameTimer(IDI_OPERATE_CARD); 
	KillGameTimer(IDI_OTHER_USEROUTCARD);


	m_GameClientView.SetStatusFlag(false,false);

	memset(m_byUserAction, 0 , sizeof(m_byUserAction));

	////�����˿�
	//if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	//{
	//	//�����˿� 
	//	WORD wOutViewChairID=SwitchViewChairID(m_wOutCardUser);
	//	m_GameClientView.m_DiscardCard[wOutViewChairID].AddCardItem(m_cbOutCardData);
	//}

	//�����û�
	if ((IsLookonMode()==true)||(pOutCard->wOutCardUser!=wMeChairID))
	{
		//��������
		
		//PlayCardSound(pOutCard->cbOutCardData);
		//���ƽ���
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(wOutViewChairID,pOutCard->cbOutCardData);

		//�����˿�
		if (wOutViewChairID==2)
		{
			//ɾ���˿�
			BYTE cbCardData[MAX_COUNT];
			m_GameLogic.RemoveCard(m_cbCardIndex,pOutCard->cbOutCardData);

			//�����˿�
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
			m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0);
		}
		else
		{
			WORD wUserIndex=(wOutViewChairID>2)?2:wOutViewChairID;
			m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(false);
		}
	} 
	//����
	const tagUserData *pUserData = GetUserData(pOutCard->wOutCardUser);//�û���Ϣ
	PlayCardSound(pUserData->cbGender, pOutCard->cbOutCardData);
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

	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

	//������ж�ʱ��
	ClearAllTimer();
	m_bIsMeCanOutCard = false;
	
	//���ñ���
	WORD wMeChairID=GetMeChairID();
	m_wCurrentUser=pSendCard->wCurrentUser;

	//�����˿�
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		//�����˿� 
		WORD wOutViewChairID=SwitchViewChairID(m_wOutCardUser);
		m_GameClientView.m_DiscardCard[wOutViewChairID].AddCardItem(m_cbOutCardData);

		////���ñ���
		//m_cbOutCardData=0;
		//m_wOutCardUser=INVALID_CHAIR;
	}

	//���ñ���
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;

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
			m_GameClientView.m_HandCardControl.SetCurrentCard(pSendCard->cbCardData);
		}

		//�۳��˿�
		if ( m_byUserAction[pSendCard->wCurrentUser] != WIK_NULL 
			&& (m_byUserAction[pSendCard->wCurrentUser] & WIK_FILL 
				||m_byUserAction[pSendCard->wCurrentUser] & WIK_GANG) )
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
			m_GameClientView.m_ControlWnd.SetControlInfo(pSendCard->cbCardData,cbActionMask,bucardresult);
			m_GameClientView.m_ControlWnd.AddGangCardData(pSendCard->byGangCard, pSendCard->byGangCardNums);
			m_GameClientView.m_ControlWnd.ShowWindow(SW_SHOW);

			//���ý���
			//m_GameClientView.m_ControlWnd.SetControlInfo(cbActionCard,cbActionMask,GangCardResult);
			//������
		}

		if (m_bHearStatus)
		{//m_GameClientView.m_ControlWnd.m_btChiHu.IsWindowVisible() && m_GameClientView.m_ControlWnd.m_btChiHu.IsWindowEnabled()
			if(pSendCard->cbActionMask&WIK_CHI_HU)
			{
				OnCardOperate(WIK_CHI_HU,0);
			}
			else
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
		//������ʾ
		m_GameClientView.SetStatusFlag(true,false);
		ActiveGameFrame();
	}
	else
	{
		SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_HEAR_STATUS);
		//������ʾ
		m_GameClientView.SetStatusFlag(false,false);
	}

	if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
	{
		if ((WIK_CHI_HU&pSendCard->cbActionMask)>0)
		{
			tagGangCardResult GangCardResult;
			ZeroMemory(&GangCardResult,sizeof(GangCardResult));
			m_GameClientView.m_ControlWnd.SetControlInfo(pSendCard->cbCardData,pSendCard->cbActionMask,GangCardResult);
			InsertSystemString("success!");
		}
		else if ( m_bHearStatus)
		{//����
			OnOutCard(m_GameClientView.m_HandCardControl.GetCurrentCard(), pSendCard->cbCardData );
		}
	}

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

	m_bIsMeCanOutCard = false;
	KillGameTimer(IDI_OPERATE_CARD);
	BYTE byChairID = GetMeChairID();
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


		//��ʾ����
		::memcpy(bucardresult.cbCardData, pOperateNotify->byBuCard, sizeof(BYTE)*pOperateNotify->byBuCardNums);
		bucardresult.cbCardCount = pOperateNotify->byBuCardNums;
		m_GameClientView.m_ControlWnd.SetControlInfo(pOperateNotify->cbActionCard,cbActionMask,bucardresult);
		m_GameClientView.m_ControlWnd.AddGangCardData(pOperateNotify->byGangCard, pOperateNotify->byGangCardNums);

		SetGameTimer(byChairID,IDI_OPERATE_CARD,TIME_OPERATE_CARD);

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

	//��������
	BYTE cbPublicCard=TRUE;
	WORD wOperateUser=pOperateResult->wOperateUser;
	BYTE cbOperateCard=pOperateResult->cbOperateCard;
	WORD wOperateViewID=SwitchViewChairID(wOperateUser);

	//���Ʊ���
	if (pOperateResult->cbOperateCode!=WIK_NULL)
	{
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}

	KillGameTimer(IDI_OTHER_USEROUTCARD);
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

	m_bIsMeCanOutCard = false;
	memset(m_byUserAction, 0, sizeof(m_byUserAction));
	//�������
	if ((pOperateResult->cbOperateCode&(WIK_GANG|WIK_FILL|WIK_TING))>0)
	{//�������,�����ǲ����ܡ���
		//Ϊ��ǰ�������
		m_wCurrentUser=INVALID_CHAIR;
		//���ñ���
		if ((pOperateResult->cbOperateCode==WIK_TING)&&(wOperateUser==GetMeChairID())) 
		{
			m_bHearStatus = true;
		}

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

		//�˿�����
		if (GetMeChairID()==wOperateUser)
		{
			m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pOperateResult->cbOperateCard)]=0;
		}

		//�����˿�
		if (GetMeChairID()==wOperateUser)
		{
			BYTE cbCardData[MAX_COUNT];
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
			m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0);
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

		//ɾ���˿�
		if (GetMeChairID()==wOperateUser)
		{
			m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard,1);
			m_GameLogic.RemoveCard(m_cbCardIndex,cbWeaveCard,cbWeaveCardCount-1);
		}

		//�����˿�
		if (GetMeChairID()==wOperateUser)
		{
			BYTE cbCardData[MAX_COUNT];
			BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
			m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount-1,cbCardData[cbCardCount-1]);
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
	m_GameClientView.SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),false);
	m_GameClientView.PlayOperationFlash(wOperateViewID);

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//��������
	const tagUserData *pUserData = GetUserData(pOperateResult->wOperateUser);//�û���Ϣ
	PlayActionSound(pUserData->cbGender, pOperateResult->cbOperateCode);

	return true;
}

//���ײ���
bool CGameClientDlg::OnSubOperateHaiDi(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_OperateHaiDi));
	if (wDataSize!=sizeof(CMD_S_OperateHaiDi)) return false;

	//��Ϣ����
	CMD_S_OperateHaiDi * pOperateHaiDi=(CMD_S_OperateHaiDi *)pBuffer;

	//����״̬
	SetGameStatus(GS_MJ_HAI_DI);

	//���ñ���
	m_bIsMeCanOutCard = false;
	m_wCurrentUser=INVALID_CHAIR;
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

	//�����˿�
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		//�����˿�
		WORD wOutViewChairID=SwitchViewChairID(m_wOutCardUser);
		m_GameClientView.m_DiscardCard[wOutViewChairID].AddCardItem(m_cbOutCardData);

		//���ñ���
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}

	//��������
	if ((IsLookonMode()==false)&&(GetMeChairID()==pOperateHaiDi->wCurrentUser))
	{
		m_wCurrentUser = GetMeChairID();
		m_GameClientView.m_btHaiDi.ShowWindow(SW_SHOW);
		m_GameClientView.m_btNoHaiDi.ShowWindow(SW_SHOW);
		ActiveGameFrame();
	}

	//ʱ������
	SetGameTimer(pOperateHaiDi->wCurrentUser,IDI_OPERATE_HAI_DI,TIME_OPERATE_HAI_DI);

	return true;
}

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

	//����״̬
	SetGameStatus(GS_MJ_FREE);
	m_GameClientView.SetStatusFlag(false,false);

	
	//���ÿؼ�
	m_bIsMeCanOutCard = false;
	//m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.m_btHaiDi.ShowWindow(SW_HIDE);
	m_GameClientView.m_btNoHaiDi.ShowWindow(SW_HIDE);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_HandCardControl.SetPositively(false);
	for (int i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetOutCardInfo(i, 0);
	}
	


	WORD MychairId=GetMeChairID();
	WORD BankerchairID=SwitchViewChairID(m_wBankerUser);
	m_ScoreView.GetViewID(MychairId,BankerchairID);

	m_GameClientView.m_wChiHuType=pGameEnd->iChiHuType;  //  0:��ׯ ;  1:���� ; 2:����;  3:��������;  4:������

	if (/*pGameEnd->cbHaiDiCard==0*/false == pGameEnd->bIsHuangzhuang)
	{
		tagGameScoreInfo GameScoreInfo;
		GameScoreInfo.iCellScore=pGameEnd->iCellScore;
		GameScoreInfo.wBankerUser=m_wBankerUser;
		GameScoreInfo.cbBirdCard=pGameEnd->cbBirdCard;
		GameScoreInfo.cbChiHuCard=pGameEnd->cbChiHuCard;
		GameScoreInfo.wProvideUser=pGameEnd->wProvideUser;
		GameScoreInfo.wBirdHitUser=pGameEnd->wBirdHitUser;

		//�ɼ���ʾ�ڼ�ʱ����Ի���
		TCHAR szBuffer[64];
		_snprintf(szBuffer,CountArray(szBuffer),TEXT("\n���ֽ���,�ɼ�ͳ��:"));
		InsertGeneralString(szBuffer,RGB(255,0,0),true);
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			const tagUserData * pUserData=GetUserData(i);
			GameScoreInfo.cbTimes[i]=pGameEnd->cbTimes[i];
			GameScoreInfo.iCellScore=pGameEnd->iCellScore;
			GameScoreInfo.lScore[i]=pGameEnd->lGameScore[i];
			GameScoreInfo.wCardType[i]=pGameEnd->wChiHuKind[i];
			GameScoreInfo.iChiHuKind[i]=pGameEnd->iChiHuKind[i];
			lstrcpyn(GameScoreInfo.szUserName[i],pUserData->szName,CountArray(GameScoreInfo.szUserName[i]));

			//��������
			if (GameScoreInfo.szUserName[i]!=0)
			{
				WORD wIndex=((i+GAME_PLAYER)-GameScoreInfo.wBankerUser)%GAME_PLAYER;
				WORD wViewID=SwitchViewChairID(i);
				if (2==wViewID)
				{
					_snprintf(szBuffer,CountArray(szBuffer),TEXT("%s��"),GameScoreInfo.szUserName[i]);
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

        
		//�ж��Ƿ����������ڡ�����
		m_bZM=false;
		m_bFP=false;
		bool m_bTP=false;  
		if (GameScoreInfo.wProvideUser != INVALID_CHAIR)
		{
			if(CHK_NULL != GameScoreInfo.wCardType[GameScoreInfo.wProvideUser])
			{
				m_bZM=true;
			}
			else /* if(CHK_NULL == GameScoreInfo.wCardType[GameScoreInfo.wProvideUser])*/
			{
				m_bFP=true;
			}	
		}
		else /*if (GameScoreInfo.wProvideUser == INVALID_CHAIR)*/
		{
			m_bTP=true;
		}

		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			BYTE byViewID = SwitchViewChairID(i);
			m_GameClientView.m_wChiHuKind[byViewID]=pGameEnd->wChiHuKind[i];
			m_GameClientView.m_wChiHuRight[byViewID]=pGameEnd->wChiHuRight[i];
		}

		m_ScoreView.SetScoreInfo(GameScoreInfo);
		//m_ScoreView.ShowWindow(SW_SHOW);
		m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
		m_GameClientView.m_bIsHuangZhuang=false;
		m_GameClientView.SetHaiDiCard(pGameEnd->cbHaiDiCard);
	}
	else 
	{//���׻�ׯ
		DeductionTableCard(true);
		m_GameClientView.SetHaiDiCard(pGameEnd->cbHaiDiCard);
		m_GameClientView.m_bIsHuangZhuang=true;
	}



	//�ж��Ƿ���������
	bool bIsLeft=false;
	for (int i=0;i<GAME_PLAYER;++i)
	{
		if (pGameEnd->IsUserLeft[i])
		{
			bIsLeft=true;
			break;
		}	
	}


	//�û������˿�
	int iChiHuNum=0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		if (pGameEnd->wChiHuKind[i]!=CHK_NULL) 
		{
			m_GameClientView.SetUserAction(wViewChairID,WIK_CHI_HU);
			m_byUserAction[i] = WIK_CHI_HU;
			iChiHuNum++;
		}
		m_GameClientView.m_TableCard[wViewChairID].SetCardData(pGameEnd->cbCardData[i],pGameEnd->cbCardCount[i]);
	}

	//���ź��ƶ���
	m_GameClientView.m_iChiHuNums=iChiHuNum;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		if (!bIsLeft)
		{
			m_GameClientView.PlayChiHuFlash(wViewChairID);
			m_bBirdUserID=SwitchViewChairID(GameScoreInfo.wBirdHitUser);
			m_GameClientView.m_cbBirdCardData=pGameEnd->cbBirdCard;
		}

		
	}


	//�����˿�
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);


	//��������
	const tagUserData *pUserData = GetUserData(GetMeChairID());//�û���Ϣ
	LONGLONG lScore=pGameEnd->lGameScore[GetMeChairID()];
	if (lScore>0L) 
	{
		if (m_bZM) //���� 
		{
			PlayGameSoundLanguage(pUserData->cbGender,TEXT("zimo"),false);
		}
		else if(m_bFP) //����
		{
			PlayGameSoundLanguage(pUserData->cbGender,TEXT("chihu"),false);
		}
		//else
		//{
		//	PlaySound("GAME_WIN",false);
		//}
	}
	else if (lScore<0L) 
	{
		PlaySound("GAME_LOST",false);
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_LOST"));
	}
	else
	{
		PlaySound("GAME_END",false);
	}


	//���ý���
	if (IsLookonMode()==true)
	{
		m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_HIDE);
		m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_HIDE);
		m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
        m_ScoreView.ShowWindow(true);
		if (pGameEnd->cbChiHuCard == 0)
		{
			m_ScoreView.ShowWindow(false);
		}
	}

	//���ý���
	if (IsLookonMode()==false)
	{
		if (pGameEnd->bIsHuangzhuang)
		{//��ׯ
			m_ScoreView.ShowWindow(SW_HIDE);
			SetTimer(IDI_HUANGZHUANG,1500,NULL);
			SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
			m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		}
		else
		{//��ʱ3.5����ʾ�����
			SetTimer(IDI_GAME_END,TIME_GAME_END,NULL);
			m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
		}
	}

	//ȡ���й�
	if(m_bStustee)
	{
        OnStusteeControl(0,0);
	}

	//if (pGameEnd->bIsHuangzhuang)
	//{//��ׯ
	//	m_ScoreView.ShowWindow(SW_HIDE);
	//	m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
	//}
	//else
	//{//��ʱ2����ʾ�����
	//    SetTimer(IDI_GAME_END,TIME_GAME_END,NULL);
	//	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	//}
		

	//���½���
	m_GameClientView.UpdateGameView(NULL);


	return true;
}

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
			PlayGameSoundLanguage(cbGender,TEXT("chi"),false);
			break;
		}
	case WIK_PENG:		//����
		{
			PlayGameSoundLanguage(cbGender,TEXT("peng"),false);
			break;
		}
	case WIK_FILL:	//����
	case WIK_GANG:	
		{
			PlayGameSoundLanguage(cbGender,TEXT("buzhang"),false);
			break;
		}
	case WIK_TING:		//����״̬��������ʾ��
		{

			PlayGameSoundLanguage(cbGender,TEXT("gang"),false);
			break;
		}
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
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapHand][0]+m_cbHeapCardInfo[m_wHeapHand][1];
		if (cbHeapCount==HEAP_FULL_COUNT) 
			m_wHeapHand=(m_wHeapHand+1)%CountArray(m_cbHeapCardInfo);

		//�����˿�
		m_cbLeftCardCount--;
		m_cbHeapCardInfo[m_wHeapHand][0]++;

		//�����˿�
		WORD wHeapViewID=SwitchViewChairID(m_wHeapHand);
		WORD wMinusHeadCount=m_cbHeapCardInfo[m_wHeapHand][0];
		WORD wMinusLastCount=m_cbHeapCardInfo[m_wHeapHand][1];
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
	//if (cbUserAction==WIK_TING)
	//{
	//	return true;
	//}
	//��ʾ����
	tagGangCardResult bucardresult;
	::memset(&bucardresult, 0, sizeof(tagGangCardResult));
	::memcpy(bucardresult.cbCardData, m_GameStartData.byBuCard, sizeof(BYTE)*m_GameStartData.byBuCardNums);
	bucardresult.cbCardCount = m_GameStartData.byBuCardNums;
	m_GameClientView.m_ControlWnd.SetControlInfo(cbStartData,cbUserAction,bucardresult);
	m_GameClientView.m_ControlWnd.AddGangCardData(m_GameStartData.byGangCard, m_GameStartData.byGangCardNums);

	return true;
}


//ʱ����Ϣ
void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	//���Ӷ�ʱ��
	if (nIDEvent==IDI_SHOW_SZ)
	{
		//��������
		KillTimer(IDI_SHOW_SZ);
		m_GameClientView.SetDiceEffect(true);
		PlaySound("GAME_START",false);
		return;
	}
	
	//������ʱ��
	if (nIDEvent==IDI_SOUND)
	{
		KillTimer(IDI_SOUND);
		PlayGameSoundLanguage(m_cbGender,m_InfoSound,false);
		return;
	}
	
	//������ʱ��
	if (nIDEvent==IDI_HUANGZHUANG)
	{
		KillTimer(IDI_HUANGZHUANG);
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		return;
	}

	//����ʱ��
	if (nIDEvent==IDI_GAME_END)
	{
		KillGameTimer(IDI_START_GAME);
		KillTimer(IDI_GAME_END);
		SetTimer(IDI_BIRD,2000,NULL);
		BYTE byViewID=m_bBirdUserID;
		m_GameClientView.SetBirdEffect(true,byViewID);
		return;
	}

	//������ʱ��
	if (nIDEvent==IDI_BIRD)
	{
		KillTimer(IDI_BIRD);
		KillGameTimer(IDI_START_GAME);
		m_ScoreView.ShowWindow(SW_SHOW);
		m_ScoreView.SetCountDown(true);
		return;
	}


	__super::OnTimer(nIDEvent);
}



//��ʼ��ť
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	

	//��������
	KillGameTimer(IDI_START_GAME);
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_btHaiDi.ShowWindow(SW_HIDE);
	m_GameClientView.m_btNoHaiDi.ShowWindow(SW_HIDE);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

	//���ý���
	m_ScoreView.ShowWindow(false);
	m_GameClientView.SetHaiDiCard(0);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetBankerUser(INVALID_CHAIR);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);

	//��������
	memset(m_byUserAction, 0 , sizeof(m_byUserAction));

	//�˿�����
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);

	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
		m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][0].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][1].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][2].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[i][3].SetCardData(NULL,0);
	}

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_cbHeapCardInfo[i][0]=0;
		m_cbHeapCardInfo[i][1]=HEAP_FULL_COUNT-(((i==m_wBankerUser)||((i+2)%4==m_wBankerUser))?28:26);
		m_GameClientView.m_HeapCard[SwitchViewChairID(i)].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],HEAP_FULL_COUNT);
	}

	//״̬����
	m_bHearStatus=false;

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
	m_wHeapHand=0;
	m_wHeapTail=0;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	//������Ϣ
	SendUserReady(NULL,0);

	m_GameClientView.UpdateGameView(NULL);
	return 0;
}

//���Ӷ��������
LRESULT CGameClientDlg::OnShowSZ(WPARAM wParam, LPARAM lParam)
{
	bool bPlayerMode=IsLookonMode();
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_ScoreView.ShowWindow(SW_HIDE);
	//�Թ۽���
	if (bPlayerMode)
	{
		m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_HIDE);
		m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_HIDE);
		m_ScoreView.ShowWindow(SW_HIDE);
		m_GameClientView.SetHaiDiCard(0);
		m_GameClientView.SetStatusFlag(false,false);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	}

	memset(m_byUserAction, 0 , sizeof(m_byUserAction));

	//�����˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_cbHeapCardInfo[i][0]=0;
		m_cbHeapCardInfo[i][1]=HEAP_FULL_COUNT-(((i==m_wBankerUser)||((i+2)%GAME_PLAYER==m_wBankerUser))?28:26);
	}

	//�ַ��˿�
	BYTE cbTakeCount=(MAX_COUNT-1)*GAME_PLAYER+1;  //�տ�ʼ����ȥ53��
	BYTE cbSiceFirst=HIBYTE(m_GameStartData.wSiceCount);
	BYTE cbSiceSecond=LOBYTE(m_GameStartData.wSiceCount);
	WORD wTakeChairID=(m_wBankerUser+7-cbSiceFirst)%GAME_PLAYER;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//������Ŀ
		BYTE cbValidCount=HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?(cbSiceSecond-1)*2:0);
		BYTE cbRemoveCount=__min(cbValidCount,cbTakeCount);

		//��ȡ�˿�
		cbTakeCount-=cbRemoveCount;
		m_cbHeapCardInfo[wTakeChairID][(i==0)?1:0]+=cbRemoveCount;

		//����ж�
		if (cbTakeCount==0)
		{
			m_wHeapHand=wTakeChairID;
			m_wHeapTail=(m_wBankerUser+7-cbSiceFirst)%GAME_PLAYER;
			break;
		}

		//�л�����
		wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
	}

	//�˿�����
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);

		//��Ͻ���
		m_GameClientView.m_WeaveCard[i][0].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][1].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][2].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][3].SetDisplayItem(true);
		m_GameClientView.m_HeapCard[wViewChairID].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],HEAP_FULL_COUNT);

		//�û��˿�
		if (wViewChairID!=2)
		{
			WORD wIndex=(wViewChairID>=3)?2:wViewChairID;
			m_GameClientView.m_UserCard[wIndex].SetCardData(MAX_COUNT-1,(i==m_wBankerUser));
		}
		else
		{
			BYTE cbBankerCard=(i==m_wBankerUser)?m_GameStartData.cbCardData[MAX_COUNT-1]:0;
			m_GameClientView.m_HandCardControl.SetCardData(m_GameStartData.cbCardData,MAX_COUNT-1,cbBankerCard);
		}

		//�Թ۽���
		if (bPlayerMode==false)
		{
			m_GameClientView.m_TableCard[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_DiscardCard[wViewChairID].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][0].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][1].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][2].SetCardData(NULL,0);
			m_GameClientView.m_WeaveCard[wViewChairID][3].SetCardData(NULL,0);
		}
	}

	//WORD wMeChairID=GetMeChairID();
	////��������
	//if ((bPlayerMode==true)&&(m_GameStartData.cbUserAction!=WIK_NULL))
	//{
	//	ShowOperateControl(m_GameStartData.cbUserAction,m_GameStartData.cbCardData[MAX_COUNT-1]);
	//	SetGameTimer(wMeChairID,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
	//}

	////������ʾ
	//if ((bPlayerMode==true)&&(m_wCurrentUser==wMeChairID/*!=INVALID_CHAIR*/))
	//{

	//	if (m_wCurrentUser==wMeChairID) 
	//	{
	//		m_GameClientView.SetStatusFlag(true,false);
	//		SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_HEAR_STATUS);
	//	}
	//}

	m_bIsMeCanOutCard = false;
	m_wCurrentUser = INVALID_CHAIR;
	m_byUserAction[GetMeChairID()] = WIK_NULL;
	WORD wMeChairID=GetMeChairID();
	if (!bPlayerMode && m_wBankerUser==wMeChairID)
	{
		m_wCurrentUser =  wMeChairID; 
		m_GameClientView.SetStatusFlag(true,false);
		if (m_GameStartData.cbUserAction!=WIK_NULL)
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

	//�����ж�
	//if ((m_bHearStatus==true)&&(m_GameClientView.m_HandCardControl.GetCurrentCard()!=(BYTE)wParam)) return 0;

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	BYTE cbOutCardData=(BYTE)wParam;
	m_GameLogic.RemoveCard(m_cbCardIndex,cbOutCardData);

	//�����˿�
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
	m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0);

	//���ý���
	KillGameTimer(IDI_OPERATE_CARD);
	KillGameTimer(IDI_OTHER_USEROUTCARD);

	m_GameClientView.UpdateGameView(NULL);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(2,cbOutCardData);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

	//��������
	memset(m_byUserAction, 0 , sizeof(m_byUserAction));

	//��������
	//PlayCardSound(m_wOutCardUser->cbGender, cbOutCardData);

	//��������
	CMD_C_OutCard OutCard;
	OutCard.cbCardData=cbOutCardData;
	SendData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));
	return 0;
}

//�˿˲���
LRESULT CGameClientDlg::OnCardOperate(WPARAM wParam, LPARAM lParam)
{
	//��������
	BYTE cbOperateCode=(BYTE)(wParam);
	BYTE cbOperateCard=(BYTE)(LOWORD(lParam));


	////״̬�ж�  ���� ȡ���᷵�� 0
	//if ((m_wCurrentUser==GetMeChairID())&&(cbOperateCode==WIK_NULL))
	//{
	//	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	//	return 0;
	//};


	//���������ж�Һ���
	//if (m_wCurrentUser!=GetMeChairID())
	//{
	//	return 0;
	//}

	//��������
	KillGameTimer(IDI_OPERATE_CARD);
	KillGameTimer(IDI_OTHER_USEROUTCARD);

	m_GameClientView.SetStatusFlag(false,true);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

	//��������
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=cbOperateCode;
	OperateCard.cbOperateCard=cbOperateCard;
	if (cbOperateCode==9 && 0!=cbOperateCard)
	{
		OperateCard.cbOperateCode=WIK_GANG;
		OperateCard.cbOperateCard=cbOperateCard;
	}
	else if (cbOperateCode==10 && 0!=cbOperateCard)
	{
		OperateCard.cbOperateCode=WIK_TING;
		OperateCard.cbOperateCard=cbOperateCard;
	}
	SendData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(CMD_C_OperateCard));

	memset(m_byUserAction, 0, sizeof(m_byUserAction));

	m_GameClientView.SetStatusFlag(false,false);
	if (0==cbOperateCode)
	{
		if (m_bIsMeCanOutCard)
		{//�Լ������Գ���.
			m_byUserAction[GetMeChairID()] = WIK_NULL;
			//����
			m_wCurrentUser = GetMeChairID();
			memset(m_byUserAction, 0, sizeof(m_byUserAction));
			SetGameTimer(GetMeChairID(),IDI_OTHER_USEROUTCARD,TIME_HEAR_STATUS-10);
			//������ʾ
			m_GameClientView.SetStatusFlag(true,false);
		}
	}
	m_GameClientView.UpdateGameView(NULL);
	return 0;
}

//���ײ���
LRESULT CGameClientDlg::OnOperateHaiDi(WPARAM wParam, LPARAM lParam)
{
	//��������
	KillGameTimer(TIME_OPERATE_HAI_DI);
	m_GameClientView.m_btHaiDi.ShowWindow(SW_HIDE);
	m_GameClientView.m_btNoHaiDi.ShowWindow(SW_HIDE);
	//m_GameClientView.m_btStart.ShowWindow(SW_SHOW);


	//������Ϣ
	CMD_C_OperateHaiDi OperateHaiDi;
	OperateHaiDi.cbTakeHaiDi=(BYTE)wParam;
	SendData(SUB_C_OPERATE_HAI_DI,&OperateHaiDi,sizeof(OperateHaiDi));

	return 0;
}

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
		TCHAR szBuffer[256];
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
void CGameClientDlg::PlayGameSoundLanguage(BYTE cbGender, CString strSound, bool bSoundType)
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

	if (bSoundType)
	{//��������
		TCHAR cBuffer[10]={""};
		int iRand=rand()%3+1;
		::_snprintf(cBuffer,sizeof(cBuffer),"%d",iRand);
		strGameSound=strGameSound+TEXT("_")+TEXT(cBuffer);
	}
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
		TCHAR szResourcePath[MAX_PATH];
		GetCurrentDirectory(MAX_PATH,szResourcePath);
		CString strPath(szResourcePath);
		strPath.Append("\\SparrowCS\\Sound\\");
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
		TCHAR szResourcePath[MAX_PATH];
		GetCurrentDirectory(MAX_PATH,szResourcePath);
		CString strPath(szResourcePath);
		strPath.Append("\\SparrowCS\\Sound\\");
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
			//InsertSystemString("CheckCanHuPai...");

			//������ʾ
			BYTE cbHuCardData[MAX_INDEX]={0};
			BYTE byTempCardIndex[MAX_INDEX]={0};
			::memcpy(byTempCardIndex, m_cbCardIndex, MAX_INDEX);
			m_GameLogic.RemoveCard(byTempCardIndex,byCardData);
			//m_cbTempKing[0]=0x01;
			//m_GameLogic.IsCanTingCard(byTempCardIndex,0,0,m_cbTempKing);
			m_GameLogic.IsCanTingCard(byTempCardIndex,m_WeaveItemArray[m_wCurrentUser],m_cbWeaveCount[m_wCurrentUser]);
			int iCardNums = m_GameLogic.GetHuPaiData(cbHuCardData,MAX_INDEX);

			if (iCardNums>0)
			{
				m_GameClientView.SetCanHuPaiData(cbHuCardData, iCardNums, iX);
				//char cbData[128]={0};
				//sprintf(cbData,"%x,%x,%x,%x,%x,%d",cbHuCardData[0],cbHuCardData[1],cbHuCardData[2],cbHuCardData[3],cbHuCardData[4],iCardNums);
				//InsertSystemString(cbData);
			}
			else
			{
				m_GameClientView.SetCanHuPaiData(NULL, 0, 0);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////

