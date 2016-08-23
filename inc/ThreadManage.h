#pragma once
#include "../inc/DllExport.h"
// CThreadManage
// CThreadManage�Ĺ��ܷǳ��򵥣����ṩ��򵥵ķ��������ඨ�����£�
// 

class CJob;
class CThreadPool;
class MYTHREADPOOL_DLL CThreadManage
{
public:
	void     SetParallelNum(int num);
	CThreadManage();
	CThreadManage(int num);
	virtual ~CThreadManage();
	void    Run(CJob* job, void* jobdata);
	void    TerminateAll(void);
private:
	CThreadPool*	m_Pool;			// ָ��ʵ�ʵ��̳߳�
	int				m_NumOfThread;	// ��ʼ����ʱ���������Ĳ������̸߳���
};