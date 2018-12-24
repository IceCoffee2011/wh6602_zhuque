#include "StdAfx.h"
#include "Direct.h"
#include "DownLoadService.h"
#include ".\downloadservice.h"
//#include <fstream>
//
//
//std::ofstream out("download-info.txt", std::ios::trunc); 
//
//static void WriteInfo( char* p)
//{
//	out << p << std::endl;
//}


void FunDownLoadFinish(char* p, bool isfinish, void* pObj)
{
	CDownLoadMission* pDownSuc = (CDownLoadMission*)pObj;
	if (NULL != pDownSuc)
	{
		pDownSuc->DownResult(isfinish);
	}
}


//////////////////////////////////////////////////////////////////////////
//�궨��

//��ʱ����ʶ
#define IDI_UPDATE_TIMER					10							//���¶�ʱ��

//��ȡ���� HTML
const TCHAR szErrorHtml1[]=TEXT("<html>");
const TCHAR szErrorHtml2[]=TEXT("<!DOCTYPE HTML PUBLIC");

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDownLoadMission, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BT_HIT_HERE, OnBnClickedHere)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CDownLoadThread::CDownLoadThread()
{
	//���ñ���
	m_bPreparative=false;
	m_szTempFile[0]=0;
	m_szLocalFile[0]=0;
	m_dwDownLoadSize=0;
	m_dwTotalFileSize=0;
	m_pHttpFile=NULL;
	m_pHttpConnection=NULL;
	m_pDownLoadRequest=NULL;
	m_DownLoadResult=enDownLoadResult_Noknow;
	m_DownLoadStatus=enDownLoadStatus_Unknow;

	return;
}

//��������
CDownLoadThread::~CDownLoadThread()
{
	DownLoadCleanUp();
	return;
}

//��ʼ���߳�
bool CDownLoadThread::InitThread(tagDownLoadRequest * pDownLoadRequest)
{
	ASSERT(pDownLoadRequest!=NULL);
	m_pDownLoadRequest=pDownLoadRequest;
	return true;
}

//����״̬
void CDownLoadThread::GetDownLoadStatus(tagDownLoadStatus & DownLoadStatus)
{
	//������Դ
	CThreadLock ThreadLockHandle(m_CriticalSection);

	//���ñ���
	DownLoadStatus.DownLoadStatus=m_DownLoadStatus;
	switch (m_DownLoadStatus)
	{
	case enDownLoadStatus_Unknow:
	case enDownLoadStatus_Ready:
		{
			DownLoadStatus.wProgress=0;
			lstrcpyn(DownLoadStatus.szStatus,TEXT("���ڻ�ȡ�ļ�..."),CountArray(DownLoadStatus.szStatus));
			break;
		}
	case enDownLoadStatus_DownLoadIng:
		{
			DownLoadStatus.wProgress=(WORD)(m_dwDownLoadSize*100L/m_dwTotalFileSize);
			_snprintf(DownLoadStatus.szStatus,sizeof(DownLoadStatus.szStatus),
				TEXT("�Ѹ��ƣ�%ld KB ���� %ld KB��"),m_dwDownLoadSize/1024L,m_dwTotalFileSize/1024L);
			break;
		}
	case enDownLoadStatus_Finish:
		{
			DownLoadStatus.wProgress=100;
			lstrcpyn(DownLoadStatus.szStatus,TEXT("���سɹ�"),CountArray(DownLoadStatus.szStatus));
			break;
		}
	case enDownLoadStatus_Fails:
		{
			DownLoadStatus.wProgress=0;
			_snprintf(DownLoadStatus.szStatus,sizeof(DownLoadStatus.szStatus),TEXT("����ʧ�ܣ�����ţ�%ld"),m_DownLoadResult);
			break;
		}
	default: 
		{
			ASSERT(FALSE);
			memset(&DownLoadStatus,0,sizeof(DownLoadStatus));
			break;
		}
	}

	return;
}

//Ŀ���ļ�
LPCTSTR CDownLoadThread::GetDownLoadFileName()
{
	ASSERT(m_DownLoadStatus==enDownLoadStatus_Finish);
	return m_szLocalFile;
}

//���к���
bool CDownLoadThread::OnEventThreadRun()
{
	try
	{
		if (m_bPreparative==true)
		{//��ʼ�������

			//��ȡ�ļ�
			TCHAR szBuffer[4096]={0};
			UINT uReadCount=m_pHttpFile->Read(szBuffer,sizeof(szBuffer));

			if (uReadCount>0)
			{//��������
				//д���ļ�
				m_LocalFile.Write(szBuffer,uReadCount);

				//������Դ
				CThreadLock ThreadLockHandle(m_CriticalSection);

				//���ñ���
				m_dwDownLoadSize+=uReadCount;
				m_DownLoadStatus=enDownLoadStatus_DownLoadIng;

				return true;
			}
			else 
			{//�������
				//�ر��ļ�
				m_LocalFile.Close();

				//�ƶ��ļ�
				_snprintf(m_szLocalFile,sizeof(m_szLocalFile),TEXT("%s\\%s"),m_pDownLoadRequest->szLocalPath,m_pDownLoadRequest->szFileName);
				DeleteFile(m_szLocalFile);
				if (MoveFileWithProgress(m_szTempFile,m_szLocalFile,NULL, NULL, MOVEFILE_WRITE_THROUGH|MOVEFILE_REPLACE_EXISTING)==FALSE)
					throw enDownLoadResult_CreateFileFails;

				//������Դ
				CThreadLock ThreadLockHandle(m_CriticalSection);

				//���ñ���
				m_DownLoadStatus=enDownLoadStatus_Finish;

				//��������
				DownLoadCleanUp();

				return false;
			}
		}
		else 
		{
			//������Դ
			CThreadLock ThreadLockHandle(m_CriticalSection);

			//���ñ���
			m_DownLoadStatus=enDownLoadStatus_Ready;

			//����׼��
			DownLoadPreparative();

			return true;
		}
	}
	catch (enDownLoadResult DownLoadResult)
	{
		//������Դ
		CThreadLock ThreadLockHandle(m_CriticalSection);

		//���ñ���
		m_DownLoadStatus=enDownLoadStatus_Fails;
		m_DownLoadResult=DownLoadResult;
	}
	catch (CInternetException * pInternetException)
	{
		//�����쳣
		pInternetException->Delete();

		//������Դ
		CThreadLock ThreadLockHandle(m_CriticalSection);

		//���ñ���
		m_DownLoadStatus=enDownLoadStatus_Fails;
		m_DownLoadResult=enDownLoadResult_Exception;
	}
	catch (...) 
	{
		//������Դ
		CThreadLock ThreadLockHandle(m_CriticalSection);

		//���ñ���
		m_DownLoadStatus=enDownLoadStatus_Fails;
		m_DownLoadResult=enDownLoadResult_Exception;
	}

	//������
	DownLoadCleanUp();

	return false;
}

//�ر��¼�
bool CDownLoadThread::OnEventThreadConclude()
{
	DownLoadCleanUp();
	return true;
}

//����׼��
void CDownLoadThread::DownLoadPreparative()
{
	//Ч��״̬
	ASSERT(m_pHttpFile==NULL);
	ASSERT(m_bPreparative==false);

	//���ñ���
	m_szTempFile[0]=0;
	m_szLocalFile[0]=0;
	m_dwDownLoadSize=0;
	m_dwTotalFileSize=0;

	//����Ŀ¼
	int nExcursion=0;
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	lstrcpyn(szDirectory,m_pDownLoadRequest->szLocalPath,CountArray(szDirectory));
	do
	{
		if (szDirectory[nExcursion]==0) 
		{
			::CreateDirectory(szDirectory,NULL);
			break;
		}
		if (szDirectory[nExcursion]==TEXT('\\'))
		{
			szDirectory[nExcursion]=0;
			::CreateDirectory(szDirectory,NULL);
			szDirectory[nExcursion]=TEXT('\\');
		}
		nExcursion++;
	} while (true);

	//�ж���������
	int nStepCount=0;
	DWORD dwStatusCode =0;
	TCHAR szDownBuffer[4096]={0};
	try
	{
		//��ȡ����
		UINT nReadCount=0;
		DWORD dwType;
		CString strServer, strObject;
		INTERNET_PORT wPort;
		DWORD dwStateCode;
		

		m_InternetSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 5000);      // 5������ӳ�ʱ
		m_InternetSession.SetOption(INTERNET_OPTION_SEND_TIMEOUT, 2000);           // 1��ķ��ͳ�ʱ    
		m_InternetSession.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 15000);        // 7��Ľ��ճ�ʱ
		m_InternetSession.SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, 2000);     // 1��ķ��ͳ�ʱ
		m_InternetSession.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 15000);       // 7��Ľ��ճ�ʱ
		m_InternetSession.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 5);          // 1������
		
		AfxParseURL(m_pDownLoadRequest->szDownLoadUrl, dwType, strServer, strObject, wPort);

		m_pHttpConnection = m_InternetSession.GetHttpConnection(strServer, wPort, NULL, NULL);

		m_pHttpFile = m_pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject);


		m_pHttpFile=(CHttpFile *)m_InternetSession.OpenURL(m_pDownLoadRequest->szDownLoadUrl,
															1,
															INTERNET_FLAG_TRANSFER_ASCII|INTERNET_FLAG_DONT_CACHE);
		//m_pHttpFile->SendRequest();
		//m_pHttpFile->QueryInfoStatusCode(dwStateCode);
		//if(dwStateCode != HTTP_STATUS_OK)
		//{
		//	return;
		//}

		DWORD dwFileSize = 0;   //�ļ�����
		DWORD dwInfoBufferSize = sizeof(szDownBuffer);
		BOOL bResult = FALSE;
		bResult = m_pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH,
			(void*)szDownBuffer,&dwInfoBufferSize,NULL);
		if (FALSE==bResult)
		{
			return;
		}

		m_dwTotalFileSize = atoi(szDownBuffer);
		memset(szDownBuffer, 0, sizeof(szDownBuffer));
		nReadCount = m_pHttpFile->Read(szDownBuffer, sizeof(szDownBuffer)); //��ȡ������������

		//nStepCount++;
		//m_pHttpFile->QueryInfoStatusCode(dwStatusCode);
		//nReadCount=m_pHttpFile->Read(szDownBuffer,sizeof(szDownBuffer));
		//nStepCount++;
		//if ( nReadCount < sizeof( szErrorHtml1 ) ) 
		//{
		//	throw 0;
		//}
		//nStepCount++;
		//if ( nReadCount < sizeof( szErrorHtml2 ) ) 
		//{
		//	throw 0;
		//}
		//nStepCount++;
		//if (memcmp(szErrorHtml1,szDownBuffer,lstrlen(szErrorHtml1))==0) 
		//{
		//	throw 0;
		//}
		//nStepCount++;
		//if (memcmp(szErrorHtml2,szDownBuffer,lstrlen(szErrorHtml2))==0)
		//{
		//	throw 0;
		//}
		//nStepCount++;

		////��ȡ��С
		//ASSERT(m_pHttpFile!=NULL);
		//BOOL bSuccess=m_pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER,m_dwTotalFileSize);
		//nStepCount++;
		//if ( FALSE == bSuccess )
		//{
		//	//CString strError;
		//	//DWORD dwError=GetLastError();
		//	//strError.Format(TEXT("����ţ�%ld"),dwError);
		//	//AfxMessageBox(strError);
		//	throw enDownLoadResult_InternetReadError;
		//}

		nStepCount++;

		//�����ļ�
		_snprintf(m_szTempFile,
				  sizeof(m_szTempFile),
				  TEXT("%s\\%s.DTM"),
				  m_pDownLoadRequest->szLocalPath,
				  m_pDownLoadRequest->szFileName);

		if (FALSE == m_LocalFile.Open(m_szTempFile,
							CFile::modeReadWrite|CFile::modeCreate))
		{
			throw enDownLoadResult_CreateFileFails;
		}

		nStepCount++;

		//д���ļ�
		m_LocalFile.Write(szDownBuffer,nReadCount);
		nStepCount++;

		//������Դ
		CThreadLock ThreadLockHandle(m_CriticalSection);
		nStepCount++;

		//���ñ���
		m_dwDownLoadSize+=nReadCount;
		m_DownLoadStatus=enDownLoadStatus_DownLoadIng;

		//���ñ���
		m_bPreparative=true;

		return;
	}
	catch (...)
	{ 
		//CString strError;
		//DWORD dwError=GetLastError();
		//strError.Format(TEXT("ִ�в��裺%ld,״̬�룺%ld"),nStepCount,dwStatusCode);
		//AfxMessageBox(szDownBuffer);
		//SafeDelete(m_pHttpFile);
		throw enDownLoadResult_InternetReadError;
	}

	return;
}

//�������
void CDownLoadThread::DownLoadCleanUp()
{
	//���ñ���
	m_dwDownLoadSize=0;
	m_dwTotalFileSize=0;
	m_bPreparative=false;

	//������Դ
	if (m_pHttpFile!=NULL) 
	{
		m_pHttpFile->Close();
		SafeDelete(m_pHttpFile);
	}
	if (m_pHttpConnection!=NULL)
	{
		m_pHttpConnection->Close();
		SafeDelete(m_pHttpConnection);
	}

	//m_InternetSession.Close();
	if (m_LocalFile!=INVALID_HANDLE_VALUE) m_LocalFile.Close();

	//ɾ���ļ�
	if (m_szTempFile[0]!=0) DeleteFile(m_szTempFile);

	return;
}

//////////////////////////////////////////////////////////////////////////


//ģ�鶨��
#ifdef _DEBUG
#define GAME_DOWNLOAD_DLL_NAME	TEXT("DownLoadD.dll")					//��� DLL ����
#else
#define GAME_DOWNLOAD_DLL_NAME	TEXT("DownLoad.dll")					//��� DLL ����
#endif


const int IWND_WIDTH		=	536;
const int IWND_HEIGHT		=	150;


const int I_DOWNPROCESS_NUMS	=	19;
const int I_DOWNFAIL_NUMS		=	2;


const int IDI_DOWN_ING_TIMER	=	2001;
const int IDI_DOWN_FAIL_TIMER	=	2002;


const int TIMER_DOWNING_INTERVAL	=	100;
const int TIMER_DOWNFAIL_INTERVAL	=	500;


const int I_FLASH_WIDTH	=	90;
const int I_FLASH_HEIGHT =	86;


//���캯��
CDownLoadMission::CDownLoadMission(IDownLoadMissionSink * pIDownLoadMissionSink) : CDialog(IDD_DOWN_LOAD)
{
	//���ñ���
	m_dwMissionID=0;
	m_dwDownLoadType=0;
	m_pIDownLoadMissionSink=pIDownLoadMissionSink;
	//m_DownLoadThread.InitThread(&m_DownLoadRequest);
	memset(&m_DownLoadRequest,0,sizeof(m_DownLoadRequest));

	m_iDownTag = 0;
	m_byDownLoadTag = 1;
	m_iDownBaiBi = 0;
	m_iSaveDownBaiBi = 0;

	m_iCountDownSuc = 0;
	m_iCountDownFail = 0;

	m_memdc.m_hDC = NULL;
	return;
}

//��������
CDownLoadMission::~CDownLoadMission()
{
	ReleaseDC(&m_memdc);
	m_memdc.m_hDC = NULL;
}

//�ؼ���
void CDownLoadMission::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BT_HIT_HERE, m_btHitHereBT);
	DDX_Control(pDX, IDCANCEL, m_btCloseBT);
}

//��ʼ������
BOOL CDownLoadMission::OnInitDialog()
{
	__super::OnInitDialog();

	HINSTANCE hInstance=GetModuleHandle(IDOWN_LOAD_DLL_NAME);
	TCHAR szCurrentPath[MAX_PATH];
	::memset(szCurrentPath, 0, sizeof(szCurrentPath));
	GetCurrentDirectory(MAX_PATH,szCurrentPath);

	CString strResourcePath2="";
	strResourcePath2.Format("%s\\data\\ClientShare\\download\\",szCurrentPath);
	
	m_ImageDownLoadFlash.DestroyImage();
	m_ImageDownLoadFlash.LoadImage(TEXT(strResourcePath2 + "DownLoadFlash.png"));
	m_ImageDownLoadNum.DestroyImage();
	m_ImageDownLoadNum.LoadImage(TEXT(strResourcePath2 + "DownLoadNum.png"));
	m_ImageFailFlash.DestroyImage();
	m_ImageFailFlash.LoadImage(TEXT(strResourcePath2 + "FailFlash.png"));
	m_ImageImageBack.DestroyImage();
	m_ImageImageBack.LoadImage(TEXT(strResourcePath2 + "ImageBack.png"));
	m_ImageImageFrontGree.DestroyImage();
	m_ImageImageFrontGree.LoadImage(TEXT(strResourcePath2 + "ImageFrontGreen.png"));
	m_ImageImageFrontRed.DestroyImage();
	m_ImageImageFrontRed.LoadImage(TEXT(strResourcePath2 + "ImageFrontRed.png"));
	m_ImageImageBai.DestroyImage();
	m_ImageImageBai.LoadImage(TEXT(strResourcePath2 + "ImageBai.png"));
	m_ImageDownBack.DestroyImage();
	m_ImageDownBack.LoadImage(TEXT(strResourcePath2 + "ImageDown.png"));
	m_ImageUpdateBack.DestroyImage();
	m_ImageUpdateBack.LoadImage(TEXT(strResourcePath2 + "ImageUpdate.png"));
	m_ImageDownFail.DestroyImage();
	m_ImageDownFail.LoadImage(TEXT(strResourcePath2 + "ImageDownFail.png"));
	
	
	m_btHitHereBT.SetButtonImage(TEXT(strResourcePath2 + "ImageHere.png"), false);
	m_btCloseBT.SetButtonImage(TEXT(strResourcePath2 + "Close.png"), false);

	SetWindowPos(NULL, 0, 0, IWND_WIDTH, IWND_HEIGHT, SWP_NOMOVE);

	m_btHitHereBT.SetWindowPos(NULL, 349, 94, 0, 0, SWP_NOSIZE);
	m_btCloseBT.SetWindowPos(NULL, IWND_WIDTH-36, 10, 0, 0, SWP_NOSIZE);

	m_iDownTag = 0;
	m_iDownBaiBi = 0;
	m_iSaveDownBaiBi = 0;

	m_iCountDownSuc = 0;
	m_iCountDownFail = 0;

	CDC *pDC = GetDC();
	m_memdc.m_hDC = NULL;
	m_memdc.CreateCompatibleDC(pDC);
	CBitmap hbmp;
	hbmp.CreateCompatibleBitmap(pDC, IWND_WIDTH, IWND_HEIGHT);
	m_memdc.SelectObject(&hbmp);
	m_memdc.SetBkMode(TRANSPARENT);
	hbmp.DeleteObject();
	ReleaseDC(pDC);
	pDC = NULL;


	m_rcFlash.left = 16;
	m_rcFlash.top = 30;
	m_rcFlash.right = m_rcFlash.left + I_FLASH_WIDTH+4;
	m_rcFlash.bottom = m_rcFlash.top + I_FLASH_HEIGHT+4;

	m_rcNums.left = 260;
	m_rcNums.top = 25;
	m_rcNums.right = m_rcNums.left + 100;
	m_rcNums.bottom = m_rcNums.top + 30;

	m_rcProcess.left = 120;
	m_rcProcess.top = 65;
	m_rcProcess.right = m_rcProcess.left + 394;
	m_rcProcess.bottom = m_rcProcess.top + 20;

	//���ö�ʱ��
	SetTimer(IDI_DOWN_ING_TIMER,TIMER_DOWNING_INTERVAL,NULL);

	return TRUE;
}


void CDownLoadMission::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	//__super::OnPaint();
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CSkinDialogEx::OnPaint()

	if (NULL != m_memdc.m_hDC)
	{
		if (1==m_byDownLoadTag || 2==m_byDownLoadTag)
		{
			if (1==m_byDownLoadTag && !m_ImageDownBack.IsNull())
			{//���ؽ���
				m_ImageDownBack.DrawImage(&m_memdc, 0, 0);
			}
			else if (2==m_byDownLoadTag && !m_ImageUpdateBack.IsNull())
			{//���½���
				m_ImageUpdateBack.DrawImage(&m_memdc, 0, 0);
			}

			if (!m_ImageDownLoadNum.IsNull())
			{//��������
				CPoint ptNum;
				ptNum.x = m_rcNums.left;
				ptNum.y = m_rcNums.top;
				int iNums[3] = {-1};
				int iCount = 0;
				int iValue = m_iDownBaiBi; 
				m_iSaveDownBaiBi = m_iDownBaiBi;
				if (0==iValue)
				{
					iNums[0] = 0;
					iCount = 1;
				}
				else
				{
					while(iValue>0&&iCount<3)
					{
						iNums[iCount++] = iValue%10;
						iValue = iValue/10;
					}
				}
				for (int i=iCount-1; i>=0; --i)
				{
					if (iNums[i]>=0)
					{
						m_ImageDownLoadNum.DrawImage(&m_memdc, ptNum.x, ptNum.y+2, 20, 23, iNums[i]*20, 0, 20, 23);
						ptNum.x += 20;
					}
				}
				if (m_iDownBaiBi>=0 && !m_ImageImageBai.IsNull())
				{//�ٷֺ�
					m_ImageImageBai.DrawImage(&m_memdc, ptNum.x, ptNum.y+2);
				}
			}

			//���ƽ�����
			if (!m_ImageImageBack.IsNull())
			{
				m_ImageImageBack.DrawImage(&m_memdc, m_rcProcess.left, m_rcProcess.top);
			}
			if (!m_ImageImageFrontGree.IsNull())
			{
				int iValue = m_iDownBaiBi*3.94;
				m_ImageImageFrontGree.DrawImage(&m_memdc, m_rcProcess.left, m_rcProcess.top, iValue, 17, 0, 0, iValue, 17);
			}

			//���ƶ���
			if (!m_ImageDownLoadFlash.IsNull())
			{
				m_ImageDownLoadFlash.DrawImage(&m_memdc, m_rcFlash.left, m_rcFlash.top, I_FLASH_WIDTH, I_FLASH_HEIGHT,	\
					m_iCountDownSuc*I_FLASH_WIDTH, 0, I_FLASH_WIDTH, I_FLASH_HEIGHT);
			}
		}
		else
		{//����ʧ�ܽ���
			if (!m_ImageDownFail.IsNull())
			{
				m_ImageDownFail.DrawImage(&m_memdc, 0, 0);
			}

			//���ƽ�����
			if (!m_ImageImageBack.IsNull())
			{
				m_ImageImageBack.DrawImage(&m_memdc, m_rcProcess.left, m_rcProcess.top);
			}
			if (!m_ImageImageFrontRed.IsNull())
			{
				int iValue = m_iDownBaiBi*3.94;
				m_ImageImageFrontRed.DrawImage(&m_memdc, m_rcProcess.left, m_rcProcess.top, iValue, 17, 0, 0, iValue, 17);
			}

			//���ƶ���
			if (!m_ImageFailFlash.IsNull())
			{
				m_ImageFailFlash.DrawImage(&m_memdc, m_rcFlash.left, m_rcFlash.top, I_FLASH_WIDTH, I_FLASH_HEIGHT,	\
					m_iCountDownFail*I_FLASH_WIDTH, 0, I_FLASH_WIDTH, I_FLASH_HEIGHT);
			}
		}

		dc.BitBlt(0,0, IWND_WIDTH, IWND_HEIGHT, &m_memdc, 0,0, SRCCOPY);
	}
}



//////////////////////////////////////////////////////////////////////////

BOOL CDownLoadMission::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CDownLoadMission::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialog::OnLButtonDown(nFlags, point);
}


//��Ϣ����
BOOL CDownLoadMission::PreTranslateMessage(MSG * pMsg)
{
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE||VK_RETURN==pMsg->wParam ||VK_SPACE==pMsg->wParam)) return TRUE;
	return __super::PreTranslateMessage(pMsg);
}

//ȡ����Ϣ
void CDownLoadMission::OnCancel()
{
	KillTimer(IDI_DOWN_ING_TIMER);
	KillTimer(IDI_DOWN_FAIL_TIMER);

	//ֹͣ����
	//m_DownLoadThread.ConcludeThread(INFINITE);

	//�¼�֪ͨ
	m_pIDownLoadMissionSink->OnMissionFinish(enDownLoadStatus_Fails,this);

	//�رս���
	DestroyWindow();

	return;
}

//���԰�ť
void CDownLoadMission::OnBnClickedReTry()
{
	KillTimer(IDI_DOWN_ING_TIMER);
	KillTimer(IDI_DOWN_FAIL_TIMER);


	//��������
	//m_DownLoadThread.StartThread();

	CCDownloadManager::getInstance().GoOn();

	//����ʱ��
	SetTimer(IDI_DOWN_ING_TIMER,TIMER_DOWNING_INTERVAL,NULL);

	return;
}

//�������
void CDownLoadMission::OnBnClickedHere()
{
	if (!m_strNetURL.empty())
	{
		ShellExecute(NULL,TEXT("open"),m_strNetURL.c_str(),NULL,NULL,SW_SHOWDEFAULT);
	}
}


//���ؽ��
void CDownLoadMission::DownResult(bool bIsSuc)
{
	if (bIsSuc)
	{//���سɹ�
		m_iDownTag = 2;	
	}
	else
	{//����ʧ��
		m_iDownTag = 3;	
	}
}


//��ʱ����Ϣ
void CDownLoadMission::OnTimer(UINT_PTR nIDEvent)
{
	__super::OnTimer(nIDEvent);

	//����״̬
	if (nIDEvent==IDI_DOWN_ING_TIMER)
	{
		if (1 == m_iDownTag)
		{
			unsigned __int64  fileSize   =  CCDownloadManager::getInstance().GetFileSize();
			unsigned __int64  downSize   =  CCDownloadManager::getInstance().GetDownLoadSize();
			//int speed = CCDownloadManager::getInstance().GetSpeed();
			if (fileSize > 0)
			{
				float rate = ((DOUBLE)downSize / (DOUBLE)fileSize)*100;
				m_iDownBaiBi = (int)rate;

				//sprintf(buf, "%0.2fKB/%0.2fKB(%0.2fKB/s)", downSize / 1024.0f, fileSize / 1024.0f, (float)speed / 1024.0f);//�����ش�С/�����ش�С
				//SetDlgItemText(IDC_STATUS,buf);
			}

			++m_iCountDownSuc;
			if (m_iCountDownSuc >= I_DOWNPROCESS_NUMS)
			{
				m_iCountDownSuc = 0;
			}
		}
		else if (2 == m_iDownTag)
		{//�������
			KillTimer(IDI_DOWN_ING_TIMER);
			KillTimer(IDI_DOWN_ING_TIMER);

			CCDownloadManager::getInstance().Clear();
			
			DestroyWindow();

			//�¼�֪ͨ
			m_pIDownLoadMissionSink->OnMissionFinish(enDownLoadStatus_Finish,this);

			std::string strFilePath="";
			strFilePath.append(m_DownLoadRequest.szLocalPath);
			strFilePath.append("\\");
			strFilePath.append(m_DownLoadRequest.szFileName);
			//���а�װ�ļ�
			ShellExecute(NULL,TEXT("open"),strFilePath.c_str(),NULL,NULL,SW_SHOWDEFAULT);
		}
		else if (3 == m_iDownTag)
		{//����ʧ��
			//���ý���
			KillTimer(IDI_DOWN_ING_TIMER);
			KillTimer(IDI_DOWN_ING_TIMER);
			//���ý���
			m_byDownLoadTag = 3;
			m_iCountDownFail = 0;
			
			SetTimer(IDI_DOWN_FAIL_TIMER, TIMER_DOWNFAIL_INTERVAL, NULL);
			InvalidateRect(&CRect(100, 10, 500, 50), FALSE);
		}
	}
	else if (IDI_DOWN_FAIL_TIMER == nIDEvent)
	{//����ʧ�ܶ���
		m_iCountDownFail = !m_iCountDownFail;
	}

	if(GetSafeHwnd())
	{
		InvalidateRect(&m_rcFlash, FALSE);
		InvalidateRect(&m_rcNums, FALSE);
		InvalidateRect(&m_rcProcess, FALSE);
	}


	////���ÿؼ�
	//tagDownLoadStatus DownLoadStatus;
	//m_DownLoadThread.GetDownLoadStatus(DownLoadStatus);
	//m_ProgressCtrl.SetPos(DownLoadStatus.wProgress);
	//SetDlgItemText(IDC_STATUS,DownLoadStatus.szStatus);

	////״̬�л�
	//switch (DownLoadStatus.DownLoadStatus)
	//{
	//case enDownLoadStatus_Fails:
	//	{
	//		//���ý���
	//		KillTimer(IDI_UPDATE_TIMER);
	//		m_btReTry.EnableWindow(TRUE);
	//		
	//		//�¼�֪ͨ
	//		if (m_DownLoadRequest.bDisplay==false)
	//		{
	//			m_pIDownLoadMissionSink->OnMissionFinish(enDownLoadStatus_Fails,this);
	//			DestroyWindow();
	//		}

	//		break;
	//	}
	//case enDownLoadStatus_Finish:
	//	{//�������
	//		DestroyWindow();
	//		LPCTSTR pszLocalFile=m_DownLoadThread.GetDownLoadFileName();
	//		ShellExecute(NULL,TEXT("open"),pszLocalFile,NULL,NULL,SW_SHOWDEFAULT);

	//		//�¼�֪ͨ
	//		m_pIDownLoadMissionSink->OnMissionFinish(enDownLoadStatus_Finish,this);

	//		break;
	//	}
	//}
	return;
}

//��ʾ����
bool CDownLoadMission::DisplayDownLoadFace(bool bDisplay)
{
	//���ñ���
	m_DownLoadRequest.bDisplay=bDisplay;

	//��ʾ����
	if (m_hWnd!=NULL)
	{
		ShowWindow(bDisplay?SW_SHOW:SW_HIDE);
	}

	return true;
}

//�Ա�����
bool CDownLoadMission::CompareRequest(tagDownLoadRequest * pDownLoadRequest)
{
	if (lstrcmp(m_DownLoadRequest.szDownLoadUrl,pDownLoadRequest->szDownLoadUrl)!=0) return false;
	return true;
}

//��ʼ����
bool CDownLoadMission::StartDownLoad(DWORD dwMissionID, DWORD dwDownLoadType, tagDownLoadRequest * pDownLoadRequest)
{
	if (NULL==pDownLoadRequest)
	{
		return false;
	}
	
	//���ñ���
	m_dwMissionID=dwMissionID;
	m_dwDownLoadType=dwDownLoadType;
	CopyMemory(&m_DownLoadRequest,pDownLoadRequest,sizeof(tagDownLoadRequest));
	if (m_DownLoadRequest.szLocalPath[0]==0)
	{
		GetCurrentDirectory(sizeof(m_DownLoadRequest.szLocalPath),
							m_DownLoadRequest.szLocalPath);
	}

	//��������
	//m_DownLoadThread.StartThread();

	std::string strURL="", strFilePath="";
	strURL.append(pDownLoadRequest->szDownLoadUrl);
	strFilePath.append(pDownLoadRequest->szLocalPath);
	strFilePath.append("\\");
	strFilePath.append(pDownLoadRequest->szFileName);

	m_strNetURL = "";
	m_strNetURL.append(strURL);

	m_strLocalFilePath = "";
	m_strLocalFilePath.append(strFilePath);

	DeleteFile(strFilePath.c_str());

	//��ʼ����
	CCDownloadManager::getInstance().Init(strURL.c_str() ,  strFilePath.c_str(), FunDownLoadFinish, this);
	CCDownloadManager::getInstance().Start();

	//��������
	AfxSetResourceHandle(GetModuleHandle(IDOWN_LOAD_DLL_NAME));
	Create(IDD_DOWN_LOAD,AfxGetMainWnd());
	AfxSetResourceHandle(GetModuleHandle(NULL));
	if (m_DownLoadRequest.bDisplay==true)
	{
		m_iDownTag = 1;
		m_byDownLoadTag = pDownLoadRequest->byDownLoadTag;
		ShowWindow(SW_SHOW);
	}

	return true;
}

//ֹͣ����
bool CDownLoadMission::StopDownLoad()
{
	KillTimer(IDI_DOWN_ING_TIMER);

	//ֹͣ�߳�
	//m_DownLoadThread.ConcludeThread(INFINITE);
	CCDownloadManager::getInstance().Clear();

	//�رս���
	DestroyWindow();

	return true;
}

//////////////////////////////////////////////////////////////////////////


//���캯��
CDownLoadService::CDownLoadService()
{
	m_dwMissionID=0;
	m_pIDownLoadServiceSink=NULL;
}

//��������
CDownLoadService::~CDownLoadService()
{
	//��������
	CDownLoadMission * pDownLoadMission=NULL;
	for (INT_PTR i=0;i<m_DownLoadMissionActive.GetCount();i++)
	{
		pDownLoadMission=m_DownLoadMissionActive[i];
		pDownLoadMission->StopDownLoad();
		SafeDelete(pDownLoadMission);
	}
	for (INT_PTR i=0;i<m_DownLoadMissionRelease.GetCount();i++)
	{
		pDownLoadMission=m_DownLoadMissionRelease[i];
		SafeDelete(pDownLoadMission);
	}
	m_DownLoadMissionActive.RemoveAll();
	m_DownLoadMissionRelease.RemoveAll();
	
	return;
}

//�ӿڲ�ѯ
void * __cdecl CDownLoadService::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	//QUERYINTERFACE(IDownLoadService,Guid,dwQueryVer);
	//QUERYINTERFACE_IUNKNOWNEX(IDownLoadService,Guid,dwQueryVer);
	return NULL;
}

//��ȡ��Ŀ
INT_PTR __cdecl CDownLoadService::GetDownLoadMissionCount()
{
	return m_DownLoadMissionActive.GetCount();
}

//���ýӿ�
bool __cdecl CDownLoadService::SetDownLoadServiceSink(IUnknownEx * pIUnknownEx)
{
	ASSERT(pIUnknownEx!=NULL);
	m_pIDownLoadServiceSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IDownLoadServiceSink);
	ASSERT(m_pIDownLoadServiceSink!=NULL);
	return (m_pIDownLoadServiceSink!=NULL);
}

//��������
DWORD __cdecl CDownLoadService::AddDownLoadRequest(DWORD dwDownLoadType, tagDownLoadRequest * pDownLoadRequest)
{
	//Ч�����
	ASSERT(pDownLoadRequest!=NULL);
	if (pDownLoadRequest==NULL) return 0;

	//Ѱ������
	CDownLoadMission * pDownLoadMission=SearchMission(pDownLoadRequest);
	if (pDownLoadMission!=NULL)
	{
		if (pDownLoadRequest->bDisplay==true) pDownLoadMission->DisplayDownLoadFace(pDownLoadRequest->bDisplay);
		return pDownLoadMission->GetMissionID();
	}

	//��������
	INT_PTR nReleaseCount=m_DownLoadMissionRelease.GetCount();
	if (nReleaseCount>0)
	{
		pDownLoadMission=m_DownLoadMissionRelease[nReleaseCount-1];
		m_DownLoadMissionRelease.RemoveAt(nReleaseCount-1);
	}
	else
	{
		try {
			pDownLoadMission=new CDownLoadMission(this); 
		}
		catch (...) {
			return 0; 
		}
	}
	ASSERT(pDownLoadMission!=NULL);
	if (pDownLoadMission==NULL) 
	{
		return 0;
	}
	m_DownLoadMissionActive.Add(pDownLoadMission);

	//��������
	m_dwMissionID++;
	pDownLoadMission->StartDownLoad(m_dwMissionID,dwDownLoadType,pDownLoadRequest);
	return pDownLoadMission->GetMissionID();
}

//����֪ͨ
void CDownLoadService::OnMissionFinish(enDownLoadStatus DownLoadStatus, CDownLoadMission * pDownLoadMission)
{
	//Ѱ������
	int i=0;
	for (i=0;i<m_DownLoadMissionActive.GetCount();i++)
	{
		if (pDownLoadMission==m_DownLoadMissionActive[i]) break;
	}
	ASSERT(i!=m_DownLoadMissionActive.GetCount());
	if (i==m_DownLoadMissionActive.GetCount()) return;

	//��������
	m_DownLoadMissionActive.RemoveAt(i);
	m_DownLoadMissionRelease.Add(pDownLoadMission);

	return;
}

//��������
CDownLoadMission * CDownLoadService::SearchMission(tagDownLoadRequest * pDownLoadRequest)
{
	CDownLoadMission * pDownLoadMission=NULL;
	for (INT_PTR i=0;i<m_DownLoadMissionActive.GetCount();i++)
	{
		pDownLoadMission=m_DownLoadMissionActive[i];
		if (pDownLoadMission->CompareRequest(pDownLoadRequest)==true) return pDownLoadMission;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////

//����������
extern "C" __declspec(dllexport) void * __cdecl CreateDownLoadService(const GUID & Guid, DWORD dwInterfaceVer)
{
	//��������
	CDownLoadService * pDownLoadService=NULL;
	try
	{
		pDownLoadService=new CDownLoadService();
		if (pDownLoadService==NULL) throw TEXT("����ʧ��");
		void * pObject=pDownLoadService->QueryInterface(Guid,dwInterfaceVer);
		if (pObject==NULL) throw TEXT("�ӿڲ�ѯʧ��");
		return pObject;
	}
	catch (...) {}

	//�������
	SafeDelete(pDownLoadService);
	return NULL;
}
void CDownLoadMission::OnDestroy()
{
	m_iDownTag = 0;
	CCDownloadManager::getInstance().Clear();

	CDialog::OnDestroy();
	// TODO: �ڴ˴������Ϣ����������
}
