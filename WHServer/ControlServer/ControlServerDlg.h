#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "ControlService.h"

//////////////////////////////////////////////////////////////////////////

//���öԻ���
class CSystemOptionDlg : public CDialog
{
	//���캯��
public:
	//�������
	CSystemOptionDlg();
	//��������
	virtual ~CSystemOptionDlg();

	//���غ���
protected:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//�ؼ����໯
	virtual void DoDataExchange(CDataExchange * pDX);
	//ȷ������
	virtual void OnOK();

	//��Ϣӳ��
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//���Ի���
class CControlServerDlg : public CDialog
{
	//�������
public:
	CControlService					m_ControlService;						//��¼����
	CRichEditTrace					m_RichEditTrace;					//�¼���¼

	//���캯��
public:
	//�������
	CControlServerDlg();
	//��������
	virtual ~CControlServerDlg();

	//���غ���
protected:
	//�ؼ����໯
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK();
	//ȡ������
	virtual void OnCancel();
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//��Ϣ����
protected:
	//��������
	afx_msg void OnBnClickedStartService();
	//ֹͣ����
	afx_msg void OnBnClickedStopService();
	//���÷���
	afx_msg void OnBnClickedInitService();
	//��������
	afx_msg void OnBnClickedSendCommand();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////