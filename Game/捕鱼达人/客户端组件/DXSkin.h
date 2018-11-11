//////////////////////////////////////
//ͼ����
/////////////////////////////////////////////////////////////////////////////

#ifndef SKIN_SPRITE_HEAD_FILE
#define SKIN_SPRITE_HEAD_FILE
#pragma once

#include "Stdafx.h"
#include <d3d9.h> 
#include <d3dx9.h>
#include <mmsystem.h>
#pragma comment (lib,"winmm.lib")
#pragma comment (lib,"d3d9.lib")
#pragma comment (lib,"d3dx9.lib")

#define MAX_TEXTURE_COUNT			10							//��������
////////////////////////////////////////////////////////////////////////////////
//X ���з�ʽ
enum enXCollocateMode 
{ 
	enXLeft,						//�����
	enXCenter,						//�ж���
	enXRight,						//�Ҷ���
};

//Y ���з�ʽ
enum enYCollocateMode 
{ 
	enYTop,							//�϶���
	enYCenter,						//�ж���
	enYBottom,						//�¶���
};

class CGameClientDlg;

//extern CGameClientDlg	* g_GameClientDlg;

//dxͼ����
class CDxSkin
{
	//���б���
public:	
	IDirect3DTexture9*				m_pTexture[MAX_TEXTURE_COUNT];		//����
	D3DXIMAGE_INFO					m_ImageInfo[MAX_TEXTURE_COUNT];		//ͼƬ��Ϣ
	BYTE							m_cbAlphaValue[MAX_TEXTURE_COUNT];	//͸���̶�
	CSize							m_csImageSize[MAX_TEXTURE_COUNT];	//ͼƬ��С
	CPoint							m_ptTarger[MAX_TEXTURE_COUNT];		//Ŀ��λ��
	bool							m_bFistLoad;						//�״μ���

	//��̬����
public:
	static WORD						m_wSpriteRefCount;					//������Ŀ
	static ID3DXSprite*				m_Sprite;							//DX����

	//��������
public:
	//���캯��
	CDxSkin();
	//��������
	virtual ~CDxSkin();

	//���ܺ���
public:
	//ͼƬ��Ϣ
	void SetImageInfo(UINT uResourceID,WORD wImageIndex=0);
	//���غ���
	void SetLoadInfo(LPDIRECT3DDEVICE9	pD3DDevice,LPCTSTR szFileName,WORD wImageIndex=0,D3DPOOL Pool=D3DPOOL_DEFAULT,D3DCOLOR dwKeyColor=D3DCOLOR_XRGB(255, 0, 255));
	//���غ���
	void SetLoadInfo(LPDIRECT3DDEVICE9	pD3DDevice,UINT uResourceID,WORD wImageIndex=0,D3DPOOL Pool=D3DPOOL_DEFAULT,D3DCOLOR dwKeyColor=D3DCOLOR_XRGB(255, 0, 255));
	//����͸��
	void SetAlphaBlend(BYTE bAlphaValue,WORD wImageIndex=0);
	//����λ��
	void SetDrawPos(INT nXpost,INT nYpost,WORD wImageIndex=0);
	//���ô�С
	void SetImageSize(INT nCx,INT nCy,WORD wImageIndex=0);
	//�滭����
	void DrawSprite(INT x,INT y,CRect &drawRect,WORD wImageIndex=0,D3DCOLOR channelColor=0x00ffffff);
	//�滭����
	void DrawSprite(INT x,INT y,D3DXVECTOR3 &vCenterPoint, CRect &drawRect,WORD wImageIndex=0,D3DXMATRIX *matRot=NULL,D3DCOLOR channelColor=0x00ffffff);
	//��ת����
	bool DrawRotateAndZoom(INT x,INT y,CRect &drawRect,float fCurrentAngle,float fXScaling=1.0f,float fYScaling=1.0f,WORD wImageIndex=0,enXCollocateMode XCollocateMode=enXCenter, enYCollocateMode YCollocateMode=enYCenter);
	//��ת����
	bool DrawRotateAndZoom(INT x,INT y,CRect &drawRect,float fCurrentAngle,int nRoteCenterX,int nRoteCenterY,float fXScaling=1.0f,float fYScaling=1.0f,WORD wImageIndex=0);
	//��ת����
	bool DrawRotateAndZoom(INT x,INT y,CRect &drawRect,float fCurrentAngle,float fXScaling,float fYScaling,WORD wImageIndex,enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode,D3DCOLOR channelColor);
	//�ͷź���
	void Release();

	//�滭����
public:
	//�滭����
	void OnDrawImage(WORD wImageIndex=0);

	//���ز���
public:
	//���ֿ��
	INT GetWidth(WORD wImageIndex=0){return m_csImageSize[wImageIndex].cx;}
	//���ָ߶�
	INT GetHeight(WORD wImageIndex=0){return m_csImageSize[wImageIndex].cy;}
	//ͼƬ���
	INT GetImageWidth(WORD wImageIndex=0){return m_ImageInfo[wImageIndex].Width;}
	//ͼƬ�߶�
	INT GetImageHeight(WORD wImageIndex=0){return m_ImageInfo[wImageIndex].Height;}
	//�����ж�
	bool IsLoaded(WORD wImageIndex=0){return (m_Sprite!=NULL&&m_pTexture[wImageIndex]!=NULL);}
	//��ת��־
	CPoint GetDrawPoint(WORD wImageIndex=0){return m_ptTarger[wImageIndex];}
	//�豸��ʧ
	HRESULT OnLostDevice() {return (NULL!=m_Sprite)?m_Sprite->OnLostDevice():D3DERR_NOTAVAILABLE;}
	//�����豸
	HRESULT OnResetDevice(){return (NULL!=m_Sprite)?m_Sprite->OnResetDevice():D3DERR_NOTAVAILABLE;}
};

////////////////////////////////////////////////////////////////////////////////
#endif