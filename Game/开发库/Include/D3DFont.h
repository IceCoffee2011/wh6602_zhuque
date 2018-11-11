#ifndef D3D_FOUNT_HEAD_FILE
#define D3D_FOUNT_HEAD_FILE

#include "D3DDevice.h"
#include "D3DTexture.h"
#include "D3DEngineHead.h"

//////////////////////////////////////////////////////////////////////////

//��������
#define MAX_GRAY_LEVEL				3									//�Ҷȵȼ�
#define MAX_BUFFER_COUNT			512									//������Ŀ

//////////////////////////////////////////////////////////////////////////

//��������
struct tagFontItem
{
	//�ַ���Ϣ
	UINT							uChar;								//�������
	CD3DTexture						D3DTexture;							//��������

	//���λ��
	CSize							SizeCell;							//��Ԫ��С
	CSize							SizeFont;							//�����С
	CPoint							PointOrigin;						//ԭ��λ��
};

//���Ͷ���
typedef CArrayTemplate<tagFontItem *>	CFontItemPtrArray;						//��������
typedef CMap<UINT,UINT,tagFontItem *,tagFontItem *>	CFontItemPtrMap;	//��������

//////////////////////////////////////////////////////////////////////////

//�������
class D3D_ENGINE_CLASS CD3DFont
{
	//���ñ���
protected:
	UINT							m_uGrayLevel;						//�Ҷȵȼ�
	UINT							m_uMaxBufferCount;					//������Ŀ

	//�������
protected:
	CFont							m_Font;								//�������
	TEXTMETRIC						m_TextMetric;						//��������

	//�ں˶���
protected:
	CFontItemPtrMap					m_FontItemPtrMap;					//��������
	CFontItemPtrArray				m_FontItemPtrActive;				//��������

	//��������
public:
	//���캯��
	CD3DFont();
	//��������
	virtual ~CD3DFont();

	//������Ŀ
public:
	//������Ŀ
	UINT GetMaxBufferCount() { return m_uMaxBufferCount; }
	//������Ŀ
	VOID SetMaxBufferCount(UINT uMaxBufferCount) { m_uMaxBufferCount=uMaxBufferCount; }

	//��������
public:
	//ɾ������
	bool DeleteFont();
	//��������
	bool CreateFont(LOGFONT & LogFont, UINT uGrayLevel);
	//��������
	bool CreateFont(INT nPointSize, LPCTSTR lpszFaceName, UINT uGrayLevel);

	//�������
public:
	//�������
	bool DrawText(CD3DDevice * pD3DDevice, LPCTSTR pszString, CRect rcDraw,UINT uFormat, D3DCOLOR D3DColor);
	//�������
	bool DrawText(CD3DDevice * pD3DDevice, LPCTSTR pszString, INT nXPos, INT nYPos, UINT uFormat, D3DCOLOR D3DColor);

	//�������
private:
	//��������
	tagFontItem * ActiveFontItem(CD3DDevice * pD3DDevice, UINT uChar);
	//����λ��
	CSize CalePostion(CD3DDevice * pD3DDevice, LPCTSTR pszString, UINT uFormat, INT nAreaWidth);
};

//////////////////////////////////////////////////////////////////////////

#endif