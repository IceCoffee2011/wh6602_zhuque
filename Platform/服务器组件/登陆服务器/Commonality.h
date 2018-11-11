#ifndef COMMONALITY_HEAD_FILE
#define COMMONALITY_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//���ݿ������

//���ݿ������ʶ
#define	DBR_GP_LOGON_BY_ACCOUNTS	1									//�ʺŵ�¼
//#define	DBR_GP_LOGON_BY_USERID	2									//I D ��¼
//#define DBR_GP_REGISTER_ACCOUNTS	3									//ע���ʺ�    
#define DBR_GP_USER_UPLOAD_FACE		4									//�ϴ�ͷ��
#define DBR_GP_USER_LEAVE			5									//����뿪
#define DBR_GP_DOWNLOAD_FACE		6									//����ͷ��
#define DBR_GP_DELETE_FACE			7									//ɾ��ͷ��
#define DBR_GP_MODIFY_INDIVIDUAL	8									//�޸�����

#define DBR_GP_READ_GRADE_INFO      10                                  //�û��ȼ�        �������
#define DBR_GP_UPDATE_USERINFO      11                                  //�����û���Ϣ    �������

#define DBR_GP_GET_GIFT_GLOD		12                                  //�õ���ѵĽ��  �������
#define DBR_GP_CONTINUE_CONNECT		13									//�������������ݿ�	Author:<cxf>

//���ݿ������ʶ
#define DBR_GP_LOGON_SUCCESS		100									//��¼�ɹ�
#define DBR_GP_LOGON_ERROR			101									//��¼ʧ��
#define DBR_GP_DOWNLOADFACE_SUCCESS	102									//���سɹ�
#define DBR_GP_UPLOAD_FACE_RESULT	103									//�ϴ����
#define DBR_GP_DELETE_FACE_RESULT	104									//ɾ�����
#define DBR_GP_MODIFY_RESULT		105									//�޸Ľ��

#define DBR_GP_UPDATE_USER_SUCC     106                                 //�����û���Ϣ�ɹ� �������
#define DBR_GP_UPDATE_USER_FAIL     107                                 //�����û���Ϣʧ�� �������


#define DBR_GR_GRADE_INFO_NUM       120                                 //�û��ȼ���Ϣ     �������
#define DBR_GR_GRADE_INFO_ONE       121                                 //һ���û��ȼ���Ϣ �������

#define DBR_GP_GIFT_MONEY_RESULT    125                                 //�����ȡ���


//�������
//�������
typedef    CMD_GP_GiftGold          DBR_GP_GiftGold;
typedef    CMD_GP_GiftGoldResult    DBR_GP_GiftGoldResult;   

//�ʺŵ�¼
struct DBR_GP_LogonByAccounts
{
	DBR_GP_LogonByAccounts()
	{
			memset(this, 0, sizeof(*this));
	}
	DWORD							dwClientIP;							//���ӵ�ַ
	TCHAR							szAccounts[NAME_LEN];				//��¼�ʺ�
	TCHAR							szPassWord[PASS_LEN];				//��¼����
	TCHAR							szComputerID[COMPUTER_ID_LEN];		//��������
};

//ID ��¼
struct DBR_GP_LogonByUserID
{
	DBR_GP_LogonByUserID()
	{
			memset(this, 0, sizeof(*this));
	}
	DWORD							dwClientIP;							//���ӵ�ַ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassWord[PASS_LEN];				//��¼����
	TCHAR							szComputerID[COMPUTER_ID_LEN];		//��������
};

//�û���Ϣ��ѯ�ṹ
typedef CMD_GP_UpdateUserInfo       DBR_GP_UpdateUserInfo;

//�û���Ϣ���³ɹ��ṹ
struct DBR_GP_UserInfoSucc
{
	DBR_GP_UserInfoSucc()
	{
			memset(this, 0, sizeof(*this));
	}
	WORD								wFaceID;						//ͷ������
	BYTE								cbGender;						//�û���
	TCHAR								szNickName[NAME_LEN];	        //�ǳ�
	LONGLONG							lMoney;							//��Ϸ���
	LONGLONG                            lGold;                          //�û��Ľ�� 
	LONGLONG                            lGem;                           //�û��ı�ʯ 
};

//�ʺ�ע��
struct DBR_GP_RegisterAccounts
{
	DBR_GP_RegisterAccounts()
	{
			memset(this, 0, sizeof(*this));
	}
	WORD							wFaceID;							//ע��ͷ��
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwClientIP;							//���ӵ�ַ
	TCHAR							szSpreader[NAME_LEN];				//�Ƽ�����
	TCHAR							szAccounts[NAME_LEN];				//��¼�ʺ�
	TCHAR							szPassWord[PASS_LEN];				//��¼����
	TCHAR							szComputerID[COMPUTER_ID_LEN];		//��������
	TCHAR                           szEmail[EMAIL_LEN];                 //email����  �������
};


//��¼�ɹ�
struct DBR_GP_LogonSuccess
{
	DBR_GP_LogonSuccess()
	{
			memset(this, 0, sizeof(*this));
	}
	
	//��������
	DWORD			dwUserID;									//�û���ʶ
	DWORD			dwGameID;								//��Ϸ��ʶ
	DWORD			dwExperience;							//������ֵ
	TCHAR				szPassword[PASS_LEN];			//��¼����
	TCHAR				szAccounts[NAME_LEN];			//��¼�ʺ�
	TCHAR				szGroupName[GROUP_LEN];	//��������
	
	//�û�����
	WORD				wFaceID;							//ͷ���ʶ
	BYTE					cbGender;							//�û��Ա�
	DWORD			dwCustomFaceVer;			//ͷ��汾
	BYTE					cbMember;						//��Ա�ȼ�
	TCHAR				szUnderWrite[UNDER_WRITE_LEN];		//����ǩ��

	//������Ϣ
	TCHAR				szDescribeString[128];				//������Ϣ
	DWORD           dwLockServerID;                     //�����ķ�����id����Ϊ0���Ǳ�����         �������

	//�������
	TCHAR				szNickName[UNDER_WRITE_LEN];	   //����ǩ��
	LONGLONG		lMoney;							   //�ر���
	LONGLONG    lGold;                             //�û��Ľ��  2011-9-30ǰ����ΪlGems
	LONGLONG    lGem;                              //�û��ı�ʯ  2011-9-30ǰ����ΪlHonor
	DWORD          dwGrade;                           //�û��ĵȼ���������256�� 2011-9-30ǰΪdwClass

	LONG				lWinCount;							//ʤ������
	LONG				lLostCount;							//ʧ������
	LONG				lDrawCount;							//�;�����
	LONG				lFleeCount;							//������Ŀ
	USER_TYPE       utIsAndroid;                        //�Ƿ��ǻ�����
	TCHAR              szKey[33];                          //��վ�õ�key
	LONG               lGiftNum;                           //��ȡ���͵Ĵ�����Ŀǰ��һ��һ�Σ�
	TCHAR				szHashID[33];						// ��Ƭ�����õİ汾��
	int					nMasterRight;						//����Ȩ��
	int					nMasterOrder;						//Ȩ�޵ȼ�

	TCHAR				szComputerID[COMPUTER_ID_LEN];		//��������
	//��ӽ���
};


//�ȼ���Ϣ�ṹ
struct DBR_GR_gradeInfo
{
	DBR_GR_gradeInfo()
	{
			memset(this, 0, sizeof(*this));
	}
	int   id;
	TCHAR szName[24];
	int   iMaxExp;
};
//��ӽ���

//��¼ʧ��
struct DBR_GP_LogonError
{
	DBR_GP_LogonError()
	{
			memset(this, 0, sizeof(*this));
	}
	LONG							lErrorCode;							//�������
	TCHAR							szErrorDescribe[128];				//������Ϣ
};

//�ϴ�ͷ��
struct DBR_GP_UploadCustomFace
{
	DBR_GP_UploadCustomFace()
	{
			memset(this, 0, sizeof(*this));
	}
	DWORD							dwUserID;							//��� ID
};

//�ϴ����
struct DBR_GP_UploadFaceResult
{
	DBR_GP_UploadFaceResult()
	{
			memset(this, 0, sizeof(*this));
	}
	TCHAR							szDescribeMsg[128];					//������Ϣ
	DWORD							dwFaceVer;							//ͷ��汾
	bool							bOperateSuccess;					//�ɹ���ʶ
};

//ɾ�����
struct DBR_GP_DeleteFaceResult
{
    DBR_GP_DeleteFaceResult()
	{
		memset(this, 0, sizeof(*this));
	}
	TCHAR							szDescribeMsg[128];					//������Ϣ
	DWORD							dwFaceVer;							//ͷ��汾
	bool							bOperateSuccess;					//�ɹ���ʶ
};

//����ͷ��
struct DBR_GP_DownloadCustomFace
{
	DBR_GP_DownloadCustomFace()
	{
		memset(this, 0, sizeof(*this));
	}
	DWORD							dwUserID;							//��� ID
};

//���سɹ�
struct DBR_GP_DownloadFaceSuccess
{
	DBR_GP_DownloadFaceSuccess()
	{
		memset(this, 0, sizeof(*this));
	}
	DWORD							dwToltalSize;						//�ܹ���С
	DWORD							dwCurrentSize;						//��ǰ��С
	DWORD							dwUserID;							//��� ID
	BYTE							bFaceData[2048];					//ͷ������
};

//ɾ����Ϣ
struct DBR_GP_CustomFaceDelete
{
	DBR_GP_CustomFaceDelete()
	{
		memset(this, 0, sizeof(*this));
	}
	DWORD							dwUserID;							//��� ID
	DWORD							dwFaceVer;							//ͷ��汾
};

//��������
struct DBR_GP_ModifyIndividual
{
	DBR_GP_ModifyIndividual()
	{
		memset(this, 0, sizeof(*this));
	}
	DWORD							dwUserID;							//��� ID
	TCHAR							szNickname[NAME_LEN];				//����ǳ�
	int								nGender;							//����Ա�
	int								nAge;								//�������
	TCHAR							szAddress[64];						//��ҵ�ַ
	TCHAR							szUnderWrite[32];					//����ǩ��
	TCHAR							szPassword[33];						//�������
};

//�޸Ľ��
struct DBR_GP_ModifyIndividualResult
{
	DBR_GP_ModifyIndividualResult()
	{
		memset(this, 0, sizeof(*this));
	}
	TCHAR							szDescribeMsg[128];					//������Ϣ
	bool							bOperateSuccess;					//�ɹ���ʶ
};

//////////////////////////////////////////////////////////////////////////

#endif