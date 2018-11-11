
//
// 2011
//

#ifndef _ROLES_H_DDZ_LOGIC_DEF_H_
#define _ROLES_H_DDZ_LOGIC_DEF_H_

#include "TagRoles.h"
#include "KddzAI.h"
#include "Roles.h"

//��ֵ����
#define	MASK_COLOR					0xF0								//��ɫ����
#define	MASK_VALUE					0x0F								//��ֵ����


//������AI�߼���
class CDDZLogic
{
public:
	CDDZLogic( void );
	virtual ~CDDZLogic( void );

public:
	//��ʼ���߼�����
	void InitLogicData(BYTE byMeChairID);
	//������������
	void SetHandCard(const BYTE* pCard, BYTE byNums, bool bIsLand=false);
	//�в��е���
	bool GetLandTag();
	//���õ���
	void SetLandTag(BYTE bBankerUserID);
	//����
	void OutCard(BYTE* pCard, BYTE& byNums);
	//���ƽ��
	void OutCardResult(BYTE byChairID, bool bIsOutCard, const BYTE* pCard=NULL, BYTE byNums=0);
	//����˭����
	void SetWhoOutCard(BYTE byChairID, bool bIsReset=false);
	//ɾ���˿�
	bool RemoveCard(const BYTE cbRemoveCard[], BYTE cbRemoveCount, BYTE cbCardData[], BYTE cbCardCount);

public:
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) { return cbCardData&MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData) { return cbCardData&MASK_COLOR; }
	//ת��������.
	int  SwitchCardDataToInt(BYTE byCardData);
	BYTE SwitchCardDataToByte(int iCardData);
	bool SwitchBYTEtoINT(const BYTE* pCard, BYTE byNums, int* pRetCard, int& iRetNums);
	bool SwitchINTtoBYTE(const int* pCard, int iNums, BYTE* pRetCard, BYTE& byRetNums);
	//���Ʊ�
	void MakeCardsTable( const RCVector& rcIn );

private:
	Roles					m_rAndroid;			// ������
	KddzAI*					m_aiAndroid;		// ������->AI

	AIRUN					m_aiRun;	 		//��ǰ����״̬
	RCVector				m_rcAll;			//����������
	BYTE					m_byMeChairID;		//�Լ����Ӻ�
	BYTE					m_byLandChairID;	//�������Ӻ�
};


#endif