#pragma once

#include "Stdafx.h"
#include "ImageNumsCS.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//������Ϣ
struct tagGameScoreInfo
{
	BYTE							cbBirdCard;							//�����˿�
	BYTE							cbChiHuCard;						//�Ժ��˿�
	WORD							wBankerUser;						//ׯ���û�
	WORD							wBirdHitUser;						//�����û�
	WORD							wProvideUser;						//�����û�
	LONGLONG						lScore[GAME_PLAYER];				//�û�����
	WORD							wCardType[GAME_PLAYER];				//��������
	TCHAR							szUserName[GAME_PLAYER][NAME_LEN];	//�û��ʺ�

	int                             iCellScore;                         //��Ԫ����
	int                             cbTimes[GAME_PLAYER];               //��ҷ���
	int                             iChiHuKind[GAME_PLAYER];            //���Ƶ�λ
};

//////////////////////////////////////////////////////////////////////////

//���ֱ���
//class CGameScoreWnd : public CWnd
class CGameScoreWnd : public CScoreView
{
	//��������
protected:
	tagGameScoreInfo				m_GameScoreInfo;					//������Ϣ
	//CPngImage						m_ImageEndTag;						//ʤ����־
	CPngImage                       m_Banker;                           //ׯ�ұ�ʶ
	CPngImage                       m_Direction;                        //��ҷ�λ��ʶ
	//CPngImage                       m_ImageBirdCount;                   //���Ƹ���
	CPngImage                       m_ImageEndBirdCard;                 //���������б�
	CPngImage                       m_ImageWin;                         //ʤ����ʶ
	CPngImage                       m_ImageLost;                        //ʧ�ܱ�ʶ
	CPngImage                       m_ImageBirdTag;                     //�����ʶ
	CPngImage                       m_ImageChiHuDirection;              //���������ʾ
	CPngImage                       m_ImageNumDown;                     //����ʱ����


	CRect							m_rcDrawTimer;                      //����ʱ����
	WORD                            m_MeChairId;                        //�ҵ����Ӻ�
	WORD                            m_BankerChairID;                    //�������Ӻ�
	bool                            m_IsUserLeft[GAME_PLAYER];           //����Ƿ�ǿ��
	CImageNumsCS                    ImageNums1,ImageNums2;             //����ͼƬ

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
