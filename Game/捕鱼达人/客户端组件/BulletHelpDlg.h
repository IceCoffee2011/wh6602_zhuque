#ifndef BULLET_HELP_DLG_HEAD_FILE
#define BULLET_HELP_DLG_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "SkinButtonEx.h"

//#define WM_SHOW_BULLET_SHOP		WM_USER+101								//�����ӵ�
//////////////////////////////////////////////////////////////////////////

//�����Ի���
class CBulletHelpDlg : public CDialog
{
	//��Դ����
protected:
	CPngImage							m_PngBack;						//����ͼ��

	//�������
protected:
	CWnd								* m_pViewWnd;					//��ͼ����

	//��ť����
protected:
	CSkinButtonEx						m_btClose;						//�رհ�ť

	//��������
public:
	//���캯��
	CBulletHelpDlg(CWnd	*pViewWnd);
	//��������
	virtual ~CBulletHelpDlg();

	//���غ���
public:
	//��ʼ����
	virtual BOOL OnInitDialog();
	//������Ϣ
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//�ؼ���
	virtual void DoDataExchange(CDataExchange* pDX);

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//�����Ϣ
	void OnLButtonDown(UINT nFlags, CPoint Point);

	DECLARE_MESSAGE_MAP()		
};

//////////////////////////////////////////////////////////////////////////

#endif