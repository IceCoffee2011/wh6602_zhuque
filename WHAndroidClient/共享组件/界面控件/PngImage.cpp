#include "StdAfx.h"
#include "PngImage.h"

#include "ISonicUI.h"
using namespace sonic_ui;

//////////////////////////////////////////////////////////////////////////
#define  STATIC_COVERT(p)           (static_cast<ISonicImage*> (p))


//���캯��
CPngImage::CPngImage():m_pSrcImage(NULL)
{
}

//��������
CPngImage::~CPngImage()
{
	//����ͼƬ
	DestroyImage(); 

	return;
}

//�Ƿ����
bool CPngImage::IsNull()
{
	//�����ж�
	if (NULL!=m_pSrcImage && STATIC_COVERT(m_pSrcImage)->IsValid())
	{
		return false;
	}
	return true;
}

//��ȡ���
INT CPngImage::GetWidth()
{
	//�����ж�
	if (IsNull()==true) return 0;

	//��ȡ���
	return STATIC_COVERT(m_pSrcImage)->GetWidth();
}

//��ȡ�߶�
INT CPngImage::GetHeight()
{
	//�����ж�
	if (IsNull()==true) return 0;

	//��ȡ�߶�
	return STATIC_COVERT(m_pSrcImage)->GetHeight();
}

//����ͼƬ
bool CPngImage::DestroyImage()
{
	//ɾ������
	if (NULL != m_pSrcImage)
	{
		GetSonicUI()->DestroyObject((ISonicImage*)(m_pSrcImage));
	}
	m_pSrcImage = NULL;
	return true;
}

//����ͼƬ
bool CPngImage::LoadImage(LPCTSTR pszFileName)
{
	//�����ж�
	DestroyImage();

	m_pSrcImage = (ISonicImage*)GetSonicUI()->CreateImage();
	STATIC_COVERT(m_pSrcImage)->Load(pszFileName);
	if (NULL != m_pSrcImage)
	{
		return true;
	}
	return false;
}

//����ͼƬ
bool CPngImage::LoadImage(HINSTANCE hInstance, LPCTSTR pszResourceName)
{

	////�����ж�
	//DestroyImage();

	//m_pSrcImage = GetSonicUI()->CreateImage();
	//bool bRet = STATIC_COVERT(m_pSrcImage)->Load(MAKEINTRESOURCE(pszResourceName), hInstance, "PNG");
	//if (NULL != m_pSrcImage && bRet)
	//{
	//	return true;
	//}
	return false;
}

//�滭ͼ��
bool CPngImage::DrawImage(CDC * pDC, INT nXPos, INT nYPos)
{
	//�����ж�
	if (IsNull()==true) return false;

	STATIC_COVERT(m_pSrcImage)->Draw(pDC->m_hDC, nXPos, nYPos);

	INT nDestWidth=0, nDestHeight=0;
	nDestWidth = STATIC_COVERT(m_pSrcImage)->GetWidth();
	nDestHeight = STATIC_COVERT(m_pSrcImage)->GetHeight();

	DRAW_PARAM param;
	param.dwMask = DP_TILE|DP_SRC_CLIP|DP_DEST_LIMIT|DP_ALPHA;
	param.nTileLength = nDestWidth;
	param.nTileDivider = 1;
	param.cAlpha = 255;
	param.fScaleX = 1.0f;
	param.fScaleY = 1.0f;
	param.cx = nDestWidth;
	param.cy = nDestHeight;

	param.rtSrc.left = 0;
	param.rtSrc.top = 0;
	param.rtSrc.right = param.rtSrc.left + nDestWidth;
	param.rtSrc.bottom = param.rtSrc.top + nDestHeight;

	STATIC_COVERT(m_pSrcImage)->Draw(pDC->m_hDC, nXPos, nYPos, &param);

	return true;
}

//�滭ͼ��
bool CPngImage::DrawImage(CDC * pDC, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT YSrc)
{
	//�����ж�
	if (IsNull()==true) return false;

	DRAW_PARAM param;
	param.dwMask = DP_TILE|DP_SRC_CLIP|DP_DEST_LIMIT|DP_ALPHA;
	param.nTileLength = nDestWidth;
	param.nTileDivider = 1;
	param.cAlpha = 255;
	param.fScaleX = 1.0f;
	param.fScaleY = 1.0f;
	param.cx = nDestWidth;
	param.cy = nDestHeight;

	param.rtSrc.left = nXScr;
	param.rtSrc.top = YSrc;
	param.rtSrc.right = param.rtSrc.left + nDestWidth;
	param.rtSrc.bottom = param.rtSrc.top + nDestHeight;

	STATIC_COVERT(m_pSrcImage)->Draw(pDC->m_hDC, nXDest, nYDest, &param);
	return true;
}

//�滭ͼ��
bool CPngImage::DrawImage(CDC * pDC, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT YSrc, INT nSrcWidth, INT nSrcHeight)
{
	//�����ж�
	if (IsNull()==true) return false;

	DRAW_PARAM param;
	param.dwMask = DP_TILE|DP_SRC_CLIP|DP_DEST_LIMIT|DP_ALPHA;
	param.nTileLength = nDestWidth;
	param.nTileDivider = 1;
	param.cAlpha = 255;
	param.fScaleX = 1.0f;
	param.fScaleY = 1.0f;
	param.cx = nDestWidth;
	param.cy = nDestHeight;

	param.rtSrc.left = nXScr;
	param.rtSrc.top = YSrc;
	param.rtSrc.right = param.rtSrc.left + nSrcWidth;
	param.rtSrc.bottom = param.rtSrc.top + nSrcHeight;

	STATIC_COVERT(m_pSrcImage)->Draw(pDC->m_hDC, nXDest, nYDest, &param);
	return true;
}

//����͸����(0-100)
void CPngImage::SetImageAlpha(int iAlpha)
{
	//�����ж�
	if (IsNull()==true) return;

	if (iAlpha<0 || iAlpha>255) return;

	STATIC_COVERT(m_pSrcImage)->SetAlphaChannel(iAlpha);
}


//���͸��ֵget the ARGB of specified coordinates
DWORD CPngImage::GetAlphaValue(const int x,const int y)
{
	//�����ж�
	if (IsNull()==true) return 0;

	return STATIC_COVERT(m_pSrcImage)->GetPixel(x, y);
}

//////////////////////////////////////////////////////////////////////////
