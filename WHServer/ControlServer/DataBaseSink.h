#ifndef DATA_BASE_SINK_HEAD_FILE
#define DATA_BASE_SINK_HEAD_FILE

#pragma once

#include "Afxinet.h"
#include "Commonality.h"
#include "InitParamter.h"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//���ݿ����湳��
class CDataBaseSink : public IDataBaseEngineSink
{
	//��Ԫ����
	friend class CControlService;

	//��������
protected:
	CInitParamter *					m_pInitParamter;					//���ò���
	IDataBaseEngineEvent *			m_pIDataBaseEngineEvent;			//�����¼�

	//�������
protected:
	CDataBaseAide					m_AccountsDBAide;					//�û����ݿ�
	CDataBaseHelper					m_AccountsDBModule;					//�û����ݿ�

	//��������
public:
	//���캯��
	CDataBaseSink();
	//��������
	virtual ~CDataBaseSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID __cdecl Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * __cdecl QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//ϵͳ�¼�
public:
	//�����¼�
	virtual bool __cdecl OnDataBaseEngineStart(IUnknownEx * pIUnknownEx);
	//ֹͣ�¼�
	virtual bool __cdecl OnDataBaseEngineStop(IUnknownEx * pIUnknownEx);

	//�ں��¼�
public:
	//ʱ���¼�
	virtual bool __cdecl OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter) { return true; }
	//�����¼�
	virtual bool __cdecl OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize) { return true; }
	//�����¼�
	virtual bool __cdecl OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//������
private:
	//GM����������
	bool OnRequestCheckGMTask(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�������а�
	bool OnRequestCreateRanking(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//д�������
	bool OnRequestWriteOnline(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//дϵͳ��Ϣ
	bool OnRequestSpeakerSend(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	
	
	//����������
	LONG SPCheckGMTask();
	//ϵͳ����������
	LONG SPCheckSpeakerTask();
	//�������а�
	LONG SPCreateRanking();
	//д�������
	LONG SPWriteOnline(int type, int value, LPCTSTR collectDate);
	//дϵͳ��Ϣ
	LONG SPSpeakerSend(DWORD dwUserID, WORD wType, LPCTSTR pszContent);
};

//////////////////////////////////////////////////////////////////////////

#endif