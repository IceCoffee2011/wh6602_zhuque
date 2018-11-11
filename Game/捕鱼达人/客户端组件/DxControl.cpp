#include "StdAfx.h"
#include "DxControl.h"

//////////////////////////////////////////////////////////////////

//���캯��
CDxControl::CDxControl()
{
	m_pDXInputKB=NULL;
	m_pDXInput=NULL;
}

//��������
CDxControl::~CDxControl()
{

}

//��ʼ������
LRESULT CDxControl::InitDXInput(HINSTANCE hInst,HWND hWnd)
{
	//����DXInput����
	HRESULT hr;
	hr=DirectInput8Create(hInst,DIRECTINPUT_VERSION,IID_IDirectInput8,(LPVOID*)&m_pDXInput,NULL);
	if FAILED(hr)
	{
		MessageBox(hWnd,TEXT("����DXInput����ʧ��"),TEXT("ERROR"),MB_OK);
		return FALSE;
	}

	//���������豸
	hr=m_pDXInput->CreateDevice(GUID_SysKeyboard,&m_pDXInputKB,NULL);
	if FAILED(hr)
	{
		MessageBox(hWnd,TEXT("���������豸ʧ��"),TEXT("ERROR"),MB_OK);
		return FALSE;
	}

	// �趨Ϊͨ��һ�� 256 �ֽڵ����鷵�ز�ѯ״ֵ̬
	hr=m_pDXInputKB->SetDataFormat(&c_dfDIKeyboard);
	if FAILED(hr)
	{
		MessageBox(hWnd,TEXT("���ü������ݸ�ʽʧ��"),TEXT("ERROR"),MB_OK);
		return FALSE;
	}

	// �趨Э��ģʽ
	//hr=m_pDXInputKB->SetCooperativeLevel(hWnd,DISCL_EXCLUSIVE|DISCL_NOWINKEY);
	//if FAILED(hr)
	//{
	//	MessageBox(hWnd,TEXT("�趨Э��ģʽʧ��"),TEXT("ERROR"),MB_OK);
	//	return FALSE;
	//}

	m_pDXInputKB->Acquire();

	return TRUE;
}

//��������
LRESULT CDxControl::UpdateInput()
{
	if(m_pDXInputKB != NULL)      // ��� m_pDXInputKB ����������
	{
		HRESULT hr;

		hr = DIERR_INPUTLOST;   // Ϊѭ�������׼��

		// if input is lost then acquire and keep trying
		while(hr == DIERR_INPUTLOST)
		{
			// ��ȡ�����豸״ֵ̬��״̬���ݻ�����
			hr = m_pDXInputKB->GetDeviceState(sizeof(m_cbKeyState), &m_cbKeyState);

			// DirectInput �������������ж�
			if(hr == DIERR_INPUTLOST)
			{
				// ���������µ��� Acquire ������Ȼ������һ��
				hr = m_pDXInputKB->Acquire();
				if(FAILED(hr))
					return hr;
			}
		}

		if(FAILED(hr))
			return hr;
	}

	//if(m_pDXInputKB==NULL) return DI_NOTATTACHED;

	////��ȡ����
	//if(DI_OK!=m_pDXInputKB->GetDeviceState(sizeof(m_cbKeyState),m_cbKeyState))
	//{	
	//	memset(&m_cbKeyState,0,sizeof(m_cbKeyState));	
	//	m_pDXInputKB->Acquire();
	//}

	return DI_OK;
}

//�ͷ��豸
void CDxControl::Release()
{
	SAFE_RELEASE(m_pDXInputKB);
	SAFE_RELEASE(m_pDXInput);

	return;
}
//////////////////////////////////////////////////////////////////
