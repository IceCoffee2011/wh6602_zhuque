#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "GoldControl.h"
#include "JettonControl.h"
#include "NumberControl.h"


//��ӡ��Ϣ
//#ifdef ZJH_GAME_PRINT_INFO



//////////////////////////////////////////////////////////////////////////
//��Ϣ����

//��Ϣ����
#define IDM_START					WM_USER+100							//��ʼ��Ϣ
#define IDM_ADD_SCORE				WM_USER+101							//��ע��Ϣ
#define	IDM_CONFIRM					WM_USER+109							//ȷ����Ϣ	
#define	IDM_CANCEL_ADD				WM_USER+119							//ȡ����Ϣ	
#define IDM_MIN_SCORE				WM_USER+102							//���ټ�ע
#define IDM_MAX_SCORE				WM_USER+103							//����ע
#define IDM_LOOK_CARD				WM_USER+104							//������Ϣ
#define IDM_COMPARE_CARD			WM_USER+105							//������Ϣ
#define IDM_OPEN_CARD				WM_USER+106							//������Ϣ
#define IDM_GIVE_UP					WM_USER+107							//������Ϣ
#define IDM_SEND_CARD_FINISH		WM_USER+108							//�������
#define IDM_COMPARE_USER			WM_USER+110							//��ѡ�û�
#define IDM_FALSH_CARD_FINISH		WM_USER+114							//�������

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//��������
struct tagSendCardItem
{
	WORD							wChairID;							//�����û�
	BYTE							cbCardData;							//��������
};

//������Ϣ
struct tagGoldMes
{
	LONGLONG						lGoldType;							//��������
	LONGLONG						lDrawCount;							//�滭����
};

//����˵��
typedef CArrayTemplate<tagSendCardItem,tagSendCardItem &> CSendCardItemArray;

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//���ݱ���
protected:
	WORD							m_wBankerUser;						//ׯ���û�
	LONGLONG						m_lMaxCellScore;					//�����ע
	LONGLONG						m_lCellScore;						//������ע
	LONGLONG						m_lTableScore[GAME_PLAYER];			//��ע��Ŀ
	LONGLONG						m_lStopUpdataScore[GAME_PLAYER];	//������Ŀ
	bool							m_bInvest;							//�ȴ���ע
	bool							m_bCompareCard;						//ѡ�ȱ�־
	WORD							m_wWaitUserChoice;					//�ȴ���־

	//��������
public:
	BYTE							m_cbPlayStatus[GAME_PLAYER];		//��Ϸ״̬
	bool							m_bShow;							//��ʾ����
	TCHAR							m_tcBuffer[GAME_PLAYER][64];		//�Ƶ�����

	//���Ʊ���
protected:
	WORD							m_wConmareIndex;					//��������
	WORD							m_wConmareCount;					//������Ŀ
	WORD							m_wCompareChairID[2];				//�����û�

	//���Ʊ���
protected:
	bool							m_bFalsh;							//���Ʋ���
	WORD							m_wFlashUser[GAME_PLAYER];			//�����û�
	WORD							m_wFalshCount;						//���ƴ���

	//������־
protected:
	bool							m_bCartoon;							//������־
	WORD							m_wLoserUser;						//���ưܻ�
	bool							m_bStopDraw;						//���Ʊ�־
	CSendCardItemArray				m_SendCardItemArray;				//��������

	//λ�ñ���
protected:
	CPoint							m_ptUserCompare;					//����λ��
	CPoint							m_SendCardPos;						//����λ��
	CPoint							m_ptUserCard[GAME_PLAYER];			//����λ��
	CPoint							m_KeepPos;							//����λ��
	CPoint							m_ptlGems[GAME_PLAYER];				//��ʯ
	CPoint							m_ptClass[GAME_PLAYER];				//�ȼ�

	//�û�״̬
protected:
	BOOL							m_bCompareUser[GAME_PLAYER];		//��ѡ�����û�

	//��ť�ؼ�
public:
	CRgnButton						m_btStart;							//��ʼ��ť

	CRgnButton						m_btMaxScore;						//���ť
	CRgnButton						m_btMinScore;						//����ť
	CRgnButton						m_btCancel;							//����ť
	CRgnButton						m_btConfirm;						//ȡ����ť

	CRgnButton						m_btAddScore;						//��ע��ť
	CRgnButton						m_btFollow;							//��ע��ť
	CRgnButton					    m_btGiveUp;							//������ť
	CRgnButton					    m_btLookCard;						//���ư�ť
	CRgnButton						m_btCompareCard;					//���ư�ť
	CRgnButton					    m_btOpenCard;						//���ư�ť


		//CSkinButton

	//�ؼ�����
public:
	//CGameScoreView				m_ScoreView;					    //�ɼ�����
	CGoldControl					m_GoldControl;						//��ע��ͼ
	CCardControl					m_CardControl[GAME_PLAYER];			//�û��˿�
	CJettonControl					m_JettonControl;					//����ؼ�
	CNumberControl					m_NumberControl[GAME_PLAYER];		//���ֿؼ�

	//λ����Ϣ
protected:
	CPoint							m_PointBanker[GAME_PLAYER];			//ׯ��λ��
	CPoint							m_ptJettons[GAME_PLAYER];			//����λ��
	CArrayTemplate<tagGoldMes>		m_lDrawGold[GAME_PLAYER];			//�滭��Ŀ
	CArrayTemplate<CPoint>			m_ptKeepJeton[GAME_PLAYER];			//���λ��

	//�������
protected:
	CPngImage						m_ImageBanker;						//ׯ����Դ
	CPngImage						m_ImageCard;						//�˿���Դ
	CSkinImage						m_ImageTitle;						//������Դ
	CSkinImage						m_ImageViewBack;					//������Դ
	CSkinImage						m_ImageViewCenter;					//������Դ
	CSkinImage						m_ImageJeton;						//������Դ
	CSkinImage						m_ImageArrowhead;					//��ͷ��Դ
	CSkinImage						m_ImageReady;						//׼����Դ
	CSkinImage						m_ImagePocket;						//�����Դ
	CPngImage						m_ImageNumber;						//������Դ
	CSkinImage						m_ImageBalance;						//������Դ

	CPngImage						m_ImageCompareCard;					//������ʾ
	CPngImage						m_ImageWait;						//�ȴ���ʾ

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//���غ���
protected:
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//�̳к���
private:
	//���ý���
	virtual void ResetGameView();
	//�����ؼ�
	virtual void RectifyGameView(int nWidth, int nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);

	//���ܺ���
public:
	//��ʼ���ֹ���
	void BeginMoveNumber();
	//ֹͣ���ֹ���
	void StopMoveNumber();
	//������ע
	void SetUserTableScore(WORD wChairID, LONGLONG lTableScore,LONGLONG lCurrentScore);
	//��ʼ���붯��
	void BeginMoveJettons();
	//ֹͣ���붯��
	void StopMoveJettons();
	//ʤ�����
	void SetGameEndInfo( WORD wWinner );
	//�ȴ�ѡ��
	void SetWaitUserChoice(WORD wChoice);
	//ׯ�ұ�־
	void SetBankerUser(WORD wBankerUse);
	//������Ϣ
	void SetScoreInfo(LONGLONG lMaxCellScore,LONGLONG lCellScore);
	//ѡ�ȱ�־
	void SetCompareCard(bool bCompareCard,BOOL bCompareUser[]);
	//ֹͣ����
	void StopUpdataScore(bool bStop);
	//�滭�˿�
	void DrawCompareCard(CDC * pDC, INT nXPos, INT nYPos, bool bChapped);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, LPRECT lpRect,UINT nFormat=DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	//�����ж�
	bool IsDispatchCard(){return m_bCartoon;}
	//��ȡ��Ϣ
	bool GetCompareInfo(){return m_bCompareCard;};

	//��������
public:
	//���ƶ���
	void PerformCompareCard(WORD wCompareUser[2], WORD wLoserUser);
	//���ƶ���
	void DispatchUserCard(WORD wChairID, BYTE cbCardData);
	//���ƶ���
	bool bFalshCard(WORD wFalshUser[]);
	//���ƴ���
	bool SendCard();
	//���ƴ���
	bool FlashCard();
	//���ƴ���
	bool CompareCard();
	//ֹͣ����
	void StopFlashCard();
	//ֹͣ����
	void StopCompareCard();
	//��ɷ���
	void FinishDispatchCard();

	//��Ϣӳ��
protected:
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
