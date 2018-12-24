#pragma once


// ISynchronyTask ����ĳ�Ա���Ͷ������
class   __declspec(dllexport)  IDownloadEvent
{
public:
	/* --------------------------------------------------------------------------
	����˵����������ؽ������һ���ص�����
	���������
		iCookie						DownWebFile �������ʲôֵ���˴�Ҳ���ش�ֵ���൱��Ψһ��ʶ
		pszLocalPathFile		Ҫ���ص����ص�·�����ļ���
		iRes							// ����״̬
										DOWNLOAD_NOT_START	 0	// ��δ��ʼ
										DOWNLOAD_RUNING		   10	// �������ع�����
										DOWNLOAD_SUCCES			20	// ���سɹ�
										DOWNLOAD_FAIL				30	// ����ʧ��
										PROGRAM_OVER					40	// ���������
		iSuccessDownload   �Ѿ����سɹ����ֽ�����
		iTotalDownload			���ļ��ܹ���Ҫ���ص��ֽ�����
	--------------------------------------------------------------------------*/
	virtual void OnDownloadResult (int iCookie, char* pszLocalPathFile, int iRes, int iSuccessDownload, int iTotalDownload) = 0;
	
};

class  __declspec(dllexport)  ISegmentDownload
{
public:
	ISegmentDownload();
	~ISegmentDownload();
	
	void RegistEventModule(IDownloadEvent* pDownloadEvent);

	// ������ע�⣺��ò�Ҫ�����߳�������
	bool Start();	

	// ֹͣ
	bool Stop();

	// ����ÿ�εĳ���
	void SetEverySegmentLen(int	iEverySegmentLen);

	// ���ÿ�εĳ���
	int GetEverySegmentLen();

	// ����ÿһ�εĳ�ʱֵ
	void SetOverTimeEverySegment(DWORD dwOverTimeEverySegment);

	// ����ÿһ�ε��������ش���
	void SetMaxDownCount(int iMaxDownCount);

	// ���������̵߳�����
	void SetTaskThreadNum(int iTaskThreadNum);

	// ������ת�ɵ�� IP ��ַ
	void HostNameToIP(char* szHostName, char* szIP);

	// ����
	void Test(int iMsgCode, void* pData, int iDataLen); 

	/* --------------------------------------------------------------------------
	����˵��������һ��Զ���ļ�
	���������
		iCookie				�˴��������ʲôֵ���� OnDownloadResult ���ص�Ҳ�Ǵ�ֵ���൱��Ψһ��ʶ
		szRemoteUrl		Web�ϵ��ļ�
		szLocalFile		Ҫ���ص����ص�·�����ļ���
		pszError			���ش���һ������ΪNULL��������ò�����128

	����ֵ��
		�ɹ��򷵻� true�����򷵻� false ��
	��ע��
		�ɹ�����Ҫ�� OnDownloadResult ������
	--------------------------------------------------------------------------*/
	bool DownWebFile(int iCookie,  char* szRemoteUrl, char* szLocalFile, char* pszError);

private:
	void*					m_pCDownload;
	IDownloadEvent* m_pDownloadEvent;
};

