#ifndef CMD_LAND_HEAD_FILE
#define CMD_LAND_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						10									//��Ϸ I D
#define GAME_PLAYER					3									//��Ϸ����
#define GAME_NAME					TEXT("������")					    //��Ϸ����

//��Ϸ״̬
#define GS_WK_FREE					GS_FREE								//�ȴ���ʼ
#define GS_WK_SCORE					GS_PLAYING							//�з�״̬
#define GS_WK_DOUBLE_SCORE			GS_PLAYING+1						//�ӱ�״̬
#define GS_WK_PLAYING				GS_PLAYING+2						//��Ϸ����
#define GS_WK_SNATCH				GS_PLAYING+3						//������״̬

//�׶α���
#define CSD_NORMAL		0
#define CSD_SNATCHLAND	1
#define CSD_BRIGHTCARD	2
#define CSD_GAMESTART	3

//��������                                               
#define BRIGHT_START_TIME				8								//���ƿ�ʼ
#define BRIGHT_CARD_TIME				4								//���Ʊ���

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_SEND_CARD				100									//��������
#define SUB_S_LAND_SCORE			101									//�е�������
#define SUB_S_GAME_START			102									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				103									//�û�����
#define SUB_S_PASS_CARD				104									//��������
#define SUB_S_GAME_END				105									//��Ϸ����
#define SUB_S_BRIGHT_START			106									//���ƿ�ʼ
#define SUB_S_BRIGHT_CARD			107									//�������
#define SUB_S_DOUBLE_SCORE			108									//�ӱ�����
#define SUB_S_USER_DOUBLE			109									//�ӱ�����
#define SUB_S_USER_SNATCH_SCORE     110                                 //����������


//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONGLONG						lBaseScore;							//��������
	bool							bBrightStart[GAME_PLAYER];			//�������
};

//��Ϸ�з�״̬
struct CMD_S_StatusScore
{
	BYTE							bLandScore;							//��������
	LONGLONG						lBaseScore;							//��������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bScoreInfo[3];						//�з���Ϣ
	BYTE							bCardData[3][20];					//�����˿�
	bool							bUserTrustee[GAME_PLAYER];			//����й�
	BYTE							bCallScorePhase;					//���ƽ׶�
	BYTE							bBrightTime;						//���Ʊ���
	bool							bUserBrightCard[GAME_PLAYER];		//�������
};

//�ӱ�״̬
struct CMD_S_StatusDoubleScore
{
	WORD							wLandUser;							//�������
	LONGLONG						lBaseScore;							//��������
	BYTE							bLandScore;							//��������
	BYTE							bBackCard[3];						//�����˿�
	BYTE							bCardData[3][20];					//�����˿�
	BYTE							bCardCount[3];						//�˿���Ŀ
	bool							bUserTrustee[GAME_PLAYER];			//����й�
	bool							bAllowDouble;						//����ӱ�
	bool							bDoubleUser[GAME_PLAYER];			//�ӱ����
	bool							bUserBrightCard[GAME_PLAYER];		//�������

};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	WORD							wLandUser;							//�������
	WORD							wBombTime;							//ը������
	WORD							wBombNums;							//ը������
	LONGLONG						lBaseScore;							//��������
	BYTE							bLandScore;							//��������
	WORD							wLastOutUser;						//���Ƶ���
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bBackCard[3];						//�����˿�
	BYTE							bCardData[3][20];					//�����˿�
	BYTE							bCardCount[3];						//�˿���Ŀ
	BYTE							bTurnCardCount;						//��������
	BYTE							bTurnCardData[20];					//�����б�
	bool							bUserTrustee[GAME_PLAYER];			//����й�
	BYTE							bBrightTime;						//���Ʊ���
	bool							bUserBrightCard[GAME_PLAYER];		//�������
	bool							bDoubleUser[GAME_PLAYER];			//��Ҽӱ�
};

//�����˿�
struct CMD_S_SendCard
{
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[17];						//�˿��б�
};

//�����˿�
struct CMD_S_SendAllCard
{
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[GAME_PLAYER][20];			//�˿��б�
	BYTE							bBackCardData[3];					//�����˿�
	bool							bUserBrightCard[GAME_PLAYER];		//�������
};

//�û��е���
struct CMD_S_LandScore
{
	WORD							bLandUser;							//�е������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bLandScore;							//�Ƿ�е���<0�����У�1���е���>
	BYTE							bPreCallScorePhase;					//֮ǰ�׶�
	BYTE							bCallScorePhase;					//���ƽ׶�
	int                             bUserBeiShu[GAME_PLAYER];           //��ұ���
	BYTE                            bCurrentScore;
}; 

//�û�������				Author:<cxf>; CreateDate:<2011-12-29>
struct CMD_S_SnatchLand
{
	WORD							wSnatchUser;						//���������
	WORD							wCurrentUser;						//��ǰ���
	BYTE							bSnatchLand;						//�Ƿ�������<0:����;1:������>
	BYTE							bPreCallScorePhase;					//֮ǰ�׶�
	BYTE							bCallScorePhase;					//���ƽ׶�
	int                             bUserBeiShu[GAME_PLAYER];           //��ұ���
	BYTE                            bSnatchLandCount;                   //����������
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD				 			wLandUser;							//�������
	BYTE							bLandScore;							//��������
	WORD				 			wCurrentUser;						//��ǰ���	
	BYTE							bBrightCard;						//���Ʊ�־
	bool							bUserBrightCard[GAME_PLAYER];		//�������
	BYTE							bBackCardData[3];					//�����˿�
	BYTE							bCardData[20];						//�˿��б�
	BYTE							bCardNum;
};

//�û�����
struct CMD_S_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wOutCardUser;						//�������
	BYTE							bCardData[20];						//�˿��б�
	WORD							wBombNums;							//ը������
	//cxf
	BYTE							bCircuit[GAME_PLAYER];				//�û����ƴ���
	WORD							wBankerUser;						//ׯ��
	bool							bIsDouble[GAME_PLAYER];				//�Ƿ��мӱ�
	int                             bUserBeiShu[GAME_PLAYER];           //��ұ���
    BYTE                            bHandCardCount;                     //���Ƹ���       
};

//��������
struct CMD_S_PassCard
{
	BYTE							bNewTurn;							//һ�ֿ�ʼ
	WORD				 			wPassUser;							//�������
	WORD				 			wCurrentUser;						//��ǰ���
	//cxf
	BYTE							bCircuit[GAME_PLAYER];				//�û����ƴ���
	WORD							wBankerUser;						//ׯ��
	bool							bIsDouble[GAME_PLAYER];				//�Ƿ��мӱ�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONGLONG						lGameTax[3];						//��Ϸ˰��
	LONGLONG						lGameScore[3];						//��Ϸ����
	BYTE							bCardCount[3];						//�˿���Ŀ
	BYTE                            bHandCardData[GAME_PLAYER][20];     //�˿�����
	BYTE							bCardData[54];						//�˿��б�
	bool                            IsChunTian;                         //�Ƿ���lq
	bool                            IsFanChun;                          //�Ƿ񷴴�lq
	bool                            IsMissile;                          //�Ƿ������
	LONGLONG                        RoomScore[GAME_PLAYER];             //�������
	int                             GameBeiShu[GAME_PLAYER];            //��Ϸ����
	BYTE                            OneBoomCount[GAME_PLAYER];          //ը������
	int								iLargessCount[GAME_PLAYER];		    //������ʯ
	bool                            IsUserLeft[GAME_PLAYER];            //�û��Ƿ�ǿ��
	//
};

//���ƿ�ʼ
struct CMD_S_BrightStart
{
	WORD							wBrightUser;						//�������
	bool							bBright;							//���Ʊ�ʶ
};
struct CMD_S_SendCardFinish
{
	bool                            bSendCardFinish;
};

//�������
struct CMD_S_BrightCard
{
	WORD							wBrightUser;						//�������
	BYTE							cbBrightTime;						//���Ʊ���
	BYTE							cbCurrenBrightTime;					//��ǰ����
	BYTE							cbCallScorePhase;					//�зֽ׶�
};

//�ӱ��ṹ
struct CMD_S_DoubleScore
{
	bool							bAllowDouble;						//����ӱ�
	BYTE							bBackCard[3];						//�����˿�
	WORD				 			wLandUser;							//�������
	BYTE							bCurrentScore;						//��ǰ�з�
};

//�ӱ��ṹ
struct CMD_S_UserDouble
{
	WORD							wDoubleUser;						//�ӱ����
	bool							bDoubleScore;						//�Ƿ�ӱ�
	BYTE							bCurrentScore;						//��ǰ�з�
	//cxf 
	BYTE							bNextUser;							//��һ���
	BYTE							bState;								//�ӱ�״̬(��-��-��)
	WORD							bDoubleType;						//�ӱ�����(��-��-��)
	int                             bBeiShu[GAME_PLAYER];               //��ұ���
};



//OSC Add
//����������ṹMISSILE
//struct tag_Server_TaskInfo
//{
//	bool							bIsOutBombWin;						//�Ƿ���ը��Ӯ
//	bool							bIsOutMissile;						//�Ƿ������Ӯ
//	BYTE							bRegBombNums;						//��¼һ�ֵ��д��ը������
//	int								iRegJoineLandNums;					//��¼������������
//	int								iRegJointWinNums;					//��¼����Ӯ�Ĵ���
//};


//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_LAND_SCORE			1									//�û��з�<�е���>
#define SUB_C_OUT_CART				2									//�û�����
#define SUB_C_PASS_CARD				3									//��������
#define SUB_C_TRUSTEE				4									//�й���Ϣ
#define SUB_C_BRIGHT_START			5									//���ƿ�ʼ
#define SUB_C_BRIGHT				6									//�������
#define SUB_C_DOUBLE_SCORE			7									//�ӱ�����
#define SUB_C_SNATCH_SCORE			8									//������

//�йܽṹ
struct CMD_C_UserTrustee {
	WORD							wUserChairID;						//�������
	bool							bTrustee;							//�йܱ�ʶ
};

//�û��е���
struct CMD_C_LandScore
{
	BYTE							bLandScore;							//�е���
};
//�û�������
struct CMD_C_SnatchLand
{
	BYTE                            bSnatchLand;                        //������
};
//�������ݰ�
struct CMD_C_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	BYTE							bCardData[20];						//�˿��б�
};

//�������
struct CMD_C_BrightCard
{
	BYTE							cbBrightCardTime;					//���Ʊ���
};

//��Ҽӱ�
struct CMD_C_DoubleScore
{
	bool							bDoubleScore;						//��Ҽӱ�
	WORD							bDoubleType;						//���ͣ���-��-����
};


//�����ʾ����
struct CMD_C_UserTSZhangNums
{
	WORD							wNums;								//ʣ�༸����
	bool							bIsTS;								//�Ƿ���ʾ
};
//////////////////////////////////////////////////////////////////////////

#endif
