#pragma once


#include "Stdafx.h"
#include <vector>

#include "ThreadGuard.h"

using namespace std;

//��ֵ����
#define	CARD_MASK_COLOR				0xF0								//��ɫ����
#define	CARD_MASK_VALUE				0x0F								//��ֵ����


//X ���з�ʽ
enum enGameCardXCollocateMode 
{ 
	enGCXLeft,						//�����
	enGCXCenter,					//�ж���
	enGCXRight,						//�Ҷ���
};

//�˿˽ṹ
struct tagGameCardItem
{
	bool							bShoot;								//�����־
	BYTE							bCardData;							//�˿�����
};

//��Ϸ�ƿؼ�
class CGameCard
{
public:
	CGameCard();
	~CGameCard(void);

public:
	//������ͼ����ָ��
	void SetViewWndPtr(CWnd* pViewWnd);
	
public:
	//���������
	int SetCardData(const BYTE* pCardData=NULL, int iSize=0);
	//���������
	int GetCardData(BYTE* pCardData, int iSize);
	//��õ������
	int GetShootCard(BYTE* pCardShootCard, int iSize);
	//���õ������
	int SetShootCard(BYTE* pCardShootCard, int iSize);
	//���������Ʋ�����
	void SetAllShoot(bool bIsShott=false);

public:
	//������Ӧ
	void SetCardPositively(bool bIsPositively=true);
	//������ʾ
	void SetCardShow(bool bIsShow=true);


public:
	//�����л�
	int SwitchCardPoint(const CPoint &Point);
	//�����Ϣ
	void OnLButtonDown(CPoint Point);
	//�����Ϣ
	void OnLButtonUp(CPoint Point);
	//����ƶ�
	void OnMouseMove(CPoint point);

public:
	//�����Ʊ���
	void SetCardBackImage(CPngImage* pBackImage);
	//��������Դ
	bool SetCardImage(CString strImagePath);
	//������ѡ��
	bool SetCardSelImage(CString strSelImagePath);
	//�����Ƽ��
	void SetCardSpace(int iCardSpace);
	//������λ��
	void SetCardPosition(CPoint ptPosition, enGameCardXCollocateMode CardMode=enGCXCenter);
	//����ƿ����
	int	 GetCardControlWidth();
	int	 GetCardControlHeight();
	//��ȡ������
	void GetCardDrawRect(CRect& rect);
	//ˢ��������
	void UpdateGameCard();
	void InvalidateCard();
	void UpdateCardPosition();
	//���»���
	void UpdateCardMem();
	//������
	void DrawCardData();
	void DrawGameCard(CDC* pDC);

private:
	//������
	std::vector<tagGameCardItem>	m_vCardList;

	//�Ƿ���Ӧ
	bool			m_bIsPositively;
	//�Ƿ���ʾ
	bool			m_bIsShow;


private:
	//����
	CPngImage*		m_pImageBack;
	//����Դ
	CPngImage		m_ImageCard;
	//��ѡ��
	CPngImage		m_ImageSelCard;

	//�ƿ����
	int				m_iCardWidth;
	int				m_iCardHeight;

	//��ѡ������
	int				m_iSelCardWidth;
	int				m_iSelCardHeight;

	//�Ƽ��
	int				m_iCardSpace;
	//�Ƶ���߶�
	int				m_iCardShootHeight;

	//�����
	CPoint			m_ptOldPosition;
	CPoint			m_ptDrawStartXY;

	//������
	CRect			m_rcGameCard;

	//����ģʽ
	enGameCardXCollocateMode	m_CardXCollocateMode;
private:
	CWnd*			m_pViewWnd;

private:
	//�ƻ���
	CDC					m_memdc;
	CCriticalSpection	m_lockMemDC;	


private:
	//����
	int				m_iMouseCurIndex;
	int				m_iMouseDownIndex;
	bool			m_bIsMouseDown;
};
