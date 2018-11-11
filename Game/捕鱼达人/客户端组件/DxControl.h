//////////////////////////////////////////////////////////////////////
//�û������࣬����ֱ�ӻ�ȡ������Ϣ
//////////////////////////////////////////////////////////////////////
#ifndef DX_CONTROL_HEAD_FILE
#define DX_CONTROL_HEAD_FILE
#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <dinputd.h>
#include <d3dx9math.h>
#pragma comment (lib,"dinput8.lib")
#pragma comment (lib,"dxguid.lib")

//////////////////////////////////////////////////////////////////////

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

//////////////////////////////////////////////////////////////////////

//���������
class CDxControl
{
	//�豸����
private:
	LPDIRECTINPUT8			m_pDXInput;				//�����豸
	LPDIRECTINPUTDEVICE8	m_pDXInputKB;			//�����豸

	//״̬����
public:
	BYTE					m_cbKeyState[256];		//����״̬

	//��������
public:
	//���캯��
	CDxControl();
	//��������
	~CDxControl();

	//���ܺ���
public:
	//��ʼ������
	LRESULT InitDXInput(HINSTANCE hInst,HWND hWnd);
	//����״̬
	LRESULT UpdateInput();
	//�ͷ���Դ
	void Release();
};
//////////////////////////////////////////////////////////////////////

#endif