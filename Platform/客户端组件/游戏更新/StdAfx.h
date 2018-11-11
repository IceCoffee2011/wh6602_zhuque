// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__6E171DE5_929C_4558_880B_14D7DDC8781D__INCLUDED_)
#define AFX_STDAFX_H__6E171DE5_929C_4558_880B_14D7DDC8781D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include "afxwin.h"
#include "CDirOperate.h"
#include "ThreadGuard.h"

using namespace std;
// ȡ�ó������е�Ŀ¼
CString GetAppDirectory(BOOL bEndWithBackSlash = TRUE);

// ���ļ�ȫ·��ȡ���ļ���
CString GetFilename(const char* sFilePath);
CString GetFilename(const CString& sFilePath);

// ȡ���ļ���չ��
CString GetFileExtendName(const char* sFilename);
CString GetFileExtendName(const CString& sFilename);

// ȡ���ļ���С
size_t GetFileSize(const char* sFilename);
size_t GetFileSize(const CString& sFilename);

// д��־
static BOOL g_IsWriteLog = TRUE;
void LOG(int iModuleID, int iLevel, const char* sFormat, ...);

// �����ļ���MD5ժҪ��
extern BOOL CalculateMD5(const char* sFilename, unsigned char acMD5Digest[]);
// �����ַ�����MD5ժҪ��
extern BOOL CalculateStringMD5(const char* sString, unsigned int nLen, unsigned char acMD5Digest[]);
extern CString MD5toString(unsigned char acMD5Digest[]);

// ��ʾ��Ϣ�������������AfxMessageBox����
int RMessageBox(CString& sText, UINT nType = MB_OK, UINT nIDHelp = 0);
int RMessageBox(LPCTSTR lpszText, UINT nType = MB_OK, UINT nIDHelp = 0);

// �滻·���е������ַ���Ϊʵ��·��
CString ResolvePath(const char* sPath);

// string ���崮ȫ���滻
string& replace_all(string& str, const string& old_value, const string& new_value);

// �����ļ��ṹ��
typedef struct tagFileHashTable
{
	string filename;	// �ļ���
	string tmpFileName; // ��ʱ�ļ���
	string DATName;		// ��չ��
	string path;		// ��ǰ�����·��
	string Hash;		// ��Ӧ��Hashֵ
	BOOL bDowndloadComplete; // �������
	BOOL bDowndLoading;		// ������
	UINT uDowndloadCount;	// ���ش���
	ULONGLONG size;		// �ļ���С
	
	tagFileHashTable()
	{
		ZeroMemory(this, sizeof(*this));
	}
}FileHashTable;

typedef enum tagThreadRunStartOperator
{
	ONLY_RUN,				// Ĭ���������
	GENERATE,				// ����INI
	GENERATE_RUN,			// ������INI��������
}ThreadRunStartOperator;

#define DOWNDLOAD_TMP_PATH					(_T("tmpupdate"))					// ���ر�����ʱ·��
#define CONFIG_PATH										(_T("config"))							// �����ļ�����·��
#define UPDATE_CONFIG_FILENAME				(_T("update.ini"))					// ���ر�������������ļ���
#define UPDATE_CONFIG_TMP_FILENAME		(_T("updateserver.ini"))			// �����������������ļ���
#define UPDATE_SET_INI_FILE							(_T("updateset.ini"))				// ���»��������ļ�
#define UPDATE_SET_INI_TMP_FILE					(_T("updatesetserver.ini"))		// ���»��������ļ�
#define DOWNDLOAD_DEFAULT_PATH				(_T("http://dls.cbwgame.com/update/hallfile/"))	// Ĭ������·��
#define PROJECT_NAME									(_T("cbwGameUpdate.exe"))	// ��Ŀ����
#define HELP_UPDATE_NAME							(_T("cbwUpdateHelp.exe"))	// ���������߳�
#define	FILE_DOWNLOAD_COUNT					3											// �ļ�����ʧ�����ԵĴ���

const int BUFFER_SIZE = 512;


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__6E171DE5_929C_4558_880B_14D7DDC8781D__INCLUDED_)
