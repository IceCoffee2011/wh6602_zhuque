#ifndef CONTROL_SERVICE_HEAD_FILE
#define CONTROL_SERVICE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DataBaseSink.h"
#include "AttemperEngineSink.h"

//////////////////////////////////////////////////////////////////////////

//����״̬
enum enServiceStatus
{
	ServiceStatus_Stop,				//ֹͣ״̬
	ServiceStatus_Service,			//����״̬
};

//////////////////////////////////////////////////////////////////////////

extern	CInitParamter					g_InitParamter;						//���ò���

//��¼������
class CControlService
{
	//���ñ���
public:
	enServiceStatus					m_ServiceStatus;					//����״̬

	//�������
protected:
	CDataBaseSink					m_DataBaseEngineSink;				//���ݹ���
	CAttemperEngineSink				m_AttemperEngineSink;				//���ȹ���

	//�ں����
protected:
	CTimerEngineHelper				m_TimerEngine;						//ʱ������
	CAttemperEngineHelper			m_AttemperEngine;					//��������
	CDataBaseEngineHelper			m_DataBaseEngine;					//��������
//	CTCPNetworkEngineHelper			m_TCPNetworkEngine;					//��������
	CTCPSocketServiceHelper			m_TCPSocketCorrespond;				//Э������

	//��������
public:
	//���캯��
	CControlService();
	//��������
	virtual ~CControlService();

	//���ܺ���
public:
	//��ʼ����
	bool StartService();
	//ֹͣ����
	bool StopService();
	//����״̬
	bool IsService() { return m_ServiceStatus==ServiceStatus_Service; }
	//�·�����
	bool SendCommand(VOID * pData, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////

#endif