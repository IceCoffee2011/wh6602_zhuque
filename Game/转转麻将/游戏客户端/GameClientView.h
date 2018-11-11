#pragma once

#include "Stdafx.h"
#include "ControlWnd.h"
#include "CardControl.h"
#include "GameScoreWnd.h"
#include "DlgAIPrompt.h"
#include "..\..\������\Include\GameFlash.h"

//////////////////////////////////////////////////////////////////////////
//��Ϣ����

#define IDM_START					(WM_USER+100)						//��ʼ��Ϣ
#define IDM_OUT_CARD				(WM_USER+101)						//������Ϣ
#define IDM_OPERATE_HAI_DI			(WM_USER+102)						//������Ϣ
#define IDM_TRUSTEE_CONTROL			(WM_USER+103)						//�йܿ���
#define IDM_SHOW_SZ                 (WM_USER+104)                       //������Ϣ
#define IDM_TING_CARD               (WM_USER+105)                       //������Ϣ
#define IDM_MOPAI                   (WM_USER+106)                       //������Ϣ
#define IDM_SZSTART                 (WM_USER+107)                       //����ɸ�Ӱ�ť��Ϣ


#define IDM_SZ_BUTTON				(WM_USER+110)						//ɫ�Ӱ�ť��Ϣ





//��λ������֡��
#define I_DIRECT_FLASH_TOTOL_NUMS		16
#define I_DIRECT_FLASH_WIDTH			630
#define I_DIRECT_FLASH_HEIGHT			390

//ɫ�Ӷ�����֡��
#define I_SZ_FLASH_TOTOL_NUMS		4
#define I_SZ_FLASH_WIDTH			197
#define I_SZ_FLASH_HEIGHT			264




 class CGameClientDlg;

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��־����
public:
	bool							m_bTrustee[GAME_PLAYER];			//�Ƿ��й�
protected:
	//bool							m_bOutCard;							//���Ʊ�־
	//bool							m_bWaitOther;						//�ȴ���־
	BYTE							m_cbHaiDiCard;						//�����˿�	
	bool                            m_bIsInitBT;                        //��ť��ʼ��
	
	//��Ϸ����
protected:
	LONGLONG						m_lCellScore;						//��������
	WORD							m_wBankerUser;						//ׯ���û�

	//�û�״̬
protected:
	BYTE							m_cbCardData;						//�����˿�
	WORD							m_wOutCardUser;						//�����û�
	BYTE							m_cbUserAction[4];					//�û�����

public:
	WORD                            m_wChiHuKind[GAME_PLAYER];          //�Ժ�����
	WORD                            m_wChiHuRight[GAME_PLAYER];         //����Ȩλ
	WORD                            m_wChiHuType;                       //��Ϸ��������
	int                             m_iChiHuNums;                       //������Ҹ���

	//λ�ñ���
protected:
	CPoint							m_UserBankerFlagPos[4];				//��־λ�� ׯ
	CPoint							m_PointTrustee[GAME_PLAYER];		//�й�λ��
	CPoint							m_ptLastMahjong[GAME_PLAYER];		//����齫λ��
	CPoint                          m_UserDuanXian[GAME_PLAYER];        //��Ҷ���
	CPoint                          m_Info[3];                          //��Ϣ��λ��(0:�Լ�; 1:�Ҽ�; 2: ���)

	POINT								m_ptActionDice;					//���Ӷ�����ʼλ
	POINT								m_ptActionDice2;				//���Ӷ���
	//λ�ñ���
	CSize								m_csActionDice1;				//���Ӷ�����С
	CSize								m_csActionDice2;				//���Ӵ�С


public:
	//���ݱ���
	//int                                 m_nKingCardIndex;               //��������
	int									m_nDiceCount1;					//������1
	int									m_nDiceCount2;					//������2

	//λͼ����
protected:
	CSkinImage						m_ImageBack;						//����ͼ��
	CPngImage                       m_ImageDirectionBack;                //��λ��־����DIRECTIONBACK.png
	CPngImage						m_ImageUserFlag;					//��λ��־
	CPngImage						m_ImageUserBankerFlag;				//ׯ�ұ�־
	//CPngImage						m_ImageUserAction;					//�û�����
	//CPngImage						m_ImageActionBack;					//��������
	CPngImage						m_ImageOutCardBack;					//���Ʊ���

	CPngImage						m_ImageHuangZhuang;					//��ׯ��־
	CPngImage						m_ImageTrustee;						//�йܱ�־	
	CPngImage						m_ImageOutCard;						//������ʾ
	//CPngImage						m_ImageWait;						//�ȴ���ʾ
	
	//CPngImage                       m_ImageMPEffect;                    //ɸ�Ӷ���
	//CPngImage                       m_ImageMPBACK;                      //���ƶ���
	//CPngImage                       m_ImageMPCARD;                      //��������

	CPngImage                       m_ImageUserInfo[GAME_PLAYER];       //���������Ϣ��
	CPngImage                       m_ImageUserGradeNum;                //�ȼ�����ͼƬ 

	CPngImage						m_ActionDice1;					    //���Ӷ���
	CPngImage						m_ActionDice2;					    //���Ӷ�������
	

	
	//���Ӻ����ƶ���
protected:
	bool							m_bDiceEffect;						//����Ч��
	bool							m_bStartEffect;						//��ʼЧ��
	BYTE							m_cbFrameIndex;					    //֡������
	bool							m_bMPEffect;						//����Ч��
   

	//�˿˿ؼ�
public:
	CHeapCard						m_HeapCard[4];						//�����˿�
	CUserCard						m_UserCard[3];						//�û��˿�
	CTableCard						m_TableCard[4];						//�����˿�
	CWeaveCard						m_WeaveCard[4][4];					//����˿�
	CDiscardCard					m_DiscardCard[4];					//�����˿�
	CCardControl					m_HandCardControl;					//�����˿�

	int								m_tDice;						    //���Ӷ���ʱ��
	int                             m_tStart;                           //��ʼ����ʱ��
	bool							m_bTingFlag[GAME_PLAYER];			//���Ʊ�־
	bool                            m_bIsHuangZhuang;                   //�Ƿ��ׯ


	//�ؼ�����
public:
	CSkinButton					    m_btStart;							//��ʼ��ť
	CSkinButton						m_btStusteeControl;					//�Ϲܿ���
	CSkinButton						m_btAutoPlayOn;
	CSkinButton						m_btAutoPlayOff;                    //ȡ���й�
	CSkinButton                     m_btSortCard;                       //����ť
	//CSkinButton						m_ControlWnd;						//���ƴ���
	//CSkinButton						m_btStusteeControl;					//�Ϲܿ���
	//CSkinButton					    m_btTingCard;					    //���ư�ť
	//CSkinButton                     m_btBuTing;                         //������ť
	//CSkinButton						m_btHaiDi;							//���װ�ť
	//CSkinButton						m_btNoHaiDi;						//������ť
	CControlWnd						m_ControlWnd;						//���ƴ���


private:
	CGameClientDlg*					m_pGameClientDlg;
    CDlgAIPrompt                    m_CardHuPaiList;                    //������ʾ��
	CCardControl                    m_CardControl;                      //�ƿؼ�


private:
	CGameFlash						m_GameOperationFlash;				//��������
	CGameFlash                      m_OutCardFlash;                     //������ʧ����
	//CGameFlash      				m_TingCardFlash;                    //���ƶ���

	//CGameFlash                      m_ChiHuKindFlashZiMo;                //��������
	//CGameFlash                      m_ChiHuKindFlashDianPao;             //���ڶ���
	//CGameFlash                      m_ChiHuKindFlashFangPao;             //���ڶ���
	//CGameFlash                      m_ChiHuKindFlashTongPao;             //ͨ�ڶ���
	//CGameFlash                      m_ChiHuKindFlashHaidi;               //����
	//CGameFlash                      m_ChiHuKindFlashGang;                //���Ͽ���

	// ���
	CThreadAnimation				* m_pTAFlashMuscle;								///< ��
	CPngImage						m_ImageGoldChar;								///< ����ַ�
	LONGLONG						m_llGoldMuscle[GAME_PLAYER];				///< ����
	BYTE									m_byShowMuscle;										///< ����ʾ
	CImageNums						m_INMuscleNumGray;							///< ���ɫ���ֿؼ�
	CImageNums						m_INMuscleNumGreen;							///< ���ɫ���ֿؼ�
	CRect								m_rcMuscleNum[GAME_PLAYER];				///< ���λ��

	//CGameFlash                      m_ChiHuKindFlashPengPeng;            //������
	//CGameFlash                      m_ChiHuKindFlashQingYiSe;            //��һɫ
	//CGameFlash                      m_ChiHuKindFlashQiXiaoDui;           //��С��
	//CGameFlash                      m_ChiHuKindFlashJiangJiang;          //������
	//CGameFlash                      m_HuangZhuangFlash;                  //��ׯ

	int								m_iSaveCheckPaiIndex;

	CImageNums						m_TestNums;
	CFont							m_fontDrawGameInfo;						//��������
private:
	//���涯������
	bool							m_bIsShowOutCardFlash;					//�Ƿ���ʾ���б���
	CRect							m_rcShowDrawArea;



public:
	bool							m_bIsOpenSZFlash;						//�Ƿ��ɫ�Ӷ���
	int								m_iCountSZFCurIndex;					//���㵱ǰɫ�����
	CRect							m_rcSZFlashDraw;						//ɫ�Ӷ�������
	CRect							m_rcSZBTRect;							//ɫ�Ӱ�ť��Ч����
	CPngImage                       m_ActionSZFlash;       

	bool							m_bIsPlayDirect;						//�Ƿ�򿪶�ׯ����
	int								m_iCountDirectIndex;					//���㵱ǰ��ׯ���
	CRect							m_rcDirectFlashDraw;					//��ׯ��������
	CPngImage                       m_ActionDirectFlash;  



private:
	CPoint							m_ptOperationFlash[GAME_PLAYER];     //��������
	//CPoint                          m_ptChiHuFlash[GAME_PLAYER];       //���ƶ���λ��
	//��������
public:
	void TestClient(int iType);
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
	// ��ȫ��ʾ�����ش���
	void SafeShowOrHideWnd(CWnd* pWnd, int nCmdShow);
public:
	void SetGameClientDlgPtr(CGameClientDlg* pGameClient);

	//���ܺ���
public:
	//��ȡ�齫·��
	void GetMahjonPath(CString& strPath);
	//��ȡͼƬ·��
	void GetPicPath(CString& strPath);
	//��������
	void SetCellScore(LONGLONG lCellScore);
	// ��ȡ��������
	LONGLONG GetCellScore();
	//�����˿�
	void SetHaiDiCard(BYTE cbHaiDiCard);
	//ׯ���û�
	void SetBankerUser(WORD wBankerUser);
	////����״̬
	//void SetStatusTingFlag(WORD wViewChairID, bool cbHearStatus);
	//״̬��־
	//void SetStatusFlag(bool bOutCard, bool bWaitOther);
	//������Ϣ
	void SetOutCardInfo(WORD wViewChairID, BYTE cbCardData);
	//������Ϣ
	void SetUserAction(WORD wViewChairID, BYTE bUserAction);
	//�����й�
	void SetTrustee(WORD wTrusteeUser,bool bTrustee);

	//
	void CreateGameBT();
	//��ť������Դ
	void  LoadRes(CGdipButton* pBT, CString strValue);
	
	//��ͼ����
	//��Ч����
	void ActionEffect(CDC *pDC,int nWidth, int nHeight,int nFrame);
	////��ʼ����
	//void ActionStart( CDC *pDC,int nFrame);
	//���Ӷ���
	void ActionDice1( CDC *pDC,int nFrame);
	//���Ӷ���2
	void ActionDice2( CDC *pDC,int pos1, int pos2 );
	//���ƶ���
	void ActionMP( CDC *pDC,int nFrame);
	//����Ч��
	bool SetDiceEffect(bool bDiceEffect);
	//����Ч��
	bool SetMPEffect(bool bDiceEffect);
    //��������
	void PlayOperationFlash(BYTE byViewID,BYTE byUserAction);
	/// ���
	/// [@param byTargetViewID in] Ŀ�� ��ͼID����ע��һ����׬Ǯ��ң�
	/// [@param bySourceViewID in] Դ ��ͼID, 255��������Ŀ������ڵ�������ң���ע��һ������Ǯ��ң�
	/// [@param byUserAction in] ����� 0 ��Чֵ, 1 ���2 ���3 �Ž�
	/// [@param llGold in] �䶯�Ľ��ֵ
	void PlayMuscleFlash(BYTE byTargetViewID, BYTE bySourceViewID, BYTE byUserAction, const LONGLONG llGold[]);
	//���ƶ���
	void PlayChiHuFlash(BYTE byViewID);
	//���ƶ���
	void PlayFangPaoFlash(BYTE byViewID);
	//��λ����
	void PlayDirectFlash(bool bIsPlay=true);
	//����ɫ�Ӷ���
	void PlaySZFlashBT(bool bIsPlay=true);
	//�������ƶ���
	//void PlayTingCardFlash(BYTE byViewID);
	void InitViewFrame();
	/// �����齫
	void OnDrawMahjong(CDC *pDC);
	/// ���Ƹմ���������齫
	void OnDrawLastMahjong(CDC *pDC);
	/// ���ƽ��
	void OnDrawMuscle(CDC *pDC);
public:
	void SetCanHuPaiData(const BYTE* pCardData, int iCardNums, int iX);
	//��������
protected:
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);	
	//��Ϣӳ��
protected:
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);
	//��ʼ��ť
	afx_msg void OnStart();
	////���װ�ť
	//afx_msg void OnHaiDi();
	////������ť
	//afx_msg void OnNoHaiDi();
	//�Ϲܿ���
	afx_msg void OnStusteeControl();
	////���ư�ť
	//afx_msg void OnTingCard();
	////������ť
	//afx_msg void OnNoTingCard();
	// �Զ��й�
	afx_msg void OnAutoPlayerOn();
	// ȡ���й�
	afx_msg void OnAutoPlayerOff();
	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()



public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

//////////////////////////////////////////////////////////////////////////
