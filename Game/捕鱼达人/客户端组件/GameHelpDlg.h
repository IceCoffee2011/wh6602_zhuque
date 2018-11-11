#ifndef GAME_HELP_DLG_HEAD_FILE
#define GAME_HELP_DLG_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "SkinButtonEx.h"

//////////////////////////////////////////////////////////////////////////

//��������
enum enHelpKind
{
	enHelpKind_Game,
	enHelpKind_Bullet,
	enHelpKind_FishScore,
	enHelpKind_Canno
};

//�����Ի���
class CGameHelpDlg : public CDialog
{
	//״̬����
private:
	enHelpKind							m_HelpKind;						//��������

	//��Դ����
protected:
	CPngImage							m_PngGameHelp;					//����ͼ��
	CPngImage							m_PngBulletHelp;				//����ͼ��
	CPngImage							m_PngFishScore;					//����ͼ��
	CPngImage							m_PngCannoHelp;					//����ͼ��

	//�������
protected:
	CWnd								* m_pViewWnd;					//��ͼ����

	//��ť����
protected:
	CSkinButtonEx						m_btClose;						//�رհ�ť
	CSkinButtonEx						m_btGameHelp;					//������ť
	CSkinButtonEx						m_btBulletHelp;					//������ť
	CSkinButtonEx						m_btFishScore;					//������ť
	CSkinButtonEx						m_btCannoHelp;					//������ť

	//��������
public:
	//���캯��
	CGameHelpDlg(CWnd	*pViewWnd);
	//��������
	virtual ~CGameHelpDlg();

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