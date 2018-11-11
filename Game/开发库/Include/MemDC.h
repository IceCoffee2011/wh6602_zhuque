#ifndef _MEMDC_H_
#define _MEMDC_H_

class CMemDC : public CDC 
{
protected:
	CBitmap	m_bitmap;			// �ڴ��г�����ʱͼ���λͼ
	CBitmap*	m_oldBitmap;		// �ɵ�
	CDC*			m_pDC;			// ��ʵ��ͼ�� CDC
	CRect		m_rect;					// ��ͼ����
	bool		m_bMemDC;			// true if CDC really is a Memory DC.
	bool		m_alreadyDraw;		// �Ѿ��� draw ������ͼ�ˣ����������Ͳ��û��ˣ�Ϊ�� ReleaseDC
	bool		m_bBg;
    
   void Construct(CDC* pDC)
   {
        ASSERT(pDC != NULL); 

        m_pDC = pDC;
        m_oldBitmap = NULL;
        m_bMemDC = !pDC->IsPrinting();

        if (m_bMemDC) 
		{
            CreateCompatibleDC(pDC);	// ��������DC���������ڴ�DC
            pDC->LPtoDP(&m_rect);
			
            m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
            m_oldBitmap = SelectObject(&m_bitmap);
            
            SetMapMode(pDC->GetMapMode());
            
			//SetWindowExt(pDC->GetWindowExt());			// ��������ȫû�б�Ҫ
			//SetViewportExt(pDC->GetViewportExt());		//��������ȫû�б�Ҫ
			
            pDC->DPtoLP(&m_rect);
            SetWindowOrg(m_rect.left, m_rect.top);
        } 
		else 
		{
            m_bPrinting = pDC->m_bPrinting;
            m_hDC       = pDC->m_hDC;
            m_hAttribDC = pDC->m_hAttribDC;
		}
	
        if( m_bBg )
			BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
					 m_pDC, m_rect.left, m_rect.top, SRCCOPY);
		else
			FillSolidRect(m_rect, pDC->GetBkColor());
    }

public:

   /* --------------------------------------------------------------------------
   ����˵����
   ���������
		pRect	ָ���Ļ�ͼ����
		bBg		true ��ʾ��ԭͼ��䱳����false ��ʾ�õ�ɫ��䱳��
   ��ע��
		pDC ���Ǵ�CDC* pDC= GetDC �ж�������һ��Ҫִ�� draw ���ܻ�ͼ
		pDC ���Ǵ�ClientDC dc(this) �ж���������ִ�� draw �����������Զ���ͼ
   --------------------------------------------------------------------------*/
   CMemDC(CDC* pDC, const RECT* pRect=NULL, bool bBg = false) : CDC() 
   { 
	   m_alreadyDraw = false;
	   m_bBg = bBg;

	   if (pRect == NULL) 
		   pDC->GetClipBox(&m_rect);
		else 
		   m_rect = *pRect;

	   Construct(pDC); 
   }
    
   virtual ~CMemDC()
   {        
		if (!m_alreadyDraw)
		{
			if (m_bMemDC) 
			{
				m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
					this, m_rect.left, m_rect.top, SRCCOPY);

				SelectObject(m_oldBitmap);        
			} 
			else 
			{
				m_hDC = m_hAttribDC = NULL;
			} 
		}

		m_bitmap.DeleteObject();
		m_pDC = NULL;
		m_oldBitmap = NULL;
    }
    
   virtual draw()
   {        
	   if (m_bMemDC) 
	   {
		   m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
			   this, m_rect.left, m_rect.top, SRCCOPY);

		   SelectObject(m_oldBitmap);  
		   m_alreadyDraw = true;
	   } 
	   else 
	   {
		   m_hDC = m_hAttribDC = NULL;
	   }    
   }
};

#endif
