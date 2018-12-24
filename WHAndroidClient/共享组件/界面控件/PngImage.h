#ifndef PNG_IMAGE_HEAD_FILE
#define PNG_IMAGE_HEAD_FILE

#pragma once

#include "SkinControls.h"

//////////////////////////////////////////////////////////////////////////

//ͼƬ����
class SKIN_CONTROL_CLASS CPngImage
{
	//��������
protected:
	void*       m_pSrcImage; 
	//��������
public:
	//���캯��
	CPngImage();
	//��������
	virtual ~CPngImage();

	//״̬����
public:
	//�Ƿ����
	bool IsNull();
	//��ȡ���
	INT GetWidth();
	//��ȡ�߶�
	INT GetHeight();

	//������
public:
	//����ͼƬ
	bool DestroyImage();
	//����ͼƬ
	bool LoadImage(LPCTSTR pszFileName);
	//����ͼƬ
	bool LoadImage(HINSTANCE hInstance, LPCTSTR pszResourceName);

public:
	//����͸����(0-100)
	void SetImageAlpha(int iAlpha=100);
	//���͸��ֵget the ARGB of specified coordinates
	DWORD GetAlphaValue(const int x,const int y);

	//�滭����
public:
	//�滭ͼ��
	bool DrawImage(CDC * pDC, INT nXPos, INT nYPos);
	//�滭ͼ��
	bool DrawImage(CDC * pDC, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT YSrc);
	//�滭ͼ��
	bool DrawImage(CDC * pDC, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXScr, INT YSrc, INT nSrcWidth, INT nSrcHeight);
};

//////////////////////////////////////////////////////////////////////////

#endif