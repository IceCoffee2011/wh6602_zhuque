//------------------------------------------------------------------------------
// �ļ����ƣ�UISkinManager.h
// �ļ��汾��v1.0
// �������ڣ�2006-02-14 16:59
// ��    �ߣ�Richard Ree
// �ļ���������������࣬�ṩʵ���Ի����Ĺ��ú����Լ�������ú��л�
//------------------------------------------------------------------------------

#pragma once

#ifndef UISKINMANAGER_H
#define UISKINMANAGER_H

class CUISkinManager
{
public:
	CUISkinManager();
	~CUISkinManager();

	static BOOL Init(bool bGenerate);
	static void Uninit();
	static BOOL LoadImages();
	static HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	static COLORREF GetColor();
	static HBRUSH GetBrush();

	// �Խ��䷽ʽ������
	static void GradientFillRect(CWnd* pWnd, CDC* pDC, CRect& rcFill, ULONG nMode, COLORREF crLeftTop, COLORREF crRightBottom);

	// ͸�����츴��
	static void TransparentBlt(HDC hdcDest, int nXDest, int nYDest, int nWidthDest, int nHeightDest, HDC hdcSrc, int nXSrc, int nYSrc, int nWidthSrc,int nHeightSrc, COLORREF crTransparent);

	// ���ƶԻ���
	static void Paint(CWnd* pWnd, CDC* pDC = NULL, BOOL bFillClient = TRUE, COLORREF crFillColor = RGB(255, 255, 255));
	static void Paint_Normal(CWnd* pWnd, CDC* pDC = NULL, BOOL bFillClient = TRUE, COLORREF crFillColor = RGB(255, 255, 255));
	static void Paint_XP(CWnd* pWnd, CDC* pDC = NULL, BOOL bFillClient = TRUE, COLORREF crFillColor = RGB(255, 255, 255));
	static void PaintDialogBorder(CWnd* pWnd, CDC* pDC = NULL);

	// ���öԻ���߿�
	static void SetRgn(CWnd* pWnd);
	static void SetRgn_Normal(CWnd* pWnd);
	static void SetRgn_XP(CWnd* pWnd);

public:
	// ������ö��
	enum SkinStyle
	{
		StyleNormal = 0,
		StyleSpring,
		StyleSummer,
		StyleAutumn,
		StyleWinter,
		StyleXP,
		StyleCustomize,
		SkinStyleBottom
	};
	// �л�������
	static void ChangeStyle(enum SkinStyle iNewStyle);

public:
	// ͸��ɫ
	static const COLORREF TRANSPARENT_COLOR;

	// ��ǰ������
	static enum SkinStyle m_iCurrentStyle;

	// ���ڱ��������

	// �������߶�
	static UINT m_iTitleBarHeight;
	// ���Ʊ����������ͼ���ȣ����أ�
	static UINT m_iTitleBarImageWidth;
	// ������ͼ����߲��ֵĿ��
	static UINT m_iTitleBarImageLeftPartWidth;
	// ������ͼ���м䲿�ֵĿ�ȣ��˲����ǿ�����ģ�
	static UINT m_iTitleBarImageMiddlePartWidth;
	// ������ͼ���ұ߲��ֵĿ��
	static UINT m_iTitleBarImageRightPartWidth;
	// ���Ʊ����������ͼ�񼯣�ÿ��ͼ����Ϊ m_iTitleBarImageWidth ���أ���������ʧȥ�����˳�����У��ܿ�Ȳ�С�� 2 * m_iTitleBarImageWidth ���أ�
	static CBitmap m_bmTitleBarImage;

	// �Ի��򴰿����ұ߾�
	static UINT m_iLeftMargin;
	// �Ի��򴰿����±߾�
	static UINT m_iTopMargin;

	// ��ť���

	// ��ť�߶ȣ����ư�ť�����ͼ��߶ȣ�
	static UINT m_iButtonHeight;
	// ���ư�ť�����ͼ���ȣ����أ�
	static UINT m_iButtonImageWidth;
	// ��ťͼ����߲��ֵĿ��
	static UINT m_iButtonImageLeftPartWidth;
	// ��ťͼ���м䲿�ֵĿ�ȣ��˲����ǿ�����ģ�
	static UINT m_iButtonImageMiddlePartWidth;
	// ��ťͼ���ұ߲��ֵĿ��
	static UINT m_iButtonImageRightPartWidth;
	// ���ư�ť�����ͼ�񼯣�ÿ��ͼ����Ϊ m_iButtonImageWidth ���أ���������ȱʡ�������ͣ����갴�µ�˳�����У��ܿ�Ȳ�С�� 4 * m_iButtonImageWidth ���أ�
	static CBitmap m_bmButtonImage;
	// ϵͳ���ư�ť�Ŀ�Ⱥ͸߶ȣ����أ�
	static UINT m_iSystemControlButtonWidth;
	static UINT m_iSystemControlButtonHeight;
	// ����ϵͳ���ư�ť����С������󻯡��رյȰ�ť�������ͼ�񼯣�ÿ��ͼ����Ϊ m_iSystemControlButtonImageWidth ���أ�����С������󻯡��ָ����رա���ť����״̬��˳�����У��ܿ�Ȳ�С�� 8 * m_iSystemControlButtonImageWidth ���أ�
	static CBitmap m_bmSystemControlButtonImage;

	// ���ƿؼ��Ļ���
	static HBRUSH m_hbr;
	// ����Բ��
	static POINT m_roundSize;
	// ���ɽ�������½����ж�
	static bool m_bGenerate;
};

#endif // #ifndef UISKINMANAGER_H
