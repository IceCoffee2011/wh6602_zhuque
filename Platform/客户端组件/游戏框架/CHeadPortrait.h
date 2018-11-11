#pragma once
#ifndef _CHEAD_PORTRAIT_H
#define _CHEAD_PORTRAIT_H
#include "stdafx.h"

/// ����ͷ�񶯻���

class CHeadPortrait : public CWnd
{
	DECLARE_DYNAMIC(CHeadPortrait)
	friend class CHeadPortraitManager;

private:
	CWnd									* m_pParent;
	CDC										* m_pParentDC;
	CImage									* m_pParentImage;
	CGDIPlus_Image					* m_pImage;

	WORD										m_wID;										///< ����ID
	UINT										m_uActionCount;						///< ������Ŀ
	UINT										m_uFrameCount;						///< ֡��Ŀ
	vector<BOOL>						m_vFrames;								///< ֡��
	UINT										m_uCurAction;							///< ��ǰ����
	UINT										m_uCurFrame;							///< ��ǰ֡
	UINT										m_uDefaultAction;					///< Ĭ�϶���
	UINT										m_uIntervalTime;						///< ���ʱ��
	UINT										m_uFrameTime;						///< ��ǰʱ��					
	UINT										m_uRunCount;							///< ����ִ����

	POINT										m_ptCurPos;								///< ��ǰλ��

public:
	CHeadPortrait();
	virtual ~CHeadPortrait();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, 
		DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

protected:
	DECLARE_MESSAGE_MAP()

public:
	///  ���ö�����
	/// [@param bLine in] ������Ŀ(һ��ͼƬ�ɺܶද�����)����:��ͨ���������ˣ���������������ϲ�����ߣ�����
	/// [@param bRow in] ����֡��һ�������ɶ���֡���
	/// [@param pAction in] ����֡�� ��Ĭ��ֵ�� 7*2 ά���飩
	void SetAction(UINT bLine=1, UINT bRow=1, BOOL *pAction=NULL);
	void PlayAction(UINT uIndex, UINT uFrameTime=150, UINT uIntervalTime=0, UINT uResultTime=0, UINT uDefaultAction=0);

protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
};

typedef vector<CHeadPortrait *> VCHeadPortrait;


//////////////////////////////////////////////////////////////////////////////////
/// ����ͷ�񶯻�������

class GAME_FRAME_CLASS CHeadPortraitManager
{
public:
	CHeadPortraitManager();
	virtual ~CHeadPortraitManager();

	/// ��ʼ������ͷ����Ŀ
	/// [@param pParent in] ������ָ��
	/// [@param uCount in] ������Ŀ
	void Init(CWnd *pParent, UINT uCount);

	/// ��������
	/// [@param pt in] ����λ�ü�
	void SetPos(CPoint pt[]);

	/// ˢ�¶���
	/// [@param pDC in] ������ DC
	/// [@param pImage in] �����ڱ�������ͼƬָ��
	void Refresh(CDC *pDC);
	void Refresh(CImage *pImage);

	/// ���Ŷ���
	/// [@param uID in] ���� ID���������ID��
	/// [@param bWoman in] �Ա���ΪŮ����Ϊ��
	/// [@param uIndex in] ��������
	/// [@param uFrameTime in] ����ÿ֡��ʱ�䣬���Ĭ��֡��Ч��Ҳ��Ĭ��֡��ʱ�䣨���룩
	/// [@param uIntervalTime in] �������ʱ�� �����룩
	/// [@param uResultTime in] �����ܹ�ʱ�䣬С�ڵ��� 0 ��ʱ�����ƣ����룩
	/// [@param uDefaultAction in] Ĭ�϶������� uIndex ���������꣬�Զ�����Ĭ�϶�����
	void PlayAction(WORD wID, BOOL bWoman, UINT uIndex, UINT uFrameTime=150, UINT uIntervalTime=0, UINT uResultTime=0, UINT uDefaultAction=0);

	/// �������ж���
	void Hide(WORD uID, BOOL bAllHide=FALSE);

private:
	VCHeadPortrait	m_vHeadPortrait;				///< ����ͷ�񶯻�����ָ�뼯
};

#endif