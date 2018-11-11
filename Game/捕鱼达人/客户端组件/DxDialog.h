/////////////////////////////////////////
//�����dx���ڹ�����
////////////////////////////////////////

#ifndef DX_DIALOG_HEAD_FILE
#define DX_DIALOG_HEAD_FILE
#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "Stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "DxControl.h"
#include "resource.h"

////////////////////////////////////////////////////////////////////////////////

#define WM_DX_RENDER					WM_APP+100			//��Ⱦ��Ϣ

struct CUSTOMVERTEX
{
	float x,y,z;
};
////////////////////////////////////////////////////////////////////////////////

//dx���ڹ�����
class CDxDialog : public CWnd
{
	//״̬����
public:
	bool					m_bD3DSucess;			//�ɹ���־

	//��������
private:
	HANDLE 					m_hDxThread;			//�߳̾��
	
	//Ӧ�ñ���
private:
	D3DDISPLAYMODE			g_d3ddm;				//��ʾģʽ
	D3DPRESENT_PARAMETERS	g_d3dpp;				//��ʾ����

	//�豸����
public:
	HWND					g_hWnd;					//d3d���ھ��
	LPDIRECT3D9				g_pD3D;					//d3d����
	LPDIRECT3DDEVICE9		g_pD3DDevice;			//d3d�豸����

	//�ؼ�����
public:
	DWORD					g_Time;					//ȫ��ʱ��
	//CDxControl				g_UserControl;			//�û�����

	//��������
public:
	//���캯��
	CDxDialog();
	//��������
	virtual ~CDxDialog();

public:
	//��������
	virtual bool InitDxWindow();
	//���´���
	bool UpdateDxWindowSize();

	//�̳к���
protected:
	//��������
	virtual void UpdateControl();
	//��Ⱦ����
	virtual void OnRender();
	//�滭2d
	virtual void DrawGameView2D()=NULL;
	//D X��ʧ
	virtual void OnDeviceLost()=NULL;
	//��ʼ������
	virtual void InitScene(INT nWidth,INT nHeight)=NULL;
	//�豸����
    virtual void ReleaseScene()=NULL;
	//ֹͣ��Ⱦ
	virtual void StopRender();
	//��ʼ��Ⱦ
	virtual void StartRender();

	//�ڲ�����
private:
	//��Ⱦ��Ϣ
	LRESULT OnFrameRender(WPARAM wParam, LPARAM lParam) ;

	DECLARE_MESSAGE_MAP()
};
////////////////////////////////////////////////////////////////////////////////
#endif
