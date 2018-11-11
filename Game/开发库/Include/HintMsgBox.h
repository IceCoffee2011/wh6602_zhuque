#ifndef HINT_MSG_HEAD_FILE
#define HINT_MSG_HEAD_FILE

#pragma once

#include "ClientShare.h"
#include "Resource.h"

#define MB_CUSTOMER		0x00000007L
//////////////////////////////////////////////////////////////////////////

//��Ϣ����
class CLIENT_SHARE_CLASS CHintMsgBox : public CSkinDialogEx
{
	//���Ա���
protected:
	UINT							m_uType;							//��Ϣ����
	CString							m_strString;						//��ʾ��Ϣ
	CString							m_strCaption;						//��Ϣ����

	//��������
protected:
	UINT							m_nElapse;							//ʣ��ʱ��
	UINT							m_nBtCount;							//��ť��Ŀ
	CRect							m_rcString;							//�ַ�λ��
	HINSTANCE						m_hResInstance;						//��Դ���

	CSkinImage m_imgMsgBox_LeftTop;
	CSkinImage m_imgMsgBox_Left;
	CSkinImage m_imgMsgBox_LeftBottom;
	CSkinImage m_imgMsgBox_Top;
	CSkinImage m_imgMsgBox_RightTop;
	CSkinImage m_imgMsgBox_Right;
	CSkinImage m_imgMsgBox_RightBottom;
	CSkinImage m_imgMsgBox_Bottom;
	CSkinImage m_imgMsgBox_Center;

	CDC m_memdc;
	//�ؼ�����
protected:
	CSkinButton						m_btButton[3];						//��ť�ؼ�
	CSkinButton						m_btnClose;
	//��������
public:
	//���캯��
	CHintMsgBox(CWnd * pParentWnd=NULL);
	//��������
	virtual ~CHintMsgBox();

	// �Ի�������
	enum { IDD = IDD_HINT_MSG };

	//���غ���
protected:
	//��ʼ����Ϣ
	virtual BOOL OnInitDialog();

	//���ܺ���
public:
	//��ʾ��Ϣ
	INT ShowMessageBox(LPCTSTR pszString, UINT uType=MB_OK, UINT nElapse=0);
	//��ʾ��Ϣ
	INT ShowMessageBox(LPCTSTR pszCaption, LPCTSTR pszString, UINT uType=MB_OK, UINT nElapse=0);
	//�߿���
	INT GetXBorder(){return 2;}
	//�߿�߶�
	INT GetYBorder(){return 2;}

	//��ͼ��Ϣ
public:
	bool Draw(CDC * pDC, CDC * pMemdc);
	bool DrawBack(CDC * pDC);
	void FillRectImage(CDC * pDC, CImage & img, const CRect & rect);

private:
	//��ȡ��ǰ·��
	void GetCurrentDirectoryPath(CString &szPath);
	//����·��
	void GetImageClientSharePath(CString& strRetValue);

	//�ڲ�����
private:
	//������ť
	VOID CreateBoxButton();

	void AddRectRgn(CDC* pDC,CRgn* rgn,CRect* rect,COLORREF col);
	void UpdateRgn(CDC * pDC);
	//��Ϣӳ��
protected:
	//�滭��Ϣ
	VOID OnPaint();
	//ʱ����Ϣ
	VOID OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedYes() { EndDialog(IDYES);}
	afx_msg void OnBnClickedNo() { EndDialog(IDNO);}
	afx_msg void OnBnClickedAbort() { EndDialog(IDABORT);}
	afx_msg void OnBnClickedRetry() { EndDialog(IDRETRY);}
	afx_msg void OnBnClickedIgnore() { EndDialog(IDIGNORE);}
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

};


//////////////////////////////////////////////////////////////////////////

//��ʾ��Ϣ
extern "C" CLIENT_SHARE_CLASS INT __cdecl ShowInformation(LPCTSTR pszString, UINT nElapse, UINT uType);
extern "C" CLIENT_SHARE_CLASS INT __cdecl ShowInformationEx(LPCTSTR pszString, UINT nElapse, UINT uType ,LPCTSTR pszCaption);


//////////////////////////////////////////////////////////////////////////

#endif