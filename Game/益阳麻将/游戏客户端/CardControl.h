#ifndef CARD_CONTROL_HEAD_FILE
#define CARD_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GameLogic.h"
#include "cassert"

//////////////////////////////////////////////////////////////////////////
//	�˿˶���

//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//����
//	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//����
//	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//ͬ��
//	0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//����
//	0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,							//����

//////////////////////////////////////////////////////////////////////////

//�˿˶���
#define HEAP_FULL_COUNT				34									//����ȫ��

//////////////////////////////////////////////////////////////////////////

//X ���з�ʽ
enum enXCollocateMode 
{ 
	enXLeft,						//�����
	enXCenter,						//�ж���
	enXRight,						//�Ҷ���
};

//Y ���з�ʽ
enum enYCollocateMode 
{ 
	enYTop,							//�϶���
	enYCenter,						//�ж���
	enYBottom,						//�¶���
};

//����ö��
enum enDirection
{
	Direction_East,					//����
	Direction_South,				//����
	Direction_West,					//����
	Direction_North,				//����
};

//////////////////////////////////////////////////////////////////////////

//�˿�ͼƬ
class CCardListImage
{
	//λ�ñ���
protected:
	int								m_nItemWidth;						//����߶�
	int								m_nItemHeight;						//������
	int								m_nViewWidth;						//����߶�
	int								m_nViewHeight;						//������

	//��Դ����
protected:
	CPngImage						m_CardListImage;					//ͼƬ��Դ

	//��������
public:
	//���캯��
	CCardListImage();
	//��������
	virtual ~CCardListImage();

	//��Ϣ����
public:
	//��ȡ���
	int GetViewWidth() { return m_nViewWidth; }
	//��ȡ�߶�
	int GetViewHeight() { return m_nViewHeight; }

	//��Դ����
public:
	//������Դ
	bool LoadResource(char* pFileName, int nViewWidth, int nViewHeight);
	//�ͷ���Դ
	bool DestroyResource();

	//���ܺ���
public:
	//��ȡλ��
	inline int GetImageIndex(BYTE cbCardData);
	//�滭�˿�
	inline bool DrawCardItem(CDC * pDestDC, BYTE cbCardData, int xDest, int yDest);
};

//////////////////////////////////////////////////////////////////////////

//�˿���Դ
class CCardResource
{
	//�û��˿�
public:
    //CCardListImage				 m_ImageUserBottom;					//ͼƬ��Դ
	//CCardListImage                  m_ImageKingBottom;                  //ͼƬ��Դ

	////PNG
	//CPngImage						m_ImageUserTop;						//ͼƬ��Դ
	//CPngImage						m_ImageUserLeft;					//ͼƬ��Դ
	//CPngImage						m_ImageUserRight;					//ͼƬ��Դ
	//CCardListImage				    m_ImageMyselfCardNum;				//��������

	

	//�����˿�
public:
	//CCardListImage					m_ImageTableTop;					//ͼƬ��Դ
	//CCardListImage					m_ImageTableLeft;					//ͼƬ��Դ
	//CCardListImage					m_ImageTableRight;					//ͼƬ��Դ
	//CCardListImage					m_ImageTableBottom;					//ͼƬ��Դ
	//CCardListImage                    m_ImageOutCardNum;                  //��������

	//�ƶ��˿�
public:
	//CPngImage						m_ImageBackH;						//ͼƬ��Դ
	//CPngImage						m_ImageBackV;						//ͼƬ��Դ
	//CPngImage						m_ImageHeapSingleV;					//ͼƬ��Դ
	//CPngImage						m_ImageHeapSingleH;					//ͼƬ��Դ
	//CPngImage						m_ImageHeapDoubleV;					//ͼƬ��Դ
	//CPngImage						m_ImageHeapDoubleH;					//ͼƬ��Դ
	//CPngImage						m_ImageHeapTripleV;				    //ͼƬ��Դ
	//CPngImage						m_ImageHeapTripleH;				    //ͼƬ��Դ

	

	//��������
public:
	//���캯��
	CCardResource();
	//��������
	virtual ~CCardResource();

	//���ܺ���
public:
	//������Դ
	bool LoadResource();
	//������Դ
	bool DestroyResource();
};

//////////////////////////////////////////////////////////////////////////

//�����˿�
class CHeapCard
{
	//���Ʊ���
protected:
	CPoint							m_ControlPoint;						//��׼λ��
	enDirection						m_CardDirection;					//�˿˷���

	CPngImage                       m_ImageHeapDoubleVL;                //ͼƬ��Դ
	CPngImage						m_ImageHeapDoubleVR;				//ͼƬ��Դ
	CPngImage						m_ImageHeapDoubleHT;				//ͼƬ��Դ
	CPngImage						m_ImageHeapDoubleHB;				//ͼƬ��Դ

	CPngImage                       m_ImageHeapSingleVL;                //ͼƬ��Դ
	CPngImage						m_ImageHeapSingleVR;				//ͼƬ��Դ
	CPngImage						m_ImageHeapSingleHT;				//ͼƬ��Դ
	CPngImage						m_ImageHeapSingleHB;			    //ͼƬ��Դ

	//�˿˱���
protected:
	WORD							m_wFullCount;						//ȫ����Ŀ
	WORD							m_wMinusHeadCount;					//ͷ����ȱ
	WORD							m_wMinusLastCount;					//β����ȱ

	//��������
public:
	//���캯��
	CHeapCard();
	//��������
	virtual ~CHeapCard();

	//���ܺ���
public:
	//�滭�˿�
	void DrawCardControl(CDC * pDC);
	//�����˿�
	bool SetCardData(WORD wMinusHeadCount, WORD wMinusLastCount, WORD wFullCount);

	//�ؼ�����
public:
	//���÷���
	void SetDirection(enDirection Direction) { m_CardDirection=Direction; }
	//��׼λ��
	void SetControlPoint(int nXPos, int nYPos) { m_ControlPoint.SetPoint(nXPos,nYPos); }
};

//////////////////////////////////////////////////////////////////////////

//����˿�
class CWeaveCard
{
	//���Ʊ���
protected:
	bool							m_bDisplayItem;						//��ʾ��־
	CPoint							m_ControlPoint;						//��׼λ��
	enDirection						m_CardDirection;					//�˿˷���
	BYTE							m_cbDirectionCardPos;				//�����˿�		
	int                             m_iIndex;                          //���������

	//�˿�����
protected:
	WORD							m_wCardCount;						//�˿���Ŀ
	BYTE							m_cbCardData[4];					//�˿�����


	//ͼƬ��Դ
	CPngImage                       m_ImageUserWeaveCardVL;             //ͼƬ��Դ
	CPngImage						m_ImageUserWeaveCardVR;				//ͼƬ��Դ
	CPngImage						m_ImageUserWeaveCardHT;				//ͼƬ��Դ
	CPngImage						m_ImageUserWeaveCardHB;			    //ͼƬ��Դ

	CPngImage                       m_ImageWeaveCardNumHT;
	CPngImage                       m_ImageWeaveCardNumHB;
	CPngImage                       m_ImageWeaveCardNumVL;
	CPngImage                       m_ImageWeaveCardNumVR;

	CPngImage                       m_ImageWeaveCardBackVL;
	CPngImage                       m_ImageWeaveCardBackVR;
	CPngImage                       m_ImageWeaveCardBackHB;
	CPngImage                       m_ImageWeaveCardBackHT;
	


	//��������
public:
	//���캯��
	CWeaveCard();
	//��������
	virtual ~CWeaveCard();

	//���ܺ���
public:
	//�滭�˿�
	void DrawCardControl(CDC * pDC);
	//�滭�˿�
	//void DrawCardControl(CDC * pDC, int nXPos, int nYPos);
	//�����˿�
	bool SetCardData(const BYTE cbCardData[], WORD wCardCount);
	//��ȡλ��
	int CWeaveCard::GetImageIndex(BYTE cbCardData);

	//�ؼ�����
public:
	//������ʾ
	void SetDisplayItem(bool bDisplayItem) { m_bDisplayItem=bDisplayItem; }
	//���÷���
	void SetDirection(enDirection Direction) { m_CardDirection=Direction; }
	//��׼λ��
	void SetControlPoint(int index, int nXPos, int nYPos) {m_iIndex = index;  m_ControlPoint.SetPoint(nXPos,nYPos); }
	//������
	void SetDirectionCardPos(BYTE cbPos) 
	{ assert(cbPos <= 3); m_cbDirectionCardPos = cbPos;}


	//�ڲ�����
private:
	//��ȡ�˿�
	inline BYTE GetCardData(WORD wIndex);
};

//////////////////////////////////////////////////////////////////////////

//�û��˿�
class CUserCard
{
	//�˿�����
protected:
	WORD							m_wCardCount;						//�˿���Ŀ
	bool							m_bCurrentCard;						//��ǰ��־

	//���Ʊ���
protected:
	CPoint							m_ControlPoint;						//��׼λ��
	enDirection						m_CardDirection;					//�˿˷���

	//ͼƬ��Դ
	CPngImage                       m_ImageUserHandVL;                  //ͼƬ��Դ
	CPngImage						m_ImageUserHandVR;				    //ͼƬ��Դ
	CPngImage						m_ImageUserHandHT;			    	//ͼƬ��Դ

	//��������
public:
	//���캯��
	CUserCard();
	//��������
	virtual ~CUserCard();

	//���ܺ���
public:
	//�滭�˿�
	void DrawCardControl(CDC * pDC);
	//�����˿�
	bool SetCurrentCard(bool bCurrentCard);
	//�����˿�
	bool SetCardData(WORD wCardCount, bool bCurrentCard);

	//�ؼ�����
public:
	//���÷���
	void SetDirection(enDirection Direction) { m_CardDirection=Direction; }
	//��׼λ��
	void SetControlPoint(int nXPos, int nYPos) { m_ControlPoint.SetPoint(nXPos,nYPos); }
};

//////////////////////////////////////////////////////////////////////////

//�����˿�
class CDiscardCard
{
	//�˿�����
protected:
	WORD							m_wCardCount;						//�˿���Ŀ
	BYTE							m_cbCardData[22];					//�˿�����

	//���Ʊ���
protected:
	CPoint							m_ControlPoint;						//��׼λ��
	enDirection						m_CardDirection;					//�˿˷���

	//ͼƬ��Դ
	CPngImage                       m_ImageUserDisCardVL;               //ͼƬ��Դ
	CPngImage						m_ImageUserDisCardVR;				//ͼƬ��Դ
	CPngImage						m_ImageUserDisCardHT;				//ͼƬ��Դ
	CPngImage						m_ImageUserDisCardHB;			    //ͼƬ��Դ
	CPngImage						m_ImageUserDisCardNumHB;			//ͼƬ��Դ


	CPngImage                       m_ImageDisCardNumVL; 
	CPngImage                       m_ImageDisCardNumVR;
	CPngImage                       m_ImageDisCardNumHT;
	CPngImage                       m_ImageDisCardNumHB;


	//��������
public:
	//���캯��
	CDiscardCard();
	//��������
	virtual ~CDiscardCard();

	//���ܺ���
public:
	//�滭�˿�
	void DrawCardControl(CDC * pDC);
	//�����˿�
	bool AddCardItem(BYTE cbCardData);
	//�����˿�
	bool SetCardData(const BYTE cbCardData[], WORD wCardCount);
	//��ȡλ��
	//CPoint GetLastCardPosition();

	//��ȡλ��
	inline int GetImageIndex(BYTE cbCardData);




	//�ؼ�����
public:
	//���÷���
	void SetDirection(enDirection Direction) { m_CardDirection=Direction; }
	//��׼λ��
	void SetControlPoint(int nXPos, int nYPos) { m_ControlPoint.SetPoint(nXPos,nYPos); }
};

//////////////////////////////////////////////////////////////////////////

//�����˿�
class CTableCard
{
	//�˿�����
protected:
	WORD							m_wCardCount;						//�˿���Ŀ
	BYTE							m_cbCardData[14];					//�˿�����

	//���Ʊ���
protected:
	CPoint							m_ControlPoint;						//��׼λ��
	enDirection						m_CardDirection;					//�˿˷���

	//ͼƬ��Դ
	CPngImage                       m_ImageUserTableCardVL;             //ͼƬ��Դ
	CPngImage						m_ImageUserTableCardVR;				//ͼƬ��Դ
	CPngImage						m_ImageUserTableCardHT;				//ͼƬ��Դ
	CPngImage						m_ImageUserTableCardHB;			    //ͼƬ��Դ


	CPngImage                       m_ImageTabCardNumVL; 
	CPngImage                       m_ImageTabCardNumVR;
	CPngImage                       m_ImageTabCardNumHT;
	CPngImage                       m_ImageTabCardNumHB;

	//��������
public:
	//���캯��
	CTableCard();
	//��������
	virtual ~CTableCard();

	//���ܺ���
public:
	//�滭�˿�
	void DrawCardControl(CDC * pDC);
	//�����˿�
	bool SetCardData(const BYTE cbCardData[], WORD wCardCount);

	//��ȡλ��
	inline int GetImageIndex(BYTE cbCardData);

	//�ؼ�����
public:
	//���÷���
	void SetDirection(enDirection Direction) { m_CardDirection=Direction; }
	//��׼λ��
	void SetControlPoint(int nXPos, int nYPos) { m_ControlPoint.SetPoint(nXPos,nYPos); }
};

//////////////////////////////////////////////////////////////////////////
//�����˿�
class CKingCard
{
	//�˿�����
protected:
	WORD							m_wCardCount;						//�˿���Ŀ
	BYTE							m_cbCardData[4];					   //�˿�����

	//���Ʊ���
protected:
	CPoint							m_ControlPoint;						//��׼λ��
	enDirection						m_CardDirection;					//�˿˷���

	//ͼƬ��Դ
	CPngImage                       m_ImageUserKingCardBack;            //ͼƬ��Դ
	CPngImage						m_ImageUserKingCardNum;				//ͼƬ��Դ


	//��������
public:
	//���캯��
	CKingCard();
	//��������
	virtual ~CKingCard();

	//���ܺ���
public:
	//�滭�˿�
	void DrawCardControl(CDC * pDC);
	//�����˿�
	bool SetCardData(const BYTE cbCardData[], WORD wCardCount);
	//��ȡλ��
	inline int GetImageIndex(BYTE cbCardData);

	//�ؼ�����
public:
	//���÷���
	void SetDirection(enDirection Direction) { m_CardDirection=Direction; }
	//��׼λ��
	void SetControlPoint(int nXPos, int nYPos) { m_ControlPoint.SetPoint(nXPos,nYPos); }
};




//////////////////////////////////////////////////////////////////////////


//�˿˽ṹ
struct tagCardItem
{
	bool							bShoot;								//�����־
	BYTE							cbCardData;							//�˿�����
	bool                            bKingFlag;                          //���Ʊ��
};

//�˿˿ؼ�
class CCardControl
{
	//״̬����
protected:
	bool							m_bPositively;						//��Ӧ��־
	bool							m_bDisplayItem;						//��ʾ��־


public:
	CPngImage					    m_ImageUserBottom;					//ͼƬ��Դ
	CPngImage                       m_ImageKingBottom;                  //ͼƬ��Դ
	CPngImage                       m_ImageOutCardNum;                       
	

	//λ�ñ���
protected:
	CPoint							m_BenchmarkPos;						//��׼λ��
	enXCollocateMode				m_XCollocateMode;					//��ʾģʽ
	enYCollocateMode				m_YCollocateMode;					//��ʾģʽ

	//�˿�����
protected:
	WORD							m_wCardCount;						//�˿���Ŀ
	WORD							m_wHoverItem;						//��������
	tagCardItem						m_CurrentCard;						//��ǰ�˿�
	tagCardItem						m_CardItemArray[13];				//�˿�����

	//��Դ����
protected:
	CSize							m_ControlSize;						//�ؼ���С
	CPoint							m_ControlPoint;						//�ؼ�λ��

	//��������
public:
	//���캯��
	CCardControl();
	//��������
	virtual ~CCardControl();

	//��ѯ����
public:
	//��ȡ�˿�
	BYTE GetHoverCard();
	//��ȡ�˿�
	BYTE GetCurrentCard() { return m_CurrentCard.cbCardData; }
	//��ȡ�˿�
	bool GetHoverCardItem(tagCardItem &CardItem);

	//�˿˿���
public:
	//�����˿�
	bool SetCurrentCard(BYTE cbCardData);
	//�����˿�
	bool SetCurrentCard(tagCardItem CardItem);
	//�����˿�
	bool SetCardData(const BYTE cbCardData[], WORD wCardCount, BYTE cbCurrentCard,BYTE cbKingCard);
	//�����˿�
	bool SetCardItem(const tagCardItem CardItemArray[], WORD wCardCount);

	//�ؼ�����
public:
	//������Ӧ
	void SetPositively(bool bPositively) { m_bPositively=bPositively; }
	//������ʾ
	void SetDisplayItem(bool bDisplayItem) { m_bDisplayItem=bDisplayItem; }
	//��׼λ��
	void SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);
	//��׼λ��
	void SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);

	//�¼�����
public:
	//�滭�˿�
	void DrawCardControl(CDC * pDC);
	//�����Ϣ
	bool OnEventSetCursor(CPoint Point, bool & bRePaint);

//private:
//    CGameClientDlg*                m_pGameClientDlg;



//private:
//	CGameClientDlg*					m_pGameClientDlg;

public:
	//��õ�ǰ����
	void GetCurCardDataAndX(BYTE& btCardData, int& iX);
	//��ʼ������
	void InitCurCardDataX();
	//�������
	int CheckIndex(CPoint Point);
	//��ȡλ��
	inline int GetImageIndex(BYTE cbCardData);

	//�ڲ�����
private:
	//�����л�
	WORD SwitchCardPoint(CPoint & MousePoint);


private:
	//����������
	BYTE	m_btCurCardData;
	int		m_iCurCardX;
	BYTE	m_iCardIndex;
};


//////////////////////////////////////////////////////////////////////////

//��������
//��������
//extern CCardResource				g_CardResource;						//�˿���Դ

//////////////////////////////////////////////////////////////////////////

#endif