#ifndef ATTEMPER_ENGINE_SINK_HEAD_FILE
#define ATTEMPER_ENGINE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Commonality.h"
#include "ServerList.h"
#include "InitParamter.h"
#include <map>

//////////////////////////////////////////////////////////////////////////

//�󶨲���
struct tagBindParameter
{
	DWORD							dwSocketID;							//�����ʶ
	DWORD							dwClientIP;							//���ӵ�ַ
	DWORD							dwActiveTime;						//����ʱ��
};

//////////////////////////////////////////////////////////////////////////

//�������湳��
class CAttemperEngineSink : public IAttemperEngineSink
{
	//��Ԫ����
	friend class CLogonService;

	//��������
protected:
	CInitParamter *					m_pInitParamter;					//���ò���
	tagBindParameter *				m_pBindParameter;					//������Ϣ

	//�������
protected:
	CServerList						m_ServerList;						//�������б�

	//�ӿڱ���
protected:
	ITimerEngine *					m_pITimerEngine;					//ʱ������
	IDataBaseEngine *				m_pIDataBaseEngine;					//��������
	ITCPNetworkEngine *				m_pITCPNetworkEngine;				//��������
	ITCPSocketService *				m_pITCPSocketCorrespond;			//Э������

private:
	std::map<unsigned int , DWORD> m_mapRechargeUserSocketID;
	std::map<DWORD, unsigned int> m_mapRechargeSocketIDUser;

	std::vector<std::string>   m_vecInValidWord;
	std::vector<std::string>   m_vecWordReplaced;

	//��������
public:
	//���캯��
	CAttemperEngineSink();
	//��������
	virtual ~CAttemperEngineSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { return; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//�첽�ӿ�
public:
	//�����¼�
	virtual bool __cdecl OnAttemperEngineStart(IUnknownEx * pIUnknownEx);
	//ֹͣ�¼�
	virtual bool __cdecl OnAttemperEngineStop(IUnknownEx * pIUnknownEx);

	//�¼��ӿ�
public:
	//�����¼�
	virtual bool __cdecl OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize) { return true; }
	//�Զ��¼�
	virtual bool __cdecl OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize) { return true; }

	//�ں��¼�
public:
	//ʱ���¼�
	virtual bool __cdecl OnEventTimer(DWORD dwTimerID, WPARAM wBindParam);
	//���ݿ��¼�
	virtual bool __cdecl OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�����¼�
public:
	//�����¼�
	virtual bool __cdecl OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode);
	//�ر��¼�
	virtual bool __cdecl OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool __cdecl OnEventTCPSocketRead(WORD wServiceID, CMD_Command Command, VOID * pData, WORD wDataSize);

	//�����¼�
public:
	//Ӧ���¼�
	virtual bool __cdecl OnEventTCPNetworkBind(DWORD dwClientIP, DWORD dwSocketID);
	//�ر��¼�
	virtual bool __cdecl OnEventTCPNetworkShut(DWORD dwClientIP, DWORD dwActiveTime, DWORD dwSocketID);
	//��ȡ�¼�
	virtual bool __cdecl OnEventTCPNetworkRead(CMD_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//���ݿ⺯��
private:
	//�û���¼ʧ��
	bool OnDBLogonError(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û���¼�ɹ�
	bool OnDBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û�ǩ��
	bool OnDBUserSignInResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û��콱
	bool OnDBUserGetAwardResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�һ��ĸ�����Ϣ
	bool OnDBUserExchangeInfoResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�һ�����Ʒ
	bool OnDBUserExchangeProductResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�һ�����Ʒ
	bool OnDBUserExchangeProductResult_V2(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�һ�
	bool OnDBUserExchangeResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�һ�
	bool OnDBUserExchangeResult_V2(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�һ��ļ�¼
	bool OnDBUserExchangeRecordResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���سɹ�
	bool OnDBDownloadFaceSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ɹ���Ϣ
	bool OnDBRCustomFaceUploadResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ɾ�����
	bool OnDBRCustomFaceDeleteResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸Ľ��
	bool OnDBRModifyIndividual(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸ĸ���ǩ�����
	bool OnDBRModifyUnderWrite(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸��ֻ��Ž��
	bool OnDBRModifyPhone(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//У������
	bool OnDBRCheckID(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//У���ǳ�
	bool OnDBRCheckNickName(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ע��
	bool OnDBRRegister(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����
	bool OnDBRIDUpdate(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��������
	bool OnDBRResetPwd(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ�û���Ϣ
	bool OnDBRUserInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ�û��Ƿ������ȡ�ƹ㽱��
	bool OnDBRUserCanSpread(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û���ȡ�ƹ㽱��
	bool OnDBRUserGetSpread(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ�ƹ�Ա��Ϣ
	bool OnDBRUserSpreaderInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����������Ϣ
	bool OnDBRUserBbsPop(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����������Ϣ
	bool OnDBRUserBbsScroll(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�б�����Ϣ
	bool OnDBRUserBbsList(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���������Ϣ
	bool OnDBRUserTaskInfo(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������а�
	bool OnDBRUserGetRanking(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������а�
	bool OnDBRUserGetRanking_V2(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����Ƿ������а���Ϣ
	bool OnDBRUserGetRankingStatus(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�����Ƿ������а񷵻���Ϣ
	bool OnDBRUserGetRankingStatusResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����������
	bool OnDBRUserFeedBack(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ����������
	bool OnDBRUserGetFeedBack(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ�û����֤�˺���Ϣ�Ľ��
	bool OnDBRUserInfoIDCard(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����ύ���֤��Ϣ�Ľ��
	bool OnDBRUserAuthentication(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����̳���Ʒ�б���Ϣ�Ľ��
	bool OnDBRUserMallProductResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����̳ǹ����¶����Ľ��
	bool OnDBRUserMallPlaceOrderResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����ѯ�û�������Ϣ�Ľ��
	bool OnDBRUserBankInfoResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û��������н��
	bool OnDBRUserBankChargeResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û�ȡ������
	bool OnDBRUserCancelOrderResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û�������
	bool OnDBRUserMallBuyResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û����½��
	bool OnDBRUserMallUpdateResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	
	//��б���
	bool OnDBRUserActivityListResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����
	bool OnDBRUserActivityResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�齱���
	bool OnDBRUserActivityLuckyResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�齱��¼
	bool OnDBRUserActivityRecordResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������
	bool OnDBRUserSpeakerSendResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ���ý��
	bool OnDBRUserQueryConfigResult(DWORD dwContextID, VOID * pData, WORD wDataSize);
	
	
	//���纯��
private:
	//��¼��Ϣ����
	bool OnSocketMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�û���Ϣ����
	bool OnSocketMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);
	//�������б���ش���
	bool OnSocketServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID);

	//soap�¼�
	bool OnSoapRequest(WORD wSubCmdID, VOID * pData, WORD wDataSize);

	//��������
private:
	//�б���Ϣ����
	bool OnCenterMainServerList(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//������������Ϣ����
	bool OnCenterMainServerManager(WORD wSubCmdID, VOID * pData, WORD wDataSize);

private:
	std::string CheckWord(char* data, short len);
};

//////////////////////////////////////////////////////////////////////////

#endif