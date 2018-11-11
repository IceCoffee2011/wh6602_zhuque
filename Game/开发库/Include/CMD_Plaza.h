#ifndef CMD_PLAZA_HEAD_FILE
#define CMD_PLAZA_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

//�㳡�汾
#define VER_PLAZA_LOW					0								//�㳡�汾
#define VER_PLAZA_HIGH					1								//�㳡�汾
#define VER_PLAZA_FRAME					MAKELONG(VER_PLAZA_LOW,VER_PLAZA_HIGH)

//////////////////////////////////////////////////////////////////////////
//��¼�����ʶ

#define ERC_GP_LOGON_SUCCESS			0								//��½�ɹ�
#define ERC_GP_ACCOUNTS_NOT_EXIST		1								//�ʺŲ�����
#define ERC_GP_LONG_NULLITY				2								//��ֹ��¼
#define ERC_GP_PASSWORD_ERCOR			3								//�������

//////////////////////////////////////////////////////////////////////////
//��¼������

#define MDM_GP_LOGON					1								//�㳡��¼

#define SUB_GP_LOGON_ACCOUNTS			1								//�ʺŵ�¼
#define SUB_GP_LOGON_USERID				2								//I D ��¼
#define SUB_GP_REGISTER_ACCOUNTS		3								//ע���ʺ�
#define SUB_GP_UPLOAD_CUSTOM_FACE		4								//����ͷ��
#define SUB_GP_LOGON_USER_GRADE_INFO    6                               //�ȼ���Ϣ     �������

#define SUB_GP_LOGON_SUCCESS			100								//��½�ɹ�
#define SUB_GP_LOGON_ERROR				101								//��½ʧ��
#define SUB_GP_LOGON_FINISH				102								//��½���

//�ʺŵ�¼
struct CMD_GP_LogonByAccounts
{
	CMD_GP_LogonByAccounts()
	{
			memset(this, 0, sizeof(*this));
	}
	DWORD								dwPlazaVersion;					//�㳡�汾
	TCHAR								szAccounts[NAME_LEN];			//��¼�ʺ�
	TCHAR								szPassWord[PASS_LEN];			//��¼����
};

//I D ��¼
struct CMD_GP_LogonByUserID
{
	CMD_GP_LogonByUserID()
	{
			memset(this, 0, sizeof(*this));
	}
	DWORD								dwPlazaVersion;					//�㳡�汾
	DWORD								dwUserID;						//�û� I D
	TCHAR								szPassWord[PASS_LEN];			//��¼����
};

//�û���Ϣ��ѯ�ṹ                                                      //�������
struct CMD_GP_UpdateUserInfo
{
	CMD_GP_UpdateUserInfo()
	{
			memset(this, 0, sizeof(*this));
	}
	DWORD								dwUserID;						//�û� I D
	TCHAR								szPassWord[PASS_LEN];			//��¼����
};

//�������
struct CMD_GP_GiftGold
{
    CMD_GP_GiftGold()
	{
			memset(this, 0, sizeof(*this));
	}
	DWORD								dwUserID;						//�û� I D
	TCHAR								szPassWord[PASS_LEN];			//��¼����
	DWORD                               dwClientIP;                 //�û�ip
};

// �����·
struct CMD_GP_CheckLine
{
	CMD_GP_CheckLine()
	{
		memset(this, 0, sizeof(*this));
	}

	DWORD								dwUserID;						//�û� I D
	DWORD								dwClientIP;						//�û�ip
};

// ɾ��������
struct CMD_GP_DeleteMachineCode
{
	CMD_GP_DeleteMachineCode()
	{
		memset(this, 0, sizeof(*this));
	}

	tagClientSerial						clientSerial;
	TCHAR								szAccounts[NAME_LEN];			//��¼�ʺ�
};

//�������
struct CMD_GP_GiftGoldResult
{
    CMD_GP_GiftGoldResult()
	{
			memset(this, 0, sizeof(*this));
	}
	LONG                                 lRet;            //�Ƿ�����ɹ�
	DWORD                                dwGold;          //���͵Ľ������
	TCHAR								 szDescribe[64];  //��ʾ��Ϣ
};



//�û���Ϣ���³ɹ��ṹ
struct CMD_GP_UserInfoSucc                                              //�������
{
	CMD_GP_UserInfoSucc()
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

//ע���ʺ�
struct CMD_GP_RegisterAccounts
{
	CMD_GP_RegisterAccounts()
	{
			memset(this, 0, sizeof(*this));
	}
	WORD								wFaceID;						//ͷ���ʶ
	BYTE								cbGender;						//�û��Ա�
	DWORD								dwPlazaVersion;					//�㳡�汾
	TCHAR								szSpreader[NAME_LEN];			//�ƹ�����
	TCHAR								szAccounts[NAME_LEN];			//��¼�ʺ�
	TCHAR								szPassWord[PASS_LEN];			//��¼����
	TCHAR                               szEmail[EMAIL_LEN];             //email����  �������
};

enum USER_TYPE {PERSON=0, ANDROID=1 };
//��½�ɹ�
struct CMD_GP_LogonSuccess
{
	CMD_GP_LogonSuccess()
	{
			memset(this, 0, sizeof(*this));
	}

	WORD								wFaceID;						//ͷ������
	BYTE								cbGender;						//�û��Ա�
	BYTE								cbMember;						//��Ա�ȼ�
	DWORD								dwUserID;						//�û� I D
	DWORD								dwGameID;						//��Ϸ I D
	DWORD								dwExperience;					//�û�����
	
	//��չ��Ϣ
	DWORD								dwCustomFaceVer;				//ͷ��汾
	BYTE								cbMoorMachine;					//��������
	DWORD                               dwLockServerID;                 //�����ķ�����id,��Ϊ0���Ǳ�����         �������




	//�������
	TCHAR								szNickName[NAME_LEN];	            //�ǳ�
	LONGLONG							lMoney;								//��Ϸ���
	LONGLONG                            lGold;                              //�û��Ľ�� 2011-9-30ǰΪ��ʯ lGems
	LONGLONG                            lGem;                              //�û��ı�ʯ 2011-9-30ǰΪ���� lHonor
	DWORD                               dwGrade;                           //�û��ĵȼ���������256�� 2011-9-30ǰΪdwClass

	LONG								lWinCount;							//ʤ������
	LONG								lLostCount;							//ʧ������
	LONG								lDrawCount;							//�;�����
	LONG								lFleeCount;							//������Ŀ

	USER_TYPE                           utIsAndroid;						//�Ƿ��ǻ�����
	TCHAR                               szKey[33];							//��վ�õ���Կ
	LONG                                lGiftNum;							//��ȡ���͵Ĵ�����Ŀǰ��һ��һ�Σ�


	//��ӽ���
	// add by HouGuoJiang 2011-11-25
	TCHAR				szHashID[33];		// ��Ƭ�����õİ汾��
	TCHAR				szReserve[48];		// �������պ���չʹ�ã��Ͳ���Ҫÿ����Ҹ��´����ˣ�������������������ݣ��͵�������Ϣ����

	int									nMasterRight;						//Ȩ�޹���
	int									nMasterOrder;						//Ȩ�޵ȼ�
};

//�������
//�û�������
//struct CMD_GP_UserPlacard
//{
//	DWORD                       dwUserId; //�û�id
//	DWORD                       dwIndex; //���
//	TCHAR                       szTitle[USER_PLACARD_TITLE_LEN]; //����
//	TCHAR                       szDate[USER_PLACARD_TITLE_LEN];   //��Ϣʱ��
//	TCHAR                       szContex[USER_PLACARD_CONTENT_LEN] ; //����
//};

//�û��ȼ���Ϣ
struct CMD_GP_UserGradeInfo 
{
	CMD_GP_UserGradeInfo()
	{
			memset(this, 0, sizeof(*this));
	}
	TCHAR strName[24];
	int   iMaxExp;
};


//��ӽ���


//��½ʧ��
struct CMD_GP_LogonError
{
	CMD_GP_LogonError()
	{
		memset(this, 0, sizeof(*this));
	}
	LONG								lErrorCode;						//�������
	TCHAR								szErrorDescribe[128];			//������Ϣ
};

//////////////////////////////////////////////////////////////////////////
//��Ϸ�б�������

#define MDM_GP_SERVER_LIST				2								//�б���Ϣ

#define SUB_GP_LIST_TYPE				100								//�����б�
#define SUB_GP_LIST_KIND				101								//�����б�
#define SUB_GP_LIST_STATION				102								//վ���б�
#define SUB_GP_LIST_SERVER				103								//�����б�
#define SUB_GP_LIST_FINISH				104								//�������
#define SUB_GP_LIST_CONFIG				105								//�б�����

//�б�����
struct CMD_GP_ListConfig
{
	CMD_GP_ListConfig()
	{
		memset(this, 0, sizeof(*this));
	}
	BYTE								bShowOnLineCount;				//��ʾ����
};

//////////////////////////////////////////////////////////////////////////
//ϵͳ������

#define MDM_GP_SYSTEM					3								//ϵͳ����

#define SUB_GP_VERSION					100								//�汾֪ͨ
#define SUB_SP_SYSTEM_MSG				101								//ϵͳ��Ϣ

//�汾֪ͨ
struct CMD_GP_Version
{
	CMD_GP_Version()
	{
		memset(this, 0, sizeof(*this));
	}
	BYTE								bNewVersion;					//���°汾
	BYTE								bAllowConnect;					//��������
};

//////////////////////////////////////////////////////////////////////////

#define MDM_GP_USER						4								//�û���Ϣ

#define SUB_GP_USER_UPLOAD_FACE			100								//�ϴ�ͷ��
#define SUB_GP_USER_DOWNLOAD_FACE		101								//����ͷ��
#define SUB_GP_UPLOAD_FACE_RESULT		102								//�ϴ����
#define SUB_GP_DELETE_FACE_RESULT		103								//ɾ�����
#define SUB_GP_CUSTOM_FACE_DELETE		104								//ɾ��ͷ��
#define SUB_GP_MODIFY_INDIVIDUAL		105								//��������
#define SUB_GP_MODIFY_INDIVIDUAL_RESULT	106								//�޸Ľ��

#define SUB_GP_SAFE_BIND				107								//��Ұ�
#define SUB_GP_SAFE_UNBIND				108								//�����

#define SUB_GP_UPDATE_USERINFO			109								//�����û���Ϣ		�������  
#define SUB_GP_UPDATE_USERINFO_SUCC     120                             //�����û���Ϣ�ɹ�  �������  


#define SUB_GP_GET_GIFT_GOLD          130                             //�����ȡ          �������
#define SUB_GP_GET_GIFT_GOLD_RESULT   131                             //�����ȡ          �������
#define SUB_GP_CHECK_LINE				132								//�����·
#define SUB_GP_DELETE_MACHINE_CODE		133								//ɾ��������
      
//���ʺ�
struct CMD_GP_SafeBind
{
	CMD_GP_SafeBind()
	{
		memset(this, 0, sizeof(*this));
	}
	DWORD								dwUserID;						//���ID
	tagClientSerial						ClientSerial;					//��������
	TCHAR								szPassWord[PASS_LEN];			//��¼����
};

//�����
struct CMD_GP_SafUnbind
{
	CMD_GP_SafUnbind()
	{
		memset(this, 0, sizeof(*this));
	}
	DWORD								dwUserID;						//���ID
	TCHAR								szPassWord[PASS_LEN];			//��¼����
};

//�����
struct CMD_GP_BindResult
{
	CMD_GP_BindResult()
	{
		memset(this, 0, sizeof(*this));
	}
	BYTE								cbSuccess;						//���ID
	//TCHAR								szErrorDescribe[128];			//��¼����
};

//��������
struct CMD_GP_ModifyIndividual
{
	CMD_GP_ModifyIndividual()
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

//����ͷ��
struct CMD_GP_UploadCustomFace
{
	CMD_GP_UploadCustomFace()
	{
		memset(this, 0, sizeof(*this));
	}
	DWORD								dwUserID;						//��� ID
	WORD								wCurrentSize;					//��ǰ��С
	bool								bLastPacket;					//����ʶ
	bool								bFirstPacket;					//��һ����ʶ
	BYTE								bFaceData[2048];				//ͷ������
};

//���سɹ�
struct CMD_GP_DownloadFaceSuccess
{
	CMD_GP_DownloadFaceSuccess()
	{
		memset(this, 0, sizeof(*this));
	}
	DWORD							dwToltalSize;						//�ܹ���С
	DWORD							dwCurrentSize;						//��ǰ��С
	DWORD							dwUserID;							//��� ID
	BYTE							bFaceData[2048];					//ͷ������
};

//����ͷ��
struct CMD_GP_DownloadFace
{
	CMD_GP_DownloadFace()
	{
		memset(this, 0, sizeof(*this));
	}
	DWORD							dwUserID;							//��� ID
};

//�ϴ����
struct CMD_GP_UploadFaceResult
{
	CMD_GP_UploadFaceResult()
	{
		memset(this, 0, sizeof(*this));
	}
	TCHAR							szDescribeMsg[128];					//������Ϣ
	DWORD							dwFaceVer;							//ͷ��汾
	bool							bOperateSuccess;					//�ɹ���ʶ
};

//ɾ�����
struct CMD_GP_DeleteFaceResult
{
	CMD_GP_DeleteFaceResult()
	{
		memset(this, 0, sizeof(*this));
	}
	TCHAR							szDescribeMsg[128];					//������Ϣ
	DWORD							dwFaceVer;							//ͷ��汾
	bool							bOperateSuccess;					//�ɹ���ʶ
};

//ɾ����Ϣ
struct CMD_GP_CustomFaceDelete
{
	CMD_GP_CustomFaceDelete()
	{
		memset(this, 0, sizeof(*this));
	}
	DWORD							dwUserID;							//��� ID
	DWORD							dwFaceVer;							//ͷ��汾
};

//�޸Ľ��
struct CMD_GP_ModifyIndividualResult
{
	CMD_GP_ModifyIndividualResult()
	{
		memset(this, 0, sizeof(*this));
	}
	TCHAR							szDescribeMsg[128];					//������Ϣ
	bool							bOperateSuccess;					//�ɹ���ʶ
};

//////////////////////////////////////////////////////////////////////////

#endif