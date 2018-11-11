
//
// 2009
// DhLj
// ��ע��(>=3),��ע��(==1)
//

#ifndef _TAGROLES_H_
#define _TAGROLES_H_

#include <vector>

const int MAXCARDNUMS = 20;

typedef enum _tagCardState
{
	no,		//δѡ��״̬
	yes		//ѡ��״̬
}CARDSTATE;

typedef enum _tagCardClass
{
	cerror,								// ��Ч��	|0
	cone,								// ����		|1!
	ctwo,								// ˫��		|2!
	cthree,								// ����		|3!
	cfour,								// ը��		|4!
	ctth,								// ��˳(2)	|5!
	cthone,								// ����һ��	|6!
	cthtwo,								// ����һ��	|7!
//	cfone,								// �Ĵ�һ��	|8!
//	cftwo,								// �Ĵ�һ��	|9!
	cttth,								// ˫˳(3)	|10!
	cthth,								// ��˳(3)	|11!
	cftone,								// �Ĵ�����	|12!
	cfttwo,								// �Ĵ�����	|13!
	ctfour,								// ˫˳(4)	|14!
	cthfour,							// ��˳(4)	|15!
	ctthone,							// ��˳������|16!
	ctthtwo,							// ��˳������|17!
	clink5,								// ��˳(5)	|18!
	clink6,								// ��˳(6)	|19!
	clink7,								// ��˳(7)	|20!
	clink8,								// ��˳(8)	|21!
	clink9,								// ��˳(9)	|22!
	clink10,							// ��˳(10)	|23!
	clink11,							// ��˳(11)	|24!
	clink12,							// ��˳(12)	|25!
	ctfive,								// ˫˳(5)	|26!
	cthfive,							// ��˳(5)	|27!
	ctsix,								// ˫˳(6)	|28!
	cthsix,								// ��˳(6)	|29!
	cththone,							// ��˳������|30!
	cththtwo,							// ��˳������|31!
	ctseven,							// ˫˳(7)	|32!
	ctevent,							// ˫˳(8)	|33!
	ctnine,								// ˫˳(9)	|34!
	ctten,								// ˫˳(10)	|35!
	cthfone,							// ��˳���ĵ�|36!
	cthftwo,							// ��˳���Ķ�|37!
	chigh,								// ˫��
	ctrue								// �׳�
	
}CARDCLASS;

typedef enum _tagCardChange
{
	R3,									// 0
	R4,									// 1
	R5,									// 2
	R6,									// 3
	R7,									// 4
	R8,									// 5
	R9,									// 6
	RO,									// 7
	RJ,									// 8
	RQ,									// 9
	RK,									// 10
	RA,									// 11
	RT,									// 12
	RWS,								// С��
	RWB									// ����
}CARDCHANGE;


typedef enum _tagCardIf
{
	sde,								// ����λ
	beg,								// ��ʼλ
	mid,								// �м�λ
	end									// ����λ
}CARDIF;


typedef struct _tagCard			// typedef struct _tagCard CARD;
{
	int iCard;
	CARDSTATE cs;
}CARD;

typedef struct _tagCardR
{
	int iNum;							// �����Ƶ�����
	CARDCHANGE rc;
}RCARD;


typedef struct _tagCardWorth
{
	RCARD rcard;
	CARDIF csif;						
}CARDWORTH;

typedef struct _tagWebPos
{
	unsigned long ulSizex;
	unsigned long ulSizey;
}WEBPOS;


typedef std::vector<int> IVector;
typedef std::vector<CARD> CVector;
typedef std::vector<RCARD> RCVector;
typedef std::vector<CARDWORTH> CWVector;

typedef enum _tagAIPos
{
	p_error,
	p_mid,
	p_up,
	p_down	
}AIPOS;


typedef struct _tagAIRun
{
	RCVector rcIn;			//��ǰ����ÿ���Ƶĸ���
	RCVector rcAll;			//���г���ÿ���Ƶĸ���
	CARDCLASS cc;			//��������
	AIPOS aipos;			//����λ��(˭�ҳ���)
	unsigned int iSizeMid;	//�Լ�������
	unsigned int iSizeUp;	//�ϼ�������
	unsigned int iSizeDown;	//�¼�������
}AIRUN;

#endif