/******************************************************************
Copyright (C),  2010-2025  
File name:		CCDownloadManager.h
Version:		1.0
Description:    ���ؽӿ�

Author:			pengwen
Create Date:	2010-5-12

------------------------------------
2010-9-23       pengwen ����
******************************************************************/



#pragma once
#ifdef  API_EXPORTS
#define API_ABC   __declspec(dllexport)   
#else
#define API_ABC   __declspec(dllimport)  
#endif

typedef  void  (*CallBackFinsh)(char* pUrl, bool isFinish, void* p);
class API_ABC CCDownloadManager
{
public:
	CCDownloadManager(void);
	~CCDownloadManager(void);
private:
	static CCDownloadManager s_dlm;

public:
	static CCDownloadManager& getInstance();
	static CCDownloadManager* getInstancePtr();
private:
	char*  m_pUrl;
	char*  m_pOutFolder;
	void*  m_pUserNeed; 
	CallBackFinsh m_pCallBackFinsh;

private:
	bool ReadDNP();
	int _CheckDownloadAlrExists();
public:	
	//������ɣ�ģ���ڲ�����
	void  Finish(char* pUrl);

public:
	bool Init(const char* pUrl, const char* pOutFolder, CallBackFinsh pFt, void* pUserNeed = NULL);
	void Clear();
	//ע����ܷ��� 0,һ��ʵ���ļ���ʼ���ص�ʱ��
	unsigned __int64  GetFileSize();
	//�õ��Ѿ����ص��ļ���С, ������صĹ�������̫�����߳���
	//���ܵ��·��صĽ�������ļ���С��ע�⴦���������
	unsigned __int64  GetDownLoadSize();
	//�õ��ٶ�
	unsigned int GetSpeed();
	//����
	void Start();
	//ȡ������
	void Stop();
	//��������
	void GoOn();

};
 