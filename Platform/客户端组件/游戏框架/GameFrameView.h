#ifndef GAME_FRAME_VIEW_HEAD_FILE
#define GAME_FRAME_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameFrame.h"
#include "ThreadDraw.h"
//#include "ImageNums.h"
//#include "CHeadPortrait.h"
#include <string>

class CTalkDlg;
using namespace std;

#define REMOVE_FLOWER 0 // 0Ϊ�Ƴ��ʻ���1Ϊ�����ʻ�
//////////////////////////////////////////////////////////////////////////

//�����ṹ
struct tagFlowerEffectInfo
{
	//�ƶ���Ϣ
	WORD							wMoveCount;							//�ƶ�����
	WORD							wMoveIndex;							//�ƶ�����
	CPoint							ptFrom;								//����λ��
	CPoint							ptDest;								//Ŀ��λ��
	CPoint							ptCurPos;							//��ǰλ��

	//������Ϣ
	WORD							wSourceChairID;						//��ʼλ��
	WORD							wTargetChairID;						//����λ��
	UINT							uFlowerID;							//�ʻ���ʶ
	UINT							uFlowerEffectID;					//������ʶ
	bool							bBombEffect;						//��ը��ʶ
};

//�滭ģʽ
enum enImageMode 
{
	enMode_Spread,					//ƽ��ģʽ
	enMode_Centent,					//����ģʽ
	enMode_ElongGate,				//����ģʽ
};

//��˵��
class CClientKernelSink;

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ͼ���
class GAME_FRAME_CLASS CGameFrameView : public CWnd, public IGameFrameView
{
	//��Ԫ����
	friend class CGameFrameDlg;
	friend class CClientKernelSink;

	//λ�ñ���
protected:
	UINT							m_nAnimeStep;						//�ƶ����
	POINT							m_ptName[MAX_CHAIR];				//����λ��
	POINT							m_ptFace[MAX_CHAIR];				//ͷ��λ��
	POINT							m_ptTimer[MAX_CHAIR];				//ʱ��λ��
	POINT							m_ptReady[MAX_CHAIR];				//׼��λ��
	POINT							m_ptSayPos[MAX_CHAIR];				//˵��λ��
	POINT							m_ptOnlineTime;						//���߼�ʱλ��

	//�����ָ��
	CTalkDlg						* m_pTalkDlg[10];					//�����ָ��

	//�û�����
private:
	tagUserData						* m_pUserItem[MAX_CHAIR];			//�û���Ϣ

	//���ָ��
private:
	void							* m_pReserve;						//����ָ��
	IUserFaceRes					* m_pIUserFaceRes;					//ͷ��ӿ�

	//��ͼ����
private:
	bool							m_bReDraw;							//�Ƿ��ػ�
	CSize							m_ImageSize;						//�����С
	CImage							m_ImageBuffer;						//����λͼ
	const bool						m_bDoubleBuf;						//�����ͼ
	const UINT						m_uColorCount;						//����ɫ��
	CThreadDraw						* const m_pThreadDraw;				//��ͼ��Ϣ
	CWnd							* m_pOnlineKeepTime;				//���߼�ʱ
	//CHeadPortraitManager			 m_HeadPortrait;

	//���Ƽ�ʱ��
private:
	CPngImage						m_ImageTimerBack;					//��ʱ������
	CPngImage						m_ImageTimerNumber;					//��ʱ������
	int								m_iCurTimeFrames[MAX_CHAIR];		//��ǰ֡
	WORD							m_wTimer[MAX_CHAIR];				//�û�ʱ��
	bool							m_bIsDrawTimer[MAX_CHAIR];			//����ʱ��
	POINT							m_ptTimerCurPT[MAX_CHAIR];			//ʱ�䵱ǰλ��
	bool							m_bIsStartTimer;					//�Ƿ�������ʱ��
	bool							m_bIsDirectorTag[MAX_CHAIR];		//�����ʶ

private:
	//���Ƶȴ�����
	bool							m_bIsPlayWaitFlash;					//�Ƿ񲥷ŵȴ�����
	CPngImage						m_ImageWaitFlash;					//�ȴ�����
	int								m_iCountWaitFrame;					//����ȴ�֡��
	CRect							m_rcWaitDrawFlash;					//��������

	//���ƶ���
private:
	CPngImage						m_ImageBreakLine;					//����

	//�����ʱ������
protected:
	bool							m_bIsDisableTimerFlash;				//�Ƿ񼤻��ʱ������
	WORD							m_wGameUserNums;					//��Ϸ�û���

	//Ӯȡ����
public:
	//Author<cxf>;DateTime<2012-03-06>;Description:<�滭Ӯȡ��Ϣ>
	CImageNums						m_ImageNumber;						//����
	CRect							m_rcWinNumberArea;					//Ӯȡ���ֻ滭����
	CPngImage						m_ImageWinLostBk;					//Ӯȡ��������
	int								m_nKindIDWinCount;					//Ӯ��
	int								m_nKindIDLostCount;					//����
	int								m_nKindIDDrawCount;					//ƽ��
	int								m_nKindIDFleeCount;					//������

	//ֻ������
public:
	static const int				m_nXFace;							//ͷ��߶�
	static const int				m_nYFace;							//ͷ����
	static const int				m_nXTimer;							//��ʱ��������
	static const int				m_nYTimer;							//��ʱ��������
	static const int				m_nXBorder;							//��ܿ��
	static const int				m_nYBorder;							//��ܸ߶�
	static const int				m_nXTimerNumber;					//��ʱ�����ֿ�
	static const int				m_nYTimerNumber;					//��ʱ�����ָ�

	//��չ����
private:

#if REMOVE_FLOWER	// del by HouGuoJiang 2011-11-25
	CFlowerEffectControl			m_FlowerEffectControl[MAX_CHAIR_NORMAL];//�ʻ��ؼ�
	tagFlowerEffectInfo				m_FlowerInfo[MAX_FLOWER_COUNT];		//�ʻ���Ϣ
	int								m_nFlowerCount;						//�ʻ���Ŀ
#endif
	//��������
public:
	//���캯��
	CGameFrameView(bool bDoubleBuf=true ,UINT uColorCount=16, CThreadDraw * pThreadDraw=NULL);
	//��������
	virtual ~CGameFrameView();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//�̳к���
private:
	//���ý���
	virtual void ResetGameView()=NULL;
	//�����ؼ�
	virtual void RectifyGameView(int nWidth, int nHeight)=NULL;
	//�滭����
	virtual void DrawGameView(CDC * pDC, int nWidth, int nHeight)=NULL;

	//���ܺ���
public:
	//��ȡʱ��
	WORD GetUserTimer(WORD wChairID);
	//��ȡ���
	const tagUserData * GetUserInfo(WORD wChairID);
	//���½���
	void UpdateGameView(const CRect * pRect);
	//��ȡ����
	bool GetGameViewImage(CImage & ImageBuffer, int nXPos, int nYPos, int nWidth, int nHeight);
	//����λ��
	void SetFlowerControlInfo(WORD wChairID,int nXPos, int nYPos, int nWidth=BIG_FACE_WIDTH, int nHeight=BIG_FACE_HEIGHT, UINT nAnimeStep=30);
	//�滭�ʻ�
	void ShowFlower(WORD wSourceChairID, WORD wTargetChairID, UINT uFlowerID, UINT uFlowerEffectID, bool bBombEffect=false);//?

	/// ����ͼ���ʼ��
	void InitHeadPortrait(CWnd *pParent, UINT uCount);
	/// �������ͼ��������� ��ע����Ŀ�� InitHeadPortrait ����������
	/// [@param pt in] ����λ�ü�
	void SetHeadPortraitPos(CPoint pt[]);
	/// ���Ŷ���
	/// [@param uID in] ���� ID���������ID��
	/// [@param uIndex in] ��������
	/// [@param uFrameTime in] ����ÿ֡��ʱ�䣬���Ĭ��֡��Ч��Ҳ��Ĭ��֡��ʱ�䣨���룩
	/// [@param uIntervalTime in] �������ʱ�� �����룩
	/// [@param uResultTime in] �����ܹ�ʱ�䣬С�ڵ��� 0 ��ʱ�����ƣ����룩
	/// [@param uDefaultAction in] Ĭ�϶������� uIndex ���������꣬�Զ�����Ĭ�϶�����
	void PlayHeadPortraitAction(WORD wID, UINT uIndex, UINT uFrameTime=150, UINT uIntervalTime=0, UINT uResultTime=0, UINT uDefaultAction=0);
	/// ��������ͷ��
	/// [@param uID in] ����ͷ��ID��Ϊ
	void HideHeadPortrait(UINT uID, BOOL bAllHide=FALSE);
	/// ��ʼ���߼�ʱ
	void OnlineKeepTime(int iTime, BOOL bStart=TRUE);
	/// �������߼�ʱ�ؼ�����
	void RectifyKeepTimePos(int x, int y);
	/// ���ƿ��
	void DrawFrameView(CDC * pDC, int nWidth, int nHeight);
	// ���ƾ���
	void ResetWinLost(tagUserData* pUserData);
	//�滭Ӯȡ����ͼƬ
	void DrawWinLostNumberBk(CDC * pDC);
	//���û滭Ӯȡ����λ��
	void SetWinLostDrawPoint(CPoint cpPoint);
	//��ͼ����
public:
	//�滭����
	void DrawViewImage(CDC * pDC, CSkinImage & SkinImage, enImageMode ImageMode);
	//�滭ͷ��
	void DrawUserFace(CDC * pDC,const tagUserData * pUserData, int nXPos, int nYPos, bool bOffLine);
	//�滭ͷ��
	void DrawCustomFace(CDC * pDC,BYTE byViewID, const tagUserData * pUserData, int nXPos, int nYPos);
	//�滭׼��
	void DrawUserReady(CDC * pDC, int nXPos, int nYPos);
	//���ƶ��߱�־
	void DrawBreakLineTag(CDC * pDC, int nXPos, int nYPos);
	//�滭ʱ��
	void DrawUserTimer(CDC * pDC , BYTE byViewID);
	//͸���滭
	void AlphaDrawImage(CDC * pDesDC, int nXDes, int nYDes, int nDesWidth, int nDesHeight, CDC * pScrDC, int nXScr, int nYScr, COLORREF crTransColor);
	//͸���滭
	void AlphaDrawImage(CDC * pDesDC, int nXDes, int nYDes, int nDesWidth, int nDesHeight, HBITMAP hBitBmp, int nXScr, int nYScr, COLORREF crTransColor);

	//�ڲ�����
private:
	//���ý���
	void ResetData();
	//���ýӿ�
	bool SetUserFaceRes(IUnknownEx * pIUnknownEx);
	//����ʱ��
	bool SetUserTimer(WORD wChairID, WORD wTimer);
	//�����û�
	bool SetUserInfo(WORD wChairID, tagUserData * pUserData);
	//������Ϸ����û���
	void SetGameUserNums(WORD dwNums);
	//������Ϸ�ȴ�����
	void PlayGameWaitFlash(bool bIsPlay=true);

	//��Ϣӳ��
protected:
	//�滭����
	afx_msg void OnPaint();
	//λ�ñ仯
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//������Ϣ
	afx_msg void OnDestroy();
	//ʱ����Ϣ
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

public:
	//��ʾ���Կ�
	void ShowUserSayInfo(BYTE btViewUserID, CString strTalkText);
	void HideUserSayInfo();
	void GetUserImageInfo(BYTE byViewID, int &iPosX, int &iPosY);
};

//////////////////////////////////////////////////////////////////////////

#endif