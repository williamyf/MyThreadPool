#include <string>
// CThread
// 
// CThread ��ʵ���˶�Linux���̲߳����ķ�װ�����������̵߳Ļ��࣬Ҳ��һ�������࣬�ṩ��һ������ӿ�Run��
// ���е�CThread������ʵ�ָ�Run������CThread�Ķ���������ʾ��
//
// �̵߳�״̬���Է�Ϊ���֣����С�æµ��������ֹ(���������˳��ͷ������˳�)��
// ����ĿǰLinux�߳̿ⲻ֧�ֹ����������ˣ����ǵĴ˴��Ĺ��������������ͣ��
// ����̴߳�����������ִ��������ô���ǿ��Խ��䡰��ͣ���������Ҫ���У����ѡ�
// ��һ�����ע����ǣ�һ���߳̿�ʼִ�����񣬽����ܱ������佫һֱִ����������ϡ�
// 
// �߳������ز�����ʮ�ּ򵥡��̵߳�ִ������Ǵ�Start()������ʼ���佫���ú���ThreadFunction��
// ThreadFunction�ٵ���ʵ�ʵ�Run������ִ��ʵ�ʵ�����
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