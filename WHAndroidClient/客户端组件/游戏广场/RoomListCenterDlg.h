#pragma once

#include "resource.h"
#include "GameRoomDlg.h"
#include "GameInfo.h"
#include <afxtempl.h>

// CRoomListCenterDlg �Ի���
const			int						IGAME_ROOM_UNIT_COUNT = 4;
class CRoomListCenterDlg : public CDialog
{
	DECLARE_DYNAMIC(CRoomListCenterDlg)

public:
	CRoomListCenterDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRoomListCenterDlg();

// �Ի�������
	enum { IDD = IDD_ROOM_LIST_CENTER };

private:
	CPngImage							m_ImageBK;									//����ͼƬ

private:
	CDC									m_memdc;	
	CGameRoomDlg						m_GameRoomDlg[IGAME_ROOM_UNIT_COUNT];		//��Ϸ���䵥Ԫ

public:
	WORD								m_wKindID;									//��ϷID(��������-�����Ϸ��Ԫ����������)
	WORD								m_wSortID;									//�����������(��������-������䵥Ԫ����������)
	CArray<CGameRoomInfo,CGameRoomInfo>	m_arrayGameRoomInfo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	void EnterRoom();//������Ϸ�б�

private:
	//��ȡ��ǰ·��
	void GetCurrentDirectoryPath(CString &szPath);
	//����·��
	void GetImagePlazaPath(CString& strRetValue);
	//������Ϸ�б�
	void CreateRoomUnit();

public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
};
