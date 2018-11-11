// GameUpdateDlg.h : header file
//

#if !defined(_GAME_UPDATE_DLG_H_20111214_175900)
#define _GAME_UPDATE_DLG_H_20111214_175900

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGameUpdateDlg dialog

#include "RDialog.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "InetSession.h"
#include "InternetGetFile.h"
#include "SkinButton.h"
//#include "../../�����ļ�/ISegmentDownload.h"
//
//#if _DEBUG	
//#pragma comment(lib, "../../���ӿ�/SegmentDownloadD.lib") 
//#else
//#pragma comment(lib, "../../���ӿ�/SegmentDownload.lib") 
//#endif

// �̲߳����ṹ
typedef struct tagThread
{
public:
	LPVOID wndPointer;								// ����ָ��
	BYTE byEventIndex;								// ֹͣ�¼�����
	CInternet Internet;								// ͬ��������
	//ISegmentDownload asynchronous;		// �첽������ 
	bool bAsynchronousStart;						// �첽�����ɹ�

	tagThread()
	{
		bAsynchronousStart = false;
		ZeroMemory(this, sizeof(*this));
	}

	tagThread(LPVOID lPointer, BYTE bIndex)
	{
		wndPointer = lPointer;
		byEventIndex = bIndex;
	}

	//bool InitDowndloadStatus(IDownloadEvent * pDownloadEvent)
	//{
	//	//// ��������
	//	//asynchronous.SetEverySegmentLen(400*1024);			// ����ÿ�εĳ���
	//	//asynchronous.SetOverTimeEverySegment(15*1000);		// ����ÿһ�εĳ�ʱֵ
	//	//asynchronous.SetMaxDownCount(20);							// ����ÿһ�ε��������ش���
	//	//asynchronous.SetTaskThreadNum(4);							// ���ù����̵߳�����
	//	//// ע�ᣬ�ǳ���Ҫ��һ��Ҫд
	//	//asynchronous.RegistEventModule(pDownloadEvent);	
	//	//// ����
	//	//return bAsynchronousStart = asynchronous.Start();
	//}
}ThreadParameter;

typedef enum tagDownloadMode
{
	IE_MODE = 0,								// IE����	
	BreakPoints_MODE = 1,				// IE֧�ֶϵ�����
	UM_DLL_MODE =2						// UM�����ط�ʽ
}DMODE;
//class CUpdateThread;

#define  THREAD_MAX_SIZE						10					// �������߳���Ŀ

class CGameUpdateDlg : public CDialog/*, public IDownloadEvent*/	// ���ص��¼��ӿ�
{
// Construction
public:
	/// standard constructor
	CGameUpdateDlg(CWnd* pParent = NULL);
	virtual ~CGameUpdateDlg();
	/// ���������ļ���
	BOOL SetConfigFile(CString& sFilename);
	// �û���Ϣ
	LRESULT OnUserMessage(WPARAM wParam, LPARAM lParam);
	/// �����ⲿ���� 
	/// [param bMyself in] �����Լ�
	BOOL TransferExternalProgram(BOOL bMyself=FALSE, string strUpdateName="");
	/// ����������ʾ�ؼ�
	void OnUpdateControlContent(LPCTSTR lpContent=NULL);
	void OnNotifyUpdateFinish(BOOL bSuccess = TRUE);
		
	// ���º���
public:
	/// ����ļ�Update.ini�ļ�
	void CheckUpdateFile();
	/// �����Ҫ���µĵ����ļ�
	void CheckSingleFileUpdate(void);
	/// �����Ҫ���µ�Ŀ¼
	void CheckDirectoryUpdate(void);
	/// �����Ҫ���µ������ļ���Ŀ¼
	void CheckWantUpdateFile(void);
	/// ��ʼ����
	void StartUpdate();
	/// ���������ļ�
	void StartCopyFile();
	/// ������
	void CheckProcess(LPCTSTR lpProcessName);
	/// �����ļ�
	BOOL CopyFileUpdate(const string &strFile, const string &strTmpFile, const string &strPath) const;
	/// ��ȫ����
	BOOL OnSuccessCopyFile(const string &strFile, const string &strTmpFile, const string &strPath);
	/// ��ʼ����������
	void InitDowndloadArray();
	/// �̺߳���
	static unsigned __stdcall ThreadWorkFun(LPVOID lParam);
	/// �����ļ�
	/// [@param bSynchronous in] Ϊ��ͬ��
	/// [@param llFileIndex in] �����ļ�ʱ��������С��0ʱ��ͳ�ƣ������첽ʱ�ò�����Ч��
	static BOOL DowndloadFile(ThreadParameter *pWnd, const TCHAR szDowndLoadPath[MAX_PATH], 
		const TCHAR szDowndLoadSavePath[MAX_PATH], long llFileIndex=-1, DMODE bSynchronous=IE_MODE);
	/// У���ļ�
	static BOOL VerifyFile(ThreadParameter *pWnd, const TCHAR szDowndLoadSavePath[MAX_PATH], string strMd5Val);
	/// �߳������ļ�����״̬
	static void ThreadSetFileStatus(ThreadParameter *pWnd, TCHAR tchFirstParam[MAX_PATH], BOOL bComplete, BOOL bDownloading);
	/// ���ػص�
	void OnDownloadResult (int iCookie, char* pszLocalPathFile, int iRes, int iSuccessDownload, int iTotalDownload);
	/// ���ȸ��¼��
	BOOL IsHavePriorityUpdate(string strFileName);
	/// ���ؽ��
	void DownResult(char* p, bool bIsSuc);
	enum { IDD = IDD_GameUpdate };

public:
	HBRUSH						m_HBrush;										///< ��̬�ؼ�������ˢ���
	CSkinButton					m_btnCancel;									///< ȡ��
	CSkinButton					m_btnDown;									///< ����������ť
	CRButton						m_btGenerateHash;						///< ����MD5 HASH
	CString							m_sAppName;								///< Ӧ�ó�����
	CString							m_sConfigFilename;						///< ���������ļ���
	BOOL							m_bSilenceMode;							///< ��Ĭ��ʽִ������������ʾ����������棬ֻ��������Ϻ������û�
	BOOL							m_bUserBreak;								///< �û���ֹ����
	BOOL							m_bGenerateIniFile;						///< �������������ļ�
	BOOL							m_bIsUpdateMySelf;						///< �����Լ�
	//set<string>				   m_setLeachName;							///< �����ļ�������չ��
	string						   m_strExternalProgramFileName;		///< �ⲿ������
	string						   m_strAccount;									///< �ⲿ�˻���
	string						   m_strPW;										///< �ⲿ����

	CBitmap						m_bitmapBG;
	CBitmap						m_bitmapErrorBG;
	CBitmap						m_bitmapPercentSign;					///< �ٷֺ�
	CBitmap						m_ImageDownLoadNum;				///< ��������
	CBitmap						m_ImageProgressBG;						///< ����������
	CBitmap						m_ImageProgressFrontGree;			///< ��ɫ������ֵ
	CBitmap						m_imageProgressFrontRed;			///< ��ɫ������ֵ
	CBitmap						m_bTransfer;									///< ת��
	CBitmap						m_bDownErrorWaring;					///< ���ش���

	int									m_btNums;									///< ת��ͼƬ����
	CString							m_strContent;									///< ���ؽ�������
	long								m_lDownBaiBi;								///< ���ذٷֱ�
	CRect							m_rcDownDrawRect;						///< ���ػ�������
	bool								m_bDownError;								///< ���ش���

	CInternet					  * m_pInternet;									///< ������
	TCHAR							m_chURL[128];								///< ���ݵ�URL��ַ
	HICON							m_hIcon;
	
	// ��Ҫ���µ�
public:
	map<string, FileHashTable>			m_setWantUpdateFile;											///< ��Ҫ���µ��ļ�
	map<string, FileHashTable>			m_setDowndloadFile;											///< �����ص��ļ�
	vector<string>								m_setPriorityDowndload;										///< ���������ļ�
	long												m_lTmpFileCount;													///< ��ʱ�ļ�����
	BOOL											m_bIsCopyfile;														///< �Ѹ����ļ�

	HANDLE										m_hUpdateThread[THREAD_MAX_SIZE];				///< �����߳�
	HANDLE										m_hDownloadEvent[THREAD_MAX_SIZE][2];			///< �����¼�
	HANDLE										m_hStopEvent[THREAD_MAX_SIZE];						///< ֹͣ�¼�
	HANDLE										m_hCompleteEvent[THREAD_MAX_SIZE];				///< ����¼�
	ThreadParameter							m_structThreadParameter[THREAD_MAX_SIZE];		///< �̲߳���
	long												m_uThreadCount;													///< �߳���Ŀ
	DMODE										m_eResumeMode;												///< ���ط�ʽ(IE, ����)

private:
	map<long, ULONGLONG>			m_mDowndloadCount;											///< �첽�����ֽ�ͳ��
	map<string, ULONGLONG>			m_strDowndloadCount;											///< �����ֽ�ͳ��
	DOWNLOAD_INFO_STRU				m_TotalFileInfo;													///< Ҫ���ص��ļ������ʹ�С�ܺ�
	DOWNLOAD_INFO_STRU				m_FinishedFileInfo;												///< �����ص��ļ������ʹ�С�ܺ�
	float												m_fDownloadSpeed;												///< �����ٶ�

	CCriticalSpection							criticalWantUpdateFile;											///< ���� m_setWantUpdateFile
	CCriticalSpection							criticalDowndloadInfo;											///< ���� m_FinishedFileInfo

	//ISegmentDownload						m_stDownload[THREAD_MAX_SIZE];						///< �첽����


	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameUpdateDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

public:
	// Generated message map functions
	//{{AFX_MSG(CGameUpdateDlg)	
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBtnDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual BOOL DestroyWindow();
};

extern class CGameUpdateDlg * g_pCGameUpdateDlg;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GameUpdateDLG_H__4CA1BB40_75FF_4A00_AF96_F141507CE885__INCLUDED_)
