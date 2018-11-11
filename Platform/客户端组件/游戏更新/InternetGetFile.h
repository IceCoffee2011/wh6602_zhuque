// InternetGetFile.h: interface for the InternetGetFile namespace.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#if !defined(INTERNETGETFILE_H)
#define INTERNETGETFILE_H

#define WM_USER_MSG							WM_USER+2000

// �ϱ������������������Ϣ
enum
{
	NOTIFY_DOWNLOAD_INFO = 1001,		// ֪ͨҪ���ص��ļ�״��
	NOTIFY_DOWNLOADED_INFO,				// ֪ͨ�����ص��ļ�״��
	NOTIFY_DOWNLOAD_PROGRESS,			// ֪ͨ���ص����ļ�����
	NOTIFY_DOWNLOADING_FILENAME,		// ֪ͨ�������ص��ļ���
	NOTIFY_DOWNLOAD_FILE_FAIL,			// ֪ͨ�����ļ�ʧ��
	NOTIFY_VERIFY_FILE_FAIL,			// ֪ͨУ���ļ�ʧ��
	NOTIFY_UPDATE_FILE_FAIL,			// ֪ͨ�����ļ�ʧ��
    NOTIFY_FINISH_UPDATE,				// ֪ͨ���������Ϣ
	NOTIFY_MESSAGE,						// ֪ͨ������ʾ��Ϣ
	NOTIFY_GENERATE_MESSAGE,			// ֪ͨ������ʾ��Ϣ
	NOTIFY_GENERATE_FILE_NAME,			// ֪ͨ�����ļ��б�
	NOTIFY_LEACH_FILE_NAME,				// ֪ͨ���˵��ļ���
};

// ���������ļ�״̬��Ϣ�ṹ
struct DOWNLOAD_INFO_STRU
{
	UINT iFileCount;					// �ϼ��ļ���
	ULONGLONG iFileSize;				// �ϼ��ֽ���

	DOWNLOAD_INFO_STRU()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

// ���ص����ļ�������Ϣ�ṹ
struct DOWNLOAD_PROGRESS_STRU
{
	long uIndex;									// �ļ�������������߳�ͳ��
	UINT iCurrentFileSize;					// ��ǰ�������ص��ļ����ֽ���
	UINT iCurrentFileFinishedSize;		// ��ǰ�ļ��������ֽ���
	DWORD dwDownloadUseTime;		// ��ǰ�ļ�������ʱ

	DOWNLOAD_PROGRESS_STRU()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

class CInternet
{
public:
	enum
	{
		INTERNET_SUCCESS = 0,
		INTERNET_ERROR_OPENURL,
		INTERNET_ERROR_FILEOPEN,
		INTERNET_ERROR_READFILE,
		INTERNET_ERROR_OPEN
	};

	BOOL bInternetGetURLUserBreak;	// �ж����ر�ǣ���Ϊ�����ж������ļ�


	CInternet();
	virtual ~CInternet();

	/// ͨ��HTTPЭ���ָ����ַ�����ļ������浽����
	/// sURL����ַ
	/// sSaveFilename��Ҫ����ı����ļ�ȫ·��
	/// sHeader��HTTP����ͷ��Ϣ
	/// hProgressWindow�����������ļ�������Ϣ����ʾ�Ĵ��ھ��
	/// llFileIndex �ļ�����(���ڶ��߳�ͳ�������ֽ���)
	int InternetGetURL(const char* sURL, const char* sSaveFilename, const char* sHeader = NULL, HWND hProgressWindow = NULL, long llFileIndex=-1);
	/// ͨ��HTTPЭ���ָ����ַ��ȡ��ҳ����
	/// sURL����ַ
	/// [@return] ���ػ�ȡ����ҳ����
	static CString InternetGetInfo(CString szUrl);
};
#endif // !defined(INTERNETGETFILE_H)
