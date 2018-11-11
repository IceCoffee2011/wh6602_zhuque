#ifndef CELL_SCORE_OPTION_DLG_HEAD_FILE
#define CELL_SCORE_OPTION_DLG_HEAD_FILE
#include "Stdafx.h"
#include "BulletHelpDlg.h"

#pragma once

class CDlgCellScoreOption : public CSkinDialogEx
{
	//��������
public:
	LONG							m_lCellScore[CELL_SCORE_COUNT];			//��Ԫ����
	LONG							m_lCellScoreSelect;						//��Ԫ����
	CWnd							* m_pCenterView;						//���Ĵ���

	//�ؼ�����
private:
	CSkinButton						m_btOK;									//ȷ����ť
	CComboBox						m_ComBoxCellScore;						//��Ԫ����

	//��������
public:
	//���캯��
	CDlgCellScoreOption(CWnd* pParent = NULL);
	//��������
	virtual ~CDlgCellScoreOption();
	
	//�̳к���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange* pDX);
	//��ʼ����
	virtual BOOL OnInitDialog();

	//��Ϣ����
private:
	//ȷ������
	virtual void OnOK();

	//��Ϣӳ��
	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CDlgCellScoreOption)	
};

#endif