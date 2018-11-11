#include "stdafx.h"
#include "DxDialog.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDxDialog, CWnd)
	ON_MESSAGE(WM_DX_RENDER, OnFrameRender)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//static bool		m_bD3DSucess;						//DX��ʼ���ɹ���־

//////////////////////////////////////////////////////////////////////////

//���캯��
CDxDialog::CDxDialog()
{
	m_bD3DSucess=false;
	g_Time = 0;
}

//��������
CDxDialog::~CDxDialog()
{
	SAFE_RELEASE(g_pD3DDevice);
	SAFE_RELEASE(g_pD3D);
}

//��Ⱦ�߳�
void DxThreadRender(LPVOID pThreadData)
{
	//Ч�����
	ASSERT(pThreadData!=NULL);
	if(pThreadData==NULL) return;

	//����ת��
	CDxDialog *pDxDialog=(CDxDialog *)pThreadData;

	//����cpuʹ����
	while(NULL!=pDxDialog && pDxDialog->GetSafeHwnd()!=NULL && pDxDialog->m_bD3DSucess)
	{
		SendMessage(pDxDialog->GetSafeHwnd(), WM_DX_RENDER, 0, 0) ;
		Sleep(10);
	}

	return;
}

//��Ⱦ��Ϣ
LRESULT CDxDialog::OnFrameRender(WPARAM wParam, LPARAM lParam)
{
	if(g_pD3DDevice==NULL) return 0;
	HRESULT hResult=g_pD3DDevice->TestCooperativeLevel();
	if (hResult==D3DERR_DEVICELOST) 
	{
		OnDeviceLost();
		return 0;
	}
	if (hResult==D3DERR_DEVICENOTRESET)
	{
		OnDeviceLost();
		UpdateDxWindowSize();
		InitScene(g_d3dpp.BackBufferWidth,g_d3dpp.BackBufferHeight);
		return 0;
	}

	OnRender() ;

	return 0 ;
}

//dx���ڳ�ʼ��
bool CDxDialog::InitDxWindow()
{
	////����ʱ��
	//DWORD dwStart = GetTickCount();

	//�����ж�
	if(m_bD3DSucess==true) return true;

	//���ʵ��
	g_hWnd=this->GetSafeHwnd();
	if(g_hWnd==NULL) return false;

	//��ʼ��d3d
	try{
		//���ؽ��
		HRESULT hr;

		//ȡ��DX�汾,��ʾģʽ
		g_pD3D=Direct3DCreate9(D3D_SDK_VERSION);
		hr=g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &g_d3ddm);
		if(FAILED(hr)) 
		{
			MessageBox(TEXT("�����ʾ������ģʽʧ��!"),TEXT("������ʾ"),MB_OK);
			return false;
		}

		//������ʾ����
		ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
		g_d3dpp.Windowed=true;										//�Ƿ񴰿�ģʽ��ʾ
		g_d3dpp.BackBufferFormat=g_d3ddm.Format;					//�󻺳���ģʽ
		g_d3dpp.SwapEffect=D3DSWAPEFFECT_DISCARD;					//������������(ֱ�ӷ���)
		//g_d3dpp.BackBufferWidth=100;								//�󻺳��
		//g_d3dpp.BackBufferHeight=20;								//�󻺳��
		g_d3dpp.hDeviceWindow=g_hWnd;								//���ھ��
		g_d3dpp.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;	//��ʾ��ˢ����
		g_d3dpp.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;	//ͼ�����ˢ���ٶ�
		g_d3dpp.AutoDepthStencilFormat=D3DFMT_D16;					//��Ȼ����ʽ
		g_d3dpp.EnableAutoDepthStencil=TRUE;						//������Ȼ���

		//���ݼ��
		D3DCAPS9 d3dCaps;
		if (FAILED(g_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps))) return false;
		DWORD dwBehaviorFlags=0;
		if (d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) dwBehaviorFlags=D3DCREATE_HARDWARE_VERTEXPROCESSING;
		else dwBehaviorFlags=D3DCREATE_SOFTWARE_VERTEXPROCESSING;

		//�����豸
		hr=g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd,dwBehaviorFlags, &g_d3dpp, &g_pD3DDevice);
		if (FAILED(hr))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, g_hWnd, dwBehaviorFlags,&g_d3dpp, &g_pD3DDevice))) return false;
		}

		//g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

		//D3DCAPS9 d3dCaps;
		//if (FAILED(g_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dCaps))) return false;

		//DWORD dwBehaviorFlags = 0;
		//if (d3dCaps.VertexProcessingCaps!=0)
		//	dwBehaviorFlags|=D3DCREATE_HARDWARE_VERTEXPROCESSING;
		//else
		//	dwBehaviorFlags|=D3DCREATE_SOFTWARE_VERTEXPROCESSING;

		//ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
		//g_d3dpp.BackBufferFormat=g_d3ddm.Format;
		//g_d3dpp.SwapEffect=D3DSWAPEFFECT_DISCARD;
		//g_d3dpp.Windowed=TRUE; 
		//g_d3dpp.EnableAutoDepthStencil=TRUE;
		//g_d3dpp.AutoDepthStencilFormat=D3DFMT_D16; 
		//g_d3dpp.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;
		//if (FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd,dwBehaviorFlags, &g_d3dpp, &g_pD3DDevice)))
		//{
		//	return false;
		//}

		//������Ⱦ�߳�
		UINT uThreadID=0;
		m_bD3DSucess = true;
		m_hDxThread=(HANDLE)::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)DxThreadRender,this,0,0);		
	}catch(...){}

 	//��ʼ������
	//g_UserControl.InitDXInput(AfxGetInstanceHandle(),g_hWnd);

	/*DWORD dwStop = GetTickCount();
	TCHAR tch[32]=TEXT("");
	_sntprintf(tch,sizeof(tch),TEXT("%d"),dwStop-dwStart);*/

	return (g_hWnd!=NULL);
}

//���´���
bool CDxDialog::UpdateDxWindowSize()
{
	//�ͷ��豸
	//ReleaseScene();
	//g_UserControl.Release();
	StopRender();
	//SAFE_RELEASE(g_pD3DDevice);
	//SAFE_RELEASE(g_pD3D);

	//���ڴ�С�ı���Ҫ��������d3d�豸�����ʼ������
	try{
		//g_pD3D=Direct3DCreate9(D3D_SDK_VERSION);
		g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &g_d3ddm);

		//������ʾ����
		ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
		g_d3dpp.Windowed=true;										//�Ƿ񴰿�ģʽ��ʾ
		g_d3dpp.BackBufferFormat=g_d3ddm.Format;					//�󻺳���ģʽ
		g_d3dpp.SwapEffect=D3DSWAPEFFECT_DISCARD;					//������������(ֱ�ӷ���)
		//g_d3dpp.BackBufferWidth=nWidth;							//�󻺳��
		//g_d3dpp.BackBufferHeight=nHeight;							//�󻺳��
		g_d3dpp.hDeviceWindow=g_hWnd;								//���ھ��
		g_d3dpp.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;	//��ʾ��ˢ����
		g_d3dpp.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;	//ͼ�����ˢ���ٶ�
		g_d3dpp.AutoDepthStencilFormat=D3DFMT_D16;					//��Ȼ����ʽ
		g_d3dpp.EnableAutoDepthStencil=TRUE;						//������Ȼ���

		//��ԭ�豸
		g_pD3DDevice->Reset(&g_d3dpp);

		//�����豸
		//g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING, &g_d3dpp, &g_pD3DDevice);
		if(g_pD3DDevice==NULL) return false;
	}catch(...){}

	//��ʼ������
	//g_UserControl.InitDXInput(AfxGetInstanceHandle(),g_hWnd);

	//��ʼ��Ⱦ
	StartRender();

	return true;
}

//�û�����
VOID CDxDialog::UpdateControl()
{
	return;
}

//��Ⱦ���� �ͻ��˿�������onrender();������дDrawGameViewEx()
void CDxDialog::OnRender()
{	
	//Ч��״̬
	if(m_bD3DSucess==false) return;
	if(g_pD3DDevice==NULL) return;

	//��������
	if((GetTickCount()-g_Time)<10) return;
	g_Time=GetTickCount();

	//���¿���
	//UpdateControl();

	//����󱸻�����
	g_pD3DDevice->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(100,100,100),1.0f,0);

	//��ʼ���Ƴ���
	g_pD3DDevice->BeginScene();

	//g_Camera.Update2DMatrix();
	DrawGameView2D();

	//��������
	g_pD3DDevice->EndScene();

	//��ҳ
	g_pD3DDevice->Present(NULL,NULL,NULL,NULL);
}

//����ֹͣ��־
void CDxDialog::StopRender()
{
	m_bD3DSucess=false;
}

//��ʼ��Ⱦ
void CDxDialog::StartRender()
{
	m_bD3DSucess=true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

