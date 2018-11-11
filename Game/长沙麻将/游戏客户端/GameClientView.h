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

 class CGameClientDlg;

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ
class CGameClientView : public CGameFrameView
{
	//��־����
protected:
	bool							m_bOutCard;							//���Ʊ�־
	bool							m_bWaitOther;						//�ȴ���־
	BYTE							m_cbHaiDiCard;						//�����˿�
	bool							m_bTrustee[GAME_PLAYER];			//�Ƿ��й�
	

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
	CPoint							m_UserFlagPos[4];					//��־λ�� ������
	CPoint							m_UserBankerFlagPos[4];				//��־λ�� ׯ
	CPoint							m_PointTrustee[GAME_PLAYER];		//�й�λ��
	CPoint							m_Class[GAME_PLAYER];		        //�ȼ�λ��
	CPoint							m_Gold[GAME_PLAYER];		        //��ʯλ��
	CPoint                          m_ptBirdPos[GAME_PLAYER];           //����λ��

	CPoint                          m_Info[3];                          //��Ϣ��λ��(0:�Լ�; 1:�Ҽ�; 2: ���)

	

	//λ�ñ���
	POINT								m_ptActionDice;					//���Ӷ�����ʼλ
	POINT								m_ptActionDice2;				//���Ӷ���
	CSize								m_csActionDice1;				//���Ӷ�����С
	CSize								m_csActionDice2;				//���Ӵ�С


public:
	//���ݱ���
	int									m_nDiceCount1;					//������1
	int									m_nDiceCount2;					//������2
	bool                                m_bIsPlayBird;                  //�Ƿ񲥷����񶯻�
	BYTE                                m_cbBirdCardData;               //����

	//λͼ����
protected:
	CPngImage						m_ImageBack;						//����ͼ��
	CPngImage                       m_ImageDirectionBack;               //��λ��־����
	CPngImage						m_ImageUserFlag;					//��λ��־
	CPngImage						m_ImageUserBankerFlag;				//ׯ�ұ�־
	CPngImage						m_ImageOutCardBack;					//���Ʊ���

	CPngImage						m_ImageHuangZhuang;					//��ׯ��־
	CPngImage						m_ImageTrustee;						//�йܱ�־	
	CPngImage						m_ImageOutCard;						//������ʾ
	CPngImage						m_ImageWait;						//�ȴ���ʾ
	CPngImage                       m_ImageMPEffect;                    //ɸ�Ӷ���
	CPngImage                       m_ImageMPBACK;                      //���ƶ���
	CPngImage                       m_ImageMPCARD;                      //��������

	CPngImage                       m_ImageUserInfo[GAME_PLAYER];       //���������Ϣ��
	CPngImage                       m_ImageUserGradeNum;                //�ȼ�����ͼƬ

	CPngImage						m_ActionDice1;					    //���Ӷ���
	CPngImage						m_ActionDice2;					    //���Ӷ�������

	CPngImage                        m_ActionBird[GAME_PLAYER];         //���񶯻�

	
	//���Ӻ����񶯻�
protected:
	bool							m_bDiceEffect;						//����Ч��
	bool							m_bBirdEffect;						//��ʼЧ��
	BYTE							m_cbFrameIndex;					    //֡������
	int   							m_tPlayBirdIndex;					//����Ч��
	int                             m_bViewBirdID;                      //�������

   

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
	bool                            m_bIsHuangZhuang;                   //�Ƿ��ׯ
	


	//�ؼ�����
public:
	CSkinButton					    m_btStart;							//��ʼ��ť
	CSkinButton						m_btAutoPlayOn;
	CSkinButton						m_btAutoPlayOff;                    //ȡ���й�
	CSkinButton						m_btHaiDi;							//���װ�ť
	CSkinButton						m_btNoHaiDi;						//������ť
	CControlWnd						m_ControlWnd;						//���ƴ���


	//�ؼ�����
public:
	//CFlash      					m_FlashControl;						//������ͼ


private:
	CGameClientDlg*					m_pGameClientDlg;
    CDlgAIPrompt                    m_CardHuPaiList;                    //������ʾ��
	CCardControl                    m_CardControl;                      //�ƿؼ�
	CGameFlash						m_GameOperationFlash;				//��������
	CGameFlash                      m_ChiHuKindFlash;                   //���ƶ���

	int								m_iSaveCheckPaiIndex;
	CPoint                          m_ptChiHuFlash[GAME_PLAYER];       //���ƶ���λ��
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
	void SetGameClientDlgPtr(CGameClientDlg* pGameClient);

	//���ܺ���
public:
	//��ȡͼƬ·��
	void GetPicPath(CString& strPath);
	//��������
	void SetCellScore(LONGLONG lCellScore);
	//�����˿�
	void SetHaiDiCard(BYTE cbHaiDiCard);
	//ׯ���û�
	void SetBankerUser(WORD wBankerUser);
	//״̬��־
	void SetStatusFlag(bool bOutCard, bool bWaitOther);
	//������Ϣ
	void SetOutCardInfo(WORD wViewChairID, BYTE cbCardData);
	//������Ϣ
	void SetUserAction(WORD wViewChairID, BYTE bUserAction);
	//�����й�
	void SetTrustee(WORD wTrusteeUser,bool bTrustee);
	
	//��ͼ����
	//��Ч����
	void ActionEffect(CDC *pDC,int nWidth, int nHeight,int nFrame);
	//���񶯻�
	void ActionPlayBird(CDC *pDC,int nFrame,BYTE byViewID, CPoint pt, BYTE cbBirdCardData);
	////��ʼ����
	//void ActionStart( CDC *pDC,int nFrame);
	//���Ӷ���
	void ActionDice1( CDC *pDC,int nFrame);
	//���Ӷ���2
	void ActionDice2( CDC *pDC,int pos1, int pos2 );
	//���񶯻�
	bool SetBirdEffect(bool bBirdEffect,BYTE bViewID);
	//����Ч��
	bool SetDiceEffect(bool bDiceEffect);
    //��������
	void PlayOperationFlash(BYTE byViewID);
	//���ƶ���
	void PlayChiHuFlash(BYTE byViewID);


	void InitViewFrame();


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
	//���װ�ť
	afx_msg void OnHaiDi();
	//������ť
	afx_msg void OnNoHaiDi();
	//�Ϲܿ���
	afx_msg void OnStusteeControl();
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
};

//////////////////////////////////////////////////////////////////////////
