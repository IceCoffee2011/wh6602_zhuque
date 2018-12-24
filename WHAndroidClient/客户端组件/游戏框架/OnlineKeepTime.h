#pragma once


// COnlineKeepTime �������񣬼�ʱ��

class COnlineKeepTime : public CWnd
{
	DECLARE_DYNAMIC(COnlineKeepTime)

public:
	COnlineKeepTime();
	virtual ~COnlineKeepTime();
	/// ����ʱ��
	void SetKeepTime(int iTime);
	/// ����״̬
	/// [@param bPlay in] Ϊ�沥�ţ�Ϊ����ͣ
	void PlayStatus(BOOL bPlay);

private:
	CPngImage				m_imgBackground;		///< ����
	CPngImage				m_imgMinNumber;			///< С����
	CPngImage				m_imgMaxNumber;			///< ������
	CPngImage				m_imgMaohao;			///< ð��

	int						m_iTotalSecond;			///< ������
	int						m_iHour;				///< ʱ
	int						m_iMinuter;				///< ��
	int						m_iSecond;				///< ��


protected:
	DECLARE_MESSAGE_MAP()
	// ����ʱ������
	void DrawTimeNum(CDC *pDC, CRect rc, CPngImage *pImg, int iFrameCount, int iNum);
	// ����ͳ��ʱ��
	void ReFigureNum(int iTime);
	// ����ͳ��
	void UpdateRgn(CDC * pDC);
	void AddRectRgn(CDC* pDC,CRgn* rgn,CRect* rect,COLORREF col);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};


