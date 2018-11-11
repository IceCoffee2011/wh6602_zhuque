#ifndef GAME_FRAME_DLG_HEAD_FILE
#define GAME_FRAME_DLG_HEAD_FILE

#pragma once

#include "GameFrame.h"
#include "GameFrameView.h"
#include <vector>
#include <string>

//��˵��
class CGameFrameControl;
class CClientKernelSink;
class CSendMsgDlg;
class CScoreView;
class CGameTaskDialog;
class CShowTimeDJList;
class CGameClientDJInfo;
class CGameGiveDJInfo;
class CUserAction;
class CShowUserActionInfo;
class CUserInfoDlg;
class CGameAffiche;
class CShowDJInfo;
class CLanguageList;
class CTaskFinishFlash;
class CGameFrameBorder;
class CNewGiftFlashDlg;


using namespace std;


//////////////////////////////////////////////////////////////////////////

//��Ϸ������
class GAME_FRAME_CLASS CGameFrameDlg : public CDialog
{
	DECLARE_DYNAMIC(CGameFrameDlg)

	//��Ԫ����
	friend class CClientKernelSink;
	friend class CGameFrameControl;
	friend class CUserDaoJu;

	//����״̬
private:
	bool							m_bRectify;							//������־
	bool							m_bHideList;						//���ر�־
	CRect							m_rcNormalSize;						//����λ��

	//���渨��
private:
	bool							m_bReset;							//�Ƿ�����
	bool							m_bGameSceen;						//��Ϸ����
	bool							m_bInitDialog;						//��ʼ��־
	bool							m_bIsMouseDown;						//��갴��

	//���ñ���
private:
	bool							m_bAllowSound;						//��������

//	//��Ϸ���������
//private:
//	bool							m_bIsDisableGameMsg;				//�Ƿ񼤻���Ϸ����
//	CRect							m_rcGameMsgInfo;					//�����������
//	std::vector<string>				m_vecGameMsg;						//��Ϸ����
//	int								m_iCountVecIndex;					//��ǰ�������

	//�ؼ�����
protected:
	CSkinButton						m_btControl;						//���ư�ť
	CSkinSplitter					m_VorSplitter;						//��ֿؼ�
	CGameFrameView					* m_pGameFrameView;					//��ͼָ��
	CGameFrameControl				* m_pGameFrameControl;				//������ͼ
	CSendMsgDlg						* m_pSendMsgDlg;					//����������Ϣ
	CGameClientDJInfo				* m_pUserDJInfo;					//��ҵ����б�
	CUserInfoDlg					* m_pUserInfoDlg;					//�û���Ϣ��

	//����
protected:
	std::string						m_strLanguage;						//����

	//�ں˱���
private:
	CClientKernelSink				* m_pKernelSink;					//���ӽӿ�
	CUserFaceResHelper				m_UserFaceResHelper;				//ͷ�����
	CClientKernelHelper				m_ClientKernelHelper;				//��Ϸ�߼�
	CMessageProxyHelper				m_MessageProxyHelper;				//�������

	HANDLE							m_hEventGameLoadOver;				// �������Ϸ�������
	CString							m_strGameLoadOver;					// ��Ϸ�������

private:
	//�û�����
	CUserDaoJu						* m_pGameUserDaoJu;					//��Ϸ�û�����
	CScoreView						* m_pGameScoreView;					//������ͼָ��
	CGameTaskDialog					* m_pGameTaskDlg;					//��Ϸ����
	CShowTimeDJList					* m_pShowTimeDJList;				//��ʾʱ������б�
	CShowDJInfo						* m_pShowDJInfo;					//��ʾ������Ϣ
	CGameGiveDJInfo					* m_pGameGiveDJInfo;				//���͵�����Ϣ
	CUserAction						* m_pGameUserAction;				//��Ϸ�û�����
	CShowUserActionInfo				* m_pShowUserActionInfo;			//��ʾ�û�������Ϣ


private:
	CGameFrameBorder				* m_pGameFrameBorder;				//��Ϸ��ܱ߿�
	CGameAffiche					* m_pGameAffiche;					//ϲ��ϵͳ
	CLanguageList					* m_pLanguageList;					//�����б�
	CTaskFinishFlash				* m_pTaskFinishFlash;				//������ɶ���
	CNewGiftFlashDlg				* m_pNewGiftFlash;					//�����������	Author<cxf>; DateTime<2012-01-17>

	//��������
public:
	//���캯��
	CGameFrameDlg(CGameFrameView * pGameFrameView, CScoreView* pScoreView=NULL);
	//��������
	virtual ~CGameFrameDlg();

	//������ӿ�
public:
	//��ť��Ϣ
	virtual bool __cdecl OnSplitterButton(CSkinSplitter * pSkinSplitter, CSkinButton * pSplitterButton);
	//�������Ϣ
	virtual bool __cdecl OnSplitterEvent(CSkinSplitter * pSkinSplitter, UINT uSplitID, int nXPos, int nYPos);

	//����̳�
private:
	//��ʼ����
	virtual bool InitGameFrame()=NULL;
	//���ÿ��
	virtual void ResetGameFrame()=NULL;
	//��Ϸ����
	virtual void OnGameOptionSet()=NULL;
	//���������Ϸ
	virtual void OnEventContinueGame()=NULL;

	//ϵͳ�¼�
private:
	//ʱ����Ϣ
	virtual bool OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)=NULL;
	//�Թ�״̬
	virtual void OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)=NULL;
	//������Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)=NULL;
	//��Ϸ����
	virtual bool OnGameSceneMessage(BYTE cbGameStatus, bool bLookonOther, const void * pBuffer, WORD wDataSize)=NULL;

	//����̳�
private:
	//��������
	virtual void RectifyControl(int nWidth, int nHeight);
	//�����Ϣ
	virtual bool OnFrameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize);

	//�û��¼�
public:
	//�û�����
	virtual void __cdecl OnEventUserEnter(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
	//�û��뿪
	virtual void __cdecl OnEventUserLeave(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
	//�û�����
	virtual void __cdecl OnEventUserScore(tagUserData * pUserData, WORD wChairID, bool bLookonUser);
	//�û�״̬
	virtual void __cdecl OnEventUserStatus(tagUserData * pUserData, WORD wChairID, bool bLookonUser);

	//�����¼�
public:
	//��ʼ����
	virtual void __cdecl OnEventInitProperty(tagPropertyInfo *pPropertyInfo, int nInfoCount);
	//��ʼ�ʻ�
	virtual void __cdecl OnEventInitFlower(tagFlowerInfo *pFlowerInfo, int nInfoCount);
	//�ʻ���Ϣ
	virtual void __cdecl OnEventFlower(const tagUserData * pSendUserData, const tagUserData * pRecvUserData, UINT uFlowerID, UINT uFlowerEffectID);
	//���ʣ�����
	virtual void __cdecl OnEventUserHaveDJInfo(CMD_GF_ResidualProperty *pUserDJInfo);
	//���߲�����Ϣ
	virtual void __cdecl OnEventUserOperationDJInfo(BYTE byTag, void * pBuffer);
	//����������Ϣ
	virtual void __cdecl OnEventGameDJInfo(const char* pInfo);
	//�û���Ϣ
	virtual void __cdecl OnEventUserMD5PWMsg(char* chUserData);
	//������Ϣ
	virtual void __cdecl OnEventGameAfficheInfo(const char* pData, WORD wDataSize);
	//�û�����
	virtual void __cdecl OnEventUserActionInfo(BYTE btSender, BYTE btRecver, BYTE btTag);
	//�û�����
	virtual void __cdecl OnEventUserLanguageInfo(BYTE btSender, DWORD dwIndex);
	//�������	Author<cxf>; DateTime<2012-01-16>
	virtual void __cdecl OnEventNewGift(CMD_GF_NEW_GIFT * pNewGift);
	//��������-��ʱ
	virtual void __cdecl OnEventOnlineKeepTime(CMD_GF_ONLINE * pOnline);

//OSC update
	//������Ϣ
public:
	//����������
	virtual void __cdecl OnEventTaskAccept(const CMD_GF_TASK_tagTask* pTaskAccept);
	//�����������
	virtual void __cdecl OnEventTaskOnceFinish(const CMD_GF_TASK_tagTaskFinish* pTaskFinish);


	//���ڿ���
public:
	//�����б�
	bool ShowViewControl(bool bShowControl);
	//��ʾ������Ϣ
	void SetTaskWndShow(bool bIsShow=true);
	//��ʾʱ������б���
	void SetShowTimeDJListWnd(bool bShow=true);
	//��ʾ������Ϣ����
	void SetShowDJInfoWnd(bool bShow=true, int iIndex=0);
	//��ʾ�̳Ǵ���
	void SetShowBizCityWnd(bool bShow=true);
	void CloseBizCityWindow(int iID);
	//���µ�����Ϣ
	void UpdateDJInfo(int iDJID);
	//��ʾ�����
	void ShowScoreView(bool bIsView=true);

	//���ܺ���
public:
	//������Դ
	bool UpdateSkinResource();
	//���ͺ���
	bool SendData(WORD wSubCmdID);
	//���ͺ���
	bool SendData(WORD wSubCmdID, void * pData, WORD wDataSize);
	//����׼��
	bool SendUserReady(void * pBuffer, WORD wDataSize);
	//���ö�ʱ��
	bool SetGameTimer(WORD wChairID, UINT nTimerID, UINT nElapse);
	//ɾ����ʱ��
	bool KillGameTimer(UINT nTimerID);
	//������ʾ״̬
	void SetChatShowTag(bool bIsShow=true, bool bIsUp=true);
	//��Ӧ��Ϣ
	void SetChatMessageBeep();


public:
	//��������
	bool PlayGameSound(LPCTSTR pszSoundName);
	//��������
	bool PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName);
	// --�����������ȴ����أ�
	bool PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName, bool bWait);
	// --��������(DirectSound)
	bool PlayDirSound(LPCTSTR szRes, bool bFromFile = false, bool bLoop = false);
	// --���ű�������(cbType: 0=WAVE, 1=MID)
	bool PlayBGMusic(LPCTSTR szRes, BYTE cbType, bool bFromFile = false);
	bool StopBGMusic();
	//�Ƿ����ڲ���
	bool IsPlayingSound();
	//����״̬
	void SetSoundState(bool bIsEnable=true);
	
	//��������
public:
	//�Ƿ�����
	bool IsEnableSound();
	//��������
	void EnableSound(bool bEnable);
	bool IsEnableBGMusic() { return m_bEnableBGMusic; }
	// --���ñ�������
	void EnableBGMusic(bool bEnable, BYTE cbType);
	// --��������
	bool SetSoundVol(BYTE cbVol = 100);
	// --��ȡ����ֵ
	BYTE GetSoundVol() { return m_cbSoundVol; }
	// --���ñ�����������
	bool SetBGMusicVol(BYTE cbVol, BYTE cbType);
	// --��ȡ��������ֵ
	BYTE GetBGMusicVol() {return m_cbBGMusicVol; }




	//��Ϣ����
public:
	//����ͼƬ
	bool InsertImage(CBitmap * pBitmap);
	//��������
	bool InsertUserName(LPCTSTR pszUserName);
	//ϵͳ��Ϣ
	bool InsertSystemString(LPCTSTR pszString);
	//��ɫ��Ϣ
	bool InsertShtickString(LPCTSTR pszString, COLORREF crColor, bool bNewLine);
	//��ͨ��Ϣ
	bool InsertGeneralString(LPCTSTR pszString, COLORREF crColor, bool bNewLine);

	//��������
public:
	//�������
	void ActiveGameFrame();
	//�л�����
	WORD SwitchViewChairID(WORD wChairID);

	//�Թۿ���
public:
	//�����Թ�
	bool IsAllowUserLookon();
	//�Թۿ���
	bool AllowUserLookon(DWORD dwUserID, bool bAllowLookon);

	//��Ϣ����
public:
	//�Թ�ģʽ
	bool IsLookonMode();
	//�����Թ�
	bool IsAllowLookon();

	//��Ϣ����
public:
	//�Լ�λ��
	WORD GetMeChairID();
	//ʱ��λ��
	WORD GetTimeChairID();
	//������Ϸ
	VOID JoinInGameTable(WORD wChairID);
	//��ȡ�û�
	const tagUserData * GetUserData(WORD wChairID);
	//�ں˽ӿ�
	void * GetClientKernel(const IID & Guid, DWORD dwQueryVer);
	//�������������
	WORD GetGameDeskChairNums();
	//��ȡ��ϷID
	WORD GetGameKindID();
	//��������
	void ParseChar(const char* pData, WORD wDataSize);
	//�����û���������
	void SetShowUserActionWnd(bool bIsShow=false);
	//���Ͷ���
	void SendUserActionMsg(BYTE btRecvChairID=-1);
	//��ͼ�������
	void ViewHitOper();
	
public:
	//����������ɶ���
	void PlayTaskFinishFlash(bool bIsPlay=true, UINT uiNums=0);
	//���ŵȴ�����
	void PlayWaitFlash(bool bIsPlay=true);

	//��Ϸ״̬
public:
	//��Ϸ״̬
	BYTE GetGameStatus();
	//��Ϸ״̬
	void SetGameStatus(BYTE bGameStatus);

	//��Ϸ����
public:
	void GetGameName(CString& strName);

	//OSC UPDATE
public:
	//�˳���Ϸ����
	void SetGameExitOperation(BYTE bTag=2);

protected:
	//��������
	bool                                m_bEnableBGMusic;
	BYTE                                m_cbSoundVol;
	BYTE                                m_cbBGMusicVol;

private:
	CImage			m_FrameImageBuffer;					//����λͼ
	CPngImage		m_ImageTitleBack;					//����������
	CPngImage		m_ImageTitleText;					//�������ı�
	CPngImage		m_ImageTitleNum;					//����

	CSkinButton		m_btTitleMinCtBt;						//��С����ť
	CSkinButton		m_btTitleSoundBt;						//������ť
	CSkinButton		m_btTitleWndCloseBt;					//�رհ�ť
	CSkinButton		m_btTitlePhoto;							//��Ϸ��ͼ
	CSkinButton		m_btTitleHome;							//�ٷ���ҳ
	CSkinButton		m_btTitleShop;							//�����̳�
	CSkinButton		m_btTitleServer;						//�ͷ�����

	//��������Դ����
	void LoadTitleResource(void);

	//��ť�¼�
	void OnBnMinClickedEvent();
	void OnBnSoundClickedEvent();
	void OnBnCloseClieckedEvent();
	void OnBnGamePhotoClieckedEvent();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//ȷ����Ϣ
	virtual void OnOK();
	//ȡ����Ϣ
	virtual void OnCancel();

protected:
	//��Ϸ��ͼ
	void SaveScreenToBmp();
	void WriteBmpToFile(HBITMAP hBitmap,CString strFileName);

	//��Ϣӳ��
protected:
	//���ư�ť
	VOID OnBnClickedControl();
	//λ�øı�
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//λ����Ϣ
	afx_msg void OnGetMinMaxInfo(MINMAXINFO * lpMMI);
	//λ�øı�
	afx_msg void OnWindowPosChanging(WINDOWPOS * lpwndpos);
	//�ػ���Ϣ
	afx_msg void OnPaint();
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnNcHitTest(CPoint point);//by hxh
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	//����������Ϣ
	void SendLanguageListMsg(UINT uiIndex);

	// ��ȫ��ʾ�����ش���
	void SafeShowOrHideWnd(CWnd* pWnd, int nCmdShow);

	// ��ȫ�������Ҵ���
	void SafeEnableWnd(CWnd* pWnd, BOOL bEnable);
	/// ���ƿ��
	void DrawFrameView(CDC * pDC, int nWidth, int nHeight);
};

extern CGameFrameDlg * g_GameFrameDlg;
//////////////////////////////////////////////////////////////////////////

#endif