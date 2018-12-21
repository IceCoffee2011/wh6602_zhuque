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
	friend class CLogonService;

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
	//��¼������
	bool OnRequestLogon(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ǩ��
	bool OnRequestUserSignIn(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�콱
	bool OnRequestUserGetAward(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�һ��ĸ�����Ϣ
	bool OnRequestUserExchangeInfo(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�һ���Ʒ
	bool OnRequestUserExchangeProduct(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�һ���Ʒ
	bool OnRequestUserExchangeProduct_V2(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�һ�
	bool OnRequestUserExchange(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�һ�
	bool OnRequestUserExchange_V2(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�һ���¼
	bool OnRequestUserExchangeRecord(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ϴ�ͷ��
	bool OnRequestUploadFace(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ͷ��
	bool OnRequestDownloadFace(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ɾ����Ϣ
	bool OnRequestCustomFaceDelete(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�����
	bool OnRequestModifyIndividual(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸ĸ���ǩ��
	bool OnRequestModifyUnderWrite(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸��ֻ���
	bool OnRequestModifyPhone(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//У������
	bool OnRequestCheckID(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//У���ǳ�
	bool OnRequestCheckNickName(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//ע��
	bool OnRequestRegister(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ʺ�����
	bool OnRequestIDUpdate(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��������
	bool OnRequestResetPwd(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û���Ϣ
	bool OnRequestUserInfo(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û��ܷ���ȡ�ƹ㽱��
	bool OnRequestUserCanSpread(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�û���ȡ�ƹ㽱��
	bool OnRequestUserGetSpread(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�ƹ�Ա��Ϣ
	bool OnRequestSpreaderInfo(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����������Ϣ
	bool OnRequestGetBbsPop(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����������Ϣ
	bool OnRequestGetBbsScroll(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�б�����Ϣ
	bool OnRequestGetBbsList(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���������Ϣ
	bool OnRequestGetTaskInfo(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������а�
	bool OnRequestGetRanking(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������а�
	bool OnRequestGetRanking_V2(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�޸�����Ƿ������а�
	bool OnRequestModifyRankingStatus(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ҷ������
	bool OnRequestFeedBack(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ��ҷ������
	bool OnRequestGetFeedBack(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ�û����֤�˺���Ϣ
	bool OnRequestUserInfoIDCard(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����ύ���֤��Ϣ(���������֤��)
	bool OnRequestUserAuthentication(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����̳���Ʒ��Ϣ
	bool OnRequestMallProduct(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize, bool isIOS=false);
	//�����̳Ƕ���
	bool OnRequestMallPlaceOrder(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����ȡ��������
	bool OnRequestUserCancelOrder(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����̳���Ʒ������
	bool OnRequestMallBuy(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����̳ǲ�ѯ�����Ÿ��½��
	bool OnRequestMallUpdateBuy(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����ѯ������Ϣ
	bool OnRequestUserBankInfo(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����������
	bool OnRequestUserBankCharge(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//��ѯ��б�
	bool OnRequestActivityList(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ�
	bool OnRequestActivity(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�齱
	bool OnRequestActivityLucky(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�齱��¼
	bool OnRequestUserActivityRecord(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��������
	bool OnRequestUserSpeakerSend(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ѯ������
	bool OnRequestUserQueryConfig(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�洢����
private:
	//I D �洢����
	LONG SPLogonByUserID(DWORD dwGameID, LPCTSTR pszPassword, DWORD dwClientIP, LPCTSTR pszComputerID);
	//�ʺŴ洢����
	LONG SPLogonByAccounts(LPCTSTR pszAccounts, LPCTSTR pszPassword, DWORD dwClientIP, LPCTSTR pszComputerID);
	//ע��洢����
	LONG SPRegisterAccounts(LPCTSTR pszAccounts, LPCTSTR pszPassword, LPCTSTR pszSpreader, WORD wFaceID, BYTE cbGender, DWORD dwClientIP, LPCTSTR pszComputerID);
	//ע��洢����WX
	LONG SPRegisterAccountsWX(LPCTSTR pszAccounts, LPCTSTR pszPassword, LPCTSTR pszSpreader, 
							  LPCTSTR wFaceURL, BYTE cbGender, DWORD dwClientIP, LPCTSTR pszWXComputerID,LPCTSTR pszComputerID,
							  LPCTSTR pszNickname, LPCTSTR pszOpenid, LPCTSTR unionid);
	//������WX�˺Ŵ洢����
	LONG SPWXUpdate(DWORD dwUserID,LPCTSTR wFaceURL, BYTE cbGender, DWORD dwClientIP, LPCTSTR pszWXComputerID,LPCTSTR pszComputerID,LPCTSTR pszNickname, LPCTSTR pszOpenid, LPCTSTR unionid);
	//�ϴ��洢����
	LONG SPUploadFace(DWORD dwUserID, BYTE *bFaceData, DWORD dwFaceDataSize);
	//ǩ���洢����
	LONG SPUserSignIn(DWORD dwUserID);
	//�콱�洢����
	LONG SPUserGetAward(DWORD dwUserID, DWORD dwTaskID);
	//�һ��ĸ�����Ϣ�洢����
	LONG SPUserExchangeInfo(DWORD dwUserID, LPCTSTR pszPhone);
	//�һ���Ʒ
	LONG SPUserExchangeProduct(DWORD dwUserID);
	//�һ�
	LONG SPUserExchange(DWORD dwUserID,DWORD dwAwardID);
	//�һ�
	LONG SPUserExchange_V2(DWORD dwUserID,DWORD dwAwardID);
	//�һ���¼
	LONG SPUserExchangeRecord(DWORD dwUserID, int nPage);

	//���ش洢����
	LONG SPDownloadFace(DWORD dwUserID);
	//ɾ���洢����
	LONG SPDeleteCustomFace(DWORD dwUserID);
	//�޸Ĵ洢����
	LONG SPModifyIndividual(TCHAR szAccount[NAME_LEN],TCHAR szNickname[NAME_LEN], TCHAR szPhone[NAME_LEN],int nGender);
	//�޸ĸ���ǩ���洢����
	LONG SPModifyUnderWrite(unsigned int dwUserID,TCHAR szDescribe[UNDER_WRITE_LEN]);
	//�޸��ֻ��Ŵ洢����
	LONG SPModifyPhone(unsigned int dwUserID,TCHAR szPhone[31]);

	//У�����ƴ洢����
	LONG SPCheckID(TCHAR szAccount[NAME_LEN]);
	//У���ǳƴ洢����
	LONG SPCheckNickName(TCHAR szNickName[NAME_LEN]);
	//ע��洢����
	LONG SPRegister(TCHAR szAccount[NAME_LEN],TCHAR szNickname[NAME_LEN], TCHAR szPhone[NAME_LEN], int nGender, TCHAR szPassword[PASS_LEN], 
					DWORD dwClientIP, LPCTSTR pszComputerID, LPCTSTR pszChannel);
	//�ʺ������洢����
	LONG SPIDUpdate(TCHAR szAccount[NAME_LEN],TCHAR szAccountNew[NAME_LEN],TCHAR szNickname[NAME_LEN], int nGender, TCHAR szPassword[PASS_LEN]);
	//��������洢����
	LONG SPResetPwd(TCHAR szAccount[NAME_LEN], TCHAR szPassword[PASS_LEN]);
	//��ѯ��Ϣ�洢����
	LONG SPUserInfo(TCHAR szAccount[NAME_LEN]);
	//��ѯ�û��Ƿ������ȡ�ƹ㽱��
	LONG SPUserCanSpread(DWORD dwUser, LPCTSTR pszComputerID);
	//�û���ȡ�ƹ㽱��
	LONG SPUserGetSpread(DWORD dwUser, DWORD dwSpreader, LPCTSTR pszComputerID);
	//��ѯ�ƹ�Ա��Ϣ
	LONG SPUserGetSpreadInfo(DWORD dwSpreader);
	//����������Ϣ
	LONG SPUserGetBbsPop(LPCTSTR pszChannel);
	//����������Ϣ
	LONG SPUserGetBbsScroll(LPCTSTR pszChannel);
	//�б�����Ϣ
	LONG SPUserGetBbsList(LPCTSTR pszChannel);
	//���������Ϣ
	LONG SPUserGetTaskInfo(DWORD dwUser);
	//���а��¼
	LONG SPUserRanking(DWORD dwUserID, int nPage);
	//���а��¼
	LONG SPUserRanking_V2(DWORD dwUserID, int nPage);
	//������а��Ƿ����
	LONG SPUserRankingStatus(DWORD dwUserID);
	//�޸��Ƿ������а�
	LONG SPUserModifyRankingStatus(DWORD dwUserID, int nRankingStatus);
	//��������
	LONG SPUserFeedBack(DWORD dwUserID, LPCTSTR pszContent, LPCTSTR pszPicture);
	//��ѯ��������
	LONG SPUserGetFeedBack(DWORD dwUserID, int nPage);
	//��ѯ�û����֤�˺���Ϣ
	LONG SPUserInfoIDCard(DWORD dwUserID);
	//�����ύ���֤��Ϣ
	LONG SPUserAuthentication(DWORD dwUserID, LPCTSTR pszName, LPCTSTR pszIDCard);

	//�̳���Ʒ��Ϣ
	LONG SPUserMallProduct(DWORD dwUserID);
	//�̳���Ʒ��Ϣ(ios)
	LONG SPUserMallProductIOS(DWORD dwUserID);
	//�¶���
	LONG SPUserMallPlaceOrder(DWORD dwUserID, DWORD dwProductID,TCHAR szPayWay[10]);
	// ȡ��������
	LONG SPUserCancelOrder(TCHAR szAccount[20]);
	// �̳ǹ�����Ʒ���
	LONG SPUserMallBuy(TCHAR orderNum[20]);
	// �̳Ǹ��¹�����
	LONG SPUserMallUpdateBuy(TCHAR orderNum[20]);
	//��ѯ�û�������Ϣ
	LONG SPUserBankInfo(DWORD dwUserID);
	//�û���������
	LONG SPUserBankCharge(int nOpCode, DWORD dwUserID, DWORD dwCurScore, DWORD dwBankScore);
	
	//��ѯ��б�
	LONG SPUserActivityList();
	//��ѯ�
	LONG SPUserActivity(DWORD dwUserID, DWORD dwActivityID);
	//�齱
	LONG SPUserActivityLuckyList(DWORD dwUserID, DWORD dwActivityID);
	LONG SPUserActivityLucky(DWORD dwUserID, DWORD dwActivityID, DWORD dwPrizeID);
	//�齱��¼
	LONG SPUserActivityRecord(DWORD dwUserID, DWORD dwActivityID, int nPage);
	//������
	LONG SPUserSpeakerSend(DWORD dwUserID, WORD wType, LPCTSTR pszContent);
	//��ѯ������
	LONG SPUserQueryConfig(DWORD dwUserID, DWORD dwVersion, DWORD dwConfigID, LPCTSTR pszChannel);
};

//////////////////////////////////////////////////////////////////////////

#endif