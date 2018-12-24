#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "GlobalUnits.h"
#include "External.h"
//////////////////////////////////////////////////////////////////////////

//Ӧ�ó������
class CGamePlazaApp : public CWinApp
{
//	//��������
//public:
//	CSkinRecordFile							m_SkinRecordFile;				//��������

	//��������
public:
	//���캯��
	CGamePlazaApp();

	//���غ���
public:
	//��ʼ������
	virtual BOOL InitInstance();
	CExternal m_external;
	ULONG_PTR m_lGdiPlusToken;					//GID+ ��ʶ

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();

public:
	void CallInnoIssMsgSelectGame(WPARAM wParam, LPARAM lParam);

public:
	//�˳�����
	void ExitGame();
	void ExitGamePlaza();

public:
	void UpdateRegedit();
	/// ���������
	void GetCmdLinePara(CStringArray &paraArr);

protected:
	//cxf
	//��Ϸ����
	CPlazaDlg*				m_pPlazaDlg;
	//��Ϸ��¼
	CDlgLogon*				m_pDlgLogon;
	//end
};

//////////////////////////////////////////////////////////////////////////

//Ӧ�ó������˵��
extern CGamePlazaApp theApp;

//////////////////////////////////////////////////////////////////////////
