#ifndef D3D_SOUND_HEAD_FILE
#define D3D_SOUND_HEAD_FILE

#include "D3DEngineHead.h"

//////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagWaveDataInfo
{
	DWORD							dwWaveSize;							//���ݴ�С
	LPVOID							pcbWaveData;						//����ָ��
	WAVEFORMATEX					WaveFormatEx;						//������ʽ
};

//////////////////////////////////////////////////////////////////////////

//��������
class D3D_ENGINE_CLASS CD3DSound
{
	//�ӿڱ���
public:
	IDirectSound8 *					m_pIDirectSound;					//�豸����

	//��̬����
public:
	static CD3DSound *				m_pD3DSound;						//����ָ��

	//��������
public:
	//���캯��
	CD3DSound();
	//��������
	virtual ~CD3DSound();

	//���ܺ���
public:
	//��������
	bool CreateD3DSound(HWND hWnd);

	//��������
public:
	//��������
	bool LoadSoundResource(LPCTSTR pszFileName);
	//��������
	bool LoadSoundResource(HINSTANCE hInstance, LPCTSTR pszResource, LPCTSTR pszTypeName);

	//��������
private:
	//��������
	bool GetWaveDataInfo(LPVOID pDataBuffer, tagWaveDataInfo & WaveDataInfo);
	//��仺��
	bool FillSoundBuffer(IDirectSoundBuffer * pDSBuffer, VOID * pSoundData, DWORD dwSoundSize);

	//��������
private:
	//��������
	bool CreateSoundBuffer(IDirectSoundBuffer * * pDSBuffer, tagWaveDataInfo & WaveDataInfo, DWORD dwFlags);
	//��������
	bool CreateSoundBuffer(IDirectSoundBuffer * * pDSBuffer, INT nChannels, INT nSecs, DWORD nSamplesPerSec, WORD wBitsPerSample, DWORD dwFlags);

	//��̬����
public:
	//��ȡ����
	static CD3DSound * GetInstance() { return m_pD3DSound; }
};

//////////////////////////////////////////////////////////////////////////

#endif