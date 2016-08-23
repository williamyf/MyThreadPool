#pragma once
#include "../inc/DllExport.h"
#include <vector>
#include <boost/thread.hpp>

// CThreadPool
// 
// CThreadPool是线程的承载容器，一般可以将其实现为堆栈、单向队列或者双向队列。在我们的系统中我们使用STL Vector对线程进行保存。
// 在CThreadPool中存在两个链表，一个是空闲链表，一个是忙碌链表。
// Idle链表中存放所有的空闲进程，当线程执行任务时候，其状态变为忙碌状态，同时从空闲链表中删除，并移至忙碌链表中。
// 
// 在CThreadPool的构造函数中，我们将创建m_InitNum个线程，创建之后即调用AppendToIdleList放入Idle链表中，由于目前没有任务分发给这些线程，因此线程执行Start后将自己挂起。
// 事实上，线程池中容纳的线程数目并不是一成不变的，其会根据执行负载进行自动伸缩。为此在CThreadPool中设定四个变量：
// 
// m_InitNum：	初始创建时线程池中的线程的个数。
// m_MaxNum :	当前线程池中所允许并发存在的线程的最大数目。
// m_AvailLow:	当前线程池中所允许存在的空闲线程的最小数目，如果空闲数目低于该值，表明负载可能过重，此时有必要增加空闲线程池的数目。实现中我们总是将线程调整为m_InitNum个。
// m_AvailHigh：当前线程池中所允许的空闲的线程的最大数目，如果空闲数目高于该值，表明当前负载可能较轻，此时将删除多余的空闲线程，删除后调整数也为m_InitNum个。
// m_AvailNum： 目前线程池中实际存在的线程的个数，其值介于m_AvailHigh和m_AvailLow之间。如果线程的个数始终维持在m_AvailLow和m_AvailHigh之间，
//				则线程既不需要创建，也不需要删除，保持平衡状态。因此如何设定m_AvailLow和m_AvailHigh的值，使得线程池最大可能的保持平衡态，是线程池设计必须考虑的问题。
// 
// 线程池在接受到新的任务之后，线程池首先要检查是否有足够的空闲池可用。检查分为三个步骤：
// 
// (1)	检查当前处于忙碌状态的线程是否达到了设定的最大值m_MaxNum，如果达到了，表明目前没有空闲线程可用，而且也不能创建新的线程，
//		因此必须等待直到有线程执行完毕返回到空闲队列中。
// (2)	如果当前的空闲线程数目小于我们设定的最小的空闲数目m_AvailLow，则我们必须创建新的线程，默认情况下，创建后的线程数目应该为
//		m_InitNum，因此创建的线程数目应该为(当前空闲线程数与m_InitNum);
//		但是有一种特殊情况必须考虑，就是现有的线程总数加上创建后的线程数可能超过m_MaxNum，因此我们必须对线程的创建区别对待。
//		如果创建后总数不超过m_MaxNum，则创建后的线程为m_InitNum；如果超过了，则只创建( m_MaxNum-当前线程总数)个。
// 
// (3)	调用GetIdleThread方法查找空闲线程。如果当前没有空闲线程，则挂起；否则将任务指派给该线程，同时将其移入忙碌队列。
//		当线程执行完毕后，其会调用MoveToIdleList方法移入空闲链表中，其中还调用m_IdleCond.Signal()方法，唤醒GetIdleThread()中可能阻塞的线程。
// 
class CJob;
typedef boost::mutex CThreadMutex;
typedef boost::condition_variable_any CCondition;
class MYTHREADPOOL_DLL CThreadPool
{
	friend class CWorkerThread;
private:
	unsigned int m_MaxNum;   //the max thread num that can create at the same time
	unsigned int m_AvailLow; //The min num of idle thread that shoule kept
	unsigned int m_AvailHigh;    //The max num of idle thread that kept at the same time
	unsigned int m_AvailNum; //the normal thread num of idle num;
	unsigned int m_InitNum;  //Normal thread num;
protected:
	CWorkerThread* GetIdleThread(void);
	void    AppendToIdleList(CWorkerThread* jobthread);
	void    MoveToBusyList(CWorkerThread* idlethread);
	void    MoveToIdleList(CWorkerThread* busythread);
	void    DeleteIdleThread(int num);
	void    CreateIdleThread(int num);
public:
	CThreadMutex m_BusyMutex;    //when visit busy list,use m_BusyMutex to lock and unlock
	CThreadMutex m_IdleMutex;    //when visit idle list,use m_IdleMutex to lock and unlock
	CThreadMutex m_JobMutex; //when visit job list,use m_JobMutex to lock and unlock
	CThreadMutex m_VarMutex;
	CCondition       m_BusyCond; //m_BusyCond is used to sync busy thread list
	CCondition       m_IdleCond; //m_IdleCond is used to sync idle thread list
	CCondition       m_IdleJobCond;  //m_JobCond is used to sync job list
	CCondition       m_MaxNumCond;
	std::vector<CWorkerThread*>   m_ThreadList;
	std::vector<CWorkerThread*>   m_BusyList;     //Thread List
	std::vector<CWorkerThread*>   m_IdleList; //Idle List
	CThreadPool();
	CThreadPool(int initnum);
	virtual ~CThreadPool();
	void    SetMaxNum(int maxnum) { m_MaxNum = maxnum; }
	int     GetMaxNum(void) { return m_MaxNum; }
	void    SetAvailLowNum(int minnum) { m_AvailLow = minnum; }
	int     GetAvailLowNum(void) { return m_AvailLow; }
	void    SetAvailHighNum(int highnum) { m_AvailHigh = highnum; }
	int     GetAvailHighNum(void) { return m_AvailHigh; }
	int     GetActualAvailNum(void) { return m_AvailNum; }
	int     GetAllNum(void) { return m_ThreadList.size(); }
	int     GetBusyNum(void) { return m_BusyList.size(); }
	void    SetInitNum(int initnum) { m_InitNum = initnum; }
	int     GetInitNum(void) { return m_InitNum; }
	void    TerminateAll(void);
	void    Run(CJob* job, void* jobdata);
};