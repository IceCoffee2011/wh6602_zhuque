#pragma once
#include "resource.h"
#include "afxcmn.h"
#include "DlgLogon.h"

#include "UserInfoView.h"
#include "ServerItemView.h"

#include "GameListManager.h"
#include "IGamePlaza.h"

#include "ImageNums.h"

#include "GameListHot.h"
#include "PlazaGameWeb.h"
#include "afxwin.h"
#include "GameListCenterDlg.h"
#include "RoomListCenterDlg.h"
#include "PlazaCloseConfirm.h"
#include "ConfirmGold.h"
#include "ConfirmGrade.h"
#include "SkinControls.h"

#include "NewHandTask.h"
#include "IpFinder.h"

// CPlazaDlg �Ի���

using namespace std;

//����ָ��
typedef BOOL (WINAPI *ShowLayer)(HWND,COLORREF,BYTE,DWORD); 

//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define		WM_DOWN_LOAD_FACE		WM_USER+100				//������Ϣ
#define		MAX_SERVER				3	
#define		IDI_HOT_KEY_BOSS		0x0100					//�ϰ��ȼ�
#define		WAIT_END_EVENT			1						//�ȴ������¼�

class CActive;

//�Զ���ͷ��
struct tagCustomFace
{
	DWORD						dwUserID;				//��� ID
	BYTE						*pFaceData;				//ͷ������
	DWORD						dwFaceSize;				//ͷ���С
	DWORD						dwCurrentSize;			//��ǰ��С

	//���캯��
	tagCustomFace():dwUserID(0),pFaceData(NULL),dwFaceSize(0),dwCurrentSize(0){}

	//����ڴ�
	void Clear(void)
	{
		delete[]pFaceData;
		pFaceData=NULL;
		dwUserID=0;
		dwFaceSize=0;
		dwCurrentSize=0;
	}

	//��������
	~tagCustomFace()
	{
		Clear();
	}

};


//ͼƬ��ť
struct PicButton 
{
	CPngImage	BTImage;
	CRect		rcRect;
	BYTE		btStatus;	//0:Ĭ������״̬; 1:�������ȥ״̬; 2:��갴��״̬; 3:������ʾ״̬.

	PicButton()
	{
		btStatus = 0;
	}
};


//������Ϣ
struct tagDownloadInfo
{
	DWORD						dwUserID;				//��� ID
	BYTE						bFaceVer;				//ͷ��汾
	BYTE						bReference;				//���ü���

	//��ʼ��
	tagDownloadInfo():dwUserID(0),bFaceVer(0),bReference(1){}
};

//���Ͷ���
typedef CArrayTemplate<tagDownloadInfo>			CDownloadInfoArrary;
//////////////////////////////////////////////////////////////////////////

class CPlazaDlg : public CDialog, public IPlazaViewItem,public ITCPSocketSink
{
	DECLARE_DYNAMIC(CPlazaDlg)

public:
	CPlazaDlg(CWnd* pParent =NULL);   // ��׼���캯��
	virtual ~CPlazaDlg();


// �Ի�������
	enum { IDD = IDD_PLAZADLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);

	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()

	//����
protected:
	//���ſؼ�
	void ResizeFrame();	 
	//��������
	bool CreateTableList(CListServer * pListServer);

protected:
	//���ӷ�����
	afx_msg void OnCommandConnect();
	//ȡ������
	afx_msg void OnCommandCancelConnect();
	 
protected:
	//����RoomList
	void OnCallRoomList();
	//����GameList
	void OnCallGameList();
	//
	void OnCallTableList();
	//�����û�����
	void OnCannUpdateUserImage();
	//ֱ�ӽ��뷿��
	void OnFlashEnterRoom();

	// ������ʾ��
	LRESULT OnCallShowCustomer(WPARAM wParam, LPARAM lParam);

	//Inno ��װ�ű���������Ϣ �رմ�����Ϣ
    LRESULT OnCallInnoIssMsgClose(WPARAM wParam, LPARAM lParam);

	LRESULT OnCallCheckConnect(WPARAM wParam, LPARAM lParam);

public:
	//Inno ��װ�ű���������Ϣ ѡ����Ϸ
	LRESULT OnCallInnoIssMsgSelectGame(WPARAM wParam, LPARAM lParam);

public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);
	//��ͼ�ӿ�
public:
	//��ȡ���
	virtual HWND __cdecl GetWindowHandle() { return GetSafeHwnd(); }
	//�����¼�
public:
	//�����¼�
	virtual bool __cdecl OnEventTCPSocketLink(WORD wSocketID, INT nErrorCode);
	//�ر��¼�
	virtual bool __cdecl OnEventTCPSocketShut(WORD wSocketID, BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool __cdecl OnEventTCPSocketRead(WORD wSocketID, CMD_Command Command, VOID * pData, WORD wDataSize);
	//ͷ�����
protected:
	bool						m_bStartDownloadFace			;	//���ر�ʶ
	bool						m_bDownloadConnect;					//���ر�ʶ
	tagCustomFace				m_CustomFace;						//�Զ���ͷ��
	CDownloadInfoArrary			m_DownloadInfoArrary			;	//���ض���
	int							m_curTab;						;	//��ǰҳ�� 
public:
	bool						m_bLogonPlaza;						//��¼��־
	bool						m_bInitDialog;						//��ʼ��־
public://����
	//cxf CDlgLogon					m_dlgLogon;						//��½����(����ע�����)
	CDlgLogon					* m_pDlgLogon;						//��½����(����ע�����)
	CActive							* m_pActivity;							//�ר��
	
	//
	//cxf CGameListDlg				m_gameListDlg;						//��Ϸ�б����(������桢��Ϸ�б�)
	//cxf1022 CRoomListDlg				m_roomListDlg;						//�����б����(�������б�)
	CRoomViewItem				* m_pRoomViewItemCreate;			//��������(����������������ͼ�������б�)
	//CFriendListDlg				m_friendlistDlg;					//�����б�
	//cxf 1022 CTaskDialog					m_TaskDlg;							//����
	//cxf 1022 CShopDialog					m_shopDlg;							//�̳�
	//cxf 1022 CSetDialog					m_setdlg;							//����
	//cxf 1022 CDoteyBoxDialog				m_doteyBoxDlg;						//�ر���
	//cxf 1022 CShowGameMsg				m_ShowGameMsg;						//��ʾ��Ϸ��Ϣ
	CDC							m_memdc;

	CRgnButton					m_btnReturn;						//����     IDC_BT_RETURN
	CRgnButton					m_btnSound;							//����	   IDC_BT_SET
	CRgnButton					m_btnClose;							//�ر�     IDC_BT_CLOSE
	CRgnButton					m_btnMinsize;						//��С��   IDCMINSIZE
	CRgnButton					m_btnReLogon;						//���µ�¼ IDC_BT_RE_LOGON
	CToolTipCtrl				m_ToolTipCtrl;						//��ʾ�ؼ�
	//cxf
	CGameListHot				m_GameListHot;						//������Ϸ
	CPlazaGameWeb				m_PlazaGameWeb;						//������Ϸ��ҳ
	CGameListCenterDlg			m_GameListCenterDlg;				//��Ϸ�б�
	CRoomListCenterDlg			m_RoomListCenterDlg;				//�����б�
	CPlazaCloseConfirm			m_PlazaCloseConfirm;				//�˳�ȷ�Ͽ�
	CConfirmGold				m_ConfirmGold;						//��Ҳ����
	CConfirmGrade				m_ConfirmGrade;						//�ȼ�����
	
	CIpFinder					m_IpFinder;							//IP�����

public:
	CMD_GP_UserGradeInfo *		m_pGradeInfo;						//�ȼ���Ϣ
	int							m_iGradeInfoDiv;					//�ȼ��ṹ��С

	WORD						m_wTitlePos;						//����λ��

	DWORD						m_dwLastConnectTime;				///< �ϴ����ӷ���ʱ��
	DWORD						m_dwConnectIntervalTime;			///< ���Ӽ��ʱ��

	//cxf
private:
	bool						m_bIsGetGlod;						//��ȡ���
	bool						m_bIsUpdateUserInfo;				//�����û���Ϣ
	bool						m_bCheckLine;						///< �����·
	bool						m_bDeleteMachineCode;				///< ɾ����½������������
	CRgnButton					m_btnVisitWeb;						//�ٷ���վ
	CRgnButton					m_btnAccountsManage;				//�ʺŹ���
	CRgnButton					m_btnGameHelp;						//��Ϸ����
	CRgnButton					m_btnQuestionFeedBack;				//���ⷴ��
	CRgnButton					m_btnContactServise;				//���߿ͷ�
	CRgnButton					m_btnGameList;						//��Ϸ�б�
	CRgnButton					m_btnTounament;						//����ר��
	CRgnButton					m_btnAccountsRecharge;				//�ʺų�ֵ
	CRgnButton					m_btnPrizeExcharge;					//��Ʒ�һ�
	CRgnButton					m_btnPropsShop;						//�����̳�
	CRgnButton					m_btnNoviceTask;					//��������
	CRgnButton					m_btnUpdateUserInfo;				//�����û���Ϣ
	CRgnButton					m_btnBackpack;						//����
	CRgnButton					m_btnUserInfo;						//�û���Ϣ
	CRgnButton					m_btnBank;							//����
	CRgnButton					m_btnEmailMessage;					//�ʼ�
	CRgnButton					m_btnRollback;						// ����
	CRgnButton					m_btnNextPage;						// ��һҳ
	CRgnButton					m_btnNewComerGuide;					//����ָ��

	CWebBrowser					m_webPlazaAdvertisement;			//�������
	CWebBrowser					m_webPlazaNotice;					//��������
	CWebBrowser					m_webPlazaMessage;					//������Ϣ
	CWebBrowser					m_webPlazaTitle;					//����ͷ�����

	CGameFlash					m_GameFlash;						//��������
	
private:
	CString						m_strCurrentPathPlaza;				//������Դ·��
	CPngImage					m_ImageTitle;						//��Ϸ�б��������
	CRect						m_rcTitleArea;						//��������λ��

	CRect						m_rcNewComerGuideArea;				//����ָ������
	CPngImage					m_ImageNewComerGuide;				//����ָ��ͼƬ
	bool						m_bNewComerGuide;					//�Ƿ񶯻�����ָ��
	WORD						m_wNewComerGuideActionNumber;		//����֡

	//���Ʊ���������
private:
	CPngImage		m_ImageBack;				//����
	CPngImage		m_UserImage;				//�û�����
	//cxf CPngImage		m_UserInfo;					//�û���Ϣ����
	CPngImage		m_Exp1Image;				//������
	CPngImage		m_Exp2Image;				//������2
	CPngImage		m_UserXJImage;				//����Ǽ�
	CPngImage		m_XJInfoBackImage;			//�Ǽ�����
	//cxf CPngImage		m_PngHat;					//ñ��
	CImageNums		m_ImageNums;				//����
	CRect			m_UserXJRect;				//�Ǽ�����
	CRect			m_UserXJInfoRect;			//�Ǽ���Ϣ��ʾ����
	bool			m_bIsShowXJInfo;			//�Ƿ���ʾ�Ǽ���Ϣ
	bool			m_bIsGamePlazyStatus;		//�Ƿ�����Ϸ����״̬
	bool			m_bIsSound;					//�Ƿ񲥷ű�������

	CImageNums		m_ImageNumsTel;				//�绰����
	//������Ϣ����
	/*cxf DWORD			m_dwGameMsgIndex;			//��Ϣ���
	CPngImage		m_GameMsgImage;				//ͼƬ
	bool			m_bIsFlashMsg;				//�Ƿ�������Ϣ��ʾ
	CRect			m_rcGameMsgArea;			//��Ϸ��Ϣ����
	BYTE			m_byFlashMsgStatus;			//״̬. 0:����״̬; 1:����ȥ״̬
	*/
	//��ʼ��ͼƬ��Ϣ
	void InitDlgBK();
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//�����Ǽ�
	void CountXJClass(DWORD dwWinNums, DWORD dwTotalNums, WORD& wClass);
	//�����Ǽ�
	void DrawXJInfo(CDC* pDC);
	
public:
	CTCPSocketHelper			m_ClientSocket;						//��������

	//��Ϸ�б����
	CGameListManager			m_gameListManager;					//��Ϸ�б����
	CServerItemView				m_ServerItemView;					//��������ͼ 
	
public:
	//��ʾ��Ϣ
	int ShowMessageBox(LPCTSTR pszMessage, UINT nType);
	//��������
	void UpdateUserImage();
	//�����û���ʯ
	void UpdateUserGold(DWORD dwGold);
	//�����û����
	void UpdateUserMoney(DWORD dwMoney);

	//��Ϣ����
protected:
	//��¼��Ϣ
	bool OnSocketMainLogon(CMD_Command Command, void * pBuffer, WORD wDataSize);
	//�б���Ϣ
	bool OnSocketMainServerList(CMD_Command Command, void * pBuffer, WORD wDataSize);
	//ϵͳ��Ϣ
	bool OnSocketMainSystem(CMD_Command Command, void * pBuffer, WORD wDataSize);
	//�û���Ϣ
	bool OnSocketMainUser(CMD_Command Command, void * pBuffer, WORD wDataSize);
public:
	//��������
	bool ConnectServer();
	//��¼�㳡
	afx_msg void OnCommandLogon();
	//�㳡�ӿ�
public:
	//��¼��Ϣ
	virtual bool __cdecl SendLogonMessage();
	//������Ϣ
	virtual bool __cdecl SendConnectMessage();

	//�����б����
public:
	//CFriendListDlg * GetFriendListInterface() { return &m_friendlistDlg; }
	afx_msg void OnMove(int x, int y);
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnPaint();
	void OnBnClickedClose();	//�رճ���
	void ResetLogon();			//���µ�¼
	afx_msg void OnBnClickedReLogon();
// 	//�ҵı��� IDC_BT_BEAUTY_SALON
// 	afx_msg void OnBnClickedBeautySalon();
// 	//��Ϸ�̳� IDC_BT_SHOP
// 	afx_msg void OnBnClickedGameShop();
	//����     IDC_BT_RETURN
	afx_msg void OnBnClickedReturn();
	//����	   IDC_BT_SET
	afx_msg void OnBnClickedSet();
	//��С��   IDCMINSIZE
	afx_msg void OnBnClickedMinsize();
	afx_msg void OnBnClickedShop();
	afx_msg void OnBnClickedTask();
	afx_msg void OnBnClickedDepository();
	afx_msg void OnBnClickedGamePlazy();

	//���ص���¼����
	afx_msg void OnBnExitLogoUI();
	afx_msg void OnBnCancelGame();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//λ����Ϣ
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//�ر�ȷ��
	afx_msg void ConfirmClose();

public:
	DECLARE_DISPATCH_MAP()//����dispatch map��
	DECLARE_EVENTSINK_MAP()
	BOOL onHtmlContextMenu(IHTMLEventObj *pEvtObj);
	void OnDocumentCompleteExplorer3(LPDISPATCH pDisp, VARIANT FAR* URL);
	//�¼�������,ԭ�Ϳ��Բο�MSDN�й���HTMLDocumentEvents2��˵��
	DWORD m_dwCookie;
	//���ڱ��һ�����ӵ�
	IHTMLDocument2* pDoc2;
	//��Ҫ�����¼���COM�����ָ��

public:
	//�رմ���
	void CloseWebWindow(int nIDD);
	//�����û�������Ϣ
	void UpdateUserDJInfo(int iDJID);
	//��������
	void UpdateMusicSound(bool bIsEnable=true);
	//������ر�����
	void SetMusicSound(bool bIsEnable=true);
	//���õ�¼����
	void SetDlgLogonPtr(CDlgLogon * pDlgLogon);
	//��ʾ����
	void ShowTab(int nPage);
	//�л�����::wPos����ͼƬ��λ��
	void SwitchTitle(WORD wPos);
	//����Ƕ����ҳ::wWebModeҳ����ʽ,wTitlePos����λ��
	void LoadPlazaGameWeb(WORD wWebMode,WORD wTitlePos);
	/// ��Ȿ�����HASHֵ�������HASHֵ��һ�����������Ƿ���Ҫ����
	/// [return] Ϊ���ʾ�и��£� Ϊ�ٲ���Ҫ����
	BOOL CheckGameSoftHash(LPCTSTR pNewHash);
	// �����ⲿ��������
	BOOL CallExternalUpdateProgram();

private:
	BOOL			m_bIsGameSoftUpdate;	// ��Ϸ����Ƿ���Ҫ����
	//ʵ�����
	HINSTANCE		m_hUser32Instance;
	//��ָ��
	ShowLayer		m_pFunShowLayer;
	//��ǰalphaֵ
	int				m_uiCurAlphaValue;
	//��ǰ����
	HWND			m_hSaveNewCurWnd;
	HWND			m_hSaveOldCurWnd;
	//��ǰ���ڱ�ʶ
	BYTE			m_byCurWndTag;	//0:��ʾ����ִ�У�1:������2:����
	//����alphaֵ
	void	SetResetAlphaValue(HWND hWnd);
	//�������д�����ʾ״̬
	void	SetAllWndShow(bool bIsShow=false);
	//�������д���͸����
	void	SetAllWndAlpha(BYTE byAlpha=0);
	//�Ƿ���Ʊ���
	//bool			m_bIsDrawBack;
	

private:
	//����
	CFont			m_fontNickName;
	CFont			m_fontXPValue;
	CFont			m_fontDrawXJ;

	CNewHandTask*	m_pNewHandTask;

private:
	//0:��Ϸ����; 1:����; 2:�ر���; 3:�̳�
	//cxf 1024 PicButton		m_TitleButtonList[4];	//��������ť.

	//ִ����Ӧ��ť
	void  ExeButtonEvent(BYTE byEventID);
	//�������а�ť
	void  DrawTitleButton(CDC *pDC);
	//����������Ϸ��־λ
	void HideFlag();

private:
	//��ʾ��Ӧ��ť
	void SetShowTab(BYTE byValue=1);
	//�������
	void OnBnClickedVisitWeb();
	//�ʺŹ���
	void OnBnClickedAccountsManage();
	//��Ϸ����
	void OnBnClickedGameHelp();
	//���ⷴ��
	void OnBnClickedQuestionFeedBack();
	//���߿ͷ�
	void OnBnClickedContactServise();

	//��Ϸ�б�
	void OnBnClickedGameList();
	//����ר��
	void OnBnClickedTounament();
	//�ʺų�ֵ
	void OnBnClickedAccountsRecharge();
	//��Ʒ�һ�
	void OnBnClickedPrizeExcharge();
	//�����̳�
	void OnBnClickedPropsShop();
	//��������
	void OnBnClickedNoviceTask();
	
	//����
	void OnBnClickedBackPack();
	//�û���Ϣ
	void OnBnClickedUserInfo();
	//����
	void OnBnClickedBank();
	//�ʼ�
	void OnBnClickedEmailMessage();
	//���ذ�ť
	void OnBnClickedRollBack();
	//��һҳ��ť
	void OnBnClickedNextPage();
	//�����û���Ϣ��ť
	void OnBnClickedUpdateUserInfo();
	//����ָ��
	void OnBnClickedNewComerGuide();
	//��������ָ��
	void DoModalNewComerGuide();
	//������ɫ��
	bool StartSiSeQiuProgram();
	//�����ר��
	bool StartActivity(WORD wID);
	/// �������Ӽ���ж�
	BOOL IsCanIntervalConnect();

protected:
	//��ȡ��ǰ·��
	void GetCurrentDirectoryPath(CString &szPath);
	//��Դ·��
	void GetResPath(CString strClipName, CString& strPath);
	//��ȡ����
	UINT GetCommonConfig(CString strTitle,CString strName, int uiDefault);
	//��������
	void SetCommonConfig(CString strTitle,CString strName, CString bIsSound);

public:
	//��ʾ����
	void SetShowHideInfo(WORD dwType);
	//����ʧЧ
	void SetInvalidateRect();
	//�����û���Ϣ
	void UpdateUserInfo();
public:
	// �������߳�  
	bool createThreads();
	static unsigned __stdcall  thread_game_close(void* pParam)  ;
public:
	HANDLE			    m_hThread;	// �߳̾��
	unsigned int		m_dwThreadID;	// �߳�ID

	HANDLE		        m_hEventGameLoadOver[1];	// �¼����,�ر���Ϸ
	CString				m_strGameLoadOver;			// ��Ϸ�������
};
