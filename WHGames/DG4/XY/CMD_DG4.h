#ifndef CMD_DG4_HEAD_FILE
#define CMD_DG4_HEAD_FILE

#include "biostream.h"

namespace DG4
{
//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID							999								    //��Ϸ I D
#define GAME_PLAYER						4									//��Ϸ����
#define GAME_NAME						TEXT("DG4")					        //��Ϸ����
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����

///////////////////////////////////
#define ALLCARD							108
#define MAXCARD							27
#define MAX_PLAYER						4					
//////////////////////////////////////////////////////////////////////////
//����������ṹ
#define SUB_S_GAME_START				100									//��Ϸ��ʼ
#define SUB_S_TIMER_START				101									//��ʼ����ʱ
#define SUB_S_POWER						102									//Ȩ��
#define SUB_S_ENDROAR					105									//��������
#define SUB_S_RELINK					106									//����
#define SUB_S_ENDOUT					107									//����һ��
#define SUB_S_TEAMERPAI					108									//���Ͷ��ѵ���
#define SUB_S_PLAYERSCORE				109									//����ÿ���˵õķ���
#define SUB_S_SENDCOUNT					110									//��������
#define SUB_S_TURNSCORE					111									//���;ַ�
#define SUB_S_SENDTURN					112									//����˳��
#define SUB_S_ENDGAME					113									//��Ϸ����
#define SUB_S_SHOW						114									//��ʾ�е��ƻ���������Ϣ

#define SUB_CS_OUT						115									//����
#define SUB_CS_ROAR						116									//����
#define SUB_S_TUOGUAN					117									//�й�
#define SUB_CS_PLAYSOUND				118									//��������
#define SUB_CS_TEAMERPAI				119									//���ѵ���
#define SUB_CS_GAMERULE					120									//����
#define SUB_C_RELINK_TIP				121			        //����������ʾ����ʾ����������

#define SUB_CS_RAND_TASK_ID				122									//��Ϸ�������
#define SUB_CS_RAND_FINISHED_TASK		123									//��Ϸ����������

inline const char * GetPorkCardName(unsigned char cardid)
{
	bool bLegal =(cardid>0 && cardid<55);
	if (bLegal)
	{
		static const char * szCardName[54] = {
				"����A", "����2", "����3", "����4", "����5", "����6", "����7", "����8", "����9", "����10", "����J", "����Q", "����K",
				"÷��A", "÷��2", "÷��3", "÷��4", "÷��5", "÷��6", "÷��7", "÷��8", "÷��9", "÷��10", "÷��J", "÷��Q", "÷��K",
				"����A", "����2", "����3", "����4", "����5", "����6", "����7", "����8", "����9", "����10", "����J", "����Q", "����K",
				"����A", "����2", "����3", "����4", "����5", "����6", "����7", "����8", "����9", "����10", "����J", "����Q", "����K",
				"С��", "����",
		};
		return szCardName[cardid-1];
	}
	else
		return "";
}

//��Ϸ״̬
struct CMD_S_StatusFree
{
	unsigned char								cbStatus;							//��ǰ״̬
	int								lCellMinScore;						//��С��ע
	int								lCellMaxScore;						//�����ע
};


//��Ϸ״̬
struct CMD_S_StatusPlay
{
	unsigned char								cbStatus;							//��ǰ״̬
	int								lCellMinScore;						//��С��ע
	int								lCellMaxScore;						//�����ע
	//��־����
	//unsigned char								bShowHand[GAME_PLAYER];				//�����־
	//unsigned char								bAddScore;							//��ע��־

	//��ע��Ϣ
	int								lCellScore;							//��Ԫ��ע
	int								lTurnMaxScore;						//�����ע
	int								lTurnLessScore;						//��С��ע
//	int								lCellMaxScore;						//�����ע
	int								lAddLessScore;						//����Сע
	int								lTableScore[GAME_PLAYER];			//��ע��Ŀ
	int								lTotalScore[GAME_PLAYER];			//�ۼ���ע
	int								lCenterScore;						//���ĳ���

	//״̬��Ϣ
	WORD								wDUser;								//D���
	WORD				 				wCurrentUser;						//��ǰ���
	unsigned char								cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	unsigned char							    cbBalanceCount;						//ƽ�����

	//�˿���Ϣ
//	unsigned char								cbCenterCardData[MAX_CENTERCOUNT];	//�˿���Ŀ
	unsigned char						cbHandCardData[MAXCARD];			//�����˿�
};

//�����˿�
struct CMD_S_GameStart_CB
{
	enum {S_ID = SUB_S_GAME_START};
	//theType,֮ǰû���õ�����������һ��ö��
	enum {GAME_START,GAME_RELINK};

	unsigned char		theIsGameStart;
	unsigned char		theBanker;					// ׯ��
	unsigned char		theType;					// ����(��ʼ��Ϸ����������
	unsigned char		theSeat;					// ˭����
	unsigned char		theCount;					// ����
	unsigned char		theCards[MAXCARD];			// ������

	void reset() { memset(this, 0, sizeof(*this)); }
	CMD_S_GameStart_CB() { reset(); }
	friend bostream& operator<<(bostream& bos,const CMD_S_GameStart_CB& pl)
	{
		bos << pl.theIsGameStart;
		bos << pl.theBanker;
		bos << pl.theType;
		bos << pl.theSeat;
		bos << pl.theCount;

		bos.write((char*)pl.theCards, sizeof(pl.theCards));

		return bos;
	}
	friend bistream& operator>>(bistream& bis,CMD_S_GameStart_CB& pl)
	{	
		pl.reset();
		bis >> pl.theIsGameStart;
		bis >> pl.theBanker;
		bis >> pl.theType;
		bis >> pl.theSeat;
		bis >> pl.theCount;

		bis.read((char*)pl.theCards, sizeof(pl.theCards));

		return bis;
	}
};

// Ȩ��
struct CMD_S_Power_CB
{
	enum { XY_ID = SUB_S_POWER };

	enum tagPower
	{
		p_Null   = 0x00,
		p_Roar	 = 0x01,
		p_Out	 = 0x02,		// ����Ȩ��
	};

	unsigned char		theWhoPlay;		// ˭��Ȩ��
	unsigned int		theDownTime;	// ��ʱ��ʱ�䣨��λ�룩
	unsigned int		thePower;		// ʲôȨ��

	void reset() { memset(this, 0, sizeof(*this)); }
	CMD_S_Power_CB() { reset(); }
	friend bostream& operator<<(bostream& bos,const CMD_S_Power_CB& pl)
	{
		bos << pl.theWhoPlay;
		bos << pl.theDownTime;
		bos << pl.thePower;

		return bos;
	}
	friend bistream& operator>>(bistream& bis,CMD_S_Power_CB& pl)
	{	
		pl.reset();
		bis >> pl.theWhoPlay;
		bis >> pl.theDownTime;
		bis >> pl.thePower;

		return bis;
	}
};

// ��������
struct CMD_S_EndRoar_CB
{
	enum { XY_ID = SUB_S_ENDROAR };

	unsigned char		theWhoRoar; // 1-4,0��ʾû�к���
	unsigned char       theBanker;	// ׯ��
	unsigned char		thePai;		// ����

	void reset() { memset(this, 0, sizeof(*this)); }
	CMD_S_EndRoar_CB() { reset(); }
	friend bostream& operator<<(bostream& bos,const CMD_S_EndRoar_CB& pl)
	{
		bos << pl.theWhoRoar;
		bos << pl.theBanker;
		bos << pl.thePai;

		return bos;
	}
	friend bistream& operator>>(bistream& bis,CMD_S_EndRoar_CB& pl)
	{	
		pl.reset();
		bis >> pl.theWhoRoar;
		bis >> pl.theBanker;
		bis >> pl.thePai;

		return bis;
	}
};

// ��Ϸ����
struct CMD_S_EndGame_CB
{
	enum {XY_ID = SUB_S_ENDGAME}; 
	enum{
		TY_NULL,
		TY_SCORE,		// ���ģʽ��������
		TY_ROAR,		// ����ģʽ��������
		TY_ESCAPE,		// ��������
	};

	unsigned char	theEndType;	    // ��Ϸģʽ	
	unsigned char	theDoubleKill;	// �Ƿ�˫�ۣ�
	unsigned char	theQing;		// �Ƿ���
	unsigned char	theGuang;		// �Ƿ��ͷ
	unsigned char	theWhoKingBomb; // ˭����ը?
	int			theFan;			// ���ٷ�

	unsigned char	theLeftPai[MAX_PLAYER][MAXCARD];//ʣ����
	unsigned char	theWinLose[MAX_PLAYER];			// �仹��Ӯ��0�䣬1Ӯ��2ƽ��
	unsigned char	the7Xi[MAX_PLAYER];				// 7ϲը������
	unsigned char	the8Xi[MAX_PLAYER];				// 8ϲը������
	unsigned char	the510K[MAX_PLAYER];			// 510K�ĸ���   
	long long		theScore[MAX_PLAYER];			// �÷�

	void reset() { memset(this, 0, sizeof(*this)); }
	CMD_S_EndGame_CB() { reset(); }
	friend bostream& operator<<(bostream& bos,const CMD_S_EndGame_CB& pl)
	{
		bos << pl.theEndType;
		bos << pl.theDoubleKill;
		bos << pl.theQing;
		bos << pl.theGuang;
		bos << pl.theWhoKingBomb;
		bos << pl.theFan;

		bos.write((char*)pl.theLeftPai, sizeof(pl.theLeftPai));
		bos.write((char*)pl.theWinLose, sizeof(pl.theWinLose));
		bos.write((char*)pl.the7Xi, sizeof(pl.the7Xi));
		bos.write((char*)pl.the8Xi, sizeof(pl.the8Xi));
		bos.write((char*)pl.the510K, sizeof(pl.the510K));
		bos.write((char*)pl.theScore, sizeof(pl.theScore));

		return bos;
	}
	friend bistream& operator>>(bistream& bis,CMD_S_EndGame_CB& pl)
	{	
		pl.reset();
		bis >> pl.theEndType;
		bis >> pl.theDoubleKill;
		bis >> pl.theQing;
		bis >> pl.theGuang;
		bis >> pl.theWhoKingBomb;
		bis >> pl.theFan;

		bis.read((char*)pl.theLeftPai, sizeof(pl.theLeftPai));
		bis.read((char*)pl.theWinLose, sizeof(pl.theWinLose));
		bis.read((char*)pl.the7Xi, sizeof(pl.the7Xi));
		bis.read((char*)pl.the8Xi, sizeof(pl.the8Xi));
		bis.read((char*)pl.the510K, sizeof(pl.the510K));
		bis.read((char*)pl.theScore, sizeof(pl.theScore));

		return bis;
	}
};

// Ȩ��
struct CMD_S_Show_CB
{
	enum { XY_ID = SUB_S_SHOW };

	enum{
		F_NULL,
		F_JIAOPAI,				// ����
		F_MINGJI,				// ����
	};
	unsigned char		theFlag;
	unsigned char		thePai;
	unsigned char		theSeat;

	void reset() { memset(this, 0, sizeof(*this)); }
	CMD_S_Show_CB() { reset(); }
	friend bostream& operator<<(bostream& bos,const CMD_S_Show_CB& pl)
	{
		bos << pl.theFlag;
		bos << pl.thePai;
		bos << pl.theSeat;

		return bos;
	}
	friend bistream& operator>>(bistream& bis,CMD_S_Show_CB& pl)
	{	
		pl.reset();
		bis >> pl.theFlag;
		bis >> pl.thePai;
		bis >> pl.theSeat;

		return bis;
	}
};

// ����һ��
struct CMD_S_EndOut_CB
{
	enum {XY_ID = SUB_S_ENDOUT}; 

	unsigned char theWhoPlay;

	void reset() { memset(this, 0, sizeof(*this)); }
	CMD_S_EndOut_CB() { reset(); }
	friend bostream& operator<<(bostream& bos,const CMD_S_EndOut_CB& pl)
	{
		bos << pl.theWhoPlay;

		return bos;
	}
	friend bistream& operator>>(bistream& bis,CMD_S_EndOut_CB& pl)
	{	
		pl.reset();
		bis >> pl.theWhoPlay;

		return bis;
	}
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ


// ����
struct CMD_Roar_CB
{
	enum { XY_ID = SUB_CS_ROAR};
	unsigned char		theSeat;
	unsigned char		theFlag;

	void reset() { memset(this, 0, sizeof(*this)); }
	CMD_Roar_CB() { reset(); }
	friend bostream& operator<<(bostream& bos,const CMD_Roar_CB& pl)
	{
		bos << pl.theSeat;
		bos << pl.theFlag;

		return bos;
	}
	friend bistream& operator>>(bistream& bis,CMD_Roar_CB& pl)
	{	
		bis >> pl.theSeat;
		bis >> pl.theFlag;

		return bis;
	}
};

typedef struct TFakeKing_CB
{
	unsigned char        index;		//ԭ����ֵ
	unsigned char		fakeindex;	//���ֵ	

	void reset() { memset(this, 0, sizeof(*this)); }
	TFakeKing_CB() { reset(); }
	friend bostream& operator<<(bostream& bos,const TFakeKing_CB& pl)
	{
		bos << pl.index;
		bos << pl.fakeindex;

		return bos;
	}
	friend bistream& operator>>(bistream& bis,TFakeKing_CB& pl)
	{	
		bis >> pl.index;
		bis >> pl.fakeindex;

		return bis;
	}
}TFakeKing;

//����
struct CMD_OutCard_CB
{
	enum {S_ID = SUB_CS_OUT};

	unsigned char		theSeat;					// ˭������
	unsigned char		theCount;					// ����
	unsigned short		theNoSound;					// �Ƿ��ֹ�����������
	// add 2010-01-06,����֮����Ҫ�Ӹ����ͣ�����Ϊ���������棬�ܿ����Ƕ������ͣ������жϣ��ǲ��ɿ���
	// ����A����334455,B����44������5��C�޷��ж�B�ߵ���444555�أ�����445566��Ĭ���ǰ���ʶ��Ϊ444555,�����bug
	unsigned char		thePaiType; 
	unsigned char		thePai[MAXCARD];		// �˿�
	TFakeKing			fakeking[4];

	void reset() { memset(this, 0, sizeof(*this)); }
	CMD_OutCard_CB() { reset(); }
	friend bostream& operator<<(bostream& bos,const CMD_OutCard_CB& pl)
	{
		bos << pl.theSeat;
		bos << pl.theCount;
		bos << pl.theNoSound;
		bos << pl.thePaiType;

		bos.write((char*)pl.thePai, sizeof(pl.thePai));
		bos.write((char*)pl.fakeking, sizeof(pl.fakeking));

		return bos;
	}
	friend bistream& operator>>(bistream& bis,CMD_OutCard_CB& pl)
	{	
		bis >> pl.theSeat;
		bis >> pl.theCount;
		bis >> pl.theNoSound;
		bis >> pl.thePaiType;

		bis.read((char*)pl.thePai, sizeof(pl.thePai));
		bis.read((char*)pl.fakeking, sizeof(pl.fakeking));

		return bis;
	}
};

// �й�
struct CMD_S_Tuoguan_CB
{
	enum { XY_ID = SUB_S_TUOGUAN };

	enum
	{
		T_NULL		= 0x00,
		T_TUOGUAN	= 0x01,
		T_CANCEL	= 0x02,
	};

	unsigned char theSeat;
	unsigned char theFlag;		// �йܱ�־
	unsigned int theTime;	// ��ʱ��ʱ�䣨��λ�룩

	void reset() { memset(this, 0, sizeof(*this)); }
	CMD_S_Tuoguan_CB() { reset(); }
	friend bostream& operator<<(bostream& bos,const CMD_S_Tuoguan_CB& pl)
	{
		bos << pl.theSeat;
		bos << pl.theFlag;
		bos << pl.theTime;

		return bos;
	}
	friend bistream& operator>>(bistream& bis,CMD_S_Tuoguan_CB& pl)
	{	
		pl.reset();
		bis >> pl.theSeat;
		bis >> pl.theFlag;
		bis >> pl.theTime;

		return bis;
	}
};

// ��������
struct CMD_CS_msgPlaySound_CB
{
	enum{XY_ID = SUB_CS_PLAYSOUND};
	enum
	{
		TY_KING_BEEP = 1,		//����ը����ʾ
		TY_8Xi_BEEP,
		TY_7Xi_BEEP,
		TY_JieFeng,				//�ӷ�
		TY_ChaDi,				//���
	};
	unsigned char thePaiType;
	unsigned char theSeat;
	void reset() { memset(this, 0, sizeof(*this)); }
	CMD_CS_msgPlaySound_CB() { reset(); }
	friend bostream& operator<<(bostream& bos,const CMD_CS_msgPlaySound_CB& pl)
	{
		bos << pl.thePaiType;
		bos << pl.theSeat;

		return bos;
	}
	friend bistream& operator>>(bistream& bis,CMD_CS_msgPlaySound_CB& pl)
	{	
		pl.reset();
		bis >> pl.thePaiType;
		bis >> pl.theSeat;

		return bis;
	}
};

//���ַ�
struct CMD_CS_TurnScore_CB
{
	enum { XY_ID = SUB_S_TURNSCORE };

	int theScore; //ץ��

	void reset() { memset(this, 0, sizeof(*this)); }
	CMD_CS_TurnScore_CB() { reset(); }
	friend bostream& operator<<(bostream& bos,const CMD_CS_TurnScore_CB& pl)
	{
		bos << pl.theScore;
		return bos;
	}
	friend bistream& operator>>(bistream& bis,CMD_CS_TurnScore_CB& pl)
	{	
		pl.reset();
		bis >> pl.theScore;
		return bis;
	}
};

struct CMD_CS_PLAYSCORE_CB
{
	enum { XY_ID = SUB_S_PLAYERSCORE };

	int theScore[MAX_PLAYER]; //ץ��

	void reset() { memset(this, 0, sizeof(*this)); }
	CMD_CS_PLAYSCORE_CB() { reset(); }
	friend bostream& operator<<(bostream& bos,const CMD_CS_PLAYSCORE_CB& pl)
	{
		bos.write((char*)pl.theScore, sizeof(pl.theScore));
		return bos;
	}
	friend bistream& operator>>(bistream& bis,CMD_CS_PLAYSCORE_CB& pl)
	{	
		pl.reset();	
		bis.read((char*)pl.theScore, sizeof(pl.theScore));
		return bis;
	}
};

struct CMD_CS_SendCount_CB
{
	enum { XY_ID = SUB_S_SENDCOUNT };

	int thePaiCount[MAX_PLAYER];

	void reset() { memset(this, 0, sizeof(*this)); }
	CMD_CS_SendCount_CB() { reset(); }
	friend bostream& operator<<(bostream& bos,const CMD_CS_SendCount_CB& pl)
	{
		bos.write((char*)pl.thePaiCount, sizeof(pl.thePaiCount));
		return bos;
	}
	friend bistream& operator>>(bistream& bis,CMD_CS_SendCount_CB& pl)
	{	
		pl.reset();
		bis.read((char*)pl.thePaiCount, sizeof(pl.thePaiCount));
		return bis;
	}
};

struct CMD_CS_SendTurn_CB
{
	enum { XY_ID = SUB_S_SENDTURN };

	bool isFirst;//��������������ٷ�һ���ⶫ��������Ϊfalse
    unsigned char	theTurn[MAX_PLAYER];

	void reset() { memset(this, 0, sizeof(*this)); }
	CMD_CS_SendTurn_CB(){ reset();isFirst = true; }
	friend bostream& operator<<(bostream& bos,const CMD_CS_SendTurn_CB& pl)
	{
		bos<<pl.isFirst;
		bos.write((char*)pl.theTurn, sizeof(pl.theTurn));
		return bos;
	}
	friend bistream& operator>>(bistream& bis,CMD_CS_SendTurn_CB& pl)
	{	
		pl.reset();
		bis>>pl.isFirst;
		bis.read((char*)pl.theTurn, sizeof(pl.theTurn));
		return bis;
	}
};

struct CMD_CS_TeamerPai
{
	enum { XY_ID = SUB_CS_TEAMERPAI };
	unsigned char theSeat;
	unsigned char theCount;
	unsigned char thePai[MAXCARD];

	void reset() { memset(this, 0, sizeof(*this)); }
	CMD_CS_TeamerPai() { reset(); }

	friend bostream& operator<<(bostream& bos,const CMD_CS_TeamerPai& pl)
	{

		bos << pl.theSeat;
		bos << pl.theCount;

		bos.write((char*)pl.thePai, sizeof(pl.thePai));
		return bos;
	}
	friend bistream& operator>>(bistream& bis,CMD_CS_TeamerPai& pl)
	{	
		pl.reset();

		bis >> pl.theSeat;
		bis >> pl.theCount;

		bis.read((char*)pl.thePai, sizeof(pl.thePai));
		return bis;
	}
};

struct CMD_CS_GameRule
{
	
    enum { XY_ID = SUB_CS_GAMERULE };
	int bPlaySR;			// �Ƿ��ǲƸ���Ϸ
	int theGiveGift;		// �Ƿ�����콱
	int theMakePai;		// �Ƿ�����
	unsigned char nTrustCounts;		// �����йܵĴ���	
	unsigned char theSpecial;		// 0����ͨ��1����
	int theBase;			// ��
	int theMinRich;		// ���ٲƸ���������ô����޷���ʼ��Ϸ(��Ϊ�����ܷ�����)
	
	void reset() { memset(this, 0, sizeof(*this)); }
	CMD_CS_GameRule() { reset(); }

	friend bostream& operator<<(bostream& bos,const CMD_CS_GameRule& pl)
	{
		bos << pl.bPlaySR;
		bos << pl.theGiveGift;
		bos << pl.theMakePai;
		bos << pl.nTrustCounts;
		bos << pl.theSpecial;
		bos << pl.theBase;
		bos << pl.theMinRich;
		return bos;
	}
	friend bistream& operator>>(bistream& bis,CMD_CS_GameRule& pl)
	{	
		pl.reset();
		bis >> pl.bPlaySR;
		bis >> pl.theGiveGift;
		bis >> pl.theMakePai;
		bis >> pl.nTrustCounts;
		bis >> pl.theSpecial;
		bis >> pl.theBase;
		bis >> pl.theMinRich;
		return bis;
	}
};

struct CMD_CS_msgReLink
{
	enum {XY_ID = SUB_S_RELINK};
	WORD        theChair;                 //��ҵ���λ�ţ����ڳ�ʼ����λ�ţ���Ȼת��Ϊlocation�������
	unsigned char	    theGameState;			// ��Ϸ״̬(GameState)
	unsigned char		theBanker;				// ׯ��
	unsigned char		theLeftTime;			// ʣ��ʱ��
	unsigned char		theWhoPlay;				// ��ǰ��
	unsigned char        theWhoPlayOut;			// ��һ��˭������
	unsigned char		theTGTime;			    // �Ѿ��йܵĴ���
	unsigned char		theRoarPai;				// �е���
	unsigned char		theWhoRoar;				// ˭�����ƣ�
	unsigned char		theWhoMJ;				// ˭��������
	unsigned char		thePass[MAX_PLAYER];	// ˭�����ˣ�
	unsigned char        theWhoBreak[MAX_PLAYER];// ˭������
	unsigned char        theTuoGuan[MAX_PLAYER]; // ˭�й��ˣ�
	unsigned char        theLastOutCard[MAX_PLAYER][MAXCARD];//��һ�γ�����
	unsigned char		theOutCard[MAX_PLAYER][MAXCARD];// ������
	bool		theWhoReady[MAX_PLAYER];// ˭�Ѿ�����˽��ƹ���
	int         theLast[MAX_PLAYER];	// ��һ��
	int			theTotal[MAX_PLAYER];	// ����Ӯ
	int			theTurnScore;			// ���ַ�
	unsigned char	    Score[MAX_PLAYER];      //ÿ����ҵĵ÷�
	int			theLastPaiType;			// �����Ƶ�����
	
	void reset() { memset(this, 0, sizeof(*this)); }
	CMD_CS_msgReLink(){reset();}

	friend bostream& operator<<(bostream& bos,const CMD_CS_msgReLink& pl)
	{
		bos << pl.theChair;
		bos << pl.theGameState;
		bos << pl.theBanker;
		bos << pl.theLeftTime;
		bos << pl.theWhoPlay;
		bos << pl.theWhoPlayOut;
		bos << pl.theTGTime;
		bos << pl.theRoarPai;

		bos << pl.theWhoRoar;
		bos << pl.theWhoMJ;
		bos.write((char*)pl.thePass, sizeof(pl.thePass));
		bos.write((char*)pl.theWhoBreak, sizeof(pl.theWhoBreak));
		bos.write((char*)pl.theWhoBreak, sizeof(pl.theWhoBreak));
		bos.write((char*)pl.theTuoGuan, sizeof(pl.theTuoGuan));
		bos.write((char*)pl.theLastOutCard, sizeof(pl.theLastOutCard));
		bos.write((char*)pl.theOutCard, sizeof(pl.theOutCard));
		bos.write((char*)pl.theWhoReady, sizeof(pl.theWhoReady));
		bos.write((char*)pl.theLast, sizeof(pl.theLast));
		bos.write((char*)pl.theTotal, sizeof(pl.theTotal));
		bos << pl.theTurnScore;
		bos.write((char*)pl.Score, sizeof(pl.Score));
		bos << pl.theLastPaiType;
	
		return bos;
	}

	friend bistream& operator>>(bistream& bis,CMD_CS_msgReLink& pl)
	{	
		pl.reset();
		bis >> pl.theChair;
		bis >> pl.theGameState;
		bis >> pl.theBanker;
		bis >> pl.theLeftTime;
		bis >> pl.theWhoPlay;
		bis >> pl.theWhoPlayOut;
		bis >> pl.theTGTime;
		bis >> pl.theRoarPai;

		bis >> pl.theWhoRoar;
		bis >> pl.theWhoMJ;
		bis.read((char*)pl.thePass, sizeof(pl.thePass));
		bis.read((char*)pl.theWhoBreak, sizeof(pl.theWhoBreak));
		bis.read((char*)pl.theWhoBreak, sizeof(pl.theWhoBreak));
		bis.read((char*)pl.theTuoGuan, sizeof(pl.theTuoGuan));
		bis.read((char*)pl.theLastOutCard, sizeof(pl.theLastOutCard));
		bis.read((char*)pl.theOutCard, sizeof(pl.theOutCard));
		bis.read((char*)pl.theWhoReady, sizeof(pl.theWhoReady));
		bis.read((char*)pl.theLast, sizeof(pl.theLast));
		bis.read((char*)pl.theTotal, sizeof(pl.theTotal));
		bis >> pl.theTurnScore;
		bis.read((char*)pl.Score, sizeof(pl.Score));
		bis >> pl.theLastPaiType;
		
		return bis;

	}

};

//��ʾ������ң����ֻ����ˣ�
struct CMD_CS_msgReLinkTip
{
	enum {XY_ID = SUB_C_RELINK_TIP};
	unsigned char	theSeat;
	unsigned char	theFlag;

	void reset() { memset(this, 0, sizeof(*this)); }
	CMD_CS_msgReLinkTip(){reset();}

	friend bostream& operator<<(bostream& bos,const CMD_CS_msgReLinkTip& pl)
	{
		bos << pl.theSeat;
		bos << pl.theFlag;
		return  bos;
	}

	friend bistream& operator>>(bistream& bis,CMD_CS_msgReLinkTip& pl)
	{	
		pl.reset();
		bis >> pl.theSeat;
		bis >> pl.theFlag;

		return bis;
	}

};

//����ѡ�е���Ϸ����
struct CMD_CS_msgTaskID
{
	enum { XY_ID = SUB_CS_RAND_TASK_ID};
	unsigned char		theID;
	int			theAward;	//������
	
	void reset() { memset(this, 0, sizeof(*this)); }
	CMD_CS_msgTaskID(){reset();}

	friend bostream& operator<<(bostream& bos,const CMD_CS_msgTaskID& pl)
	{
		bos << pl.theID;
		bos << pl.theAward;
		return  bos;
	}

	friend bistream& operator>>(bistream& bis,CMD_CS_msgTaskID& pl)
	{	
		pl.reset();
		bis >> pl.theID;
		bis >> pl.theAward;

		return bis;
	}
};

//��Ϸ�������״̬
struct CMD_CS_msgFinishedTaskID
{
	enum { XY_ID = SUB_CS_RAND_FINISHED_TASK};
	unsigned char	theSeat;		//˭
	unsigned char	theID;			//������ĸ�����
	unsigned char	theFlag;		//��־λ��Ŀǰ��1����־���������͵�
	int		theCnt;			//����˼���
	int		theTotal;		//��������

	void reset() { memset(this, 0, sizeof(*this)); }
	CMD_CS_msgFinishedTaskID(){reset();}

	friend bostream& operator<<(bostream& bos,const CMD_CS_msgFinishedTaskID& pl)
	{
		bos << pl.theSeat;
		bos << pl.theID;
		bos << pl.theFlag;
		bos << pl.theCnt;
		bos << pl.theTotal;
		return  bos;
	}

	friend bistream& operator>>(bistream& bis,CMD_CS_msgFinishedTaskID& pl)
	{	
		pl.reset();
		bis >> pl.theSeat;
		bis >> pl.theID;
		bis >> pl.theFlag;
		bis >> pl.theCnt;
		bis >> pl.theTotal;

		return bis;
	}
};

}; //end of namespace

#endif

