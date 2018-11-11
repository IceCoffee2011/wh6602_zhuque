#pragma once

// CGameBizCity �Ի���
#include "Stdafx.h"
#include "Resource.h"
#include "GameFrame.h"
#include <afxcmn.h>
#include "GameFrameExmain.h"

//��Ϸ�̳�
class CGameBizCity : public CDialog
{
	DECLARE_DYNAMIC(CGameBizCity)

public:
	CGameBizCity(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGameBizCity();

// �Ի�������
	enum { IDD = IDD_GAME_BIZ_CITY_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	//���������
	CGameFrameExmain		*				m_exMain;
	//�û�MD5����
	char						m_chUserMD5PassWord[PASS_LEN];	
public:
	virtual BOOL OnInitDialog();

	DECLARE_DISPATCH_MAP()//����dispatch map��
	DECLARE_EVENTSINK_MAP()
	BOOL onHtmlContextMenu(IHTMLEventObj *pEvtObj);
	BOOL onHtmlMouseDown(IHTMLEventObj *pEvtObj);
	void OnDocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT FAR* URL);
	//�¼�������,ԭ�Ϳ��Բο�MSDN�й���HTMLDocumentEvents2��˵��
	DWORD m_dwCookie;
	//���ڱ��һ�����ӵ�
	IHTMLDocument2* pDoc2;
	//��Ҫ�����¼���COM�����ָ��

public:
	//������ҳ����
	void SetGameBizCity(int iUserID, char* chUserData);
};
