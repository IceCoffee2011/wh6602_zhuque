#ifndef		___H_DEF_GAME_DOWN_LOAD_SERVICE_INTERFACE_H_DEF___
#define		___H_DEF_GAME_DOWN_LOAD_SERVICE_INTERFACE_H_DEF___

#include "DownLoad.h"
//��˵��
class CDownLoadMission;
class CDownLoadService;
typedef CArrayTemplate<CDownLoadMission *> CDownLoadMissionArray;

//����״̬
enum enDownLoadStatus
{
	enDownLoadStatus_Unknow,			//δ֪״̬
	enDownLoadStatus_Ready,				//׼��״̬
	enDownLoadStatus_DownLoadIng,		//����״̬
	enDownLoadStatus_Finish,			//���״̬
	enDownLoadStatus_Fails,				//����ʧ��
};


//����ص��ӿ�
interface IDownLoadMissionSink
{
	//����֪ͨ
	virtual void OnMissionFinish(enDownLoadStatus DownLoadStatus, CDownLoadMission * pDownLoadMission)=NULL;
};

//////////////////////////////////////////////////////////////////////////

//���ط���
class IDOWN_LOAD_CLASS CDownLoadService : public IDownLoadMissionSink
{
	//��������
protected:
	DWORD								m_dwMissionID;					//�����ʶ
	IDownLoadServiceSink				* m_pIDownLoadServiceSink;		//�ص��ӿ�
	CDownLoadMissionArray				m_DownLoadMissionActive;		//��������
	CDownLoadMissionArray				m_DownLoadMissionRelease;		//��������

	//��������
public:
	//���캯��
	CDownLoadService();
	//��������
	virtual ~CDownLoadService();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���ýӿ�
public:
	//��ȡ��Ŀ
	virtual INT_PTR __cdecl GetDownLoadMissionCount();
	//���ýӿ�
	virtual bool __cdecl SetDownLoadServiceSink(IUnknownEx * pIUnknownEx);
	//��������
	virtual DWORD __cdecl AddDownLoadRequest(DWORD dwDownLoadType, tagDownLoadRequest * pDownLoadRequest);

	//�¼��ӿ�
public:
	//����֪ͨ
	virtual void OnMissionFinish(enDownLoadStatus DownLoadStatus, CDownLoadMission * pDownLoadMission);

	//�ڲ�����
private:
	//��������
	CDownLoadMission * SearchMission(tagDownLoadRequest * pDownLoadRequest);
};

//////////////////////////////////////////////////////////////////////////








#endif