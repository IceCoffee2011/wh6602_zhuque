#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "ActionWnd.h"
#include "GameCard.h"
#include "..\..\������\Include\GameFlash.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_PASS_CARD				(WM_USER+102)						//��������
#define IDM_CALL_LAND				(WM_USER+103)						//�е�����Ϣ
#define IDM_AUTO_OUTCARD			(WM_USER+104)                       //��ʾ��Ϣ
#define IDM_LAND_AUTOPLAY			(WM_USER+105)                       //�й���Ϣ
#define IDM_SORT_HAND_CARD			(WM_USER+106)						//�����˿�
#define IDM_BRIGHT_START			(WM_USER+107)						//���ƿ�ʼ
#define IDM_DOUBLE_SCORE			(WM_USER+108)						//�ӱ���Ϣ
#define IDM_SNATCH_LAND             (WM_USER+109)                       //��������Ϣ


//��������
#define MARK_ACTION_SHUNLIAN          0x01                               //˳��,����
#define MARK_ACTION_ZHADAN            0x02                               //ը��
//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��Ϸ����
protected:
	WORD							m_wLandUser;						//�����û�
	WORD							m_wBombTime;						//ը������
	LONGLONG						m_lBaseScore;						//��������
	BYTE							m_cbLandScore;						//��������
	BYTE							m_bCardBackDate[3];					//��������
	bool							m_bDisplay;							//������ʾ
	BYTE                            m_bCardData[3][20];                 //�����������
	BYTE                            m_bHandCardCount[GAME_PLAYER];      //���������Ŀ
	BYTE                            m_DiPaiCount;                       //��������


	//״̬����
public:
	bool							m_bLandTitle;						//������־
	bool							m_bPass[GAME_PLAYER];				//��������
	BYTE							m_bScore[GAME_PLAYER];				//�û��з�
	BYTE							m_bCardCount[GAME_PLAYER];			//�˿���Ŀ
	bool							m_bDeasilOrder;						//˳ʱ���
	bool							m_bUserTrustee[GAME_PLAYER];		//����й�
	bool							m_bShowWarming[GAME_PLAYER];		//��ʾ����
	BYTE                            CardData[54];

	//������
public:
	BYTE							m_cbCallScorePhase;					//���ƽ׶�
	BYTE							m_cbUserPhase[GAME_PLAYER];			//��ҽ׶�
	bool							m_bBrightStart[GAME_PLAYER];		//���ƿ�ʼ
	CPoint							m_ptBrightStart[GAME_PLAYER];		//���ƿ�ʼ
	BYTE							m_cbBrightTime;						//���Ʊ���
	bool							m_bShowLandTime;					//��ʾ����
	bool							m_bDoubleScore[GAME_PLAYER];		//��Ҽӱ�
	bool                            m_bLeftUser;                          //������
	bool                            m_bTXEffect;                        //��ЧЧ��
	BYTE                            m_bTXFrameIndex;                    //��Ч����
	bool                            m_bHuoJian;                         //���Ч��
	BYTE                            m_bHuojianIndex;                    //�������
//lq* 
	//λ����Ϣ
public:
	CSize							m_LandSize;							//�ڿӱ�־
	CPoint							m_ptScore[GAME_PLAYER];				//�з�λ��
	CPoint							m_ptLand[GAME_PLAYER];				//����λ��
	CPoint							m_ptWarmingCard[GAME_PLAYER];		//����λ��
	CPoint							m_ptUserTrustee[GAME_PLAYER];		//����й�λ��
	CPoint							m_ptUserBaoShi[GAME_PLAYER];		//��ұ�ʯ
	CPoint							m_ptUserGrade[GAME_PLAYER];			//��ҵȼ�
	CPoint							m_ptUserSYPTS[GAME_PLAYER];			//ʣ������ʾ
	CPoint                          m_GrandFrame[GAME_PLAYER];          //�ȼ���
	CPoint                          m_UserDuanXian[GAME_PLAYER];        //��Ҷ���
	CPoint                          m_Rating[GAME_PLAYER];              //�ȼ���ν 
	CPoint                          m_OutCardPass[GAME_PLAYER];         //����

	//λͼ����
protected:
	CSkinImage						m_ImageBomb;						//ը��ͼƬ
	CSkinImage						m_ImageCenter;						//������Դ
	CPngImage						m_ImageWaitScore;					//�ȴ��з�
	CPngImage                       m_ImageBack;                        //����

	//PNG����
protected:
	CPngImage                       m_Imagebigness;                     //��������ʾͼƬ  lq;
	CPngImage						m_ImageScore;						//����ͼƬ
	CPngImage						m_ImageLand;						//ׯ��ͼƬ
	CPngImage						m_ImageUserTrustee;					//����й�
	//CPngImage						m_ImageOnecardWarming;				//������ʾ
	//CPngImage						m_ImageTwoCardWarming;				//��˫��ʾ
	CPngImage						m_ImageLandTimes;					//���Ʊ���
	CPngImage                       m_ImageLandbei;                     //����
	CPngImage						m_PngCardBack;						//����
	CPngImage                       m_XieCard;                          //�Լ����Ʊ�����ʾ
	CPngImage                       m_XieCardThree;                     //�Լ����Ʊ�������
	CPngImage                       m_XieCardTwo; 
	CPngImage                       m_XieCardOne; 
	CPngImage                       m_ImagePass;                        //passͼƬ 

	//ʣ��������
	//CPngImage						m_ImageSYPBack;						//ʣ������������
	//CPngImage						m_ImageSYPNums;						//����
	CPngImage                       m_ImageCardNum;                     //���Ƹ���
	CPngImage                       m_ImageTCard;                       //���Ƹ�������
	CPngImage                       m_ImageGrand;                        //�ȼ�����ͼƬ
	CPngImage                       m_ImageGrandFrame;                   //�ȼ���ͼƬ
	//����ͼƬ
	CGDIPlus_Image                  m_ImageShunZi;                              //˳��
    CGDIPlus_Image                  m_ImageLianDui;                              //����

	bool                            m_BIsshowFourcolor;     

	
	//��ť�ؼ�
public:
	CSkinButton						m_btStart;							//��ʼ��ť
	CSkinButton						m_btOutCard;						//���ư�ť
	CSkinButton						m_btPassCard;						//������ť
	CSkinButton						m_btAutoOutCard;					//��ʾ��ť
	CSkinButton						m_btCallLand;						//�е�����ť
	CSkinButton						m_btGiveUpCallLand;					//���а�ť
	CSkinButton						m_btSnatchLand;						//��������ť
	CSkinButton						m_btGiveUpSnatchLand;				//������ť
	CSkinButton						m_btSortCard;						//����ť
	CSkinButton					    m_btAutoPlayOn;						// �����Զ��й�
	CSkinButton						m_btAutoPlayOff;					// �ر��Զ��й�

	//��չ��ť
public:
	//CSkinButton						m_btSnatchLand;						//��ׯ��ť
	//CSkinButton						m_btBrightCard;						//���ư�ť
	//CSkinButton						m_btBrightStart;					//���ƿ�ʼ
	//cxf ɾ���ӱ���ť  ��(��-��-��)����
	//CRgnButton						m_btDoubleScore;					//�ӱ���ť
	//CRgnButton						m_btNotDoubleScore;					//���ӱ���ť
	//CSkinButton						m_btCardDao;							//��
	//CSkinButton						m_btCardGen;							//��
	//CSkinButton						m_btCardFan;							//��
	//CSkinButton                      m_bLastRound;                           //��һ�� lq
	//cxf end
	//�˿˿ؼ�
public:
	CCardControl					m_UserCardControl[GAME_PLAYER];		//���ƣ��˿���ͼ
	CCardControl					m_HandCardControl[GAME_PLAYER];		//�����˿�
//��������
public:
	ActionWnd                       m_ActionWnd[GAME_PLAYER];           //˳�ӣ����ԣ�ը����ͼ
	ActionWnd                       m_BActionWnd;                       //�ɻ���ͼ
	ActionWnd                       m_HuoJianAction;                    //���
	ActionWnd                       m_DoubleAction;                     //�ӱ�
	ActionWnd                       m_SpringAction;                     //����
	

public:
	//��Ϸ�ƿؼ�
	CGameCard						m_GameCard;
	CPngImage						m_PngCard;
	

	//��ʾʣ�༸����
public:
	CMD_C_UserTSZhangNums			m_UserTSCardNums[GAME_PLAYER];		//ÿ��������е�ʣ������(<=3)

#ifdef VIDEO_GAME

	//��Ƶ���
private:
	CVideoServiceManager			m_VedioServiceManager;				//��Ƶ����
	CVideoServiceControl			m_DlgVedioService[GAME_PLAYER];		//��Ƶ���

#endif

private:
	int								m_iMeBeiShu;                        //��ǰ����

private:
	bool							m_bIsShowbigness;					//�Ƿ���ʾ������ͼƬ
	bool                            m_bIsShowZhaDan;                    //�Ƿ���ʾը��
	bool                            m_bIsShowSpring;                    //�Ƿ���ʾ����

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();

	//�̳к���
private:
	//���ý���
	virtual void ResetGameView();
	//�����ؼ�
	virtual void RectifyGameView(int nWidth, int nHeight);
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight);

public:
	void GetPicPath(CString& strPath);

	//���ܺ���
public:
	//��ұ���
	public:
	void SetMeBeiShu(int iBeiShu);
	//ը������
	void SetBombTime(WORD wBombTime);
	//��������
	void SetBaseScore(LONGLONG lBaseScore);
	//��ʾ��ʾ
	void ShowLandTitle(bool bLandTitle);
	//���÷���
	void SetPassFlag(WORD wChairID, bool bPass);
	//�˿���Ŀ
	void SetCardCount(WORD wChairID, BYTE bCardCount);
	//�ڿӷ���
	void SetLandUser(WORD wChairID, BYTE bLandScore);
	//�ڿӷ���
	void SetLandScore(WORD wChairID, BYTE bLandScore, BYTE cbCallScorePhase, BYTE cbPreCallScorePhase);
	//����˳��
	void SetUserOrder(bool bDeasilOrder);
	//��ȡ˳��
	bool IsDeasilOrder() { return m_bDeasilOrder; }
	//���ñ�ը
	bool SetBombEffect(bool bBombEffect);
	//���ô���
	void SetSpring(); //lq
	bool setFanSpring(bool bSpring);//lq
	//���üӱ�
	bool SetDouble(bool bDouble);//lq

	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//����й�
	void SetUserTrustee( WORD wChairID, bool bTrustee );
	//��ʾ����
	void SetShowWarming(WORD wChairID, bool bShowWarming) ;
	//���õ���
	void SetCardBackData(BYTE *bCardDate,BYTE bCardCount)
	{
		CopyMemory(m_bCardBackDate,bCardDate,sizeof(BYTE)*bCardCount);
		m_DiPaiCount=bCardCount;
	};
	//������ʾ
	void SetCardBackDisPlay(bool bDisplay){m_bDisplay = bDisplay;};


	//OSC update
	//���ܣ���ʾ����3���ƵĹ���.
public:
	void SetUserShouPaiNums(WORD wViewID, WORD wNums);
	//����������
	void SetResetShouPaiNums();
	void SetResetSYPaiNums();

	//�����
public:
	//������Ч
	void ActionEffect(CDC *pDC, int nWidth, int nHeigth, int nFrame, int type);
	bool SetFeiJi(bool bTXEffect);
	bool SetHuoJian(bool bTXEffect);
	//���ƿ�ʼ
	void SetBrightStart(WORD wChairID, bool bBrightStart) ;
	//��ʾ����
	void DrawLandTime(CDC *pDC, INT nViewWidth, INT nViewHeight);
	//��ʾ�Լ����Ƹ���
	void DrawOtherUserHandNums(CDC *pDC, INT nViewWidth, INT nViewHeight, BOOL bLeftUser, INT nCardNums);
	//���Ʊ���
	void SetBrightTime(BYTE cbBrightTime);
	//��ʾ����
	void ShowLandTime(bool bShowLandTime);
	//��Ҽӱ�
	void SetDoubleScoreUser( WORD wChairID, bool bDoubleScore );
	//��ʾ�������
	void SetHandCardData(const BYTE bCardData[],BYTE bCardCount,WORD wChairID);

	//play shunzi
	void PlayActionFlash(BYTE byViewID,BYTE TyprAction);
	//play feiji
	void PlayFeijiFlash(bool bIsShow);
	// play Double
    void PlayDoubelFlash();

public:
	//������ʾ������ͼƬ
	void SetShowBigness(bool bIsShow=false);
    //������Դ
	void Loadres(CGdipButton* pBT, CString strValue);

	//��Ϣӳ��
protected:
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//��ʼ��ť
	afx_msg void OnStart();
	//���ư�ť
	afx_msg void OnOutCard();
	//������ť
	afx_msg void OnPassCard();
	//�е�����ť
	afx_msg void OnCallLand();
	//���а�ť
	afx_msg void OnGiveUpCallLand();
	//��������ť
	afx_msg void OnSnatchLand();
	//������ť
	afx_msg void OnGiveUpSnatchLand();
	//������ʾ
	afx_msg void OnAutoOutCard();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint Point);
	//���˫��
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	// �Զ��й�
    afx_msg void OnAutoPlayerOn();
    // ȡ���й�
    afx_msg void OnAutoPlayerOff();
	//����ť
	VOID OnBnClickedSortCard();

	//�����Ϣ
protected:
	//�зְ�ť
	//afx_msg void OnSnatchLand();
	//�зְ�ť
	afx_msg void OnBrightCard();
	//���ƿ�ʼ
	afx_msg void OnBrightStart();	
	/*cxf ������ ����ӱ�
	//�ӱ���Ϣ
	afx_msg void OnDoubleScore();
	//�ӱ���Ϣ
	afx_msg void OnNotDoubleScore();	
	*/
	//����Ϣ
	afx_msg void OnDoubleScoreDao();
	//����Ϣ
	afx_msg void OnDoubleScoreGen();
	//����Ϣ
	afx_msg void OnDoubleScoreFan();
	//cxf end 
	afx_msg void OnLastRound();//lq

	DECLARE_MESSAGE_MAP()

	//osc 2011-10-22
public:
  void InitializeAction();
	
private:
	bool	m_bIsInitBT;
	bool    m_bThreeIsInitBT;
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

//////////////////////////////////////////////////////////////////////////
