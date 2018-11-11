#ifndef DX_GAME_VIEW_HEAD_FILE
#define DX_GAME_VIEW_HEAD_FILE
#pragma once

#include "Stdafx.h"
#include "DxSkin.h"
#include "DxDialog.h"
#include "Fish.h"
#include "cannon.h"
#include "Bullet.h"
#include "Coin.h"
#include "SkinButtonEx.h"
#include "ScoreView.h"
#include "Seascene.h"
#include "AudioPlayer.h"
#include "Bonus.h"

/////////////////////////////////////////////////////////////
//#define WM_SHOOT				WM_USER+100								//�����ڵ�

////////////////////////////////////////////////////////////

#define TEXTURE_INDEX_BULLET_COUNT		0								//��������
#define TEXTURE_INDEX_COIN_COUNT		1								//��������
#define TEXTURE_INDEX_NONE				2								//��������

//�켣��Ϣ
struct tagFishTraceInfo
{
	int							nInitX[5];
	int							nInitY[5];
	int							nInitCount;
	BYTE						byMoveSpace;
	tagFishInfo					*pFishInfo;
};

//λ������
enum enDrawPos
{
	enDrawPos_MeGoldBack,
	enDrawPos_MeGold,
	enDrawPos_AccountBack,
	enDrawPos_Account,
	enDrawPos_BulletBack,
	enDrawPos_Bullet,
	enDrawPos_CoinBack,
	enDrawPos_Coin
};

//������
class CGameClientDlg;

//d3d������
class CGameClientViewDX : public CDxDialog
{
	//�û���Ϣ
private:
	LONG							m_lBulletCount[GAME_PLAYER][enBulletCountKind_End];//�ӵ���Ŀ
	LONGLONG						m_lCoinCount[GAME_PLAYER];			//�����Ŀ
	WORD							m_wCptrFishCount[enFishKind_No];	//��Ⱥ��Ŀ
	LONG							m_lBulletCharge[enBulletCountKind_End];	//�ӵ�����
	bool							m_bLookonMode;						//�Թ۱�ʶ

	//�ǳ�����
private:
	int								m_nSceneMovePosX;					//�ƶ�λ��
	enSeaSceneKind					m_NewSeaScene;						//�µĳ���
	bool							m_bWaveLToR;						//�ı䷽��
	bool							m_bWaveEffect;						//�ǳ���ʶ
	bool							m_bWaveComing;						//�ǳ���ʶ

	//�̱߳���
public:
	HANDLE							m_hEventFinish;						//�¼����
	HANDLE 							m_hCalcThread;						//�߳̾��
	bool							m_bExist;							//�˳���ʶ
	bool							m_bResetArray;						//���ñ�ʶ
	bool							m_bLoadResFinish;					//���ر�ʶ
	bool							m_bDeviceReset;						//���ñ�ʶ

	//��Ϸ����
protected:
	bool							m_bReleaseKey;						//�ͷż���
	bool							m_bWaitScoreInfo;					//�ȴ�����
	bool							m_bInitControlTag;					//��ʼ��־
	DWORD							m_dwDelayTimes;						//��ʱʱ��
	DWORD							m_dwCurTimes;						//��ǰʱ��
	WORD							m_wMeChairID;						//�Լ���λ
	DWORD							m_dwLastShootTime;					//���ʱ��
	WORD							m_wIdleTime;						//����ʱ��
	enSeaSceneKind					m_CurSeaScene;						//��ǰ����
	bool							m_bNoBulletWarn;					//û�о���
	DWORD							m_dwNoBulletWarnTime;				//����ʱ��
	LONG							m_lCellScore[GAME_PLAYER];			//��Ԫ����
	DWORD							m_dwLastCalcTime;					//����ʱ��
	DWORD							m_dwLastSwitchTime;					//�л�ʱ��

	//�ʽ����
protected:
	LONG							m_lBonusCount;						//�ʽ���Ŀ
	CBonus							m_Bonus;							//�ʽ�Ч��
	
	//�켣����
public:
	CArrayTemplate<tagFishTraceInfo>m_FishArrayBuffer;					//������Ⱥ
	CArrayTemplate<tagFishInfo*>	m_FishArrayActive;					//������Ⱥ
	
	//��Դ����
protected:	
	CCannon							m_Cannon;							//���ڱ���
	CBullet							m_Bullet;							//�ڵ�����
	CCoin							m_Coin;								//��ұ���

	//��Դ����
protected:
	LPD3DXFONT						m_pFontUserInfo;					//D X ����
	LPD3DXFONT						m_pFontGameInfo;					//D X ����

	//�������
protected:
	CDxSkin							m_ToolBarBk;						//����ͼƬ
	CDxSkin							m_UserInfo;							//�����Ϣ
	CDxSkin							m_Number;							//����ͼƬ
	CDxSkin							m_Wave;								//�ǳ�����
	CDxSkin							m_NoBulletWarn;						//�ǳ�����
	CDxSkin							m_WaveEffect;						//����Ч��
	CDxSkin							m_GameTip;							//��Ϸ��ʾ
	CSeaScene						m_SeaScene[enSeaSceneKind_No];		//���׳���

	//�������
private:
	CGameClientDlg					* m_pGameClientDlg;					//��������

	//��ť�ؼ�
private:
	CSkinButtonEx					m_btShowBulletShop;					//�����ӵ�
	CSkinButtonEx					m_btQuitGame;						//�˳���Ϸ
	CSkinButtonEx					m_btGameHelp;						//��Ϸ����

	//��������
protected:
	CAudioPlayer						m_AudioPlayerBG;				//��������
	CAudioPlayer						m_AudioPlayerSyn;				//ͬ������
	CAudioPlayer						m_AudioPlayerAsyn[MAX_SYN_SOUND];//�첽����

	//��������
public:
	//���캯��
	CGameClientViewDX();
	//��������
	virtual ~CGameClientViewDX();

	//�̳к���
public:
	//��������
	VOID PlayGameSound(WORD wSoundType);

	//�̳к���
public:
	//��������
	//virtual VOID UpdateControl();
	//��Ⱦ����
	virtual VOID DrawGameView2D();
	//����λ��
	virtual VOID InitScene(INT nWidth,INT nHeight);
	//�ͷ���Դ
	virtual VOID ReleaseScene();
	//�ͷ���Դ
	virtual VOID OnDeviceLost();
	//���´���
	bool UpdateDxWindowSize();

	//���ܺ���
public:
	//����ͼƬ
	VOID LoadDXImage();
	//���س���
	void LoadSeaScene(enSeaSceneKind SeaSceneKind);
	//������Ⱥ
	void LoadFish();
	//�ͷų���
	void ReleaseSeaScene(enSeaSceneKind SeaSceneKind);
	//����ͼƬ
	VOID OnResetDevice();
	//���ű�־
	VOID EnablePlaySound(bool bEnable);
	//��������
	VOID DrawTextString(LPD3DXFONT pFont, LPCWSTR pszString, D3DCOLOR crText, int nXPos, int nYPos);
	//��Ϸ��Ϣ
	void DrawGameInfo();

	//���ýӿ�
public:
	//������λ
	void SetMeChairID(WORD wMeChairID);
	//���ӹ켣
	void AddTrace(int nInitX[], int nInitY[], int nInitCount, enFishKind FishKind, DWORD dwFishID, bool bRegular);
	//���ô���
	void SetGameClientDlg(CGameClientDlg *pGameClientDlg) { m_pGameClientDlg=pGameClientDlg; }
	//�����ӵ�
	void SetUserBulletCount(WORD wChairID, enBulletCountKind BulletCountKind, LONG lCount);
	//���ý��
	void SetUserCoinCount(WORD wChairID, int nCount);
	//��Ⱥ��Ŀ
	void SetCptrFishCount(enFishKind FishKind, WORD wCount) {m_wCptrFishCount[FishKind]=wCount;}
	//�ӵ���Ŀ
	void SetShootBulletCount(WORD wChairID, int nBulletCount) {m_Cannon.SetBulletCount(wChairID,nBulletCount,false);}
	//����״̬
	bool SetFishStatus(enFishKind FishKind, DWORD dwFishID, enFishStatus FishStatus);
	//�ӵ�����
	void SetBulletCharge(LONG lBulletCharge[enBulletCountKind_End]) {CopyMemory(m_lBulletCharge,lBulletCharge,sizeof(m_lBulletCharge));}
	//�Թ�ģʽ
	void SetLokonMode(bool bLookonMode);
	//�л�����
	void SetChangeScene(bool bWaveLToR, bool bChange, enSeaSceneKind SeaSceneKind);
	//��Ԫ����
	void SetCellScore(WORD wChairID, LONG lCellScore) {m_lCellScore[wChairID]=lCellScore;}
	//������Ϣ
	void SetWaitScoreInfo(bool bWaiting) {m_bWaitScoreInfo=bWaiting;}
	//���òʽ�
	void SetBonusInfo(LONG lBonusCount) {m_lBonusCount=lBonusCount;}

	//�����ӿ�
public:
	//��ȡ���
	CAudioPlayer *GetaudioPlayer(LPCTSTR pszAudioName);
	//��������
	bool PlayMp3Sound(LPCTSTR pszSoundName, bool bSyn);
	//��������
	bool PlayMp3Sound(LPCTSTR pszPath, LPCTSTR pszSoundName, bool bSyn);
	//��������
	bool PlayMp3SoundBG(LPCTSTR pszSoundName, bool bLoop=true);
	//��������
	bool PlayMp3SoundBG(LPCTSTR pszPath, LPCTSTR pszSoundName, bool  bLoop=true);
	//ֹͣ����
	bool StopMp3SoundBG();
	//��ͣ����
	bool PauseMp3SoundBG();

	//��Ϣ�ӿ�
public:
	//��ȡ�Ƕ�
	float GetMeShootAngle();
	//�ӵ�����
	enBulletCountKind GetCurUseBulletKind(WORD wChairID);
	//��ҷ���
	void UserShoot(WORD wChairID, float fAngle, enBulletCountKind BulletCountKind);
	//���ӽ��
	void UserAddCoin(WORD wChairID, int nSourcePosX, int nSourcePosY, LONG lCoinCount);
	//���Ӳʽ�
	void AddBonus(WORD wChairID, LONG lBonusCount);
	//��Ⱥ����
	enFishKind GetFishKind(DWORD dwFish);
	//�˳���ѯ
	bool QuitGameInquire();
	//��ʾ�ɼ�
	bool ShowScoreView();
	//�����ؼ�
	VOID RectifyGameView(INT nWidth, INT nHeight);
	//�ӵ���Ŀ
	LONG GetBulletCount(WORD wChairID, enBulletCountKind BulletCountKind);
	//����λ��
	WORD GetMeChairID() {return m_wMeChairID;}
	//���òʽ�
	LONG GetBonusCount() { return m_lBonusCount; }
	
	//���غ��� 
public:
	//������Ϣ
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//��������
	virtual bool InitDxWindow();
	//ֹͣ��Ⱦ
	virtual void StopRender() {m_bDeviceReset=true;}
	//��ʼ��Ⱦ
	virtual void StartRender() {m_bDeviceReset=false;}

	//��Ϣӳ��
protected:
	//������
	afx_msg VOID OnLButtonDown(UINT nFlags, CPoint CPoint);
	//������
	afx_msg VOID OnLButtonUp(UINT nFlags, CPoint point);
	//����ƶ�
	afx_msg VOID OnMouseMove(UINT nFlags, CPoint point);
	//��Ⱥ��ʧ
	afx_msg LRESULT OnFishDisappear(WPARAM wParam, LPARAM lParam);
	//�켣��Ϣ
	afx_msg LRESULT OnFinishCalcTrace(WPARAM wParam, LPARAM lParam);
	//��ʱ��Ϣ
	afx_msg void OnTimer(UINT nIDEvent);
	//�Ҽ���Ϣ
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//��С��Ϣ
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//����
	LRESULT OnNetOpen(WPARAM wParam, LPARAM lParam);

	//�ڲ�����
private:
	//����λ��
	void InitPosition();
	//������Ϣ
	void DrawUserInfo();
	//�滭����
	void DrawNumber(int nXPos, int nYPos, LONGLONG lNumber, int nTextIndex);
	//��ȡλ��
	void GetDrawPos(WORD wChairID,enDrawPos DrawPos, int &nPosX, int &nPosY, int nIndex=0);
	//������
	bool _UserShoot(WORD wChairID, bool bReleaseKey=true);
	//��ǰ��Ŀ
	LONG GetCurBulletCount(WORD wChairID);
	//�л��ڵ�
	void SwitchCannon();	
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//add by Ivan_han
	BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};

//////////////////////////////////////////////////////////////////////////
#endif
