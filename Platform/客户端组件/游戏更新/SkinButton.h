#pragma once

#ifndef SKIN_BUTTON_HEAD_FILE
#define SKIN_BUTTON_HEAD_FILE

#include "SkinImage.h"

//////////////////////////////////////////////////////////////////////////

//�궨��
#define DEF_TEXT_FRAME_COLOR			RGB(255,255,255)				//Ĭ����ɫ
#define DEF_TEXT_COLOR					RGB(10,10,10)					//Ĭ����ɫ
#define DEF_TEXT_COLOR_EX				RGB(10,10,10)					//Ĭ����ɫ
#define DEF_TRANS_COLOR					RGB(255,0,255)					//Ĭ��͸������ɫ

//////////////////////////////////////////////////////////////////////////

//��ť��
class CSkinButton : public CButton
{
	friend class CRgnButton;

	//��������
protected:
	bool								m_bShowTextFrame;				//����߿�
	bool								m_bExpand;						//�Ƿ�����
	bool								m_bHovering;					//������־
	HICON								m_hIcon;						//��ťͼ��
	COLORREF							m_crTextColor;					//������ɫ
	COLORREF							m_crTextFrameColor;				//������ɫ
	CSkinImage							m_ImageBack;					//��ťλͼ
	int									m_nImageCount;					//ͼƬ����
	//�ӿ�ָ��
protected:
	//IMouseEvent							* m_pIMouseEvent;				//����¼�
	//osc
	CImage								m_BackImageBuffer;				//����λͼ
	//��ť������
public:
	//static CSkinButtonAttribute			m_SkinAttribute;				//��ť����

	//��������
public:
	//���캯��
	CSkinButton();
	//��������
	virtual ~CSkinButton();

	//���غ���
protected:
	//�ؼ����໯
	virtual void PreSubclassWindow();
	//����滭����
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//Ĭ�ϻص�����
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//���ܺ���
public:
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);

	//��ȡ���ص��ӿ�
	//IMouseEvent * GetMouseEventCallBack() { return m_pIMouseEvent; }
	//�������ص��ӿ�
	//void SetMouseEventCallBack(IMouseEvent * pIMouseEvent) { m_pIMouseEvent=pIMouseEvent; };

	//���ú���
public:
	//����λ��
	bool FixButtonSize();
	//����ͼ��
	bool SetButtonIcon(HICON hIcon);
	//������ɫ
	bool SetTextColor(COLORREF crTextColor,COLORREF crTextFrameColor=DEF_TEXT_FRAME_COLOR,bool bShowFrame=false);
	//����λͼ
	bool SetButtonImage(LPCTSTR pszFileName, bool bExpandImage, int nImageCount=5);
	//����λͼ
	bool SetButtonImage(UINT uBitmapID, HINSTANCE hInstance, bool bExpandImage, int nImageCount=5);
	//Ĭ�ϰ�ť
	void SetDefaultButton(bool bResetIcon=true, bool bResetColor=true, bool bResetImage=true);

	//��ѯ����
public:
	//��ȡͼ��
	HICON GetButtonIcon() { return m_hIcon; }
	//��ȡ��ɫ
	COLORREF GetButtonColor() { return m_crTextColor; }
	//��ȡλͼ����
	bool GetButtonLoadInfo(tagImageLoadInfo & LoadInfo);
	//��ȡ���Խӿ�
	//static CSkinButtonAttribute * GetSkinAttribute() { return &m_SkinAttribute; }

	//��Ϣ����
protected:
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//����ƶ�
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//����뿪
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	//������Ϣ
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CSkinButton)
};

//////////////////////////////////////////////////////////////////////////

//����ť��
class CRgnButton : public CButton
{
	//��������
protected:
	bool								m_bHovering;					//������־
	bool								m_bSelect;						//����
	CRgn								m_ButtonRgn;					//��������
	COLORREF							m_crTrans;						//͸����ɫ
	COLORREF							m_crTextColor;					//������ɫ
	CSkinImage							m_ImageBack;					//����λͼ
	int									m_nImageCount;					//ͼƬ����
	//�ӿ�ָ��
protected:
	//IMouseEvent							* m_pIMouseEvent;				//����¼�

	//��������
public:
	//���캯��
	CRgnButton();
	//��������
	virtual ~CRgnButton();

	//���غ���
protected:
	//�ؼ����໯
	virtual void PreSubclassWindow();
	//����滭����
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//Ĭ�ϻص�����
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	//���ܺ���
public:
	//����λͼ
	bool LoadRgnImage(LPCTSTR pszFileName, COLORREF crTrans, int nImageCount=5);
	//����λͼ
	bool LoadRgnImage(HINSTANCE hInstance, UINT uBitmapID, COLORREF crTrans=DEF_TRANS_COLOR, int nImageCount=5);
	//��ȡ���ص��ӿ�
	//IMouseEvent * GetMouseEventCallBack() { return m_pIMouseEvent; }
	//�������ص��ӿ�
	//void SetMouseEventCallBack(IMouseEvent * pIMouseEvent) { m_pIMouseEvent=pIMouseEvent; };
	//����ѡ��״̬
	void SetSelect(bool bselect){ m_bSelect = bselect;}
	//�ڲ����� 
private:
	//��������
	bool CreateControlRgn(COLORREF crTrans);

	//��Ϣ����
protected:
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//������Ϣ
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//����ƶ�
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//����뿪
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CRgnButton)
};

//////////////////////////////////////////////////////////////////////////

#endif