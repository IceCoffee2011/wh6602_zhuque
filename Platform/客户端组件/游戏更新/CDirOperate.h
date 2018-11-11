//////////////////////////////////////////////////////////
// CDirOperate.h�ļ�

#ifndef __CDIROPERATE_H_
#define __CDIROPERATE_H_
#pragma once

#include <afxwin.h>

class CDirOperate
{
public:
	CDirOperate();
	virtual ~CDirOperate();

	//�������ַ�����lpszDir��ָ����Ŀ¼
	static void CreateAllDirectory(LPCTSTR lpszDir);
	//ɾ����lpszDir������Ŀ¼
	static void DeleteAllDirectory(LPCTSTR lpszDir);

protected:
	//�ж�һ��Ŀ¼�Ƿ����
	static BOOL IsDirExist(LPCTSTR lpszDir);
	//�ݹ�ѭ��ɾ��ָ��Ŀ¼�µ������ļ�������Ŀ¼
	static void RecursiveDelete(LPCTSTR lpszPath); 
};

#endif