#include "StdAfx.h"
#include ".\cglobaldata.h"

CCGlobalData::CCGlobalData(void)
{
}

CCGlobalData::~CCGlobalData(void)
{
}


CMD_GP_UserGradeInfo*   CCGlobalData::m_pGradeInfo = 0;                      //�ȼ���Ϣ
int                     CCGlobalData:: m_iGradeNum = 0;                        //�ȼ���Ŀ


//����ȼ��ڴ�
void CCGlobalData::AllocGradeInfoMemory(int num)
{
	m_pGradeInfo = new CMD_GP_UserGradeInfo[num];
	m_iGradeNum  = num;
}

//�ͷŵȼ���Ϣ�ڴ�
void CCGlobalData::UnAllocGradeInfoMemory()
{
	delete m_pGradeInfo;
	m_iGradeNum = 0;
}

//�õ��ȼ���Ϣ
//CMD_GP_UserGradeInfo*  CCGlobalData::GetGradeInfo(int index)
//{
//	if (index >= m_iGradeNum || index < 0)
//	{
//		return NULL;
//	}
//
//	return &m_pGradeInfo[index];
//}

//���õȼ�����
void CCGlobalData::SetGradeInfoData(int id, TCHAR* szName, int iMaxExp)
{
	if (id >= m_iGradeNum || id < 0)
	{
		return ;
	}

	m_pGradeInfo[id].iMaxExp = iMaxExp;
	memcpy(m_pGradeInfo[id].strName , szName, sizeof(m_pGradeInfo[id].strName));
	m_pGradeInfo[id].strName[sizeof(m_pGradeInfo[id].strName) - 1] = 0;
}

//���ݾ�������µĵȼ�
int  CCGlobalData::CaluGrade(LONG exp)
{
    for (int i=m_iGradeNum-1; i>=0; --i)
    {
		if (exp > m_pGradeInfo[i].iMaxExp)
		{
			return i + 1;
		}
    }
	return 1;
}


