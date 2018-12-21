#ifndef SERVICE_PARAMETER_HEAD_FILE
#define SERVICE_PARAMETER_HEAD_FILE
#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

#define VER_PARAMETER				2									//�ļ��汾

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//������Ϣ
struct tagOptionParameter
{
	WORD							m_wTreasureDBCount;					//����
	TCHAR							szDataBaseAddr[MAX_TREASURE_DB][32];//���ݿ��ַ
	TCHAR							szDataBaseName[32];					//���ݿ�����
	TCHAR							szModuleName[MODULE_LEN];			//ģ������
	tagGameServiceOption			GameServiceOption;					//��������

	//by leon ����ϵͳ���á�ҳ�Ĳ�����Ҳ���浽ROP�ļ���ȥ
	TCHAR							m_szCenterServerAddr[16];			//���ķ�����
	//�û����ݿ�
	WORD							m_wGameUserDBPort;					//���ݿ�˿�
	TCHAR							m_szGameUserDBAddr[16];				//���ݿ��ַ
	TCHAR							m_szGameUserDBUser[32];				//���ݿ��û�
	TCHAR							m_szGameUserDBPass[32];				//���ݿ�����
	TCHAR							m_szGameUserDBName[32];				//���ݿ�����
	//��Ϸ���ݿ�
	WORD							m_wServerInfoDBPort;				//���ݿ�˿�
	TCHAR							m_szServerInfoDBAddr[16];			//���ݿ��ַ
	TCHAR							m_szServerInfoDBUser[32];			//���ݿ��û�
	TCHAR							m_szServerInfoDBPass[32];			//���ݿ�����
	TCHAR							m_szServerInfoDBName[32];			//���ݿ�����
};

//��¼����
struct tagRecordParameter
{
	WORD							wVersion;							//���ð汾
	WORD							wFileSize;							//�ļ���С
	tagOptionParameter				OptionParameter;					//���ò���
};

//////////////////////////////////////////////////////////////////////////

//���������
class CServiceParameter : public CDialog
{
	//��������
public:
	bool							m_bLoaded;							//���ر�־
	tagOptionParameter				m_OptionParameter;					//���ò���

	//�������
protected:
	HINSTANCE						m_hDllInstance;						//��Ϸʵ��
	tagGameServiceAttrib			m_GameServiceAttrib;				//��������
	IGameServiceManager				* m_pIGameServiceManager;			//����ӿ�

	//��������
public:
	//���캯��
	CServiceParameter(void);
	//��������
	virtual ~CServiceParameter(void);

	//���غ���
protected:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//�ؼ����໯
	virtual void DoDataExchange(CDataExchange * pDX);
	//ȷ������
	virtual void OnOK();

	//���ܺ���
public:
	//�Ƿ����
	bool IsLoaded() { return m_bLoaded; }
	//��ȡ����
	const tagOptionParameter & GetOptionParameter() { return m_OptionParameter; }

	//������
public:
	//���ò���
	void ResetParameter();
	//�޸Ĳ���
	bool ConfigParameter();
	//��������
	bool LoadParameter(LPCTSTR pszFileName);
	//��������
	bool SaveParameter(LPCTSTR pszFileName);
	//��������
	bool CreateParameter(LPCTSTR pszModuleName);

	//�ڲ�����
private:
	//����ģ��
	bool LoadGameServiceModule(LPCTSTR pszModuleName);
	//�ͷ�ģ��
	void FreeGameServiceModule();
	//��������
	void UpdateOptionParameter();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif