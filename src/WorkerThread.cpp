#include "../inc/WorkerThread.h"
#include "../inc/Job.h"
#include "../inc/ThreadPool.h"

CWorkerThread::CWorkerThread()
{
	m_Job = NULL;
	m_JobData = NULL;
	m_ThreadPool = NULL;
	m_IsEnd = false;
}

CWorkerThread::~CWorkerThread()
{
	if (NULL != m_Job)
		delete m_Job;
	if (m_ThreadPool != NULL)
		delete m_ThreadPool;
}

void CWorkerThread::Run()
{
	SetThreadState(THREAD_RUNNING);
	for (;;)
	{
		while (m_Job == NULL)
		{
		//	m_JobCond.Wait();
		}

		m_Job->Run(m_JobData);
		m_Job->SetWorkThread(NULL);
		m_Job = NULL;
		m_ThreadPool->MoveToIdleList(this);

		if (m_ThreadPool->m_IdleList.size() > m_ThreadPool->GetAvailHighNum())
		{
			m_ThreadPool->DeleteIdleThread(m_ThreadPool->m_IdleList.size() - m_ThreadPool->GetInitNum());
		}
		m_WorkMutex.unlock();
	}
}

void CWorkerThread::SetJob(CJob* job, void* jobdata)
{
	m_VarMutex.lock();
	m_Job = job;
	m_JobData = jobdata;
	job->SetWorkThread(this);
	m_VarMutex.unlock();
	//m_JobCond.Signal();
	m_JobCond.notify_all();
}

void CWorkerThread::SetThreadPool(CThreadPool* thrpool)
{
	m_VarMutex.lock();
	m_ThreadPool = thrpool;
	m_VarMutex.unlock();
}