#pragma once

#include "resource.h"
#include "afxwin.h"
// CGameListHot �Ի���

class CGameListHot : public CDialog
{
	DECLARE_DYNAMIC(CGameListHot)

public:
	CGameListHot(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGameListHot();

// �Ի�������
	enum { IDD = IDD_GAMELIST_HOT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	//CPngImage							m_imgLandCrazy;					// ������
	//CPngImage							m_imgSparrowZZ;					// ��ɳ�齫
	//CPngImage							m_imgSparrowYY;					// �����齫
	CPngImage							m_imgFrame;						// �տ�
	CPngImage							m_ImageBK;						//����ͼƬ
	CPngImage							m_ImageFlag;					//��־ͼƬ
	CRect								m_rectFlag;						//��־λ��
	//CRect								m_rectLandCrazyArea;			//������λ������
	//CRect								m_rectSparrowZZArea;			//��ɳ�齫λ������
	//CRect								m_rectSparrowYYArea;			//�����齫λ������
	CRect								m_rectFlagArea;					//��־λ������

	//bool								m_bIsDown;						
	
	CSkinButton							m_btnLandCrazy;					//������
	CSkinButton							m_btnSparrowZZ;					//תת�齫
	CSkinButton							m_btnSparrowYY;					//�����齫
	CSkinButton							m_btnSiSeQiu;					///< ��ɫ��

private:
	//CDC									m_memdc;						
	//CString								m_strCurrentPathPlaza;			//������Դ·��

public:
	//WORD								m_wLandCrazyPos;				//�滭λ��
	//WORD								m_wSparrowZZPos;				//�滭λ��
	//WORD								m_wSparrowYYPos;				//�滭λ��
	bool								m_bFlag;						//�Ƿ�滭��־
	
public:
	virtual BOOL OnInitDialog();
	// ������
	afx_msg void OnBnClickedLandcrazy();
	// ��ɳ�齫
	afx_msg void OnBnClickedSparrowZz();
	// �����齫
	afx_msg void OnBnClickedSparrowYy();
	/// ��ɫ��
	afx_msg void OnBnClickedSiSeQiu();

private:
	//��ȡ��ǰ·��
	void GetCurrentDirectoryPath(CString &szPath);
	//����·��
	void GetImagePlazaPath(CString& strRetValue);
public:
	afx_msg void OnPaint();

public:
	//���ñ�־
	void SetFlag(bool bFlag);
	//���ñ�־λ��
	void SetFlagPos(int x, int y);
	//������Ч����
	void SetInvalidateRect();
	//������ϷID
	void SetGameID(WORD wKinds);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
