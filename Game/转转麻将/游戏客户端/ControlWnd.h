#ifndef CONTROL_WND_HEAD_FILE
#define CONTROL_WND_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GameLogic.h"

class CGameClientView;
class CGameClientDlg;
//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define IDM_CARD_OPERATE			(WM_USER+301)						//�˿˲���

////////////////////////////////////////////////////////////////////////////
//
//struct ShowGangListData
//{
//	BYTE	byCardData;
//	BYTE	byTag;	//0:��, 1:��
//};
//
//
//
//
//
////���ƴ���
//class CControlWnd : public CWnd
//{
//	//���ñ���
//protected:
//	BYTE							m_cbActionMask;						//��������
//	BYTE							m_cbCenterCard;						//�����˿�
//	BYTE							m_cbGangCard[5];					//��������
//	CPoint							m_PointBenchmark;					//��׼λ��
//
//	ShowGangListData				m_sgldCardData[8];					//��ʾ�ܲ�������
//	BYTE							m_sgldCardNums;						//����
//
//
//	//״̬����
//protected:
//	BYTE							m_cbItemCount;						//������Ŀ
//	BYTE							m_cbCurrentItem;					//��ǰ����
//
//	//�ؼ�����
//public:
//	CSkinButton						m_btChiHu;							//�Ժ���ť
//	CSkinButton						m_btGiveUp;							//������ť
//
//
//	//��Դ����
//protected:
//	CPngImage						m_ImageControlTop;					//��ԴͼƬ
//	CGDIPlus_Image					m_ImageControlMid;					//��ԴͼƬ
//	CPngImage						m_ImageControlBottom;				//��ԴͼƬ
//  	CPngImage						m_ImageActionExplain;				//��������
//	CPngImage                       m_ImageTableBottom;                 //�齫��Դ
//	CPngImage                       m_ImageActionBlack;                 //��������Ӱ
//	CPngImage                       m_ImageActionCardBack;              //�������Ʊ���
//	CPngImage                       m_ImageActionCardNum;               //������������
//
//	//��������
//public:
//	//���캯��
//	CControlWnd();
//	//��������
//	virtual ~CControlWnd();
//
//	//�ؼ�����
//public:
//	//��׼λ��
//	void SetBenchmarkPos(int nXPos, int nYPos);
//	//�����˿�
//	void SetControlInfo(BYTE cbCenterCard, BYTE cbActionMask, tagGangCardResult & GangCardResult);
//	//�ܲ���Ϣ
//	void AddGangCardData(const BYTE* pGangCardData, BYTE byCardNums );
//	//��ȡλ��
//	int GetImageIndex(BYTE cbCardData);
//
//
//	//�ڲ�����
//protected:
//	//�����ؼ�
//	void RectifyControl();
//
//	//��Ϣӳ��
//protected:
//	//�ػ�����
//	afx_msg void OnPaint();
//	//�Ժ���ť
//	afx_msg void OnChiHu();
//	//������ť
//	afx_msg void OnGiveUp();
//	//������Ϣ
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	//�����Ϣ
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
//	//�����Ϣ
//	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
//
//	DECLARE_MESSAGE_MAP()
//
//
//public:
//	void AddRectRgn(CDC* pDC,CRgn* rgn,CRect* rect,COLORREF col);
//	void UpdateRgn(CDC * pDC);
//	//bool DrawWindow(CDC * pDC);
//};

//////////////////////////////////////////////////////////////////////////
class CControlWnd: public CWnd
{
	friend class CGameClientView;
	friend class CGameClientDlg;

	DECLARE_MESSAGE_MAP()
public:
	CControlWnd();
	~CControlWnd();
	/// ���ö���
	/// [@param byCardData in] ������
	/// [@param byAction in] ��������
	void SetAction(BYTE byCardData, BYTE byAction);

protected:
	BOOL SetImageRgn(HWND hWnd, CPngImage * pImage);
	// ��ȫ��ʾ�����ش���
	void SafeShowOrHideWnd(CWnd* pWnd, int nCmdShow);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnBtClickPeng();
	afx_msg void OnBtClickBu();
	afx_msg void OnBtClickHu();
	afx_msg void OnBtClickGiveup();


private:
	CSkinButton						m_btPeng;					///< ����ť
	CSkinButton						m_btBu;						///< ����ť
	CSkinButton						m_bthu;						///< ����ť
	CSkinButton						m_btGiveUp;				///< ������ť

	CPngImage						m_pngBackground;	///< ����
	BYTE									m_bAction;					///< ����
	BYTE									m_bCardData;			///< ������
};
#endif