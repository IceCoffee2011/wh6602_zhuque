#pragma once

#include "Stdafx.h"
#include "ImageNumsYY.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//������Ϣ
struct tagGameScoreInfo
{
	BYTE							cbBirdCard;							//�����˿�
	BYTE							cbChiHuCard;						//�Ժ��˿�
	BYTE                            cbKingCard;                         //��������

	WORD							wBankerUser;						//ׯ���û�
	WORD							wBirdHitUser;						//�����û�
	WORD							wProvideUser;						//�����û�
		
	LONGLONG				iCellScore;                         //��Ԫ����
	LONGLONG				iBasicScore;						//������

	BYTE                            cbKingCount[GAME_PLAYER];           //������Ŀ
	WORD							wCardType[GAME_PLAYER];				//��������

	
	int                             iChiHuKind[GAME_PLAYER];            //���Ƶ�λ
	LONGLONG								iMuscle[GAME_PLAYER];					// ��
	LONGLONG                             cbTimes[GAME_PLAYER];               //��ҷ���
	LONGLONG						lScore[GAME_PLAYER];				//�û�����
	
	TCHAR							szUserName[GAME_PLAYER][NAME_LEN];	//�û��ʺ�

	tagGameScoreInfo()
	{
		cbBirdCard = 0;
		cbChiHuCard = 0;;
		cbKingCard = 0;
		wBankerUser = 0;
		wBirdHitUser = 0;
		wProvideUser = 0;
		iCellScore = 0;
		iBasicScore = 0;
		memset(cbKingCount, 0, sizeof(cbKingCount));
		memset(wCardType, 0, sizeof(wCardType));
		memset(iMuscle, 0, sizeof(iMuscle));
		memset(cbTimes, 0, sizeof(cbTimes));

		memset(iChiHuKind, 0, sizeof(iChiHuKind));
		memset(lScore, 0, sizeof(lScore));
		memset(szUserName, 0, sizeof(szUserName));
	}
};

//////////////////////////////////////////////////////////////////////////

//���ֱ���
//class CGameScoreWnd : public CWnd
class CGameScoreWnd : public CScoreView
{
	//��������
protected:
	tagGameScoreInfo				m_GameScoreInfo;					//������Ϣ
	CPngImage                       m_Direction;                        //��ҷ�λ��ʶ
	CPngImage                       m_ImageLeft;                        //���ܱ�ʶ
	CPngImage						m_ImageNature;					// ������ʶ
	CPngImage						m_ImageFangPao;					// ���ڱ�ʶ
	CPngImage						m_ImageOneMuscle;						// ���
	CPngImage                       m_ImageNumDown;                     //����ʱ����


	CRect							m_rcDrawTimer;                      //����ʱ����
	WORD                            m_MeChairId;                        //�ҵ����Ӻ�
	WORD                            m_BankerChairID;                     //�������Ӻ�
	bool                            m_IsUserLeft[GAME_PLAYER];            //����Ƿ�ǿ��

	CImageNums						m_NFanNum;									// ����
	CImageNums						m_NScore;										// ����

	//��ť
public:
	CSkinButton                     m_btGoOn;                            //������ť
	CSkinButton                     m_btChangeTable;                     //����
	CSkinButton                     m_btExit;                            //�˳�
	CSkinButton                     m_btOverTime;                        //��ʱ

	//��������
public:
	//���캯��
	CGameScoreWnd();
	//��������
	virtual ~CGameScoreWnd();


	//���ܺ���
public:
	//���û���
	void SetScoreInfo(tagGameScoreInfo GameScoreInfo);
	//����ʱ����
	void SetCountDown(bool Istrue);
	//��ȡ��ͼID
	void GetViewID(WORD MyChairID, WORD BankerChairID);
	//��ȡλ��
	int GetImageIndex(BYTE cbCardData);
	//����ǿ��
	void SetUserLeft(WORD wChairID,bool IsLeft);

public:
	//��ʼ����Ϣ
	virtual void InitScoreViewInfo();
	//��ʼ����Ϣ��
	virtual void AlreadyInitInfo();
	//����
	virtual void DrawScoreInfo(CDC* pDC);
	//��ȡ·��
	void GetPicPath(CString& strPath);
protected:
	//������ť
	afx_msg void OnGoOn();
	//�˳���ť
	afx_msg void OnExit();
	//������ť
	afx_msg void OnChangeTable();
	//��ʱ��ť
	void OnOverTime();

protected:
	//�Ƿ���ʾ����ʱ
	bool   m_IsTrue; 
	BYTE   m_SecondCount;

public:
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP();
};

//////////////////////////////////////////////////////////////////////////
