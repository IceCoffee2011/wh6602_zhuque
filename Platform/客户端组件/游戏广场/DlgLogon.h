#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "KeyDialog.h"
#include "DlgLogonLogo.h"

//////////////////////////////////////////////////////////////////////////

//��¼ģʽ
enum enLogonMode
{
	LogonMode_Accounts,			//�ʺŵ�¼
	LogonMode_UserID,			//I D ��¼
};

#define KEY_DOWN			WM_USER + 0x300
//////////////////////////////////////////////////////////////////////////

//��¼ TAB �ؼ�
class CTabCtrlLogon : public CTabCtrl
{
	//��������
public:
	//���캯��
	CTabCtrlLogon();
	//��������
	virtual ~CTabCtrlLogon();

	//��Ϣ����
protected:
	//�ػ�����	
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
public:
	//	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
};

//////////////////////////////////////////////////////////////////////////

//�û�ע��
class CDlgRegister : public CSkinDialogEx
{
	friend class CDlgLogon;

	//��¼��Ϣ
public:
	WORD								m_wFaceID;						//ͷ���ʶ
	BYTE								m_cbGender;						//�û��Ա�
	TCHAR								m_szSpreader[NAME_LEN];			//��������
	TCHAR								m_szAccounts[NAME_LEN];			//��Ϸ�ʺ�
	TCHAR								m_szPassword[PASS_LEN];			//��Ϸ����
	TCHAR								m_szMail[NAME_LEN];				//�����ʺ�
	TCHAR								m_szMailSuffix[NAME_LEN];		//�����׺
	//�ؼ�����
public:
	CImageList							m_ImageList;
	CComboBoxEx							m_FaceSelect;
	CSkinHyperLink						m_LineRegWeb;					//��վע��
	CSkinHyperLink						m_LineMainPage;					//��Ϸ��ҳ
	CSkinHyperLink						m_LinePassWord;					//���뱣��
	CRgnButton							m_btLogon;						//��¼\ע�ᰴť
	CRgnButton							m_btCancel;						//ȡ����ť
	CRgnButton							m_btUserDocu;					//�û�Э��
	CRgnButton							m_btUserSexMan;					//�Ա���
	CRgnButton							m_btUserSexWoman;				//�Ա�Ů

	CString								m_strOutput;						//�û���ʾ
	CRect								m_rcOutput;
	//��������
public:
	//���캯��
	CDlgRegister();
	//��������
	virtual ~CDlgRegister();


	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual void OnOK();
	//ȡ����Ϣ
	virtual void OnCancel();

	virtual void OnPaint();

	//�޸��û��Ա𡪡���
	afx_msg void OnEnMakeUserSexMan();
	//�޸��û��Ա𡪡�Ů
	afx_msg void OnEnMakeUserSexWoman();
	//�鿴�û�Э��
	afx_msg void OnBnClickedUserDocu();
	//�����ʺ�
	afx_msg void OnEnSetfocusAccouts();
	//��������
	afx_msg void OnEnSetfocusPassword();
	//����ȷ������
	afx_msg void OnEnSetfocusPassword2();
	//��������
	afx_msg void OnEnSetfocusMail();
	//���������׺
	afx_msg void OnEnSetfocusMail2();
	//�����Ƽ���
	afx_msg void OnEnSetfocusSpreader();

	DECLARE_MESSAGE_MAP()
protected:
	void UpdateUserSex();
	BOOL IsOK();
	virtual void PreSubclassWindow();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);	
};

//////////////////////////////////////////////////////////////////////////

//��¼�Ի���
class CDlgLogon : public CSkinDialogEx
{
	friend class CRoomViewItem;
	friend class CPlazaViewItem;
	friend class CPlazaDlg;

	//��¼��Ϣ
protected:
	WORD								m_wFaceID;						//ͷ���ʶ
	BYTE								m_cbGender;						//�û��Ա�
	DWORD								m_dwUserID;						//�û� I D
	TCHAR								m_szSpreader[NAME_LEN];			//�ƹ�����
	TCHAR								m_szAccounts[NAME_LEN];			//��Ϸ�ʺ�
	TCHAR								m_szPassword[PASS_LEN];			//��Ϸ����
	TCHAR								m_szMail[NAME_LEN*3];	//����

	//λ�ñ���
protected:
	INT									m_nFullWidth;					//ȫ�����
	INT									m_nFullHeight;					//ȫ���߶�
	bool								m_bNetOption;					//��������
	CRect								m_rcNormalFrame;				//���λ��

	//��¼ģʽ
protected:
	enLogonMode							m_LogonMode;					//��¼ģʽ
	DWORD								m_enRule;						//�ʺ�ģʽ
	//������Ϣ
protected:
	bool								m_bRegister;					//ע���־
	bool								m_bChangePassWord;				//�ı��־
	bool								m_bRemPassword;					//��ס�����־
	//������Ϣ
protected:
	CString								m_strLogonServer;				//��������ַ

public:
	CPngImage							m_pngLogon;						//��½����(����ע�����)

	//��ť����
public:
	CSkinButton							m_btLogon;						//��¼��ť
	CSkinButton							m_btCancel;						//ȡ����ť
	CRgnButton							m_btDelete;						//ɾ����ť
	CRgnButton							m_btRegister;					//ע�ᰴť
	CRgnButton							m_btNetOption;					//���簴ť
	CRgnButton							m_btGetBackPassword;            //�һ�����
	CRgnButton							m_btVisitWeb;					//���ʹ���
	CRgnButton							m_checkPassword;				//��ס����
	CRgnButton							m_btnGameHelp;					//��Ϸ����
	CRgnButton							m_btnGameShop;					//�̳���ֵ
	CRgnButton							m_btnGameClinet;				//�ͻ�����
	CRgnButton							m_btnKey;						//�����
	CToolTipCtrl						m_ToolTipCtrl;					//��ʾ�ؼ�
	CRgnButton							m_btTelecom;					//����
	CRgnButton							m_btMeshWork;					//��ͨ
	CRgnButton							m_btAuto;						//�Զ�
	CRgnButton							m_btAccountsManage;				//�ʺŹ���
	CRgnButton							m_btGameWeb;					//��Ϸ��վ
	CRgnButton							m_btExit;						//�˳���¼
	//�ؼ�����
public:
	CWebBrowser							m_BrowerAD;						//�������
	CWebBrowser							m_LogonAdvertisement;			//��¼���
	CWebBrowser							m_LogonNotice;					//��¼����
	CTabCtrlLogon						m_TabLogonMode;					//��¼ѡ��

	CSkinHyperLink						m_LineMainPage;					//��Ϸ��ҳ
	CSkinHyperLink						m_LinePassWord;					//���뱣��
	CSkinHyperLink						m_LineGetPassWord;				//ȡ������

	//�������
public:
	CSkinEdit							m_edProxyPort;					//����˿�
	CSkinEdit							m_edProxyServer;				//�����ַ
	CSkinEdit							m_edProxyUserName;				//�����û�
	CSkinEdit							m_edProxyPassword;				//��������
	CComboBox							m_cmProxyServerType;			//��������
	CRgnButton							m_btProxyTest;					//�������
	CKeyDialog							m_keydlg;						//�����
	bool								m_bIsKeyDlgShow;				//������Ƿ���ʾ
	//�༭�ؼ�
protected:
	ComboBoxEx							m_edUserAccounts;				//�ʺ�
	CSkinEdit							m_edUserPassword;				//����

private:
	CDlgLogonLogo						* m_DlgLogonLogo;				//��¼Logoͼ����
	bool								m_bIsLogoCreate;

	//��������
public:
	//���캯��
	CDlgLogon();
	//��������
	virtual ~CDlgLogon();

	//���غ���
public:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��ʼ������
	virtual BOOL OnInitDialog();

	virtual void OnLButtonDown(UINT nFlags, CPoint point);

public:
	//ȷ������
	virtual void OnOK();
	//ȡ����Ϣ
	virtual void OnCancel();
	//�˳���¼
	void OnBnClickedOnExit();
	/// �ؼ���ֹ
	void EnableControlWindow(BOOL bEnable = 1);

public:
	void ExitLogon();

	//���ܺ���
public:
	//��ȡ������
	LPCTSTR GetLogonServer() { return m_strLogonServer; }
	//���͵�¼��
	bool SendLogonPacket(ITCPSocket * pIClientSocke);
	//��¼�ɹ�����
	bool OnLogonSuccess();

	//�ڲ�����
private:
	//���ط�����
	void LoadLogonServer();
	//��ȡ�ʺ�
	void LoadAccountsInfo();
	//������Ϣ
	void LoadProxyServerInfo();
	//Ч������
	bool CheckLogonInput(bool bShowError);

	//��������
private:
	//��������
	void UpdateUserPassWord(DWORD dwUserDBID);
	//���������־
	void UpdateNetWorkTag(DWORD dwUserDBID);
	//���²��ű������ֱ�ʶ
	void UpdateBackMusicTag(DWORD dwUserDBID);
	//����ģʽ
	void SwitchNetOption(bool bNetOption);
	//�����ַ�
	int ComboBoxFindString(CComboBox * pComboBox, LPCTSTR pszFindString);
	//��ȡ����
	void GetProxyInfo(enProxyServerType &ProxyServerType, tagProxyServerInfo &ProxyServerInfo);
	//�����ж�
	bool EnableProxy() { return m_bNetOption;}
	//��ȡ��ǰ·��
	void GetCurrentDirectoryPath(CString &szResourcePath);
	//ͼƽ·��
	void GetImagePath(CString& strRetValue);
	//
	void GetResPath(CString strClipName, CString &strResPath);
public:
	BOOL SetImageRgn(CPngImage * pImage);
	//�˳�
	LRESULT OnExitMessage(WPARAM wParam, LPARAM lParam);
	//Inno ��װ�ű���������Ϣ ѡ����Ϸ
	LRESULT OnCallInnoIssMsgSelectGame(WPARAM wParam, LPARAM lParam);

	void UpdateKeyPos();
	//��Ϣ����
public:
	//��ʱ��
	afx_msg void OnTimer(UINT nIDEvent);
	//ע���ʺ�
	afx_msg void OnRegisterAccounts();
	//ɾ���û�
	afx_msg void OnDeleteAccounts();
	//��������
	afx_msg void OnBnClickedNetOption();
	//�������
	afx_msg void OnBnClickedProxyTest();
	//����ı�
	afx_msg void OnEnChangePassword();
	//��ס����
	afx_msg void OnBnClickCheckPassword();
	//��Ϸ����
	afx_msg void OnBnClickGameHelp();
	//��Ϸ�̳�
	afx_msg void OnBnClickGameShop();
	//�ͻ�����
	afx_msg void OnBnClickGameClient();
	//�����
	afx_msg void OnBnClickKey();
	//�ػ���Ϣ
	afx_msg void OnPaint();
	//�߿�
	afx_msg void OnNcPaint();

	afx_msg void OnBnClickedGetBackPassword();

	afx_msg void OnBnClickedVisitWeb();
	//�ʺŹ���
	afx_msg void OnBnClickedAccountsManage();
	//��Ϸ����
	afx_msg void OnBnClickedGameWeb();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnDestroy();

	//����
	afx_msg void OnEnTelecomMsg();
	//��ͨ
	afx_msg void OnEnMeshWorkMsg();
	/// �Զ�
	afx_msg void OnEnAutoMsg();
	/// ���ü����·���
	void SetCheckLineResult(BYTE bNetWorkTag);

	virtual  LRESULT OnKeyDownDlg(WPARAM wParam, LPARAM lParam);
	//	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	//	afx_msg void OnMove(int x, int y);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);

	//��ⵯ���Ĵ���
	void OnPopRegisterDlg();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);

public:
	//���ؼ���
	void SetKeyHide();
	//ˢ�¿ؼ�
	void InvalidateRectControl();
};

//////////////////////////////////////////////////////////////////////////

