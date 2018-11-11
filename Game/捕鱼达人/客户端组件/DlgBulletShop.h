#ifndef BULLET_SHOP_DLG_HEAD_FILE
#define BULLET_SHOP_DLG_HEAD_FILE
#include "Stdafx.h"
#include "DxSkin.h"
#include "DxDialog.h"
#include "BulletHelpDlg.h"

#pragma once

//��ť����
enum enButtonsID
{
	enButtonsID_Begin=100,
	enButtonsID_Invalidate,

	enButtonsID_Bullet100,
	enButtonsID_Bullet600,
	enButtonsID_Bullet800,
	enButtonsID_Return,
	enButtonsID_Buy,
	enButtonsID_Help,

	enButtonsID_End
};

//������
class CGameClientViewDX;

//�ӵ�����
class CDlgBulletShop : public CDialog
{
	//״̬����
private:
	enButtonsID							m_CurButtonsID;						//��ǰ��ť
	enButtonsID							m_CurBulletID;						//��ǰ��ť
	bool								m_bLeftButtonDown;					//��갴��
	enBulletCountKind					m_BulletCountKindSelected;			//ѡ������

	//��Ϸ����
protected:
	LONG								m_lBulletCharge[enBulletCountKind_End];	//�ӵ�����
	LONG								m_lCellScore;							//��Ԫ����

	//λ�ñ���
private:
	CRect								m_rcBullet100;						//λ�ñ���
	CRect								m_rcBullet600;						//λ�ñ���
	CRect								m_rcBullet800;						//λ�ñ���
	CRect								m_rcReturn;							//λ�ñ���
	CRect								m_rcBuy;							//λ�ñ���
	CRect								m_rcHelp;							//λ�ñ���

	//�������
protected:
	CPngImage							m_PngBack;							//���汳��
	CPngImage							m_PngBullet100;						//�ӵ�ͼƬ
	CPngImage							m_PngBullet600;						//�ӵ�ͼƬ
	CPngImage							m_PngBullet800;						//�ӵ�ͼƬ
	CPngImage							m_PngReturn;						//����ͼƬ
	CPngImage							m_PngBuy;							//����ͼƬ
	CPngImage							m_PngHelp;							//����ͼƬ

	//�������
protected:
	CGameClientViewDX					* m_pGameClientViewDX;				//��ͼ����

	//��������
public:
	//���캯��
	CDlgBulletShop(CGameClientViewDX * pGameClientViewDX, enBulletCountKind	BulletCountKindSelected, LONG lCellScore);
	//��������
	virtual ~CDlgBulletShop();

	//�̳к���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange* pDX);
	//��ʼ����
	virtual BOOL OnInitDialog();

	//�ӿں���
public:
	//�ӵ�����
	enBulletCountKind GetBulletKind();
	//���÷���
	void SetBulletCharge(LONG lBulletCharge[enBulletCountKind_End]);

	//�̳нӿ�
protected:
	//�滭����
	afx_msg void OnPaint();    	

	//��Ϣ��Ӧ
public:
	//��갴��
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//�ƶ���Ϣ
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//������Ϣ
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	//��������
private:
	//��������
	enButtonsID GetButtonID(CPoint &ptPos);

	//��Ϣӳ��
	DECLARE_DYNAMIC(CDlgBulletShop)
	DECLARE_MESSAGE_MAP()	
};

#endif