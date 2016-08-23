#pragma once
#include "../inc/DllExport.h"
#include <string>

// CJob
// CJob����Լ򵥣����װ������Ļ��������Ժͷ�������������Ҫ����Run�������������£�
// 

class CThread;
class MYTHREADPOOL_DLL CJob
{
public:
	CJob(void);
	virtual ~CJob();

	int      GetJobNo(void) const { return m_JobNo; }
	void     SetJobNo(int jobno) { m_JobNo = jobno; }
	
	const std::string& GetJobName(void) const
	{ return m_JobName; }
	void SetJobName(const std::string &strName) 
	{ m_JobName = strName; }

	CThread* GetWorkThread(void) { return m_pWorkThread; }
	void     SetWorkThread(CThread *pWorkThread) {
		m_pWorkThread = pWorkThread;
	}
	
	virtual void Run(void *ptr) = 0;

private:
	int			m_JobNo;        // The num was assigned to the job
	std::string	m_JobName;      // The job name
	CThread		*m_pWorkThread; // The thread associated with the job
};