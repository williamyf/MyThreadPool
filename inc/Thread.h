#include <string>
// CThread
// 
// CThread 类实现了对Linux中线程操作的封装，它是所有线程的基类，也是一个抽象类，提供了一个抽象接口Run，
// 所有的CThread都必须实现该Run方法。CThread的定义如下所示：
//
// 线程的状态可以分为四种，空闲、忙碌、挂起、终止(包括正常退出和非正常退出)。
// 由于目前Linux线程库不支持挂起操作，因此，我们的此处的挂起操作类似于暂停。
// 如果线程创建后不想立即执行任务，那么我们可以将其“暂停”，如果需要运行，则唤醒。
// 有一点必须注意的是，一旦线程开始执行任务，将不能被挂起，其将一直执行任务至完毕。
// 
// 线程类的相关操作均十分简单。线程的执行入口是从Start()函数开始，其将调用函数ThreadFunction，
// ThreadFunction再调用实际的Run函数，执行实际的任务。
//
enum ThreadState
{
	THREAD_RUNNING,

};
struct Impl;
class CThread
{
public:
	CThread();
	CThread(bool createsuspended, bool detach);
	virtual ~CThread();
	virtual void Run(void) = 0;
	void     SetThreadState(ThreadState state) { m_ThreadState = state; }
	bool     Terminate(void);    //Terminate the threa
	bool     Start(void);        //Start to execute the thread
	void     Exit(void);
	bool     Wakeup(void);
	ThreadState  GetThreadState(void) { return m_ThreadState; }
	int      GetLastError(void) { return m_ErrCode; }

	void     SetThreadName(const std::string &thrname) { m_ThreadName = thrname; }
	const std::string& GetThreadName(void) { return m_ThreadName; }

	int      GetThreadID(void) { return m_ThreadID; }
	bool     SetPriority(int priority);
	int      GetPriority(void);
	int      GetConcurrency(void);
	void     SetConcurrency(int num);
	bool     Detach(void);
	bool     Join(void);
	bool     Yield(void);
	int      Self(void);
protected:
	void     SetErrcode(int errcode) { m_ErrCode = errcode; }
	static void* ThreadFunction(void*);
private:
	int          m_ErrCode;
	//Semaphore    m_ThreadSemaphore;		//the inner semaphore, which is used to realize
	unsigned     long m_ThreadID;
	bool         m_Detach;				//The thread is detached
	bool         m_CreateSuspended;		//if suspend after creating
	std::string  m_ThreadName;
	ThreadState  m_ThreadState;			//the state of the thread
	Impl		*m_pImpl;
};