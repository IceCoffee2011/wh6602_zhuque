#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE
#include <d3dx9.h> 
//////////////////////////////////////////////////////////////////////////
/*ȫ����λ�ţ�

				1			0
-----------------------------------


-----------------------------------
				2			3

0��1ͬһ�࣬2��3ͬһ��

���Լ�����λ��ΪC����D���Լ���ͬһ����ж�Ϊ(C/2)==(D/2)

���Լ�������Ϊ(x,y)�����͵��Է�������Ϊ
1.�Լ�ͬ�ࣺ(x,y)
2.�Լ����棺(VIEW_WIDTH-x,VIEW_HEIGHT-y)

���Լ����ڵ��Ƕ�ΪAngle�����͵��Է�������Ϊ
1.�Լ�ͬ�ࣺAngle
2.�Լ����棺D3DX_PI+Angle

�Լ��Ĵ��ڽǶȷ�Χ��3*D3DX_PI/2 <--> 5*D3DX_PI/2
����Ĵ��ڽǶȷ�Χ��1*D3DX_PI/2 <--> 3*D3DX_PI/2

*/

//��غ궨��
#define IsSameSide(wMeChairID,wTestChairID)	(((wMeChairID)/2==(wTestChairID)/2)?true:false)
#define SwitchAngle(fAngle)	(D3DX_PI+(fAngle))
#define SwitchCoorX(nPosX)	(VIEW_WIDTH-(nPosX))
#define SwitchCoorY(nPosY)	(VIEW_HEIGHT-(nPosY))

//��̨�Ƕ�
#define ME_MAX_CAN_ANGLE				(5*D3DX_PI/2)							//���Ƕ�
#define ME_MIN_CAN_ANGLE				(3*D3DX_PI/2)							//��С�Ƕ�

#define UP_MAX_CAN_ANGLE				(3*D3DX_PI/2)							//���Ƕ�
#define UP_MIN_CAN_ANGLE				(D3DX_PI/2)								//��С�Ƕ�
/////////////////////////////////////////////////////////////////////////////////
//������
#define KIND_ID						601									//��Ϸ I D

#define GAME_NAME					TEXT("�������")					//��Ϸ����

#define VIEW_WIDTH					1024								//��ͼ���
#define VIEW_HEIGHT					595									//��ͼ�߶�

#define TRACE_POINT_SPACE_NORMAL	5									//�켣�����
#define TRACE_POINT_SPACE_BIG		7									//�켣�����

#define CELL_SCORE_COUNT			5									//��Ԫ��Ŀ

#define MAX_SHOOT_BULLET_COUNT		5									//�ӵ���Ŀ

//�������
#define GAME_PLAYER					4									//��Ϸ����
#define VERSION_SERVER				PROCESS_VERSION(5,1,0)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(5,1,0)				//����汾

//////////////////////////////////////////////////////////////////////////////////

//��Ⱥ����
enum enFishKind
{
	enFishKind_1,					//��Ⱥ����
	enFishKind_2,					//��Ⱥ����
	enFishKind_3,					//��Ⱥ����
	enFishKind_4,					//��Ⱥ����
	enFishKind_5,					//��Ⱥ����
	enFishKind_6,					//��Ⱥ����
	enFishKind_7,					//��Ⱥ����
	enFishKind_8,					//��Ⱥ����
	enFishKind_9,					//��Ⱥ����
	enFishKind_10,					//��Ⱥ����
	enFishKind_11,					//��Ⱥ����
	enFishKind_12,					//��Ⱥ����
	enFishKind_No					//��������
};

//���׳���
enum enSeaSceneKind
{
	enSeaSceneKind_1,					//��������
	enSeaSceneKind_2,					//��������
	enSeaSceneKind_3,					//��������
	enSeaSceneKind_No					//��������
};
////////////////////////////////////////////////////////////////////////////////
//�ӵ�����(6.1û�����ö��)
enum enBulletCountKind
{
	enBulletCountKind_100,
	enBulletCountKind_600,
	enBulletCountKind_800,
	enBulletCountKind_End
};

//��Ⱥ��ֵ
int const g_nFishScore[enFishKind_No]={1,2,4,7,10,20,30,40,50,60,100,200};

///////////////////////////////////////////////////////////////////////////////

//�����
enum enServerSubCmd
{
	enServerSubCmd_Begin=100,

	SUB_S_GAME_SCENE,									//������Ϣ
	SUB_S_TRACE_POINT,									//�켣����
	SUB_S_USER_SHOOT,									//�����ڵ�
	SUB_S_CAPTURE_FISH,									//������Ⱥ
	SUB_S_BULLET_COUNT,									//�ӵ���Ŀ
	SUB_S_COIN_COUNT,									//�����Ŀ
	SUB_S_FISH_COUNT,									//��Ⱥ��Ŀ
	SUB_S_CHANGE_SCENE,									//�л�����
	SUB_S_SCORE_INFO,									//������Ϣ
	SUB_S_SET_CELL_SCORE,								//��Ԫ����
	SUB_S_BONUS_INFO,									//�ʽ���Ϣ
	SUB_S_GAME_MESSAGE,										//��Ϸ��Ϣ

	enServerSubCmd_End
};

//��Ϣ���ݰ�
struct CMD_S_GameMessage
{
	WORD								wMessageLength;					//��Ϣ����
	TCHAR								szContent[1024];				//��Ϣ����
};

//�ʽ���Ϣ
struct CMD_S_BonusInfo
{
	LONG							lBonus;									//�ʽ���Ŀ
};

//��Ԫ����
struct CMD_S_SetCellScore
{
	WORD							wChairID;								//�������
	LONG							lCellScore;								//��Ԫ����
};

//�л�����
struct CMD_S_ChangeScene
{
	enSeaSceneKind					CureSeaSceneKind;						//��ǰ����
	bool							bChange;								//�ı��ʶ
	bool							bLToR;									//�ı䷽��
};

//�ӵ���Ŀ
struct CMD_S_BulletCount
{
	WORD							wChairID;								//�������
	enBulletCountKind				BulletCountKind;						//�ӵ�����
	LONG							lBulletCount;							//�ӵ���Ŀ
};
////6.1�ӵ���Ŀ
//struct CMD_S_BulletCount
//{
//	WORD							wChairID;								//�������
//	LONG							lBulletCount;							//�ӵ���Ŀ
//};

//��Ⱥ��Ŀ
struct CMD_S_CptrFishCount
{
	enFishKind						FishKind;								//��Ⱥ����
	WORD							wFishCount;								//��Ⱥ��Ŀ
};

//�����Ŀ
struct CMD_S_CoinCount
{
	WORD							wChairID;								//�������
	LONG							lCoinCount;								//�����Ŀ
};

//������Ϣ
struct CMD_S_GameScene
{
	LONG							lBulletCharge[enBulletCountKind_End];	//�ӵ�����
	enSeaSceneKind					CureSeaSceneKind;						//��ǰ����
	LONG							lCellScore[CELL_SCORE_COUNT];			//��Ԫ����
	LONG							lUserCellScore[GAME_PLAYER];			//��Ԫ����
};
////6.1������Ϣ
//struct CMD_S_GameScene
//{
//	LONG							lBulletCharge;							//�ӵ�����
//	enSeaSceneKind					CureSeaSceneKind;						//��ǰ����
//	LONG							lCellScore;								//����
//	WORD							wCellMedal;								//��Ԫ����
//};

//��Ϸ״̬
struct CMD_S_FishTrace
{
	//�ؼ�����
	int								nInitX[5];								//�ؼ�����
	int								nInitY[5];								//�ؼ�����
	int								nInitCount;								//������Ŀ

	//��Ⱥ����
	enFishKind						FishKind;								//��Ⱥ����
	DWORD							dwFishID;								//�����ʶ
	bool							bRegular;								//�����ʶ
};


//�����ڵ�
struct CMD_S_UserShoot
{
	WORD							wChairID;								//�������
	float							fAngle;									//����Ƕ�
	enBulletCountKind				BulletCountKind;						//�ӵ�����
	BYTE							byShootCount;							//�ӵ���Ŀ	
};

//������Ⱥ
struct CMD_S_CaptureFish
{
	WORD							wChairID;								//�������
	DWORD							dwFishID;								//��Ⱥ��ʶ
	enFishKind						FishKind;								//��Ⱥ����
	LONG							lFishScore;								//��Ⱥ�÷�
};
//////////////////////////////////////////////////////////////////////////
//�����

enum enClientSubCmd
{
	enClientSubCmd_Begin=200,

	SUB_C_BUY_BULLET,									//�����ӵ�
	SUB_C_USER_SHOOT,									//�����ڵ�
	SUB_C_CAPTURE_FISH,									//������Ⱥ
	SUB_C_GET_SCORE_INFO,								//������Ϣ
	SUB_C_SET_PROPABILITY,								//���ø���
	SUB_C_CALC_SCORE,									//�������
	SUB_C_SET_CELL_SCORE,								//��Ԫ����
	SUB_C_HIT_FISH,										//������Ⱥ

	enClientSubCmd_End
};

//��Ԫ����
struct CMD_C_SetCellScore
{
	LONG							lCellScore;								//��Ԫ����
};

//�����ӵ�
struct CMD_C_BuyBullet
{
	enBulletCountKind				BulletCountKind;						//��Ŀ����
};

//�����ڵ�
struct CMD_C_UserShoot
{
	enBulletCountKind				BulletCountKind;						//�ӵ�����
	BYTE							byShootCount;							//�����Ŀ
	float							fAngle;									//����Ƕ�
	DWORD							dwBulletID;								//�ӵ���ʶ
};

//������Ⱥ
struct CMD_C_HitFish
{
	DWORD							dwFishID;								//��Ⱥ��ʶ
	DWORD							dwBulletID;								//�ӵ���ʶ
};

//���ø���
struct CMD_C_SetProbability
{
	BYTE							byCptrProbability[5][enFishKind_No];	//�������
	BYTE							byError;								//������ʾ
};
////������Ⱥ
//struct CMD_C_CaptureFish
//{
//	DWORD							dwFishID;								//��Ⱥ��ʶ
//	BYTE							byCptrProbability;						//�������
//	DWORD							dwBulletID;								//�ӵ���ʶ
//	BYTE							byBulletCount;							//�ӵ���Ŀ
//};
//////////////////////////////////////////////////////////////////////////

#endif