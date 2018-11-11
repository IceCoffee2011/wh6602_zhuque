#ifndef DOWN_LOAD_SERVICE_HEAD_FILE
#define DOWN_LOAD_SERVICE_HEAD_FILE

#pragma once

#include "AfxInet.h"
#include "Resource.h"
#include "DownLoad.h"
#include <string>
#include "CDownloadManager.h"
#include "DownLoadServiceInterface.h"


using namespace std;

//////////////////////////////////////////////////////////////////////////
//ö�ٶ���


//����ö��
enum enDownLoadResult
{
	enDownLoadResult_Noknow,			//û�д���
	enDownLoadResult_Exception,			//�쳣����
	enDownLoadResult_CreateFileFails,	//����ʧ��
	enDownLoadResult_InternetReadError,	//��ȡ����
};

//����״̬
struct tagDownLoadStatus
{
	WORD								wProgress;						//���ؽ���
	TCHAR								szStatus[128];					//״̬����
	enDownLoadStatus					DownLoadStatus;					//����״̬
};

//////////////////////////////////////////////////////////////////////////

//�����߳�
class CDownLoadThread : public CServiceThread
{
	//��������
protected:
	bool							m_bPreparative;						//��ʼ��־
	tagDownLoadRequest				* m_pDownLoadRequest;				//��������

	//��Ϣ����
protected:
	TCHAR							m_szTempFile[MAX_PATH];				//��ʱ�ļ�
	TCHAR							m_szLocalFile[MAX_PATH];			//�����ļ�

	//״̬����
protected:
	CCriticalSection				m_CriticalSection;					//�߳�����
	enDownLoadStatus				m_DownLoadStatus;					//����״̬
	enDownLoadResult				m_DownLoadResult;					//���״̬
	DWORD							m_dwDownLoadSize;					//���ش�С
	DWORD							m_dwTotalFileSize;					//�ļ���С

	//�̱߳���
protected:
	CFile							m_LocalFile;						//�����ļ�
	CHttpFile						* m_pHttpFile;						//�����ļ�
	CHttpConnection					* m_pHttpConnection;				//��������
	CInternetSession				m_InternetSession;					//����Ự

	//��������
public:
	//���캯��
	CDownLoadThread();
	//��������
	virtual ~CDownLoadThread();

	//���ܺ���
public:
	//��ʼ���߳�
	bool InitThread(tagDownLoadRequest * pDownLoadRequest);
	//����״̬
	void GetDownLoadStatus(tagDownLoadStatus & DownLoadStatus);
	//Ŀ���ļ�
	LPCTSTR GetDownLoadFileName();

	//���غ���
private:
	//���к���
	virtual bool OnEventThreadRun();
	//�ر��¼�
	virtual bool OnEventThreadConclude();

	//�̺߳���
private:
	//����׼��
	void DownLoadPreparative();
	//��������
	void DownLoadCleanUp();
};

//////////////////////////////////////////////////////////////////////////


//��������
class CDownLoadMission : public CDialog
{
	//��������
protected:
	DWORD								m_dwMissionID;					//�����ʶ
	DWORD								m_dwDownLoadType;				//��������
	tagDownLoadRequest					m_DownLoadRequest;				//��������
	IDownLoadMissionSink				* m_pIDownLoadMissionSink;		//�ص��ӿ�

	//�ؼ�����
protected:
	CSkinButton							m_btCloseBT;						//�رհ�ť
	CSkinButton							m_btHitHereBT;						//������ﰴť

	CPngImage							m_ImageDownLoadFlash;				//���ض���
	CPngImage							m_ImageDownLoadNum;					//��������
	CPngImage							m_ImageFailFlash;					//����ʧ�ܶ���
	CPngImage							m_ImageImageBack;					//����������
	CPngImage							m_ImageImageFrontGree;				//������ǰ��(��)
	CPngImage							m_ImageImageFrontRed;				//������ǰ��(��)
	CPngImage							m_ImageImageBai;					//%�ֺ�
	CPngImage							m_ImageDownBack;					//���ر���
	CPngImage							m_ImageUpdateBack;					//���±���
	CPngImage							m_ImageDownFail;					//����ʧ�ܱ���

private:
	//����״̬
	int									m_iDownTag;						//0:û������;1:��������;2:�������;3:����ʧ��
	BYTE								m_byDownLoadTag;				//���ر�ʶ.1:����;2:����;3:����ʧ��

	//URL
	std::string							m_strNetURL;					//����URL
	//��������·��
	std::string							m_strLocalFilePath;				//�����ļ�·��


private:
	int									m_iDownBaiBi;					//���ذٷֱ�ֵ
	int									m_iSaveDownBaiBi;				//����

	//����
	int									m_iCountDownSuc;				//�������ع���
	int									m_iCountDownFail;				//��������ʧ��

private:
	CDC									m_memdc;						//�ڴ�DC

	CRect								m_rcFlash;						//����
	CRect								m_rcNums;						//����
	CRect								m_rcProcess;					//������

	//��������
public:
	//���캯��
	CDownLoadMission(IDownLoadMissionSink * pIDownLoadMissionSink);
	//��������
	virtual ~CDownLoadMission();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//ȷ������
	virtual void OnOK() { return; }
	//ȡ����Ϣ
	virtual void OnCancel();

	//���ܺ���
public:
	//�����ʶ
	DWORD GetMissionID() { return m_dwMissionID; }
	//��������
	DWORD GetDownLoadType() { return m_dwDownLoadType; }
	//��ʼ����
	bool StartDownLoad(DWORD dwMissionID, DWORD dwDownLoadType, tagDownLoadRequest * pDownLoadRequest);
	//ֹͣ����
	bool StopDownLoad();
	//�Ա�����
	bool CompareRequest(tagDownLoadRequest * pDownLoadRequest);
	//��ʾ����
	bool DisplayDownLoadFace(bool bDisplay);
	//���ؽ��
	void DownResult(bool bIsSuc);

	//��Ϣ����
protected:
	//���԰�ť
	afx_msg void OnBnClickedReTry();
	//�������
	afx_msg void OnBnClickedHere();
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
};


#endif