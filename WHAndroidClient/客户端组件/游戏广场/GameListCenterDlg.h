#pragma once

#include "resource.h"
#include "GameRoomDlg.h"

const int IGAME_COUNT = 9;

class CGameListCenterDlg : public CDialog
{
	DECLARE_DYNAMIC(CGameListCenterDlg)

public:
	CGameListCenterDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGameListCenterDlg();

// �Ի�������
	enum { IDD = IDD_GAME_LIST_CENTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CPngImage							m_ImageBK;						//����ͼƬ

private:
	CDC									m_memdc;						
	CGameRoomDlg						m_GameRoomDlg[IGAME_COUNT];				//��Ϸ
private:
	//��ȡ��ǰ·��
	void GetCurrentDirectoryPath(CString &szPath);
	//����·��
	void GetImagePlazaPath(CString& strRetValue);
	//������Ϸ��Ԫ
	void CreateGameUnit();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

public:
	//������Ϸ�б�
	void SetGameList();
	//����������Ϸ
	void SetWndHide();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
