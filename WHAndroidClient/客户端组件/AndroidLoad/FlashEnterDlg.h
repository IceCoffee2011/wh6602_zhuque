// FlashEnterDlg.h : ͷ�ļ�
//

#pragma once
#include "resource.h"
#include "GameLogon.h"
#include "GameServer.h"
#include "afxwin.h"
#include "afxcmn.h"
//#include "xmlfile.h"
#include "xmlparse.h"

// CFlashEnterDlg �Ի���
class CFlashEnterDlg : public CDialog
{
// ����
public:
	CFlashEnterDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FLASHENTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

public:
	//���µ�¼��Ϣ
	void UpdateLogonInfo();
	void UpdateGameInfo();
	void UpdateQueueInfo();
	void UpdateLogonConnInfo();
	void UpdateGameConnInfo();
	//������Ϸ������Ϣ
	bool SetGameOptionInfo();
	//�����б�����
	void InsertListData(WORD dwSocketID, int dwErrorID, char* pErrorData);
	//�����������
	void ClearAllListData();
	//�رչ���
	void OnCloseTool();

private:
	//��¼����������
	CGameLogon		*m_pGameLogon;
	//��Ϸ����������
	CGameServer		*m_pGameServer;


public:
	afx_msg void OnBnClickedBtLogonConnect();
	afx_msg void OnBnClickedBtLogonExitServer();
	afx_msg void OnBnClickedBtGameConnect();
	afx_msg void OnBnClickedBtExitGameServer();
	afx_msg void OnBnClickedBtAutoUserQueue();
	afx_msg void OnBnClickedBtAllUserExitGame();


private:
	CEdit m_UserID;
	CEdit m_UserPassWord;
	CIPAddressCtrl m_ServerIP;
	CEdit m_GameID;
	CEdit m_RoomID;
	CEdit m_TotalUserNums;
	CEdit m_ServerPort;
	CEdit m_UserIndex;
	CEdit m_GameDLL;
	CListCtrl m_ErrorList;
	DWORD	m_dwListCountItem;
	CButton m_ConnectLogonServer;
	CButton m_ResetUserData;
	CButton m_ConnectGameServer;
	CButton m_ExitGameServer;
	CButton m_AutoQueue;
	CButton m_AllExitGame;
	CIPAddressCtrl m_GameServerIP;//Author<cxf>
	CEdit m_GameServerPort;//Author<cxf>


private:
	//CXmlFile*	m_XmlFile;
	CXMLParse xmlParse;
};
