#pragma once
#include "SkinControls.h"
#include "resource.h"
#include "afxwin.h"
#include "ImageNums.h"
#include "GameRuleDlg.h"

// CGameRoomDlg �Ի���
struct GameRoomRec
{
	CString						strImgNameBK;						//����ͼƬ
	CString						strImgNameFT;						//ǰ��ͼƬ
	CString						strImgNameFocus;					//��ȡ���ͼƬ
	WORD						wKindID;							//��ϷID
	WORD						wLight;								//��������״̬���
	WORD						wSortID;							//��Ϸ����(1:����;2:����.....)
	LONG						lCellScore;							//��Ԫ����
	LONG						lRestrictScore;						//����������
	int							iDifference;						//��ť����λ�ò���
	LONG						lEnterMoney;						//��ͽ�����
	GameRoomRec()
	{
		strImgNameBK="";
		strImgNameFT="";
		strImgNameFocus="";
		wKindID=0;
		wLight=0;
		wSortID=0;
		lCellScore=0;
		lRestrictScore=0;
		iDifference=0;
	}
};

class CGameRoomDlg : public CDialog
{
	DECLARE_DYNAMIC(CGameRoomDlg)

public:
	CGameRoomDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGameRoomDlg();

// �Ի�������
	enum { IDD = IDD_GAME_ROOM_DLG };

public:
	WORD					m_wMode;						//����ģ��(1:��Ϸ�б�;2:�����б�)
	CPngImage				m_ImgBK;						//����ͼƬ
	CPngImage				m_ImgFT;						//ǰ��ͼƬ
	CPngImage				m_ImgFocus;						//��ȡ����ͼƬ
	CPngImage				m_ImgGameRoomFont;				//��Ϸ�������
	WORD					m_wKindID;						//��ϷID
	WORD					m_wLight;						//��������״̬���
	WORD					m_wSortID;						//��Ϸ����(1:����;2:����.....)
	LONG					m_lCellScore;					//��Ԫ����
	LONG					m_lRestrictScore;				//����������
	int						m_iDifference;					//��ť����λ�ò���
	LONG					m_lEnterMoney;					//��ͽ�����
private:
	CPngImage				m_ImgRoomName;					//��������ͼƬ
	CPngImage				m_ImgLight;						//��������״̬��־
	CImageNums				m_ImageNumsHuang;				//���ֻ�ɫ
	CImageNums				m_ImageNumsLv;					//������ɫ

	//��ť
	CPngImage				m_ImageBTRule;					//��Ϸ����ť
	CPngImage				m_ImageBTGame;					//������Ϸ��ť

private:
	CDC						m_memdc;						//
	CRect					m_rcClientRect;					//��ǰ���ڴ�С


private:
	CRect					m_rcGameBT;						//��Ϸ��ť����
	CRect					m_rcRuleBT;						//����ť����

	//״̬: 0:Ĭ��״̬; 2:�������ȥ״̬; 1:��갴��ȥ״̬.
	BYTE					m_byGameBTState;				//��Ϸ��ť״̬
	BYTE					m_byRuleBTState;				//����ť״̬

	bool					m_bHovering;
	bool					m_bIsDrawFocus;					//�Ƿ���ƽ���

private:
	CGameRuleDlg			m_GameRuleDlg;				//��Ϸ����

public:
	//���ô���λ��
	void SetWndPos(WORD x, WORD y, WORD w, WORD h);
	//���ô���ģʽ(1:��Ϸ�б�;2:�����б�)
	void SetMode(WORD wMode);
	//������Դ
	void SetResourceInfo(GameRoomRec stGameRoomRec);
	//��ʾ����
	void SetWndShow();
	//���ش���
	void SetWndHide();

private:
	//��ǰ·��
	void GetCurrentDirectoryPath(CString &szPath);
	//������Դ·��
	void GetImagePlazaPath(CString& strRetValue);
	//��Դ·��
	void GetImageDataPath(CString& strRetValue);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��


public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	afx_msg void OnBnClickedGameRule();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedEnterGame();

	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
};
