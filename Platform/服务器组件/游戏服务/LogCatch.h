/********************************************************************
	created:	2012/03/27
	created:	27:3:2012   15:27
	filename: 	e:\ZhuQue\Platform\���������\��Ϸ����\LogCatch.h
	file path:	e:\ZhuQue\Platform\���������\��Ϸ����
	file base:	LogCatch
	file ext:	h
	author:		�����
	purpose:	�첽��־�����쳣�����Զ�λ���쳣�ĺ���
*********************************************************************/

#ifndef LOG_CATCH_H
#define LOG_CATCH_H

#include "cnd_ILogMgr.h"

#define __ENTER_FUNCTION_LOG_CATCH try{
#define __LEAVE_FUNCTION_LOG_CATCH }catch(...){		\
	if(m_pRoomLog)	\
{							\
	CString strErrorLine;		\
	strErrorLine.Format("��������%s  �����У�%d �ļ�����%s",  __FUNCTION__ ,  __LINE__  , 	__FILE__ );	\
	CString strPrompt;	\
	strPrompt.Format("%s ,����ԭ�� ���顣", strErrorLine.GetBuffer());	\
	CTraceService::TraceString(strPrompt,TraceLevel_Exception);	\
	m_pRoomLog->AddMsg(0, 0, TRUE, strPrompt.GetBuffer());		\
} }	



#endif