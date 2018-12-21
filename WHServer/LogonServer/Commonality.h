#ifndef COMMONALITY_HEAD_FILE
#define COMMONALITY_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include <vector>
using std::vector;

//////////////////////////////////////////////////////////////////////////
//���ݿ������

//���ݿ������ʶ
#define	DBR_GP_LOGON_BY_ACCOUNTS	1									//�ʺŵ�¼
#define	DBR_GP_LOGON_BY_USERID		2									//I D ��¼
#define DBR_GP_REGISTER_ACCOUNTS	3									//ע���ʺ�
#define DBR_GP_USER_UPLOAD_FACE		4									//�ϴ�ͷ��
#define DBR_GP_USER_LEAVE			5									//����뿪
#define DBR_GP_DOWNLOAD_FACE		6									//����ͷ��
#define DBR_GP_DELETE_FACE			7									//ɾ��ͷ��
#define DBR_GP_MODIFY_INDIVIDUAL	8									//�޸�����
#define DBR_GP_USER_SIGNIN			9									//ǩ��
#define DBR_GP_USER_EXCHANGE_INFO			10							//�һ��ĸ�����Ϣ
#define DBR_GP_USER_EXCHANGE_PRODUCT		11							//�һ��Ĳ�Ʒ�б�
#define DBR_GP_USER_EXCHANGE				12							//�һ�
#define DBR_GP_USER_EXCHANGE_RECORD			13							//�һ��ļ�¼
#define DBR_GP_USER_GETAWARD		14									//�콱
#define DBR_GP_CHECKID				15									//У������
#define DBR_GP_CHECK_NICKNAME		16									//У���ǳ�
#define DBR_GP_REGISTER				17									//ע��
#define DBR_GP_ID_UPDATE			18									//�ʺ�����
#define DBR_GP_RESET_PWD			19									//��������
#define DBR_GP_USER_INFO			20									//��ѯ�û���Ϣ
#define DBR_GP_USER_CAN_SPREAD		21									//�û��ܷ���ȡ�ƹ㽱��
#define DBR_GP_USER_GET_SPREAD		22									//�û���ȡ�ƹ㽱��
#define DBR_GP_USER_SPREADER_INFO	23									//�ƹ�Ա��Ϣ
#define DBR_GP_USER_GET_BBS_POP		24									//��ѯ����������Ϣ
#define DBR_GP_USER_GET_BBS_SCROLL		25								//��ѯ����������Ϣ
#define DBR_GP_USER_GET_BBS_LIST		26								//��ѯ�б�����Ϣ
#define DBR_GP_USER_GET_TASK_INFO		27								//��ѯ��ҵ�����ͳ����Ϣ
#define DBR_GP_USER_GET_BANKING			28								//��ѯ��ҵ����а�
#define DBR_GP_USER_FEEDBACK			29								//��ҷ������
#define DBR_GP_USER_GET_FEEDBACK		30								//��ѯ������
#define DBR_GP_USER_INFO_ID_CARD		31								//��ѯ�û����֤�˺���Ϣ
#define DBR_GP_USER_AUTHENTICATION		32								//�����ύ���֤��Ϣ(���������֤��)
#define DBR_GP_USER_MODIFY_RANKINGSTATUS		33						//�޸�����Ƿ������а�
#define DBR_GP_USER_BANK_INFO			34								//��ѯ���������Ϣ
#define DBR_GP_USER_BANK_CHARGE			35								//��Ҳ�������
#define DBR_GP_USER_MALL_PRODUCT		36								//�����̳���Ʒ
#define DBR_GP_USER_MALL_PLACE_ORDER	37								//�����̳��¶���
#define DBR_GP_USER_CANCEL_ORDER		38								//����ȡ��������
#define DBR_GP_USER_MALL_BUY			39								//�����̳���Ʒ������
#define DBR_GP_USER_MALL_UPDATE			40								//�����̳ǲ�ѯ�����Ÿ��½��
#define DBR_GP_USER_MALL_PRODUCT_IOS	41								//�����̳���Ʒ(ios)

#define DBR_GP_USER_ACTIVITY_LIST			43							//��б�
#define DBR_GP_USER_ACTIVITY				44							//ָ���
#define DBR_GP_USER_ACTIVITY_LUCKY			45							//��齱
#define DBR_GP_USER_ACTIVITY_RECORD			46							//�н���¼

#define DBR_GP_USER_EXCHANGE_PRODUCT_V2		47							//�һ��Ĳ�Ʒ�б�
#define DBR_GP_USER_EXCHANGE_V2				48							//�һ�

#define DBR_GP_USER_SPEAKER_SEND			49							//��������
#define DBR_GP_USER_QUERY_CONFIG			50							//��ѯ������

#define DBR_GP_MODIFY_UNDERWRITE	            51							//�޸ĸ���ǩ��
#define DBR_GP_REGISTER_ACCOUNTS_WX			52						    //ע���ʺ�WX

#define DBR_GP_USER_GET_BANKING_V2			53							//��ѯ��ҵ����а�

#define DBR_GP_MODIFY_PHONE	                54							//�޸��ֻ���
#define DBR_GP_WXUPDATE			            55						    //������WX�ʺ�

//���ݿ������ʶ
#define DBR_GP_LOGON_SUCCESS		100									//��¼�ɹ�
#define DBR_GP_LOGON_ERROR			101									//��¼ʧ��
#define DBR_GP_DOWNLOADFACE_SUCCESS	102									//���سɹ�
#define DBR_GP_UPLOAD_FACE_RESULT	103									//�ϴ����
#define DBR_GP_DELETE_FACE_RESULT	104									//ɾ�����
#define DBR_GP_MODIFY_RESULT		105									//�޸Ľ��
#define DBR_GP_SIGNIN_RESULT		106									//ǩ�����
#define DBR_GP_EXCHANGE_INFO_RESULT			107							//�����ĸ�����Ϣ
#define DBR_GP_EXCHANGE_PRODUCT_RESULT		108							//�һ��Ĳ�Ʒ
#define DBR_GP_EXCHANGE_RESULT				109							//�һ�
#define DBR_GP_EXCHANGE_RECORD_RESULT		110							//�һ��ļ�¼
#define DBR_GP_GETAWARD_RESULT		111									//�콱���
#define DBR_GP_CHECKID_RESULT		112									//У�����ƽ��
#define DBR_GP_CHECK_NICKNAME_RESULT	113								//У���ǳ�
#define DBR_GP_REGISTER_RESULT			114								//ע��
#define DBR_GP_ID_UPDATE_RESULT			115								//�ʺ�����
#define DBR_GP_RESET_PWD_RESULT			116								//��������
#define DBR_GP_USER_INFO_RESULT			117								//��ѯ�û���Ϣ
#define DBR_GP_USER_CAN_SPREAD_RESULT			118						//�û��ܷ���ȡ�ƹ㽱��
#define DBR_GP_USER_GET_SPREAD_RESULT			119						//�û���ȡ�ƹ㽱��
#define DBR_GP_USER_SPREADER_INFO_RESULT		120						//�ƹ�Ա��Ϣ
#define DBR_GP_USER_GET_BBS_POP_RESULT			121						//��ѯ����������Ϣ
#define DBR_GP_USER_GET_BBS_SCROLL_RESULT		122						//��ѯ����������Ϣ
#define DBR_GP_USER_GET_BBS_LIST_RESULT			123						//��ѯ�б�����Ϣ
#define DBR_GP_USER_GET_TASK_INFO_RESULT		124						//��ѯ��ҵ�����ͳ����Ϣ
#define DBR_GP_USER_GET_RANKING_RESULT			125						//���а�ļ�¼
#define DBR_GP_USER_FEEDBACK_RESULT				126						//��ҷ�������Ľ��
#define DBR_GP_USER_GET_FEEDBACK_RESULT			127						//��ѯ�������Ľ��
#define DBR_GP_USER_INFO_ID_CARD_RESULT			128						//��ѯ�û����֤�˺���Ϣ�Ľ��
#define DBR_GP_USER_AUTHENTICATION_RESULT		129						//�����ύ���֤��Ϣ�Ľ��
#define DBR_GP_USER_GET_RANKINGSTATUS			130						//��ѯ����Ƿ������а�
#define DBR_GP_USER_GET_MODIFY_RANKINGSTATUS	131						//�޸�����Ƿ������а񷵻���Ϣ

#define DBR_GP_USER_MALL_PRODUCT_RESULT			132						//�̳���Ʒ�б���Ϣ���
#define DBR_GP_USER_MALL_PLACE_ORDER_RESULT		133						//�̳��¶������
#define DBR_GP_USER_CANCEL_ORDER_NUM			134						//����ȡ��������
#define DBR_GP_USER_MALL_BUY_RESULT				135						//�̳���Ʒ������
#define DBR_GP_USER_MALL_UPDATE_RESULT			136						//�̳ǲ�ѯ�����Ÿ��½��

#define DBR_GP_USER_BANK_INFO_RESULT			137						//��ѯ���������Ϣ���
#define DBR_GP_USER_BANK_CHARGE_RESULT			138						//��Ҳ������н��
#define DBR_GP_EXCHANGE_PRODUCT_RESULT_V2		139						//�һ��Ĳ�Ʒ
#define DBR_GP_EXCHANGE_RESULT_V2				140						//�һ�

#define DBR_GP_USER_ACTIVITY_LIST_RESULT		141						//��ѯ��б���Ϣ���
#define DBR_GP_USER_ACTIVITY_RESULT				142						//��ѯ���Ϣ���
#define DBR_GP_USER_ACTIVITY_LUCKY_RESULT		143						//��齱���
#define DBR_GP_USER_ACTIVITY_RECORD_RESULT		144						//��ѯ��б���Ϣ���

#define DBR_GP_USER_SPEAKER_SEND_RESULT			145						//�������Ƚ��
#define DBR_GP_USER_QUERY_CONFIG_RESULT			146						//��ѯ��������

#define DBR_GP_MODIFY_UNDERWRITE_RESULT		    147						//�޸ĸ���ǩ�����
#define DBR_GP_USER_GET_RANKING_RESULT_V2		148						//���а�ļ�¼

#define DBR_GP_MODIFY_PHONE_RESULT		        149						//�޸��ֻ��Ž��

//�ʺŵ�¼
struct DBR_GP_LogonByAccounts
{
	DWORD							dwClientIP;							//���ӵ�ַ
	TCHAR							szAccounts[NAME_LEN];				//��¼�ʺ�
	TCHAR							szPassWord[PASS_LEN];				//��¼����
	TCHAR							szComputerID[COMPUTER_ID_LEN];		//��������
};

//ID ��¼
struct DBR_GP_LogonByUserID
{
	DWORD							dwClientIP;							//���ӵ�ַ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassWord[PASS_LEN];				//��¼����
	TCHAR							szComputerID[COMPUTER_ID_LEN];		//��������
};

//�ʺ�ע��
struct DBR_GP_RegisterAccounts
{
	WORD							wFaceID;							//ע��ͷ��
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwClientIP;							//���ӵ�ַ
	TCHAR							szSpreader[NAME_LEN];				//�Ƽ�����
	TCHAR							szAccounts[NAME_LEN];				//��¼�ʺ�
	TCHAR							szPassWord[PASS_LEN];				//��¼����
	TCHAR							szComputerID[COMPUTER_ID_LEN];		//��������
};

//WX�ʺ�ע��
struct DBR_GP_RegisterAccountsWX
{
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwClientIP;							//���ӵ�ַ

	TCHAR							szSpreader[NAME_LEN];				//�Ƽ�����

	TCHAR							szNickName[NAME_LEN];				//�ǳ�
	TCHAR							szOpenID[32];						//΢��Ψһ��ʶ
	TCHAR							szUnionID[64];						//΢��Ψһ��ʶ
	TCHAR							szFaceUrl[256];						//ͷ���ַ

	TCHAR							szAccounts[NAME_LEN];				//��¼�ʺ�
	TCHAR							szPassWord[PASS_LEN];				//��¼����
	TCHAR							szWXComputerID[COMPUTER_ID_LEN];	//������(΢����Ϣ��Ψһ����)
	TCHAR							szComputerID[COMPUTER_ID_LEN];		//��������
};

//������WX�ʺ�
struct DBR_GP_WXUpdate
{
	DWORD							dwUserID;							//�û���ʶ
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwClientIP;							//���ӵ�ַ
	TCHAR							szNickName[NAME_LEN];				//�ǳ�
	TCHAR							szOpenID[32];						//΢��Ψһ��ʶ
	TCHAR							szUnionID[64];						//΢��Ψһ��ʶ
	TCHAR							szFaceUrl[256];						//ͷ���ַ
	TCHAR							szWXComputerID[COMPUTER_ID_LEN];	//������(΢����Ϣ��Ψһ����)
	TCHAR							szComputerID[COMPUTER_ID_LEN];		//��������
};

//��¼�ɹ�
struct DBR_GP_LogonSuccess
{
	//��������
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwGameID;							//��Ϸ��ʶ
	DWORD							dwExperience;						//������ֵ
	LONG							lGoldScore;						//�Ƹ�
	LONG							lWelfare;						//�ͱ�
	WORD							wWelfareLost;						//�ͱ�ʣ�����
	TCHAR							szPassword[PASS_LEN];				//��¼����
	TCHAR							szAccounts[NAME_LEN];				//��¼�ʺ�
	TCHAR							szNickName[NAME_LEN];				//�ǳƺ�
	TCHAR							szGroupName[GROUP_LEN];				//��������

	//�û�����
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwCustomFaceVer;					//ͷ��汾
	BYTE							cbIsGuest;							//�Ƿ�Ϊ�ο��ʺ�
	BYTE							cbHasGuest;							//�Ƿ����ο��ʺ�
	BYTE							cbMember;							//��Ա�ȼ�
	TCHAR							szUnderWrite[UNDER_WRITE_LEN];		//����ǩ��

	//������Ϣ
	TCHAR							szDescribeString[128];				//������Ϣ
	TCHAR							szRule[512];						//���ù���
};

//��¼ʧ��
struct DBR_GP_LogonError
{
	LONG							lErrorCode;							//�������
	TCHAR							szErrorDescribe[128];				//������Ϣ
};

//�ϴ�ͷ��
struct DBR_GP_UploadCustomFace
{
	DWORD							dwUserID;							//��� ID
};

//�ϴ����
struct DBR_GP_UploadFaceResult
{
	TCHAR							szDescribeMsg[128];					//������Ϣ
	DWORD							dwFaceVer;							//ͷ��汾
	bool							bOperateSuccess;					//�ɹ���ʶ
};

//ɾ�����
struct DBR_GP_DeleteFaceResult
{
	TCHAR							szDescribeMsg[128];					//������Ϣ
	DWORD							dwFaceVer;							//ͷ��汾
	bool							bOperateSuccess;					//�ɹ���ʶ
};

//����ͷ��
struct DBR_GP_DownloadCustomFace
{
	DWORD							dwUserID;							//��� ID
};

//ǩ��
struct DBR_GP_UserSignIn
{
	DWORD							dwUserID;							//��� ID
};

//ǩ�����
struct DBR_GP_UserSignInResult
{
	TCHAR							szDescribeMsg[128];					//������Ϣ
	unsigned short					wRankAward;
	unsigned short					wRankNum;							
	unsigned short					wAllSend;						
	unsigned short					wTodaySend;						
	unsigned short					wTimes;							
	unsigned char					cbTodaySign;
	DWORD							dwGoldScore;
	bool							bOperateSuccess;					//�ɹ���ʶ
};

//�콱
struct DBR_GP_UserGetAward
{
	DWORD							dwUserID;							//��� ID
	DWORD							dwTaskID;							//���� ID
};

//�콱���
struct DBR_GP_UserGetAwardResult
{
	DWORD							dwTaskID;
	DWORD							dwScore;				
	unsigned char					cbStatus;
	TCHAR							szDescribeMsg[128];					//������Ϣ
};

//�һ�������Ϣ����
struct DBR_GP_UserExchangeInfo
{
	DWORD							dwUserID;						//��� ID
	TCHAR							szPhone[32];					//�绰
};

//�һ�������Ϣ������
struct DBR_GP_UserExchangeInfoResult
{
	char							szPhone[32];					//�绰
	DWORD							dwGold;							//��
	bool							bOperateSuccess;				//�ɹ���ʶ
};

//�һ���Ʒ����
struct DBR_GP_UserExchangeProduct
{
	DWORD							dwUserID;						//��� ID
};

//��Ʒ��Ϣ
struct DBR_GP_ExchangeProduct
{
	DWORD							dwAwardID;					//��Ʒ ID
	DWORD							dwPrice;						//�۸�
	DWORD							dwLeft;						//ʣ����
	TCHAR							szAwardImg[128];			//ͼƬ��ַ
	TCHAR							szAwardname[32];			//ͼƬ��ַ
	DWORD							dwType;						//��Ʒ����:1Ϊ���Զ����ŵģ�Ĭ��Ϊ0	
};

//�һ�������Ϣ������
struct DBR_GP_UserExchangeProductResult
{
	vector<DBR_GP_ExchangeProduct>	vecProducts;					//��Ʒ�б�
	bool							bOperateSuccess;				//�ɹ���ʶ
};

//�һ�
struct DBR_GP_UserExchange
{
	DWORD							dwUserID;						//��� ID
	DWORD							dwAwardID;						//��Ʒ ID
};

//�һ�������Ϣ������
struct DBR_GP_UserExchangeResult
{
	DWORD							dwGold;							//��
	DWORD							dwAwardID;						//��Ʒ ID
	bool							bOperateSuccess;				//�ɹ���ʶ
	DWORD							dwScore;						//��ǰ���ֶ�����	
};

//�һ���Ʒ����
struct DBR_GP_UserExchangeRecord
{
	DWORD							dwUserID;						//��� ID
	int								nPage;							//ҳ��
};

//�һ���¼
struct DBR_GP_ExchangeRecord
{
	DWORD							dwTotal;						//�ܼ�
	DWORD							dwID;							//ID
	DWORD							dwAwardID;						//��Ʒ ID
	//DWORD							dwDate;							//�һ�ʱ��
	TCHAR							szStatus[32];					//״̬
	TCHAR							szAwardName[32];				//����
	TCHAR							szDate[32];						//�һ�ʱ��
};

//�һ�������Ϣ������
struct DBR_GP_UserExchangeRecordResult
{
	vector<DBR_GP_ExchangeRecord>	vecRecords;						//��¼�б�
	bool							bOperateSuccess;				//�ɹ���ʶ
};


//���سɹ�
struct DBR_GP_DownloadFaceSuccess
{
	DWORD							dwToltalSize;						//�ܹ���С
	DWORD							dwCurrentSize;						//��ǰ��С
	DWORD							dwUserID;							//��� ID
	BYTE							bFaceData[2048];					//ͷ������
};

//ɾ����Ϣ
struct DBR_GP_CustomFaceDelete
{
	DWORD							dwUserID;							//��� ID
	DWORD							dwFaceVer;							//ͷ��汾
};

//��������
struct DBR_GP_ModifyIndividual
{
	TCHAR							szAccount[NAME_LEN];				//����ǳ�
	TCHAR							szNickname[NAME_LEN];				//����ǳ�
	int								nGender;							//����Ա�
	TCHAR							szPhone[NAME_LEN];					//��ҵ绰
};

//�޸Ľ��
struct DBR_GP_ModifyIndividualResult
{
	TCHAR							szDescribeMsg[128];					//������Ϣ
	TCHAR							szAccount[NAME_LEN];				//�������
	int								iCode;								//�����롣 0Ϊ�ɹ�
};

//����ǩ��
struct DBR_GP_ModifyUnderWrite
{
	unsigned int	dwUserID;		//���ID
	TCHAR			szDescribe[UNDER_WRITE_LEN];				//����ǩ��
};

//�޸Ľ��
struct DBR_GP_ModifyUnderWriteResult
{
	TCHAR							szDescribeMsg[128];					//������Ϣ
	TCHAR							szAccount[NAME_LEN];				//�������
	int								iCode;								//�����롣 0Ϊ�ɹ�
};

//�ֻ���
struct DBR_GP_ModifyPhone
{
	unsigned int	dwUserID;		//���ID
	TCHAR			szPhone[31];	//�ֻ���
};

//�޸Ľ��
struct DBR_GP_ModifyPhoneResult
{
	TCHAR							szDescribeMsg[128];					//������Ϣ
	TCHAR							szAccount[NAME_LEN];				//�������
	TCHAR							szPhone[31];				        //�ֻ���
	int								iCode;								//�����롣 0Ϊ�ɹ�
};

//����У��
struct DBR_GP_CheckID
{
	TCHAR							szAccount[NAME_LEN];				//�������
};

//�ǳ�У��
struct DBR_GP_CheckNickName
{
	TCHAR							szNickName[NAME_LEN];				//����ǳ�
};

//ע��
struct DBR_GP_Register
{
	TCHAR							szAccount[NAME_LEN];				//�������
	TCHAR							szNickName[NAME_LEN];				//����ǳ�
	TCHAR							szPhone[NAME_LEN];					//����ֻ�
	TCHAR							szPassword[PASS_LEN];				//����
	int								nGender;							//����Ա�
	DWORD							dwClientIP;							//���ӵ�ַ
	TCHAR							szComputerID[COMPUTER_ID_LEN];		//��������
	TCHAR							szChannel[NAME_LEN];		//������
};

//����
struct DBR_GP_IDUpdate
{
	TCHAR							szAccount[NAME_LEN];				//�������
	TCHAR							szAccountOld[NAME_LEN];				//�������
	TCHAR							szNickName[NAME_LEN];				//����ǳ�
	TCHAR							szPassword[PASS_LEN];				//����
	int								nGender;							//����Ա�
};

//��������
struct DBR_GP_ResetPwd
{
	TCHAR							szAccount[NAME_LEN];				//�������
	TCHAR							szPassword[PASS_LEN];				//����
};

//��ѯ�û���Ϣ
struct DBR_GP_UserInfo
{
	TCHAR							szAccount[NAME_LEN];				//�������
};
struct DBR_GP_UserInfo_Result
{
	TCHAR							szAccount[NAME_LEN];				//�������
	TCHAR							szNickName[NAME_LEN];				//����ǳ�
	TCHAR							szPhone[NAME_LEN];					//����ֻ�
	int								nGender;							//����Ա�
	TCHAR							szDescribeMsg[128];					//������Ϣ
	int								iCode;								//�����롣 0Ϊ�ɹ�
};

//ͨ�ý��
struct DBR_GP_CommonResult
{
	TCHAR						szDescribeMsg[128];		//������Ϣ
	TCHAR						szAccount[NAME_LEN];				//�������
	int							iCode;					//�����롣 0Ϊ�ɹ�
	DWORD						dwUserID;							//��� ID
};

//�û��ܷ���ȡ�ƹ㽱��
struct DBR_GP_UserCanSpread
{
	DWORD							dwUserID;							//��� ID
	TCHAR							szComputerID[COMPUTER_ID_LEN];		//��������
};

//�û���ȡ�ƹ㽱��
struct DBR_GP_UserGetSpread
{
	DWORD							dwUserID;							//��� ID
	DWORD							dwSpreaderID;						//�ƹ�Ա ID
	TCHAR							szComputerID[COMPUTER_ID_LEN];		//��������
};
//�û���ȡ�ƹ㽱�����
struct DBR_GP_UserGetSpreadResult
{
	TCHAR						szDescribeMsg[128];		//������Ϣ
	int							iCode;					//�����롣 0Ϊ�ɹ�
	DWORD						dwUserID;				//��� ID
	DWORD						dwSpreaderID;				//�ƹ�Ա ID
	DWORD						dwAmount;				//����
	DWORD						dwType;				//����
};

//�ƹ�Ա��Ϣ
struct DBR_GP_UserSpreaderInfo
{
	DWORD							dwSpreaderID;						//�ƹ�Ա ID
};
//�ƹ�Ա��Ϣ���
struct DBR_GP_UserSpreaderInfoResult
{
	TCHAR						szDescribeMsg[128];		//������Ϣ
	int							iCode;					//�����롣 0Ϊ�ɹ�
	DWORD						dwSpreaderID;			//�ƹ�Ա ID
	DWORD						dwAmount;				//������
	DWORD						dwUsers;					//������
	DWORD						dwNum;					//�����ǳ�����
	TCHAR						vecNickNames[10][NAME_LEN];	//�����ǳ�  --���10��
	short						iAward[10];					//ÿ�˵Ľ�����
};


//��ѯ����������Ϣ
struct DBR_GP_UserGetBbsPop
{
	TCHAR							szChannel[NAME_LEN];		//������
};
//����������Ϣ���
struct DBR_GP_UserGetBbsPopResult
{
	TCHAR						szDetails[512];			//����������ϸ����
	int							iAction;				//������
	int							iCode;					//�����롣 0Ϊ�ɹ�
	int							iScrollCount;			//�������������
	int							iListCount;				//�б��������
	DWORD						dwListID[3];			//�б����ID
};

//��ѯ����������Ϣ
struct DBR_GP_UserGetBbsScroll
{
	TCHAR							szChannel[NAME_LEN];		//������
};
//����������Ϣ���
struct DBR_GP_UserGetBbsScrollResult
{
	TCHAR						szDetails[10][100];		//����������ϸ����
	int							iCount;					//����
	int							iCode;					//�����롣 0Ϊ�ɹ�
};

//��ѯ�б�����Ϣ
struct DBR_GP_UserGetBbsList
{
	TCHAR							szChannel[NAME_LEN];		//������
};
//�б�����Ϣ���
struct DBR_GP_UserGetBbsListResult
{
	TCHAR						szTitle[3][100];		//�б������
	TCHAR						szDetails[3][512];		//�б�����ϸ����
	TCHAR						szDate[3][64];				//�б���ʱ��
	int							iID[3];					//�б���ID
	int							iType[3];				//�б�������
	int							iCount;					//����
	int							iCode;					//�����롣 0Ϊ�ɹ�
};

//��ѯ��ҵ�����ͳ����Ϣ
struct DBR_GP_UserGetTaskInfo
{
	DWORD							dwUserID;							//��� ID
};
//��ҵ�����ͳ����Ϣ���
struct DBR_GP_UserGetTaskInfoResult
{
	DWORD						dwUserID;				//��� ID
	int							iWinToday3;				//����Ӯ������
	int							iLostToday3;			//�����������
	int							iWinTotal3;				//Ӯ��������
	int							iLostTotal3;			//���������
	int							iAwardToday3;			//ÿ��������콱�׶���
	int							iAwardTotal3;			//ϵͳ������콱�׶���
	int							iWinToday4;				//����Ӯ������
	int							iLostToday4;			//�����������
	int							iWinTotal4;				//Ӯ��������
	int							iLostTotal4;			//���������
	int							iAwardToday4;			//ÿ��������콱�׶���
	int							iAwardTotal4;			//ϵͳ������콱�׶���
	int							iCode;					//�����롣 0Ϊ�ɹ�
};

//��ѯ��ҵ����а���Ϣ
struct DBR_GP_UserGetRanking
{
	DWORD						dwUserID;			//��� ID
	int							nPage;				//ҳ��
};
//���а��¼
struct DBR_GP_UserRankingRecord
{
	DWORD							dwUserID;						//ID
	SHORT							nIndex;							//����
	LONG							lGoldScore;						//�Ƹ�
	BYTE							cbGender;						//�û��Ա�
	TCHAR							szNickName[32];					//����
};

//���а��¼
struct DBR_GP_UserRankingRecord_V2
{
	DWORD							dwUserID;						//ID
	SHORT							nIndex;							//����
	LONG							lGoldScore;						//�Ƹ�
	BYTE							cbGender;						//�û��Ա�
	TCHAR							szNickName[32];					//����
	TCHAR							szDescribe[128];
};

//��ҵ����а���Ϣ���
struct DBR_GP_UserGetRankingResult
{
	vector<DBR_GP_UserRankingRecord>	vecRecords;						//��¼�б�
	DWORD							dwTotal;				//�ܼ�
	DWORD							dwUserID;				//�ҵ� ID
	SHORT							nMyIndex;				//�ҵ�����
	TCHAR							szDate[64];				//������ʱ��
	int								iCode;					//�����롣 0Ϊ�ɹ�
};

//��ҵ����а���Ϣ���
struct DBR_GP_UserGetRankingResult_V2
{
	vector<DBR_GP_UserRankingRecord_V2>	vecRecords;						//��¼�б�
	DWORD							dwTotal;				//�ܼ�
	DWORD							dwUserID;				//�ҵ� ID
	SHORT							nMyIndex;				//�ҵ�����
	TCHAR							szDate[64];				//������ʱ��
	int								iCode;					//�����롣 0Ϊ�ɹ�
};

//������а�״̬��Ϣ���
struct DBR_GP_UserGetRankingStatusResult
{
	unsigned int		dwUserID;					//��� ID
	char				isInRanking;				//����Ƿ����Ϊ1ʱ�μ�������Ϊ0ʱ���μ�����
};

//�޸�����Ƿ������а�
struct DBR_GP_UserModifyRankingStatus
{
	unsigned int		dwUserID;					//��� ID
	char				isInRanking;				//����Ƿ����Ϊ1ʱ�μ�������Ϊ0ʱ���μ�����
};

//�޸�����Ƿ������а񷵻ؽ��
struct DBR_GP_UserModifyRankingStatusResult
{
	DWORD				dwUserID;			//��� ID
	int					iCode;				//�����롣 0Ϊ�ɹ�
};

//��ҷ�������
struct DBR_GP_UserFeedBack
{
	DWORD				dwUserID;			//��� ID
	TCHAR				szPicture[128];		//ͼƬ��ַ
	TCHAR				szContent[372];		//�������
};
//��ҷ���������
struct DBR_GP_UserFeedBackResult
{
	int							iCode;				//�����롣 0Ϊ�ɹ�
	TCHAR						szDetails[128];		//����������ϸ����
};
//��ѯ��ҷ�������ļ�¼
struct DBR_GP_UserGetFeedBack
{
	DWORD						dwUserID;			//��� ID
	int							nPage;				//ҳ��
};
//��������ļ�¼
struct DBR_GP_UserFeedBackRecord
{
	TCHAR				szQuestion[512];	//��������
	TCHAR				szAnswer[256];		//��ά�ظ�
	TCHAR				szDate[32];			//�����ύʱ��
	BYTE				cbHavePic;			//�Ƿ��ͼƬ
	TCHAR				szPicture[128];		//ͼƬ��ַ
};
//��ҵ����а���Ϣ���
struct DBR_GP_UserGetFeedBackResult
{
	vector<DBR_GP_UserFeedBackRecord>	vecRecords;			//��¼�б�
	DWORD							dwTotal;				//�ܼ�
	DWORD							dwUserID;			//��� ID
	int								iCode;					//�����롣 0Ϊ�ɹ�
};
//��ѯ�û����֤�˺���Ϣ
struct DBR_GP_UserInfoIDCard
{
	DWORD	dwUserID;			//��� ID
};
//�����ύ���֤��Ϣ(���������֤��)
struct DBR_GP_UserAuthentication
{
	DWORD	dwUserID;				//��� ID
	TCHAR	szName[NAME_LEN];		//��������
	TCHAR	szIdCardNum[NAME_LEN];	//��ά�ظ�
};

//�̳���Ʒ����
struct DBR_GP_UserMallProduct
{
	DWORD	dwUserID;				//��� ID
	DWORD	dwVersion;				//�汾
};

//�̳���Ʒ��Ϣ
struct DBR_GP_UserMallProductResult
{
	unsigned int					version;			//�汾��
	unsigned int					productID;			//��Ʒ ID
	unsigned int					productPrice;			//�۸�
	char							cbHotFlag;			//������ʶ
	char							szProductName[32];	//��Ʒ����
	char							szProductNameAdd[32];	//��Ʒ����
	char							szProductImage[128];	//��ƷͼƬ
};

//�̳���Ʒ������
struct DBR_GP_UserMallProductResultFinish
{
	vector<DBR_GP_UserMallProductResult>	vecProducts;					//��Ʒ�б�
	bool							bOperateSuccess;				//�ɹ���ʶ
};

struct DBR_Cancel_Order_Num
{
	char orderNum[20];
};

struct DBR_Cancel_Order_Resut
{
	short iCode;
};


//�¶�������
struct DBR_GP_UserMallPlaceOrder
{
	unsigned int					dwUserID;		//��� ID
	unsigned int					dwProductID;	//��Ʒ ID
	char							szPayWay[10];	// ֧����ʽ
};
//�¶���������
struct DBR_GP_UserMallPlaceOrderResult
{
	short nResult;		//���
	char dwOrderID[20];	//����ID
	char token[128];	//token��Ϣ
	char productName[32]; //��Ʒ����
	int productPrice; //��Ʒ�۸�
	char billingCode[20]; // �Ʒ���
};
//��������
struct DBR_GP_UserMallBuy
{		
	char dwOrderID[20];	//����ID
};
//������
struct DBR_GP_UserMallBuyResult
{
	unsigned int	dwUserID;			//��� ID
	int				productID;
	unsigned int	dwGoldScore;	//�û��Ƹ�
	int				iCode;				//�����롣 0Ϊ�ɹ�
};
//������¹���
struct DBR_GP_UserMallUpdate
{		
	char dwOrderID[20];	//����ID
};
//���¹�����
struct DBR_GP_UserMallUpdateResult
{
	unsigned int	dwUserID;			//��� ID
	int				productID;
	unsigned int	dwGoldScore;	//�û��Ƹ�
	int				iCode;				//�����롣 0Ϊ�ɹ�
};


//��ѯ���������Ϣ����
struct DBR_GP_UserBankInfo
{
	unsigned int					dwUserID;		//��� ID
};
//��ѯ���������Ϣ���
struct DBR_GP_UserBankInfoResult
{
	short							nResult;		//���	
	unsigned int					dwUserID;		//��� ID
	unsigned int					dwCurScore;		//��ǰ������Ϸ����
	unsigned int					dwBankScore;	//���з���
};
//��Ҳ�����������
struct DBR_GP_UserBankCharge
{
	short							nOpCode;		//�����룺1Ϊ�棬2Ϊȡ	
	unsigned int					dwUserID;		//��� ID
	unsigned int					dwCurScore;		//��ǰ������Ϸ����
	unsigned int					dwBankScore;	//���з���
};
//��Ҳ������н��
struct DBR_GP_UserBankChargeResult
{
	short							nResult;		//���	
	unsigned int					dwUserID;		//��� ID
	unsigned int					dwCurScore;		//��ǰ������Ϸ����
	unsigned int					dwBankScore;	//���з���
};

//��б�����
struct DBR_GP_UserActivityListResult
{
	TCHAR						szTitle[5][50];			//����
	TCHAR						szDetails[5][100];		//����
	TCHAR						szLinkUrl[5][50];		//���ӵ�ַ
	TCHAR						szIconUrl[5][50];		//ͼ���ַ
	int							iID[5];					//�б���ID
	int							iStatus[5];				//״̬
	int							iType[5];				//����
	int							iCount;					//����
	int							iCode;					//�����롣 0Ϊ�ɹ�
};

//��ѯָ�����Ϣ
struct DBR_GP_UserActivity
{
	unsigned int					dwUserID;		//��� ID
	unsigned int					dwActivityID;	//�ID
};
//��ѯָ�����Ϣ���
struct DBR_GP_UserActivityResult
{
	int								iCode;			//�����롣 0Ϊ�ɹ�
	unsigned int					dwUserID;		//��� ID
	unsigned int					dwActivityID;	//�ID
	unsigned int					wCount;			//��������������5��
	TCHAR							szText[5][100];	//���������
	TCHAR							szResourceUrl[50];	//��Դ���ص�ַ
	TCHAR							szParam[128];	//����
};

//�齱
struct DBR_GP_UserActivityLucky
{
	unsigned int					dwUserID;		//��� ID
	unsigned int					dwActivityID;	//�ID
};
//�齱���
struct DBR_GP_UserActivityLuckyResult
{
	int								iCode;			//�����롣 0Ϊ�ɹ�
	unsigned int					dwUserID;		//��� ID
	unsigned int					dwActivityID;	//�ID
	unsigned int					dwPrizeID;		//��Ʒid
	unsigned int					dwPrice;		//��Ʒ��ֵ
	TCHAR							szImageUrl[50];	//��ƷͼƬ
	TCHAR							szPrizeName[50];//��Ʒ����
};

//�н���¼����
struct DBR_GP_UserActivityRecord
{
	DWORD							dwUserID;						//��� ID
	DWORD							dwActivityID;					//� ID
	int								nPage;							//ҳ��
};

//�н���¼
struct DBR_GP_ActivityRecord
{
	DWORD							dwTotal;						//�ܼ�
	DWORD							dwID;							//ID
	DWORD							dwAwardID;						//��Ʒ ID
	//DWORD							dwDate;							//�һ�ʱ��
	TCHAR							szStatus[32];					//״̬
	TCHAR							szAwardName[32];				//����
	TCHAR							szDate[32];						//�һ�ʱ��
};

//�н�������Ϣ������
struct DBR_GP_UserActivityRecordResult
{
	vector<DBR_GP_ActivityRecord>	vecRecords;						//��¼�б�
	bool							bOperateSuccess;				//�ɹ���ʶ
	DWORD							dwUserID;						//��� ID
	DWORD							dwActivityID;					//� ID
};

//����������
struct DBR_GP_UserSpeakerSend
{
	short							iType;					//�������ͣ�1�û���Ϣ��2ϵͳ��Ϣ
	DWORD							dwUserID;				//��� ID
	DWORD							nLen;					
	TCHAR							szDate[256];					//��������
};

//������������
struct DBR_GP_UserSpeakerSendResult
{
	short							iCode;					//�����롣 0Ϊ�ɹ�
	short							iType;					//�������ͣ�1�û���Ϣ��2ϵͳ��Ϣ
	unsigned int					dwScore;				//���������ϵĲƸ���
	DWORD							dwUserID;				//��� ID
	DWORD							nLen;					
	TCHAR							szDate[256];				//��������
};

//��ѯ����������
struct DBR_GP_UserQueryConfig
{
	DWORD							dwUserID;				//��� ID
	DWORD							dwVersion;					
	DWORD							dwConfigID;		
	TCHAR							szChannel[NAME_LEN];		//������		
};

//��ѯ��������
struct DBR_GP_UserQueryConfigResult
{
	short							iCode;
	DWORD							dwUserID;				//��� ID
	DWORD							dwVersion;					
	DWORD							dwLen;	
	TCHAR							szDate[256];			//����
};

//////////////////////////////////////////////////////////////////////////

#endif
