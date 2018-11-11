#pragma once

enum ST_SHOWTAG
{
	ST_LEFT = 0,
	ST_CENTER,
	ST_RIGHT,
};

// CDlgAIPrompt �Ի���

class CDlgAIPrompt : public CDialog
{
	DECLARE_DYNAMIC(CDlgAIPrompt)

	//���ñ���
protected:
	CPoint							m_PointBenchmark;					//��׼λ��

public:
	CDlgAIPrompt(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAIPrompt();

// �Ի�������
	enum { IDD = IDD_AI_PROMPT };


public:
	//����ͼƬ
	bool LoadImage(const char* pImagePath);
	//����������
	void SetCardData(const BYTE* pCardData, int iCardNums);
	//��ʾ����
	void ShowWin(POINT ptPoint, ST_SHOWTAG showtag=ST_CENTER);
	//������͸����
	void SetCardAlpha(float fAlpha=1.0f);
	//�����ƻ������
	void SetCardDrawXY(POINT ptPoint);

public:
	void AddRectRgn(CDC* pDC,CRgn* rgn,CRect* rect,COLORREF col);
	void UpdateRgn(CDC * pDC);

private:
	CGDIPlus_Image		m_ImageCard;//��
	CGDIPlus_Image		m_ImageCenter;//��
	CGDIPlus_Image		m_ImageLeft;//��
	CGDIPlus_Image		m_ImageRight;//��
	CGDIPlus_Image      m_ImageCardBack; //�Ƶ�

	float				m_fCardAlpha;//͸����
	POINT				m_fDrawXY;	//���������

private:
	int					m_iAbs_Left;
	int					m_iAbs_Right;
	int					m_iAbs_Center_X;
	int					m_iAbs_Center_Y;

	int					m_iCurWidth;
	int					m_iCurHeight;

private:
	std::vector<BYTE>	m_vecCardList;	//���б�


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
