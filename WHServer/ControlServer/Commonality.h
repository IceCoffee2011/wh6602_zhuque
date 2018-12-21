#ifndef COMMONALITY_HEAD_FILE
#define COMMONALITY_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include <vector>
using std::vector;

//////////////////////////////////////////////////////////////////////////
//���ݿ������

//���ݿ������ʶ
#define DBR_GP_CHECK_GM_TASK			100								//���GM����
#define DBR_GP_CHECK_GM_TASK_RESULT		101								//���GM����Ľ��
#define DBR_GP_CREATE_RANKING			102								//�������а�
#define DBR_GP_WRITE_ONLINE				103								//д�������
#define DBR_GP_CHECK_SPEAKER_TASK_RESULT		104						//���ϵͳ��������Ľ��
#define DBR_GP_SPEAKER_SEND				105								//ϵͳ�������

struct DBR_GR_Online
{
	DWORD							dwTypeID;							//����
	DWORD							dwValue;							//ֵ
	TCHAR							szCollectDate[32];					//ʱ���
};

struct DBR_GP_Speaker_Task_Result
{
	DWORD							dwUserID;							//�û�ID
	short							nLen;								//����
	TCHAR							szMsg[256];							//����
};

//����������
struct DBR_GP_SpeakerSend
{
	short							iType;					//�������ͣ�1�û���Ϣ��2ϵͳ��Ϣ
	DWORD							dwUserID;				//��� ID
	DWORD							nLen;					
	TCHAR							szDate[256];					//��������
};

//////////////////////////////////////////////////////////////////////////

#endif

