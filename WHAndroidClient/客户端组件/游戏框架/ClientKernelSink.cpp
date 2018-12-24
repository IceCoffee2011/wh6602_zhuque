#include "StdAfx.h"
#include "GameFrameDlg.h"
#include "GlobalOption.h"
#include "UserInfoDlg.h"
#include "GameFrameControl.h"
#include "ClientKernelSink.h"

//////////////////////////////////////////////////////////////////////////

//���캯��
CClientKernelSink::CClientKernelSink(CGameFrameDlg & GameFrameDlg) : m_GameFrameDlg(GameFrameDlg)
{
}

//��������
CClientKernelSink::~CClientKernelSink()
{
}

//�ӿڲ�ѯ
void * __cdecl CClientKernelSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IClientKernelSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IClientKernelSink,Guid,dwQueryVer);
	return NULL;
}

//��ȡ����
HWND __cdecl CClientKernelSink::GetFrameWnd()
{
	return m_GameFrameDlg.m_hWnd;
}

//����״̬
void __cdecl CClientKernelSink::ResetGameFrame()
{
	//���ñ���
	m_GameFrameDlg.m_bGameSceen=false;

	//���ý���
	m_GameFrameDlg.m_pGameFrameView->ResetData();
	m_GameFrameDlg.m_pGameFrameView->ResetGameView();
	m_GameFrameDlg.ResetGameFrame();
	m_GameFrameDlg.m_pGameFrameControl->ResetGameFrameControl();

#if REMOVE_VIDEO // add by HouGuoJiang 2011-11-25
	//������Ƶ
	CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();
	if (pVideoServiceManager!=NULL) pVideoServiceManager->OnEventGameReset();
#endif

	return;
}

//�رմ���
void __cdecl CClientKernelSink::CloseGameFrame(bool bForceClose)
{
	if (m_GameFrameDlg.m_hWnd!=NULL) m_GameFrameDlg.PostMessage(WM_CLOSE,0,0);
	return;
}

//ʱ����Ϣ
bool __cdecl CClientKernelSink::OnEventTimer(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	//��������
	WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(wChairID);
	m_GameFrameDlg.m_pGameFrameView->SetUserTimer(wViewChairID,nElapse);

	//�¼�֪ͨ
	return m_GameFrameDlg.OnTimerMessage(wChairID,nElapse,nTimerID);
}

//������Ϣ
bool __cdecl CClientKernelSink::OnEventSocket(const CMD_Command & Command, const void * pBuffer, WORD wDataSize)
{
	switch (Command.wMainCmdID)
	{
	case MDM_GF_FRAME:	//�����Ϣ
		{
			return m_GameFrameDlg.OnFrameMessage(Command.wSubCmdID,pBuffer,wDataSize);
		}
	case MDM_GF_GAME:	//��Ϸ��Ϣ
		{
			return m_GameFrameDlg.OnGameMessage(Command.wSubCmdID,pBuffer,wDataSize);
		}
	case MDM_GF_VIDEO:	//��Ƶ��Ϣ
		{
			switch (Command.wSubCmdID)
			{
			case SUB_GF_S_VIDEO_CONFIG:		//��Ƶ����
				{
					//Ч�����
					ASSERT(wDataSize==sizeof(CMD_S_VideoConfig));
					if (wDataSize!=sizeof(CMD_S_VideoConfig)) return false;

					//��Ϣ����
					CMD_S_VideoConfig * pVideoConfig=(CMD_S_VideoConfig *)pBuffer;

					//�����û�
					tagUserData * pRomterUserData=m_GameFrameDlg.m_ClientKernelHelper->SearchUserItem(pVideoConfig->dwLocalUserID);
					if (pRomterUserData==NULL) return true;

					//�û�Ч��
					ASSERT((m_GameFrameDlg.IsLookonMode()==false)&&(pRomterUserData->cbUserStatus!=US_LOOKON));
					if ((m_GameFrameDlg.IsLookonMode()==true)||(pRomterUserData->cbUserStatus==US_LOOKON)) return true;

				#if REMOVE_VIDEO // add by HouGuoJiang 2011-11-25
					//��ȡ��Ƶ
					WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(pRomterUserData->wChairID);
					CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();

					//��Ƶ����
					if (pVideoServiceManager!=NULL)
					{
						//��������
						tagVideoNetwordInfo VideoNetwordInfo;
						ZeroMemory(&VideoNetwordInfo,sizeof(VideoNetwordInfo));

						//���ñ���
						VideoNetwordInfo.wNatPort=pVideoConfig->wNatPort;
						VideoNetwordInfo.wLocalPort=pVideoConfig->wLocalPort;
						VideoNetwordInfo.dwNatAddr=pVideoConfig->dwNatAddr;
						VideoNetwordInfo.dwLocalAddr=pVideoConfig->dwLocalAddr;

						//��Ƶ����
						pVideoServiceManager->OnEventUserConfig(wViewChairID,pVideoConfig->dwLocalUserID,&VideoNetwordInfo);
					}
				#endif
					return true;
				}
			case SUB_GF_S_VIDEO_CLOSE:		//�ر���Ƶ
				{
					//Ч�����
					ASSERT(wDataSize==sizeof(CMD_S_VideoClose));
					if (wDataSize!=sizeof(CMD_S_VideoClose)) return false;

					//��Ϣ����
					CMD_S_VideoClose * pVideoClose=(CMD_S_VideoClose *)pBuffer;

					//�����û�
					tagUserData const * pRomterUserData=m_GameFrameDlg.GetUserData(pVideoClose->wCloseUser);
					if (pRomterUserData==NULL) return true;

					//�û�Ч��
					ASSERT((m_GameFrameDlg.IsLookonMode()==false)&&(pRomterUserData->cbUserStatus!=US_LOOKON));
					if ((m_GameFrameDlg.IsLookonMode()==true)||(pRomterUserData->cbUserStatus==US_LOOKON)) return true;
				#if REMOVE_VIDEO // add by HouGuoJiang 2011-11-25
					//��ȡ��Ƶ
					WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(pRomterUserData->wChairID);
					CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();

					//��Ƶ����
					if (pVideoServiceManager!=NULL)
					{
						pVideoServiceManager->OnEventCloseVideo( wViewChairID );
					}
				#endif
					return true;
				}
			case SUB_GF_S_VIDEO_OPEN:		//����Ƶ
				{
					//Ч�����
					ASSERT(wDataSize==sizeof(CMD_S_VideoOpen));
					if (wDataSize!=sizeof(CMD_S_VideoOpen)) return false;

					//��Ϣ����
					CMD_S_VideoOpen * pVideoOpen=(CMD_S_VideoOpen *)pBuffer;

					//�����û�
					tagUserData const * pRomterUserData=m_GameFrameDlg.GetUserData(pVideoOpen->wOpenUser);
					if (pRomterUserData==NULL) return true;

					//�û�Ч��
					ASSERT((m_GameFrameDlg.IsLookonMode()==false)&&(pRomterUserData->cbUserStatus!=US_LOOKON));
					if ((m_GameFrameDlg.IsLookonMode()==true)||(pRomterUserData->cbUserStatus==US_LOOKON)) return true;

				#if 0 // add by HouGuoJiang 2011-11-25
					//��ȡ��Ƶ
					WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(pRomterUserData->wChairID);
					CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();

					//��Ƶ����
					if (pVideoServiceManager!=NULL)
					{
						pVideoServiceManager->OnEventOpenVideo( wViewChairID );
					}
				#endif

					return true;
				}
			}
		}
	}

	return false;
}

//������Ϣ
bool __cdecl CClientKernelSink::OnEventProcess(const IPC_Head & Head, const void * pBuffer, WORD wDataSize, HWND hWndSend)
{
	return false;
}

//��Ϸ״̬
bool __cdecl CClientKernelSink::OnEventGameScene(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize)
{
	//���ñ���
	m_GameFrameDlg.m_bGameSceen=true;

	return m_GameFrameDlg.OnGameSceneMessage(cbGameStation,bLookonOther,pBuffer,wDataSize);
}

//������Ϣ
bool __cdecl CClientKernelSink::OnUserChatMessage(tagUserData * pSendUserData, tagUserData * pRecvUserData, LPCTSTR pszChatMessage, COLORREF crColor)
{
	CString strChatMessage = "";
	strChatMessage.Append(pszChatMessage);
	//���ù���
	g_GlobalOption.m_FilterString.SetFilterString(strChatMessage);
	if (strChatMessage.IsEmpty())
	{
		return true;
	}
	
	//��ʾ��Ϣ
	if (pRecvUserData==NULL) 
	{
		const tagUserData * pMeInfo = m_GameFrameDlg.m_ClientKernelHelper->GetMeUserInfo();
		if (NULL != pMeInfo)
		{
			if (pMeInfo->dwUserID==pSendUserData->dwUserID)
			{
				m_GameFrameDlg.m_MessageProxyHelper->InsertUserChat(pSendUserData->szName,strChatMessage.GetBuffer(),crColor,MS_NORMAL);
			}
			else
			{
				m_GameFrameDlg.m_MessageProxyHelper->InsertUserChat("���",strChatMessage.GetBuffer(),crColor,MS_NORMAL);
			}
		}
	}
	else 
	{//������Ϸ�в�����Ŀ��
		m_GameFrameDlg.m_MessageProxyHelper->InsertUserChat(pSendUserData->szName,pRecvUserData->szName,strChatMessage.GetBuffer(),crColor,MS_NORMAL);
	}
	
	//��Ӧ��Ϣ
	//OnEventHaveChatMessage();

	return true;
}

//����Ϣ
void __cdecl CClientKernelSink::OnEventHaveChatMessage()
{
	m_GameFrameDlg.SetChatMessageBeep();
}

//������Ϣ
void __cdecl CClientKernelSink::OnEventGameAfficheInfo(const char* pDataInfo, WORD wDataSize)
{
	m_GameFrameDlg.OnEventGameAfficheInfo(pDataInfo, wDataSize);
}

//�û�md5����
void __cdecl CClientKernelSink::OnEventUserMD5PWMessage(void *pBuffer)
{
	if (NULL != pBuffer)
	{
		IPC_UserPassWord * pUserMD5PW=(IPC_UserPassWord *)pBuffer;
		if (NULL != pUserMD5PW)
		{
			m_GameFrameDlg.OnEventUserMD5PWMsg(pUserMD5PW->chUserPassWord);
		}
	}
}

//�û�������Ϣ
void __cdecl CClientKernelSink::OnEventUserActionInfo(BYTE btSender, BYTE btRecver, BYTE btTag)
{
	m_GameFrameDlg.OnEventUserActionInfo(btSender, btRecver, btTag);
}

//�û�������Ϣ
void __cdecl CClientKernelSink::OnEventUserLanguageInfo(BYTE bySender, DWORD dwIndex)
{
	m_GameFrameDlg.OnEventUserLanguageInfo(bySender, dwIndex);
}

//ʱ�����
void __cdecl CClientKernelSink::OnEventTimerKilled(WORD wChairID, UINT nTimerID)
{
	//���ý���
	WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(wChairID);
	m_GameFrameDlg.m_pGameFrameView->SetUserTimer(wViewChairID,0);

	return;
}

//�Թ�״̬
void __cdecl CClientKernelSink::OnEventLookonChanged(bool bLookonUser, void * pBuffer, WORD wDataSize)
{
	//���ý���
	m_GameFrameDlg.OnLookonChanged(bLookonUser,pBuffer,wDataSize);

	return;
}

//�û�����
void __cdecl CClientKernelSink::OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	//�����б�
	//m_GameFrameDlg.m_pGameFrameControl->m_UserListView.m_wKindID=m_GameFrameDlg.m_ClientKernelHelper->GetServerAttribute()->wKindID;
	//m_GameFrameDlg.m_pGameFrameControl->m_UserListView.m_wGameGenre=m_GameFrameDlg.m_ClientKernelHelper->GetServerAttribute()->wGameGenre;

//#ifdef _DEBUG
//		TCHAR szUserCount[64];
//		_sntprintf(szUserCount,CountArray(szUserCount),TEXT("[ %s ]����"),pUserData->szName);
//		m_GameFrameDlg.InsertSystemString(szUserCount);	
//#endif

	//�����б�
	m_GameFrameDlg.m_pGameFrameControl->InsertUserItem(pUserData);

	//���ý���
	if (bLookonUser==false)
	{
		WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(wChairID);
		m_GameFrameDlg.m_pGameFrameView->SetUserInfo(wViewChairID,pUserData);
	}

	//�����û�
	if (pUserData!=m_GameFrameDlg.m_ClientKernelHelper->GetMeUserInfo())
	{
		//�����û�
		//int nItem=m_GameFrameDlg.m_pUserInfoDlg->m_pGameFrameControl->m_ChatObject.AddString(pUserData->szName);
		//m_GameFrameDlg.m_pUserInfoDlg->m_pGameFrameControl->m_ChatObject.SetItemData(nItem,pUserData->dwUserID);

		//��ʾ��Ϣ
		if (m_GameFrameDlg.m_bGameSceen==true)
		{
			if (US_LOOKON!=pUserData->cbUserStatus)
			{
				m_GameFrameDlg.m_MessageProxyHelper->InsertUserName("���"/*pUserData->szName*/);
				m_GameFrameDlg.m_MessageProxyHelper->InsertGeneralString(TEXT("������"),g_GlobalOption.m_crMsgNormal,0,true);
			}

			//��Ӧ������Ϣ
			OnEventHaveChatMessage();
		}
	}

	//�¼�֪ͨ
	m_GameFrameDlg.OnEventUserEnter(pUserData,wChairID,bLookonUser);

	//��Ƶ֪ͨ
	if (bLookonUser==false)
	{
	#if REMOVE_VIDEO // add by HouGuoJiang 2011-11-25
		//��ȡ��Ƶ
		CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();

		//��Ƶ֪ͨ
		if (pVideoServiceManager!=NULL)
		{
			WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(wChairID);
			pVideoServiceManager->OnEventUserEnter(wViewChairID,pUserData->dwUserID);
		}
	#endif
	}



	//��Ϣ��ӡ
	//char chData[255]={0};
	//sprintf(chData, "%d��ҽ����ı�ʯ:%d,�ȼ�:%d", wChairID, pUserData->lGems, pUserData->dwClass);
	//m_GameFrameDlg.m_MessageProxyHelper->InsertGeneralString(TEXT(chData),g_GlobalOption.m_crMsgNormal,0,true);

	return;
}

//�û��뿪
void __cdecl CClientKernelSink::OnEventUserLeave(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	//�¼�֪ͨ
	m_GameFrameDlg.OnEventUserLeave(pUserData,wChairID,bLookonUser);

	//��Ƶ֪ͨ
	if ( ! bLookonUser )
	{
	#if REMOVE_VIDEO // add by HouGuoJiang 2011-11-25
		WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(wChairID);
		CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();
		if (pVideoServiceManager!=NULL) pVideoServiceManager->OnEventUserLeave(wViewChairID,pUserData->dwUserID);
	#endif
	}

	//���ý���
	if (bLookonUser==false)
	{
		WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(wChairID);
		m_GameFrameDlg.m_pGameFrameView->SetUserInfo(wViewChairID,NULL);
	}

	//ɾ���б�
	m_GameFrameDlg.m_pGameFrameControl->DeleteUserItem(pUserData);

	//��ʾ��Ϣ
	if (m_GameFrameDlg.m_bGameSceen==true)
	{
		if(US_LOOKON!=pUserData->cbUserStatus)
		{
			m_GameFrameDlg.m_MessageProxyHelper->InsertUserName("���"/*pUserData->szName*/);
			m_GameFrameDlg.m_MessageProxyHelper->InsertGeneralString(TEXT("�뿪��"),g_GlobalOption.m_crMsgNormal,0,true);
		}
		
		//��Ӧ������Ϣ
		OnEventHaveChatMessage();
	}

	//ɾ���û�
	//CComboBox * pChatObject=&m_GameFrameDlg.m_pUserInfoDlg->m_pGameFrameControl->m_ChatObject;
	//int nCount=pChatObject->GetCount();
	//for (int i=1;i<nCount;i++)
	//{
	//	if (pChatObject->GetItemData(i)==pUserData->dwUserID)
	//	{
	//		if (pChatObject->GetCurSel()==i) pChatObject->SetCurSel(0);
	//		pChatObject->DeleteString(i);
	//		break;
	//	}
	//}

	return;
}

//�û�����
void __cdecl CClientKernelSink::OnEventUserScore(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	//���ý���
	m_GameFrameDlg.m_pGameFrameControl->UpdateUserItem(pUserData);
	if (bLookonUser==false)
	{
		WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(wChairID);
		m_GameFrameDlg.m_pGameFrameView->SetUserInfo(wViewChairID, pUserData);
		m_GameFrameDlg.m_pGameFrameView->UpdateGameView(NULL);
	}

	//�¼�֪ͨ
	m_GameFrameDlg.OnEventUserScore(pUserData,wChairID,bLookonUser);


	//��Ϣ��ӡ
	//char chData[255]={0};
	//sprintf(chData, "%d����û���Ϣ���±�ʯ:%d,�ȼ�:%d", wChairID, pUserData->lGems, pUserData->dwClass);
	//m_GameFrameDlg.m_MessageProxyHelper->InsertGeneralString(TEXT(chData),g_GlobalOption.m_crMsgNormal,0,true);

	return;
}

//�û�״̬
void __cdecl CClientKernelSink::OnEventUserStatus(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	//���ý���
	m_GameFrameDlg.m_pGameFrameControl->UpdateUserItem(pUserData);
	if (bLookonUser==false) m_GameFrameDlg.m_pGameFrameView->UpdateGameView(NULL);

	//�¼�֪ͨ
	m_GameFrameDlg.OnEventUserStatus(pUserData,wChairID,bLookonUser);

	//��Ƶ֪ͨ
	tagUserData const * pMeUserData=m_GameFrameDlg.GetUserData(m_GameFrameDlg.GetMeChairID());
	if ((pMeUserData!=pUserData)&&((pUserData->cbUserStatus==US_OFFLINE)||(pUserData->cbUserStatus==US_PLAY)))
	{
	#if REMOVE_VIDEO // add by HouGuoJiang 2011-11-25
		//����ת��
		WORD wViewChairID=m_GameFrameDlg.SwitchViewChairID(pUserData->wChairID);

		//�¼�֪ͨ
		CVideoServiceManager * pVideoServiceManager=CVideoServiceManager::GetInstance();
		if (pVideoServiceManager!=NULL) pVideoServiceManager->OnUpdateUserItem(wViewChairID,pUserData->cbUserStatus,pUserData->dwUserID);
	#endif
	}

	return;
}

//��Ա�ȼ�
void __cdecl CClientKernelSink::OnEventUserMemberOrder(tagUserData * pUserData, WORD wChairID, bool bLookonUser)
{
	//���ý���
	m_GameFrameDlg.m_pGameFrameControl->UpdateUserItem(pUserData);
	if (bLookonUser==false) m_GameFrameDlg.m_pGameFrameView->UpdateGameView(NULL);

	return;
}

//��ʼ����
void __cdecl CClientKernelSink::OnEventInitProperty(tagPropertyInfo *pPropertyInfo, int nInfoCount)
{
	m_GameFrameDlg.OnEventInitProperty(pPropertyInfo, nInfoCount);
	return;
}

//��ʼ�ʻ�
void __cdecl CClientKernelSink::OnEventInitFlower(tagFlowerInfo *pFlowerInfo, int nInfoCount)
{
	m_GameFrameDlg.OnEventInitFlower(pFlowerInfo, nInfoCount);

	return;
}

//�ʻ���Ϣ
void __cdecl CClientKernelSink::OnEventFlower(const tagUserData * pSendUserData, const tagUserData * pRecvUserData, UINT uFlowerID, UINT uFlowerEffectID)
{
	//��������
	m_GameFrameDlg.PlayGameSound(GetModuleHandle(GAME_FRAME_DLL_NAME), TEXT("FLOWER"));

	//�Է�����
	m_GameFrameDlg.OnEventFlower(pSendUserData, pRecvUserData, uFlowerID, uFlowerEffectID+1);	

	//������
	if(pSendUserData->dwUserID!=pRecvUserData->dwUserID)
		m_GameFrameDlg.OnEventFlower(pSendUserData, pSendUserData, uFlowerID, uFlowerEffectID);	

	return;
}


//���ʣ�����
void __cdecl CClientKernelSink::OnEventUserHaveDJInfo(CMD_GF_ResidualProperty *pUserDJInfo)
{
	if (NULL == pUserDJInfo)
	{
		return;
	}
	m_GameFrameDlg.OnEventUserHaveDJInfo(pUserDJInfo);
}

//���߲�����Ϣ
void __cdecl CClientKernelSink::OnEventUserOperationDJInfo(BYTE byTag, void * pBuffer)
{
	if (0==byTag || NULL==pBuffer)
	{
		return;
	}
	m_GameFrameDlg.OnEventUserOperationDJInfo(byTag, pBuffer);
}

//����������Ϣ
void __cdecl CClientKernelSink::OnEventGameDJInfo(const char* pInfo)
{
	if (NULL == pInfo)
	{
		return;
	}
	m_GameFrameDlg.OnEventGameDJInfo(pInfo);
}



//OSC update
//����������
void __cdecl CClientKernelSink::OnEventTaskAccept(const CMD_GF_TASK_tagTask* pTaskAccept)
{
	if (NULL != pTaskAccept)
	{
		m_GameFrameDlg.OnEventTaskAccept(pTaskAccept);
	}
}
//�����������
void __cdecl CClientKernelSink::OnEventTaskOnceFinish(const CMD_GF_TASK_tagTaskFinish* pTaskFinish)
{
	if (NULL != pTaskFinish)
	{
		m_GameFrameDlg.OnEventTaskOnceFinish(pTaskFinish);
	}
}
//�������	Author<cxf>; DateTime<2012-01-16>
void __cdecl CClientKernelSink::OnEventNewGift(CMD_GF_NEW_GIFT* pNewGift)
{
	if (NULL != pNewGift)
	{
		m_GameFrameDlg.OnEventNewGift(pNewGift);
	}
}

void __cdecl CClientKernelSink::OnEventOnlineKeepTime(CMD_GF_ONLINE * pOnline)
{
	if (NULL != pOnline)
	{
		m_GameFrameDlg.OnEventOnlineKeepTime(pOnline);
	}
}
//////////////////////////////////////////////////////////////////////////
