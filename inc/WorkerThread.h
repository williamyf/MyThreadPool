#include "../inc/Thread.h"

// CWorkerThread
// 
// CWorkerThread是CThread的派生类，是事实上的工作线程。在CThreadPool的构造函数中，我们创建了一定数量的CWorkerThread。
// 一旦这些线程创建完毕，我们将调用Start()启动该线程。Start方法最终会调用Run方法。Run方法是个无限循环的过程。
// 在没有接受到实际的任务的时候，m_Job为NULL，此时线程将调用Wait方法进行等待，从而处于挂起状态。
// 一旦线程池将具体的任务分发给该线程，其将被唤醒，从而通知线程从挂起的地方继续执行。CWorkerThread的完整定义如下:
// 
// 当线程执行任务之前首先必须判断空闲线程的数目是否低于m_AvailLow，如果低于，则必须创建足够的空闲线程，使其数目达到m_InitNum个，
// 然后将调用MoveToBusyList()移出空闲队列，移入忙碌队列。当任务执行完毕后，其又调用MoveToIdleList()移出忙碌队列，移入空闲队列，等待新的任务。
// 
// 除了Run方法之外，CWorkerThread中另外一个重要的方法就是SetJob，该方法将实际的任务赋值给线程。当没有任何执行任务即m_Job为NULL的时候，
// 线程将调用m_JobCond.Wait进行等待。一旦Job被赋值给线程，其将调用m_JobCond.Signal方法唤醒该线程。由于m_JobCond属于线程内部的变量，
// 每个线程都维持一个m_JobCond，只有得到任务的线程才被唤醒，没有得到任务的将继续等待。无论一个线程何时被唤醒，
// 其都将从等待的地方继续执行m_Job->Run(m_JobData)，这是线程执行实际任务的地方。
// 
// 在线程执行给定Job期间，我们必须防止另外一个Job又赋给该线程，因此在赋值之前，通过m_VarMutex进行锁定， Job执行期间，其于的Job将不能关联到该线程；
// 任务执行完毕，我们调用m_VarMutex.Unlock()进行解锁，此时，线程又可以接受新的执行任务。
// 
// 在线程执行任务结束后返回空闲队列前，我们还需要判断当前空闲队列中的线程是否高于m_AvailHigh个。如果超过m_AvailHigh，
// 则必须从其中删除(m_ThreadPool->m_IdleList.size() - m_ThreadPool->GetInitNum())个线程，使线程数目保持在m_InitNum个。
// 
class CJob;
class CThreadPool;
class CThreadMutex;
class CCondition;
class CWorkerThread :public CThread
{
private:
	CThreadPool*  m_ThreadPool;
	CJob*    m_Job;
	void*    m_JobData;
	CThreadMutex m_VarMutex;
	bool      m_IsEnd;
protected:
public:
	CCondition   m_JobCond;
	CThreadMutex m_WorkMutex;
	CWorkerThread();
	virtual ~CWorkerThread();
	void Run();
	void    SetJob(CJob* job, void* jobdata);
	CJob*   GetJob(void) { return m_Job; }
	void    SetThreadPool(CThreadPool* thrpool);
	CThreadPool* GetThreadPool(void) { return m_ThreadPool; }
};