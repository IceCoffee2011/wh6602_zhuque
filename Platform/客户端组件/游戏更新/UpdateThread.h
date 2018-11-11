//------------------------------------------------------------------------------
// �ļ����ƣ�UpdateThread.h
// �ļ��汾��v1.0
// �������ڣ�2006-05-04 14:25
// ��    �ߣ�Richard Ree
// �ļ��������Զ������߳���
//------------------------------------------------------------------------------

#if !defined(AFX_UPDATETHREAD_H__194A514F_A0D7_4ADD_8D2A_9E7081810D82__INCLUDED_)
#define AFX_UPDATETHREAD_H__194A514F_A0D7_4ADD_8D2A_9E7081810D82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// UpdateThread.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUpdateThread thread

class CUpdateThread : public CWinThread
{
	DECLARE_DYNCREATE(CUpdateThread)
protected:

// Attributes
public:
	CString											m_sConfigFilename;						// ���������ļ���
	BOOL											m_bSilenceMode;							// ��Ĭ��ʽִ������������ʾ����������棬ֻ��������Ϻ������û�
	BOOL											m_bUserBreak;								// �û���ֹ����
	HWND											m_hProgressWindow;						// ��ʾ�������ȵĴ��ھ��
	map<string, FileHashTable>			m_setWantUpdateFile;					// ��Ҫ���µ��ļ�
	map<string, FileHashTable>			m_setDowndloadFile;						// �����ص��ļ�
	ULONGLONG									m_CompleteDownByte;					// ������ص��ֽ�
	ThreadRunStartOperator				m_StartOperator;							// �߳̿�ʼ����ʱ�Ĳ���
	
// Operations
public:
	/// protected constructor used by dynamic creation
	CUpdateThread();
	/// ִ������
	BOOL DoUpdateEx();								
	/// ֱ������
	static BOOL DownFile(const char* sURL, const char* sSaveFilename, const char* sHeader, HWND hProgressWindow);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpdateThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CUpdateThread();

	// Generated message map functions
	//{{AFX_MSG(CUpdateThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	//�����ļ�
	// [@param strFile in] �ļ���
	// [@param strTmpFile in] ��ʱ������ļ���
	// [@param strPath in] �ļ����·��
	BOOL DownloadFile(const string &strFile, const string &strTmpFile, const string &strPath)  const; //�����ļ�
	// У���ļ�
	// [@param strFile in] �ļ���
	// [@param strTmpFile in] ��ʱ������ļ���
	// [@param strHash in] �ļ�HASHֵ
	BOOL VerifyFile(const string &strFile, const string &strTmpFile, const string &strHash) const; 
	// �����ļ�
	// [@param strFile in] �ļ���
	// [@param strTmpFile in] ��ʱ������ļ���
	// [@param strPath in] ���·��
	BOOL UpdateFile(const string &strFile, const string &strTmpFile, const string &strPath) const;
	// �����Ҫ���µĵ����ļ�
	void CheckSingleFileUpdate(void);
	// �����Ҫ���µ�Ŀ¼
	void CheckDirectoryUpdate(void);
	// �����Ҫ���µ��ļ�
	void CheckWantUpdateFile(void);
	// ��Ŀ¼�������ļ�����MD5Hash
	// [@param bShush in] �Ƿ񰲾�ģʽ����������Ϸ������ʾ���������Ϣ
	BOOL DoGenerateMD5Hash(BOOL bShush=FALSE);
	// ���ý��������
	// [@param bShush TRUE ���ɸ��� FALSE ֻ����
	// [@param iPos in] ���ý�����λ��
	void SetProgressPos(BOOL bShush, int iPos);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPDATETHREAD_H__194A514F_A0D7_4ADD_8D2A_9E7081810D82__INCLUDED_)
