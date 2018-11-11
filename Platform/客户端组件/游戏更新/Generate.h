#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "RButton.h"

// CGenerate ����MD5�Ի���

class CGenerate : public CRDialog
{
	DECLARE_DYNAMIC(CGenerate)

public:
	CGenerate(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGenerate();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();

// �Ի�������
	enum { IDD = IDD_GENERATE };
	CRichEditCtrl			m_ctrLeachfile;										///< ����&���Ե��ļ�����չ��
	CRButton				m_btGenerateHash;								///< ���ɰ�ť
	TCHAR					m_chURL[128];										///< ���ݵ�URL��ַ
	set<string>			m_setLeachName;									///< �����ļ�������չ��
	vector<string>		m_strLeachFileName;							///< �����ļ���
	BOOL					m_bSilenceMode;									///< ��Ĭ��ʽִ������������ʾ����������棬ֻ��������Ϻ������û�
	BOOL					m_bStopGenerate;								///<  ֹͣ����
	BOOL					m_bComplete;										///< �������
	BOOL					m_bDowndloadServerSetFile;				///< ���ط����������ļ�����
	HANDLE				m_hThread;											///< �߳̾��
	CString					m_sGenerateProgressMsg;					///< ������ʾ������Ϣ

	// ����MD5����
public:
	/// ��Ŀ¼�������ļ�����MD5Hash
	BOOL GenerateMD5Hash();	
	/// �������������ļ�
	BOOL DowndloadGenerateDeployFile(const string strPath, const string strUpdatePath);
	/// ����ָ��Ŀ¼�������ļ�
	/// [vResult out] �������е��ļ���
	/// [strFindPath in] ��Ҫ���ҵ�Ŀ¼
	/// [strDelPath in] �����ļ���ʱ��Ҫ�滻��Ŀ¼
	void SearchCatalogueAllFile(multimap<string, FileHashTable> & vResult, string strFindPath, string strDelPath);	
	/// ���ò���Ҫ���ɵ��ļ�������չ��
	/// [@param bLoadIni in] TRUE ��ȡINI�����ַ��� / FALSE ��ȡ�����ϵĹ����ַ���
	void SetLeachFileName(BOOL bLoadIni);
	/// �������ɱ����˵��ļ�
	void OnSetLeachFileName(string str);
	/// ���ð���ģʽ
	void SetQuietMode(BOOL bMode) { m_bSilenceMode = bMode; }
	/// �߳����ɺ���
	static unsigned __stdcall ThreadGenerateWorkFun(LPVOID lParam);
	/// �����ļ�
	static BOOL DowndloadFile(CGenerate *pWnd, const TCHAR szDowndLoadPath[MAX_PATH], 
		const TCHAR szDowndLoadSavePath[MAX_PATH], long llFileIndex=-1);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ���ɰ�ť�¼�
	afx_msg void OnBnGenerateIniFile();
	afx_msg void OnPaint();
	// �û���Ϣ
	LRESULT OnUserMessage(WPARAM wParam, LPARAM lParam);	
	virtual BOOL DestroyWindow();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
