// DDZAndroidGame.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "DDZAndroidGame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//	ע�⣡
//
//		����� DLL ��̬���ӵ� MFC
//		DLL���Ӵ� DLL ������
//		���� MFC ���κκ����ں�������ǰ��
//		��������� AFX_MANAGE_STATE �ꡣ
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CDDZAndroidGameApp

BEGIN_MESSAGE_MAP(CDDZAndroidGameApp, CWinApp)
END_MESSAGE_MAP()


// CDDZAndroidGameApp ����

CDDZAndroidGameApp::CDDZAndroidGameApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDDZAndroidGameApp ����

CDDZAndroidGameApp theApp;


// CDDZAndroidGameApp ��ʼ��

BOOL CDDZAndroidGameApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
