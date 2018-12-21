#include <afxwin.h>
#include "../Resource.h"
#include "RechargeResult.nsmap"
#include "../Commonality.h"
#include "../LogonService.h"


//��ֵ��־
void TraceRechargeMessage(LPCTSTR pszMessage)
{
	if(0 == lstrlen(pszMessage))
	{
		return ;
	}

	SYSTEMTIME sys; 
	GetLocalTime( &sys ); 
	char buf[128];
	memset(buf, 0, 128);
	sprintf(buf, "%4d/%02d/%02d %02d:%02d:%02d.%03d ",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds);

	//���������־
	char filename[128];
	memset(filename, 0, 128);
	sprintf(filename, "Recharge_TraceData_%4d%02d%02d.txt",sys.wYear,sys.wMonth,sys.wDay);

	CFile File;
	if ((File.Open(filename,CFile::modeWrite)==FALSE)&&
		(File.Open(filename,CFile::modeWrite|CFile::modeCreate)==FALSE))
	{
		ASSERT(FALSE);
		return;
	}

	File.SeekToEnd();
	File.Write(buf,lstrlen(buf));
	File.Write(pszMessage,lstrlen(pszMessage));
	File.Write(TEXT("\r\n"),lstrlen(TEXT("\r\n")));

	File.Flush();
	File.Close();

	return;
}

//3�������ֱ�Ϊ֧�������app_key����phpһ��������ʶ�������ǵ�app����������
int ns__RechargeResult(struct soap * rSoap, std::string param0, std::string param1, std::string param2, std::string &param3)
{
	std::cout<<param0<<std::endl;
	std::cout<<param1<<std::endl;
	std::cout<<param2<<std::endl;
	std::cout<<param3<<std::endl;
	

	//���ص�֧�����
	if (param0 == "charge.succeeded" && g_InitParamter.m_szRechargeKey == param1) //�Ƚ�app_key������У��
	{
		//֧���ɹ�
		//����ɹ�������Ϣ���������ݿ�
		CMD_GP_UserMallBuyReq * req = new CMD_GP_UserMallBuyReq;
		strcpy(req->dwOrderID, param2.c_str());
		::PostMessage(::AfxGetMainWnd()->m_hWnd, WM_SOAP_MESSAGE, WPARAM(req->S_ID), (LPARAM)req);

		param3 = "Recharge ok!";
		//֧���ɹ���־
		char buf[200];
		memset(buf, 0, 200);
		sprintf(buf, "�յ�php soap���󣬳�ֵУ����ȷ��RechargeResult��ֵ���=%s��APPKEY=%s��������=%s", param0.c_str(), param1.c_str(), param2.c_str());
		TraceRechargeMessage(buf);
	}
	else
	{
		//֧��ʧ��
		param3 = "Recharge failed!";
		//֧��ʧ����־
		char buf[200];
		memset(buf, 0, 200);
		sprintf(buf, "�յ�php soap���󣬳�ֵУ��ʧ�ܣ�RechargeResult��ֵ���=%s��APPKEY=%s��������=%s", param0.c_str(), param1.c_str(), param2.c_str());
		TraceRechargeMessage(buf);
	}

	return 0;
}

int ns__UpdateResult(struct soap * uSoap, std::string param0, std::string param1, std::string param2, std::string &param3)
{
	std::cout<<param0<<std::endl;
	std::cout<<param1<<std::endl;
	std::cout<<param2<<std::endl;
	std::cout<<param3<<std::endl;

	//���ص�֧�����
	if (param0 == "charge.succeeded" && g_InitParamter.m_szRechargeKey == param1) //�Ƚ�app_key������У��
	{
		//֧���ɹ�
		CMD_GP_UserMallUpdateReq * req = new CMD_GP_UserMallUpdateReq;
		strcpy(req->dwOrderID, param2.c_str());
		::PostMessage(::AfxGetMainWnd()->m_hWnd, WM_SOAP_MESSAGE, WPARAM(req->S_ID), (LPARAM)req);

		param3 = "Update ok!";
		//֧���ɹ���־
		char buf[200];
		memset(buf, 0, 200);
		sprintf(buf, "�յ�php soap���󣬸���У����ȷ��UpdateResult���½��=%s��APPKEY=%s��������=%s", param0.c_str(), param1.c_str(), param2.c_str());
		TraceRechargeMessage(buf);
	}
	else
	{
		//֧��ʧ��
		param3 = "Update failed!";
		//֧��ʧ����־
		char buf[200];
		memset(buf, 0, 200);
		sprintf(buf, "�յ�php soap���󣬸���У��ʧ�ܣ�UpdateResult���½��=%s��APPKEY=%s��������=%s", param0.c_str(), param1.c_str(), param2.c_str());
		TraceRechargeMessage(buf);
	}

	return 0;
}

