#ifndef CONSTANT_HEAD_FILE
#define CONSTANT_HEAD_FILE

#ifndef _DEBUG

//////////////////////////////////////////////////////////////////////////
//��Ŀ��Ϣ

const TCHAR	szProductName[]=TEXT("�ر���");										//��Ʒ����
const TCHAR szSystemRegKey[]=TEXT("WHQPGame");								//ע������
const TCHAR szStationPage[]=TEXT("192.168.1.108");							//��Ϸ��ҳ
const TCHAR szCookieUrl[]=TEXT("http://192.168.1.108");							//��¼��ַ

#define MAIN_DLG_CLASSNAME				TEXT("WHQPGamePlaza")				//��������

//////////////////////////////////////////////////////////////////////////

#else

//////////////////////////////////////////////////////////////////////////
//������Ϣ

const TCHAR	szProductName[]=TEXT("�ر���");								//��Ʒ����
const TCHAR szSystemRegKey[]=TEXT("WHQPGame");								//ע������
const TCHAR szStationPage[]=TEXT("192.168.1.108");							//��Ϸ��ҳ
const TCHAR szCookieUrl[]=TEXT("http://192.168.1.108");							//��¼��ַ

#define MAIN_DLG_CLASSNAME				TEXT("WHQPGamePlaza")				//��������

#endif

//////////////////////////////////////////////////////////////////////////

#endif