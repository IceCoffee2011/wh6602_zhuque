#ifndef LOGON_SERVICE_HEAD_FILE
#define LOGON_SERVICE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DataBaseSink.h"
#include "AttemperEngineSink.h"

//////////////////////////////////////////////////////////////////////////

//服务状态
enum enServiceStatus
{
	ServiceStatus_Stop,				//停止状态
	ServiceStatus_Service,			//服务状态
};

//////////////////////////////////////////////////////////////////////////

extern	CInitParamter					g_InitParamter;						//配置参数

//登录服务类
class CLogonService
{
	//配置变量
public:
	enServiceStatus					m_ServiceStatus;					//运行状态

	//组件变量
protected:
	CDataBaseSink					m_DataBaseEngineSink;				//数据钩子
	CAttemperEngineSink				m_AttemperEngineSink;				//调度钩子

	//内核组件
protected:
	CTimerEngineHelper				m_TimerEngine;						//时间引擎
	CAttemperEngineHelper			m_AttemperEngine;					//调度引擎
	CDataBaseEngineHelper			m_DataBaseEngine;					//数据引擎
	CTCPNetworkEngineHelper			m_TCPNetworkEngine;					//网络引擎
	CTCPSocketServiceHelper			m_TCPSocketCorrespond;				//协调服务

	//函数定义
public:
	//构造函数
	CLogonService();
	//析构函数
	virtual ~CLogonService();

	//功能函数
public:
	//开始服务
	bool StartService();
	//停止服务
	bool StopService();
	//服务状态
	bool IsService() { return m_ServiceStatus==ServiceStatus_Service; }

	//
	LRESULT OnSoapMessage(WPARAM wParam, LPARAM lParam);
};

//////////////////////////////////////////////////////////////////////////

#endif