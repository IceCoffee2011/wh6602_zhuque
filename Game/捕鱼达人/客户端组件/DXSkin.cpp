
#include "StdAfx.h"
#include "DxSkin.h"

//////////////////////////////////////////////////////////////////////////////////////

//��������
#define LESS_MOVE_COUNT				10									//�ƶ�����


//��̬����
ID3DXSprite*				CDxSkin::m_Sprite=NULL;							//����
WORD						CDxSkin::m_wSpriteRefCount=0;					//������Ŀ
//////////////////////////////////////////////////////////////////////////////////////

#include "GameClientDlg.h"

//CGameClientDlg	* g_GameClientDlg=NULL;

//���캯��
CDxSkin::CDxSkin()
{
	//���б���
	m_Sprite=NULL;
	for(WORD i=0;i<MAX_TEXTURE_COUNT;i++)
	{
		m_pTexture[i]=NULL;
		m_cbAlphaValue[i]=0xff;
		m_csImageSize[i].SetSize(0,0);
		m_ptTarger[i].SetPoint(0,0);
	}
	ZeroMemory(&m_ImageInfo,sizeof(m_ImageInfo));

	//���ñ���
	m_bFistLoad=true;
}

//��������
CDxSkin::~CDxSkin()
{
	Release();
}

//�ͷź���
void CDxSkin::Release()
{
	//�ͷž���
	if (m_wSpriteRefCount>0) --m_wSpriteRefCount;
	if (0==m_wSpriteRefCount) SAFE_RELEASE(m_Sprite);

	//�ͷ�����
	for(WORD i=0;i<MAX_TEXTURE_COUNT;i++)
		SAFE_RELEASE(m_pTexture[i]);

	//���ñ���
	m_bFistLoad=true;
}

//����͸��
void CDxSkin::SetAlphaBlend(BYTE bAlphaValue,WORD wImageIndex)
{
	ASSERT(wImageIndex<MAX_TEXTURE_COUNT);
	if(wImageIndex>=MAX_TEXTURE_COUNT)return;

	m_cbAlphaValue[wImageIndex]=bAlphaValue;

	return;
}

//����λ��
void CDxSkin::SetDrawPos(INT nXpost,INT nYpost,WORD wImageIndex)
{	
	ASSERT(wImageIndex<MAX_TEXTURE_COUNT);
	if(wImageIndex>=MAX_TEXTURE_COUNT)return;

	m_ptTarger[wImageIndex].x=nXpost;
	m_ptTarger[wImageIndex].y=nYpost;

	return;
}

//���ô�С
void CDxSkin::SetImageSize(INT nCx,INT nCy,WORD wImageIndex)
{
	ASSERT(wImageIndex<MAX_TEXTURE_COUNT);
	if(wImageIndex>=MAX_TEXTURE_COUNT)return;

	m_csImageSize[wImageIndex].SetSize(nCx,nCy);

	return;
}

//ͼƬ��Ϣ
void CDxSkin::SetImageInfo(UINT uResourceID,WORD wImageIndex)
{
	ASSERT(wImageIndex<MAX_TEXTURE_COUNT);
	if(wImageIndex>=MAX_TEXTURE_COUNT)return;

	//��ȡ��Ϣ
	D3DXGetImageInfoFromResource(GetModuleHandle(NULL),MAKEINTRESOURCE(uResourceID),&m_ImageInfo[wImageIndex]);

	//�����С
	SetImageSize(m_ImageInfo[wImageIndex].Width,m_ImageInfo[wImageIndex].Height);

	return;
}

//���غ���(���ļ���)
void CDxSkin::SetLoadInfo(LPDIRECT3DDEVICE9	pD3DDevice,LPCTSTR szFileName,WORD wImageIndex,D3DPOOL Pool, D3DCOLOR dwKeyColor)
{
	ASSERT(wImageIndex<MAX_TEXTURE_COUNT);
	if(wImageIndex>=MAX_TEXTURE_COUNT)return;

//#ifdef _DEBUG
	//if (NULL!=g_GameClientDlg) g_GameClientDlg->InsertSystemString(szFileName);
//#endif

	//��������
	if (NULL==m_Sprite) D3DXCreateSprite( pD3DDevice, &m_Sprite);

	//�״��ж�
	if (true==m_bFistLoad)
	{
		m_bFistLoad=false;
		++m_wSpriteRefCount;
	}

	//��������
	HRESULT hr=D3DXCreateTextureFromFileEx(pD3DDevice,	szFileName, 0, 0, 0, 0,
		D3DFMT_UNKNOWN, Pool, D3DX_FILTER_NONE,D3DX_DEFAULT,	dwKeyColor, NULL, NULL,
 		&m_pTexture[wImageIndex]);

	//�ɹ��ж�
	if (D3D_OK!=hr && D3DPOOL_DEFAULT==Pool)
	{
		D3DXCreateTextureFromFileEx(pD3DDevice,	szFileName, 0, 0, 0, 0,
			D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE,D3DX_DEFAULT,	dwKeyColor, NULL, NULL,
		&m_pTexture[wImageIndex]);
	}

	if(m_Sprite==NULL||m_pTexture[wImageIndex]==NULL) throw 0 ;

	//��ȡ��Ϣ
	D3DXGetImageInfoFromFile(szFileName, &m_ImageInfo[wImageIndex]);

	//�����С
	SetImageSize(m_ImageInfo[wImageIndex].Width,m_ImageInfo[wImageIndex].Height,wImageIndex);

	return;
}

//���غ���(����ԴID)
void CDxSkin::SetLoadInfo(LPDIRECT3DDEVICE9	pD3DDevice,UINT uResourceID,WORD wImageIndex,D3DPOOL Pool,D3DCOLOR dwKeyColor)
{
	ASSERT(wImageIndex<MAX_TEXTURE_COUNT);
	if(wImageIndex>=MAX_TEXTURE_COUNT)return;

	//��������
	if (NULL==m_Sprite) D3DXCreateSprite( pD3DDevice, &m_Sprite);

	//�״��ж�
	if (true==m_bFistLoad)
	{
		m_bFistLoad=false;
		++m_wSpriteRefCount;
	}

	//��������
	HRESULT hr=D3DXCreateTextureFromResourceEx(pD3DDevice,	NULL,MAKEINTRESOURCE(uResourceID), 0, 0, 0, 0,
		D3DFMT_UNKNOWN, Pool, D3DX_FILTER_NONE,D3DX_DEFAULT,	dwKeyColor, NULL, NULL,
		&m_pTexture[wImageIndex]);
	
	//�ɹ��ж�
	if (D3D_OK!=hr && D3DPOOL_DEFAULT==Pool)
	{
		D3DXCreateTextureFromResourceEx(pD3DDevice,	NULL,MAKEINTRESOURCE(uResourceID), 0, 0, 0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE,D3DX_DEFAULT,	dwKeyColor, NULL, NULL,
		&m_pTexture[wImageIndex]);
	}

	if(m_Sprite==NULL||m_pTexture[wImageIndex]==NULL) throw 0 ;

	//�����С
	SetImageSize(m_ImageInfo[wImageIndex].Width,m_ImageInfo[wImageIndex].Height);
}

//�ƶ�����
void CDxSkin::OnDrawImage(WORD wImageIndex)
{
	ASSERT(wImageIndex<MAX_TEXTURE_COUNT);
	if(wImageIndex>=MAX_TEXTURE_COUNT)return;

	if(m_Sprite==NULL||m_pTexture[wImageIndex]==NULL) return;

	//λ����Ϣ
	INT x=m_ptTarger[wImageIndex].x;
	INT y=m_ptTarger[wImageIndex].y;
	CRect drawRect(0,0,m_csImageSize[wImageIndex].cx,m_csImageSize[wImageIndex].cy);

	//���ò���
	D3DXMATRIX matRot;D3DXMatrixIdentity(&matRot);//����

	//���Ĳ�����ͼƬ�Ļ���λ��
	float fCenterX=float(drawRect.right-drawRect.left)/2;
	float fCenterY=float(drawRect.bottom-drawRect.top)/2;
	D3DXMatrixTransformation2D(	&matRot,0,0,0,&D3DXVECTOR2(fCenterX,fCenterY),0.0f,&D3DXVECTOR2((float)x,(float)y));

	//�滭ͼƬ
	DrawSprite(0,0,D3DXVECTOR3(0,0,0),drawRect,wImageIndex,&matRot);

	return ;
}

//�滭����
void CDxSkin::DrawSprite(INT x,INT y,CRect &drawRect,WORD wImageIndex,D3DCOLOR channelColor)
{
	//�Ϸ��ж�
	if (m_Sprite==NULL||m_pTexture==NULL) return ;
	ASSERT(wImageIndex<MAX_TEXTURE_COUNT);
	if(wImageIndex>=MAX_TEXTURE_COUNT)return;

	////״̬���
	//LPDIRECT3DDEVICE9 pD3DDevice=NULL;
	//m_Sprite->GetDevice(&pD3DDevice);
	//if (NULL!=pD3DDevice)
	//{
	//	HRESULT hResult=pD3DDevice->TestCooperativeLevel();
	//	if (hResult==D3DERR_DEVICELOST || hResult==D3DERR_DEVICENOTRESET) return ;
	//}

	//����͸��
	D3DCOLOR color = (m_cbAlphaValue[wImageIndex]&255)<<24|channelColor;

	//���ò���
	D3DXMATRIX matRot;D3DXMatrixIdentity(&matRot);//����

	//�滭��ǰ֡
	D3DXVECTOR3 drawAtPoint((float)x,(float)y,0);
 	//m_Sprite->Begin(D3DXSPRITE_ALPHABLEND|D3DXSPRITE_SORT_TEXTURE);
	m_Sprite->SetTransform(&matRot);
	m_Sprite->Draw(m_pTexture[wImageIndex], drawRect, NULL, &drawAtPoint, color);
	m_Sprite->Flush();
	//m_Sprite->End();

	return ;
}

//�滭����
void CDxSkin::DrawSprite(INT x,INT y,D3DXVECTOR3 &vCenterPoint,CRect &drawRect,WORD wImageIndex,D3DXMATRIX *matRot,D3DCOLOR channelColor)
{
	//�Ϸ��ж�
	if (m_Sprite==NULL||m_pTexture==NULL) return ;
	ASSERT(wImageIndex<MAX_TEXTURE_COUNT);
	if(wImageIndex>=MAX_TEXTURE_COUNT)return;

	////״̬���
	//LPDIRECT3DDEVICE9 pD3DDevice=NULL;
	//m_Sprite->GetDevice(&pD3DDevice);
	//if (NULL!=pD3DDevice)
	//{
	//	HRESULT hResult=pD3DDevice->TestCooperativeLevel();
	//	if (hResult==D3DERR_DEVICELOST || hResult==D3DERR_DEVICENOTRESET) return ;
	//}

	//����͸��
	D3DCOLOR color = (m_cbAlphaValue[wImageIndex]&255)<<24|channelColor;

	//�滭λ��
	D3DXVECTOR3 drawAtPoint((float)x,(float)y,0);
	//m_Sprite->Begin(D3DXSPRITE_ALPHABLEND|D3DXSPRITE_SORT_TEXTURE);
	m_Sprite->SetTransform(matRot);
	m_Sprite->Draw(m_pTexture[wImageIndex], drawRect, &vCenterPoint, &drawAtPoint, color);
	m_Sprite->Flush();
	//m_Sprite->End();

	return ;	
}

//��ת����
bool CDxSkin::DrawRotateAndZoom(INT x,INT y,CRect &drawRect,float fCurrentAngle,float fXScaling,float fYScaling,WORD wImageIndex,enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ò���
	D3DXMATRIX matRot;D3DXMatrixIdentity(&matRot);//����

	//���Ĳ�����ͼƬ�Ļ���λ��
	float fCenterX=float(drawRect.right-drawRect.left)/2;
	float fCenterY=float(drawRect.bottom-drawRect.top)/2;
	D3DXMatrixTransformation2D(&matRot,&D3DXVECTOR2(fCenterX,fCenterY),0,&D3DXVECTOR2(fXScaling,fYScaling),&D3DXVECTOR2(0,0),fCurrentAngle,&D3DXVECTOR2((float)x,(float)y));

	//�滭ͼƬ
	float fXCollocate=0;
	float fYCollocate=0;
	if (XCollocateMode==enXLeft) fXCollocate=0.f;
	else if (XCollocateMode==enXRight) fXCollocate=float(drawRect.Width());
	else fXCollocate=fCenterX;

	if (YCollocateMode==enYTop) fYCollocate=0.f;
	else if (YCollocateMode==enYBottom) fYCollocate=float(drawRect.Height());
	else fYCollocate=fCenterY;

	DrawSprite(0,0,D3DXVECTOR3(fXCollocate,fYCollocate,0),drawRect,wImageIndex,&matRot);

	return true;
}

//��ת����
bool CDxSkin::DrawRotateAndZoom(INT x,INT y,CRect &drawRect,float fCurrentAngle,int nRoteCenterX,int nRoteCenterY,float fXScaling,float fYScaling,WORD wImageIndex)
{
	//���ò���
	D3DXMATRIX matRot;D3DXMatrixIdentity(&matRot);//����

	//���Ĳ�����ͼƬ�Ļ���λ��
	float fCenterX=float(drawRect.right-drawRect.left)/2;
	float fCenterY=float(drawRect.bottom-drawRect.top)/2;
	D3DXMatrixTransformation2D(&matRot,&D3DXVECTOR2(fCenterX,fCenterY),0,&D3DXVECTOR2(fXScaling,fYScaling),&D3DXVECTOR2(0,0),fCurrentAngle,&D3DXVECTOR2((float)x,(float)y));

	//�滭ͼƬ
	DrawSprite(0,0,D3DXVECTOR3(float(nRoteCenterX),float(nRoteCenterY),0),drawRect,wImageIndex,&matRot);

	return true;
}

//��ת����
bool CDxSkin::DrawRotateAndZoom(INT x,INT y,CRect &drawRect,float fCurrentAngle,float fXScaling,float fYScaling,WORD wImageIndex,enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode,D3DCOLOR channelColor)
{
	//���ò���
	D3DXMATRIX matRot;D3DXMatrixIdentity(&matRot);//����

	//���Ĳ�����ͼƬ�Ļ���λ��
	float fCenterX=float(drawRect.right-drawRect.left)/2;
	float fCenterY=float(drawRect.bottom-drawRect.top)/2;
	D3DXMatrixTransformation2D(&matRot,&D3DXVECTOR2(fCenterX,fCenterY),0,&D3DXVECTOR2(fXScaling,fYScaling),&D3DXVECTOR2(0,0),fCurrentAngle,&D3DXVECTOR2((float)x,(float)y));

	//�滭ͼƬ
	float fXCollocate=0;
	float fYCollocate=0;
	if (XCollocateMode==enXLeft) fXCollocate=0.f;
	else if (XCollocateMode==enXRight) fXCollocate=float(drawRect.Width());
	else fXCollocate=fCenterX;

	if (YCollocateMode==enYTop) fYCollocate=0.f;
	else if (YCollocateMode==enYBottom) fYCollocate=float(drawRect.Height());
	else fYCollocate=fCenterY;

	DrawSprite(0,0,D3DXVECTOR3(fXCollocate,fYCollocate,0),drawRect,wImageIndex,&matRot,channelColor);

	return true;
}
//HRESULT Draw(
//����LPDIRECT3DTEXTURE9 pTexture,
//����CONST RECT *pSrcRect,
//����CONST D3DXVECTOR3 *pCenter,
//����CONST D3DXVECTOR3 *pPosition,
//����D3DCOLOR Color );
//
//pTexture ����Ҫ���Ƶ���ͼ
//pSrcRect ����Ҫ���Ƶ���ͼ�ϵ�һ����������, ����������ͼ����ָ��ΪNULL.
//pCenter �ǻ��Ƶ���������(��תʱ���Դ˵�Ϊ����), ָ��NULL��ʾ��������Ϊ(0,0,0)
//pPosition �ǻ��Ƶ�λ������, Ҳ����ָ��NULL��ʾ(0,0,0)
//Color �ǻ��Ƶ���ɫ, һ�������ָ��Ϊ 0xffffffff. ���λ����λ�ĸ�8�ֽ�ΪAlpha, ��, ��, ��, ���ָ�� 0x80ffffff���ǰ�͸����ͼ. 
//���0xffff0000��ֻ������ͼ��ĺ�ɫ����, ���幦���Լ����. ��Ȼ��ͼ������԰���Alpha��Ϣ.
//��Ҫ��ת�ȹ��ܿ���ʹ�� pSprite->SetTransform(), ����ԭ��Ϊ:
//HRESULT SetTransform( CONST D3DXMATRIX *pTransform );

//////////////////////////////////////////////////////////////////////////////////////

//����Ϊʹ��ʾ��  ƽ�����̵���ת
////ƽ������
//D3DXMATRIX matRot;
//D3DXMatrixIdentity(&matRot);
//float   iAngle=0;
//if((INT)m_Ball_MoveRadius!=WIN_RADIUS&&m_bWinNumber==255) //��ת����
//{
//	iAngle=1.5f*timeGetTime()/500.0f;   
//}
//else if(m_bWinNumber!=255)	//ת��ʤ������
//{
//	for(INT i=0;i<37;i++) if(m_bWinNumber==numberArrayIndex[i]) iAngle=i*(2*D3DX_PI/37);;
//}
//D3DXMatrixTransformation2D(	&matRot,0,0,0,&D3DXVECTOR2((float)m_PlatePlane.GetWidth()/2,(float)m_PlatePlane.GetHeight()/2),iAngle,&D3DXVECTOR2((float)m_nScreenWidth/2+309-m_PlatePlane.GetWidth()/2,(float)m_nScreenHeight/2-68-m_PlatePlane.GetHeight()/2));
//m_PlatePlane.DrawSprite(0,0,D3DXVECTOR3(0,0,0), CRect(0,0,584,584),&matRot);
//


