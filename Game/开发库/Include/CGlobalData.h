#pragma once
#include "GameServiceExport.h"
#include <set> 

class CCGlobalData
{
public:
	CCGlobalData(void);
	~CCGlobalData(void);
private:
	static CMD_GP_UserGradeInfo*           m_pGradeInfo;                      //�ȼ���Ϣ
	static int                             m_iGradeNum;                        //�ȼ���Ŀ
public:
	//����ȼ��ڴ�
	static void AllocGradeInfoMemory(int num);
	//�ͷŵȼ���Ϣ�ڴ�
	static void UnAllocGradeInfoMemory();
	//�õ��ȼ���Ϣ
	/*static CMD_GP_UserGradeInfo*  GetGradeInfo(int index);  */
	//���õȼ�����
	static void SetGradeInfoData(int id, TCHAR* szName, int iMaxExp);
	//���ݾ�������µĵȼ�
	static int  CaluGrade(LONG exp); 
};	



class  CCTask
{
	typedef std::set<DWORD>            TaskMarkSet;
	typedef std::set<DWORD>::iterator  TaskMarkSetIt;

	TaskMarkSet                               m_taskMarkSet;  //��־��ѯ����
public:
	void SetMark(DWORD taskMark);
	void Init();
	void Clear();
	bool FindMark(DWORD taskMark);
	//����֮ǰλ����ģʽ
	bool operator &(DWORD taskMark);
	bool operator |(DWORD taskMark);

};