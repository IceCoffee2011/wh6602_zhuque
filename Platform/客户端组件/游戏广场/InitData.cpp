#include "StdAfx.h"
#include "initdata.h"

char  CInitData::s_cLoginServer[128];						//��¼��������ַ
char  CInitData::s_cLoadServer[128];						//��¼��������ַ
char  CInitData::s_cHomePage[128];							//�ٷ���վ
char  CInitData::s_cGameHelp[128];							//��Ϸ����
char  CInitData::s_cClientCenter[128];						//�ͷ�����
char  CInitData::s_cGetPassword[128];						//�һ�����
char  CInitData::s_cErrorTickling[128];						//���ⷴ��
char  CInitData::s_cAccountsManage[128];					//�˺Ź���
char  CInitData::s_cLogonAdverTisement[128];				//��¼���
char  CInitData::s_cLogonNotice[128];						//��¼����
char  CInitData::s_cContactService[256];					//���߿ͷ�
char  CInitData::s_cConfirmGetGold[128];					//��ȡ���
char  CInitData::s_cLogonRegister[128];						//ע���ʺ�
char  CInitData::s_cLoginHomePage[128];						//�ٷ���վ(��¼)
char  CInitData::s_cLoginAccountsManage[128];				//�˺Ź���(��¼)
char  CInitData::s_cLoginPayment[128];						//�ʺų�ֵ
char  CInitData::s_cLoginExchange[128];						//��Ʒ�һ�
char  CInitData::s_cLoginPropShop[128];						//�����̳�
char  CInitData::s_cLoginPropShopOpen[128];					//�����̳�(����ʽ)
char  CInitData::s_cLoginTaskNew[128];						//��������
char  CInitData::s_cLoginUserInfo[128];						//�����Ϣ
char  CInitData::s_cLoginSysMessage[128];					//ϵͳ�ʼ�
char  CInitData::s_cLoginBank[128];							//����
char  CInitData::s_cLoginBackpack[128];						//����
char  CInitData::s_cGameHelpSingle[128];					//������Ϸ����
char  CInitData::s_cXBMessage[128];							//ϲ����Ϣ(��Ϸ�б�������ҳ)
char  CInitData::s_cNotice[128];							//�����
char  CInitData::s_cAdverTisement[128];						//�ײ����
char  CInitData::s_cTitleNotice[128];						//ͷ�����

CInitData::CInitData(void)
{
}

CInitData::~CInitData(void)
{
}

//��ʼ��
void CInitData::InitData()
{
	CString	sFileName("");
	CString sFilePath("");
	GetModuleFileName(NULL, sFileName.GetBufferSetLength(128), 128);
	sFilePath=sFileName.Left(sFileName.ReverseFind('\\') + 1) + TEXT("config\\config.ini");
	
	GetPrivateProfileString(TEXT("ServersAddr"), TEXT("LoginServer"), TEXT(""), s_cLoginServer, 128, sFilePath);
	GetPrivateProfileString(TEXT("ServersAddr"), TEXT("LoadServer"), TEXT(""), s_cLoadServer, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("HomePage"), TEXT(""), s_cHomePage, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("GameHelp"), TEXT(""), s_cGameHelp, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("ClientCenter"), TEXT(""), s_cClientCenter, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("GetPassword"), TEXT(""), s_cGetPassword, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("ErrorTickling"), TEXT(""), s_cErrorTickling, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("AccountsManage"), TEXT(""), s_cAccountsManage, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("LogonAdverTisement"), TEXT(""), s_cLogonAdverTisement, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("LogonNotice"), TEXT(""), s_cLogonNotice, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("ContactService"), TEXT(""), s_cContactService, 256, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("ConfirmGetGold"), TEXT(""), s_cConfirmGetGold, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("LogonRegister"), TEXT(""), s_cLogonRegister, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("LoginHomePage"), TEXT(""), s_cLoginHomePage, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("LoginAccountsManage"), TEXT(""), s_cLoginAccountsManage, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("LoginPayment"), TEXT(""), s_cLoginPayment, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("LoginExchange"), TEXT(""), s_cLoginExchange, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("LoginPropShop"), TEXT(""), s_cLoginPropShop, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("LoginPropShopOpen"), TEXT(""), s_cLoginPropShopOpen, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("LoginTaskNew"), TEXT(""), s_cLoginTaskNew, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("LoginUserInfo"), TEXT(""), s_cLoginUserInfo, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("LoginSysMessage"), TEXT(""), s_cLoginSysMessage, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("LoginBank"), TEXT(""), s_cLoginBank, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("LoginBackpack"), TEXT(""), s_cLoginBackpack, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("GameHelpSingle"), TEXT(""), s_cGameHelpSingle, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("XBMessage"), TEXT(""), s_cXBMessage, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("Notice"), TEXT(""), s_cNotice, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("AdverTisement"), TEXT(""), s_cAdverTisement, 128, sFilePath);
	GetPrivateProfileString(TEXT("WebBrowser"), TEXT("TitleNotice"), TEXT(""), s_cTitleNotice, 128, sFilePath);
}
//�ͷ���Դ
void CInitData::FreeData()
{
}
