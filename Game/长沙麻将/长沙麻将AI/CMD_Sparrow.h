#ifndef CMD_SPARROW_HEAD_FILE
#define CMD_SPARROW_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						303									//��Ϸ I D
#define GAME_PLAYER					4									//��Ϸ����
#define GAME_NAME					TEXT("��ɳ�齫")					//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH)	//��Ϸ����

//��Ϸ״̬
#define GS_MJ_FREE					GS_FREE								//����״̬
#define GS_MJ_PLAY					(GS_PLAYING+1)						//��Ϸ״̬
#define GS_MJ_HAI_DI				(GS_PLAYING+2)						//����״̬

//////////////////////////////////////////////////////////////////////////

//�������
struct CMD_WeaveItem
{
	BYTE							cbWeaveKind;						//�������
	BYTE							cbCenterCard;						//�����˿�
	BYTE							cbPublicCard;						//������־
	WORD							wProvideUser;						//��Ӧ�û�
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				101									//��������
#define SUB_S_SEND_CARD				102									//�����˿�
#define SUB_S_OPERATE_NOTIFY		103									//������ʾ
#define SUB_S_OPERATE_RESULT		104									//��������
#define SUB_S_OPERATE_HAI_DI		105									//���ײ���
#define SUB_S_GAME_END				106									//��Ϸ����
#define SUB_S_TRUSTEE				107									//�û��й�

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONGLONG						lCellScore;							//�������
	WORD							wBankerUser;						//ׯ���û�
	bool							bTrustee[GAME_PLAYER];				//�Ƿ��й�

};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ����
	LONGLONG						lCellScore;							//��Ԫ����
	WORD							wSiceCount;							//���ӵ���
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�
	bool							bTrustee[GAME_PLAYER];				//�Ƿ��й�

	//״̬����
	BYTE							cbHearStatus;						//����״̬
	BYTE							cbActionCard;						//�����˿�
	BYTE							cbActionMask;						//��������
	BYTE							cbLeftCardCount;					//ʣ����Ŀ

	//������Ϣ
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����˿�
	BYTE							cbDiscardCount[4];					//������Ŀ
	BYTE							cbDiscardCard[4][55];				//������¼

	//�˿�����
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[14];						//�˿��б�

	//����˿�
	BYTE							cbWeaveCount[4];					//�����Ŀ
	CMD_WeaveItem					WeaveItemArray[4][4];				//����˿�
	bool                            m_bTingCard;                         //�ܷ�����
	BYTE							byGangCard[4];						//����
	BYTE							byGangCardNums;						//����
	BYTE							byBuCard[4];						//����
	BYTE							byBuCardNums;						//����
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wSiceCount;							//���ӵ���
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�
	BYTE							cbUserAction;						//�û�����
	BYTE							cbCardData[14*GAME_PLAYER];			//�˿��б�
	bool							bTrustee[GAME_PLAYER];						//�Ƿ��й�

	BYTE							byGangCard[4];						//����
	BYTE							byGangCardNums;						//����
	BYTE							byBuCard[4];						//����
	BYTE							byBuCardNums;						//����
};

//��������
struct CMD_S_OutCard
{
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����˿�
	CMD_S_OutCard()
	{
		wOutCardUser = -1;
		cbOutCardData = -1;
	}
};

//�����˿�
struct CMD_S_SendCard
{
	BYTE							cbCardData;							//�˿�����
	BYTE							cbActionMask;						//��������
	WORD							wCurrentUser;						//��ǰ�û�
	BYTE							byGangCard[4];						//����
	BYTE							byGangCardNums;						//����
	BYTE							byBuCard[4];						//����
	BYTE							byBuCardNums;						//����
};

//������ʾ
struct CMD_S_OperateNotify
{
	WORD							wResumeUser;						//��ԭ�û�
	BYTE							cbActionMask;						//��������
	BYTE							cbActionCard;						//�����˿�
	BYTE							byGangCard[4];						//����
	BYTE							byGangCardNums;						//����
	BYTE							byBuCard[4];						//����
	BYTE							byBuCardNums;						//����
};

//��������
struct CMD_S_OperateResult
{
	WORD							wOperateUser;						//�����û�
	WORD							wProvideUser;						//��Ӧ�û�
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����˿�
};

//���ײ���
struct CMD_S_OperateHaiDi
{
	WORD							wCurrentUser;						//��ǰ�û�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	BYTE							cbBirdCard;							//�����˿�
	BYTE							cbChiHuCard;						//�Ժ��˿�
	BYTE							cbHaiDiCard;						//�����˿�
	WORD							wProvideUser;						//�����û�
	WORD							wBirdHitUser;						//�����û�
	LONGLONG						lGameScore[4];					    //��Ϸ����
	WORD							wChiHuKind[4];						//��������
	WORD                            wChiHuRight[4];                     //����Ȩλ
	BYTE							cbCardCount[4];						//�˿���Ŀ
	BYTE							cbCardData[4][14];					//�˿�����

	bool							bIsHuangzhuang;						//�Ƿ��ׯ
	int                             cbTimes[GAME_PLAYER];               //��ҷ���
	int                             iCellScore;                         //���䱶�ʣ���Ԫ���֣�
	bool                            IsUserLeft[GAME_PLAYER];            //�û��Ƿ�ǿ��
	int                             iChiHuKind[GAME_PLAYER];           //�������ͣ�0��û����1��������2�����ƣ�3�����ڣ� 
	int                             iChiHuType;                         //��������
};
//�û��й�
struct CMD_S_Trustee
{
	bool							bTrustee;							//�Ƿ��й�
	WORD							wChairID;							//�й��û�
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CARD				1									//��������
#define SUB_C_OPERATE_CARD			2									//�����˿�
#define SUB_C_OPERATE_HAI_DI		3									//���ײ���
#define SUB_C_TRUSTEE				4									//�û��й�

//��������
struct CMD_C_OutCard
{
	BYTE							cbCardData;							//�˿�����
};

//��������
struct CMD_C_OperateCard
{
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����˿�
};

//���ײ���
struct CMD_C_OperateHaiDi
{
	BYTE							cbTakeHaiDi;						//��ȡ����
};
//�û��й�
struct CMD_C_Trustee
{
	bool							bTrustee;							//�Ƿ��й�	
};
//////////////////////////////////////////////////////////////////////////

#endif