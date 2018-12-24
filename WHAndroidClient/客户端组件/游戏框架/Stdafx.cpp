// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// GameFrame.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

#define USE_HOU 0	// 

/*
���ַ�ʽ�ıȽ�
SIRT_TRANS_DOT_MOREģʽ  SIRT_SOLID_DOT_MOREģʽ
iCount=8                   iCount=258 
iCount=0                   iCount=17 
iCount=0                   iCount=26 
iCount=48                  iCount=154 
iCount=14                  iCount=325
iCount=0                   iCount=20 
iCount=710                 iCount=1430
*/

BOOL SetImageRgn_TransDotLess(HWND hWnd, CPngImage * pImage);
BOOL SetImageRgn_SolidDotLess(HWND hWnd, CPngImage * pImage);
BOOL SetImageRgn_FullDotLess(HWND hWnd, CPngImage* pImage);
BOOL SetImageRgn_DeburringDotLess(HWND hWnd, CPngImage* pImage);//Author:<cxf>; CreateDate:<2011-12-27>; Description:<����޼�͸���Ľ�>

BOOL SetImageRgn(HWND hWnd, CPngImage * pImage, int iType)
{
	switch(iType)
	{
	case SIRT_TRANS_DOT_LESS:
		{
			return SetImageRgn_TransDotLess(hWnd, pImage);
		}
	case SIRT_SOLID_DOT_LESS:
		{
			return SetImageRgn_SolidDotLess(hWnd, pImage);
		}
	case SIRT_FULL_DOT_LESS:
		{
			return SetImageRgn_FullDotLess(hWnd, pImage);
		}
	case SIRT_DEBURRING_DOT_LESS://Author:<cxf>; CreateDate:<2011-12-27>; Description:<����޼�͸���Ľ�>
		{
			return SetImageRgn_DeburringDotLess(hWnd, pImage);
		}
	default:
		break;
	}
	
	return false;
}

// add by HouGuoJiang 2011-11-25
// ���һ��ͼ������ģ�͸�����ĵ�Ƚ���
BOOL SetImageRgn_TransDotLess(HWND hWnd, CPngImage * pImage)
{
//	if (!pImage) 
//		return FALSE;
//	if (pImage->IsNull()) 
//		return FALSE;
//
//	CBitmap* pBmp;
//	CImage imgBuf;
//
//	int iWidth = pImage->GetWidth();
//	int iHeight = pImage->GetHeight();
//	int iCount = 0;
//
//	imgBuf.Create(iWidth, iHeight, 32);
//	CDC *pDCTmp = CDC::FromHandle(imgBuf.GetDC());	// ȱ�ٳɶԳ��ֵ� CDC::DeleteTempMap  ???
//	pImage->DrawImage(pDCTmp, 0, 0, iWidth, iHeight, 0, 0, iWidth, iHeight);
//	pBmp = ((CBitmap *)(&imgBuf));
//	DWORD * pData = new DWORD[iWidth * iHeight];
//	pBmp->GetBitmapBits((sizeof (DWORD)* iWidth * iHeight),  pData);
//
//	CRgn rgnImage;			// pImage �� ����
//	CRgn rgnDecrease;		// Ҫ����������
//
//	rgnImage.CreateRectRgn(0,  0,  iWidth,  iHeight);
//	rgnDecrease.CreateRectRgn(0,  0,  iWidth,  iHeight);
//
//	DWORD *pDataTmp = pData;
//	
//	bool		bStart = true;	// ���������
//	CPoint	ptStartTrans;		// ͸�������
//	CPoint	ptEndTrans;			// ͸�����յ�
//	int		iPix;		
//	int		iPhalanx = 0;		// ����ĵ�
//	
//	for (int h=0; h<iHeight; ++h)	// �߶�
//	{
//		iPhalanx = iWidth * h;
//		bStart = true;				// ���������
//		ptStartTrans.SetPoint(0, h);
//		ptEndTrans.SetPoint(0, h);
//		for(int w=0;  w<iWidth;  w++, pDataTmp++)
//		{
//			iPix = (*pDataTmp) & 0xff000000;
//			if(0 == iPix)	// ��ȫ͸��
//			{
//				if(bStart)
//				{
//					ptStartTrans.SetPoint(w, h);
//					bStart = false;	// ���������
//
//					// ���һ�����͸�� ,ֱ�Ӳü�
//					if(w==iWidth)
//					{
//						// �ü���͸����ȷ�͸�����٣����Բü�͸���ȵ��ӷ�͸�������Ч�� ...... 
//						rgnDecrease.SetRectRgn(ptStartTrans.x, ptStartTrans.y,  ptEndTrans.x,  ptEndTrans.y+1);	// Ҫ�ü�������
//						rgnImage.CombineRgn(&rgnImage, &rgnDecrease, RGN_XOR);	// �������������ȫ����ȥ��������		
//						iCount++;
//					}
//				}
//			}
//			else	// ����ȫ͸��
//			{
//				ptEndTrans.SetPoint(w-1, h);
//				
//				// ��ȫ���벢��
//				if(ptStartTrans.x <= ptEndTrans.x)
//				{
//					iPix = pData[iPhalanx + ptEndTrans.x ] & 0xff000000;
//					if (0==iPix)	// �õ�͸��
//					{
//						// �ü���͸����ȷ�͸�����٣����Բü�͸���ȵ��ӷ�͸�������Ч�� ...... 
//						rgnDecrease.SetRectRgn(ptStartTrans.x, ptStartTrans.y,  ptEndTrans.x,  ptEndTrans.y+1);	// Ҫ�ü�������
//						rgnImage.CombineRgn(&rgnImage, &rgnDecrease, RGN_XOR);	// 	�������������ȫ����ȥ��������	
//						iCount++;
//					}
//				}
//				bStart = true;
//			}
//		}
//	}
//
//#if _DEBUG
//	CString strTmp;
//	strTmp.Format("iCount=%d \r\n", iCount);
//	TRACE(strTmp.GetBuffer());
//#endif
//
//	SafeDeleteArray(pData);
//	imgBuf.ReleaseDC();
//	imgBuf.Destroy();
//
//	BOOL bResult = TRUE;
//
//	if (rgnImage.m_hObject)
//	{
//		SetWindowRgn(hWnd, (HRGN)rgnImage.m_hObject, TRUE);
//		DeleteObject(rgnImage);
//	}
//	else
//	{
//		bResult = FALSE;
//	}
//
//	DeleteObject(rgnDecrease);
//	
//	CDC::DeleteTempMap();	// add by HouGuoJiang 2011-11-25
//
//	return bResult;

	if (!pImage) 
		return FALSE;
	if (pImage->IsNull()) 
		return FALSE;

	CBitmap* pBmp;
	CImage imgBuf;

	int iWidth = pImage->GetWidth();
	int iHeight = pImage->GetHeight();
	int iCount = 0;

	imgBuf.Create(iWidth, iHeight, 32);
	CDC *pDCTmp = CDC::FromHandle(imgBuf.GetDC());	// ȱ�ٳɶԳ��ֵ� CDC::DeleteTempMap  ???
	pImage->DrawImage(pDCTmp, 0, 0, iWidth, iHeight, 0, 0, iWidth, iHeight);
	pBmp = ((CBitmap *)(&imgBuf));
	DWORD * pData = new DWORD[iWidth * iHeight];
	pBmp->GetBitmapBits((sizeof (DWORD)* iWidth * iHeight),  pData);

	CRgn rgnImage;			// pImage �� ����
	CRgn rgnDecrease;		// Ҫ����������

	rgnImage.CreateRectRgn(0,  0,  iWidth,  iHeight);
	rgnDecrease.CreateRectRgn(0,  0,  iWidth,  iHeight);

	DWORD *pDataTmp = pData;

	bool		bStart = true;	// ���������
	CPoint	ptStartTrans;		// ͸�������
	CPoint	ptEndTrans;			// ͸�����յ�
	int		iPix;		
	int		iPhalanx = 0;		// ����ĵ�

	for (int h=0; h<iHeight; ++h)	// �߶�
	{
		iPhalanx = iWidth * h;
		bStart = true;				// ���������
		ptStartTrans.SetPoint(0, h);
		ptEndTrans.SetPoint(0, h);
		for(int w=0;  w<iWidth;  w++, pDataTmp++)
		{
			iPix = (*pDataTmp) & 0xff000000;
			if(0 == iPix)	// ��ȫ͸��
			{
				if(bStart)
				{
					ptStartTrans.SetPoint(w, h);
					bStart = false;	// ���������
				}
				// ���һ�����͸�� ,ֱ�Ӳü�
				if(w==(iWidth-1))
				{
					// �ü���͸����ȷ�͸�����٣����Բü�͸���ȵ��ӷ�͸�������Ч�� ...... 
					rgnDecrease.SetRectRgn(ptEndTrans.x+2, ptEndTrans.y,  iWidth,  ptStartTrans.y+1);	// Ҫ�ü�������
					rgnImage.CombineRgn(&rgnImage, &rgnDecrease, RGN_XOR);	// �������������ȫ����ȥ��������		
					iCount++;
				}
			}
			else	// ����ȫ͸��
			{
				ptEndTrans.SetPoint(w-1, h);

				// ��ȫ���벢��
				if(ptStartTrans.x <= ptEndTrans.x)
				{
					iPix = pData[iPhalanx + ptEndTrans.x ] & 0xff000000;
					if (0==iPix)	// �õ�͸��
					{
						// �ü���͸����ȷ�͸�����٣����Բü�͸���ȵ��ӷ�͸�������Ч�� ...... 
						rgnDecrease.SetRectRgn(ptStartTrans.x, ptStartTrans.y,  ptEndTrans.x,  ptEndTrans.y+1);	// Ҫ�ü�������
						rgnImage.CombineRgn(&rgnImage, &rgnDecrease, RGN_XOR);	// 	�������������ȫ����ȥ��������	
						iCount++;
					}
				}
				bStart = true;
			}
		}
	}

#if _DEBUG
	CString strTmp;
	strTmp.Format("iCount=%d \r\n", iCount);
	TRACE(strTmp.GetBuffer());
#endif

	SafeDeleteArray(pData);
	imgBuf.ReleaseDC();
	imgBuf.Destroy();

	BOOL bResult = TRUE;

	if (rgnImage.m_hObject)
	{
		SetWindowRgn(hWnd, (HRGN)rgnImage.m_hObject, TRUE);
		DeleteObject(rgnImage);
	}
	else
	{
		bResult = FALSE;
	}

	DeleteObject(rgnDecrease);

	CDC::DeleteTempMap();	// add by HouGuoJiang 2011-11-25

	return bResult;
}

// ���һ��ͼ����ʵ�ģ���͸�����ĵ�Ƚ���
BOOL SetImageRgn_SolidDotLess(HWND hWnd, CPngImage * pImage)
{
	if (NULL==pImage) 
		return FALSE;

	if (pImage->IsNull()) 
		return FALSE;

	int iCount = 0;
	INT wWidth = pImage->GetWidth();
	INT wHeight = pImage->GetHeight();
	CBitmap * pbmp;
	CImage imgBuf;
	imgBuf.Create(wWidth, wHeight, 32);

	CDC *pDCTmp = CDC::FromHandle(imgBuf.GetDC());	// һ��Ҫ�ɶԳ��� CDC::DeleteTempMap 
	pImage->DrawImage(pDCTmp, 0, 0, wWidth, wHeight, 0, 0, wWidth, wHeight);
	pbmp = ((CBitmap *)(&imgBuf));
	DWORD * pData = new DWORD[wWidth * wHeight];
	pbmp->GetBitmapBits((sizeof (DWORD)* wWidth * wHeight), pData);

	CRgn rgnImage, tempRgn;
	rgnImage.CreateRectRgn(0, 0, 0, 0);
	tempRgn.CreateRectRgn(0,  0,  wWidth,  wHeight);	// ������
	DWORD *pDataTmp = pData;
	for (UINT h = 0;h < wHeight;++h)	// �߶�
	{
		UINT w = 0;
		UINT leftX = 0;		// ��͸����������͸�������ص������
		UINT rightX = 0;	// ��͸����������͸�������ص����Ҷ�
		while(w < wWidth)	// ���
		{
			// 0xABGR ��ʾΪ 0xFFFFFFFF ��λ��AΪ 00  ��ʾȫ͸����FF��ʾ��͸��
			// ����ӹ�͸�����أ�һֱ�ߵ���͸����������͸����������
			while (w++ < wWidth && (0 == (*pDataTmp++ & 0xff000000)));	

			leftX =(w - 1);	// ��һ����͸����������͸����������
			if(leftX >= wWidth-1)
				continue;	

			// ����ӹ���͸����������͸���������أ�һֱ�ߵ�͸��������
			while (w++ < wWidth &&(0 != (*pDataTmp++ & 0xff000000))); 

			rightX = w-1;
			if(rightX > wWidth-1)	// Լ�����
				rightX = wWidth - 1;
			
			tempRgn.SetRectRgn(leftX, h,  rightX, h+1);	// modi by HouGuoJiang 2011-11-25 ��
														// û��Ҫ��һ���������淴���� CreateRectRgn �� DeleteObject �ü����������������Ĺ���

			rgnImage.CombineRgn(&rgnImage, &tempRgn, RGN_OR);
			iCount++;
		}
	}


	BOOL bResult = TRUE;
	CString strTmp;
	strTmp.Format("iCount=%d \r\n", iCount);
	TRACE(strTmp.GetBuffer());
	
	SafeDeleteArray(pData);
	imgBuf.ReleaseDC();
	imgBuf.Destroy();
	
	if (rgnImage.m_hObject)
	{
		SetWindowRgn(hWnd, (HRGN)rgnImage.m_hObject, TRUE);
		DeleteObject(rgnImage);
	}
	else
	{
		bResult = FALSE;
	}

	DeleteObject(tempRgn);
	CDC::DeleteTempMap();	// add by HouGuoJiang 2011-11-25

	return bResult;
}

BOOL SetImageRgn_FullDotLess(HWND hWnd, CPngImage* pImage)
{
	if (NULL==pImage) return FALSE;
	if (pImage->IsNull()) return FALSE;
	INT wWidth = pImage->GetWidth();
	INT wHeight = pImage->GetHeight();
	CBitmap * pbmp;
	CImage imgBuf;
	imgBuf.Create(wWidth, wHeight, 32);
	CDC *pDCTmp = CDC::FromHandle(imgBuf.GetDC());
	pImage->DrawImage(pDCTmp, 0, 0, wWidth, wHeight, 0, 0, wWidth, wHeight);
	pbmp = ((CBitmap *)(&imgBuf));
	DWORD * pData = new DWORD[wWidth * wHeight];
	pbmp->GetBitmapBits((sizeof (DWORD)* wWidth * wHeight), pData);
	imgBuf.ReleaseDC();
	imgBuf.Destroy();

	CRgn rgnImage, tempRgn;
	rgnImage.CreateRectRgn(0, 0, 0, 0);
	tempRgn.CreateRectRgn(0, 0, wWidth, wHeight);
	DWORD *pDataTmp = pData;
	for (UINT h = 0;h < wHeight;++h)
	{
		UINT w = 0, leftX = 0;
		while(w < wWidth)
		{
			while (w++ < wWidth &&(0 == (*pDataTmp++ & 0xff000000)));
			leftX =(w - 1);
			while (w++ < wWidth &&(0 != (*pDataTmp++ & 0xff000000))); 
			tempRgn.SetRectRgn(leftX, h,(w - 1),(h + 1));
			rgnImage.CombineRgn(&rgnImage, &tempRgn, RGN_OR);
		}
	}

	DeleteObject(tempRgn);

	SafeDeleteArray(pData);

	if (NULL == rgnImage.m_hObject)
		return FALSE;

	SetWindowRgn(hWnd, (HRGN)rgnImage.m_hObject, TRUE);
	DeleteObject(rgnImage);
	CDC::DeleteTempMap();

	return TRUE;
}

//Author:<cxf>
//CreateDate:<2011-12-27>
//Description:<����޼�͸���Ľ�>
BOOL SetImageRgn_DeburringDotLess(HWND hWnd, CPngImage* pImage)
{
	if (NULL==pImage) return FALSE;
	if (pImage->IsNull()) return FALSE;
	INT wWidth = pImage->GetWidth();
	INT wHeight = pImage->GetHeight();
	CBitmap * pbmp;
	CImage imgBuf;
	imgBuf.Create(wWidth, wHeight, 32);
	CDC *pDCTmp = CDC::FromHandle(imgBuf.GetDC());
	pImage->DrawImage(pDCTmp, 0, 0, wWidth, wHeight, 0, 0, wWidth, wHeight);
	pbmp = ((CBitmap *)(&imgBuf));
	DWORD * pData = new DWORD[wWidth * wHeight];
	pbmp->GetBitmapBits((sizeof (DWORD)* wWidth * wHeight), pData);
	imgBuf.ReleaseDC();
	imgBuf.Destroy();

	CRgn rgnImage, tempRgn;
	rgnImage.CreateRectRgn(0, 0, 0, 0);
	tempRgn.CreateRectRgn(0, 0, wWidth, wHeight);
	DWORD *pDataTmp = pData;
	WORD wAreaW=30,wAreaH=30;
	if (wAreaW > wWidth || wAreaH > wHeight)return false;

	//͸����
	for (UINT i=0; i<wHeight; ++i)
	{
		UINT j=0,k=wWidth;
		while (j++ < wAreaW && (0 == (pData[(i*wWidth)+j] & 0xff000000)));

		while (k-- > wWidth-wAreaW && (0 == (pData[(i*wWidth)+k] & 0xff000000)));

		tempRgn.SetRectRgn(j, i, k, (i + 1));
		rgnImage.CombineRgn(&rgnImage, &tempRgn, RGN_OR);
	}

	DeleteObject(tempRgn);

	SafeDeleteArray(pData);

	if (NULL == rgnImage.m_hObject) return FALSE;

	SetWindowRgn(hWnd, (HRGN)rgnImage.m_hObject, TRUE);
	DeleteObject(rgnImage);
	CDC::DeleteTempMap();

	return TRUE;
}