#ifndef DLG_STATUS_HEAD_FILE
#define DLG_STATUS_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ʾ״̬��
class CDlgStatus : public CDialog
{
	//��������
protected:
	WORD								m_wImagePos;					//λͼλ��					
	CWnd								* m_pNotifyWnd;					//֪ͨ����
	CString								m_strMessage;					//��Ϣ�ַ�

	//�ؼ�����
protected:
	CSkinButton							m_btCancel;						//ȡ����ť
	CGDIPlus_Image						m_ImageBack;					//����ͼ
	int									m_iAction;						//֡��

private:
	BLENDFUNCTION						m_Blend;
	HINSTANCE							m_hFuncInst;
	int									m_BakWidth;
	int									m_BakHeight;
	CDC									m_hdcMemory;
	typedef BOOL						(WINAPI *MYFUNC)(HWND,HDC,POINT*,SIZE*,HDC,POINT*,COLORREF,BLENDFUNCTION*,DWORD);
	MYFUNC								UpdateLayeredWindow;

	HICON								m_hIcon;
//	CDC m_memdc;
	//��������
public:
	//���캯��
	CDlgStatus();
	//��������
	virtual ~CDlgStatus();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//ȷ������
	virtual void OnOK() { };
	//ȡ������
	virtual void OnCancel() { return; };

	//��ȡ��ǰ·��
	void GetCurrentDirectoryPath(CString &szPath);
	//��Դ·��
	void GetResPath(CString strClipName, CString& strPath);

	BOOL UpdateClockDisplay(int Transparent=255);
	//���ܺ���
public:
	//������Ϣ
	bool ShowStatusMessage(LPCTSTR pszMessage, CWnd * pNotifyWnd);
	//���ٴ���
	bool DestroyStatusWnd(CWnd * pNotifyWnd);

	//��Ϣӳ��
protected:
	//�ػ���Ϣ
	afx_msg void OnPaint();
	//ȡ����ť
	afx_msg void OnBnClickedClose();
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();


	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

//////////////////////////////////////////////////////////////////////////

#endif