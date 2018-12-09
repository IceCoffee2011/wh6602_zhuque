#ifndef DATA_BASE_SINK_HEAD_FILE
#define DATA_BASE_SINK_HEAD_FILE

#pragma once

#include "Afxinet.h"
#include "Commonality.h"
//#include "InitParamter.h"
#include "ServerParameter.h"
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////



//���ݿ����湳��
class CDataBaseSink : public IDataBaseEngineSink
{
	//��Ԫ����
	friend class CLogonService;

	//��������
protected:
	//CInitParamter *					m_pInitParamter;					//���ò���
	CServerParameter *              m_pInitParamter1;
	IDataBaseEngineEvent *			m_pIDataBaseEngineEvent;			//�����¼�

	//�������
protected:
	CDataBaseAide					m_AccountsDBAide;					//�û����ݿ�
	CDataBaseHelper				m_AccountsDBModule;		//�û����ݿ�

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
	//��¼������
	bool OnRequestLogon(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ���
	bool OnGiftGlod(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����û���Ϣ
	bool UpdateUserInfo(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ϴ�ͷ��
	bool OnRequestUploadFace(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ͷ��
	bool OnRequestDownloadFace(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ɾ����Ϣ
	bool OnRequestCustomFaceDelete(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�����
	bool OnRequestModifyIndividual(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ�ȼ���Ϣ
	bool OnReadGradeInfo(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�洢����
private:
	//�������
	//��ӽ���

	//�ʺŴ洢����
	LONG SPLogonByAccounts(LPCTSTR pszAccounts, LPCTSTR pszPassword, DWORD dwClientIP, LPCTSTR pszComputerID);
	//�ϴ��洢����
	LONG SPUploadFace(DWORD dwUserID, BYTE *bFaceData, DWORD dwFaceDataSize);
	//���ش洢����
	LONG SPDownloadFace(DWORD dwUserID);
	//ɾ���洢����
	LONG SPDeleteCustomFace(DWORD dwUserID);
	//�޸Ĵ洢����
	LONG SPModifyIndividual(DWORD dwUserID, TCHAR szNickname[NAME_LEN], int nGender, int nAge, TCHAR szAddress[64], TCHAR szUnderWrite[32], TCHAR szPassword[32]);
	//Author:<cxf>; CreateDate:<2011-12-22>; Description:<�������ݿ����ӳ�һֱ����>
	bool ContinueConnect();

};

//////////////////////////////////////////////////////////////////////////

#endif