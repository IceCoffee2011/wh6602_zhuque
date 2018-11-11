#pragma once

#include "Resource.h"
#include "GameAffiche.h"
#include "ShowDJInfo.h"
#include "UserDaoJu.h"
#include "LanguageList.h"
#include <string>
#include "ImageNums.h"


class CGameFrameDlg;
class CGameFrameControl;


using namespace std;

// CUserInfoDlg �Ի���

class CUserInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserInfoDlg)

public:
	CUserInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUserInfoDlg();

// �Ի�������
	enum { IDD = IDD_USERINFODLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CPngImage		m_ImageBack;		//����
	CPngImage		m_InfoBack;			//��Ϣ����
	CPngImage		m_GoldBack;			//��ұ���
	CPngImage		m_GemsBack;			//��ʯ����
	CPngImage		m_NumImage;			//����ͼƬ
	CPngImage		m_ClassNumImage;	//�ȼ�����

	//CPngImage		m_GameInfoBack;		//��Ϸ���汳��

	CSkinButton		m_btSound;			//����
	CSkinButton		m_btBB;				//����

private:
	CImageNums		m_ClassNum;			//�ȼ�
	CImageNums		m_GoldNum;			//���
	CImageNums		m_GemsNum;			//��ʯ

	CToolTipCtrl	m_ToolTipCtrl;		//��ʾ�ؼ�
	CPoint			m_ptWnd;			//��������

private:
	//�û�����
	tagUserData*	m_pUserData;		//�û�����


	//������Ӵ���
	CGameAffiche				m_Affiche;						//
	//��Ϸ�û�����
	CUserDaoJu					* m_pGameUserDaoJu;		
	//���ָ��
	CGameFrameDlg				* m_pGameFrameDlg;
	//�����б�
	CLanguageList				* m_pLanguageListPtr;


private:
	std::string					m_strGradeName;


public:
	CGameFrameControl			* m_pGameFrameControl;				//������ͼ

public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();

public:
	//BOOL SetImageRgn(CPngImage * pImage);
	//�����û���Ϣ
	void SetUserData(tagUserData* pUserData);
	//���õ�����Ϣ
	void SetShowDJInfo(CShowDJInfo* pShowDJInfoPtr);
	//���������б�
	void SetLLInfoPtr(CLanguageList* pLLPtr);
	//��õ����б���Ϣ
	CUserDaoJu* GetUserDaoJuListPtr();
	//���ÿ��ָ��
	void SetGameFrameDlgPtr(CGameFrameDlg* pFrameDlg);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//���ť
	void EnableBTWindow();
	//�����û�����
	void UpdateUserData(CMD_GF_NEW_GIFT* pNewData);


	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
public:
	//����
	afx_msg void OnLanguageList();
	//����
	afx_msg void OnOpenBB();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	BOOL SetWindowPos(const CWnd* pWndInsertAfter, int x, int y,
		int cx, int cy, UINT nFlags);
	// �滭���� -����Ǿ��α߿�
	void DrawBackground(CDC *pDC, int nWidth, int nHeight, int iExcursionY=0);
};
